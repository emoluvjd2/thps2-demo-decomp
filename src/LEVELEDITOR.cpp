// LEVELEDITOR.cpp -- Reconstructed translation unit
// Original: LEVELEDITOR.CPP
// Functions: 44 reconstructed entries
// Address range: 0x80082F4C - 0x80087600
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#define LEVELEDITOR_OWNER_DECLARATIONS
#include "LEVELEDITOR.h"
#undef LEVELEDITOR_OWNER_DECLARATIONS
#include "CITEM.h"
#include "CBODY.h"
#include "SFX.h"
#include "LEVELGEN.h"
#include "MAIN.h"
#include "M3D.h"
#include "EDITORPIECES.h"
#include "PACK.h"
#include "PAD.h"
#include "PSXREGION.h"
#define SCREENTOOLS_AGGREGATE_LAYOUT
#define SCREENTOOLS_AGGREGATE_LAYOUT_ONLY
#include "SCREENTOOLS.h"
#undef SCREENTOOLS_AGGREGATE_LAYOUT_ONLY
#undef SCREENTOOLS_AGGREGATE_LAYOUT
#include "PROMPTSCREENELEMENT.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include "SMODEL.h"
#include <LIBETC.H>
#include <LIBGPU.H>

/** Placement construction for explicitly managed Font storage. */
inline void *operator new(unsigned int, void *address)
{
    return address;
}

/** Exact-size runtime camera view used by the editor's construction path. */
struct CCamera
{
    unsigned char opaque[0x684];

    CCamera(CBody *tripod, unsigned short mikeIndex);
};

typedef char level_editor_runtime_camera_size_must_be_0x684[
    (sizeof(CCamera) == 0x684) ? 1 : -1];

// === Forward type declarations ===

class EditorCamera;
class SelectManager;

/** Owner-local EditorInModel (source model) layout; matches MAIN.SYM. */
struct EditorInModel
{
    /** Toggles transparency bits on every face in this source model. */
    void SetTransparency(bool on);

    void *vtable;
    void *Faces;
    SModel *mModel;
    unsigned char mIndex;
    unsigned char indexPadding[3];
    int w;
    int h;
    int l;
    int xleft;
    int xright;
    int ztop;
    int zbottom;
    int yoff;
    int railXOff;
    int railYOff;
    int railZOff;
    void *railInfo;
    unsigned char flags;
    unsigned char flagsPadding[3];
};

typedef char editor_source_model_size_must_be_68[
    (sizeof(EditorInModel) == 68) ? 1 : -1];

/** Owner-local EditorOutModel slice used by cursor sorting and visibility. */
struct EditorOutModel
{
    unsigned char unused[0x60];
    unsigned char XCell;
    unsigned char YCell;
    unsigned char ZCell;
    unsigned char WCell;
    unsigned char HCell;
    unsigned char LCell;
    short X;
    short Y;
    short Z;
    unsigned char Rot;
    unsigned char rotPadding[3];
    EditorInModel *SourceModel;
    SModel *TargetModel;
    Item *TargetItem;
    EditorOutModel *Next;
};

typedef char editor_out_model_size_must_be_128[
    (sizeof(EditorOutModel) == 128) ? 1 : -1];

/** Opaque LevelGenerator method surface used by the level editor. */
struct LevelGenerator
{
    /** Constructs a generator for editor or in-game output. */
    LevelGenerator(bool game_build);
    /** Initializes the shared editor grid and model tables. */
    static void Init();
    /** Loads a saved park map into the shared editor state. */
    static void LoadMap(char *load_name);
    /** Selects the active editor map theme. */
    static void SetMapTheme(int theme);
    /** Releases the shared generated-level output. */
    static void UnloadOutPSX(bool kill_buffers);
    /** Loads source models for one editor theme. */
    void LoadModels(char *file);
    /** Builds a runtime PSX level image and returns its allocated word buffer. */
    unsigned int *CreatePSX(int *ret_region, bool refresh_models);
    /** Restores or hides the floor beneath one placed model. */
    void UnhideFloor(EditorOutModel *model, bool on);
    /** Updates shared faces around one placed model. */
    void UnhideFaces(EditorOutModel *model, bool on);
    /** Applies wall visibility for the active editor view. */
    void HideWalls(bool hide);
    /** Adds one source model to the editor occupancy map. */
    void AddModel(int x, int z, int slot, int rot, int index);
    /** Removes the uppermost model occupying one editor map cell. */
    int RemoveModelFromMap(
        int x, int z, int *noLowerThan, EditorInModel **removedModel);
};

#define LEVELGEN_GRID_MAP_LAYOUT
#define LEVELGEN_GRID_MAP_LAYOUT_ONLY
#include "LEVELGEN.h"
#undef LEVELGEN_GRID_MAP_LAYOUT_ONLY
#undef LEVELGEN_GRID_MAP_LAYOUT

/** LevelGenerator slice through the placed-model occupancy map. */
struct EditorLevelGeneratorView
{
    unsigned char opaqueToOutModelMap[0x280c];
    MapCell OutModelMap[24][24];
};

/** LevelGenerator slice exposing its placed-model and floor-model chains. */
struct EditorLevelGeneratorListsView
{
    unsigned char opaqueToOutModelList[0x2808];
    EditorOutModel *OutModelList;
    unsigned char opaqueToFloorModelList[0x7928 - 0x280c];
    EditorOutModel *FloorModelList;
};

typedef char editor_level_generator_lists_size_must_be_0x792c[
    (sizeof(EditorLevelGeneratorListsView) == 0x792c) ? 1 : -1];

struct ModelFace;
struct RailInfo;

#include "IMAGEBASE.h"

/** Sliced-image service view used by the editor arrows. */
struct SlicedImage2 : Image
{
    unsigned char opaque[32];

    int screenWidth();
};
/** Sprite storage with its natural sliced-image base. */
struct Sprite2 : SlicedImage2
{
    /** Allocate sprite storage through the shared CClass arena. */
    static void *operator new(unsigned int size) throw()
    {
        return ::CClass::operator new(size);
    }
    /** Build a sprite from a bitmap file. */
    Sprite2(char *name, int pack, int sliceWidth, int sliceHeight);
};

typedef char editor_sliced_image2_size_must_be_60[
    (sizeof(SlicedImage2) == 60) ? 1 : -1];
typedef char editor_sprite2_size_must_be_60[
    (sizeof(Sprite2) == 60) ? 1 : -1];
/** Pointer-only menu service used by the editor wrapper. */
struct BasicMenu
{
    /** Highlights the menu item with the requested identifier. */
    void setHighlighted(int id);
};

/** One editor source model record (EditorInModel layout, 68-byte stride). */
struct EditorSourceModelView
{
    void *CClass;
    ModelFace *Faces;
    SModel *mModel;
    unsigned char mIndex;
    int w;
    int h;
    int l;
    int xleft;
    int xright;
    int ztop;
    int zbottom;
    int yoff;
    int railXOff;
    int railYOff;
    int railZOff;
    RailInfo *railInfo;
    unsigned char flags;
};

typedef char editor_source_model_stride_must_be_68[
    (sizeof(EditorSourceModelView) == 68) ? 1 : -1];

/** LevelGenerator prefix through the editor's source-model table. */
struct EditorLevelGeneratorModelsView
{
    void *vtable;
    unsigned int NumSourceModels;
    char SourceFileName[16];
    int SourceRegion;
    EditorSourceModelView SourceModelTab[150];
};

/**
 * Generator-relative source-model view used when code keeps the generator
 * header as its addressing base while stepping by the 68-byte model stride.
 */
struct EditorSourceModelAddressView
{
    unsigned char generatorHeader[0x1c];
    EditorSourceModelView SourceModel;
};

typedef char editor_source_model_address_size_must_be_0x60[
    (sizeof(EditorSourceModelAddressView) == 0x60) ? 1 : -1];

typedef char editor_source_model_table_offset_must_be_28[
    ((sizeof(void *) + sizeof(unsigned int) + 16 + sizeof(int)) == 28)
        ? 1
        : -1];

/** Polymorphic screen element prefix used by editor menu draw calls. */
struct EditorDrawableView
{
    void **vtable;
};

/** ScreenElement prefix used to reposition the editor's controller prompts. */
struct EditorScreenElementView
{
    void **vtable;
    int mHomeX;
    int mHomeY;
};

typedef char editor_screen_element_prefix_must_be_12[
    (sizeof(EditorScreenElementView) == 12) ? 1 : -1];

/** Double-buffer slice exposing the panel ordering table at offset 0x84. */
struct EditorDoubleBufferView
{
    unsigned char opaqueToPanelOT[0x84];
    unsigned int *PanelOT;
    unsigned int *OrderingTable;
};

typedef char editor_double_buffer_prefix_size_must_be_0x8c[
    (sizeof(EditorDoubleBufferView) == 0x8c) ? 1 : -1];

#include "SCAMERA.h"

/**
 * Polymorphic Font overlay used to recover the compiler-generated assignment
 * operator.  GCC preserves the destination vtable while copying the complete
 * object image, matching the original Font value-assignment semantics.
 */
struct EditorClassAssignmentView
{
    virtual void ClassTypeAnchor();
};

/** Font value layout with its CClass base vtable at offset zero. */
struct EditorFontAssignmentView : public EditorClassAssignmentView
{
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

typedef char editor_font_assignment_size_must_be_80[
    (sizeof(EditorFontAssignmentView) == 80) ? 1 : -1];

/** CBody prefix used by the level-editor camera's focus calculations. */
struct EditorCameraBodyView
{
    unsigned short flags;
    unsigned short inquiry;
    CVector mPos;
};

/** CItem polymorphic prefix; its virtual-function table lives at offset 0x48. */
struct EditorItemVtableView
{
    unsigned char opaqueToVtable[0x48];
    void **vtable;
};

typedef char editor_item_vtable_offset_must_be_0x48[
    (sizeof(EditorItemVtableView) == 0x4c) ? 1 : -1];

/** Packed fixed-point sine/cosine entry used by the original table macros. */
struct EditorSinCos
{
    short Sin;
    short Cos;
};

/** Text-menu item prefix containing the horizontal justification field. */
struct EditorMenuItemView
{
    void **vtable;
    int unused;
    int JustX;
};

/** Image prefix containing Sprite2's byte-sized justification controls. */
struct EditorSpriteJustificationView
{
    unsigned char opaqueToJustX[0x13];
    unsigned char JustX;
    unsigned char JustY;
};

typedef char editor_sprite_justification_size_must_be_0x15[
    (sizeof(EditorSpriteJustificationView) == 0x15) ? 1 : -1];

/** TextMenuWrapper layout used by the level editor's state machine. */
struct TextMenuWrapper
{
    void **vtable;
    BasicMenu *Menu;
    int HighlightedId;
    unsigned char properties[0x80];
    EditorMenuItemView *ItemList[32];
    int NumItems;
};

typedef TextMenuWrapper EditorTextMenuView;

typedef char editor_text_menu_size_must_be_272[
    (sizeof(EditorTextMenuView) == 272) ? 1 : -1];

/**
 * Runtime-camera slice used by EditorCamera.  The named fields retain their
 * original CCamera offsets while unrelated camera state stays opaque.
 */
struct EditorRuntimeCameraView
{
    unsigned char opaqueToVtable[0x48];
    void (**vtable)(EditorRuntimeCameraView *);
    unsigned char opaqueToTripod[0x3f4 - 0x4c];
    EditorCameraBodyView *mTripod;
    EditorCameraBodyView *mOldTripod;
    void *mCalcTripod;
    CVector mOldTripodPos;
    int newFocus;
    CVector mTripodPos;
    CVector mTripodPosIncrement;
    unsigned int mTripodTime;
    EditorCameraBodyView *mFocus;
    int mCalcFocus;
    unsigned char opaqueToCameraMode[0x558 - 0x434];
    int mCameraMode;

    /** Select an explicit tripod position for the next camera update. */
    void SetTripodPosition(const CVector &position)
    {
        mCalcTripod = 0;
        mTripodPos = position;
        mTripod = 0;
        newFocus = 1;
    }

    /** Select the body that the runtime camera should follow. */
    void SetFocus(EditorCameraBodyView *focus)
    {
        mCalcFocus = 1;
        mFocus = focus;
        newFocus = 1;
    }

    /** Dispatch the camera's virtual update method. */
    void Update()
    {
        vtable[4](this);
    }
};

typedef char editor_runtime_camera_mode_offset_must_be_0x558[
    (sizeof(EditorRuntimeCameraView) == 0x55c) ? 1 : -1];

// Cursor — level-editor cursor (originally a real C++ class, retired from
// `extern "C"` mangled-name shims in Phase F1.6).  Layout matches
// the direct `MAIN.SYM` layout cache's `struct Cursor`.
struct Cursor : CClass {
    struct CursorModel Models[4];
    struct EditorOutModel (*SortList[1200]);
    int LatestModel;
    signed char CameraDirX;
    signed char CameraDirZ;
    char Pad146A[2];
    void *ResortGrid;
    int Piece;
    int Rot;
    int XRot;
    int YRot;
    int ZRot;
    int WRot;
    int HRot;
    int LRot;
    int X;
    int Y;
    int Z;
    int W;
    int H;
    int L;
    int StartAngle;
    int TargetAngle;
    int StartScreenX;
    int StartScreenZ;
    int TargetScreenX;
    int TargetScreenZ;
    int Gradient;
    void *NewLocation;
    int CameraX;
    int CameraZ;
    int StartCameraX;
    int StartCameraZ;
    int TargetCameraX;
    int TargetCameraZ;
    int Angle;
    int ScreenX;
    int ScreenY;
    int ScreenZ;
    struct LevelGenerator *Ed;
    SVector CamVect;

