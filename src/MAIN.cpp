// MAIN.cpp -- Reconstructed translation unit
// Original: MAIN.CPP
// Functions: 36 reconstructed entries
// Address range: 0x80033160 - 0x80036848
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#define MAIN_OWNER_DECLARATIONS
#include "MAIN.h"
#undef MAIN_OWNER_DECLARATIONS
#include "APOCALYPSEVIEWPORT.h"
#include "GSKATER_STATE.h"
#include "BADDY.h"
#include "VIDEO.h"
#include "PHYSICS.h"
#include "RFRONT.h"
#include "COMPCHECK_H.h"
#include "CITEM.h"
#include "SHELL.h"
#include "CARD.h"
#include "CAREER.h"
#define CAREER_OWNER_LAYOUT
#include "CAREER.h"
#undef CAREER_OWNER_LAYOUT
#include "FLASH.h"
#include "GAMEFMV.h"
#include "HIRES.h"
#include "INIT.h"
#include "PACK.h"
#include "POWERUP.h"
#include "REDBOOK.h"
#include "SHATTER.h"
#include "STATS.h"
#include "MECH.h"
#include "BOARDSTATSSTATE.h"
#include "TRICKOB.h"
#include "TRICKTIPS.h"
#include "OB.h"
#include "CBODY.h"
#include "CSUPER.h"
#include "M3D.h"
#include "UTILS.h"
#include "VBLANKUPDATEBAR.h"
#include "STARTSCREEN.h"
#include "FONT.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBGPU.H>

/** Placement construction for explicitly allocated gameplay objects. */
inline void *operator new(unsigned int, void *address)
{
    return address;
}

/** Exact-size camera view used by MAIN's construction and query paths. */
struct CCamera
{
    unsigned char _padToCameraMode[0x558];
    int mCameraMode;
    unsigned char _remainingState[0x684 - 0x55C];

    CCamera(CBody *tripod, unsigned short mikeIndex);
    /** Return the current fixed-point projection zoom. */
    int GetZoom();
    /** Enter the competition-intro camera mode. */
    void StartCompetitionIntro();
    /** Return whether the competition-intro camera mode is active. */
    int InCompetitionIntro();
};

typedef CCamera MainCreationCamera;

/**
 * MAIN's read-only diagnostic text.
 *
 * These strings form one contiguous main.obj contribution immediately before
 * the two compiler-generated switch tables. Keeping even the currently unused
 * memory-report labels preserves the original table and literal order.
 */
    static char Main_checksumSuccessFormat[]
        __attribute__((section(".rdata"))) =
        "FOUND Valid checksum %lx , score %d, on the %dth try\n";
    static char Main_randomAttemptsFormat[]
        __attribute__((section(".rdata"))) =
        "Tried\t%d randoms numbers, \n";
    static char sDumpAutoTestFmt[]
        __attribute__((section(".rdata"))) =
        "L-%d, P1-%d, P2-%d, G-%d";
    static char Main_memoryRowFormat[]
        __attribute__((section(".rdata"))) =
        "%5d,%03d - %s (%d%%)\n";
    static char Main_memoryUsageHeader[]
        __attribute__((section(".rdata"))) =
        "\nMEMORY USAGE  (1K = 1000 bytes\n\n";
    static char Main_tablesAndVariablesLabel[]
        __attribute__((section(".rdata"))) =
        "Tables and Variables";
    static char Main_commandPointUsageLabel[]
        __attribute__((section(".rdata"))) =
        "Command Point usage";
    static char Main_bitEffectsLabel[]
        __attribute__((section(".rdata"))) =
        "Bits effects (glows, fringes)";
    static char Main_skater1DecompressBufferLabel[]
        __attribute__((section(".rdata"))) =
        "Skater 1 Decompress Buffer";
    static char Main_skater1VideoReplayBufferLabel[]
        __attribute__((section(".rdata"))) =
        "Skater 1 Video Replay buffer";
    static char Main_skater2DecompressBufferLabel[]
        __attribute__((section(".rdata"))) =
        "Skater 2 Decompress Buffer";
    static char Main_skater2VideoReplayBufferLabel[]
        __attribute__((section(".rdata"))) =
        "Skater 2 Video Replay buffer";
    static char Main_otherHeapUsageLabel[]
        __attribute__((section(".rdata"))) =
        "Other Heap Usage";
    static char Main_endCodeTweakJumpMessage[]
        __attribute__((section(".rdata"))) =
        "ENDCODE_TWEAKJUMP\n";
    static char Main_notEndCodeTweakJumpMessage[]
        __attribute__((section(".rdata"))) =
        "not .... ENDCODE_TWEAKJUMP\n";

/**
 * MAIN's initialized small-data prefix.
 *
 * MAIN.SYM assigns this complete run to main.obj, from Cheat_FlipScreen
 * through PatchY. Keeping the real owners together also preserves the
 * owner-local Released/LoopTimer/FPS string fragment which follows it.
 */
    int Cheat_FlipScreen __attribute__((section(".sdata"))) = 0;
    short *GTricks __attribute__((section(".sdata"))) = 0;
    char Code_String[9] __attribute__((section(".sdata"))) = "12345678";
    int GGame __attribute__((section(".sdata"))) = 1;
    int GNumberOfPlayers __attribute__((section(".sdata"))) = 1;
    int GAutoTest __attribute__((section(".sdata"))) = 0;
    int GAutoPlayer1 __attribute__((section(".sdata"))) = 0;
    int GAutoPlayer2 __attribute__((section(".sdata"))) = 0;
    int GAutoLevel __attribute__((section(".sdata"))) = 0;
    int GAutoGame __attribute__((section(".sdata"))) = 0;
    int GAutoPlayer1a __attribute__((section(".sdata"))) = 0;
    int GAutoPlayer2a __attribute__((section(".sdata"))) = 0;
    int GAutoLevela __attribute__((section(".sdata"))) = 0;
    int GAutoGamea __attribute__((section(".sdata"))) = 0;
    int GHorseWordChosen __attribute__((section(".sdata"))) = 0;
    int Mick_MapMode __attribute__((section(".sdata"))) = 0;
    int Mick_MapDoit __attribute__((section(".sdata"))) = 0;
    int SelectedSkater2 __attribute__((section(".sdata"))) = 0;

/** Optional auto-test restart label displayed over the current frame. */
static char *pAutoTestRestartName __attribute__((section(".sdata"))) = 0;

    EEndCode EndCode __attribute__((section(".sdata"))) = ENDCODE_NONE;
    int TTime __attribute__((section(".sdata"))) = 0;
    int EveryOther __attribute__((section(".sdata"))) = 0;
    int Restart __attribute__((section(".sdata"))) = 0;
    int CurrentWaterLevel __attribute__((section(".sdata"))) = 0x7fffffff;
    int CurrentWaterLevel2 __attribute__((section(".sdata"))) = 0x7fffffff;
    int CurrentDualShockLevel __attribute__((section(".sdata"))) = 2;
    int GameFrozen __attribute__((section(".sdata"))) = 0;
#define CHEATPANELON_OWNER_DECLARATION
#include "CHEATPANELON.h"
    int Cheat_PanelOn __attribute__((section(".sdata"))) = 1;
    int Cheat_L2KillsBaddies __attribute__((section(".sdata"))) = 0;
    int Cheat_LevelSelect __attribute__((section(".sdata"))) = 0;
    int Cheat_AutoTest __attribute__((section(".sdata"))) = 0;
#define FRONTCHEATS_OWNER_DECLARATION
#include "FRONTCHEATS.h"
    int Cheat_RestartSkip __attribute__((section(".sdata"))) = 0;
    int Cheat_GameInfo __attribute__((section(".sdata"))) = 0;
    int Cheat_FPS __attribute__((section(".sdata"))) = 0;
    int Cheat_Blood __attribute__((section(".sdata"))) = 1;
    int Cheat_SlowMo __attribute__((section(".sdata"))) = 0;
    int Cheat_BigHead __attribute__((section(".sdata"))) = 0;
    int Cheat_ScreenShots __attribute__((section(".sdata"))) = 0;
    int Cheat_Fire __attribute__((section(".sdata"))) = 0;
    int PolyBufferBytesUsed __attribute__((section(".sdata"))) = 0;
    int MusicStopped __attribute__((section(".sdata"))) = 0;
    VECTOR Zero __attribute__((section(".sdata"))) = {0, 0, 0, 0};
    #define TOTALCCLASSUSAGE_OWNER_DECLARATION
    #include "TOTALCCLASSUSAGE.h"
    int TotalCClassUsage __attribute__((section(".sdata"))) = 0;
    int Mick_Darkness __attribute__((section(".sdata"))) = 0;
    int ViewportMode __attribute__((section(".sdata"))) = 0;
    int PatchX __attribute__((section(".sdata"))) = 16;
    int PatchY __attribute__((section(".sdata"))) = 150;

/** Select-button edge latch owned by Display. */
static int Display_Released asm("Released.30")
    __attribute__((section(".sdata"))) = 1;

/** MAIN-owned frame-loop state stored alongside the FPS counters. */
static unsigned int LoopTimer __attribute__((section(".sdata"))) = 0;

int FPSIndex __attribute__((section(".sdata"))) = 0;

/**
 * MAIN-private storage that also needs compiler-distinct absolute address
 * views remains namespace-scoped while retaining its target C symbols.
 */
namespace MainAddressViewOwners
{
extern "C"
{
static char Main_fpsFormat[]
    __attribute__((section(".sdata"))) = " %d FPS";
}
}

/**
 * Small diagnostic strings retained by the original main.obj data image.
 */
namespace MainAddressViewOwners
{
extern "C"
{
    static char DefaultPSH[]
        __attribute__((section(".sdata"))) = "sk2anim";
}
}
    static char Main_gameFontName[]
        __attribute__((section(".sdata"))) = "ingame1.fnt";
    int MemTotal __attribute__((section(".sdata"))) = 0;
    static char Main_systemAreaLabel[]
        __attribute__((section(".sdata"))) = "System Area";
    static char Main_codeLabel[]
        __attribute__((section(".sdata"))) = "Code";
    static char Main_polyBuffersLabel[]
        __attribute__((section(".sdata"))) = "2 Poly Buffers ";
    static char Main_orderingTablesLabel[]
        __attribute__((section(".sdata"))) = "2 OTs";
    static char Main_tricksFileName[]
        __attribute__((section(".sdata"))) = "Tricks.bin";
    static char Main_triggerFileLabel[]
        __attribute__((section(".sdata"))) = ".TRG file";
    static char Main_classUsageLabel[]
        __attribute__((section(".sdata"))) = "CClass usage";
    static char Main_palettesLabel[]
        __attribute__((section(".sdata"))) = "Palletes";
    static char Main_packRegionsLabel[]
        __attribute__((section(".sdata"))) = "Pack Regions";
    static char Main_textureInfoLabel[]
        __attribute__((section(".sdata"))) = "Texture Info";
    static char Main_vabHeadersLabel[]
        __attribute__((section(".sdata"))) = "VAB Headers";
    static char Main_memoryDivider[]
        __attribute__((section(".sdata"))) = "---------\n";
    static char Main_freeMemoryLabel[]
        __attribute__((section(".sdata"))) = "Free Memory";
    int DemoDiskMode __attribute__((section(".sdata"))) = 0;

