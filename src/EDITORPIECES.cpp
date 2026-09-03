// EDITORPIECES.cpp -- Reconstructed translation unit
// Original: EDITORPIECES.CPP
// Functions: 9 reconstructed entries
// Address range: 0x80082A50 - 0x80082F10
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#define EDITORPIECES_OWNER_DECLARATIONS
#include "EDITORPIECES.h"
#undef EDITORPIECES_OWNER_DECLARATIONS
#include <STDIO.H>

/** Maps a theme-zero piece identifier into the requested theme. */
int MapBasePieceToTheme(int piece, int theme);

#define EDITOR_PIECE(piece, icon, display_name, piece_flags) \
    {{piece, piece, piece, piece}, icon, display_name, piece_flags}

/**
 * Theme-independent editor-piece metadata in the order used by save files.
 *
 * The prototype ships one mapping value per entry for all four themes, while
 * retaining the four-byte map for the retail theme-specific format.
 */
EditorPiece PieceMap[EditorPieceCount] __attribute__((section(".data"))) = {
    EDITOR_PIECE(0x7C, "EdIc1_00.bmp", "under construction", 0x80),
    EDITOR_PIECE(0x0E, "EdIc1_28.bmp", "low single corner", 0x80),
    EDITOR_PIECE(0x10, "EdIc1_30.bmp", "low double corner", 0x80),
    EDITOR_PIECE(0x0F, "EdIc1_29.bmp", "high single corner", 0x80),
    EDITOR_PIECE(0x11, "EdIc1_31.bmp", "high double corner", 0x80),
    EDITOR_PIECE(0x25, "EdIc1_10.bmp", "Wall piece", 0x80),
    EDITOR_PIECE(0x26, "EdIc1_10.bmp", "Wall piece with window", 0x80),
    EDITOR_PIECE(0x3B, "EdIc1_10.bmp", "Corner wall piece", 0x80),
    EDITOR_PIECE(0x19, "EdIc1_12.bmp", "high triple", 0x84),
    EDITOR_PIECE(0x2C, "EdIc1_15.bmp", "low single", 0x10),
    EDITOR_PIECE(0x60, "EdIc1_15.bmp", "low double", 0x10),
    EDITOR_PIECE(0x61, "EdIc1_15.bmp", "low quad", 0x10),
    EDITOR_PIECE(0x64, "EdIc1_15.bmp", "medium single", 0x10),
    EDITOR_PIECE(0x63, "EdIc1_15.bmp", "medium double", 0x10),
    EDITOR_PIECE(0x62, "EdIc1_15.bmp", "medium quad", 0x10),
    EDITOR_PIECE(0x67, "EdIc1_15.bmp", "high single", 0x10),
    EDITOR_PIECE(0x66, "EdIc1_15.bmp", "high double", 0x10),
    EDITOR_PIECE(0x65, "EdIc1_15.bmp", "high quad", 0x10),
    EDITOR_PIECE(0x03, "EdIc1_00.bmp", "low single", 0x04),
    EDITOR_PIECE(0x7E, "EdIc1_00.bmp", "low double", 0x04),
    EDITOR_PIECE(0x06, "EdIc1_19.bmp", "low quad", 0x04),
    EDITOR_PIECE(0x13, "EdIc1_02.bmp", "low corner", 0x04),
    EDITOR_PIECE(0x14, "EdIc1_04.bmp", "low outside corner", 0x00),
    EDITOR_PIECE(0x02, "EdIc1_01.bmp", "high single", 0x04),
    EDITOR_PIECE(0x00, "EdIc1_01.bmp", "high double", 0x04),
    EDITOR_PIECE(0x05, "EdIc1_18.bmp", "high quad", 0x04),
    EDITOR_PIECE(0x01, "EdIc1_03.bmp", "high corner", 0x04),
    EDITOR_PIECE(0x04, "EdIc1_05.bmp", "high outside corner", 0x00),
    EDITOR_PIECE(0x7F, "EdIc1_20.bmp", "low to high", 0x04),
    EDITOR_PIECE(0x07, "EdIc1_24.bmp", "high to low", 0x04),
    EDITOR_PIECE(0x08, "EdIc1_21.bmp", "low spine", 0x04),
    EDITOR_PIECE(0x09, "EdIc1_22.bmp", "low hump", 0x04),
    EDITOR_PIECE(0x0A, "EdIc1_23.bmp", "high spine", 0x04),
    EDITOR_PIECE(0x83, "EdIc1_00.bmp", "low single", 0x04),
    EDITOR_PIECE(0x8F, "EdIc1_00.bmp", "low double", 0x04),
    EDITOR_PIECE(0x86, "EdIc1_19.bmp", "low quad", 0x04),
    EDITOR_PIECE(0x8B, "EdIc1_02.bmp", "low corner", 0x04),
    EDITOR_PIECE(0x8C, "EdIc1_04.bmp", "low outside corner", 0x00),
    EDITOR_PIECE(0x82, "EdIc1_01.bmp", "high single", 0x04),
    EDITOR_PIECE(0x89, "EdIc1_01.bmp", "high double", 0x04),
    EDITOR_PIECE(0x85, "EdIc1_18.bmp", "high quad", 0x04),
    EDITOR_PIECE(0x81, "EdIc1_03.bmp", "high corner", 0x04),
    EDITOR_PIECE(0x84, "EdIc1_05.bmp", "high outside corner", 0x00),
    EDITOR_PIECE(0x90, "EdIc1_20.bmp", "low to high", 0x04),
    EDITOR_PIECE(0x87, "EdIc1_24.bmp", "high to low", 0x04),
    EDITOR_PIECE(0x88, "EdIc1_21.bmp", "low spine", 0x04),
    EDITOR_PIECE(0x80, "EdIc1_22.bmp", "low hump", 0x04),
    EDITOR_PIECE(0x8A, "EdIc1_23.bmp", "high spine", 0x04),
    EDITOR_PIECE(0x3C, "EdIc1_34.bmp", "low single", 0x01),
    EDITOR_PIECE(0x37, "EdIc1_34.bmp", "low double", 0x01),
    EDITOR_PIECE(0x1E, "EdIc1_34.bmp", "low quad", 0x01),
    EDITOR_PIECE(0x36, "EdIc1_34.bmp", "low angle", 0x01),
    EDITOR_PIECE(0x20, "EdIc1_34.bmp", "low corner", 0x01),
    EDITOR_PIECE(0x46, "EdIc1_34.bmp", "low junction", 0x01),
    EDITOR_PIECE(0x58, "EdIc1_34.bmp", "high single", 0x01),
    EDITOR_PIECE(0x57, "EdIc1_34.bmp", "high double", 0x01),
    EDITOR_PIECE(0x54, "EdIc1_34.bmp", "high quad", 0x01),
    EDITOR_PIECE(0x56, "EdIc1_34.bmp", "high angle", 0x01),
    EDITOR_PIECE(0x55, "EdIc1_34.bmp", "high corner", 0x01),
    EDITOR_PIECE(0x53, "EdIc1_34.bmp", "high junction", 0x01),
    EDITOR_PIECE(0x3F, "EdIc1_34.bmp", "low single", 0x01),
    EDITOR_PIECE(0x3E, "EdIc1_34.bmp", "low double", 0x01),
    EDITOR_PIECE(0x3D, "EdIc1_34.bmp", "low quad", 0x01),
    EDITOR_PIECE(0x68, "EdIc1_34.bmp", "low angle", 0x01),
    EDITOR_PIECE(0x6A, "EdIc1_34.bmp", "low corner", 0x01),
    EDITOR_PIECE(0x5B, "EdIc1_34.bmp", "high single", 0x01),
    EDITOR_PIECE(0x5A, "EdIc1_34.bmp", "high double", 0x01),
    EDITOR_PIECE(0x59, "EdIc1_34.bmp", "high quad", 0x01),
    EDITOR_PIECE(0x69, "EdIc1_34.bmp", "high angle", 0x01),
    EDITOR_PIECE(0x6B, "EdIc1_34.bmp", "high corner", 0x01),
    EDITOR_PIECE(0x21, "EdIc1_32.bmp", "low single", 0x00),
    EDITOR_PIECE(0x22, "EdIc1_32.bmp", "low double", 0x00),
    EDITOR_PIECE(0x24, "EdIc1_32.bmp", "medium single", 0x00),
    EDITOR_PIECE(0x23, "EdIc1_32.bmp", "medium double", 0x00),
    EDITOR_PIECE(0x12, "EdIc1_32.bmp", "low single", 0x04),
    EDITOR_PIECE(0x4F, "EdIc1_34.bmp", "low single low rail", 0x04),
    EDITOR_PIECE(0x5C, "EdIc1_34.bmp", "low single high rail", 0x04),
    EDITOR_PIECE(0x0B, "EdIc1_25.bmp", "low double", 0x04),
    EDITOR_PIECE(0x51, "EdIc1_34.bmp", "low double low rail", 0x04),
    EDITOR_PIECE(0x5E, "EdIc1_34.bmp", "low double high rail", 0x04),
    EDITOR_PIECE(0x0D, "EdIc1_27.bmp", "high single", 0x04),
    EDITOR_PIECE(0x50, "EdIc1_34.bmp", "high single low rail", 0x04),
    EDITOR_PIECE(0x5D, "EdIc1_34.bmp", "high single high rail", 0x04),
    EDITOR_PIECE(0x0C, "EdIc1_26.bmp", "high double", 0x04),
    EDITOR_PIECE(0x52, "EdIc1_34.bmp", "high double low rail", 0x04),
    EDITOR_PIECE(0x5F, "EdIc1_34.bmp", "high double high rail", 0x04),
    EDITOR_PIECE(0x49, "EdIc1_15.bmp", "Double set", 0x04),
    EDITOR_PIECE(0x4A, "EdIc1_15.bmp", "double set low rail", 0x04),
    EDITOR_PIECE(0x70, "EdIc1_15.bmp", "double set high rail", 0x04),
    EDITOR_PIECE(0x4B, "EdIc1_15.bmp", "double set corner", 0x00),
    EDITOR_PIECE(0x4E, "EdIc1_15.bmp", "double set inside corner", 0x00),
    EDITOR_PIECE(0x43, "EdIc1_15.bmp", "triple set", 0x04),
    EDITOR_PIECE(0x45, "EdIc1_15.bmp", "triple set low rail", 0x04),
    EDITOR_PIECE(0x6F, "EdIc1_15.bmp", "triple set high rail", 0x04),
    EDITOR_PIECE(0x48, "EdIc1_15.bmp", "triple set corner", 0x00),
    EDITOR_PIECE(0x4C, "EdIc1_15.bmp", "triple set inside corner", 0x00),
    EDITOR_PIECE(0x42, "EdIc1_15.bmp", "quad set", 0x04),
    EDITOR_PIECE(0x44, "EdIc1_15.bmp", "quad set low rail", 0x04),
    EDITOR_PIECE(0x6E, "EdIc1_15.bmp", "quad set high rail", 0x04),
    EDITOR_PIECE(0x47, "EdIc1_15.bmp", "quad set corner", 0x00),
    EDITOR_PIECE(0x4D, "EdIc1_15.bmp", "quad set inside corner", 0x00),
    EDITOR_PIECE(0x6C, "EdIc1_16.bmp", "bowl", 0x0B),
    EDITOR_PIECE(0x7D, "EdIc1_16.bmp", "pool", 0x0B),
    EDITOR_PIECE(0x73, "EdIc1_16.bmp", "side", 0x8A),
    EDITOR_PIECE(0x75, "EdIc1_16.bmp", "corner", 0x8A),
    EDITOR_PIECE(0x76, "EdIc1_16.bmp", "outside corner", 0x8A),
    EDITOR_PIECE(0x74, "EdIc1_16.bmp", "floor", 0x8A),
    EDITOR_PIECE(0x6D, "EdIc1_16.bmp", "pungee pit", 0x8B),
    EDITOR_PIECE(0x1C, "EdIc1_16.bmp", "small", 0x05),
    EDITOR_PIECE(0x16, "EdIc1_09.bmp", "large", 0x05),
    EDITOR_PIECE(0x1D, "EdIc1_17.bmp", "octagonal", 0x01),
    EDITOR_PIECE(0x95, "EdIc1_17.bmp", "long oct", 0x01),
    EDITOR_PIECE(0x91, "EdIc1_17.bmp", "high oct", 0x01),
    EDITOR_PIECE(0x92, "EdIc1_17.bmp", "long high oct", 0x01),
    EDITOR_PIECE(0x93, "EdIc1_17.bmp", "bump", 0x01),
    EDITOR_PIECE(0x94, "EdIc1_17.bmp", "pyramid", 0x01),
    EDITOR_PIECE(0x1F, "EdIc1_33.bmp", "spine box", 0x05),
    EDITOR_PIECE(0x29, "EdIc1_06.bmp", "Taxi", 0x01),
    EDITOR_PIECE(0x27, "EdIc1_11.bmp", "Pyramid hip", 0x05),
    EDITOR_PIECE(0x28, "EdIc1_11.bmp", "Twinkie", 0x05),
    EDITOR_PIECE(0x38, "EdIc1_08.bmp", "rausch ramp", 0x05),
    EDITOR_PIECE(0x18, "EdIc1_11.bmp", "small kicker", 0x04),
    EDITOR_PIECE(0x15, "EdIc1_07.bmp", "large kicker", 0x04),
    EDITOR_PIECE(0x71, "EdIc1_23.bmp", "park bench", 0x00),
    EDITOR_PIECE(0x72, "EdIc1_23.bmp", "angle park bench", 0x00),
    EDITOR_PIECE(0x78, "EdIc1_23.bmp", "bench", 0x00),
    EDITOR_PIECE(0x7A, "EdIc1_23.bmp", "angle bench", 0x00),
    EDITOR_PIECE(0x77, "EdIc1_23.bmp", "lunch table", 0x00),
    EDITOR_PIECE(0x79, "EdIc1_23.bmp", "angle lunch table", 0x00),
    EDITOR_PIECE(0x7B, "EdIc1_23.bmp", "slope lunch table", 0x01),
    EDITOR_PIECE(0x35, "EdIc1_14.bmp", "Neversoft", 0x81),
    EDITOR_PIECE(0x2E, "EdIc1_14.bmp", "rough", 0x81),
    EDITOR_PIECE(0x33, "EdIc1_14.bmp", "plate", 0x81),
    EDITOR_PIECE(0x34, "EdIc1_14.bmp", "manhole", 0x81),
    EDITOR_PIECE(0x31, "EdIc1_14.bmp", "wood", 0x81),
    EDITOR_PIECE(0x40, "EdIc1_00.bmp", "fountain", 0x81),
    EDITOR_PIECE(0x41, "EdIc1_00.bmp", "lightpost", 0x81),
    EDITOR_PIECE(0x1B, "EdIc1_14.bmp", "pillar", 0x81),
    EDITOR_PIECE(0x3A, "EdIc1_14.bmp", "beat up car", 0x91),
    EDITOR_PIECE(0x17, "EdIc1_10.bmp", "crate", 0x81),
    EDITOR_PIECE(0x1A, "EdIc1_10.bmp", "trashcan", 0x81),
    EDITOR_PIECE(0x2D, "EdIc1_14.bmp", "sawhorse", 0x91),
    EDITOR_PIECE(0x39, "EdIc1_14.bmp", "table", 0x91),
    EDITOR_PIECE(0x2F, "EdIc1_15.bmp", "low small wood", 0x92),
    EDITOR_PIECE(0x30, "EdIc1_10.bmp", "small wood", 0x81),
    EDITOR_PIECE(0x32, "EdIc1_13.bmp", "big wood", 0x81),
    EDITOR_PIECE(0x2A, "EdIc1_23.bmp", "low rollin", 0x04),
    EDITOR_PIECE(0x2B, "EdIc1_23.bmp", "high rollin", 0x04),
    EDITOR_PIECE(0x8D, "EdIc1_23.bmp", "low rollin wood", 0x04),
    EDITOR_PIECE(0x8E, "EdIc1_23.bmp", "high rollin wood", 0x04),
    EDITOR_PIECE(EditorPieceUnavailable, "", "", 0x00),
};

