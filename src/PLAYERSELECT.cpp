// PLAYERSELECT.cpp -- Reconstructed translation unit
// Original: PLAYERSELECT.CPP
// Functions: 53 reconstructed
// Address range: 0x8008AC80 - 0x8008EECC
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

// Owner-local declarations attributed to PLAYERSELECT.cpp by MAIN.SYM.
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

struct CBit;
struct CGlow;
struct CSpecialDisplay;
struct CFlatBit;
struct Font;
struct TextEffect;
struct FontManager;
struct PREManager;
struct RailPoint;
struct SCardHead;
struct SCamera;
struct STexWibVertInfo;
struct SViewport;
struct SColCache;
struct SLineInfo;
struct STexWibItemInfo;
struct SBlockHeader;
struct ControllerData;
struct SDoubleBuffer;
struct SCareer;
struct SStats;
struct ICON_INFO;
struct SBoard;
struct hierarchyInfo;
struct SkaterScreenElement;

extern "C" SStats Stats[];
// Owner-local object layouts used by PLAYERSELECT.cpp.

#define SCREENTOOLS_AGGREGATE_LAYOUT
#define SCREENTOOLS_AGGREGATE_LAYOUT_ONLY
#include "SCREENTOOLS.h"
#undef SCREENTOOLS_AGGREGATE_LAYOUT_ONLY
#undef SCREENTOOLS_AGGREGATE_LAYOUT
#include "CITEM.h"
#define OB_LAYOUT_ONLY
#define OB_OWNER_LAYOUT
#include "OB.h"
#undef OB_OWNER_LAYOUT
#undef OB_LAYOUT_ONLY
#include "PRE.h"
#include "FRONTSCREENS.h"
#include "M3D.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#define GLOBALS_SCENE_VALUES_ONLY
#include "globals.h"
#undef GLOBALS_SCENE_VALUES_ONLY
#include "PSXREGION.h"

struct BioScreenElement;
struct MultiPlayerScreenElement;
struct SHook;
struct SModel;

#include "SVIEWPORT.h"

#include "SLIGHT.h"

#include "SCAMERA.h"

/** Display-buffer slice through the preview ordering table and top scanline. */
struct PlayerSelectDoubleBuffer {
    unsigned char mReserved[0x84];
    unsigned int *PanelOT;
    unsigned int *OrderingTable;
    unsigned int *ViewportOT;
    unsigned short Top;
    unsigned short TopUpper;
};

/** Fixed prefix of a model file carrying its compacted-data directory. */
struct PlayerSelectAnimFileHeader {
    unsigned int Magic;
    unsigned int DataOffset;
    unsigned int EntryCount;
};

/** One 36-byte directory row; the terminal row supplies the block count. */
struct PlayerSelectAnimDirectoryEntry {
    unsigned int BlockCount;
    unsigned int Reserved[8];
};

/** Strided view of the pPSX member across the 0x40-byte region array. */
struct PlayerSelectRegionPSXColumn {
    unsigned int *Value;
    unsigned char NextRegion[60];
};

/** Strided view of the ppModels member across the 0x40-byte region array. */
struct PlayerSelectRegionModelsColumn {
    struct SModel **Value;
    unsigned char NextRegion[60];
};

/** Sliced-image base services used by front-end sprites. */
struct SlicedImage2 {
    void *vtable;
    unsigned char opaque[56];

    void pack();
    void unpack();
};

/** Front-end sprite storage allocated through the original CClass arena. */
struct Sprite2 : SlicedImage2 {

    static void *operator new(unsigned int size) throw()
    {
        return ::CClass::operator new(size);
    }
    Sprite2(char *fileName, int useVram, int width, int height);
    Sprite2(unsigned char *data, int useVram, int x, int y);
};

/** Front-end skater object constructed through the original CItem arena. */
struct CSuper : CSuperOwner {
    static void *operator new(unsigned int size) throw()
    {
        return ::CItem::operator new(size);
    }
    CSuper();
    void CalculateAnimOrder(char **animHierarchy, int animPartCount,
                            char **modelHierarchy, int modelPartCount);
    void CycleAnim(int animation, signed char direction);
};

/** Original polymorphic prefix needed for Font assignment semantics. */
struct PlayerSelectClassAssignmentBase {
    virtual ~PlayerSelectClassAssignmentBase();
};

/** Owner-local Font view whose assignment preserves the destination vtable. */
struct PlayerSelectFontAssignmentView : PlayerSelectClassAssignmentBase {
    int JustX;
    int JustY;
    int SpaceX;
    int SpaceY;
    int R;
    int G;
    int B;
    void *Aliased;
    void *Shadow;
    int ShadowDx;
    int ShadowDy;
    int CharMap;
    int FixedWidth;
    int Scale;
    struct FontCharacter *CharTab;
    int NumChars;
    int Clut;
    struct TextEffect *CurrentTextEffect;
    unsigned char OldR;
    unsigned char OldG;
    unsigned char OldB;
};

/** Proven item-pose prefix of the front-end skater model. */
struct CSuperPosePrefix {
    unsigned short mFlags;
    unsigned short mInquiry;
    struct CVector mPos;
    struct CSVector mAngles;
};

#define STATS_BOARD_LAYOUT
#define STATS_BOARD_LAYOUT_ONLY
#include "STATS.h"
#undef STATS_BOARD_LAYOUT_ONLY
#undef STATS_BOARD_LAYOUT

/** Layout of one skater-select icon and its full-window offsets. */
struct ICON_INFO {
    char *Name;
    int x;
    int y;
    int fullOffsetX;
    int fullOffsetY;
    int mHide;
    int mSpoolID;
};

/** Hierarchy pointers and active part count loaded for one model. */
struct hierarchyInfo {
    char *PSH_Hierarchy[32];
    int mPartCount;
};

/** Full skater-stat stride used by indexed access to the Stats array. */
struct SStats {
    char *PSXName;
    char *PSXName2;
    char *FrontName;
    int ComboID;
    unsigned char Board;
    unsigned char Wheel1;
    unsigned char Wheel2;
    unsigned char Head;
    char Neck;
    char pad_15[3];
    int Toughness;
    int Speed;
    int Air;
    int Balance;
    int Street;
    char *Name;
    unsigned int BoardChecksum;
    unsigned int WheelChecksum;
    unsigned int TruckChecksum;
    struct SBoard Boards[8];
    unsigned char Ability[12];
    char *PortraitName;
    char **BioText;
    int Available;
    int FrontEndOrientation;
};

/** Model-and-animation presentation element for the active skater. */
struct SkaterScreenElement {
    struct ScreenElementList ScreenElementList;
    int mSkaterChanged;
    int mShowCard;
    struct CSuper *mpFrontEndSkater[14];
    int mFlash;
    struct ScreenElement *mpIcon[1];
    int mSpoolID[42];
    struct Sprite2 *mpFaceSprite[14];
    struct Font *mpFont;
    int mFullWindow;
    struct ScreenElement *mpSkaterWindow;
    struct Sprite2 *mpIconSprite[8];
    int mAnimRegion;
    int mModelRegionGeneric;
    unsigned int *mModelFile[14];
    int mVisibleSkater;
    struct Item mCardItem;
    int mCardRegion;
    struct hierarchyInfo mAnimHierInfo;
    struct hierarchyInfo mModelHierInfo[14];
    char *mpAnimHierData;
    char *mpModelHierData[14];
    int mSkaterLoaded[14];

    static void *operator new(unsigned int size) throw()
    {
        return ::CClass::operator new(size);
    }
    static void operator delete(void *storage)
    {
        ::CClass::operator delete(storage);
    }
    SkaterScreenElement(int fullWindow, struct Font *pFont,
                        int homeX, int homeY, int screenX, int screenY);
    ~SkaterScreenElement();
    void RotateSkater(int angle);
    void LoadAllIcons();
    void UpdateSpoolItems();
    void AddSpoolItems();
    void LoadAllSkaters();
    unsigned int *GetReducedPSXFile(char *modelFile);
    int GetReducedAnim(char *animFile);
    void refreshSkater();
    void update();
    void refresh();
    void draw(int x, int y);
};

/** Top-level player-select screen and its child panes. */
struct PlayerSelectScreen {
    struct ScreenElementList ScreenElementList;
    struct Sprite2 *mBackground;
    struct PlayerSelectFontAssignmentView mFont1;
    struct PlayerSelectFontAssignmentView mFont2;
    struct PlayerSelectFontAssignmentView mFont3;
    int mMulti;
    int mMaxPlayers;
    struct SkaterScreenElement *mpSkaterScreenElement;
    struct ScreenElement *mpCareerScreenElement;
    struct BioScreenElement *mpBioScreenElement;
    struct MultiPlayerScreenElement *mpMultiScreenElement;

    void init(int gameMode);
    void draw();
    void update();
    void refresh();
    int handlePress();
};

/** Original seven-slot dispatch prefix shared by front-end elements. */
struct ScreenElementDispatchTable {
    void *mReserved0;
    void *mReserved1;
    void (*Destroy)(void *, int);
    void (*Update)(void *);
    void (*Draw)(void *, int, int);
    int (*Width)(void *);
    void (*Refresh)(void *);
};

/** Sprite dispatch prefix whose final slot accepts the draw-first flag. */
struct Sprite2DispatchTable {
    void *mReserved0;
    void *mReserved1;
    void (*Destroy)(void *, int);
    void *mSetData;
    void *mReserved4;
    void (*Draw)(void *, int, int, int);
};

typedef char playerselect_board_size_must_be_32[
    (sizeof(SBoard) == 32) ? 1 : -1];
typedef char playerselect_light_size_must_be_52[
    (sizeof(SLight) == 52) ? 1 : -1];
typedef char playerselect_double_buffer_prefix_size_must_be_148[
    (sizeof(PlayerSelectDoubleBuffer) == 148) ? 1 : -1];
typedef char playerselect_anim_file_header_size_must_be_12[
    (sizeof(PlayerSelectAnimFileHeader) == 12) ? 1 : -1];
typedef char playerselect_anim_directory_entry_size_must_be_36[
    (sizeof(PlayerSelectAnimDirectoryEntry) == 36) ? 1 : -1];
typedef char playerselect_sprite_size_must_be_60[
    (sizeof(Sprite2) == 60) ? 1 : -1];
typedef char playerselect_csuper_size_must_be_400[
    (sizeof(CSuper) == 400) ? 1 : -1];
typedef char playerselect_hierarchy_size_must_be_132[
    (sizeof(hierarchyInfo) == 132) ? 1 : -1];
typedef char playerselect_stats_size_must_be_344[
    (sizeof(SStats) == 344) ? 1 : -1];
typedef char playerselect_skater_element_size_must_be_2648[
    (sizeof(SkaterScreenElement) == 2648) ? 1 : -1];
typedef char playerselect_screen_size_must_be_372[
    (sizeof(PlayerSelectScreen) == 372) ? 1 : -1];
typedef char playerselect_screen_dispatch_size_must_be_28[
    (sizeof(ScreenElementDispatchTable) == 28) ? 1 : -1];
typedef char playerselect_sprite_dispatch_size_must_be_24[
    (sizeof(Sprite2DispatchTable) == 24) ? 1 : -1];
typedef char playerselect_font_assignment_view_size_must_be_80[
    (sizeof(PlayerSelectFontAssignmentView) == 80) ? 1 : -1];

#include "OB.h"
#include "PREMGR.h"
#include "SFX.h"
#include "MEM.h"
#include "PAD.h"
#include "PACK.h"
#include "STRLIB.h"
#include "UTILS.h"
#include "M3DINIT.h"
#include "MAIN.h"
#include "RFRONT.h"
#include "SCREENTOOLS.h"
#include <LIBGPU.H>
#include <STDIO.H>
#include <STRINGS.H>

/** Placement construction for explicitly managed Font storage. */
inline void *operator new(unsigned int, void *address)
{
    return address;
}

// === External declarations (C linkage) ===
extern "C"
{
    extern char s__s_psh_800dc4b0[];  // Format string: "%s.psh"
    extern char s__spart__800dc4b8[]; // Format string: "%spart_"
    extern char s__s_psx_800dc4c0[];  // Format string: "%s.psx"
    extern char s__spart__800daa1c[]; // Shared rodata blob holding bio sprite names
    extern char s_Loading____800dc4a4[];
    extern char s_s2dots_bmp_800dc4f0[];
    extern char s_dummy_800dc4fc[];
    extern char s_choose_skater_800dc504[];
    extern char s_anims_fe_800dc4c8[];
    extern char s_anims_cs_psx_800dc4d4[];
    extern char DAT_800dc4e4[];
    extern char DAT_800dc4e8[];
    // 0x800DC4EC is PlayerSelect_FrontEndAnimations+4, not a standalone object.
    extern char s_player_pre_800dc2e8[];
    extern char s_s2bgd1_bmp_800dc318[];
    extern char s_s2prmpt4_bmp_800dc324[];
    extern char s_select_800dc334[];
    extern char s_s2prmpt5_bmp_800dc33c[];
    extern char s_scroll_bio_800dc34c[];
    extern char s_s2prmpt1_bmp_800dc358[];
    extern char s_accept_800dc368[];
    extern char s_s2prmpt2_bmp_800dc370[];
    extern char s_back_800dc380[];
    extern char s_TEAM_CHICKEN_800dc388[];
    extern char s_loading_pre_file_manually_800b4068[];
}

// === PlayerSelect-owned initialized data ===

/** Projection bounds used by the front-end skater preview. */
static SViewport CreateViewport1
    __attribute__((section(".data"))) = {
    0x200, 0xF0, 0, 0,
    10, 30000, 0xD55, 0,
    0, 0,
};

/** Standard and unavailable-skater lighting used by the preview models. */
static SLight Skater_CreateLight
    __attribute__((section(".data"))) = {
    {{0, 4096, 0}, {0, -4096, 0}, {0, 0, 4096}},
    0,
    {{640, 2944, 320}, {640, 2944, 320}, {640, 2944, 320}},
    0,
    {2176, 2176, 2176},
};
static SLight Skater_NotAvailableLight
    __attribute__((section(".data"))) = {
    {{0, 4096, 0}, {0, -4096, 0}, {0, 0, 4096}},
    0,
    {{320, 1472, 160}, {320, 1472, 160}, {320, 1472, 160}},
    0,
    {1088, 1088, 1088},
};

extern "C" {
/** Front-end selection state stored in PlayerSelect's small-data prefix. */
int MultiPlayerListCount __attribute__((section(".sdata"))) = 0;
int mCASTexturesSpooled __attribute__((section(".sdata"))) = 0;
/** Viewport selected for the front-end skater preview. */
static SViewport *pViewport
    __attribute__((section(".sdata"))) = &CreateViewport1;

/**
 * Skater-select icon placement, full-window offsets, and spool state.
 *
 * The icon names are part of the contiguous front-end string pool currently
 * represented by its proven base symbol.  Keeping these as relocatable
 * expressions preserves the original pointer data without baking addresses.
 */
ICON_INFO mIconInfo[8] __attribute__((section(".data"))) = {
    {(char *)(s__spart__800daa1c + 0x1A30), -63, 14, 154, 62, 1, 0},
    {(char *)(s__spart__800daa1c + 0x1A3C), -60, 14, 154, 62, 1, 0},
    {(char *)(s__spart__800daa1c + 0x1A48), -48, 79, 154, 62, 1, 0},
    {(char *)(s__spart__800daa1c + 0x1A54), -34, 76, 154, 62, 1, 0},
    {(char *)(s__spart__800daa1c + 0x1A54), -34, 76, 154, 62, 1, 0},
    {(char *)(s__spart__800daa1c + 0x1A60), -52, 100, 154, 62, 1, 0},
    {(char *)(s__spart__800daa1c + 0x1A6C), -44, 100, 154, 62, 1, 0},
    {(char *)(s__spart__800daa1c + 0x1A78), 36, 34, -53, 15, 1, 0},
};

/** Selected skaters for the current multiplayer setup. */
int MultiPlayerList[8] __attribute__((section(".data"))) = {0};
}

// === Player-select screen-element layouts ===

#include "PROMPTSCREENELEMENT.h"


/** Multiplayer player-number prompt and selection state. */
struct MultiPlayerScreenElement {
    struct ScreenElementList ScreenElementList;
    struct Font *mpFont;
    int mFlash;

    static void *operator new(unsigned int size) throw()
    {
        return ::CClass::operator new(size);
    }
    MultiPlayerScreenElement(Font *pFont, int homeX, int homeY, int screenX, int screenY);
    ~MultiPlayerScreenElement();
    void ResetStartingLocation();
    void refresh();
    void update();
};


/** Ten-meter ability display for the selected skater. */
struct StatsScreenElement {
    struct ScreenElementList ScreenElementList;
    struct SpriteScreenElement *mpDots;

    static void *operator new(unsigned int size) throw()
    {
        return ::CClass::operator new(size);
    }
    StatsScreenElement(int homeX, int homeY, int screenX, int screenY);
    ~StatsScreenElement();
    void draw(int x, int y);
    void update();
};