namespace MainAddressViewOwners
{
extern "C"
{
    static char s_xxx_main_800d976c[]
        __attribute__((section(".sdata"))) = "xxx main\n";
    static char DAT_800d9778[]
        __attribute__((section(".sdata"))) = "9,";
    static char DAT_800d977c[]
        __attribute__((section(".sdata"))) = "10,";
    static char DAT_800d9780[]
        __attribute__((section(".sdata"))) = "11,";
    static char DAT_800d9784[]
        __attribute__((section(".sdata"))) = "12,";
    static char DAT_800d9788[]
        __attribute__((section(".sdata"))) = "13\n";
    static char s_s2bgd0_bmp_800d978c[]
        __attribute__((section(".sdata"))) = "s2bgd0.bmp";
    static char s_s2market_bmp_800d9798[]
        __attribute__((section(".sdata"))) = "s2market.bmp";
}
}

    void *OldSP __attribute__((section(".sdata"))) = 0;
    int CurrentGameLevel __attribute__((section(".sdata"))) = 0;
    int CurrentWaterDamage __attribute__((section(".sdata"))) = 0;
    int CurrentDifficultyLevel __attribute__((section(".sdata"))) = 0;
    int XFrames __attribute__((section(".sdata"))) = 0;
    int TimeScale __attribute__((section(".sdata"))) = 0;
    int TimeScaleSquared __attribute__((section(".sdata"))) = 0;
    unsigned char *PolyBufferEnd __attribute__((section(".sdata"))) = 0;
    int GState __attribute__((section(".sdata"))) = 0;
    int GSpin __attribute__((section(".sdata"))) = 0;
    int GSpinSpeed __attribute__((section(".sdata"))) = 0;
    int GStartTime __attribute__((section(".sdata"))) = 0;
    int GSkaterRegion[2] __attribute__((section(".sdata"))) = {0, 0};
    int GHorsePlayer __attribute__((section(".sdata"))) = 0;
    int GHorseMode __attribute__((section(".sdata"))) = 0;
    int GHorseScore __attribute__((section(".sdata"))) = 0;
    int GHorseDoneStart __attribute__((section(".sdata"))) = 0;
    int GHorseRestart __attribute__((section(".sdata"))) = 0;
    /** Per-player counts of letters accumulated in the current HORSE round. */
    int GHorseLetters[2] __attribute__((section(".sdata"))) = {0, 0};
    int GHorseCursorPos __attribute__((section(".sdata"))) = 0;
    int GHorseWaitTime __attribute__((section(".sdata"))) = 0;
    int GHorseClearInput2 __attribute__((section(".sdata"))) = 0;
    char GVideoFlags __attribute__((section(".sdata"))) = 0;
    short *GTricksKeys __attribute__((section(".sdata"))) = 0;
    short *GGrindTricksList __attribute__((section(".sdata"))) = 0;
    short *GBailsList __attribute__((section(".sdata"))) = 0;
    CBruce *GSkater[2]
        __attribute__((section(".sdata"))) = {0, 0};

    CPowerUp *GPowS __attribute__((section(".sdata"))) = 0;
    CPowerUp *GPowK __attribute__((section(".sdata"))) = 0;
    CPowerUp *GPowA __attribute__((section(".sdata"))) = 0;
    CPowerUp *GPowT __attribute__((section(".sdata"))) = 0;
    CPowerUp *GPowE __attribute__((section(".sdata"))) = 0;
    CPowerUp *GPowTape __attribute__((section(".sdata"))) = 0;
    int GClear1 __attribute__((section(".sdata"))) = 0;
    int GClear2 __attribute__((section(".sdata"))) = 0;
    int GRenderedInvisible __attribute__((section(".sdata"))) = 0;
    int GRenderedCulled __attribute__((section(".sdata"))) = 0;
    int GRenderedFast __attribute__((section(".sdata"))) = 0;
    int GRenderedNonRotated __attribute__((section(".sdata"))) = 0;
    int GRenderedRotated __attribute__((section(".sdata"))) = 0;
    int NumPolyF3 __attribute__((section(".sdata"))) = 0;
    int NumPolyFT3 __attribute__((section(".sdata"))) = 0;
    int NumPolyG3 __attribute__((section(".sdata"))) = 0;
    int NumPolyGT3 __attribute__((section(".sdata"))) = 0;
    int NumPolyF4 __attribute__((section(".sdata"))) = 0;
    int NumPolyFT4 __attribute__((section(".sdata"))) = 0;
    int NumPolyG4 __attribute__((section(".sdata"))) = 0;
    int NumPolyGT4 __attribute__((section(".sdata"))) = 0;
    int NumSetTile __attribute__((section(".sdata"))) = 0;
    int NumTile1 __attribute__((section(".sdata"))) = 0;
    int NumOther __attribute__((section(".sdata"))) = 0;
    int CloseCam __attribute__((section(".sdata"))) = 0;
    int PrimitiveCount __attribute__((section(".sdata"))) = 0;
    int XFramesShifted __attribute__((section(".sdata"))) = 0;
    int FPS __attribute__((section(".sdata"))) = 0;
    static int Main_FPSTail[2]
        __attribute__((section(".sdata"))) = {0, 0};
    int LastXblanks __attribute__((section(".sdata"))) = 0;
    int Profile_Bop __attribute__((section(".sdata"))) = 0;
    Sprite *pScoreSprite __attribute__((section(".sdata"))) = 0;
    Sprite *pSpecialSprite __attribute__((section(".sdata"))) = 0;
    Sprite *pBarSprite __attribute__((section(".sdata"))) = 0;
    Sprite *pPausedSprite __attribute__((section(".sdata"))) = 0;
    int MemDumpLoop __attribute__((section(".sdata"))) = 0;
    int DemoSize __attribute__((section(".sdata"))) = 0;

#include "PAD.h"

#include "SAVEGAME.h"

/**
 * Persistent career/save state owned by MAIN.
 *
 * The member layout is recovered from MAIN_STRUCTS and is shared with the
 * proven career owner overlays.  Its 2,420-byte size is part of the original
 * binary interface.
 */
struct SCareer
{
    int Version;
    int MaxTapes;
    SCharacterProgress CharacterProgress[14];
    SHighScore HighScore[16][8];
    char LastSkater[2];
    char LastDeck[2];
    char LastWheel[2];
    char LastTrucks[2];
    char LastLevel;
    char pad_7D1;
    unsigned short TimeStamp;
    SOptions Options;
};

typedef char SCareer_size_must_be_2420[(sizeof(SCareer) == 2420) ? 1 : -1];
typedef char SDemo_size_must_be_8[(sizeof(SDemo) == 8) ? 1 : -1];
typedef char SViewport_size_must_be_20[(sizeof(SViewport) == 20) ? 1 : -1];

#include "SCAMERA.h"

/** Display buffer fields used to select the ordering table for a viewport. */
struct MainDoubleBuffer
{
    DRAWENV Draw;
    DISPENV Disp;
    int Visible;
    int RenderNow;
    int Rendering;
    int FlipNow;
    int LastVblanks;
    unsigned int *PanelOT;
    unsigned int *OrderingTable;
    unsigned int *ViewportOT;
    int Top;
    RECT Window;
    DR_AREA Clip;
    unsigned char *Polys;
};

/** CBruce fields consulted while selecting the mechanical-model OT bias. */
struct MainViewportBruce
{
    unsigned char pad_0000[0xF7C];
    int mInBail;
    unsigned char pad_0F80[0xEC];
    int mPhysicsState;
};

/** CBruce fields used to decide whether to draw each player's balance meter. */
struct MainDisplayBruce
{
    unsigned char pad_0000[0xE10];
    int mBalanceMeterActive;
    unsigned char pad_0E14[0x134];
    int mBalanceValue;
};

/** Palette prefix used by MAIN's runtime texture replacement path. */
struct MainTexturePalette
{
    unsigned short Clut;
    unsigned char slot;
    unsigned char flags;
};

/** Texture prefix through its VRAM coordinates, recovered from MAIN_STRUCTS. */
struct MainTexture
{
    unsigned char pad_000[0x18];
    MainTexturePalette *pPalette;
    unsigned short x;
    unsigned short y;
};

typedef char Main_SDoubleBuffer_size_must_be_172[(sizeof(MainDoubleBuffer) == 172) ? 1 : -1];
typedef char Main_TexturePalette_prefix_size_must_be_4[
    (sizeof(MainTexturePalette) == 4) ? 1 : -1];
typedef char Main_Texture_prefix_size_must_be_32[
    (sizeof(MainTexture) == 32) ? 1 : -1];

extern char Main_fpsFormat[];

// FILEIO is a natural C++ owner interface; keep it outside the C-linkage
// declarations that follow.
#include "FILEIO.h"
#include "MEM.h"
#include "SPOOL.h"

/** Draw every active transient bit effect. */
void Bit_Display(void);

/** Method-only view of the skater interface used by MAIN. */
struct CBruce
{
    /** Save and temporarily adjust this skater's render transform. */
    void TempTransform();
    /** Restore this skater's saved render transform. */
    void TempRestore();
    /** Initialize item data and the skater-specific hook packet. */
    void InitItem(char *filename);
    /** Fill one displayed-stat slot for this skater. */
    void GetStats(int skaterSlot);
};

// === External declarations (C linkage) ===
extern "C"
{
    #include <STDIO.H>
    #include <STRINGS.H>
    #include <LIBAPI.H>

    #include "STARTUPGAMESTATE.h"
    #define FRONTFLOWSTATE_DECLARE_BACKOUT
    #include "FRONTFLOWSTATE.h"
    #include "GENERATELEVEL.h"
    extern char DAT_800d9778[];
    extern char DAT_800d977c[];
    extern char DAT_800d9780[];
    extern char DAT_800d9784[];
    extern char DAT_800d9788[];
    // Draw_Rect / Mick_Darken globals (absolute addressing)
    // db.obj owns pPoly at 0x800DA344; no pPoly_arr array view is needed in MAIN.
    // db.obj owns pPoly at 0x800DA344; no pPoly2_arr array view is needed in MAIN.
    // db.obj owns pPoly at 0x800DA344; no pPoly3_arr array view is needed in MAIN.
    // db.obj owns pPoly at 0x800DA344; no pPoly4_arr array view is needed in MAIN.
    // db.obj owns pPoly at 0x800DA344; no pPoly5_arr array view is needed in MAIN.
    // Auto-test stats variables (GP-relative)
    // Mick_Darken externals
    // Game_Over externals
    // Game_Over globals (GP-relative)
    // Game_Over globals (absolute addressing)

    // Multiplayer stat globals are owned by physics.obj; MAIN uses typed scalar views.
    // physics.obj owns GWins1 at 0x800D7D48; MAIN reads the live GWins1 scalar.
    // physics.obj owns GWins1 at 0x800D7D48; MAIN writes the live GWins1 scalar.
    // physics.obj owns GLosses1 at 0x800D7D4C; MAIN reads the live GLosses1 scalar.
    // physics.obj owns GLosses1 at 0x800D7D4C; MAIN writes the live GLosses1 scalar.
    // physics.obj owns GStreak1 at 0x800D7D50; MAIN reads the live GStreak1 scalar.
    // physics.obj owns GStreak1 at 0x800D7D50; MAIN writes the live GStreak1 scalar.
    // physics.obj owns GStreak1 at 0x800D7D50; MAIN reloads the live GStreak1 scalar.
    // physics.obj owns GHighTags1 at 0x800D7D54; MAIN reads the live GHighTags1 scalar.
    // physics.obj owns GHighTags1 at 0x800D7D54; MAIN writes the live GHighTags1 scalar.
    // physics.obj owns GWins2 at 0x800D7D58; MAIN reads the live GWins2 scalar.
    // physics.obj owns GWins2 at 0x800D7D58; MAIN writes the live GWins2 scalar.
    // physics.obj owns GLosses2 at 0x800D7D5C; MAIN reads the live GLosses2 scalar.
    // physics.obj owns GLosses2 at 0x800D7D5C; MAIN writes the live GLosses2 scalar.
    // physics.obj owns GStreak2 at 0x800D7D60; MAIN reads the live GStreak2 scalar.
    // physics.obj owns GStreak2 at 0x800D7D60; MAIN writes the live GStreak2 scalar.
    // physics.obj owns GStreak2 at 0x800D7D60; MAIN reloads the live GStreak2 scalar.
    // physics.obj owns GHighTags2 at 0x800D7D64; MAIN reads the live GHighTags2 scalar.
    // physics.obj owns GHighTags2 at 0x800D7D64; MAIN writes the live GHighTags2 scalar.

    // FPS_Init globals
    // The scalar declaration is the original code-generation view; the
    // three-word owner storage is defined above under the same linker name.

    // Display pipeline and diagnostic-message helpers.
}
#include "MESS.h"
#define PANEL_PRIMITIVE_API
#include "PANEL.h"
#undef PANEL_PRIMITIVE_API
#include "FRONTEND2.h"
#include "FRONT.h"
void LoadingScreen_Main(void);
int Horse_Reset(void);
#include "CROWD.h"
#include "RAIL.h"
#include "TRIG.h"


#define CAMERA_TUNING_ABSOLUTE_DECLARATIONS
#include "CAMERATUNING.h"
#include "FRONTSTATUS.h"

// === Free function declarations ===

void UnloadGTricks(void);
#include "DB.h"

// CClass memory tracking counter (gp+0x19E4)
// Used[1] is the global memory-usage counter at 0x800DA4C0.  This scalar
// address view preserves the two target loads around Mem_Delete.
extern int CClassDelete_Used asm("Used+4")
    __attribute__((section(".data")));

/** Eight-character header view of MAIN's nine-byte encoded-check buffer. */
#define COMPCHECK_DECLARE_CHECKSUM_STRING_VIEW
#include "COMPCHECK_H.h"

static const u32 CHECKSUM_RANDOM_MULTIPLIER = 0xF26D;
static const u32 CHECKSUM_POLYNOMIAL = 0x4517215A;
static const u32 CHECKSUM_VALUE_MASK = 0xFFFFFE00;
static const u32 CHECKSUM_GS_BIT = 0x100;
static const u32 CHECKSUM_FINAL_XOR = 0xAF350F23;

// === Function implementations (address order) ===

/**
 * Write one hexadecimal digit through the shared lookup table.
 *
 * @param p Destination character.
 * @param n Lookup-table index.
 */
void PutHexDigit(char *p, unsigned int n)
{
    *p = HexDigits[n];
}

/**
 * Write the low byte as two hexadecimal digits.
 *
 * @param p Two-character destination.
 * @param n Value to encode.
 */
void PutHexByte(char *p, unsigned int n)
{
    PutHexDigit(p, n >> 4);
    PutHexDigit(p + 1, n & 0xF);
}

/**
 * Write the low halfword as four hexadecimal digits.
 *
 * @param p Four-character destination.
 * @param n Value to encode.
 */
void PutHex2Byte(char *p, unsigned int n)
{
    PutHexByte(p, n >> 8);
    PutHexByte(p + 2, n & 0xFF);
}