/** Pool-edge compatibility nibble pairs for each rotatable pool piece. */
PoolLinks PoolLinkMap[EditorPoolLinkCount]
    __attribute__((section(".data"))) = {
    {0x73, {0x21, 0x00, 0x12, 0x33}},
    {0x75, {0x00, 0x00, 0x12, 0x21}},
    {0x76, {0x33, 0x21, 0x12, 0x33}},
    {0x74, {0x33, 0x33, 0x33, 0x33}},
    {0x6D, {0x44, 0x44, 0x44, 0x44}},
    {EditorPieceUnavailable, {0x00, 0x00, 0x00, 0x00}},
};

/** Ordered editor category names and their first prototype piece IDs. */
EditorCategory CategoryMap[EditorCategoryCount]
    __attribute__((section(".data"))) = {
    {0x2C, "Risers", 0},
    {0x03, "Quarter pipes", 0},
    {0x83, "Quarter pipes 2", 0},
    {0x3C, "Rails", 0},
    {0x3F, "Offset Rails", 0},
    {0x21, "Walls", 0},
    {0x12, "Slopes", 0},
    {0x49, "Stairs", 0},
    {0x6C, "Pools", 0},
    {0x1C, "Funboxes", 0},
    {0x18, "Kickers", 0},
    {0x71, "Benches", 0},
    {0x35, "Floor", 0},
    {0x40, "Objects", 0},
    {0x17, "Interactive Objects", 0},
    {0x2F, "Signs", 0},
    {0x2A, "Misc", 0},
    {EditorPieceUnavailable, "", 0},
};

