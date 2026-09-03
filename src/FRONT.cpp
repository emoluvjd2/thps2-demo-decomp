// FRONT.cpp -- Front-end menus, pause flow, and level-loading coordination.
// Compiled with the original GCC 2.91.66 PSX toolchain.

#include <STDDEF.H>
#include <LIBGTE.H>
#define FRONT_OWNER_DECLARATIONS
#include "FRONT.h"
#undef FRONT_OWNER_DECLARATIONS
#include "M3DINIT.h"
#include "INIT.h"
#include "LEVELEDITOR_STATE.h"
#include "PHYSICS.h"
#include "RFRONT.h"
#include "VIDEO.h"
#include "STATS.h"

    // Primitive types
    typedef unsigned char u8;
    typedef unsigned short u16;
    typedef unsigned int u32;
    typedef signed char s8;
    typedef signed short s16;
    typedef signed int s32;
    typedef unsigned char uchar;
    typedef unsigned short ushort;
    typedef unsigned int uint;
    typedef unsigned char byte;
    typedef int undefined;
    typedef char undefined1;
    typedef short undefined2;
    typedef int undefined4;
    typedef unsigned long ulong;
    typedef unsigned char undefined3[3];

    // Small front-end records are defined below or in FRONT.cpp's owner-local layout block.

    // Forward declarations for large/opaque types
    struct SAnimFrame;
    struct CBody;
    struct CSuper;
    struct EditorOutModel;
    struct EditorInModel;
    struct LevelGenerator;
    struct CBruce;
    struct CBaddy;
    struct SModel;
    struct SHook;
    struct CPowerUp;
    struct GamePreferences;
    struct SCareer;
    struct SVideoInfo;
    struct SDoubleBuffer;
    struct CMenu;
    struct Sprite;

    // Save-game and front-end preference records are defined below.

    // Front-end memcard enums and local helper layout
    enum EMemCardState
    {
        SVSTATUS_CHECKINGCARDPAUSE = 0,
        SVSTATUS_CHECKINGCARD = 1,
        SVSTATUS_ENTERNAME = 2,
        SVSTATUS_OKTOOVERWRITE = 3,
        SVSTATUS_SAVING = 4,
        SVSTATUS_SAVING2 = 5,
        SVSTATUS_SAVEFAILED = 6,
        SVSTATUS_SAVECOMPLETED = 7,
        SVSTATUS_CARDFULL = 8,
        SVSTATUS_INSERTCARD = 9,
        SVSTATUS_UNFORMATTEDCARD = 10,
        SVSTATUS_FORMATTING = 11,
        SVSTATUS_FORMATFAILED = 12,
        SVSTATUS_FORMATSUCCEEDED = 13,
        SVSTATUS_DELETEQUERY = 14,
        LVSTATUS_CHECKINGCARDPAUSE = 15,
        LVSTATUS_CHECKINGCARD = 16,
        LVSTATUS_INSERTCARD = 17,
        LVSTATUS_UNFORMATTEDCARD = 18,
        LVSTATUS_NOFILES = 19,
        LVSTATUS_SELECTFILE = 20,
        LVSTATUS_LOADING = 21,
        LVSTATUS_LOADFAILED = 22,
        LVSTATUS_LOADCOMPLETED = 23,
        LVSTATUS_FULLCARDCHECK = 24,
        LVSTATUS_FULLCARDWARNING = 25,
        LVSTATUS_HIRESREFRESH1 = 26,
        LVSTATUS_HIRESREFRESH2 = 27,
        LVSTATUS_HIRESREFRESH3 = 28,
        LVSTATUS_BUILDMENU = 29,
        LCSTATUS_CHECKINGCARDPAUSE = 30,
        LCSTATUS_CHECKINGCARD = 31,
        LCSTATUS_INSERTCARD = 32,
        LCSTATUS_NOFILES = 33,
        LCSTATUS_SELECTFILE = 34,
        LCSTATUS_LOADING = 35,
        LCSTATUS_LOADFAILED = 36,
        LCSTATUS_LOADCOMPLETED = 37,
    };

    enum EPausedStatus
    {
        STATUS_NORMAL = 0,
        STATUS_PAUSEDMENU = 1,
        STATUS_SKIPTORESTART = 2,
        STATUS_REALLYQUIT = 3,
        STATUS_PADUNPLUGGED = 4,
        STATUS_SAVEGAME = 5,
        STATUS_QUITSAVE = 6,
        STATUS_MEDALSAVE = 7,
        STATUS_ENDLEVELFADESCREEN = 8,
        STATUS_DOSAVEFADESCREEN = 9,
        STATUS_NOSAVEFADESCREEN = 10,
        STATUS_GOTTAPE = 11,
        STATUS_TAPESAVE = 12,
        STATUS_STATS = 13,
        STATUS_ENDINGSCREEN = 14,
        STATUS_VIDEOSCREEN = 15,
        STATUS_VIDEOSCREEN2 = 16,
        STATUS_REALLYMAINMENU = 17,
        STATUS_MEMCARDMENUS = 18,
        STATUS_ENDHEAT = 19,
        STATUS_ENDHEATREPLAY = 20,
        STATUS_ENDHEATJUDGES = 21,
        STATUS_ENDHEATLADDER = 22,
        STATUS_ENDHEATWINLOSE = 23,
        STATUS_ENDHEATMENU = 24,
        STATUS_ENDHEATVIDEOMENU = 25,
        STATUS_BEGINHEAT = 26,
        STATUS_MPC_BEGIN = 27,
        STATUS_MPC_BEGINHEAT = 28,
        STATUS_MPC_JUDGES = 29,
        STATUS_MPC_LADDER = 30,
        STATUS_MPC_LADDERFINAL = 31,
        STATUS_MPC_SWITCH = 32,
        STATUS_MPC_ENDHEAT = 33,
        STATUS_MPC_WINNER = 34,
        STATUS_ENDGAME = 35,
        STATUS_ENDGAMEALLGOLD = 36,
        STATUS_ENDGAMEPLAYERMOVIE = 37,
        STATUS_ENDGAMEBAILMOVIE = 38,
        STATUS_ENDGAMESAVE = 39,
        STATUS_ENDGAMECREDITS = 40,
        STATUS_STARTHORSE = 41,
        STATUS_CHANGEHORSEWORD = 42,
        STATUS_LOADSUCCESS = 43,
        STATUS_LOADFAILED = 44,
        STATUS_HIGHSCORE = 45,
        STATUS_SETSOUNDLEVELS = 46,
        STATUS_DUMMY = 47,
    };

    struct SRECT
    {
        short x;
        short w;
        short y;
        short h;
    };

#include "STARTUPGAMESTATE.h"

struct SFrontBackground {
    char *pLibName;
    char *pEnvName;
    char *pSizesName;
    VECTOR Focus;
    int HorizDist;
    int Altitude;
};

// === Variables (gp-relative in this TU) ===
#define FRONTFLOWSTATE_OWNER_DECLARATION
#define FRONTFLOWSTATE_DECLARE_BACKOUT
#include "FRONTFLOWSTATE.h"
#define DEMOTIMER_OWNER_DECLARATION
#include "DEMOTIMER.h"
#define FRONTFLOWSTATE_OWNER_DECLARATION
#define FRONTFLOWSTATE_DECLARE_ROLL_CREDITS
#include "FRONTFLOWSTATE.h"
#define SOUNDLEVELS_OWNER_DECLARATIONS
#define SOUNDLEVELS_DECLARE_SFXLEVEL
#include "SOUNDLEVELS.h"
#define FRONTSTATUS_OWNER_DECLARATION
#include "FRONTSTATUS.h"
#define SOUNDLEVELS_DECLARE_XALEVEL
#include "SOUNDLEVELS.h"
#undef SOUNDLEVELS_OWNER_DECLARATIONS
#define YESNOMENU_OWNER_DECLARATION
#include "YESNOMENU.h"

// === Variables (absolute in this TU) ===
#define FRONTCHEATS_ABSOLUTE_DECLARATION
#include "FRONTCHEATS.h"
#include "CURRENTGAMELEVEL.h"
// MAIN owns the shared GBailsList pointer.
#include "GLEVEL.h"
#define VIDEOINFO_DECLARE_GVINFO1
#define VIDEOINFO_DECLARE_GVINFO2
#include "VIDEOINFO.h"
#include "GENERATELEVEL.h"
#include "OTENTRY.h"
// CREDITS owns the TU-local SKATER_Y_POS word.
#include "PDOUBLEBUFFER.h"
#include "PAUSEDSPRITE.h"


#include "globals.h"
#include "CMENU.h"
#include "FONTTOOLS.h"
#include "TEXTEFFECTS.h"
#include "PAD.h"
#include "PSXREGION.h"
#include <LIBGPU.H>

struct SONG_POS;
struct Texture;
struct CBruce;

#include "LOCALTXT.h"

/** FRONT-owned replay descriptor slice with the complete array stride. */
struct SVideoInfo
{
    int VideoMode;
    char reserved_004[0x18];
    unsigned long NumFrames;
    char reserved_020[0x2c];
    int ReplayHighlight;
    char reserved_050[0x6c];
    int PlaybackFrozen;
};

/** FRONT-owned tape view retaining the complete object size. */
struct CTape
{
    /** Reports whether the tape title may be drawn this frame. */
    static int DisplayTitle();
    /** Reports whether the continue prompt may be drawn this frame. */
    static int DisplayContinueButton();
    /** Advances tape animation and progression messaging. */
    void AI(int tapeNumber);

    char reserved_000[0x48];
    void **mVTable;
    char reserved_04c[0xc8];
    int mSessionTapeNum;
    int mTapeNum;
    char reserved_11c[0x54];
};

#define SCREENTOOLS_AGGREGATE_LAYOUT
#define SCREENTOOLS_AGGREGATE_LAYOUT_ONLY
#include "SCREENTOOLS.h"
#undef SCREENTOOLS_AGGREGATE_LAYOUT_ONLY
#undef SCREENTOOLS_AGGREGATE_LAYOUT

/** Body fields read by the front-end menu and save-state paths. */
struct CBodyFrontView
{
    char reserved_000[0x1c];
    void *mNextItem;
    char reserved_020[0x9a];
    short mHealth;
    char reserved_0bc[0x4c];
};

struct CSuperFrontView
{
    struct CBodyFrontView CBody;
    char reserved_108[0x88];
};

/** FRONT-visible leading fields of the skater's panel and run statistics. */
struct CPanelDataFrontView
{
    struct CBruce *mpBruce;
    unsigned int mTotalScore;
    unsigned short mPlayerNum;
    unsigned short mNumTricksLanded;
    unsigned short mNumBails;
    unsigned short mNumTricksInCombo;
    unsigned int mBestComboScore;
    char mBestComboName[100];
    char mBestComboName2[100];
    char mBestComboName3[100];
    unsigned int mLandedTotal;
    char reserved_144[0x70];
    struct Font mTrickFont;
    struct Font mScoreFont;
    char reserved_254[0x690];
};

/** FRONT-owned complete-size skater view. */
struct CBruce
{
    struct CSuperFrontView CSuper;
    struct CPanelDataFrontView mPanelData;
    char reserved_a74[0x334];
    int mSkaterType;
    int mIndex;
    int mPlayTime;
    struct SControl *mPad;
    int mTags;
    int mSteals;
    char reserved_dc0[0xa8];
    int mJumpInhibit;
    int mRailInhibit;
    char reserved_e70[0x2d0];
    int mSessionTapes;
    char reserved_1144[0x328];
};

#include "SANIMFRAME.h"

/**
 * Loading-gauge geometry retained by the loading-screen
 * helpers.  The two fixed rectangles surround a zero-width progress rectangle.
 */
struct FrontLoadingGaugeState
{
    RECT Frame;
    RECT Interior;
    unsigned char Color[4];
    RECT Progress;
};

typedef char Front_LoadingGaugeState_size_must_be_28[
    (sizeof(struct FrontLoadingGaugeState) == 28) ? 1 : -1];

typedef char Front_CClass_size_must_be_4[(sizeof(CClass) == 4) ? 1 : -1];
typedef char Front_SControl_size_must_be_364[(sizeof(SControl) == 0x16c) ? 1 : -1];
typedef char Front_SPSXRegion_size_must_be_64[(sizeof(SPSXRegion) == 0x40) ? 1 : -1];
typedef char Front_LocalText_size_must_be_652[(sizeof(LocalText) == 0x28c) ? 1 : -1];
typedef char Front_SVideoInfo_size_must_be_192[(sizeof(SVideoInfo) == 0xc0) ? 1 : -1];
typedef char Front_CTape_size_must_be_368[(sizeof(CTape) == 0x170) ? 1 : -1];
typedef char Front_CBodyView_size_must_be_264[(sizeof(CBodyFrontView) == 0x108) ? 1 : -1];
typedef char Front_CSuperView_size_must_be_400[(sizeof(CSuperFrontView) == 0x190) ? 1 : -1];
typedef char Front_CPanelDataView_size_must_be_2276[
    (sizeof(CPanelDataFrontView) == 0x8e4) ? 1 : -1];
typedef char Front_CBruce_size_must_be_5228[(sizeof(CBruce) == 0x146c) ? 1 : -1];
typedef char Front_SEntry_size_must_be_24[(sizeof(SEntry) == 0x18) ? 1 : -1];
typedef char Front_SDK_VECTOR_size_must_be_16[
    (sizeof(VECTOR) == 16) ? 1 : -1];
typedef char Front_SFrontBackground_focus_offset_must_be_12[
    ((unsigned long)&(((SFrontBackground *)0)->Focus) == 12) ? 1 : -1];
typedef char Front_SFrontBackground_size_must_be_36[(sizeof(SFrontBackground) == 0x24) ? 1 : -1];

#include "SFX.h"
#include "LEVELGEN.h"
#include "MESS.h"

/** Opaque LevelGenerator method surface used while loading a level. */
struct LevelGenerator
{
    /** Constructs a generator for editor or in-game output. */
    LevelGenerator(bool gameBuild);
    /** Loads the source models for one level theme. */
    void LoadModels(char *file);
    /** Builds a runtime PSX level image and returns its allocated word buffer. */
    unsigned int *CreatePSX(int *ret_region, bool refresh_models);
    /** Builds the generated level's trigger data. */
    void FakeTrig();
};

#define PANEL_PRIMITIVE_API
#include "PANEL.h"
#undef PANEL_PRIMITIVE_API
#include "TAPES.h"
#include "TRIG.h"
#include "SHELL.h"
#include "CARD.h"
#include "REDBOOK.h"
#include "CAREER.h"
#include "FLASH.h"
#include "MIPS.h"
#define CAREER_OWNER_LAYOUT
#include "CAREER.h"
#undef CAREER_OWNER_LAYOUT
#include "UTILS.h"
#include "PAD.h"
#include "FONT.h"
#include "COMPCHECK_H.h"

inline void *operator new(unsigned int, void *p)
{
    return p;
}

// MAIN.SYM places MechList in ordinary data, outside this TU's small-data set.
#include "MECHLIST.h"

// === Forward type declarations ===

typedef char Front_CMenu_size_must_be_384[
    (sizeof(CMenu) == 0x180) ? 1 : -1];

/**
 * Original seven-word virtual table emitted beside FRONT's CMenu methods.
 *
 * GCC 2.x does not permit taking a C++ destructor's address directly, so the
 * two destructor slots use ABI declarations while ordinary CMenu methods stay
 * expressed as C++ members.
 */
struct FrontCMenuVTable
{
    int reserved_00;
    int reserved_04;
    void (*destroyMenu)(CMenu *self, int flags);
    int reserved_0c;
    int reserved_10;
    int reserved_14;
    void (*destroyClass)(CClass *self, int flags);
};

void Front_CMenuDestructorAbi(CMenu *, int) asm("_._5CMenu");
extern "C" {
static void __DOT__6CClass_8002D678(CClass *, int);
}
#define FRONT_CMENU_VTABLE_ABI_TYPE FrontCMenuVTable
#define FRONT_DECLARE_CMENU_VTABLE_ADDRESS_VIEWS
#include "FRONT.h"

typedef char Front_CMenu_vtable_size_must_be_28[
    (sizeof(FrontCMenuVTable) == 28) ? 1 : -1];

/** Invoke the deleting-destructor slot shared by front-end menu objects. */
static inline void Front_DestroyMenu(CMenu *menu)
{
    reinterpret_cast<FrontCMenuVTable *>(menu->CClass.vtable)
        ->destroyMenu(menu, 3);
}

/** Invoke the same deleting-destructor prefix on a collected tape. */
static inline void Front_DestroyTape(CTape *tape)
{
    reinterpret_cast<FrontCMenuVTable *>(tape->mVTable)
        ->destroyMenu(reinterpret_cast<CMenu *>(tape), 3);
}

struct Sprite2;

/** Original Sprite2 virtual-table slots used by the front-end. */
struct Sprite2VTable {
    int _vf0;
    int _vf1;
    void (*Delete)(Sprite2 *self, int flags);
    int _vf3;
    int _vf4;
    void (*Display)(Sprite2 *self, int x, int y, int flags);
};

/** Pointer-only Sprite2 view exposing the original virtual-table slots. */
struct Sprite2 {
    Sprite2VTable *vtable;
    void *Transparent;
    void *Shaded;
    int TransType;
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char JustX;
    unsigned char JustY;
    unsigned char reserved_15[3];
    int Scale;
    unsigned char reserved_1c[0x20];

    static void *operator new(unsigned int size) throw()
    {
        return ::CClass::operator new(size);
    }
    /** Build a sprite from a bitmap file. */
    Sprite2(char *name, int pack, int sliceWidth, int sliceHeight);

    void Display(int x, int y, int flags)
    {
        vtable->Display(this, x, y, flags);
    }

    void Delete(int flags)
    {
        vtable->Delete(this, flags);
    }
};

typedef char Front_Sprite2_size_must_be_60[
    (sizeof(Sprite2) == 0x3c) ? 1 : -1];

#include "SPRITE.h"

typedef char Front_Sprite_size_must_be_24[
    (sizeof(Sprite) == 0x18) ? 1 : -1];

#include "PROMPTSCREENELEMENT.h"

enum
{
    FrontLevelGeneratorStorageSize = 42548,
    FrontPromptScreenElementStorageSize = 104
};

/** Common virtual prefix used by temporary level-generator and prompt objects. */
struct FrontPolymorphicVTable
{
    int reserved_00;
    int reserved_04;
    void (*Destroy)(void *object, int flags);
    int reserved_0c;
    void (*Display)(void *object, int x, int y);
};

struct FrontPolymorphicView
{
    FrontPolymorphicVTable *vtable;
};

static inline void Front_DestroyPolymorphic(void *object)
{
    FrontPolymorphicView *view =
        reinterpret_cast<FrontPolymorphicView *>(object);
    view->vtable->Destroy(object, 3);
}

static inline void Front_DisplayPolymorphic(
    void *object, int x, int y)
{
    FrontPolymorphicView *view =
        reinterpret_cast<FrontPolymorphicView *>(object);
    view->vtable->Display(object, x, y);
}

/** Display-buffer slice needed to append front-end cursor primitives. */
struct SDoubleBuffer {
    RECT DrawClip;
    char reserved_008[0x7c];
    unsigned int *PanelOT;
};

typedef char Front_SDoubleBuffer_size_must_be_at_least_136[
    (sizeof(SDoubleBuffer) == 0x88) ? 1 : -1];

#include "GAMEPREFERENCES.h"

typedef char Front_SFrontBackground_size_must_be_36[
    (sizeof(SFrontBackground) == 36) ? 1 : -1];
/*
 * FRONT's initialized small-data prefix.  MAIN.SYM and the executable place
 * this contribution at 0x800D8BF8-0x800D8F18; keeping the declarations in
 * address order also preserves the string targets used by FrontBackgrounds.
 */
int GHorseContinue __attribute__((section(".sdata"))) = 0;
int MIDILEVEL __attribute__((section(".sdata"))) = 0x4000;
int SFXLEVEL __attribute__((section(".sdata"))) = 0x4000;
int XALEVEL __attribute__((section(".sdata"))) = 0x7f;
#define FRONTTIMERS_TITLE_TIMEOUT_OWNER_DECLARATION
#include "FRONTTIMERS.h"
int TitleTimeout __attribute__((section(".sdata"))) = 0x14;
int GRollCredits __attribute__((section(".sdata"))) = 0;
int GHighScoreLineIndex __attribute__((section(".sdata"))) = -1;
int GHighScoreInitialCursorPos __attribute__((section(".sdata"))) = 0;
int GSoftResetEnabled __attribute__((section(".sdata"))) = 1;
EPausedStatus Status __attribute__((section(".sdata"))) = STATUS_NORMAL;
static int DemoFlashTimer __attribute__((section(".sdata"))) = 0x30;
static char DemoString[]
    __attribute__((section(".sdata"), aligned(4))) = "demo mode";
static char HorseLabel1[]
    __attribute__((section(".sdata"), aligned(4))) = "PLAYER 1";
static char HorseLabel2[]
    __attribute__((section(".sdata"), aligned(4))) = "PLAYER 2";
int CurrentFrontBackground __attribute__((section(".sdata"))) = 0;

static char FrontRoofLib[] __attribute__((section(".sdata"), aligned(4))) =
    "roof_lib";
static char FrontRoofEnv[] __attribute__((section(".sdata"), aligned(4))) =
    "roof_1";
static char FrontRoofSizes[] __attribute__((section(".sdata"), aligned(4))) =
    "roofback";
static char FrontSewerLib[] __attribute__((section(".sdata"), aligned(4))) =
    "sewr_lib";
static char FrontSewerEnv[] __attribute__((section(".sdata"), aligned(4))) =
    "sewr_4";
static char FrontSewerSizes[] __attribute__((section(".sdata"), aligned(4))) =
    "sewrback";
static char FrontNoBackground[] __attribute__((section(".sdata"), aligned(4))) =
    "";

const unsigned int *pSizeData __attribute__((section(".sdata"))) = 0;
int GamePaused __attribute__((section(".sdata"))) = 0;

static int WobbleAmplitude __attribute__((section(".sdata"))) = 0;
static int WobbleT __attribute__((section(".sdata"))) = 0;
static int UpDownAutoRepeatTimer __attribute__((section(".sdata"))) = 0;