/**
 * Write a complete word as eight hexadecimal digits.
 *
 * @param p Eight-character destination.
 * @param n Value to encode.
 */
void PutHex4Byte(char *p, unsigned int n)
{
    PutHex2Byte(p, n >> 16);
    PutHex2Byte(p + 4, n & 0xFFFF);
}

/**
 * Mix score state into the game's 32-bit check value.
 *
 * @param Score Score value.
 * @param Random Random byte embedded in the result.
 * @param gs Whether to toggle the game-state bit.
 * @return Encoded check value.
 */
u32 CheckSum_Calculate(u32 Score, u8 Random, int gs)
{
    u32 c;
    u32 x;
    int i;

    x = 0;
    c = Score ^ (Random & 0xFF) * CHECKSUM_RANDOM_MULTIPLIER;
    i = 0;
    do
    {
        x = (x << 1) | (x >> 31);
        if ((c & 1) != 0)
        {
            x = x ^ CHECKSUM_POLYNOMIAL;
        }
        i = i + 1;
        c = c >> 1;
    } while (i < 32);
    x &= CHECKSUM_VALUE_MASK;
    x |= (Random & 0xFF);
    if (gs != 0)
    {
        x = x ^ CHECKSUM_GS_BIT;
    }
    return x ^ CHECKSUM_FINAL_XOR;
}

/**
 * Calculate the check value and write its eight hexadecimal characters.
 *
 * @param Score Score value.
 * @param Random Random byte embedded in the result.
 * @param gs Whether to toggle the game-state bit.
 */
void CheckSum_Encode(u32 Score, u8 Random, int gs)
{
    PutHex4Byte(Main_ChecksumCodeString,
                CheckSum_Calculate(Score, Random, gs));
}

/**
 * DumpAutoTest__Fv
 *
 * Address:   0x800332F0, Size: 252 bytes (63 instructions)
 * Mangled:   DumpAutoTest__Fv
 * Demangled: DumpAutoTest(void)
 *            Debug diagnostic: displays auto-test stats on screen via PSX font system
 */
void DumpAutoTest(void)
{
    char pad[24];     // 24 bytes padding before DrawEnv
    char DrawEnv[96]; // DRAWENV at sp+0x30
    char buf[200];    // sprintf buffer at sp+0x90
    int fntId;
    DRAWENV *pDraw;
    char *pBuf;
    (void)pad;

    SetDispMask(1);
    SetGraphDebug(0);
    FntLoad(0, 0);
    fntId = FntOpen(0xc8, 0xaf, 0xa0, 0x10, 1, 0x400);
    DrawSync(0);
    pDraw = reinterpret_cast<DRAWENV *>(DrawEnv);
    SetDefDrawEnv(pDraw, 0, 0, 0x200, 0xf0);
    PutDrawEnv(pDraw);
    pBuf = buf;
    sprintf(pBuf, sDumpAutoTestFmt, GAutoLevela, GAutoPlayer1a, GAutoPlayer2a,
            GAutoGamea);
    FntPrint(pBuf);
    FntFlush(fntId);

    // Copy "previous" stats to "current"
    {
        int a = GAutoLevel;
        int b = GAutoPlayer1;
        int c = GAutoPlayer2;
        int d = GAutoGame;
        GAutoLevela = a;
        GAutoPlayer1a = b;
        GAutoPlayer2a = c;
        GAutoGamea = d;
    }
}

/**
 * ResetMainDemoTimer__Fv
 *
 * Address:   0x800333EC
 * Mangled:   ResetMainDemoTimer__Fv
 * Demangled: ResetMainDemoTimer(void)
 */
void ResetMainDemoTimer(void)
{
}

/**
 * __nw__6CClassUi
 *
 * Address:   0x800333F4
 * Mangled:   __nw__6CClassUi
 * Demangled: CClass::operator new(unsigned int)
 *            Allocates, zero-fills, and tracks CClass memory usage
 *
 * @param size  Buffer size in bytes.
 * @return ptr
 */
void *CClass::operator new(unsigned int size)
{
    void *pnew = Mem_New(size, 0, 1);
    size = (size + 3) & ~3U;
    unsigned int *p = static_cast<unsigned int *>(pnew);
    int i;
    for (i = 0; static_cast<unsigned int>(i) < size >> 2; i++)
    {
        *p++ = 0;
    }
    TotalCClassUsage += size + 4;
    return pnew;
}

/**
 * __dl__6CClassPv
 *
 * Address:   0x80033468
 * Mangled:   __dl__6CClassPv
 * Demangled: CClass::operator delete(void *)
 *            Deletes memory and tracks the freed size delta in CClass_MemUsage.
 *
 * @param p  CClass allocation to release.
 */
void CClass::operator delete(void *p)
{
    int Before = CClassDelete_Used;
    Mem_Delete(p);
    Before -= CClassDelete_Used;
    TotalCClassUsage -= Before;
}

/**
 * CalcPolyBufferEnd__Fv
 *
 * Address:   0x800334AC, Size: 52 bytes (13 instructions)
 * Mangled:   CalcPolyBufferEnd__Fv
 * Demangled: CalcPolyBufferEnd(void)
 *            Recomputes the active polygon buffer end from the current double buffer.
 */
#include "PDOUBLEBUFFER.h"
void CalcPolyBufferEnd(void)
{
    PolyBufferEnd = reinterpret_cast<unsigned char *>(
        (reinterpret_cast<unsigned int>(reinterpret_cast<MainDoubleBuffer *>(pDoubleBuffer)->Polys) +
         POLYBUFFERSIZE - 256) & 0x7FFFFFFF);
}

/**
 * PollHost__Fv
 *
 * Address:   0x800334E0
 * Mangled:   PollHost__Fv
 * Demangled: PollHost(void)
 *
 */
void PollHost(void)
{
}

/** CBruce fields owned by the end-of-game scoreboard path. */
struct MainGameOverSkater
{
    unsigned char _opaqueToTags[0xDB8];
    int mTags;
    unsigned char _opaqueToLeanAngle[0x10F0 - 0xDBC];
    int mLeanAngle;
};

/**
 * Game_Over__Fv
 * Handles end-of-game scoring: compares scores in head-to-head mode (GGame==8),
 * updates win/loss/streak counters and high scores, initializes game-over state.
 *
 * Address:   0x800334E8, Size: 420 bytes (105 instructions)
 * Mangled:   Game_Over__Fv
 * Demangled: Game_Over(void)
 */
void Game_Over(void)
{

    Redbook_XABeginFade();

    if (GGame != 8)
        goto end_section;

    {
        if (reinterpret_cast<MainGameOverSkater *>(GSkater[1])->mTags ==
            reinterpret_cast<MainGameOverSkater *>(GSkater[0])->mTags)
            goto end_section;

        if (reinterpret_cast<MainGameOverSkater *>(GSkater[0])->mTags <
            reinterpret_cast<MainGameOverSkater *>(GSkater[1])->mTags)
        {
            // Player 1 wins
            GWins2 = GWins2 + 1;
            GLosses1 = GLosses1 + 1;
            if (GStreak2 >= 0)
            {
                GStreak2 = GStreak2 + 1;
            }
            else
            {
                GStreak2 = 1;
            }
            GStreak1 = -GStreak2;
        }
        else
        {
            // Player 2 wins
            GWins1 = GWins1 + 1;
            GLosses2 = GLosses2 + 1;
            if (GStreak1 >= 0)
            {
                GStreak1 = GStreak1 + 1;
            }
            else
            {
                GStreak1 = 1;
            }
            GStreak2 = -GStreak1;
        }

        if (GHighTags1 <
            reinterpret_cast<MainGameOverSkater *>(GSkater[0])->mTags)
        {
            GHighTags1 =
                reinterpret_cast<MainGameOverSkater *>(GSkater[0])->mTags;
        }

        if (GHighTags2 <
            reinterpret_cast<MainGameOverSkater *>(GSkater[1])->mTags)
        {
            GHighTags2 =
                reinterpret_cast<MainGameOverSkater *>(GSkater[1])->mTags;
        }
    }

end_section:
    if (GState == 0)
    {
        GState = 1;
        {
            MainGameOverSkater *skater =
                reinterpret_cast<MainGameOverSkater *>(GSkater[1]);
            GSpin = 0x50;
            GSpinSpeed = 0x40;
            if (skater != 0)
            {
                skater->mLeanAngle = 0;
            }
        }
    }
}

/**
 * Mick_Darken__Fv
 * Draws a full-screen semi-transparent dark overlay.
 * Builds a POLY_F4 (flat shaded quad) covering the entire screen (0,0)-(512,240)
 * with RGB = Mick_Darkness (increasing each frame by 4, max 40).
 * Also emits a DR_TPAGE primitive for semi-transparency blending mode.
 * Both primitives are linked into the ordering table via inline addPrim.
 *
 * Address:   0x8003368C, Size: 276 bytes (69 instructions)
 * Mangled:   Mick_Darken__Fv
 * Demangled: Mick_Darken(void)
 */
void Mick_Darken(void)
{
    register int darkness asm("$4") = Mick_Darkness;
    register MainDoubleBuffer *dblBuf asm("$2") =
        reinterpret_cast<MainDoubleBuffer *>(pDoubleBuffer);
    register int cond asm("$3") = (darkness < 0x28);
    register unsigned int *otBase asm("$9") = dblBuf->PanelOT;
    if (cond)
    {
        register int newDarkness asm("$2") = darkness + 4;
        Mick_Darkness = newDarkness;
    }

    register int p asm("$8") = reinterpret_cast<int>(pPoly);
    int newppoly = p + sizeof(POLY_F4) + sizeof(DR_TPAGE);
    register int tpage_p asm("$10") = p + sizeof(POLY_F4);
    if ((u32)PolyBufferEnd < (u32)newppoly)
    {
        return;
    }
    asm volatile("" : "+r"(tpage_p));
    register int loMask asm("$5") = 0x00FFFFFF;

    // Advance pPoly past both primitives
    pPoly = reinterpret_cast<unsigned int *>(newppoly);

    register int code asm("$2") = 5;
    asm volatile("" : "+r"(code));
    register int color asm("$3") = 0x2A;
    register int tpage_cmd asm("$7") = 0xE1000000;
    asm volatile("" : "+r"(tpage_cmd));
    // Set POLY_F4 tag code = 5, color code = 0x2A (semi-transparent)
    reinterpret_cast<P_TAG *>(p)->len = code;
    reinterpret_cast<POLY_F4 *>(p)->code = color;

    register u8 d1 asm("$2") = (u8)Mick_Darkness;
    {
        register int v200 asm("$3") = 0x200;
        reinterpret_cast<POLY_F4 *>(p)->x1 = v200;
        reinterpret_cast<POLY_F4 *>(p)->x3 = v200;
    }

    register int tag asm("$3") = *(volatile int *)p;
    asm volatile("" : "+r"(tag));
    tpage_cmd |= 0x640;

    reinterpret_cast<POLY_F4 *>(p)->x0 = 0;
    reinterpret_cast<POLY_F4 *>(p)->y0 = 0;
    reinterpret_cast<POLY_F4 *>(p)->y1 = 0;
    reinterpret_cast<POLY_F4 *>(p)->x2 = 0;

    reinterpret_cast<POLY_F4 *>(p)->r0 = d1;
    u8 d2 = (u8)Mick_Darkness;

    reinterpret_cast<POLY_F4 *>(p)->y2 = 0xF0;
    reinterpret_cast<POLY_F4 *>(p)->y3 = 0xF0;

    reinterpret_cast<POLY_F4 *>(p)->g0 = d2;
    u8 d3 = (u8)Mick_Darkness;

    register int hiMask asm("$6") = 0xFF000000;

    reinterpret_cast<POLY_F4 *>(p)->b0 = d3;

    // First addPrim: link POLY_F4 into OT at otBase+4
    register int link asm("$2") = otBase[1];
    tag = (tag & hiMask) | (link & loMask);
    reinterpret_cast<POLY_F4 *>(p)->tag = tag;
    register int link2 asm("$4") = otBase[1];
    link2 = (link2 & hiMask) |
            ((u32)p & loMask);
    otBase[1] = link2;

    // Store DR_TPAGE primitive
    reinterpret_cast<DR_TPAGE *>(p + sizeof(POLY_F4))->tag = 0x01000000;
    reinterpret_cast<DR_TPAGE *>(tpage_p)->code[0] = tpage_cmd;

    // Second addPrim: link DR_TPAGE into OT at otBase+4
    register int tag2 asm("$4") =
        reinterpret_cast<DR_TPAGE *>(p + sizeof(POLY_F4))->tag;
    register int link3 asm("$3") = otBase[1];
    tag2 = (tag2 & hiMask) | (link3 & loMask);
    reinterpret_cast<DR_TPAGE *>(p + sizeof(POLY_F4))->tag = tag2;
    register int link4 asm("$2") = otBase[1];
    link4 = (link4 & hiMask) |
            ((u32)tpage_p & loMask);
    otBase[1] = link4;
}