/** Scrollable biography pane for the selected skater. */
struct BioScreenElement {
    struct ScreenElementList ScreenElementList;
    int mSize;
    struct ScreenElement *mpBioText[16];
    int mBioTextCount;
    struct Font *mpFont;

    static void *operator new(unsigned int size) throw()
    {
        return ::CClass::operator new(size);
    }
    BioScreenElement(Font *pFont, int homeX, int homeY, int screenX, int screenY);
    ~BioScreenElement();
    void HandleDown();
    void HandleUp();
    void draw(int x, int y);
    void refresh();
    void update();
};


/** Career totals and earned-medal display for the selected skater. */
struct CareerScreenElement {
    struct ScreenElementList ScreenElementList;
    struct ScreenElement *mpMedalIcons[3];
    int mDelay[9];

    static void *operator new(unsigned int size) throw()
    {
        return ::CClass::operator new(size);
    }
    CareerScreenElement(Font *pFont, int homeX, int homeY, int screenX, int screenY);
    ~CareerScreenElement();
    void draw(int x, int y);
    void refresh();
    void update();
};


/** Animated numeric score row. */
struct ScoreScreenElement {
    struct ScreenElementList ScreenElementList;
    int mTargetValue;
    int mValue;
    struct TextScreenElement *mpScoreElement;
    struct Font *mpFont;

    static void *operator new(unsigned int size) throw()
    {
        return ::CClass::operator new(size);
    }
    ScoreScreenElement(int value, Font *pFont, int x, int y);
    ~ScoreScreenElement();
    void draw(int x, int y);
    void setValue(int value);
    void update();
};

typedef char playerselect_prompt_element_size_must_be_104[
    (sizeof(PromptScreenElement) == 104) ? 1 : -1];
typedef char playerselect_multiplayer_element_size_must_be_112[
    (sizeof(MultiPlayerScreenElement) == 112) ? 1 : -1];
typedef char playerselect_stats_element_size_must_be_108[
    (sizeof(StatsScreenElement) == 108) ? 1 : -1];
typedef char playerselect_bio_element_size_must_be_180[
    (sizeof(BioScreenElement) == 180) ? 1 : -1];
typedef char playerselect_career_element_size_must_be_152[
    (sizeof(CareerScreenElement) == 152) ? 1 : -1];
typedef char playerselect_score_element_size_must_be_120[
    (sizeof(ScoreScreenElement) == 120) ? 1 : -1];

struct Spooler {
    static Spooler *SpoolMgr __attribute__((section(".data")));
    void Reset();
    static void OrderHelper(int *order, int count, int first);
    int AddFile(char *filename, int priority);
    void Spooool();
    void Update();
    int IsFileLoaded(int spoolID);
    void *GetFile(int spoolID);
};
// === Free function declarations ===
void SetClippingBox3(int x, int y, int w, int h);
char *Spool_LoadPSH2(char *fileBuffer, char *baseName, char **nameList, int *nameCount, int unused);
void Spool_StripAnimFile(char *fileData);

// === Typed collaborator declarations ===
extern "C" {
    TextScreenElement *PlayerSelect_ConstructTextElement(
        TextScreenElement *element, char *text, Font *font,
        int homeX, int homeY, int screenX, int screenY)
        asm("__17TextScreenElementPcP4Fontiiii");
    SpriteScreenElement *PlayerSelect_ConstructSpriteElement(
        SpriteScreenElement *element, char *fileName,
        int homeX, int homeY, int screenX, int screenY,
        int justY, int justX)
        asm("__19SpriteScreenElementPciiiiii");
    extern char s__7d_800dc3a8[];       // Format string: "%7d"
    extern char s_player__d_800dc514[]; // Format string: "player %d"
}

#include "IMAGE.h"

// === ABI bridges for collaborators not yet exposed as natural C++ classes ===
extern "C" PlayerSelectScreen *PlayerSelectScreen_ctor(
    PlayerSelectScreen *, int) asm("__18PlayerSelectScreeni");
extern "C" void PlayerSelectScreen_dtor(PlayerSelectScreen *, int)
    asm("_._18PlayerSelectScreen");
extern "C" void PlayerSelect_ListDtor(ScreenElementList *, int)
    asm("_._17ScreenElementList");
extern "C" void PlayerSelect_ListDraw(ScreenElementList *, int, int)
    asm("draw__17ScreenElementListii");
extern "C" void PlayerSelect_ListUpdate(ScreenElementList *)
    asm("update__17ScreenElementList");
extern "C" void PlayerSelect_ElementUpdate(ScreenElement *)
    asm("update__13ScreenElement");
// === draw() helper declarations ===
#include "PDOUBLEBUFFER.h"


// === PlayerSelect-owned virtual dispatch tables ===

// C-linkage views of the methods stored in the original GCC 2.x tables.
extern "C" void PlayerSelect_MultiPlayerDestructorVtable(void)
    asm("_._24MultiPlayerScreenElement");
extern "C" void PlayerSelect_MultiPlayerUpdateVtable(void)
    asm("update__24MultiPlayerScreenElement");
extern "C" void PlayerSelect_MultiPlayerRefreshVtable(void)
    asm("refresh__24MultiPlayerScreenElement");
extern "C" void PlayerSelect_StatsDestructorVtable(void)
    asm("_._18StatsScreenElement");
extern "C" void PlayerSelect_StatsUpdateVtable(void)
    asm("update__18StatsScreenElement");
extern "C" void PlayerSelect_StatsDrawVtable(void)
    asm("draw__18StatsScreenElementii");
extern "C" void PlayerSelect_SkaterDestructorVtable(void)
    asm("_._19SkaterScreenElement");
extern "C" void PlayerSelect_SkaterUpdateVtable(void)
    asm("update__19SkaterScreenElement");
extern "C" void PlayerSelect_SkaterDrawVtable(void)
    asm("draw__19SkaterScreenElementii");
extern "C" void PlayerSelect_SkaterRefreshVtable(void)
    asm("refresh__19SkaterScreenElement");
extern "C" void PlayerSelect_BioDestructorVtable(void)
    asm("_._16BioScreenElement");
extern "C" void PlayerSelect_BioUpdateVtable(void)
    asm("update__16BioScreenElement");
extern "C" void PlayerSelect_BioDrawVtable(void)
    asm("draw__16BioScreenElementii");
extern "C" void PlayerSelect_BioRefreshVtable(void)
    asm("refresh__16BioScreenElement");
extern "C" void PlayerSelect_CareerDestructorVtable(void)
    asm("_._19CareerScreenElement");
extern "C" void PlayerSelect_CareerUpdateVtable(void)
    asm("update__19CareerScreenElement");
extern "C" void PlayerSelect_CareerDrawVtable(void)
    asm("draw__19CareerScreenElementii");
extern "C" void PlayerSelect_CareerRefreshVtable(void)
    asm("refresh__19CareerScreenElement");
extern "C" void PlayerSelect_PromptDestructorVtable(void)
    asm("_._19PromptScreenElement");
extern "C" int PlayerSelect_PromptWidthVtable(void)
    asm("width__19PromptScreenElement");
extern "C" void PlayerSelect_ScoreDestructorVtable(void)
    asm("_._18ScoreScreenElement");
extern "C" void PlayerSelect_ScoreUpdateVtable(void)
    asm("update__18ScoreScreenElement");
extern "C" void PlayerSelect_ScoreDrawVtable(void)
    asm("draw__18ScoreScreenElementii");
extern "C" void PlayerSelect_ScreenDestructorVtable(void)
    asm("_._18PlayerSelectScreen");
extern "C" void PlayerSelect_ScreenUpdateVtable(void)
    asm("update__18PlayerSelectScreen");
extern "C" void PlayerSelect_ScreenRefreshVtable(void)
    asm("refresh__18PlayerSelectScreen");
extern "C" void PlayerSelect_ListUpdateVtable(void)
    asm("update__17ScreenElementList");
extern "C" void PlayerSelect_ListDrawVtable(void)
    asm("draw__17ScreenElementListii");
extern "C" int PlayerSelect_ElementWidthVtable(void)
    asm("width__13ScreenElement");
extern "C" {
/**
 * Diagnostic used when the skater archive is absent from the active PRE.
 *
 * The explicit extent preserves the target's five trailing alignment bytes
 * before the screen-element dispatch tables.
 */
static char PlayerSelect_ManualPreLoadMessage_owner[32]
    asm("s_loading_pre_file_manually_800b4068")
    __attribute__((section(".rdata"), aligned(8))) =
        "loading pre file manually\n";

/**
 * Player-select screen-element dispatch tables in original read-only order.
 *
 * GCC 2.x emits seven words per table and aligns each table to eight bytes,
 * leaving one zero padding word between adjacent tables.
 */
int __vt_24MultiPlayerScreenElement[7]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&PlayerSelect_MultiPlayerDestructorVtable,
        (int)&PlayerSelect_MultiPlayerUpdateVtable,
        (int)&PlayerSelect_ListDrawVtable,
        (int)&PlayerSelect_ElementWidthVtable,
        (int)&PlayerSelect_MultiPlayerRefreshVtable,
};

int __vt_18StatsScreenElement[7]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&PlayerSelect_StatsDestructorVtable,
        (int)&PlayerSelect_StatsUpdateVtable,
        (int)&PlayerSelect_StatsDrawVtable,
        (int)&PlayerSelect_ElementWidthVtable,
        (int)&ScreenElement_Refresh,
};

int __vt_19SkaterScreenElement[7]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&PlayerSelect_SkaterDestructorVtable,
        (int)&PlayerSelect_SkaterUpdateVtable,
        (int)&PlayerSelect_SkaterDrawVtable,
        (int)&PlayerSelect_ElementWidthVtable,
        (int)&PlayerSelect_SkaterRefreshVtable,
};

int __vt_16BioScreenElement[7]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&PlayerSelect_BioDestructorVtable,
        (int)&PlayerSelect_BioUpdateVtable,
        (int)&PlayerSelect_BioDrawVtable,
        (int)&PlayerSelect_ElementWidthVtable,
        (int)&PlayerSelect_BioRefreshVtable,
};

int __vt_19CareerScreenElement[7]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&PlayerSelect_CareerDestructorVtable,
        (int)&PlayerSelect_CareerUpdateVtable,
        (int)&PlayerSelect_CareerDrawVtable,
        (int)&PlayerSelect_ElementWidthVtable,
        (int)&PlayerSelect_CareerRefreshVtable,
};

int __vt_19PromptScreenElement[7]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&PlayerSelect_PromptDestructorVtable,
        (int)&PlayerSelect_ListUpdateVtable,
        (int)&PlayerSelect_ListDrawVtable,
        (int)&PlayerSelect_PromptWidthVtable,
        (int)&ScreenElement_Refresh,
};

int __vt_18ScoreScreenElement[7]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&PlayerSelect_ScoreDestructorVtable,
        (int)&PlayerSelect_ScoreUpdateVtable,
        (int)&PlayerSelect_ScoreDrawVtable,
        (int)&PlayerSelect_ElementWidthVtable,
        (int)&ScreenElement_Refresh,
};

int __vt_18PlayerSelectScreen[7]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&PlayerSelect_ScreenDestructorVtable,
        (int)&PlayerSelect_ScreenUpdateVtable,
        (int)&PlayerSelect_ListDrawVtable,
        (int)&PlayerSelect_ElementWidthVtable,
        (int)&PlayerSelect_ScreenRefreshVtable,
};
}


// === PlayerSelect_Main callees ===
#include "DB.h"
#include "FRONTEND2.h"
#include "SCHARACTERPROGRESS.h"

struct SPlayerSelectCareerOverlay
{
    int Version;
    int MaxTapes;
    SCharacterProgress CharacterProgress[14];
};

typedef char playerselect_character_progress_size_must_be_32[
    (sizeof(SCharacterProgress) == 32) ? 1 : -1];
typedef char playerselect_career_prefix_size_must_be_456[
    (sizeof(SPlayerSelectCareerOverlay) == 456) ? 1 : -1];

/** Owner-local virtual interface for the ScreenElement vtable prefix. */
struct PlayerSelectDrawableView {
    virtual void slot0();
    virtual void slot1();
    virtual void draw(int x, int y);
};

/** Dispatches a draw through the original ScreenElement vtable prefix. */
static inline void PlayerSelect_DrawElement(
    ScreenElement *element, int x, int y)
{
    ((PlayerSelectDrawableView *)element)->draw(x, y);
}

/** Returns an element's width through the original dispatch-table slot. */
static inline int PlayerSelect_GetElementWidth(ScreenElement *element)
{
    return ((ScreenElementDispatchTable *)element->CClass.vtable)->Width(
        element);
}

/** Virtual Sprite2 prefix used to replace the bitmap data of face sprites. */
struct PlayerSelectSpriteDataView
{
    virtual void slot0();
    virtual void slot1();
    virtual void setData(void *data);
    virtual void draw(int x, int y, int drawFirst);
};

// === PlayerSelect-owned font resources ===

/** PRE package and fonts used by the player-select screen. */
static char PlayerSelect_PlayerPreName[12] asm("s_player_pre_800dc2e8")
    __attribute__((section(".sdata"), aligned(4))) = "player.pre";
static char PSFont2[12]
    __attribute__((section(".sdata"), aligned(4))) = "s2font2.fnt";
static char PSFont1[12]
    __attribute__((section(".sdata"), aligned(4))) = "s2font1.fnt";
static char PSFont3[12]
    __attribute__((section(".sdata"), aligned(4))) = "s2bio.fnt";

// === Absolute-addressed globals ===
extern "C" char s_LOAD_CHARACTER_800dc398[];

// === Additional collaborator functions ===
#include "CREATE.h"
#include "SPOOL.h"
#include "PAL.h"
#define M3DMATHS_TYPED_API
#include "M3DMATHS.h"
#undef M3DMATHS_TYPED_API
#include "FILEIO.h"
// === Function implementations (address order) ===

/**
 * __18PlayerSelectScreeni
 *
 * Address:   0x8008AC80
 * Mangled:   __18PlayerSelectScreeni
 * Demangled: PlayerSelectScreen::PlayerSelectScreen(int)
 *            Constructor: calls base ctor, inits 3 fonts, sets vtable, calls init
 *
 * @param _this  Player-select screen storage to initialize.
 * @param gameMode  Front-end mode used to choose single- or multiplayer panes.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
extern "C" ScreenElementList *ScreenElementListBase_ctor(
    ScreenElementList *, int, int, int, int)
    asm("__17ScreenElementListiiii");
PlayerSelectScreen *PlayerSelectScreen_ctor(
    PlayerSelectScreen *_this, int gameMode)
{
    ScreenElementListBase_ctor(&_this->ScreenElementList, 0, 0, -1, -1);
    PlayerSelectFontAssignmentView *font1 = &_this->mFont1;
    _this->ScreenElementList.ScreenElement.CClass.vtable =
        __vt_18PlayerSelectScreen;
    ::new (reinterpret_cast<Font *>(font1)) Font;
    ::new (reinterpret_cast<Font *>(&_this->mFont2)) Font;
    ::new (reinterpret_cast<Font *>(&_this->mFont3)) Font;
    _this->init(gameMode);
    return _this;
}

/**
 * Builds the player-select screen's fonts, prompts, information panes, and
 * skater preview for the selected game mode.
 *
 * Address:   0x8008ACF8
 * Mangled:   init__18PlayerSelectScreeni
 * Demangled: PlayerSelectScreen::init(int)
 *
 * @param gameMode  Front-end game mode used to select single/multiplayer panes.
 */