#define FRONTTIMERS_LEFT_RIGHT_TIMER_OWNER_DECLARATION
#include "FRONTTIMERS.h"
int LeftRightAutoRepeatTimer __attribute__((section(".sdata"))) = 0;
SAnimFrame *ButtonsAnim __attribute__((section(".sdata"))) = 0;
int VoiceLevelGotFocus __attribute__((section(".sdata"))) = 0;

CMenu *pYesNoMenu __attribute__((section(".sdata"))) = 0;
CMenu *pESMenu __attribute__((section(".sdata"))) = 0;
CMenu *pVSMenu __attribute__((section(".sdata"))) = 0;
CMenu *pVS2Menu __attribute__((section(".sdata"))) = 0;
CMenu *pVS3Menu __attribute__((section(".sdata"))) = 0;
CMenu *pEHMenu __attribute__((section(".sdata"))) = 0;
CMenu *pERMMenu __attribute__((section(".sdata"))) = 0;
CMenu *pSoundMenu __attribute__((section(".sdata"))) = 0;

/*
 * Target-named owners export the canonical data symbols.  Objects larger
 * than FRONT's -G8 threshold are addressed directly; smaller objects retain
 * incomplete-array views where their target code uses absolute addresses.
 */
namespace FrontAddressViewOwners
{
extern "C"
{
static RECT Db_SkyColorBak[]
    __attribute__((section(".sdata"))) = {
        {0, 0, 512, 511},
    };
}
}

static int GaugeOn __attribute__((section(".sdata"))) = 0;
static unsigned int TotalProgress __attribute__((section(".sdata"))) = 0;
static unsigned int TotalSize __attribute__((section(".sdata"))) = 0;
static FrontLoadingGaugeState FrontLoadingGauge
    __attribute__((section(".sdata"))) = {
        {106, 200, 300, 12},
        {108, 201, 296, 10},
        {0, 0, 0, 0},
        {108, 201, 0, 10},
    };

extern "C"
{
    static CMenu *pPausedMenu __attribute__((section(".sdata"))) = 0;
    static CMenu *pRestartsMenu __attribute__((section(".sdata"))) = 0;

    namespace FrontAddressViewOwners
    {
    static char GrafLabel1[]
        __attribute__((section(".sdata"), aligned(4))) = "TAGS";
    static char GrafLabel2[]
        __attribute__((section(".sdata"), aligned(4))) = "STEALS";
    static char GrafLabel3[]
        __attribute__((section(".sdata"), aligned(4))) = "HIGH";
    static char GrafLabel4[]
        __attribute__((section(".sdata"), aligned(4))) = "WINS";
    static char GrafLabel5[]
        __attribute__((section(".sdata"), aligned(4))) = "LOSSES";
    static char GrafLabel6[]
        __attribute__((section(".sdata"), aligned(4))) = "STREAK";
    }

    /**
     * FRONT's compiler-coalesced display-string pool.
     *
     * Fixed-width fields name the original strings while retaining every
     * target padding byte and every existing offset-based access.
     */
    struct FrontDisplayStringPool
    {
        char Score[4];
        char Player1Match[16];
        char Player2Match[16];
        char Separator[4];
        char StringFormat[4];
        char TimeUp[12];
        char GameOver[12];
        char CountFormat[12];
        char LongFormat[4];
        char TrickFont[16];
        char PasscodeFormat[16];
        char Tie[4];
        char Player2Wins[16];
        char Player1Wins[16];
        char ScoreSetFormat[16];
        char GoingForFormat[16];
        char YouScoredFormat[16];
        char Push[8];
        char YouAreAn[12];
        char YouAreA[12];
        char Switch[8];
        char Player1ScoreFormat[16];
        char Player2ScoreFormat[16];
        char Draw[8];
        char GameStateFormat[12];
    };

    namespace FrontAddressViewOwners
    {
    static char GrafFmt[4]
        __attribute__((section(".sdata"), aligned(4))) = "%d";
    }
    static char Front_GrafPlayer1Match[16]
        __attribute__((section(".sdata"), aligned(4))) = "PLAYER 1 MATCH";
    static char Front_GrafPlayer2Match[16]
        __attribute__((section(".sdata"), aligned(4))) = "PLAYER 2 MATCH";
    static char Front_GrafSeparator[4]
        __attribute__((section(".sdata"), aligned(4))) = "_";
    static char Front_GrafStringFormat[4]
        __attribute__((section(".sdata"), aligned(4))) = "%s";
    static char Front_GrafTimeUp[12]
        __attribute__((section(".sdata"), aligned(4))) = "TIME IS UP!";
    static char Front_GrafGameOver[12]
        __attribute__((section(".sdata"), aligned(4))) = "GAME OVER";
    static char Front_GrafCountFormat[12]
        __attribute__((section(".sdata"), aligned(4))) = "%ld of %ld";
    static char Front_GrafLongFormat[4]
        __attribute__((section(".sdata"), aligned(4))) = "%ld";
    static char Front_GrafTrickFont[16]
        __attribute__((section(".sdata"), aligned(4))) = "s2tricks.fnt";
    static char Front_GrafPasscodeFormat[16]
        __attribute__((section(".sdata"), aligned(4))) = "PASSCODE : %s";
    static char Front_GrafTie[4]
        __attribute__((section(".sdata"), aligned(4))) = "TIE";
    static char Front_GrafPlayer2Wins[16]
        __attribute__((section(".sdata"), aligned(4))) = "PLAYER TWO WINS";
    static char Front_GrafPlayer1Wins[16]
        __attribute__((section(".sdata"), aligned(4))) = "PLAYER ONE WINS";
    static char Front_GrafScoreSetFormat[16]
        __attribute__((section(".sdata"), aligned(4))) = "SCORE SET AT %d";
    static char Front_GrafGoingForFormat[16]
        __attribute__((section(".sdata"), aligned(4))) = "GOING FOR %d";
    static char Front_GrafYouScoredFormat[16]
        __attribute__((section(".sdata"), aligned(4))) = "YOU SCORED %d";
    static char Front_GrafPush[8]
        __attribute__((section(".sdata"), aligned(4))) = "PUSH";
    static char Front_GrafYouAreAn[12]
        __attribute__((section(".sdata"), aligned(4))) = "YOU ARE AN";
    static char Front_GrafYouAreA[12]
        __attribute__((section(".sdata"), aligned(4))) = "YOU ARE A";
    static char Front_GrafSwitch[8]
        __attribute__((section(".sdata"), aligned(4))) = "SWITCH";
    static char Front_GrafPlayer1ScoreFormat[16]
        __attribute__((section(".sdata"), aligned(4))) = "PLAYER ONE %d";
    static char Front_GrafPlayer2ScoreFormat[16]
        __attribute__((section(".sdata"), aligned(4))) = "PLAYER TWO %d";
    static char Front_GrafDraw[8]
        __attribute__((section(".sdata"), aligned(4))) = "DRAW";
    static char Front_GrafGameStateFormat[12]
        __attribute__((section(".sdata"), aligned(4))) = "gs = %d\n";

    namespace FrontAddressViewOwners
    {
    static char _restart_comp_string[]
        __attribute__((section(".sdata"), aligned(4))) = "RE_START_FRIG";
    }
}

static int StartSelectPressed asm("StartSelectPressed.98")
    __attribute__((section(".sdata"))) = 0;
static unsigned int VblanksWhenStartSelectFirstPressed
    asm("VblanksWhenStartSelectFirstPressed.99")
    __attribute__((section(".sdata"))) = 0;
static char FrontEditorPrefix[]
    __attribute__((section(".sdata"), aligned(4))) = "de_";

extern "C"
{
    namespace FrontAddressViewOwners
    {
    static char s_s2demo2_bmp_800d8e78[]
        __attribute__((section(".sdata"), aligned(4))) = "s2demo2.bmp";
    static char s_skate2_vab_800d8e84[]
        __attribute__((section(".sdata"), aligned(4))) = "skate2.vab";
    static char s_scorep_bmp_800d8e90[]
        __attribute__((section(".sdata"), aligned(4))) = "scorep.bmp";
    static char s_special_bmp_800d8e9c[]
        __attribute__((section(".sdata"), aligned(4))) = "special.bmp";
    static char s_spec_bar_bmp_800d8ea8[]
        __attribute__((section(".sdata"), aligned(4))) = "spec_bar.bmp";
    static char s_paused_bmp_800d8eb8[]
        __attribute__((section(".sdata"), aligned(4))) = "paused.bmp";
    static char DAT_800d8ec4[]
        __attribute__((section(".sdata"), aligned(4))) = "bits";
    static char s_items_800d8ecc[]
        __attribute__((section(".sdata"), aligned(4))) = "items";
    static char s_SkMedals_800d8ed4[]
        __attribute__((section(".sdata"), aligned(4))) = "SkMedals";
    static char s_sk2anim_800d8ee0[]
        __attribute__((section(".sdata"), aligned(4))) = "sk2anim";
    static char s_SkEd_d_800d8ee8[]
        __attribute__((section(".sdata"), aligned(4))) = "SkEd%d";
    }
    /*
     * The derived DAT_* aliases currently point at unrelated target addresses,
     * so retain their proven bytes under owner-local names without publishing
     * misleading native-link definitions.
     */
    struct FrontEditorLevelSuffixBytes
    {
        char Text[4];
    };
    namespace FrontAddressViewOwners
    {
    static FrontEditorLevelSuffixBytes DAT_800d8ef0
        __attribute__((section(".sdata"))) = {{'a', 'c', 'k', '\0'}};
    }
    namespace FrontAddressViewOwners
    {
    static char s_s2prmpt1_bmp_800d8ef4[]
        __attribute__((section(".sdata"), aligned(4))) = "s2prmpt1.bmp";
    static char s_to_continue_800d8f04[]
        __attribute__((section(".sdata"), aligned(4))) = "to continue";
    }
    namespace FrontAddressViewOwners
    {
    static int DAT_800d8f10
        __attribute__((section(".sdata"))) = 0x00cb00a3;
    static int DAT_800d8f14
        __attribute__((section(".sdata"))) = 0x002300ba;
    }
}

/*
 * FRONT's complete zero-initialized small-data tail.  MAIN.SYM places this
 * contribution immediately after FrontPromptClipBottomRight, at
 * 0x800D8F18-0x800D8F90.  Keeping the vtable and globals together preserves
 * their original owner, offsets, and export surface.
 */
namespace FrontCMenuVTableOwner
{
FrontCMenuVTable __vt_5CMenu
    __attribute__((section(".sdata"))) = {
        0,
        0,
        Front_CMenuDestructorAbi,
        0,
        0,
        0,
        __DOT__6CClass_8002D678,
    };
}
int BackOut = 0;
int StartScreen = 0;
int DemoTimer = 0;
int Cheat_AllTapes = 0;
int GHorseSounded = 0;
int GHorseScale = 0;
int GCompoSounded = 0;
int NextHorseMode = 0;
int NextHorsePlayer = 0;
int NextHorseScore = 0;
int NextHorseRestart = 0;
int NextHorseLetters[2] = {0, 0};
SONG_POS Paused_SongPos
    __attribute__((section(".sdata"))) = {0, 0, 0};
int Front_OnTime = 0;
int StatsMode = 0;
int GStart1 = 0;
int GStart2 = 0;
unsigned int JudgeTimer = 0;
unsigned int LadderTimer = 0;
unsigned int WinLoseTimer = 0;

typedef char Front_GrafFmt_size_must_be_292[
    (sizeof(FrontDisplayStringPool) == 292) ? 1 : -1];

/**
 * Messages retained in FRONT's original read-only literal pool.
 *
 * The linked image keeps these strings together after `DefaultGameState` and
 * before FRONT's switch tables.  The six competition messages have no
 * surviving code reference, but are part of the original owner contribution.
 */
struct FrontReadOnlyLiteralPool
{
    char Player1SetScore[20];
    char Player2SetScore[20];
    char DidNotMakeCut[28];
    char TryAgain[32];
    char CompetitionName[36];
    char EnterScore[40];
    char CompetitionSite[44];
    char CompetitionPrizes[44];
    char NoScoreSwitch[20];
    char LoadingSprites[28];
};

typedef char Front_ReadOnlyLiteralPool_size_must_be_312[
    (sizeof(FrontReadOnlyLiteralPool) == 312) ? 1 : -1];

/** Exact-sized owner wrapper for the public default save-game symbol. */
struct FrontDefaultGameStateOwner
{
    SSaveGame State;
};

typedef char Front_DefaultGameStateOwner_size_must_be_136[
    (sizeof(FrontDefaultGameStateOwner) == 136) ? 1 : -1];

typedef char Front_ReadOnlyContribution_size_must_be_448[
    (sizeof(FrontDefaultGameStateOwner) +
     sizeof(FrontReadOnlyLiteralPool) == 448) ? 1 : -1];

/**
 * Default state followed by FRONT's original source-literal contribution.
 *
 * C linkage preserves the public `DefaultGameState` symbol at offset zero.
 * Its type and size come directly from MAIN.SYM; the source-literal pool is
 * the immediately following owner-local contribution.
 */
namespace FrontReadOnlyPrefixOwner
{
extern "C"
{
FrontDefaultGameStateOwner DefaultGameState
    __attribute__((section(".rdata"))) = {
  {
    0,
    {
        "skul_t",
        "Re_Start_School",
        0,
        0,
        0,
        1,
        0,
        {0, 0, 0, 0, 0, 0, 0, 0},
        100,
        3,
        5,
    },
    {
        1,
        0x2000,
        0x30,
        0xb4,
        1,
        4,
        8,
        2,
        1,
        4,
        8,
        2,
    },
  },
};
}

static FrontReadOnlyLiteralPool DefaultGameStateLiterals
    __attribute__((section(".rdata"))) = {
    "PLAYER 1 SET SCORE",
    "PLAYER 2 SET SCORE",
    "You didn't make the cut...",
    "Try again to earn a passcode",
    "for the {Big Score Competition!}",
    "Congrats! Enter your score and passcode",
    "online at {activision.com}. You could win",
    "huge prizes in the {Big Score Competition!}",
    "NO SCORE   SWITCH",
    "Loading some sprites\n",
};
}

/** Address one FRONT literal through the public state symbol and full addend. */
static inline char *Front_Player1SetScoreText()
{
    return reinterpret_cast<char *>(
        &FrontReadOnlyPrefixOwner::DefaultGameState) + 136;
}

/** Address one FRONT literal through the public state symbol and full addend. */
static inline char *Front_Player2SetScoreText()
{
    return reinterpret_cast<char *>(
        &FrontReadOnlyPrefixOwner::DefaultGameState) + 156;
}

/** Address one FRONT literal through the public state symbol and full addend. */
static inline char *Front_NoScoreSwitchText()
{
    return reinterpret_cast<char *>(
        &FrontReadOnlyPrefixOwner::DefaultGameState) + 400;
}

/** Address one FRONT literal through the public state symbol and full addend. */
static inline char *Front_LoadingSpritesText()
{
    return reinterpret_cast<char *>(
        &FrontReadOnlyPrefixOwner::DefaultGameState) + 420;
}

/** Typed state view of the read-only prefix's public leading symbol. */
#define FRONT_DECLARE_DEFAULT_GAME_STATE_VIEW
#include "FRONT.h"

/*
 * FRONT's complete 0x340-byte mutable-data contribution.  Every object below
 * has a named, target-proven type and initializer; no anonymous padding object
 * is needed because the natural alignments reproduce the two-byte DemoName
 * tail pad.
 */
#include "GTAPE.h"
CTape *GTape[5] __attribute__((section(".data"))) = {};
unsigned char GHorseFallingText[sizeof(FallingTextEffect)]
    __attribute__((section(".data"))) = {};
SFrontBackground FrontBackgrounds[3]
    __attribute__((section(".data"))) = {
        {
            FrontRoofLib,
            FrontRoofEnv,
            FrontRoofSizes,
            {0x1cce, -0x49d, 0x14d4, 0},
            0x6d0,
            0x300,
        },
        {
            FrontSewerLib,
            FrontSewerEnv,
            FrontSewerSizes,
            {-0x2c10, 0x2ab2, 0x150fd, 0},
            0xdd0,
            0x900,
        },
        {
            FrontNoBackground,
            FrontNoBackground,
            FrontNoBackground,
            {0, 0, 0, 0},
            0,
            0,
        },
    };
SSaveGame LastGameState __attribute__((section(".data"))) = {};
SSaveGame StartupGameState __attribute__((section(".data"))) = {};
#define DEMONAME_OWNER_DECLARATION
#include "DEMONAME.h"
char DemoName[30] __attribute__((section(".data"))) = {};
int StatsLeft[8] __attribute__((section(".data"))) = {};
int StatsRight[8] __attribute__((section(".data"))) = {};
char soundBuf[24] __attribute__((section(".data"))) = {};
char musicBuf[24] __attribute__((section(".data"))) = {};

/**
 * Recover the target-proven class identity from FRONT's inert global storage.
 *
 * Keeping the storage itself trivial prevents GCC 2.x from inventing
 * `_GLOBAL_.I/D.GHorseContinue`; the target names the two lifecycle wrappers
 * after `LoadingScreen_Main`, so those wrappers remain explicit below.
 */
static inline FallingTextEffect &Front_HorseFallingText()
{
    return *reinterpret_cast<FallingTextEffect *>(
        GHorseFallingText);
}

// === Free function declarations ===
void LoadingScreen_Main(void);
unsigned int Front_CalculateGameChecksum(SSaveGame &game);
void Horse_Reset(void);
void Horse_DisplayWords(int *pNumLetters);
static void InitStatsSwisch(void);
int InCompetition(void);
extern "C"
{
static void PrintPaused(void);
}
int Graffiti_Stats(CBruce *pSkater, int x, int y, int r, int g, int b, int Win, int Lose, int Streak, int High);
static void CommonSwitchOnMenuCode(void);
void RestartCompFrig(void);

// === External function declarations ===
#include "MAIN.h"
#include "DB.h"
int Competition_IsOver(void);
#include <STDIO.H>
#include <STRINGS.H>
#include "GSKATER_STATE.h"
// === External globals (gp-relative, <=8 bytes for -G8) ===
// 0x800D8F44 is GHorseSounded, not a standalone HorsePlayerCount object.

// === External globals (absolute addressing via incomplete array) ===
static int soundVol;
static int musicVol;

// === External globals (absolute) — additional ===
// 0x800B73B8 is GameText+0x1C0, not a standalone CommonMenuStr object.
extern char GrafLabel1[];                                // 0x800D8D08 — graffiti stat label 1
extern char GrafLabel2[];                                // 0x800D8D10 — graffiti stat label 2
extern char GrafLabel3[];                                // 0x800D8D18 — graffiti stat label 3
extern char GrafLabel4[];                                // 0x800D8D20 — graffiti stat label 4
extern char GrafLabel5[];                                // 0x800D8D28 — graffiti stat label 5
extern char GrafLabel6[];                                // 0x800D8D30 — graffiti stat label 6
extern char GrafFmt[];                                  // 0x800D8D38 — graffiti stat format pool
struct FrontSinCos
{
    short Sin;
    short Cos;
};
#define RCOSSIN_DECL_TYPE FrontSinCos
#define RCOSSIN_SCALAR_VIEW
#include "RCOSSIN.h"
extern RECT Db_SkyColorBak[];                            // 0x800D8CD0 — sky color backup

// === External function declarations (for Front_Update) ===
#define FRONT_MEMCARD_API
#include "FRONT.h"
#undef FRONT_MEMCARD_API
int GetUpDown(void);
void Competition_Init(void);
// A scalar source view retains the target's gp-relative address formation;
// referencing the typed owner object directly changes Front_Update by
// 1,873 instructions and 47 relocations under GCC 2.91.
#define PAUSEDSONGPOS_DECLARE_SCALAR_ADDRESS_VIEW
#include "PAUSEDSONGPOS.h"

static inline SONG_POS *Front_PausedSongPosition()
{
    return reinterpret_cast<SONG_POS *>(
        &FrontPausedSongPositionAddressView::Paused_SongPos);
}
void Competition_JudgeLogic(void);
void Competition_LadderLogic(void);
int Competition_JudgeFullyDisplayed(void);
int Competition_LadderFullyDisplayed(void);
void Competition_FullyDisplayJudge(void);
void Competition_FullyDisplayLadder(void);
int Competition_NewHeat(void);
void Competition_WinLoseLogic(EPausedStatus &, int &);
void Competition_EndGameLogic(EPausedStatus &, int &);

// === External globals (absolute) for Front_Update ===
// front.obj owns int[2] NextHorseLetters at 0x800D8F60; no NextHorseLetters_abs alias is needed.
// main.obj owns int GameFrozen at 0x800D9638; no GameFrozen_abs alias is needed.
// main.obj owns int TTime at 0x800D9620; no TTime_abs alias is needed.
// No GBailsList absolute-view alias is needed in FRONT.
// rfront.obj owns struct SVideoRestart GVideoRestart at 0x800B5138; no GVideoRestart_abs alias is needed.
// 0x800C8EB0 is FaceCache+0x798, not a standalone MechPlayers object.
// 0x800B5110 is GPreferences+0x190, not a standalone HighScoreTable object.
#include "RESTARTNAMETABLES.h"

// === External function declarations (for Front_Display) ===
void Competition_JudgeDisplay(void);
void Competition_LadderDisplay(void);
void Competition_WinLoseDisplay(void);
void Competition_EndGameDisplay(void);

// === External function declarations (for Front_LoadGame) ===
#include "FILEIO.h"
#include "SPOOL.h"
void Bit_Init(void);
#include "CREATE.h"
#include "RAIL.h"
// === External globals (absolute) for Front_LoadGame ===
extern int __vt_4Font[];

/** Copy a font value while installing the original Font dispatch table. */
inline Font::Font(const Font &source)
{
    CClass.vtable = __vt_4Font;
    JustX = source.JustX;
    JustY = source.JustY;
    SpaceX = source.SpaceX;
    SpaceY = source.SpaceY;
    R = source.R;
    G = source.G;
    B = source.B;
    Aliased = source.Aliased;
    Shadow = source.Shadow;
    ShadowDx = source.ShadowDx;
    ShadowDy = source.ShadowDy;
    CharMap = source.CharMap;
    FixedWidth = source.FixedWidth;
    Scale = source.Scale;
    CharTab = source.CharTab;
    NumChars = source.NumChars;
    Clut = source.Clut;
    CurrentTextEffect = source.CurrentTextEffect;
    OldR = source.OldR;
    OldG = source.OldG;
    OldB = source.OldB;
}

