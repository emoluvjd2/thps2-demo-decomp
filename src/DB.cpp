// Double-buffered display setup and GPU frame submission.

#define DB_BUFFER_LAYOUT
#define DB_OWNER_DECLARATIONS
#include "DB.h"
#undef DB_OWNER_DECLARATIONS
#undef DB_BUFFER_LAYOUT

/*
 * DB-owned data and the foreign state used by this owner remain local to the
 * translation unit.  Their declaration spelling and encounter point preserve
 * the target's absolute-versus-GP-relative addressing decisions.
 */
#define DB_DECLARE_OWNER_ADDRESS_VIEWS
#include "DB.h"
#define POLYBUFFERSIZE_OWNER_DECLARATION
#include "POLYBUFFERSIZE.h"
#define DOUBLEBUFFER_DECLARE_KNOWN_BOUND
#include "DOUBLEBUFFER.h"
#undef DOUBLEBUFFER_DECLARE_KNOWN_BOUND
#define PDOUBLEBUFFER_OWNER_DECLARATIONS
#include "PDOUBLEBUFFER.h"
#undef PDOUBLEBUFFER_OWNER_DECLARATIONS
#define PDOUBLEBUFFER_DECLARE_OWNER_ADDRESS_VIEWS
#include "PDOUBLEBUFFER.h"

#include "GLEVEL.h"
#include "BMR.h"
#include "M3DINIT.h"
#include "STATS.h"
#include "UTILS.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBGPU.H>

enum {
    DbBufferCount = 2,
    DbScreenWidth = 512,
    DbScreenHeight = 240,
    DbFrameBufferHeight = 256,
    DbFullVramHeight = 511,
    DbBasePolyBufferSize = 97280,
    DbCachedAddressMask = 0x7FFFFFFF
};

/** The two GPU display buffers, initialized before the first frame is built. */
SDoubleBuffer DoubleBuffer[DbBufferCount]
    __attribute__((section(".data"))) = {{0}};

/**
 * The ordering-table word follows Db_Init's display-area constant and the two
 * reserved buffer-pointer words in this owner's small-data contribution.
 */
/** Legacy global ordering-table pointer owned by the display-buffer module. */
unsigned int *OrderingTable;

#include <LIBAPI.H>
#include "MAIN.h"
#include "M3DASM_FLAT.h"
#include "MEM.h"

/** Convert the CPU polygon-buffer pointer to the address consumed by DMA. */
static inline unsigned int *Db_GetDmaPolyAddress(SDoubleBuffer *buffer)
{
    return reinterpret_cast<unsigned int *>(
        reinterpret_cast<unsigned int>(buffer->Polys) & DbCachedAddressMask);
}

/**
 * Return the active ordering-table depth.
 *
 * @return Number of entries in each display ordering table.
 */
int Db_OTSize(void)
{
    return CurrentOTSize;
}

/**
 * Apply the configured sky-clear color to both drawing environments.
 */
void Db_UpdateSky(void)
{
    unsigned int skyColor = Db_SkyColor;
    if (skyColor == static_cast<unsigned int>(-1))
    {
        SDoubleBuffer *buffers = DoubleBuffer;
        buffers[0].Draw.isbg = 0;
        buffers[1].Draw.isbg = 0;
    }
    else
    {
        // These cursors keep one array base live across the component stores;
        // direct indexing changes fifteen of this function's 23 instructions.
        SDoubleBuffer *buffers = DoubleBuffer;
        buffers[0].Draw.isbg = 1;
        buffers[0].Draw.r0 = skyColor >> 16;
        buffers[0].Draw.g0 = skyColor >> 8;
        buffers[1].Draw.isbg = 1;
        unsigned char b = static_cast<unsigned char>(Db_SkyColor);
        SDoubleBuffer *secondBuffer = buffers + 1;
        secondBuffer->Draw.r0 = skyColor >> 16;
        secondBuffer->Draw.g0 = skyColor >> 8;
        buffers->Draw.b0 = b;
        secondBuffer->Draw.b0 = b;
    }
}

/**
 * Complete any finished GPU submissions and mark their buffers ready to flip.
 */
void Db_CallBack(void)
{
    SDoubleBuffer *buffers = DoubleBuffer;

    if (buffers[0].Rendering == 0 && buffers[1].Rendering == 0)
    {
        return;
    }

    // A non-blocking synchronization check leaves busy submissions untouched.
    if (DrawSync(1) != 0)
    {
        return;
    }

    int i = 0;
    // Hoisting the ready value preserves its allocation across the loop;
    // spelling the literal at the store changes three instructions.
    int ready = 1;
    int *readyFlag = &buffers->FlipNow;
    int *renderingFlag = &buffers->Rendering;
    for (i = 0; i < DbBufferCount; i++)
    {
        if (*renderingFlag != 0)
        {
            *renderingFlag = 0;
            *readyFlag = ready;
        }
        readyFlag += sizeof(SDoubleBuffer) / sizeof(*readyFlag);
        renderingFlag += sizeof(SDoubleBuffer) / sizeof(*renderingFlag);
    }
}