/**
 * Stable save-file piece ordering. FF entries are intentionally unavailable
 * slots; FE terminates the table.
 */
unsigned char SaveMap[EditorSaveMapCount]
    __attribute__((section(".data"))) = {
    0x7E, 0x00, 0x01, 0x04, 0x15, 0x38, 0x16, EditorPieceUnavailable,
    0x18, 0x19, EditorPieceUnavailable, EditorPieceUnavailable,
    0x1C, 0x1D, 0x1E, 0x05,
    0x06, 0x07, 0x08, 0x09, 0x0A, 0x7F, 0x0B, 0x0C,
    0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x1F, 0x20,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2A, 0x2B, 0x13, 0x14, 0x2C, 0x36, 0x03,
    0x02, 0x2E, 0x35, 0x33, 0x34, 0x31, 0x83, 0x8F,
    0x86, 0x8B, 0x8C, 0x87, 0x82, 0x89, 0x85, 0x81,
    0x84, 0x90, 0x88, 0x80, 0x8A, 0x8D, 0x8E, 0x3B,
    0x3C, 0x37, 0x3F, 0x3E, 0x3D, 0x49, 0x43, 0x42,
    0x4A, 0x45, 0x44, 0x46, 0x4B, 0x48, 0x47, 0x4E,
    0x4C, 0x4D, 0x6C, 0x4F, 0x51, 0x50, 0x52, 0x58,
    0x57, 0x54, 0x55, 0x56, 0x53, 0x5B, 0x5A, 0x59,
    0x5C, 0x5E, 0x5D, 0x5F, 0x40, 0x41, 0x68, 0x69,
    0x6A, 0x6B, 0x6D, 0x73, 0x75, 0x76, 0x74, 0x6E,
    0x6F, 0x70, 0x77, 0x71, 0x78, 0x72, 0x7A, 0x79,
    0x7B, 0x60, 0x61, 0x64, 0x63, 0x62, 0x67, 0x66,
    0x65, 0x7C, 0x7D, 0x91, 0x92, 0x93, 0x94, 0x95,
    EditorSaveMapEnd,
};

