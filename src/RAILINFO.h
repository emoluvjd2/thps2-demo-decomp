/*
 * Re-entrant destructor ABI selector. Owner translation units retain their
 * target-proven receiver views while sharing RailInfo's single ABI label.
 */
#if defined(RAILINFO_DECLARE_DESTRUCTOR)
#if !defined(RAILINFO_DESTRUCTOR_NAME) || \
    !defined(RAILINFO_DESTRUCTOR_RECEIVER)
#error RAILINFO_DECLARE_DESTRUCTOR requires a name and receiver type
#endif
void RAILINFO_DESTRUCTOR_NAME(
    RAILINFO_DESTRUCTOR_RECEIVER *rails, int inCharge)
    asm("_._8RailInfo");
#undef RAILINFO_DESTRUCTOR_RECEIVER
#undef RAILINFO_DESTRUCTOR_NAME
#undef RAILINFO_DECLARE_DESTRUCTOR
#endif

#ifndef THPS2_RAILINFO_H
#define THPS2_RAILINFO_H

#include "globals.h"

struct RailString;

/** Collection of generated rail strings and its ordinary service interface. */
struct RailInfo {
    struct CClass CClass;
    struct RailString *StringList;
    struct RailString *CurrentString;

    /** Allocate and append one empty rail string. */
    RailString *NewString();
    /** Destroy every owned rail string and clear the list. */
    void Clear();
    /** Count the strings currently linked into this collection. */
    int CountStrings();
    /** Recount and total all points in the linked strings. */
    int CountPoints();
    /** Merge or append one candidate rail-string list. */
    void MergeAdd(RailString *string, bool debugify);
};

/** Construct an empty generated-rail collection in caller-provided storage. */
namespace RailInfoConstructorABI
{
extern "C" RailInfo *__8RailInfo(RailInfo *rails);
}
#define RailInfo_ctor RailInfoConstructorABI::__8RailInfo

typedef char RailInfo_size_must_be_12[
    (sizeof(RailInfo) == 12) ? 1 : -1];

#endif // THPS2_RAILINFO_H