/**
 * Flip ready buffers and begin eligible GPU submissions during vertical blank.
 */
void Db_Vblank(void)
{
    {
        int i;
        for (i = 0; i < DbBufferCount; i++)
        {
            if (DoubleBuffer[i].FlipNow)
            {
                int *lastVblanks = &DoubleBuffer[i].LastVblanks;
                unsigned int currentVblanks;
                // Vblanks and these flags are interrupt-shared. Preserve both
                // counter samples and publish the post-call state in this order.
                // Folding either local changes 5-101 instructions because it
                // shortens the buffer-address lifetimes across PutDispEnv.
                currentVblanks = Vblanks;
                PutDispEnv(&DoubleBuffer[1 - i].Disp);
                currentVblanks = Vblanks;
                DoubleBuffer[i].FlipNow = 0;
                DoubleBuffer[i].Visible = 1;
                DoubleBuffer[1 - i].Visible = 0;
                *lastVblanks = currentVblanks;
            }
        }
    }

    {
        int i;
        for (i = 0; i < DbBufferCount; i++)
        {
            if (DoubleBuffer[i].RenderNow &&
                !DoubleBuffer[i].Rendering &&
                !DoubleBuffer[1 - i].Rendering &&
                !DoubleBuffer[i].Visible)
            {
                DoubleBuffer[i].RenderNow = 0;
                DoubleBuffer[i].Rendering = 1;
                PutDrawEnv(&DoubleBuffer[i].Draw);
                DrawOTag(reinterpret_cast<unsigned long *>(
                    DoubleBuffer[i].PanelOT + Db_OTSize() - 1));
            }
        }
    }
}

/**
 * Wait for the active buffer, clear its ordering table, and reset polygon output.
 */
void Db_WaitRender(void)
{
    // This initial cursor must occupy $v0 so the callback wait loop retains
    // the target load schedule; an ordinary local changes ten instructions.
    register SDoubleBuffer *buffer asm("$2") = pDoubleBuffer;
    int renderNow = buffer->RenderNow;
    pVB = buffer;

    if (renderNow != 0)
    {
        do
        {
            Pause(1);
            buffer = pVB;
        } while (buffer->RenderNow != 0);
    }

    buffer = pDoubleBuffer;
    if (buffer->Rendering != 0)
    {
        DrawSync(0);
        Db_CallBack();
    }

    int otsize = Db_OTSize();
    ClearOTagR(reinterpret_cast<unsigned long *>(pDoubleBuffer->PanelOT),
              otsize);

    pPoly = Db_GetDmaPolyAddress(pDoubleBuffer);
    CalcPolyBufferEnd();
}

/**
 * Finalize the active ordering table and exchange the build and display buffers.
 */
void Db_Render(void)
{
    if (pOtherBuffer->RenderNow != 0)
    {
        Pause(1);
    }

    SetDrawArea(&pDoubleBuffer->Clip, &pDoubleBuffer->Window);

    {
        SDoubleBuffer *pDB = pDoubleBuffer;
        addPrim(&pDB->PanelOT[1], &pDB->Clip);
    }

    CompactOTagR(pDoubleBuffer->PanelOT, Db_OTSize());

    {
        // The old compiler must keep this array base in $v1 across both
        // comparisons. Ordinary/reordered locals changed 3-12 instructions,
        // so only the base retains a legacy allocation annotation.
        register SDoubleBuffer *buffers asm("$3") = DoubleBuffer;
        SDoubleBuffer *nextBuffer = buffers;
        SDoubleBuffer *activeBuffer = pDoubleBuffer;
        activeBuffer->RenderNow = 1;
        if (activeBuffer == buffers)
        {
            nextBuffer = buffers + 1;
        }
        pDoubleBuffer = nextBuffer;
        if (nextBuffer == buffers)
        {
            buffers = buffers + 1;
        }
        pOtherBuffer = buffers;
    }
}

/**
 * Register the display-buffer completion callback with the GPU library.
 */
void Db_InitAtStart(void)
{
    DrawSyncCallback(Db_CallBack);
}

/**
 * Forget both allocated polygon-buffer addresses.
 */
void Db_Reset(void)
{
    DoubleBuffer[0].Polys = 0;
    DoubleBuffer[1].Polys = 0;
}

/**
 * Allocate and initialize both display buffers and their GPU environments.
 */
