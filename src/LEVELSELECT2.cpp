// LEVELSELECT2.cpp -- Reconstructed translation unit
// Original: LEVELSELECT2.CPP
// Functions: 31 reconstructed functions
// Address range: 0x8007FC88 - 0x80081FCC
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

// Owner-local declarations attributed to LEVELSELECT2.cpp by MAIN.SYM.

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

#include "M3D.h"
#include "FRONTSCREENS.h"

#define FRONTEND2STATE_ABSOLUTE_DECLARATIONS
#define FRONTEND2STATE_DECLARE_GAME_MODE
#define FRONTEND2STATE_DECLARE_TWO_PLAYER
#include "FRONTEND2STATE.h"
#define GAMEFMVSTATE_ABSOLUTE_DECLARATION
#define GAMEFMVSTATE_DECLARE_END_X
#define GAMEFMVSTATE_DECLARE_START_X
#define GAMEFMVSTATE_DECLARE_START_Y
#include "GAMEFMVSTATE.h"
// LevelGenerator owns the shared GridCellWidth object.

#define SCREENTOOLS_AGGREGATE_LAYOUT
#define SCREENTOOLS_AGGREGATE_LAYOUT_ONLY
#include "SCREENTOOLS.h"
#undef SCREENTOOLS_AGGREGATE_LAYOUT_ONLY
#undef SCREENTOOLS_AGGREGATE_LAYOUT
#include "GAMEFMV.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include "globals.h"
#include "PSXREGION.h"
#include <LIBGPU.H>

struct GoalScreenElement;
struct GlobeScreenElement;
struct LevelNameScreenElement;
struct LevelSelectScreen;
struct NameScreenElement;
struct PromptScreenElement;
struct CareerScreenElement;
struct SkaterNameScreenElement;
struct SlicedImage2;
struct Sprite2;
struct VideoScreenElement;
#define SPOOLER_ABSOLUTE_MANAGER_DECLARATION
#include "SPOOLER.h"
#undef SPOOLER_ABSOLUTE_MANAGER_DECLARATION

#include "SCAMERA.h"

/** Ordering-table fields consumed from the active double buffer. */
struct LevelSelectDoubleBuffer {
    unsigned char pad_00[0x84];
    unsigned int *panelOrderingTable;
    unsigned int *globeOrderingTable;
    unsigned char pad_8c[4];
    unsigned short topY;
    unsigned short pad_92;
};

#include "SVIEWPORT.h"

#include "PAD.h"

/** Typed collaborators used by the level-selection owner. */
#include "DB.h"
#include "STATS.h"
#include "SPOOL.h"
#include "PAL.h"
#include "FRONTEND2.h"
#include "FILEIO.h"
#include "MEM.h"
#define M3DMATHS_TYPED_API
#include "M3DMATHS.h"
#undef M3DMATHS_TYPED_API
#include "IMAGE.h"
/** Typed construction, screen-hierarchy, font, and spooler entry points. */
#define SCREENTOOLS_DECLARE_SCREEN_ELEMENT_CONSTRUCTOR
#define SCREENTOOLS_DECLARE_SCREEN_ELEMENT_LIST_CONSTRUCTOR
#include "SCREENTOOLS.h"
#define LevelSelect_ConstructScreenElement __13ScreenElementiiii
#define LevelSelect_ConstructScreenElementList __17ScreenElementListiiii
#define SCREENTOOLS_DECLARE_SCREEN_ELEMENT_LIST_DESTRUCTOR
#include "SCREENTOOLS.h"
#define SCREENTOOLS_DECLARE_SCREEN_ELEMENT_DESTRUCTOR
#include "SCREENTOOLS.h"

#define SCREENTOOLS_DECLARE_TEXT_SCREEN_ELEMENT_CONSTRUCTOR
#include "SCREENTOOLS.h"
#define LevelSelect_ConstructTextScreenElement \
    __17TextScreenElementPcP4Fontiiii
#define SCREENTOOLS_DECLARE_TEXT_SCREEN_ELEMENT_DESTRUCTOR
#include "SCREENTOOLS.h"
extern "C" void draw__17TextScreenElementii(
    TextScreenElement *element, int x, int y);
#define LevelSelect_DrawTextScreenElement draw__17TextScreenElementii

#define SCREENTOOLS_DECLARE_SPRITE_SCREEN_ELEMENT_CONSTRUCTOR
#include "SCREENTOOLS.h"
#define LevelSelect_ConstructSpriteScreenElement \
    __19SpriteScreenElementPciiiiii
/** Typed aliases for constructors whose storage is allocated by CClass. */
namespace LevelSelectConstructorAbi
{
extern "C" LevelSelectScreen *__17LevelSelectScreen(
    LevelSelectScreen *screen);
extern "C" SkaterNameScreenElement *__23SkaterNameScreenElementbiiii(
    SkaterNameScreenElement *element, bool secondPlayer,
    int homeX, int homeY, int screenX, int screenY);
extern "C" GlobeScreenElement *__18GlobeScreenElementiiii(
    GlobeScreenElement *element,
    int homeX, int homeY, int screenX, int screenY);
extern "C" NameScreenElement *__17NameScreenElementPcP4Fontiiii(
    NameScreenElement *element, char *text, Font *font,
    int homeX, int homeY, int screenX, int screenY);
extern "C" LevelNameScreenElement *__22LevelNameScreenElementiiii(
    LevelNameScreenElement *element,
    int homeX, int homeY, int screenX, int screenY);
extern "C" VideoScreenElement *__18VideoScreenElementiiii(
    VideoScreenElement *element,
    int homeX, int homeY, int screenX, int screenY);
extern "C" GoalScreenElement *__17GoalScreenElementiiii(
    GoalScreenElement *element,
    int homeX, int homeY, int screenX, int screenY);
}

#define LevelSelect_ConstructLevelSelectScreen \
    LevelSelectConstructorAbi::__17LevelSelectScreen
#define LevelSelect_ConstructSkaterNameScreenElement \
    LevelSelectConstructorAbi::__23SkaterNameScreenElementbiiii
#define LevelSelect_ConstructGlobeScreenElement \
    LevelSelectConstructorAbi::__18GlobeScreenElementiiii
#define LevelSelect_ConstructNameScreenElement \
    LevelSelectConstructorAbi::__17NameScreenElementPcP4Fontiiii
extern "C" CareerScreenElement *__19CareerScreenElementP4Fontiiii(
    CareerScreenElement *element, Font *font,
    int homeX, int homeY, int screenX, int screenY);
#define LevelSelect_ConstructCareerScreenElement \
    __19CareerScreenElementP4Fontiiii
#define LevelSelect_ConstructLevelNameScreenElement \
    LevelSelectConstructorAbi::__22LevelNameScreenElementiiii
#define LevelSelect_ConstructVideoScreenElement \
    LevelSelectConstructorAbi::__18VideoScreenElementiiii
#define LevelSelect_ConstructGoalScreenElement \
    LevelSelectConstructorAbi::__17GoalScreenElementiiii

typedef char levelselect_item_size_must_be_36[
    (sizeof(Item) == 36) ? 1 : -1];
typedef char levelselect_button_size_must_be_16[
    (sizeof(SButton) == 16) ? 1 : -1];
typedef char levelselect_control_size_must_be_364[
    (sizeof(SControl) == 364) ? 1 : -1];
typedef char levelselect_double_buffer_size_must_be_148[
    (sizeof(LevelSelectDoubleBuffer) == 148) ? 1 : -1];
#include "SFX.h"
#include "M3DINIT.h"
#include "MAIN.h"
#include "RFRONT.h"
#include "PACK.h"
#include "SCREENTOOLS.h"
#include "UTILS.h"
#include <STDIO.H>

/** Placement construction for embedded polymorphic objects. */
inline void *operator new(unsigned int, void *address)
{
    return address;
}

// === Forward type declarations ===

#include "IMAGEBASE.h"

struct Slice;

/** Sliced image storage inherited by front-end sprites. */
struct SlicedImage2 : Image {
    Slice *SliceTab;
    short W;
    short H;
    short SliceW;
    short SliceH;
    short SliceCountX;
    short SliceCountY;
    void *InMemory;
    void *Packed;
    int BppShift;
    unsigned short Clut;

    void pack();
    void unpack();
    int screenWidth();
    int screenHeight();
};

/** Goal-screen sprite with the original Image/SlicedImage2 base layout. */
struct Sprite2 : SlicedImage2 {
    /** Allocate sprite storage through the shared CClass arena. */
    static void *operator new(unsigned int size) throw()
    {
        return ::CClass::operator new(size);
    }
    /** Build a sprite from a bitmap file. */
    Sprite2(char *name, int pack, int sliceWidth, int sliceHeight);
    /** Build a sprite from an in-memory bitmap. */
    Sprite2(unsigned char *data, int pack, int sliceWidth, int sliceHeight);
};

/** Original polymorphic class prefix used by Font assignment. */
struct LevelSelectClassAssignmentBase {
    virtual ~LevelSelectClassAssignmentBase();
};

/** Polymorphic view used to recover the original Font assignment semantics. */
struct LevelSelectFontAssignmentView : LevelSelectClassAssignmentBase {
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
    FontCharacter *CharTab;
    int NumChars;
    int Clut;
    TextEffect *CurrentTextEffect;
    unsigned char OldR;
    unsigned char OldG;
    unsigned char OldB;
};

/**
 * POD font storage embedded by SkaterNameScreenElement.
 *
 * The target destructor tears this Font down explicitly before its screen-list
 * base.  This non-polymorphic view preserves that recovered order without a
 * second compiler-generated subobject destructor.
 */
