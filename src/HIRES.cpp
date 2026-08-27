// HIRES.cpp -- Reconstructed translation unit
// Original: HIRES.CPP
// Functions: 5 reconstructed entries
// Address range: 0x800632BC - 0x800633B8
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#include "HIRES.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include "UTILS.h"

#include "SVIEWPORT.h"

#define DB_BUFFER_LAYOUT
#include "DB.h"
#undef DB_BUFFER_LAYOUT
#define STATS_LEVELS_READ_ONLY
#include "STATS.h"
#undef STATS_LEVELS_READ_ONLY

#include "STARTUPGAMESTATE.h"
/** The complete element layout proves stride; this TU does not need the bound. */
#include "DOUBLEBUFFER.h"
#include "PDOUBLEBUFFER.h"
#define GLEVEL_OWNER_DECLARATION
#include "GLEVEL.h"

/** Cross-owner front-end services used by the high-resolution shell. */
#include "FRONT.h"
#include "SFX.h"
#include "FRONTEND2.h"

#include "MAIN.h"
#include "RFRONT.h"

enum {
    HiresOrderingTableDrawOffset = 4096,
    HiresCachedAddressMask = 0x7FFFFFFF,
};

/** Projection used while rendering the high-resolution front end. */
SViewport HiResViewport __attribute__((section(".data"))) = {
    640, 480, 0, 0, 10, 30000, 0x0D55, 0, 0, 0,
};

/** Skater-region table retained by the high-resolution shell. */
int SkaterRegion[HiresSkaterRegionCount]
    __attribute__((section(".data"))) = {};

/** Default VAB loaded while the high-resolution title screen is active. */
static char SFXDataPath[] __attribute__((section(".sdata"))) = "shell.vab";

/**
 * Reserved initialization hook for high-resolution rendering.
 *
 * @param clear_screen Whether the caller requested an initial screen clear.
 */
void HiRes_Init(bool clear_screen)
{
}

/**
 * Clears the current high-resolution ordering table.
 */
void HiRes_ClearOTag(void)
{
    ClearOTagR(reinterpret_cast<unsigned long *>(pDoubleBuffer->PanelOT),
              Db_OTSize());
}

/**
 * Presents the current high-resolution ordering table, selects the next double
 * buffer, refreshes pPoly, and polls the host after the draw submission.
 */
void HiRes_DrawOTag(void)
{
    FlickerSync();
    ResetGraph(1);
    SDoubleBuffer *currentBuffer = pDoubleBuffer;
    DrawOTag(reinterpret_cast<unsigned long *>(
        currentBuffer->OrderingTable + HiresOrderingTableDrawOffset));

    SDoubleBuffer *newBuffer = DoubleBuffer;
    currentBuffer = pDoubleBuffer;
    if (newBuffer == currentBuffer)
    {
        newBuffer++;
    }
    pPoly = reinterpret_cast<unsigned int *>(
        reinterpret_cast<unsigned int>(newBuffer->Polys) &
        HiresCachedAddressMask);
    pDoubleBuffer = newBuffer;
    PollHost();
}

/**
 * Initializes the front end, resets its demo timer, loads sound data, and
 * enters the title-screen loop.
 */
void HiRes_TitleScreen(void)
{
    RECT r; // Original scratch rectangle retained in the frame layout.

    Front_Init();
    ResetMainDemoTimer();
    SFX_Init(SFXDataPath);
    FrontEnd2_Main();
}

/**
 * Selects the startup level and skater, copies the level trigger filename,
 * clears the restart name, and launches gameplay.
 *
 * @param Selected Index of the level to launch.
 * @param guy Index of the selected skater.
 */
void kickIt(int Selected, int guy)
{
    SelectedSkater = guy;

    GLevel = Selected;
    Utils_CopyString(
        Levels[Selected].pTRGName, StartupGameState.State.TrigFileName);
    StartupGameState.State.RestartName[0] = '\0';
    LaunchGame();
}
