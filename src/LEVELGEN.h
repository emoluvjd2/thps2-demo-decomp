// Hand-maintained per-TU interface for LEVELGEN.cpp.
// tools/gen_tu_headers.py is retained for bootstrapping and does not reproduce it.
#if defined(LEVELGEN_GRID_MAP_LAYOUT_ONLY) && \
    !defined(LEVELGEN_GRID_MAP_LAYOUT)
#error "LEVELGEN grid/map layout-only mode requires LEVELGEN_GRID_MAP_LAYOUT"
#endif

#if defined(LEVELGEN_GRID_MAP_LAYOUT_ONLY) && \
    defined(LEVELGEN_OWNER_DECLARATIONS)
#error "LEVELGEN owner declarations cannot be requested in layout-only mode"
#endif

#ifndef LEVELGEN_GRID_MAP_LAYOUT_ONLY
#ifndef LEVELGEN_INC_H
#define LEVELGEN_INC_H

#ifdef LEVELGEN_OWNER_DECLARATIONS
#define LEVELGEN_GRID_STATE_DECL_ATTR
#else
#define LEVELGEN_GRID_STATE_DECL_ATTR __attribute__((section(".data")))
#endif

/** Number of cells along each side of the shared editor grid. */
extern "C" int GridSize LEVELGEN_GRID_STATE_DECL_ATTR;

/** Height of one vertical editor-grid slot. */
extern "C" int _14LevelGenerator_GridCellHeight
    asm("_14LevelGenerator.GridCellHeight") LEVELGEN_GRID_STATE_DECL_ATTR;

/** Width and depth of one editor-grid cell. */
extern "C" int _14LevelGenerator_GridCellWidth
    asm("_14LevelGenerator.GridCellWidth") LEVELGEN_GRID_STATE_DECL_ATTR;

#undef LEVELGEN_GRID_STATE_DECL_ATTR

#endif // LEVELGEN_INC_H
#endif // LEVELGEN_GRID_MAP_LAYOUT_ONLY

#ifdef LEVELGEN_DECLARE_OUTPUT_NAME_ADDRESS_VIEW
/** Absolute address view of LEVELGEN's GP-sized output-region name. */
extern signed char DAT_800da664[] __attribute__((aligned(4)));
#undef LEVELGEN_DECLARE_OUTPUT_NAME_ADDRESS_VIEW
#endif

#if defined(LEVELGEN_OWNER_DECLARATIONS) && \
    !defined(LEVELGEN_OWNER_DECLARATIONS_HEAD_EMITTED) && \
    (defined(LEVELGEN_GRID_CELL_LAYOUT_EMITTED) || \
     defined(LEVELGEN_MAP_CELL_LAYOUT_EMITTED))
#error "LEVELGEN owner declarations must precede a focused grid/map layout encounter"
#endif

#if defined(LEVELGEN_OWNER_DECLARATIONS) && \
    !defined(LEVELGEN_OWNER_DECLARATIONS_HEAD_EMITTED)
#define LEVELGEN_OWNER_DECLARATIONS_HEAD_EMITTED
#define LEVELGEN_OWNER_DECLARATIONS_ACTIVE

#include "globals.h"