    Cursor(struct LevelGenerator &ed);
    ~Cursor();
    int SetPiece(int piece);
    void Rotate(int dir);
    int Move(int x, int y, int z);
    void Move(int dir);
    void SetPosition();
    void Update();
    void Draw();
    int Stamp();
    int Remove();
    void WireframeCollidingPieces(bool);
    void PushbackPieces(bool);
    void AddSortModel(struct EditorOutModel *new_model);
    void SortModels();
    void SetCameraTargetVector(SVector camera);
};

typedef char level_editor_Cursor_size_must_be_5372[(sizeof(Cursor) == 5372) ? 1 : -1];

// === Owner-local vtable anchors ===
extern int __vt_12EditorCamera;
extern int __vt_12EditorCamera_base;
extern int __vt_11LevelEditor;
extern int __vt_11LevelEditor_base;
extern int __vt_13SelectManager;
extern int __vt_13SelectManager_base;
extern int __vt_6Cursor;
extern int __vt_base_6Cursor;

// === External function declarations ===
#include <STDIO.H>

// SelectManager diagnostics retained by the original object.
extern const char DAT_800B3C18[];
extern const char s_not_available_in_demo_800b3c30[];

// Draw__11LevelEditor helpers
#include "DB.h"

// === Extern "C" forward declarations ===
// Cursor methods are declared as members on `struct Cursor` above (Phase F1.6).
#include "PDOUBLEBUFFER.h"
#define MENU_ABI_DECLARATIONS_ONLY
#define MENU_DECLARE_TEXT_WRAPPER_CONSTRUCTOR_ABI
#define MENU_TEXT_WRAPPER_CONSTRUCTOR_RECEIVER EditorTextMenuView
#include "MENU.h"
#undef MENU_ABI_DECLARATIONS_ONLY
// --- __11LevelEditor ctor dependencies ---
#include "SPOOL.h"
#include "PAL.h"
#define RCOSSIN_DECL_TYPE int
#define RCOSSIN_SCALAR_VIEW
#include "RCOSSIN.h"
#define EditorSine(angle) \
    ((*(EditorSinCos *)(&rcossin_tbl + ((angle) & 0xfff))).Sin)
#define EditorCosine(angle) \
    ((*(EditorSinCos *)(&rcossin_tbl + ((angle) & 0xfff))).Cos)
// === Function implementations (address order) ===

/**
 * LevelEditor_Main__Fv
 *
 * Address:   0x80082F4C
 * Mangled:   LevelEditor_Main__Fv
 * Demangled: LevelEditor_Main(void)
 *            Main editor loop: alloc, construct, draw/update loop, destroy
 *
 */
#include "FRONTEND2.h"
void LevelEditor_Main(void)
{
    LevelEditor *Ed = static_cast<LevelEditor *>(
        ::CClass::operator new(sizeof(LevelEditor)));
    if (Ed != 0)
    {
        Ed = ::new (Ed) LevelEditor;
    }
    do
    {
        FrontEnd2_UpdateMusicLoop();
        Ed->Draw();
    } while (Ed->Update() == 0);
    if (Ed != 0)
    {
        int *vtable = Ed->CClass.vtable;
        reinterpret_cast<void (*)(LevelEditor *, int)>(vtable[2])(Ed, 3);
    }
}

/**
 * Construct a level-editor session and its owned editor subsystems.
 *
 * Address:   0x80082FC8
 * Mangled:   __11LevelEditor
 * Demangled: LevelEditor::LevelEditor(void)
 *
 */
LevelEditor::LevelEditor()
{
    char theme_name[10];

    this->CClass.vtable = &__vt_11LevelEditor;
    this->NewTheme = -1;
    Spool_RemoveUnusedTextures();
    Pal_RemoveUnusedPalettes();
    Pack_DefragmentFully();
    printf("Welcome to the Level Editor\n");
    this->CreateMenu();
    Db_Init();
    FontManager::LoadFont("ingame1.fnt");
    if (!GenerateLevel) {
        LevelGenerator::Init();
        EditorPieces_SetTheme(GenerateThemeNumber);
        PiecesRemaining = 0x30;
    }
    GenerateLevel = 0;

    void *obj;
    obj = ::CClass::operator new(sizeof(SelectManager));
    if (obj != 0)
        obj = ::new (obj) SelectManager;
    {
        int theme = GenerateThemeNumber;
        this->PieceManager = static_cast<SelectManager *>(obj);
        this->PieceManager->SetTheme(theme);
    }

    obj = ::CClass::operator new(0xa634);
    if (obj != 0)
        obj = ::new (obj) LevelGenerator(false);
    int theme = GenerateThemeNumber;
    this->Gen = static_cast<LevelGenerator *>(obj);
    sprintf(theme_name, "SkEd%d", theme + 1);
    this->Gen->LoadModels(theme_name);
    this->Gen->CreatePSX(&this->mRegion, true);
    int region = this->mRegion;
    this->pItemList =
        reinterpret_cast<Item *>(PSXRegion[region].pPSX + 3);
    printf("mRegion is %d\n", region);

    obj = ::CClass::operator new(sizeof(EditorCamera));
    if (obj != 0)
        obj = ::new (obj) EditorCamera;
    this->mCamera = static_cast<EditorCamera *>(obj);

    obj = ::CClass::operator new(sizeof(Cursor));
    if (obj != 0)
        obj = ::new (obj) Cursor(*this->Gen);
    this->mCursor = static_cast<Cursor *>(obj);

    {
        int pidx = this->PieceManager->GetPieceIndex();
        this->mCursor->SetPiece(pidx);
    }
    this->mCursor->SetCameraTargetVector(
        this->mCamera->GetCameraTargetVector());

    this->mViewPort.xR = 0x200;
    this->mViewPort.yB = 0xf0;
    this->mViewPort.xL = 0;
    this->mViewPort.yT = 0;
    this->mViewPort.Hither = 0xa;
    this->mViewPort.Yon = 0x7530;
    this->mViewPort.TanHalf = 0xd55;
    this->mTime = VSync(-1);
    Pad_ClearTriggers(Pad);
    this->mPadWait = 0;
}

/**
 * _._11LevelEditor — Tear down owned editor subobjects, unload editor resources,
 * reset viewport/pushback state, then optionally delete the LevelEditor itself.
 *
 * Address:   0x800831D4
 * Mangled:   _._11LevelEditor
 * Demangled: LevelEditor::~LevelEditor()
 *
 */
LevelEditor::~LevelEditor()
{
    this->CClass.vtable = &__vt_11LevelEditor;

    ::CClass *obj = static_cast< ::CClass *>(this->mCamera);
    if (obj != 0)
    {
        int *vt = obj->vtable;
        reinterpret_cast<void (*)(::CClass *, int)>(vt[2])(obj, 3);
    }

    obj = static_cast< ::CClass *>(this->mCursor);
    if (obj != 0)
    {
        int *vt = obj->vtable;
        reinterpret_cast<void (*)(::CClass *, int)>(vt[2])(obj, 3);
    }

    LevelGenerator::UnloadOutPSX(true);

    obj = reinterpret_cast< ::CClass *>(this->Gen);
    if (obj != 0)
    {
        int *vt = obj->vtable;
        reinterpret_cast<void (*)(::CClass *, int)>(vt[2])(obj, 3);
    }

    obj = static_cast< ::CClass *>(this->PieceManager);
    if (obj != 0)
    {
        int *vt = obj->vtable;
        reinterpret_cast<void (*)(::CClass *, int)>(vt[2])(obj, 3);
    }

    FontManager::UnloadFont("ingame1.fnt");
    register int *vtbase asm("$3") = &__vt_11LevelEditor_base;
    pCurrentViewport = (SViewport *)-1;
    M3d_OTPushback = 0;
    M3d_OTPushback2 = 0;
    this->CClass.vtable = vtbase;
}

/**
 * Draw the level editor and its active menu, cursor, and scene.
 *
 * Address:   0x800832E8
 * Mangled:   Draw__11LevelEditor
 * Demangled: LevelEditor::Draw(void)
 *
 */
void LevelEditor::Draw()
{
    Db_DisplayDraw();
    Db_FlipClear();
    SCamera *C = MikeCamera;
    this->PieceManager->Draw();

    EditorTextMenuView *wrapper = this->Wrapper;
    if (wrapper != 0)
    {
        EditorDrawableView *menu =
            reinterpret_cast<EditorDrawableView *>(wrapper->Menu);
        reinterpret_cast<void (*)(EditorDrawableView *)>(menu->vtable[5])(
            menu);
    }

    M3d_RenderSetup(
        C, &this->mViewPort, reinterpret_cast<EditorDoubleBufferView *>(pDoubleBuffer)->OrderingTable);

    ForceWireFrameColor = 0x8000;
    CurrentOrderingTable += 200;
    this->mCursor->PushbackPieces(true);
    M3d_Render(this->pItemList);

    M3d_OTPushback = 0;
    M3d_OTPushback2 = 0;
    CurrentOrderingTable -= 100;
    this->mCursor->Draw();

    M3d_OTPushback = 0x400;
    M3d_OTPushback2 = 0x400;
    CurrentOrderingTable -= 100;
    this->mCursor->PushbackPieces(false);
    M3d_Render(this->pItemList);

    M3d_RenderCleanup();
    ForceWireFrameColor = 0;
}

/**
 * Update__11LevelEditor
 *
 * Address:   0x8008342C
 * Mangled:   Update__11LevelEditor
 * Demangled: LevelEditor::Update(void)
 *
 * @return One when the editor is leaving, otherwise zero.
 */
int LevelEditor::Update()
{
    int new_time;

    this->UpdateMenu();
    if (this->MenuState == 4)
    {
        return 1;
    }

    if (this->DidLoad != 0)
    {
        this->DidLoad = 0;
        this->Gen->CreatePSX(&this->mRegion, true);
        this->pItemList = (struct Item *)(PSXRegion[this->mRegion].pPSX + 3);
        this->mCursor->Move(this->mCursor->X,
                              this->mCursor->Y,
                              this->mCursor->Z);
        this->mCursor->ResortGrid = (void *)1;
    }

    if (this->MenuState == 0)
    {
        this->PieceManager->SetMenuState(true);
        this->HandleInput();

        if (this->NewTheme != -1)
        {
            char theme_name[10];

            GenerateThemeNumber = this->NewTheme;
            this->NewTheme = -1;
            LevelGenerator::SetMapTheme(GenerateThemeNumber);
            this->PieceManager->SetTheme(GenerateThemeNumber);
            sprintf(theme_name, "SkEd%d", GenerateThemeNumber + 1);
            LevelGenerator::UnloadOutPSX(true);
            this->Gen->LoadModels(theme_name);
            this->Gen->CreatePSX(&this->mRegion, true);
            this->mCursor->ResortGrid = (void *)1;
            this->pItemList = (struct Item *)(PSXRegion[this->mRegion].pPSX + 3);
        }
    }
    else
    {
        this->PieceManager->SetMenuState(false);
    }

    this->mCursor->Update();
    this->mCamera->SetTarget(this->mCursor->CameraX,
                               0,
                               this->mCursor->CameraZ);
    this->mCamera->Update();
    this->PieceManager->Update();

    while (DrawSync(1) != 0)
    {
    }
    VSync(0);
    do
    {
        new_time = VSync(-1);
    } while (new_time < this->mTime + 2);
    this->mTime = new_time;

    if (this->mPadWait != 0)
    {
        this->mPadWait--;
    }

    Pad_Update();
    PollHost();

    return 0;
}

/**
 * Handle the editor controller buttons, piece/category selection, placement,
 * removal, and directional cursor movement.
 *
 * Address:   0x80083634
 * Mangled:   HandleInput__11LevelEditor
 * Demangled: LevelEditor::HandleInput(void)
 *
 */
void LevelEditor::HandleInput()
{
    if (Pad[0].Square.Triggered != 0)
    {
        Pad[0].Square.Triggered = 0;
        this->mCamera->Rotate();
        this->mCursor->SetCameraTargetVector(
            this->mCamera->GetCameraTargetVector());
        this->mCursor->ResortGrid = (void *)1;
    }

    if (Pad[0].Circle.Triggered != 0)
    {
        Pad[0].Circle.Triggered = 0;
        this->mCursor->Rotate(1);
    }

    if (Pad[0].Select.Triggered != 0)
    {
        Pad[0].Select.Triggered = 0;
        this->mCamera->SwitchType();
    }

    if (Pad[0].X.Triggered != 0)
    {
        Pad[0].X.Triggered = 0;
        if (this->mCursor->Stamp() != 0)
        {
            SFX_PlayX(0xea, 0x2000, 0);
            this->Gen->CreatePSX(&this->mRegion, false);
        }
        else
        {
            SFX_PlayX(0x92, 0x2000, 0);
        }
        this->mCursor->ResortGrid = (void *)1;
        this->pItemList =
            (Item *)(PSXRegion[this->mRegion].pPSX + 3);
    }

    if (Pad[0].Triangle.Triggered != 0)
    {
        Pad[0].Triangle.Triggered = 0;
        if (this->mCursor->Remove() != 0)
        {
            SFX_PlayX(0x93, 0x2000, 0);
            this->Gen->CreatePSX(&this->mRegion, false);
        }
        else
        {
            SFX_PlayX(0x92, 0x2000, 0);
        }
        this->mCursor->ResortGrid = (void *)1;
        this->pItemList =
            (Item *)(PSXRegion[this->mRegion].pPSX + 3);
    }

    if (Pad[0].LeftTwo.Triggered != 0)
    {
        Pad[0].LeftTwo.Triggered = 0;
        this->PieceManager->ChangeCategory(-1);
        this->mCursor->SetPiece(
            this->PieceManager->GetPieceIndex());
    }

    if (Pad[0].LeftOne.Triggered != 0)
    {
        Pad[0].LeftOne.Triggered = 0;
        this->PieceManager->ChangeCategory(1);
        this->mCursor->SetPiece(
            this->PieceManager->GetPieceIndex());
    }

    if (Pad[0].RightTwo.Triggered != 0)
    {
        Pad[0].RightTwo.Triggered = 0;
        this->PieceManager->ChangePiece(-1);
        this->mCursor->SetPiece(
            this->PieceManager->GetPieceIndex());
    }

    if (Pad[0].RightOne.Triggered != 0)
    {
        Pad[0].RightOne.Triggered = 0;
        this->PieceManager->ChangePiece(1);
        this->mCursor->SetPiece(
            this->PieceManager->GetPieceIndex());
    }

    int press = -1;
    if ((Pad[0].Left.Pressed != 0 && this->mPadWait == 0) ||
        Pad[0].Left.Triggered != 0)
    {
        this->mPadWait = 4;
        press = 2;
        if (Pad[0].Left.Triggered != 0)
        {
            Pad[0].Left.Triggered = 0;
        }
    }

    if ((Pad[0].Right.Pressed != 0 && this->mPadWait == 0) ||
        Pad[0].Right.Triggered != 0)
    {
        this->mPadWait = 4;
        press = 0;
        if (Pad[0].Right.Triggered != 0)
        {
            Pad[0].Right.Triggered = 0;
        }
    }

    if ((Pad[0].Up.Pressed != 0 && this->mPadWait == 0) ||
        Pad[0].Up.Triggered != 0)
    {
        this->mPadWait = 4;
        press = 1;
        if (Pad[0].Up.Triggered != 0)
        {
            Pad[0].Up.Triggered = 0;
        }
    }

    if ((Pad[0].Down.Pressed != 0 && this->mPadWait == 0) ||
        Pad[0].Down.Triggered != 0)
    {
        this->mPadWait = 4;
        press = 3;
        if (Pad[0].Down.Triggered != 0)
        {
            Pad[0].Down.Triggered = 0;
        }
    }

    if (press != -1)
    {
        press = this->mCamera->TranslatePress(press);
        this->mCursor->Move(press);
    }
}

/**
 * CreateMenu__11LevelEditor — Clear the level editor's menu state.
 *
 * Address:   0x80083A58, Size: 16 bytes (4 instructions)
 * Mangled:   CreateMenu__11LevelEditor
 * Demangled: LevelEditor::CreateMenu(void)
 *
 */
void LevelEditor::CreateMenu()
{
    this->MenuState = NOTHING;
    this->Wrapper = 0;
    this->DidLoad = 0;
}

/**
 * Advance the level editor's pause, save/load, theme, and confirmation menus.
 * Menu wrappers are created on entry to a state and destroyed once a choice
 * has completed.
 *
 * Address:   0x80083A68
 * Mangled:   UpdateMenu__11LevelEditor
 * Demangled: LevelEditor::UpdateMenu(void)
 *
 */
void LevelEditor::UpdateMenu()
{
    LevelEditor *editor = this;
    typedef int (*MenuFunction)(EditorTextMenuView *);
    typedef void (*MenuUpdateFunction)(EditorTextMenuView *);
    typedef void (*MenuDestroyFunction)(EditorTextMenuView *, int);

    switch (editor->MenuState)
    {
    case 0:
        if (Pad[0].Start.Triggered != 0)
        {
            Pad[0].Start.Triggered = 0;
            editor->LastMenuState = 0;
            editor->MenuState = 1;
        }
        goto update_menu;

    case 1:
    {
        if (editor->Wrapper == 0)
        {
            const char *names[6] = {
                "CONTINUE", "PLAY", "SET THEME", "SAVE", "LOAD", "QUIT"
            };
            int ids[6] = {2, 4, 5, 1, 3, 0};

            EditorTextMenuView *wrapper =
                static_cast<EditorTextMenuView *>(
                    ::CClass::operator new(sizeof(EditorTextMenuView)));
            if (wrapper != 0)
            {
                wrapper = __15TextMenuWrapperPCPcPiiPcb(
                    wrapper, names, ids, 6,
                    "ingame1.fnt", false);
            }
            editor->Wrapper = wrapper;
            editor->Wrapper->ItemList[3]->JustX = 0;
            editor->Wrapper->ItemList[4]->JustX = 0;
            goto update_menu;
        }

        if (reinterpret_cast<MenuFunction>(editor->Wrapper->vtable[4])(
                editor->Wrapper) == 0)
        {
            goto update_menu;
        }

        editor->LastMenuState = 1;
        EnumMenuState next_state;
        switch (reinterpret_cast<MenuFunction>(editor->Wrapper->vtable[5])(
            editor->Wrapper))
        {
        case 0:
            next_state = VERIFY_CHOICE;
            break;
        case 1:
            next_state = SAVE;
            break;
        case 3:
            next_state = LOAD;
            break;
        case 4:
            editor->MenuState = DONE;
            GenerateLevel = (void *)1;
            goto destroy_main_menu;
        case 5:
            next_state = SET_THEME;
            break;
        default:
            editor->MenuState = NOTHING;
            goto destroy_main_menu;
        }
        editor->MenuState = next_state;
destroy_main_menu:
        if (editor->Wrapper != 0)
        {
            EditorTextMenuView *wrapper = editor->Wrapper;
            ((MenuDestroyFunction)wrapper->vtable[2])(wrapper, 3);
        }
        editor->Wrapper = 0;
        break;
    }

    case 2:
        if (editor->Wrapper == 0)
        {
            char names[10][2];
            const char *name_ptrs[10];
            int ids[10];

            for (int i = 0; i < 10; i++)
            {
                sprintf(names[i], "%d", i);
                name_ptrs[i] = names[i];
                ids[i] = i;
            }

            EditorTextMenuView *wrapper =
                static_cast<EditorTextMenuView *>(
                    ::CClass::operator new(sizeof(EditorTextMenuView)));
            if (wrapper != 0)
            {
                wrapper = __15TextMenuWrapperPCPcPiiPcb(
                    wrapper, name_ptrs, ids, 10,
                    "ingame1.fnt", true);
            }
            editor->Wrapper = wrapper;
        }
        else if (reinterpret_cast<MenuFunction>(editor->Wrapper->vtable[4])(
                     editor->Wrapper) != 0)
        {
            editor->LastMenuState = 2;
            int id = reinterpret_cast<MenuFunction>(
                editor->Wrapper->vtable[5])(editor->Wrapper);
            if (id >= 0 && id < 10)
            {
                sprintf(editor->SaveName, "park%d.prk", id);
                editor->MenuState = VERIFY_CHOICE;
            }
            else
            {
                editor->MenuState = MAIN;
            }
            if (editor->Wrapper != 0)
            {
                EditorTextMenuView *wrapper = editor->Wrapper;
                ((MenuDestroyFunction)wrapper->vtable[2])(wrapper, 3);
            }
            editor->Wrapper = 0;
        }
        break;

    case 3:
        if (editor->Wrapper == 0)
        {
            char names[10][2];
            const char *name_ptrs[10];
            int ids[10];

            for (int i = 0; i < 10; i++)
            {
                sprintf(names[i], "%d", i);
                name_ptrs[i] = names[i];
                ids[i] = i;
            }

            EditorTextMenuView *wrapper =
                static_cast<EditorTextMenuView *>(
                    ::CClass::operator new(sizeof(EditorTextMenuView)));
            if (wrapper != 0)
            {
                wrapper = __15TextMenuWrapperPCPcPiiPcb(
                    wrapper, name_ptrs, ids, 10,
                    "ingame1.fnt", true);
            }
            editor->Wrapper = wrapper;
            goto update_menu;
        }

        if (reinterpret_cast<MenuFunction>(editor->Wrapper->vtable[4])(
                editor->Wrapper) == 0)
        {
            goto update_menu;
        }

        editor->LastMenuState = 3;
        {
            int id = reinterpret_cast<MenuFunction>(
                editor->Wrapper->vtable[5])(editor->Wrapper);
            if (id >= 0 && id < 10)
            {
                char load_name[14];
                sprintf(load_name, "park%d.prk", id);
                LevelGenerator::LoadMap(load_name);
                editor->MenuState = 0;
                editor->DidLoad = (void *)1;
            }
            else
            {
                editor->MenuState = 1;
            }
        }
        if (editor->Wrapper != 0)
        {
            EditorTextMenuView *wrapper = editor->Wrapper;
            ((MenuDestroyFunction)wrapper->vtable[2])(wrapper, 3);
        }
        editor->Wrapper = 0;
        break;

    case 4:
        goto update_menu;

    case 5:
        if (editor->Wrapper == 0)
        {
            const char *names[4] = {
                "THEME ONE", "THEME TWO", "THEME THREE", "THEME FOUR"
            };
            int ids[4] = {0, 1, 2, 3};

            EditorTextMenuView *wrapper =
                static_cast<EditorTextMenuView *>(
                    ::CClass::operator new(sizeof(EditorTextMenuView)));
            if (wrapper != 0)
            {
                wrapper = __15TextMenuWrapperPCPcPiiPcb(
                    wrapper, names, ids, 4,
                    "ingame1.fnt", false);
            }
            editor->Wrapper = wrapper;
            editor->Wrapper->ItemList[2]->JustX = 0;
            editor->Wrapper->ItemList[3]->JustX = 0;
            goto update_menu;
        }

        if (reinterpret_cast<MenuFunction>(editor->Wrapper->vtable[4])(
                editor->Wrapper) == 0)
        {
            goto update_menu;
        }

        editor->LastMenuState = 5;
        editor->NewTheme = reinterpret_cast<MenuFunction>(
            editor->Wrapper->vtable[5])(editor->Wrapper);
        editor->MenuState = NOTHING;
        if (editor->Wrapper != 0)
        {
            EditorTextMenuView *wrapper = editor->Wrapper;
            ((MenuDestroyFunction)wrapper->vtable[2])(wrapper, 3);
        }
        editor->Wrapper = 0;
        break;

    case 6:
        if (editor->Wrapper == 0)
        {
            const char *names[3] = {"", "YES", "NO"};
            int ids[3] = {-1, 0, 1};
            char title[30];

            if (editor->LastMenuState == 2)
            {
                sprintf(title, "save over this file?");
            }
            else
            {
                sprintf(title, "are you sure?");
            }
            names[0] = title;

            EditorTextMenuView *wrapper =
                static_cast<EditorTextMenuView *>(
                    ::CClass::operator new(sizeof(EditorTextMenuView)));
            if (wrapper != 0)
            {
                wrapper = __15TextMenuWrapperPCPcPiiPcb(
                    wrapper, names, ids, 3,
                    "ingame1.fnt", false);
            }
            editor->Wrapper = wrapper;
            editor->Wrapper->Menu->setHighlighted(1);

            EditorDrawableView *menu =
                reinterpret_cast<EditorDrawableView *>(editor->Wrapper->Menu);
            reinterpret_cast<void (*)(EditorDrawableView *)>(menu->vtable[4])(
                menu);
            goto update_menu;
        }

        if (reinterpret_cast<MenuFunction>(editor->Wrapper->vtable[4])(
                editor->Wrapper) == 0)
        {
            goto update_menu;
        }

        {
            int id = reinterpret_cast<MenuFunction>(
                editor->Wrapper->vtable[5])(editor->Wrapper);
            if (id == 1 || id == -1)
            {
                editor->MenuState = NOTHING;
            }
            else
            {
                if (editor->LastMenuState != MAIN)
                {
                    if (editor->LastMenuState == SAVE)
                    {
                        editor->MenuState = NOTHING;
                    }
                }
                else
                {
                    editor->MenuState = DONE;
                }
            }
        }
        if (editor->Wrapper != 0)
        {
            EditorTextMenuView *wrapper = editor->Wrapper;
            ((MenuDestroyFunction)wrapper->vtable[2])(wrapper, 3);
        }
        editor->Wrapper = 0;
        break;
    }

update_menu:
    if (editor->Wrapper != 0)
    {
        EditorTextMenuView *wrapper = editor->Wrapper;
        ((MenuUpdateFunction)wrapper->vtable[3])(wrapper);
    }

    static const char demoPiecesLeft[] asm("DAT_800B3C18") =
        "demo pieces left: %d";
    static const char notAvailableInDemo[]
        asm("s_not_available_in_demo_800b3c30") =
        "not available in demo";

    /*
     * The original owner retains these diagnostics in its read-only data even
     * though the corresponding PSX verification path is disabled in the demo.
     */
    static const char verifyPsx[] = "\nVERIFYING PSX\n\n";
    static const char itemCount[] = "number of items: %d\n";
    static const char itemAddress[] =
        "Item %d at: %x, next at %x, diff is %d\n";
    static const char modelCount[] = "\nnumber of models: %d\n";
    static const char modelAddress[] =
        "model %d: supposedly at %x, actually at %x\n";
    static const char modelDimensions[] =
        "   NumVertices %d, NumNormals %d, NumFaces %d\n";
    static const char modelBox[] =
        "   Box: [%d,%d],[%d,%d],[%d,%d]\n";
    static const char zonesOkay[] = "Zones packet seems OK\n";
    static const char zonesIncorrect[] = "Zones packet incorrect\n";
    static const char terminatorOkay[] = "Terminator packet seems OK\n";
    static const char terminatorIncorrect[] =
        "Terminator packet incorrect\n";
}

/**
 * __13SelectManager
 *
 * Address:   0x80084144
 * Mangled:   __13SelectManager
 * Demangled: SelectManager::SelectManager(void)
 *
 * Builds the editor's category/piece selector, loads its fonts and arrows,
 * and creates the four controller-prompt elements.
 *
 */
SelectManager::SelectManager()
{
    this->vtable = &__vt_13SelectManager;
    ::new (FontFromStorage(&this->TheFont)) Font;
    ::new (FontFromStorage(&this->PromptFont)) Font;

    int i = 0;
    while (PieceMap[i].map[0] != 0xff)
    {
        i++;
    }
    this->NumPieces = i;

    i = 0;
    while (CategoryMap[i].firstPiece != 0xff)
    {
        i++;
    }
    this->NumCategories = i;

    Font *loaded_font = FontManager::LoadFont("s2park1.fnt");
    *(EditorFontAssignmentView *)&this->TheFont =
        *(EditorFontAssignmentView *)loaded_font;
    this->TheFont.JustX = 0;
    this->TheFont.ShadowDx = 2;
    this->TheFont.ShadowDy = 2;
    this->TheFont.SpaceX = 1;
    *(EditorFontAssignmentView *)&this->PromptFont =
        *(EditorFontAssignmentView *)&this->TheFont;
    this->PromptFont.JustY = 2;

    this->WindowHeight = 43;
    this->WindowMidpoint = 28;
    this->WindowTop = -43;
    Sprite2 *sprite = new Sprite2("s2l1l2.bmp", 1, 0, 0);
    this->LeftButton = sprite;
    ((EditorSpriteJustificationView *)this->LeftButton)->JustX = 2;
    ((EditorSpriteJustificationView *)this->LeftButton)->JustY = 1;
    this->LeftX =
        reinterpret_cast<SlicedImage2 *>(this->LeftButton)->screenWidth() +
        12;

    sprite = new Sprite2("s2r1r2.bmp", 1, 0, 0);
    this->RightButton = sprite;
    ((EditorSpriteJustificationView *)this->RightButton)->JustX = 0;
    ((EditorSpriteJustificationView *)this->RightButton)->JustY = 1;
    this->RightX =
        0x1f2 - reinterpret_cast<SlicedImage2 *>(this->RightButton)
                    ->screenWidth();

    this->Theme = 0;
    this->Category = -1;
    this->Piece = -1;
    this->UnpackCount = 0;
    this->SetCategory(0);

    PromptScreenElement *prompt = new PromptScreenElement(
        "s2EDsqua.bmp", "view ", FontFromStorage(&this->PromptFont),
        60, 221, -1, 260);
    this->PromptItem[0] = reinterpret_cast<ScreenElement *>(prompt);

    prompt = new PromptScreenElement(
        "s2EDcirc.bmp", "piece ", FontFromStorage(&this->PromptFont),
        144, 221, -1, 260);
    this->PromptItem[1] = reinterpret_cast<ScreenElement *>(prompt);

    prompt = new PromptScreenElement(
        "s2EDcros.bmp", "place ", FontFromStorage(&this->PromptFont),
        264, 221, -1, 260);
    this->PromptItem[2] = reinterpret_cast<ScreenElement *>(prompt);

    prompt = new PromptScreenElement(
        "s2EDtria.bmp", "erase ", FontFromStorage(&this->PromptFont),
        358, 221, -1, 260);
    this->PromptItem[3] = reinterpret_cast<ScreenElement *>(prompt);

    this->SetMenuState(true);
    this->OldPiecesRemaining = -1;
    this->Blink = 1;
}

/**
 * _._13SelectManager — SelectManager dtor: unload font, tear down child objects,
 * destroy embedded Font subobjects, optionally delete self.
 *
 * Address:   0x800844CC
 * Mangled:   _._13SelectManager
 * Demangled: SelectManager::~SelectManager()
 */
SelectManager::~SelectManager()
{
    vtable = &__vt_13SelectManager;
    FontManager::UnloadFont("s2park1.fnt");

    CClass *obj = reinterpret_cast<CClass *>(LeftButton);
    if (obj != 0)
    {
        int *vt = obj->vtable;
        reinterpret_cast<void (*)(CClass *, int)>(vt[2])(obj, 3);
    }

    obj = reinterpret_cast<CClass *>(RightButton);
    if (obj != 0)
    {
        int *vt = obj->vtable;
        reinterpret_cast<void (*)(CClass *, int)>(vt[2])(obj, 3);
    }

    int i = 0;
    ScreenElement **slot = PromptItem;
    do
    {
        CClass *item = reinterpret_cast<CClass *>(*slot);
        if (item != 0)
        {
            int *vt = item->vtable;
            reinterpret_cast<void (*)(CClass *, int)>(vt[2])(item, 3);
        }
        i++;
        slot++;
    } while (i < 4);

    FontFromStorage(&PromptFont)->~Font();
    FontFromStorage(&TheFont)->~Font();
    vtable = &__vt_13SelectManager_base;
}

/**
 * Draw__13SelectManager
 *
 * Address:   0x800845DC
 * Mangled:   Draw__13SelectManager
 * Demangled: SelectManager::Draw(void)
 *
 * Draws the category and piece labels, the scrolling window background, its
 * navigation buttons, and the four controller prompts.
 *
 */
void SelectManager::Draw()
{
    typedef void (*DrawAtFunction)(void *, int, int, int);
    typedef void (*DrawPromptFunction)(void *, int, int);

    EditorDrawableView *left =
        (EditorDrawableView *)this->LeftButton;
    ((DrawAtFunction)left->vtable[5])(
        left, this->LeftX,
        this->WindowTop + this->WindowMidpoint, 0);

    Font *font = FontFromStorage(&this->TheFont);
    font->draw(this->CatStringX[0],
               this->WindowTop + this->CatStringY[0],
               this->CatString[0], 0);
    font->draw(this->CatStringX[1],
               this->WindowTop + this->CatStringY[1],
               this->CatString[1], 0);

    EditorDrawableView *right =
        (EditorDrawableView *)this->RightButton;
    ((DrawAtFunction)right->vtable[5])(
        right, this->RightX,
        this->WindowTop + this->WindowMidpoint, 0);

    font->draw(this->PieceStringX[0],
               this->WindowTop + this->PieceStringY[0],
               this->PieceString[0], 0);
    font->draw(this->PieceStringX[1],
               this->WindowTop + this->PieceStringY[1],
               this->PieceString[1], 0);
    font->draw(this->PieceStringX[2],
               this->WindowTop + this->PieceStringY[2],
               this->PieceString[2], 0);

    if (this->Blink > 12 && this->NotInDemo != 0)
    {
        font->draw(this->PieceStringX[3],
                   this->WindowTop + this->PieceStringY[3],
                   this->PieceString[3], 0);
    }

    this->Blink--;
    if (this->Blink == 0)
    {
        this->Blink = 48;
    }

    POLY_F4 *Poly = (POLY_F4 *)pPoly;
    pPoly = (unsigned int *)(Poly + 1);
    setPolyF4(Poly);
    setRGB0(Poly, 60, 60, 60);
    setXY4(Poly,
           0, this->WindowTop,
           0x200, this->WindowTop,
           0, this->WindowTop + this->WindowHeight,
           0x200, this->WindowTop + this->WindowHeight);
    setSemiTrans(Poly, 1);

    int tpage = GetTPage(2, 2, 0, 0);
    DR_MODE *dr = (DR_MODE *)pPoly;
    pPoly = (unsigned int *)(dr + 1);
    SetDrawMode(dr, 0, 1, tpage, 0);
    addPrim(reinterpret_cast<EditorDoubleBufferView *>(pDoubleBuffer)->PanelOT, dr);

    for (int i = 0; i < 4; i++)
    {
        EditorDrawableView *prompt =
            (EditorDrawableView *)this->PromptItem[i];
        ((DrawPromptFunction)prompt->vtable[4])(prompt, 0, 0);
    }
}

/**
 * Update__13SelectManager
 *
 * Address:   0x800848A4
 * Mangled:   Update__13SelectManager
 * Demangled: SelectManager::Update(void)
 *
 */
void SelectManager::Update()
{
    int top = this->WindowTop;
    int target = this->WindowTarget;
    int diffY = target - top;

    if ((unsigned int)(diffY + 1) < 3)
    {
        this->WindowTop = target;
        top = target;
    }
    int change = diffY >> 2;
    if (change == 0)
    {
        change = diffY >> 1;
    }
    this->WindowTop = top + change;

    unsigned char count = this->UnpackCount;
    if (count != 0)
    {
        this->UnpackCount = count - 1;
    }

    int i = 0;
    ScreenElement **prompt = this->PromptItem;
    do
    {
        EditorDrawableView *item =
            reinterpret_cast<EditorDrawableView *>(*prompt);
        void **vt = item->vtable;
        prompt++;
        reinterpret_cast<void (*)(EditorDrawableView *)>(vt[3])(item);
        i++;
    } while (i < 4);

    if (diffY != 0)
    {
        this->Blink = 1;
    }

    int pieces = PiecesRemaining;
    if (pieces != this->OldPiecesRemaining)
    {
        char *pieceString = this->PieceString[2];
        this->OldPiecesRemaining = pieces;
        sprintf(pieceString, (char *)DAT_800B3C18, pieces);
        int width = FontFromStorage(&this->TheFont)->width(pieceString);
        int x = this->RightX - width - 6;
        this->PieceStringX[2] = x;
        int height = FontFromStorage(&this->TheFont)->height(
            this->PieceString[1]);
        this->PieceStringY[2] = this->WindowMidpoint + height + 3;
    }
}

/**
 *
 * Address:   0x800849D0
 * Mangled:   SetPiece__13SelectManageri
 * Demangled: SelectManager::SetPiece(int)
 *            Selects a piece, refreshes its labels, and positions the piece/demo prompts.
 *
 * @param piece  Editor piece data.
 */
void SelectManager::SetPiece(int piece)
{
    int pieceReg = piece;

    if (this->UnpackCount != 0)
    {
        return;
    }

    int current = this->Piece;
    if (current != -1)
    {
        this->PieceToUnpack = current;
        this->UnpackCount = 2;
    }

    this->Piece = pieceReg;
    CategoryMap[this->Category].lastUsedPiece =
        PieceMap[pieceReg].map[this->Theme];

    char *pieceCountString = this->PieceString[0];
    sprintf(pieceCountString, "Piece %d of %d:",
            this->Piece - this->FirstPieceInCategory + 1,
            this->NumPiecesInCategory);

    char *pieceNameString = this->PieceString[1];
    sprintf(pieceNameString, "%s",
            PieceMap[this->Piece].name);

    Font *font = FontFromStorage(&this->TheFont);
    int width = font->width(pieceCountString);
    this->PieceStringX[0] = this->RightX - width - 6;

    width = font->width(pieceNameString);
    this->PieceStringX[1] = this->RightX - width - 6;

    int height = font->height(pieceCountString);
    this->PieceStringY[0] = this->WindowMidpoint - height - 1;
    this->PieceStringY[1] = this->WindowMidpoint + 1;

    if ((PieceMap[this->Piece].flags & 0x80) != 0)
    {
        this->NotInDemo = (void *)1;
    }
    else
    {
        this->NotInDemo = 0;
    }

    char *demoString = this->PieceString[3];
    sprintf(demoString, (char *)s_not_available_in_demo_800b3c30);

    width = font->width(demoString);
    this->PieceStringX[3] = 0x100 - (width >> 1);

    height = font->height(demoString);
    this->PieceStringY[3] = 0x78 - (height >> 1);
}

/**
 * ChangePiece__13SelectManageri
 *
 * Address:   0x80084BCC
 * Mangled:   ChangePiece__13SelectManageri
 * Demangled: SelectManager::ChangePiece(int)
 *            Wraps piece index within category bounds, then calls SetPiece
 *
 * @param dir  Direction.
 */
void SelectManager::ChangePiece(int dir)
{
    int piece = this->Piece + dir;
    int diff = piece - this->FirstPieceInCategory;
    if (diff < 0)
    {
        piece = this->FirstPieceInCategory +
                this->NumPiecesInCategory - 1;
    }
    if (diff >= this->NumPiecesInCategory)
    {
        piece = this->FirstPieceInCategory;
    }
    this->SetPiece(piece);
}

/**
 *
 * Address:   0x80084C28
 * Mangled:   SetCategory__13SelectManageri
 * Demangled: SelectManager::SetCategory(int)
 *            Chooses the active editor category, finds its first/default piece, refreshes
 *            the displayed category strings, and recenters their screen positions.
 *
 * @param category Category index to activate.
 */
void SelectManager::SetCategory(int category)
{
    int last_used_piece;

    if (UnpackCount != 0)
    {
        return;
    }

    last_used_piece = -1;
    int i = 0;
    while (i < NumPieces + 1)
    {
        if (PieceMap[i].map[Theme] ==
            CategoryMap[category].lastUsedPiece)
        {
            last_used_piece = i;
        }

        if (PieceMap[i].map[0] ==
            CategoryMap[category].firstPiece)
        {
            FirstPieceInCategory = i;
        }

        if (PieceMap[i].map[0] ==
            CategoryMap[category + 1].firstPiece)
        {
            NumPiecesInCategory = i - FirstPieceInCategory;
            break;
        }

        i++;
    }

    Category = category;
    SetPiece(last_used_piece);

    sprintf(CatString[0], "Set %d of %d:", Category + 1, NumCategories);
    sprintf(CatString[1], "%s",
            CategoryMap[Category].name);

    int y = LeftX + 6;
    CatStringX[0] = y;
    CatStringX[1] = y;

    int stringHeight = FontFromStorage(&TheFont)->height(CatString[0]);
    CatStringY[0] = WindowMidpoint - stringHeight - 1;
    CatStringY[1] = WindowMidpoint + 1;
}

/**
 * ChangeCategory__13SelectManageri — Change category with wrapping, call SetCategory
 *
 * Address:   0x80084DB8, Size: 80 bytes (20 instructions)
 * Mangled:   ChangeCategory__13SelectManageri
 * Demangled: SelectManager::ChangeCategory(int)
 *            Assembly:
 *            lw $v0, 0x164($a0); addiu $sp, -0x18; addu $a1, $v0, $a1
 *            bgez $a1, .check_upper; sw $ra, 0x10($sp)
 *            lw $v0, 0x574($a0); nop; addiu $a1, $v0, -1
 *            .check_upper: lw $v0, 0x574($a0); nop; slt $v0, $a1, $v0
 *            bnez $v0, .call; nop; move $a1, $zero
 *            .call: jal SetCategory; nop
 *            lw $ra, 0x10($sp); nop; jr $ra; addiu $sp, 0x18
 *
 * @param dir  Direction.
 */
void SelectManager::ChangeCategory(int dir)
{
    int cat = this->Category + dir;
    if (cat < 0)
    {
        cat = this->NumCategories - 1;
    }
    if (cat >= this->NumCategories)
    {
        cat = 0;
    }
    this->SetCategory(cat);
    return;
}

/**
 * SetMenuState__13SelectManagerb
 *
 * Address:   0x80084E08
 * Mangled:   SetMenuState__13SelectManagerb
 * Demangled: SelectManager::SetMenuState(bool)
 *
 * @param turn_on  Nonzero to open the selector menu; zero to hide it.
 */
void SelectManager::SetMenuState(bool turn_on)
{
    if (turn_on)
    {
        this->WindowTarget = 0;
        int i = 0;
        int val = 0xD7;
        ScreenElement **items = this->PromptItem;
        do
        {
            EditorScreenElementView *item =
                (EditorScreenElementView *)*items++;
            i++;
            item->mHomeY = val;
        } while (i < 4);
    }
    else
    {
        int i = 0;
        int val = 0xFB;
        int neg = this->WindowHeight;
        ScreenElement **items = this->PromptItem;
        this->WindowTarget = -neg;
        do
        {
            EditorScreenElementView *item =
                (EditorScreenElementView *)*items++;
            i++;
            item->mHomeY = val;
        } while (i < 4);
    }

    if (turn_on)
    {
        int h = FontFromStorage(&this->TheFont)->height(
            this->PieceString[3]);
        int v = 0x78 - (h >> 1);
        this->PieceStringY[3] = v;
    }
    else
    {
        this->PieceStringY[3] = -0x78;
    }
}

/**
 * GetPieceIndex__13SelectManager — Return piece index from table[theme + category*50]
 *
 * Address:   0x80084EB8, Size: 56 bytes (14 instructions)
 * Mangled:   GetPieceIndex__13SelectManager
 * Demangled: SelectManager::GetPieceIndex(void)
 *            Assembly:
 *            lui $a2, 0x800c; addiu $a2, 0x6244
 *            lw $a1, 0x168($a0); lw $v1, 0x160($a0)
 *            sll $v0, $a1, 1; addu $v0, $a1; sll $v0, 3; addu $v0, $a1; sll $v0, 1
 *            addu $v1, $v0; addu $v1, $a2; lbu $v0, ($v1); jr $ra; nop
 *
 * @return The theme byte from the category's PieceMap record.
 */
int SelectManager::GetPieceIndex()
{
    int theme = this->Theme;
    int category = this->Piece;
    return PieceMap[category].map[theme];
}

/**
 * SetTheme__13SelectManageri — Store the active theme index.
 *
 * Address:   0x80084EF0, Size: 8 bytes (2 instructions)
 * Mangled:   SetTheme__13SelectManageri
 * Demangled: SelectManager::SetTheme(int)
 *            Assembly: jr $ra; sw $a1, 0x160($a0)
 *
 * @param theme  Theme identifier.
 */
void SelectManager::SetTheme(int theme)
{
    this->Theme = theme;
}

/**
 * Construct the editor camera and its tripod, camera, and target bodies.
 *
 * Address:   0x80084EF8
 * Mangled:   __12EditorCamera
 * Demangled: EditorCamera::EditorCamera(void)
 *
 */
EditorCamera::EditorCamera()
{
    this->vtable = &__vt_12EditorCamera;

    CBody *body1 = static_cast<CBody *>(::CItem::operator new(0x108));
    if (body1 != 0)
    {
        body1 = ::new (body1) CBody();
    }
    this->CamTri = body1;
    ((EditorCameraBodyView *)this->CamTri)->mPos.vx = 0;
    EditorCameraBodyView *b1 =
        (EditorCameraBodyView *)this->CamTri;
    b1->mPos.vy = 0;
    EditorCameraBodyView *b1b =
        (EditorCameraBodyView *)this->CamTri;
    b1b->mPos.vz = 0;

    CCamera *cam = static_cast<CCamera *>(::CItem::operator new(0x684));
    if (cam != 0)
    {
        cam = ::new (cam) CCamera(this->CamTri, 0);
    }
    this->Camera = cam;
    reinterpret_cast<EditorRuntimeCameraView *>(cam)->mCameraMode = 2;
    register int size2 asm("$4") = 0x108;
    asm volatile("" : "+r"(size2));
    register int yval asm("$2") = 0xff830000;
    EditorRuntimeCameraView *c1 =
        (EditorRuntimeCameraView *)this->Camera;
    asm volatile("" : "+r"(c1));
    int one = 1;
    asm volatile("" : "+r"(one));
    this->Tripod.vx = 0;
    this->Tripod.vy = yval;
    this->Tripod.vz = 0;
    c1->mCalcTripod = 0;
    register int tx asm("$7") = this->Tripod.vx;
    register int ty asm("$8") = this->Tripod.vy;
    register int tz asm("$9") = this->Tripod.vz;
    c1->mTripodPos.vx = tx;
    c1->mTripodPos.vy = ty;
    c1->mTripodPos.vz = tz;
    asm volatile("");
    c1->mTripod = 0;
    c1->newFocus = one;

    CBody *body2 = static_cast<CBody *>(::CItem::operator new(size2));
    if (body2 != 0)
    {
        body2 = ::new (body2) CBody();
    }
    this->Target = body2;
    ((EditorCameraBodyView *)this->Target)->mPos.vx = 0;
    EditorCameraBodyView *b2 =
        (EditorCameraBodyView *)this->Target;
    b2->mPos.vy = 0;
    EditorCameraBodyView *b2b =
        (EditorCameraBodyView *)this->Target;
    b2b->mPos.vz = 0;

    EditorRuntimeCameraView *camPtr =
        (EditorRuntimeCameraView *)this->Camera;
    EditorCameraBodyView *focus =
        (EditorCameraBodyView *)this->Target;
    camPtr->mCalcFocus = one;
    camPtr->mFocus = focus;
    camPtr->newFocus = one;
    this->Type = one;
    this->Rot = 0xc00;
    this->TargetRot = 0xc00;
    this->Change = 0;
}

/**
 * EditorCamera::~EditorCamera()
 *
 * Address:   0x80085028
 * Mangled:   _._12EditorCamera
 * Demangled: EditorCamera::~EditorCamera()
 *
 */
EditorCamera::~EditorCamera()
{
    vtable = &__vt_12EditorCamera;
    CBody *child1 = CamTri;
    if (child1 != 0)
    {
        void **childVtable =
            reinterpret_cast<EditorItemVtableView *>(child1)->vtable;
        typedef void (*VtFunc)(void *, int);
        VtFunc func = reinterpret_cast<VtFunc>(childVtable[2]);
        func(child1, 3);
    }
    CCamera *child2 = Camera;
    if (child2 != 0)
    {
        void **childVtable =
            reinterpret_cast<EditorItemVtableView *>(child2)->vtable;
        typedef void (*VtFunc)(void *, int);
        VtFunc func = reinterpret_cast<VtFunc>(childVtable[2]);
        func(child2, 3);
    }
    CBody *child3 = Target;
    if (child3 != 0)
    {
        void **childVtable =
            reinterpret_cast<EditorItemVtableView *>(child3)->vtable;
        typedef void (*VtFunc)(void *, int);
        VtFunc func = reinterpret_cast<VtFunc>(childVtable[2]);
        func(child3, 3);
    }
    vtable = &__vt_12EditorCamera_base;
}

/**
 * SetTarget__12EditorCameraiii — Store x, y, and z in the target body's position.
 *
 * Address:   0x800850E4, Size: 36 bytes (9 instructions)
 * Mangled:   SetTarget__12EditorCameraiii
 * Demangled: EditorCamera::SetTarget(int, int, int)
 *            Assembly:
 *            lw $v0, 0x28($a0); nop; sw $a1, 4($v0)
 *            lw $v1, 0x28($a0); nop; sw $a2, 8($v1)
 *            lw $v0, 0x28($a0); jr $ra; sw $a3, 0xC($v0)
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param z  Z coordinate.
 */
void EditorCamera::SetTarget(int x, int y, int z)
{
    ((EditorCameraBodyView *)this->Target)->mPos.vx = x;
    ((EditorCameraBodyView *)this->Target)->mPos.vy = y;
    ((EditorCameraBodyView *)this->Target)->mPos.vz = z;
}

/**
 * SwitchType__12EditorCamera — Increment camera type, wrap at 4 back to 0
 *
 * Address:   0x80085108, Size: 36 bytes (9 instructions)
 * Mangled:   SwitchType__12EditorCamera
 * Demangled: EditorCamera::SwitchType(void)
 *            Assembly:
 *            lw $v0, 4($a0); nop; addiu $v0, 1; slti $v1, $v0, 4
 *            bnez $v1, .skip; nop; move $v0, $zero
 *            .skip: jr $ra; sw $v0, 4($a0)
 *
 */
void EditorCamera::SwitchType()
{
    register int type asm("$2") = this->Type;
    type = type + 1;
    if (type >= 4)
    {
        type = 0;
    }
    this->Type = type;
}

/**
 * Rotate__12EditorCamera — Start the next quarter-turn when no turn is active.
 *
 * Address:   0x8008512C, Size: 40 bytes (10 instructions)
 * Mangled:   Rotate__12EditorCamera
 * Demangled: EditorCamera::Rotate(void)
 *            Assembly:
 *            lw $v0, 0x10($a0); nop; bnez $v0, .end; addiu $v1, $zero, 0x80
 *            lw $v0, 0xC($a0); sw $v1, 0x10($a0); addiu $v0, 0x400; sw $v0, 0xC($a0)
 *            .end: jr $ra; nop
 *
 */
void EditorCamera::Rotate()
{
    if (this->Change == 0)
    {
        this->Change = 0x80;
        this->TargetRot = this->TargetRot + 0x400;
    }
}

/**
 * Update__12EditorCamera
 *
 * Address:   0x80085154
 * Mangled:   Update__12EditorCamera
 * Demangled: EditorCamera::Update(void)
 *
 * Advances an in-progress camera rotation, derives the editor tripod from the
 * selected view mode, clamps it to the buildable grid, and updates CCamera.
 *
 */
void EditorCamera::Update()
{
    int wall;
    int diffx;
    int diffz;

    if (this->Change != 0)
    {
        int start_sector = this->Rot >> 10;
        this->Rot += this->Change;
        if ((this->Rot >> 10) != start_sector)
        {
            this->Rot &= 0xc00;
            if (this->Rot >= 0x1000)
            {
                this->Rot -= 0x1000;
            }
            this->Change = 0;
        }
    }

    switch (this->Type)
    {
    case 0:
        this->Tripod.vx =
            ((EditorCameraBodyView *)this->Target)->mPos.vx;
        this->Tripod.vy =
            ((EditorCameraBodyView *)this->Target)->mPos.vy - 0x7d0000;
        this->Tripod.vz =
            ((EditorCameraBodyView *)this->Target)->mPos.vz;
        this->Rot = 0xc00;
        break;

    case 1:
        this->Tripod.vx =
            ((EditorCameraBodyView *)this->Target)->mPos.vx +
            EditorCosine(this->Rot + 0x154) * 2000;
        this->Tripod.vy =
            ((EditorCameraBodyView *)this->Target)->mPos.vy - 0x6c4000;
        this->Tripod.vz =
            ((EditorCameraBodyView *)this->Target)->mPos.vz +
            EditorSine(this->Rot + 0x154) * 2000;
        break;

    case 2:
        this->Tripod.vx =
            ((EditorCameraBodyView *)this->Target)->mPos.vx +
            EditorCosine(this->Rot) * 2000;
        this->Tripod.vy =
            ((EditorCameraBodyView *)this->Target)->mPos.vy - 0x6c4000;
        this->Tripod.vz =
            ((EditorCameraBodyView *)this->Target)->mPos.vz +
            EditorSine(this->Rot) * 2000;
        break;

    case 3:
        this->Tripod.vx =
            ((EditorCameraBodyView *)this->Target)->mPos.vx;
        this->Tripod.vy =
            ((EditorCameraBodyView *)this->Target)->mPos.vy - 0xfa0000;
        this->Tripod.vz =
            ((EditorCameraBodyView *)this->Target)->mPos.vz;
        this->Rot = 0xc00;
        break;
    }

    wall = (GridSize * _14LevelGenerator_GridCellWidth) >> 1;
    diffx = wall - (this->Tripod.vx >> 12);
    if ((unsigned int)diffx < 30)
    {
        this->Tripod.vx = 0xff2000;
    }
    diffx = wall + (this->Tripod.vx >> 12);
    if ((unsigned int)diffx < 30)
    {
        this->Tripod.vx = (int)0xff00e000;
    }

    diffz = wall - (this->Tripod.vz >> 12);
    if ((unsigned int)diffz < 30)
    {
        this->Tripod.vz = 0xff2000;
    }
    diffz = wall + (this->Tripod.vz >> 12);
    if ((unsigned int)diffz < 30)
    {
        this->Tripod.vz = (int)0xff00e000;
    }

    ((EditorRuntimeCameraView *)this->Camera)->SetTripodPosition(this->Tripod);
    ((EditorRuntimeCameraView *)this->Camera)->SetFocus(
        (EditorCameraBodyView *)this->Target);
    ((EditorRuntimeCameraView *)this->Camera)->Update();
}

/**
 * TranslatePress__12EditorCamerai
 *
 * Address:   0x800853F4
 * Mangled:   TranslatePress__12EditorCamerai
 * Demangled: EditorCamera::TranslatePress(int)
 *            Maps d-pad direction based on the camera rotation.
 *
 * @param press  Button press code.
 * @return The press code rotated into camera-relative direction space.
 */
int EditorCamera::TranslatePress(int press)
{
    int angle = this->Rot;
    unsigned int a = (unsigned int)(angle - 0x200);
    if (a >= 0xC00u)
    {
        return (press + 1) & 3;
    }
    if (a < 0x400u)
    {
        return (press + 2) & 3;
    }
    if ((unsigned int)(angle - 0x600) < 0x400u)
    {
        return (press + 3) & 3;
    }
    return press;
}

/**
 * GetCameraTargetVector__12EditorCamera - Return the fixed-point direction from
 * the camera target to its look-at point, applying editor-camera type offsets.
 *
 * Address:   0x80085448
 * Mangled:   GetCameraTargetVector__12EditorCamera
 * Demangled: EditorCamera::GetCameraTargetVector(void)
 *
 * @param out  Output buffer.
 * @return Fixed-point direction from the target to the look-at point.
 */
SVector EditorCamera::GetCameraTargetVector()
{
    Vector temp;
    EditorCameraBodyView *target;
    SVector vect;
    int type = this->Type;

    if (type == 1)
    {
        goto type_one;
    }
    int defaultYOffset;
    defaultYOffset = (int)0xFF060000;
    if (type == 2)
        goto type_two;
    goto type_default;

type_one:
    {
        int tableBase = (int)&rcossin_tbl;
        register int yOffset asm("$8") = (int)0xFF930000;
        asm volatile("" : "+r"(yOffset));
        yOffset |= 0xC000;
        int angle = this->TargetRot;
        target = reinterpret_cast<EditorCameraBodyView *>(this->Target);
        angle += 0x154;
        angle &= 0xFFF;
        angle <<= 2;
        angle += tableBase;
        int cosValue = reinterpret_cast<EditorSinCos *>(angle)->Cos;
        register int targetPos asm("$3") = target->mPos.vx;
        int product = cosValue * 2000;
        targetPos += product;
        temp.vx = targetPos;
        int targetYPos = target->mPos.vy;
        int sinValue = reinterpret_cast<EditorSinCos *>(angle)->Sin;
        temp.vy = targetYPos + yOffset;
        register int targetZPos asm("$3") = target->mPos.vz;
        int zProduct = sinValue * 2000;
        targetZPos += zProduct;
        temp.vz = targetZPos;
        goto make_vector;
    }

type_two:
    {
        int tableBase = (int)&rcossin_tbl;
        register int yOffset asm("$8") = (int)0xFF930000;
        asm volatile("" : "+r"(yOffset));
        yOffset |= 0xC000;
        int angle = this->TargetRot;
        target = reinterpret_cast<EditorCameraBodyView *>(this->Target);
        angle &= 0xFFF;
        angle <<= 2;
        angle += tableBase;
        int cosValue = reinterpret_cast<EditorSinCos *>(angle)->Cos;
        register int targetPos asm("$3") = target->mPos.vx;
        int product = cosValue * 2000;
        targetPos += product;
        temp.vx = targetPos;
        int targetYPos = target->mPos.vy;
        int sinValue = reinterpret_cast<EditorSinCos *>(angle)->Sin;
        temp.vy = targetYPos + yOffset;
        register int targetZPos asm("$3") = target->mPos.vz;
        int zProduct = sinValue * 2000;
        targetZPos += zProduct;
        temp.vz = targetZPos;
        goto make_vector;
    }

type_default:
    {
        register EditorCameraBodyView *defaultTarget asm("$5") =
            reinterpret_cast<EditorCameraBodyView *>(this->Target);
        temp.vx = defaultTarget->mPos.vx;
        register int defaultY asm("$2") = defaultTarget->mPos.vy;
        defaultY += defaultYOffset;
        temp.vy = defaultY;
        int defaultZ = defaultTarget->mPos.vz;
        temp.vz = defaultZ;
    }

make_vector:
    {
        EditorCameraBodyView *targetReg =
            reinterpret_cast<EditorCameraBodyView *>(this->Target);
        vect.vx = (short)((targetReg->mPos.vx - temp.vx) >> 12);
        vect.vy = (short)((targetReg->mPos.vy - temp.vy) >> 12);
        {
            register int zValue asm("$2") = targetReg->mPos.vz;
            int zPoint = temp.vz;
            zValue = zValue - zPoint;
            zValue = zValue >> 12;
            vect.vz = (short)zValue;
        }
    }
    return vect;
}

/**
 * Construct a cursor bound to the supplied level generator.
 *
 * Address:   0x80085604
 * Mangled:   __6CursorR14LevelGenerator
 * Demangled: Cursor::Cursor(LevelGenerator &)
 *
 * @param ed Level generator whose model tables the cursor edits.
 */
Cursor::Cursor(struct LevelGenerator &ed)
{
    vtable = &__vt_6Cursor;
    Ed = &ed;
    Rot = 0;
    X = GridSize >> 1;
    Y = 0;
    Z = GridSize >> 1;
    XRot = 0;
    ZRot = 0;
    WRot = 1;
    LRot = 1;
    ResortGrid = (void *)1;
    Gradient = 4;
    NewLocation = (void *)1;
    Models[0].index = -1;
    SetPiece(0);
}

/**
 * _._6Cursor
 *
 * Address:   0x80085688
 * Mangled:   _._6Cursor
 * Demangled: Cursor::~Cursor()
 *
 */
Cursor::~Cursor()
{
    vtable = &__vt_base_6Cursor;
    // cc1plus generates the trailing `if (__in_chrg & 1) operator delete(this)`
    // branch automatically for the in-charge dtor variant.
}

/**
 * SetPiece__6Cursori
 *
 * Address:   0x800856B8
 * Mangled:   SetPiece__6Cursori
 * Demangled: Cursor::SetPiece(int)
 */
int Cursor::SetPiece(int piece)
{
    Item *pSourceItem;
    int min_x;
    int max_x;
    int min_y;
    int max_y;
    int min_z;
    int max_z;

    if ((((EditorLevelGeneratorModelsView *)Ed)
             ->SourceModelTab[piece]
             .flags &
         0x80) != 0)
    {
        piece = NotInDemoPiece;
    }

    WireframeCollidingPieces(false);

    for (int i = 0; i < 4; i++)
    {
        Models[i].index = -1;
        Models[i].rot = Rot;
        Models[i].X = 0;
        Models[i].Y = 0;
        Models[i].Z = 0;
        Models[i].item.Flags = 0;
        Models[i].item.Inquiry = 0;
        Models[i].item.Angles.vx = 0;
        Models[i].item.Angles.vy = 0;
        Models[i].item.Angles.vz = 0;
        Models[i].item.Frame = 0;
        Models[i].item.TintIndex = 0;
        Models[i].item.Anim = 0;
        Models[i].item.Region =
            ((EditorLevelGeneratorModelsView *)Ed)->SourceRegion;
        Models[i].item.RGB = 0xff00;
        Models[i].item.NextItem = 0;
    }

    Piece = piece;
    Models[0].index = piece;
    Models[0].W = ((EditorLevelGeneratorModelsView *)Ed)
                      ->SourceModelTab[piece]
                      .w;
    Models[0].H = ((EditorLevelGeneratorModelsView *)Ed)
                      ->SourceModelTab[piece]
                      .h;
    Models[0].L = ((EditorLevelGeneratorModelsView *)Ed)
                      ->SourceModelTab[piece]
                      .l;

    if ((((EditorLevelGeneratorModelsView *)Ed)
             ->SourceModelTab[piece]
             .flags &
         0x10) != 0)
    {
        if (Models[0].W == 1 && Models[0].L == 1)
        {
            Models[1].index = Floor1X1Piece[4];
        }
        else if (Models[0].W == 2)
        {
            if (Models[0].L == 1)
            {
                Models[1].index = Floor1X2Piece[4];
            }
            else if (Models[0].L == 2)
            {
                Models[1].index = Floor2X2Piece[4];
            }
        }

        Models[1].W = ((EditorLevelGeneratorModelsView *)Ed)
                          ->SourceModelTab[Models[1].index]
                          .w;
        Models[1].H = ((EditorLevelGeneratorModelsView *)Ed)
                          ->SourceModelTab[Models[1].index]
                          .h;
        Models[1].L = ((EditorLevelGeneratorModelsView *)Ed)
                          ->SourceModelTab[Models[1].index]
                          .l;
        Models[1].Y = Models[0].H;
    }

    min_x = 0;
    max_x = -1;
    min_y = 0;
    max_y = -1;
    min_z = 0;
    max_z = -1;
    pSourceItem =
        (Item *)(PSXRegion[
                     ((EditorLevelGeneratorModelsView *)Ed)->SourceRegion]
                     .pPSX +
                 3);

    for (int i = 0; i < 4; i++)
    {
        if (Models[i].index != -1)
        {
            Models[i].item.Frame = pSourceItem[Models[i].index].Frame;
            Models[i].item.Anim = pSourceItem[Models[i].index].Anim;

            if ((((EditorLevelGeneratorModelsView *)Ed)
                     ->SourceModelTab[Models[i].index]
                     .flags &
                 8) != 0)
            {
                Models[i].item.Frame = 0x81;
                Models[i].item.Anim = 0x81;
            }

            if (Models[i].X < min_x)
            {
                min_x = Models[i].X;
            }
            if (max_x < Models[i].X + Models[i].W)
            {
                max_x = Models[i].X + Models[i].W;
            }
            if (Models[i].Y < min_y)
            {
                min_y = Models[i].Y;
            }
            if (max_y < Models[i].Y + Models[i].H)
            {
                max_y = Models[i].Y + Models[i].H;
            }
            if (Models[i].Z < min_z)
            {
                min_z = Models[i].Z;
            }
            if (max_z < Models[i].Z + Models[i].L)
            {
                max_z = Models[i].Z + Models[i].L;
            }
        }
    }

    W = max_x - min_x;
    H = max_y - min_y;
    L = max_z - min_z;

    switch (Rot)
    {
    case 0:
    case 2:
        WRot = W;
        LRot = L;
        break;

    case 1:
    case 3:
        WRot = L;
        LRot = W;
        break;
    }

    Gradient = 4;
    SetPosition();
    NewLocation = (void *)1;
    return 1;

}

/**
 * Rotate__6Cursori
 *
 * Address:   0x80085BA0
 * Mangled:   Rotate__6Cursori
 * Demangled: Cursor::Rotate(int)
 *            Rotate the cursor orientation and swap footprint dimensions for odd turns.
 *
 * @param dir  Direction.
 */
void Cursor::Rotate(int dir)
{
    int deltaReg = dir;
    WireframeCollidingPieces(false);

    int rot = Rot;
    int scale = rot << 10;
    scale += 0x1000;
    rot += deltaReg;
    StartAngle = scale;
    scale = rot << 10;
    scale += 0x1000;
    Rot = rot;
    rot += 4;
    rot &= 3;
    TargetAngle = scale;
    Rot = rot;

    switch (rot)
    {
    case 0:
    case 2:
        WRot = W;
        LRot = L;
        break;
    case 1:
    case 3:
        WRot = L;
        LRot = W;
        break;
    }

    Gradient = 0;
    SetPosition();
}

/**
 * Move__6Cursoriii
 *
 * Address:   0x80085C70
 * Mangled:   Move__6Cursoriii
 * Demangled: Cursor::Move(int, int, int)
 *            Sets movement target and speed, resets counter, calls SetPosition
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param z  Z coordinate.
 * @return Always one after starting the move.
 */
int Cursor::Move(int x, int y, int z)
{
    WireframeCollidingPieces(false);
    int scale = Rot;
    X = x;
    Y = y;
    Z = z;
    Gradient = 0;
    scale = (scale << 10) + 0x1000;
    TargetAngle = scale;
    StartAngle = scale;
    SetPosition();
    return 1;
}

/**
 * Move__6Cursori
 *
 * Address:   0x80085CEC
 * Mangled:   Move__6Cursori
 * Demangled: Cursor::Move(int)
 *
 * @param dir  Direction.
 */
void Cursor::Move(int dir)
{
    int index = Models[0].index;
    EditorLevelGeneratorModelsView *modelBase =
        reinterpret_cast<EditorLevelGeneratorModelsView *>(Ed);
    int flags = modelBase->SourceModelTab[index].flags;
    int inc = 1;
    if (flags & 8)
    {
        inc = 2;
    }

    switch (dir)
    {
    case 0:
        X += inc;
        break;
    case 1:
        Z -= inc;
        break;
    case 2:
        X -= inc;
        break;
    case 3:
        Z += inc;
        break;
    }

    Move(X, Y, Z);
}

/**
 * SetPosition__6Cursor
 *
 * Address:   0x80085DD4
 * Mangled:   SetPosition__6Cursor
 * Demangled: Cursor::SetPosition(void)
 *
 * Clamps the cursor footprint to the editor grid, computes its interpolated
 * screen and camera targets, and rotates each visible model into its placed
 * footprint.  The source-model flags also enforce the two-cell alignment used
 * by pieces that occupy the editor's coarse grid.
 */
void Cursor::SetPosition()
{
    static int last_x = XRot;
    static int last_z = ZRot;

    int rot_adjust = 0;
    int lowest_slot;
    int last_model;

    if ((WRot & 1) != (LRot & 1))
    {
        rot_adjust = Rot;
    }

    switch (rot_adjust)
    {
    case 0:
        XRot = X - (WRot >> 1);
        ZRot = Z - (LRot >> 1);
        break;

    case 1:
        XRot = X - (WRot >> 1);
        ZRot = Z - ((LRot + 1) >> 1);
        break;

    case 2:
        XRot = X - ((WRot + 1) >> 1);
        ZRot = Z - ((LRot + 1) >> 1);
        break;

    case 3:
        XRot = X - ((WRot + 1) >> 1);
        ZRot = Z - (LRot >> 1);
        break;
    }

    HRot = H;
    YRot = Y;

    while (XRot < 0)
    {
        XRot++;
        X++;
    }
    while (XRot + WRot > GridSize)
    {
        XRot--;
        X--;
    }
    while (ZRot < 0)
    {
        ZRot++;
        Z++;
    }
    while (ZRot + LRot > GridSize)
    {
        ZRot--;
        Z--;
    }

    if ((((EditorLevelGeneratorModelsView *)Ed)
             ->SourceModelTab[Models[0].index]
             .flags &
         8) != 0)
    {
        if ((XRot & 1) != 0)
        {
            XRot--;
            X--;
        }
        if ((ZRot & 1) != 0)
        {
            ZRot--;
            Z--;
        }
    }

    int screen_x = ((-GridSize >> 1) + XRot) * _14LevelGenerator_GridCellWidth;
    int screen_w = WRot * _14LevelGenerator_GridCellWidth;
    int screen_z = ((GridSize >> 1) - ZRot) * _14LevelGenerator_GridCellWidth;
    int screen_l = LRot * _14LevelGenerator_GridCellWidth;
    int camera_x = ((-GridSize >> 1) + X) * _14LevelGenerator_GridCellWidth;
    int camera_z = ((GridSize >> 1) - Z) * _14LevelGenerator_GridCellWidth;

    StartScreenX = ScreenX;
    StartScreenZ = ScreenZ;

    TargetScreenX = screen_x + (screen_w >> 1);
    TargetScreenZ = screen_z - (screen_l >> 1);

    TargetScreenX <<= 12;
    TargetScreenZ <<= 12;

    StartCameraX = CameraX;
    StartCameraZ = CameraZ;

    TargetCameraX = camera_x << 12;
    TargetCameraZ = camera_z << 12;

    ScreenY = (-H * _14LevelGenerator_GridCellHeight) >> 1;
    ScreenY <<= 12;

    lowest_slot = 0;
    for (int j = 0; j < LRot; j++)
    {
        for (int i = 0; i < WRot; i++)
        {
            int temp =
                _14LevelGenerator_GridTab[ZRot + j][XRot + i].numUsedSlots - 1;
            if (lowest_slot < temp)
            {
                lowest_slot = temp;
            }
        }
    }

    last_model = -1;
    for (int i = 0; i < 4; i++)
    {
        if (Models[i].index != -1)
        {
            Models[i].rot = Rot;

            switch (Rot)
            {
            case 0:
                Models[i].XRot = XRot + Models[i].X;
                Models[i].ZRot = ZRot + Models[i].Z;
                Models[i].WRot = Models[i].W;
                Models[i].LRot = Models[i].L;
                break;

            case 1:
                Models[i].XRot = XRot + Models[i].Z;
                Models[i].ZRot =
                    ZRot + LRot - Models[i].X - Models[i].W;
                Models[i].WRot = Models[i].L;
                Models[i].LRot = Models[i].W;
                break;

            case 2:
                Models[i].XRot =
                    XRot + WRot - Models[i].X - Models[i].W;
                Models[i].ZRot =
                    ZRot + LRot - Models[i].Z - Models[i].L;
                Models[i].WRot = Models[i].W;
                Models[i].LRot = Models[i].L;
                break;

            case 3:
                Models[i].XRot =
                    XRot + WRot - Models[i].Z - Models[i].L;
                Models[i].ZRot = ZRot + Models[i].X;
                Models[i].WRot = Models[i].L;
                Models[i].LRot = Models[i].W;
                break;
            }

            Models[i].YRot = Models[i].Y;
            Models[i].HRot = Models[i].H;
            Models[i].ScreenY =
                (-lowest_slot - Models[i].YRot) *
                _14LevelGenerator_GridCellHeight * 0x1000;
            Models[i].item.Model = Models[i].index;

            if (last_model >= 0)
            {
                Models[last_model].item.NextItem = &Models[i].item;
            }
            last_model = i;
        }
    }

    NewLocation = 0;

}

/**
 * Update__6Cursor
 *
 * Address:   0x800864D0
 * Mangled:   Update__6Cursor
 * Demangled: Cursor::Update(void)
 *
 * Interpolates the cursor and editor-camera positions, advances an active
 * move, and transforms each visible cursor model into world space.
 */
void Cursor::Update()
{
    Angle = (StartAngle + (((TargetAngle - StartAngle) * Gradient) >> 2)) &
            0xfff;
    ScreenX = StartScreenX +
              (((TargetScreenX - StartScreenX) * Gradient) >> 2);
    ScreenZ = StartScreenZ +
              (((TargetScreenZ - StartScreenZ) * Gradient) >> 2);
    CameraX = StartCameraX +
              (((TargetCameraX - StartCameraX) * Gradient) >> 2);
    CameraZ = StartCameraZ +
              (((TargetCameraZ - StartCameraZ) * Gradient) >> 2);

    if (NewLocation != 0)
    {
        WireframeCollidingPieces(true);
        NewLocation = 0;
    }

    if (Gradient < 4)
    {
        WireframeCollidingPieces(true);
        Gradient++;
        if (Gradient == 4)
        {
            NewLocation = (void *)1;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (Models[i].index != -1)
        {
            Models[i].ScreenX =
                ScreenX +
                (Models[i].X * EditorCosine(Angle) -
                 Models[i].Z * EditorSine(Angle)) *
                    _14LevelGenerator_GridCellWidth;
            Models[i].ScreenZ =
                ScreenZ +
                (Models[i].X * EditorSine(Angle) +
                 Models[i].Z * EditorCosine(Angle)) *
                    _14LevelGenerator_GridCellWidth;

            Models[i].item.Position.vx = Models[i].ScreenX;
            Models[i].item.Position.vy = Models[i].ScreenY;
            Models[i].item.Position.vz = Models[i].ScreenZ;
            Models[i].item.Angles.vy = 0x1000 - Angle;
        }
    }
}

// ====== 0x8008673C Draw__6Cursor ===============================================
// Address: 0x8008673C
// Demangled: Cursor::Draw(void)
/**
 * Draws the cursor piece model, toggling its visibility before and after the
 * shared cursor render pass.
 */
void Cursor::Draw()
{
    int idx = Models[0].index;
    unsigned char *models = reinterpret_cast<unsigned char *>(Ed);
    int offset = ((idx << 4) + idx) << 2;
    unsigned int flags =
        reinterpret_cast<EditorSourceModelAddressView *>(
            models + offset)->SourceModel.flags;
    unsigned int shifted = flags >> 3;
    int v = shifted & 1;
    int is_pool_piece = v;
    if (v == 0) goto skip1;
    {
        int a0arg = offset +
            sizeof(((EditorSourceModelAddressView *)0)->generatorHeader);
        reinterpret_cast<EditorInModel *>(models + a0arg)
            ->SetTransparency(true);
    }
skip1:
    M3d_Render(&Models[0]);
    if (is_pool_piece == 0) goto end;
    {
        int idx2 = Models[0].index;
        unsigned char *models2 = reinterpret_cast<unsigned char *>(Ed);
        int a0arg = ((idx2 << 4) + idx2) << 2;
        a0arg +=
            sizeof(((EditorSourceModelAddressView *)0)->generatorHeader);
        reinterpret_cast<EditorInModel *>(models2 + a0arg)
            ->SetTransparency(false);
    }
end:
    ;
}

/**
 * Stamp__6Cursor
 *
 * Address:   0x800867D4
 * Mangled:   Stamp__6Cursor
 * Demangled: Cursor::Stamp(void)
 *
 * @return 1 when the piece was placed, or 0 when placement is invalid.
 */
int Cursor::Stamp()
{
    int num_faces;
    int cost;
    int slot;
    int top_slot;
    int this_slot;
    int link1;
    EditorOutModel *mod;

    num_faces = ((EditorLevelGeneratorModelsView *)Ed)
                    ->SourceModelTab[Models[0].index]
                    .mModel
                    ->NumFaces;
    cost = 1;
    if (num_faces > 36)
    {
        cost = 2;
    }

    if (PiecesRemaining < cost)
    {
        return 0;
    }

    if (Models[0].index == NotInDemoPiece)
    {
        return 0;
    }

    slot = _14LevelGenerator_GridTab[ZRot][XRot].numUsedSlots - 1;
    if ((((EditorLevelGeneratorModelsView *)Ed)
             ->SourceModelTab[Models[0].index]
             .flags &
         0x10) != 0)
    {
        top_slot = slot + Models[0].H;
    }
    else
    {
        top_slot = slot;
    }

    if (top_slot >= 5)
    {
        return 0;
    }

    for (int z = ZRot; z < ZRot + LRot; z++)
    {
        for (int x = XRot; x < XRot + WRot; x++)
        {
            if (_14LevelGenerator_GridTab[z][x].numUsedSlots - 1 != slot)
            {
                return 0;
            }
            if (((EditorLevelGeneratorView *)Ed)
                    ->OutModelMap[z][x]
                    .modelInSlot[slot] != 0)
            {
                return 0;
            }
        }
    }

    this_slot = Models[0].YRot + slot;
    if ((((EditorLevelGeneratorModelsView *)Ed)
             ->SourceModelTab[Models[0].index]
             .flags &
         8) != 0 &&
        this_slot > 0)
    {
        return 0;
    }

    if (XRot > 0)
    {
        mod = ((EditorLevelGeneratorView *)Ed)
                  ->OutModelMap[ZRot][XRot - 1]
                  .modelInSlot[this_slot];
        if (mod != 0)
        {
            link1 = GetPoolLink(Models[0].index, 2, Rot);
            if (ComparePoolLinks(
                    link1,
                    GetPoolLink(
                        mod->SourceModel->mIndex, 0, mod->Rot)) == 0)
            {
                return 0;
            }
        }
    }

    if (XRot + WRot < GridSize)
    {
        mod = ((EditorLevelGeneratorView *)Ed)
                  ->OutModelMap[ZRot][XRot + WRot]
                  .modelInSlot[this_slot];
        if (mod != 0)
        {
            link1 = GetPoolLink(Models[0].index, 0, Rot);
            if (ComparePoolLinks(
                    link1,
                    GetPoolLink(
                        mod->SourceModel->mIndex, 2, mod->Rot)) == 0)
            {
                return 0;
            }
        }
    }

    if (ZRot > 0)
    {
        mod = ((EditorLevelGeneratorView *)Ed)
                  ->OutModelMap[ZRot - 1][XRot]
                  .modelInSlot[this_slot];
        if (mod != 0)
        {
            link1 = GetPoolLink(Models[0].index, 1, Rot);
            if (ComparePoolLinks(
                    link1,
                    GetPoolLink(
                        mod->SourceModel->mIndex, 3, mod->Rot)) == 0)
            {
                return 0;
            }
        }
    }

    if (ZRot + LRot < GridSize)
    {
        mod = ((EditorLevelGeneratorView *)Ed)
                  ->OutModelMap[ZRot + LRot][XRot]
                  .modelInSlot[this_slot];
        if (mod != 0)
        {
            link1 = GetPoolLink(Models[0].index, 3, Rot);
            if (ComparePoolLinks(
                    link1,
                    GetPoolLink(
                        mod->SourceModel->mIndex, 1, mod->Rot)) == 0)
            {
                return 0;
            }
        }
    }

    Ed->AddModel(XRot, ZRot, this_slot, Rot, Models[0].index);

    if (_14LevelGenerator_GridTab[ZRot][XRot].numUsedSlots <= this_slot)
    {
        _14LevelGenerator_GridTab[ZRot][XRot].numUsedSlots =
            Models[0].H + this_slot;
    }

    if ((((EditorLevelGeneratorModelsView *)Ed)
             ->SourceModelTab[Models[0].index]
             .flags &
         0x10) != 0)
    {
        for (int z = ZRot; z < ZRot + LRot; z++)
        {
            for (int x = XRot; x < XRot + WRot; x++)
            {
                _14LevelGenerator_GridTab[z][x].numUsedSlots =
                    Models[0].H + this_slot + 1;
                for (int s = 1; s < Models[0].H + 1; s++)
                {
                    _14LevelGenerator_GridTab[z][x].modelIndex[this_slot + s] = -1;
                }
            }
        }
    }

    PiecesRemaining -= cost;
    return 1;

}

/**
 * Remove__6Cursor
 *
 * Address:   0x80086DCC
 * Mangled:   Remove__6Cursor
 * Demangled: Cursor::Remove(void)
 *            Removes models under the cursor footprint and refunds their demo-piece cost.
 *
 * @return 1 when at least one model was removed, otherwise 0.
 */
int Cursor::Remove()
{
    int successful_remove = 0;
    int lowest_removal_slot = 0;
    struct EditorInModel *in_model;

    int cursor_z = ZRot;
    if (cursor_z < cursor_z + LRot)
    {
        do
        {
            int cursor_x = XRot;
            if (cursor_x < cursor_x + WRot)
            {
                do
                {
                    int result = Ed->RemoveModelFromMap(
                        cursor_x, cursor_z, &lowest_removal_slot,
                        &in_model);
                    if (result != 0)
                    {
                        successful_remove = 1;
                    }

                    int cost = 1;
                    int source_index = in_model->mIndex;
                    EditorLevelGeneratorModelsView *generator =
                        reinterpret_cast<EditorLevelGeneratorModelsView *>(Ed);
                    int num_faces = generator->SourceModelTab[source_index]
                                        .mModel->NumFaces;
                    if (num_faces >= 0x25)
                    {
                        cost = 2;
                    }
                    if (result != 0)
                    {
                        PiecesRemaining += cost;
                    }
                    cursor_x++;
                } while (cursor_x < XRot + WRot);
            }
            cursor_z++;
        } while (cursor_z < ZRot + LRot);
    }

    Move(X, Y, Z);
    return successful_remove;
}

/**
 * WireframeCollidingPieces__6Cursorb
 *
 * Address:   0x80086F10
 * Mangled:   WireframeCollidingPieces__6Cursorb
 * Demangled: Cursor::WireframeCollidingPieces(bool)
 *
 * Applies or removes wireframe visibility on every placed model beneath the
 * cursor footprint, including models stacked in higher occupancy slots.
 *
 * @param wireframe_on Whether intersecting models should be wireframed.
 */
void Cursor::WireframeCollidingPieces(bool wireframe_on)
{
    int ztop = ZRot;
    if (ztop < 0)
    {
        ztop = 0;
    }

    int zbottom = (ZRot + LRot > GridSize)
                      ? GridSize
                      : ZRot + LRot;

    int xleft = XRot;
    if (xleft < 0)
    {
        xleft = 0;
    }

    int xright = (XRot + WRot > GridSize)
                     ? GridSize
                     : XRot + WRot;

    int highest_slot = 0;
    for (int z = ztop; z < zbottom; z++)
    {
        for (int x = xleft; x < xright; x++)
        {
            int cell_highest = _14LevelGenerator_GridTab[z][x].numUsedSlots - 1;
            if (highest_slot < cell_highest)
            {
                highest_slot = cell_highest;
            }
        }
    }

    for (int slot = 0; slot < highest_slot + 1; slot++)
    {
        for (int z = ztop; z < zbottom; z++)
        {
            for (int x = xleft; x < xright; x++)
            {
                EditorOutModel *das_model =
                    ((EditorLevelGeneratorView *)Ed)
                        ->OutModelMap[z][x]
                        .modelInSlot[slot];

                if (das_model != 0 &&
                    (das_model->SourceModel->flags & 0x10) == 0)
                {
                    Ed->UnhideFaces(das_model, wireframe_on);

                    if (wireframe_on)
                    {
                        das_model->TargetModel->Flags |= 0x400;
                    }
                    else
                    {
                        das_model->TargetModel->Flags &=
                            (unsigned short)~0x420;
                    }

                    Ed->UnhideFloor(das_model, wireframe_on);
                }
            }
        }
    }
}

/**
 * PushbackPieces__6Cursorb
 *
 * Address:   0x800871AC
 * Mangled:   PushbackPieces__6Cursorb
 * Demangled: Cursor::PushbackPieces(bool)
 *
 * @param draw_back When true, keep the nearer sorted group visible and push the
 *                 remaining group back; false selects the complementary group.
 */
void Cursor::PushbackPieces(bool draw_back)
{
    if (ResortGrid != 0)
    {
        SortModels();
        ResortGrid = 0;
    }

    {
        int i = 0;
        int last_blocked = 0;
        if (LatestModel > 0)
        {
            do
            {
                EditorOutModel *model = SortList[i];
                void *behind = (void *)1;
                if (CameraDirX < 0 &&
                    XRot + WRot <= model->XCell)
                {
                    behind = 0;
                }
                else if (CameraDirX > 0 &&
                         model->XCell + model->WCell <= XRot)
                {
                    behind = 0;
                }
                if (CameraDirZ < 0 &&
                    ZRot + LRot <= model->ZCell)
                {
                    behind = 0;
                }
                else if (CameraDirZ > 0 &&
                         model->ZCell + model->LCell <= ZRot)
                {
                    behind = 0;
                }

                if (behind && IsAFloorPiece(model->SourceModel->mIndex) == 0)
                {
                    last_blocked = i;
                }

                i++;
            } while (i < LatestModel);
        }

        {
            int i = 0;
            bool reversePushback = !draw_back;
            if (LatestModel > 0)
            {
                EditorOutModel **modelSlot = SortList;
                do
                {
                    EditorOutModel *model = *modelSlot;
                    void *draw_it = (void *)(i <= last_blocked
                                                 ? draw_back
                                                 : reversePushback);

                    if (draw_it)
                    {
                        if ((model->TargetModel->Flags & 2) == 0)
                        {
                            model->TargetModel->Flags &= (unsigned short)~0x20;
                        }
                    }
                    else
                    {
                        model->TargetModel->Flags |= 0x20;
                    }

                    i++;
                    modelSlot++;
                } while (i < LatestModel);
            }

            Ed->HideWalls(reversePushback);
        }
    }
}

/**
 * AddSortModel__6CursorP14EditorOutModel
 *
 * Address:   0x80087390
 * Mangled:   AddSortModel__6CursorP14EditorOutModel
 * Demangled: Cursor::AddSortModel(EditorOutModel *)
 *
 * @param new_model  Model to insert into the camera-relative sort order.
 */
void Cursor::AddSortModel(struct EditorOutModel *new_model)
{
    EditorOutModel *temp;
    int i;

    for (i = 0; i < LatestModel; i++)
    {
        void *behind = (void *)1;

        if (CameraDirX < 0 &&
            SortList[i]->XCell + SortList[i]->WCell <= new_model->XCell)
        {
            behind = 0;
        }
        else if (CameraDirX > 0 &&
                 new_model->XCell + new_model->WCell <= SortList[i]->XCell)
        {
            behind = 0;
        }

        if (CameraDirZ < 0 &&
            SortList[i]->ZCell + SortList[i]->LCell <= new_model->ZCell)
        {
            behind = 0;
        }
        else if (CameraDirZ > 0 &&
                 new_model->ZCell + new_model->LCell <= SortList[i]->ZCell)
        {
            behind = 0;
        }

        if (behind)
            break;
    }

    while (i < LatestModel)
    {
        temp = SortList[i];
        SortList[i] = new_model;
        new_model = temp;
        i++;
    }

    SortList[LatestModel] = new_model;
    LatestModel++;
}

/**
 * SortModels__6Cursor
 *
 * Address:   0x800874D8
 * Mangled:   SortModels__6Cursor
 * Demangled: Cursor::SortModels(void)
 *            Build the cursor's sorted model list from the two editor model chains.
 *
 */
void Cursor::SortModels()
{
    int val1 = CamVect.vx;
    CameraDirX = 0;
    if (val1 < 0)
    {
        CameraDirX = -1;
    }
    else if (val1 > 0)
    {
        CameraDirX = 1;
    }

    int val2 = CamVect.vz;
    CameraDirZ = 0;
    if (val2 < 0)
    {
        CameraDirZ = 1;
    }
    else if (val2 > 0)
    {
        CameraDirZ = -1;
    }

    LatestModel = 0;

    int list;
    for (list = 0; list < 2; list++)
    {
        EditorLevelGeneratorListsView *generator =
            reinterpret_cast<EditorLevelGeneratorListsView *>(Ed);
        EditorOutModel *model;
        if (list == 0)
        {
            model = generator->OutModelList;
        }
        else
        {
            model = generator->FloorModelList;
        }
        while (model != 0)
        {
            AddSortModel(model);
            model = model->Next;
        }
    }
}

/**
 * SetCameraTargetVector__6CursorG7SVector
 *
 * Address:   0x800875A8
 * Mangled:   SetCameraTargetVector__6CursorG7SVector
 * Demangled: Cursor::SetCameraTargetVector(SVector)
 *
 * @param camera  New camera-target direction vector.
 */
void Cursor::SetCameraTargetVector(SVector camera)
{
    CamVect = camera;
}

// GCC 2.x emits a private copy of CClass::~CClass at the end of every owner
// containing a CClass-derived hierarchy. MAIN.SYM records this copy at the
// byte immediately after Cursor::SetCameraTargetVector.
extern "C" {
static void __DOT__6CClass_800875D0(CClass *base, int flags);
}

/**
 * Destroy the private CClass base emitted at the end of LevelEditor.obj.
 *
 * @param base Base-subobject address.
 * @param flags GCC 2.x in-charge flags; bit zero also deletes the object.
 */
static void __DOT__6CClass_800875D0(CClass *base, int flags)
{
    base->vtable = &__vt_11LevelEditor_base;
    if (flags & 1)
    {
        ::CClass::operator delete(base);
    }
}