extern char DAT_800d8ec4[];
extern char s_items_800d8ecc[];
extern char s_sk2anim_800d8ee0[];
extern char s_SkEd_d_800d8ee8[];
extern FrontEditorLevelSuffixBytes DAT_800d8ef0[]
    __attribute__((section(".data")));
extern RECT DAT_800d8f10[] __attribute__((section(".data")));

// === Function implementations (address order) ===

/**
 * LoadingScreen_Main__Fv
 *
 * Address:   0x80025F94
 * Mangled:   LoadingScreen_Main__Fv
 * Demangled: LoadingScreen_Main(void)
 *            ASM: jr $ra; nop
 */
void LoadingScreen_Main(void)
{
}

/**
 * ClearSoftReset__Fv
 *
 * Address:   0x80025F9C
 * Mangled:   ClearSoftReset__Fv
 * Demangled: ClearSoftReset(void)
 *            ASM: jr $ra; nop
 */
void ClearSoftReset(void)
{
}

/**
 * LoadingScreen_SetCurrentLevel__Fiii
 *
 * Address:   0x80025FA4
 * Mangled:   LoadingScreen_SetCurrentLevel__Fiii
 * Demangled: LoadingScreen_SetCurrentLevel(int, int, int)
 *            ASM: jr $ra; nop
 *
 * @param a unused loading-screen state value
 * @param b unused loading-screen state value
 * @param c unused loading-screen state value
 */
void LoadingScreen_SetCurrentLevel(int a, int b, int c)
{
}

/**
 * FlickerSync__Fv
 *
 * Address:   0x80025FAC
 * Mangled:   FlickerSync__Fv
 * Demangled: FlickerSync(void)
 *            ASM: jr $ra; nop
 */
void FlickerSync(void)
{
}

/**
 * Front_PrintDemo__Fv
 *
 * Address:   0x80025FB4
 * Mangled:   Front_PrintDemo__Fv
 * Demangled: Front_PrintDemo(void)
 */
void Front_PrintDemo(void)
{
    int demo_tick = DemoFlashTimer - 1;
    DemoFlashTimer = demo_tick;
    if (demo_tick == 0)
    {
        DemoFlashTimer = 0x30;
    }
    if (DemoFlashTimer >= 0xd)
    {
        if (GNumberOfPlayers == 2)
        {
            Mess_SetTextJustify(0);
            Mess_SetScale(0xff);
            Mess_SetRGB(0x96, 0x64, 0x19, 0);
            Mess_DrawText(0x80, 0x23, DemoString, 1, 0, 0x100, 0);
            Mess_DrawText(0x180, 0x23, DemoString, 1, 0, 0x100, 0);
        }
        else
        {
            Mess_SetTextJustify(0);
            Mess_SetScale(0xff);
            Mess_SetRGB(0x96, 0x64, 0x19, 0);
            Mess_DrawText(0x100, 0x23, DemoString, 1, 0, 0x100, 0);
        }
    }
}

/**
 * Front_DisplayScreen__FPc
 *
 * Address:   0x800260D0
 * Mangled:   Front_DisplayScreen__FPc
 * Demangled: Front_DisplayScreen(char *)
 *
 * Draws a temporary full-screen sprite into both display buffers, then deletes
 * it. The original `int` function falls through without an explicit return.
 *
 * @param pName  Asset / object name string.
 */
int Front_DisplayScreen(char *pName)
{
    Sprite2 *pBackground = new Sprite2(pName, 1, 0, 0);
    for (int i = 0; i < 2; i++)
    {
        Db_FlipClear();
        CalcPolyBufferEnd();
        pBackground->Display(0, 0, 0);
        DrawSync(0);
        Pause(1);
        Db_DisplayDraw();
    }
    if (pBackground != 0)
    {
        pBackground->Delete(3);
    }
}

/**
 * Horse_Reset__Fv
 *
 * Address:   0x800261A0
 * Mangled:   Horse_Reset__Fv
 * Demangled: Horse_Reset(void)
 *
 */
void Horse_Reset(void)
{
    GHorseSounded = 0;
    GHorsePlayer = 0;
    GHorseMode = 0;
    GHorseScore = 0;
    GHorseDoneStart = 0;
    GHorseRestart = 0;
    GHorseLetters[0] = 0;
    GHorseLetters[1] = 0;
    GHorseCursorPos = 0;
    GHorseWordChosen = 0;
    GHorseClearInput2 = 1;
}

/**
 * Horse_DisplayWords__FPi
 *
 * Address:   0x80026200
 * Mangled:   Horse_DisplayWords__FPi
 * Demangled: Horse_DisplayWords(int *)
 *
 * @param pNumLetters  NumLetters to operate on.
 */
void Horse_DisplayWords(int *pNumLetters)
{
    char Got[30];
    Mess_SetScale(0x100);
    Mess_SetTextJustify(0);
    int i = 0;
    // Reusing pNumLetters directly changes ten instructions in this loop;
    // the original keeps a separate advancing count cursor.
    int *pNums = pNumLetters;
    do
    {
        char *pLetter = reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord;
        int Letters = *pNums;
        char *pGot = Got;
        u8 Letter = *pLetter;
        if (Letter != 0)
        {
            char Dash = '-';
            do
            {
                if (Letters != 0)
                {
                    Letters--;
                    *pGot++ = *pLetter++;
                }
                else
                {
                    *pGot++ = Dash;
                    pLetter++;
                }
                Letter = *pLetter;
            } while (Letter != 0);
        }
        *pGot = 0;
        if (i == 0)
        {
            Mess_SetRGB(0x50, 0x50, 0x80, 0);
            Mess_DrawText(0x56, 0x20, HorseLabel1, 0, 0, 0x100, 0);
        }
        else
        {
            Mess_SetRGB(0x80, 0x50, 0x50, 0);
            Mess_DrawText(0x1AA, 0x20, HorseLabel2, 0, 0, 0x100, 0);
        }
        int off = (1 - i) * 340;
        int x = 0x1AA - off;
        Mess_DrawText(x, 0x30, Got, 0, 0, 0x100, 0);
        i++;
        pNums++;
    } while (i < 2);
}

/**
 * InitStatsSwisch__Fv
 *
 * Address:   0x80026370
 * Mangled:   InitStatsSwisch__Fv
 * Demangled: InitStatsSwisch(void)
 *
 */
static void InitStatsSwisch(void)
{
    int i = 0;
    // Direct StatsRight[i]/StatsLeft[i] stores change four instructions and
    // two relocations; these advancing cursors preserve the target addressing.
    int four = 4;
    int neg100 = -100;
    int *right = StatsRight;
    int *left = StatsLeft;
    do
    {
        // The original register allocation keeps this delta in a0; ordinary forms
        // commute four instructions in this exact 22-instruction loop.
        register int diff asm("$4");
        diff = four - i;
        int val;
        val = (diff << 4) - diff;
        val <<= 1;
        int rightVal = val + 0x264;
        *right = rightVal;
        right++;
        val = neg100 - val;
        *left = val;
        // This boundary keeps the left cursor increment in the loop branch's
        // delay slot; removing it changes four instructions.
        asm volatile("");
        i++;
        left++;
    } while (i < 8);
}

/**
 * InCompetition__Fv
 *
 * Address:   0x800263C8
 * Mangled:   InCompetition__Fv
 * Demangled: InCompetition(void)
 *
 * @return one during a single-player competition, otherwise zero
 */
int InCompetition(void)
{
    if (Levels[GLevel].isCompetition == 0)
        goto zero;
    if (GGame == 1)
        return 1;
zero:
    return 0;
}

/**
 * Creates a front-end menu and initializes each entry's color and scale
 * animation state.
 *
 * @param x horizontal text position
 * @param y initial vertical text position
 * @param Justification text-justification mode
 * @param HiScale scale used by the selected entry
 * @param LowScale scale used by unselected entries
 * @param LineSep vertical separation between visible entries
 */
CMenu::CMenu(int x, int y, unsigned char Justification, int HiScale, int LowScale, int LineSep)
{
    CClass.vtable = &FrontCMenuVTableAddressView::__vt_5CMenu;
    mX = x;
    mY = y;
    mJustification = Justification;
    mLineSep = LineSep;
    int i = 0;
    do
    {
        mEntry[i].HiR = 0xB6;
        mEntry[i].HiG = 0x84;
        mEntry[i].HiB = 0x24;
        mEntry[i].LowR = 0x7A;
        mEntry[i].LowG = 0x7A;
        mEntry[i].LowB = 0x7A;
        mEntry[i].FadeUpCounter = 0x80;
        mEntry[i].HiScale = static_cast<short>(HiScale);
        mEntry[i].LowScale = static_cast<short>(LowScale);
        i++;
    } while (i < 15);
    SetLine(0);
}

/** Restores the deleting-destructor vtable before the compiler-owned cleanup. */
CMenu::~CMenu()
{
    CClass.vtable = &FrontCMenuVTableAddressView::__vt_5CMenu + 4;
    // cc1plus generates the trailing `if (__in_chrg & 1) operator delete(this)`
    // branch automatically for the in-charge dtor variant.
}

/** Centers the visible menu entries vertically on the 240-line display. */
void CMenu::CentreY()
{
    // These two scalar constraints retain the original accumulator/count
    // allocation. Ordinary forms change 11 and 29 instructions respectively.
    register int numactivelines asm("$3") = 0;
    int i = 0;
    int totalclearance = 0;
    register int count asm("$2");
    count = mNumLines;
    if (count == 0)
        goto done;
    {
        // Reusing the pinned count as the loop bound changes twenty-eight
        // instructions and two relocations; preserve the copied limit.
        int limit = count;
        do
        {
            if (mEntry[i].Visible != 0)
            {
                numactivelines++;
                totalclearance += mEntry[i].Clearance;
            }
            i++;
        } while (i < limit);
    }
done:
{
    numactivelines--;
    int menuheight = numactivelines * mLineSep + totalclearance;
    int y = (0xF0 - menuheight) / 2;
    mY = y;
}
}

/**
 * Selects a menu line and resets every entry's current and target scale.
 *
 * @param Line selected entry index
 */
void CMenu::SetLine(unsigned char Line)
{
    mLine = Line;
    int i = 0;
    do
    {
        u16 val;
        if (i == mLine)
        {
            val = static_cast<u16>(mEntry[i].HiScale);
        }
        else
        {
            val = static_cast<u16>(mEntry[i].LowScale);
        }
        mEntry[i].TargetScale = static_cast<short>(val);
        mEntry[i].CurrentScale = static_cast<short>(val);
        i++;
    } while (i < 15);
}

/**
 * Finds a menu entry by its displayed text.
 *
 * @param pString text to find
 * @return matching entry index, or zero when no entry matches
 */
int CMenu::FindEntry(const char *pString)
{
    int i = 0;
    if (mNumLines == 0)
        return 0;
    do
    {
        if (Utils_CompareStrings(pString, mEntry[i].pString,
                                 static_cast<unsigned int>(-1)) != 0)
        {
            return i;
        }
        i++;
        if (i >= mNumLines)
            break;
    } while (1);
    return 0;
}

/**
 * Appends a visible, enabled menu entry.
 *
 * @param pString text displayed for the new entry
 */
void CMenu::AddEntry(const char *pString)
{
    mEntry[mNumLines].pString = const_cast<char *>(pString);
    mEntry[mNumLines].Visible = 1;
    mEntry[mNumLines].Enabled = 1;
    mEntry[mNumLines].Clearance = 0;
    mNumLines++;
}

/**
 * Makes the named entry visible.
 *
 * @param pString text identifying the entry
 */
void CMenu::EntryOn(const char *pString)
{
    int idx = FindEntry(pString);
    mEntry[idx].Visible = 1;
}

/**
 * Hides the named entry.
 *
 * @param pString text identifying the entry
 */
void CMenu::EntryOff(const char *pString)
{
    int idx = FindEntry(pString);
    mEntry[idx].Visible = 0;
}

/**
 * Reports whether the named entry is visible.
 *
 * @param pString text identifying the entry
 * @return one when visible, otherwise zero
 */
int CMenu::IsEntryOn(const char *pString)
{
    int idx = FindEntry(pString);
    return mEntry[idx].Visible == 1;
}

/**
 * Compares text with the currently selected entry.
 *
 * @param pString text to compare
 * @return nonzero when the selected entry has the requested text
 */
int CMenu::ChoiceIs(const char *pString)
{
    return Utils_CompareStrings(
        pString, mEntry[mLine].pString, static_cast<unsigned int>(-1));
}

/** Renders the visible entries and highlights the current selection. */
void CMenu::Display()
{
    Mess_SetTextJustify(mJustification);
    int y = mY;

    for (int i = 0; i < mNumLines; i++)
    {
        y += mEntry[i].Clearance;
        if (mEntry[i].Visible != 0)
        {
            if (mEntry[i].CurrentScale > 0)
            {
                Mess_SetScale(mEntry[i].CurrentScale);
                if (i == mLine)
                {
                    Mess_SetRGB(mEntry[i].HiR, mEntry[i].HiG,
                        mEntry[i].HiB, 0);
                    mEntry[i].FadeUpCounter = 0;
                }
                else
                {
                    if (mEntry[i].Enabled != 0)
                        mEntry[i].FadeUpCounter = 0x80;
                    else
                        mEntry[i].FadeUpCounter = 0x40;
                    Mess_SetRGB(
                        static_cast<u8>((mEntry[i].LowR *
                                         mEntry[i].FadeUpCounter) >> 7),
                        static_cast<u8>((mEntry[i].LowG *
                                         mEntry[i].FadeUpCounter) >> 7),
                        static_cast<u8>((mEntry[i].LowB *
                                         mEntry[i].FadeUpCounter) >> 7), 0);

                    mEntry[i].FadeUpCounter += 0x3c;
                    if (mEntry[i].FadeUpCounter > 0x80)
                        mEntry[i].FadeUpCounter = 0x80;
                }

                if (mEntry[i].CurrentScale > mEntry[i].LowScale)
                    Mess_SetSort(0);
                else
                    Mess_SetSort(1);
                int length = Mess_DrawText(mX, y, mEntry[i].pString,
                    0, 0, 0, 0);
                if (mHasCursor != 0 && i == mCursorLine &&
                    mJustification == 0)
                {
                    int off =
                        length * mEntry[i].CurrentScale / 512 +
                        mEntry[i].CurrentScale * 14 / 256;

                    POLY_F3 *leftpoly = reinterpret_cast<POLY_F3 *>(pPoly);
                    POLY_F3 *rightpoly = leftpoly + 1;
                    unsigned char *newppoly =
                        reinterpret_cast<unsigned char *>(rightpoly + 1);
                    if (newppoly <= PolyBufferEnd)
                    {
                        pPoly = reinterpret_cast<unsigned int *>(newppoly);
                        setPolyF3(leftpoly);
                        setPolyF3(rightpoly);
                        setRGB0(leftpoly, 0xff, 0, 0);
                        leftpoly->x0 = static_cast<unsigned short>(mX) - off;
                        leftpoly->y0 = y;
                        leftpoly->x1 = leftpoly->x0 - 0x14;
                        leftpoly->y1 = y - 6;
                        leftpoly->x2 = leftpoly->x1;
                        leftpoly->y2 = y + 6;
                        setRGB0(rightpoly, 0xff, 0, 0);
                        rightpoly->x0 = static_cast<unsigned short>(mX) + off;
                        rightpoly->y0 = y;
                        rightpoly->x1 = rightpoly->x0 + 0x14;
                        rightpoly->y1 = y - 6;
                        rightpoly->x2 = rightpoly->x1;
                        rightpoly->y2 = y + 6;
                        addPrim(pDoubleBuffer->PanelOT, leftpoly);
                        addPrim(pDoubleBuffer->PanelOT, rightpoly);
                    }
                }
            }

            y += mLineSep;
        }
    }
}

/**
 * Update__5CMenu
 *
 * Address:   0x80026CBC
 * Mangled:   Update__5CMenu
 * Demangled: CMenu::Update(void)
 * Advances the visible menu selection from controller input and eases every
 * entry toward its selected or unselected display scale.
 */
void CMenu::Update()
{
    while (mEntry[mLine].Visible == 0)
    {
        mEntry[mLine].TargetScale = mEntry[mLine].LowScale;
        mEntry[mLine].CurrentScale = mEntry[mLine].LowScale;
        mLine++;
        if (mNumLines <= mLine)
            mLine = 0;
        mEntry[mLine].TargetScale = mEntry[mLine].HiScale;
        mEntry[mLine].CurrentScale = mEntry[mLine].LowScale;
    }

    if (Pad[0].Up.Pressed != 0 || Pad[0].Down.Pressed != 0 || Pad[1].Up.Pressed != 0 || Pad[1].Down.Pressed != 0)
    {
        if (UpDownAutoRepeatTimer == 0 || (UpDownAutoRepeatTimer > 0xf && (UpDownAutoRepeatTimer & 3) == 0))
        {
            if (Pad[0].Up.Pressed != 0 || Pad[1].Up.Pressed != 0)
            {
                mEntry[mLine].TargetScale = mEntry[mLine].LowScale;
                do
                {
                    if (mLine != 0)
                    {
                        mLine--;
                    }
                    else
                    {
                        mLine = mNumLines - 1;
                    }
                } while (mEntry[mLine].Visible == 0 || mEntry[mLine].Enabled == 0);
                mEntry[mLine].TargetScale = mEntry[mLine].HiScale;
                SFX_PlayX(6, 0x2000, 0);
            }

            if (Pad[0].Down.Pressed != 0 || Pad[1].Down.Pressed != 0)
            {
                mEntry[mLine].TargetScale = mEntry[mLine].LowScale;
                do
                {
                    mLine++;
                    if (mNumLines <= mLine)
                        mLine = 0;
                } while (mEntry[mLine].Visible == 0 || mEntry[mLine].Enabled == 0);
                mEntry[mLine].TargetScale = mEntry[mLine].HiScale;
                SFX_PlayX(6, 0x2000, 0);
            }
        }
        UpDownAutoRepeatTimer++;
    }
    else
    {
        UpDownAutoRepeatTimer = 0;
    }

    int i = 0;
    while (i < mNumLines)
    {
        if (mEntry[i].CurrentScale < mEntry[i].TargetScale)
        {
            mEntry[i].CurrentScale += 0x28;
            if (mEntry[i].CurrentScale > mEntry[i].TargetScale)
            {
                mEntry[i].CurrentScale = mEntry[i].TargetScale;
            }
        }
        if (mEntry[i].CurrentScale > mEntry[i].TargetScale)
        {
            mEntry[i].CurrentScale -= 0x28;
            if (mEntry[i].CurrentScale < mEntry[i].TargetScale)
            {
                mEntry[i].CurrentScale = mEntry[i].TargetScale;
            }
        }
        i++;
    }
}

/**
 *
 * Address: 0x80027044
 * Mangled: Front_ClearScreen__Fv
 *
 */
/**
 * Clears the front-end draw buffers using the saved sky color, then clears the
 * final scanline used by the display setup.
 */
void Front_ClearScreen(void)
{
    GameFade = 0;
    RECT screen = Db_SkyColorBak[0];
    ClearImage(&screen, 0, 0, 0);
    DrawSync(0);
    screen.y = 0x1ff;
    screen.h = 1;
    ClearImage(&screen, 0, 0, 0);
    DrawSync(0);
}

/**
 * Front_SavePreferences__FR12SPreferences
 *
 * Address:   0x800270DC
 * Mangled:   Front_SavePreferences__FR12SPreferences
 * Demangled: Front_SavePreferences(SPreferences &)
 *            ASM: jr $ra; nop
 *
 * @param Pref preferences destination reserved by the prototype build
 */
void Front_SavePreferences(SPreferences &Pref)
{
}

/**
 * Front_SetPreferences__FRC12SPreferences
 *
 * Address:   0x800270E4
 * Mangled:   Front_SetPreferences__FRC12SPreferences
 * Demangled: Front_SetPreferences(const SPreferences &)
 *            ASM: jr $ra; nop
 *
 * @param Pref preferences source reserved by the prototype build
 */
void Front_SetPreferences(const SPreferences &Pref)
{
}

/**
 * Front_Init__Fv
 *
 * Address:   0x800270EC
 * Mangled:   Front_Init__Fv
 * Demangled: Front_Init(void)
 *            Initializes all front-end menus: copies default preferences, creates CMenu objects,
 *            adds entries for paused/restart/yes-no/end-level/video/sound menus
 */
