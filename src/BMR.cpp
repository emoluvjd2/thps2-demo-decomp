// BMR.cpp -- Reconstructed data-only bmr.obj translation unit.
// Original: bmr.obj

#include "BMR.h"

/** Compressed background image retained while its decoded form is built. */
unsigned short *pCompressedBMR
    __attribute__((section(".sdata"))) = 0;

/** Decoded background image uploaded by the display-buffer code. */
char *pBMR
    __attribute__((section(".sdata"))) = 0;
