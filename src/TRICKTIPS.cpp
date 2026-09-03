// TRICKTIPS.cpp -- Reconstructed translation unit
// Original: TRICKTIPS.CPP
// Functions: 7
// Address range: 0x80073274 - 0x80074008
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#include "TRICKTIPS.h"
#include "MAIN.h"
#include "STATS.h"

/** First-field view needed for the playback-mode guards. */
struct SVideoInfo {
    int VideoMode;
};

/** Proven CBruce counter slice used by the monitor. */
struct TrickTipsSkater {
    char pad_0000[0xec0];
    int mNumGrinds;
    char pad_0ec4[0x120];
    int mBails;
    int mPerpendicularBails;
    int mTrickBails;
    int mRailBails;
    int mJumps;
    int mWeakJumps;
    int mAirs;
    int mTricks;
    int mRails;
    int mAirPolys;
    int mAirPolyBreaks;
    int mBigAirs;
};

enum TrickTipsConstant {
    TrickTipsPlaybackVideoMode = 2,
    TrickTipsDescriptorCount = 26,
    TrickTipsPanelX = 20,
    TrickTipsPanelY = 100,
    TrickTipsSlideFrames = 10,
    TrickTipsSlideDistance = 40,
    TrickTipsTextScale = 220,
    TrickTipsFireScale = 256,
    TrickTipsDisplayFrames = 200,
    TrickTipsHoldingForwardFrames = 150,
    TrickTipsReminderDelay = 600
};

typedef char TrickTips_Skater_prefix_must_be_4116[
    (sizeof(TrickTipsSkater) == 4116) ? 1 : -1];
typedef char TrickTips_Monitor_size_must_be_276[
    (sizeof(CTrickTipMonitor) == 276) ? 1 : -1];

#define CAREER_LAYOUT_DECLARATIONS
#include "CAREER.h"
#undef CAREER_LAYOUT_DECLARATIONS
#include "MESS.h"
#include "PAD.h"

/** View the counter fields proved for the current gameplay skater. */
static inline TrickTipsSkater *TrickTips_AsSkater(CBruce *skater)
{
    return reinterpret_cast<TrickTipsSkater *>(skater);
}

// === Vtable forward declarations ===
#define TRICKTIPS_DECLARE_MONITOR_VTABLE_ADDRESSES
#include "TRICKTIPS.h"
#include "MECHLIST.h"

/** Deferred owner-local destructor bodies referenced by the two vtables. */
extern "C" {
static inline void __DOT__16CTrickTipMonitor(
    CTrickTipMonitor *self, int __in_chrg);
static inline void __DOT__6CClass_80073FD8(
    CClass *self, int __in_chrg);
}

/** Restore the private base vtable and optionally release the object. */
inline void __DOT__6CClass_80073FD8(CClass *self, int __in_chrg)
{
    self->vtable = &__vt_16CTrickTipMonitor;
    if (__in_chrg & 1)
        CClass::operator delete(self);
}

/** Destroy the complete monitor through its recovered owner-local vtable. */
inline void __DOT__16CTrickTipMonitor(
    CTrickTipMonitor *self, int __in_chrg)
{
    self->CClass.vtable = &__vt_16CTrickTipMonitor;
    if (__in_chrg & 1)
        CClass::operator delete(self);
}

/**
 * Long trick-tip lines emitted in read-only data by the original owner.
 *
 * The fixed field widths preserve the compiler's four-byte string alignment
 * while retaining meaningful names for every line used by TRICKTIP.
 */
struct TrickTipLongTextBlock
{
    char DontHoldForward[0x14];
    char LandStraightTo[0x14];
    char FinishYourTricks[0x14];
    char ReleaseXToJump[0x1c];
    char HoldJumpLonger[0x14];
    char TheTopOfRamps[0x14];
    char GetOverPoolLip[0x14];
    char RightToBalance[0x14];
    char PressJumpWhile[0x14];
    char GrindingToOllieOut[0x18];
    char SpinVertTricks[0x14];
};

typedef char TrickTipLongTextBlock_size_must_be_0xE8[
    (sizeof(TrickTipLongTextBlock) == 0xe8) ? 1 : -1];

static TrickTipLongTextBlock TrickTipLongText
    __attribute__((section(".rdata"), aligned(4))) = {
    "DONT HOLD FORWARD",
    "LAND STRAIGHT TO",
    "FINISH YOUR TRICKS",
    "RELEASE X TO JUMP (OLLIE)",
    "HOLD JUMP LONGER",
    "THE TOP OF RAMPS",
    "GET OVER POOL LIP",
    "RIGHT TO BALANCE",
    "PRESS JUMP WHILE",
    "GRINDING TO OLLIE OUT",
    "SPIN VERT TRICKS",
};