/**
 * DownloadBMP__FPciiii
 *
 * Address:   0x800337A0
 * Mangled:   DownloadBMP__FPciiii
 * Demangled: DownloadBMP(char *, int, int, int, int)
 *
 * @param pData  Data to operate on.
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param ClutX  VRAM X coordinate for the uploaded color table.
 * @param ClutY  VRAM Y coordinate for the uploaded color table.
 */
/** BMP header fields consumed by the texture download path. */
struct MainBitmapHeader
{
    unsigned char pad_00[0x12];
    unsigned short width;
    unsigned char pad_14[2];
    unsigned short height;
    unsigned char pad_18[4];
    unsigned short bitsPerPixel;
    unsigned char pad_1E[0x10];
    unsigned short colorCount;
    unsigned char pad_30[6];
    char colorData[1];
};

void DownloadBMP(char *pData, int x, int y, int ClutX, int ClutY)
{
    unsigned short colorTab[256];
    RECT r;

    int w, h;
    w = reinterpret_cast<MainBitmapHeader *>(pData)->width;
    h = reinterpret_cast<MainBitmapHeader *>(pData)->height;

    int colors = reinterpret_cast<MainBitmapHeader *>(pData)->colorCount;
    if (colors == 0)
        colors = 0x100;

    char *pColor = reinterpret_cast<MainBitmapHeader *>(pData)->colorData;
    int i = 0;
    int transparent_color = 0;
    if (colors != 0)
    {
        int solidBlack = 0x8000;
        unsigned short *cp = colorTab;
        char *pRed = pColor + 2;
        do
        {
            unsigned short r, g, b;
            b = (unsigned short)*pColor;
            g = (unsigned short)*(pRed - 1);
            r = (unsigned short)*pRed;
            if (b > 247 && g == 0 && r > 247)
            {
                transparent_color = i;
                *cp = 0;
            }
            else
            {
                b = b >> 3;
                g = g >> 3;
                r = r >> 3;
                if (b == 0 && g == 0 && r == 0)
                    *cp = solidBlack;
                else
                    *cp = (b << 10) + (g << 5) + r;
            }
            pRed += 4;
            pColor += 4;
            i++;
            cp++;
        } while (i < colors);
    }

    r.x = ClutX;
    r.y = ClutY;
    r.w = 0x100;
    r.h = 1;
    LoadImage(&r, reinterpret_cast<unsigned long *>(colorTab));
    DrawSync(0);

    int pad = w % 4;
    if (pad > 0)
        pad = 4 - pad;
    w += pad;

    char *data = static_cast<char *>(Mem_New((w + 3) * h, 1, 1));
    char *in = reinterpret_cast<MainBitmapHeader *>(pData)->colorData +
               colors * 4;
    char *out = data + (h - 1) * w;
    for (int row = 0; row < h; row++)
    {
        for (int c = 0; c < w - pad; c++)
            *out++ = *in++;

        for (int f = 0; f < pad; f++)
        {
            *out++ = static_cast<char>(transparent_color);
            in++;
        }
        out -= w * 2;
    }

    r.x = x;
    r.y = y;
    r.w = w / 2;
    r.h = h;
    LoadImage(&r, reinterpret_cast<unsigned long *>(data));
    DrawSync(0);
    Mem_Delete(data);
}

/**
 * ReplaceTexture__FUiPc — Replace a texture in VRAM with data from a BMP file
 * Looks up a texture by checksum, loads a BMP file from disc into RAM,
 * and downloads it to the texture's VRAM location. Used for dynamic
 * texture replacement (e.g. custom skater graphics).
 * The palette Y coordinate is computed from the 256-color palette base
 * plus the palette entry's slot index.
 *
 * Address:   0x800339E4, Size: 192 bytes (48 instructions)
 * Mangled:   ReplaceTexture__FUiPc
 * Demangled: void ReplaceTexture(unsigned int, char *)
 *
 * @param Checksum  Asset hash for content lookup.
 * @param pFile  File to operate on.
 */

void ReplaceTexture(u32 Checksum, char *pFile)
{
    MainTexture *pTexture;
    int X;
    int Y;
    int ClutX;
    int ClutY;
    char *buf;

    pTexture = reinterpret_cast<MainTexture *>(
        Spool_FindTextureEntry(Checksum));
    if (pTexture != 0)
    {
        ClutY = Pal256Y;
        X = pTexture->x;
        Y = pTexture->y;
        ClutY += pTexture->pPalette->slot;
        ClutX = Pal256X;
        buf = static_cast<char *>(Mem_New(FileIO_Open(pFile), 1, 1));
        FileIO_Load(buf);
        FileIO_Sync();
        DownloadBMP(buf, X, Y, ClutX, ClutY);
        Mem_Delete(buf);
    }
}

/**
 * FlipPrimitives__Fv
 *
 * Address:   0x80033AA4
 * Mangled:   FlipPrimitives__Fv
 * Demangled: FlipPrimitives(void)
 *            Mirror the horizontal coordinates of polygon primitives linked
 *            into the active panel ordering table.
 */

/** X-coordinate view shared by the PSY-Q POLY_FT3 and POLY_G3 layouts. */
struct MainThreeVertexWidePrimitive
{
    unsigned char pad_00[8];
    short x0;
    unsigned char pad_0A[6];
    short x1;
    unsigned char pad_12[6];
    short x2;
};

/** X-coordinate view shared by the PSY-Q POLY_FT4 and POLY_G4 layouts. */
struct MainFourVertexWidePrimitive
{
    unsigned char pad_00[8];
    short x0;
    unsigned char pad_0A[6];
    short x1;
    unsigned char pad_12[6];
    short x2;
    unsigned char pad_1A[6];
    short x3;
};

typedef char Main_ThreeVertexWidePrimitive_size_must_be_0x1A[
    (sizeof(MainThreeVertexWidePrimitive) == 0x1A) ? 1 : -1];
typedef char Main_FourVertexWidePrimitive_size_must_be_0x22[
    (sizeof(MainFourVertexWidePrimitive) == 0x22) ? 1 : -1];

/** Reflect one GPU primitive X coordinate across the 512-pixel framebuffer. */
static inline short Main_MirrorPrimitiveX(unsigned short x)
{
    return static_cast<short>(0x200 - x);
}

void FlipPrimitives(void)
{
    int otSize = Db_OTSize();
    unsigned int *ot = reinterpret_cast<MainDoubleBuffer *>(pDoubleBuffer)->PanelOT;
    P_TAG *pPrim;

    pPrim = (P_TAG *)ot[otSize - 1];
    while (pPrim != (P_TAG *)0xffffff)
    {
        if (getlen(pPrim) == 0)
        {
            pPrim = (P_TAG *)getaddr(pPrim);
        }
        else
        {
            switch (getcode(pPrim) & 0x7c)
            {
                case 0x20:
                {
                    POLY_F3 *poly = reinterpret_cast<POLY_F3 *>(pPrim);
                    poly->x0 = Main_MirrorPrimitiveX(poly->x0);
                    poly->x1 = Main_MirrorPrimitiveX(poly->x1);
                    poly->x2 = Main_MirrorPrimitiveX(poly->x2);
                    break;
                }
                case 0x24:
                case 0x30:
                {
                    MainThreeVertexWidePrimitive *poly =
                        reinterpret_cast<MainThreeVertexWidePrimitive *>(pPrim);
                    poly->x0 = Main_MirrorPrimitiveX(poly->x0);
                    poly->x1 = Main_MirrorPrimitiveX(poly->x1);
                    poly->x2 = Main_MirrorPrimitiveX(poly->x2);
                    break;
                }
                case 0x34:
                {
                    POLY_GT3 *poly = reinterpret_cast<POLY_GT3 *>(pPrim);
                    poly->x0 = Main_MirrorPrimitiveX(poly->x0);
                    poly->x1 = Main_MirrorPrimitiveX(poly->x1);
                    poly->x2 = Main_MirrorPrimitiveX(poly->x2);
                    break;
                }
                case 0x28:
                {
                    POLY_F4 *poly = reinterpret_cast<POLY_F4 *>(pPrim);
                    poly->x0 = Main_MirrorPrimitiveX(poly->x0);
                    poly->x1 = Main_MirrorPrimitiveX(poly->x1);
                    poly->x2 = Main_MirrorPrimitiveX(poly->x2);
                    poly->x3 = Main_MirrorPrimitiveX(poly->x3);
                    break;
                }
                case 0x2c:
                case 0x38:
                {
                    MainFourVertexWidePrimitive *poly =
                        reinterpret_cast<MainFourVertexWidePrimitive *>(pPrim);
                    poly->x0 = Main_MirrorPrimitiveX(poly->x0);
                    poly->x1 = Main_MirrorPrimitiveX(poly->x1);
                    poly->x2 = Main_MirrorPrimitiveX(poly->x2);
                    poly->x3 = Main_MirrorPrimitiveX(poly->x3);
                    break;
                }
                case 0x3c:
                {
                    POLY_GT4 *poly = reinterpret_cast<POLY_GT4 *>(pPrim);
                    poly->x0 = Main_MirrorPrimitiveX(poly->x0);
                    poly->x1 = Main_MirrorPrimitiveX(poly->x1);
                    poly->x2 = Main_MirrorPrimitiveX(poly->x2);
                    poly->x3 = Main_MirrorPrimitiveX(poly->x3);
                    break;
                }
                case 0x68:
                    // One-pixel tiles have no horizontal span to mirror.
                    break;
            }
            pPrim = (P_TAG *)getaddr(pPrim);
        }
    }
}

// Display_Viewport globals whose owners use small data.  These declarations
// preserve MAIN's target-proven absolute-addressing behavior.
#include "CAMERASELECTION.h"
#define BODYLISTS_DECLARE_ENVIRONMENTAL_OBJECT_LIST
#include "BODYLISTS.h"
#include "MECHLIST.h"
#define BODYLISTS_DECLARE_BACKGROUND_LIST
#include "BODYLISTS.h"

/**
 * Display_Viewport__FP9SViewporti
 *
 * Address:   0x80033C7C
 * Mangled:   Display_Viewport__FP9SViewporti
 * Demangled: Display_Viewport(SViewport *, int)
 *            Render one camera viewport and its environment, skaters,
 *            gameplay objects, effects, and background in ordering-table
 *            order.
 *
 * MAIN.SYM records the legacy `int` ABI; the target intentionally falls
 * through after rendering and leaves the final call's incidental `$v0`.
 *
 * @param pViewport  Projection and screen bounds for this render pass.
 * @param ViewportNumber  Camera and ordering-table slice used by the viewport.
 */
int Display_Viewport(SViewport *pViewport, int ViewportNumber)
{
    Camera_CurrentMikeCamera = ViewportNumber;

    MainDoubleBuffer *doubleBuffer =
        reinterpret_cast<MainDoubleBuffer *>(pDoubleBuffer);
    unsigned int *viewportOT =
        doubleBuffer->OrderingTable + ViewportNumber * 0x1000;
    doubleBuffer->ViewportOT = viewportOT;
    M3d_RenderSetup(
        &MikeCamera[ViewportNumber], pViewport, viewportOT);

    Mick_MapDoit = Mick_MapMode;
    M3d_Render((void *)EnviroList);
    Mick_MapDoit = 0;
    M3d_Render(EnvironmentalObjectList);

    OldSP = reinterpret_cast<void *>(SetSp(0x1F8003C0));
    GSkater[0]->TempTransform();
    if (GSkater[1] != 0)
    {
        GSkater[1]->TempTransform();
    }
    SetSp(reinterpret_cast<unsigned int>(OldSP));

    int t1 = M3d_OTPushback;
    MainViewportBruce *skater = (MainViewportBruce *)GSkater[0];
    int t2 = M3d_OTPushback2;
    M3d_OTPushback = -0x40;

    if (skater->mPhysicsState != 0 &&
        skater->mInBail == 0)
    {
        M3d_OTPushback2 = 0x80;
    }
    else
    {
        M3d_OTPushback2 = 0;
    }

    M3d_Render(MechList);

    OldSP = reinterpret_cast<void *>(SetSp(0x1F8003C0));
    GSkater[0]->TempRestore();
    if (GSkater[1] != 0)
    {
        GSkater[1]->TempRestore();
    }
    SetSp(reinterpret_cast<unsigned int>(OldSP));

    M3d_OTPushback = t1;
    M3d_OTPushback2 = t2;

    Bit_Display();
    M3d_Render(BaddyList);
    M3d_Render(PowerUpList);
    if (ViewportMode == 0)
    {
        M3d_RenderBackground(BackgroundList);
    }

    M3d_RenderCleanup();
}

/** Typed render-buffer globals used by the original PSY-Q primitive macros. */

/**
 * Draw_Rect__Fiiiiiiiii
 * Draws a flat-shaded quad (POLY_F4) primitive.
 * Params: x0,y0, x1,y1, x2,y2, x3,y3, color
 * x1,x2 get +1 and y1,y2 get +1 (PSX GPU exclusive endpoint convention)
 * Links the primitive into the ordering table via inline addPrim.
 *
 * Address:   0x80033E6C, Size: 208 bytes (52 instructions)
 * Mangled:   Draw_Rect__Fiiiiiiiii
 * Demangled: Draw_Rect(int, int, int, int, int, int, int, int, int)
 *
 * @param X0  First corner X coordinate.
 * @param Y0  First corner Y coordinate.
 * @param X1  Second corner X coordinate.
 * @param Y1  Second corner Y coordinate.
 * @param X2  Third corner X coordinate.
 * @param Y2  Third corner Y coordinate.
 * @param X3  Fourth corner X coordinate.
 * @param Y3  Fourth corner Y coordinate.
 * @param c  Packed primitive command and flat color.
 */