void PlayerSelectScreen::init(int gameMode)
{
    Spooler::SpoolMgr->Reset();
    mMulti = !((unsigned int)(gameMode - 1) < 3);
    PREManager *preManager = PREMgr;
    if (preManager->inPRE(s_player_pre_800dc2e8) == 0)
    {
        printf(s_loading_pre_file_manually_800b4068);
        PREMgr->loadPRE(s_player_pre_800dc2e8);
    }

    mCASTexturesSpooled = 0;
    Create_InitSkaterFrontEnd();

    mFont1 = *(PlayerSelectFontAssignmentView *)
        FontManager::LoadFont(PSFont2);
    mFont1.JustX = 0;
    mFont1.JustY = 3;
    reinterpret_cast<Font *>(&mFont1)->setRGB(0x80, 0x80, 0x80);
    mFont1.Shadow = 0;
    mFont1.SpaceX = 0;
    mFont1.SpaceY = 0;
    mFont1.Scale = 0x1000;

    mFont2 = *(PlayerSelectFontAssignmentView *)
        FontManager::LoadFont(PSFont1);
    mFont2.JustX = 0;
    mFont2.JustY = 3;
    reinterpret_cast<Font *>(&mFont2)->setRGB(0x80, 0x80, 0x80);
    mFont2.Shadow = 0;
    mFont2.SpaceX = -1;
    mFont2.SpaceY = 0;
    mFont2.Scale = 0x1000;

    mFont3 = *(PlayerSelectFontAssignmentView *)
        FontManager::LoadFont(PSFont3);
    mFont3.JustX = 0;
    mFont3.JustY = 3;
    reinterpret_cast<Font *>(&mFont3)->setRGB(0xff, 0xff, 0xff);
    mFont3.Shadow = 0;
    mFont3.SpaceX = 1;
    mFont3.SpaceY = 0;
    mFont3.Scale = 0x1000;
    mFont3.CharMap = 2;

    mBackground = new Sprite2(
        s_s2bgd1_bmp_800dc318, 1, 0x40, 0x40);

    ScreenElement *item[16];
    {
        int i = 0;
        ScreenElement **slot = item;
        do
        {
            *slot = 0;
            slot++;
        } while (++i < 16);
    }

    item[0] = (ScreenElement *)new PromptScreenElement(
        s_s2prmpt4_bmp_800dc324, s_select_800dc334,
        (Font *)&mFont1, 0x36, 0xdc, -1, 0x104);

    item[1] = (ScreenElement *)new PromptScreenElement(
        s_s2prmpt5_bmp_800dc33c, s_scroll_bio_800dc34c,
        (Font *)&mFont1, 0xa4, 0xdc, -1, 0x104);

    item[2] = (ScreenElement *)new PromptScreenElement(
        s_s2prmpt1_bmp_800dc358, s_accept_800dc368,
        (Font *)&mFont1, 0x125, 0xdc, -1, 0x104);

    item[3] = (ScreenElement *)new PromptScreenElement(
        s_s2prmpt2_bmp_800dc370, s_back_800dc380,
        (Font *)&mFont1, 0x18b, 0xdc, -1, 0x104);

    {
        TextScreenElement *element = (TextScreenElement *)
            ::CClass::operator new(sizeof(TextScreenElement));
        if (element != 0)
        {
            element = PlayerSelect_ConstructTextElement(
                element, s_TEAM_CHICKEN_800dc388,
                (Font *)&mFont2, 0x42, 0x23, -1, -1);
        }
        item[5] = (ScreenElement *)element;
    }

    item[6] = (ScreenElement *)new StatsScreenElement(
        0x9a, 0x2d, -1, -1);

    mpMultiScreenElement = 0;
    mpBioScreenElement = 0;

    if (mMulti != 0)
    {
        mpMultiScreenElement = new MultiPlayerScreenElement(
            (Font *)&mFont2, 0x10e, 0xa3, 0x200, 0xa3);

        if ((gameMode == 5) || (gameMode == 8) || (gameMode == 9))
        {
            mMaxPlayers = 2;
        }
        else
        {
            mMaxPlayers = 8;
        }
        item[7] = (ScreenElement *)mpMultiScreenElement;
    }
    else if (gameMode != 3)
    {
        CareerScreenElement *element = new CareerScreenElement(
            (Font *)&mFont1, 0xf4, 0x85, 0x200, 0x85);
        mpCareerScreenElement = (ScreenElement *)element;
        item[7] = mpCareerScreenElement;
    }

    mpBioScreenElement = new BioScreenElement(
        (Font *)&mFont3, 0x41, 0x7f, -0x200, 0x7f);
    item[8] = (ScreenElement *)mpBioScreenElement;

    {
        int i = 0;
        ScreenElement **slot = item;
        do
        {
            if (*slot != 0)
            {
                ScreenElementList.addItem(*slot);
            }
            i++;
            slot++;
        } while (i < 16);
    }

    SkaterScreenElement *skaterElement = new SkaterScreenElement(
        gameMode == 3, (Font *)&mFont1,
        0x154, 0x10, 0x154, -0xc8);
    item[9] = (ScreenElement *)skaterElement;
    ScreenElementList.addItem(item[9]);
    mpSkaterScreenElement = (SkaterScreenElement *)item[9];

    Create_BeginSpooling();

    {
        ScreenElementDispatchTable *vtable =
            (ScreenElementDispatchTable *)
                ScreenElementList.ScreenElement.CClass.vtable;
        vtable->Refresh(this);
    }
    mpSkaterScreenElement->refreshSkater();
    PREMgr->unloadPRE(s_player_pre_800dc2e8);
}

/**
 *
 * Address:   0x8008B2EC
 * Mangled:   _._18PlayerSelectScreen
 * Demangled: PlayerSelectScreen::~PlayerSelectScreen()
 *
 * @param _this  Player-select screen storage to destroy.
 * @param flag  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
void PlayerSelectScreen_dtor(
    PlayerSelectScreen *_this, int flag)
{
    _this->ScreenElementList.ScreenElement.CClass.vtable =
        __vt_18PlayerSelectScreen;
    Sprite2 *obj = _this->mBackground;
    if (obj != 0)
    {
        Sprite2DispatchTable *vtbl =
            (Sprite2DispatchTable *)obj->vtable;
        void (*dtor)(void *, int) = vtbl->Destroy;
        dtor(obj, 3);
    }
    FontManager::UnloadFont(PSFont1);
    FontManager::UnloadFont(PSFont2);
    FontManager::UnloadFont(PSFont3);
    Create_FreeSkaterFrontEnd();
    Create_FreeSkater();
    Spool_RemoveUnusedTextures();
    Pal_RemoveUnusedPalettes();
    Pack_DefragmentFully();
    reinterpret_cast<Font *>(&_this->mFont3)->~Font();
    reinterpret_cast<Font *>(&_this->mFont2)->~Font();
    reinterpret_cast<Font *>(&_this->mFont1)->~Font();
    PlayerSelect_ListDtor(&_this->ScreenElementList, 0);
    if (flag & 1)
    {
        ::CClass::operator delete(_this);
    }
}

/**
 * Draw the player-select background and child screen elements.
 *
 * Address:   0x8008B3D4
 * Mangled:   draw__18PlayerSelectScreen
 * Demangled: PlayerSelectScreen::draw(void)
 *
 */
void PlayerSelectScreen::draw()
{
    PlayerSelect_ListDraw(&ScreenElementList, 0, 0);
    ((Sprite2DispatchTable *)mBackground->vtable)->Draw(
        mBackground, 0, 0, 1);
}

/**
 *
 * Address:   0x8008B420
 * Mangled:   update__18PlayerSelectScreen
 * Demangled: PlayerSelectScreen::update(void)
 *            Checks mCASTexturesSpooled; if 0, calls Create_SpoolTextures and AddSpoolItems;
 *            if nonzero, calls UpdateSpoolItems. Then calls base class update.
 *
 */
void PlayerSelectScreen::update()
{
    if (mCASTexturesSpooled == 0)
    {
        mCASTexturesSpooled = Create_SpoolTextures();
        if (mCASTexturesSpooled != 0)
        {
            mpSkaterScreenElement->AddSpoolItems();
        }
    }
    else
    {
        mpSkaterScreenElement->UpdateSpoolItems();
    }

    PlayerSelect_ListUpdate(&ScreenElementList);
}

/**
 * Refresh the player-select child panes after a skater change.
 *
 * Address:   0x8008B484
 * Mangled:   refresh__18PlayerSelectScreen
 * Demangled: PlayerSelectScreen::refresh(void)
 *
 */
void PlayerSelectScreen::refresh()
{
    if (mpSkaterScreenElement->mShowCard != 0)
    {
        ((TextScreenElement *)ScreenElementList.mpItem[4])
            ->setString(s_LOAD_CHARACTER_800dc398);
    }
    else
    {
        ((TextScreenElement *)ScreenElementList.mpItem[4])
            ->setString(Stats[SelectedSkater].Name);
    }

    if (mpCareerScreenElement != 0)
    {
        ((ScreenElementDispatchTable *)mpCareerScreenElement->CClass.vtable)
            ->Refresh(mpCareerScreenElement);
    }

    if (mpMultiScreenElement != 0)
    {
        ((ScreenElementDispatchTable *)
             mpMultiScreenElement->ScreenElementList.ScreenElement.CClass.vtable)
            ->Refresh(mpMultiScreenElement);
    }

    ((ScreenElementDispatchTable *)
         mpBioScreenElement->ScreenElementList.ScreenElement.CClass.vtable)
        ->Refresh(mpBioScreenElement);

    ((ScreenElementDispatchTable *)
         mpSkaterScreenElement->ScreenElementList.ScreenElement.CClass.vtable)
        ->Refresh(mpSkaterScreenElement);
}

/** Refresh the whole player-select screen through its virtual interface. */
static inline void PlayerSelect_RefreshScreen(PlayerSelectScreen *screen)
{
    ((ScreenElementDispatchTable *)
         screen->ScreenElementList.ScreenElement.CClass.vtable)
        ->Refresh(screen);
}

/**
 * Process controller input while choosing one or two skaters.
 *
 * Address:   0x8008B598
 * Mangled:   handlePress__18PlayerSelectScreen
 * Demangled: PlayerSelectScreen::handlePress(void)
 *
 * @return 1 when the selection is accepted, 2 when it is cancelled, or 0
 *         while the screen remains active.
 */
int PlayerSelectScreen::handlePress()
{
    int SELECTED_CONTROLLER = 0;

    if (mMulti != 0 && MultiPlayerListCount > 0)
    {
        SELECTED_CONTROLLER = (mMaxPlayers == 2);
    }

    if (Pad[SELECTED_CONTROLLER].Down.Pressed != 0)
    {
        mpBioScreenElement->HandleDown();
    }
    else if (Pad[SELECTED_CONTROLLER].Up.Pressed != 0)
    {
        mpBioScreenElement->HandleUp();
    }

    if (Pad[SELECTED_CONTROLLER].LeftOne.Pressed != 0)
    {
        mpSkaterScreenElement->RotateSkater(-30);
    }
    if (Pad[SELECTED_CONTROLLER].RightOne.Pressed != 0)
    {
        mpSkaterScreenElement->RotateSkater(30);
    }

    if (Pad[SELECTED_CONTROLLER].Left.Triggered != 0)
    {
        SFX_PlayX(0x90, 0x2000, 0);
        Pad[SELECTED_CONTROLLER].Left.Triggered = 0;

        if (mpSkaterScreenElement->mShowCard != 0)
        {
            mpSkaterScreenElement->mShowCard = 0;
        }
        else
        {
            --SelectedSkater;
            if (SelectedSkater < 0)
            {
                SelectedSkater = 13;
                mpSkaterScreenElement->mShowCard = 1;
            }
        }
        PlayerSelect_RefreshScreen(this);
    }
    else if (Pad[SELECTED_CONTROLLER].Right.Triggered != 0)
    {
        SFX_PlayX(0x90, 0x2000, 0);
        Pad[SELECTED_CONTROLLER].Right.Triggered = 0;

        if (SelectedSkater == 13 &&
            mpSkaterScreenElement->mShowCard == 0)
        {
            mpSkaterScreenElement->mShowCard = 1;
        }
        else
        {
            ++SelectedSkater;
            if (SelectedSkater > 13)
            {
                mpSkaterScreenElement->mShowCard =
                    SelectedSkater = 0;
            }
        }
        PlayerSelect_RefreshScreen(this);
    }
    else if (mpSkaterScreenElement->mSkaterChanged != 0)
    {
        --mpSkaterScreenElement->mSkaterChanged;
        if (mpSkaterScreenElement->mSkaterChanged == 0)
        {
            mpSkaterScreenElement->refreshSkater();
        }
    }

    if (mMulti != 0)
    {
        if (MultiPlayerListCount >= mMaxPlayers)
        {
            SelectedSkater = MultiPlayerList[0];
            SelectedSkater2 = MultiPlayerList[1];
            return 1;
        }

        if (Pad[SELECTED_CONTROLLER].X.Triggered != 0)
        {
            int available = Stats[SelectedSkater].Available;
            Pad[SELECTED_CONTROLLER].X.Triggered = 0;

            if (available == 0)
            {
                SFX_PlayX(0x92, 0x2000, 0);
                return 0;
            }
            if (SelectedSkater > 1 && SelectedSkater != 13)
            {
                SFX_PlayX(0x92, 0x2000, 0);
                return 0;
            }
            if (mpSkaterScreenElement->mShowCard != 0)
            {
                SFX_PlayX(0x92, 0x2000, 0);
                return 0;
            }

            SFX_PlayX(0x91, 0x2000, 0);
            int selectedSkater = SelectedSkater;
            SelectedSkater = 0;
            MultiPlayerList[MultiPlayerListCount] = selectedSkater;
            ++MultiPlayerListCount;
            mpMultiScreenElement->ResetStartingLocation();
            PlayerSelect_RefreshScreen(this);
        }

        if (Pad[SELECTED_CONTROLLER].Triangle.Triggered != 0)
        {
            SFX_PlayX(0x93, 0x2000, 0);
            Pad[SELECTED_CONTROLLER].Triangle.Triggered = 0;

            if (--MultiPlayerListCount < 0)
            {
                if (SelectedSkater < 2)
                {
                    return 2;
                }
                SelectedSkater = 0;
                return 2;
            }

            SelectedSkater = MultiPlayerList[MultiPlayerListCount];
            mpMultiScreenElement->ResetStartingLocation();
            PlayerSelect_RefreshScreen(this);
        }

        if (Pad[SELECTED_CONTROLLER].Select.Triggered != 0)
        {
            Pad[SELECTED_CONTROLLER].Select.Triggered = 0;
            if (MultiPlayerListCount > 1)
            {
                SFX_PlayX(0x91, 0x2000, 0);
                if (SelectedSkater < 2)
                {
                    return 1;
                }
                SelectedSkater = 0;
                return 1;
            }
            SFX_PlayX(0x92, 0x2000, 0);
        }
    }
    else
    {
        if (Pad[0].X.Triggered != 0)
        {
            Pad[0].X.Triggered = 0;

            if (Stats[SelectedSkater].Available == 0)
            {
                SFX_PlayX(0x92, 0x2000, 0);
                return 0;
            }
            if (SelectedSkater > 1 && SelectedSkater != 13)
            {
                SFX_PlayX(0x92, 0x2000, 0);
                return 0;
            }
            if (mpSkaterScreenElement->mShowCard != 0)
            {
                SFX_PlayX(0x92, 0x2000, 0);
                return 0;
            }

            SFX_PlayX(0x91, 0x2000, 0);
            if (SelectedSkater < 2)
            {
                return 1;
            }
            SelectedSkater = 0;
            return 1;
        }

        if (Pad[0].Triangle.Triggered != 0)
        {
            Pad[0].Triangle.Triggered = 0;
            SFX_PlayX(0x93, 0x2000, 0);
            if (SelectedSkater < 2)
            {
                return 2;
            }
            SelectedSkater = 0;
            return 2;
        }
    }

    Pad_ClearTriggers(&Pad[0]);
    Pad_ClearTriggers(&Pad[1]);
    return 0;
}

/**
 * Run the interactive player-select front-end.
 *
 * Address:   0x8008BBF0
 * Mangled:   PlayerSelect_Main__Fi
 * Demangled: PlayerSelect_Main(int)
 *
 * @param gameMode  Front-end mode used to choose single- or multiplayer panes.
 * @return -1 when selection is cancelled; otherwise 0.
 */
int PlayerSelect_Main(int gameMode)
{
    unsigned int oldSkyColor = Db_SkyColor;
    Db_SkyColor = 0;
    Db_UpdateSky();

    PlayerSelectScreen *pPlayerSelectScreen =
        (PlayerSelectScreen *)::CClass::operator new(sizeof(PlayerSelectScreen));
    if (pPlayerSelectScreen != 0)
        pPlayerSelectScreen =
            PlayerSelectScreen_ctor(pPlayerSelectScreen, gameMode);

    Pad_ClearTriggers(&Pad[0]);
    Pad_ClearTriggers(&Pad[1]);

    int press;
    while (1)
    {
        FrontEnd2_UpdateMusicLoop();
        press = pPlayerSelectScreen->handlePress();
        if ((unsigned int)(press - 1) < 2)
            break;

        GStartTime = Vblanks;
        Db_FlipClear();
        Pad_Update();
        ((ScreenElementDispatchTable *)
             pPlayerSelectScreen->ScreenElementList.ScreenElement.CClass.vtable)
            ->Update(pPlayerSelectScreen);
        pPlayerSelectScreen->draw();
        DrawSync(0);
        Pause(1);
        while ((unsigned int)Vblanks <
               (unsigned int)GStartTime + 2)
            ;
        Db_DisplayDraw();
        PollHost();
    }

    Spooler::SpoolMgr->Reset();
    Db_SkyColor = oldSkyColor;
    if (pPlayerSelectScreen != 0)
    {
        ((ScreenElementDispatchTable *)
             pPlayerSelectScreen->ScreenElementList.ScreenElement.CClass.vtable)
            ->Destroy(pPlayerSelectScreen, 3);
    }

    if (press == 2)
        return -1;
    return 0;
}