void Front_Init(void)
{
    pSizeData = reinterpret_cast<const unsigned int *>(&SizesFile);

    StartupGameState = DefaultGameState;
    LastGameState = DefaultGameState;

    pYesNoMenu = new CMenu(0x100, 0, 0, 0x14f, 0x100, 0x16);
    pYesNoMenu->AddEntry(GameText.pYes);
    pYesNoMenu->AddEntry(GameText.pNo);

    pESMenu = new CMenu(0x100, 0, 0, 0x14f, 0x100, 0x16);
    pESMenu->AddEntry(GameText.pUpdateSaveFile);
    pESMenu->AddEntry(
        ((Levels[GLevel].isCompetition != 0) && (GGame == 1))
            ? GameText.pRestart
            : GameText.pRetry);
    pESMenu->AddEntry(GameText.pLevelHints);
    pESMenu->AddEntry(GameText.pSelectLevel);
    pESMenu->AddEntry(GameText.pEquipmentSelect);
    pESMenu->AddEntry(GameText.pWatchReplay);
    pESMenu->AddEntry(GameText.pSaveCareer);
    pESMenu->AddEntry(GameText.pQuit);
    if (((GGame != 1) && (pESMenu->EntryOff(GameText.pSaveCareer), GGame != 1)) ||
        (Levels[GLevel].isCompetition != 0))
    {
        pESMenu->EntryOff(GameText.pLevelHints);
    }
    if (GGame != 3)
    {
        pESMenu->EntryOff(GameText.pUpdateSaveFile);
    }
    if ((GGame == 2) || (GGame == 7))
    {
        pESMenu->EntryOff(GameText.pWatchReplay);
    }
    pESMenu->EntryOff(GameText.pWatchReplay);
    pESMenu->EntryOff(GameText.pEquipmentSelect);
    pESMenu->EntryOff(GameText.pUpdateSaveFile);
    pESMenu->EntryOff(GameText.pSelectLevel);
    pESMenu->CentreY();
    pESMenu->SetLine(pESMenu->FindEntry(
        ((Levels[GLevel].isCompetition != 0) && (GGame == 1))
            ? GameText.pRestart
            : GameText.pRetry));

    pVSMenu = new CMenu(0x100, 0, 0, 0x14f, 0x100, 0x16);
    pVSMenu->AddEntry(GameText.pFullReplay);
    pVSMenu->AddEntry(GameText.pHighlights);
    pVSMenu->AddEntry(GameText.pSaveVideo);
    pVSMenu->AddEntry(GameText.pExit);
    pVSMenu->CentreY();

    pVS2Menu = new CMenu(0x100, 0, 0, 0x14f, 0x100, 0x16);
    pVS2Menu->AddEntry(GameText.pFullReplay);
    pVS2Menu->AddEntry(GameText.pHighlights);
    pVS2Menu->AddEntry(GameText.pExit);
    pVS2Menu->CentreY();

    pVS3Menu = new CMenu(0x100, 0, 0, 0x14f, 0x100, 0x16);
    pVS3Menu->AddEntry(GameText.pFullReplay);
    pVS3Menu->AddEntry(GameText.pHighlights);
    pVS3Menu->AddEntry(GameText.pSaveVideo);
    pVS3Menu->AddEntry(GameText.pReturnToCompetition);
    pVS3Menu->CentreY();

    pEHMenu = new CMenu(0x100, 0, 0, 0x14f, 0x100, 0x16);
    pEHMenu->AddEntry(GameText.pNextHeat);
    pEHMenu->AddEntry(GameText.pWatchReplay);
    pEHMenu->AddEntry(GameText.pEndComp);
    pEHMenu->AddEntry(GameText.pQuit);
    pEHMenu->CentreY();

    pERMMenu = new CMenu(0x100, 0, 0, 0x14f, 0x100, 0x16);
    pERMMenu->AddEntry(GameText.pNextPlayer);
    pERMMenu->AddEntry(GameText.pWatchReplay);
    pERMMenu->AddEntry(GameText.pEndComp);
    pERMMenu->AddEntry(GameText.pQuit);
    pERMMenu->CentreY();

    pSoundMenu = new CMenu(0x100, 0, 0, 0x14f, 0x100, 0x16);
    soundVol = ((reinterpret_cast<SCareerOwner *>(&GCareer)->Options.SFXVolume + 1) * 10) / 12;
    sprintf(soundBuf, GameText.pSFXLevel, soundVol);
    pSoundMenu->AddEntry(soundBuf);
    musicVol = ((reinterpret_cast<SCareerOwner *>(&GCareer)->Options.MusicVolume + 1) * 10) / 12;
    sprintf(musicBuf, GameText.pMusicLevel, musicVol);
    pSoundMenu->AddEntry(musicBuf);
    pSoundMenu->AddEntry(GameText.pSkipTrack);
    pSoundMenu->AddEntry(GameText.pBackToMainMenu);
    pSoundMenu->CentreY();
    GCompoSounded = 0;
}

/** Draws the paused-screen sprite in the front-end ordering table. */
void PrintPaused(void)
{
    OT_entry = 1;
    reinterpret_cast<Sprite2 *>(pPausedSprite)->Display(
        0x100, 0x28, 0);
}

/**
 * Front_PrintGoBack__Fv
 *
 * Address:   0x800278C0
 * Mangled:   Front_PrintGoBack__Fv
 * Demangled: Front_PrintGoBack(void)
 *
 */
void Front_PrintGoBack(void)
{
    Mess_SetTextJustify(1);
    Mess_SetScale(0xc0);
    Mess_SetRGB(0x96, 0x64, 0x19, 0);
    Mess_DrawText(0x5c, 0xd6, GameText.pToGoBack, 0, 0, 0x100, 0);
    Mess_SetTextJustify(0);
    POLY_FT4 *pAPoly = Panel_DrawTexturedPoly(ButtonsAnim + 1);
    if (pAPoly != 0)
    {
        Panel_SetStretchedScreenCoords(0x40, 0xd3, pAPoly, ButtonsAnim + 1);
    }
}

/**
 * Front_PrintSelect__Fv
 *
 * Address:   0x80027954
 * Mangled:   Front_PrintSelect__Fv
 * Demangled: Front_PrintSelect(void)
 *
 */
void Front_PrintSelect(void)
{
    Mess_SetTextJustify(1);
    Mess_SetScale(0xc0);
    Mess_SetRGB(0x96, 0x64, 0x19, 0);
    Mess_DrawText(0x15c, 0xd6, GameText.pToSelect, 0, 0, 0x100, 0);
    Mess_SetTextJustify(0);
    POLY_FT4 *pAPoly = Panel_DrawTexturedPoly(ButtonsAnim);
    if (pAPoly != 0)
    {
        Panel_SetStretchedScreenCoords(0x140, 0xd3, pAPoly, ButtonsAnim);
    }
}

/**
 * Front_PrintGoBackSelect__Fv
 *
 * Address:   0x800279E4
 * Mangled:   Front_PrintGoBackSelect__Fv
 * Demangled: Front_PrintGoBackSelect(void)
 *            Calls Front_PrintGoBack() then Front_PrintSelect()
 *
 */
void Front_PrintGoBackSelect(void)
{
    Front_PrintGoBack();
    Front_PrintSelect();
}

/**
 * Front_PrintCentredPrompt__FPCc
 *
 * Address:   0x80027A0C
 * Mangled:   Front_PrintCentredPrompt__FPCc
 * Demangled: Front_PrintCentredPrompt(const char *)
 *
 * @param Text prompt text to draw
 */
void Front_PrintCentredPrompt(const char *Text)
{
    Mess_SetTextJustify(1);
    Mess_SetScale(0xc0);
    Mess_SetRGB(0x96, 0x64, 0x19, 0);
    Mess_DrawText(0xe3, 0xd6, Text, 0, 0, 0x100, 0);
    POLY_FT4 *pAPoly = Panel_DrawTexturedPoly(ButtonsAnim);
    if (pAPoly != 0)
    {
        Panel_SetStretchedScreenCoords(0xc0, 0xd3, pAPoly, ButtonsAnim);
    }
}

/**
 * Graffiti_Stats__FP6CBruceiiiiiiiii
 *
 * Address:   0x80027A98
 * Mangled:   Graffiti_Stats__FP6CBruceiiiiiiiii
 * Demangled: Graffiti_Stats(CBruce *, int, int, int, int, int, int, int, int, int)
 *
 * @param pSkater  Active skater being processed.
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 * @param Win number of graffiti wins
 * @param Lose number of graffiti losses
 * @param Streak current win streak
 * @param High highest tag count
 */
int Graffiti_Stats(CBruce *pSkater, int x, int y, int r, int g, int b, int Win, int Lose, int Streak, int High)
{
    char N[16];
    int scale = 0x100;
    Mess_SetRGB(r & 0xff, g, b, 0);
    Mess_SetTextJustify(1);
    int drawX = x + 0x14;
    Mess_DrawText(drawX, y, GrafLabel1, 0, 0, scale, 0);
    int y2 = y + 0x11;
    Mess_DrawText(drawX, y2, GrafLabel2, 0, 0, scale, 0);
    int y3 = y + 0x33;
    Mess_DrawText(drawX, y3, GrafLabel3, 0, 0, scale, 0);
    int y4 = y + 0x44;
    Mess_DrawText(drawX, y4, GrafLabel4, 0, 0, scale, 0);
    int y5 = y + 0x55;
    Mess_DrawText(drawX, y5, GrafLabel5, 0, 0, scale, 0);
    int y6 = y + 0x66;
    Mess_DrawText(drawX, y6, GrafLabel6, 0, 0, scale, 0);
    Mess_SetTextJustify(2);
    const char *fmt = GrafFmt;
    sprintf(N, fmt, pSkater->mTags);
    int drawX2 = x + 0x88;
    Mess_DrawText(drawX2, y, N, 0, 0, scale, 0);
    sprintf(N, fmt, pSkater->mSteals);
    Mess_DrawText(drawX2, y2, N, 0, 0, scale, 0);
    sprintf(N, fmt, High);
    Mess_DrawText(drawX2, y3, N, 0, 0, scale, 0);
    sprintf(N, fmt, Win);
    Mess_DrawText(drawX2, y4, N, 0, 0, scale, 0);
    sprintf(N, fmt, Lose);
    Mess_DrawText(drawX2, y5, N, 0, 0, scale, 0);
    sprintf(N, fmt, Streak);
    Mess_DrawText(drawX2, y6, N, 0, 0, scale, 0);
}

/** Storage for the debug-backed, compiler-elided Font1 local in Front_Display. */
typedef unsigned int FrontDisplayFontStorage[sizeof(Font) / sizeof(unsigned int)];


/**
 * Front_Display__Fv
 *
 * Address:   0x80027D4C
 * Mangled:   Front_Display__Fv
 * Demangled: Front_Display(void)
 *            Massive switch on (Status-1) with 46 cases — display logic for pause/menus/competition
 *
 */