void Db_Init(void)
{
    EnterCriticalSection();
    SetDispMask(0);
    DrawSync(0);

    Db_SkyColor = 0;
    CurrentOTSize = (GNumberOfPlayers << 12) + 2;
    POLYBUFFERSIZE =
        Levels[GLevel].ExtraPolyBuffer[GNumberOfPlayers - 1] +
        DbBasePolyBufferSize;

    if (DoubleBuffer[0].Polys == 0)
    {
        DoubleBuffer[0].Polys =
            static_cast<unsigned char *>(Mem_New(POLYBUFFERSIZE * 2, 1, 1));
        DoubleBuffer[1].Polys = DoubleBuffer[0].Polys + POLYBUFFERSIZE;

        DoubleBuffer[0].PanelOT = static_cast<unsigned int *>(Mem_New(
            (Db_OTSize() << 2) | 2, 1, 1));
        DoubleBuffer[0].OrderingTable = DoubleBuffer[0].PanelOT + 2;

        DoubleBuffer[1].PanelOT = static_cast<unsigned int *>(Mem_New(
            (Db_OTSize() << 2) | 2, 1, 1));
        DoubleBuffer[1].OrderingTable = DoubleBuffer[1].PanelOT + 2;
    }

    DoubleBuffer[0].Top = 0;
    DoubleBuffer[1].Top = DbFrameBufferHeight;
    SetDefDrawEnv(&DoubleBuffer[0].Draw, 0, 0,
        DbScreenWidth, DbScreenHeight);
    SetDefDrawEnv(&DoubleBuffer[1].Draw, 0, DbFrameBufferHeight,
        DbScreenWidth, DbScreenHeight);
    SetDefDispEnv(&DoubleBuffer[0].Disp, 0, DbFrameBufferHeight,
        DbScreenWidth, DbScreenHeight);
    SetDefDispEnv(&DoubleBuffer[1].Disp, 0, 0,
        DbScreenWidth, DbScreenHeight);

    DoubleBuffer[0].Disp.screen.x = DoubleBuffer[1].Disp.screen.x = 0;
    DoubleBuffer[0].Disp.screen.y = DoubleBuffer[1].Disp.screen.y = 0;

    Db_UpdateSky();

    PutDrawEnv(&DoubleBuffer[0].Draw);
    PutDispEnv(&DoubleBuffer[0].Disp);

    {
        int i;
        for (i = 0; i < DbBufferCount; i++)
        {
            DoubleBuffer[i].Visible = 0;
            DoubleBuffer[i].Rendering = 0;
            DoubleBuffer[i].FlipNow = 0;
            DoubleBuffer[i].RenderNow = 0;
            DoubleBuffer[i].Window.x = 0;
            DoubleBuffer[i].Window.y = DoubleBuffer[i].Top;
            DoubleBuffer[i].Window.w = DbScreenWidth;
            DoubleBuffer[i].Window.h = DbScreenHeight;
            ClearOTagR(reinterpret_cast<unsigned long *>(
                           DoubleBuffer[i].PanelOT),
                       Db_OTSize());
        }
    }

    pDoubleBuffer = &DoubleBuffer[1];
    pOtherBuffer = &DoubleBuffer[0];
    RECT screen = {0, 0, DbScreenWidth, DbFullVramHeight};
    ClearImage2(&screen, 0, 0, 0);
    DrawSync(0);

    screen.y = DbFullVramHeight;
    screen.h = 1;
    ClearImage2(&screen, 0, 0, 0);
    DrawSync(0);

    SetDispMask(1);
    M3dInit_SetResolution(DbScreenWidth, DbScreenHeight);
    ExitCriticalSection();
}

/**
 * Reserve the two small-data words occupied by the active and alternate
 * buffer pointers. Defining them here would place their labels after their
 * first use, making ASPSX choose absolute loads instead of the target's
 * GP-relative form.
 */
static unsigned int DbSharedBufferPointers[2]
    __attribute__((section(".sdata"))) = {0, 0};

/**
 * Exchange display buffers without submitting a frame, then clear the new OT.
 */
void Db_FlipClear(void)
{
    pOtherBuffer = pDoubleBuffer;
    pDoubleBuffer = pDoubleBuffer == DoubleBuffer
        ? DoubleBuffer + 1 : DoubleBuffer;
    int size = Db_OTSize();
    ClearOTagR(reinterpret_cast<unsigned long *>(pDoubleBuffer->PanelOT),
              size);
    pPoly = Db_GetDmaPolyAddress(pDoubleBuffer);
    CalcPolyBufferEnd();
}

/**
 * Submit the current display frame and optionally upload its background image.
 */
void Db_DisplayDraw(void)
{
    ResetGraph(1);

    PutDispEnv(&pDoubleBuffer->Disp);
    PutDrawEnv(&pDoubleBuffer->Draw);

    if (Db_Background != 0)
    {
        if (pBMR != 0)
        {
            RECT r;
            r.x = 0;
            r.y = pDoubleBuffer->Top;
            r.w = DbScreenWidth;
            r.h = DbScreenHeight;
            LoadImage(&r, reinterpret_cast<unsigned long *>(pBMR));
            DrawSync(0);
            goto after_bg;
        }
    }
    Db_Background = 0;
after_bg:
    CompactOTagR(pDoubleBuffer->PanelOT, Db_OTSize());

    // The post-compaction sample must remain in $v1; an ordinary or direct
    // condition commutes two instructions in the final submission path.
    register int bg2 asm("$3") = Db_Background;
    if (bg2 != 0)
    {
        DrawSync(0);
    }

    int otsize2 = Db_OTSize();
    DrawOTag(reinterpret_cast<unsigned long *>(
        pDoubleBuffer->PanelOT + otsize2 - 1));
}
