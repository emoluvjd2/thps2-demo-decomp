// INIT.cpp -- Reconstructed translation unit
// Original: INIT.CPP
// Functions: 8 reconstructed entries
// Address range: 0x8006AAE0 - 0x8006AF38
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#include "BIT.h"
#include "MESS.h"
#include "BMR.h"
#define INIT_OWNER_DECLARATIONS
#include "INIT.h"
#undef INIT_OWNER_DECLARATIONS
#include "MAIN.h"
#include "BADDY.h"
#include "SAVEGAME.h"

// Owner-local layouts and fixed-address views used by this translation unit.
typedef unsigned char InitU8;
typedef unsigned short InitU16;

struct CBody;

/** CItem vtable prefix through the deleting-destructor slot used by INIT. */
struct InitBodyVTable
{
    void *opaqueSlot0;
    void *opaqueSlot1;
    void (*Destroy)(CBody *body, int deleteMode);
};

/** CItem slice needed while walking and destroying the global body lists. */
struct InitCItemLayout
{
    InitU8 opaqueBeforeNext[0x1c];
    CBody *mNextItem;
    InitU8 opaqueBeforeVTable[0x28];
    InitBodyVTable *_vf;
};

/** CBody prefix through the flags inspected by INIT's list cleanup. */
struct CBody
{
    InitCItemLayout CItem;
    CBody **mppOriginalList;
    InitU16 mInputFlags;
    InitU16 mCBodyFlags;
};

#include "SVIEWPORT.h"

typedef char InitCItemLayout_size_must_be_0x4c[
    (sizeof(InitCItemLayout) == 0x4c) ? 1 : -1];
typedef char InitCBodyPrefix_size_must_be_0x54[
    (sizeof(CBody) == 0x54) ? 1 : -1];
// Absolute-addressed globals used by the initialization owner.
extern "C" SViewport ApocalypseViewport;
extern "C" int TopOfCode
    __attribute__((section(".data")));

#define FRONTEND2STATE_ABSOLUTE_DECLARATIONS
#define FRONTEND2STATE_DECLARE_MUSIC_LOOP_VOICE
#include "FRONTEND2STATE.h"
extern "C" SSaveGame LastGameState;
#define FRONTTIMERS_TITLE_TIMEOUT_ABSOLUTE_DECLARATION
#include "FRONTTIMERS.h"
#define BITSTATE_DECLARE_TOTAL_BIT_USAGE
#include "BITSTATE.h"
#define TOTALCCLASSUSAGE_ABSOLUTE_DECLARATION
#include "TOTALCCLASSUSAGE.h"
#define TOTALCOMMANDPOINTMEMORY_ABSOLUTE_DECLARATION
#include "TOTALCOMMANDPOINTMEMORY.h"
#define TOTALPALETTEUSAGE_ABSOLUTE_DECLARATION
#include "TOTALPALETTEUSAGE.h"
#define TOTALTEXTUREUSAGE_ABSOLUTE_DECLARATION
#include "TOTALTEXTUREUSAGE.h"
extern "C" int TotalVabUsage
    __attribute__((section(".data")));

#include "MECHLIST.h"
#include "CAMERALIST.h"
#define BODYLISTS_DECLARE_ENVIRONMENTAL_OBJECT_LIST
#include "BODYLISTS.h"
#define BODYLISTS_DECLARE_BACKGROUND_LIST
#include "BODYLISTS.h"
#define BODYLISTS_DECLARE_SUSPENDED_LIST
#include "BODYLISTS.h"

/** Typed access to one target-fixed global that has no usable source symbol. */
template <typename T, unsigned int Address>
static inline T &Init_FixedAddress(void)
{
    return *reinterpret_cast<T *>(Address);
}

/** Start address of the small heap. */
static inline unsigned int &Init_Heap0Start(void)
{
    return Init_FixedAddress<unsigned int, 0x800DA488>();
}

/** End address of the small heap. */
static inline unsigned int &Init_Heap0End(void)
{
    return Init_FixedAddress<unsigned int, 0x800DA48C>();
}

/** Start address of the large heap. */
static inline unsigned int &Init_Heap1Start(void)
{
    return Init_FixedAddress<unsigned int, 0x800DA490>();
}

/** End address of the large heap. */
static inline unsigned int &Init_Heap1End(void)
{
    return Init_FixedAddress<unsigned int, 0x800DA494>();
}

/** Frontend selection for the first player. */
static inline int &Init_SelectedSkater(void)
{
    return Init_FixedAddress<int, 0x800D7E44>();
}

/** Frontend selection for the second player. */
static inline int &Init_SelectedSkater2(void)
{
    return Init_FixedAddress<int, 0x800D9614>();
}

/** Number of active players in the current game. */
static inline int &Init_NumberOfPlayers(void)
{
    return Init_FixedAddress<int, 0x800D95E0>();
}