void Front_Display(void)
{
    CBruce *pMan;
    static char Score1[20] asm("Score1.380");
    static char Score2[20] asm("Score2.381");
    /** `Front_Display`'s target-named function-local small-BSS buffers. */
    static char Tapes[5] asm("Tapes.382");
    static char Tricks[8] asm("Tricks.383");
    static char Bails[8] asm("Bails.384");
    static char BestCombo[40] asm("BestCombo.385");
    int Line;

    if (Cheat_GameInfo != 0)
    {
        Panel_DrawFlatShadedPoly(0x13, 0xbe, 0xc2, 0x2a, 0, 0, 0, 0);
    }
    if (GamePaused != 0)
    {
        Shell_PauseScreen();
    }
    Mess_SetTextJustify(0);
    Mess_SetScale(0x100);

    switch (Status)
    {
        int Level;

    case STATUS_MEMCARDMENUS:
        Front_MemCardDisplay();
        break;
    case STATUS_PAUSEDMENU:
        PrintPaused();
        if (Cheat_GameInfo != 0)
        {
            Front_PrintSelect();
        }
        else
        {
            Front_PrintGoBackSelect();
        }
        {
            if (WobbleAmplitude != 0)
            {
                WobbleAmplitude -= 2;
                WobbleT += 0x2b2;
                if (WobbleAmplitude < 0)
                {
                    WobbleAmplitude = 0;
                    WobbleT = 0;
                }
            }
            pPausedMenu->mX =
                WobbleAmplitude *
                    (&rcossin_tbl)[WobbleT & 0xfff].Cos /
                    0x1000 +
                0x100;
            pPausedMenu->Display();
        }
        break;
    case STATUS_SKIPTORESTART:
        if (pRestartsMenu != 0)
        {
            pRestartsMenu->Display();
        }
        break;
    case STATUS_REALLYQUIT:
        PrintPaused();
        Front_PrintGoBackSelect();
        Mess_SetRGB(0x96, 0x64, 0x19, 0);
        Mess_DrawText(0x100, 0x5f, GameText.pReallyQuit, 0, 0, 0x100, 0);
        pYesNoMenu->mY = 0x87;
        pYesNoMenu->Display();
        break;
    case STATUS_PADUNPLUGGED:
        PrintPaused();
        Mess_SetRGB(0xb4, 0x0a, 0x0a, 0);
        if (((TTime / 10) & 1) != 0)
        {
            Mess_DrawText(0x100, 0xb8, GameText.pPleaseReinsertController, 0, 0, 0x100, 0);
        }
        break;
    case STATUS_ENDLEVELFADESCREEN:
    case STATUS_DOSAVEFADESCREEN:
    case STATUS_NOSAVEFADESCREEN:
        break;
    case STATUS_GOTTAPE:
        {
            int NumTapes = GSkater[0]->mSessionTapes;
            if (GTape[0] == 0)
            {
                SFX_PlayX(0x13, 0x2000, 0);
                switch (NumTapes)
                {
                case 1:
                    GTape[0] = Tape_Create(
                        0, 0, 0x8c, 0x900,
                        -0x50, 0, 0x8c, 0x400);
                    break;
                case 2:
                    GTape[0] = Tape_Create(
                        -0x32, 0, 0x8c, 0x880,
                        -0x50, -0x19, 0x8c, 0x400);
                    GTape[1] = Tape_Create(
                        0x32, 0, 0x8c, 0x880,
                        -0x50, 0x19, 0x8c, 0x400);
                    break;
                case 3:
                    GTape[0] = Tape_Create(
                        0, 0, 0x8c, 0x900,
                        -0x50, -0x28, 0x8c, 0x400);
                    GTape[1] = Tape_Create(
                        -0x3c, 0, 0x8c, 0x700,
                        -0x50, 0, 0x8c, 0x400);
                    GTape[2] = Tape_Create(
                        0x3c, 0, 0x8c, 0x700,
                        -0x50, 0x28, 0x8c, 0x400);
                    break;
                case 4:
                    GTape[0] = Tape_Create(
                        -0x2d, -0x19, 0x8c, 0x700,
                        -0x50, -0x32, 0x8c, 0x400);
                    GTape[1] = Tape_Create(
                        0x2d, -0x19, 0x8c, 0x700,
                        -0x46, -0x14, 0x8c, 0x400);
                    GTape[2] = Tape_Create(
                        -0x2d, 0x19, 0x8c, 0x700,
                        -0x50, 0x0a, 0x8c, 0x400);
                    GTape[3] = Tape_Create(
                        0x2d, 0x19, 0x8c, 0x700,
                        -0x46, 0x28, 0x8c, 0x400);
                    break;
                case 5:
                    GTape[0] = Tape_Create(
                        0, 0, 0x8c, 0x900,
                        -0x50, -0x32, 0x8c, 0x400);
                    GTape[1] = Tape_Create(
                        -0x37, -0x19, 0x8c, 0x600,
                        -0x46, -0x19, 0x8c, 0x400);
                    GTape[2] = Tape_Create(
                        0x37, -0x19, 0x8c, 0x600,
                        -0x50, 0, 0x8c, 0x400);
                    GTape[3] = Tape_Create(
                        -0x37, 0x19, 0x8c, 0x600,
                        -0x46, 0x19, 0x8c, 0x400);
                    GTape[4] = Tape_Create(
                        0x37, 0x19, 0x8c, 0x600,
                        -0x50, 0x32, 0x8c, 0x400);
                    break;
                }
            }
            for (int i = 0; i < 5; i++)
            {
                if (GTape[i] != 0)
                {
                    GTape[i]->mTapeNum =
                        reinterpret_cast<SCareerOwner *>(&GCareer)->CharacterProgress[
                            GSkater[0]->mSkaterType].NumTapes -
                        (NumTapes - i);
                    GTape[i]->mSessionTapeNum = i;
                }
            }
            for (int i = 0; i < 5; i++)
            {
                if (GTape[i] != 0)
                {
                    GTape[i]->AI(
                        reinterpret_cast<SCareerOwner *>(&GCareer)->CharacterProgress[
                            GSkater[0]->mSkaterType].NumTapes -
                            (NumTapes - i));
                }
            }
            Mess_SetScale(0x100);
            Mess_SetRGB(0x7a, 0x7a, 0x7a, 0);
            if (CTape::DisplayTitle() != 0)
            {
                if (NumTapes == 1)
                {
                    Mess_DrawText(0x100, 0x28,
                                  GameText.pYouGotATape,
                                  0, 0, 0x100, 0);
                }
                else
                {
                    static char GotATape[25] asm("GotATape.386");
                    sprintf(GotATape, GameText.pYouGotNTapes, NumTapes);
                    Mess_DrawText(0x100, 0x28, GotATape,
                                  0, 0, 0x100, 0);
                }
            }
            if (CTape::DisplayContinueButton() != 0)
            {
                Front_PrintCentredPrompt(GameText.pContinue);
            }
        }
        break;
    case STATUS_QUITSAVE:
    case STATUS_MEDALSAVE:
    case STATUS_TAPESAVE:
        Mess_SetRGB(0x96, 0x64, 0x19, 0);
        Mess_DrawText(0x100, 0x50, GameText.pSaveCareerQM, 0, 0, 0x100, 0);
        pYesNoMenu->mY = 0x82;
        pYesNoMenu->Display();
        Front_PrintCentredPrompt(GameText.pToSelect);
        break;
    case STATUS_REALLYMAINMENU:
        Mess_SetRGB(0x96, 0x64, 0x19, 0);
        Mess_DrawText(0x100, 0x50, GameText.pAreYouSure, 0, 0, 0x100, 0);
        pYesNoMenu->mY = 0x82;
        pYesNoMenu->Display();
        Front_PrintCentredPrompt(GameText.pToSelect);
        break;
    case STATUS_ENDINGSCREEN:
        if ((GRollCredits == 0) && (Front_OnTime >= 0xb))
        {
            pESMenu->Display();
            Front_PrintCentredPrompt(GameText.pToSelect);
        }
        break;
    case STATUS_VIDEOSCREEN:
        if (Front_OnTime >= 0xb)
        {
            if (GNumberOfPlayers == 2)
            {
                pVSMenu->EntryOff(GameText.pHighlights);
            }
            else
            {
                pVSMenu->EntryOn(GameText.pHighlights);
            }
            pVSMenu->CentreY();
            pVSMenu->Display();
            Front_PrintGoBackSelect();
        }
        break;
    case STATUS_VIDEOSCREEN2:
        if (Front_OnTime >= 0xb)
        {
            if (GNumberOfPlayers == 2)
            {
                pVS2Menu->EntryOff(GameText.pHighlights);
            }
            else
            {
                pVS2Menu->EntryOn(GameText.pHighlights);
            }
            pVS2Menu->CentreY();
            pVS2Menu->Display();
            Front_PrintGoBackSelect();
        }
        break;
    case STATUS_CHANGEHORSEWORD:
        {
            char horseBuf[2];
            int termFound = 0;
            Mess_SetTextJustify(0);
            Mess_SetRGB(0x7a, 0x7a, 0x7a, 0);
            Mess_DrawText(0x100, 0x50, GameText.pHorsePlayTo, 0, 0, 0x100, 0);
            horseBuf[1] = 0;
            for (int i = 0; i < 10; i++)
            {
                if (SizeUp > 0)
                {
                    SizeUp--;
                }
                if (reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord[i] == 0)
                {
                    termFound = 1;
                }
                horseBuf[0] = reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord[i];
                if (i == GHorseCursorPos)
                {
                    Mess_SetRGB(0xb6, 0x84, 0x24, 0);
                    Mess_SetScale(SizeUp * 6 + 0x100);
                }
                else
                {
                    Mess_SetRGB(0x7a, 0x7a, 0x7a, 0);
                    Mess_SetScale(0x100);
                }
                if (termFound == 0)
                {
                    Mess_DrawText(i * 0x14 + 0x9c, 0x78, horseBuf,
                                  0, 0, 0x100, 0);
                }
                horseBuf[0] = '_';
                Mess_SetScale(0x100);
                Mess_DrawText(i * 0x14 + 0x9c, 0x82, horseBuf,
                              0, 0, 0x100, 0);
            }
            Front_PrintCentredPrompt(GameText.pToAccept);
        }
        break;
    case STATUS_STARTHORSE:
        Mess_SetScale(0x100);
        {
            if (GHorsePlayer == 0)
            {
                Mess_SetRGB(0x50, 0x50, 0x80, 0);
            }
            else
            {
                Mess_SetRGB(0x80, 0x50, 0x50, 0);
            }
            if (GHorseMode == 0)
            {
                if (GHorsePlayer == 0)
                {
                    Mess_DrawText(0x100, 0x78,
                                  Front_Player1SetScoreText(),
                                  0, 0, 0x100, 0);
                }
                else
                {
                    Mess_DrawText(0x100, 0x78,
                                  Front_Player2SetScoreText(),
                                  0, 0, 0x100, 0);
                }
            }
            else
            {
                char Score[10];
                if (GHorsePlayer == 0)
                {
                    Mess_DrawText(0x100, 0x78,
                                  GrafFmt + 4,
                                  0, 0, 0x100, 0);
                }
                else
                {
                    Mess_DrawText(0x100, 0x78,
                                  GrafFmt + 0x14,
                                  0, 0, 0x100, 0);
                }
                sprintf(Score, GrafFmt, GHorseScore);
                Mess_DrawText(0x100, 0x8c, Score,
                              0, 0, 0x100, 0);
            }

            Horse_DisplayWords(GHorseLetters);
            Front_PrintCentredPrompt(GameText.pContinue);
        }
        break;
    case STATUS_LOADSUCCESS:
    case STATUS_LOADFAILED:
        break;
    case STATUS_HIGHSCORE:
        {
            static char highScoreBuf[64] asm("highScoreBuf.387");
            int i;
            Mess_SetScale(0x100);

            Level = 8;
            if (GenerateLevel == 0)
            {
                Level = GLevel;
            }

            sprintf(highScoreBuf, GameText.pHighScores, Levels[Level].pLevelName);
            Mess_SetRGB(0xb6, 0x84, 0x24, 0);
            Mess_DrawText(0x100, 0x28, highScoreBuf, 0, 0, 0x100, 0);
            Mess_SetRGB(0x7a, 0x7a, 0x7a, 0);

            for (i = 0; i < 8; i++)
            {
                StatsLeft[i] += 0x1e;
                if (StatsLeft[i] > 0xb4)
                {
                    StatsLeft[i] = 0xb4;
                }

                StatsRight[i] -= 0x1e;
                if (StatsRight[i] < 0x19c)
                {
                    StatsRight[i] = 0x19c;
                }

                if (i == GHighScoreLineIndex)
                {
                    Mess_SetRGB(0xb6, 0x84, 0x24, 0);
                }
                else
                {
                    Mess_SetRGB(0x7a, 0x7a, 0x7a, 0);
                }

                if (SizeUp > 0)
                {
                    SizeUp--;
                }

                highScoreBuf[0] =
                    reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][i].Name[0];
                highScoreBuf[1] = 0;
                Mess_SetTextJustify(0);
                if ((i == GHighScoreLineIndex) &&
                    (GHighScoreInitialCursorPos == 0))
                {
                    Mess_SetScale(SizeUp * 6 + 0x100);
                }
                else
                {
                    Mess_SetScale(0x100);
                }
                Mess_DrawText(StatsLeft[i] - 0x50,
                              (i + 1) * 0x13 + 0x28, highScoreBuf,
                              0, 0, 0x100, 0);

                Mess_SetScale(0x100);
                highScoreBuf[0] =
                    reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][i].Name[1];
                Mess_SetTextJustify(0);
                if ((i == GHighScoreLineIndex) &&
                    (GHighScoreInitialCursorPos == 1))
                {
                    Mess_SetScale(SizeUp * 6 + 0x100);
                }
                else
                {
                    Mess_SetScale(0x100);
                }
                Mess_DrawText(StatsLeft[i] - 0x3c,
                              (i + 1) * 0x13 + 0x28, highScoreBuf,
                              0, 0, 0x100, 0);

                Mess_SetScale(0x100);
                highScoreBuf[0] =
                    reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][i].Name[2];
                Mess_SetTextJustify(0);
                if ((i == GHighScoreLineIndex) &&
                    (GHighScoreInitialCursorPos == 2))
                {
                    Mess_SetScale(SizeUp * 6 + 0x100);
                }
                else
                {
                    Mess_SetScale(0x100);
                }
                Mess_DrawText(StatsLeft[i] - 0x28,
                              (i + 1) * 0x13 + 0x28, highScoreBuf,
                              0, 0, 0x100, 0);

                Mess_SetScale(0x100);
                if ((i == GHighScoreLineIndex) &&
                    (((TTime / 10) & 1) != 0))
                {
                    sprintf(highScoreBuf,
                            GrafFmt + 0x24);
                    Mess_SetTextJustify(0);
                    int cursorOffset =
                        GHighScoreInitialCursorPos * 0x14 - 0x50;
                    Mess_DrawText(StatsLeft[i] + cursorOffset,
                                  (i + 1) * 0x13 + 0x30,
                                  highScoreBuf, 0, 0, 0x100, 0);
                }

                sprintf(highScoreBuf,
                        GrafFmt + 0x28,
                        Stats[static_cast<unsigned char>(
                                  reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][i].Skater)]
                            .Name);
                char *p = highScoreBuf;
                while (*p != 0)
                {
                    if (*p == ' ')
                    {
                        *p = 0;
                        break;
                    }
                    p++;
                }

                Mess_SetTextJustify(1);
                Mess_DrawText(StatsLeft[i],
                              (i + 1) * 0x13 + 0x28, highScoreBuf,
                              0, 0, 0x100, 0);

                sprintf(highScoreBuf, GrafFmt,
                        reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][i].Score);
                for (int j = strlen(highScoreBuf) - 1; j >= 0; j--)
                {
                    char temp[2];
                    temp[0] = highScoreBuf[j];
                    temp[1] = 0;
                    Mess_SetTextJustify(0);
                    int scoreLength = strlen(highScoreBuf);
                    int charactersToRight = j + 1;
                    Mess_DrawText(StatsRight[i] -
                                      (scoreLength - charactersToRight) * 0x12,
                                  (i + 1) * 0x13 + 0x28, temp,
                                  0, 0, 0x100, 0);
                }
            }

            Front_PrintCentredPrompt(GameText.pToAccept);
        }
        break;
    case STATUS_STATS:
        {
            Mess_SetScale(0x100);
            if (GNumberOfPlayers == 1)
            {
                Mess_SetRGB(0xb6, 0x84, 0x24, 0);
                Mess_DrawText(0x100, 0x28, GameText.pStatistics,
                              0, 0, 0x100, 0);
            }
            else
            {
                Mess_SetRGB(0xb6, 0x84, 0x24, 0);
                if (GGame == 7)
                {
                    Mess_DrawText(0x100, 0x28,
                                  GrafFmt + 0x2c,
                                  0, 0, 0x100, 0);
                }
                else
                {
                    Mess_DrawText(0x100, 0x28,
                                  GrafFmt + 0x38,
                                  0, 0, 0x100, 0);
                }
            }

            Mess_SetRGB(0x7a, 0x7a, 0x7a, 0);
            Mess_SetScale(0x100);

            if (GNumberOfPlayers == 1)
            {
                int Line;
                int Level;
                Mess_SetTextJustify(1);
                for (int i = 0; i < 5; i++)
                {
                    StatsLeft[i] += 0x1e;
                    if (StatsLeft[i] > 0x5a)
                    {
                        StatsLeft[i] = 0x5a;
                    }
                }

                Level = 8;
                if (GenerateLevel == 0)
                {
                    Level = GLevel;
                }

                Mess_DrawText(StatsLeft[0], 0x3c, GameText.pTotalScore,
                              0, 0, 0x100, 0);
                Line = 2;
                Mess_DrawText(StatsLeft[1], 0x4e, GameText.pHighScore,
                              0, 0, 0x100, 0);
                if (GGame == 1)
                {
                    Line = 3;
                    Mess_DrawText(StatsLeft[2], 0x60,
                                  GameText.pTapesCollected,
                                  0, 0, 0x100, 0);
                }
                Mess_DrawText(StatsLeft[3], Line++ * 0x12 + 0x3c,
                              GameText.pTricksLanded,
                              0, 0, 0x100, 0);
                Mess_DrawText(StatsLeft[4], Line * 0x12 + 0x3c,
                              GameText.pBails, 0, 0, 0x100, 0);

                Mess_SetTextJustify(0);
                pMan = reinterpret_cast<CBruce *>(MechList);
                Mess_SetTextJustify(2);
                for (int i = 0; i < 5; i++)
                {
                    StatsRight[i] -= 0x1e;
                    if (StatsRight[i] < 0x19a)
                    {
                        StatsRight[i] = 0x19a;
                    }
                }

                sprintf(Score1, GrafFmt,
                        pMan->mPanelData.mTotalScore);
                Line = 2;
                Mess_DrawText(StatsRight[0], 0x3c, Score1,
                              0, 0, 0x100, 0);

                sprintf(Score2, GrafFmt,
                        reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][0].Score);
                Mess_DrawText(StatsRight[1], 0x4e, Score2,
                              0, 0, 0x100, 0);

                if (GGame == 1)
                {
                    Line = 3;
                    int NumTapes = Career_LevelTapes(GLevel);
                    sprintf(Tapes,
                            Front_GrafCountFormat,
                            NumTapes, 5);
                    Mess_DrawText(StatsRight[2], 0x60, Tapes,
                                  0, 0, 0x100, 0);
                }

                sprintf(Tricks,
                        Front_GrafCountFormat + 0xc,
                        static_cast<unsigned int>(
                            pMan->mPanelData.mNumTricksLanded));
                Mess_DrawText(StatsRight[3], Line++ * 0x12 + 0x3c,
                              Tricks,
                              0, 0, 0x100, 0);

                sprintf(Bails,
                        Front_GrafCountFormat + 0xc,
                        static_cast<unsigned int>(
                            pMan->mPanelData.mNumBails));
                Mess_DrawText(StatsRight[4],
                              Line * 0x12 + 0x3c, Bails,
                              0, 0, 0x100, 0);

                if (StatsLeft[0] == 0x5a)
                {
                    Mess_SetTextJustify(0);
                    if (pMan->mPanelData.mBestComboScore == 0)
                    {
                        Mess_SetScale(0xdc);
                        Mess_DrawText(0x100, 0x97, GameText.pBestCombo,
                                      0, 0, 0x100, 0);
                        Mess_SetScale(0x100);
                        Mess_DrawText(0x100, 0xa8, GameText.pNone,
                                      0, 0, 0x100, 0);
                    }
                    else
                    {
                        if (pMan->mPanelData.mNumTricksInCombo == 1)
                        {
                            sprintf(
                                BestCombo,
                                GameText.pBestComboNTrickNPoints,
                                pMan->mPanelData.mNumTricksInCombo,
                                pMan->mPanelData.mBestComboScore);
                        }
                        else
                        {
                            sprintf(
                                BestCombo,
                                GameText.pBestComboNTricksNPoints,
                                pMan->mPanelData.mNumTricksInCombo,
                                pMan->mPanelData.mBestComboScore);
                        }
                        FrontDisplayFontStorage Font1;

                        Mess_SetScale(0xdc);
                        Mess_DrawText(0x100, 0x97, BestCombo,
                                      0, 0, 0x100, 0);
                        Mess_SetScale(0xbc);
                        Mess_DrawText(0x100, 0xa6,
                                      pMan->mPanelData.mBestComboName,
                                      0, 0, 0x100, 0);
                        Mess_DrawText(0x100, 0xb4,
                                      pMan->mPanelData.mBestComboName2,
                                      0, 0, 0x100, 0);
                        Mess_DrawText(0x100, 0xc2,
                                      pMan->mPanelData.mBestComboName3,
                                      0, 0, 0x100, 0);
                    }
                }
            }
            else
            {
                Mess_SetScale(0x100);
                int s1 = GSkater[0]->mPanelData.mTotalScore;
                int s2 = 0;
                if (GSkater[1] != 0)
                {
                    s2 = GSkater[1]->mPanelData.mTotalScore;
                }

                switch (GGame)
                {
                    char Score[40];

                case 8:
                    if (GSkater[1]->mTags == GSkater[0]->mTags)
                    {
                        Mess_DrawText(0x100, 0x40,
                                      GrafFmt + 0x74,
                                      0, 0, 0x100, 0);
                    }
                    else if (GSkater[0]->mTags < GSkater[1]->mTags)
                    {
                        Mess_DrawText(0x100, 0x40,
                                      Front_GrafPlayer2Wins,
                                      0, 0, 0x100, 0);
                    }
                    else
                    {
                        Mess_DrawText(0x100, 0x40,
                                      Front_GrafPlayer1Wins,
                                      0, 0, 0x100, 0);
                    }
                    Mess_SetScale(0xe0);
                    Graffiti_Stats(GSkater[0], 0x1e, 0x56,
                                   0x20, 0x20, 0x80,
                                   GWins1, GLosses1,
                                   GStreak1, GHighTags1);
                    Graffiti_Stats(GSkater[1], 0x11e, 0x56,
                                   0x80, 0x20, 0x20,
                                   GWins2, GLosses2,
                                   GStreak2, GHighTags2);
                    break;
                case 7:
                    NextHorseMode = GHorseMode;
                    NextHorsePlayer = GHorsePlayer;
                    NextHorseScore = GHorseScore;
                    NextHorseRestart = GHorseRestart;
                    NextHorseLetters[0] = GHorseLetters[0];
                    NextHorseLetters[1] = GHorseLetters[1];

                    if (GHorsePlayer == 0)
                    {
                        Mess_SetRGB(0x50, 0x50, 0x80, 0);
                    }
                    else
                    {
                        Mess_SetRGB(0x80, 0x50, 0x50, 0);
                    }

                    if (GHorseMode == 0)
                    {
                        GHorseScore = GSkater[0]->mPanelData.mLandedTotal;
                        if (GHorseScore)
                        {
                            sprintf(Score,
                                    Front_GrafScoreSetFormat,
                                    GHorseScore);
                            Mess_DrawText(0x100, 0x50, Score,
                                          0, 0, 0x100, 0);
                            NextHorseMode ^= 1;
                        }
                        else
                        {
                            Mess_DrawText(
                                0x100, 0x50,
                                Front_NoScoreSwitchText(),
                                0, 0, 0x100, 0);
                        }
                        NextHorsePlayer ^= 1;
                    }
                    else
                    {
                        sprintf(Score, Front_GrafGoingForFormat,
                                GHorseScore);
                        Mess_DrawText(0x100, 0x50, Score,
                                      0, 0, 0x100, 0);

                        sprintf(Score, Front_GrafYouScoredFormat,
                                GSkater[0]->mPanelData.mLandedTotal);
                        Mess_DrawText(0x100, 0x68, Score,
                                      0, 0, 0x100, 0);

                        if (GSkater[0]->mPanelData.mLandedTotal ==
                            static_cast<unsigned int>(GHorseScore))
                        {
                            Mess_DrawText(0x100, 0x80,
                                          GrafFmt + 0xc8,
                                          0, 0, 0x100, 0);
                            NextHorsePlayer ^= 1;
                        }
                        else if (GSkater[0]->mPanelData.mLandedTotal <
                                 static_cast<unsigned int>(GHorseScore))
                        {
                            NextHorseLetters[GHorsePlayer]++;
                            if (NextHorseLetters[GHorsePlayer] ==
                                reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseLen)
                            {
                                char YouAre[40];
                                char firstLetter =
                                    reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord[0];
                                if ((firstLetter == 'A') ||
                                    (firstLetter == 'E') ||
                                    (firstLetter == 'I') ||
                                    (firstLetter == 'O') ||
                                    (firstLetter == 'U'))
                                {
                                    sprintf(YouAre,
                                            Front_GrafYouAreAn);
                                }
                                else
                                {
                                    sprintf(YouAre,
                                            Front_GrafYouAreA);
                                }

                                if (GHorseContinue == 0)
                                {
                                    GHorseScale += 0x20;
                                    if (GHorseScale > 0x200)
                                    {
                                        Mess_SetRGB(0xff, 0x30, 0x30, 0);
                                        GHorseScale = 0x200;
                                    }

                                    Mess_SetScale(GHorseScale);
                                    Mess_DrawText(0x100, 0x98, YouAre,
                                                  0, 0, 0x100, 0);

                                    Mess_SetScale(
                                        GHorseScale < 0x201
                                            ? GHorseScale
                                            : 0x200);
                                    Mess_DrawText(
                                        0x100, 0xbc,
                                        reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord,
                                        0, 0, 0x100, 0);
                                    GHorseWaitTime = 0x2d;
                                    Front_PrintCentredPrompt(
                                        GameText.pContinue);
                                }
                                else
                                {
                                    if (GHorseSounded == 0)
                                    {
                                        SFX_PlayX(0x2a, 0x2000, 0);
                                        GHorseSounded = 1;
                                    }

                                    Front_HorseFallingText().update();
                                    Front_HorseFallingText().bind(
                                        &GSkater[0]->mPanelData.mTrickFont);
                                    GSkater[0]->mPanelData.mTrickFont.Scale = 0x2000;
                                    GSkater[0]->mPanelData.mTrickFont.draw(
                                            0x100, 0x98, YouAre, 0);
                                    if (Front_HorseFallingText()
                                            .handleComplete() == 0)
                                    {
                                        GHorseWaitTime = 0x2d;
                                    }
                                    Front_HorseFallingText().TextEffect.unbind(
                                        &GSkater[0]->mPanelData.mTrickFont);

                                    Front_HorseFallingText().bind(
                                        &GSkater[0]->mPanelData.mTrickFont);
                                    GSkater[0]->mPanelData.mTrickFont.draw(
                                            0x100, 0xbc,
                                            reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord,
                                            0);
                                    Front_HorseFallingText().TextEffect.unbind(
                                        &GSkater[0]->mPanelData.mTrickFont);
                                }
                                NextHorseMode = 2;
                            }
                            else
                            {
                                char buf[32];
                                buf[0] =
                                    reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord[
                                        NextHorseLetters[GHorsePlayer] - 1];
                                buf[1] = 0;

                                if (GHorseScale < 0x200)
                                {
                                    GHorseScale += 0x20;
                                    if (GHorseScale > 0x200)
                                    {
                                        GHorseScale = 0x200;
                                    }

                                    Mess_SetScale(
                                        GHorseScale < 0x181
                                            ? GHorseScale
                                            : 0x180);
                                    Mess_DrawText(
                                        0x100, 0x98,
                                        GameText.pYouGetTheLetter,
                                        0, 0, 0x100, 0);

                                    Mess_SetScale(
                                        GHorseScale < 0x201
                                            ? GHorseScale
                                            : 0x200);
                                    Mess_DrawText(0x100, 0xbc, buf,
                                                  0, 0, 0x100, 0);
                                    GHorseWaitTime = 0x2d;
                                }
                                else
                                {
                                    if (GHorseSounded == 0)
                                    {
                                        SFX_PlayX(0x2a, 0x2000, 0);
                                        GHorseSounded = 1;
                                    }

                                    Front_HorseFallingText().update();
                                    GSkater[0]->mPanelData.mTrickFont.Scale = 0x1800;
                                    Front_HorseFallingText().bind(
                                        &GSkater[0]->mPanelData.mTrickFont);
                                    GSkater[0]->mPanelData.mTrickFont.draw(
                                            0x100, 0x98,
                                            GameText.pYouGetTheLetter,
                                            0);
                                    if (Front_HorseFallingText()
                                            .handleComplete() == 0)
                                    {
                                        GHorseWaitTime = 0x2d;
                                    }
                                    Front_HorseFallingText().TextEffect.unbind(
                                        &GSkater[0]->mPanelData.mTrickFont);

                                    GSkater[0]->mPanelData.mTrickFont.Scale = 0x2000;
                                    Front_HorseFallingText().bind(
                                        &GSkater[0]->mPanelData.mTrickFont);
                                    GSkater[0]->mPanelData.mTrickFont.draw(
                                            0x100, 0xbc, buf, 0);
                                    Front_HorseFallingText().TextEffect.unbind(
                                        &GSkater[0]->mPanelData.mTrickFont);
                                }

                                NextHorseRestart++;
                                if (NextHorseRestart ==
                                    NumHorseRestarts)
                                {
                                    NextHorseRestart = 0;
                                }
                                NextHorsePlayer ^= 1;
                                NextHorseMode ^= 1;
                            }
                        }
                        else
                        {
                            Mess_DrawText(0x100, 0x80,
                                      GrafFmt + 0xe8,
                                          0, 0, 0x100, 0);
                            NextHorseScore =
                                GSkater[0]->mPanelData.mLandedTotal;
                            NextHorsePlayer ^= 1;
                        }
                    }

                    Horse_DisplayWords(NextHorseLetters);
                    break;
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                default:
                    sprintf(Score1, Front_GrafPlayer1ScoreFormat,
                            s1);
                    sprintf(Score2, Front_GrafPlayer2ScoreFormat,
                            s2);
                    Mess_DrawText(0x100, 0x4b, Score1,
                                  0, 0, 0x100, 0);
                    Mess_DrawText(0x100, 0x63, Score2,
                                  0, 0, 0x100, 0);

                    if (s1 > s2)
                    {
                        Mess_DrawText(0x100, 0x7b,
                                      Front_GrafPlayer1Wins,
                                      0, 0, 0x100, 0);
                    }
                    else if (s2 > s1)
                    {
                        Mess_DrawText(0x100, 0x7b,
                                      Front_GrafPlayer2Wins,
                                      0, 0, 0x100, 0);
                    }
                    else
                    {
                        Mess_DrawText(0x100, 0x7b,
                                      GrafFmt + 0x110,
                                      0, 0, 0x100, 0);
                    }
                    break;
                }
            }

            if ((GGame != 7) && (Front_OnTime >= 0xb))
            {
                Front_PrintCentredPrompt(GameText.pContinue);
            }
        }
        break;
    case STATUS_SAVEGAME:
        Mess_SetRGB(0x96, 0x64, 0x19, 0);
        Mess_DrawText(0x100, 0x67, GameText.pSave, 0, 0, 0x100, 0);
        pYesNoMenu->mY = 0x8f;
        pYesNoMenu->Display();
        Front_PrintCentredPrompt(GameText.pToSelect);
        break;
    case STATUS_ENDHEATMENU:
        if (Front_OnTime >= 0xb)
        {
            pEHMenu->Display();
            Front_PrintCentredPrompt(GameText.pToSelect);
        }
        break;
    case STATUS_ENDHEATVIDEOMENU:
        if (Front_OnTime >= 0xb)
        {
            if (GNumberOfPlayers == 2)
            {
                pVS3Menu->EntryOff(GameText.pHighlights);
            }
            else
            {
                pVS3Menu->EntryOn(GameText.pHighlights);
            }
            pVS3Menu->CentreY();
            pVS3Menu->Display();
            Front_PrintGoBackSelect();
        }
        break;
    case STATUS_ENDGAME:
        Competition_EndGameDisplay();
        break;
    case STATUS_ENDHEATWINLOSE:
    case STATUS_MPC_WINNER:
        Competition_WinLoseDisplay();
        break;
    case STATUS_ENDHEATLADDER:
    case STATUS_MPC_LADDER:
    case STATUS_MPC_LADDERFINAL:
        Competition_LadderDisplay();
        break;
    case STATUS_ENDHEATREPLAY:
    case STATUS_MPC_JUDGES:
        Competition_JudgeDisplay();
        break;
    case STATUS_ENDHEAT:
    case STATUS_ENDHEATJUDGES:
    case STATUS_BEGINHEAT:
    case STATUS_MPC_BEGIN:
    case STATUS_MPC_BEGINHEAT:
    case STATUS_MPC_SWITCH:
    case STATUS_MPC_ENDHEAT:
    case STATUS_ENDGAMEALLGOLD:
    case STATUS_ENDGAMEPLAYERMOVIE:
    case STATUS_ENDGAMEBAILMOVIE:
    case STATUS_ENDGAMESAVE:
    case STATUS_ENDGAMECREDITS:
        break;
    case STATUS_SETSOUNDLEVELS:
        pSoundMenu->Display();
        Front_PrintCentredPrompt(GameText.pToSelect);
        break;
    default:
        break;
    }

    return;
}

/**
 * Front_GetButtons__FRiN30
 *
 * Address:   0x80029EAC
 * Mangled:   Front_GetButtons__FRiN30
 * Demangled: Front_GetButtons(int &, int &, int &, int &)
 *
 * @param Activated receives whether confirm or start was pressed
 * @param GoBack receives whether cancel was pressed
 * @param AnyButton receives whether any handled menu button was pressed
 * @param Start receives whether either start button was pressed
 */