/** Labels prepended to trick-tip and reminder messages. */
static char TrickTipLabel[] __attribute__((section(".sdata"), aligned(4))) =
    "TIP: ";
char *TEXT_TIP __attribute__((section(".sdata"))) = TrickTipLabel;
static char TrickTipRememberLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "REMEMBER: ";
char *TEXT_REMEMBER __attribute__((section(".sdata"))) =
    TrickTipRememberLabel;

/**
 * Complete-object vtable installed by the static monitor constructor.
 */
int __vt_16CTrickTipMonitor_complete
    __attribute__((section(".sdata"), aligned(8))) = 0;
struct TrickTipMonitorCompleteVtableTailLayout
{
    void *reserved;
    void (*destroy)(CTrickTipMonitor *, int);
    void *unused;
};
static TrickTipMonitorCompleteVtableTailLayout
    TrickTipMonitorCompleteVtableTail
    __attribute__((section(".sdata"), aligned(4))) = {
    0,
    &__DOT__16CTrickTipMonitor,
    0,
};

/**
 * Destruction-phase vtable installed before the CClass base destructor.
 */
int __vt_16CTrickTipMonitor
    __attribute__((section(".sdata"), aligned(4))) = 0;
struct TrickTipMonitorVtableTailLayout
{
    void *reserved;
    void (*destroyBase)(CClass *, int);
};
static TrickTipMonitorVtableTailLayout TrickTipMonitorVtableTail
    __attribute__((section(".sdata"), aligned(4))) = {
    0,
    &__DOT__6CClass_80073FD8,
};

/**
 * Short trick-tip lines emitted in small data by the original owner.
 *
 * These fixed field widths make the original four-byte string padding
 * explicit without reducing the recovered text to an opaque byte blob.
 */
struct TrickTipSmallTextBlock
{
    char HoldJumpTo[0x10];
    char CrouchAnd[0x0c];
    char GoFaster[0x0c];
    char UnlessYouWant[0x10];
    char ToLeaveRamps[0x10];
    char AvoidBailing[0x10];
    char BeforeLanding[0x10];
    char ToOllieHigher[0x10];
    char ReleaseJumpAt[0x10];
    char ForBigAir[0x0c];
    char HoldForwardTo[0x10];
    char PressGrind[0x0c];
    char ButtonNear[0x0c];
    char RailsAndLips[0x10];
    char UseLeftAnd[0x10];
    char WhileGrinding[0x10];
    char LandingTricks[0x10];
    char IncreasesYour[0x10];
    char TopSpeed[0x0c];
    char VaryYour[0x0c];
    char TricksFor[0x0c];
    char HigherScores[0x10];
    char LookForGaps[0x10];
    char ToJumpFor[0x0c];
    char ExtraPoints[0x10];
    char GrindEvery[0x0c];
    char LipAndEdge[0x10];
    char ScorePoints[0x10];
    char ToFillYour[0x10];
    char SpecialBar[0x0c];
    char Line540Or720For[0x10];
    char HugeMultipiers[0x10];
    char AttemptSpecial[0x10];
    char TricksWhen[0x0c];
    char PoweredUp[0x0c];
    char EarnTapes[0x0c];
    char ByCompleting[0x10];
    char LevelGoals[0x0c];
    char TapesUnlock[0x10];
    char NewLevels[0x0c];
    char AndDecks[0x0c];
    char TapUp[0x08];
    char ThenJump[0x0c];
    char ToNollie[0x0c];
    char DoubleTapUp[0x10];
    char ToFastplant[0x10];
    char UseFastplants[0x10];
    char ToLaunchTo[0x10];
    char HighLedges[0x0c];
    char UseR1AndL1[0x10];
    char ToSpinFaster[0x10];
    char OllieIntoWall[0x10];
    char AndHitGrind[0x10];
    char ToWallride[0x0c];
    char HoldUpAnd[0x0c];
    char GrindToDoA[0x10];
    char Handplant[0x0c];
};

typedef char TrickTipSmallTextBlock_size_must_be_0x32C[
    (sizeof(TrickTipSmallTextBlock) == 0x32c) ? 1 : -1];
typedef char CTrickTipText_size_must_be_0x14[
    (sizeof(CTrickTipText) == 0x14) ? 1 : -1];

