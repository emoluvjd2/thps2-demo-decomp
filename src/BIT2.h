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

#ifndef BIT2_OWNER_DECLARATIONS
/** ABI entry for constructing a grind spark in CBit-owned storage. */
extern "C" CGrindSpark *__11CGrindSparkP5CBodyR7CVectoriii(
    CGrindSpark *storage, CBody *parent, CVector &position,
    int velocity, int gravity, int lifetime);

/** Semantic source name for the client-visible constructor entry. */
#define Bit2_ConstructGrindSpark __11CGrindSparkP5CBodyR7CVectoriii
#endif

#endif // BIT2_INC_H

#ifdef BIT2_DECLARE_MOTION_BLUR_ANIMATION_VIEW
/** Absolute-address view of BIT2's owner-local motion-blur animation name. */
extern char DAT_800d98b8[];
#undef BIT2_DECLARE_MOTION_BLUR_ANIMATION_VIEW
#endif