struct LevelSelectSkaterFontStorage {
    int *vtable;
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
    FontCharacter *CharTab;
    int NumChars;
    int Clut;
    TextEffect *CurrentTextEffect;
    unsigned char OldR;
    unsigned char OldG;
    unsigned char OldB;
    unsigned char Pad;
};

typedef char levelselect_skater_font_storage_size_must_be_80[
    (sizeof(LevelSelectSkaterFontStorage) == 80) ? 1 : -1];


typedef char levelselect_image_size_must_be_28[
    (sizeof(Image) == 28) ? 1 : -1];
typedef char levelselect_sliced_image_size_must_be_60[
    (sizeof(SlicedImage2) == 60) ? 1 : -1];
typedef char levelselect_sprite_size_must_be_60[
    (sizeof(Sprite2) == 60) ? 1 : -1];
typedef char levelselect_font_assignment_view_size_must_be_80[
    (sizeof(LevelSelectFontAssignmentView) == 80) ? 1 : -1];

/** Goal selector and its eight streamed goal-image sprites. */
struct GoalScreenElement {
    struct ScreenElement ScreenElement;
    int mW;
    int mH;
    int mState;
    int mLevel;
    int mLastLevel;
    int mTargetLevel;
    int mFreeLastCount;
    struct Sprite2 (*mpSprite[15]);
    int mSpoolID[15];
    int mGoalOffsX;
    int mGoalOffsY;
    int mLeavingX;
    int mLeavingDir;
    int mNextDir;

    GoalScreenElement(int, int, int, int);
    ~GoalScreenElement();
    void SetLevel(int level, int dir);
    void draw(int x, int y);
    void update();
};

/** Little-endian views used by the PSX texture and VRAM interfaces. */
union LevelSelectPackedInt {
    int value;
    u16 lowHalf;
    s16 lowSignedHalf;
    u8 lowByte;
};


/** Video preview, mask sprites, playback state, and textured quad. */
struct VideoScreenElement {
    struct ScreenElementList ScreenElementList;
    int mLevel;
    int mNewLevel;
    int mState;
    int mCount;
    LevelSelectPackedInt mVideoVramX;
    LevelSelectPackedInt mVideoVramY;
    LevelSelectPackedInt mVideoW;
    LevelSelectPackedInt mVideoH;
    POLY_FT4 *mVideoPoly;
    struct Sprite2 (*mVideoSprite[2]);
    int mVideoOffsX;
    int mVideoOffsY;

    VideoScreenElement(int homeX, int homeY, int screenX, int screenY);
    ~VideoScreenElement();
    void SetLevel(int level);
    void draw(int x, int y);
    void update();
};


/** Animated text row used by the level and city labels. */
struct NameScreenElement {
    struct TextScreenElement TextScreenElement;

    NameScreenElement(char *, Font *, int, int, int, int);
    ~NameScreenElement();
    void update();
};

#include "PROMPTSCREENELEMENT.h"

/** Career-medal row used by the level-select screen. */
struct CareerScreenElement {
    ScreenElementList ScreenElementList;
    ScreenElement *mpMedalIcons[3];
    int mDelay[9];
};


/** Composite park/city label with its two local fonts. */
struct LevelNameScreenElement {
    struct ScreenElementList ScreenElementList;
    struct TextScreenElement *mpPark;
    struct TextScreenElement *mpCity;
    FontStorage mFont1;
    FontStorage mFont2;
    int mOriginalHomeX;
    int mSpacing;
    int mLevel;
    int mTargetLevel;
    int mState;

    LevelNameScreenElement(int, int, int, int);
    ~LevelNameScreenElement();
    void SetLevel(int level);
    void update();
};


/** Globe preview and its crosshair overlay. */
struct GlobeScreenElement {
    struct ScreenElement ScreenElement;
    struct Item mGlobeItem;
    union {
        int mGlobeRegion;
        unsigned char mGlobeRegionByte;
    };
    struct Sprite2 *mpCrosshair;
    int mCrossY;
    int mTargetCrossY;
    union {
        int mGlobeRot;
        u16 mGlobeRotLowHalf;
    };
    int mTargetGlobeRot;

    GlobeScreenElement(int, int, int, int);
    ~GlobeScreenElement();
    void SetLevel(int level);
    void draw(int x, int y);
    void update();
};

/** Complete level-selection screen and its child elements. */
struct LevelSelectScreen {
    struct ScreenElementList ScreenElementList;
    struct LevelNameScreenElement *mpLevelName;
    struct GlobeScreenElement *mpGlobe;
    struct VideoScreenElement *mpVideo;
    struct GoalScreenElement *mpGoal;
    struct Sprite2 *mBackground;
    FontStorage mFont1;
    FontStorage mFont2;

    LevelSelectScreen();
    ~LevelSelectScreen();
    void draw();
    void update();
    void refresh();
    int isDone(int *return_value);
    /** Release level-select storage through the shared CClass arena. */
    static void operator delete(void *p) { ::CClass::operator delete(p); }
};

/** One- or two-player skater portrait/name row. */
struct SkaterNameScreenElement {
    struct ScreenElementList ScreenElementList;
    struct SpriteScreenElement *mpFace;
    struct TextScreenElement *mpName;
    LevelSelectSkaterFontStorage mFont;

    SkaterNameScreenElement(bool secondPlayer,
                            int homeX, int homeY, int screenX, int screenY);
    ~SkaterNameScreenElement();
    /** Release skater-name storage through the shared CClass arena. */
    static void operator delete(void *p) { ::CClass::operator delete(p); }
};

typedef char levelselect_goal_element_size_must_be_204[
    (sizeof(GoalScreenElement) == 204) ? 1 : -1];
typedef char levelselect_video_element_size_must_be_156[
    (sizeof(VideoScreenElement) == 156) ? 1 : -1];
typedef char levelselect_packed_int_size_must_be_4[
    (sizeof(LevelSelectPackedInt) == 4) ? 1 : -1];
typedef char levelselect_name_element_size_must_be_44[
    (sizeof(NameScreenElement) == 44) ? 1 : -1];
typedef char levelselect_prompt_element_size_must_be_104[
    (sizeof(PromptScreenElement) == 104) ? 1 : -1];
typedef char levelselect_career_element_size_must_be_152[
    (sizeof(CareerScreenElement) == 152) ? 1 : -1];
typedef char levelselect_level_name_element_size_must_be_292[
    (sizeof(LevelNameScreenElement) == 292) ? 1 : -1];
typedef char levelselect_globe_element_size_must_be_96[
    (sizeof(GlobeScreenElement) == 96) ? 1 : -1];
typedef char levelselect_screen_size_must_be_284[
    (sizeof(LevelSelectScreen) == 284) ? 1 : -1];
typedef char levelselect_skater_name_size_must_be_192[
    (sizeof(SkaterNameScreenElement) == 192) ? 1 : -1];

enum {
    LevelSelectLevelCount = 8,
};

enum LevelSelectAbsoluteAddress {
    LevelSelectDoubleBufferAddress = 0x800DA338,
    LevelSelectSpoolerAddress = 0x800DA694,
};

/**
 * Globe preview viewport and the original small-data pointer that owns it.
 */
static SViewport CreateViewport1 = {
    342, 120, 171, 0,
    10, 30000, 0xD55, 0,
    0, 0,
};
static SViewport *pViewport = &CreateViewport1;

/**
 * Level selected by the front-end globe.
 *
 * The original owner keeps this zero-initialized word between the viewport
 * pointer and its filename pool in `.sdata`.
 */
static int SelectedLevel __attribute__((section(".sdata"))) = 0;

// === External object declarations ===
#include "PDOUBLEBUFFER.h"

// === Resource-name address views ===
/*
 * Keep incomplete address views only for owners whose complete extent is at
 * most the translation unit's -G8 threshold.  Larger arrays are addressed
 * directly through their complete owner definitions below; both forms retain
 * the target's small-data versus absolute-address split.
 */
extern char s_select_800dbe24[];
extern char s_accept_800dbe3c[];
extern char DAT_800dbe54[];
extern char s_globe_800dbe6c[];
extern char s_mar_bmp_800dbe84[];

/**
 * Owner-local font and image-name storage in its original `.sdata` order.
 *
 * Owners larger than eight bytes are used directly through this namespace;
 * the five smaller owners retain incomplete external address views above so
 * GCC preserves their target small-data addressing.  C linkage preserves the
 * natural linker identifiers without source-level assembly aliases.
 */