void Draw_Rect(int X0, int Y0, int X1, int Y1, int X2, int Y2, int X3, int Y3, int c)
{
    POLY_F4 *pF4 = (POLY_F4 *)pPoly;
    unsigned char *newppoly = (unsigned char *)(pF4 + 1);
    *(int *)&pF4->r0 = c;

    pF4->x0 = X0;
    pF4->y0 = Y0;
    pF4->x1 = X1 + 1;
    pF4->y1 = Y1;
    pF4->x3 = X2 + 1;
    pF4->y3 = Y2 + 1;
    pF4->x2 = X3;
    pF4->y2 = Y3 + 1;

    SetPolyF4(pF4);
    addPrim((P_TAG *)reinterpret_cast<MainDoubleBuffer *>(pDoubleBuffer)->OrderingTable,
            (P_TAG *)pPoly);
    pPoly = (unsigned int *)newppoly;
}

/**
 * BlackOut__Fiiii
 * Draws a black filled rectangle by calling Draw_Rect with the four corner
 * coordinates computed from (x, y, w, h) and a flag/color of 0x40000000.
 *
 * Address:   0x80033F3C, Size: 64 bytes (16 instructions)
 * Mangled:   BlackOut__Fiiii
 * Demangled: BlackOut(int, int, int, int)
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param w  Width.
 * @param h  Height.
 */
void BlackOut(int x, int y, int w, int h)
{
    int x2 = x + w;
    int y2 = y + h;
    Draw_Rect(x, y, x2, y, x2, y2, x, y2, 0x40000000);
}

/**
 * Display__Fv
 *
 * Address:   0x80033F7C
 * Mangled:   Display__Fv
 * Demangled: Display(void)
 *
 * Updates the active camera and viewport layout, draws the game and panel
 * overlays, and records the primitive-buffer usage for the frame.
 */
void Display(void)
{
    if (pAutoTestRestartName)
    {
        Mess_SetRGB(255, 0, 0, 0);
        Mess_SetScale(256);
        Mess_SetTextJustify(0);
        Mess_DrawText(
            256, 60, (const char *)pAutoTestRestartName, 0, 0, 256, false);
    }

    ApocalypseViewport.Zoom = static_cast<unsigned short>(
        reinterpret_cast<CCamera *>(CameraList)->GetZoom());

    if (!GameFrozen || Cheat_ScreenShots)
    {
        Ob_AI(&CameraList);
    }

    if (!Cheat_ScreenShots)
    {
        if (Pad[0].Select.Pressed)
        {
            if (Display_Released)
            {
                Display_Released = 0;
                if (GNumberOfPlayers == 2 && GGame != 7)
                {
                    ViewportMode++;
                    if (ViewportMode > 3)
                    {
                        ViewportMode = 0;
                    }
                }
                else
                {
                    CloseCam ^= 1;
                }
            }
        }
        else
        {
            Display_Released = 1;
        }

        if (Pad[1].Select.Triggered)
        {
            Pad[1].Select.Triggered = 0;
            ViewportMode++;
            if (ViewportMode > 3)
            {
                ViewportMode = 0;
            }
        }
    }

    if (GNumberOfPlayers == 2 && ViewportMode == 0 && GGame != 7)
    {
        ViewportMode = 1;
    }

    switch (ViewportMode)
    {
    case 0:
        if (!GameFrozen)
        {
            if (CloseCam)
            {
                camDist = 386;
                camPitch = 30;
                camYOffset = -160;
            }
            else
            {
                camDist = 290;
                camPitch = 150;
                camYOffset = -108;
            }
        }
        Display_Viewport(&ApocalypseViewport, 0);
        break;

    case 1:
        if (!GameFrozen)
        {
            camDist = 286;
            camPitch = 118;
            camYOffset = -140;
        }
        BlackOut(254, 0, 4, 240);
        Display_Viewport(&LeftViewport, 0);
        Display_Viewport(&RightViewport, 1);
        break;

    case 2:
        if (!GameFrozen)
        {
            camDist = 450;
            camPitch = 60;
            camYOffset = -120;
        }
        BlackOut(0, 118, 512, 4);
        Display_Viewport(&TopViewport, 0);
        Display_Viewport(&BottomViewport, 1);
        break;

    case 3:
        if (!GameFrozen)
        {
            camDist = 642;
            camPitch = 32;
            camYOffset = -172;
        }
        BlackOut(256, 0, 256, 120);
        BlackOut(0, 120, 256, 120);
        Display_Viewport(&TopLeftViewport, 0);
        Display_Viewport(&BottomRightViewport, 1);
        break;

    case 4:
        if (!GameFrozen)
        {
            camDist = 610;
            camPitch = 0;
            camYOffset = -200;
        }
        BlackOut(304, 0, 208, 88);
        BlackOut(0, 152, 208, 88);
        Display_Viewport(&TopLeftBigViewport, 0);
        Display_Viewport(&BottomRightBigViewport, 1);
        break;
    }

    if (Cheat_FlipScreen)
    {
        FlipPrimitives();
    }

    Font_Display();
    Panel_Display();

    if (!GSkater[1])
    {
        if (((MainDisplayBruce *)GSkater[0])->mBalanceMeterActive)
        {
            Panel_BalanceUD(
                ((MainDisplayBruce *)GSkater[0])->mBalanceValue,
                4000, 30, 200, 120, 255, 0xff00);
        }
    }
    else
    {
        if (((MainDisplayBruce *)GSkater[0])->mBalanceMeterActive)
        {
            Panel_BalanceUD(
                ((MainDisplayBruce *)GSkater[0])->mBalanceValue,
                4000, 25, 80, 120, 255, 0xff00);
        }
        if (((MainDisplayBruce *)GSkater[1])->mBalanceMeterActive)
        {
            Panel_BalanceUD(
                ((MainDisplayBruce *)GSkater[1])->mBalanceValue,
                4000, 25, 336, 120, 255, 0xff00);
        }
    }

    OldSP = reinterpret_cast<void *>(SetSp(0x1f8003c0));
    Front_Display();
    Mess_Display();
    SetSp(reinterpret_cast<unsigned int>(OldSP));

    if (GamePaused && Status != STATUS_STARTHORSE)
    {
        Mick_Darken();
    }
    else
    {
        Mick_Darkness = 0;
    }

    Flash_Display();

    int polyBufferStart = (int)reinterpret_cast<MainDoubleBuffer *>(pDoubleBuffer)->Polys;
    int polyBufferEnd = reinterpret_cast<int>(pPoly);
    PolyBufferBytesUsed =
        (polyBufferEnd & 0x7fffffff) -
        (polyBufferStart & 0x7fffffff);
}

/**
 * FPS_Init__Fv
 * Initializes FPS tracking: sets 3 frame counters to 2 and stores
 * current Xblanks - 2 into LastXblanks.
 *
 * Address:   0x800344E0, Size: 60 bytes (15 instructions)
 * Mangled:   FPS_Init__Fv
 * Demangled: FPS_Init(void)
 *
 * MAIN.SYM records the legacy `int` ABI; the target intentionally falls
 * through with the final `Xblanks - 2` value still in `$v0`.
 */
int FPS_Init(void)
{
    int i = 0;
    int value = 2;
    int *pFPS = &FPS;
    do
    {
        *pFPS = value;
        i++;
        pFPS++;
    } while (i < 3);
    LastXblanks = Xblanks - 2;
}

/**
 * FPS_Display__Fv
 *
 * Address:   0x8003451C
 * Mangled:   FPS_Display__Fv
 * Demangled: FPS_Display(void)
 *
 * Updates the game time scale from a rolling three-frame average and draws
 * the optional frame-rate diagnostic.
 */
int FPS_Display(void)
{
    if (!GamePaused)
    {
        (&FPS)[FPSIndex] = Xblanks - LastXblanks;
        FPSIndex++;
        if (FPSIndex == 3)
        {
            FPSIndex = 0;
        }
    }

    LastXblanks = Xblanks;

    int Total = 0;
    int i = 0;
    int *fps = &FPS;
    do
    {
        Total += *fps++;
        i++;
    } while (i < 3);

    int averageFPS = 60 * 3 / Total;
    if (Total > 5 * 3)
    {
        Total = 5 * 3;
    }

    if (!GamePaused)
    {
        TimeScale = 256 * Total / (2 * 3);
        if (Cheat_SlowMo)
        {
            TimeScale /= 4;
        }

        TimeScaleSquared = (TimeScale * TimeScale) >> 8;
        XFramesShifted += TimeScale * 2;
        XFrames = XFramesShifted >> 8;
    }

    if (Cheat_FPS)
    {
        static int FPS_Pulse asm("FPS_Pulse.335");
        FPS_Pulse += 10;
        FPS_Pulse &= 127;

        static char tmp1[100] asm("tmp1.336");
        sprintf(tmp1, Main_fpsFormat, averageFPS);
        if (!GameFrozen)
        {
            if (averageFPS >= 25)
            {
                Font_String(450, 210, tmp1, 50, 100, 50, 1);
            }
            else if (averageFPS >= 22)
            {
                Font_String(450, 210, tmp1, 100, 100, 50, 1);
            }
            else
            {
                Font_String(
                    450, 210, tmp1, 50 + FPS_Pulse, 50, 50, 1);
            }
            Font_Display();
        }
    }
}

/**
 * PlayAway_Init__Fv
 *
 * Address:   0x80034740
 * Mangled:   PlayAway_Init__Fv
 * Demangled: PlayAway_Init(void)
 */
#include "M3DCOLIJ.h"
void EnsureGTricksLoaded(void);
int PlayAway_Init(void)
{
    char buf1[16];
    char buf2[16];

    GClear1 = 0;
    GClear2 = 0;

    strcpy(buf1, Stats[SelectedSkater].Boards[GCareer.LastDeck[0]].BoardTexture);
    strcpy(buf2, Stats[SelectedSkater2].Boards[GCareer.LastDeck[1]].BoardTexture);

    {
        int i = 0;
        int separator = '_';
        char *p = &buf1[1];
        do
        {
            *p = separator;
            i++;
            p += 0x10;
        } while (i < 2);
    }

    GState = 0;
    ApocalypseViewport.TanHalf = 0xd55;
    LeftViewport.TanHalf = 0x8e3;
    RightViewport.TanHalf = 0x8e3;
    TopViewport.TanHalf = 0xd55;
    BottomViewport.TanHalf = 0xd55;
    TopLeftViewport.TanHalf = 0x8e3;
    BottomRightViewport.TanHalf = 0x8e3;
    TopLeftBigViewport.TanHalf = 0x8e3;
    BottomRightBigViewport.TanHalf = 0x8e3;
    XFramesShifted = 0;
    XFrames = 0;
    Vblanks = 0;
    Xblanks = 0;
    LastXblanks = Xblanks;
    TTime = 0;
    EveryOther = 0;

    ResetMainDemoTimer();
    InShell = 0;
    Pad_ClearAll();
    EndCode = ENDCODE_NONE;
    Utils_InitialRand(0x12a6cc58);
    M3dColij_FlushCache();
    EnsureGTricksLoaded();
    return Redbook_XAPlayRandom();
}

// ====== 0x800348DC SetSkater__FP6CBrucePc ======================================
// SetSkater__FP6CBrucePc
// Address: 0x800348DC
// Demangled: SetSkater(CBruce *, char *)
#include "CREATE.h"
/** CBruce prefix through the create-a-skater discriminator. */
struct MainSetSkaterBruce
{
    unsigned char pad_000[0xDA8];
    int mSkaterType;
};

typedef char Main_SetSkaterBruce_prefix_size_must_be_0xDAC[
    (sizeof(MainSetSkaterBruce) == 0xDAC) ? 1 : -1];
/**
 * Absolute-addressed call-site view of MAIN's `DefaultPSH` string.
 *
 * Keeping this declaration distinct from the small-data owner preserves the
 * original caller's address materialization without duplicating storage.
 */
extern char DefaultPSH[];
void SetSkater(CBruce *pBruce, char *filename)
{
    if (Create_UseSkater(
            reinterpret_cast<MainSetSkaterBruce *>(pBruce)->mSkaterType) != 0)
    {
        Create_BuildSkater(reinterpret_cast<CSuper *>(pBruce));
        return;
    }
    pBruce->InitItem(filename);
    char *PSH_AnimHierarchy[32];
    int mAnimPartCount = 0;
    char *PSH_ModelHierarchy[32];
    int mModelPartCount = 0;
    char *pData1 = Spool_LoadPSH(
        DefaultPSH, PSH_AnimHierarchy, &mAnimPartCount, 32);
    char *pData2 = Spool_LoadPSH(
        filename, PSH_ModelHierarchy, &mModelPartCount, 32);
    reinterpret_cast<CSuper *>(pBruce)->CalculateAnimOrder(
        PSH_AnimHierarchy, mAnimPartCount,
        PSH_ModelHierarchy, mModelPartCount);
    Mem_Delete(pData1);
    Mem_Delete(pData2);
}

