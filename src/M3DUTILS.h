// Public model-animation utility interface.
#ifndef M3DUTILS_INC_H
#define M3DUTILS_INC_H

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

// Forward declarations for the model-animation interface.
struct CVector;
struct CSuper;
struct SHook;

#ifdef __cplusplus
}

/** Read a region's model-hook packet. */
void M3dUtils_ReadHooksPacket(CSuper *pSuper, void *pPacket);
/** Resolve a numbered model hook into world space. */
void M3dUtils_GetHookPosition(VECTOR *pResult, CSuper *pSuper, int Hook);
/** Resolve an explicit model hook into world space. */
void M3dUtils_GetDynamicHookPosition(
    VECTOR *pResult, CSuper *pSuper, SHook *pHook);
/** Build the current interpolated model pose. */
void M3dUtils_InBetween(CSuper *pSuper);
/** Convert a rotation matrix to Euler angles. */
void M3dUtils_AnglesFromMatrix(SVECTOR *pAngles, MATRIX *pMatrix);
/** Find the closest points on two fixed-point line segments. */
int M3dUtils_LineLineClosestPoints(CVector *L1Start, CVector *L1End,
    CVector *L2Start, CVector *L2End, CVector *PointL1, int *t1,
    CVector *PointL2, int *t2);
/** Normalize a fixed-point vector in place. */
void M3dUtils_NormaliseVector(CVector *pV);
#endif

#endif // M3DUTILS_INC_H
