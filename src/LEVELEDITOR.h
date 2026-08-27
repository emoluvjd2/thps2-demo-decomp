// Public interface and recovered layouts for LEVELEDITOR.cpp.
#ifndef LEVELEDITOR_INC_H
#define LEVELEDITOR_INC_H

#include "globals.h"
#include "LEVELEDITOR_STATE.h"
#include "FACEREMOVAL.h"
#include "FONTTOOLS.h"

#ifdef __cplusplus
extern "C" {
#endif

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

#include "SVIEWPORT.h"

struct SDoubleBuffer;
struct SViewport;

#ifdef __cplusplus
}
#endif

/*
 * Keep the recovered C++ owner types outside the C-linkage declaration block.
 * GCC 2.91 uses this context when selecting its deleting-destructor ABI.
 */
struct SelectManager : CClass {
#ifdef __cplusplus
    /** Constructs the editor's piece-selection menu. */
    SelectManager();
    /** Draws the editor piece-selection menu. */
    void Draw();
    /** Advances selector animation, prompts, and remaining-piece text. */
    void Update();
    /** Select and refresh one editor piece. */
    void SetPiece(int piece);
    /** Move the piece selection within the active category. */
    void ChangePiece(int direction);
    void SetCategory(int category);
    /** Move the category selection with wrapping. */
    void ChangeCategory(int direction);
    /** Open or hide the selector menu. */
    void SetMenuState(bool turnOn);
    /** Return the active piece index for the current theme and category. */
    int GetPieceIndex();
    /** Store the active editor theme. */
    void SetTheme(int theme);
    ~SelectManager();
#endif
    int WindowTop;
    int WindowTarget;
    int WindowHeight;
    int WindowMidpoint;
    char CatString[2][40];
    int CatStringX[2];
    int CatStringY[2];
    char PieceString[4][40];
    int PieceStringX[4];
    int PieceStringY[4];
    int OldPiecesRemaining;
    int Blink;
    void * NotInDemo;
    struct Sprite2 *LeftButton;
    int LeftX;
    struct Sprite2 *RightButton;
    int RightX;
    struct ScreenElement (*PromptItem[4]);
    int Theme;
    int Category;
    int Piece;
    struct Sprite2 *GridSprt;
    struct Sprite2 (*IconSprt[256]);
    int NumPieces;
    int NumCategories;
    int FirstPieceInCategory;
    int NumPiecesInCategory;
    int PieceToUnpack;
    unsigned char UnpackCount;
    FontStorage TheFont;
    FontStorage PromptFont;
};

struct CursorModel {
    struct Item item;
    int index;
    int rot;
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
    int ScreenX;
    int ScreenY;
    int ScreenZ;
};

/** States in the level editor's pause/menu workflow. */
enum EnumMenuState {
    NOTHING = 0,
    MAIN = 1,
    SAVE = 2,
    LOAD = 3,
    DONE = 4,
    SET_THEME = 5,
    VERIFY_CHOICE = 6,
};

struct LevelEditor {
#ifdef __cplusplus
    /** Constructs a level-editor session and its owned subsystems. */
    LevelEditor();
    /** Destroys the level editor and its owned subsystems. */
    ~LevelEditor();
    /** Release editor storage through the shared CClass arena. */
    static void operator delete(void *p) { ::CClass::operator delete(p); }
    /** Draws the level editor scene and active menus. */
    void Draw();
    /** Advances the editor and reports whether it is leaving. */
    int Update();
    /** Handles editor controller input. */
    void HandleInput();
    /** Clears the level editor's menu workflow state. */
    void CreateMenu();
    void UpdateMenu();
#endif
    struct CClass CClass;
    int NewTheme;
    struct TextMenuWrapper *Wrapper;
    enum EnumMenuState MenuState;
    enum EnumMenuState LastMenuState;
    char SaveName[14];
    void * DidLoad;
    struct SelectManager *PieceManager;
    struct LevelGenerator *Gen;
    struct Item *pItemList;
    struct EditorCamera *mCamera;
    struct Cursor *mCursor;
    struct SViewport mViewPort;
    int mTime;
    int mRegion;
    int mPadWait;
};

struct EditorCamera_alt_definition_0001 {
    struct CClass CClass;
    int Type;
    int Rot;
    int TargetRot;
    int Change;
    struct CBody *CamTri;
    struct CCamera *Camera;
    struct CVector Tripod;
    struct CBody *Target;
};

struct EditorCamera_alt_definition_0002 {
    struct CClass CClass;
    int Type;
    int Rot;
    int TargetRot;
    int Change;
    struct CBody *CamTri;
    struct CCamera *Camera;
    struct CVector Tripod;
    struct CBody *Target;
};

struct EditorCamera : CClass {
#ifdef __cplusplus
    /** Constructs the editor camera and its owned runtime bodies. */
    EditorCamera();
    /** Stores the editor camera's target position. */
    void SetTarget(int x, int y, int z);
    /** Advances to the next editor camera view type. */
    void SwitchType();
    /** Begins the next editor camera quarter-turn. */
    void Rotate();
    /** Updates the editor camera's rotation, tripod, and focus. */
    void Update();
    /** Maps a directional press into camera-relative space. */
    int TranslatePress(int press);
    /** Return the fixed-point direction from the target to the look-at point. */
    SVector GetCameraTargetVector();
    ~EditorCamera();
#endif
    int Type;
    int Rot;
    int TargetRot;
    int Change;
    struct CBody *CamTri;
    struct CCamera *Camera;
    struct CVector Tripod;
    struct CBody *Target;
};

typedef char level_editor_Font_size_must_be_80[
    (sizeof(FontStorage) == 80) ? 1 : -1];
typedef char level_editor_SelectManager_size_must_be_1576[(sizeof(SelectManager) == 1576) ? 1 : -1];
typedef char level_editor_CursorModel_size_must_be_104[(sizeof(CursorModel) == 104) ? 1 : -1];
typedef char level_editor_LevelEditor_size_must_be_92[(sizeof(LevelEditor) == 92) ? 1 : -1];
typedef char level_editor_EditorCamera_size_must_be_44[(sizeof(EditorCamera) == 44) ? 1 : -1];

// === Variables (gp-relative in this TU) ===
#ifdef LEVELEDITOR_OWNER_DECLARATIONS
#define GENERATELEVEL_OWNER_DECLARATIONS
#include "GENERATELEVEL.h"
#undef GENERATELEVEL_OWNER_DECLARATIONS
#endif
extern "C" int PiecesRemaining;       // 0x800DC180

// === Variables (absolute in this TU) ===
#include "PDOUBLEBUFFER.h"

/** Runs the interactive level editor until the user exits it. */
void LevelEditor_Main(void);

#endif // LEVELEDITOR_INC_H
