#ifndef EDITORPIECES_H
#define EDITORPIECES_H

/** Selects the active theme and rebuilds its special-piece mappings. */
void EditorPieces_SetTheme(int theme);

/** Finds a theme-specific piece in PieceMap, or returns -1. */
int MapPieceToIndex(int piece, int theme);

/** Maps a theme-zero piece identifier into the requested theme. */
int MapBasePieceToTheme(int piece, int theme);

/** Reports whether an identifier is one of the configured floor pieces. */
int IsAFloorPiece(int piece);

/** Converts a runtime piece identifier to its stable save-table index. */
int EditorPieces_TranslatePieceOut(int piece);

/** Converts a stable save-table index into the active theme's piece. */
int EditorPieces_TranslatePieceIn(int save_index);

/** Returns the flags attached to a runtime piece identifier. */
unsigned char EditorPieces_GetPieceFlags(int piece_number);

/** Returns one rotated edge link for a pool piece. */
int GetPoolLink(int piece, int side, int rot);

/** Reports whether two encoded pool-edge links are compatible. */
int ComparePoolLinks(int link1, int link2);

enum {
    EditorPieceCount = 151,
    EditorCategoryCount = 18,
    EditorThemeMapCount = 4,
    EditorFloorPieceCount = 5,
    EditorPieceUnavailable = 0xFF,
};

/** Metadata and theme mappings for one level-editor piece. */
struct EditorPiece {
    unsigned char map[EditorThemeMapCount];
    char iconFile[13];
    char name[32];
    unsigned char flags;
};

/** One entry in the level editor's ordered category menu. */
struct EditorCategory {
    unsigned char firstPiece;
    char name[32];
    unsigned char lastUsedPiece;
};

typedef char EditorPiece_size_must_be_50[
    (sizeof(EditorPiece) == 50) ? 1 : -1];
typedef char EditorCategory_size_must_be_34[
    (sizeof(EditorCategory) == 34) ? 1 : -1];

extern "C" {
/** Theme-specific replacement used for pieces unavailable in the demo. */
extern int NotInDemoPiece;

/** Editor piece metadata in stable save-table order. */
extern EditorPiece PieceMap[];

/** Ordered level-editor category metadata. */
extern EditorCategory CategoryMap[];

/**
 * Active-theme identifiers for the supported floor-piece footprints.
 *
 * LEVELGEN's target object records the complete import sizes. Other consumers
 * retain incomplete declarations so they do not acquire those assembler-only
 * `.extern` size records.
 */
#ifdef EDITORPIECES_SIZED_FLOOR_DECLARATIONS
#define EDITOR_FLOOR_PIECE_DECL_BOUND EditorFloorPieceCount
#else
#define EDITOR_FLOOR_PIECE_DECL_BOUND
#endif

extern int Floor1X1Piece[EDITOR_FLOOR_PIECE_DECL_BOUND];
extern int Floor1X2Piece[EDITOR_FLOOR_PIECE_DECL_BOUND];
extern int Floor2X2Piece[EDITOR_FLOOR_PIECE_DECL_BOUND];

#undef EDITOR_FLOOR_PIECE_DECL_BOUND
}

#ifdef EDITORPIECES_OWNER_DECLARATIONS

enum {
    EditorPoolLinkCount = 6,
    EditorSaveMapCount = 145,
    EditorPoolSideCount = 4,
    EditorSaveMapEnd = 0xFE,
    EditorPoolSideMask = EditorPoolSideCount - 1,
    EditorLinkNibbleBits = 4,
    EditorLinkNibbleMask = 0xF,
};

/** Rotation-sensitive edge links used only by the editor-piece owner. */
struct PoolLinks {
    unsigned char piece;
    unsigned char link[EditorPoolSideCount];
};

typedef char PoolLinks_size_must_be_5[
    (sizeof(PoolLinks) == 5) ? 1 : -1];

extern "C" {
extern int RiserPiece;
static int Theme;
extern PoolLinks PoolLinkMap[EditorPoolLinkCount];
extern unsigned char SaveMap[EditorSaveMapCount];
}

#endif // EDITORPIECES_OWNER_DECLARATIONS

#endif // EDITORPIECES_H