/** Runtime-selected floor-piece identifiers for the active theme. */
int Floor1X1Piece[EditorFloorPieceCount]
    __attribute__((section(".data"))) = {0};
int Floor1X2Piece[EditorFloorPieceCount]
    __attribute__((section(".data"))) = {0};
int Floor2X2Piece[EditorFloorPieceCount]
    __attribute__((section(".data"))) = {0};

/** Runtime-selected riser and unavailable-piece IDs. */
int RiserPiece __attribute__((section(".sdata"))) = 0;
int NotInDemoPiece __attribute__((section(".sdata"))) = 0;

#undef EDITOR_PIECE

/**
 * Selects the active theme and rebuilds its special-piece mappings.
 *
 * @param theme Theme identifier.
 */
void EditorPieces_SetTheme(int theme) {
    Theme = theme;

    Floor1X1Piece[0] = MapBasePieceToTheme(0x17, theme);
    Floor1X1Piece[1] = MapBasePieceToTheme(0x2D, theme);
    Floor1X1Piece[2] = MapBasePieceToTheme(0x2D, theme);
    Floor1X1Piece[3] = MapBasePieceToTheme(0x2D, theme);
    Floor1X1Piece[4] = MapBasePieceToTheme(0x2D, theme);

    Floor1X2Piece[0] = MapBasePieceToTheme(0x1A, theme);
    Floor1X2Piece[1] = MapBasePieceToTheme(0x3A, theme);
    Floor1X2Piece[2] = MapBasePieceToTheme(0x3A, theme);
    Floor1X2Piece[3] = MapBasePieceToTheme(0x3A, theme);
    Floor1X2Piece[4] = MapBasePieceToTheme(0x3A, theme);

    Floor2X2Piece[0] = MapBasePieceToTheme(0x1B, theme);
    Floor2X2Piece[1] = MapBasePieceToTheme(0x39, theme);
    Floor2X2Piece[2] = MapBasePieceToTheme(0x39, theme);
    Floor2X2Piece[3] = MapBasePieceToTheme(0x39, theme);
    Floor2X2Piece[4] = MapBasePieceToTheme(0x39, theme);

    RiserPiece = MapBasePieceToTheme(0x2C, theme);
    NotInDemoPiece = MapBasePieceToTheme(0x7C, theme);

    int i = 0;
    while (CategoryMap[i].firstPiece != EditorPieceUnavailable) {
        CategoryMap[i].lastUsedPiece = CategoryMap[i].firstPiece;
        i++;
    }
}


