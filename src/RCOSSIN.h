#ifndef RCOSSIN_INC_H
#define RCOSSIN_INC_H

/**
 * Packed sine/cosine lookup table supplied by Psy-Q LIBGTE.
 *
 * Most users consume the canonical halfword array.  A few target-proven
 * compiler views opt into a scalar type because GCC 2.x uses that declarator
 * shape when allocating registers around word-indexed table accesses.
 */
#ifndef RCOSSIN_DECL_TYPE
#define RCOSSIN_DECL_TYPE short
#endif

#ifdef RCOSSIN_SCALAR_VIEW
#define RCOSSIN_DECL_SUFFIX
#define RCOSSIN_DECL_ATTR __attribute__((section(".data")))
#else
#define RCOSSIN_DECL_SUFFIX []
#define RCOSSIN_DECL_ATTR
#endif

extern "C" RCOSSIN_DECL_TYPE rcossin_tbl RCOSSIN_DECL_SUFFIX
    RCOSSIN_DECL_ATTR;

#undef RCOSSIN_DECL_ATTR
#undef RCOSSIN_DECL_SUFFIX
#undef RCOSSIN_SCALAR_VIEW
#undef RCOSSIN_DECL_TYPE

#endif // RCOSSIN_INC_H