namespace LevelSelectAddressViewOwners
{
extern "C"
{
static char s_mainf_fnt_800dbdd8[]
    __attribute__((section(".sdata"), aligned(4))) = "mainf.fnt";
static char s_s2font2_fnt_800dbde4[]
    __attribute__((section(".sdata"), aligned(4))) = "s2font2.fnt";
static char s_s2font1_fnt_800dbdf0[]
    __attribute__((section(".sdata"), aligned(4))) = "s2font1.fnt";
static char s_ingame1_fnt_800dbdfc[]
    __attribute__((section(".sdata"), aligned(4))) = "ingame1.fnt";
static char s_s2bgd3_bmp_800dbe08[]
    __attribute__((section(".sdata"), aligned(4))) = "s2bgd3.bmp";
static char s_s2prmpt4_bmp_800dbe14[]
    __attribute__((section(".sdata"), aligned(4))) = "s2prmpt4.bmp";
static char s_select_800dbe24[]
    __attribute__((section(".sdata"), aligned(4))) = "select";
static char s_s2prmpt1_bmp_800dbe2c[]
    __attribute__((section(".sdata"), aligned(4))) = "s2prmpt1.bmp";
static char s_accept_800dbe3c[]
    __attribute__((section(".sdata"), aligned(4))) = "accept";
static char s_s2prmpt2_bmp_800dbe44[]
    __attribute__((section(".sdata"), aligned(4))) = "s2prmpt2.bmp";
static char DAT_800dbe54[]
    __attribute__((section(".sdata"), aligned(4))) = "back";
static char s_s2versus_bmp_800dbe5c[]
    __attribute__((section(".sdata"), aligned(4))) = "s2versus.bmp";
static char s_globe_800dbe6c[]
    __attribute__((section(".sdata"), aligned(4))) = "globe";
static char s_s2crossh_bmp_800dbe74[]
    __attribute__((section(".sdata"), aligned(4))) = "s2crossh.bmp";
static char s_mar_bmp_800dbe84[]
    __attribute__((section(".sdata"), aligned(4))) = "mar.bmp";
static char s_levna_bmp_800dbe8c[]
    __attribute__((section(".sdata"), aligned(4))) = "levna.bmp";
static char s_s2win5_bmp_800dbe98[]
    __attribute__((section(".sdata"), aligned(4))) = "s2win5.bmp";
static char s_s2levl00_bmp_800dbea4[]
    __attribute__((section(".sdata"), aligned(4))) = "s2levl00.bmp";
static char s_s2levl0_d_bmp_800dbeb4[]
    __attribute__((section(".sdata"), aligned(4))) = "s2levl0%d.bmp";
}
}
// === Additional extern declarations for destructors ===
typedef void (*LevelSelectVirtualFunction)(void);
#define FRONTSCREENS_DECLARE_LEVELSELECT_DESTRUCTOR_ABI
#include "FRONTSCREENS.h"
#define FRONTSCREENS_DECLARE_LEVELSELECT_OWNER_VTABLES
#include "FRONTSCREENS.h"

/**
 * Dispatch tables owned by the level-selection front end.
 *
 * The trailing null in each table is part of the original GCC 2 layout.  The
 * compiler-generated VideoScreenElement switch table follows these seven
 * tables in the same read-only section.
 */
LevelSelectVirtualFunction const __vt_17GoalScreenElement[]
    __attribute__((aligned(8))) = {
        0,
        0,
        (LevelSelectVirtualFunction)LevelSelectGoalDestructorVtable,
        (LevelSelectVirtualFunction)(void (*)(GoalScreenElement *))
            &GoalScreenElement::update,
        (LevelSelectVirtualFunction)(void (*)(GoalScreenElement *, int, int))
            &GoalScreenElement::draw,
        (LevelSelectVirtualFunction)&ScreenElement::width,
        (LevelSelectVirtualFunction)&ScreenElement::refresh,
        0,
    };

LevelSelectVirtualFunction const __vt_18VideoScreenElement[]
    __attribute__((aligned(8))) = {
        0,
        0,
        (LevelSelectVirtualFunction)LevelSelectVideoDestructorVtable,
        (LevelSelectVirtualFunction)(void (*)(VideoScreenElement *))
            &VideoScreenElement::update,
        (LevelSelectVirtualFunction)(void (*)(VideoScreenElement *, int, int))
            &VideoScreenElement::draw,
        (LevelSelectVirtualFunction)&ScreenElement::width,
        (LevelSelectVirtualFunction)&ScreenElement::refresh,
        0,
    };

LevelSelectVirtualFunction const __vt_17NameScreenElement[]
    __attribute__((aligned(8))) = {
        0,
        0,
        (LevelSelectVirtualFunction)LevelSelectNameDestructorVtable,
        (LevelSelectVirtualFunction)(void (*)(NameScreenElement *))
            &NameScreenElement::update,
        (LevelSelectVirtualFunction)LevelSelect_DrawTextScreenElement,
        (LevelSelectVirtualFunction)&TextScreenElement::width,
        (LevelSelectVirtualFunction)&ScreenElement::refresh,
        0,
    };

LevelSelectVirtualFunction const __vt_22LevelNameScreenElement[]
    __attribute__((aligned(8))) = {
        0,
        0,
        (LevelSelectVirtualFunction)LevelSelectLevelNameDestructorVtable,
        (LevelSelectVirtualFunction)(void (*)(LevelNameScreenElement *))
            &LevelNameScreenElement::update,
        (LevelSelectVirtualFunction)(void (*)(ScreenElementList *, int, int))
            &ScreenElementList::draw,
        (LevelSelectVirtualFunction)&ScreenElement::width,
        (LevelSelectVirtualFunction)&ScreenElement::refresh,
        0,
    };

LevelSelectVirtualFunction const __vt_18GlobeScreenElement[]
    __attribute__((aligned(8))) = {
        0,
        0,
        (LevelSelectVirtualFunction)LevelSelectGlobeDestructorVtable,
        (LevelSelectVirtualFunction)(void (*)(GlobeScreenElement *))
            &GlobeScreenElement::update,
        (LevelSelectVirtualFunction)(void (*)(GlobeScreenElement *, int, int))
            &GlobeScreenElement::draw,
        (LevelSelectVirtualFunction)&ScreenElement::width,
        (LevelSelectVirtualFunction)&ScreenElement::refresh,
        0,
    };

LevelSelectVirtualFunction const __vt_23SkaterNameScreenElement[]
    __attribute__((aligned(8))) = {
        0,
        0,
        (LevelSelectVirtualFunction)SkaterNameScreenElementDestructorVtable,
        (LevelSelectVirtualFunction)(void (*)(ScreenElementList *))
            &ScreenElementList::update,
        (LevelSelectVirtualFunction)(void (*)(ScreenElementList *, int, int))
            &ScreenElementList::draw,
        (LevelSelectVirtualFunction)&ScreenElement::width,
        (LevelSelectVirtualFunction)&ScreenElement::refresh,
        0,
    };

LevelSelectVirtualFunction const __vt_17LevelSelectScreen[]
    __attribute__((aligned(8))) = {
        0,
        0,
        (LevelSelectVirtualFunction)LevelSelectScreenDestructorVtable,
        (LevelSelectVirtualFunction)(void (*)(LevelSelectScreen *))
            &LevelSelectScreen::update,
        (LevelSelectVirtualFunction)(void (*)(ScreenElementList *, int, int))
            &ScreenElementList::draw,
        (LevelSelectVirtualFunction)&ScreenElement::width,
        (LevelSelectVirtualFunction)(void (*)(LevelSelectScreen *))
            &LevelSelectScreen::refresh,
        0,
    };

/** Invoke the deleting-destructor slot shared by CClass-derived objects. */
static inline void LevelSelect_DeleteVirtual(void *object)
{
    if (object != 0)
    {
        CClass *base = (CClass *)object;
        typedef void (*DeletingDestructor)(void *, int);
        ((DeletingDestructor)base->vtable[2])(object, 3);
    }
}

/** Invoke the draw slot shared by the front-end screen hierarchy. */
static inline void LevelSelect_DrawVirtual(void *object,
                                           int x, int y, int flags)
{
    CClass *base = (CClass *)object;
    typedef void (*DrawFunction)(void *, int, int, int);
    ((DrawFunction)base->vtable[5])(object, x, y, flags);
}

/** Read the low halfword of an integer-backed PSX screen coordinate. */
static inline u16 LevelSelect_LowHalf(const int &value)
{
    return *reinterpret_cast<const u16 *>(&value);
}

// === Function implementations (address order) ===

/**
 * LevelSelect2_Main
 *
 * Address:   0x8007FC88
 * Mangled:   LevelSelect2_Main__Fv
 * Demangled: LevelSelect2_Main(void)
 *
 * @return 0
 */
int LevelSelect2_Main(void)
{
    unsigned int oldSkyColor = Db_SkyColor;
    Db_SkyColor = 0;
    Db_UpdateSky();
    Spool_RemoveUnusedTextures();
    Pal_RemoveUnusedPalettes();
    Pack_DefragmentFully();
    FontManager::LoadFont(
        LevelSelectAddressViewOwners::s_mainf_fnt_800dbdd8);
    FontManager::LoadFont(
        LevelSelectAddressViewOwners::s_s2font2_fnt_800dbde4);
    FontManager::LoadFont(
        LevelSelectAddressViewOwners::s_s2font1_fnt_800dbdf0);
    FontManager::LoadFont(
        LevelSelectAddressViewOwners::s_ingame1_fnt_800dbdfc);

    LevelSelectScreen *pScreen =
        (LevelSelectScreen *)::CClass::operator new(sizeof(LevelSelectScreen));
    if (pScreen != 0)
    {
        pScreen = LevelSelect_ConstructLevelSelectScreen(pScreen);
    }

    Pad_ClearTriggers(&Pad[0]);
    Pad_ClearTriggers(&Pad[1]);

    int return_value = -1;
    while (1)
    {
        FrontEnd2_UpdateMusicLoop();
        if (pScreen->isDone(&return_value) != 0)
        {
            break;
        }
        GStartTime = Vblanks;
        Db_FlipClear();
        CalcPolyBufferEnd();
        Pad_Update();

        LevelSelectVirtualFunction fn =
            ((LevelSelectVirtualFunction *)pScreen->ScreenElementList
                 .ScreenElement.CClass.vtable)[3];
        ((void (*)(LevelSelectScreen *))fn)(pScreen);
        pScreen->draw();
        DrawSync(0);
        Pause(1);
        while ((unsigned int)Vblanks <
               (unsigned int)(GStartTime + 2))
        {
        }
        Db_DisplayDraw();
        PollHost();
    }

    FontManager::UnloadFont(
        LevelSelectAddressViewOwners::s_mainf_fnt_800dbdd8);
    FontManager::UnloadFont(
        LevelSelectAddressViewOwners::s_s2font2_fnt_800dbde4);
    FontManager::UnloadFont(
        LevelSelectAddressViewOwners::s_s2font1_fnt_800dbdf0);
    FontManager::UnloadFont(
        LevelSelectAddressViewOwners::s_ingame1_fnt_800dbdfc);
    Db_SkyColor = oldSkyColor;

    LevelSelect_DeleteVirtual(pScreen);

    return return_value;
}