/**
 * Finds a theme-specific piece in PieceMap.
 *
 * @param piece Piece identifier to find.
 * @param theme Theme map to search.
 * @return PieceMap index, or -1 at the unavailable-piece sentinel.
 */
int MapPieceToIndex(int piece, int theme) {
    int i = 0;
    while (PieceMap[i].map[theme] != piece) {
        if (PieceMap[i].map[theme] == EditorPieceUnavailable) {
            return -1;
        }
        i++;
    }
    return i;
}


/**
 * Maps a theme-zero piece identifier into the requested theme.
 *
 * @param piece Theme-zero piece identifier.
 * @param theme Destination theme.
 * @return Corresponding piece identifier in the destination theme.
 */
int MapBasePieceToTheme(int piece, int theme) {
    int index = MapPieceToIndex(piece, 0);
    return PieceMap[index].map[theme];
}


/**
 * Reports whether an identifier is one of the configured floor pieces.
 *
 * @param piece Piece identifier to test.
 * @return 1 for a floor piece, otherwise 0.
 */
int IsAFloorPiece(int piece) {
    int i = 0;
    do {
        if (Floor1X1Piece[i] == piece) goto is_floor;
        if (Floor1X2Piece[i] != piece) goto check_large_floor;
    is_floor:
        return 1;
    check_large_floor:
        if (Floor2X2Piece[i] == piece) goto is_floor;
        i++;
    } while (i < EditorFloorPieceCount);
    return 0;
}