static TrickTipSmallTextBlock TrickTipSmallText
    __attribute__((section(".sdata"), aligned(4))) = {
    "HOLD JUMP TO",
    "CROUCH AND",
    "GO FASTER",
    "UNLESS YOU WANT",
    "TO LEAVE RAMPS",
    "AVOID BAILING",
    "BEFORE LANDING",
    "TO OLLIE HIGHER",
    "RELEASE JUMP AT",
    "FOR BIG AIR",
    "HOLD FORWARD TO",
    "PRESS GRIND",
    "BUTTON NEAR",
    "RAILS AND LIPS",
    "USE LEFT AND",
    "WHILE GRINDING",
    "LANDING TRICKS",
    "INCREASES YOUR",
    "TOP SPEED",
    "VARY YOUR",
    "TRICKS FOR",
    "HIGHER SCORES",
    "LOOK FOR GAPS",
    "TO JUMP FOR",
    "EXTRA POINTS",
    "GRIND EVERY",
    "LIP AND EDGE",
    "SCORE POINTS",
    "TO FILL YOUR",
    "SPECIAL BAR",
    "540 OR 720 FOR",
    "HUGE MULTIPIERS",
    "ATTEMPT SPECIAL",
    "TRICKS WHEN",
    "POWERED UP",
    "EARN TAPES",
    "BY COMPLETING",
    "LEVEL GOALS",
    "TAPES UNLOCK",
    "NEW LEVELS",
    "AND DECKS",
    "TAP UP",
    "THEN JUMP",
    "TO NOLLIE",
    "DOUBLE TAP UP",
    "TO FASTPLANT",
    "USE FASTPLANTS",
    "TO LAUNCH TO",
    "HIGH LEDGES",
    "USE R1 AND L1",
    "TO SPIN FASTER",
    "OLLIE INTO WALL",
    "AND HIT GRIND",
    "TO WALLRIDE",
    "HOLD UP AND",
    "GRIND TO DO A",
    "HANDPLANT",
};

/**
 * Target-owned trick-tip descriptors and their recovered message lines.
 */
static CTrickTipText TRICKTIP[27]
    __attribute__((section(".rdata"), aligned(4))) = {
    { START_TIP, 2, TrickTipSmallText.HoldJumpTo,
      TrickTipSmallText.CrouchAnd, TrickTipSmallText.GoFaster },
    { ANYTIME_TIP, 2, TrickTipLongText.DontHoldForward,
      TrickTipSmallText.UnlessYouWant, TrickTipSmallText.ToLeaveRamps },
    { BAIL_TIP, 2, TrickTipLongText.LandStraightTo,
      TrickTipSmallText.AvoidBailing, 0 },
    { BAIL_TIP, 2, TrickTipLongText.FinishYourTricks,
      TrickTipSmallText.BeforeLanding, 0 },
    { ANYTIME_TIP, 2, TrickTipLongText.ReleaseXToJump, 0, 0 },
    { ANYTIME_TIP, 2, TrickTipLongText.HoldJumpLonger,
      TrickTipSmallText.ToOllieHigher, 0 },
    { ANYTIME_TIP, 2, TrickTipSmallText.ReleaseJumpAt,
      TrickTipLongText.TheTopOfRamps, TrickTipSmallText.ForBigAir },
    { ANYTIME_TIP, 2, TrickTipSmallText.HoldForwardTo,
      TrickTipLongText.GetOverPoolLip, 0 },
    { START_TIP, 2, TrickTipSmallText.PressGrind,
      TrickTipSmallText.ButtonNear, TrickTipSmallText.RailsAndLips },
    { BAIL_TIP, 2, TrickTipSmallText.UseLeftAnd,
      TrickTipLongText.RightToBalance, TrickTipSmallText.WhileGrinding },
    { ANYTIME_TIP, 2, TrickTipLongText.PressJumpWhile,
      TrickTipLongText.GrindingToOllieOut, 0 },
    { START_TIP, 2, TrickTipSmallText.LandingTricks,
      TrickTipSmallText.IncreasesYour, TrickTipSmallText.TopSpeed },
    { START_TIP, 2, TrickTipSmallText.VaryYour,
      TrickTipSmallText.TricksFor, TrickTipSmallText.HigherScores },
    { START_TIP, 2, TrickTipSmallText.LookForGaps,
      TrickTipSmallText.ToJumpFor, TrickTipSmallText.ExtraPoints },
    { START_TIP, 2, TrickTipSmallText.GrindEvery,
      TrickTipSmallText.LipAndEdge, 0 },
    { START_TIP, 2, TrickTipSmallText.ScorePoints,
      TrickTipSmallText.ToFillYour, TrickTipSmallText.SpecialBar },
    { START_TIP, 2, TrickTipLongText.SpinVertTricks,
      TrickTipSmallText.Line540Or720For, TrickTipSmallText.HugeMultipiers },
    { START_TIP, 2, TrickTipSmallText.AttemptSpecial,
      TrickTipSmallText.TricksWhen, TrickTipSmallText.PoweredUp },
    { START_TIP, 2, TrickTipSmallText.EarnTapes,
      TrickTipSmallText.ByCompleting, TrickTipSmallText.LevelGoals },
    { START_TIP, 2, TrickTipSmallText.TapesUnlock,
      TrickTipSmallText.NewLevels, TrickTipSmallText.AndDecks },
    { START_TIP, 1, TrickTipSmallText.TapUp,
      TrickTipSmallText.ThenJump, TrickTipSmallText.ToNollie },
    { START_TIP, 2, TrickTipSmallText.DoubleTapUp,
      TrickTipSmallText.ThenJump, TrickTipSmallText.ToFastplant },
    { START_TIP, 2, TrickTipSmallText.UseFastplants,
      TrickTipSmallText.ToLaunchTo, TrickTipSmallText.HighLedges },
    { START_TIP, 1, TrickTipSmallText.UseR1AndL1,
      TrickTipSmallText.ToSpinFaster, 0 },
    { START_TIP, 2, TrickTipSmallText.OllieIntoWall,
      TrickTipSmallText.AndHitGrind, TrickTipSmallText.ToWallride },
    { START_TIP, 2, TrickTipSmallText.HoldUpAnd,
      TrickTipSmallText.GrindToDoA, TrickTipSmallText.Handplant },
    { START_TIP, 2, 0, 0, 0 },
};