/**
 * CreateSkatersAndCameras__Fv
 *
 * Address:   0x800349A4
 * Mangled:   CreateSkatersAndCameras__Fv
 * Demangled: CreateSkatersAndCameras(void)
 */
/** CBruce fields initialized while constructing the active skater list. */
struct MainCreationBruce
{
    unsigned char _padToExtraFlags[0x108];
    unsigned int mExtraFlags;
    unsigned char _padToCamera[0xA74 - 0x10C];
    MainCreationCamera *mpCamera;
    BoardStats *mpBoardStats;
    void *mIgnoreCar;
    MainCreationBruce *mpOtherPlayer;
    unsigned char _padToSkaterType[0xDA8 - 0xA84];
    int mSkaterType;
    int mIndex;
    unsigned char _remainingState[0x146C - 0xDB0];
};

typedef char MainCreationBruce_size_check[
    (sizeof(MainCreationBruce) == 0x146C) ? 1 : -1];
typedef char MainCreationCamera_size_check[
    (sizeof(MainCreationCamera) == 0x684) ? 1 : -1];

void CreateSkatersAndCameras(void)
{
    TrickTips_Init();
    int NumberOfOnscreenSkaters = 1;
    CBruce **skaters = GSkater;
    skaters[0] = 0;
    skaters[1] = 0;
    if (GNumberOfPlayers == 2 && GGame != 7)
        NumberOfOnscreenSkaters = 2;
    int i = 0;
    if (NumberOfOnscreenSkaters != 0)
    {
        int oneP = NumberOfOnscreenSkaters < 2;
        SStats *stats = Stats;
        char *multiplayerStats = (char *)&stats[0].PSXName2;
        BoardStats *bs = &GBoardStats;
        int cameraActive = 1;
        do
        {
            MainCreationBruce *pBruce =
                (MainCreationBruce *)::CItem::operator new(
                    sizeof(MainCreationBruce));
            if (pBruce != 0)
                pBruce = reinterpret_cast<MainCreationBruce *>(
                    CBruceConstructorABI::__6CBruce(
                        reinterpret_cast<CBruceCtorSlice *>(pBruce)));
            int selectedSkater = SelectedSkater;
            GSkater[i] = reinterpret_cast<CBruce *>(pBruce);
            pBruce->mIndex = i;
            pBruce->mSkaterType = selectedSkater;
            if (i > 0 && (GGame != 7 || GHorsePlayer != 0))
                pBruce->mSkaterType = SelectedSkater2;
            MainCreationCamera *pCam =
                (MainCreationCamera *)::CItem::operator new(
                    sizeof(MainCreationCamera));
            if (pCam != 0)
                pCam = ::new (pCam) CCamera(
                    reinterpret_cast<CBody *>(pBruce), 0);
            pCam->mCameraMode = cameraActive;
            pBruce->mpCamera = pCam;
            char *skaterFilename;
            if (!oneP)
                skaterFilename = *(char **)(
                    multiplayerStats + pBruce->mSkaterType * sizeof(SStats));
            else
                skaterFilename = stats[pBruce->mSkaterType].PSXName;
            SetSkater((CBruce *)pBruce, skaterFilename);
            if (stats[pBruce->mSkaterType].Ability[11] == 0)
                pBruce->mExtraFlags |= 2;
            pBruce->mpBoardStats = bs;
            reinterpret_cast<CBruce *>(pBruce)->GetStats(i);
            i++;
            bs++;
        } while (i < NumberOfOnscreenSkaters);
    }
    if (GNumberOfPlayers == 2)
    {
        ((MainCreationBruce *)GSkater[1])->mpOtherPlayer =
            (MainCreationBruce *)GSkater[0];
        ((MainCreationBruce *)GSkater[0])->mpOtherPlayer =
            (MainCreationBruce *)GSkater[1];
    }
    if (Levels[GLevel].isCompetition != 0 && GGame == 1 && GVideoRestart.NumSkaters == 0)
    {
        if (GVideoReplay == 0)
            ((MainCreationBruce *)GSkater[0])->mpCamera
                ->StartCompetitionIntro();
    }
}

#define VIDEO_HEAD_LAYOUT
#include "VIDEO.h"
#undef VIDEO_HEAD_LAYOUT

/**
 * Active replay descriptor.  MAIN only needs the block pointer, the frame
 * count, and the highlight table; the reserved runs keep the 192-byte stride.
 */
struct SVideoInfo
{
    int VideoMode;
    unsigned long *pBlockStart;
    char reserved_008[0x14];
    unsigned long NumFrames;
    char reserved_020[0x2C];
    int ReplayHighlight;
    SHighlight Highlights[5];
    char reserved_078[0x48];
};

typedef char Main_SVideoInfo_size_must_be_192[
    (sizeof(SVideoInfo) == 192) ? 1 : -1];

/** Powerup lifetime re-armed when a replay carries the S-K-A-T-E letters. */
struct MainGameInitPowerUp
{
    unsigned char pad_0000[0x130];
    unsigned short mLifetime;
};

/** Mech-list entry fields used while priming a replay playback. */
struct MainGameInitReplaySkater
{
    unsigned char pad_0000[0x1C];
    MainGameInitReplaySkater *mNextItem;
    unsigned char pad_0020[0xDB0 - 0x20];
    int mPlayTime;
};

/** Skater velocity/facing pair primed so the AI settles the start camera. */
struct MainGameInitSkater
{
    unsigned char pad_0000[0x54];
    int mVelX;
    int mVelY;
    int mVelZ;
    unsigned char pad_0060[0x10A0 - 0x60];
    int mFrontX;
    int mFrontY;
    int mFrontZ;
};

#include "LOCALTXT.h"
#include "DEMONAME.h"

void ResetOT(void);
void SetClippingBox(int x, int y, int w, int h);
void DrawColoredBox(int x, int y, int w, int h, unsigned char r,
                    unsigned char g, unsigned char b);

/**
 * Game_Init__Fv
 *
 * Address:   0x80034C70
 * Mangled:   Game_Init__Fv
 * Demangled: Game_Init(void)
 *
 * Brings a level up to its first logic frame.  Restarts the attract timer and
 * the crowd, then either reloads a replay (memory card while a demo is queued,
 * otherwise the next CD demo) or arms a fresh recording, re-arms the collectible
 * letters the saved video says are still available, and settles the camera by
 * running ten AI passes with the skater launched along its facing vector.  A
 * card failure paints the "load failed" prompt, waits for X or Start, and leaves
 * ENDCODE_LOADVIDEOFAILED so the main loop quits back to the front end.
 */
void Game_Init(void)
{
    int XStart;

    PlayAway_Init();
    XStart = Xblanks;
    Pause(1);
    FPS_Init();
    Crowd_Init();
    Crowd_Start();
    EndCode = ENDCODE_NONE;
    ResetMainDemoTimer();

    if (GVideoRestart.NumSkaters > 0)
    {
        if (DemoTimer != 0)
        {
            int LoadError;

            Front_SetLoadAction(LOAD_VIDEO);
            VblankUpdateBar = 1;
            if (GVideoRestart.NumSkaters == 1)
            {
                LoadError =
                    Card_Load(THPS_VIDEO_FILE, GVInfo1.pBlockStart, 0);
            }
            else
            {
                LoadError = Card_Load(
                    THPS_VIDEO_FILE, GVInfo1.pBlockStart, GVInfo2.pBlockStart);
            }
            VblankUpdateBar = 0;
            if (LoadError != 0)
            {
                Mess_SetCurrentFont(Main_gameFontName);
                HiRes_ClearOTag();
                ResetOT();
                SetClippingBox(0, 0, 0x200, 0xF0);
                DrawColoredBox(0, 0, 0x200, 0xF0, 0, 0, 0);
                Mess_SetTextJustify(0);
                Mess_SetScale(0xFF);
                Mess_SetRGB(0x96, 0x64, 0x19, 0);
                Mess_DrawText(
                    0x100, 0x6E, (const char *)GameText.pLoadFailed, 1, 0, 0x100, 0);
                Mess_DrawText(
                    0x100, 0x82, (const char *)GameText.pPressXToContinue, 1, 0, 0x100, 0);
                HiRes_DrawOTag();
                Pad_ClearTriggers(&Pad[0]);
                Pad_ClearTriggers(&Pad[1]);
                while (Pad[0].X.Triggered == 0 &&
                       Pad[1].X.Triggered == 0 &&
                       Pad[0].Start.Triggered == 0 &&
                       Pad[1].Start.Triggered == 0)
                {
                    Pad_Update();
                }
                EndCode = ENDCODE_LOADVIDEOFAILED;
                return;
            }
            {
                int i;
                SVideoHead *VH1 =
                    (SVideoHead *)((char *)GVInfo1.pBlockStart + 0x200);
                SHighlight *pDst;
                SHighlight *pSrc;

                i = 0;
                pDst = GVInfo1.Highlights;
                pSrc = VH1->Highlights;
                GVInfo1.NumFrames = VH1->NumFrames;
                do
                {
                    *pDst = *pSrc;
                    pDst++;
                    pSrc++;
                    i++;
                } while (i < 5);
            }
            if (GVideoRestart.NumSkaters == 2)
            {
                int i;
                SVideoHead *VH2 =
                    (SVideoHead *)((char *)GVInfo2.pBlockStart + 0x200);
                SHighlight *pDst;
                SHighlight *pSrc;

                i = 0;
                pDst = GVInfo2.Highlights;
                pSrc = VH2->Highlights;
                GVInfo2.NumFrames = VH2->NumFrames;
                do
                {
                    *pDst = *pSrc;
                    pDst++;
                    pSrc++;
                    i++;
                } while (i < 5);
            }
            GVideoReplay = -1;
            GVideoRestart.NumSkaters =
                -GVideoRestart.NumSkaters;
        }
    }

    if (DemoTimer == 0)
    {
        int LoadError;
        SVideoHead *VH;

        VblankUpdateBar = 1;
        LoadError = Card_LoadReplayFromCD(
            GVInfo1.pBlockStart, GVInfo2.pBlockStart, (char *)DemoName);
        DemoName[4]++;
        if (DemoName[4] > 0x4A)
        {
            DemoName[4] = 0x41;
        }
        VblankUpdateBar = 0;
        if (LoadError != 0)
        {
            EndCode = ENDCODE_LOADVIDEOFAILED;
            return;
        }
        VH = (SVideoHead *)((char *)GVInfo1.pBlockStart + 0x200);
        GVideoRestart.NumSkaters = VH->NumSkaters;
        GVideoRestart.Skater = VH->Skater;
        GVideoRestart.Skater2 = VH->Skater2;
        GVideoRestart.Level = VH->Level;
        GVideoRestart.GGame = VH->GGame;
        {
            int i;
            SHighlight *pDst;
            SHighlight *pSrc;

            i = 0;
            pDst = GVInfo1.Highlights;
            pSrc = VH->Highlights;
            GVInfo1.NumFrames = VH->NumFrames;
            do
            {
                *pDst = *pSrc;
                pDst++;
                pSrc++;
                i++;
            } while (i < 5);
        }
        if (GVideoRestart.NumSkaters == 2)
        {
            int i;
            SVideoHead *VH2 =
                (SVideoHead *)((char *)GVInfo2.pBlockStart + 0x200);
            SHighlight *pDst;
            SHighlight *pSrc;

            i = 0;
            pDst = GVInfo2.Highlights;
            pSrc = VH2->Highlights;
            GVInfo2.NumFrames = VH2->NumFrames;
            do
            {
                *pDst = *pSrc;
                pDst++;
                pSrc++;
                i++;
            } while (i < 5);
        }
        GVideoReplay = -1;
        GVideoRestart.NumSkaters = -GVideoRestart.NumSkaters;
    }

    if (GVideoReplay == 0)
    {
        if (GGame == 1)
        {
            GVideoFlags = 0;
            if (Career_Got(3) != 0)
            {
                GVideoFlags |= 1;
            }
            if (Career_Got(2) != 0)
            {
                GVideoFlags |= 2;
            }
        }
        else
        {
            GVideoFlags = 3;
        }
    }
    else
    {
        GVideoFlags =
            ((SVideoHead *)((char *)GVInfo1.pBlockStart + 0x200))->VideoFlags;
    }

    if (GGame == 1 && Levels[GLevel].isCompetition == 0)
    {
        if ((GVideoFlags & 2) != 0)
        {
            MainGameInitPowerUp *pS = (MainGameInitPowerUp *)GPowS;
            MainGameInitPowerUp *pK = (MainGameInitPowerUp *)GPowK;
            MainGameInitPowerUp *pA = (MainGameInitPowerUp *)GPowA;
            MainGameInitPowerUp *pT = (MainGameInitPowerUp *)GPowT;
            MainGameInitPowerUp *pE = (MainGameInitPowerUp *)GPowE;

            pS->mLifetime = 1;
            pK->mLifetime = 1;
            pA->mLifetime = 1;
            pT->mLifetime = 1;
            pE->mLifetime = 1;
        }
        if ((GVideoFlags & 1) != 0)
        {
            ((MainGameInitPowerUp *)GPowTape)->mLifetime = 1;
        }
    }

    if (GVideoReplay != 0)
    {
        MainGameInitReplaySkater *pSkater =
            (MainGameInitReplaySkater *)MechList;
        MainGameInitReplaySkater *pSkater2 = pSkater->mNextItem;

        if (GVideoReplay > 0)
        {
            GVInfo1.ReplayHighlight = GVideoReplay;
            GVInfo2.ReplayHighlight = GVideoReplay;
            Video_Start(
                reinterpret_cast<CBruce *>(pSkater), VIDEO_PLAYBACK,
                GVInfo1.Highlights[GVideoReplay - 1].FromFrame,
                GVInfo1.Highlights[GVideoReplay - 1].ToFrame);
            if (pSkater2 != 0)
            {
                Video_Start(
                    reinterpret_cast<CBruce *>(pSkater2), VIDEO_PLAYBACK,
                    GVInfo1.Highlights[GVideoReplay - 1].FromFrame,
                    GVInfo1.Highlights[GVideoReplay - 1].ToFrame);
            }
            Video_SyncObsIfNeeded();
        }
        else
        {
            GVideoReplay = 0;
            GVInfo1.ReplayHighlight = 0;
            GVInfo2.ReplayHighlight = 0;
            Video_Start(reinterpret_cast<CBruce *>(pSkater), VIDEO_PLAYBACK,
                        1, 0x7FFFFFFF);
            if (pSkater2 != 0)
            {
                Video_Start(reinterpret_cast<CBruce *>(pSkater2),
                            VIDEO_PLAYBACK, 1, 0x7FFFFFFF);
            }
        }
        GState = 0;
        Flash_Reset();
        {
            MainGameInitReplaySkater *pMech =
                (MainGameInitReplaySkater *)MechList;

            pMech->mPlayTime = Xblanks + 0x34BDE;
            if (pMech->mNextItem != 0)
            {
                pMech->mNextItem->mPlayTime = Xblanks + 0x34BDE;
            }
        }
    }
    else
    {
        MainGameInitReplaySkater *pSkater;
        MainGameInitReplaySkater *pSkater2;

        Video_Cleanup();
        pSkater = (MainGameInitReplaySkater *)MechList;
        pSkater2 = pSkater->mNextItem;
        if (GVInfo1.VideoMode == 0 &&
            reinterpret_cast<CCamera *>(CameraList)
                    ->InCompetitionIntro() == 0)
        {
            Video_Start(reinterpret_cast<CBruce *>(pSkater), VIDEO_RECORD,
                        1, 0x7FFFFFFF);
            if (pSkater2 != 0)
            {
                Video_Start(reinterpret_cast<CBruce *>(pSkater2), VIDEO_RECORD,
                            1, 0x7FFFFFFF);
            }
            ((SVideoHead *)((char *)GVInfo1.pBlockStart + 0x200))->VideoFlags =
                GVideoFlags;
        }
    }

    XStart = Xblanks;
    if (GNumberOfPlayers == 1)
    {
        PadEnableCom(1);
    }
    ((MainGameInitSkater *)GSkater[0])->mVelX =
        ((MainGameInitSkater *)GSkater[0])->mFrontX * 20;
    ((MainGameInitSkater *)GSkater[0])->mVelY =
        ((MainGameInitSkater *)GSkater[0])->mFrontY * 20;
    ((MainGameInitSkater *)GSkater[0])->mVelZ =
        ((MainGameInitSkater *)GSkater[0])->mFrontZ * 20;
    {
        int i;

        for (i = 0; i < 10; i++)
        {
            Ob_AI(&CameraList);
        }
    }
    ((MainGameInitSkater *)GSkater[0])->mVelX = 0;
    ((MainGameInitSkater *)GSkater[0])->mVelY = 0;
    ((MainGameInitSkater *)GSkater[0])->mVelZ = 0;
    GameFade = 0;
    (void)XStart;
}

