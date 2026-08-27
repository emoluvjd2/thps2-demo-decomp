#ifndef FACEREMOVAL_INC_H
#define FACEREMOVAL_INC_H

#define GLOBALS_SCENE_VALUES_ONLY
#include "globals.h"
#undef GLOBALS_SCENE_VALUES_ONLY

/** Circular point used by the face-clipping polygon routines. */
struct FP {
    Vector pt;
    unsigned char in;
    FP *next;
    FP *next2;
};

typedef char FP_size_must_be_24[(sizeof(FP) == 24) ? 1 : -1];
typedef char FP_pt_offset_must_be_0[((unsigned long)&(((FP *)0)->pt) == 0) ? 1 : -1];
typedef char FP_in_offset_must_be_12[((unsigned long)&(((FP *)0)->in) == 12) ? 1 : -1];
typedef char FP_next_offset_must_be_16[((unsigned long)&(((FP *)0)->next) == 16) ? 1 : -1];
typedef char FP_next2_offset_must_be_20[((unsigned long)&(((FP *)0)->next2) == 20) ? 1 : -1];

/** Allocates the fixed face-removal point pool. */
void FaceRemoval_Setup(void);

/** Releases the fixed face-removal point pool. */
void FaceRemoval_Cleanup(void);

/** Returns the next point from the fixed face-removal pool. */
FP *NewFP(void);

/** Resets the face-removal pool cursor for another polygon pass. */
void ResetFPList(void);

/** Returns the absolute signed area of a circular point list. */
int GetArea(FP *list);

/** Returns the area shared by two coplanar circular point lists. */
int GetEmbeddedArea(FP *points1, FP *points2);

#ifdef FACEREMOVAL_OWNER_DECLARATIONS

enum FaceRemovalConstant {
    FaceRemovalPoolCapacity = 40
};

// Face-point pool state (gp-relative in this owner).
extern "C" FP *CurrentFP;  // 0x800DA57C
extern "C" FP *FPBuf;      // 0x800DA578
extern "C" int NumUsedFps; // 0x800DA580

#endif // FACEREMOVAL_OWNER_DECLARATIONS

#endif // FACEREMOVAL_INC_H