/**
 * Static-storage owner for the trick-tip monitor.  Its inline construction
 * and destruction reproduce the original vtable transitions while leaving
 * the monitor's state zero-initialized until TrickTips_Init fills it.
 */
struct TrickTipMonitorStorage : CTrickTipMonitor
{
    /** Install the complete-object vtable during static construction. */
    TrickTipMonitorStorage()
    {
    CClass.vtable = &__vt_16CTrickTipMonitor_complete;
    }

    /** Restore the destruction-phase vtable before static teardown. */
    ~TrickTipMonitorStorage()
    {
    CClass.vtable = &__vt_16CTrickTipMonitor;
    }
};

static TrickTipMonitorStorage TrickTipMonitor
    __attribute__((section(".bss")));

// === Function implementations (address order) ===

/**
 * TrickTips_Display__Fv
 *
 * Address:   0x80073274
 * Mangled:   TrickTips_Display__Fv
 * Demangled: TrickTips_Display(void)
 *
 * Draws the active trick-tip text panel and clears it when its timer expires.
 */
void TrickTips_Display(void) {
    unsigned short xpos = TrickTipsPanelX;

    if (TrickTipMonitor.mTipLine1 != 0) {
        if (static_cast<unsigned short>(
                TrickTipsDisplayFrames - TrickTipMonitor.mTipTimer) <
            TrickTipsSlideFrames) {
            xpos -= (TrickTipsSlideFrames - static_cast<unsigned short>(
                TrickTipsDisplayFrames - TrickTipMonitor.mTipTimer)) *
                TrickTipsSlideDistance;
        }

        if (TrickTipMonitor.mTipTimer < TrickTipsSlideFrames) {
            xpos -= (TrickTipsSlideFrames - TrickTipMonitor.mTipTimer) *
                TrickTipsSlideDistance;
        }

        Mess_SetTextJustify(1);
        Mess_SetRGB(0x80, 0x80, 0, 0);
        Mess_SetScale(TrickTipsTextScale);
        Mess_DrawPanelText(
            xpos, TrickTipsPanelY, TrickTipMonitor.mTipLine1,
            0, 0, TrickTipsFireScale);

        if (TrickTipMonitor.mTipLine2 != 0) {
            Mess_DrawPanelText(
                xpos, (Mess_GetScale() / 11) + TrickTipsPanelY,
                TrickTipMonitor.mTipLine2,
                0, 0, TrickTipsFireScale);
        }

        if (TrickTipMonitor.mTipLine3 != 0) {
            Mess_DrawPanelText(
                xpos, ((Mess_GetScale() / 11) * 2) +
                      TrickTipsPanelY,
                TrickTipMonitor.mTipLine3,
                0, 0, TrickTipsFireScale);
        }

        if (TrickTipMonitor.mTipLine4 != 0) {
            Mess_DrawPanelText(
                xpos, ((Mess_GetScale() / 11) * 3) +
                      TrickTipsPanelY,
                TrickTipMonitor.mTipLine4,
                0, 0, TrickTipsFireScale);
        }

        TrickTipMonitor.mTipTimer--;
        if (TrickTipMonitor.mTipTimer == 0) {
            TrickTipMonitor.mTipLine4 = 0;
            TrickTipMonitor.mTipLine3 = 0;
            TrickTipMonitor.mTipLine2 = 0;
            TrickTipMonitor.mTipLine1 = 0;
        }
    }
}