void Front_GetButtons(int &Activated, int &GoBack, int &AnyButton, int &Start)
{
    if (Cheat_ScreenShots == 0)
        goto check_back;
    if (GameFrozen != 0)
        goto frozen;

check_back:
{
    // Folding this result into Start changes sixteen instructions and one
    // relocation, so retain the target's separately live boolean value.
    int back = 0;
    if (Pad[0].Start.Triggered != 0 || Pad[1].Start.Triggered != 0)
    {
        back = 1;
    }
    Start = back;
    // Directly spelling the two trigger loads at their stores changes 142
    // instructions and three relocations across this heavily scheduled block.
    int s1 = Pad[0].Start.Triggered;
    GStart1 = s1;
    if (s1 == 0)
    {
        int s2 = Pad[1].Start.Triggered;
        GStart2 = s2;
    }
    else
    {
        GStart2 = 0;
    }
    Pad[0].Start.Triggered = 0;
    Pad[1].Start.Triggered = 0;
    goto check_up;
}

frozen:
    Start = 0;

check_up:
{
    Activated = Pad[0].X.Triggered != 0 ||
                Pad[1].X.Triggered != 0 || Start != 0;
    Pad[0].X.Triggered = 0;
    Pad[1].X.Triggered = 0;
    GoBack = Pad[0].Triangle.Triggered != 0 ||
             Pad[1].Triangle.Triggered != 0;
    Pad[0].Triangle.Triggered = 0;
    Pad[1].Triangle.Triggered = 0;
    AnyButton = GoBack | Activated;

    if (Pad[0].Square.Triggered != 0)
    {
        Pad[0].Square.Triggered = 0;
        AnyButton = 1;
    }
    if (Pad[0].Circle.Triggered != 0)
    {
        Pad[0].Circle.Triggered = 0;
        AnyButton = 1;
    }
    if (Pad[0].Select.Triggered != 0)
    {
        Pad[0].Select.Triggered = 0;
        AnyButton = 1;
    }
    if (Pad[0].LeftOne.Triggered != 0)
    {
        Pad[0].LeftOne.Triggered = 0;
        AnyButton = 1;
    }
    if (Pad[0].LeftTwo.Triggered != 0)
    {
        Pad[0].LeftTwo.Triggered = 0;
        AnyButton = 1;
    }
    if (Pad[0].RightOne.Triggered != 0)
    {
        Pad[0].RightOne.Triggered = 0;
        AnyButton = 1;
    }
    if (Pad[0].RightTwo.Triggered != 0)
    {
        Pad[0].RightTwo.Triggered = 0;
        AnyButton = 1;
    }
    if (Pad[1].Square.Triggered != 0)
    {
        Pad[1].Square.Triggered = 0;
        AnyButton = 1;
    }
    if (Pad[1].Circle.Triggered != 0)
    {
        Pad[1].Circle.Triggered = 0;
        AnyButton = 1;
    }
    if (Pad[1].Select.Triggered != 0)
    {
        Pad[1].Select.Triggered = 0;
        AnyButton = 1;
    }
    if (Pad[1].LeftOne.Triggered != 0)
    {
        Pad[1].LeftOne.Triggered = 0;
        AnyButton = 1;
    }
    if (Pad[1].LeftTwo.Triggered != 0)
    {
        Pad[1].LeftTwo.Triggered = 0;
        AnyButton = 1;
    }
    if (Pad[1].RightOne.Triggered != 0)
    {
        Pad[1].RightOne.Triggered = 0;
        AnyButton = 1;
    }
    if (Pad[1].RightTwo.Triggered != 0)
    {
        Pad[1].RightTwo.Triggered = 0;
        AnyButton = 1;
    }

    if (Activated != 0)
    {
        GoBack = 0;
    }
}
}

/**
 * CommonSwitchOnMenuCode__Fv
 *
 * Address:   0x8002A140
 * Mangled:   CommonSwitchOnMenuCode__Fv
 * Demangled: CommonSwitchOnMenuCode(void)
 *
 */
static void CommonSwitchOnMenuCode(void)
{
    Pad_ClearTriggers(&Pad[0]);
    Pad_ClearTriggers(&Pad[1]);
    GamePaused = 1;
    int idx = pYesNoMenu->FindEntry(
        reinterpret_cast<const char *>(GameText.pNo));
    pYesNoMenu->SetLine(idx & 0xff);
    Mess_DeleteAll();
    Pad_ActuatorOff(0, 0);
    Pad_ActuatorOff(0, 1);
    SFX_Pause();
    SFX_PlayX(2, 0x1000, 0);
    Front_OnTime = 0;
}

/**
 * Front_SwitchOnEndHeat__Fv
 *
 * Address:   0x8002A1D8
 * Mangled:   Front_SwitchOnEndHeat__Fv
 * Demangled: Front_SwitchOnEndHeat(void)
 *
 */
void Front_SwitchOnEndHeat(void)
{
    CBruce *pSkater = reinterpret_cast<CBruce *>(MechList);
    JudgeTimer = 0;
    LadderTimer = 0;
    WinLoseTimer = 0;
    CBruce *pSkater2 =
        reinterpret_cast<CBruce *>(pSkater->CSuper.CBody.mNextItem);
    if (GVInfo1.VideoMode != 0)
    {
        Video_Stop(pSkater);
    }
    if (pSkater2 != 0)
    {
        if (GVInfo2.VideoMode != 0)
        {
            Video_Stop(pSkater2);
        }
    }
    Status = STATUS_ENDHEAT;
    Pad_ClearTriggers(&Pad[0]);
    Pad_ClearTriggers(&Pad[1]);
    GamePaused = 1;
    int idx = pYesNoMenu->FindEntry(
        reinterpret_cast<const char *>(GameText.pNo));
    pYesNoMenu->SetLine(idx & 0xff);
    Mess_DeleteAll();
    Pad_ActuatorOff(0, 0);
    Pad_ActuatorOff(0, 1);
    SFX_Pause();
    SFX_PlayX(2, 0x1000, 0);
}

/**
 * Front_SwitchOnEndLevelMenu__Fv
 *
 * Address:   0x8002A2C4
 * Mangled:   Front_SwitchOnEndLevelMenu__Fv
 * Demangled: Front_SwitchOnEndLevelMenu(void)
 *            Sets up end-of-level menu: checks PAL region, stops video, shifts high score table, calls CommonSwitch
 *
 */
void Front_SwitchOnEndLevelMenu(void)
{
    // Check the PAL-region byte exposed by the development hardware.
    struct HardwareRegion
    {
        u8 reserved[3];
        u8 Region;
    };
    unsigned int palFlag =
        reinterpret_cast<HardwareRegion *>(0x1F000000)->Region ==
        0x1F;

    GState = 0;

    printf(FrontAddressViewOwners::_restart_comp_string - 0xC, palFlag);
    CheckSum_Encode(GSkater[0]->mPanelData.mTotalScore,
                    static_cast<u8>(Rnd(0xff) + 1), palFlag);

    if (Status != STATUS_GOTTAPE && Status != STATUS_STATS)
    {
        CBruce *pSkater = reinterpret_cast<CBruce *>(MechList);
        CBruce *pSkater2 =
            reinterpret_cast<CBruce *>(pSkater->CSuper.CBody.mNextItem);

        if (GVInfo1.VideoMode != 0)
        {
            Video_Stop(pSkater);
        }
        if (pSkater2 != 0 && GVInfo2.VideoMode != 0)
        {
            Video_Stop(pSkater2);
        }

        if (GSkater[0]->mSessionTapes != 0 && GNumberOfPlayers == 1)
        {
            SFX_PlayX(0x13, 0x2000, 0);
            Status = STATUS_GOTTAPE;
        }
        else
        {
            if (GGame == 2)
            {
                Status = STATUS_ENDINGSCREEN;
            }
            else
            {
                Status = STATUS_STATS;
            }
        }

        if (((GGame == 1) && (Levels[GLevel].isCompetition == 0)) || GGame == 3)
        {
            GHighScoreLineIndex = -1;
            GHighScoreInitialCursorPos = 0;

            for (int i = 0; i < 8; i++)
            {
                int score = pSkater->mPanelData.mTotalScore;
                int Level = 8;
                if (GenerateLevel == 0)
                {
                    Level = GLevel;
                }

                if ((u32)reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][i].Score <= (u32)score)
                {
                    if (i < 7)
                    {
                        for (int j = 7; i < j; j--)
                        {
                            reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][j].Score = reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][j - 1].Score;
                            reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][j].Skater = reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][j - 1].Skater;
                            char *dst = reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][j].Name;
                            int k = 3;
                            char *src = reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][j - 1].Name;
                            for (; k != -1; k--)
                            {
                                *dst = *src;
                                src++;
                                dst++;
                            }
                        }
                    }
                    reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][i].Score =
                        pSkater->mPanelData.mTotalScore;
                    reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][i].Skater =
                        *reinterpret_cast<unsigned char *>(
                            &GSkater[0]->mSkaterType);
                    sprintf(reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][i].Name, reinterpret_cast<SCareerOwner *>(&GCareer)->Options.LastHighScoreInitials);
                    GHighScoreLineIndex = i;
                    i = 8;
                }
            }
        }

        if (GGame == 7)
        {
            Front_HorseFallingText().init();
            Front_HorseFallingText().SetBloodiness(0);
            GHorseScale = 0;
            GHorseWaitTime = 0x2d;
            GHorseContinue = 0;
        }

        CommonSwitchOnMenuCode();
        InitStatsSwisch();
    }

}

/**
 * Front_SwitchOnStartLevelMenu__Fv
 *
 * Address:   0x8002A630
 * Mangled:   Front_SwitchOnStartLevelMenu__Fv
 * Demangled: Front_SwitchOnStartLevelMenu(void)
 *            Sets Status = 0x29, calls CommonSwitchOnMenuCode
 *
 */
void Front_SwitchOnStartLevelMenu(void)
{
    Status = STATUS_STARTHORSE;
    CommonSwitchOnMenuCode();
}

/**
 * Front_PrintHorseLetters__Fv
 *
 * Address:   0x8002A658
 * Mangled:   Front_PrintHorseLetters__Fv
 * Demangled: Front_PrintHorseLetters(void)
 *            Sets Status = 0x2a, calls CommonSwitchOnMenuCode
 *
 */
void Front_PrintHorseLetters(void)
{
    Status = STATUS_CHANGEHORSEWORD;
    CommonSwitchOnMenuCode();
}

/**
 * Front_SwitchOnVideoScreen__Fv
 *
 * Address:   0x8002A680
 * Mangled:   Front_SwitchOnVideoScreen__Fv
 * Demangled: Front_SwitchOnVideoScreen(void)
 *
 */
void Front_SwitchOnVideoScreen(void)
{
    EPausedStatus VideoScreenMode;
    if (GVideoRestart.NumSkaters != 0)
        goto set_status;
    {
        SLevel *pBase = Levels;
        int level = GLevel;
        int ofs = level * sizeof(SLevel);
        // Clean typed pointer addition differs only in the commutative addu's
        // operand order. This generic operandized instruction preserves it.
        asm volatile("addu\t%0,%0,%1" : "+r"(pBase) : "r"(ofs));
        int video = reinterpret_cast<int>(pBase->isCompetition);
        int one = 1;
        if (video == 0)
            goto set_status;
        {
            // An ordinary local changes four instructions around this branch.
            register int game asm("$3") = GGame;
            if (game != one)
                goto set_status;
        }
        if (Competition_IsOver() == 0)
        {
            VideoScreenMode = STATUS_ENDHEATVIDEOMENU;
            goto store;
        }
    }
set_status:
    VideoScreenMode = STATUS_VIDEOSCREEN2;
    if (GVideoRestart.NumSkaters == 0)
    {
        VideoScreenMode = STATUS_VIDEOSCREEN;
    }
store:
    Status = VideoScreenMode;
    CommonSwitchOnMenuCode();
}

/**
 * RestartCompFrig__Fv
 *
 * Address:   0x8002A728
 * Mangled:   RestartCompFrig__Fv
 * Demangled: RestartCompFrig(void)
 *            If GLevel == 8, saves RestartNode, calls Trig_SetRestart + Trig_ExecuteRestart, restores
 */
void RestartCompFrig(void)
{
    if (GLevel == 8)
    {
        int t = RestartNode;
        Trig_SetRestart(FrontAddressViewOwners::_restart_comp_string);
        Trig_ExecuteRestart();
        RestartNode = t;
    }
}

/**
 * Front_Update__Fv
 *
 * Address:   0x8002A778
 * Mangled:   Front_Update__Fv
 * Demangled: Front_Update(void)
 *            Massive switch on Status (47 cases) — game pause/menu/competition/horse/video update logic
 */