/** Distance at which inactive bodies enter the suspended list. */
static inline unsigned int &Init_SuspendDistance(void)
{
    return Init_FixedAddress<unsigned int, 0x800D9B48>();
}

#include "PACK.h"
#include "POWERUP.h"
#include "CARD.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include "DB.h"
#include "SFX.h"
#include "PAD.h"
#include "REDBOOK.h"
#include "TRICKOB.h"
#include "M3DINIT.h"
#include "M3DZONE.h"
#include "GAMEFMV.h"
#include "UTILS.h"
#include "FONT.h"
#include "FLASH.h"
#include "FRONT.h"
#include "VIDEO.h"
#include "RAIL.h"
#include "CAMPT.h"

// Absolute-addressed view of the owner-local startup rectangle.
extern "C" RECT Init_BootClearRectAddress[] asm("DAT_800daaa4");

// === External function declarations (C++ mangling) ===
#include "MEM.h"

// === Init_Cleanup external functions ===
#include "CREATE.h"
#include "TRIG.h"
#include "SPOOL.h"

// === Init_Restart external functions ===
#include "PAL.h"

// === Init_AtStart external functions ===
#include <LIBETC.H>
#include "CAMERAAPI.h"
#include "FILEIO.h"

/**
 * Placeholder trigger name used before a level has supplied its real TRG
 * filename.
 */
static char Init_DefaultTrigFileName[8]
    __attribute__((section(".sdata"))) = "prsn_t";

/**
 * Debug continuation marker retained by the original initialization module.
 */
static const char Init_ContinueTRGName[24]
    __attribute__((section(".rdata"))) = "********blaablaa";

extern "C" char *ContinueTRG;
extern "C" int NumContinues;

/** Active trigger filename initialized to the module's placeholder name. */
char *TrigFileName
    __attribute__((section(".sdata"))) = Init_DefaultTrigFileName;

/** Optional trigger continuation name used by the debug continuation path. */
char *ContinueTRG
    __attribute__((section(".sdata"))) = const_cast<char *>(Init_ContinueTRGName);

/**
 * Full-screen clear rectangle copied to the stack during boot graphics
 * initialization.
 */
static RECT Init_BootClearRect_owner asm("DAT_800daaa4")
    __attribute__((section(".sdata"))) = {0, 0, 1024, 512};

/** Number of trigger continuation requests pending at startup. */
int NumContinues
    __attribute__((section(".sdata"))) = 0;

// === Owner-local function declaration ===
static void DeleteList(CBody * pList);

// === Function implementations (address order) ===

/**
 * DeleteList__FP5CBody
 *
 * Address:   0x8006AAE0
 * Mangled:   DeleteList__FP5CBody
 * Demangled: DeleteList(CBody *)
 *
 * @param pList  List to operate on.
 */
static void DeleteList(CBody * pList) {
    if (pList != 0) {
        CBody *pNext = pList->CItem.mNextItem;
        while (1) {
            if (!(pList->mCBodyFlags & 0x20)) {
                if (pList != 0) {
                    pList->CItem._vf->Destroy(pList, 3);
                }
            }
            if (pNext == 0) break;
            pList = pNext;
            pNext = pList->CItem.mNextItem;
        }
    }
}


/**
 * Init_KillAll__Fv
 *
 * Address:   0x8006AB4C
 * Mangled:   Init_KillAll__Fv
 * Demangled: Init_KillAll(void)
 *            Delete all active gameplay object lists, then clear trick, rail, bit, and message systems.
 *
 */
void Init_KillAll(void) {
    DeleteList(MechList);
    DeleteList(CameraList);
    DeleteList(BaddyList);
    DeleteList(EnvironmentalObjectList);
    DeleteList(BackgroundList);
    DeleteList(PowerUpList);
    DeleteList(SuspendedList);
    TrickOb_DeleteAll();
    Rail_DeleteAll();
    Bit_DeleteAll();
    Mess_DeleteAll();
}


/**
 * Init_Cleanup__Fi
 *
 * Address:   0x8006ABF4
 * Mangled:   Init_Cleanup__Fi
 * Demangled: Init_Cleanup(int)
 *
 * @param type  Type code.
 */