/**
 * Converts a runtime piece identifier to its stable save-table index.
 *
 * @param piece Piece identifier in the active theme.
 * @return Matching save-table index, or EditorSaveMapEnd when absent.
 */
int EditorPieces_TranslatePieceOut(int piece) {
    int index = MapPieceToIndex(piece, Theme);
    int theme0_piece = PieceMap[index].map[0];
    int i;
    for (i = 0; SaveMap[i] != EditorSaveMapEnd; i++) {
        if (SaveMap[i] == theme0_piece) {
            return i;
        }
    }
    // The target returns the loaded end sentinel; an explicit return reshapes
    // this GCC version's otherwise exact loop and epilogue.
}


/**
 * Converts a stable save-table index into the active theme's piece.
 *
 * @param save_index Index in SaveMap.
 * @return Active-theme piece identifier, or -1 for an unavailable slot.
 */
int EditorPieces_TranslatePieceIn(int save_index) {
    int theme0_piece = SaveMap[save_index];
    if (theme0_piece == EditorPieceUnavailable) return -1;
    int index = MapPieceToIndex(theme0_piece, 0);
    return PieceMap[index].map[Theme];
}


/**
 * Returns the flags attached to a runtime piece identifier.
 *
 * @param piece_number Piece identifier in the active theme.
 * @return Piece flags, or 3 when the identifier is absent.
 */