void Front_Update(void)
{
    int SoundEffect = -1;
    int SwitchOffPauseMode = 0;
    int Activated;
    int GoBack;
    int AnyButton;
    int Start;
    // Poll menu buttons unless pause/start-select handling owns the pad state.
    if (((Pad[0].LeftOne.Pressed == 0) || (Status != STATUS_PAUSEDMENU)) &&
        (Status != STATUS_MEMCARDMENUS))
    {
        Front_GetButtons(Activated, GoBack, AnyButton, Start);
    }
    if (Status != STATUS_NORMAL)
    {
        Card_CheckStatus(0, 0);
    }

    // Both pads participate in the start/select soft-reset chord.
    if (((GSoftResetEnabled != 0) && (Pad[0].Select.Pressed != 0) &&
         (Pad[0].Start.Pressed != 0)) ||
        ((Pad[1].Select.Pressed != 0) && (Pad[1].Start.Pressed != 0)))
    {
        if (StartSelectPressed == 0)
        {
            StartSelectPressed = 1;
            VblanksWhenStartSelectFirstPressed =
                Vblanks;
        }
        if ((Vblanks -
             VblanksWhenStartSelectFirstPressed) >= 0x79u)
        {
            StartSelectPressed = 0;
            EndCode = ENDCODE_PLAYERQUIT;
            Horse_Reset();
            SwitchOffPauseMode = 1;
        }
    }
    else
    {
        StartSelectPressed = 0;
    }

    GSoftResetEnabled = 1;
    switch (Status)
    {
    case STATUS_MEMCARDMENUS:
    {
        Status = Front_MemCardUpdate();
        break;
    }

    case STATUS_NORMAL:
    {
        {
            CBruce *pSkater = reinterpret_cast<CBruce *>(MechList);
            CBruce *pSkater2 =
                reinterpret_cast<CBruce *>(pSkater->CSuper.CBody.mNextItem);
            SVideoInfo *VInfo = &GVInfo1;
            if (VInfo->VideoMode == 2 && Activated != 0)
            {
                VInfo->PlaybackFrozen = 0;
                GameFrozen = 0;
                Video_Stop(pSkater);
                if (pSkater2 != 0)
                {
                    Video_Stop(pSkater2);
                }
                Front_SwitchOnVideoScreen();
                break;
            }
        }
        if ((((Start != 0) || (Pad[0].Type == 0)) ||
             ((GNumberOfPlayers == 2) && (Pad[1].Type == 0))) &&
            (DemoTimer != 0) && (GState != 1))
        {
            {
                CBruce *pSkater = GSkater[0];
                pSkater->mJumpInhibit = XFrames;
                GamePaused = 1;
                CBruce *pSkater2 = GSkater[1];
                if (pSkater2 != 0)
                {
                    pSkater2->mJumpInhibit = XFrames;
                }
            }
            {
                CBruce *pSkater = GSkater[0];
                pSkater->mRailInhibit = XFrames;
                CBruce *pSkater2 = GSkater[1];
                if (pSkater2 != 0)
                {
                    pSkater2->mRailInhibit = XFrames;
                }
            }
            Mess_DeleteAll();
            Pad_ClearTriggers(Pad);
            Pad_ClearTriggers(&Pad[1]);
            WobbleAmplitude = 0;
            SFX_Pause();
            SFX_PlayX(2, 0x1000, 0);
            Pad_ActuatorOff(0, 0);
            Pad_ActuatorOff(0, 1);

            pPausedMenu = new CMenu(0x100, 0, 0, 0x14f, 0x100, 0x16);

            pPausedMenu->AddEntry(GameText.pContinue);
            pPausedMenu->AddEntry(
                ((Levels[GLevel].isCompetition != 0) && (GGame == 1))
                    ? GameText.pRestart
                    : GameText.pRetry);
            pPausedMenu->AddEntry(GameText.pSkipToRestart);
            if (Cheat_RestartSkip == 0)
            {
                pPausedMenu->EntryOff(GameText.pSkipToRestart);
            }
            pPausedMenu->AddEntry(GameText.pChangeSoundLevels);
            if (GGame == 7)
            {
                pPausedMenu->AddEntry(GameText.pEndGame);
            }
            else
            {
                pPausedMenu->AddEntry(GameText.pEndRun);
            }
            pPausedMenu->CentreY();
            pPausedMenu->mY += 0xd;

            if ((Pad[0].Type != 0) &&
                ((GNumberOfPlayers == 1) || (Pad[1].Type != 0)))
            {
                Status = STATUS_PAUSEDMENU;
                if ((GStart1 != 0) && (GStart2 != 0))
                {
                    GClear1 = 0;
                    GClear2 = 0;
                }
                else if (GGame != 7)
                {
                    GClear1 = GStart2;
                    GClear2 = GStart1;
                }
            }
            else
            {
                Status = STATUS_PADUNPLUGGED;
            }
        }
        break;
    }

    case STATUS_PAUSEDMENU:
    {
        if (GamePaused == 0)
        {
            SwitchOffPauseMode = 1;
        }
        if ((Pad[0].Type == 0) || ((GNumberOfPlayers == 2) && (Pad[1].Type == 0)))
        {
            Status = STATUS_PADUNPLUGGED;
        }
        if (Pad[0].LeftOne.Pressed != 0)
        {
            if (Shell_DoCheatProcessing() != 0)
            {
                WobbleAmplitude = 0x20;
                WobbleT = 0;
            }
            Pad_ClearTriggers(Pad);
            Pad_ClearTriggers(&Pad[1]);
            break;
        }

        CheatCheck = 0;
        if (Cheat_RestartSkip != 0)
        {
            pPausedMenu->EntryOn(GameText.pSkipToRestart);
        }
        else
        {
            pPausedMenu->EntryOff(GameText.pSkipToRestart);
        }

        pPausedMenu->Update();
        if (Activated != 0)
        {
            LocalText *menuText = &GameText;
            if (pPausedMenu->ChoiceIs(menuText->pEndRun) != 0)
            {
                SoundEffect = 5;
                Game_Over();
                SwitchOffPauseMode = 1;
                GState = 1;
                GSpin = 1;
            }
            if (pPausedMenu->ChoiceIs(menuText->pContinue) != 0)
            {
                SoundEffect = 5;
                SwitchOffPauseMode = 1;
            }

            if (pPausedMenu->ChoiceIs(
                    ((Levels[GLevel].isCompetition != 0) && (GGame == 1))
                        ? menuText->pRestart
                        : GameText.pRetry) != 0)
            {
                SwitchOffPauseMode = 1;
                Horse_Reset();
                EndCode = ENDCODE_TWEAKJUMP;
                Competition_Init();
            }
            menuText = &GameText;
            if (pPausedMenu->ChoiceIs(menuText->pSkipToRestart) != 0)
            {
                SoundEffect = 5;
                Status = STATUS_SKIPTORESTART;
            }
            if (pPausedMenu->ChoiceIs(menuText->pQuit) != 0)
            {
                SoundEffect = 5;
                pYesNoMenu->SetLine(pYesNoMenu->FindEntry(menuText->pNo));
                Status = STATUS_REALLYQUIT;
            }
            if (pPausedMenu->ChoiceIs(menuText->pChangeSoundLevels) != 0)
            {
                Status = STATUS_SETSOUNDLEVELS;
                SoundEffect = 5;
                Redbook_XARemember(Front_PausedSongPosition());
            }
            if (pPausedMenu->ChoiceIs(menuText->pEndGame) != 0)
            {
                SoundEffect = 5;
                Front_OnTime = 0;
                GHorseClearInput2 = 0;
                Status = STATUS_ENDINGSCREEN;
            }
        }
        if (GoBack != 0)
        {
            SoundEffect = 5;
            SwitchOffPauseMode = 1;
        }
        SFX_PlayX(SoundEffect, 0x1000, 0);
        break;
    }

    case STATUS_SKIPTORESTART:
    {
        if (pRestartsMenu == 0)
        {
            pRestartsMenu = new CMenu(0x100, 0, 0, 0x14f, 0x100, 0x16);
            pRestartsMenu->mLineSep = 0x12;

            int i = 0;
            if (i < NumCheatRestarts)
            {
                do
                {
                    char head[4];
                    head[0] = CheatRestarts[i][0];
                    head[1] = CheatRestarts[i][1];
                    head[2] = CheatRestarts[i][2];
                    head[3] = 0;
                    if (Utils_CompareStrings(
                            head,
                            FrontAddressViewOwners::_restart_comp_string + 0x18,
                            0xffffffff) == 0)
                    {
                        pRestartsMenu->AddEntry(CheatRestarts[i]);
                    }
                    i++;
                } while (i < NumCheatRestarts);
            }
            pRestartsMenu->CentreY();
        }

        pRestartsMenu->Update();
        if (Activated != 0)
        {
            Trig_SetRestart(
                pRestartsMenu->mEntry[pRestartsMenu->mLine].pString);
            if (pRestartsMenu != 0)
            {
                Front_DestroyMenu(pRestartsMenu);
            }
            SwitchOffPauseMode = 1;
            pRestartsMenu = 0;
            EndCode = ENDCODE_TWEAKJUMP;
            SFX_PlayX(5, 0x2000, 0);
        }
        if (GoBack != 0)
        {
            if (pRestartsMenu != 0)
            {
                Front_DestroyMenu(pRestartsMenu);
            }
            pRestartsMenu = 0;
            Status = STATUS_PAUSEDMENU;
            SFX_PlayX(2, 0x2000, 0);
        }
        break;
    }

    case STATUS_REALLYQUIT:
    {
        pYesNoMenu->Update();
        if (Activated != 0)
        {
            char *yesText = GameText.pYes;
            if (pYesNoMenu->ChoiceIs(yesText) != 0)
            {
                EndCode = ENDCODE_PLAYERQUIT;
                SwitchOffPauseMode = 1;
                Horse_Reset();
                SFX_PlayX(5, 0x2000, 0);
            }
            else
            {
                Status = STATUS_PAUSEDMENU;
                SFX_PlayX(2, 0x2000, 0);
            }
        }
        if (GoBack != 0)
        {
            SFX_PlayX(2, 0x2000, 0);
            Status = STATUS_PAUSEDMENU;
        }
        break;
    }

    case STATUS_PADUNPLUGGED:
    {
        if ((Pad[0].Type != 0) && ((GNumberOfPlayers == 1) || (Pad[1].Type != 0)))
        {
            Status = STATUS_PAUSEDMENU;
        }
        break;
    }

    case STATUS_QUITSAVE:
    {
        pYesNoMenu->Update();
        if (Activated != 0)
        {
            if (pYesNoMenu->ChoiceIs(GameText.pYes) != 0)
            {
                SFX_PlayX(5, 0x2000, 0);
                Front_SetSaveAction(SAVE_GAME);
                Front_InitMemCardMenus(
                    0x200, SVSTATUS_CHECKINGCARDPAUSE, STATUS_ENDINGSCREEN);
                Status = STATUS_MEMCARDMENUS;
            }
            else
            {
                Status = STATUS_ENDLEVELFADESCREEN;
                SFX_PlayX(2, 0x2000, 0);
            }
        }
        if (GoBack != 0)
        {
            SFX_PlayX(2, 0x2000, 0);
            Status = STATUS_ENDINGSCREEN;
        }
        break;
    }

    case STATUS_REALLYMAINMENU:
    {
        pYesNoMenu->Update();
        if (Activated != 0)
        {
            if (pYesNoMenu->ChoiceIs(GameText.pYes) != 0)
            {
                Status = STATUS_ENDLEVELFADESCREEN;
                SFX_PlayX(5, 0x2000, 0);
            }
            else
            {
                SFX_PlayX(2, 0x2000, 0);
                if ((InCompetition() & (Competition_IsOver() ^ 1)) != 0)
                {
                    Status = STATUS_ENDHEATMENU;
                    pEHMenu->SetLine(pEHMenu->FindEntry(GameText.pNextHeat));
                }
                else
                {
                    Status = STATUS_ENDINGSCREEN;
                }
            }
        }
        if (GoBack != 0)
        {
            SFX_PlayX(2, 0x2000, 0);
            if ((InCompetition() & (Competition_IsOver() ^ 1)) != 0)
            {
                Status = STATUS_ENDHEATMENU;
                pEHMenu->SetLine(pEHMenu->FindEntry(GameText.pNextHeat));
            }
            else
            {
                Status = STATUS_ENDINGSCREEN;
            }
        }
        break;
    }

    case STATUS_ENDINGSCREEN:
    {
        if (GRollCredits != 0)
        {
            Status = STATUS_ENDLEVELFADESCREEN;
            break;
        }
        if (Front_OnTime < 0xb)
        {
            Front_OnTime++;
            break;
        }
        pESMenu->Update();
        if (Activated == 0)
        {
            break;
        }
        SFX_PlayX(5, 0x2000, 0);
        Flash_Fade(0x10, 1);
        if (pESMenu->ChoiceIs(GameText.pQuit) != 0)
        {
            if ((Levels[GLevel].isCompetition != 0) ||
                (pESMenu->IsEntryOn(GameText.pSaveCareer) == 0) ||
                (GLoadedCareerIsSaved != 0))
            {
                Status = STATUS_REALLYMAINMENU;
            }
            else
            {
                Status = STATUS_QUITSAVE;
                pYesNoMenu->SetLine(
                    pYesNoMenu->FindEntry(GameText.pYes));
            }
        }
        else if (pESMenu->ChoiceIs(
                     ((Levels[GLevel].isCompetition != 0) && (GGame == 1))
                         ? GameText.pRestart
                         : GameText.pRetry) != 0)
        {
            EndCode = ENDCODE_TWEAKJUMP;
            SwitchOffPauseMode = 1;
            Competition_Init();
            Horse_Reset();
        }
        else if (pESMenu->ChoiceIs(GameText.pWatchReplay) != 0)
        {
            Status = STATUS_VIDEOSCREEN;
        }
        else if ((pESMenu->ChoiceIs(GameText.pSaveCareer) != 0) ||
                 (pESMenu->ChoiceIs(
                      GameText.pUpdateSaveFile) != 0))
        {
            pYesNoMenu->SetLine(
                pYesNoMenu->FindEntry(GameText.pYes));
            Status = STATUS_SAVEGAME;
        }
        else
        {
            if (pESMenu->ChoiceIs(
                    GameText.pSelectLevel) != 0)
            {
                StartScreen = 1;
                Status = STATUS_ENDLEVELFADESCREEN;
            }
            else if (pESMenu->ChoiceIs(
                         GameText.pEquipmentSelect) != 0)
            {
                StartScreen = 2;
                Status = STATUS_ENDLEVELFADESCREEN;
            }
            else if (pESMenu->ChoiceIs(
                         GameText.pLevelHints) != 0)
            {
                StartScreen = 6;
                Status = STATUS_ENDLEVELFADESCREEN;
            }
            else
            {
                break;
            }
        }
        break;
    }

    case STATUS_VIDEOSCREEN:
    {
        if (Front_OnTime < 0xb)
        {
            Front_OnTime++;
            break;
        }
        Front_SetSaveAction(SAVE_VIDEO);
        pVSMenu->Update();
        if (Activated != 0)
        {
            GVideoReplay = 0;
            SFX_PlayX(5, 0x2000, 0);
            Flash_Fade(0x20, 1);
            if (pVSMenu->ChoiceIs(GameText.pExit) != 0)
            {
                Video_Cleanup();
                Status = STATUS_ENDINGSCREEN;
            }
            else
            {
                if (pVSMenu->ChoiceIs(GameText.pFullReplay) != 0)
                {
                    GVideoReplay = -1;
                    SwitchOffPauseMode = 1;
                    EndCode = ENDCODE_TWEAKJUMP;
                    Video_SortAndMerge();
                }
                else if (pVSMenu->ChoiceIs(GameText.pHighlights) != 0)
                {
                    GVideoReplay = 1;
                    SwitchOffPauseMode = 1;
                    EndCode = ENDCODE_TWEAKJUMP;
                    Video_SortAndMerge();
                }
                else if (pVSMenu->ChoiceIs(GameText.pSaveVideo) != 0)
                {
                    Front_InitMemCardMenus(
                        0x200, SVSTATUS_CHECKINGCARDPAUSE, STATUS_VIDEOSCREEN);
                    Status = STATUS_MEMCARDMENUS;
                }
            }
        }
        if (GoBack != 0)
        {
            SFX_PlayX(2, 0x2000, 0);
            Status = STATUS_ENDINGSCREEN;
        }
        break;
    }

    case STATUS_VIDEOSCREEN2:
    {
        if (DemoTimer == 0)
        {
            Video_Cleanup();
            Status = STATUS_ENDLEVELFADESCREEN;
        }
        if (Front_OnTime < 0xb)
        {
            Front_OnTime++;
            break;
        }
        Front_SetSaveAction(SAVE_VIDEO);
        pVS2Menu->Update();
        if (Activated != 0)
        {
            GVideoReplay = 0;
            SFX_PlayX(5, 0x2000, 0);
            Flash_Fade(0x20, 1);
            if (pVS2Menu->ChoiceIs(GameText.pExit) != 0)
            {
                Video_Cleanup();
                Status = STATUS_ENDLEVELFADESCREEN;
            }
            else
            {
                if (pVS2Menu->ChoiceIs(GameText.pFullReplay) != 0)
                {
                    GVideoReplay = -1;
                    SwitchOffPauseMode = 1;
                    EndCode = ENDCODE_TWEAKJUMP;
                    Video_SortAndMerge();
                }
                else if (pVS2Menu->ChoiceIs(GameText.pHighlights) != 0)
                {
                    GVideoReplay = 1;
                    SwitchOffPauseMode = 1;
                    EndCode = ENDCODE_TWEAKJUMP;
                    Video_SortAndMerge();
                }
            }
        }
        if (GoBack != 0)
        {
            SFX_PlayX(2, 0x2000, 0);
            Video_Cleanup();
            Status = STATUS_ENDLEVELFADESCREEN;
        }
        break;
    }

    case STATUS_ENDHEATVIDEOMENU:
    {
        if (Front_OnTime < 0xb)
        {
            Front_OnTime++;
            break;
        }
        Front_SetSaveAction(SAVE_VIDEO);
        pVS3Menu->Update();
        if (Activated != 0)
        {
            GVideoReplay = 0;
            SFX_PlayX(5, 0x2000, 0);
            Flash_Fade(0x20, 1);
            if (pVS3Menu->ChoiceIs(GameText.pReturnToCompetition) != 0)
            {
                Video_Cleanup();
                Status = STATUS_ENDHEATMENU;
                pEHMenu->SetLine(pEHMenu->FindEntry(GameText.pNextHeat));
            }
            else
            {
                if (pVS3Menu->ChoiceIs(GameText.pFullReplay) != 0)
                {
                    GVideoReplay = -1;
                    SwitchOffPauseMode = 1;
                    EndCode = ENDCODE_TWEAKJUMP;
                    Video_SortAndMerge();
                }
                else if (pVS3Menu->ChoiceIs(GameText.pHighlights) != 0)
                {
                    GVideoReplay = 1;
                    SwitchOffPauseMode = 1;
                    EndCode = ENDCODE_TWEAKJUMP;
                    Video_SortAndMerge();
                }
                else if (pVS3Menu->ChoiceIs(GameText.pSaveVideo) != 0)
                {
                    Front_InitMemCardMenus(
                        0x200, SVSTATUS_CHECKINGCARDPAUSE,
                        STATUS_ENDHEATVIDEOMENU);
                    Status = STATUS_MEMCARDMENUS;
                }
            }
        }
        if (GoBack != 0)
        {
            SFX_PlayX(2, 0x2000, 0);
            Status = STATUS_ENDHEATMENU;
            pEHMenu->SetLine(pEHMenu->FindEntry(GameText.pNextHeat));
        }
        break;
    }

    case STATUS_GOTTAPE:
    {
        if (Front_OnTime < 0xb)
        {
            Front_OnTime++;
            break;
        }
        if (Activated == 0 || CTape::DisplayContinueButton() == 0)
        {
            break;
        }
        SFX_PlayX(5, 0x2000, 0);
        int i = 0;
        do
        {
            if (GTape[i] != 0)
            {
                Front_DestroyTape(GTape[i]);
            }
            GTape[i] = 0;
            i++;
        } while (i < 5);
        Status = STATUS_TAPESAVE;
        pYesNoMenu->SetLine(pYesNoMenu->FindEntry(GameText.pYes));
        break;
    }

    case STATUS_CHANGEHORSEWORD:
    {
        GHorseClearInput2 = 0;
        int UpDown = GetUpDown();
        char lastAlpha = 'Z';
        char firstAlpha = 'A';
        if ((reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseLen - 1 <= GHorseCursorPos) &&
            (reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseLen > 3))
        {
            lastAlpha = ' ';
            firstAlpha = ' ';
        }
        if (UpDown == -1)
        {
            SizeUp = 0x10;
            SFX_PlayX(0x11, 0x2000, 0);
            if ((reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord[GHorseCursorPos] == ' ') ||
                (reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord[GHorseCursorPos] == 0))
            {
                reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord[GHorseCursorPos] = 'A';
            }
            else
            {
                reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord[GHorseCursorPos]++;
                if (static_cast<unsigned char>(
                        reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord[GHorseCursorPos]) > 'Z')
                {
                    reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord[GHorseCursorPos] = firstAlpha;
                }
            }
        }
        if (UpDown == 1)
        {
            SizeUp = 0x10;
            SFX_PlayX(0x11, 0x2000, 0);
            if ((reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord[GHorseCursorPos] == ' ') ||
                (reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord[GHorseCursorPos] == 0))
            {
                reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord[GHorseCursorPos] = 'Z';
            }
            else
            {
                reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord[GHorseCursorPos]--;
                if (static_cast<unsigned char>(
                        reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord[GHorseCursorPos]) < 'A')
                {
                    reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord[GHorseCursorPos] = lastAlpha;
                }
            }
        }

        reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseLen = 10;
        for (int i = 0; i < 10; i++)
        {
            if ((reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord[i] == ' ') ||
                (reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord[i] == 0))
            {
                reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseLen = i;
                break;
            }
        }

        if ((Pad[0].Left.Triggered != 0) || (Pad[1].Left.Triggered != 0))
        {
            SizeUp = 0x10;
            SFX_PlayX(5, 0x2000, 0);
            Pad[0].Left.Triggered = 0;
            Pad[1].Left.Triggered = 0;
            GHorseCursorPos--;
            if (GHorseCursorPos < 0)
            {
                GHorseCursorPos = reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseLen;
            }
            if (GHorseCursorPos > 9)
            {
                GHorseCursorPos = 9;
            }
        }
        if ((Pad[0].Right.Triggered != 0) || (Pad[1].Right.Triggered != 0))
        {
            SizeUp = 0x10;
            SFX_PlayX(5, 0x2000, 0);
            Pad[0].Right.Triggered = 0;
            Pad[1].Right.Triggered = 0;
            GHorseCursorPos++;
            if (reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseLen < GHorseCursorPos)
            {
                GHorseCursorPos = 0;
            }
            if (GHorseCursorPos > 9)
            {
                GHorseCursorPos = 0;
            }
        }
        if (Activated != 0)
        {
            int i = 0;
            while (i < 10)
            {
                if (reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord[i] == ' ')
                {
                    reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord[i] = 0;
                    break;
                }
                i++;
            }
            sprintf(GPreferences.Options.HorseWord, reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseWord);
            GHorseWordChosen = 1;
            GHorseDoneStart = 0;
            GPreferences.Options.HorseLen = reinterpret_cast<SCareerOwner *>(&GCareer)->Options.HorseLen;
            SFX_PlayX(5, 0x2000, 0);
        }
        break;
    }

    case STATUS_STARTHORSE:
    {
        CBruce *pSkater = GSkater[0];
        GHorseClearInput2 = 1;
        pSkater->mJumpInhibit = XFrames;
        if (Activated != 0)
        {
            SwitchOffPauseMode = 1;
        }
        break;
    }

    case STATUS_TAPESAVE:
    {
        pYesNoMenu->Update();
        if (Activated != 0)
        {
            SFX_PlayX(5, 0x2000, 0);
            Flash_Fade(0x20, 1);
            if (pYesNoMenu->ChoiceIs(GameText.pYes) != 0)
            {
                Front_SetSaveAction(SAVE_GAME);
                Front_InitMemCardMenus(
                    0x200, SVSTATUS_CHECKINGCARDPAUSE, STATUS_STATS);
                Status = STATUS_MEMCARDMENUS;
            }
            else
            {
                Status = STATUS_STATS;
            }
        }
        if (GoBack != 0)
        {
            SFX_PlayX(2, 0x2000, 0);
            Status = STATUS_GOTTAPE;
        }
        break;
    }

    case STATUS_HIGHSCORE:
    {
        if (Front_OnTime > 10)
        {
            if (GHighScoreLineIndex != -1)
            {
                int Level = 8;
                if (GenerateLevel == 0)
                {
                    Level = GLevel;
                }
                int UpDown = GetUpDown();
                if (UpDown == -1)
                {
                    SizeUp = 0x10;
                    SFX_PlayX(0x11, 0x2000, 0);
                    if ((reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][GHighScoreLineIndex].Name[GHighScoreInitialCursorPos] == ' ') ||
                        (reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][GHighScoreLineIndex].Name[GHighScoreInitialCursorPos] == 0))
                    {
                        reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][GHighScoreLineIndex].Name[GHighScoreInitialCursorPos] = 'A';
                    }
                    else
                    {
                        reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][GHighScoreLineIndex].Name[GHighScoreInitialCursorPos]++;
                        if (static_cast<unsigned char>(reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][GHighScoreLineIndex].Name[GHighScoreInitialCursorPos]) > 'Z')
                        {
                            reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][GHighScoreLineIndex].Name[GHighScoreInitialCursorPos] = ' ';
                        }
                    }
                }
                if (UpDown == 1)
                {
                    SizeUp = 0x10;
                    SFX_PlayX(0x11, 0x2000, 0);
                    if ((reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][GHighScoreLineIndex].Name[GHighScoreInitialCursorPos] == ' ') ||
                        (reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][GHighScoreLineIndex].Name[GHighScoreInitialCursorPos] == 0))
                    {
                        reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][GHighScoreLineIndex].Name[GHighScoreInitialCursorPos] = 'Z';
                    }
                    else
                    {
                        reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][GHighScoreLineIndex].Name[GHighScoreInitialCursorPos]--;
                        if (static_cast<unsigned char>(reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][GHighScoreLineIndex].Name[GHighScoreInitialCursorPos]) < 'A')
                        {
                            reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][GHighScoreLineIndex].Name[GHighScoreInitialCursorPos] = ' ';
                        }
                    }
                }
                if ((Pad[0].Left.Triggered != 0) || (Pad[1].Left.Triggered != 0))
                {
                    SizeUp = 0x10;
                    SFX_PlayX(5, 0x2000, 0);
                    Pad[0].Left.Triggered = 0;
                    Pad[1].Left.Triggered = 0;
                    GHighScoreInitialCursorPos--;
                    if (GHighScoreInitialCursorPos < 0)
                    {
                        GHighScoreInitialCursorPos = 2;
                    }
                }
                if ((Pad[0].Right.Triggered != 0) || (Pad[1].Right.Triggered != 0))
                {
                    SizeUp = 0x10;
                    SFX_PlayX(5, 0x2000, 0);
                    Pad[0].Right.Triggered = 0;
                    Pad[1].Right.Triggered = 0;
                    GHighScoreInitialCursorPos++;
                    if (GHighScoreInitialCursorPos > 2)
                    {
                        GHighScoreInitialCursorPos = 0;
                    }
                }
            }
            if (Activated != 0)
            {
                SFX_PlayX(0x12, 0x2000, 0);
                int Level = 8;
                if (GenerateLevel == 0)
                {
                    Level = GLevel;
                }
                sprintf(reinterpret_cast<SCareerOwner *>(&GCareer)->Options.LastHighScoreInitials,
                        reinterpret_cast<SCareerOwner *>(&GCareer)->HighScore[Level][GHighScoreLineIndex].Name);
                GHighScoreLineIndex = -1;
                Status = STATUS_ENDINGSCREEN;
            }
        }
        break;
    }

    case STATUS_STATS:
    {
        if (Front_OnTime < 0xb)
        {
            Front_OnTime++;
            break;
        }
        if (GGame == 7)
        {
            GHorseClearInput2 = 0;
            GHorseWaitTime--;
            if ((GHorseWaitTime < 1) && (NextHorseMode != 2))
            {
                Activated = 1;
            }
            if ((NextHorseMode == 2) && (GHorseContinue != 0) &&
                (Front_HorseFallingText().handleComplete() != 0))
            {
                Front_OnTime = 0;
                Status = STATUS_ENDINGSCREEN;
            }
        }

        if (Activated == 0)
        {
            break;
        }
        GCompoSounded = 0;
        SFX_PlayX(5, 0x2000, 0);
        int game = GGame;
        GHorseSounded = 0;
        if (GGame == 7)
        {
            if (NextHorseMode == 2)
            {
                GHorseContinue = 1;
                GHorseRestart = 0;
                Trig_SetRestart(HorseRestarts[0]);
            }
            else
            {
                GHorseMode = NextHorseMode;
                GHorseRestart = NextHorseRestart;
                GHorseScore = NextHorseScore;
                GHorsePlayer = NextHorsePlayer;
                GHorseLetters[0] = NextHorseLetters[0];
                GHorseLetters[1] = NextHorseLetters[1];
                Trig_SetRestart(HorseRestarts[NextHorseRestart]);
                SwitchOffPauseMode = 1;
                EndCode = static_cast<EEndCode>(game);
            }
            break;
        }
        if (GHighScoreLineIndex != -1)
        {
            Status = STATUS_HIGHSCORE;
            InitStatsSwisch();
            break;
        }
        Status = STATUS_ENDINGSCREEN;
        break;
    }

    case STATUS_ENDHEATMENU:
    {
        if (Front_OnTime < 0xb)
        {
            Front_OnTime++;
            break;
        }
        pEHMenu->Update();
        if (Activated == 0)
        {
            break;
        }
        SFX_PlayX(5, 0x2000, 0);
        Flash_Fade(0x20, 1);
        if (pEHMenu->ChoiceIs(GameText.pQuit) != 0)
        {
            Status = STATUS_REALLYMAINMENU;
        }
        else if (pEHMenu->ChoiceIs(GameText.pNextHeat) != 0)
        {
            SwitchOffPauseMode = 1;
            EndCode = ENDCODE_TWEAKJUMP;
        }
        else if (pEHMenu->ChoiceIs(GameText.pEndComp) != 0)
        {
            Status = STATUS_ENDHEATWINLOSE;
        }
        else if (pEHMenu->ChoiceIs(GameText.pWatchReplay) != 0)
        {
            Status = STATUS_ENDHEATVIDEOMENU;
        }
        break;
    }

    case STATUS_ENDGAME:
    {
        Competition_EndGameLogic(Status, Activated);
        break;
    }

    case STATUS_ENDHEATWINLOSE:
    {
        Competition_WinLoseLogic(Status, Activated);
        break;
    }

    case STATUS_MEDALSAVE:
    {
        pYesNoMenu->Update();
        if (Activated != 0)
        {
            SFX_PlayX(5, 0x2000, 0);
            Flash_Fade(0x20, 1);
            if (pYesNoMenu->ChoiceIs(GameText.pYes) != 0)
            {
                Front_SetSaveAction(SAVE_GAME);
                Front_InitMemCardMenus(
                    0x200, SVSTATUS_CHECKINGCARDPAUSE, STATUS_ENDINGSCREEN);
                Status = STATUS_MEMCARDMENUS;
                break;
            }
            Status = STATUS_ENDINGSCREEN;
        }
        break;
    }

    case STATUS_ENDHEATLADDER:
    {
        Competition_LadderLogic();
        if (Activated == 0)
        {
            break;
        }
        if (Competition_LadderFullyDisplayed() != 0)
        {
            Video_Stop(reinterpret_cast<CBruce *>(MechList));
            GamePaused = 1;
            SFX_StopAll();
            if (Competition_NewHeat() != 0)
            {
                Status = STATUS_ENDHEATMENU;
                pEHMenu->SetLine(pEHMenu->FindEntry(GameText.pNextHeat));
            }
            else
            {
                Status = STATUS_ENDHEATWINLOSE;
            }
            break;
        }
        Competition_FullyDisplayLadder();
        SFX_PlayX(0x34, 0x2000, 0);
        break;
    }

    case STATUS_ENDHEATREPLAY:
    {
        Competition_JudgeLogic();
        if (Front_OnTime >= 0xb)
        {
            if (Activated != 0)
            {
                if (Competition_JudgeFullyDisplayed() != 0)
                {
                    SFX_PlayX(5, 0x2000, 0);
                    Status = STATUS_ENDHEATLADDER;
                    GVideoReplay = -1;
                    RestartCompFrig();
                    if (pPausedMenu != 0)
                    {
                        Front_DestroyMenu(pPausedMenu);
                    }
                    pPausedMenu = 0;
                    if ((EndCode != ENDCODE_PLAYERQUIT) &&
                        (EndCode != ENDCODE_ENDLEVELDOSAVE) &&
                        (EndCode != ENDCODE_ENDLEVELNOSAVE))
                    {
                        SFX_Unpause();
                    }
                    Video_SortAndMerge();
                    if (GVInfo1.NumFrames > 0x5a)
                    {
                        CBruce *pSkater =
                            reinterpret_cast<CBruce *>(MechList);
                        CBruce *pSkater2 =
                            reinterpret_cast<CBruce *>(
                                pSkater->CSuper.CBody.mNextItem);
                        GamePaused = 0;
                        GVideoReplay = 0;
                        GVInfo1.ReplayHighlight = 0;
                        GVInfo2.ReplayHighlight = 0;
                        Video_Start(
                            pSkater, VIDEO_PLAYBACK, 1, 0x7fffffff);
                        if (pSkater2 != 0)
                        {
                            Video_Start(
                                pSkater2, VIDEO_PLAYBACK, 1, 0x7fffffff);
                    }
                    GState = 0;
                    Flash_Reset();
                    {
                        CBruce *pSkater =
                            reinterpret_cast<CBruce *>(MechList);
                        CBruce *pSkater2 =
                            reinterpret_cast<CBruce *>(
                                pSkater->CSuper.CBody.mNextItem);
                        pSkater->mPlayTime = Xblanks + 0x34bde;
                        if (pSkater2 != 0)
                        {
                            pSkater2->mPlayTime = Xblanks + 0x34bde;
                        }
                    }
                }
                }
                else
                {
                    Competition_FullyDisplayJudge();
                    SFX_PlayX(0x34, 0x2000, 0);
                }
            }
        }
        else
        {
            Front_OnTime++;
        }
        break;
    }

    case STATUS_ENDHEAT:
    {
        Status = STATUS_ENDHEATREPLAY;
        Front_OnTime++;
        break;
    }

    case STATUS_SAVEGAME:
    {
        pYesNoMenu->Update();
        if (Activated != 0)
        {
            SFX_PlayX(5, 0x2000, 0);
            Flash_Fade(0x20, 1);
            if (pYesNoMenu->ChoiceIs(GameText.pYes) != 0)
            {
                Front_SetSaveAction(SAVE_GAME);
                Front_InitMemCardMenus(
                    0x200, SVSTATUS_CHECKINGCARDPAUSE, STATUS_ENDINGSCREEN);
                Status = STATUS_MEMCARDMENUS;
            }
            else
            {
                Status = STATUS_ENDINGSCREEN;
            }
        }
        if (GoBack != 0)
        {
            SFX_PlayX(2, 0x2000, 0);
            Status = STATUS_ENDINGSCREEN;
        }
        break;
    }

    case STATUS_ENDLEVELFADESCREEN:
    {
        Flash_Update();
        if (Flash_FadeFinished() != 0)
        {
            EndCode = ENDCODE_PLAYERQUIT;
            SwitchOffPauseMode = 1;
            Horse_Reset();
        }
        break;
    }

    case STATUS_DOSAVEFADESCREEN:
    case STATUS_NOSAVEFADESCREEN:
    {
        Flash_Update();
        if (Flash_FadeFinished() != 0)
        {
            if (Status == STATUS_DOSAVEFADESCREEN)
            {
                EndCode = ENDCODE_ENDLEVELDOSAVE;
            }
            else
            {
                EndCode = ENDCODE_ENDLEVELNOSAVE;
            }
            SwitchOffPauseMode = 1;
        }
        break;
    }

    case STATUS_SETSOUNDLEVELS:
    {
        pSoundMenu->Update();
        XALEVEL =
            ((pSoundMenu->ChoiceIs(musicBuf) != 0) ||
             (pSoundMenu->ChoiceIs(GameText.pSkipTrack) != 0))
                ? GPreferences.Options.MusicVolume * 0x15
                : 0;
        Redbook_XAUpdateVolume();

        if ((pSoundMenu->ChoiceIs(musicBuf) != 0) ||
            (pSoundMenu->ChoiceIs(GameText.pSkipTrack) != 0))
        {
            Redbook_XAPause(false);
        }
        else
        {
            Redbook_XAPause(true);
        }

        if ((Pad[0].Left.Triggered != 0) || (Pad[1].Left.Triggered != 0))
        {
            Pad[0].Left.Triggered = 0;
            Pad[1].Left.Triggered = 0;
            if (pSoundMenu->ChoiceIs(soundBuf) != 0)
            {
                if (soundVol > 0)
                {
                    soundVol--;
                }
                reinterpret_cast<SCareerOwner *>(&GCareer)->Options.SFXVolume = (soundVol * 6) / 5;
                GPreferences.Options.SFXVolume =
                    reinterpret_cast<SCareerOwner *>(&GCareer)->Options.SFXVolume;
                sprintf(soundBuf, GameText.pSFXLevel, soundVol);
                SFXLEVEL = reinterpret_cast<SCareerOwner *>(&GCareer)->Options.SFXVolume * 0x555;
                SFX_PlayX(5, 0x2000, 0);
            }
            else if (pSoundMenu->ChoiceIs(musicBuf) != 0)
            {
                if (musicVol > 0)
                {
                    musicVol--;
                }
                reinterpret_cast<SCareerOwner *>(&GCareer)->Options.MusicVolume = (musicVol * 6) / 5;
                GPreferences.Options.MusicVolume =
                    reinterpret_cast<SCareerOwner *>(&GCareer)->Options.MusicVolume;
                sprintf(musicBuf, GameText.pMusicLevel, musicVol);
            }
        }
        if ((Pad[0].Right.Triggered != 0) || (Pad[1].Right.Triggered != 0))
        {
            Pad[0].Right.Triggered = 0;
            Pad[1].Right.Triggered = 0;
            if (pSoundMenu->ChoiceIs(soundBuf) != 0)
            {
                if (soundVol < 10)
                {
                    soundVol++;
                }
                reinterpret_cast<SCareerOwner *>(&GCareer)->Options.SFXVolume = (soundVol * 6) / 5;
                GPreferences.Options.SFXVolume =
                    reinterpret_cast<SCareerOwner *>(&GCareer)->Options.SFXVolume;
                sprintf(soundBuf, GameText.pSFXLevel, soundVol);
                SFXLEVEL = reinterpret_cast<SCareerOwner *>(&GCareer)->Options.SFXVolume * 0x555;
                SFX_PlayX(5, 0x2000, 0);
            }
            else if (pSoundMenu->ChoiceIs(musicBuf) != 0)
            {
                if (musicVol < 10)
                {
                    musicVol++;
                }
                reinterpret_cast<SCareerOwner *>(&GCareer)->Options.MusicVolume = (musicVol * 6) / 5;
                GPreferences.Options.MusicVolume =
                    reinterpret_cast<SCareerOwner *>(&GCareer)->Options.MusicVolume;
                sprintf(musicBuf, GameText.pMusicLevel, musicVol);
            }
        }
        if (Activated != 0)
        {
            if (pSoundMenu->ChoiceIs(GameText.pSkipTrack) != 0)
            {
                Redbook_XANextTrack();
            }
            else if (pSoundMenu->ChoiceIs(GameText.pBackToMainMenu) != 0)
            {
                GoBack = 1;
            }
        }
        if (GoBack != 0)
        {
            SFX_PlayX(2, 0x2000, 0);
            Status = STATUS_PAUSEDMENU;
            XALEVEL = reinterpret_cast<SCareerOwner *>(&GCareer)->Options.MusicVolume * 0x15;
            Redbook_XAUpdateVolume();
            Redbook_XARestore(Front_PausedSongPosition());
        }
        break;
    }

    case STATUS_ENDHEATJUDGES:
    {
        break;
    }

    case STATUS_BEGINHEAT:
    {
        break;
    }

    case STATUS_MPC_BEGIN:
    {
        break;
    }

    case STATUS_MPC_BEGINHEAT:
    {
        break;
    }

    case STATUS_MPC_JUDGES:
    {
        break;
    }

    case STATUS_MPC_LADDER:
    {
        // Case 30: MPC ladder
        break;
    }

    case STATUS_MPC_LADDERFINAL:
    {
        // Case 31: MPC ladder final
        break;
    }

    case STATUS_MPC_SWITCH:
    {
        // Case 32: MPC switch
        break;
    }

    case STATUS_MPC_ENDHEAT:
    {
        // Case 33: MPC end heat
        break;
    }

    case STATUS_MPC_WINNER:
    {
        // Case 34: MPC winner
        break;
    }

    case STATUS_ENDGAMEALLGOLD:
    {
        // Case 36: End game all gold
        break;
    }

    case STATUS_ENDGAMEPLAYERMOVIE:
    {
        // Case 37: End game player movie
        break;
    }

    case STATUS_ENDGAMEBAILMOVIE:
    {
        // Case 38: End game bail movie
        break;
    }

    case STATUS_ENDGAMESAVE:
    {
        // Case 39: End game save
        break;
    }

    case STATUS_ENDGAMECREDITS:
    {
        // Case 40: End game credits
        break;
    }

    case STATUS_LOADSUCCESS:
    {
        // Case 43: Load success
        break;
    }

    case STATUS_LOADFAILED:
    {
        // Case 44: Load failed
        break;
    }

    default:
        break;
    }

