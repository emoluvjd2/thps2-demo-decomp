// SQRTTAB.cpp -- Reconstructed data-only translation unit
// Original: sqrttab.obj

enum
{
    SQRT_TABLE_ENTRY_COUNT = 3072,
};

/**
 * Ten-bit fixed-point square-root samples for inputs from 1024.5 through
 * 4095.5.  The checked-in initializer is reproducibly generated from the
 * integer formula documented in tools/generate_sqrttab.py.
 */
unsigned short SqrtTable[SQRT_TABLE_ENTRY_COUNT]
    __attribute__((section(".data"))) = {
#include "SQRTTAB.inc"
};