/**
 * __17LevelSelectScreen
 *
 * Address:   0x8007FE74
 * Mangled:   __17LevelSelectScreen
 * Demangled: LevelSelectScreen::(void)
 *
 * @return `this` pointer (GCC 2.x ctor convention).
 */
LevelSelectScreen::LevelSelectScreen()
{
    LevelSelect_ConstructScreenElementList(
        &ScreenElementList, 0, 0, -1, -1);
    ScreenElementList.ScreenElement.CClass.vtable =
        (int *)__vt_17LevelSelectScreen;
    ::new (FontFromStorage(&mFont1)) Font;
    ::new (FontFromStorage(&mFont2)) Font;

    Sprite2 *sprite = new Sprite2(
        LevelSelectAddressViewOwners::s_s2bgd3_bmp_800dbe08,
        1, 0, 0);
    mBackground = sprite;

    FontManager::GetFont(
        LevelSelectAddressViewOwners::s_mainf_fnt_800dbdd8);
    Font *sourceFont = FontManager::GetFont(
        LevelSelectAddressViewOwners::s_s2font2_fnt_800dbde4);
    *(LevelSelectFontAssignmentView *)&mFont2 =
        *(LevelSelectFontAssignmentView *)sourceFont;

    mFont2.JustX = 0;
    mFont2.JustY = 3;
    mFont2.R = 0x80;
    mFont2.G = 0x80;
    mFont2.B = 0x80;
    mFont2.Shadow = 0;
    mFont2.SpaceX = 0;
    mFont2.SpaceY = 0;
    mFont2.Scale = 0x1000;

    PromptScreenElement *pPromptElement[3];
    PromptScreenElement *item = new PromptScreenElement(
        LevelSelectAddressViewOwners::s_s2prmpt4_bmp_800dbe14,
        s_select_800dbe24,
        FontFromStorage(&mFont2), 115, 220, -1, 260);
    pPromptElement[0] = item;

    item = new PromptScreenElement(
        LevelSelectAddressViewOwners::s_s2prmpt1_bmp_800dbe2c,
        s_accept_800dbe3c,
        FontFromStorage(&mFont2), 228, 220, -1, 260);
    pPromptElement[1] = item;

    item = new PromptScreenElement(
        LevelSelectAddressViewOwners::s_s2prmpt2_bmp_800dbe44,
        DAT_800dbe54,
        FontFromStorage(&mFont2), 333, 220, -1, 260);
    pPromptElement[2] = item;

    int i = 0;
    do
    {
        ScreenElementList.addItem(
            &pPromptElement[i]->ScreenElementList.ScreenElement);
        i++;
    } while (i < 3);

    if (FrontEnd2_GameMode == 1)
    {
        CareerScreenElement *item = (CareerScreenElement *)
            ::CClass::operator new(sizeof(CareerScreenElement));
        if (item != 0)
        {
            item = LevelSelect_ConstructCareerScreenElement(
                item, FontFromStorage(&mFont2), 244, 133, 512, 133);
        }
        ScreenElementList.addItem(
            &item->ScreenElementList.ScreenElement);
    }

    if (FrontEnd2_GameMode == 1)
    {
        SkaterNameScreenElement *item2 = (SkaterNameScreenElement *)
            ::CClass::operator new(sizeof(SkaterNameScreenElement));
        if (item2 != 0)
        {
            item2 = LevelSelect_ConstructSkaterNameScreenElement(
                item2, false, 427, 119, 640, 119);
        }
        ScreenElementList.addItem((ScreenElement *)item2);
    }
    else if (FrontEnd2_TwoPlayer != 0)
    {
        SkaterNameScreenElement *item2 = (SkaterNameScreenElement *)
            ::CClass::operator new(sizeof(SkaterNameScreenElement));
        if (item2 != 0)
        {
            item2 = LevelSelect_ConstructSkaterNameScreenElement(
                item2, false, 427, 126, 640, -1);
        }
        ScreenElementList.addItem((ScreenElement *)item2);

        item2 = (SkaterNameScreenElement *)
            ::CClass::operator new(sizeof(SkaterNameScreenElement));
        if (item2 != 0)
        {
            item2 = LevelSelect_ConstructSkaterNameScreenElement(
                item2, true, 427, 180, 640, -1);
        }
        ScreenElementList.addItem((ScreenElement *)item2);
    }
    else
    {
        SkaterNameScreenElement *item2 = (SkaterNameScreenElement *)
            ::CClass::operator new(sizeof(SkaterNameScreenElement));
        if (item2 != 0)
        {
            item2 = LevelSelect_ConstructSkaterNameScreenElement(
                item2, false, 427, 180, 640, 180);
        }
        ScreenElementList.addItem((ScreenElement *)item2);
    }

    {
        GlobeScreenElement *item = (GlobeScreenElement *)
            ::CClass::operator new(sizeof(GlobeScreenElement));
        if (item != 0)
        {
            item = LevelSelect_ConstructGlobeScreenElement(
                item, 367, 70, -1, -1);
        }
        mpGlobe = item;
        ScreenElementList.addItem((ScreenElement *)item);
    }

    {
        LevelNameScreenElement *item = (LevelNameScreenElement *)
            ::CClass::operator new(sizeof(LevelNameScreenElement));
        if (item != 0)
        {
            item = LevelSelect_ConstructLevelNameScreenElement(
                item, 56, 35, -1, -1);
        }
        mpLevelName = item;
        ScreenElementList.addItem((ScreenElement *)item);
    }

    {
        VideoScreenElement *item = (VideoScreenElement *)
            ::CClass::operator new(sizeof(VideoScreenElement));
        if (item != 0)
        {
            item = LevelSelect_ConstructVideoScreenElement(
                item, 61, 126, -128, 126);
        }
        mpVideo = item;
        ScreenElementList.addItem((ScreenElement *)item);
    }

    {
        GoalScreenElement *item = (GoalScreenElement *)
            ::CClass::operator new(sizeof(GoalScreenElement));
        if (item != 0)
        {
            item = LevelSelect_ConstructGoalScreenElement(
                item, 65, 46, -1, -1);
        }
        mpGoal = item;
        ScreenElementList.addItem((ScreenElement *)item);
    }
}

/**
 * LevelSelectScreen::~LevelSelectScreen()
 *
 * Address:   0x800802DC
 * Mangled:   _._17LevelSelectScreen
 * Demangled: LevelSelectScreen::~LevelSelectScreen()
 *
 */
LevelSelectScreen::~LevelSelectScreen()
{
    ScreenElementList.ScreenElement.CClass.vtable =
        (int *)__vt_17LevelSelectScreen;
    LevelSelect_DeleteVirtual(mBackground);
    FontFromStorage(&mFont2)->~Font();
    FontFromStorage(&mFont1)->~Font();
    ScreenElementList_DestroyAbi(&ScreenElementList, 0);
}

/**
 * LevelSelectScreen::draw
 *
 * Address:   0x8008036C
 * Mangled:   draw__17LevelSelectScreen
 * Demangled: LevelSelectScreen::draw(void)
 *
 */
void LevelSelectScreen::draw()
{
    ScreenElementList.draw(0, 0);
    LevelSelect_DrawVirtual(mBackground, 0, 0, 1);
}

/**
 * LevelSelectScreen::update
 *
 * Address:   0x800803B8
 * Mangled:   update__17LevelSelectScreen
 * Demangled: LevelSelectScreen::update(void)
 *            Advance the level-selection cursor and propagate the chosen level to child
 *            screen elements whenever the left/right pad flags fire.
 *
 */
void LevelSelectScreen::update()
{
    if (Pad[0].Left.Triggered != 0)
    {
        SFX_PlayX(0xEA, 0x2000, 0);
        Pad[0].Left.Triggered = 0;
        SelectedLevel--;
        if (SelectedLevel < 0)
        {
            SelectedLevel = LevelSelectLevelCount - 1;
        }

        mpLevelName->SetLevel(SelectedLevel);
        mpGlobe->SetLevel(SelectedLevel);
        mpVideo->SetLevel(SelectedLevel);
        mpGoal->SetLevel(SelectedLevel, 1);
    }

    if (Pad[0].Right.Triggered != 0)
    {
        SFX_PlayX(0xEA, 0x2000, 0);
        Pad[0].Right.Triggered = 0;
        SelectedLevel++;
        if (SelectedLevel >= LevelSelectLevelCount)
        {
            SelectedLevel = 0;
        }

        mpLevelName->SetLevel(SelectedLevel);
        mpGlobe->SetLevel(SelectedLevel);
        mpVideo->SetLevel(SelectedLevel);
        mpGoal->SetLevel(SelectedLevel, 0);
    }

    ScreenElementList.update();
}

/**
 * LevelSelectScreen::refresh
 *
 * Address:   0x800804EC
 * Mangled:   refresh__17LevelSelectScreen
 * Demangled: LevelSelectScreen::refresh(void)
 *
 */
void LevelSelectScreen::refresh()
{
}

/**
 * LevelSelectScreen::isDone
 *
 * Address:   0x800804F4
 * Mangled:   isDone__17LevelSelectScreenPi
 * Demangled: LevelSelectScreen::isDone(int *)
 *
 * @param return_value Receives the selected level, or -1 when backing out.
 * @return Nonzero once the screen has accepted a selection or back command.
 */