switch_end:
    // Tear down pause mode after a state requests gameplay to resume.
    if (SwitchOffPauseMode != 0)
    {
        GamePaused = 0;
        GClear1 = 0;
        GClear2 = 0;
        Status = STATUS_NORMAL;
        if (pPausedMenu != 0)
        {
            Front_DestroyMenu(pPausedMenu);
        }
        {
            pPausedMenu = 0;
            if (EndCode != ENDCODE_PLAYERQUIT &&
                EndCode != ENDCODE_ENDLEVELDOSAVE &&
                EndCode != ENDCODE_ENDLEVELNOSAVE)
            {
                SFX_Unpause();
            }
        }
    }
}

/**
 * Front_FindLevel__FPc
 *
 * Address:   0x8002CC00
 * Mangled:   Front_FindLevel__FPc
 * Demangled: Front_FindLevel(char *)
 *
 * @param pTRGName Trigger filename to search for in the level table.
 * @return Matching level record, or null when the name is absent.
 */
SLevel *Front_FindLevel(char *pTRGName)
{
    SLevel *pLevel = 0;
    int k = 0;
    while (*Levels[k].pTRGName != 0)
    {
        if (Utils_CompareStrings(Levels[k].pTRGName, pTRGName,
                                 static_cast<unsigned int>(-1)) != 0)
        {
            CurrentGameLevel = k;
            pLevel = &Levels[k];
            break;
        }
        if (++k >= 0x29)
            break;
    }
    return pLevel;
}

/**
 * Front_SaveGameState__Fv
 *
 * Address:   0x8002CCC0
 * Mangled:   Front_SaveGameState__Fv
 * Demangled: Front_SaveGameState(void)
 *
 */
void Front_SaveGameState(void)
{
    char *pSource = TrigFileName;
    char *pDest = LastGameState.State.TrigFileName;
    while (*pSource != '\0')
    {
        *pDest++ = *pSource++;
    }
    *pDest = '\0';

    CVector Dummy;
    CSVector *pAngles =
        reinterpret_cast<CSVector *>(Trig_GetPosition(Dummy, RestartNode));
    pSource = reinterpret_cast<char *>(pAngles + 1);
    pDest = LastGameState.State.RestartName;
    while (*pSource != '\0')
    {
        *pDest++ = *pSource++;
    }
    *pDest = '\0';

    CBruce *pBruce = reinterpret_cast<CBruce *>(MechList);
    if (pBruce != 0)
    {
        // Directly storing the member changes fifteen instructions and one
        // relocation; the original keeps the loaded health value live first.
        int health = pBruce->CSuper.CBody.mHealth;
        LastGameState.State.WeaponsGot = 0;
        LastGameState.State.Health = health;
    }

    Front_SavePreferences(LastGameState.Preferences);
    LastGameState.Checksum = Front_CalculateGameChecksum(LastGameState);
}

/**
 * Front_CalculateGameChecksum__FR9SSaveGame
 *
 * Address:   0x8002CD9C
 * Mangled:   Front_CalculateGameChecksum__FR9SSaveGame
 * Demangled: Front_CalculateGameChecksum(SSaveGame &)
 *            XORs the saved state and preferences, excluding the checksum.
 *
 * @param Game save record to checksum
 * @return nonzero XOR checksum
 */
unsigned int Front_CalculateGameChecksum(SSaveGame &Game)
{
    unsigned int chksum = 0;
    // MAIN.SYM renders this debug local as signed, but the target's `sltiu`
    // loop bound proves the source expression was compiled as unsigned.
    unsigned int i = 1;

    do
    {
        chksum ^= reinterpret_cast<int *>(&Game.State)[i - 1];
        i++;
    } while (i < 0x22);
    return chksum | 1;
}

/**
 * Front_LoadGame__FP9SSaveGame
 *
 * Address:   0x8002CDC8
 * Mangled:   Front_LoadGame__FP9SSaveGame
 * Demangled: Front_LoadGame(SSaveGame *)
 *            Linear initialization: display screen, init rand, SFX, level setup, spool, skater load
 *
 * @param pGame  Game to operate on.
 */
void Front_LoadGame(SSaveGame *pGame)
{
    // The original's prologue completes every callee-saved store before it
    // builds the first call's %hi/%lo argument pair.  cc1plus' post-reload
    // scheduler otherwise ranks the lui/addiu chain above the leaf stores and
    // hoists it to insn 4, which also robs the jal of its %lo delay-slot
    // filler.  An empty volatile asm anchors the entry block so the saves are
    // emitted first and the addiu lands in the delay slot, as in the original.
    __asm__ __volatile__("");
    Front_DisplayScreen(
        FrontAddressViewOwners::s_s2demo2_bmp_800d8e78);

    TrigFileName = pGame->State.TrigFileName;

    Utils_InitialRand(0x12a6cc58);
    FileIO_SetSectorsPerFrame(0x7d0);
    SFX_Init(FrontAddressViewOwners::s_skate2_vab_800d8e84);

    // Level setup from Levels table
    SFX_SpoolInLevelSFX(Levels[GLevel].pVab);

    SFX_SetReverbType(0);
    printf(Front_LoadingSpritesText());

    // Create sprites (score, special, bar, paused)
    {
        Sprite *spr = static_cast<Sprite *>(
            ::CClass::operator new(sizeof(Sprite)));
        if (spr != 0)
        {
            spr = ::new (spr) Sprite(
                FrontAddressViewOwners::s_scorep_bmp_800d8e90,
                1, 0x100, 0x100);
        }
        pScoreSprite = spr;
    }
    {
        Sprite *spr = static_cast<Sprite *>(
            ::CClass::operator new(sizeof(Sprite)));
        if (spr != 0)
        {
            spr = ::new (spr) Sprite(
                FrontAddressViewOwners::s_special_bmp_800d8e9c,
                1, 0x100, 0x100);
        }
        pSpecialSprite = spr;
    }
    {
        Sprite *spr = static_cast<Sprite *>(
            ::CClass::operator new(sizeof(Sprite)));
        if (spr != 0)
        {
            spr = ::new (spr) Sprite(
                FrontAddressViewOwners::s_spec_bar_bmp_800d8ea8,
                1, 0x100, 0x100);
        }
        pBarSprite = spr;
    }
    {
        Sprite2 *spr = new Sprite2(
            FrontAddressViewOwners::s_paused_bmp_800d8eb8,
            1, 0x40, 0x100);
        pPausedSprite = reinterpret_cast<Sprite *>(spr);
        spr->JustX = 1;
    }

    int region = Spool_PSX(DAT_800d8ec4, 0, 0, 1);
    PSXRegion[region].Protected = 1;
    region = Spool_PSX(s_items_800d8ecc, 0, 0, 1);
    PSXRegion[region].Protected = 1;
    CurrentItemsFileRegion = static_cast<u8>(region);
    Spool_Sync();

    FileIO_SetSectorsPerFrame(4);
    Panel_Init();
    Font_Init();
    Bit_Init();
    Spool_ClearAllPSXs();

    if (Levels[GLevel].isCompetition != 0 && GGame == 1)
    {
        region = Spool_PSX(
            FrontAddressViewOwners::s_SkMedals_800d8ed4, 0, 0, 1);
    }
    CurrentMedalsFileRegion = static_cast<u8>(region);

    int NumberOfOnscreenSkaters = 1;
    if (GNumberOfPlayers == 2 && GGame != 7)
    {
        NumberOfOnscreenSkaters = 2;
    }

    int i = 0;
    if (NumberOfOnscreenSkaters != 0)
    {
        do
        {
            int selectedSkater;
            if (i == 0)
            {
                selectedSkater = SelectedSkater;
            }
            else
            {
                selectedSkater = SelectedSkater2;
            }

            if (Cheat_BigHead != 0)
            {
                unsigned char head = Stats[selectedSkater].Head;
                signed char neck = Stats[selectedSkater].Neck;
                GHeadPart = head;
                GHeadUp = neck;
            }

            if (Create_UseSkater(selectedSkater) != 0)
            {
                Create_InitSkater(1);
            }
            else if (NumberOfOnscreenSkaters >= 2)
            {
                GSkaterRegion[i] = Spool_GetReducedSkater(
                        Stats[selectedSkater].PSXName2,
                        s_sk2anim_800d8ee0);
            }
            else
            {
                GSkaterRegion[i] = Spool_GetReducedSkater(
                        Stats[selectedSkater].PSXName,
                        s_sk2anim_800d8ee0);
            }

            GHeadPart = -1;
            i++;
        } while (i < NumberOfOnscreenSkaters);
    }

    if (Front_FindLevel(TrigFileName) != 0)
    {
        SFX_MIDIStop();
        SFX_SpoolOutMIDI();
    }

    FileIO_SetSectorsPerFrame(0x7d0);
    if (GenerateLevel != 0)
    {
        LevelGenerator *ed = static_cast<LevelGenerator *>(
            ::CClass::operator new(FrontLevelGeneratorStorageSize));
        if (ed != 0)
        {
            ed = ::new (ed) LevelGenerator(true);
        }

        char theme_name[10];
        int r;
        sprintf(theme_name, s_SkEd_d_800d8ee8, GenerateThemeNumber + 1);
        ed->LoadModels(theme_name);
        ed->CreatePSX(&r, true);
        ed->FakeTrig();
        if (ed != 0)
        {
            Front_DestroyPolymorphic(ed);
        }
        *reinterpret_cast<FrontEditorLevelSuffixBytes *>(
            pGame->State.RestartName) =
            DAT_800d8ef0[0];
    }
    else
    {
        Trig_LoadTRG(TrigFileName);
        Trig_InitialParseTRGFile();
        Trig_ParseTRGFile();
    }

    Rail_CreateRails();
    FileIO_SetSectorsPerFrame(4);

    if (BackOut == 0)
    {
        CreateSkatersAndCameras();
        if (pGame->State.RestartName[0] != 0)
        {
            Trig_SetRestart(pGame->State.RestartName);
        }
        Trig_ExecuteRestart();
        Front_SaveGameState();

        RECT oldClipRect;
        Font *sourceFont = FontManager::GetFont(Front_GrafTrickFont);
        Font promptFont = *sourceFont;
        // Passing &promptFont directly changes two instructions at the
        // polymorphic construction boundary; retain the original pointer local.
        Font *promptFontPointer = &promptFont;
        promptFont.Scale = 0x800;
        promptFont.JustX = 0;
        promptFont.JustY = 3;

        PromptScreenElement *pElement = new PromptScreenElement(
            FrontAddressViewOwners::s_s2prmpt1_bmp_800d8ef4,
            FrontAddressViewOwners::s_to_continue_800d8f04,
            promptFontPointer, 0xbd, 0xda, -1, -1);

        unsigned int oldDb_SkyColor = Db_SkyColor;
        Db_SkyColor = 0;
        Db_UpdateSky();

        int flash = 0;
        while (Pad[0].X.Triggered == 0)
        {
            RECT newClipRect = DAT_800d8f10[0];
            Db_FlipClear();

            oldClipRect = pDoubleBuffer->DrawClip;
            newClipRect.y += pDoubleBuffer->DrawClip.y;
            pDoubleBuffer->DrawClip = newClipRect;

            if (flash > 0x28)
            {
                Front_DisplayPolymorphic(pElement, 0, 0);
            }

            flash++;
            if (flash > 0x50)
            {
                flash = 0;
            }

            DrawSync(0);
            Pause(1);
            Db_DisplayDraw();
            Pad_Update();

            pDoubleBuffer->DrawClip = oldClipRect;
        }

        if (pElement != 0)
        {
            Front_DestroyPolymorphic(pElement);
        }
        Db_SkyColor = oldDb_SkyColor;
        Db_UpdateSky();
        Pad[0].X.Triggered = 0;
    }
}


/**
 * Destroy the global HORSE effect with the target's complete-object flag.
 *
 * A normal destructor expression emits the same flag, but making the global
 * itself non-trivial causes GCC 2.x to name this wrapper after
 * `GHorseContinue` instead of the target's `LoadingScreen_Main` identity.
 */
void Front_GlobalDestroy_LoadingScreen_Main(void)
    asm("_GLOBAL_.D.LoadingScreen_Main__Fv");
void Front_GlobalDestroy_LoadingScreen_Main(void)
{
    Front_HorseFallingText().~FallingTextEffect();
}

/** Invoke the global HORSE effect constructor. */
void Front_GlobalInit_LoadingScreen_Main(void)
    asm("_GLOBAL_.I.LoadingScreen_Main__Fv");
void Front_GlobalInit_LoadingScreen_Main(void)
{
    FallingTextEffect_ConstructAbi(&Front_HorseFallingText());
}

/** Target-ordered constructor-table entry for the explicit global wrapper. */
static void (*volatile Front_GlobalCtorSlot)(void)
    __attribute__((section(".ctors"))) = Front_GlobalInit_LoadingScreen_Main;

/** Target-ordered destructor-table entry for the explicit global wrapper. */
static void (*volatile Front_GlobalDtorSlot)(void)
    __attribute__((section(".dtors"))) = Front_GlobalDestroy_LoadingScreen_Main;

/**
 * Destroy FRONT's private CClass base and optionally release its storage.
 *
 * GCC 2.x emits one file-local CClass destructor copy for each owner whose
 * derived vtable embeds the private base-table suffix.  MAIN.SYM places this
 * copy last in FRONT's text at 0x8002D678.
 *
 * @param self   Implicit CClass `this` pointer.
 * @param flags  GCC 2.x in-charge flags; bit zero requests operator delete.
 */
extern "C" {
static void __DOT__6CClass_8002D678(CClass *self, int flags)
{
    self->vtable = &FrontCMenuVTableAddressView::__vt_5CMenu + 4;
    if (flags & 1)
    {
        ::CClass::operator delete(self);
    }
}
}
