// Hand-maintained per-TU interface for SHATTER.cpp.
// gen_tu_headers.py is retained only for explicit bootstrap regeneration.
#ifndef SHATTER_INC_H
#define SHATTER_INC_H

struct Item;

/** Split or shatter one face on an environmental item. */
int Shatter_Face(Item *pItem, unsigned int *pFace, int numSplits,
                 int makeFlash, int useFlags, int modifyFlags,
                 int tryToShatter);
/** Shatter an environmental item when its model permits it. */
int Shatter_Item(Item *pItem, int useFlags, int modifyFlags);
/** Play the rate-limited glass-shatter sound when its lockout permits it. */
void Shatter_MaybeMakeGlassShatterSound(void);

#ifdef SHATTER_OWNER_DECLARATIONS

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

#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBGPU.H>

#include "globals.h"

// TSPack — VRAM pack allocator entry

// Palette entry (20 bytes)

// MeshHeader (28 bytes)
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

// SAnimFrame — Animation frame entry (8 bytes)
#ifdef __cplusplus
}
#endif

// === Variables (absolute in this TU) ===
#include "CAMERASELECTION.h"
#define LOWMEMORY_ABSOLUTE_DECLARATION
#include "LOWMEMORY.h"
#define REPLAYSOUNDLOCKOUT_ABSOLUTE_DECLARATION
#include "REPLAYSOUNDLOCKOUT.h"
#define BITSTATE_DECLARE_USE_CACHE_FOR_BITS
#include "BITSTATE.h"

#endif // SHATTER_OWNER_DECLARATIONS

#endif // SHATTER_INC_H

#ifdef SHATTER_DECLARE_CSHATTERBIT_VTABLE_ADDRESS
/** Scalar address view of SHATTER's owner-local CShatterBit vtable. */
extern int __vt_11CShatterBit;
#undef SHATTER_DECLARE_CSHATTERBIT_VTABLE_ADDRESS
#endif