int LevelSelectScreen::isDone(int *return_value)
{
    int v;
    int ret;
    if (Pad[0].X.Triggered != 0 || Pad[0].Start.Triggered != 0)
    {
        Pad[0].Start.Triggered = 0;
        Pad[0].X.Triggered = 0;
        int sel = SelectedLevel;
        signed char field = Levels[sel].STR;
        if (field != -27)
        {
            SFX_PlayX(0x92, 0x2000, 0);
            return 0;
        }
        SFX_PlayX(0x91, 0x2000, 0);
        v = SelectedLevel;
        ret = 1;
        goto done;
    }
    if (Pad[0].Triangle.Triggered == 0)
    {
        return 0;
    }
    SFX_PlayX(0x93, 0x2000, 0);
    ret = 1;
    // Preserve the joined-path return lifetime emitted by the original GCC 2.
    asm volatile("" : "+r"(ret));
    v = -1;
    Pad[0].Triangle.Triggered = 0;
done:
    *return_value = v;
    return ret;
}

/**
 * SkaterNameScreenElement constructor
 *
 * Address:   0x800805E0
 * Mangled:   __23SkaterNameScreenElementbiiii
 * Demangled: SkaterNameScreenElement::(bool, int, int, int, int)
 *
 * @return `this` pointer (GCC 2.x ctor convention).
 */
SkaterNameScreenElement::SkaterNameScreenElement(
    bool secondPlayer,
    int homeX, int homeY, int screenX, int screenY)
{
    LevelSelect_ConstructScreenElementList(
        &ScreenElementList, homeX, homeY, screenX, screenY);

    Font *font = (Font *)&mFont;
    ScreenElementList.ScreenElement.CClass.vtable =
        (int *)__vt_23SkaterNameScreenElement;
    ::new (font) Font;

    Font *sourceFont = FontManager::GetFont(
        LevelSelectAddressViewOwners::s_s2font1_fnt_800dbdf0);
    *(LevelSelectFontAssignmentView *)&mFont =
        *(LevelSelectFontAssignmentView *)sourceFont;

    mFont.JustX = 2;
    mFont.JustY = 2;
    mFont.R = 0x80;
    mFont.G = 0x80;
    mFont.B = 0x80;
    mFont.Shadow = 0;
    mFont.SpaceX = 0;
    mFont.SpaceY = 0;
    mFont.Scale = 0x1000;

    if (secondPlayer)
    {
        SpriteScreenElement *pElement = (SpriteScreenElement *)
            ::CClass::operator new(sizeof(SpriteScreenElement));
        if (pElement != 0)
        {
            pElement = LevelSelect_ConstructSpriteScreenElement(
                pElement,
                Stats[SelectedSkater2].PortraitName,
                0, -32, -1, -1, 0, 0);
        }
        mpFace = pElement;

        TextScreenElement *name = (TextScreenElement *)
            ::CClass::operator new(sizeof(TextScreenElement));
        if (name != 0)
        {
            name = LevelSelect_ConstructTextScreenElement(
                name,
                Stats[SelectedSkater2].Name,
                font, -2, 17, -2, -30);
        }
        mpName = name;

        SpriteScreenElement *versus = (SpriteScreenElement *)
            ::CClass::operator new(sizeof(SpriteScreenElement));
        if (versus != 0)
        {
            versus = LevelSelect_ConstructSpriteScreenElement(
                versus,
                LevelSelectAddressViewOwners::s_s2versus_bmp_800dbe5c,
                -76, -36, -1, -1, 0, 0);
        }
        ScreenElementList.addItem((ScreenElement *)versus);
    }
    else
    {
        SpriteScreenElement *pElement = (SpriteScreenElement *)
            ::CClass::operator new(sizeof(SpriteScreenElement));
        if (pElement != 0)
        {
            pElement = LevelSelect_ConstructSpriteScreenElement(
                pElement,
                Stats[SelectedSkater].PortraitName,
                0, -32, -1, -1, 0, 0);
        }
        mpFace = pElement;

        TextScreenElement *name = (TextScreenElement *)
            ::CClass::operator new(sizeof(TextScreenElement));
        if (name != 0)
        {
            name = LevelSelect_ConstructTextScreenElement(
                name,
                Stats[SelectedSkater].Name,
                font, -2, 17, -2, -30);
        }
        mpName = name;
    }

    ScreenElementList.addItem(&mpFace->ScreenElement);
    ScreenElementList.addItem(&mpName->ScreenElement);
}

/**
 * SkaterNameScreenElement::~SkaterNameScreenElement()
 *
 * Address:   0x800808D8
 * Mangled:   _._23SkaterNameScreenElement
 * Demangled: SkaterNameScreenElement::~SkaterNameScreenElement()
 *
 */
SkaterNameScreenElement::~SkaterNameScreenElement()
{
    ScreenElementList.ScreenElement.CClass.vtable =
        (int *)__vt_23SkaterNameScreenElement;
    reinterpret_cast<Font *>(&mFont)->~Font();
    ScreenElementList_DestroyAbi(&ScreenElementList, 0);
}

/**
 * GlobeScreenElement constructor
 *
 * Address:   0x8008093C
 * Mangled:   __18GlobeScreenElementiiii
 * Demangled: GlobeScreenElement::(int, int, int, int)
 *
 */
GlobeScreenElement::GlobeScreenElement(
    int homeX, int homeY, int screenX, int screenY)
{
    LevelSelect_ConstructScreenElement(
        &ScreenElement,
        homeX, homeY, screenX, screenY);
    ScreenElement.CClass.vtable = (int *)__vt_18GlobeScreenElement;

    mGlobeRegion = Spool_PSX(s_globe_800dbe6c, 0, 0, 1);
    Spool_Sync();

    Item *pItemList = &mGlobeItem;
    pItemList->Flags = 0;
    pItemList->Inquiry = 0;
    pItemList->Position.vx = 0;
    pItemList->Position.vy = 0;
    pItemList->Position.vz = -0x50000;
    pItemList->Angles.vx = 0;
    pItemList->Angles.vy = 0;
    pItemList->Angles.vz = 0;
    pItemList->Frame = 0;
    pItemList->TintIndex = 0;
    pItemList->Anim = 0;
    pItemList->Model = 0;
    pItemList->Region = mGlobeRegionByte;
    pItemList->RGB = 0xFF00;
    pItemList->NextItem = 0;

    M3dMaths_SetIdentityRotation(
        &MikeCamera->Transform);
    MikeCamera->Position.vx = 0;
    MikeCamera->Position.vy = 0;
    MikeCamera->Position.vz = -300;
    TransMatrix(
        &MikeCamera->Transform,
        &MikeCamera->Position);
    M3dInit_SetFoggingParams(10, 6000, 0x800);
    M3dInit_InitAtStart();

    Sprite2 *crosshair = new Sprite2(
        LevelSelectAddressViewOwners::s_s2crossh_bmp_800dbe74,
        1, 0, 0);
    mpCrosshair = crosshair;
    crosshair->JustX = 1;
    mpCrosshair->JustY = 1;

    int level = SelectedLevel;
    mGlobeRot = Levels[level].map_loc[0];
    mCrossY = Levels[level].map_loc[1];
    SetLevel(level);
}

/**
 * GlobeScreenElement destructor
 *
 * Address:   0x80080AB0
 * Mangled:   _._18GlobeScreenElement
 * Demangled: GlobeScreenElement::~GlobeScreenElement()
 *
 */
GlobeScreenElement::~GlobeScreenElement()
{
    ScreenElement.CClass.vtable = (int *)__vt_18GlobeScreenElement;
    ClearRegion(mGlobeRegion, 1, 0);
    PSXRegion[mGlobeRegion].Filename[0] = 0;
    LevelSelect_DeleteVirtual(mpCrosshair);
    ScreenElement_DestroyAbi(&ScreenElement, 0);

}

/**
 *
 * Address:   0x80080B58
 * Mangled:   update__18GlobeScreenElement
 * Demangled: GlobeScreenElement::update(void)
 *            Ease the globe rotation and crosshair toward their selected-level targets.
 *
 */
void GlobeScreenElement::update()
{
    // The target keeps the receiver in a2 across the complete easing update.
    register GlobeScreenElement *self asm("$6") = this;
    asm volatile("" : "+r"(self));
    int diff = self->mTargetGlobeRot - self->mGlobeRot;

    self->mGlobeItem.Angles.vy = (short)self->mGlobeRotLowHalf;

    if (diff >= 0x801)
    {
        self->mTargetGlobeRot = self->mTargetGlobeRot - 0x1000;
        diff -= 0x1000;
    }
    else if (diff < -0x800)
    {
        self->mTargetGlobeRot = self->mTargetGlobeRot + 0x1000;
        diff += 0x1000;
    }

    if ((unsigned int)(diff + 3) < 7)
    {
        self->mGlobeRot = self->mTargetGlobeRot;
    }

    int currentRotation = self->mGlobeRot + (diff >> 2);
    self->mGlobeRot = currentRotation;
    if (currentRotation < 0)
    {
        self->mGlobeRot = currentRotation + 0x1000;
        self->mTargetGlobeRot = self->mTargetGlobeRot + 0x1000;
    }

    currentRotation = self->mGlobeRot;
    if (currentRotation >= 0x1000)
    {
        self->mGlobeRot = currentRotation - 0x1000;
        self->mTargetGlobeRot = self->mTargetGlobeRot - 0x1000;
    }

    int crossY = self->mCrossY;
    int targetCrossY = self->mTargetCrossY;
    int crossDelta = targetCrossY - crossY;
    if ((unsigned int)(crossDelta + 3) < 7)
    {
        self->mCrossY = targetCrossY;
        crossY = targetCrossY;
    }
    int crossStep = crossDelta >> 2;
    self->mCrossY = crossY + crossStep;
    self->ScreenElement.update();
}

