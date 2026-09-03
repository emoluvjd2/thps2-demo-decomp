#if defined(M3DMATHS_DECLARE_VECTOR_SUBTRACT)
#if !defined(M3DMATHS_VECTOR_SUBTRACT_DECLARATION)
#error M3DMATHS_DECLARE_VECTOR_SUBTRACT requires a full declaration
#endif
/** Subtract two vectors through GCC's lowered aggregate-return ABI. */
M3DMATHS_VECTOR_SUBTRACT_DECLARATION
    asm("__mi__FRC7CVectorT0");
#undef M3DMATHS_VECTOR_SUBTRACT_DECLARATION
#undef M3DMATHS_DECLARE_VECTOR_SUBTRACT
#endif

#ifndef M3DMATHS_ABI_ONLY
#ifndef M3DMATHS_H
#define M3DMATHS_H

/** Returns the integer square root of a fixed-point magnitude. */
int M3dMaths_SquareRoot0(int value);

/** Multiply two integers and divide the 64-bit product by a third. */
int M3dMaths_MulDiv64(int value, int multiplier, int divisor);

/** Copy the rotation words from one matrix to another. */
void M3dMaths_CopyMat(unsigned short *source, unsigned short *destination);

/** Transpose the rotation words from one matrix into another. */
void M3dMaths_TransposeMat(
    unsigned short *source, unsigned short *destination);

/** Transform the vector currently loaded in the GTE by a rotation matrix. */
void M3dMaths_TransformLastVec(unsigned short *matrix);

#endif // M3DMATHS_H

#if defined(M3DMATHS_TYPED_API) && \
    !defined(M3DMATHS_TYPED_API_DECLARATIONS)

#ifndef _LIBGTE_H_
#error "M3DMATHS_TYPED_API requires <LIBGTE.H> first"
#endif

#define M3DMATHS_TYPED_API_DECLARATIONS

struct Item;

/** Initialize a GTE matrix rotation to the identity transform. */
void M3dMaths_SetIdentityRotation(MATRIX *matrix);

/** Build a Y-X-Z rotation matrix from packed Euler angles. */
void M3dMaths_RotMatrixYXZ(SVECTOR *angles, MATRIX *matrix);

/** Apply an item's fixed-point scale to a rotation matrix. */
void M3dMaths_ScaleMatrix(Item *item, MATRIX *matrix);

#endif // M3DMATHS_TYPED_API && !M3DMATHS_TYPED_API_DECLARATIONS
#endif // !M3DMATHS_ABI_ONLY
