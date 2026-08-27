// Character-creation models, textures, palettes, and front-end editor.

#define __SKIP_MYSKATERTEXTUREINFO_EXTERNS__

#define __SKIP_CCREATETEXSPEC_EXTERNS__

// CREATE owner-local types and declarations.
#ifdef __cplusplus
extern "C" {
#endif

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

#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBGPU.H>

#include "globals.h"
#include "CITEM.h"
#include "SMODEL.h"

struct tag_S_Pal;

#include "TEXTURE.h"

/** Mesh header. */
typedef struct MeshHeader {
    u16 meshFlags;
    u16 vertexCount;
    u16 normalCount;
    u16 faceCount;
    u32 radius;
    s16 bboxXMax;
    s16 bboxXMin;
    s16 bboxYMax;
    s16 bboxYMin;
    s16 bboxZMax;
    s16 bboxZMin;
    s16 lodDepth;
    u16 lodNextMesh;
} MeshHeader;

#include "SANIMFRAME.h"

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
struct SDoubleBuffer;
struct SViewport;

#define CREATE_TEX_SPEC_LAYOUT
#define CREATE_TEX_SPEC_LAYOUT_ONLY
#include "CREATE.h"
#undef CREATE_TEX_SPEC_LAYOUT_ONLY
#undef CREATE_TEX_SPEC_LAYOUT

struct _StitchEntry {
    short initialSourceStitch;
    short initialBuildStitch;
    short buildIndex;
};

struct _SpoolerEntry {
    unsigned int checksum;
    char filename[32];
    int spoolID;
    int texPart;
};

struct SPart {
    char *pName;
    char GroupName[16];
};

struct SGroup {
    unsigned char Num;
    char *pSearch;
};

struct SManualStitchPart {
    char *pName;
};

struct MySkaterTextureInfo {
    struct CClass CClass;
    unsigned int mChecksum;
    RECT mUVRect;
    unsigned int mPart[120];
    int mPartCount;

    /** Record a body part that uses this texture. */
    void AddBodyPart(int partNum);
    /** Copy the source texture's UV rectangle. */
    void SetTextureInfo(Texture *pTexture);
};

// CharacterEditor is manipulated through its recovered constructor/destructor
// entry points in this TU. Its large UI layout remains opaque here.
struct CharacterEditor;

#ifdef __cplusplus
}
#endif

#include "M3D.h"

extern "C" int Create_Parts __attribute__((section(".sbss")));
extern "C" int GFirstTime;
extern "C" int NextPart __attribute__((section(".sbss")));
extern "C" int PaletteSelected;
extern "C" char *pPSH __attribute__((section(".sbss")));

#include "FILEIOSIZE.h"
#include "PAL256.h"
extern "C" int Stats[];
// LevelGenerator owns the shared GridCellWidth object.

#include "PDOUBLEBUFFER.h"

#include "CREATE_COLOR.h"

#include "SVIEWPORT.h"

#include "SLIGHT.h"

typedef char Create_SLight_size_must_be_52[
    (sizeof(SLight) == 52) ? 1 : -1];

#include "OB.h"
#include "CSUPER.h"
#include "FILEIO.h"
#include "SFX.h"
#include "PACK.h"
#include "PAD.h"
#include "PSXREGION.h"
#include "SPOOL.h"
#include "SCREENTOOLS.h"
#include "FONTTOOLS.h"
#include "M3DINIT.h"
#include "MAIN.h"
#include "RFRONT.h"
#include "MEM.h"
#include "UTILS.h"
#include <STDIO.H>
#include <STRINGS.H>

/** Pointer-only view of the shared asynchronous file manager. */
struct Spooler {
    static Spooler *SpoolMgr __attribute__((section(".data")));
    int AddFile(char *filename, int ignoreExtension);
    void *GetFile(int slot);
    int IsFileLoaded(int slot);
    int IsSetDone();
    void Spooool();
    void Update();
};

#define PAL_ENTRY_LAYOUT
#include "PAL.h"
#undef PAL_ENTRY_LAYOUT

// === External declarations (C linkage) ===
extern "C"
{
    extern SPart Part[];          // 0x800C4DD4, recovered original table name
    extern char *BodyPart[];      // 19 animation-name pointers
    extern SGroup Group[4];
    // Pal256X and Pal256Y are declared above (0x800DA384, 0x800DA388)
    extern char SearchPattern1[];    // 0x800DB38C — first search keyword
    extern char SearchPattern2[];    // 0x800DB398 — second search keyword (frame count section)
    void CItem_Init(void *, char *) asm("InitItem__5CItemPc");
    extern char CreateSubDir[];
    extern char sBodyPart_Head[];
    extern char sBodyPart_Board1[];
    extern char sBodyPart_Board2[];
    extern char sBodyPart_Board3[];
    extern char ErrorMsg_GetFirstTexture[];
    extern char sAnimFormatStr[]; // 0x800DB41C "anims_cs" format
    extern char sSk2Def[];        // 0x800DB3A0 "sk2def"
    extern char sSk2DefPSH[];     // 0x800DB3A8 "sk2def.psh"
}

#include "STRLIB.h"
#include "M3DASM.h"

// === CREATE-owned body-part names and model-selection state ===
//
// Access sites retain their incomplete-array declarations below so the old
// compiler keeps the target's absolute address formation.  These owner names
// provide the real storage and exact assembler labels without exposing the
// target-private data as linker exports or changing source-facing codegen.
static char Create_BodyPelvisOwner[8] asm("sBodyPart_Pelvis")
    __attribute__((section(".sdata"), aligned(4))) = "_pelvis";
static char Create_BodyRightThighOwner[16] asm("sBodyPart_Legs5")
    __attribute__((section(".sdata"), aligned(4))) = "_right_thigh";
static char Create_BodyRightShoeOwner[12] asm("sBodyPart_Legs7")
    __attribute__((section(".sdata"), aligned(4))) = "_right_shoe";
static char Create_BodyRightShinOwner[12] asm("sBodyPart_Legs6")
    __attribute__((section(".sdata"), aligned(4))) = "_right_shin";
static char Create_BodyLeftThighOwner[12] asm("sBodyPart_Legs2")
    __attribute__((section(".sdata"), aligned(4))) = "_left_thigh";
static char Create_BodyLeftShoeOwner[12] asm("sBodyPart_Legs4")
    __attribute__((section(".sdata"), aligned(4))) = "_left_shoe";
static char Create_BodyLeftShinOwner[12] asm("sBodyPart_Legs3")
    __attribute__((section(".sdata"), aligned(4))) = "_left_shin";
static char Create_BodyStomachOwner[12] asm("sBodyPart_Torso1")
    __attribute__((section(".sdata"), aligned(4))) = "_stomach";
static char Create_BodyChestOwner[8] asm("sBodyPart_Chest")
    __attribute__((section(".sdata"), aligned(4))) = "_chest";
static char Create_BodyLeftHandOwner[12] asm("sBodyPart_Torso8")
    __attribute__((section(".sdata"), aligned(4))) = "_left_hand";
static char Create_BodyLeftForearmOwner[16] asm("sBodyPart_Torso7")
    __attribute__((section(".sdata"), aligned(4))) = "_left_forearm";
static char Create_BodyLeftBicepOwner[12] asm("sBodyPart_Torso6")
    __attribute__((section(".sdata"), aligned(4))) = "_left_bicep";
static char Create_BodyRightHandOwner[12] asm("sBodyPart_Torso5")
    __attribute__((section(".sdata"), aligned(4))) = "_right_hand";
static char Create_BodyRightForearmOwner[16] asm("sBodyPart_Torso4")
    __attribute__((section(".sdata"), aligned(4))) = "_right_forearm";
static char Create_BodyRightBicepOwner[16] asm("sBodyPart_Torso3")
    __attribute__((section(".sdata"), aligned(4))) = "_right_bicep";
static char Create_BodyHeadOwner[8] asm("sBodyPart_Head")
    __attribute__((section(".sdata"), aligned(4))) = "_head";
static char Create_BodyBoardOwner[8] asm("sBodyPart_Board1")
    __attribute__((section(".sdata"), aligned(4))) = "_board";
static char Create_BodyFrontWheelOwner[16] asm("sBodyPart_Board2")
    __attribute__((section(".sdata"), aligned(4))) = "_front_wheel";
static char Create_BodyBackWheelOwner[12] asm("sBodyPart_Board3")
    __attribute__((section(".sdata"), aligned(4))) = "_back_wheel";

/**
 * Four model-name prefixes searched by Create_GetModelIndex.
 *
 * The first pointer carries the original aggregate export.  Keeping it as a
 * four-byte source object preserves GCC's target gp-relative address load;
 * the three owner-private tail pointers remain physically contiguous.
 */
char *Create_ManualStitchFirst asm("ManualStitchPart") =
    Create_BodyPelvisOwner;
static char *Create_ManualStitchSecond = Create_BodyStomachOwner;
static char *Create_ManualStitchThird = Create_BodyChestOwner;
static char *Create_ManualStitchFourth = Create_BodyHeadOwner;

int PaletteSelected = -1;
int GFirstTime = 1;
static int Create_NextBuildIndex = 0;
static int Create_IsSourceRegion = 0;
static int Create_IsBuildRegion = 0;

// The old synthetic DAT name is the first pSearch field in the typed Group
// table.  Keeping this view as a source expression avoids inventing a second
// overlapping object owner.
#define AnimGroupData8 ((int *)&Group[0].pSearch)

struct SCamera;
extern "C" SViewport CreateViewport;
static SViewport *pViewport = &CreateViewport;

/**
 * CREATE-owned PSH parser state in its complete target common-data order.
 *
 * MAIN.SYM records all three names as owner-local definitions at
 * 0x800DC77C..0x800DC788.  In particular, pPSH is also the buffer released by
 * Free2; keeping one object for both uses avoids the synthetic PSHFileBuffer
 * alias that previously allocated a fourth common.
 */
static int Create_NextPartStorage asm("NextPart");
static int Create_PartsStorage asm("Create_Parts");
static char *Create_PSHStorage asm("pPSH");

/**
 * Compact animation range copied into a newly created skater.
 *
 * The two-byte alignment is required for the original unaligned word-copy
 * sequence.
 */
struct CreateAnimHdr8
{
    short w[4];
};

extern "C" void Create_MySkaterDestructorSlot(void)
    asm("_._8MySkater");
extern "C" {
static void Create_MySkaterTextureInfoDestructor(
    MySkaterTextureInfo *, int)
    asm("_._19MySkaterTextureInfo");
static void Create_CClassDestructor_80073244(CClass *, int)
    asm("__DOT__6CClass_80073244");
}

// These definitions precede the functions that use the three tables so the
// original compiler selects its one-instruction gp-relative address form.
/** PSH part-table search markers. */
static char Create_SearchPattern1Owner[12] asm("SearchPattern1")
    __attribute__((section(".sdata"))) =
    "sk2defpart_";
static char Create_SearchPattern2Owner[8] asm("SearchPattern2")
    __attribute__((section(".sdata"))) =
    "parent:";

/** Character-editor source model names. */
static char Create_Sk2DefOwner[8] asm("sSk2Def")
    __attribute__((section(".sdata"))) = "sk2def";
static char Create_Sk2DefPSHOwner[12] asm("sSk2DefPSH")
    __attribute__((section(".sdata"))) = "sk2def.psh";

/** Body-part suffixes used while selecting replacement textures. */
static char Create_BodyPartBicepOwner[8] asm("sBodyPart_Bicep")
    __attribute__((section(".sdata"))) = "_bicep";
static char Create_BodyPartForearmOwner[12] asm("sBodyPart_Forearm")
    __attribute__((section(".sdata"))) = "_forearm";
static char Create_BodyPartHandOwner[8] asm("sBodyPart_Hand")
    __attribute__((section(".sdata"))) = "_hand";
static char Create_BodyPartThighOwner[8] asm("sBodyPart_Thigh")
    __attribute__((section(".sdata"))) = "_thigh";
static char Create_BodyPartShinOwner[8] asm("sBodyPart_Shin")
    __attribute__((section(".sdata"))) = "_shin";
static char Create_BodyPartShoeOwner[8] asm("sBodyPart_Shoe")
    __attribute__((section(".sdata"))) = "_shoe";

/** Character-editor directory and texture-name diagnostics. */
static char Create_SubDirOwner[12] asm("CreateSubDir")
    __attribute__((section(".sdata"))) = "skatedit\\";
static char Create_TextureFormatOwner[8] asm("sCreateTexFmt")
    __attribute__((section(".sdata"))) = "%s.bmp";
static char Create_NextFaceFormatOwner[12] asm("sNextFaceFmt")
    __attribute__((section(".sdata"))) = "%s face!\n";
static char Create_NextFaceForwardOwner[8] asm("sNextFaceFwd")
    __attribute__((section(".sdata"))) = "Next";
static char Create_NextFaceBackwardOwner[8] asm("sNextFaceBwd")
    __attribute__((section(".sdata"))) = "Prev";

/** Animation-name formatting strings. */
static char Create_SubDir2Owner[4] asm("CreateSubDir2")
    __attribute__((section(".sdata"))) = "%s_";
static char Create_AnimFormatOwner[12] asm("sAnimFormatStr")
    __attribute__((section(".sdata"))) = "anims_cs";

/** Character-editor art, fonts, labels, and prompts in original link order. */
static char Create_BackgroundNameOwner[12] asm("s_s2bgd4_bmp_800db428")
    __attribute__((section(".sdata"))) = "s2bgd4.bmp";
static char Create_Font1Owner[12] asm("sFont1")
    __attribute__((section(".sdata"))) = "s2font1.fnt";
static char Create_Font4Owner[16] asm("sFont4")
    __attribute__((section(".sdata"))) = "s2skata2.fnt";
static char Create_Font2Owner[12] asm("sFont2")
    __attribute__((section(".sdata"))) = "s2font2.fnt";
static char Create_Font3Owner[12] asm("sFont3")
    __attribute__((section(".sdata"))) = "s2menu1.fnt";
static char Create_HeadOwner[8] asm("s_Head_800db468")
    __attribute__((section(".sdata"))) = "Head";
static char Create_FaceOwner[8] asm("s_face_800db470")
    __attribute__((section(".sdata"))) = " face";
static char Create_CapOwner[8] asm("s_cap_800db478")
    __attribute__((section(".sdata"))) = " cap";
static char Create_UpperOwner[8] asm("s_Upper_800db480")
    __attribute__((section(".sdata"))) = "Upper";
static char Create_ChestOwner[8] asm("s_chest_800db488")
    __attribute__((section(".sdata"))) = " chest";
static char Create_BicepOwner[8] asm("s_bicep_800db490")
    __attribute__((section(".sdata"))) = " bicep";
static char Create_ForearmOwner[12] asm("s_forearm_800db498")
    __attribute__((section(".sdata"))) = " forearm";
static char Create_LowerOwner[8] asm("s_Lower_800db4a4")
    __attribute__((section(".sdata"))) = "Lower";
static char Create_ThighOwner[8] asm("s_thigh_800db4ac")
    __attribute__((section(".sdata"))) = " thigh";
static char Create_ShinOwner[8] asm("s_shin_800db4b4")
    __attribute__((section(".sdata"))) = " shin";
static char Create_ShoeOwner[8] asm("s_shoe_800db4bc")
    __attribute__((section(".sdata"))) = " shoe";
static char Create_CursorNameOwner[16] asm("s_s2crskw1_bmp_800db4c4")
    __attribute__((section(".sdata"))) = "s2crskw1.bmp";
static char Create_Prompt4NameOwner[16] asm("s_s2prmpt4_bmp_800db4d4")
    __attribute__((section(".sdata"))) = "s2prmpt4.bmp";
static char Create_SelectOwner[8] asm("s_select_800db4e4")
    __attribute__((section(".sdata"))) = "select";
static char Create_Prompt5NameOwner[16] asm("s_s2prmpt5_bmp_800db4ec")
    __attribute__((section(".sdata"))) = "s2prmpt5.bmp";
static char Create_ChangeOwner[8] asm("s_change_800db4fc")
    __attribute__((section(".sdata"))) = "change";
static char Create_Prompt1NameOwner[16] asm("s_s2prmpt1_bmp_800db504")
    __attribute__((section(".sdata"))) = "s2prmpt1.bmp";
static char Create_AcceptOwner[8] asm("s_accept_800db514")
    __attribute__((section(".sdata"))) = "accept";
static char Create_Prompt2NameOwner[16] asm("s_s2prmpt2_bmp_800db51c")
    __attribute__((section(".sdata"))) = "s2prmpt2.bmp";
static char Create_BackOwner[8] asm("s_back_800db52c")
    __attribute__((section(".sdata"))) = "back";
static char Create_AnimGameFormatOwner[8] asm("sAnimGameFmt")
    __attribute__((section(".sdata"))) = "sk2anim";

/** Default animation range copied into a newly created skater. */
static CreateAnimHdr8 Create_DefaultAnimHeaderOwner[1] asm("DAT_800db53c")
    __attribute__((section(".sdata"), aligned(2))) = {
        {{0, -1, -1, 9999}},
};

/**
 * MySkater constructor/destructor dispatch table.
 *
 * The exported first word stays a scalar so GCC 2.x retains the target's
 * gp-relative address load; the private tail is the rest of the same table.
 */
int __vt_8MySkater = 0;
static int Create_MySkaterVtableTail[3]
    __attribute__((section(".sdata"))) = {
        0,
        (int)&Create_MySkaterDestructorSlot,
        0,
};

/**
 * MySkaterTextureInfo constructor dispatch table.
 *
 * This has the same scalar-plus-tail layout but remains owner-local because
 * the target symbol table does not expose its assembler label.
 */
static int Create_MySkaterTextureInfoVtableRef
    asm("__vt_19MySkaterTextureInfo") = 0;
static int Create_MySkaterTextureInfoVtableTail[3]
    __attribute__((section(".sdata"))) = {
        0,
        (int)&Create_MySkaterTextureInfoDestructor,
        0,
};

/** Owner-local base table installed by the generated texture-info destructor. */
static int Create_MySkaterDestructorVtableRef asm("__vt_8MySkater_dtor") = 0;
static int Create_MySkaterDestructorVtableTail[2]
    __attribute__((section(".sdata"))) = {
        0,
        (int)&Create_CClassDestructor_80073244,
};

// === Forward type declarations ===


class CharacterEditor;
class MySkaterTextureInfo;
class VerticalMenu2;
struct VMenuItem;
struct ScreenElement;
struct FontCharacter;
struct TextEffect;
struct TextMenuProperties;

/** Common first three slots of deleting-destructor-capable vtables. */
struct CreateDeleteVtable
{
    void (*Reserved0)(void);
    void (*Reserved1)(void);
    void (*destroy)(void *object, int flags);
    void (*update)(void *object);
    void (*drawElement)(void *object, int x, int y);
    void (*drawSprite)(void *object, int x, int y, int drawFirst);
};

struct CreateDeletableObject
{
    CreateDeleteVtable *vtable;
};

/** Render-buffer fields used by the character-editor draw pass. */
struct SDoubleBuffer
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

typedef char Create_SDoubleBuffer_size_must_be_0xAC[
    (sizeof(SDoubleBuffer) == 0xAC) ? 1 : -1];

/** Polymorphic prefix needed for original CClass assignment semantics. */
struct CreateClassAssignmentBase
{
    virtual ~CreateClassAssignmentBase();
};

/** Polymorphic assignment view matching Font's original CClass semantics. */
class CreateAssignableFont : public CreateClassAssignmentBase
{
public:
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

typedef char Create_assignable_font_size_must_be_0x50[
    (sizeof(CreateAssignableFont) == 0x50) ? 1 : -1];

/** Front-end sprite storage allocated through the original CClass arena. */
struct Sprite2
{
    CreateDeleteVtable *vtable;
    unsigned char mStorage[0x38];

    static void *operator new(unsigned int size) throw() {
        return ::CClass::operator new(size);
    }
    /** Build a sprite from a bitmap file. */
    Sprite2(char *name, int pack, int sliceWidth, int sliceHeight);
};

typedef char Create_Sprite2_size_must_be_0x3C[
    (sizeof(Sprite2) == 0x3C) ? 1 : -1];

/** Menu style shared by every character-part text item. */
struct TextMenuProperties : public CreateClassAssignmentBase
{
    FontStorage TheFont;
    int BigScale;
    int NormalScale;
    int SelectedR;
    int SelectedG;
    int SelectedB;
    int NormalR;
    int NormalG;
    int NormalB;
    int GrayR;
    int GrayG;
    int GrayB;
};

typedef char Create_TextMenuProperties_size_must_be_0x80[
    (sizeof(TextMenuProperties) == 0x80) ? 1 : -1];

/** Allocation extent of a character-editor text menu item. */
struct CreateTextMenuItemStorage
{
    unsigned char bytes[0x50];
};

/** Allocation extent of a text-only screen element. */
struct CreateTextScreenElementStorage
{
    unsigned char bytes[0x2C];
};

/** Allocation extent of an image-backed screen element. */
struct CreateSpriteScreenElementStorage
{
    unsigned char bytes[0x28];
};

/** Runtime dispatch prefix for a vertical-menu item. */
struct CreateVMenuItemVtable
{
    void (*Reserved0)(VMenuItem *item);
    void (*Reserved1)(VMenuItem *item);
    void (*destroy)(VMenuItem *item, int flags);
    void (*setXY)(VMenuItem *item, int x, int y);
};

/** Proven menu-item layout through the vertical justification field. */
struct VMenuItem
{
    CreateVMenuItemVtable *vtable;
    int ID;
    int Selectable;
    int Choosable;
    int Highlighted;
    int InternalState;
    int W;
    int H;
    int X;
    int Y;
    int JustX;
};

typedef char Create_VMenuItem_size_must_be_0x2C[
    (sizeof(VMenuItem) == 0x2C) ? 1 : -1];

/** Proven virtual dispatch prefix for the vertical character menu. */
struct CreateVerticalMenuVtable
{
    void (*Reserved0)(VerticalMenu2 *menu);
    void (*Reserved1)(VerticalMenu2 *menu);
    void (*destroy)(VerticalMenu2 *menu, int flags);
    void (*setArrows)(VerticalMenu2 *menu, void *left, void *right,
                      int spacing);
    void (*activate)(VerticalMenu2 *menu);
    void (*draw)(VerticalMenu2 *menu);
    void (*update)(VerticalMenu2 *menu);
    void (*processPress)(VerticalMenu2 *menu, int press);
    void (*addItem)(VerticalMenu2 *menu, void *item);
};

class BasicMenu
{
public:
    CreateVerticalMenuVtable *vtable;
    void *LeftArrow;
    void *RightArrow;
    int ArrowSpace;
    VMenuItem *Item[16];
    int HighestSlot;
    int Selected;
    int X;
    int Y;
    int W;
    int H;