/**
 * Build a prompt row from a sprite and text label.
 *
 * Address: 0x8008BD78
 * Mangled: __19PromptScreenElementPcT1P4Fontiiii
 * Match:   PERFECT (85/85)
 *
 * @param pFileName  FileName to operate on.
 * @param pString  Input string.
 * @param pFont  Font used for rendering.
 * @param homeX  Home X coordinate (resting position).
 * @param homeY  Home Y coordinate (resting position).
 * @param screenX  Screen-space X coordinate.
 * @param screenY  Screen-space Y coordinate.
 */
PromptScreenElement::PromptScreenElement(char *pFileName, char *pString, Font *pFont, int homeX, int homeY, int screenX, int screenY)
{
    ScreenElement *item[2];
    ScreenElement *element;
    int width;

    ScreenElementListBase_ctor(
        &ScreenElementList, homeX, homeY, screenX, screenY);
    ScreenElementList.ScreenElement.CClass.vtable =
        __vt_19PromptScreenElement;

    element = (ScreenElement *)::CClass::operator new(
        sizeof(SpriteScreenElement));
    if (element != 0)
    {
        element = (ScreenElement *)PlayerSelect_ConstructSpriteElement(
            (SpriteScreenElement *)element,
            pFileName, 0, 0, -1, -1, 1, 0);
    }
    item[0] = element;
    ScreenElementList.addItem(item[0]);

    element = (ScreenElement *)::CClass::operator new(
        sizeof(TextScreenElement));
    if (element != 0)
    {
        element = (ScreenElement *)PlayerSelect_ConstructTextElement(
            (TextScreenElement *)element,
            pString, pFont, 0, 0, -1, -1);
    }
    item[1] = element;
    ScreenElementList.addItem(item[1]);

    width = PlayerSelect_GetElementWidth(item[0]);
    item[1]->mHomeX = item[0]->mHomeX + width + 4;
    item[1]->mHomeY += 6;
    item[1]->SnapToHomeX();
}

/**
 * Destroy the prompt row and its child elements.
 *
 * Address:   0x8008BECC
 * Mangled:   _._19PromptScreenElement
 * Demangled: PromptScreenElement::~PromptScreenElement()
 *
 */
PromptScreenElement::~PromptScreenElement()
{
    ScreenElementList.ScreenElement.CClass.vtable =
        __vt_19PromptScreenElement;
    PlayerSelect_ListDtor(&ScreenElementList, 0);

}

/**
 * width__19PromptScreenElement
 *
 * Address:   0x8008BF20
 * Mangled:   width__19PromptScreenElement
 * Demangled: PromptScreenElement::width(void)
 *            Returns combined width of two sub-elements plus 4 pixel gap
 *
 * @return w1 + w2 + 4
 */
int PromptScreenElement::width()
{
    ScreenElement *pTextElement = ScreenElementList.mpItem[1];
    ScreenElement *pSpriteElement = ScreenElementList.mpItem[0];
    return ((ScreenElementDispatchTable *)pTextElement->CClass.vtable)
               ->Width(pTextElement) +
           ((ScreenElementDispatchTable *)pSpriteElement->CClass.vtable)
               ->Width(pSpriteElement) +
           4;
}

/**
 * Build an animated numeric score row.
 *
 * Address:   0x8008BF88
 * Mangled:   __18ScoreScreenElementiP4Fontii
 * Demangled: ScoreScreenElement::(int, Font *, int, int)
 *
 * @param value  Input value.
 * @param pFont  Font used for rendering.
 * @param x  X coordinate.
 * @param y  Y coordinate.
 */
ScoreScreenElement::ScoreScreenElement(int value, Font *pFont, int x, int y)
{
    char newString[32];
    TextScreenElement *text;

    ScreenElementListBase_ctor(&ScreenElementList, x, y, -1, -1);
    ScreenElementList.ScreenElement.CClass.vtable =
        __vt_18ScoreScreenElement;
    mValue = 0;
    mTargetValue = value;
    (mpFont = pFont, sprintf(newString, s__7d_800dc3a8, 0));

    text = (TextScreenElement *)
        ::CClass::operator new(sizeof(TextScreenElement));
    if (text != 0)
    {
        text = PlayerSelect_ConstructTextElement(
            text,
            newString, pFont, 0, 0, -1, -1);
    }
    ScreenElementList.addItem((ScreenElement *)text);
    mpScoreElement = text;
}

/**
 * Destroy the score row and its text element.
 *
 * Address:   0x8008C054
 * Mangled:   _._18ScoreScreenElement
 * Demangled: ScoreScreenElement::~ScoreScreenElement()
 *
 */
ScoreScreenElement::~ScoreScreenElement()
{
    ScreenElementList.ScreenElement.CClass.vtable =
        __vt_18ScoreScreenElement;
    PlayerSelect_ListDtor(&ScreenElementList, 0);

}

/**
 * setValue__18ScoreScreenElementi — Clamp value to 9,999,999 and store at this+0x68
 *
 * Address:   0x8008C0A8, Size: 36 bytes (9 instructions)
 * Mangled:   setValue__18ScoreScreenElementi
 * Demangled: ScoreScreenElement::setValue(int)
 *            Assembly:
 *            lui $v0, 0x98; ori $v0, 0x967f; slt $v0, $v0, $a1
 *            beqz $v0, .store; nop
 *            lui $a1, 0x98; ori $a1, 0x967f
 *            .store: jr $ra; sw $a1, 0x68($a0)
 *
 * @param value  Input value.
 */
void ScoreScreenElement::setValue(int value)
{
    if (value > 9999999)
    {
        value = 9999999;
    }
    mTargetValue = value;
}

/**
 * Draw the score with fixed-width digits.
 *
 * Address:   0x8008C0CC
 * Mangled:   draw__18ScoreScreenElementii
 * Demangled: ScoreScreenElement::draw(int, int)
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 */
void ScoreScreenElement::draw(int x, int y)
{
    int oldFixedWidth = mpFont->FixedWidth;
    mpFont->FixedWidth = 1;
    PlayerSelect_ListDraw(&ScreenElementList, x, y);
    mpFont->FixedWidth = oldFixedWidth;
}

/**
 *
 * Address:   0x8008C110
 * Mangled:   update__18ScoreScreenElement
 * Demangled: ScoreScreenElement::update(void)
 *            Interpolates displayed score towards target score, formats as string, updates text element
 *
 */
void ScoreScreenElement::update()
{
    char newString[32];

    {
        int target = mTargetValue;
        int diff = target - mValue;
        int current = mValue;
        int step = diff >> 1;
        current = current + step;
        mValue = current;
        if ((u32)(diff + 1) < 3)
        {
            mValue = target;
        }
    }

    sprintf(newString, s__7d_800dc3a8, mValue);

    mpScoreElement->setString(newString);
    PlayerSelect_ElementUpdate(&ScreenElementList.ScreenElement);
}

/**
 * __19CareerScreenElementP4Fontiiii - Build the career summary UI: two score
 * readouts, the backing sprite, three stored medal icons, then refresh values
 * and medal reveal delays from the selected skater's career progress.
 *
 * Address:   0x8008C184
 * Mangled:   __19CareerScreenElementP4Fontiiii
 * Demangled: CareerScreenElement::(Font *, int, int, int, int)
 *
 * @param pFont  Font used for rendering.
 * @param homeX  Home X coordinate (resting position).
 * @param homeY  Home Y coordinate (resting position).
 * @param screenX  Screen-space X coordinate.
 * @param screenY  Screen-space Y coordinate.
 */
CareerScreenElement::CareerScreenElement(
    Font *pFont, int homeX, int homeY, int screenX, int screenY)
{
    CareerScreenElement *self = this;
    ScreenElement *item[3];
    int i;
    ScreenElement **ptr;
    ScoreScreenElement *obj;

    ScreenElementListBase_ctor(
        &self->ScreenElementList, homeX, homeY, screenX, screenY);
    self->ScreenElementList.ScreenElement.CClass.vtable =
        __vt_19CareerScreenElement;

    obj = new ScoreScreenElement(0xC350, pFont, 0xAA, 0x0C);
    item[0] = (ScreenElement *)obj;

    obj = new ScoreScreenElement(0x14, pFont, 0xAA, 0x1A);
    item[1] = (ScreenElement *)obj;

    {
        SpriteScreenElement *spriteObj = (SpriteScreenElement *)
            ::CClass::operator new(sizeof(SpriteScreenElement));
        if (spriteObj != 0)
        {
            spriteObj = PlayerSelect_ConstructSpriteElement(
                spriteObj,
                s__spart__800daa1c + 0x1994, 0, 0, -1, -1, 0, 0);
        }
        item[2] = (ScreenElement *)spriteObj;
    }

    i = 0;
    ptr = item;
    do
    {
        self->ScreenElementList.addItem(*ptr);
        ptr++;
        i++;
    } while (i < 3);

    {
        SpriteScreenElement *spriteObj = (SpriteScreenElement *)
            ::CClass::operator new(sizeof(SpriteScreenElement));
        if (spriteObj != 0)
        {
            spriteObj = PlayerSelect_ConstructSpriteElement(
                spriteObj,
                s__spart__800daa1c + 0x19A0,
                0x2E, 0x2E, -1, -1, 0, 0);
        }
        self->mpMedalIcons[0] = (ScreenElement *)spriteObj;
    }

    {
        SpriteScreenElement *spriteObj = (SpriteScreenElement *)
            ::CClass::operator new(sizeof(SpriteScreenElement));
        if (spriteObj != 0)
        {
            spriteObj = PlayerSelect_ConstructSpriteElement(
                spriteObj,
                s__spart__800daa1c + 0x19B0,
                0x2E, 0x2E, -1, -1, 0, 0);
        }
        self->mpMedalIcons[1] = (ScreenElement *)spriteObj;
    }

    {
        SpriteScreenElement *spriteObj = (SpriteScreenElement *)
            ::CClass::operator new(sizeof(SpriteScreenElement));
        if (spriteObj != 0)
        {
            spriteObj = PlayerSelect_ConstructSpriteElement(
                spriteObj,
                s__spart__800daa1c + 0x19C0,
                0x2E, 0x2E, -1, -1, 0, 0);
        }
        self->mpMedalIcons[2] = (ScreenElement *)spriteObj;
    }

    self->refresh();
}

/**
 *
 * Address:   0x8008C360
 * Mangled:   _._19CareerScreenElement
 * Demangled: CareerScreenElement::~CareerScreenElement()
 *
 */
CareerScreenElement::~CareerScreenElement()
{
    ScreenElementList.ScreenElement.CClass.vtable =
        __vt_19CareerScreenElement;
    ScreenElement *obj = mpMedalIcons[0];
    if (obj != 0)
    {
        ScreenElementDispatchTable *vtbl =
            (ScreenElementDispatchTable *)obj->CClass.vtable;
        void (*dtor)(void *, int) = vtbl->Destroy;
        dtor(obj, 3);
    }
    obj = mpMedalIcons[1];
    if (obj != 0)
    {
        ScreenElementDispatchTable *vtbl =
            (ScreenElementDispatchTable *)obj->CClass.vtable;
        void (*dtor)(void *, int) = vtbl->Destroy;
        dtor(obj, 3);
    }
    obj = mpMedalIcons[2];
    if (obj != 0)
    {
        ScreenElementDispatchTable *vtbl =
            (ScreenElementDispatchTable *)obj->CClass.vtable;
        void (*dtor)(void *, int) = vtbl->Destroy;
        dtor(obj, 3);
    }
    PlayerSelect_ListDtor(&ScreenElementList, 0);

}

/**
 * Draw the career totals and earned medals.
 *
 * Address:   0x8008C428
 * Mangled:   draw__19CareerScreenElementii
 * Demangled: CareerScreenElement::draw(int, int)
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 */
void CareerScreenElement::draw(int x, int y)
{
    int i = 0;
    unsigned char *medals = (unsigned char *)
        &((SPlayerSelectCareerOverlay *)&GCareer)
            ->CharacterProgress[0].Medals[0];
    int itemOffset = 0;
    CareerScreenElement *t = this;
    int *delayp = t->mDelay;

    do
    {
        int delay = *delayp;
        if (delay > 0)
        {
            *delayp = delay - 1;
        }
        else
        {
            int selectedSkater = SelectedSkater;
            int space = (i / 3) * 7;
            int medalIndex = i;
            medalIndex += selectedSkater * sizeof(SCharacterProgress);
            char medal = *(unsigned char *)(medalIndex + (unsigned int)medals);

            switch (medal)
            {
                case 7:
                    ((PlayerSelectDrawableView *)t->mpMedalIcons[2])->draw(
                        t->ScreenElementList.ScreenElement.mScreenX +
                            x + space + itemOffset,
                        t->ScreenElementList.ScreenElement.mScreenY + y);
                    break;
                case 6:
                    ((PlayerSelectDrawableView *)t->mpMedalIcons[1])->draw(
                        t->ScreenElementList.ScreenElement.mScreenX +
                            x + space + itemOffset,
                        t->ScreenElementList.ScreenElement.mScreenY + y);
                    break;
                case 5:
                    ((PlayerSelectDrawableView *)t->mpMedalIcons[0])->draw(
                        t->ScreenElementList.ScreenElement.mScreenX +
                            x + space + itemOffset,
                        t->ScreenElementList.ScreenElement.mScreenY + y);
                    break;
            }
        }
        itemOffset = itemOffset + 0x14;
        i = i + 1;
        delayp = delayp + 1;
    } while (i < 9);

    itemOffset = 0;
    if (t->ScreenElementList.mCount > 0)
    {
        ScreenElement **items = t->ScreenElementList.mpItem;
        do
        {
            ScreenElement *obj = *items;
            items = items + 1;
            PlayerSelect_DrawElement(
                obj,
                t->ScreenElementList.ScreenElement.mScreenX + x,
                t->ScreenElementList.ScreenElement.mScreenY + y);
            itemOffset = itemOffset + 1;
        } while (itemOffset < t->ScreenElementList.mCount);
    }
}

/**
 *
 * Address:   0x8008C5C4
 * Mangled:   update__19CareerScreenElement
 * Demangled: CareerScreenElement::update(void)
 *            Calls base class ScreenElementList::update(this)
 *
 */
void CareerScreenElement::update()
{
    PlayerSelect_ListUpdate(&ScreenElementList);
}

/**
 *
 * Address:   0x8008C5E4
 * Mangled:   refresh__19CareerScreenElement
 * Demangled: CareerScreenElement::refresh(void)
 *
 *            Refresh the selected skater's career totals and medal reveal delays.
 *
 */
void CareerScreenElement::refresh()
{
    SPlayerSelectCareerOverlay *career =
        (SPlayerSelectCareerOverlay *)&GCareer;
    int delay;

    ((ScoreScreenElement *)ScreenElementList.mpItem[0])->setValue(
        career->CharacterProgress[SelectedSkater].CareerEarnings);
    ((ScoreScreenElement *)ScreenElementList.mpItem[1])->setValue(
        career->CharacterProgress[SelectedSkater].Cash);
    int *items = mDelay;
    delay = 0;
    int i = 0;
    int eight = 8;
    char *medalBase = career->CharacterProgress[0].Medals;
    char *medals = medalBase +
                   SelectedSkater * sizeof(SCharacterProgress);
    for (; i < 9; i++)
    {
        if (*medals != eight)
        {
            *items = delay;
            delay += 5;
        }
        else
        {
            *items = 0;
        }
        items++;
        medals++;
    }
}

/**
 * Build the selected skater's scrollable biography pane.
 *
 * Address:   0x8008C6A8
 * Mangled:   __16BioScreenElementP4Fontiiii
 * Demangled: BioScreenElement::(Font *, int, int, int, int)
 *
 * @param pFont  Font used for rendering.
 * @param homeX  Home X coordinate (resting position).
 * @param homeY  Home Y coordinate (resting position).
 * @param screenX  Screen-space X coordinate.
 * @param screenY  Screen-space Y coordinate.
 */
BioScreenElement::BioScreenElement(Font *pFont, int homeX, int homeY, int screenX, int screenY)
{
    SpriteScreenElement *obj;
    int i;
    ScreenElement **ptr;

    ScreenElementListBase_ctor(
        &ScreenElementList, homeX, homeY, screenX, screenY);
    ScreenElementList.ScreenElement.CClass.vtable =
        __vt_16BioScreenElement;
    mpFont = pFont;

    obj = (SpriteScreenElement *)
        ::CClass::operator new(sizeof(SpriteScreenElement));
    if (obj != 0)
    {
        obj = PlayerSelect_ConstructSpriteElement(
            obj,
            s__spart__800daa1c + 0x19D0, 2, 0x10, -1, -1, 0, 0);
    }
    ScreenElementList.addItem((ScreenElement *)obj);

    obj = (SpriteScreenElement *)
        ::CClass::operator new(sizeof(SpriteScreenElement));
    if (obj != 0)
    {
        obj = PlayerSelect_ConstructSpriteElement(
            obj,
            s__spart__800daa1c + 0x19E0, 0, 0, -1, -1, 0, 0);
    }
    ScreenElementList.addItem((ScreenElement *)obj);

    mSize = 0;
    mBioTextCount = 0;
    i = 0;
    ptr = mpBioText;
    do
    {
        *ptr = 0;
        i++;
        ptr++;
    } while (i < 16);

    this->refresh();
}

