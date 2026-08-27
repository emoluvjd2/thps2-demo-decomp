#ifndef MIPS_INC_H
#define MIPS_INC_H

struct CVector;

/**
 * One file-size record whose name occupies eight bytes and which has seven
 * size values.
 */
struct SizesFileCommonRecord {
    char name[8];
    unsigned int sizes[7];
};

/**
 * One file-size record whose name occupies eight bytes and which has six
 * size values.
 */
struct SizesFileSkateRecord {
    char name[8];
    unsigned int sizes[6];
};

/**
 * One file-size record whose name occupies twelve bytes and which has seven
 * size values.
 */
struct SizesFileSchoolRecord {
    char name[12];
    unsigned int sizes[7];
};

/**
 * One file-size record whose name occupies twelve bytes and which has six
 * size values.
 */
struct SizesFileVansRecord {
    char name[12];
    unsigned int sizes[6];
};

/**
 * Front-end file-size metadata embedded in the original MIPS text section.
 */
struct SizesFileTable {
    SizesFileCommonRecord common;
    SizesFileSkateRecord skate;
    SizesFileSchoolRecord school;
    SizesFileVansRecord vans;
    unsigned int padding;
};

extern "C" const SizesFileTable SizesFile;

/**
 * Returns the sign of the modulo-2^64 sum of two signed 32-bit products.
 */
extern "C" int Mips_DotProd2Sign(int a, int b, int c, int d);

/**
 * Multiplies two fixed-point values and shifts the 64-bit product right by 12.
 */
int Mips_FixMul(int a, int b);

/**
 * Adds two signed product bit patterns modulo 2^64 and shifts the sum right
 * by 12.
 */
extern "C" int Mips_DotProd2(int a, int b, int c, int d);

/**
 * Computes a fixed-point vector dot product with modulo-2^64 accumulation.
 */
int Mips_CVecDotProd(CVector *a, CVector *b);

#endif // MIPS_INC_H
