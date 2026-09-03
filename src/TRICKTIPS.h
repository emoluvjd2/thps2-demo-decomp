// Public trick-tip descriptors and monitor layout.
#ifndef TRICKTIPS_INC_H
#define TRICKTIPS_INC_H

#include "globals.h"

struct CBody;
struct SVideoInfo;

/** Scheduling class used by each recovered trick-tip descriptor. */
enum ETipOccurance {
    START_TIP = 0,
    BAIL_TIP = 1,
    ANYTIME_TIP = 2,
};

struct CTrickTipText {
    ETipOccurance mOccurance;
    unsigned char mLineCount;
    char *mLine1;
    char *mLine2;
    char *mLine3;
};

/** Persistent counters and display state for the trick-tip system. */
struct CTrickTipMonitor {
    struct CClass CClass;
    int mShowAsNeededQueue[6];
    int mShowAsNeededIndex;
    int mShowAfterBailQueue[6];
    int mShowAfterBailIndex;
    int mShowAtStartQueue[6];
    int mShowAtStartIndex;
    unsigned int mTimer;
    unsigned int mLastTipTime;
    unsigned int mJumpTimer;
    struct CBruce *pSkater;
    char mTipLine1Buf[40];
    char *mTipLine1;
    char *mTipLine2;
    char *mTipLine3;
    char *mTipLine4;
    unsigned int mTipTimer;
    int mOldBails;
    int mOldPerpendicularBails;
    int mOldTrickBails;
    int mOldRailBails;
    int mOldJumps;
    int mOldWeakJumps;
    int mOldAirs;
    int mOldTricks;
    int mOldRails;
    int mOldAirPolys;
    int mOldAirPolyBreaks;
    int mOldBigAirs;
    int mOldGrinds;
    int mBailCounter;
    int mPerpendicularBailCounter;
    int mTrickBailCounter;
    int mRailBailCounter;
    int mJumpCounter;
    int mWeakJumpCounter;
    int mAirCounter;
    int mTrickCounter;
    int mRailCounter;
    int mAirPolyCounter;
    int mAirPolyBreakCounter;
    int mBigAirCounter;
    int mGrindCounter;
    int mHoldingForwardCounter;
    void *mOkayToShow;
};

/** Initialize trick-tip state for a newly created skater. */
void TrickTips_Init(void);
/** Advance the active trick-tip monitor. */
void TrickTips_Update(void);
/** Draw the current trick-tip text. */
void TrickTips_Display(void);

// === Variables (absolute in this TU) ===
#define DEMOTIMER_ABSOLUTE_DECLARATION
#include "DEMOTIMER.h"
#include "GLEVEL.h"
#define VIDEOINFO_DECLARE_GVINFO1
#define VIDEOINFO_DECLARE_GVINFO2
#include "VIDEOINFO.h"
#endif // TRICKTIPS_INC_H

#ifdef TRICKTIPS_DECLARE_MONITOR_VTABLE_ADDRESSES
/** Scalar address view of the monitor's destruction-phase vtable. */
extern int __vt_16CTrickTipMonitor;
/** Scalar address view of the monitor's complete-object vtable. */
extern int __vt_16CTrickTipMonitor_complete;
#undef TRICKTIPS_DECLARE_MONITOR_VTABLE_ADDRESSES
#endif