    int getHighlighted(void);
};

class VerticalMenu2 : public BasicMenu
{
public:
    int JustX;
    int JustY;
    int Spacing;
};

typedef char Create_VerticalMenu2_size_must_be_0x74[
    (sizeof(VerticalMenu2) == 0x74) ? 1 : -1];

class ScreenElementList
{
public:
    CreateDeleteVtable *vtable;
    char mBase[0x18];
    int mScreenX;
    int mScreenY;
    void *mpItem[16];
    int mCount;

    void update(void);
    void draw(int x, int y);
    /** Add one element to the first unused list slot. */
    void addItem(ScreenElement *element);
};

typedef char Create_ScreenElementList_size_must_be_0x68[
    (sizeof(ScreenElementList) == 0x68) ? 1 : -1];

/** Prompt row containing an icon and its explanatory text. */
struct PromptScreenElement
{
    ScreenElementList ScreenElementList;

    static void *operator new(unsigned int size) throw() {
        return ::CClass::operator new(size);
    }
    PromptScreenElement(char *imageName, char *prompt, Font *font,
                        int homeX, int homeY, int screenX, int screenY);
};

typedef char Create_PromptScreenElement_size_must_be_0x68[
    (sizeof(PromptScreenElement) == 0x68) ? 1 : -1];

/** Fixed prefix shared by the variable-sized face packets in a model. */
struct CreateFacePacket
{
    unsigned short Flags;
    unsigned short Size;
    unsigned char PadToUV[12];
    unsigned char U0;
    unsigned char V0;
    unsigned char PadToTexturePage[4];
    unsigned short TexturePage;
    unsigned char PadToTextureWindow[4];
    unsigned int TextureWindow;
};

typedef char Create_FacePacket_prefix_size_must_be_32[
    (sizeof(CreateFacePacket) == 32) ? 1 : -1];

typedef char Create_SMatrix_size_must_be_24[
    (sizeof(SMatrix) == 24) ? 1 : -1];

#include "SCAMERA.h"

/** CREATE-owned slice of the 400-byte preview skater object. */
struct CreateCSuperView
{
    unsigned short Flags;
    unsigned short Inquiry;
    CVector Position;
    CSVector Angles;
    unsigned char mRegion;
    unsigned char PadToLight[0x3C - 0x17];
    SLight *mpLight;
    unsigned char PadToVtable[0x48 - 0x40];
    CreateDeleteVtable *vtable;
    unsigned char PadToAnimSpeed[0x12C - 0x4C];
    int mAnimSpeed;
    unsigned char PadToEnd[0x190 - 0x130];
};

typedef char Create_CSuper_size_must_be_0x190[
    (sizeof(CreateCSuperView) == 0x190) ? 1 : -1];

/** Narrow stats-table slice retaining the original 344-byte stride. */
struct CreateStatsEntry
{
    char *PSXName;
    char *PSXName2;
    char *FrontName;
    int ComboID;
    unsigned char Board;
    unsigned char Wheel1;
    unsigned char Wheel2;
    unsigned char Head;
    unsigned char PadToName[0x2C - 0x14];
    char *Name;
    unsigned char PadToEnd[0x158 - 0x30];
};

typedef char Create_StatsEntry_size_must_be_0x158[
    (sizeof(CreateStatsEntry) == 0x158) ? 1 : -1];

// === Class declarations ===
class MySkater
{
public:
    CClass CClass;
    _SpoolerEntry mSpoolerEntryList[9];
    int mSpoolerEntryCount;
    int mSpoolSetDone;
    int mSkinToneChanged;
    _SCreateTexList *mpTexList[9];
    _SCreateTexList *mpOldList[9];
    HSV_VALUE *mpPaletteBuffer[9];
    tag_S_Pal *mpPal[9];
    HSV_VALUE mDesiredHSV[9];
    int mInheritBicep;
    int mInheritForearm;
    int mSourceRegion;
    int mBuildRegion;
    int mFree;
    MySkaterTextureInfo **mppTextures;
    int mNumTextures;
    int mSelectedSkinTone;
    int mRebuild;
    int mSelectedPart[4];
    _StitchEntry mStitchDatabase[120];
    int mAnimOrder[19];
    unsigned int mChecksumList[32];
    Texture *mpChecksumTexture[32];
    int mChecksumCount;
    int mModelRegionPartCount;
    char mAnimName[32];

    char *GetFrontEndName(int Number, CCreateTexSpec *pSpec);
    char *GetGroupName(int Number);
    char *Create_GetGroupName(char *p, int Number);
    MySkater(void);
    ~MySkater();
    /** Releases create-a-skater storage through the original CClass allocator. */
    static void operator delete(void *p) {
        ::CClass::operator delete(p);
    }
    Texture *FindTexture(unsigned int checksum);
    void SaveTextures(void);
    void UpdateTextureList(void);
    MySkaterTextureInfo *FindTextureInfo(unsigned int checksum);
    void BuildChecksumList(SModel *pModel, unsigned int *pChecksumList, int *pChecksumCount);
    void Init(char *pAnimFile);
    void UpdateStats(char *partName, int modelIndex);
    void Rebuild(CSuper *pSourceSkater, CSuper *pBuildSkater, int spoolTextures);
    int TextureIsInBodyPart(unsigned int checksum, char *part);
    unsigned int FindUniqueTextureBySize(char *inPart, int findLarger);
    unsigned int FindUniqueTexture(char *inPart, char *notInPart,
                                   char *notInPart2);
    int FindBuildIndex(char *name);
    unsigned int GetChecksumToReplace(int texPart);
    char *GetSkinToneFileName(char *filename);
    void SpoolTextures(void);
    void ReplaceTextures(void);
    void AddSpoolerEntry(unsigned int checksum, char *filename, int texPart);
    void AddSpoolerEntries(void);
    void ProcessSpoolerEntry(void);
    void Free2(void);
    void Free(void);
    _SCreateTexList *GetFirstTexture(CCreateTexSpec *pCurrSpec, int bodyGroup,
                                     int index, int lastTexture);
    void UpdateGeometry(int bodyGroup);
    void UpdateDefaultHue(int bodyGroup);
    void NextGeometry(int bodyGroup, int forwards);
    void NextTexture(int texPart, int forwards);
    void NextFace(CCreateTexSpec *pSpec, int forwards);
    void Create_Off(void);
    void Create_On(char *p1, char *p2);
    void Create_OnHead(char *p);
    void Create_OnBoard(char *p);
    void Create_OnTorso(char *p);
    void Create_OnLegs(char *p);
    int GetSourceIndex(int buildIndex);
    void GetInternalStitches(void);
    void PatchInternalStitches(void);
    int InsertStitch(SVECTOR *pNewStitchVertex, SVECTOR *pNewGhostVertex, SVECTOR *pNewStitchNormal, SVECTOR *pNewGhostNormal);
    void CreateManualStitches(CSuper *mySuper, int partIndexA, int partIndexB);
    void DecrementBuildTextures(void);
    void IncrementTextureUsage(void);
    void RememberPalette(int texPart, unsigned int checksum);
    void UpdatePalette(int texPart);
    void PatchAnimationOrder(CSuper *pBuildSkater);
};

/**
 * CREATE-owned layout slice for the character editor screen.
 *
 * The layout follows the owner record in `MAIN_STRUCTS.h`, including the
 * embedded fonts and background sprite consumed by `draw`.
 */
class CharacterEditor : public ScreenElementList
{
public:
    MySkater *mpSkater;
    CSuper *mpBuildSkater;
    CSuper *mpSourceSkater;
    int mBitsRegion;
    VerticalMenu2 *mMenu;
    VMenuItem *mItem[11];
    TextMenuProperties *mProps;
    FontStorage mFont1;
    FontStorage mFont2;
    FontStorage mFont3;
    Sprite2 *mBackground;