void Init_Cleanup(int type) {
    pCurrentViewport = &ApocalypseViewport;

    SFX_ResetMIDIPausedPriorToPause();
    SFX_LevelStart();
    SFX_StopAll();

    FrontEnd2_MusicLoopVoice = 0;
    ApocalypseViewport.Zoom = 256;

    GameFMV_StopFMV();
    Redbook_XAStop();
    Init_KillAll();
    Trig_DeleteCommandPoints();
    CamPt_Cleanup();

    if (type == 0) {
        SFX_MIDIStop();
        SFX_SpoolOutMIDI();
        SFX_ReverbOff();

        if (Create_UseSkater(Init_SelectedSkater())) {
            Create_FreeSkater();
        }

        if (Init_NumberOfPlayers() >= 2) {
            if (Create_UseSkater(Init_SelectedSkater2())) {
                Create_FreeSkater();
            }
        }

        Spool_ClearAllPSXs();
        Trig_RemoveDualBuffer();
        Trig_DeleteTrigFile();
        Pack_DefragmentFully();
        LastGameState.State.RestartName[0] = 0;
    } else if (type == 1) {
        Spool_ClearEnvironmentRegions();
    }

    Utils_InitialRand(0x12a6cc58);
    Trig_ClearTrigMenu();
    Flash_Reset();

    if (type != 2) {
        M3dZone_Init();
        TrickOb_Init();
        Rail_Init();
    }

    TTime = 0;
    Pad_ClearAll();

    Init_SuspendDistance() = 13000;
    CurrentWaterDamage = 0;
    CurrentWaterLevel = 0x7FFFFFFF;
    CurrentWaterLevel2 = 0x7FFFFFFF;
}


/**
 * Init_ResetMemory__Fi
 *
 * Address:   0x8006ADA4
 * Mangled:   Init_ResetMemory__Fi
 * Demangled: Init_ResetMemory(int)
 *            Reset heap boundaries, reinitialize memory, and clear tracked subsystem memory usage.
 *
 * @param Heap0Size  Number of bytes reserved for the primary heap.
 */
int Init_ResetMemory(int Heap0Size) {
    Init_Heap0Start() = TopOfCode;
    Init_Heap0End() = TopOfCode + Heap0Size;
    Init_Heap1Start() = TopOfCode + Heap0Size;
    Init_Heap1End() = 0x801FE000;

    Mem_Init();

    TotalCClassUsage = 0;
    TotalCommandPointMemory = 0;
    TotalPaletteUsage = 0;
    TotalPackUsage = 0;
    TotalTextureUsage = 0;
    TotalVabUsage = 0;
    TotalBitUsage = 0;
}


/**
 * Init_Restart__F13EPackInitTypei
 *
 * Address:   0x8006AE2C
 * Mangled:   Init_Restart__F13EPackInitTypei
 * Demangled: Init_Restart(EPackInitType, int)
 *
 * @param Mode  Pack-buffer layout selected for the restarted game mode.
 * @param SmallHeapSize  Number of bytes reserved for the primary heap.
 */
void Init_Restart(EPackInitType Mode, int SmallHeapSize) {
    pBMR = 0;
    pCompressedBMR = 0;

    Init_Cleanup(0);
    Init_ResetMemory(SmallHeapSize);
    Db_Reset();
    Spool_Init();

    TitleTimeout = 20;

    Flash_Reset();

    if (Mode == PACK640x480) {
        Pack_Init640x480();
    } else {
        Pack_Init();
    }

    SFX_InitAtStart();
    Pal_Init();
    Font_Reset();
    Pad_LockAnalog();
}


/**
 * Init_ForGame__Fv
 *
 * Address:   0x8006AEE0
 * Mangled:   Init_ForGame__Fv
 * Demangled: Init_ForGame(void)
 *
 */
void Init_ForGame(void) {
    pCurrentViewport = &ApocalypseViewport;
    Db_Init();
    Front_Init();
    Video_InitOnce();
    CamPt_Init();
    Video_Init();
}


/**
 * Init_DebugStuff__Fv
 *
 * Address:   0x8006AF30
 * Mangled:   Init_DebugStuff__Fv
 * Demangled: Init_DebugStuff(void)
 *
 */
void Init_DebugStuff(void) {
}


/**
 * Init_AtStart__Fv
 *
 * Address:   0x8006AF38
 * Mangled:   Init_AtStart__Fv
 * Demangled: Init_AtStart(void)
 *            Initialize callbacks, graphics, input, boot clearing, RNG, audio, file, FMV, 3D, and XA systems.
 *
 */
void Init_AtStart(void) {
    ResetCallback();
    Card_Init(1);

    pCurrentViewport = &ApocalypseViewport;

    Pad_InitAtStart();
    InitGeom();
    ResetGraph(0);
    SetGraphDebug(0);
    Db_InitAtStart();

    // Clear the boot draw area using the startup RECT copied from data.
    {
        RECT screen = Init_BootClearRectAddress[0];
        ClearImage(&screen, 0, 0, 0);
    }

    VSyncCallback(MyVSync);
    SetDispMask(1);
    Init_DebugStuff();
    Camera_InitialRand(0x9F6AD);
    SFX_InitAtStart();
    FileIO_Init();
    GameFMV_Init();
    M3dInit_InitAtStart();
    Redbook_XAInit();
}