/**
 * System_Logic__Fv
 *
 * Address:   0x80035450
 * Mangled:   System_Logic__Fv
 * Demangled: System_Logic(void)
 *
 * @return 0
 */
void System_Logic(void)
{
    if (GGame == 7 && GHorsePlayer == 1)
    {
        SControl temp = Pad[0];
        Pad[0] = Pad[1];
        Pad[1] = temp;
    }
    Pad_Restore();
    Pad_Update();
    Pad_Remap();
    if (GGame == 7 && GHorsePlayer == 1)
    {
        SControl temp = Pad[0];
        Pad[0] = Pad[1];
        Pad[1] = temp;
    }
    if (GClear1 != 0)
        Pad_ClearAllOne(0);
    if (GClear2 != 0)
        Pad_ClearAllOne(1);
}

/**
 * CheckEndGame__Fv
 *
 * Address:   0x800356DC
 * Mangled:   CheckEndGame__Fv
 * Demangled: CheckEndGame(void)
 *
 * @return 0
 */
#include "SFX.h"

/** Camera prefix through the frontend skater's extra yaw accumulator. */
struct MainLogicCamera
{
    unsigned char pad_000[0x5C4];
    int extra_YRot;
};

/**
 * CBruce slice shared by MAIN's end-of-game and input-clear paths.
 * Field names and offsets come from the target's CBruce definition.
 */
struct MainLogicSkater
{
    unsigned char pad_000[0x2D0];
    unsigned int mLandedTotal;
    unsigned char pad_2D4[0x7A0];
    MainLogicCamera *mpCamera;
    unsigned char pad_A78[0x398];
    int mManual;
    unsigned char pad_E14[0x22C];
    unsigned int mSFXWallride;
    unsigned int mSFXGrind;
    unsigned int mSFXGround;
    unsigned char pad_104C[0x20];
    int mPhysicsState;
};

typedef char MainLogicCamera_prefix_size_must_be_0x5C8[
    (sizeof(MainLogicCamera) == 0x5C8) ? 1 : -1];
typedef char MainLogicSkater_prefix_size_must_be_0x1070[
    (sizeof(MainLogicSkater) == 0x1070) ? 1 : -1];
void CheckEndGame(void)
{
    MainLogicSkater *p0 = reinterpret_cast<MainLogicSkater *>(GSkater[0]);
    MainLogicSkater *p1;
    if ((((p0->mPhysicsState == 0 && p0->mManual == 0) ||
          p0->mPhysicsState == 7)
         && (((p1 = reinterpret_cast<MainLogicSkater *>(GSkater[1])) == 0
              || (p1->mPhysicsState == 0 && p1->mManual == 0))
             || p1->mPhysicsState == 7))
        && (GState == 1 && (GSpin = GSpin - 1, GSpin < 1)))
    {
        if (GGame != 7 &&
            reinterpret_cast<MainLogicSkater *>(GSkater[0])->mLandedTotal != 0)
        {
            GSpinSpeed = 0;
            GSpin = GState;
            return;
        }

        MainLogicSkater *b0 =
            reinterpret_cast<MainLogicSkater *>(GSkater[0]);
        if (b0->mSFXGrind != 0)
        {
            SFX_Stop(b0->mSFXWallride);
            reinterpret_cast<MainLogicSkater *>(GSkater[0])->mSFXGrind = 0;
            if (reinterpret_cast<MainLogicSkater *>(GSkater[0])->mSFXGrind != 0)
            {
                SFX_Stop(
                    reinterpret_cast<MainLogicSkater *>(GSkater[0])->mSFXGrind);
                reinterpret_cast<MainLogicSkater *>(GSkater[0])->mSFXGrind = 0;
            }
        }
        if (reinterpret_cast<MainLogicSkater *>(GSkater[0])->mSFXGround != 0)
        {
            SFX_Stop(
                reinterpret_cast<MainLogicSkater *>(GSkater[0])->mSFXGround);
            reinterpret_cast<MainLogicSkater *>(GSkater[0])->mSFXGround = 0;
        }
        if (GSkater[1] != 0)
        {
            if (reinterpret_cast<MainLogicSkater *>(GSkater[1])->mSFXGrind != 0)
            {
                SFX_Stop(
                    reinterpret_cast<MainLogicSkater *>(GSkater[1])->mSFXGrind);
                reinterpret_cast<MainLogicSkater *>(GSkater[1])->mSFXGrind = 0;
            }
            if (GSkater[1] != 0 &&
                reinterpret_cast<MainLogicSkater *>(GSkater[1])->mSFXGround != 0)
            {
                SFX_Stop(
                    reinterpret_cast<MainLogicSkater *>(GSkater[1])->mSFXGround);
                reinterpret_cast<MainLogicSkater *>(GSkater[1])->mSFXGround = 0;
            }
        }
        GState = 0;
        if (Levels[GLevel].isCompetition != 0 && GGame == 1)
            Front_SwitchOnEndHeat();
        else
            Front_SwitchOnEndLevelMenu();
    }
}

/**
 * CheckSkatersStop__Fv
 *
 * Address:   0x80035920
 * Mangled:   CheckSkatersStop__Fv
 * Demangled: CheckSkatersStop(void)
 *
 */
void CheckSkatersStop(void)
{
    int state = GState;
    if (state != 1)
        return;
    if (GamePaused != 0)
        return;

    MainLogicSkater *player0 =
        reinterpret_cast<MainLogicSkater *>(GSkater[0]);
    if (player0->mPhysicsState == 0)
    {
        if (player0->mManual == 0)
        {
            int t = Pad[0].Circle.Pressed;
            Pad_ClearAllOne(0);
            Pad[0].Down.Pressed = static_cast<unsigned char>(state);
            Pad[0].Circle.Pressed = static_cast<unsigned char>(t);
        }
    }

    if (GNumberOfPlayers != 2)
        return;

    MainLogicSkater *player1 =
        reinterpret_cast<MainLogicSkater *>(GSkater[1]);
    if (player1->mPhysicsState != 0)
        return;
    if (player1->mManual != 0)
        return;

    int t = Pad[1].Circle.Pressed;
    Pad_ClearAllOne(1);
    Pad[1].Down.Pressed = 1;
    Pad[1].Circle.Pressed = static_cast<unsigned char>(t);
}

/**
 * Game_Logic__Fv
 *
 * Address:   0x80035A08
 * Mangled:   Game_Logic__Fv
 * Demangled: Game_Logic(void)
 *
 */
void Bit_Move(void);
void Ob_MaybeUnSuspendOrCull(void);
void Bit_RemoveDeadBits(void);
void Game_Logic(void)
{
    CheckEndGame();
    Crowd_Update();
    CalcPolyBufferEnd();
    Video_HandleStepping();
    CheckSkatersStop();

    if ((GNumberOfPlayers == 1 || (GGame == 7 && GHorseClearInput2 != 0)) &&
        Cheat_ScreenShots == 0)
    {
        /* Preserve player 2's Select across the single-player input clear.
         * The original saves and restores INSIDE the conditional
         * (MAIN.CPP:3857-3859: lbu / jal / sb around Pad_ClearAllOne). */
        unsigned char select = Pad[1].Select.Pressed;
        Pad_ClearAllOne(1);
        Pad[1].Select.Pressed = select;
    }
    TTime = TTime + 1;

    Font_Reset();

    int Frozen = 0;
    if (GamePaused != 0 || GameFrozen != 0)
        Frozen = 1;

    if (!Frozen)
    {
        Flash_Update();
        Utils_ClearExplosionStack();
        Ob_ClearInputFlags(BaddyList);
        Ob_ClearInputFlags(EnvironmentalObjectList);
        Ob_AI(&MechList);
        if (GamePaused == 0)
        {
            Bit_Move();
            Rail_Update();
            Ob_AI(&PowerUpList);
            TrickOb_Update();
            Ob_QuickAI(&EnvironmentalObjectList);
            Ob_QuickAI(&BackgroundList);
            Ob_MaybeUnSuspendOrCull();
            Ob_AI(&BaddyList);
            Bit_RemoveDeadBits();

            /* Constant slow spin on the frontend skater display. */
            if (GGame != 7 && GState == 1 && GSpin < 0x28)
            {
                if ((reinterpret_cast<MainLogicSkater *>(GSkater[0])
                             ->mPhysicsState == 0 &&
                     reinterpret_cast<MainLogicSkater *>(GSkater[0])
                             ->mManual == 0) ||
                    reinterpret_cast<MainLogicSkater *>(GSkater[0])
                            ->mPhysicsState == 7)
                {
                    if (GSkater[1] == 0 ||
                        (reinterpret_cast<MainLogicSkater *>(GSkater[1])
                                 ->mPhysicsState == 0 &&
                         reinterpret_cast<MainLogicSkater *>(GSkater[1])
                                 ->mManual == 0) ||
                        reinterpret_cast<MainLogicSkater *>(GSkater[1])
                                ->mPhysicsState == 7)
                    {
                        int spin = GSpinSpeed;
                        reinterpret_cast<MainLogicSkater *>(GSkater[0])
                            ->mpCamera->extra_YRot += spin;
                        if (GSkater[1] != 0)
                        {
                            reinterpret_cast<MainLogicSkater *>(GSkater[1])
                                ->mpCamera->extra_YRot += spin;
                        }
                    }
                }
            }
        }
    }
    else
    {
        if (GState == 1)
            Ob_AI(&CameraList);
    }

    Shatter_MaybeMakeGlassShatterSound();
    Panel_Update();

    /* HORSE hands the front end player 1's pad: rotate Pad[0] -> TempPad,
     * Pad[1] -> Pad[0], TempPad -> Pad[1] around the update, and back after. */
    if (GGame == 7 && GHorsePlayer == 1)
    {
        TempPad = Pad[0];
        Pad[0] = Pad[1];
        Pad[1] = TempPad;
    }
    Pad_Restore();
    Front_Update();
    Pad_Remap();
    if (GGame == 7 && GHorsePlayer == 1)
    {
        TempPad = Pad[0];
        Pad[0] = Pad[1];
        Pad[1] = TempPad;
    }

    TrickTips_Update();
    Mess_Update();
    Spool_Continue();
    FileIO_Continue();

    if (GGame == 7)
    {
        if (GHorseWordChosen == 0)
        {
            GHorseDoneStart = 1;
            GHorseWordChosen = 1;
            Front_PrintHorseLetters();
        }
        else if (GHorseDoneStart == 0)
        {
            GHorseDoneStart = 1;
            Front_SwitchOnStartLevelMenu();
        }
    }

    if (MechList->IsDead() != 0)
        EndCode = ENDCODE_BRUCEDIED;
}