    ~CharacterEditor();
    /** Releases editor-screen storage through the original CClass allocator. */
    static void operator delete(void *p) {
        ::CClass::operator delete(p);
    }
    void update(void);
    void draw(int x, int y);
};

typedef char Create_CharacterEditor_size_must_be_0x1A0[
    (sizeof(CharacterEditor) == 0x1A0) ? 1 : -1];

/** CREATE-owned storage for the original file-local MySkater singleton. */
static unsigned char sMySkaterStorage[0x734] asm("sMySkater");
typedef char Create_MySkater_size_must_be_0x734[
    (sizeof(MySkater) == 0x734 && sizeof(sMySkaterStorage) == 0x734) ? 1 : -1];

#define sMySkater (*(MySkater *)sMySkaterStorage)

// === Free function declarations ===
int Create_GetAnimIndex(int i);
int Create_GetModelIndex(int i);
void Create_LoadPSH(char *pPSHName);
int Create_GetModelSize(SModel *pModel);
int Create_CountString(char *p, int sourceRegion);
int Create_CountStitches(SModel *pModel);
void RefreshTexture(unsigned int Checksum, char *pBuf);
#define CREATE_TEX_LIST_LAYOUT
#define CREATE_TEX_LIST_LAYOUT_ONLY
#include "CREATE.h"
#undef CREATE_TEX_LIST_LAYOUT_ONLY
#undef CREATE_TEX_LIST_LAYOUT
#include "CREATE.h"

// === External functions and globals ===
// === Extern "C" forward declarations ===
extern "C" MySkaterTextureInfo *MySkaterTextureInfo_ctor(
    MySkaterTextureInfo *, unsigned int) asm("__19MySkaterTextureInfoUi");
extern "C" CharacterEditor *CharacterEditor_Ctor(CharacterEditor *)
    asm("__15CharacterEditor");
extern "C" int CharacterEditor_handlePress(CharacterEditor *)
    asm("handlePress__15CharacterEditor");
extern "C" CreateDeleteVtable CharacterEditor_vtable
    asm("__vt_15CharacterEditor");
extern "C" void ScreenElementList_base_destructor(ScreenElementList *, int)
    asm("_._17ScreenElementList");

// === CREATE-owned read-only data ===

extern "C" void Create_CharacterEditorDestructorSlot(void)
    asm("_._15CharacterEditor");
extern "C" void Create_CharacterEditorUpdateSlot(void)
    asm("update__15CharacterEditor");
extern "C" void Create_CharacterEditorDrawSlot(void)
    asm("draw__15CharacterEditorii");
extern "C" int Create_ScreenElementWidthSlot(ScreenElement *)
    asm("width__13ScreenElement");

/** Diagnostic emitted while resolving a unique replacement texture. */
static char Create_FindUniquePrintFmtOwner[40] asm("FindUniquePrintFmt")
    __attribute__((section(".rdata"), aligned(4))) =
    "numFound = %d oldNumFound = %d %s %s\n";

/** Diagnostic emitted when a create-a-skater part has no geometry. */
static char Create_GetFirstTextureErrorOwner[32] asm("ErrorMsg_GetFirstTexture")
    __attribute__((section(".rdata"), aligned(4))) =
    "No geometry defined for %s\n";

/**
 * Character-editor virtual dispatch table.
 *
 * The final null word is a genuine reserved slot, not alignment padding.
 */
int Create_CharacterEditorVtableOwner[8] asm("__vt_15CharacterEditor")
    __attribute__((section(".rdata"), aligned(4))) = {
        0,
        0,
        (int)&Create_CharacterEditorDestructorSlot,
        (int)&Create_CharacterEditorUpdateSlot,
        (int)&Create_CharacterEditorDrawSlot,
        (int)&Create_ScreenElementWidthSlot,
        (int)&ScreenElement_Refresh,
        0,
};

// === Function implementations (address order) ===

/**
 * Create_GetAnimIndex__Fi — Find animation index by name match
 * Given a group index, looks up the animation name from the group table
 * (0x800C4DD4, stride 20 bytes per entry), then searches the animation
 * name table (0x800C4D20, 4 bytes per pointer, up to 19 entries) for a
 * match using strstr. Returns the matching index or -1 if not found.
 * External globals (non-$gp, accessed via lui/addiu)
 *
 * Address:   0x8006D86C, Size: 124 bytes (31 instructions)
 * Mangled:   Create_GetAnimIndex__Fi
 * Demangled: int Create_GetAnimIndex(int)
 * @param i Index of the body-part group whose animation name is requested.
 * @return Matching animation slot, or -1 when the name is absent.
 */

int Create_GetAnimIndex(int i)
{
    int n = 0;
    int step1 = i << 2;
    SPart *base = Part;
    int offset = (step1 + i) << 2;
    // Direct `&Part[i]` changes four target instructions.
    SPart *groupEntry = (SPart *)((char *)base + offset);
    char **searchTable = BodyPart;

    do
    {
        if (strstr(groupEntry->pName, *searchTable) != 0)
        {
            return n;
        }
        n++;
        searchTable++;
    } while (n < 19);

    return -1;
}

/**
 * Create_GetModelIndex__Fi — Find model index by name match
 * Given a group index, looks up the model name from the group table
 * (0x800C4DD4, stride 20 bytes), then searches the model name table
 * ($gp+0x36C4, 4 entries max) for a match using strstr.
 * If no match found, returns and increments a global counter ($gp+0x36DC).
 * External globals
 *
 * Address:   0x8006D8E8, Size: 136 bytes (34 instructions)
 * Mangled:   Create_GetModelIndex__Fi
 * Demangled: int Create_GetModelIndex(int)
 * @param i Index of the body-part group whose model name is requested.
 * @return Matching model slot, or the next generated build slot.
 */

int Create_GetModelIndex(int i)
{
    int n = 0;
    int step1 = i << 2;
    SPart *base = Part;
    int offset = (step1 + i) << 2;
    // Direct `&Part[i]` changes six target instructions.
    SPart *groupEntry = (SPart *)((char *)base + offset);
    // Letting GCC choose this cursor changed 9 of the function's 34 words.
    register char **searchTable asm("$17") = &Create_ManualStitchFirst;

    do
    {
        if (strstr(groupEntry->pName, *searchTable) != 0)
        {
            return n;
        }
        n++;
        searchTable++;
    } while (n < 4);

    return Create_NextBuildIndex++;
}

/**
 * Create_LoadPSH__FPc — Load and parse a PSH animation header text file
 * Loads a PSH text file that defines character animation groups. Parses it
 * line by line, extracting animation names, model names, and frame counts.
 * Stores the number of parsed entries in Create_Parts.
 * PSH text file format (null-terminated text, lines separated by newlines):
 *   Each entry: <keyword>_<name>_<framecount>\n
 *   - keyword is found via strstr with a search pattern
 *   - name is copied up to '_' delimiter into Part
 *   - frame count is read after second strstr with a different pattern
 *   - Whitespace separators are skipped via WhiteSpace() function
 * Uses strstr (0x8006C9D0) for pattern matching.
 * Uses LowerCase (0x8006CA38) to normalize the loaded file.
 * Uses WhiteSpace (0x8006CA84) to skip whitespace.
 * Part (0x800C4DD4, 0x800C4DD8 base):
 *   Stride 20 bytes per entry (5 words):
 *     +0x00: char[?] name (copied from PSH line)
 *     -0x04 (relative to table): pointer to frame count string
 * Globals:
 *   $gp+0x4AE4: file buffer pointer
 *   $gp+0x4ADC: entry count
 *   $gp+0x4AE0: final entry count (stored on return)
 * Address:   0x8006D970, Size: 404 bytes (102 instructions)
 * Mangled:   Create_LoadPSH__FPc
 * Demangled: void Create_LoadPSH(char *)
 *
 * @param pPSHName Name of the PSH text file to load.
 */
void Create_LoadPSH(char *pPSHName)
{
    char *p;
    char *g;
    char *pGroupName;

    pPSH = FileIO_OpenLoad(pPSHName);
    pPSH[FileIO_Size - 1] = '\0';
    LowerCase(pPSH);
    p = pPSH;
    NextPart = 0;

    while (*p != '\0')
    {
        p = strstr(p, SearchPattern1);
        if (p == 0)
        {
            break;
        }
        p += 0x0B;

        g = p;
        pGroupName = Part[NextPart].GroupName;
        while (*g != '_')
        {
            *pGroupName++ = *g++;
        }

        Part[NextPart].pName = p;

        while (WhiteSpace(p) == 0)
        {
            p++;
        }

        *p = '\0';
        p = strstr(p + 1, SearchPattern2);

        while (WhiteSpace(p) == 0)
        {
            p++;
        }

        p++;
        while (WhiteSpace(p) == 0)
        {
            if (*p == '\n')
            {
                break;
            }
            p++;
        }

        *p++ = '\0';
        NextPart++;
    }

    Create_Parts = NextPart;
}

/**
 * GetFrontEndName__8MySkateriP14CCreateTexSpec
 *
 * Address:   0x8006DB08
 * Mangled:   GetFrontEndName__8MySkateriP14CCreateTexSpec
 * Demangled: MySkater::GetFrontEndName(int, CCreateTexSpec *)
 *
 * @param Number Body-part group index.
 * @param pSpec First texture specification to inspect.
 * @return Matching front-end display name, or null when none exists.
 */
char *MySkater::GetFrontEndName(int Number, CCreateTexSpec *pSpec)
{
    // Direct indexed members changed 4 of 48 words; retain the two proven
    // byte-offset calculations while naming the values they address.
    int numTimes8 = Number * 8;
    int *pAnimData = (int *)((char *)AnimGroupData8 + numTimes8);
    int numTimes4 = Number * sizeof(mSelectedPart[0]);
    int *buildBase = mSelectedPart;
    int *pBuildIdx = (int *)((char *)buildBase + numTimes4);
    while (*pSpec->GetGeometryName() != 0)
    {
        char *geoName = pSpec->GetGeometryName();
        char *groupName = Create_GetGroupName((char *)*pAnimData, *pBuildIdx);
        if (strcmp(geoName, groupName) == 0)
        {
            return pSpec->GetFrontEndName();
        }
        pSpec++;
    }
    return 0;
}

/**
 * GetGroupName__8MySkateri — Look up group name for a given skater body part index
 * Wrapper that calls Create_GetGroupName with:
 *   name  = Group[Number].name (at +4 within stride-8 entries)
 *   index = this->groupIndices[Number] (at this + Number*4 + 0x2E0)
 * Body part name table at 0x800C4D6C: stride 8 bytes, name pointer at offset +4
 *
 * Address:   0x8006DBC8, Size: 68 bytes (17 instructions)
 * Mangled:   GetGroupName__8MySkateri
 * Demangled: MySkater::GetGroupName(int)
 *
 * @param Number Body-part group index.
 * @return Selected group name for that body part.
 */
char *MySkater::GetGroupName(int Number)
{
    char *name = Group[Number].pSearch;
    int index = mSelectedPart[Number];
    return Create_GetGroupName(name, index);
}

/**
 * Create_GetGroupName__8MySkaterPci — Find Nth matching group name in Part
 * Searches Part for entries matching `name` via strstr. Returns a
 * pointer to the name field (+4 within stride-20 entry) of the Nth match,
 * where N = Number. Match counter starts at -1 (first match = 0).
 * Entry count comes from PSXRegion[this->field_0x2C4].entryCount (+0x34).
 *
 * Address:   0x8006DC0C, Size: 196 bytes (49 instructions)
 * Mangled:   Create_GetGroupName__8MySkaterPci
 * Demangled: MySkater::Create_GetGroupName(char *, int)
 *
 * @param p Body-part name fragment to find.
 * @param Number Zero-based occurrence of that fragment.
 * @return Matching group name, or null when no occurrence exists.
 */
char *MySkater::Create_GetGroupName(char *p, int Number)
{
    int n = -1;
    int i = 0;
    SPSXRegion *pRegion;
    u16 NumParts;
    char *nameField;
    char **entries;

    pRegion = &PSXRegion[mSourceRegion];
    NumParts = pRegion->NumParts;

    if (NumParts != 0)
    {
        {
            SPart *part = (SPart *)Part;
            nameField = part->GroupName;
            entries = &part->pName;
        }

        do
        {
            if (strstr(*entries, p) != 0)
            {
                n++;
            }
            i++;
            if (n == Number)
                return nameField;
            nameField += sizeof(SPart);
            entries = (char **)((u8 *)entries + sizeof(SPart));
        } while (i < (int)NumParts);
    }
}

/**
 * Create_GetModelSize__FP6SModel — Calculate total mesh data size
 * Computes the total byte size of a mesh: header (0x1C) + vertices (8 each) +
 * normals (8 each) + variable-length faces. Face stride is read from the
 * upper halfword of each face's first dword, masked to 4-byte alignment.
 *
 * Address:   0x8006DCD0, Size: 72 bytes (18 instructions)
 * Mangled:   Create_GetModelSize__FP6SModel
 * Demangled: int Create_GetModelSize(SModel *)
 * @param pModel  Model data being processed.
 * @return Total byte size of the model and its variable-length face data.
 */
int Create_GetModelSize(SModel *pModel)
{
    int i = 0;
    u16 vertexCount = pModel->NumVertices;
    u16 normalCount = pModel->NumNormals;
    u16 faceCount = pModel->NumFaces;

    SVECTOR *Normals = pModel->Vertices + vertexCount;
    unsigned int *pFaceX =
        (unsigned int *)((u8 *)Normals + normalCount * sizeof(SVECTOR));

    if (faceCount != 0)
    {
        do
        {
            CreateFacePacket *face = (CreateFacePacket *)pFaceX;
            u16 faceLen = face->Size & 0xFFFC;
            i++;
            pFaceX = (unsigned int *)((u8 *)pFaceX + faceLen);
        } while (i < faceCount);
    }

    return (int)((u8 *)pFaceX - (u8 *)pModel);
}

/**
 * Create_CountString__FPci — Count matching strings in PSX region's group table
 * For a given PSX region slot, reads the entry count from PSXRegion[slot]+0x34,
 * then iterates the animation group table (0x800C4DD4, stride 20 bytes) and
 * counts how many entries contain the given string (via strstr).
 * External globals (non-$gp)
 *
 * Address:   0x8006DD18, Size: 152 bytes (38 instructions)
 * Mangled:   Create_CountString__FPci
 * Demangled: int Create_CountString(char *, int)
 * @param p Name fragment to count.
 * @param sourceRegion Region containing the part table.
 * @return Number of matching part names.
 */

int Create_CountString(char *p, int sourceRegion)
{
    int i = 0;
    int n = 0;
    SPSXRegion *pRegion =
        (SPSXRegion *)((u8 *)PSXRegion +
                       sourceRegion * sizeof(SPSXRegion));
    u16 NumParts = pRegion->NumParts;

    if (NumParts != 0)
    {
        SPart *part = Part;
        do
        {
            if (strstr(part->pName, p) != 0)
            {
                n++;
            }
            i++;
            part++;
        } while (i < NumParts);
    }

    return n;
}

/**
 * Create_CountStitches__FP6SModel — Count stitch vertices in a mesh
 * Iterates all vertices in a mesh and counts those with type bit 0 set.
 * Type bit 0 indicates "attachable/stitchable" vertices used for seaming
 * character body parts together (e.g., torso-to-legs, head-to-torso).
 * The target keeps `vertexCount` in `$v0`. Three ordinary forms (plain int,
 * plain register, and unsigned-short count) changed 14-17 of 19 instructions,
 * so this one proven allocation constraint remains load-bearing.
 *
 * Address:   0x8006DDB0, Size: 76 bytes (19 instructions)
 * Mangled:   Create_CountStitches__FP6SModel
 * Demangled: int Create_CountStitches(SModel *)
 * @param pModel  Model data being processed.
 * @return count
 */
int Create_CountStitches(SModel *pModel)
{
    int i = 0;
    SVECTOR *pVertex = pModel->Vertices;
    register int vertexCount asm("$2") = pModel->NumVertices;
    int count = 0;

    if (vertexCount != 0)
    {
        int total = vertexCount;
        do
        {
            if (pVertex->pad & 1)
            {
                count++;
            }
            i++;
            pVertex++;
        } while (i < total);
    }

    return count;
}

/**
 * __19MySkaterTextureInfoUi — Constructor for MySkaterTextureInfo(unsigned int)
 *
 * Address:   0x8006DDFC, Size: 24 bytes (6 instructions)
 * Mangled:   __19MySkaterTextureInfoUi
 * Demangled: MySkaterTextureInfo::(unsigned int)
 *            Assembly: sets vtable (gp+0x38b4), zeroes count at +0x1F0, stores id at +4
 *
 * @param self  Texture metadata being initialized.
 * @param checksum  Asset hash for content lookup.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
MySkaterTextureInfo *MySkaterTextureInfo_ctor(
    MySkaterTextureInfo *self, unsigned int checksum)
{
    self->CClass.vtable = &Create_MySkaterTextureInfoVtableRef;
    self->mPartCount = 0;
    self->mChecksum = checksum;
    return self;
}

/**
 * AddBodyPart__19MySkaterTextureInfoi — Add body part index to array, increment count
 *
 * Address:   0x8006DE14, Size: 40 bytes (10 instructions)
 * Mangled:   AddBodyPart__19MySkaterTextureInfoi
 * Demangled: MySkaterTextureInfo::AddBodyPart(int)
 *            Assembly:
 *            lw $v0, 0x1F0($a0); nop; sll $v0, 2; addu $v0, $a0, $v0; sw $a1, 0x10($v0)
 *            lw $v1, 0x1F0($a0); nop; addiu $v1, 1; jr $ra; sw $v1, 0x1F0($a0)
 *
 * @param partNum  Index of a body part that uses this texture.
 */
void MySkaterTextureInfo::AddBodyPart(int partNum)
{
    MySkaterTextureInfo *self = this;
    self->mPart[self->mPartCount] = partNum;
    self->mPartCount++;
}

/**
 * SetTextureInfo__19MySkaterTextureInfoP7Texture — Copy texture dimensions from Texture struct
 *
 * Address:   0x8006DE3C, Size: 80 bytes (20 instructions)
 * Mangled:   SetTextureInfo__19MySkaterTextureInfoP7Texture
 * Demangled: MySkaterTextureInfo::SetTextureInfo(Texture *)
 *            Assembly: reads byte offsets 0,1,0xA,0xB from texture, stores halfwords to this+8..0xE
 *
 * @param pTexture  Texture to operate on.
 */
void MySkaterTextureInfo::SetTextureInfo(Texture *pTexture)
{
    MySkaterTextureInfo *self = this;
    self->mUVRect.x = pTexture->u0 >> 1;
    self->mUVRect.y = pTexture->v0;
    self->mUVRect.w = pTexture->u3 - pTexture->u0 + 1;
    self->mUVRect.h = pTexture->v3 - pTexture->v0 + 1;
}

/**
 *
 * Address:   0x8006DE8C
 * Mangled:   __8MySkater
 * Demangled: MySkater::(void)
 *
 */
MySkater::MySkater(void)
{
    CClass.vtable = &__vt_8MySkater;

    // No body-part geometry is selected until Init populates these slots.
    {
        int i = 0;
        int *p = mSelectedPart;
        do
        {
            *p = 0;
            i++;
            p++;
        } while (i < 4);
    }

    // Set scalar fields
    mSourceRegion = -1;
    mBuildRegion = -1;
    mSelectedSkinTone = 0;
    mChecksumCount = 0;
    mppTextures = 0;
    mNumTextures = 0;
    mRebuild = 1;
    mFree = 1;

    // Initialize 9 iterations of multi-stride arrays
    {
        int i = 0;
        int val100 = 256;
        int val80 = 128;
        int *p234 = &mDesiredHSV[0].v;
        int *p230 = &mDesiredHSV[0].s;
        int *p22c = &mDesiredHSV[0].h;
        HSV_VALUE **p1e4 = mpPaletteBuffer;
        tag_S_Pal **p208 = mpPal;
        _SCreateTexList **p19c = mpTexList;
        do
        {
            *p19c = 0;
            *p208 = 0;
            *p1e4 = 0;
            *p22c = 0;
            *p230 = val100;
            *p234 = val80;
            p234 = (int *)((char *)p234 + sizeof(HSV_VALUE));
            p230 = (int *)((char *)p230 + sizeof(HSV_VALUE));
            p22c = (int *)((char *)p22c + sizeof(HSV_VALUE));
            p1e4++;
            p208++;
            i++;
            p19c++;
        } while (i < 9);
    }
}

/**
 * MySkater::~MySkater() — MySkater deleting destructor
 *
 * Address:   0x8006DF40, Size: 28 bytes (12 instructions)
 * Mangled:   _._8MySkater
 * Demangled: MySkater::~MySkater()
 *
 */
MySkater::~MySkater()
{
    CClass.vtable = &Create_MySkaterDestructorVtableRef;
}

/**
 * FindTexture__8MySkaterUi — Find texture entry by checksum in PSXRegion data
 *
 * Address:   0x8006DF70, Size: 152 bytes (38 instructions)
 * Mangled:   FindTexture__8MySkaterUi
 * Demangled: MySkater::FindTexture(unsigned int)
 *
 * @param checksum  Asset hash for content lookup.
 * @return Matching texture, or null when the checksum is absent.
 */
Texture *MySkater::FindTexture(unsigned int checksum)
{
    SPSXRegion *pRegion = &PSXRegion[mSourceRegion];
    SModel **ppModels = pRegion->ppModels;
    unsigned int *pPSX = pRegion->pPSX;
    unsigned int NumModels = ppModels[-1];
    unsigned int *ChecksumTable = Spool_SkipPackets(pPSX);
    ChecksumTable += NumModels + 1;
    unsigned int NumChecksums = ChecksumTable[-1];
    unsigned int i = 0;
    if (NumChecksums != 0)
    {
        Texture **cursor = (Texture **)ChecksumTable;
        do
        {
            Texture *tex = *cursor;
            if (tex->Checksum == checksum)
            {
                return tex;
            }
            i++;
            cursor++;
        } while (i < NumChecksums);
    }
    return 0;
}

/**
 * SaveTextures__8MySkater
 *
 * Address:   0x8006E008
 * Mangled:   SaveTextures__8MySkater
 * Demangled: MySkater::SaveTextures(void)
 *
 * Walks the source region's packed texture table and caches a lightweight
 * description for each texture used by the character editor.
 */
void MySkater::SaveTextures(void)
{
    unsigned int *pPSX = PSXRegion[mSourceRegion].pPSX;
    unsigned int *pStuff = Spool_SkipPackets(pPSX);
    int NumModels = PSXRegion[mSourceRegion].ppModels[-1];
    pStuff += NumModels;

    int Temp = *pStuff++;
    Texture **TextureLookup = (Texture **)pStuff;
    pStuff += Temp;
    Temp = *pStuff++;
    pStuff += Temp * 9;
    Temp = *pStuff++;
    pStuff += Temp * 129;

    unsigned int *pTextureInfo = pStuff;
    unsigned int NumTextures = *pTextureInfo++;
    mppTextures =
        (MySkaterTextureInfo **)Mem_New(
        NumTextures * sizeof(MySkaterTextureInfo *), 0, 1);
    mNumTextures = NumTextures;

    int i = 0;
    if (NumTextures != 0)
    {
        do
        {
            unsigned int *pTexture = (unsigned int *)(
                (unsigned char *)pPSX + *pTextureInfo++);
            unsigned int TextureIndex = pTexture[3];
            int textureSlotOffset = i * sizeof(*mppTextures);
            // Natural `&mppTextures[i]` commutes the target's `addu` operands.
            MySkaterTextureInfo **storePtr =
                (MySkaterTextureInfo **)(textureSlotOffset +
                                          (int)mppTextures);
            MySkaterTextureInfo *result =
                (MySkaterTextureInfo *)CClass::operator new(
                    sizeof(MySkaterTextureInfo));
            if (result != 0)
            {
                result = MySkaterTextureInfo_ctor(
                    result, TextureLookup[TextureIndex]->Checksum);
            }
            i++;
            *storePtr = result;
        } while ((unsigned int)i < NumTextures);
    }
}

/**
 * UpdateTextureList__8MySkater
 *
 * Address:   0x8006E160
 * Mangled:   UpdateTextureList__8MySkater
 * Demangled: MySkater::UpdateTextureList(void)
 *
 */
void MySkater::UpdateTextureList(void)
{
    unsigned int checksums[32];
    int checksumCount;
    SPSXRegion *pRegion = &PSXRegion[mSourceRegion];
    u16 numParts = pRegion->NumParts;
    SModel **ppModels = pRegion->ppModels;
    int i = 0;
    if (numParts != 0)
    {
        do
        {
            SModel *pModel = *ppModels;
            ppModels++;
            checksumCount = 0;
            BuildChecksumList(pModel, checksums, &checksumCount);
            int j = 0;
            if (checksumCount > 0)
            {
                unsigned int *pCheck = checksums;
                do
                {
                    MySkaterTextureInfo *pInfo = FindTextureInfo(*pCheck);
                    j++;
                    pInfo->AddBodyPart(i);
                    Texture *tex = FindTexture(*pCheck);
                    pInfo->SetTextureInfo(tex);
                    pCheck++;
                } while (j < checksumCount);
            }
            i++;
        } while (i < (int)pRegion->NumParts);
    }
}

/**
 * FindTextureInfo__8MySkaterUi — Find texture info entry by checksum
 *
 * Address:   0x8006E264, Size: 80 bytes (20 instructions)
 * Mangled:   FindTextureInfo__8MySkaterUi
 * Demangled: MySkater::FindTextureInfo(unsigned int)
 *            Assembly: iterates pointer array at this+0x2D0, count at this+0x2D4,
 *            returns entry where entry->0x04 == checksum, or 0 if not found
 *
 * @param checksum  Asset hash for content lookup.
 * @return Matching texture metadata, or null when absent.
 */
MySkaterTextureInfo *MySkater::FindTextureInfo(unsigned int checksum)
{
    // An unpinned loop bound changed 18 of this function's 20 words.
    register int count asm("$2") = mNumTextures;
    if (count > 0)
    {
        int i = 0;
        int max = count;
        MySkaterTextureInfo **ptr = mppTextures;
        do
        {
            MySkaterTextureInfo *entry;

            entry = *ptr;
            if (entry->mChecksum == checksum)
            {
                return entry;
            }
            i++;
            ptr++;
        } while (i < max);
    }
    return 0;
}

/**
 * BuildChecksumList__8MySkaterP6SModelPUiPi
 *
 * Address:   0x8006E2B4
 * Mangled:   BuildChecksumList__8MySkaterP6SModelPUiPi
 * Demangled: MySkater::BuildChecksumList(SModel *, unsigned int *, int *)
 *            Iterates faces in a model, for each face checks if the face's texture tpage
 *            matches any loaded texture entry. If so, adds the texture checksum to the list
 *            (if not already present). Uses two matching modes: flag 0x20 checks tpage+xor,
 *            otherwise checks UV bounds.
 *
 * @param pModel Model whose faces reference the textures.
 * @param pChecksumList Destination list of unique texture checksums.
 * @param pChecksumCount Current destination count, updated for new entries.
 */
void MySkater::BuildChecksumList(SModel *pModel, unsigned int *pChecksumList,
                                 int *pChecksumCount)
{
    unsigned int NumModels;
    unsigned int *ChecksumTable;
    unsigned int NumChecksums;
    SVECTOR *Normals;
    unsigned int *pFace;
    int i;

    unsigned int *pPSX = PSXRegion[mSourceRegion].pPSX;
    NumModels = (unsigned int)PSXRegion[mSourceRegion].ppModels[-1];
    ChecksumTable = Spool_SkipPackets(pPSX);
    i = 0;
    Normals = pModel->Vertices + pModel->NumVertices;
    pFace = (unsigned int *)(Normals + pModel->NumNormals);
    unsigned int NumFaces = pModel->NumFaces;
    ChecksumTable += NumModels + 1;
    NumChecksums = ChecksumTable[-1];

    while (i < (int)NumFaces)
    {
        unsigned char v0;
        unsigned char u0;
        unsigned int face;
        unsigned short tpage;
        int match;

        CreateFacePacket *packet = (CreateFacePacket *)pFace;
        face = *pFace;
        tpage = packet->TexturePage;
        v0 = packet->V0;
        u0 = packet->U0;

        {
            int i = 0;
            while ((unsigned int)i < NumChecksums)
            {
                if ((*pFace & 0x20) != 0)
                {
                    match = 0;
                    if (tpage == ((Texture **)ChecksumTable)[i]->tpage)
                    {
                        match = (((Texture **)ChecksumTable)[i]->TexWin ==
                                 packet->TextureWindow);
                    }
                }
                else
                {
                    Texture *pTexture = ((Texture **)ChecksumTable)[i];
                    unsigned short minU = pTexture->u0;
                    unsigned short maxU = pTexture->u3;
                    unsigned short minV = pTexture->v0;
                    unsigned short textureTPage = pTexture->tpage;
                    unsigned short maxV = pTexture->v3;

                    match = 0;
                    if (tpage == textureTPage &&
                        minU <= u0 && u0 <= maxU &&
                        minV <= v0 && v0 <= maxV)
                    {
                        match = 1;
                    }
                }

                if (match != 0)
                {
                    int foundInChecksums = 0;
                    for (int j = 0; j < *pChecksumCount; j++)
                    {
                        if (pChecksumList[j] ==
                            ((Texture **)ChecksumTable)[i]->Checksum)
                        {
                            foundInChecksums = 1;
                        }
                    }

                    if (foundInChecksums == 0)
                    {
                        pChecksumList[*pChecksumCount] =
                            ((Texture **)ChecksumTable)[i]->Checksum;
                        (*pChecksumCount)++;
                        NumFaces = pModel->NumFaces;
                        face = *pFace;
                    }
                    break;
                }

                i++;
            }
        }
        i++;
        pFace += face >> 18;
    }
}

/**
 * Init__8MySkaterPc
 *
 * Address:   0x8006E4A8
 * Mangled:   Init__8MySkaterPc
 * Demangled: MySkater::Init(char *)
 *
 * @param pAnimFile  AnimFile to operate on.
 */
void MySkater::Init(char *pAnimFile)
{
    SModel **modelPointers = (SModel **)Mem_New(
        20 * sizeof(SModel *), 1, 1);
    Create_IsBuildRegion = 1;
    mBuildRegion = Spool_PSX(pAnimFile, 0, 0, 1);
    Spool_Sync();
    Create_IsBuildRegion = 0;
    Create_IsSourceRegion = 1;
    mSourceRegion = Spool_PSX(sSk2Def, 0, 0, 0);
    Spool_Sync();
    SPSXRegion *sourceRegion = &PSXRegion[mSourceRegion];
    u16 numParts = sourceRegion->NumParts;
    Create_IsSourceRegion = 0;
    mModelRegionPartCount = numParts;
    UpdateTextureList();
    Create_LoadPSH(sSk2DefPSH);
    // A direct typed SPSXRegion view changed one word and five relocations.
    // The target forms each ppModels slot from this shared interior base.
    char *psxModels = (char *)&PSXRegion[0].ppModels;
    int buildReg = mBuildRegion;
    SModel ***modelsSlot =
        (SModel ***)(psxModels + (buildReg << 6));
    *modelsSlot = modelPointers;
    {
        int slotAddr2 =
            (int)(psxModels + (mBuildRegion << 6));
        SModel **ppBuildModels = *(SModel ***)slotAddr2;
        ppBuildModels++;
        *(SModel ***)slotAddr2 = ppBuildModels;
    }
    {
        int slotAddr3 =
            (int)(psxModels + (mBuildRegion << 6));
        // Either unpin changes two of the function's 84 words.
        register int *models3 asm("$5") =
            (int *)*(SModel ***)slotAddr3;
        register int count asm("$4") = 19;
        models3[-1] = count;
    }
    buildReg = mBuildRegion;
    SModel ***modelsSlot4 =
        (SModel ***)(psxModels + (buildReg << 6));
    SModel **dst = *modelsSlot4;
    int i = 0;
    do
    {
        *dst = 0;
        i++;
        dst++;
    } while (i < 19);
    mRebuild = 1;
    mFree = 0;
}

/**
 * UpdateStats__8MySkaterPci — Check part name patterns and store model index into stat slots
 *
 * Address:   0x8006E5F8, Size: 152 bytes (38 instructions)
 * Mangled:   UpdateStats__8MySkaterPci
 * Demangled: MySkater::UpdateStats(char *, int)
 *
 * @param partName Model part name used to identify the stat slot.
 * @param modelIndex Model index stored in the matched slot.
 */
void MySkater::UpdateStats(char *partName, int modelIndex)
{
    if (strstr(partName, (char *)sBodyPart_Board1) != 0)
        ((CreateStatsEntry *)Stats)[13].Board = modelIndex;
    if (strstr(partName, (char *)sBodyPart_Board2) != 0)
        ((CreateStatsEntry *)Stats)[13].Wheel1 = modelIndex;
    if (strstr(partName, (char *)sBodyPart_Board3) != 0)
        ((CreateStatsEntry *)Stats)[13].Wheel2 = modelIndex;
    if (strstr(partName, (char *)sBodyPart_Head) != 0)
        ((CreateStatsEntry *)Stats)[13].Head = modelIndex;
}

/**
 * Rebuild__8MySkaterP6CSuperT1i
 *
 * Address: 0x8006E694
 *          Rebuilds the skater model: turns off all parts, enables selected head/torso/legs/board,
 *          assigns model indices, copies meshes, patches stitches, creates manual stitches,
 *          then either spools or replaces textures.
 * Mangled: Rebuild__8MySkaterP6CSuperT1i
 *
 * @param pSourceSkater Source skater whose model state is rebuilt.
 * @param pBuildSkater Destination skater used by the stitch passes.
 * @param spoolTextures Select texture spooling instead of in-place replacement.
 */
void MySkater::Rebuild(CSuper *pSourceSkater, CSuper *pBuildSkater,
                       int spoolTextures)
{
    SModel **ppSourceModels;
    SModel **ppBuildModels;

    if (mRebuild == 0)
        goto done;

    Create_Off();

    // Head
    {
        char *name = Create_GetGroupName(
            Group[0].pSearch, mSelectedPart[0]);
        Create_OnHead(name);
    }
    // Torso
    {
        char *name = Create_GetGroupName(
            Group[1].pSearch, mSelectedPart[1]);
        Create_OnTorso(name);
    }
    // Legs
    {
        char *name = Create_GetGroupName(
            Group[2].pSearch, mSelectedPart[2]);
        Create_OnLegs(name);
    }
    // Board
    {
        char *name = Create_GetGroupName(
            Group[3].pSearch, mSelectedPart[3]);
        Create_OnBoard(name);
    }

    // Reserve the four manually stitched model-name slots.
    Create_NextBuildIndex = 4;
    ppSourceModels = PSXRegion[mSourceRegion].ppModels;
    ppBuildModels = PSXRegion[mBuildRegion].ppModels;
    PSXRegion[mBuildRegion].NumParts = 0;
    // Get internal stitches
    GetInternalStitches();

    // Iterate source region entries
    if (PSXRegion[mSourceRegion].NumParts != 0)
    {
        int i = 0;
        SPart *animGroup = Part;
        do
        {
            SModel *pModel = *ppSourceModels++;
            // Check if entry is hidden (flag 0x20)
            if (pModel->Flags & 0x20)
                goto skipEntry;

            {
                int animIndex = Create_GetAnimIndex(i);
                int modelIndex = Create_GetModelIndex(i);
                // Store anim/model indices
                mAnimOrder[modelIndex] = animIndex;
                mStitchDatabase[i].buildIndex = (short)modelIndex;
                // UpdateStats
                UpdateStats(animGroup->pName, modelIndex);

                if (modelIndex != -1)
                {
                    // Free old model if any
                    if (ppBuildModels[modelIndex] != 0)
                    {
                        Mem_Delete(ppBuildModels[modelIndex]);
                    }
                    // Get model size and allocate copy
                    int modelSize = Create_GetModelSize(pModel);
                    ppBuildModels[modelIndex] =
                        (SModel *)Mem_New(modelSize, 0, 1);
                    Mem_Copy(ppBuildModels[modelIndex], pModel,
                                    modelSize);

                    // Increment build region entry count
                    PSXRegion[mBuildRegion].NumParts++;
                }
            }
        skipEntry:
            animGroup++;
            i++;
        } while (i < PSXRegion[mSourceRegion].NumParts);
    }

    PatchInternalStitches();
    CreateManualStitches(pSourceSkater, 0, 1);
    CreateManualStitches(pSourceSkater, 2, 3);

    if (spoolTextures != 0)
    {
        SpoolTextures();
    }
    else
    {
        ReplaceTextures();
    }

done:
    mRebuild = 0;
}

/**
 * TextureIsInBodyPart__8MySkaterUiPc
 *
 * Address:   0x8006E95C
 * Mangled:   TextureIsInBodyPart__8MySkaterUiPc
 * Demangled: MySkater::TextureIsInBodyPart(unsigned int, char *)
 *            Checks if a texture (by checksum) belongs to a body part (by name).
 *            Iterates texture info entries, for matching checksums iterates body part indices,
 *            looks up anim group name and calls strstr to match against part name.
 *
 * @param checksum  Asset hash for content lookup.
 * @param part  Body-part name matched against animation group names.
 * @return 1 when the texture belongs to the requested body part; otherwise 0.
 */
int MySkater::TextureIsInBodyPart(unsigned int checksum, char *part)
{
    for (int i = 0; i < mNumTextures; i++)
    {
        if (mppTextures[i]->mChecksum == checksum)
        {
            for (int j = 0; j < mppTextures[i]->mPartCount; j++)
            {
                int bodyPart = mppTextures[i]->mPart[j];
                if (strstr(Part[bodyPart].pName, part) != 0)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/**
 * FindUniqueTextureBySize__8MySkaterPci
 *
 * Address:   0x8006EA8C
 * Mangled:   FindUniqueTextureBySize__8MySkaterPci
 * Demangled: MySkater::FindUniqueTextureBySize(char *, int)
 *            Finds a unique texture matching part name. If multiple match, returns largest
 *            (findLarger != 0) or smallest (findLarger == 0) by pixel area.
 *
 * @param inPart  Build-part name whose source animation group is searched.
 * @param findLarger  Select the largest match when multiple textures qualify.
 * @return The sole matching checksum, or the largest/smallest matching checksum.
 */
unsigned int MySkater::FindUniqueTextureBySize(char *inPart, int findLarger)
{
    int numFound = 0;
    unsigned int checksum = 0;
    int sourceIndex = GetSourceIndex(FindBuildIndex(inPart));
    int largestChecksum = 0;
    int smallestChecksum = 0;
    unsigned int largestSize = 0;
    unsigned int smallestSize = (unsigned int)-1;

    for (int j = 0; j < mNumTextures; j++)
    {
        if (TextureIsInBodyPart(
                mppTextures[j]->mChecksum,
                Part[sourceIndex].pName) != 0)
        {
            checksum = mppTextures[j]->mChecksum;
            MySkaterTextureInfo *myTexInfo = FindTextureInfo(checksum);
            int size = myTexInfo->mUVRect.w * myTexInfo->mUVRect.h;
            if (largestSize < (unsigned int)size)
            {
                largestChecksum = checksum;
                largestSize = size;
            }
            if ((unsigned int)size < smallestSize)
            {
                smallestChecksum = checksum;
                smallestSize = size;
            }
            numFound++;
        }
    }

    if (numFound == 1)
        return checksum;
    if (findLarger != 0)
        return largestChecksum;
    return smallestChecksum;
}

/**
 * FindUniqueTexture__8MySkaterPcN21
 *
 * Address: 0x8006EC04
 *          Finds a texture checksum unique to 'part' that does NOT also match 'skin'.
 *          If no unique match, tries excluding 'unused' as well.
 *          Multi-pass: first by part alone, then by part+skin exclusion,
 *          then by part+skin+unused exclusion.
 * Mangled: FindUniqueTexture__8MySkaterPcN21
 *
 * @param inPart  Body part whose textures are candidates.
 * @param notInPart  Body part excluded on the second and third passes.
 * @param notInPart2  Optional second exclusion used by the fallback passes.
 * @return The checksum when a pass finds exactly one candidate, or the last
 *         candidate considered if no pass is unique.
 */
extern "C" char FindUniquePrintFmt[]; // 0x800B2CE4 — debug print format
unsigned int MySkater::FindUniqueTexture(char *inPart, char *notInPart,
                                         char *notInPart2)
{
    int numFound = 0;
    unsigned int checksum = 0;
    int sourceIndex = GetSourceIndex(FindBuildIndex(inPart));
    int sourceIndex2 = GetSourceIndex(FindBuildIndex(notInPart));

    {
        for (int j = 0; j < mNumTextures; j++)
        {
            if (TextureIsInBodyPart(mppTextures[j]->mChecksum,
                                    Part[sourceIndex].pName) != 0)
            {
                numFound++;
                checksum = mppTextures[j]->mChecksum;
            }
        }
    }

    if (numFound == 1)
    {
        return checksum;
    }

    int oldNumFound = numFound;
    {
        numFound = 0;
        for (int j = 0; j < mNumTextures; j++)
        {
            if (TextureIsInBodyPart(mppTextures[j]->mChecksum,
                                    Part[sourceIndex].pName) != 0)
            {
                if (TextureIsInBodyPart(mppTextures[j]->mChecksum,
                                        Part[sourceIndex2].pName) == 0)
                {
                    numFound++;
                    checksum = mppTextures[j]->mChecksum;
                }
            }
        }
    }

    if (numFound == 1)
    {
        return checksum;
    }

    if (notInPart2 != 0)
    {
        int sourceIndex3 = GetSourceIndex(FindBuildIndex(notInPart2));
        {
            numFound = 0;
            for (int j = 0; j < mNumTextures; j++)
            {
                if (TextureIsInBodyPart(mppTextures[j]->mChecksum,
                                        Part[sourceIndex].pName) != 0)
                {
                    if (TextureIsInBodyPart(mppTextures[j]->mChecksum,
                                            Part[sourceIndex2].pName) == 0)
                    {
                        if (TextureIsInBodyPart(
                                mppTextures[j]->mChecksum,
                                Part[sourceIndex3].pName) == 0)
                        {
                            numFound++;
                            checksum = mppTextures[j]->mChecksum;
                        }
                    }
                }
            }
        }
    }

    if (numFound == 1)
    {
        return checksum;
    }

    if (notInPart2 != 0)
    {
        int sourceIndex3 = GetSourceIndex(FindBuildIndex(notInPart2));
        {
            numFound = 0;
            for (int j = 0; j < mNumTextures; j++)
            {
                if (TextureIsInBodyPart(mppTextures[j]->mChecksum,
                                        Part[sourceIndex].pName) != 0)
                {
                    if (TextureIsInBodyPart(
                            mppTextures[j]->mChecksum,
                            Part[sourceIndex3].pName) == 0)
                    {
                        numFound++;
                        checksum = mppTextures[j]->mChecksum;
                    }
                }
            }
        }
    }

    if (numFound == 1)
    {
        return checksum;
    }

    printf(FindUniquePrintFmt, numFound, oldNumFound, inPart, notInPart);
    return checksum;
}

/**
 * FindBuildIndex__8MySkaterPc
 *
 * Address:   0x8006F020
 * Mangled:   FindBuildIndex__8MySkaterPc
 * Demangled: MySkater::FindBuildIndex(char *)
 *
 * @param name Animation name fragment to find.
 * @return Build-order slot for the matching animation, or -1.
 */
int MySkater::FindBuildIndex(char *name)
{
    int animIndex = -1;
    int i = 0;
    char **pName = BodyPart;
    do
    {
        if (strstr(*pName, name) != 0)
        {
            animIndex = i;
        }
        i++;
        pName++;
    } while (i < 0x13);

    int n = 0;
    int *buildArr = mAnimOrder;
    do
    {
        if (*buildArr == animIndex)
        {
            return n;
        }
        n++;
        buildArr++;
    } while (n < 0x13);
    return -1;
}

/**
 * Select the source texture checksum replaced by one editor texture slot.
 *
 * Address: 0x8006F0C8
 * Mangled: GetChecksumToReplace__8MySkateri
 *
 * @param texPart Texture-part slot from zero through eight.
 * @return Checksum of the source texture selected for replacement.
 */
extern "C" char sBodyPart_Pelvis[];
extern "C" char sBodyPart_Chest[];
extern "C" char sBodyPart_Bicep[];
extern "C" char sBodyPart_Forearm[];
extern "C" char sBodyPart_Hand[];
extern "C" char sBodyPart_Thigh[];
extern "C" char sBodyPart_Shin[];
extern "C" char sBodyPart_Shoe[];
extern "C" char sBodyPart_Torso1[];
extern "C" char sBodyPart_Legs2[];
unsigned int MySkater::GetChecksumToReplace(int texPart)
{
    if ((unsigned int)texPart < 9)
    {
        switch (texPart)
        {
        case 0:
            return FindUniqueTextureBySize(sBodyPart_Head, 1);
        case 1:
            return FindUniqueTextureBySize(sBodyPart_Head, 0);
        case 2:
            return FindUniqueTexture(sBodyPart_Chest, sBodyPart_Bicep, 0);
        case 3:
        {
            char *bodyPart = sBodyPart_Bicep;
            char *excludedPart = sBodyPart_Chest;
            _SCreateTexList *texEntry = mpTexList[4];
            char *optionalPart;
            if (texEntry != 0)
            {
                optionalPart = sBodyPart_Forearm;
            }
            else
            {
                optionalPart = 0;
            }
            return FindUniqueTexture(bodyPart, excludedPart, optionalPart);
        }
        case 4:
            return FindUniqueTexture(sBodyPart_Forearm, sBodyPart_Hand, 0);
        case 5:
            return FindUniqueTexture(sBodyPart_Hand, sBodyPart_Forearm, 0);
        case 6:
            return FindUniqueTexture(sBodyPart_Thigh, sBodyPart_Pelvis, 0);
        case 7:
            return FindUniqueTexture(sBodyPart_Shin, sBodyPart_Thigh, 0);
        case 8:
            return FindUniqueTexture(sBodyPart_Shoe, sBodyPart_Shin, 0);
        }
    }
}

/**
 * GetSkinToneFileName__8MySkaterPc — Modify filename[7] with skin tone digit
 *
 * Address:   0x8006F220, Size: 76 bytes (19 instructions)
 * Mangled:   GetSkinToneFileName__8MySkaterPc
 * Demangled: MySkater::GetSkinToneFileName(char *)
 *            Sets filename[7] to (skinTone + '1'), checks if file exists.
 *            If not, restores original char. Returns filename pointer.
 *
 * @param filename  Filename string.
 * @return Filename with the requested skin-tone digit when that file exists.
 */
char *MySkater::GetSkinToneFileName(char *filename)
{
    u8 origChar = filename[7];
    u8 skinTone = mSelectedSkinTone;
    filename[7] = skinTone + '1';
    if (FileIO_Exists(filename) == 0)
    {
        filename[7] = origChar;
    }
    return filename;
}

/**
 * SpoolTextures__8MySkater
 *
 * Address:   0x8006F26C
 * Mangled:   SpoolTextures__8MySkater
 * Demangled: MySkater::SpoolTextures(void)
 *            For each of 9 texture slots, get checksum, find filename, spool it.
 *            Special case for slot 2: also spool torso texture.
 *
 */
extern "C" char sCreateTexFmt[]; // 0x800EB3F4 format string "%s.bmp"
void MySkater::SpoolTextures(void)
{
    char FileNameWithExt[32];
    char oldSubDir[16];
    mSpoolerEntryCount = 0;
    strcpy(oldSubDir, FileIO_GetSubDir());
    FileIO_SetSubDir(CreateSubDir);
    int i = 0;
    HSV_VALUE **paletteSlot = mpPaletteBuffer;
    _SCreateTexList **texList = mpTexList;
    do
    {
        _SCreateTexList *texEntry = *texList;
        if (texEntry == 0)
            goto skipslot;
        if (i == 8)
            goto skipslot;
        {
            sprintf(FileNameWithExt, sCreateTexFmt, texEntry->TextureName);
            int checksum = GetChecksumToReplace(i);
            char *skinName = GetSkinToneFileName(FileNameWithExt);
            AddSpoolerEntry(checksum, skinName, i);
            if (i == 2)
            {
                // Also handle torso arms texture
                int checksum2 = FindUniqueTexture(sBodyPart_Pelvis, sBodyPart_Thigh, 0);
                _SCreateTexList *torsoTexture = mpTexList[2];
                sprintf(FileNameWithExt, sCreateTexFmt, torsoTexture->TextureName);
                char *skinName2 = GetSkinToneFileName(FileNameWithExt);
                AddSpoolerEntry(checksum2, skinName2, 8);
            }
        }
    skipslot:
    {
        HSV_VALUE **modelBuf = paletteSlot;
        HSV_VALUE *oldBuf = *modelBuf;
        if (oldBuf != 0)
        {
            Mem_Delete(oldBuf);
        }
    }
        *paletteSlot = 0;
        paletteSlot++;
        i++;
        texList++;
    } while (i < 9);

    AddSpoolerEntries();
    FileIO_SetSubDir(oldSubDir);
}

/**
 * ReplaceTextures__8MySkater
 *
 * Address:   0x8006F3E8
 * Mangled:   ReplaceTextures__8MySkater
 * Demangled: MySkater::ReplaceTextures(void)
 *            Replaces textures for each slot: if the texture changed, load from file and update.
 *            Special handling for slot 2 (torso) and skin tone '0'.
 *
 */
void MySkater::ReplaceTextures(void)
{
    char FileNameWithExt[32];
    char oldSubDir[16];
    strcpy(oldSubDir, FileIO_GetSubDir());
    FileIO_SetSubDir(CreateSubDir);
    int i = 0;
    int zeroChar = '0';
    _SCreateTexList **texList = mpTexList;
    do
    {
        _SCreateTexList *texEntry = *texList;
        int texOff = i << 2;
        if (texEntry == 0)
            goto skipslot;
        if (i == 8)
            goto skipslot;
        {
            char *oldListBase = (char *)mpOldList;
            _SCreateTexList **oldList =
                (_SCreateTexList **)(oldListBase + texOff);
            _SCreateTexList *oldTex = *oldList;
            if (oldTex == texEntry)
            {
                // Same texture - check if skin changed
                int skinChanged = mSkinToneChanged;
                if (skinChanged == 0)
                    goto skipslot;
            }
            // Load and replace
            sprintf(FileNameWithExt, sCreateTexFmt, texEntry->TextureName);
            int checksum = GetChecksumToReplace(i);
            char *skinName = GetSkinToneFileName(FileNameWithExt);
            ReplaceTexture(checksum, skinName);
            *oldList = *texList;

            if (i == 2)
            {
                // Also handle arms texture
                int checksum2 = FindUniqueTexture(sBodyPart_Pelvis, sBodyPart_Thigh, 0);
                _SCreateTexList *torsoTexture = mpTexList[2];
                sprintf(FileNameWithExt, sCreateTexFmt, torsoTexture->TextureName);
                char *skinName2 = GetSkinToneFileName(FileNameWithExt);
                ReplaceTexture(checksum2, skinName2);
                // Check if skin tone is '0'
                _SCreateTexList *torsoTexture2 = mpTexList[2];
                u8 nameChar = torsoTexture2->TextureName[7];
                if (nameChar == zeroChar)
                {
                    RememberPalette(8, checksum2);
                    goto skipslot2;
                }
                {
                    HSV_VALUE *oldBuf = mpPaletteBuffer[8];
                    if (oldBuf != 0)
                    {
                        Mem_Delete(oldBuf);
                    }
                }
                mpPaletteBuffer[8] = 0;
            }
        }
    skipslot2:
        // Check skin tone for current slot
        {
            _SCreateTexList *texListCur = *texList;
            u8 nameChar = texListCur->TextureName[7];
            if (nameChar == zeroChar)
            {
                RememberPalette(i, -1);
                goto nextslot;
            }
            {
                char *paletteBase = (char *)mpPaletteBuffer;
                HSV_VALUE **modelSlot =
                    (HSV_VALUE **)(paletteBase + texOff);
                HSV_VALUE *oldModel = *modelSlot;
                if (oldModel != 0)
                {
                    Mem_Delete(oldModel);
                }
                *modelSlot = 0;
            }
        }
    skipslot:
    nextslot:
        texList++;
        i++;
    } while (i < 9);

    FileIO_SetSubDir(oldSubDir);
}

/**
 * RefreshTexture__FUiPc — Refresh a texture from file: find it, then DownloadBMP
 *
 * Address:   0x8006F610, Size: 92 bytes (23 instructions)
 * Mangled:   RefreshTexture__FUiPc
 * Demangled: RefreshTexture(unsigned int, char *)
 *
 * @param Checksum  Asset hash for content lookup.
 * @param pBuf  Buf to operate on.
 */
void RefreshTexture(unsigned int Checksum, char *pBuf)
{
    Texture *pTexture = Spool_FindTextureEntry(Checksum);
    if (pTexture != 0)
    {
        int X = pTexture->x;
        int Y = pTexture->y;
        int ClutX = Pal256X;
        tag_S_Pal *palette = pTexture->pPalette;
        int ClutY = Pal256Y + palette->slot;
        DownloadBMP(pBuf, X, Y, ClutX, ClutY);
    }
}

/**
 * AddSpoolerEntry__8MySkaterUiPci
 *
 * Address:   0x8006F66C
 * Mangled:   AddSpoolerEntry__8MySkaterUiPci
 * Demangled: MySkater::AddSpoolerEntry(unsigned int, char *, int)
 *
 * @param checksum  Asset hash for content lookup.
 * @param filename  Filename string.
 * @param texPart Texture-part slot refreshed when the file arrives.
 */
void MySkater::AddSpoolerEntry(unsigned int checksum, char *filename, int texPart)
{
    mSpoolerEntryList[mSpoolerEntryCount].checksum = checksum;
    strcpy(mSpoolerEntryList[mSpoolerEntryCount].filename, filename);
    mSpoolerEntryList[mSpoolerEntryCount].texPart = texPart;
    mSpoolerEntryCount++;
}

/**
 * AddSpoolerEntries__8MySkater — Submit all spooler entries to the Spooler
 *
 * Address:   0x8006F70C, Size: 132 bytes (33 instructions)
 * Mangled:   AddSpoolerEntries__8MySkater
 * Demangled: MySkater::AddSpoolerEntries(void)
 *
 */
void MySkater::AddSpoolerEntries(void)
{
    int count = mSpoolerEntryCount;
    int loopCount;
    int i = 0;
    mSpoolSetDone = 0;
    if (count > 0)
    {
        int *storePtr = &mSpoolerEntryList[0].spoolID;
        char *namePtr = mSpoolerEntryList[0].filename;
        do
        {
            char *filename = namePtr;
            namePtr += sizeof(_SpoolerEntry);
            i++;
            int result = Spooler::SpoolMgr->AddFile(filename, 0);
            *storePtr = result;
            storePtr = (int *)((char *)storePtr + sizeof(_SpoolerEntry));
            loopCount = mSpoolerEntryCount;
        } while (i < loopCount);
    }
}

/**
 * ProcessSpoolerEntry__8MySkater
 *
 * Address:   0x8006F790
 * Mangled:   ProcessSpoolerEntry__8MySkater
 * Demangled: MySkater::ProcessSpoolerEntry(void)
 *            Process all spooler entries: check if file is loaded, get file buffer,
 *            refresh texture, free buffer, maybe remember palette.
 *
 */
void MySkater::ProcessSpoolerEntry(void)
{
    if (mSpoolSetDone != 0)
        return;
    int count = mSpoolerEntryCount;
    if (count <= 0)
        goto alldone;
    {
        int i = 0;
        unsigned int *checksumPtr = &mSpoolerEntryList[0].checksum;
        int *texPartPtr = &mSpoolerEntryList[0].texPart;
        int *handlePtr = &mSpoolerEntryList[0].spoolID;
        do
        {
            int loaded = Spooler::SpoolMgr->IsFileLoaded(*handlePtr);
            if (loaded == 0)
                goto next;
            {
                char *theData =
                    (char *)Spooler::SpoolMgr->GetFile(*handlePtr);
                int checksum = *checksumPtr;
                RefreshTexture(checksum, theData);
                Mem_Delete(theData);
                int texPart = *(int *)texPartPtr;
                if (texPart == 8)
                {
                    unsigned int *entryChecksum = checksumPtr;
                    RememberPalette(2, *entryChecksum);
                }
                else
                {
                    _SCreateTexList *texEntry = mpTexList[texPart];
                    u8 nameChar = texEntry->TextureName[7];
                    if (nameChar == '0')
                    {
                        RememberPalette(texPart, -1);
                    }
                }
            }
        next:
            checksumPtr = (unsigned int *)((char *)checksumPtr +
                                           sizeof(_SpoolerEntry));
            texPartPtr = (int *)((char *)texPartPtr + sizeof(_SpoolerEntry));
            count = mSpoolerEntryCount;
            i++;
            handlePtr = (int *)((char *)handlePtr + sizeof(_SpoolerEntry));
        } while (i < count);
    }
alldone:
    mSpoolSetDone = Spooler::SpoolMgr->IsSetDone();
}

/**
 * Free2__8MySkater — Free PSH buffer, texture info entries, and model arrays
 *
 * Address:   0x8006F8D0, Size: 228 bytes (57 instructions)
 * Mangled:   Free2__8MySkater
 * Demangled: MySkater::Free2(void)
 */
void MySkater::Free2(void)
{
    Mem_Delete(pPSH);
    int count = mNumTextures;
    pPSH = 0;
    if (count > 0)
    {
        int i = 0;
        do
        {
            // Natural cursor allocation changes four of the 57 words.
            register MySkaterTextureInfo **arr asm("$3") = mppTextures;
            int offset = i << 2;
            // Natural `&arr[i]` commutes two target `addu` instructions.
            MySkaterTextureInfo **slot =
                (MySkaterTextureInfo **)(offset + (int)arr);
            MySkaterTextureInfo *entry = *slot;
            if (entry != 0)
            {
                int *vtable = entry->CClass.vtable;
                // GCC otherwise changes two words around the virtual call.
                register void (*dtor)(MySkaterTextureInfo *, int) asm("$3") =
                    (void (*)(MySkaterTextureInfo *, int))vtable[2];
                dtor(entry, 3);
                arr = mppTextures;
                slot = (MySkaterTextureInfo **)(offset + (int)arr);
            }
            *slot = 0;
            {
                // An unpinned reload changes three words at the loop latch.
                register int cnt asm("$3") = mNumTextures;
                i++;
                if (i >= cnt)
                    break;
            }
        } while (1);
    }
    void *arr2 = mppTextures;
    int j = 0;
    mNumTextures = 0;
    Mem_Delete(arr2);
    HSV_VALUE **modelPtr = mpPaletteBuffer;
    mppTextures = 0;
    do
    {
        HSV_VALUE *model = *modelPtr;
        if (model != 0)
        {
            Mem_Delete(model);
            *modelPtr = 0;
        }
        j++;
        modelPtr++;
    } while (j < 9);
}

/**
 * Free__8MySkater — Free all skater resources, clear region, delete models
 *
 * Address:   0x8006F9B4, Size: 296 bytes (74 instructions)
 * Mangled:   Free__8MySkater
 * Demangled: MySkater::Free(void)
 */
void MySkater::Free(void)
{
    ClearRegion(mSourceRegion, 1, 0);
    PSXRegion[mSourceRegion].Filename[0] = 0;
    SModel **entries = PSXRegion[mBuildRegion].ppModels;
    int i = 0;
    SModel **ppModels = entries - 1;
    SModel **ppBuildModels = entries;
    do
    {
        Mem_Delete(*ppBuildModels);
        i++;
        *ppBuildModels = 0;
        ppBuildModels++;
    } while (i < 19);
    Mem_Delete(ppModels);
    int buildSlot2;
    {
        int buildSlot = mBuildRegion;
        if (PSXRegion[buildSlot].Usable != 2)
            DecrementTextureUsage(buildSlot);
    }
    buildSlot2 = mBuildRegion;
    Mem_Delete(PSXRegion[buildSlot2].pPSX);
    PSXRegion[mBuildRegion].Filename[0] = 0;
    Spool_RemoveUnusedTextures();
    Pal_RemoveUnusedPalettes();
    Pack_DefragmentFully();
    mFree = 1;
    Free2();
}

/**
 * GetFirstTexture__8MySkaterP14CCreateTexSpeciii — Find first matching texture
 *
 * Address:   0x8006FADC, Size: 280 bytes (70 instructions)
 * Mangled:   GetFirstTexture__8MySkaterP14CCreateTexSpeciii
 * Demangled: MySkater::GetFirstTexture(CCreateTexSpec *, int, int, int)
 *
 * @param pCurrSpec First texture specification to inspect.
 * @param bodyGroup Body-part group matched against the specification.
 * @param index Texture-list slot within the specification.
 * @param lastTexture Nonzero to return the final entry in the list.
 * @return First or final matching texture entry.
 */
_SCreateTexList *MySkater::GetFirstTexture(
    CCreateTexSpec *pCurrSpec, int bodyGroup, int index, int lastTexture)
{
    _SCreateTexList *pTexList =
        pCurrSpec->GetTextureList(index);
    goto check;
body:
{
    char *geoName = pCurrSpec->GetGeometryName();
    char *groupName = GetGroupName(bodyGroup);
    if (strcmp(geoName, groupName) != 0)
        goto nomatch;
    pTexList = pCurrSpec->GetTextureList(index);
    if (pTexList == 0)
        return 0;
    if (lastTexture != 0)
    {
        _SCreateTexList *pNextList;
        goto walkcheck;
    walkloop:
        pTexList = pNextList;
    walkcheck:
        pNextList = pTexList + 1;
        if (pNextList->TextureName != 0)
            goto walkloop;
    }
    return pTexList;
}
nomatch:
    pCurrSpec++;
check:
    if (*pCurrSpec->GetGeometryName() != 0)
        goto body;
    {
        char *groupName2 = GetGroupName(bodyGroup);
        printf(ErrorMsg_GetFirstTexture, groupName2);
    }
    return pTexList;
}

/**
 * UpdateGeometry__8MySkateri — Load first textures for each body group slot
 *
 * Address:   0x8006FBF4, Size: 348 bytes (87 instructions)
 * Mangled:   UpdateGeometry__8MySkateri
 * Demangled: MySkater::UpdateGeometry(int)
 *
 * @param bodyGroup Body-part group whose texture lists are initialized.
 */
extern "C" CCreateTexSpec HeadSpec[];
extern "C" CCreateTexSpec UpperBodySpec[];
extern "C" CCreateTexSpec LowerBodySpec[];
void MySkater::UpdateGeometry(int bodyGroup)
{
    if (bodyGroup == 0)
    {
        mpTexList[0] = GetFirstTexture(HeadSpec, 0, 0, 0);
        mpTexList[1] = GetFirstTexture(HeadSpec, 0, 1, 0);
    }
    else if (bodyGroup == 1)
    {
        mpTexList[2] = GetFirstTexture(UpperBodySpec, 1, 0, 0);
        mpTexList[3] = GetFirstTexture(UpperBodySpec, 1, 1, 0);
        mpTexList[4] = GetFirstTexture(UpperBodySpec, 1, 2, 0);
        mpTexList[5] = GetFirstTexture(UpperBodySpec, 1, 3, 0);
    }
    else if (bodyGroup == 2)
    {
        mpTexList[6] = GetFirstTexture(LowerBodySpec, 2, 0, 0);
        mpTexList[7] = GetFirstTexture(LowerBodySpec, 2, 1, 0);
        mpTexList[8] = GetFirstTexture(LowerBodySpec, 2, 2, 0);
    }
}

/**
 * UpdateDefaultHue__8MySkateri — Set default hue for matching spec entries
 *
 * Address:   0x8006FD48, Size: 484 bytes (121 instructions)
 * Mangled:   UpdateDefaultHue__8MySkateri
 * Demangled: MySkater::UpdateDefaultHue(int)
 *
 * @param bodyGroup Body-part group whose default hues are refreshed.
 */
void MySkater::UpdateDefaultHue(int bodyGroup)
{
    CCreateTexSpec *pCurrSpec;
    switch (bodyGroup)
    {
    case 0:
        pCurrSpec = HeadSpec;
        while (*pCurrSpec->GetGeometryName() != 0)
        {
            char *geoName = pCurrSpec->GetGeometryName();
            char *groupName = GetGroupName(bodyGroup);
            if (strcmp(geoName, groupName) == 0)
            {
                mDesiredHSV[0].h = pCurrSpec->GetDefaultHue();
                mDesiredHSV[1].h = pCurrSpec->GetDefaultHue();
            }
            pCurrSpec++;
        }
        break;
    case 1:
        pCurrSpec = UpperBodySpec;
        while (*pCurrSpec->GetGeometryName() != 0)
        {
            char *geoName = pCurrSpec->GetGeometryName();
            char *groupName = GetGroupName(bodyGroup);
            if (strcmp(geoName, groupName) == 0)
            {
                mDesiredHSV[2].h = pCurrSpec->GetDefaultHue();
                mDesiredHSV[3].h = pCurrSpec->GetDefaultHue();
                mDesiredHSV[4].h = pCurrSpec->GetDefaultHue();
                mDesiredHSV[5].h = pCurrSpec->GetDefaultHue();
            }
            pCurrSpec++;
        }
        break;
    case 2:
        pCurrSpec = LowerBodySpec;
        while (*pCurrSpec->GetGeometryName() != 0)
        {
            char *geoName = pCurrSpec->GetGeometryName();
            char *groupName = GetGroupName(bodyGroup);
            if (strcmp(geoName, groupName) == 0)
            {
                mDesiredHSV[6].h = pCurrSpec->GetDefaultHue();
                mDesiredHSV[7].h = pCurrSpec->GetDefaultHue();
                mDesiredHSV[8].h = pCurrSpec->GetDefaultHue();
            }
            pCurrSpec++;
        }
        break;
    }
}

/**
 * NextGeometry__8MySkaterii — Cycle to next/prev geometry for a body group
 *
 * Address:   0x8006FF2C, Size: 268 bytes (67 instructions)
 * Mangled:   NextGeometry__8MySkaterii
 * Demangled: MySkater::NextGeometry(int, int)
 *
 * @param bodyGroup Body-part group whose geometry selection is cycled.
 * @param forwards Nonzero to advance; zero to rewind.
 */
void MySkater::NextGeometry(int bodyGroup, int forwards)
{
    // Direct `&mSelectedPart[bodyGroup]` changed 10 of 68 words; preserve
    // the target's explicit byte-offset pointer formation in both branches.
    if (forwards != 0)
    {
        int partOff = bodyGroup << 2;
        char *partBase = (char *)mSelectedPart;
        int *pIndex = (int *)(partBase + partOff);
        int idx = *pIndex + 1;
        *pIndex = idx;
        int maxCount = Group[bodyGroup].Num;
        if (idx >= maxCount)
        {
            *pIndex = 0;
        }
    }
    else
    {
        int partOff = bodyGroup << 2;
        char *partBase = (char *)mSelectedPart;
        int *pIndex = (int *)(partBase + partOff);
        int idx = *pIndex - 1;
        *pIndex = idx;
        if (idx < 0)
        {
            int maxCount = Group[bodyGroup].Num;
            maxCount--;
            *pIndex = maxCount;
        }
    }
    UpdateGeometry(bodyGroup);
    UpdateDefaultHue(bodyGroup);
    if (bodyGroup == 0)
    {
        _SCreateTexList *modelList = mpTexList[0];
        int newVal = modelList->TextureName[7] - '1';
        int oldSkinTone = mSelectedSkinTone;
        mSelectedSkinTone = newVal;
        if (oldSkinTone != newVal)
        {
            mSkinToneChanged = 1;
        }
    }
    mRebuild = 1;
    mInheritBicep = 1;
    mInheritForearm = 1;
}

/**
 * NextTexture__8MySkaterii — Cycle a character-create texture selection.
 *
 * Resolves the texture specification, body group, and texture index for the
 * requested slot, then advances or rewinds that slot's texture-list entry.
 * The face slot delegates to NextFace because it also cycles skin tones.
 *
 * Address:   0x8007003C, Size: 488 bytes (122 instructions)
 * Mangled:   NextTexture__8MySkaterii
 * Demangled: MySkater::NextTexture(int, int)
 *
 * @param texPart Texture slot to cycle.
 * @param forwards Nonzero to advance; zero to rewind.
 */
void MySkater::NextTexture(int texPart, int forwards)
{
    CCreateTexSpec *pSpec;
    int bodyGroup;
    int index;

    switch (texPart)
    {
    case 0:
        pSpec = HeadSpec;
        bodyGroup = 0;
        index = 0;
        break;
    case 1:
        pSpec = HeadSpec;
        bodyGroup = 0;
        index = 1;
        break;
    case 2:
        pSpec = UpperBodySpec;
        bodyGroup = 1;
        index = 0;
        break;
    case 3:
        pSpec = UpperBodySpec;
        bodyGroup = 1;
        index = 1;
        break;
    case 4:
        pSpec = UpperBodySpec;
        bodyGroup = 1;
        index = 2;
        break;
    case 6:
        pSpec = LowerBodySpec;
        bodyGroup = 2;
        index = 0;
        break;
    case 7:
        pSpec = LowerBodySpec;
        bodyGroup = 2;
        index = 1;
        break;
    case 8:
        pSpec = LowerBodySpec;
        bodyGroup = 2;
        index = 2;
        break;
    }

    if (forwards != 0)
    {
        if (texPart == 0)
        {
            NextFace(pSpec, forwards);
        }
        else
        {
            mpTexList[texPart]++;
            if (mpTexList[texPart]->TextureName == 0)
            {
                mpTexList[texPart] = GetFirstTexture(
                    pSpec, bodyGroup, index, 0);
            }
        }
    }
    else if (texPart == 0)
    {
        NextFace(pSpec, 0);
    }
    else
    {
        if (mpTexList[texPart] == GetFirstTexture(
                pSpec, bodyGroup, index, 0))
        {
            mpTexList[texPart] = GetFirstTexture(
                pSpec, bodyGroup, index, 1);
        }
        else
        {
            mpTexList[texPart]--;
        }
    }
    mRebuild = 1;
}

/**
 * NextFace__8MySkaterP14CCreateTexSpeci
 *
 * Address:   0x80070224
 * Mangled:   NextFace__8MySkaterP14CCreateTexSpeci
 * Demangled: MySkater::NextFace(CCreateTexSpec *, int)
 *            Cycles through skin tone variants for the current texture.
 *
 * @param pSpec Character-create texture specification to cycle.
 * @param forwards Nonzero to advance; zero to rewind.
 */
extern "C" char sNextFaceFmt[]; // 0x800DB3FC "%s face "
extern "C" char sNextFaceFwd[]; // 0x800DB408 forward direction word
extern "C" char sNextFaceBwd[]; // 0x800DB410 backward direction word
void MySkater::NextFace(CCreateTexSpec *pSpec, int forwards)
{
    int oldSkinTone = mSelectedSkinTone;
    char oldSubDir[16];

    printf(sNextFaceFmt, forwards ? sNextFaceFwd : sNextFaceBwd);
    int textureIndex = 0;
    strcpy(oldSubDir, FileIO_GetSubDir());
    FileIO_SetSubDir(CreateSubDir);

    while (1)
    {
        if (forwards != 0)
        {
            mSelectedSkinTone++;
        }
        else
        {
            mSelectedSkinTone--;
        }

        {
            if ((unsigned int)mSelectedSkinTone > 2)
            {
                if (forwards != 0)
                {
                    mpTexList[textureIndex]++;
                    if (mpTexList[textureIndex]->TextureName == 0)
                    {
                        mpTexList[textureIndex] =
                            GetFirstTexture(
                                pSpec, 0, 0, 0);
                    }
                    mSelectedSkinTone = (unsigned char)
                        mpTexList[textureIndex]->TextureName[7] - '1';
                    break;
                }
                else
                {
                    if (mpTexList[textureIndex] ==
                        GetFirstTexture(
                            pSpec, 0, 0, 0))
                    {
                        mpTexList[textureIndex] =
                            GetFirstTexture(
                                pSpec, 0, 0, 1);
                    }
                    else
                    {
                        mpTexList[textureIndex]--;
                    }
                    mSelectedSkinTone = 3;
                }
            }
            else
            {
                {
                    char textureName[32];
                    sprintf(textureName, sCreateTexFmt,
                            mpTexList[textureIndex]->TextureName);
                    textureName[7] = (char)mSelectedSkinTone + '1';
                    if (FileIO_Exists(textureName) != 0)
                    {
                        break;
                    }
                }
            }
        }
    }

    if (oldSkinTone != mSelectedSkinTone)
    {
        mSkinToneChanged = 1;
    }
    FileIO_SetSubDir(oldSubDir);
}

/**
 * Create_Off__8MySkater — Set flag bit 0x20 on all model entries to hide them
 *
 * Address:   0x80070428, Size: 76 bytes (19 instructions)
 * Mangled:   Create_Off__8MySkater
 * Demangled: MySkater::Create_Off(void)
 *            Assembly: iterates PSXRegion[this->0x2C4] entry pointer array at slot+0x10,
 *            count at slot+0x34, sets *(u16*)entry |= 0x20 for each entry
 */
void MySkater::Create_Off(void)
{
    int region = mSourceRegion;
    SPSXRegion *pRegion = &PSXRegion[region];
    u16 NumParts = pRegion->NumParts;
    // An unpinned model cursor changed 6 of the function's 19 words.
    register SModel **ppModels asm("$5") = pRegion->ppModels;
    if (NumParts != 0)
    {
        int i = 0;
        do
        {
            SModel *pModel = *ppModels;
            ppModels++;
            i++;
            pModel->Flags |= 0x20;
        } while (i < NumParts);
    }
}

/**
 * Create_On__8MySkaterPcT1 — Enable model entries matching body part pattern
 *
 * Address:   0x80070474, Size: 248 bytes (62 instructions)
 * Mangled:   Create_On__8MySkaterPcT1
 * Demangled: MySkater::Create_On(char *, char *)
 *
 * @param p1 Group-name prefix.
 * @param p2 Body-part suffix.
 */
extern "C" char CreateSubDir2[];
void MySkater::Create_On(char *p1, char *p2)
{
    char buf[32];
    SPSXRegion *pRegion = &PSXRegion[mSourceRegion];
    SModel **ppModels = pRegion->ppModels;
    u16 Parts = pRegion->NumParts;
    sprintf(buf, CreateSubDir2, p1);
    int i = 0;
    if (Parts == 0)
        return;
    SPart *group = Part;
    do
    {
        SModel *pModel = *ppModels;
        ppModels++;
        char *groupName = group->pName;
        char *s1 = strstr(groupName, buf);
        char *s2 = strstr(group->pName, p2);
        if (s1 != 0)
        {
            if (s2 != 0)
            {
                if (group->pName == s1)
                {
                    pModel->Flags &= ~0x20;
                }
            }
        }
        i++;
        group++;
    } while (i < Parts);
}

/**
 * Create_OnHead__8MySkaterPc — Enable head body part
 *
 * Address:   0x8007056C, Size: 36 bytes (9 instructions)
 * Mangled:   Create_OnHead__8MySkaterPc
 * Demangled: MySkater::Create_OnHead(char *)
 *            Calls Create_On with one body part string
 *
 * @param p Selected head group name.
 */
void MySkater::Create_OnHead(char *p)
{
    Create_On(p, sBodyPart_Head);
}

/**
 * Create_OnBoard__8MySkaterPc — Enable board body parts
 *
 * Address:   0x80070590, Size: 96 bytes (24 instructions)
 * Mangled:   Create_OnBoard__8MySkaterPc
 * Demangled: MySkater::Create_OnBoard(char *)
 *            Calls Create_On 3 times with different board part strings
 *
 * @param p Selected board group name.
 */
void MySkater::Create_OnBoard(char *p)
{
    Create_On(p, sBodyPart_Board1);
    Create_On(p, sBodyPart_Board2);
    Create_On(p, sBodyPart_Board3);
}

/**
 * Enable every torso model belonging to the selected group.
 *
 * Address: 0x800705F0, Size: 192 bytes (48 instructions)
 * Mangled: Create_OnTorso__8MySkaterPc
 *
 * @param p Selected torso group name.
 */
extern "C" char sBodyPart_Torso3[];
extern "C" char sBodyPart_Torso4[];
extern "C" char sBodyPart_Torso5[];
extern "C" char sBodyPart_Torso6[];
extern "C" char sBodyPart_Torso7[];
extern "C" char sBodyPart_Torso8[];
void MySkater::Create_OnTorso(char *p)
{
    Create_On(p, sBodyPart_Torso1);
    Create_On(p, sBodyPart_Chest);
    Create_On(p, sBodyPart_Torso3);
    Create_On(p, sBodyPart_Torso4);
    Create_On(p, sBodyPart_Torso5);
    Create_On(p, sBodyPart_Torso6);
    Create_On(p, sBodyPart_Torso7);
    Create_On(p, sBodyPart_Torso8);
}

/**
 * Enable every leg model belonging to the selected group.
 *
 * Address: 0x800706B4, Size: 176 bytes (44 instructions)
 * Mangled: Create_OnLegs__8MySkaterPc
 *
 * @param p Selected leg group name.
 */
extern "C" char sBodyPart_Legs3[];
extern "C" char sBodyPart_Legs4[];
extern "C" char sBodyPart_Legs5[];
extern "C" char sBodyPart_Legs6[];
extern "C" char sBodyPart_Legs7[];
void MySkater::Create_OnLegs(char *p)
{
    Create_On(p, sBodyPart_Pelvis);
    Create_On(p, sBodyPart_Legs2);
    Create_On(p, sBodyPart_Legs3);
    Create_On(p, sBodyPart_Legs4);
    Create_On(p, sBodyPart_Legs5);
    Create_On(p, sBodyPart_Legs6);
    Create_On(p, sBodyPart_Legs7);
}

/**
 * GetSourceIndex__8MySkateri — Find source index by short value match
 *
 * Address:   0x80070764, Size: 72 bytes (18 instructions)
 * Mangled:   GetSourceIndex__8MySkateri
 * Demangled: MySkater::GetSourceIndex(int)
 *            Assembly: searches stride-6 array at this+0x2F4, count at this+0x710,
 *            returns index where *(short*) == buildIndex, or -1 if not found
 *
 * @param buildIndex Build-model index to resolve.
 * @return Corresponding source-model index, or -1.
 */
int MySkater::GetSourceIndex(int buildIndex)
{
    // An unpinned count changed 16 of the function's 18 words.
    register int count asm("$2") = mModelRegionPartCount;
    if (count > 0)
    {
        int sourceIndex = 0;
        int max = count;
        short *ptr = &mStitchDatabase[0].buildIndex;
        do
        {
            if (*(short *)ptr == buildIndex)
            {
                return sourceIndex;
            }
            sourceIndex++;
            ptr = (short *)((char *)ptr + sizeof(_StitchEntry));
        } while (sourceIndex < max);
    }
    return -1;
}

/**
 * GetInternalStitches__8MySkater — Count stitches for all models in current region
 *
 * Address:   0x800707AC, Size: 212 bytes (53 instructions)
 * Mangled:   GetInternalStitches__8MySkater
 * Demangled: MySkater::GetInternalStitches(void)
 *            Iterates PSXRegion entries for the skater's slot, stores running stitch
 *            total and -1 marker into stride-6 array at this+0x2F0/0x2F4,
 *            calls Create_CountStitches on each model entry.
 *
 */
void MySkater::GetInternalStitches(void)
{
    int stitchCount = 0;
    for (int i = 0; i < PSXRegion[mSourceRegion].NumParts; i++)
    {
        SModel *pModel = PSXRegion[mSourceRegion].ppModels[i];
        mStitchDatabase[i].initialSourceStitch = stitchCount;
        mStitchDatabase[i].buildIndex = -1;
        stitchCount += Create_CountStitches(pModel);
    }
}

/**
 * PatchInternalStitches__8MySkater
 *
 * Address:   0x80070880
 * Mangled:   PatchInternalStitches__8MySkater
 * Demangled: MySkater::PatchInternalStitches(void)
 *            Two-pass stitch patching: first pass fills in stitch counts per build model,
 *            second pass patches vertex indices based on stitch offsets.
 */
void MySkater::PatchInternalStitches(void)
{
    {
        int stitchCount = 0;
        int i = 0;
        while (i < (int)PSXRegion[mBuildRegion].NumParts)
        {
            int sourceIndex = GetSourceIndex(i);
            SModel *pModel =
                PSXRegion[mSourceRegion].ppModels[sourceIndex];
            mStitchDatabase[sourceIndex].initialBuildStitch =
                (short)stitchCount;
            stitchCount += Create_CountStitches(pModel);
            i++;
        }
    }

    {
        int i = 0;
        int offset;
        while (i < (int)PSXRegion[mBuildRegion].NumParts)
        {
            int sourceIndex = GetSourceIndex(i);
            if (mStitchDatabase[sourceIndex].buildIndex != -1)
            {
                SModel *pModel =
                    PSXRegion[mBuildRegion].ppModels[i];
                SVECTOR *pVertex = pModel->Vertices;
                SVECTOR *pNormal = pVertex + pModel->NumVertices;
                int j = 0;

                if (pModel->NumVertices != 0)
                {
                    do
                    {
                        if ((pVertex->pad & 2) != 0)
                        {
                            int stitchedVertexIndex =
                                (int)(short)*(unsigned short *)pVertex >> 3;
                            int stitchedPartIndex = 0;
                            int k = 0;

                            while (k < (int)PSXRegion[mSourceRegion].NumParts)
                            {
                                if (mStitchDatabase[stitchedPartIndex].buildIndex != -1 &&
                                    stitchedVertexIndex <
                                        mStitchDatabase[k].initialSourceStitch)
                                {
                                    break;
                                }
                                stitchedPartIndex = k;
                                k = stitchedPartIndex + 1;
                            }

                            offset =
                                (mStitchDatabase[stitchedPartIndex].initialBuildStitch -
                                 mStitchDatabase[stitchedPartIndex].initialSourceStitch) * 8;
                            *(unsigned short *)pVertex =
                                *(unsigned short *)pVertex + offset;
                            *(unsigned short *)pNormal =
                                *(unsigned short *)pNormal + offset;
                        }
                        pVertex++;
                        j++;
                        pNormal++;
                    } while (j < (int)pModel->NumVertices);
                }
            }
            i++;
        }
    }
}

/**
 * InsertStitch__8MySkaterP7SVECTORN31
 *
 * Address: 0x80070B28
 *          Inserts a stitch between two vertex sets. Counts stitchable vertices before
 *          the new stitch vertex, then patches type-2 vertices in all build models that
 *          reference past this point.
 * Mangled: InsertStitch__8MySkaterP7SVECTORN31
 *
 * @param pNewStitchVertex  Vertex to mark as a directly stitchable endpoint.
 * @param pNewGhostVertex  Vertex that will reference the new stitch index.
 * @param pNewStitchNormal  Normal paired with the stitch vertex.
 * @param pNewGhostNormal  Normal paired with the ghost vertex.
 * @return Number of stitchable vertices preceding the new endpoint.
 */
int MySkater::InsertStitch(SVECTOR *pNewStitchVertex, SVECTOR *pNewGhostVertex,
                           SVECTOR *pNewStitchNormal, SVECTOR *pNewGhostNormal)
{
    int stitchIndex = 0;
    int found = 0;
    int n = 0;

    SModel **buildModels = PSXRegion[mBuildRegion].ppModels;

    do
    {
        SModel *pModel = *buildModels;
        int j = 0;
        u16 vertCount = pModel->NumVertices;
        SVECTOR *pVertex = pModel->Vertices;
        int vertsEnd = vertCount * 8;
        SVECTOR *pNormal = (SVECTOR *)((u8 *)pVertex + vertsEnd);
        if (vertCount == 0)
            goto nextModel;
        do
        {
            if (pVertex == pNewStitchVertex)
            {
                found = 1;
            }
            u16 pad = pVertex->pad;
            if (pad & 1)
            {
                // Type 1 (attachable)
                if (found != 0)
                    goto patchDone;
                stitchIndex++;
            }
            else if (pad & 2)
            {
                // Type 2 (reference)
                u16 vertData = *(u16 *)pVertex;
                int sVal = (int)(short)vertData >> 3;
                if (sVal >= stitchIndex)
                {
                    // Preserve the target's explicit bit toggle before testing.
                    u16 p1pad = pNewStitchVertex->pad;
                    unsigned xored = (unsigned)p1pad ^ 1;
                    if ((xored & 1) == 0)
                        goto patchDone;
                    // Patch: add 8 to vertex index and normal end
                    u16 newVert = (u16)(vertData + 8);
                    *(u16 *)pVertex = newVert;
                    u16 normEnd = *(u16 *)pNormal;
                    *(u16 *)pNormal = normEnd + 8;
                    vertCount = pModel->NumVertices;
                }
            }
        patchDone:
            pVertex++;
            j++;
            pNormal++;
        } while (j < (int)vertCount);

    nextModel:
        n++;
        buildModels++;
    } while (n < 19);

    // Mark the stitch vertex as type-1.
    pNewStitchVertex->pad = pNewStitchVertex->pad | 1;

    // Set up the ghost vertex as a type-2 reference.
    *(u16 *)pNewGhostVertex = (u16)(stitchIndex << 3);
    pNewGhostVertex->vy = 0;
    pNewGhostVertex->vz = 0;
    pNewGhostVertex->pad = pNewGhostVertex->pad | 2;

    // Mark the stitch normal as type-1.
    pNewStitchNormal->pad = pNewStitchNormal->pad | 1;

    // Set up the ghost normal as a type-2 reference.
    *(u16 *)pNewGhostNormal = (u16)(stitchIndex << 3);
    pNewGhostNormal->vy = 0;
    pNewGhostNormal->vz = 0;
    pNewGhostNormal->pad = pNewGhostNormal->pad | 2;

    return stitchIndex;
}

/**
 * CreateManualStitches__8MySkaterP6CSuperii
 *
 * Address:   0x80070C74
 * Mangled:   CreateManualStitches__8MySkaterP6CSuperii
 * Demangled: MySkater::CreateManualStitches(CSuper *, int, int)
 *            Creates manual stitches between two body parts by comparing transformed
 *            vertex positions and linking those within distance threshold.
 *
 * @param mySuper Skater whose decompressed pose transforms are tested.
 * @param partIndexA First source/build body-part index.
 * @param partIndexB Second source/build body-part index.
 */
SMatrix *Decomp_GetAnimTransform(CSuper *);
#include <ABS.H>
void MySkater::CreateManualStitches(CSuper *mySuper, int partIndexA,
                                    int partIndexB)
{
    SMatrix *pTestTransform =
        Decomp_GetAnimTransform(mySuper);

    SModel *pBuildModelJ =
        PSXRegion[mBuildRegion].ppModels[partIndexA];
    SModel *pBuildModelK =
        PSXRegion[mBuildRegion].ppModels[partIndexB];

    SModel *pCreateModelJ = PSXRegion[mSourceRegion].ppModels[
        GetSourceIndex(partIndexA)];
    SVECTOR *pVertexBuildJ = pBuildModelJ->Vertices;

    SModel *pCreateModelK = PSXRegion[mSourceRegion].ppModels[
        GetSourceIndex(partIndexB)];
    SVECTOR *pVertexCreateJ = pCreateModelJ->Vertices;
    SVECTOR *pVertexNormalBuildJ =
        pVertexBuildJ + pBuildModelJ->NumVertices;
    int j = 0;

    if (pCreateModelJ->NumVertices == 0)
    {
        return;
    }

    do
    {
        SVECTOR *pVertexCreateK = pCreateModelK->Vertices;
        int k = 0;
        SVECTOR *pVertexBuildK = pBuildModelK->Vertices;
        SVECTOR *pVertexNormalBuildK =
            pVertexBuildK + pBuildModelK->NumVertices;

        if (pCreateModelK->NumVertices != 0)
        {
            long flag;
            do
            {
                VECTOR xformedVertJ;
                VECTOR xformedVertK;

                M3dAsm_NewSetRotTransMatrix(
                    &pTestTransform[GetSourceIndex(partIndexA)]);
                RotTrans(pVertexCreateJ, &xformedVertJ, &flag);

                M3dAsm_NewSetRotTransMatrix(
                    &pTestTransform[GetSourceIndex(partIndexB)]);
                RotTrans(pVertexCreateK, &xformedVertK, &flag);

                if (abs(xformedVertJ.vx - xformedVertK.vx) < 10)
                {
                    if (abs(xformedVertJ.vy - xformedVertK.vy) < 10)
                    {
                        if (abs(xformedVertJ.vz - xformedVertK.vz) < 10)
                        {
                            InsertStitch(pVertexBuildJ, pVertexBuildK,
                                         pVertexNormalBuildJ,
                                         pVertexNormalBuildK);
                        }
                    }
                }

                pVertexCreateK++;
                pVertexBuildK++;
                pVertexNormalBuildK++;
                k++;
            } while (k < (int)pCreateModelK->NumVertices);
        }

        pVertexCreateJ++;
        pVertexBuildJ++;
        pVertexNormalBuildJ++;
        j++;
    } while (j < (int)pCreateModelJ->NumVertices);
}

/**
 * DecrementBuildTextures__8MySkater — Decrement usage count for all build textures
 *
 * Address:   0x80070F14, Size: 76 bytes (19 instructions)
 * Mangled:   DecrementBuildTextures__8MySkater
 * Demangled: MySkater::DecrementBuildTextures(void)
 *            Assembly: iterates pointer array at this+0x68C, count at this+0x70C,
 *            decrements entry->0x10 (u16), then clears count to 0
 *
 */
void MySkater::DecrementBuildTextures(void)
{
    int count = mChecksumCount;
    if (count > 0)
    {
        int i = 0;
        Texture **ptr = mpChecksumTexture;
        do
        {
            Texture *entry = *ptr;
            entry->Usage--;
            ptr++;
        } while (++i < mChecksumCount);
    }
    mChecksumCount = 0;
}

/**
 * IncrementTextureUsage__8MySkater
 *
 * Address:   0x80070F60
 * Mangled:   IncrementTextureUsage__8MySkater
 * Demangled: MySkater::IncrementTextureUsage(void)
 * Builds a checksum list for all 19 build models, then finds each checksum in
 * the source region's texture table and increments that texture's usage count.
 */
void MySkater::IncrementTextureUsage(void)
{
    int buildRegion = mBuildRegion;
    int sourceRegion = mSourceRegion;
    SModel **ppModels = PSXRegion[buildRegion].ppModels;
    unsigned int *pPSX = PSXRegion[sourceRegion].pPSX;
    SModel **sourceModels = PSXRegion[sourceRegion].ppModels;
    unsigned int NumModels = sourceModels[-1];
    unsigned int *ChecksumTable = Spool_SkipPackets(pPSX);
    ChecksumTable += NumModels + 1;
    unsigned int NumChecksums = ChecksumTable[-1];

    mChecksumCount = 0;
    for (int i = 0; i < 19; i++)
    {
        SModel *pModel = *ppModels++;
        BuildChecksumList(
            pModel,
            mChecksumList,
            &mChecksumCount);
    }

    for (int i = 0;
         i < mChecksumCount;
         i++)
    {
        for (int j = 0; (unsigned int)j < NumChecksums; j++)
        {
            if (mChecksumList[i] ==
                ((Texture **)ChecksumTable)[j]->Checksum)
            {
                mpChecksumTexture[i] = ((Texture **)ChecksumTable)[j];
                ((Texture **)ChecksumTable)[j]->Usage++;
                break;
            }
        }
    }
}

/**
 * RememberPalette__8MySkateriUi — Remember palette for a texture part
 *
 * Address:   0x800710C4, Size: 284 bytes (71 instructions)
 * Mangled:   RememberPalette__8MySkateriUi
 * Demangled: MySkater::RememberPalette(int, unsigned int)
 *
 * @param texPart Texture-part slot whose palette is remembered.
 * @param checksum  Asset hash for content lookup.
 */
void MySkater::RememberPalette(int texPart, unsigned int checksum)
{
    unsigned int textureChecksum;
    if ((int)checksum == -1)
    {
        textureChecksum = GetChecksumToReplace(texPart);
    }
    else
    {
        textureChecksum = checksum;
    }
    int j = 0;
    int count = mNumTextures;
    unsigned int paletteChecksum = 0;
    if (count > 0)
    {
        do
        {
            MySkaterTextureInfo **arr = mppTextures;
            MySkaterTextureInfo *entry = arr[j];
            int entryChecksum = entry->mChecksum;
            if (textureChecksum != (unsigned int)entryChecksum)
                goto next;
            {
                Texture *tex = Spool_FindTextureEntry(textureChecksum);
                tag_S_Pal *pPalette = tex->pPalette;
                count = mNumTextures;
                paletteChecksum = pPalette->Checksum;
            }
        next:
            j++;
        } while (j < count);
    }
    tag_S_Pal *palEntry = Pal_FindPaletteEntry(paletteChecksum);
    int texOff = texPart << 2;
    char *paletteEntryBase = (char *)mpPal;
    tag_S_Pal **paletteEntrySlot =
        (tag_S_Pal **)(paletteEntryBase + texOff);
    char *paletteBufferBase = (char *)mpPaletteBuffer;
    HSV_VALUE **paletteBufferSlot =
        (HSV_VALUE **)(paletteBufferBase + texOff);
    *paletteEntrySlot = palEntry;
    HSV_VALUE *oldModel = *paletteBufferSlot;
    if (oldModel != 0)
    {
        Mem_Delete(oldModel);
    }
    HSV_VALUE *newBuf =
        (HSV_VALUE *)Mem_New(256 * sizeof(HSV_VALUE), 0, 1);
    *paletteBufferSlot = newBuf;
    Create_RememberPalette(*paletteEntrySlot, newBuf);
    UpdatePalette(texPart);
}

/**
 * UpdatePalette__8MySkateri
 *
 * Address:   0x800711E0
 * Mangled:   UpdatePalette__8MySkateri
 * Demangled: MySkater::UpdatePalette(int)
 *            Updates HSV palette values for a texture part based on pad input.
 *            Reads pad button states from a global pad table (0x800C40C0),
 *            adjusts hue/saturation/value with clamping, then calls palette update.
 *
 * @param texPart Texture-part slot whose HSV transform is applied.
 */
void MySkater::UpdatePalette(int texPart)
{
    HSV_VALUE *pPaletteBuffer;

    if (texPart == 3)
    {
        if (Pad[0].Square.Pressed != 0 || Pad[0].Circle.Pressed != 0 ||
            Pad[0].LeftOne.Pressed != 0 || Pad[0].RightOne.Pressed != 0 ||
            Pad[0].LeftTwo.Pressed != 0 || Pad[0].RightTwo.Pressed != 0)
        {
            mInheritBicep = 0;
        }
    }

    if (texPart == 3)
    {
        if (Pad[0].Square.Pressed != 0 || Pad[0].Circle.Pressed != 0 ||
            Pad[0].LeftOne.Pressed != 0 || Pad[0].RightOne.Pressed != 0 ||
            Pad[0].LeftTwo.Pressed != 0 || Pad[0].RightTwo.Pressed != 0)
        {
            mInheritForearm = 0;
        }
    }

    if (Pad[0].Square.Pressed != 0)
    {
        mDesiredHSV[texPart].h -= 4;
    }
    if (Pad[0].Circle.Pressed != 0)
    {
        mDesiredHSV[texPart].h += 4;
    }
    if (mDesiredHSV[texPart].h < 0)
    {
        mDesiredHSV[texPart].h += 360;
    }
    if (mDesiredHSV[texPart].h >= 360)
    {
        mDesiredHSV[texPart].h -= 360;
    }

    if (Pad[0].LeftOne.Pressed != 0 && mDesiredHSV[texPart].s > 0)
    {
        mDesiredHSV[texPart].s -= 8;
    }
    if (Pad[0].RightOne.Pressed != 0 && mDesiredHSV[texPart].s < 512)
    {
        mDesiredHSV[texPart].s += 8;
    }
    if (Pad[0].LeftTwo.Pressed != 0 && mDesiredHSV[texPart].v > 0)
    {
        mDesiredHSV[texPart].v -= 2;
    }
    if (Pad[0].RightTwo.Pressed != 0 && mDesiredHSV[texPart].v < 255)
    {
        mDesiredHSV[texPart].v += 2;
    }

    if (mDesiredHSV[texPart].s < 16)
    {
        mDesiredHSV[texPart].s = 16;
    }
    if (mDesiredHSV[texPart].s > 512)
    {
        mDesiredHSV[texPart].s = 512;
    }
    if (mDesiredHSV[texPart].v < 64)
    {
        mDesiredHSV[texPart].v = 64;
    }
    if (mDesiredHSV[texPart].v > 172)
    {
        mDesiredHSV[texPart].v = 172;
    }

    pPaletteBuffer = mpPaletteBuffer[texPart];
    Create_UpdatePalette(
        mpPal[texPart], pPaletteBuffer,
        mDesiredHSV[texPart].h,
        mDesiredHSV[texPart].s,
        mDesiredHSV[texPart].v);

    if (texPart == 2)
    {
        HSV_VALUE *pPaletteBuffer;

        mDesiredHSV[8] = mDesiredHSV[2];
        pPaletteBuffer = mpPaletteBuffer[8];
        if (pPaletteBuffer != 0)
        {
            Create_UpdatePalette(
                mpPal[8], pPaletteBuffer,
                mDesiredHSV[8].h,
                mDesiredHSV[8].s,
                mDesiredHSV[8].v);
        }

        if (mInheritBicep != 0)
        {
            mDesiredHSV[3] = mDesiredHSV[2];
            pPaletteBuffer = mpPaletteBuffer[3];
            if (pPaletteBuffer != 0)
            {
                Create_UpdatePalette(
                    mpPal[3], pPaletteBuffer,
                    mDesiredHSV[3].h,
                    mDesiredHSV[3].s,
                    mDesiredHSV[3].v);
            }
        }

        if (mInheritForearm != 0)
        {
            mDesiredHSV[4] = mDesiredHSV[2];
            pPaletteBuffer = mpPaletteBuffer[4];
            if (pPaletteBuffer != 0)
            {
                Create_UpdatePalette(
                    mpPal[4], pPaletteBuffer,
                    mDesiredHSV[4].h,
                    mDesiredHSV[4].s,
                    mDesiredHSV[4].v);
            }
        }
    }
    else if (texPart == 3)
    {
        HSV_VALUE *pPaletteBuffer;

        if (mInheritForearm != 0)
        {
            mDesiredHSV[4] = mDesiredHSV[3];
            pPaletteBuffer = mpPaletteBuffer[4];
            if (pPaletteBuffer != 0)
            {
                Create_UpdatePalette(
                    mpPal[4], pPaletteBuffer,
                    mDesiredHSV[4].h,
                    mDesiredHSV[4].s,
                    mDesiredHSV[4].v);
            }
        }
    }
}

/**
 * PatchAnimationOrder__8MySkaterP6CSuper — Apply animation order to CSuper
 *
 * Address:   0x80071604, Size: 96 bytes (24 instructions)
 * Mangled:   PatchAnimationOrder__8MySkaterP6CSuper
 * Demangled: MySkater::PatchAnimationOrder(CSuper *)
 *            Iterates 19 animation slots, reading byte at this+0x5C0 (stride 4),
 *            calls SetAnimOrder(CSuper*, index, order_byte) for each.
 *
 * @param pBuildSkater  BuildSkater to operate on.
 */
void MySkater::PatchAnimationOrder(CSuper *pBuildSkater)
{
    int i = 0;
    u8 *orderPtr = (u8 *)mAnimOrder;
    do
    {
        pBuildSkater->SetAnimOrder(i & 0xFF, *orderPtr);
        orderPtr += 4;
        i++;
    } while (i < 19);
}

extern "C" ScreenElementList *ScreenElementList_ctor(
    ScreenElementList *, int, int, int, int) asm("__17ScreenElementListiiii");
extern "C" Font *Create_ConstructFont(FontStorage *) asm("__4Font");
extern "C" CreateCSuperView *CSuper_ctor(CreateCSuperView *)
    asm("__6CSuper");
#define M3DMATHS_TYPED_API
#include "M3DMATHS.h"
#undef M3DMATHS_TYPED_API
extern "C" TextMenuProperties *TextMenuProperties_ctor(
    TextMenuProperties *) asm("__18TextMenuProperties");
extern "C" VerticalMenu2 *VerticalMenu2_ctor(
    VerticalMenu2 *, int, int, int, int, int, int, int)
    asm("__13VerticalMenu2iiiiiii");
extern "C" VMenuItem *TextMenuItem_ctor(
    VMenuItem *, int, char *, TextMenuProperties *)
    asm("__12TextMenuItemiPCcR18TextMenuProperties");
extern "C" ScreenElement *TextScreenElement_ctor(
    ScreenElement *, char *, Font *, int, int, int, int)
    asm("__17TextScreenElementPcP4Fontiiii");
extern "C" ScreenElement *SpriteScreenElement_ctor(
    ScreenElement *, char *, int, int, int, int, int, int)
    asm("__19SpriteScreenElementPciiiiii");
extern "C" char sEditorBackground[] asm("s_s2bgd4_bmp_800db428");
extern "C" char sFont1[];
extern "C" char sFont2[];
extern "C" char sFont3[];
extern "C" char sFont4[];
extern "C" char sHead[] asm("s_Head_800db468");
extern "C" char sFace[] asm("s_face_800db470");
extern "C" char sCap[] asm("s_cap_800db478");
extern "C" char sUpper[] asm("s_Upper_800db480");
extern "C" char sChest[] asm("s_chest_800db488");
extern "C" char sBicep[] asm("s_bicep_800db490");
extern "C" char sForearm[] asm("s_forearm_800db498");
extern "C" char sLower[] asm("s_Lower_800db4a4");
extern "C" char sThigh[] asm("s_thigh_800db4ac");
extern "C" char sShin[] asm("s_shin_800db4b4");
extern "C" char sShoe[] asm("s_shoe_800db4bc");
extern "C" char sCreateSkaterWindow[] asm("s_s2crskw1_bmp_800db4c4");
extern "C" char sSelectPromptImage[] asm("s_s2prmpt4_bmp_800db4d4");
extern "C" char sSelectPromptText[] asm("s_select_800db4e4");
extern "C" char sChangePromptImage[] asm("s_s2prmpt5_bmp_800db4ec");
extern "C" char sChangePromptText[] asm("s_change_800db4fc");
extern "C" char sAcceptPromptImage[] asm("s_s2prmpt1_bmp_800db504");
extern "C" char sAcceptPromptText[] asm("s_accept_800db514");
extern "C" char sBackPromptImage[] asm("s_s2prmpt2_bmp_800db51c");
extern "C" char sBackPromptText[] asm("s_back_800db52c");

/**
 * Build the create-a-skater preview actors, fonts, part menu, and prompts.
 *
 * Address:   0x80071660
 * Mangled:   __15CharacterEditor
 * Demangled: CharacterEditor::CharacterEditor(void)
 *
 * @param self Implicit `this` pointer in the GCC 2.x constructor ABI.
 * @return The constructed editor.
 */
CharacterEditor *CharacterEditor_Ctor(CharacterEditor *self)
{
    CharacterEditor *editor = self;
    ScreenElement *pElement;
    /* Single loop counter reused by the Group and setXY loops: its combined
     * reference count outranks `this` in the allocno priority sort, so `this`
     * lands in $s3 and this counter in $s2 (matching the target). */
    int i;

    ScreenElementList_ctor(self, 0, 0, -1, -1);
    self->vtable = &CharacterEditor_vtable;
    Create_ConstructFont(&editor->mFont1);
    Create_ConstructFont(&editor->mFont2);
    Create_ConstructFont(&editor->mFont3);

    editor->mpSkater = &sMySkater;

    {
        CreateCSuperView *skater = static_cast<CreateCSuperView *>(
            ::CItem::operator new(sizeof(CreateCSuperView)));
        if (skater != 0)
        {
            skater = CSuper_ctor(skater);
        }
        editor->mpBuildSkater = (CSuper *)skater;
    }
    ((CreateCSuperView *)editor->mpBuildSkater)->mpLight =
        reinterpret_cast<SLight *>(&AnimGroupData8[12]);
    ((CreateCSuperView *)editor->mpBuildSkater)->Position.vx = 0x96000;

    {
        CreateCSuperView *skater = static_cast<CreateCSuperView *>(
            ::CItem::operator new(sizeof(CreateCSuperView)));
        if (skater != 0)
        {
            skater = CSuper_ctor(skater);
        }
        editor->mpSourceSkater = (CSuper *)skater;
    }
    ((CreateCSuperView *)editor->mpSourceSkater)->mpLight =
        reinterpret_cast<SLight *>(&AnimGroupData8[12]);
    {
        CreateCSuperView *sourceSkater =
            (CreateCSuperView *)editor->mpSourceSkater;
        sourceSkater->Position.vx = 0;
    }

    Create_InitSystem();
    sprintf(editor->mpSkater->mAnimName, sAnimFormatStr);

    {
        Sprite2 *background = new Sprite2(
            sEditorBackground, 1, 0x40, 0x40);
        editor->mBackground = background;
    }

    editor->mpSkater->Init(editor->mpSkater->mAnimName);

    {
        int i = 0;
        do
        {
            editor->mpSkater->mpOldList[i] = 0;
            i++;
        } while (i < 9);
    }
    editor->mpSkater->mInheritBicep = 1;
    editor->mpSkater->mInheritForearm = 1;

    CItem_Init(editor->mpBuildSkater,
                       editor->mpSkater->mAnimName);
    CItem_Init(editor->mpSourceSkater, sSk2Def);
    ((CreateCSuperView *)editor->mpBuildSkater)->mAnimSpeed = 0x10000;
    {
        CreateCSuperView *cycleSkater =
            (CreateCSuperView *)editor->mpBuildSkater;
        TimeScale = 0x100;
        reinterpret_cast<CSuper *>(cycleSkater)->CycleAnim(0, 1);
    }

    {
        i = 0;
        do
        {
            Group[i].Num = (unsigned char)Create_CountString(
                Group[i].pSearch, editor->mpSkater->mSourceRegion);
            i++;
        } while (i < 4);
    }

    M3dMaths_SetIdentityRotation(&MikeCamera[0].Transform);
    MikeCamera[0].Position.vx = 0;
    MikeCamera[0].Position.vy = -50;
    MikeCamera[0].Position.vz = -300;
    TransMatrix(&MikeCamera[0].Transform, &MikeCamera[0].Position);
    M3dInit_SetFoggingParams(10, 6000, 0x800);
    M3dInit_InitAtStart();

    *reinterpret_cast<CreateAssignableFont *>(&editor->mFont1) =
        *reinterpret_cast<CreateAssignableFont *>(
            FontManager::GetFont(sFont1));
    editor->mFont1.JustX = 0;
    editor->mFont1.JustY = 3;
    editor->mFont1.R = 0x80;
    editor->mFont1.G = 0x80;
    editor->mFont1.B = 0x80;
    editor->mFont1.Shadow = 0;
    editor->mFont1.SpaceX = 0;
    editor->mFont1.SpaceY = 0;
    editor->mFont1.Scale = 0x1000;

    *reinterpret_cast<CreateAssignableFont *>(&editor->mFont2) =
        *reinterpret_cast<CreateAssignableFont *>(
            FontManager::GetFont(sFont4));
    editor->mFont2.JustX = 0;
    editor->mFont2.JustY = 3;
    editor->mFont2.R = 0x80;
    editor->mFont2.G = 0x80;
    editor->mFont2.B = 0x80;
    editor->mFont2.Shadow = 0;
    editor->mFont2.SpaceX = 0;
    editor->mFont2.SpaceY = 0;
    editor->mFont2.Scale = 0x1000;

    *reinterpret_cast<CreateAssignableFont *>(&editor->mFont3) =
        *reinterpret_cast<CreateAssignableFont *>(
            FontManager::GetFont(sFont2));
    editor->mFont3.JustX = 0;
    editor->mFont3.JustY = 3;
    editor->mFont3.R = 0x80;
    editor->mFont3.G = 0x80;
    editor->mFont3.B = 0x80;
    editor->mFont3.Shadow = 0;
    editor->mFont3.SpaceX = 0;

    {
        VerticalMenu2 *menu =
            (VerticalMenu2 *)CClass::operator new(sizeof(VerticalMenu2));
        if (menu != 0)
        {
            menu = VerticalMenu2_ctor(
                menu, 0x3C, 0x2F, 0x60, 0x90, 0, 0, 3);
        }
        editor->mMenu = menu;
    }

    {
        TextMenuProperties *properties =
            (TextMenuProperties *)CClass::operator new(
                sizeof(TextMenuProperties));
        if (properties != 0)
        {
            properties = TextMenuProperties_ctor(properties);
        }
        editor->mProps = properties;
    }

    *reinterpret_cast<CreateAssignableFont *>(&editor->mProps->TheFont) =
        *reinterpret_cast<CreateAssignableFont *>(
            FontManager::GetFont(sFont3));
    editor->mProps->TheFont.JustX = 2;
    editor->mProps->TheFont.JustY = 3;
    editor->mProps->TheFont.R = 0xFF;
    editor->mProps->TheFont.G = 0x60;
    editor->mProps->TheFont.B = 0x20;
    editor->mProps->TheFont.Shadow = 0;
    editor->mProps->TheFont.SpaceX = 0;
    editor->mProps->BigScale = 0x1000;
    {
        TextMenuProperties *np = editor->mProps;
        np->NormalScale = 0x1000;
        {
            VMenuItem *item = (VMenuItem *)CClass::operator new(
                sizeof(CreateTextMenuItemStorage));
            if (item != 0)
            {
                item = TextMenuItem_ctor(
                    item, 0, sHead, editor->mProps);
            }
            editor->mItem[0] = item;
        }
    }

#define CREATE_TEXT_MENU_ITEM(slot, id, text)                              \
    do                                                                     \
    {                                                                      \
        VMenuItem *item = (VMenuItem *)CClass::operator new(            \
            sizeof(CreateTextMenuItemStorage));                          \
        if (item != 0)                                                     \
        {                                                                  \
            item = TextMenuItem_ctor(                                     \
                item, id, text, editor->mProps);                           \
        }                                                                  \
        editor->mItem[slot] = item;                                       \
    } while (0)

    CREATE_TEXT_MENU_ITEM(1, 1, sFace);
    CREATE_TEXT_MENU_ITEM(2, 2, sCap);
    CREATE_TEXT_MENU_ITEM(3, 3, sUpper);
    CREATE_TEXT_MENU_ITEM(4, 4, sChest);
    CREATE_TEXT_MENU_ITEM(5, 5, sBicep);
    CREATE_TEXT_MENU_ITEM(6, 6, sForearm);
    CREATE_TEXT_MENU_ITEM(7, 7, sLower);
    CREATE_TEXT_MENU_ITEM(8, 8, sThigh);
    CREATE_TEXT_MENU_ITEM(9, 9, sShin);
    CREATE_TEXT_MENU_ITEM(10, 10, sShoe);

#undef CREATE_TEXT_MENU_ITEM

    {
        int i = 0;
        do
        {
            editor->mMenu->vtable->addItem(
                editor->mMenu, editor->mItem[i]);
            i++;
        } while (i < 11);
    }

    editor->mMenu->vtable->activate(editor->mMenu);

    {
        int y;
        i = 0;
        y = 0x2F;
        do
        {
            editor->mItem[i]->vtable->setXY(
                editor->mItem[i], editor->mItem[i]->X, y);
            y += 0x0D;
            i++;
        } while (i < 11);
    }

    {
        pElement = (ScreenElement *)CClass::operator new(
            sizeof(CreateTextScreenElementStorage));
        if (pElement != 0)
        {
            pElement = TextScreenElement_ctor(
                pElement, ((CreateStatsEntry *)Stats)[13].Name,
                FontFromStorage(&editor->mFont1),
                0x42, 0x21, -1, -1);
        }
    }
    self->addItem(pElement);

    {
        pElement = (ScreenElement *)CClass::operator new(
            sizeof(CreateSpriteScreenElementStorage));
        if (pElement != 0)
        {
            pElement = SpriteScreenElement_ctor(
                pElement, sCreateSkaterWindow,
                0x32, 0x11, -0x100, -1, 0, 0);
        }
    }
    self->addItem(pElement);

#define CREATE_PROMPT(image, text, x)                                      \
    do                                                                     \
    {                                                                      \
        pElement = reinterpret_cast<ScreenElement *>(                      \
            new PromptScreenElement(                                      \
                image, text, FontFromStorage(&editor->mFont3),            \
                x, 0xDC, -1, 0x104));                                     \
        self->addItem(pElement);                                          \
    } while (0)

    CREATE_PROMPT(sSelectPromptImage, sSelectPromptText, 0x3F);
    CREATE_PROMPT(sChangePromptImage, sChangePromptText, 0xAE);
    CREATE_PROMPT(sAcceptPromptImage, sAcceptPromptText, 0x114);
    CREATE_PROMPT(sBackPromptImage, sBackPromptText, 0x17E);

#undef CREATE_PROMPT

    return self;
}

/**
 * CharacterEditor::~CharacterEditor() — Tear down the create-a-skater editor: owned UI widgets,
 * skater resources, preview actors, embedded fonts, and the ScreenElementList base.
 *
 * Address:   0x80071F84
 * Mangled:   _._15CharacterEditor
 * Demangled: CharacterEditor::~CharacterEditor()
 *
 */
CharacterEditor::~CharacterEditor()
{
    vtable = &CharacterEditor_vtable;

    CreateDeletableObject *obj = (CreateDeletableObject *)mBackground;
    if (obj != 0)
    {
        CreateDeleteVtable *vtbl = obj->vtable;
        void (*dtor)(void *, int) = vtbl->destroy;
        dtor(obj, 3);
    }

    mpSkater->Free();

    obj = (CreateDeletableObject *)mpSourceSkater;
    if (obj != 0)
    {
        CreateDeleteVtable *vtbl = ((CreateCSuperView *)obj)->vtable;
        void (*dtor)(void *, int) = vtbl->destroy;
        dtor(obj, 3);
    }

    mpSourceSkater = 0;
    obj = (CreateDeletableObject *)mpBuildSkater;
    if (obj != 0)
    {
        CreateDeleteVtable *vtbl = ((CreateCSuperView *)obj)->vtable;
        void (*dtor)(void *, int) = vtbl->destroy;
        dtor(obj, 3);
    }

    mpBuildSkater = 0;
    obj = (CreateDeletableObject *)mMenu;
    if (obj != 0)
    {
        CreateDeleteVtable *vtbl = obj->vtable;
        void (*dtor)(void *, int) = vtbl->destroy;
        dtor(obj, 3);
    }

    obj = (CreateDeletableObject *)mProps;
    if (obj != 0)
    {
        CreateDeleteVtable *vtbl = obj->vtable;
        void (*dtor)(void *, int) = vtbl->destroy;
        dtor(obj, 3);
    }

    int i = 0;
    VMenuItem **item = mItem;
    do
    {
        obj = (CreateDeletableObject *)*item;
        if (obj != 0)
        {
            CreateDeleteVtable *vtbl = obj->vtable;
            void (*dtor)(void *, int) = vtbl->destroy;
            dtor(obj, 3);
        }
        i++;
        item++;
    } while (i < 11);

    FontFromStorage(&mFont3)->~Font();
    FontFromStorage(&mFont2)->~Font();
    FontFromStorage(&mFont1)->~Font();
    ScreenElementList_base_destructor(this, 0);
}

/**
 * Per-frame update for the create-a-skater editor: run the build skater's
 * body AI, spin the model with L1/R1, rebuild geometry, advance part
 * selection from pad input, and refresh the selected palette.
 *
 * Address:   0x8007211C
 * Mangled:   update__15CharacterEditor
 * Demangled: CharacterEditor::update(void)
 */
void CharacterEditor::update(void)
{
    // MAIN.SYM names an optimized-out SVECTOR `RotAngle`; materializing that
    // declaration preserves words but introduces 12 relocation mismatches.
    CBody_InterleaveAI(reinterpret_cast<CBody *>(mpBuildSkater));

    if (Pad[0].LeftOne.Pressed != 0)
    {
        CreateCSuperView *skater = (CreateCSuperView *)mpBuildSkater;
        skater->Angles.vy += 60;
    }
    if (Pad[0].RightOne.Pressed != 0)
    {
        CreateCSuperView *skater = (CreateCSuperView *)mpBuildSkater;
        skater->Angles.vy -= 60;
    }

    mpSkater->Rebuild(mpSourceSkater, mpBuildSkater, 0);
    mpSkater->PatchAnimationOrder(mpBuildSkater);

    int prev = 0;
    int next = 0;
    if (Pad[0].Left.Triggered != 0)
    {
        SFX_PlayX(0x90, 0x2000, 0);
        Pad[0].Left.Triggered = 0;
        prev = 1;
    }
    if (Pad[0].Right.Triggered != 0 || Pad[0].X.Triggered != 0)
    {
        SFX_PlayX(0x90, 0x2000, 0);
        next = 1;
        Pad[0].Right.Triggered = 0;
        Pad[0].X.Triggered = 0;
    }
    mpSkater->mSkinToneChanged = 0;

    if (prev != 0 || next != 0)
    {
        switch (mMenu->getHighlighted())
        {
        case 0:
            mpSkater->NextGeometry(0, next);
            break;
        case 1:
            mpSkater->NextTexture(0, next);
            break;
        case 2:
            if (mpSkater->mpTexList[1] != 0)
            {
                mpSkater->NextTexture(1, next);
            }
            break;
        case 3:
            mpSkater->NextGeometry(1, next);
            break;
        case 4:
            mpSkater->NextTexture(2, next);
            break;
        case 5:
            mpSkater->NextTexture(3, next);
            break;
        case 6:
            if (mpSkater->mpTexList[4] != 0)
            {
                mpSkater->NextTexture(4, next);
            }
            break;
        case 7:
            mpSkater->NextGeometry(2, next);
            break;
        case 8:
            mpSkater->NextTexture(6, next);
            break;
        case 9:
            if (mpSkater->mpTexList[7] != 0)
            {
                mpSkater->NextTexture(7, next);
            }
            break;
        case 10:
            mpSkater->NextTexture(8, next);
            break;
        default:
            break;
        }
    }

    VerticalMenu2 *menu = mMenu;
    // Three ordinary initializations changed two instructions (or five plus
    // two relocations when reordered); the target materializes -1 in `$v1`.
    {
        register int defaultPalette asm("$3") = -1;
        PaletteSelected = defaultPalette;
    }
    switch (menu->getHighlighted())
    {
    case 0:
    case 1:
        PaletteSelected = 0;
        break;
    case 2:
        PaletteSelected = 1;
        break;
    case 3:
    case 4:
        PaletteSelected = 2;
        break;
    case 5:
        PaletteSelected = 3;
        break;
    case 6:
        if (mpSkater->mpTexList[4] != 0)
        {
            PaletteSelected = 4;
        }
        break;
    case 7:
    case 8:
        PaletteSelected = 6;
        break;
    case 9:
        if (mpSkater->mpTexList[7] != 0)
        {
            PaletteSelected = 7;
        }
        break;
    case 10:
    default:
        break;
    }
    if (mpSkater->mpPaletteBuffer[PaletteSelected] != 0)
    {
        mpSkater->UpdatePalette(PaletteSelected);
    }
    ScreenElementList::update();
}

/**
 * Draw the current character-part names, menu, skater, and background.
 *
 * Address:   0x80072430
 * Mangled:   draw__15CharacterEditorii
 * Demangled: CharacterEditor::draw(int, int)
 *
 * @param x Parent-list horizontal offset.
 * @param y Parent-list vertical offset.
 */
#include "MENUINPUT.h"

void CharacterEditor::draw(int x, int y)
{
    static_cast<Font *>(&mFont2)->draw(
        140, 55, mpSkater->GetFrontEndName(0, HeadSpec), 0);
    static_cast<Font *>(&mFont2)->draw(
        140, 68, mpSkater->mpTexList[0]->FrontEndName, 0);
    if (mpSkater->mpTexList[1] != 0)
    {
        static_cast<Font *>(&mFont2)->draw(
            140, 81, mpSkater->mpTexList[1]->FrontEndName, 0);
    }

    static_cast<Font *>(&mFont2)->draw(
        140, 94, mpSkater->GetFrontEndName(1, UpperBodySpec), 0);
    static_cast<Font *>(&mFont2)->draw(
        140, 107, mpSkater->mpTexList[2]->FrontEndName, 0);
    static_cast<Font *>(&mFont2)->draw(
        140, 120, mpSkater->mpTexList[3]->FrontEndName, 0);
    if (mpSkater->mpTexList[4] != 0)
    {
        static_cast<Font *>(&mFont2)->draw(
            140, 133, mpSkater->mpTexList[4]->FrontEndName, 0);
    }

    static_cast<Font *>(&mFont2)->draw(
        140, 146, mpSkater->GetFrontEndName(2, LowerBodySpec), 0);
    static_cast<Font *>(&mFont2)->draw(
        140, 159, mpSkater->mpTexList[6]->FrontEndName, 0);
    if (mpSkater->mpTexList[7] != 0)
    {
        static_cast<Font *>(&mFont2)->draw(
            140, 172, mpSkater->mpTexList[7]->FrontEndName, 0);
    }
    static_cast<Font *>(&mFont2)->draw(
        140, 185, mpSkater->mpTexList[8]->FrontEndName, 0);

    // MAIN.SYM records this original local as `OldJust`; the source reapplies
    // the current font alignment before handing control to the menu renderer.
    int OldJust = mFont2.JustX;
    mFont2.JustX = OldJust;
    mMenu->vtable->draw(mMenu);

    int press = Menu_GetPress();
    mMenu->vtable->processPress(mMenu, press);

    mMenu->vtable->update(mMenu);

    M3d_RenderSetup(
        MikeCamera, pViewport, pDoubleBuffer->OrderingTable);
    M3d_BuildTransform(mpBuildSkater);
    M3d_Render(mpBuildSkater);
    M3d_RenderCleanup();

    ScreenElementList::draw(mScreenX + x, mScreenY + y);

    CreateDeleteVtable *backgroundVtable = mBackground->vtable;
    backgroundVtable->drawSprite(mBackground, 0, 0, 1);
}

/**
 * handlePress__15CharacterEditor — Check button presses and play sound effects
 * Checks Pad byte fields and plays SFX_PlayX with corresponding sound IDs.
 * Returns: 0=no press, 1=field_0x31 press, 2=field_0x01 press.
 *
 * Address:   0x800726FC, Size: 168 bytes (42 instructions)
 * Mangled:   handlePress__15CharacterEditor
 * Demangled: CharacterEditor::handlePress(void)
 *
 * @param self  Implicit CharacterEditor pointer.
 * @return 0 for no action, 1 to accept, or 2 to leave the editor.
 */
int CharacterEditor_handlePress(CharacterEditor *self)
{
    (void)self;
    if (Pad[0].Up.Triggered != 0 || Pad[0].Down.Triggered != 0)
    {
        SFX_PlayX(0x90, 0x2000, 0);
    }
    if (Pad[0].X.Triggered != 0)
    {
        SFX_PlayX(0x91, 0x2000, 0);
        Pad[0].X.Triggered = 0;
        return 1;
    }
    if (Pad[0].Triangle.Triggered != 0)
    {
        SFX_PlayX(0x93, 0x2000, 0);
        Pad[0].Triangle.Triggered = 0;
        return 2;
    }
    return 0;
}

/**
 * Build the selected skater for gameplay or front-end display.
 *
 * Address: 0x800727A4
 * Mangled: Create_InitSkater__Fi
 *
 * @param forGame Nonzero to retain the gameplay animation set.
 */
void ExtraAnims_AddSkatersAnims(void);
extern "C" char sAnimGameFmt[];                           // 0x800EB534 — "anims_cs%d" format for game
// No Db_SkyColorCSV/DAT_800eb53c alias is needed in CREATE.
extern "C" struct CreateAnimHdr8 DAT_800db53c_hdr[] asm("DAT_800db53c"); // 0x800DB53C — default anim header (absolute)
#include "DEFAULTANIMATIONS.h"
void Create_InitSkater(int forGame)
{
    Spool_RemoveUnusedTextures();
    Pal_RemoveUnusedPalettes();
    Pack_DefragmentFully();

    CreateCSuperView *pSourceSkater = static_cast<CreateCSuperView *>(
        ::CItem::operator new(sizeof(CreateCSuperView)));
    if (pSourceSkater != 0)
    {
        pSourceSkater = CSuper_ctor(pSourceSkater);
    }

    pSourceSkater->mpLight =
        reinterpret_cast<SLight *>(&AnimGroupData8[12]);
    pSourceSkater->Position.vx = 100 * 4096;

    MySkater *pMySkater = &sMySkater;
    char *animName;

    if (forGame != 0)
    {
        animName = pMySkater->mAnimName;
        sprintf(animName, sAnimGameFmt);
    }
    else
    {
        animName = pMySkater->mAnimName;
        sprintf(animName, sAnimFormatStr);
    }

    pMySkater->Init(animName);

    // InitItem
    CItem_Init(pSourceSkater, sSk2Def);

    // Clear old textures for non-null slots
    {
        int i = 0;
        _SCreateTexList **oldPtr = pMySkater->mpOldList;
        _SCreateTexList **texPtr = pMySkater->mpTexList;
        do
        {
            _SCreateTexList *tex = *texPtr;
            if (tex != 0)
            {
                *oldPtr = 0;
            }
            oldPtr++;
            i++;
            texPtr++;
        } while (i < 9);
    }

    // Rebuild
    pMySkater->Rebuild(
        (CSuper *)pSourceSkater, 0, (forGame == 0) ? 1 : 0);

    // Strip anim
    Spool_StripAnim(pMySkater->mSourceRegion);

    // Default animation list = local copy of the 8-byte header at 0x800DB53C
    struct CreateAnimHdr8 FrontEndAnimations = DAT_800db53c_hdr[0];
    short *pAnimationList = (short *)&FrontEndAnimations;
    if (forGame != 0)
    {
        ExtraAnims_AddSkatersAnims();
        Spool_StripModel(pMySkater->mBuildRegion, DefaultAnimations);
    }
    else
    {
        Spool_StripModel(pMySkater->mBuildRegion, pAnimationList);
    }

    // Mark the completed build region as usable.
    PSXRegion[pMySkater->mBuildRegion].Usable = 2;

    // Increment texture usage
    pMySkater->IncrementTextureUsage();

    // ClearRegion on source
    ClearRegion(pMySkater->mSourceRegion, 1, 0);

    // Clear the source region filename marker.
    PSXRegion[pMySkater->mSourceRegion].Filename[0] = 0;

    // Destroy CSuper if valid
    if (pSourceSkater != 0)
    {
        CreateDeleteVtable *vtable2 = pSourceSkater->vtable;
        void (*dtorFn)(void *, int) = vtable2->destroy;
        dtorFn(pSourceSkater, 3);
    }

    // Free2
    pMySkater->Free2();

    Spool_RemoveUnusedTextures();
    Pal_RemoveUnusedPalettes();
    Pack_DefragmentFully();
}

/**
 * Create_InitSkaterFrontEnd__Fv
 *
 * Address:   0x800729A4
 * Mangled:   Create_InitSkaterFrontEnd__Fv
 * Demangled: Create_InitSkaterFrontEnd(void)
 *            Similar to Create_InitSkater but for front-end display.
 *            forGame=0, always uses anims_cs format, zeroes all old texture slots.
 *
 */
void Create_InitSkaterFrontEnd(void)
{
    Spool_RemoveUnusedTextures();
    Pal_RemoveUnusedPalettes();
    Pack_DefragmentFully();

    int forGame = 0;

    CreateCSuperView *pSourceSkater = static_cast<CreateCSuperView *>(
        ::CItem::operator new(sizeof(CreateCSuperView)));
    if (pSourceSkater != 0)
    {
        pSourceSkater = CSuper_ctor(pSourceSkater);
    }

    MySkater *pMySkater = &sMySkater;
    char *animName = pMySkater->mAnimName;

    pSourceSkater->mpLight =
        reinterpret_cast<SLight *>(&AnimGroupData8[12]);
    pSourceSkater->Position.vx = 100 * 4096;

    sprintf(animName, sAnimFormatStr);

    pMySkater->Init(animName);

    CItem_Init(pSourceSkater, sSk2Def);

    // Zero ALL old texture slots (unlike Create_InitSkater which conditionally zeroes)
    {
        int i = 0;
        _SCreateTexList **oldPtr = pMySkater->mpOldList;
        _SCreateTexList **texPtr = pMySkater->mpTexList;
        do
        {
            _SCreateTexList *tex = *texPtr;
            if (tex != 0)
            {
                *oldPtr = 0;
            }
            *oldPtr = 0;
            oldPtr++;
            i++;
            texPtr++;
        } while (i < 9);
    }

    // Zero again
    {
        int i = 0;
        _SCreateTexList **oldPtr = pMySkater->mpOldList;
        do
        {
            *oldPtr = 0;
            i++;
            oldPtr++;
        } while (i < 9);
    }

    // Rebuild with spool flag = 1
    pMySkater->Rebuild((CSuper *)pSourceSkater, 0, 1);

    // Strip anim
    Spool_StripAnim(pMySkater->mSourceRegion);

    // Default animation list = local copy of the 8-byte header at 0x800DB53C
    struct CreateAnimHdr8 FrontEndAnimations = DAT_800db53c_hdr[0];
    short *pAnimationList = (short *)&FrontEndAnimations;
    if (forGame != 0)
    {
        ExtraAnims_AddSkatersAnims();
        Spool_StripModel(pMySkater->mBuildRegion, DefaultAnimations);
    }
    else
    {
        Spool_StripModel(pMySkater->mBuildRegion, pAnimationList);
    }

    // Mark the completed build region as usable.
    PSXRegion[pMySkater->mBuildRegion].Usable = 2;

    // Increment texture usage
    pMySkater->IncrementTextureUsage();

    // ClearRegion on source
    ClearRegion(pMySkater->mSourceRegion, 1, 0);

    // Clear the source region filename marker.
    PSXRegion[pMySkater->mSourceRegion].Filename[0] = 0;

    // Destroy CSuper if valid
    if (pSourceSkater != 0)
    {
        CreateDeleteVtable *vtable2 = pSourceSkater->vtable;
        void (*dtorFn)(void *, int) = vtable2->destroy;
        dtorFn(pSourceSkater, 3);
    }
}

/**
 * Create_FreeSkaterFrontEnd__Fv — Free skater front-end resources
 *
 * Address:   0x80072B88, Size: 60 bytes (15 instructions)
 * Mangled:   Create_FreeSkaterFrontEnd__Fv
 * Demangled: Create_FreeSkaterFrontEnd(void)
 *            Calls Free2 on sMySkater, then cleans up textures/palettes/VRAM.
 *
 */
void Create_FreeSkaterFrontEnd(void)
{
    sMySkater.Free2();
    Spool_RemoveUnusedTextures();
    Pal_RemoveUnusedPalettes();
    Pack_DefragmentFully();
}

/**
 * Create_BuildSkater__FP6CSuper — Build skater from sMySkater data
 *
 * Address:   0x80072BC4, Size: 72 bytes (18 instructions)
 * Mangled:   Create_BuildSkater__FP6CSuper
 * Demangled: Create_BuildSkater(CSuper *)
 *            Calls PatchAnimationOrder, then InitItem with sMySkater+0x714 name
 *
 * @param pBuildSkater  BuildSkater to operate on.
 */
void Create_BuildSkater(CSuper *pBuildSkater)
{
    MySkater *pMySkater = &sMySkater;
    pMySkater->PatchAnimationOrder(pBuildSkater);
    CItem_Init(pBuildSkater, pMySkater->mAnimName);
}

/**
 * Create_FreeSkater__Fv — Free all skater build resources
 *
 * Address:   0x80072C0C, Size: 300 bytes (75 instructions)
 * Mangled:   Create_FreeSkater__Fv
 * Demangled: Create_FreeSkater(void)
 */
void Create_FreeSkater(void)
{
    MySkater *pMySkater = &sMySkater;
    if (pMySkater->mFree == 0)
    {
        SModel **entries =
            PSXRegion[pMySkater->mBuildRegion].ppModels;
        int i = 0;
        SModel **ppModels = entries - 1;
        SModel **ppBuildModels = entries;
        do
        {
            Mem_Delete((void *)*ppBuildModels);
            i++;
            *ppBuildModels = 0;
            ppBuildModels++;
        } while (i < 19);
        Mem_Delete((void *)ppModels);
        pMySkater->DecrementBuildTextures();
        int buildRegion = pMySkater->mBuildRegion;
        SPSXRegion *region = &PSXRegion[pMySkater->mBuildRegion];
        PSXRegion[pMySkater->mBuildRegion].pAnimFile = 0;
        PSXRegion[buildRegion].pColourPulseData = 0;
        PSXRegion[buildRegion].pTexWibData = 0;
        PSXRegion[buildRegion].Protected = 0;
        PSXRegion[buildRegion].Usable = 0;
        PSXRegion[buildRegion].pHooks = 0;
        PSXRegion[buildRegion].ppModels = 0;
        region->Filename[0] = 0;
        PSXRegion[buildRegion].TexturesInRAM = 0;
        PSXRegion[buildRegion].KeepTexturesInRAM = 0;
        PSXRegion[buildRegion].WhichHeap = 1;
        Mem_Delete(PSXRegion[buildRegion].pPSX);
        PSXRegion[pMySkater->mBuildRegion].pPSX = 0;
        Spool_RemoveUnusedTextures();
        Pal_RemoveUnusedPalettes();
        Pack_DefragmentFully();
        int one = 1;
        pMySkater->mFree = one;
    }
}

/**
 * Create_SaveTextures__Fv — Wrapper: calls sMySkater.SaveTextures()
 *
 * Address:   0x80072D38, Size: 36 bytes (9 instructions)
 * Mangled:   Create_SaveTextures__Fv
 * Demangled: Create_SaveTextures(void)
 *
 */
void Create_SaveTextures(void)
{
    sMySkater.SaveTextures();
}

/**
 * Create_ProcessingSourcePSX__Fv — Return Create_IsSourceRegion flag
 *
 * Address:   0x80072D5C, Size: 12 bytes (3 instructions)
 * Mangled:   Create_ProcessingSourcePSX__Fv
 * Demangled: Create_ProcessingSourcePSX(void)
 *            Assembly: lw $v0, 0x36e0($gp); jr $ra; nop
 *
 * @return Create_IsSourceRegion
 */
int Create_ProcessingSourcePSX(void)
{
    return Create_IsSourceRegion;
}

/**
 * Create_ProcessingBuildPSX__Fv — Return Create_IsBuildRegion flag
 *
 * Address:   0x80072D68, Size: 12 bytes (3 instructions)
 * Mangled:   Create_ProcessingBuildPSX__Fv
 * Demangled: Create_ProcessingBuildPSX(void)
 *            Assembly: lw $v0, 0x36e4($gp); jr $ra; nop
 *
 * @return Create_IsBuildRegion
 */
int Create_ProcessingBuildPSX(void)
{
    return Create_IsBuildRegion;
}

/**
 * Create_UseSkater__Fi — Return 1 if selectedSkater == 13 (the create-a-skater level)
 *
 * Address:   0x80072D74, Size: 12 bytes (3 instructions)
 * Mangled:   Create_UseSkater__Fi
 * Demangled: Create_UseSkater(int)
 *            Assembly: xori $v0, $a0, 0xd; jr $ra; sltiu $v0, $v0, 1
 *
 * @param selectedSkater Selected character index.
 * @return selectedSkater == 13
 */
int Create_UseSkater(int selectedSkater)
{
    return selectedSkater == 13;
}

/**
 * Create_Main__Fv
 *
 * Address:   0x80072D80
 * Mangled:   Create_Main__Fv
 * Demangled: Create_Main(void)
 *            Run the character editor, including its update/draw loop and
 *            the surrounding font, palette, texture, and sky cleanup.
 *
 * @return -1 when the editor exits through action 2; otherwise 0.
 */
#include "DB.h"
#include "FRONTEND2.h"
int Create_Main(void)
{
    Spool_RemoveUnusedTextures();
    Pal_RemoveUnusedPalettes();
    Pack_DefragmentFully();
    FontManager::LoadFont(Create_Font1Owner);
    FontManager::LoadFont(Create_Font2Owner);
    FontManager::LoadFont(Create_Font3Owner);
    FontManager::LoadFont(Create_Font4Owner);

    CharacterEditor *pEditor =
        static_cast<CharacterEditor *>(
            CClass::operator new(sizeof(CharacterEditor)));
    if (pEditor != 0)
    {
        pEditor = CharacterEditor_Ctor(pEditor);
    }

    // Save and clear Db_SkyColor
    unsigned int oldSkyColor = Db_SkyColor;
    Db_SkyColor = 0;
    Db_UpdateSky();
    Pad_ClearTriggers(Pad);
    Pad_ClearTriggers(Pad + 1);

    // Main loop
    int press;
mainloop:
    FrontEnd2_UpdateMusicLoop();
    GStartTime = Vblanks;
    Db_FlipClear();
    CalcPolyBufferEnd();
    Pad_Update();

    press = CharacterEditor_handlePress(pEditor);
    if (press == 1)
        goto exitloop;
    if (press == 2)
        goto exitloop;

    // update (vtable + 0xC)
    {
        CreateDeleteVtable *vtable = pEditor->vtable;
        void (*updateFn)(void *) = vtable->update;
        updateFn(pEditor);
    }
    // draw (vtable + 0x10)
    {
        CreateDeleteVtable *vtable = pEditor->vtable;
        void (*drawFn)(void *, int, int) = vtable->drawElement;
        drawFn(pEditor, 0, 0);
    }

    DrawSync(0);
    Pause(1);

    // Wait for 2 vblanks
    {
        int target = GStartTime + 2;
        while ((unsigned int)Vblanks < (unsigned int)target)
        {
            // busy wait
        }
    }

    Db_DisplayDraw();
    PollHost();
    goto mainloop;

exitloop:
    // Delete editor
    if (pEditor != 0)
    {
        CreateDeleteVtable *vtable = pEditor->vtable;
        void (*dtorFn)(void *, int) = vtable->destroy;
        dtorFn(pEditor, 3);
    }

    // Restore SkyColor
    Db_SkyColor = oldSkyColor;

    Spool_RemoveUnusedTextures();
    Pal_RemoveUnusedPalettes();
    Pack_DefragmentFully();

    SelectedSkater = 13;

    FontManager::UnloadFont(Create_Font1Owner);
    FontManager::UnloadFont(Create_Font2Owner);
    FontManager::UnloadFont(Create_Font3Owner);
    FontManager::UnloadFont(Create_Font4Owner);

    if (press == 2)
        return -1;
    return 0;
}

/**
 * Create_InitSystem__Fv
 *
 * Address:   0x80072FB4
 * Mangled:   Create_InitSystem__Fv
 * Demangled: Create_InitSystem(void)
 *
 */
void Create_InitSystem(void)
{
    if (GFirstTime != 0)
    {
        MySkater *mpSkater = &sMySkater;
        char *animName = mpSkater->mAnimName;
        sprintf(animName, sAnimFormatStr);
        mpSkater->Init(animName);
        mpSkater->UpdateGeometry(0);
        mpSkater->UpdateGeometry(1);
        mpSkater->UpdateGeometry(2);
        mpSkater->UpdateDefaultHue(0);
        mpSkater->UpdateDefaultHue(1);
        mpSkater->UpdateDefaultHue(2);
        int i = 0;
        _SCreateTexList **pOldList = mpSkater->mpOldList;
        do
        {
            *pOldList = 0;
            i++;
            pOldList++;
        } while (i < 9);
        char *psxPtr = (char *)&PSXRegion[0];
        char *psxBase = psxPtr + (int)&((SPSXRegion *)0)->ppModels;
        int buildSlot = mpSkater->mBuildRegion;
        _SCreateTexList **texListPtr = mpSkater->mpTexList;
        _SCreateTexList *texEntry = *texListPtr;
        int skinChar = texEntry->TextureName[7];
        SModel **models = *(SModel ***)(psxBase + (buildSlot << 6));
        mpSkater->mInheritBicep = 1;
        mpSkater->mInheritForearm = 1;
        GFirstTime = 0;
        mpSkater->mSelectedSkinTone = skinChar - '1';
        Mem_Delete(models - 1);
        int buildReg2 = mpSkater->mBuildRegion;
        int a0val = buildReg2;
        SModel ***modelsSlot =
            (SModel ***)(psxBase + (buildReg2 << 6));
        *modelsSlot = 0;
        ClearRegion(a0val, 1, 0);
        int negOne = -1;
        int sourceReg = mpSkater->mSourceRegion;
        mpSkater->mBuildRegion = negOne;
        ClearRegion(sourceReg, 1, 0);
        mpSkater->mSourceRegion = negOne;
        Spool_RemoveUnusedTextures();
        Pal_RemoveUnusedPalettes();
        Pack_DefragmentFully();
        mpSkater->Free2();
    }
}

/**
 * Create_SpoolTextures__Fv — Spool textures: update spooler, process entries, restore dir
 *
 * Address:   0x8007310C, Size: 108 bytes (27 instructions)
 * Mangled:   Create_SpoolTextures__Fv
 * Demangled: Create_SpoolTextures(void)
 *
 * @return Nonzero once the create-a-skater texture set is fully loaded.
 */
int Create_SpoolTextures(void)
{
    char oldSubDir[16];
    strcpy(oldSubDir, FileIO_GetSubDir());
    FileIO_SetSubDir(CreateSubDir);
    Spooler::SpoolMgr->Update();
    sMySkater.ProcessSpoolerEntry();
    FileIO_SetSubDir(oldSubDir);
    return Spooler::SpoolMgr->IsSetDone();
}

/**
 * Create_BeginSpooling__Fv — Begin spooling: save dir, spool, restore dir
 *
 * Address:   0x80073178, Size: 80 bytes (20 instructions)
 * Mangled:   Create_BeginSpooling__Fv
 * Demangled: Create_BeginSpooling(void)
 */
void Create_BeginSpooling(void)
{
    char oldSubDir[16];
    strcpy(oldSubDir, FileIO_GetSubDir());
    FileIO_SetSubDir(CreateSubDir);
    Spooler *spoolMgr = Spooler::SpoolMgr;
    spoolMgr->Spooool();
    FileIO_SetSubDir(oldSubDir);
}

extern "C" MySkater *Create_MySkaterCtor(MySkater *self)
    asm("__8MySkater");
extern "C" void Create_MySkaterDtor(MySkater *self, int flags)
    asm("_._8MySkater");

/** Destroy the CREATE-owned MySkater singleton during global teardown. */
extern "C" void Create_GlobalDtor_BodyPart(void)
    asm("_GLOBAL_.D.BodyPart");
void Create_GlobalDtor_BodyPart(void)
{
    Create_MySkaterDtor(&sMySkater, 2);
}

/** Construct the CREATE-owned MySkater singleton during global startup. */
extern "C" void Create_GlobalInit_BodyPart(void)
    asm("_GLOBAL_.I.BodyPart");
void Create_GlobalInit_BodyPart(void)
{
    Create_MySkaterCtor(&sMySkater);
}

/**
 * Destroy one CREATE-owned texture-info entry and optionally free its storage.
 *
 * This inline `CREATE.H` body was instantiated in the original `create.obj`.
 *
 * Address: 0x80073214
 * Mangled: _._19MySkaterTextureInfo
 */
extern "C" {
static void Create_MySkaterTextureInfoDestructor(
    MySkaterTextureInfo *self, int flag)
{
    self->CClass.vtable = &Create_MySkaterDestructorVtableRef;
    if (flag & 1)
        CClass::operator delete(self);
}

/**
 * Destroy the private `CClass` base instantiated by `create.obj`.
 *
 * Address: 0x80073244
 * Mangled: _._6CClass
 */
static void Create_CClassDestructor_80073244(CClass *self, int flag)
{
    self->vtable = &Create_MySkaterDestructorVtableRef;
    if (flag & 1)
        CClass::operator delete(self);
}
}

// === CREATE-owned initialized data (target .data 0x800C4D20..0x800C5734) ===

/**
 * Body-part search names in animation-order slot order.
 *
 * MAIN.SYM names this original 19-pointer table `BodyPart`; all recovered
 * consumers use that canonical identity directly.
 */
char *BodyPart[19] __attribute__((section(".data"))) = {
    sBodyPart_Pelvis,
    sBodyPart_Legs5,
    sBodyPart_Legs7,
    sBodyPart_Legs6,
    sBodyPart_Legs2,
    sBodyPart_Legs4,
    sBodyPart_Legs3,
    sBodyPart_Torso1,
    sBodyPart_Chest,
    sBodyPart_Torso8,
    sBodyPart_Torso7,
    sBodyPart_Torso6,
    sBodyPart_Torso5,
    sBodyPart_Torso4,
    sBodyPart_Torso3,
    sBodyPart_Head,
    sBodyPart_Board1,
    sBodyPart_Board2,
    sBodyPart_Board3,
};

/** Body-part group prefixes selected by the character editor. */
SGroup Group[4] __attribute__((section(".data"))) = {
    {0, sBodyPart_Head},
    {0, sBodyPart_Chest},
    {0, sBodyPart_Legs3},
    {0, sBodyPart_Board1},
};

/** Projection bounds used by the character-editor skater preview. */
SViewport CreateViewport __attribute__((section(".data"))) = {
    0x200, 0xF0, 0, 0,
    10, 30000, 0xD55, 0,
    0, 0,
};

/** Lighting used by the character-editor source and build skaters. */
static SLight Skater_CreateLight
    __attribute__((section(".data"))) = {
    {{0, 4096, 0}, {0, -4096, 0}, {0, 0, 4096}},
    0,
    {{640, 2944, 320}, {640, 2944, 320}, {640, 2944, 320}},
    0,
    {2176, 2176, 2176},
};

/**
 * Runtime-populated PSH part records.
 *
 * The original keeps this zero-filled storage in `.data`; loading a skater PSH
 * fills each pointer/name pair in place.
 */
SPart Part[120] __attribute__((section(".data"))) = {};

/**
 * `.ctors` table slot for this owner's hand-written global constructor.
 *
 * GCC emits this entry itself for a compiler-generated constructor, but this owner's
 * hook is written by hand, so the entry has to be declared explicitly; the
 * recovered application object order reproduces the target's table sequence.
 */
static void (*Create_GlobalCtorSlot)(void)
    __attribute__((section(".ctors"))) = Create_GlobalInit_BodyPart;

/**
 * `.dtors` table slot for this owner's hand-written global destructor.
 *
 * GCC emits this entry itself for a compiler-generated destructor, but this owner's
 * hook is written by hand, so the entry has to be declared explicitly; the
 * recovered application object order reproduces the target's table sequence.
 */
static void (*Create_GlobalDtorSlot)(void)
    __attribute__((section(".dtors"))) = Create_GlobalDtor_BodyPart;