/**
 *
 * Address:   0x8008C7B4
 * Mangled:   _._16BioScreenElement
 * Demangled: BioScreenElement::~BioScreenElement()
 *
 */
BioScreenElement::~BioScreenElement()
{
    ScreenElementList.ScreenElement.CClass.vtable =
        __vt_16BioScreenElement;
    int i = 0;
    ScreenElement **ptr = mpBioText;
    do
    {
        ScreenElement *obj = *ptr;
        if (obj != 0)
        {
            ScreenElementDispatchTable *vtbl =
                (ScreenElementDispatchTable *)obj->CClass.vtable;
            void (*dtor)(void *, int) = vtbl->Destroy;
            dtor(obj, 3);
        }
        *ptr = 0;
        i++;
        ptr++;
    } while (i < 16);
    PlayerSelect_ListDtor(&ScreenElementList, 0);

}

/**
 *
 * Address:   0x8008C860
 * Mangled:   draw__16BioScreenElementii
 * Demangled: BioScreenElement::draw(int, int)
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 */
void BioScreenElement::draw(int x, int y)
{
    BioScreenElement *t = this;
    int drawX = x;
    int drawY = y;
    int i;
    ScreenElement **entry;

    SetClippingBox2(0, 0, 0x200, 0xF0);

    if (t->mBioTextCount > 0)
    {
        i = 0;
        entry = t->mpBioText;
        do
        {
            ScreenElement *item;

            item = *entry;
            entry++;
            PlayerSelect_DrawElement(
                item,
                t->ScreenElementList.ScreenElement.mScreenX + drawX,
                t->ScreenElementList.ScreenElement.mScreenY + drawY);
            i++;
        } while (i < t->mBioTextCount);
    }

    SetClippingBox2(
        t->ScreenElementList.ScreenElement.mScreenX + 0x17,
        t->ScreenElementList.ScreenElement.mScreenY + 0x0A, 0xA5, 0x43);
    PlayerSelect_ListDraw(&t->ScreenElementList, drawX, drawY);
}

/**
 *
 * Address:   0x8008C93C
 * Mangled:   update__16BioScreenElement
 * Demangled: BioScreenElement::update(void)
 *            Computes scroll position, iterates elements calling virtual update, then base update
 *
 */
void BioScreenElement::update()
{
    ScreenElement *elem0;
    int counter;
    int val;
    int i;

    elem0 = mpBioText[0];
    counter = elem0->mHomeY;
    val = 20 - counter;
    ScreenElementList.mpItem[0]->mHomeY =
        ((val * 2 + val) * 16 + val) / (mSize + 5) + 16;

    i = mBioTextCount;
    if (i > 0)
    {
        ScreenElement **elemPtr;
        int j;
        j = 0;
        elemPtr = mpBioText;
        do
        {
            ScreenElement *elem = *elemPtr;
            ScreenElementDispatchTable *vtable =
                (ScreenElementDispatchTable *)elem->CClass.vtable;
            void (*update_fn)(void *) = vtable->Update;
            update_fn(elem);
            elemPtr++;
            j++;
        } while (j < mBioTextCount);
    }

    PlayerSelect_ListUpdate(&ScreenElementList);
}

/**
 * Rebuild the selected skater's biography text.
 *
 * Address:   0x8008C9FC
 * Mangled:   refresh__16BioScreenElement
 * Demangled: BioScreenElement::refresh(void)
 *            Clear existing bio text, rebuild the visible entries for the selected skater,
 *            and toggle the list visibility based on the resulting text height.
 *
 */
void BioScreenElement::refresh()
{
    int i = 0;
    ScreenElement **bioText = mpBioText;
    ScreenElement **entry = bioText;

    do
    {
        ScreenElement *item = *entry;
        if (item != 0)
        {
            ((ScreenElementDispatchTable *)item->CClass.vtable)->Destroy(
                item, 3);
        }
        *entry = 0;
        i++;
        entry++;
    } while (i < 16);

    char **foo = Stats[SelectedSkater].BioText;
    mBioTextCount = 0;
    int currY = 20;
    if (*foo != 0)
    {
        do
        {
            if (**foo != '\0')
            {
                ScreenElement *item =
                    (ScreenElement *)::CClass::operator new(
                        sizeof(TextScreenElement));
                if (item != 0)
                {
                    item = (ScreenElement *)PlayerSelect_ConstructTextElement(
                        (TextScreenElement *)item,
                        *foo,
                        mpFont,
                        0x17,
                        currY,
                        -1,
                        -1);
                }
                ScreenElement **slot = bioText;
                *(slot += mBioTextCount++) = item;
            }
            foo++;
            currY += 10;
        } while (*foo != 0);
    }

    mSize = currY - 0x57;
    if (mSize > 0)
    {
        ScreenElementList.mpItem[0]->mHide = 0;
    }
    else
    {
        ScreenElementList.mpItem[0]->mHide = 1;
    }
}

/**
 * HandleDown__16BioScreenElement
 *
 * Address:   0x8008CB68
 * Mangled:   HandleDown__16BioScreenElement
 * Demangled: BioScreenElement::HandleDown(void)
 *            Scrolls bio elements down if last element is at or past bottom
 *
 */
void BioScreenElement::HandleDown()
{
    int i;

    if ((0x48 < mpBioText[mBioTextCount - 1]->mHomeY) &&
        (i = 0, 0 < mBioTextCount))
    {
        do
        {
            mpBioText[i]->mHomeY = mpBioText[i]->mHomeY - 1;
            i = i + 1;
        } while (i < mBioTextCount);
    }
}

/**
 * HandleUp__16BioScreenElement
 *
 * Address:   0x8008CBD8
 * Mangled:   HandleUp__16BioScreenElement
 * Demangled: BioScreenElement::HandleUp(void)
 *            Scrolls bio elements up if first element's y < 0x14
 *
 */
void BioScreenElement::HandleUp()
{
    if (mpBioText[0]->mHomeY < 0x14)
    {
        if (mBioTextCount > 0)
        {
            int i = 0;
            do
            {
                mpBioText[i]->mHomeY = mpBioText[i]->mHomeY + 1;
                i++;
            } while (i < mBioTextCount);
        }
    }
}

/**
 *
 * Address:   0x8008CC40
 * Mangled:   __19SkaterScreenElementiP4Fontiiii
 * Demangled: SkaterScreenElement::(int, Font *, int, int, int, int)
 *            Build the skater-select preview UI and seed its skater/model state.
 *
 * @param fullWindow  Nonzero to use the full-window preview layout.
 * @param pFont  Font used for rendering.
 * @param homeX  Home X coordinate (resting position).
 * @param homeY  Home Y coordinate (resting position).
 * @param screenX  Screen-space X coordinate.
 * @param screenY  Screen-space Y coordinate.
 */
SkaterScreenElement::SkaterScreenElement(
    int fullWindow, Font *pFont, int homeX, int homeY, int screenX, int screenY)
{
    ScreenElement *item[4];
    Item *pItemList;
    Sprite2 **faceSprite;
    unsigned char *bmpBuf;

    ScreenElementListBase_ctor(
        &ScreenElementList, homeX, homeY, screenX, screenY);
    ScreenElementList.ScreenElement.CClass.vtable =
        __vt_19SkaterScreenElement;
    mpFont = pFont;
    mFullWindow = fullWindow;
    if (fullWindow == 0)
    {
        {
            SpriteScreenElement *element = (SpriteScreenElement *)
                ::CClass::operator new(sizeof(SpriteScreenElement));
            if (element != 0)
            {
                element = PlayerSelect_ConstructSpriteElement(
                    element,
                    s__spart__800daa1c + 0x19EC,
                    -0x44, 0x28, -1, -1, 0, 0);
            }
            item[0] = (ScreenElement *)element;
        }
        ScreenElementList.addItem(item[0]);
        {
            SpriteScreenElement *element = (SpriteScreenElement *)
                ::CClass::operator new(sizeof(SpriteScreenElement));
            if (element != 0)
            {
                element = PlayerSelect_ConstructSpriteElement(
                    element,
                    s__spart__800daa1c + 0x19FC,
                    0x96, 0x28, -1, -1, 0, 0);
            }
            item[1] = (ScreenElement *)element;
        }
        ScreenElementList.addItem(item[1]);
    }
    else
    {
        {
            SpriteScreenElement *element = (SpriteScreenElement *)
                ::CClass::operator new(sizeof(SpriteScreenElement));
            if (element != 0)
            {
                element = PlayerSelect_ConstructSpriteElement(
                    element,
                    s__spart__800daa1c + 0x19EC,
                    -0x44, 0x28, -1, -1, 0, 0);
            }
            item[0] = (ScreenElement *)element;
        }
        ScreenElementList.addItem(item[0]);
        {
            SpriteScreenElement *element = (SpriteScreenElement *)
                ::CClass::operator new(sizeof(SpriteScreenElement));
            if (element != 0)
            {
                element = PlayerSelect_ConstructSpriteElement(
                    element,
                    s__spart__800daa1c + 0x19FC,
                    0x50, 0x28, -1, -1, 0, 0);
            }
            item[1] = (ScreenElement *)element;
        }
        ScreenElementList.addItem(item[1]);
    }
    mpSkaterWindow = 0;
    if (mFullWindow == 0)
    {
        {
            SpriteScreenElement *element = (SpriteScreenElement *)
                ::CClass::operator new(sizeof(SpriteScreenElement));
            if (element != 0)
            {
                element = PlayerSelect_ConstructSpriteElement(
                    element,
                    s__spart__800daa1c + 0x1A0C,
                    0, 0, -1, -1, 0, 0);
            }
            item[3] = (ScreenElement *)element;
        }
        ScreenElementList.addItem(item[3]);
        item[3]->mDrawFirst = 1;
        mpSkaterWindow = item[3];
    }

    {
        int i = 0;
        int *skaterLoaded = mSkaterLoaded;
        int *modelPartCount = &mModelHierInfo[0].mPartCount;
        char **modelHierData = mpModelHierData;
        mFlash = 0;
        mpAnimHierData = 0;
        mAnimHierInfo.mPartCount = 0;
        do
        {
            *modelHierData = 0;
            *modelPartCount = 0;
            *skaterLoaded = 0;
            ++skaterLoaded;
            modelPartCount = (int *)((char *)modelPartCount +
                                      sizeof(hierarchyInfo));
            ++modelHierData;
            ++i;
        } while (i < 14);
    }

    LoadAllSkaters();
    mCardRegion = Spool_PSX(
        (char *)(s__spart__800daa1c + 0x1A18), 0, 0, 1);
    Spool_Sync();
    mShowCard = 0;

    pItemList = &mCardItem;
    pItemList->Flags = 0;
    pItemList->Inquiry = 0;
    if (mFullWindow != 0)
    {
        pItemList->Position.vx = 0x73000;
        pItemList->Position.vy = -0x2C000;
        pItemList->Position.vz = 0x24000;
    }
    else
    {
        pItemList->Position.vx = 0xE8000;
        pItemList->Position.vy = -0x4A000;
        pItemList->Position.vz = 0xA8000;
    }
    pItemList->Angles.vx = 0;
    pItemList->Angles.vy = 0;
    pItemList->Angles.vz = 0;
    pItemList->Frame = 0;
    pItemList->TintIndex = 0;
    pItemList->Anim = 0;
    pItemList->Model = 0;
    pItemList->Region = *(unsigned char *)&mCardRegion;
    pItemList->RGB = 0xFF00;
    pItemList->NextItem = 0;

    {
        MATRIX *cameraMatrix = &MikeCamera[0].Transform;
        M3dMaths_SetIdentityRotation(cameraMatrix);
        VECTOR *cameraPosition = &MikeCamera[0].Position;
        SCamera *camera = &MikeCamera[0];
        camera->Position.vx = 0;
        camera->Position.vy = 0;
        camera->Position.vz = -300;
        TransMatrix(cameraMatrix, cameraPosition);
        M3dInit_SetFoggingParams(10, 6000, 0x800);
        M3dInit_InitAtStart();

        unsigned int size = Mem_GetBlockSize(mModelFile[0]);
        PSXRegion[mModelRegionGeneric].pPSX =
            (unsigned int *)Mem_New(size, 0, 1);

        int copySize = Mem_GetBlockSize(mModelFile[0]);
        int i = 0;
        Mem_Copy(
            PSXRegion[mModelRegionGeneric].pPSX,
            mModelFile[0], copySize);
        ProcessNewPSX(mModelRegionGeneric);
        mSkaterLoaded[0] = 1;
        LoadAllIcons();

        unsigned int fileSize =
            FileIO_Open(s__spart__800daa1c + 0x1A24);
        bmpBuf = (unsigned char *)Mem_New(fileSize, 0, 1);
        FileIO_Load(bmpBuf);
        FileIO_Sync();
        faceSprite = mpFaceSprite;
        while (i < 14)
        {
            *faceSprite = new Sprite2(bmpBuf, 1, 0, 0);
            ++i;
            ++faceSprite;
        }
        Mem_Delete(bmpBuf);
    }
}

/**
 * _._19SkaterScreenElement — Release skater-select preview UI assets, clear the
 * PSX regions used by loaded skater/card data, and destroy front-end skater actors.
 *
 * Address:   0x8008D03C
 * Mangled:   _._19SkaterScreenElement
 * Demangled: SkaterScreenElement::~SkaterScreenElement()
 *
 */
SkaterScreenElement::~SkaterScreenElement()
{
    SkaterScreenElement *_this = this;
    _this->ScreenElementList.ScreenElement.CClass.vtable =
        __vt_19SkaterScreenElement;

    int i = 0;
    void **ptr = (void **)_this->mpIcon;
    do
    {
        ScreenElement *obj = (ScreenElement *)*ptr;
        if (obj != 0)
        {
            ScreenElementDispatchTable *vtbl =
                (ScreenElementDispatchTable *)obj->CClass.vtable;
            void (*dtor)(void *, int) = vtbl->Destroy;
            dtor(obj, 3);
        }
        i++;
        ptr++;
    } while (i <= 0);

    i = 0;
    ptr = (void **)_this->mpIconSprite;
    do
    {
        Sprite2 *obj = (Sprite2 *)*ptr;
        if (obj != 0)
        {
            Sprite2DispatchTable *vtbl =
                (Sprite2DispatchTable *)obj->vtable;
            void (*dtor)(void *, int) = vtbl->Destroy;
            dtor(obj, 3);
        }
        i++;
        ptr++;
    } while (i < 8);

    i = 0;
    ptr = (void **)_this->mpFaceSprite;
    do
    {
        Sprite2 *obj = (Sprite2 *)*ptr;
        if (obj != 0)
        {
            Sprite2DispatchTable *vtbl =
                (Sprite2DispatchTable *)obj->vtable;
            void (*dtor)(void *, int) = vtbl->Destroy;
            dtor(obj, 3);
        }
        i++;
        ptr++;
    } while (i < 14);

    ClearRegion(_this->mModelRegionGeneric, 1, 0);
    PSXRegion[_this->mModelRegionGeneric].Filename[0] = 0;
    Mem_Delete(_this->mpAnimHierData);

    {
        int i2 = 0;
        char **modelHierData = _this->mpModelHierData;
        unsigned int **modelFile = _this->mModelFile;
        CSuper **skater = _this->mpFrontEndSkater;
        do
        {
            CSuper *obj = *skater;
            if (obj != 0)
            {
                ScreenElementDispatchTable *vtbl =
                    (ScreenElementDispatchTable *)
                        obj->CBody.CItem.vtable;
                void (*dtor)(void *, int) = vtbl->Destroy;
                dtor(obj, 3);
            }

            if (*modelFile != 0)
            {
                Mem_Delete(*modelFile);
            }

            if (*modelHierData != 0)
            {
                Mem_Delete(*modelHierData);
            }

            modelHierData++;
            modelFile++;
            i2++;
            skater++;
        } while (i2 < 14);
    }

    ClearRegion(_this->mAnimRegion, 1, 0);
    PSXRegion[_this->mAnimRegion].Filename[0] = 0;
    ClearRegion(_this->mCardRegion, 1, 0);
    PSXRegion[_this->mCardRegion].Filename[0] = 0;
    PlayerSelect_ListDtor(&_this->ScreenElementList, 0);

}

/**
 * RotateSkater__19SkaterScreenElementi — Add rotation delta to skater's rotation field
 *
 * Address:   0x8008D26C, Size: 44 bytes (11 instructions)
 * Mangled:   RotateSkater__19SkaterScreenElementi
 * Demangled: SkaterScreenElement::RotateSkater(int)
 *            Assembly: loads skater index from this+0x1FC, computes body ptr at this+idx*4+0x70,
 *            adds delta to body->0x12 (u16 rotation)
 *
 * @param angle  Signed yaw delta applied to the visible skater.
 */
void SkaterScreenElement::RotateSkater(int angle)
{
    ((CSuperPosePrefix *)mpFrontEndSkater[mVisibleSkater])->mAngles.vy += angle;
}

