// Declarations shared by the BIT2 particle/effect owner.
#ifndef BIT2_INC_H
#define BIT2_INC_H

#include "BIT.h"

#ifdef BIT2_OWNER_DECLARATIONS
#define BITSTATE_DECLARE_TOTAL_BIT_USAGE
#include "BITSTATE.h"
#endif

struct CBody;
struct CGrindSpark;
struct CVector;

/** Target-proven storage size of one grind spark. */
enum { BIT2_GRIND_SPARK_STORAGE_SIZE = 120 };

/** Construct a grind spark in storage returned by the CBit allocator. */
extern "C" CGrindSpark *Bit2_ConstructGrindSpark(
    CGrindSpark *storage, CBody *parent, CVector &position,
    int velocity, int gravity, int lifetime)
    asm("__11CGrindSparkP5CBodyR7CVectoriii");

#endif // BIT2_INC_H