/**
 * GlobeScreenElement::draw
 *
 * Address:   0x80080C54
 * Mangled:   draw__18GlobeScreenElementii
 * Demangled: GlobeScreenElement::draw(int, int)
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 */
void GlobeScreenElement::draw(int x, int y)
{
    SViewport *vp = pViewport;

    u16 cx1 = LevelSelect_LowHalf(ScreenElement.mScreenX);
    vp->xL = (s16)(cx1 + x - 65);

    u16 cy1 = LevelSelect_LowHalf(ScreenElement.mScreenY);
    vp->yT = (s16)(cy1 + y - 40);

    u16 cx2 = LevelSelect_LowHalf(ScreenElement.mScreenX);
    vp->xR = (s16)(cx2 + x + 65);

    u16 cy2 = LevelSelect_LowHalf(ScreenElement.mScreenY);
    LevelSelectDoubleBuffer *db =
        reinterpret_cast<LevelSelectDoubleBuffer *>(pDoubleBuffer);
    vp->yB = (s16)(cy2 + y + 40);

    unsigned int *ot = db->globeOrderingTable;
    M3d_RenderSetup(MikeCamera, vp, ot);
    M3d_Render(&mGlobeItem);
    M3d_RenderCleanup();
    Sprite2 *obj = mpCrosshair;
    int a1 = ScreenElement.mScreenX;
    int a2 = ScreenElement.mScreenY;
    int crossY = mCrossY;
    CClass *base = (CClass *)obj;
    int *vtable = base->vtable;
    typedef void (*DrawFunction)(Sprite2 *, int, int, int);
    ((DrawFunction)vtable[5])(obj, a1 + x, a2 + crossY + y, 0);
}

/**
 * Looks up the level entry in GlobeLevelTable and copies the globe element fields.
 *
 * Address:   0x80080D2C, Size: 56 bytes (14 instructions)
 * Mangled:   SetLevel__18GlobeScreenElementi
 * Demangled: GlobeScreenElement::SetLevel(int)
 *            Assembly:
 *            lui $a2, 0x800b             ; table base
 *            addiu $a2, $a2, 0x6764
 *            sll $v0, $a1, 3             ; level*8
 *            subu $v0, $v0, $a1          ; level*7
 *            sll $v0, $v0, 2             ; level*28
 *            addu $v0, $v0, $a1          ; level*29
 *            sll $v0, $v0, 2             ; level*116
 *            addu $v1, $a2, $v0          ; &table[level]
 *            lw $a1, 0x2c($v1)           ; table[level].field_2C
 *            move $a2, $v1               ; (reuse for next load)
 *            sw $a1, 0x5c($a0)           ; this->0x5C = field_2C
 *            lw $v1, 0x30($a2)           ; table[level].field_30
 *            jr $ra
 *            sw $v1, 0x54($a0)           ; this->0x54 = field_30
 *            Update this globe's target rotation and display frame from the selected level entry.
 *
 * @param level Level whose globe rotation and crosshair target should be used.
 */
void GlobeScreenElement::SetLevel(int level)
{
    mTargetGlobeRot = Levels[level].map_loc[0];
    mTargetCrossY = Levels[level].map_loc[1];
}

/**
 * LevelNameScreenElement constructor
 *
 * Address:   0x80080D64
 * Mangled:   __22LevelNameScreenElementiiii
 * Demangled: LevelNameScreenElement::(int, int, int, int)
 *
 * Construct the park/city label pair and initialize its two local font styles.
 *
 * @param homeX Initial home-space X coordinate.
 * @param homeY Initial home-space Y coordinate.
 * @param screenX Initial screen-space X coordinate.
 * @param screenY Initial screen-space Y coordinate.
 */
LevelNameScreenElement::LevelNameScreenElement(int homeX, int homeY, int screenX, int screenY)
{
    LevelSelect_ConstructScreenElementList(
        &ScreenElementList, homeX, homeY, screenX, screenY);

    ScreenElementList.ScreenElement.CClass.vtable =
        (int *)__vt_22LevelNameScreenElement;
    ::new (FontFromStorage(&mFont1)) Font;
    ::new (FontFromStorage(&mFont2)) Font;

    Font *sourceFont = FontManager::GetFont(
        LevelSelectAddressViewOwners::s_s2font1_fnt_800dbdf0);
    *(LevelSelectFontAssignmentView *)&mFont1 =
        *(LevelSelectFontAssignmentView *)sourceFont;

    mFont1.JustX = 2;
    mFont1.JustY = 3;
    mFont1.R = 0x80;
    mFont1.G = 0x80;
    mFont1.B = 0x80;
    mFont1.Shadow = 0;
    mFont1.SpaceX = 0;
    mFont1.SpaceY = 0;
    mFont1.Scale = 0x1000;
    *(LevelSelectFontAssignmentView *)&mFont2 =
        *(LevelSelectFontAssignmentView *)&mFont1;

    mFont2.JustX = 0;
    mSpacing = 20;

    NameScreenElement *park = (NameScreenElement *)
        ::CClass::operator new(sizeof(NameScreenElement));
    if (park != 0)
    {
        park = LevelSelect_ConstructNameScreenElement(
            park,
            Levels[SelectedLevel].pLevelName, FontFromStorage(&mFont1),
            -262, 0, -1, -1);
    }
    mpPark = &park->TextScreenElement;

    NameScreenElement *city = (NameScreenElement *)
        ::CClass::operator new(sizeof(NameScreenElement));
    if (city != 0)
    {
        city = LevelSelect_ConstructNameScreenElement(
            city,
            Levels[SelectedLevel].pCityName, FontFromStorage(&mFont2),
            522, 0, -1, -1);
    }
    mpCity = &city->TextScreenElement;

    mOriginalHomeX = ScreenElementList.ScreenElement.mHomeX;
    ScreenElementList.addItem(&mpPark->ScreenElement);
    ScreenElementList.addItem(&mpCity->ScreenElement);
    SetLevel(SelectedLevel);

}

/**
 * LevelNameScreenElement destructor
 *
 * Address:   0x80080F88
 * Mangled:   _._22LevelNameScreenElement
 * Demangled: LevelNameScreenElement::~LevelNameScreenElement()
 *
 */
LevelNameScreenElement::~LevelNameScreenElement()
{
    ScreenElementList.ScreenElement.CClass.vtable =
        (int *)__vt_22LevelNameScreenElement;
    FontFromStorage(&mFont2)->~Font();
    FontFromStorage(&mFont1)->~Font();
    ScreenElementList_DestroyAbi(&ScreenElementList, 0);

}

/**
 * SetLevel__22LevelNameScreenElementi — Set level, animation state, and sub-element targets
 *
 * Address:   0x80080FF8, Size: 40 bytes (10 instructions)
 * Mangled:   SetLevel__22LevelNameScreenElementi
 * Demangled: LevelNameScreenElement::SetLevel(int)
 *            Assembly:
 *            addiu $v0, $zero, 2        ; state = 2
 *            sw $a1, 0x11c($a0)         ; this->level = level
 *            lw $a1, 0x68($a0)          ; elem1 = this->elem1
 *            addiu $v1, $zero, -0x106   ; target1 = -0x106
 *            sw $v0, 0x120($a0)         ; this->state = 2
 *            sw $v1, 4($a1)             ; elem1->target = -0x106
 *            lw $a2, 0x6c($a0)          ; elem2 = this->elem2
 *            addiu $v0, $zero, 0x20a    ; target2 = 0x20A
 *            jr $ra
 *            sw $v0, 4($a2)             ; elem2->target = 0x20A
 *
 * @param level Level whose park/city labels should be displayed.
 */
void LevelNameScreenElement::SetLevel(int level)
{
    mTargetLevel = level;
    mState = 2;
    mpPark->ScreenElement.mHomeX = -262;
    mpCity->ScreenElement.mHomeX = 522;
}

/**
 * LevelNameScreenElement::update
 *
 * Address:   0x80081020
 * Mangled:   update__22LevelNameScreenElement
 * Demangled: LevelNameScreenElement::update(void)
 *            Drive the level-name transition state machine and keep the two text children
 *            aligned while their scroll animation settles.
 *
 */
void LevelNameScreenElement::update()
{
    ScreenElementList.update();

    switch (mState)
    {
    case 0:
        mLevel = mTargetLevel;
        mpPark->ScreenElement.mHomeX = (-mSpacing) >> 1;
        mpCity->ScreenElement.mHomeX = mSpacing >> 1;

        mpPark->setString(Levels[mLevel].pLevelName);
        mpCity->setString(Levels[mLevel].pCityName);

        {
            TextScreenElement *park = mpPark;
            int *vtable = park->ScreenElement.CClass.vtable;
            typedef int (*WidthFunction)(TextScreenElement *);
            int width = ((WidthFunction)vtable[5])(park);
            int homeX = mOriginalHomeX + width + (mSpacing >> 1);
            mState = 1;
            ScreenElementList.ScreenElement.mHomeX = homeX;
            ScreenElementList.ScreenElement.mScreenX = homeX;
        }
        break;

    case 2:
        if (mpCity->ScreenElement.mAtHome != 0)
        {
            if (mpPark->ScreenElement.mAtHome != 0)
            {
                mState = 0;
            }
        }
        break;
    }
}

/**
 * Construct the animated text row and install its owner dispatch table.
 *
 * Address: 0x80081174
 * Mangled: __17NameScreenElementPcP4Fontiiii
 */
NameScreenElement::NameScreenElement(
    char *pString, Font *pFont,
    int homeX, int homeY, int screenX, int screenY)
{
    LevelSelect_ConstructTextScreenElement(
        &TextScreenElement, pString, pFont,
        homeX, homeY, screenX, screenY);
    TextScreenElement.ScreenElement.CClass.vtable =
        (int *)__vt_17NameScreenElement;
}