#ifdef __cplusplus
extern "C"
{
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

    #include "PSXREGION.h"

    // Forward declarations for large/opaque types
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
    struct Item;
    struct RailInfo;

    // Additional shared forward declarations
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

#endif // LEVELGEN_OWNER_DECLARATIONS_HEAD_EMITTED

#if defined(LEVELGEN_OWNER_DECLARATIONS_ACTIVE) && \
    !defined(LEVELGEN_GRID_MAP_LAYOUT)
#define LEVELGEN_GRID_MAP_LAYOUT
#define LEVELGEN_GRID_MAP_LAYOUT_WAS_IMPLIED
#endif

#if defined(LEVELGEN_GRID_MAP_LAYOUT) && \
    !defined(LEVELGEN_GRID_CELL_LAYOUT_EMITTED)
#define LEVELGEN_GRID_CELL_LAYOUT_EMITTED

/** One five-slot cell in the editor's compact piece grid. */
struct GridCell {
    short modelIndex[5];
    unsigned char rot[5];
    unsigned char numUsedSlots;
};

/** Shared 24-by-24 editor model-placement grid owned by LEVELGEN.cpp. */
extern "C" GridCell _14LevelGenerator_GridTab[24][24]
    asm("_14LevelGenerator.GridTab")
    __attribute__((section(".data")));

#endif // LEVELGEN_GRID_CELL_LAYOUT_EMITTED

#if defined(LEVELGEN_OWNER_DECLARATIONS_ACTIVE) && \
    !defined(LEVELGEN_ZONE_CELL_LAYOUT_EMITTED)
#define LEVELGEN_ZONE_CELL_LAYOUT_EMITTED

/** One generated-level zone and its indexed item list. */
struct ZoneCell {
    unsigned char num;
    unsigned short item[48];
};

#endif // LEVELGEN_ZONE_CELL_LAYOUT_EMITTED

#if defined(LEVELGEN_GRID_MAP_LAYOUT) && \
    !defined(LEVELGEN_MAP_CELL_LAYOUT_EMITTED)
#define LEVELGEN_MAP_CELL_LAYOUT_EMITTED

/** One five-slot cell in the generated model occupancy map. */
struct MapCell {
    struct EditorOutModel (*modelInSlot[5]);
};

#endif // LEVELGEN_MAP_CELL_LAYOUT_EMITTED

#ifdef LEVELGEN_GRID_MAP_LAYOUT_WAS_IMPLIED
#undef LEVELGEN_GRID_MAP_LAYOUT
#undef LEVELGEN_GRID_MAP_LAYOUT_WAS_IMPLIED
#endif

#if defined(LEVELGEN_OWNER_DECLARATIONS_ACTIVE) && \
    !defined(LEVELGEN_OWNER_DECLARATIONS_TAIL_EMITTED)
#define LEVELGEN_OWNER_DECLARATIONS_TAIL_EMITTED

#ifdef __cplusplus
}
#endif

// === Variables (absolute in this TU) ===
#define ENVREGIONS_ABSOLUTE_DECLARATION
#include "ENVREGIONS.h"
#define ENVIROLIST_ABSOLUTE_DECLARATION
#include "ENVIROLIST.h"
#define M3DCOLOURSTATE_DECLARE_FADE_COLOUR
#include "M3DCOLOURSTATE.h"
#define TRIGSTATE_ABSOLUTE_DECLARATION
#define TRIGSTATE_DECLARE_NUM_NODES
#include "TRIGSTATE.h"
#define TRIGSTATE_ABSOLUTE_DECLARATION
#define TRIGSTATE_DECLARE_OFFSET_LIST
#include "TRIGSTATE.h"
#define TOTALTEXTUREUSAGE_ABSOLUTE_DECLARATION
#include "TOTALTEXTUREUSAGE.h"
#define TRANSPARENTPALFOREDITOR_ABSOLUTE_DECLARATION
#include "TRANSPARENTPALFOREDITOR.h"
#define TRIGSTATE_ABSOLUTE_DECLARATION
#define TRIGSTATE_DECLARE_TRIG_FILE
#include "TRIGSTATE.h"
#define M3DCOLOURSTATE_DECLARE_COLOUR_TABLE
#include "M3DCOLOURSTATE.h"

#endif // LEVELGEN_OWNER_DECLARATIONS_TAIL_EMITTED

#ifdef LEVELGEN_OWNER_DECLARATIONS_ACTIVE
#undef LEVELGEN_OWNER_DECLARATIONS_ACTIVE
#endif

/*
 * Re-entrant destructor ABI selector.  The owner retains its local receiver
 * and source name while sharing LevelGenerator's single target ABI label.
 */
#if defined(LEVELGEN_DECLARE_DESTRUCTOR_ABI)
#if !defined(LEVELGEN_DESTRUCTOR_NAME) || \
    !defined(LEVELGEN_DESTRUCTOR_RECEIVER)
#error "Select the LevelGenerator destructor source name and receiver type"
#endif

/** Addressable destructor entry used by LEVELGEN's owner-local vtable. */
void LEVELGEN_DESTRUCTOR_NAME(
    LEVELGEN_DESTRUCTOR_RECEIVER *generator, int inCharge)
    asm("_._14LevelGenerator");

#undef LEVELGEN_DESTRUCTOR_RECEIVER
#undef LEVELGEN_DESTRUCTOR_NAME
#undef LEVELGEN_DECLARE_DESTRUCTOR_ABI
#endif
