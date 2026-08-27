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

typedef char RailInfo_size_must_be_12[
    (sizeof(RailInfo) == 12) ? 1 : -1];

#endif // THPS2_RAILINFO_H