/**
 * NameScreenElement destructor
 *
 * Address:   0x800811C0
 * Mangled:   _._17NameScreenElement
 * Demangled: NameScreenElement::~NameScreenElement()
 *
 */
NameScreenElement::~NameScreenElement()
{
    TextScreenElement.ScreenElement.CClass.vtable =
        (int *)__vt_17NameScreenElement;
    TextScreenElement_DestroyAbi(&TextScreenElement, 0);

}

/**
 * Ease this element toward its home position, then latch the at-home flag.
 *
 * Address: 0x80081214
 * Mangled: update__17NameScreenElement
 */
void NameScreenElement::update()
{
    ScreenElement *elem = &TextScreenElement.ScreenElement;
    int distX;
    int distY;

    if (0 < elem->mDelay)
    {
        elem->mDelay -= 1;
        return;
    }

    distX = elem->mHomeX - elem->mScreenX;
    if ((unsigned int)(distX + 1) < 3)
    {
        elem->mScreenX = elem->mHomeX;
    }
    else
    {
        int add = distX >> 1;
        elem->mScreenX += add;
    }

    distY = elem->mHomeY - elem->mScreenY;
    if ((unsigned int)(distY + 1) < 3)
    {
        elem->mScreenY = elem->mHomeY;
    }
    else
    {
        int add = distY >> 1;
        elem->mScreenY += add;
    }

    if ((elem->mScreenY == elem->mHomeY) &&
        (elem->mScreenX == elem->mHomeX))
    {
        elem->mAtHome = (void *)1;
        return;
    }

    elem->mAtHome = 0;
}

/**
 * Build the level preview element tree and initialize playback state.
 *
 * Address: 0x800812C4
 * Mangled: __18VideoScreenElementiiii
 *
 * @param homeX  Home X coordinate (resting position).
 * @param homeY  Home Y coordinate (resting position).
 * @param screenX  Screen-space X coordinate.
 * @param screenY  Screen-space Y coordinate.
 */
VideoScreenElement::VideoScreenElement(int homeX, int homeY, int screenX, int screenY)
{
    LevelSelect_ConstructScreenElementList(
        &ScreenElementList, homeX, homeY, screenX, screenY);
    ScreenElementList.ScreenElement.CClass.vtable =
        (int *)__vt_18VideoScreenElement;

    {
        Sprite2 *sprite = new Sprite2(s_mar_bmp_800dbe84, 1, 0, 0);
        mVideoSprite[0] = sprite;
    }

    {
        Sprite2 *sprite = new Sprite2(
            LevelSelectAddressViewOwners::s_levna_bmp_800dbe8c,
            1, 0, 0);
        mVideoSprite[1] = sprite;
    }

    {
        SpriteScreenElement *item = (SpriteScreenElement *)
            ::CClass::operator new(sizeof(SpriteScreenElement));
        if (item != 0)
        {
            item = LevelSelect_ConstructSpriteScreenElement(
                item,
                LevelSelectAddressViewOwners::s_s2win5_bmp_800dbe98,
                0, 0, -1, -1, 0, 0);
        }
        ScreenElementList.addItem(&item->ScreenElement);
    }

    mVideoW.value = 176;
    mVideoH.value = 48;
    Pack_Pack(0, 176, 48,
              &mVideoVramX.value, &mVideoVramY.value, 16, 0, 0);

    {
        POLY_FT4 *videoPoly = (POLY_FT4 *)Mem_New(sizeof(POLY_FT4), 1, 1);
        mVideoPoly = videoPoly;
        setlen(videoPoly, 9);
        mVideoPoly->code = 0x2C;
        mVideoPoly->r0 = 0x80;
        mVideoPoly->g0 = 0x80;
        mVideoPoly->b0 = 0x80;
        mVideoPoly->u0 = mVideoVramX.lowByte & 0x3F;
        mVideoPoly->v0 = mVideoVramY.lowByte;
        mVideoPoly->u1 = (u8)(mVideoW.lowByte +
                              (mVideoVramX.lowByte & 0x3F));
        mVideoPoly->v1 = mVideoVramY.lowByte;
        mVideoPoly->u2 = mVideoVramX.lowByte & 0x3F;
        {
            u8 videoY = mVideoVramY.lowByte;
            u8 videoH = mVideoH.lowByte;
            mVideoPoly->v2 = (u8)(videoH + videoY);
        }
        mVideoPoly->u3 = (u8)(mVideoW.lowByte +
                              (mVideoVramX.lowByte & 0x3F));
        {
            u8 videoY = mVideoVramY.lowByte;
            u8 videoH = mVideoH.lowByte;
            mVideoPoly->v3 = (u8)(videoH + videoY);
        }

        mVideoPoly->code = (u8)(mVideoPoly->code & 0xFD);
        mVideoPoly->code = (u8)(mVideoPoly->code & 0xFE);

        u8 *videoBytes = (u8 *)mVideoPoly;
        int level = SelectedLevel;
        int tpage = (mVideoVramY.value & 0x100) >> 4;
        int xPage = ((mVideoVramX.value & 0x3FF) >> 6) | 0x100;
        tpage |= xPage;
        tpage |= (mVideoVramY.lowHalf & 0x200) << 2;
        ((POLY_FT4 *)videoBytes)->tpage = (u16)tpage;

        mLevel = level;
    }

    mNewLevel = -1;
    mState = 0;
    mCount = 32;
    mVideoOffsX = 10;
    mVideoOffsY = 19;
}

/**
 * VideoScreenElement destructor
 *
 * Address:   0x80081548
 * Mangled:   _._18VideoScreenElement
 * Demangled: VideoScreenElement::~VideoScreenElement()
 *
 */
VideoScreenElement::~VideoScreenElement()
{
    ScreenElementList.ScreenElement.CClass.vtable =
        (int *)__vt_18VideoScreenElement;
    LevelSelect_DeleteVirtual(mVideoSprite[0]);
    LevelSelect_DeleteVirtual(mVideoSprite[1]);
    Mem_Delete(mVideoPoly);
    Pack_Unpack(mVideoVramX.lowSignedHalf, mVideoVramY.lowSignedHalf);
    ScreenElementList_DestroyAbi(&ScreenElementList, 0);

}

/**
 * VideoScreenElement::draw(int, int) - draw the level preview video quad or
 * placeholder sprite, copy the ready frame into VRAM, then draw child elements.
 *
 * Address: 0x80081604
 * Mangled: draw__18VideoScreenElementii
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 */
void VideoScreenElement::draw(int x, int y)
{
    int video_x = ScreenElementList.ScreenElement.mScreenX + x + mVideoOffsX;
    int video_y = ScreenElementList.ScreenElement.mScreenY + y + mVideoOffsY;

    if ((unsigned int)(mState - 3) < 2)
    {
        LevelSelectDoubleBuffer *db =
            *(LevelSelectDoubleBuffer **)LevelSelectDoubleBufferAddress;
        mVideoPoly->x0 = (s16)video_x;
        mVideoPoly->y0 = (s16)video_y;
        mVideoPoly->x1 = (s16)(mVideoW.lowHalf + video_x);
        mVideoPoly->y1 = (s16)video_y;
        mVideoPoly->x2 = (s16)video_x;
        mVideoPoly->y2 = (s16)(mVideoH.lowHalf + video_y);
        mVideoPoly->x3 = (s16)(mVideoW.lowHalf + video_x);
        mVideoPoly->y3 = (s16)(mVideoH.lowHalf + video_y);

        unsigned int *panelOT = db->panelOrderingTable;
        mVideoPoly->tag =
            (mVideoPoly->tag & 0xFF000000) | (*panelOT & 0x00FFFFFF);
        unsigned int *panelOT2 = db->panelOrderingTable;
        *panelOT2 =
            (*panelOT2 & 0xFF000000) |
            ((unsigned int)mVideoPoly & 0x00FFFFFF);
    }
    else
    {
        SLevel *table = Levels;
        int level = mLevel;
        SLevel *entry = &table[level];
        signed char movie = entry->STR;
        typedef void (*DrawFunction)(Sprite2 *, int, int, int);
        if (movie == -27)
        {
            Sprite2 *sprite = mVideoSprite[0];
            int *vtable = sprite->CClass.vtable;
            ((DrawFunction)vtable[5])(
                sprite, video_x, video_y, 0);
        }
        else
        {
            Sprite2 *sprite = mVideoSprite[1];
            int *vtable = sprite->CClass.vtable;
            ((DrawFunction)vtable[5])(
                sprite, video_x, video_y, 0);
        }
    }

    if (mState == 2)
    {
        if (mCount == 0)
        {
            RECT r;
            LevelSelectDoubleBuffer *db =
                *(LevelSelectDoubleBuffer **)LevelSelectDoubleBufferAddress;
            r.x = (s16)video_x;
            u16 videoW = mVideoW.lowHalf;
            u16 videoH = mVideoH.lowHalf;
            int topY = db->topY;
            r.w = videoW;
            r.h = videoH;
            r.y = (s16)(topY + video_y);
            MoveImage(
                &r, mVideoVramX.value, mVideoVramY.value);
            mState = 3;
        }
    }

    ScreenElementList.draw(x, y);
}

/**
 * VideoScreenElement::update(void) - advances the level-preview state machine:
 * wait for the preload spooler, start/sync/stop FMV playback, and swap levels.
 *
 * Address: 0x80081804
 * Mangled: update__18VideoScreenElement
 *
 */