/**
 * Build the loading label and reset the icon slots.
 *
 * Address:   0x8008D298
 * Mangled:   LoadAllIcons__19SkaterScreenElement
 * Demangled: SkaterScreenElement::LoadAllIcons(void)
 *
 */
void SkaterScreenElement::LoadAllIcons()
{
    SkaterScreenElement *self = this;
    TextScreenElement *loadingText;
    int i;

    if (self->mFullWindow)
    {
        loadingText = (TextScreenElement *)
            ::CClass::operator new(sizeof(TextScreenElement));
        if (loadingText == 0)
        {
            goto LAB_8008D310;
        }
        loadingText = PlayerSelect_ConstructTextElement(
            loadingText,
            s_Loading____800dc4a4,
            self->mpFont,
            -0xF,
            0x38,
            -1,
            -1);
    }
    else
    {
        loadingText = (TextScreenElement *)
            ::CClass::operator new(sizeof(TextScreenElement));
        if (loadingText == 0)
        {
            goto LAB_8008D310;
        }
        loadingText = PlayerSelect_ConstructTextElement(
            loadingText,
            s_Loading____800dc4a4,
            self->mpFont,
            0x24,
            0x38,
            -1,
            -1);
    }

LAB_8008D310:
    self->mpIcon[0] = (ScreenElement *)loadingText;
    i = 0;
    int one = 1;
    int *info_ptr = &mIconInfo[0].mHide;
    Sprite2 **icon_slot = self->mpIconSprite;
    do
    {
        *icon_slot = 0;
        *info_ptr = one;
        info_ptr = (int *)((char *)info_ptr + sizeof(ICON_INFO));
        icon_slot++;
        i++;
    }
    while (i < 8);
}

/**
 *
 * Address:   0x8008D354
 * Mangled:   SetClippingBox3__Fiiii
 * Demangled: SetClippingBox3(int, int, int, int)
 *            Emit a draw-area primitive for the preview viewport and link it into the OT.
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param w  Width.
 * @param h  Height.
 */
void SetClippingBox3(int x, int y, int w, int h)
{
    DR_AREA *pClip = reinterpret_cast<DR_AREA *>(pPoly);
    RECT r;
    r.x = x;
    r.y =
        reinterpret_cast<PlayerSelectDoubleBuffer *>(pDoubleBuffer)->Top + y;
    r.w = w;
    r.h = h;
    SetDrawArea(pClip, &r);
    addPrim(&reinterpret_cast<PlayerSelectDoubleBuffer *>(pDoubleBuffer)->OrderingTable[4095], pClip);
    pPoly = reinterpret_cast<unsigned int *>(pClip + 1);
}

/**
 * Draw the active skater, selector icons, and portrait.
 *
 * Address:   0x8008D3F8
 * Mangled:   draw__19SkaterScreenElementii
 * Demangled: SkaterScreenElement::draw(int, int)
 *
 * @param x Parent-provided horizontal draw offset.
 * @param y Parent-provided vertical draw offset.
 */
void SkaterScreenElement::draw(int x, int y)
{
    int DrawThisSkater;
    int fullWindowFaceOffsetX;
    int fullWindowFaceOffsetY;

    if (mSkaterChanged != 0)
    {
        DrawThisSkater = mVisibleSkater;
    }
    else
    {
        DrawThisSkater = SelectedSkater;
    }

    if (mSkaterLoaded[DrawThisSkater] != 0 &&
        (mCASTexturesSpooled != 0 || mVisibleSkater != 13 ||
         mShowCard != 0))
    {
        M3d_RenderSetup(
            MikeCamera, pViewport,
            reinterpret_cast<PlayerSelectDoubleBuffer *>(pDoubleBuffer)->OrderingTable);

        if ((Stats[mVisibleSkater].Available != 0 ||
             mVisibleSkater == 13) &&
            (mVisibleSkater != 13 ||
             SelectedSkater != 0))
        {
            if (mShowCard != 0)
            {
                M3d_Render(&mCardItem);
            }
            else
            {
                M3d_BuildTransform(mpFrontEndSkater[DrawThisSkater]);

                int t1 = M3d_OTPushback;
                int t2 = M3d_OTPushback2;
                M3d_OTPushback = -0x20;
                M3d_OTPushback2 = 0x20;
                M3d_Render(mpFrontEndSkater[DrawThisSkater]);
                M3d_OTPushback = t1;
                M3d_OTPushback2 = t2;

                if (mFullWindow == 0)
                {
                    int clipX =
                        ScreenElementList.ScreenElement.mScreenX + x +
                        mpSkaterWindow->GetScreenX() + 6;
                    SetClippingBox3(
                        clipX,
                        ScreenElementList.ScreenElement.mScreenY + y +
                            mpSkaterWindow->GetScreenY() + 15,
                        0x83, 0x70);
                }
            }
        }
        M3d_RenderCleanup();
    }

    {
        int i = 0;
        ScreenElement **icon = mpIcon;
        do
        {
            ScreenElement *element = *icon;
            ++icon;
            ++i;
            PlayerSelect_DrawElement(
                element,
                ScreenElementList.ScreenElement.mScreenX + x,
                ScreenElementList.ScreenElement.mScreenY + y);
        } while (i <= 0);
    }

    fullWindowFaceOffsetX = 0;
    fullWindowFaceOffsetY = 0;
    if (mFullWindow != 0)
    {
        fullWindowFaceOffsetX = 0x84;
        fullWindowFaceOffsetY = 0x3e;
    }

    {
        int i;
        for (i = 0; i < 8; ++i)
        {
            if (mpIconSprite[i] != 0 && mIconInfo[i].mHide == 0)
            {
                if (mFullWindow == 0)
                {
                    ((PlayerSelectSpriteDataView *)mpIconSprite[i])->draw(
                        ScreenElementList.ScreenElement.mScreenX + x +
                            mIconInfo[i].x,
                        ScreenElementList.ScreenElement.mScreenY + y +
                            mIconInfo[i].y,
                        0);
                }
                else
                {
                    ((PlayerSelectSpriteDataView *)mpIconSprite[i])->draw(
                        ScreenElementList.ScreenElement.mScreenX + x +
                            mIconInfo[i].x + mIconInfo[i].fullOffsetX,
                        ScreenElementList.ScreenElement.mScreenY + y +
                            mIconInfo[i].y + mIconInfo[i].fullOffsetY,
                        0);
                }
            }
        }
    }

    {
        int faceX = ScreenElementList.ScreenElement.mScreenX + x;
        int faceOffsetX = fullWindowFaceOffsetX - 0x26;
        Sprite2 *face = mpFaceSprite[SelectedSkater];
        int faceY = ScreenElementList.ScreenElement.mScreenY + y;
        int faceOffsetY = fullWindowFaceOffsetY + 0x20;
        ((PlayerSelectSpriteDataView *)face)->draw(
            faceX + faceOffsetX,
            faceY + faceOffsetY,
            0);
    }

    PlayerSelect_ListDraw(&ScreenElementList, x, y);
}

/**
 * Spool_LoadPSH2__FPcT0PPcPii — Parse a pre-loaded PSH texture name list
 * Variant of Spool_LoadPSH that operates on an already-loaded file buffer
 * instead of loading from disc. Uses Mem_GetBlockSize to determine buffer
 * length for null-termination. Otherwise identical parsing logic.
 * Used when the PSH data has already been loaded into memory (e.g. as part
 * of a larger archive or streaming load), avoiding a redundant disc read.
 * Parameters:
 *   fileBuffer — pre-loaded PSH text data in memory
 *   baseName   — level/region base name for constructing search pattern
 *   nameList   — output array of char* pointers to extracted names
 *   nameCount  — output: number of names extracted
 *   unused     — unused (present in mangled signature)
 * Returns: fileBuffer (names point into this buffer, which is modified in-place)
 * External globals
 *
 * Address:   0x8008D7DC, Size: 376 bytes (94 instructions)
 * Mangled:   Spool_LoadPSH2__FPcT0PPcPii
 * Demangled: char * Spool_LoadPSH2(char *, char *, char **, int *, int)
 *
 * @param pPSHBuffer  PSHBuffer to operate on.
 * @param pPSHName  PSHName to operate on.
 * @param pHierarchyArray  HierarchyArray to operate on.
 * @param pPartCount  PartCount to operate on.
 * @param maxPartNames  Maximum number of names to place in the output array.
 * @return The modified input buffer containing the extracted names.
 */

char *Spool_LoadPSH2(char *pPSHBuffer, char *pPSHName, char **pHierarchyArray, int *pPartCount, int maxPartNames)
{
    int prefixStringLength;
    int matchPos;
    char *p;
    char filename[16];
    char prefixString[16];

    // Build PSH filename and search pattern
    sprintf(filename, s__s_psh_800dc4b0, pPSHName);
    sprintf(prefixString, s__spart__800dc4b8, pPSHName);
    LowerCase(prefixString);

    // Null-terminate and lowercase the pre-loaded buffer
    pPSHBuffer[Mem_GetBlockSize(pPSHBuffer) + -1] = '\0';
    LowerCase(pPSHBuffer);

    *pPartCount = 0;
    prefixStringLength = strlen(prefixString);
    p = pPSHBuffer;

    // Parse loop: find each occurrence of the part pattern
    while (*p != '\0')
    {
        // Search for pattern in remaining text
        p = strstr(p, prefixString);
        if (p == 0)
            break; // No more matches
        p = p + prefixStringLength;

        // Advance to the '_' separator (start of the actual part name)
        if (*p != '_')
        {
            do
            {
                p = p + 1;
            } while (*p != '_');
        }

        // Store pointer to this name in the output array
        pHierarchyArray[*pPartCount] = p;

        // Find end of name: scan forward to first whitespace character
        while (matchPos = WhiteSpace(p), matchPos == 0)
        {
            p = p + 1;
        }

        // Null-terminate the name at the whitespace
        *p = '\0';
        p = p + 1;

        // Skip remaining data on this line (advance past newline)
        if (*p != '\n')
        {
            do
            {
                p = p + 1;
            } while (*p != '\n');
        }

        *pPartCount = *pPartCount + 1;
    }
    return pPSHBuffer;
}

/**
 * Consume completed skater-select spool entries and install their sprite,
 * model, hierarchy, and portrait data.
 *
 * Address:   0x8008D954
 * Mangled:   UpdateSpoolItems__19SkaterScreenElement
 * Demangled: SkaterScreenElement::UpdateSpoolItems(void)
 *
 */
void SkaterScreenElement::UpdateSpoolItems()
{
    Spooler::SpoolMgr->Update();

    {
        int i;
        for (i = 0; i < 8; ++i)
        {
            if (Spooler::SpoolMgr->IsFileLoaded(mIconInfo[i].mSpoolID))
            {
                unsigned char *theData = (unsigned char *)
                    Spooler::SpoolMgr->GetFile(mIconInfo[i].mSpoolID);
                mpIconSprite[i] = new Sprite2(theData, 1, 0, 0);
                Mem_Delete(theData);
            }
        }
    }

    {
        int i = 0;
        for (; i < 14; ++i)
        {
            if (Spooler::SpoolMgr->IsFileLoaded(mSpoolID[i + 28]))
            {
                unsigned char *theData = (unsigned char *)
                    Spooler::SpoolMgr->GetFile(mSpoolID[i + 28]);
                ((PlayerSelectSpriteDataView *)mpFaceSprite[i])->setData(
                    theData);
                Mem_Delete(theData);
                mpFaceSprite[i]->unpack();
                mpFaceSprite[i]->pack();
            }

            if (Spooler::SpoolMgr->IsFileLoaded(mSpoolID[i]))
            {
                unsigned int *theData =
                    (unsigned int *)Spooler::SpoolMgr->GetFile(mSpoolID[i]);
                if (mModelFile[i] != 0)
                {
                    Mem_Delete(mModelFile[i]);
                    mModelFile[i] = 0;
                }
                if (i < 2)
                {
                    Spool_StripAnimFile((char *)theData);
                    mModelFile[i] = theData;
                }
                else
                {
                    Mem_Delete(theData);
                }
            }

            if (Spooler::SpoolMgr->IsFileLoaded(mSpoolID[i + 14]))
            {
                char *theData =
                    (char *)Spooler::SpoolMgr->GetFile(mSpoolID[i + 14]);
                if (mSkaterLoaded[i] == 0 &&
                    i < 2 && i != 13)
                {
                    mpModelHierData[i] =
                        Spool_LoadPSH2(
                            theData,
                            Stats[i].PSXName,
                            mModelHierInfo[i].PSH_Hierarchy,
                            &mModelHierInfo[i].mPartCount,
                            32);
                    mpFrontEndSkater[i]->CalculateAnimOrder(
                        mAnimHierInfo.PSH_Hierarchy,
                        mAnimHierInfo.mPartCount,
                        mModelHierInfo[i].PSH_Hierarchy,
                        mModelHierInfo[i].mPartCount);
                    if (i == SelectedSkater)
                    {
                        mSkaterChanged = 1;
                        refreshSkater();
                    }
                }
                else
                {
                    Mem_Delete(theData);
                }
                mSkaterLoaded[i] = 1;
            }
        }
    }

}

/**
 *
 * Address:   0x8008DC94
 * Mangled:   update__19SkaterScreenElement
 * Demangled: SkaterScreenElement::update(void)
 *            Counter-based visibility, skater data check, element updates, InterleaveAI loop, rotation
 *
 */
void SkaterScreenElement::update()
{
    SkaterScreenElement *self = this;
    int counter;
    int counter2;
    int skaterIdx;
    void **element;
    int i;

    /* Increment counter */
    counter = self->mFlash;
    counter = counter + 1;
    self->mFlash = counter;

    /* Set visibility on two UI elements: show if counter >= 21 */
    self->ScreenElementList.mpItem[0]->mHide = !(counter < 21);

    counter2 = self->mFlash;
    self->ScreenElementList.mpItem[1]->mHide = !(counter2 < 21);

    /* Check skater data availability */
    skaterIdx = self->mVisibleSkater;
    if (self->mSkaterLoaded[skaterIdx] == 0)
        goto _hide;
    if (mCASTexturesSpooled != 0)
        goto _show;
    if (skaterIdx != 13)
        goto _show;
    if (self->mShowCard != 0)
        goto _show;
_hide:
    self->mpIcon[0]->mHide = 0;
    goto _after_vis;
_show:
    self->mpIcon[0]->mHide = 1;
_after_vis:

    /* Wrap counter at 40 */
    if (self->mFlash >= 40)
    {
        self->mFlash = 0;
    }

    /* Loop 1: call virtual update on element at 0xAC (single element) */
    i = 0;
    element = (void **)self->mpIcon;
    do
    {
        ScreenElement *elem = (ScreenElement *)*element;
        ScreenElementDispatchTable *vtable =
            (ScreenElementDispatchTable *)elem->CClass.vtable;
        void (*update_fn)(void *) = vtable->Update;
        element++;
        update_fn(elem);
        i++;
    } while (i <= 0);

    /* Call base class update */
    PlayerSelect_ListUpdate(&self->ScreenElementList);

    /* Loop 2: call InterleaveAI on 14 CBody objects at offset 0x70 */
    i = 0;
    element = (void **)self->mpFrontEndSkater;
    do
    {
        CSuper *body = (CSuper *)*element;
        element++;
        CBody_InterleaveAI(reinterpret_cast<CBody *>(&body->CBody));
        i++;
    } while (i < 14);

    /* Update rotation angle: add 0x3C, wrap to 12 bits */
    {
        int rot = (u16)self->mCardItem.Angles.vy;
        rot = (rot + 0x3C) & 0xFFF;
        self->mCardItem.Angles.vy = (u16)rot;
    }
}

/**
 * Refresh icon visibility for the selected skater.
 *
 * Address:   0x8008DDD8
 * Mangled:   refresh__19SkaterScreenElement
 * Demangled: SkaterScreenElement::refresh(void)
 *            Sync the selected skater icon flags to the current career/stat state.
 * Match:     PERFECT (44/44)
 *
 */
void SkaterScreenElement::refresh()
{
    SkaterScreenElement *self = this;
    int hide = 0;
    SkaterScreenElement *screen = self;
    ICON_INFO *icons = mIconInfo;
    int selectedSkater = SelectedSkater;
    int isStandardSkater = selectedSkater != 13;

    self->mSkaterChanged = 1;
    icons[4].mHide = 1;
    icons[0].mHide = isStandardSkater;
    icons[1].mHide = !isStandardSkater;
    icons[2].mHide = Stats[selectedSkater].Ability[10] != 1;
    icons[3].mHide = Stats[selectedSkater].Ability[10] != 0;
    icons[5].mHide = Stats[selectedSkater].Ability[11] != 0;
    icons[6].mHide = Stats[selectedSkater].Ability[11] != 1;
    if (Stats[selectedSkater].Available != 0)
    {
        hide = screen->mShowCard == 0;
    }
    icons[7].mHide = hide;
}