unsigned char EditorPieces_GetPieceFlags(int piece_number) {
    int index = MapPieceToIndex(piece_number, Theme);
    if (index == -1) {
        printf("couldn't find piece %d in EditorPieces.cpp\n", piece_number);
        return 3;
    }
    return PieceMap[index].flags;
}


/**
 * Returns one rotated edge link for a pool piece.
 *
 * @param piece Piece identifier in the active theme.
 * @param side Side whose encoded link is requested.
 * @param rot Piece rotation.
 * @return Encoded link, or zero when the piece has no pool-link entry.
 */
int GetPoolLink(int piece, int side, int rot) {
    int index = MapPieceToIndex(piece, Theme);
    int theme0_piece = PieceMap[index].map[0];
    int link_index = 0;

    while (PoolLinkMap[link_index].piece != theme0_piece) {
        if (PoolLinkMap[link_index].piece == EditorPieceUnavailable) {
            return 0;
        }
        link_index++;
    }

    int the_side = rot - EditorPoolSideCount;
    the_side = side - the_side;
    the_side &= EditorPoolSideMask;
    return PoolLinkMap[link_index].link[the_side];
}


/**
 * Reports whether two encoded pool-edge links are compatible.
 *
 * @param link1 First encoded edge link.
 * @param link2 Second encoded edge link.
 * @return 1 when the links are compatible, otherwise 0.
 */
int ComparePoolLinks(int link1, int link2) {
    if (link1 == 0 || link2 == 0) return 1;
    if ((link1 >> EditorLinkNibbleBits) !=
        (link2 & EditorLinkNibbleMask)) return 0;
    return (link2 >> EditorLinkNibbleBits) ==
           (link1 & EditorLinkNibbleMask);
}
