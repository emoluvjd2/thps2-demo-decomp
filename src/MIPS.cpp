// MIPS.cpp -- semantic C++ reference for the original MIPS.S application module.
// Original: MIPS.S
// Address range: 0x8006B024 - 0x8006B1E4
// The canonical link object is assembled from the independently reconstructed
// target instructions in output/asm/Mips_*.s.

#include "globals.h"
#include "MIPS.h"

typedef signed long long MipsSignedWide;
typedef unsigned long long MipsUnsignedWide;

/**
 * Little-endian access to the two words of a 64-bit intermediate.
 */
union MipsWideValue {
    MipsUnsignedWide whole;
    struct {
        unsigned int low;
        unsigned int high;
    } word;
};

typedef char SizesFileCommonRecordSize[
    sizeof(SizesFileCommonRecord) == 36 ? 1 : -1];
typedef char SizesFileSkateRecordSize[
    sizeof(SizesFileSkateRecord) == 32 ? 1 : -1];
typedef char SizesFileSchoolRecordSize[
    sizeof(SizesFileSchoolRecord) == 40 ? 1 : -1];
typedef char SizesFileVansRecordSize[
    sizeof(SizesFileVansRecord) == 36 ? 1 : -1];
typedef char SizesFileTableSize[sizeof(SizesFileTable) == 148 ? 1 : -1];

/**
 * File-size metadata used by the front end.
 *
 * The original hand-written module deliberately stored this read-only table
 * before its routines in the text section.
 */
extern "C" const SizesFileTable SizesFile
    __attribute__((section(".text"))) = {
        {
            "common",
            {0x800, 0x800, 0x71800, 0x7000, 0xD000, 0, 0xFFFFFFFF}
        },
        {
            "skate_t",
            {0x7E800, 0x7000, 0xD000, 0x32000, 0x1800, 0xFFFFFFFF}
        },
        {
            "skschl_t",
            {0x7E800, 0x7000, 0xD000, 0x52800, 0x5800, 0x76000,
             0xFFFFFFFF}
        },
        {
            "skvans_t",
            {0x7E800, 0x7000, 0xD000, 0, 0x33000, 0xFFFFFFFF}
        },
        0
    };

/**
 * Returns the sign of the modulo-2^64 sum of two signed 32-bit products.
 *
 * @return -1 for a negative sum, 0 for zero, or 1 for a positive sum.
 */
extern "C" int Mips_DotProd2Sign(int a, int b, int c, int d)
{
    MipsWideValue value;
    int sign = 1;

    value.whole = (MipsUnsignedWide)((MipsSignedWide)a * b)
                + (MipsUnsignedWide)((MipsSignedWide)c * d);
    if (!(value.word.low | value.word.high))
    {
        sign = 0;
    }
    else if (value.word.high & 0x80000000U)
    {
        sign = -1;
    }
    return sign;
}

/**
 * Multiplies two signed fixed-point values and returns bits 12 through 43 of
 * the full 64-bit product.
 */
int Mips_FixMul(int a, int b)
{
    MipsWideValue value;

    value.whole = (MipsUnsignedWide)((MipsSignedWide)a * b);
    unsigned int low = value.word.low;
    unsigned int high = value.word.high;
    low >>= 12;
    high <<= 20;
    return high | low;
}

/**
 * Adds two signed 32-bit product bit patterns modulo 2^64 and returns bits 12
 * through 43 of the resulting sum.
 */
extern "C" int Mips_DotProd2(int a, int b, int c, int d)
{
    MipsWideValue value;

    value.whole = (MipsUnsignedWide)((MipsSignedWide)a * b)
                + (MipsUnsignedWide)((MipsSignedWide)c * d);
    unsigned int low = value.word.low;
    unsigned int high = value.word.high;
    low >>= 12;
    high <<= 20;
    return high | low;
}

/**
 * Computes the fixed-point dot product with signed products and modulo-2^64
 * accumulation, matching the original low/high-word arithmetic.
 */
int Mips_CVecDotProd(CVector *a, CVector *b)
{
    MipsWideValue value;

    value.whole = (MipsUnsignedWide)((MipsSignedWide)a->vx * b->vx)
                + (MipsUnsignedWide)((MipsSignedWide)a->vy * b->vy)
                + (MipsUnsignedWide)((MipsSignedWide)a->vz * b->vz);
    unsigned int high = value.word.high << 20;
    unsigned int low = value.word.low >> 12;
    high |= low;
    return high;
}

/**
 * Original module-local saved global-pointer word.
 */
static unsigned int StoredGP __attribute__((section(".rdata"))) = 0;