/**
 * Reset the monitor timers, counters, and active text.
 *
 * Address:   0x8007347C
 * Mangled:   TrickTips_Init__Fv
 * Demangled: TrickTips_Init(void)
 */
void TrickTips_Init(void) {
    CTrickTipMonitor &monitor = TrickTipMonitor;
    monitor.mTimer = 0;
    monitor.mLastTipTime = 0;
    monitor.mJumpTimer = 0;
    monitor.mBailCounter = 0;
    monitor.mPerpendicularBailCounter = 0;
    monitor.mTrickBailCounter = 0;
    monitor.mRailBailCounter = 0;
    monitor.mJumpCounter = 0;
    monitor.mWeakJumpCounter = 0;
    monitor.mAirCounter = 0;
    monitor.mTrickCounter = 0;
    monitor.mRailCounter = 0;
    monitor.mAirPolyCounter = 0;
    monitor.mAirPolyBreakCounter = 0;
    monitor.mGrindCounter = 0;
    monitor.mTipTimer = 0;
    monitor.mTipLine1 = 0;
    monitor.mTipLine2 = 0;
    monitor.mTipLine3 = 0;
    monitor.mTipLine4 = 0;
    monitor.mOldBails = 0;
    monitor.mOldPerpendicularBails = 0;
    monitor.mOldTrickBails = 0;
    monitor.mOldRailBails = 0;
    monitor.mOldJumps = 0;
    monitor.mOldWeakJumps = 0;
    monitor.mOldAirs = 0;
    monitor.mOldTricks = 0;
    monitor.mOldRails = 0;
    monitor.mOldAirPolys = 0;
    monitor.mOldAirPolyBreaks = 0;
    monitor.mOldGrinds = 0;
    monitor.mHoldingForwardCounter = 0;
    monitor.mOkayToShow = reinterpret_cast<void *>(1);
}

/** Returns the career bitfield bank containing a trick-tip index. */
static inline int TrickTips_GetBank(int tip)
{
    if (tip < 32)
    {
        return 0;
    }
    else if (tip < 64)
    {
        return 1;
    }
}

/**
 * Update the monitor from current gameplay counters.
 *
 * Address:   0x80073514
 * Mangled:   TrickTips_Update__Fv
 * Demangled: TrickTips_Update(void)
 *
 * Updates the trick-tip monitor from the current skater's gameplay counters,
 * chooses enabled tips that have not been suppressed for this level, and
 * populates the display text consumed by TrickTips_Display().
 */