/**
 * Game_Display__Fv
 *
 * Address:   0x80035F9C
 * Mangled:   Game_Display__Fv
 * Demangled: Game_Display(void)
 *
 */
void Game_Display(void)
{
    FPS_Display();
    Display();
    if (GameFrozen == 0 && Cheat_FPS != 0)
    {
        Panel_DrawFlatShadedPoly(0x1C0, 0xD0, 0x36, 0x12, 0, 0, 0, 0);
    }
    if (DemoTimer == 0)
    {
        Front_PrintDemo();
    }
    int hasVideo = Levels[GLevel].FMV;
    int arg;
    if (hasVideo != 0 && (GNumberOfPlayers == 1 || GGame == 7))
    {
        arg = GModelVideoDisplayed;
    }
    else
    {
        arg = 0;
    }
    GameFMV_SyncFMV(arg);
    Redbook_XAUpdate();
    GModelVideoDisplayed = 0;
}

/**
 * MainLoop_End__Fv
 *
 * Address:   0x800360A0, Size: 112 bytes (28 instructions)
 * Mangled:   MainLoop_End__Fv
 * Demangled: MainLoop_End(void)
 *            Keeps the frame loop aligned to the original two-vblank cadence.
 *            Synchronizes frame timing: pauses if Vblanks matches GStartTime, renders,
 *            pauses if 1-behind, updates GStartTime, polls host.
 *
 */
int MainLoop_End(void)
{
    if (Vblanks == GStartTime)
    {
        Pause(1);
    }
    Db_Render();
    if (Vblanks == GStartTime + 1)
    {
        Pause(1);
    }
    int currentVblank = Vblanks;
    GStartTime = currentVblank;
    PollHost();
}

/**
 * PlayAway__Fv
 *
 * Address:   0x80036110
 * Mangled:   PlayAway__Fv
 * Demangled: PlayAway(void)
 *
 */
// Runs the main gameplay loop until EndCode requests shutdown.
void PlayAway(void)
{
    Game_Init();
    int vb = Vblanks;
    int ec = EndCode;
    MemDumpLoop = 0;
    GStartTime = vb;
    if (ec != ENDCODE_NONE) goto done;
loop:
    System_Logic();
    Game_Logic();
    if (EndCode != ENDCODE_NONE) goto done;
    Db_WaitRender();
    Game_Display();
    MainLoop_End();
    if (EndCode == ENDCODE_NONE) goto loop;
done:
    UnloadGTricks();
    PadEnableCom(3);
    Spool_Sync();
    M3d_OTPushback = 1;
    Db_WaitRender();
    DrawSync(0);
    Db_Init();
    Crowd_Stop();
}

/**
 * Initializes the game systems, plays the startup movies, and enters the
 * front end before displaying the high-resolution title screen.
 *
 * Address: 0x800361D8
 * Mangled: main
 *
 * @return 0 after the title-screen flow finishes
 */
int main(void)
{
    printf(MainAddressViewOwners::s_xxx_main_800d976c);
    printf(DAT_800d9778);
    Options_Init();
    printf(DAT_800d977c);
    Career_Init();
    printf(DAT_800d9780);
    Init_AtStart();
    printf(DAT_800d9784);
    Init_Restart(PACK512x240, 16);
    printf(DAT_800d9788);
    Db_Init();

    Pad_Update();
    if (!Pad[0].Start.Pressed &&
        (!Pad[0].LeftOne.Pressed || !Pad[0].LeftTwo.Pressed))
    {
        GameFMV_PlayMovie(-1);
    }

    Pad_Update();
    if (!Pad[0].Start.Pressed &&
        (!Pad[0].LeftOne.Pressed || !Pad[0].LeftTwo.Pressed))
    {
        GameFMV_PlayMovie(-2);
    }

    Pad_Update();
    if (!Pad[0].Start.Pressed &&
        (!Pad[0].LeftOne.Pressed || !Pad[0].LeftTwo.Pressed))
    {
        GameFMV_PlayMovie(-25);
    }

    Front_DisplayScreen(MainAddressViewOwners::s_s2bgd0_bmp_800d978c);

    Pad_Update();
    int i = 0;
    Pad[0].Start.Triggered = 0;
    Pad[0].X.Triggered = 0;
    Pad[1].Start.Triggered = 0;
    Pad[1].X.Triggered = 0;

    while (true)
    {
        Pause(1);
        Pad_Update();
        if (Pad[0].Start.Triggered ||
            Pad[0].X.Triggered ||
            Pad[1].Start.Triggered ||
            Pad[1].X.Triggered)
        {
            break;
        }
        if (++i >= 120)
        {
            break;
        }
    }

    Pad[0].Start.Triggered = 0;
    Pad[0].X.Triggered = 0;
    Pad[1].Start.Triggered = 0;
    Pad[1].X.Triggered = 0;

    FrontEnd2_Main();

    Options_Init();
    Career_Init();
    Init_AtStart();
    Init_Restart(PACK512x240, 16);
    Db_Init();

    Pad_Update();
    if (!Pad[0].Start.Pressed &&
        (!Pad[0].LeftOne.Pressed || !Pad[0].LeftTwo.Pressed))
    {
        GameFMV_PlayMovie(-1);
    }

    Pad_Update();
    if (!Pad[0].Start.Pressed &&
        (!Pad[0].LeftOne.Pressed || !Pad[0].LeftTwo.Pressed))
    {
        GameFMV_PlayMovie(-2);
    }

    Pad_Update();
    if (!Pad[0].Start.Pressed &&
        (!Pad[0].LeftOne.Pressed || !Pad[0].LeftTwo.Pressed))
    {
        GameFMV_PlayMovie(-25);
    }

    Init_Restart(PACK512x240, 16);
    Db_Init();
    GNumberOfPlayers = 1;
    Init_Restart(PACK640x480, 16);
    HiRes_Init(true);
    HiRes_TitleScreen();

    return 0;
}

/**
 * LaunchGame__Fv
 *
 * Address:   0x8003651C
 * Mangled:   LaunchGame__Fv
 * Demangled: LaunchGame(void)
 *
 * @return 0
 */
int LaunchGame(void)
{
    int Result = 0;

    if (Pad[0].RightOne.Pressed)
    {
        GGame = 8;
        GNumberOfPlayers = 2;
    }

    Pack_Init();
    Init_Restart(PACK512x240, 16);

    SSaveGame tmp;
    tmp = StartupGameState;
    Init_ForGame();
    StartupGameState = tmp;
    Front_SetPreferences(StartupGameState.Preferences);

    switch (Result)
    {
    case 0:
        LoadingScreen_Main();
        if (GAutoTest)
        {
            DumpAutoTest();
        }
        Front_LoadGame(&StartupGameState);
        break;
    }

    if (BackOut)
    {
        if (DemoTimer)
        {
            StartScreen = 1;
        }
        else
        {
            StartScreen = 4;
        }
        BackOut = 0;
        return 0;
    }

    if (GGame == 7)
    {
        Horse_Reset();
    }

play_game:
    Pad[0].X.Pressed = 1;
    Pad[0].X.Triggered = 0;
    Pad[1].X.Pressed = 1;
    Pad[1].X.Triggered = 0;

    CloseCam = 0;
    ViewportMode = 0;
    if (GNumberOfPlayers == 2 && GGame != 7)
    {
        ViewportMode = 1;
    }

    PlayAway();
    GClear1 = 0;
    GClear2 = 0;
    GHorseDoneStart = 0;

    switch (EndCode)
    {
    case ENDCODE_TWEAKJUMP:
        printf(Main_endCodeTweakJumpMessage);
        Init_Cleanup(GenerateLevel ? 2 : 1);
        Trig_ParseTRGFile();
        Rail_CreateRails();
        CreateSkatersAndCameras();
        Trig_ExecuteRestart();
        goto play_game;

    default:
        printf(Main_notEndCodeTweakJumpMessage);
        Panel_Uninit();
        Init_Restart(PACK512x240, 16);
        Db_Init();
        GNumberOfPlayers = 1;
        Front_DisplayScreen(MainAddressViewOwners::s_s2market_bmp_800d9798);
        Pad_Update();

        Pad[0].Start.Triggered = 0;
        Pad[0].X.Triggered = 0;
        Pad[1].Start.Triggered = 0;
        Pad[1].X.Triggered = 0;

        int i = 0;
        while (i < 480)
        {
            Pause(1);
            Pad_Update();
            if (Pad[0].Start.Triggered ||
                Pad[0].X.Triggered ||
                Pad[1].Start.Triggered ||
                Pad[1].X.Triggered)
            {
                break;
            }
            ++i;
        }

        Pad[0].Start.Triggered = 0;
        Pad[0].X.Triggered = 0;
        Pad[1].Start.Triggered = 0;
        Pad[1].X.Triggered = 0;
    }

    return 0;
}

    /** Uppercase digits used by the checksum/debug formatting helpers. */
    char HexDigits[17] __attribute__((section(".data"))) =
        "ABCFHJKLMNPSTVXY";

    /** Short controller scripts used by the attract-mode demo. */
    SDemo pLittleDemo[4] __attribute__((section(".data"))) =
    {
        {5, 5, 10, 1},
        {5, 5, 0, 1},
        {5, 5, 2, 1},
        {10, 0xffff, 0, 0},
    };

    /** Full-screen viewport used by the Apocalypse camera mode. */
    SViewport ApocalypseViewport __attribute__((section(".data"))) =
        {0x200, 0xf0, 0, 0, 10, 30000, 0, 0, 0, 0};

    /** Left half of the two-player horizontal layout. */
    SViewport LeftViewport __attribute__((section(".data"))) =
        {0xfe, 0xf0, 0, 0, 10, 30000, 0, 0, 0, 0};

    /** Right half of the two-player horizontal layout. */
    SViewport RightViewport __attribute__((section(".data"))) =
        {0x200, 0xf0, 0x102, 0, 10, 30000, 0, 0, 0, 0};

    /** Top half of the two-player vertical layout. */
    SViewport TopViewport __attribute__((section(".data"))) =
        {0x200, 0x76, 0, 0, 10, 30000, 0, 0, 0, 0};

    /** Bottom half of the two-player vertical layout. */
    SViewport BottomViewport __attribute__((section(".data"))) =
        {0x200, 0xf0, 0, 0x7a, 10, 30000, 0, 0, 0, 0};

    /** Top-left quadrant of the four-way layout. */
    SViewport TopLeftViewport __attribute__((section(".data"))) =
        {0x100, 0x78, 0, 0, 10, 30000, 0, 0, 0, 0};

    /** Bottom-right quadrant of the four-way layout. */
    SViewport BottomRightViewport __attribute__((section(".data"))) =
        {0x200, 0xf0, 0x100, 0x78, 10, 30000, 0, 0, 0, 0};

    /** Enlarged top-left viewport used by asymmetric layouts. */
    SViewport TopLeftBigViewport __attribute__((section(".data"))) =
        {0x130, 0x98, 0, 0, 10, 30000, 0xd55, 0, 0, 0};

    /** Small bottom-right viewport paired with TopLeftBigViewport. */
    SViewport BottomRightBigViewport __attribute__((section(".data"))) =
        {0x200, 0xf0, 0xd0, 0x58, 10, 30000, 0, 0, 0, 0};

    /** Persistent career/save state, zeroed at startup. */
    SCareer GCareer __attribute__((section(".data"))) = {};

    /** Temporary controller state used while remapping or restoring input. */
    SControl TempPad __attribute__((section(".data"))) = {};


/**
 * Starts an animation over an explicit frame range with the default playback
 * speed and no transition.
 */


/**
 * Starts an animation over an explicit frame range with the default playback
 * speed and a caller-selected transition.
 */