void VideoScreenElement::update()
{
    switch (mState)
    {
    case 0:
    {
        int count = mCount;
        if (count != 0)
        {
            count--;
            mCount = count;
            if (count != 0)
            {
                break;
            }
        }
        if ((*(Spooler **)LevelSelectSpoolerAddress)->IsSetDone())
        {
            mState = 1;
        }
        break;
    }
    case 1:
    {
        int level = mLevel;
        SLevel *table = Levels;
        signed char movie = table[level].STR;
        if (movie != 0)
        {
            GameFMV_SetStartTrack(movie);
            int videoX = mVideoVramX.lowHalf;
            int videoW = mVideoW.lowHalf;
            int videoY = mVideoVramY.lowHalf;
            GameFMV_StartX = videoX;
            GameFMV_EndX = videoX + videoW;
            GameFMV_StartY = videoY;
            GameFMV_StartFMV(1);
        }
        mState = 2;
        mCount = 2;
        break;
    }
    case 2:
        mCount = mCount - 1;
        break;
    case 3:
        GameFMV_SyncFMV(1);
        if (mNewLevel != -1)
        {
            mState = 4;
        }
        break;
    case 4:
    {
        int level = mLevel;
        SLevel *table = Levels;
        signed char movie = table[level].STR;
        if (movie != 0)
        {
            GameFMV_StopFMV();
        }
        int newLevel = mNewLevel;
        mNewLevel = -1;
        mState = 1;
        mLevel = newLevel;
        break;
    }
    }
    ScreenElementList.update();
}

/**
 * VideoScreenElement::SetLevel — queue a new level preview.
 *
 * Address:   0x80081998, Size: 8 bytes (2 instructions)
 * Mangled:   SetLevel__18VideoScreenElementi
 * Demangled: VideoScreenElement::SetLevel(int)
 *            Assembly: jr $ra; sw $a1, 0x6c($a0)
 *
 * @param level Level whose preview should be queued.
 */
void VideoScreenElement::SetLevel(int level)
{
    mNewLevel = level;
}

/**
 * GoalScreenElement constructor
 *
 * Address:   0x800819A0
 * Mangled:   __17GoalScreenElementiiii
 * Demangled: GoalScreenElement::(int, int, int, int)
 *
 */
GoalScreenElement::GoalScreenElement(int homeX, int homeY, int screenX, int screenY)
{
    LevelSelect_ConstructScreenElement(
        &ScreenElement, homeX, homeY, screenX, screenY);
    ScreenElement.CClass.vtable = (int *)__vt_17GoalScreenElement;

    unsigned int size = FileIO_Open(
        LevelSelectAddressViewOwners::s_s2levl00_bmp_800dbea4);
    unsigned char *bmpBuf = (unsigned char *)Mem_New(size, 1, 1);
    FileIO_Load(bmpBuf);
    FileIO_Sync();

    {
        int i = 0;
        do
        {
            Sprite2 *sprite = new Sprite2(bmpBuf, 0, 0, 0);
            mpSprite[i] = sprite;
            mpSprite[i]->JustX = 1;
            mpSprite[i]->JustY = 1;
            i++;
        } while (i < LevelSelectLevelCount);
    }

    Mem_Delete(bmpBuf);

    int i = 0;
    int level = SelectedLevel;
    mLevel = level;
    mpSprite[level]->pack();
    mW = mpSprite[mLevel]->screenWidth();
    mH = mpSprite[mLevel]->screenHeight();
    mGoalOffsY = mH >> 1;
    mState = 0;
    mTargetLevel = -1;
    mFreeLastCount = 0;
    mGoalOffsX = mW >> 1;

    Spooler::SpoolMgr->Reset();

    int order[LevelSelectLevelCount];
    Spooler::OrderHelper(order, 8, SelectedLevel);

    char name[16];
    do
    {
        int num = order[i];
        sprintf(name,
                LevelSelectAddressViewOwners::s_s2levl0_d_bmp_800dbeb4,
                num % 9 + 1);
        mSpoolID[num] = Spooler::SpoolMgr->AddFile(name, 0);
        i++;
    } while (i < LevelSelectLevelCount);

    Spooler::SpoolMgr->Spooool();
}

/**
 * GoalScreenElement destructor
 *
 * Address:   0x80081BB4
 * Mangled:   _._17GoalScreenElement
 * Demangled: GoalScreenElement::~GoalScreenElement()
 *            Destroy goal sprites and reset the front-end preload spooler.
 *
 */
GoalScreenElement::~GoalScreenElement()
{
    ScreenElement.CClass.vtable = (int *)__vt_17GoalScreenElement;
    int i = 0;
    Sprite2 **ptr = mpSprite;
    do
    {
        LevelSelect_DeleteVirtual(*ptr);
        i++;
        ptr++;
    } while (i < LevelSelectLevelCount);
    Spooler::SpoolMgr->Reset();
    ScreenElement_DestroyAbi(&ScreenElement, 0);

}

/**
 * GoalScreenElement::update
 *
 * Address:   0x80081C6C
 * Mangled:   update__17GoalScreenElement
 * Demangled: GoalScreenElement::update(void)
 *
 * Poll the eight goal-image spools, install completed sprite data, and advance
 * the outgoing-goal scale/slide state machine.
 */
void GoalScreenElement::update()
{
    Spooler::SpoolMgr->Update();

    for (int i = 0; i < LevelSelectLevelCount; i++)
    {
        if (Spooler::SpoolMgr->IsFileLoaded(mSpoolID[i]))
        {
            void *theData = Spooler::SpoolMgr->GetFile(mSpoolID[i]);
            Sprite2 *sprite = mpSprite[i];
            int *vtable = sprite->CClass.vtable;
            typedef void (*SetDataFunction)(Sprite2 *, void *);
            ((SetDataFunction)vtable[4])(sprite, theData);
            Mem_Delete(theData);
        }
    }

    switch (mState)
    {
    case 0:
        if (mTargetLevel != -1)
        {
            int newLevel = mTargetLevel;
            int oldLevel = mLevel;
            mLevel = newLevel;
            mTargetLevel = -1;
            mLastLevel = oldLevel;
            mpSprite[newLevel]->pack();
            mLeavingX = 0;
            mState = 1;
            mLeavingDir = mNextDir;
        }
        break;

    case 1:
    {
        mpSprite[mLastLevel]->Scale += 400;

        int scale_offs = ((mpSprite[mLastLevel]->Scale * mW) >> 13)
            + (mW >> 1);
        mLeavingX += mLeavingDir;

        if (mLeavingDir > 0)
        {
            mLeavingDir += (mLeavingDir >> 1) + 1;
        }
        else
        {
            mLeavingDir += (mLeavingDir >> 1) - 1;
        }

        if (scale_offs < mLeavingX || mLeavingX < -scale_offs)
        {
            mFreeLastCount = 2;
            mState = 2;
        }
        break;
    }

    case 2:
        if (mFreeLastCount != 0)
        {
            mFreeLastCount--;
            if (mFreeLastCount == 0)
            {
                mpSprite[mLastLevel]->Scale = 0x1000;
                mpSprite[mLastLevel]->unpack();
                mState = 0;
            }
        }
        break;
    }

    ScreenElement.update();
}

/**
 * GoalScreenElement::draw
 *
 * Address:   0x80081EC8
 * Mangled:   draw__17GoalScreenElementii
 * Demangled: GoalScreenElement::draw(int, int)
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 */
// Draw the scrolling goal entry pair, then restore this element's clipping box.
void GoalScreenElement::draw(int x, int y)
{
    SetClippingBox2(0, 0, 512, 256);
    if (mState == 1)
    {
        int sx = ScreenElement.mScreenX;
        int offsetX = mGoalOffsX;
        int offsetX2 = mLeavingX;
        int idx = mLastLevel;
        int sy = ScreenElement.mScreenY;
        sx = sx + x + offsetX + offsetX2;
        Sprite2 *item = mpSprite[idx];
        int *vtable = item->CClass.vtable;
        int offsetY = mGoalOffsY;
        sy = sy + y + offsetY;
        typedef void (*DrawFunction)(Sprite2 *, int, int, int);
        ((DrawFunction)vtable[5])(item, sx, sy, 0);
    }
    int sx2 = ScreenElement.mScreenX;
    int offsetX3 = mGoalOffsX;
    int idx3 = mLevel;
    int sy2 = ScreenElement.mScreenY;
    sx2 = sx2 + x + offsetX3;
    Sprite2 *item2 = mpSprite[idx3];
    int *vtable2 = item2->CClass.vtable;
    int offsetY2 = mGoalOffsY;
    sy2 = sy2 + y + offsetY2;
    typedef void (*DrawFunction)(Sprite2 *, int, int, int);
    ((DrawFunction)vtable2[5])(item2, sx2, sy2, 0);
    int clipW = mW;
    int clipH = mH;
    int cx = ScreenElement.mScreenX;
    int cy = ScreenElement.mScreenY;
    SetClippingBox2(cx + x, cy + y, clipW, clipH);
}

/**
 * SetLevel__17GoalScreenElementii — Conditionally update level and direction
 *
 * Address:   0x80081FCC, Size: 36 bytes (9 instructions)
 * Mangled:   SetLevel__17GoalScreenElementii
 * Demangled: GoalScreenElement::SetLevel(int, int)
 *            Assembly:
 *            lw $v0, 0x30($a0)          ; old = this->0x30
 *            nop
 *            beq $a1, $v0, .exit        ; if (level == old) skip
 *            sll $v0, $a2, 1            ; dir = param2 * 2
 *            addiu $v0, $v0, -1         ; dir = param2*2 - 1
 *            sw $v0, 0xc8($a0)          ; this->0xC8 = dir
 *            sw $a1, 0x38($a0)          ; this->0x38 = level
 *            .exit:
 *            jr $ra
 *            nop
 *
 * @param level Level whose goal list should replace the current one.
 * @param dir  Direction.
 */
void GoalScreenElement::SetLevel(int level, int dir)
{
    if (level != mLevel)
    {
        mNextDir = dir * 2 - 1;
        mTargetLevel = level;
    }
}