/**
 * Queue the selected skater first, followed by every remaining skater and the
 * shared selector icons.
 *
 * Address:   0x8008DE88
 * Mangled:   AddSpoolItems__19SkaterScreenElement
 * Demangled: SkaterScreenElement::AddSpoolItems(void)
 *
 */
void SkaterScreenElement::AddSpoolItems()
{
    int order[14];
    char filename[32];

    Spooler::SpoolMgr->Reset();
    Spooler::OrderHelper(order, 14, SelectedSkater);

    {
        int num = order[0];

        if (num < 2)
        {
            sprintf(filename, s__s_psx_800dc4c0, Stats[num].PSXName);
        }
        else
        {
            sprintf(filename, s__s_psx_800dc4c0, Stats[0].PSXName);
        }
        mSpoolID[num] = Spooler::SpoolMgr->AddFile(filename, 1);

        if (num < 2)
        {
            sprintf(filename, s__s_psh_800dc4b0, Stats[num].PSXName);
        }
        else
        {
            sprintf(filename, s__s_psh_800dc4b0, Stats[0].PSXName);
        }
        mSpoolID[num + 14] = Spooler::SpoolMgr->AddFile(filename, 0);
        mSpoolID[num + 28] =
            Spooler::SpoolMgr->AddFile(Stats[num].PortraitName, 0);
    }

    {
        int i = 0;
        do
        {
            mIconInfo[i].mSpoolID =
                Spooler::SpoolMgr->AddFile(mIconInfo[i].Name, 0);
            ++i;
        } while (i < 8);
    }

    {
        int i = 1;
        do
        {
            int num = order[i];

            if (num < 2)
            {
                sprintf(filename, s__s_psx_800dc4c0, Stats[num].PSXName);
            }
            else
            {
                sprintf(filename, s__s_psx_800dc4c0, Stats[0].PSXName);
            }
            mSpoolID[num] =
                Spooler::SpoolMgr->AddFile(filename, 1);

            if (num < 2)
            {
                sprintf(filename, s__s_psh_800dc4b0, Stats[num].PSXName);
            }
            else
            {
                sprintf(filename, s__s_psh_800dc4b0, Stats[0].PSXName);
            }
            mSpoolID[num + 14] =
                Spooler::SpoolMgr->AddFile(filename, 0);
            mSpoolID[num + 28] =
                Spooler::SpoolMgr->AddFile(Stats[num].PortraitName, 0);

            ++i;
        } while (i < 14);
    }

    Spooler::SpoolMgr->Spooool();

}

/**
 * Build the front-end skater objects and their shared animation/model region.
 *
 * Address:   0x8008E1F0
 * Mangled:   LoadAllSkaters__19SkaterScreenElement
 * Demangled: SkaterScreenElement::LoadAllSkaters(void)
 *
 */
void SkaterScreenElement::LoadAllSkaters()
{
    unsigned int *pSpoolFile;

    if (SelectedSkater > 1 && SelectedSkater != 13)
    {
        SelectedSkater = 0;
    }

    {
        int i = 0;
        do
        {
            if (Stats[i].Available == 0 && i != 13)
            {
                mSkaterLoaded[i] = 1;
            }
            ++i;
        } while (i < 14);
    }

    mAnimRegion = GetReducedAnim(s_anims_fe_800dc4c8);
    /* The original keeps the anim hierarchy table in a named local: the target
     * computes `addiu $s6,$s1,0x228` once at this call and re-uses $s6 for the
     * CalculateAnimOrder call in the loop below. */
    char **animHier = mAnimHierInfo.PSH_Hierarchy;
    mpAnimHierData = Spool_LoadPSH(
        s_anims_fe_800dc4c8,
        animHier,
        &mAnimHierInfo.mPartCount,
        32);
    mpModelHierData[0] = Spool_LoadPSH(
        Stats[0].PSXName,
        mModelHierInfo[0].PSH_Hierarchy,
        &mModelHierInfo[0].mPartCount,
        32);

    int modelRegion = Spool_FreeRegion();
    mModelRegionGeneric = modelRegion;
    PSXRegion[modelRegion].pAnimFile = 0;
    PSXRegion[modelRegion].pColourPulseData = 0;
    PSXRegion[modelRegion].pTexWibData = 0;
    PSXRegion[modelRegion].Protected = 0;
    PSXRegion[modelRegion].Usable = 0;
    PSXRegion[modelRegion].pHooks = 0;
    PSXRegion[modelRegion].ppModels = 0;
    PSXRegion[modelRegion].Filename[0] = 'G';
    PSXRegion[modelRegion].Filename[1] = 'E';
    PSXRegion[modelRegion].Filename[2] = 'N';
    PSXRegion[modelRegion].Filename[3] = '\0';
    PSXRegion[modelRegion].TexturesInRAM = 0;
    PSXRegion[modelRegion].KeepTexturesInRAM = 0;
    PSXRegion[modelRegion].WhichHeap = 1;

    pSpoolFile = GetReducedPSXFile(Stats[0].PSXName);

    {
        int i = 0;
        /* The first skater owns the generic model file and hierarchy shared by
         * every front-end skater. Keeping that index in the loop's enclosing
         * block also reproduces the target's register-held zero comparison. */
        int firstSkater = 0;
        do
        {
            /* The original walks this+0x1c4 in a callee-saved register ($s2,
             * `addiu $s2,$s2,4` per iteration) and keeps the counter in $s3.
             * Spelling the walker as a loop-body `&mModelFile[i]` (rather than a
             * preheader `&mModelFile[0]` that is stepped by `++`) is load-bearing:
             * it becomes a strength-reduced induction variable whose initialiser
             * loop.c emits *after* the hoisted loop invariant, reproducing the
             * target's preheader order `addiu <inv>,$s1,...` then
             * `addiu $s2,$s1,0x1c4`, and it keeps the walker ahead of the counter
             * in the allocation order. */
            unsigned int **ppModelFile = &mModelFile[i];
            mpFrontEndSkater[i] = new CSuper;

            ((CSuperOwner *)mpFrontEndSkater[i])->CBody.CItem.mpLight =
                &Skater_CreateLight;
            if (Stats[i].Available == 0)
            {
                ((CSuperOwner *)mpFrontEndSkater[i])->CBody.CItem.mpLight =
                    &Skater_NotAvailableLight;
            }

            if (mFullWindow != 0)
            {
                ((CSuperOwner *)mpFrontEndSkater[i])->CBody.CItem.mPos.x =
                    0x5e000;
                ((CSuperOwner *)mpFrontEndSkater[i])->CBody.CItem.mPos.y =
                    0x3c000;
                ((CSuperOwner *)mpFrontEndSkater[i])->CBody.CItem.mPos.z =
                    -0xc000;
            }
            else
            {
                ((CSuperOwner *)mpFrontEndSkater[i])->CBody.CItem.mPos.x =
                    0xba000;
                ((CSuperOwner *)mpFrontEndSkater[i])->CBody.CItem.mPos.y =
                    0x2000;
                ((CSuperOwner *)mpFrontEndSkater[i])->CBody.CItem.mPos.z =
                    0x60000;
            }

            ((CSuperOwner *)mpFrontEndSkater[i])->CBody.CItem.mAngles.y =
                (short)Stats[i].FrontEndOrientation;
            *ppModelFile = 0;
            if (i == firstSkater)
            {
                *ppModelFile = pSpoolFile;
            }

            mpFrontEndSkater[i]->CalculateAnimOrder(
                animHier,
                mAnimHierInfo.mPartCount,
                mModelHierInfo[firstSkater].PSH_Hierarchy,
                mModelHierInfo[firstSkater].mPartCount);

            PSXRegion[mModelRegionGeneric].pHierarchy =
                PSXRegion[mAnimRegion].pHierarchy;
            PSXRegion[mModelRegionGeneric].pAnimFile =
                PSXRegion[mAnimRegion].pAnimFile;
            ((CSuperOwner *)mpFrontEndSkater[i])->CBody.CItem.mRegion =
                (unsigned char)mModelRegionGeneric;
            ((CSuperOwner *)mpFrontEndSkater[i])->mAnimSpeed = 0x10000;
            TimeScale = 0x100;
            mpFrontEndSkater[i]->CycleAnim(0, 1);
            ((CSuperOwner *)mpFrontEndSkater[i])->mExtraFlags |= 2;

            ++i;
        } while (i < 14);
    }

    Create_BuildSkater(mpFrontEndSkater[13]);
    mpFrontEndSkater[13]->CycleAnim(0, 1);
}

/**
 * Compact a skater model file to the data needed by the front-end preview.
 *
 * Address:   0x8008E574
 * Mangled:   GetReducedPSXFile__19SkaterScreenElementPc
 * Demangled: SkaterScreenElement::GetReducedPSXFile(char *)
 *
 * @param modelFile  Model base name without the `.psx` suffix.
 * @return Heap buffer containing the compacted model file.
 */
unsigned int *SkaterScreenElement::GetReducedPSXFile(char *modelFile)
{
    char filename[16];
    sprintf(filename, s__s_psx_800dc4c0, modelFile);
    char *pData = FileIO_OpenLoad(filename);
    Spool_StripAnimFile(pData);
    return (unsigned int *)pData;
}

/**
 * Spool_StripAnimFile__FPc — Strip animation data from a PSH file in-place
 * Compacts a PSH (animation header) file by copying data from the source
 * position to a compacted destination, stripping skip-packet headers.
 * Reveals the PSH file block structure:
 * PSH file layout:
 *   +0x00: u32 magic/version
 *   +0x04: u32 dataOffset (offset to start of animation data)
 *   +0x08: u32 entryCount (number of table entries)
 *   +0x0C: entry table (entryCount entries, 36 bytes each)
 *     entry+0x0C: u32 blockCount (at offset 0x0C within entry stride=36)
 *   After entries: skip-packet area → animation data
 * Animation data blocks (copied in order):
 *   Block 1: blockCount words (unsigned loop, from entry table last entry's blockCount)
 *   Block 2: count + count words (animation names, signed)
 *   Block 3: count + count × 9 words (bone rotation data, 36 bytes per bone)
 *   Block 4: count + count × 129 words (extended bone data, 516 bytes per entry)
 *   Block 5: count + count pointer-relocated words (offsets adjusted for compaction)
 *   Block 6: for each entry from block 5: 5 header words + variable texture/sprite data
 *     Sprite data size = width × height bytes (÷2 if type == 0x10 for 4-bit)
 *     Aligned to 4 bytes
 *
 * Address:   0x8008E5BC, Size: 620 bytes (155 instructions)
 * Mangled:   Spool_StripAnimFile__FPc
 * Demangled: void Spool_StripAnimFile(char *)
 * Match:     PERFECT (155/155)
 *
 * @param pPSX  PSX to operate on.
 */

void Spool_StripAnimFile(char *pPSX)
{
    char *pDest;
    u32 *pAnim;
    u32 NumModels;
    u32 count;
    int i;
    int j;
    {
        register u32 dataOffset asm("$16") =
            ((PlayerSelectAnimFileHeader *)pPSX)->DataOffset;
        pDest = pPSX + dataOffset;
    }

    NumModels =
        ((PlayerSelectAnimDirectoryEntry *)(pPSX +
            sizeof(PlayerSelectAnimFileHeader)))[
                ((PlayerSelectAnimFileHeader *)pPSX)->EntryCount].BlockCount;

    /* Skip packets to get to animation data */
    pAnim = Spool_SkipPackets((u32 *)pPSX);

    /* Write sentinel */
    register u32 sentinel asm("$2") = (u32)-1;
    *(u32 *)pDest = sentinel;
    pDest += 4;

    /* Block 1: copy NumModels words (unsigned comparison) */
    i = 0;
    if (NumModels != 0)
    {
        do
        {
            *(u32 *)pDest = *pAnim;
            pAnim++;
            i++;
            pDest += 4;
        } while ((u32)i < NumModels);
    }

    /* Block 2: read count, copy count words (signed comparison) */
    count = *pAnim;
    pAnim++;
    *(u32 *)pDest = count;
    pDest += 4;

    i = 0;
    if ((s32)count > 0)
    {
        do
        {
            *(u32 *)pDest = *pAnim;
            pAnim++;
            i++;
            pDest += 4;
        } while (i < (s32)count);
    }

    /* Block 3: read count, then count × 9 words per entry (bone rotations) */
    count = *pAnim;
    pAnim++;
    *(u32 *)pDest = count;
    pDest += 4;

    j = 0;
    if ((s32)count > 0)
    {
        i = 0;
        do
        {
            *(u32 *)pDest = *pAnim;
            pAnim++;
            i++;
            pDest += 4;
        } while (i < 9 || (j++, i = 0, j < (s32)count));
    }

    /* Block 4: read count, then count × 129 words per entry (extended bone data) */
    count = *pAnim;
    pAnim++;
    *(u32 *)pDest = count;
    pDest += 4;

    j = 0;
    if ((s32)count > 0)
    {
        i = 0;
        do
        {
            *(u32 *)pDest = *pAnim;
            pAnim++;
            i++;
            pDest += 4;
        } while (i < 0x81 || (j++, i = 0, j < (s32)count));
    }

    /* Block 5: read count, then count pointer-relocated words */
    count = *pAnim;
    pAnim++;
    *(u32 *)pDest = count;
    pDest += 4;

    i = 0;
    if ((s32)count > 0)
    {
        do
        {
            u32 reloc = (u32)((char *)pAnim - pDest);
            u32 value = *pAnim;
            pAnim++;
            i++;
            *(u32 *)pDest = value - reloc;
            pDest += 4;
        } while (i < (s32)count);
    }

    /* Block 6: for each sprite entry, copy 5 header words + pixel data */
    {
        int i = 0;

        if ((s32)count > 0)
        {
            u32 mask = 0xFFFF0000;

            do
            {
                u32 word0, word1, word2, word3, word4;
                u32 width, height;
                int pixelSize;

                /* Copy 5 header words (20 bytes) */
                word0 = *pAnim;
                pAnim++;
                *(u32 *)pDest = word0;
                pDest += 4;
                word1 = *pAnim;
                pAnim++;
                *(u32 *)pDest = word1;
                pDest += 4;
                word2 = *pAnim;
                pAnim++;
                *(u32 *)pDest = word2;
                pDest += 4;
                word3 = *pAnim;
                pAnim++;
                *(u32 *)pDest = word3;
                pDest += 4;
                word4 = *pAnim;
                *(u32 *)pDest = word4;

                /* Extract dimensions from word4: upper 16 = width, lower 16 = height */
                width = (word4 & mask) >> 16;
                height = word4 & 0xFFFF;
                pixelSize = width * height;
                pAnim++;
                pDest += 4;

                /* If type == 0x10 (4-bit texture), halve the pixel data size */
                if (word1 == 0x10)
                {
                    pixelSize >>= 1;
                }

                /* Align pixel data size: add 4 then subtract remainder */
                {
                    int numBytes = pixelSize + 4;
                    pixelSize = numBytes - (pixelSize & 3);
                }

                /* Copy pixel data words */
                {
                    int j = 0;
                    while (j < pixelSize / 4 - 1)
                    {
                        *(u32 *)pDest = *pAnim;
                        pAnim++;
                        pDest += 4;
                        j++;
                    }
                }

                i++;
            } while (i < (s32)count);
        }
    }

    /* Shrink allocation to actual used size */
    Mem_Shrink(pPSX, (u32)(pDest - pPSX));
}

/**
 * Load and compact a front-end animation file into a PSX region.
 *
 * Address:   0x8008E828
 * Mangled:   GetReducedAnim__19SkaterScreenElementPc
 * Demangled: SkaterScreenElement::GetReducedAnim(char *)
 *            Extract the reduced animation PSX into a fresh PSX region slot.
 * Match:     PERFECT (73/73)
 *
 * @return AnimRegion
 */
int SkaterScreenElement::GetReducedAnim(char *animFile)
{
    SkaterScreenElement *self = this;
    struct SVec3 { short x, y, z; };
    short FrontEndAnimations[3];
    int size;
    char *preAnimFile = PREMgr->getFile(s_anims_cs_psx_800dc4d4, &size);
    int AnimRegion = Spool_FreeRegion();

    PSXRegion[AnimRegion].pAnimFile = 0;
    PSXRegion[AnimRegion].pColourPulseData = 0;
    PSXRegion[AnimRegion].pTexWibData = 0;
    PSXRegion[AnimRegion].Protected = 0;
    PSXRegion[AnimRegion].Usable = 0;
    PSXRegion[AnimRegion].pHooks = 0;
    PSXRegion[AnimRegion].ppModels = 0;

    sprintf(PSXRegion[AnimRegion].Filename, DAT_800dc4e4, s_anims_fe_800dc4c8);

    PSXRegion[AnimRegion].TexturesInRAM = 0;
    PSXRegion[AnimRegion].KeepTexturesInRAM = 0;
    PSXRegion[AnimRegion].WhichHeap = 1;
    PSXRegion[AnimRegion].pPSX = (unsigned int *)Mem_New(size, 0, 1);
    Mem_Copy(PSXRegion[AnimRegion].pPSX, preAnimFile, size);

    ProcessNewPSX(AnimRegion);

    *(struct SVec3 *)FrontEndAnimations = *(struct SVec3 *)DAT_800dc4e8;
    Spool_StripModel(AnimRegion, FrontEndAnimations);
    PSXRegion[AnimRegion].Usable = 2;
    return AnimRegion;
}