void TrickTips_Update(void)
{
    CTrickTipMonitor &monitor = TrickTipMonitor;

    if (!Levels[GLevel].isCompetition &&
        GVInfo1.VideoMode != TrickTipsPlaybackVideoMode &&
        GVInfo2.VideoMode != TrickTipsPlaybackVideoMode &&
        DemoTimer &&
        GGame == 1)
    {
        monitor.pSkater = reinterpret_cast<CBruce *>(MechList);

        if (monitor.mTimer == 0)
        {
            int i;
            int tip;

            for (i = 0; i < TrickTipsDescriptorCount; i++)
            {
                if (TRICKTIP[i].mOccurance == 0)
                {
                    if (((GCareer.Options.TrickTipUsed[TrickTips_GetBank(i)] >>
                          (i - TrickTips_GetBank(i) * 32)) & 1) == 0)
                    {
                        tip = i;
                        goto tip_search_complete;
                    }
                }
            }

            tip = -1;

        tip_search_complete:
            if (tip >= 0 &&
                (monitor.mTimer == 0 ||
                 monitor.mLastTipTime + TrickTipsReminderDelay <=
                 monitor.mTimer))
            {
                int tip_mask;
                int tip_used;
                int tip_bank;
                monitor.mLastTipTime = monitor.mTimer;

                if (tip < 32)
                {
                    tip_bank = 0;
                }
                else if (tip < 64)
                {
                    tip_bank = 1;
                }

                if (GCareer.Options.TrickTipsEnabled &&
                    ((Levels[GLevel].TrickTipMask[tip_bank] >>
                      (tip - tip_bank * 32)) & 1) == 0)
                {
                    tip_mask = GCareer.Options.TrickTipMask[tip_bank];
                    tip_used = GCareer.Options.TrickTipUsed[tip_bank];
                    int bit = 1 << (tip - tip_bank * 32);
                    GCareer.Options.TrickTipUsed[tip_bank] = tip_used | bit;
                    tip_mask &= bit;
                    tip_used &= bit;
                    if (tip_mask == 0)
                    {
                        if (tip_used)
                        {
                            monitor.mTipLine1 = TEXT_REMEMBER;
                            GCareer.Options.TrickTipMask[tip_bank] |= bit;
                        }
                        else
                        {
                            monitor.mTipLine1 = TEXT_TIP;
                        }

                        monitor.mTipLine2 = TRICKTIP[tip].mLine1;
                        monitor.mTipLine3 = TRICKTIP[tip].mLine2;
                        monitor.mTipLine4 = TRICKTIP[tip].mLine3;
                        monitor.mTipTimer = TrickTipsDisplayFrames;
                    }
                }
            }
        }

        monitor.mTimer++;
        monitor.mJumpTimer++;

        if (monitor.mOldTricks <
            TrickTips_AsSkater(monitor.pSkater)->mTricks)
        {
            monitor.mOldTricks =
                TrickTips_AsSkater(monitor.pSkater)->mTricks;
        }

        if (monitor.mOldGrinds <
            TrickTips_AsSkater(monitor.pSkater)->mNumGrinds)
        {
            monitor.mOldGrinds =
                TrickTips_AsSkater(monitor.pSkater)->mNumGrinds;
            monitor.mGrindCounter++;
            GCareer.Options.TrickTipUsed[0] |= 0x100;
            GCareer.Options.TrickTipMask[0] |= 0x100;
        }

        if (monitor.mOldJumps <
            TrickTips_AsSkater(monitor.pSkater)->mJumps)
        {
            monitor.mOldJumps =
                TrickTips_AsSkater(monitor.pSkater)->mJumps;
            monitor.mJumpTimer = 0;
            monitor.mJumpCounter++;
        }

        if (monitor.mJumpTimer > TrickTipsReminderDelay)
        {
            monitor.mJumpTimer = 0;
        }

        if (Pad[0].Up.Pressed || Pad[1].Up.Pressed)
        {
            monitor.mHoldingForwardCounter++;
        }
        else
        {
            monitor.mHoldingForwardCounter = 0;
        }

        if (monitor.mHoldingForwardCounter > TrickTipsHoldingForwardFrames)
        {
            if ((monitor.mTimer == 0 ||
                 monitor.mLastTipTime + TrickTipsReminderDelay <=
                 monitor.mTimer) &&
                (monitor.mLastTipTime = monitor.mTimer,
                 GCareer.Options.TrickTipsEnabled) &&
                ((Levels[GLevel].TrickTipMask[0] >> 1) & 1) == 0)
            {
                int tip_mask = GCareer.Options.TrickTipMask[0] & (1 << 1);
                int tip_used = GCareer.Options.TrickTipUsed[0] & (1 << 1);

                GCareer.Options.TrickTipUsed[0] |= 1 << 1;
                if (tip_mask == 0)
                {
                    if (tip_used)
                    {
                        GCareer.Options.TrickTipMask[0] |= 1 << 1;
                        monitor.mTipLine1 = TEXT_REMEMBER;
                    }
                    else
                    {
                        monitor.mTipLine1 = TEXT_TIP;
                    }

                    monitor.mTipTimer = TrickTipsDisplayFrames;
                    monitor.mTipLine2 = TRICKTIP[1].mLine1;
                    monitor.mTipLine3 = TRICKTIP[1].mLine2;
                    monitor.mTipLine4 = TRICKTIP[1].mLine3;
                }
            }

            monitor.mHoldingForwardCounter = 0;
        }

        if (monitor.mOldAirPolyBreaks <
            TrickTips_AsSkater(monitor.pSkater)->mAirPolyBreaks)
        {
            monitor.mOldAirPolyBreaks =
                TrickTips_AsSkater(monitor.pSkater)->mAirPolyBreaks;
            monitor.mAirPolyBreakCounter++;

            if (monitor.mAirPolyBreakCounter >= 3)
            {
                monitor.mAirPolyBreakCounter = 0;

                if (monitor.mBigAirCounter < 3 &&
                    (monitor.mTimer == 0 ||
                     monitor.mLastTipTime + TrickTipsReminderDelay <=
                     monitor.mTimer) &&
                    (monitor.mLastTipTime = monitor.mTimer,
                     GCareer.Options.TrickTipsEnabled) &&
                    ((Levels[GLevel].TrickTipMask[0] >> 6) & 1) == 0)
                {
                    int tip_mask =
                        GCareer.Options.TrickTipMask[0] & (1 << 6);
                    int tip_used =
                        GCareer.Options.TrickTipUsed[0] & (1 << 6);

                    GCareer.Options.TrickTipUsed[0] |= 1 << 6;
                    if (tip_mask == 0)
                    {
                        if (tip_used)
                        {
                            GCareer.Options.TrickTipMask[0] |= 1 << 6;
                            monitor.mTipLine1 = TEXT_REMEMBER;
                        }
                        else
                        {
                            monitor.mTipLine1 = TEXT_TIP;
                        }

                        monitor.mTipTimer = TrickTipsDisplayFrames;
                        monitor.mTipLine2 = TRICKTIP[6].mLine1;
                        monitor.mTipLine3 = TRICKTIP[6].mLine2;
                        monitor.mTipLine4 = TRICKTIP[6].mLine3;
                    }
                }
            }
        }

        if (monitor.mOldAirPolys <
            TrickTips_AsSkater(monitor.pSkater)->mAirPolys)
        {
            monitor.mOldAirPolys =
                TrickTips_AsSkater(monitor.pSkater)->mAirPolys;
            monitor.mAirPolyCounter++;
        }

        if (monitor.mOldBigAirs <
            TrickTips_AsSkater(monitor.pSkater)->mBigAirs)
        {
            monitor.mOldBigAirs =
                TrickTips_AsSkater(monitor.pSkater)->mBigAirs;
            monitor.mBigAirCounter++;
        }

        if (monitor.mOldWeakJumps <
            TrickTips_AsSkater(monitor.pSkater)->mWeakJumps)
        {
            monitor.mOldWeakJumps =
                TrickTips_AsSkater(monitor.pSkater)->mWeakJumps;
            monitor.mWeakJumpCounter++;

            if (monitor.mWeakJumpCounter >= 5)
            {
                if ((monitor.mTimer == 0 ||
                     monitor.mLastTipTime + TrickTipsReminderDelay <=
                     monitor.mTimer) &&
                    (monitor.mLastTipTime = monitor.mTimer,
                     GCareer.Options.TrickTipsEnabled) &&
                    ((Levels[GLevel].TrickTipMask[0] >> 5) & 1) == 0)
                {
                    int tip_mask =
                        GCareer.Options.TrickTipMask[0] & (1 << 5);
                    int tip_used =
                        GCareer.Options.TrickTipUsed[0] & (1 << 5);

                    GCareer.Options.TrickTipUsed[0] |= 1 << 5;
                    if (tip_mask == 0)
                    {
                        if (tip_used)
                        {
                            GCareer.Options.TrickTipMask[0] |= 1 << 5;
                            monitor.mTipLine1 = TEXT_REMEMBER;
                        }
                        else
                        {
                            monitor.mTipLine1 = TEXT_TIP;
                        }

                        monitor.mTipTimer = TrickTipsDisplayFrames;
                        monitor.mTipLine2 = TRICKTIP[5].mLine1;
                        monitor.mTipLine3 = TRICKTIP[5].mLine2;
                        monitor.mTipLine4 = TRICKTIP[5].mLine3;
                    }
                }

                monitor.mWeakJumpCounter = 0;
            }
        }

        if (monitor.mOldBails <
            TrickTips_AsSkater(monitor.pSkater)->mBails)
        {
            monitor.mOldBails =
                TrickTips_AsSkater(monitor.pSkater)->mBails;
            monitor.mBailCounter++;

            if (monitor.mBailCounter >= 3)
            {
                if ((monitor.mTimer == 0 ||
                     monitor.mLastTipTime + TrickTipsReminderDelay <=
                     monitor.mTimer) &&
                    (monitor.mLastTipTime = monitor.mTimer,
                     GCareer.Options.TrickTipsEnabled) &&
                    ((Levels[GLevel].TrickTipMask[0] >> 3) & 1) == 0)
                {
                    int tip_mask =
                        GCareer.Options.TrickTipMask[0] & (1 << 3);
                    int tip_used =
                        GCareer.Options.TrickTipUsed[0] & (1 << 3);

                    GCareer.Options.TrickTipUsed[0] |= 1 << 3;
                    if (tip_mask == 0)
                    {
                        if (tip_used)
                        {
                            GCareer.Options.TrickTipMask[0] |= 1 << 3;
                            monitor.mTipLine1 = TEXT_REMEMBER;
                        }
                        else
                        {
                            monitor.mTipLine1 = TEXT_TIP;
                        }

                        monitor.mTipTimer = TrickTipsDisplayFrames;
                        monitor.mTipLine2 = TRICKTIP[3].mLine1;
                        monitor.mTipLine3 = TRICKTIP[3].mLine2;
                        monitor.mTipLine4 = TRICKTIP[3].mLine3;
                    }
                }

                monitor.mBailCounter = 0;
            }
        }

        if (monitor.mOldPerpendicularBails <
            TrickTips_AsSkater(monitor.pSkater)->mPerpendicularBails)
        {
            monitor.mOldPerpendicularBails =
                TrickTips_AsSkater(monitor.pSkater)->mPerpendicularBails;
            monitor.mPerpendicularBailCounter++;

            if (monitor.mPerpendicularBailCounter >= 3)
            {
                if ((monitor.mTimer == 0 ||
                     monitor.mLastTipTime + TrickTipsReminderDelay <=
                     monitor.mTimer) &&
                    (monitor.mLastTipTime = monitor.mTimer,
                     GCareer.Options.TrickTipsEnabled) &&
                    ((Levels[GLevel].TrickTipMask[0] >> 2) & 1) == 0)
                {
                    int tip_mask =
                        GCareer.Options.TrickTipMask[0] & (1 << 2);
                    int tip_used =
                        GCareer.Options.TrickTipUsed[0] & (1 << 2);

                    GCareer.Options.TrickTipUsed[0] |= 1 << 2;
                    if (tip_mask == 0)
                    {
                        if (tip_used)
                        {
                            GCareer.Options.TrickTipMask[0] |= 1 << 2;
                            monitor.mTipLine1 = TEXT_REMEMBER;
                        }
                        else
                        {
                            monitor.mTipLine1 = TEXT_TIP;
                        }

                        monitor.mTipTimer = TrickTipsDisplayFrames;
                        monitor.mTipLine2 = TRICKTIP[2].mLine1;
                        monitor.mTipLine3 = TRICKTIP[2].mLine2;
                        monitor.mTipLine4 = TRICKTIP[2].mLine3;
                    }
                }

                monitor.mPerpendicularBailCounter = 0;
            }
        }

        if (monitor.mOldRailBails <
            TrickTips_AsSkater(monitor.pSkater)->mRailBails)
        {
            monitor.mOldRailBails =
                TrickTips_AsSkater(monitor.pSkater)->mRailBails;
            monitor.mRailBailCounter++;

            if (monitor.mRailBailCounter >= 3)
            {
                if ((monitor.mTimer == 0 ||
                     monitor.mLastTipTime + TrickTipsReminderDelay <=
                     monitor.mTimer) &&
                    (monitor.mLastTipTime = monitor.mTimer,
                     GCareer.Options.TrickTipsEnabled) &&
                    ((Levels[GLevel].TrickTipMask[0] >> 9) & 1) == 0)
                {
                    int tip_mask =
                        GCareer.Options.TrickTipMask[0] & (1 << 9);
                    int tip_used =
                        GCareer.Options.TrickTipUsed[0] & (1 << 9);

                    GCareer.Options.TrickTipUsed[0] |= 1 << 9;
                    if (tip_mask == 0)
                    {
                        if (tip_used)
                        {
                            GCareer.Options.TrickTipMask[0] |= 1 << 9;
                            monitor.mTipLine1 = TEXT_REMEMBER;
                        }
                        else
                        {
                            monitor.mTipLine1 = TEXT_TIP;
                        }

                        monitor.mTipTimer = TrickTipsDisplayFrames;
                        monitor.mTipLine2 = TRICKTIP[9].mLine1;
                        monitor.mTipLine3 = TRICKTIP[9].mLine2;
                        monitor.mTipLine4 = TRICKTIP[9].mLine3;
                    }
                }

                monitor.mRailBailCounter = 0;
            }
        }
    }
}


// _GLOBAL_.D.TEXT_TIP and _GLOBAL_.I.TEXT_TIP are emitted from the
// TrickTipMonitorStorage global above.
