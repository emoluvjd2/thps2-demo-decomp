#ifndef ZEROVECTOR_INC_H
#define ZEROVECTOR_INC_H

struct CVector;

/**
 * Shared zero vector physically owned by OB.cpp.
 * Full-vector consumers form the target-recorded absolute address.
 */
#ifdef ZEROVECTOR_FIRST_COMPONENT_DECLARATIONS
/** Scalar address views used by OB's zero-initialization owner tail. */
extern int ObZeroVectorFirstComponent asm("ZeroVector");
extern short ObZeroSVectorFirstComponent asm("ZeroSVector");
#elif defined(ZEROVECTOR_EIGHT_BYTE_DECLARATION)
/** Eight-byte address view used to keep ZeroVector at the -G8 boundary. */
struct ZeroVectorDecl {
    int lo;
    int hi;
};
extern ZeroVectorDecl ZeroVector __attribute__((section(".data")));
#elif defined(ZEROVECTOR_SCALAR_DECLARATION)
extern int ZeroVector __attribute__((section(".data")));
#else
extern "C" CVector ZeroVector __attribute__((section(".data")));
#endif

#endif // ZEROVECTOR_INC_H