/**
 *
 * Address:   0x8008E94C
 * Mangled:   refreshSkater__19SkaterScreenElement
 * Demangled: SkaterScreenElement::refreshSkater(void)
 *            Rebuild the selected skater preview model and sync its visible state.
 *
 */
void SkaterScreenElement::refreshSkater()
{
    int LoadThisSkater = 0;

    if (mModelFile[SelectedSkater] != 0)
    {
        LoadThisSkater = SelectedSkater;
    }

    if (mModelFile[LoadThisSkater] != 0)
    {
        DecrementTextureUsage(mModelRegionGeneric);
        Spool_RemoveUnusedTextures();
        Pal_RemoveUnusedPalettes();
        Pack_DefragmentFully();

        SPSXRegion *psxRegion = PSXRegion;
        PlayerSelectRegionPSXColumn *pPSXBase =
            (PlayerSelectRegionPSXColumn *)&psxRegion[0].pPSX;

        if (pPSXBase[mModelRegionGeneric].Value != 0)
        {
            Mem_Delete(pPSXBase[mModelRegionGeneric].Value);
        }

        pPSXBase[mModelRegionGeneric].Value =
            (unsigned int *)Mem_New(
                Mem_GetBlockSize(mModelFile[LoadThisSkater]),
                0, 1);
        Mem_Copy(pPSXBase[mModelRegionGeneric].Value,
                 mModelFile[LoadThisSkater],
                 Mem_GetBlockSize(mModelFile[LoadThisSkater]));
        ProcessNewPSX(mModelRegionGeneric);

        PlayerSelectRegionModelsColumn *ppModelsBase =
            (PlayerSelectRegionModelsColumn *)&psxRegion[0].ppModels;
        int animRegion = mAnimRegion;
        int genericRegion = mModelRegionGeneric;
        int visibleSkater = SelectedSkater;
        ppModelsBase[animRegion].Value = ppModelsBase[genericRegion].Value;
        mVisibleSkater = visibleSkater;
        mSkaterChanged = 0;
        ((CSuperPosePrefix *)mpFrontEndSkater[visibleSkater])->mAngles.vy =
            (short)Stats[visibleSkater].FrontEndOrientation;
    }
}

/**
 *
 * Address:   0x8008EAF0
 * Mangled:   __18StatsScreenElementiiii
 * Demangled: StatsScreenElement::StatsScreenElement(int, int, int, int)
 *
 * @param homeX  Home X coordinate (resting position).
 * @param homeY  Home Y coordinate (resting position).
 * @param screenX  Screen-space X coordinate.
 * @param screenY  Screen-space Y coordinate.
 */
StatsScreenElement::StatsScreenElement(int homeX, int homeY, int screenX, int screenY)
{
    ScreenElementListBase_ctor(
        &ScreenElementList, homeX, homeY, screenX, screenY);
    ScreenElementList.ScreenElement.CClass.vtable =
        __vt_18StatsScreenElement;

    SpriteScreenElement *obj = (SpriteScreenElement *)
        ::CClass::operator new(sizeof(SpriteScreenElement));
    if (obj != 0)
    {
        obj = PlayerSelect_ConstructSpriteElement(
            obj,
            s_s2dots_bmp_800dc4f0, 0, 0, -1, -1, 0, 0);
    }
    mpDots = obj;
}

/**
 *
 * Address:   0x8008EB68
 * Mangled:   _._18StatsScreenElement
 * Demangled: StatsScreenElement::~StatsScreenElement()
 *
 */
StatsScreenElement::~StatsScreenElement()
{
    ScreenElementList.ScreenElement.CClass.vtable =
        __vt_18StatsScreenElement;
    ScreenElement *obj = (ScreenElement *)mpDots;
    if (obj != 0)
    {
        ScreenElementDispatchTable *vtbl =
            (ScreenElementDispatchTable *)obj->CClass.vtable;
        void (*dtor)(void *, int) = vtbl->Destroy;
        dtor(obj, 3);
    }
    PlayerSelect_ListDtor(&ScreenElementList, 0);

}

/**
 * Draw the selected skater's ten ability meters using the dot sprite and a
 * clipping box scaled to each ability value.
 *
 * Address:   0x8008EBE0
 * Mangled:   draw__18StatsScreenElementii
 * Demangled: StatsScreenElement::draw(int, int)
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 */
void StatsScreenElement::draw(int x, int y)
{
    int i = 0;

    do
    {
        SetClippingBox2(0, 0, 0x200, 0xf0);
        ((ScreenElementDispatchTable *)
            mpDots->ScreenElement.CClass.vtable)->Draw(
            mpDots,
            ScreenElementList.ScreenElement.mScreenX + x,
            ScreenElementList.ScreenElement.mScreenY + y + i * 8);
        SetClippingBox2(
            ScreenElementList.ScreenElement.mScreenX,
            0,
            (uint)Stats[SelectedSkater].Ability[i] * 0xc,
            0xf0);
        i = i + 1;
    } while (i < 10);
}

/**
 *
 * Address:   0x8008ECD4
 * Mangled:   update__18StatsScreenElement
 * Demangled: StatsScreenElement::update(void)
 *            Loads this->field_0x68 (a ScreenElement*) and calls its virtual update method
 *
 */
void StatsScreenElement::update()
{
    ((ScreenElementDispatchTable *)mpDots->ScreenElement.CClass.vtable)
        ->Update(mpDots);
}

/**
 * Build the multiplayer player-number prompt.
 *
 * Address:   0x8008ED0C
 * Mangled:   __24MultiPlayerScreenElementP4Fontiiii
 * Demangled: MultiPlayerScreenElement::(Font *, int, int, int, int)
 *
 * @param pFont  Font used for rendering.
 * @param homeX  Home X coordinate (resting position).
 * @param homeY  Home Y coordinate (resting position).
 * @param screenX  Screen-space X coordinate.
 * @param screenY  Screen-space Y coordinate.
 */
MultiPlayerScreenElement::MultiPlayerScreenElement(Font *pFont, int homeX, int homeY, int screenX, int screenY)
{
    char test[128];
    TextScreenElement *obj;

    ScreenElementListBase_ctor(
        &ScreenElementList, homeX, homeY, screenX, screenY);
    ScreenElementList.ScreenElement.CClass.vtable =
        __vt_24MultiPlayerScreenElement;
    MultiPlayerListCount = 0;
    mpFont = pFont;
    mFlash = 0;

    sprintf(test, s_dummy_800dc4fc);
    obj = (TextScreenElement *)
        ::CClass::operator new(sizeof(TextScreenElement));
    if (obj != 0)
    {
        obj = PlayerSelect_ConstructTextElement(
            obj,
            test, mpFont, 0x40, 6, -1, -1);
    }
    ScreenElementList.addItem((ScreenElement *)obj);

    sprintf(test, s_choose_skater_800dc504);
    obj = (TextScreenElement *)
        ::CClass::operator new(sizeof(TextScreenElement));
    if (obj != 0)
    {
        obj = PlayerSelect_ConstructTextElement(
            obj,
            test, mpFont, 0x1A, 0x16, -1, -1);
    }
    ScreenElementList.addItem((ScreenElement *)obj);
    this->refresh();
}

/**
 * Destroy the multiplayer prompt and its child elements.
 *
 * Address:   0x8008EE14
 * Mangled:   _._24MultiPlayerScreenElement
 * Demangled: MultiPlayerScreenElement::~MultiPlayerScreenElement()
 *
 */
MultiPlayerScreenElement::~MultiPlayerScreenElement()
{
    ScreenElementList.ScreenElement.CClass.vtable =
        __vt_24MultiPlayerScreenElement;
    PlayerSelect_ListDtor(&ScreenElementList, 0);

}

/**
 *
 * Address:   0x8008EE68
 * Mangled:   update__24MultiPlayerScreenElement
 * Demangled: MultiPlayerScreenElement::update(void)
 *            Calls base class ScreenElementList::update(this)
 *
 */
void MultiPlayerScreenElement::update()
{
    PlayerSelect_ListUpdate(&ScreenElementList);
}

/**
 * Refresh the multiplayer player label.
 *
 * Address:   0x8008EE88
 * Mangled:   refresh__24MultiPlayerScreenElement
 * Demangled: MultiPlayerScreenElement::refresh(void)
 *
 */
void MultiPlayerScreenElement::refresh()
{
    char test[128];
    sprintf(test, s_player__d_800dc514, MultiPlayerListCount + 1);
    ((TextScreenElement *)ScreenElementList.mpItem[0])->setString(test);
}

/**
 * ResetStartingLocation__24MultiPlayerScreenElement — Set starting location to 0x200
 *
 * Address:   0x8008EECC, Size: 12 bytes (3 instructions)
 * Mangled:   ResetStartingLocation__24MultiPlayerScreenElement
 * Demangled: MultiPlayerScreenElement::ResetStartingLocation(void)
 *            Assembly: addiu $v0, $zero, 0x200; jr $ra; sw $v0, 0x1c($a0)
 *
 */
void MultiPlayerScreenElement::ResetStartingLocation()
{
    this->ScreenElementList.ScreenElement.mScreenX = 0x200;
}

// === PlayerSelect-owned small-data literals ===

/** Prompt, navigation, and career-summary resources. */
static char PlayerSelect_BackgroundName[12] asm("s_s2bgd1_bmp_800dc318")
    __attribute__((section(".sdata"), aligned(4))) = "s2bgd1.bmp";
static char PlayerSelect_Prompt4Name[16] asm("s_s2prmpt4_bmp_800dc324")
    __attribute__((section(".sdata"), aligned(4))) = "s2prmpt4.bmp";
static char PlayerSelect_SelectText[8] asm("s_select_800dc334")
    __attribute__((section(".sdata"), aligned(4))) = "select";
static char PlayerSelect_Prompt5Name[16] asm("s_s2prmpt5_bmp_800dc33c")
    __attribute__((section(".sdata"), aligned(4))) = "s2prmpt5.bmp";
static char PlayerSelect_ScrollBioText[12] asm("s_scroll_bio_800dc34c")
    __attribute__((section(".sdata"), aligned(4))) = "scroll bio";
static char PlayerSelect_Prompt1Name[16] asm("s_s2prmpt1_bmp_800dc358")
    __attribute__((section(".sdata"), aligned(4))) = "s2prmpt1.bmp";
static char PlayerSelect_AcceptText[8] asm("s_accept_800dc368")
    __attribute__((section(".sdata"), aligned(4))) = "accept";
static char PlayerSelect_Prompt2Name[16] asm("s_s2prmpt2_bmp_800dc370")
    __attribute__((section(".sdata"), aligned(4))) = "s2prmpt2.bmp";
static char PlayerSelect_BackText[8] asm("s_back_800dc380")
    __attribute__((section(".sdata"), aligned(4))) = "back";
static char PlayerSelect_TeamChickenText[16]
    asm("s_TEAM_CHICKEN_800dc388")
    __attribute__((section(".sdata"), aligned(4))) = "TEAM CHICKEN";
static char PlayerSelect_LoadCharacterText[16]
    asm("s_LOAD_CHARACTER_800dc398")
    __attribute__((section(".sdata"), aligned(4))) = "LOAD CHARACTER";
static char PlayerSelect_ScoreFormat[8] asm("s__7d_800dc3a8")
    __attribute__((section(".sdata"), aligned(4))) = "$%7d";
static char PlayerSelect_Win4Name[12] asm("s_s2win4_bmp_800dc3b0")
    __attribute__((section(".sdata"), aligned(4))) = "s2win4.bmp";
static char PlayerSelect_Medal1Name[16] asm("s_s2medal1_bmp_800dc3bc")
    __attribute__((section(".sdata"), aligned(4))) = "s2medal1.bmp";
static char PlayerSelect_Medal2Name[16] asm("s_s2medal2_bmp_800dc3cc")
    __attribute__((section(".sdata"), aligned(4))) = "s2medal2.bmp";
static char PlayerSelect_Medal3Name[16] asm("s_s2medal3_bmp_800dc3dc")
    __attribute__((section(".sdata"), aligned(4))) = "s2medal3.bmp";
static char PlayerSelect_ScrollName[16] asm("s_s2scroll_bmp_800dc3ec")
    __attribute__((section(".sdata"), aligned(4))) = "s2scroll.bmp";
static char PlayerSelect_Win3Name[12] asm("s_s2win3_bmp_800dc3fc")
    __attribute__((section(".sdata"), aligned(4))) = "s2win3.bmp";
static char PlayerSelect_Arrow1Name[16] asm("s_s2arrow1_bmp_800dc408")
    __attribute__((section(".sdata"), aligned(4))) = "s2arrow1.bmp";
static char PlayerSelect_Arrow2Name[16] asm("s_s2arrow2_bmp_800dc418")
    __attribute__((section(".sdata"), aligned(4))) = "s2arrow2.bmp";
static char PlayerSelect_Win1Name[12] asm("s_s2win1_bmp_800dc428")
    __attribute__((section(".sdata"), aligned(4))) = "s2win1.bmp";
static char PlayerSelect_MemCardName[12] asm("s_mem_card_800dc434")
    __attribute__((section(".sdata"), aligned(4))) = "mem_card";
static char PlayerSelect_Win2Name[12] asm("s_s2win2_bmp_800dc440")
    __attribute__((section(".sdata"), aligned(4))) = "s2win2.bmp";
static char PlayerSelect_Icon1Name[12] asm("s_s2icon1_bmp_800dc44c")
    __attribute__((section(".sdata"), aligned(4))) = "s2icon1.bmp";
static char PlayerSelect_Icon2Name[12] asm("s_s2icon2_bmp_800dc458")
    __attribute__((section(".sdata"), aligned(4))) = "s2icon2.bmp";
static char PlayerSelect_Icon3Name[12] asm("s_s2icon3_bmp_800dc464")
    __attribute__((section(".sdata"), aligned(4))) = "s2icon3.bmp";
static char PlayerSelect_Icon4Name[12] asm("s_s2icon4_bmp_800dc470")
    __attribute__((section(".sdata"), aligned(4))) = "s2icon4.bmp";
static char PlayerSelect_Icon5Name[12] asm("s_s2icon5_bmp_800dc47c")
    __attribute__((section(".sdata"), aligned(4))) = "s2icon5.bmp";
static char PlayerSelect_Icon6Name[12] asm("s_s2icon6_bmp_800dc488")
    __attribute__((section(".sdata"), aligned(4))) = "s2icon6.bmp";
static char PlayerSelect_NaDemoName[16] asm("s_s2nademo_bmp_800dc494")
    __attribute__((section(".sdata"), aligned(4))) = "s2nademo.bmp";

/** Skater-spooling formats, animation assets, and multiplayer labels. */
static char PlayerSelect_LoadingText[12] asm("s_Loading____800dc4a4")
    __attribute__((section(".sdata"), aligned(4))) = "Loading...";
static char PlayerSelect_PshFormat[8] asm("s__s_psh_800dc4b0")
    __attribute__((section(".sdata"), aligned(4))) = "%s.psh";
static char PlayerSelect_PartFormat[8] asm("s__spart__800dc4b8")
    __attribute__((section(".sdata"), aligned(4))) = "%spart_";
static char PlayerSelect_PsxFormat[8] asm("s__s_psx_800dc4c0")
    __attribute__((section(".sdata"), aligned(4))) = "%s.psx";
static char PlayerSelect_FrontEndAnimName[12] asm("s_anims_fe_800dc4c8")
    __attribute__((section(".sdata"), aligned(4))) = "anims_fe";
static char PlayerSelect_AnimArchiveName[16]
    asm("s_anims_cs_psx_800dc4d4")
    __attribute__((section(".sdata"), aligned(4))) = "anims_cs.psx";
static char PlayerSelect_NameFormat[4] asm("DAT_800dc4e4")
    __attribute__((section(".sdata"), aligned(4))) = "%s";
static short PlayerSelect_FrontEndAnimations[4] asm("DAT_800dc4e8")
    __attribute__((section(".sdata"), aligned(4))) = {0, -1, -1, 0};
static char PlayerSelect_DotsName[12] asm("s_s2dots_bmp_800dc4f0")
    __attribute__((section(".sdata"), aligned(4))) = "s2dots.bmp";
static char PlayerSelect_DummyText[8] asm("s_dummy_800dc4fc")
    __attribute__((section(".sdata"), aligned(4))) = "dummy";
static char PlayerSelect_ChooseSkaterText[16]
    asm("s_choose_skater_800dc504")
    __attribute__((section(".sdata"), aligned(4))) = "choose skater";
static char PlayerSelect_PlayerFormat[12] asm("s_player__d_800dc514")
    __attribute__((section(".sdata"), aligned(4))) = "player %d";
