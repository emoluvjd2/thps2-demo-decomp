// PHYSICS.cpp
// Original: PHYSICS.CPP
// Address range: 0x80010000 - 0x80020164
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#define PHYSICS_OWNER_DECLARATIONS
#include "PHYSICS.h"
#undef PHYSICS_OWNER_DECLARATIONS

#ifdef __cplusplus
extern "C"
{
#endif

// Primitive types
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned char byte;
typedef int undefined;
typedef char undefined1;
typedef short undefined2;
typedef int undefined4;
typedef unsigned long ulong;
typedef unsigned char undefined3[3];

// Game vector types
struct CBruce;
struct CVector
{
    int vx, vy, vz;
#ifdef __cplusplus
    void Set(int x, int y, int z)
    {
        vx = x;
        vy = y;
        vz = z;
    }
    /** Return this vector's fixed-point magnitude. */
    int Length() const;
    /** Normalize this vector in place. */
    void Normalize();
    int operator!=(const CVector &vector);
    CVector &operator+=(const CVector &vector);
    CVector &operator-=(const CVector &vector);
    CVector &operator*=(const int &scale);
#endif
};
struct CSVector
{
    short vx, vy, vz;
};
#include "CFRICTION.h"

// Additional shared forward declarations
struct CBit;
struct CGlow;
struct CSpecialDisplay;
struct CFlatBit;
struct Font;
struct TextEffect;
struct FontManager;
struct RailPoint;
struct SCardHead;
struct SCamera;
struct STexWibVertInfo;
struct SViewport;
struct SColCache;
struct SLineInfo;
struct STexWibItemInfo;
struct SBlockHeader;
struct ControllerData;
struct SVideoInfo;

struct SButtonEvent
{
    unsigned char Button;
    unsigned char Pressed;
    unsigned int Time;
};

struct STrickQueue
{
    short *pTrick;
    unsigned int Time;
    int Button;
    unsigned int EventTime;
};

struct SGapTrick;

#ifdef __cplusplus
}
#endif

#include "M3D.h"
#include "PANEL.h"
#include "MECH.h"
#include "MAIN.h"
#include "RFRONT.h"
#include "STATS.h"

// Absolute globals used directly by PHYSICS.
#include "GLEVEL.h"
#define VIDEOINFO_DECLARE_GVINFO1
#define VIDEOINFO_DECLARE_GVINFO2
#include "VIDEOINFO.h"
#define BITSTATE_DECLARE_USE_CACHE_FOR_BITS
#include "BITSTATE.h"

#include <STDDEF.H>
#include <LIBGTE.H>
#include <INLINE_C.H>
#include <LIBGPU.H>
#include "MIPS.h"
#include "M3DUTILS.h"
#include "M3DZONE.h"
// Owner-local CBruce overlays for PHYSICS.cpp.
// These stay local until the field names and offsets are proven across
// multiple owner TUs without match regressions.
#define OB_LAYOUT_ONLY
#define OB_OWNER_LAYOUT
#include "OB.h"
#undef OB_OWNER_LAYOUT
#undef OB_LAYOUT_ONLY
#include "PAD.h"
#include "PSXREGION.h"
#define M3DCOLIJ_LINE_INFO_LAYOUT
#include "M3DCOLIJ.h"
#undef M3DCOLIJ_LINE_INFO_LAYOUT

/** Short vector retaining the original inline component setter. */
struct CSVectorPhysicsOwner {
    short vx;
    short vy;
    short vz;

    /** Assign all three fixed-point components. */
    void Set(int _x, int _y, int _z) {
        vx = _x;
        vy = _y;
        vz = _z;
    }
};

/** Long game vector with the original inline component setter. */
struct CVectorPhysicsOwner {
    union { int x; int vx; };
    union { int y; int vy; };
    union { int z; int vz; };

    /** Assign all three game-space components. */
    void Set(int _x, int _y, int _z) {
        x = _x;
        y = _y;
        z = _z;
    }
};

/**
 * CBit prefix through the velocity field used by terrain-spark creation.
 *
 * Keeping this as an owner-local view makes the allocator boundary explicit
 * without turning the returned pointer into an integer or spelling a field
 * offset in PHYSICS.cpp.
 */
struct CBitVelocityPhysicsOwner {
    CBitVelocityPhysicsOwner *mPrevious;
    CBitVelocityPhysicsOwner *mNext;
    short mAge;
    unsigned short mLifetime;
    CVector mTweenPos;
    CVector mPos;
    CVector mVel;
};

typedef char CBitVelocityPhysicsOwner_size_must_be_48[
    (sizeof(CBitVelocityPhysicsOwner) == 48) ? 1 : -1];

#include "CTRICK.h"

/** Owner-local panel slice containing the fields read when a trick lands. */
struct CPanelData {
    /** Queue a trick and update its spin, stance, and lock state. */
    void Trick(char *name, int points, int lock, int depreciate);
    /** Calculate the current trick-chain score and combo multiplier. */
    int TotalScore();

    u8 pad_to_mLandedTotal[320];
    unsigned int mLandedTotal;
    u8 pad_to_mTQ[1200 - 324];
    struct CTrick mTQ[20];
    int mTQStack;
    u8 pad_after_mTQStack[2276 - 1924];
};

/**
 * Career slice containing the per-player automatic-kick and vibration options.
 *
 * `SCareer::Options` starts at 0x7d4. Within `SOptions`, `AutoKickEnabled`
 * starts at 0x104 and `VibrationEnabled` follows it at 0x10c.
 */
struct SCareerOptionsPhysicsOwner {
    u8 pad_to_AutoKickEnabled[0x8D8];
    int AutoKickEnabled[2];
    int VibrationEnabled[2];
};

struct CBruceScriptQueueSlice {
    struct CSuperOwner CSuper;
    u8 pad_to_mpTrick[2292];
    short *mpTrick;
    short mTrickPoints;
    u8 pad_after_trickPoints[2];
    char *mpTrickName;
    int mDoingAScript;
    short *mpOffTrick;
    short *mpJumpTrick;
    int mDebugScript;
    short mBailNumber;
    short mRailBailNumber;
    short mTrickGravity;
    u8 pad_after_trickGravity[2];
    short *mpReturn;
    int mActionButton;
    int mActionButtonEventTime;
    struct SButtonEvent mEventQueue[32];
    int mEventQueuePos;
    u8 mCurrentButtonState[32];
    struct STrickQueue mTrickQueue[10];
    int mTricksQueued;
    int mTrickQueueTail;
    short *mpRailTrickQueued;
    int mRailTrickTime;
};

/**
 * Owner-local view of the button history and state consulted while matching
 * trick-key sequences.
 */
struct CBruceTrickTriggerSlice {
    u8 pad_to_mEventQueue[0xAB4];
    struct SButtonEvent mEventQueue[32];
    int mEventQueuePos;
    u8 pad_to_mSkaterType[0xDA8 - 0xBB8];
    int mSkaterType;
    u8 pad_to_mInBail[0xF7C - 0xDAC];
    int mInBail;
    u8 pad_to_mMagicTime[0xF88 - 0xF80];
    int mMagicTime;
    u8 pad_to_mPhysicsState[0x106C - 0xF8C];
    int mPhysicsState;
};

/** Whether a bail should clamp the skater's velocity. */
enum ELimitBailVel {
    DO_LIMIT = 0,
    DONT_LIMIT = 1
};

struct CBrucePhysicsSlice {
    struct CSuperOwner CSuper;
    struct CPanelData mPanelData;                 /* 0x0190 */
    struct CCamera *mpCamera;                     /* 0x0A74 */
    struct BoardStats *mpBoardStats;              /* 0x0A78 */
    struct CCar *mIgnoreCar;                      /* 0x0A7C */
    struct CBruce *mpOtherPlayer;                 /* 0x0A80 */
    /* Script and trick-queue block (also mirrored in CBruceScriptQueueSlice). */
    short *mpTrick;                               /* 0x0A84 */
    short mTrickPoints;
    u8 pad_after_trickPoints[2];
    char *mpTrickName;
    int mDoingAScript;
    short *mpOffTrick;
    short *mpJumpTrick;
    int mDebugScript;
    short mBailNumber;
    short mRailBailNumber;
    short mTrickGravity;
    u8 pad_after_trickGravity[2];
    short *mpReturn;
    int mActionButton;
    int mActionButtonEventTime;
    struct SButtonEvent mEventQueue[32];          /* 0x0AB4 */
    int mEventQueuePos;
    u8 mCurrentButtonState[32];
    struct STrickQueue mTrickQueue[10];
    int mTricksQueued;
    int mTrickQueueTail;
    short *mpRailTrickQueued;
    int mRailTrickTime;                           /* 0x0C84 */
    /* The rail-state flag block the grind and rail-physics paths read. */
    int mRail_Parallel;                           /* 0x0C88 */
    int mRail_Backwards;                          /* 0x0C8C */
    int mRail_RightOfRail;                        /* 0x0C90 */
    int mRail_FrontSide;                          /* 0x0C94 */
    int mRail_SkaterFlipped;                      /* 0x0C98 */
    int mRail_LedgeType;                          /* 0x0C9C */
    int mRail_Direction;                          /* 0x0CA0 */
    int mRail_BadLedge;                           /* 0x0CA4 */
    int mRailSparks;                              /* 0x0CA8 */
    int mHeavyFriction;                           /* 0x0CAC */
    /* Embedded Dust and Blood emitters, addressed by their owning helpers. */
    u8 mDust[64];                                 /* 0x0CB0 */
    u8 mBlood[92];                                /* 0x0CF0 */
    short mFrozen;
    u8 pad_after_frozen[2];
    int mDoingATrick;
    int mCanStartNextTrick;
    int mTrickAwarded;
    int mDoingWhatTrick;
    int mFlipAfter;
    int mRotateAfter;
    int mNoSpin;
    int mNoSpinScore;
    int mFlipNum;
    int mFlipAngle;
    int mFlipSpeed;
    int mRotateSpeed[3];
    int mRotateCount[3];
    int mRotateTotal[3];
    int mNoTweak;
    int mNoTrick;
    int mSkaterType;
    int mIndex;
    int mPlayTime;
    struct SControl *mPad;
    int mTags;
    int mSteals;
    struct Item *mLastSItem;
    unsigned int *mLastPSX;
    int mJustUp;
    int mAutoTurn;
    int mTooFast;
    int mLandedTime;
    int mTotalAirSnap;
    int mInAirStickToStart;
    int mWasAir;
    struct CVectorOwner mAirGravity;
    int mIsOnTaggedPoly;
    int mStickToInvisible;
    struct CVectorOwner mLastVel;
    int mMaxKickSpeed;
    int mSpeedBoost;
    int mCess;
    int mManual;
    int mManIsCrouched;
    int mManIsJumping;
    int mJumpPending;
    int mAirTime;
    int mJumpStrength;
    int mLastJumpStrength;
    int mApplyJumpForce;
    int mJumpsOffWallRide;
    int mBrakeMode;
    struct CVectorOwner mOldPos2;
    struct CVectorOwner mSafePos;
    struct CVectorOwner mLastGroundPos;
    int mLastTurnDirection;
    int mLastXFrames;
    int mRailSlack;
    int mJumpInhibit;
    int mRailInhibit;
    MATRIX mOldRotationMatrix;
    MATRIX mRotationMatrix;
    int mDoingSharpTurn;
    int mTurning;
    int mTapTurn;
    int mPitchAngle;
    int mNumGrinds;
    int mRailType;
    int mOnRail;
    int mStickToRail;
    int mLastRailStart;
    int mCurrentRailStart;
    int mCurrentRailSegment;
    int mDistToCurrentRail;
    struct CVectorOwner mRailPos;
    struct CVectorOwner mRailDir;
    int mDontStickTimer;
    int mSameRailTimer;
    int mDontStickRail;
    struct CVectorOwner mGrindStartPos;
    int mRailFlip;
    struct CVectorOwner mGrindPos;
    int mGrindReleasedLR;
    int mGrindLean;
    int mGrindLeanDir;
    int mGrindLeanAcc;
    int mGrindTime;
    int mGrindTweak;
    struct CVectorOwner mManualPos;
    int mManualReleasedLR;
    int mManualLean;
    int mManualLeanDir;
    int mManualLeanAcc;
    int mManualTime;
    int mManualCheese;
    int mGrindTrick;
    int mKickLockout;
    int mJumpSpeed;
    int mJumpFrame;
    int mWallrideTime;
    int mHandPlantTweaked;
    int mBailState;
    int mLimitBailVel;
    int mInBail;
    int mHeadHit;
    int mMagicPoints;
    int mMagicTime;
    int mGapChecksum[10];
    int mGapType[10];
    int mGaps;
    struct SGapTrick *mpGrindGapTrick;
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
    u8 mJumpTimer;
    u8 pad_after_jumpTimer[3];
    int mFirstLeftRightPressed;
    int mTrickTookOff;
    int mTallyAngles;
    int mNum180Spins;
    int mComboCounter;
    int mPerfectLanding;
    int mSloppyLanding;
    int mLastShadowCheckHeight;
    int mGroundVol;
    int mGroundPitch;
    u32 mSFXWallride;
    u32 mSFXGrind;
    u32 mSFXGround;
    u32 mSFXAir;
    u32 mSFXPitch;
    u32 mSFXVol;
    int mTruckSqueak;
    int mFootSound;
    int mFootSFX;
    int mTerrain;
    int mTerrainSFX;
    int mPhysicsState;
    int mOldPhysicsState;
    MATRIX mTempRotationMatrix;
    struct CVectorOwner mTempPos;
    struct CVectorOwner mFront;
    struct CVectorOwner mRight;
    struct CVectorOwner mUp;
    struct CVectorOwner mLastFallLine;
    int mFallLineAdjustment;
    SVECTOR mLastPolyNormal;
    int mSameNormal;
    VECTOR mOldNormal;
    int mLeanAngle;
    int mAnimLeanAngle;
    struct CVectorOwner mLastPos;
    struct CVectorOwner mNextPos;
    struct CSVectorOwner mLastAngles;
    struct CSVectorOwner mNextAngles;
    u8 mGlowR;
    u8 mGlowG;
    u8 mGlowB;
    u8 mTargetGlowR;
    u8 mTargetGlowG;
    u8 mTargetGlowB;
    u8 mGlowSpeed;
    u8 mGlowDownSpeed;
    int mBrightness;
    void *mAllowDeathCam;
    int mIgnoreBruceInputTimer;
    int mIgnoreBruceInputFreezeTimer;
    u8 mAnalogue;
    char mAX;
    char mAY;
    u8 mAnalogueAimInput;                         /* 0x1137 */
    char mAnalogueAimForwardsBackwards;
    char mAnalogueAimLeftRight;
    u8 mDPadAimInput;
    short mAnalogueMotionAngle;                   /* 0x113C */
    short mAnalogueAimAngle;
    int mSessionTapes;                            /* 0x1140 */
    u16 mInAirTimer;                              /* 0x1144 */
    u8 pad_after_mInAirTimer[2];
    int mHistoryPos;                              /* 0x1148 */
    struct SHistory mHistory[100];                /* 0x114C */
};

struct SLight;

/**
 * Offset-zero CItem prefix recovered for naturally spelled physics methods.
 */
struct CItemPhysicsNatural {
    u16 mFlags;
    u16 mInquiry;
    CVector mPos;
    CSVector mAngles;
    u16 mModel;
    char mDummyFrame;
    u8 mTintIndex;
    u8 mDummyAnim;
    u8 mRegion;
    CItemPhysicsNatural *mNextItem;
    u32 mRGB;
    CSVector mScale;
    u32 mTRN;
    CItemPhysicsNatural *mPreviousItem;
    u32 mStoredRGB;
    u8 mPredatorEffectType;
    u8 mPredatorEffectFade;
    u16 mType;
    SLight *mpLight;
    u16 mFlashTimer;
    s16 mFlash_dR;
    s16 mFlash_dG;
    s16 mFlash_dB;
    void *_vf;
};

/**
 * Offset-zero CBody prefix recovered for naturally spelled physics methods.
 */
struct CBodyPhysicsNatural : CItemPhysicsNatural {
    CBodyPhysicsNatural **mppOriginalList;
    u16 mInputFlags;
    u16 mCBodyFlags;
    CVector mVel;
    CVector mAcc;
    CFriction mFric;
    CSVector mAngVel;
    CSVector mAngAcc;
    CFriction mAngFric;
    CSVector mNormal;
    u8 mEventFlags;
    u32 mEllipsoidRGB;
    u16 mEllipsoidScale;
    s16 mEllipsoidOTPushback;
    CVector mShadowPos;
    CSVector mShadowNormal;
    void *mpShadow;
    u16 mShadowScale;
    s16 mShadowDist;
    s16 mShadowThreshold;
    int mPushVal;
    u16 mRMinor;
    u16 mNode;
    u16 mCollision;
    s16 mHealth;
    u32 mPlayerDist;
    u8 mAIFrequency;
    u8 mAICount;
    CVector mOldPos;
    CVector mTweenPos;
    CVector mNewPos;
    CVector mDPos;
    CSVector mOldAngles;
    CSVector mNewAngles;
    CSVector mDAngles;
};

/**
 * Offset-zero CSuper prefix recovered for naturally spelled physics methods.
 */
struct CSuperPhysicsNatural : CBodyPhysicsNatural {
    u32 mExtraFlags;
    u16 *mpCalculationOrder;
    void *mpDecompressedFrame;
    u16 mRoot;
    u8 mDecompressedAnim;
    char mDecompressedFrame;
    s16 mFrame;
    u16 mAnim;
    u8 mAnimMode;
    s16 mPingPongStart;
    s16 mPingPongEnd;
    s16 mPingPongFrame;
    signed char mAnimDir;
    char mTargetFrame;
    char mTargetFrame2;
    u16 mFrameFrac;
    u8 mNumFrames;
    u8 mAnimFinished;
    int mAnimSpeed;
    s16 mOldFrame;
    s16 mNewFrame;
    u8 mOldAnim;
    char mOldAnimDir;
    s16 mStartFrame;
    u8 mShadowLights;
    SVECTOR *mpShadowSource;
    CVECTOR *mpShadowRGB;
    MATRIX mTransform;
    void *mpPoseBuffer;
    void *mpJoints;
    void *mpLinks;
    short *mpHierarchy;
    u32 mSubObjectDisplayMask;
    u32 mUseAnimOrder;
    u8 mAnimOrder[19];
};

/**
 * Natural CBruce spelling for methods that use the proven inheritance prefix.
 * The remaining object is kept opaque and accessed through owner-local slices.
 */
struct CBruce : CSuperPhysicsNatural {
    u8 pad_to_mEventQueue[0xAB4 - sizeof(CSuperPhysicsNatural)];
    SButtonEvent mEventQueue[32];
    int mEventQueuePos;
    u8 pad_to_mSkaterType[0xDA8 - 0xBB8];
    int mSkaterType;
    u8 pad_to_mInBail[0xF7C - 0xDAC];
    int mInBail;
    u8 pad_to_mMagicTime[0xF88 - 0xF80];
    int mMagicTime;
    u8 pad_to_mPhysicsState[0x106C - 0xF8C];
    int mPhysicsState;
    u8 mOpaqueTail[5228 - 0x1070];

    /** Initialize panel state for this player. */
    void Trick_PlayerInit(unsigned short playerNumber);
    /** Start a new airborne trick sequence. */
    void Trick_TakeOff();
    /** Clears accumulated trick state after a bail. */
    void Trick_Bail();
    /** Adds one named trick to the active scoring sequence. */
    void Trick_Trick(char *name, int points, int lock);
    /** Clears the buffered tap-turn state. */
    void ResetTrickBuffers();
    /** End an active manual and score its landing. */
    void EndManual();
    /** Count completed half-turns for the active trick. */
    int Get180s();
    /** Finalize the current trick sequence after landing. */
    void Trick_Land();
    void Bail(int bailNumber, ELimitBailVel limitVelocity);
    int GetPreviousPressedEvent(int event, int skip, int time, int top);
    int GetDirection();
    void ButtonRemoveLast(unsigned char button, unsigned char pressed);
    void QueueTrick(short *trick, int button, int eventTime);
    void MaybeTriggerTrick();
    /** Apply bail-state velocity constraints and heavy friction. */
    void HandleBailStates();
    /** Update the skater while a hand-plant animation is active. */
    void DoHandPlantPhysics();
    /** Apply grounded movement, collision, and friction behavior. */
    void DoOnGroundPhysics();
    /** Advance the skater along the active grind rail. */
    void DoOnRailPhysics();
    /** Apply kick input and the kick-animation impulse. */
    void HandleKick();
    /** Process jump charging, release, and airborne launch behavior. */
    void HandleJump();
    /** Continue scoring a held trick. */
    void Trick_HoldTrick(int points);
};

typedef char CItemPhysicsNatural_size_must_be_76[
    (sizeof(CItemPhysicsNatural) == 76) ? 1 : -1];
typedef char CBodyPhysicsNatural_size_must_be_264[
    (sizeof(CBodyPhysicsNatural) == 264) ? 1 : -1];
typedef char CSuperPhysicsNatural_size_must_be_400[
    (sizeof(CSuperPhysicsNatural) == 400) ? 1 : -1];
typedef char CSuperPhysicsNatural_shadow_source_offset_must_be_316[
    ((unsigned long)&(((CSuperPhysicsNatural *)0)->mpShadowSource) == 316)
        ? 1 : -1];
typedef char CSuperPhysicsNatural_shadow_rgb_offset_must_be_320[
    ((unsigned long)&(((CSuperPhysicsNatural *)0)->mpShadowRGB) == 320)
        ? 1 : -1];
typedef char CSuperPhysicsNatural_transform_offset_must_be_324[
    ((unsigned long)&(((CSuperPhysicsNatural *)0)->mTransform) == 324) ? 1 : -1];
typedef char CSuperPhysicsNatural_pose_buffer_offset_must_be_356[
    ((unsigned long)&(((CSuperPhysicsNatural *)0)->mpPoseBuffer) == 356) ? 1 : -1];
typedef char CBruceNatural_size_must_be_5228[
    (sizeof(CBruce) == 5228) ? 1 : -1];
typedef char CBruceNatural_event_queue_offset_must_be_0xAB4[
    ((unsigned long)&(((CBruce *)0)->mEventQueue) == 0xAB4) ? 1 : -1];

typedef char SDK_VECTOR_size_must_be_16[
    (sizeof(VECTOR) == 16) ? 1 : -1];
typedef char SDK_SVECTOR_size_must_be_8[
    (sizeof(SVECTOR) == 8) ? 1 : -1];
typedef char SDK_CVECTOR_size_must_be_4[
    (sizeof(CVECTOR) == 4) ? 1 : -1];
typedef char SDK_MATRIX_size_must_be_32[
    (sizeof(MATRIX) == 32) ? 1 : -1];
typedef char MATRIX_translation_offset_must_be_20[
    ((unsigned long)&(((MATRIX *)0)->t) == 20) ? 1 : -1];
typedef char SButtonEvent_size_must_be_8[
    (sizeof(SButtonEvent) == 8) ? 1 : -1];
typedef char SButtonEvent_Button_offset_must_be_0[
    ((unsigned long)&(((SButtonEvent *)0)->Button) == 0) ? 1 : -1];
typedef char SButtonEvent_Pressed_offset_must_be_1[
    ((unsigned long)&(((SButtonEvent *)0)->Pressed) == 1) ? 1 : -1];
typedef char SButtonEvent_Time_offset_must_be_4[
    ((unsigned long)&(((SButtonEvent *)0)->Time) == 4) ? 1 : -1];
typedef char CBruceTrickTriggerSlice_size_must_be_0x1070[
    (sizeof(CBruceTrickTriggerSlice) == 0x1070) ? 1 : -1];
typedef char CBruceTrickTriggerSlice_event_queue_offset_must_be_0xAB4[
    ((unsigned long)&(((CBruceTrickTriggerSlice *)0)->mEventQueue) == 0xAB4)
        ? 1 : -1];
typedef char STrickQueue_size_must_be_16[
    (sizeof(STrickQueue) == 16) ? 1 : -1];
typedef char STrickQueue_pTrick_offset_must_be_0[
    ((unsigned long)&(((STrickQueue *)0)->pTrick) == 0) ? 1 : -1];
typedef char STrickQueue_Time_offset_must_be_4[
    ((unsigned long)&(((STrickQueue *)0)->Time) == 4) ? 1 : -1];
typedef char STrickQueue_Button_offset_must_be_8[
    ((unsigned long)&(((STrickQueue *)0)->Button) == 8) ? 1 : -1];
typedef char STrickQueue_EventTime_offset_must_be_12[
    ((unsigned long)&(((STrickQueue *)0)->EventTime) == 12) ? 1 : -1];
typedef char CSVectorPhysicsOwner_size_must_be_6[
    (sizeof(CSVectorPhysicsOwner) == 6) ? 1 : -1];
typedef char CVectorPhysicsOwner_size_must_be_12[
    (sizeof(CVectorPhysicsOwner) == 12) ? 1 : -1];
typedef char CTrick_size_must_be_36[
    (sizeof(CTrick) == 36) ? 1 : -1];
typedef char CPanelData_size_must_be_2276[
    (sizeof(CPanelData) == 2276) ? 1 : -1];
typedef char SCareerOptionsPhysicsOwner_prefix_size_check[
    (sizeof(SCareerOptionsPhysicsOwner) == 0x8E8) ? 1 : -1];
typedef char SCareerOptionsPhysicsOwner_vibration_at_0x8E0[
    ((unsigned long)&(((SCareerOptionsPhysicsOwner *)0)->VibrationEnabled)
     == 0x8E0) ? 1 : -1];
typedef char CBruceScriptQueueSlice_size_must_be_3208[
    (sizeof(CBruceScriptQueueSlice) == 3208) ? 1 : -1];
typedef char CBruceScriptQueueSlice_event_queue_offset_must_be_0xAB4[
    ((unsigned long)&(((CBruceScriptQueueSlice *)0)->mEventQueue) == 0xAB4)
        ? 1 : -1];
typedef char CBruceScriptQueueSlice_trick_queue_offset_must_be_0xBD8[
    ((unsigned long)&(((CBruceScriptQueueSlice *)0)->mTrickQueue) == 0xBD8)
        ? 1 : -1];
typedef char CBruceScriptQueueSlice_pressed_at_0xAB5[
    ((unsigned long)&(((CBruceScriptQueueSlice *)0)->mEventQueue[0].Pressed)
     == 0xAB5) ? 1 : -1];
typedef char CBrucePhysicsSlice_size_must_be_5228[
    (sizeof(CBrucePhysicsSlice) == 5228) ? 1 : -1];
typedef char CBrucePhysicsSlice_event_queue_offset_must_be_0xAB4[
    ((unsigned long)&(((CBrucePhysicsSlice *)0)->mEventQueue) == 0xAB4)
        ? 1 : -1];
typedef char CBrucePhysicsSlice_trick_queue_offset_must_be_0xBD8[
    ((unsigned long)&(((CBrucePhysicsSlice *)0)->mTrickQueue) == 0xBD8)
        ? 1 : -1];
typedef char CBrucePhysicsSlice_history_pos_offset_must_be_0x1148[
    ((unsigned long)&(((CBrucePhysicsSlice *)0)->mHistoryPos) == 0x1148)
        ? 1 : -1];
typedef char CBrucePhysicsSlice_old_rotation_offset_must_be_0xe70[
    ((unsigned long)&(((CBrucePhysicsSlice *)0)->mOldRotationMatrix) == 0xE70)
        ? 1 : -1];
typedef char CBrucePhysicsSlice_rotation_offset_must_be_0xe90[
    ((unsigned long)&(((CBrucePhysicsSlice *)0)->mRotationMatrix) == 0xE90)
        ? 1 : -1];
typedef char CBrucePhysicsSlice_temp_rotation_offset_must_be_0x1074[
    ((unsigned long)&(((CBrucePhysicsSlice *)0)->mTempRotationMatrix) == 0x1074)
        ? 1 : -1];
typedef char CBrucePhysicsSlice_panel_at_0x190[
    ((unsigned long)&(((CBrucePhysicsSlice *)0)->mPanelData) == 0x190) ? 1 : -1];
typedef char CBrucePhysicsSlice_rail_flags_at_0xC88[
    ((unsigned long)&(((CBrucePhysicsSlice *)0)->mRail_Parallel) == 0xC88) ? 1 : -1];
typedef char CBrucePhysicsSlice_physics_state_at_0x106C[
    ((unsigned long)&(((CBrucePhysicsSlice *)0)->mPhysicsState) == 0x106C) ? 1 : -1];
typedef char CBrucePhysicsSlice_history_at_0x114C[
    ((unsigned long)&(((CBrucePhysicsSlice *)0)->mHistory) == 0x114C) ? 1 : -1];

#include "BIT.h"
#include "CROWD.h"
#include "FILEIO.h"
#include "RAIL.h"
#include "TRIG.h"
#include "CSUPER.h"

// Narrow typed dependencies used only by the PHYSICS owner.
struct CBruce;
/** Narrow natural interface for the embedded blood manager. */
struct Blood {
    void Reset();
    void Update(CBruce *bruce);
};
struct CBrucePhysicsSlice;
struct CBody;
struct CPanelData;
struct CSpark;
struct CSuperPhysicsNatural;
struct CSVector;
struct CSVectorPhysicsOwner;
struct CVector;
/** Narrow natural interface for the embedded dust manager. */
struct Dust {
    void Reset(int terrainType, int dustType);
    void Update(CBruce *bruce);
};
struct Item;

/** Lowered aggregate-return ABI for unary CVector negation. */
extern "C" void Physics_VectorNegate(
    CVector *result, const CVector *vector) asm("__mi__C7CVector");
/** Lowered aggregate-return ABI for fixed-point CVector scaling. */
extern "C" void Physics_VectorMultiply(
    CVector *result, const CVector *vector,
    const int *scale) asm("__ml__FRC7CVectorRCi");
#include "M3DMATHS.h"

/** Orient the skater to its current velocity. */
extern "C" int Physics_FlipToVelocity(CBrucePhysicsSlice *bruce)
    asm("FlipToVelocity__6CBruce");
/** Reset the skater's input history. */
extern "C" void Physics_HistoryReset(CBrucePhysicsSlice *bruce)
    asm("History_Reset__6CBruce");
/** Enable or disable the skater board. */
extern "C" void Physics_BoardOn(CBrucePhysicsSlice *bruce)
    asm("BoardOn__6CBruce");
extern "C" void Physics_BoardOff(CBrucePhysicsSlice *bruce)
    asm("BoardOff__6CBruce");
/** Spawn skater blood at the requested frame and amount. */
extern "C" void Physics_BloodSplat(
    CBrucePhysicsSlice *bruce, int frame, int amount)
    asm("BloodSplat__6CBruceii");
/** Panel method retained as an ABI bridge until its natural member spelling
 * reproduces the target register allocation. */
extern "C" void Physics_PanelRefreshTrickMessage(CPanelData *panel)
    asm("TrickMessageRefresh__10CPanelData");

/** Apply dynamic lighting from one collision face. */
extern "C" void Physics_CalculateDynamicLighting(
    CBrucePhysicsSlice *bruce, unsigned int *face, Item *item)
    asm("CalculateDynamicLighting__6CBrucePUiP4Item");
/** Construct one ordinary terrain spark. */
extern "C" CSpark *Physics_ConstructSpark(
    CSpark *spark, CVector &position, int size, int speed, int life)
    asm("__6CSparkR7CVectoriii");

#include "SFX.h"
#include "MEM.h"
#include "PLATFORM.h"
#include "TRICKOB.h"
#include "UTILS.h"

// === Free function declarations ===
// PHYSICS uses owner-local layout slices rather than the incomplete generated
// repo-wide aggregate layouts.
struct Blood;
void GetFaceFlags(SLineInfo *line_info);
int PingPong(int a, int b, int fps);
void ProjectCVectorToPlane(CVector *vec, SVECTOR normal);
void CVectorRotateToPlane(CVector *InVector, SVECTOR *Normal, CVector *OutVector);
CVector operator*(const CVector &vector, const int &scale);
CVector operator+(const CVector &left, const CVector &right);
CVector operator-(const CVector &left, const CVector &right);
CVector operator/(const CVector &vector, const int &scale);
CVector operator>>(const CVector &vector, const int &shift);
void UberFrig(CBruce *pSkater, CVector Pos, char *text);
short *Trick_Skip(short *pTrick);
int EnsureGTricksLoaded(void);
void UnloadGTricks(void);
void ExtraAnims_Reset(void);
void ExtraAnims_Add(short Anim);
void ExtraAnims_AddTrick(short *pTrick);
void ExtraAnims_AddTrickTable(short *pKeys);
void ExtraAnims_AddSkatersAnims(void);
int InterpolateFrame(int current, int target);
int arccos(int a);

// === External function declarations ===
#include <STDIO.H>
struct CVector;
struct Blood;
struct Dust;
struct CBruce;
struct CPanelData;

// === Forward declarations for the file-local stat/absolute helpers ===
//
// These are defined `static inline` further down, but several call sites sit
// above their definitions.  Without these declarations cc1plus implicitly
// declares each name as an external function and emits a real `jal` to an
// unmangled symbol that exists in neither this object nor the target, which
// both breaks the native PSYLINK link and prevents the call site from ever
// matching the original's inlined code.
static inline int PhysicsAbsolute(int value);
static inline int PhysicsShortAbsolute(unsigned short value);
static inline int PhysicsMaxSpeed(CBrucePhysicsSlice *bruce);

// $gp-relative globals (size <= 8 bytes, -G8)

// === Absolute-addressed globals (use lui+lw, not $gp-relative) ===
#define BITSTATE_DECLARE_BIT_COUNT
#include "BITSTATE.h"
// main.obj owns int GGame at 0x800D95DC; no GGame_abs alias is needed in PHYSICS.
// main.obj owns int TimeScale at 0x800D97BC; no TimeScale_physics alias is needed in PHYSICS.
#define DEFAULTANIMATIONS_SIZED_DECLARATION
#include "DEFAULTANIMATIONS.h"
/** Default animation IDs followed by the writable extra-animation workspace. */
s16 DefaultAnimations[362] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    12, 13, 14, 16, 17, 25, 26, 27, 33, 34, 35, 36,
    37, 38, 39, 40, 41, 42, 44, 45, 46, 47, 48, 49,
    54, 55, 56, 57, 58, 65, 66, 67, 68, 69, 70, 71,
    72, 99, 100, 101, 102, 103, 104, 123, 124, 125, 126, 128,
    129, 132, 133, 130, 131, 135, 136, 137, 69, 145, -1, -1,
};

/**
 * Maps the four digital/analog direction bits to the skater direction enum.
 */
extern "C" int DirectionTable[16];
static int PhysicsDirectionTableOwner[16]
    asm("DirectionTable")
    __attribute__((section(".data"))) = {
    0, 1, 2, 2,
    3, 5, 7, 7,
    4, 6, 8, 8,
    4, 6, 8, 8,
};

extern "C" {
/** Physics stat override, shared up axis, and active control scheme. */
int Cheat_StatsOverride __attribute__((section(".sdata"))) = 0;
CVector WorldUp __attribute__((section(".sdata"))) = {0, 0, 0};
int SelectedControlScheme __attribute__((section(".sdata"))) = 1;
}

/**
 * Owner-local construction type for the global gravity vector.  The original
 * CVector constructor stores the three supplied components during static
 * initialization; keeping that behavior here reproduces the compiler-emitted
 * initializer without hand-writing it as a function.
 */
struct PhysicsWorldGravity : CVector
{
    PhysicsWorldGravity(int x, int y, int z)
    {
        vx = x;
        vy = y;
        vz = z;
    }
};

extern "C" {
PhysicsWorldGravity WorldGravity(0, 0x8000, 0);
}

/** Compact transition-trick names stored ahead of the loadable trick table. */
struct PhysicsTransitionNames
{
    char Ollie[8];
    char WallRide[12];
    char Handplant[12];
    char Trick[8];
};
typedef char PhysicsTransitionNames_size_must_be_40[
    (sizeof(PhysicsTransitionNames) == 40) ? 1 : -1];

extern "C" {
/** Physics normal and compact trick-name table. */
CVector UpNormal __attribute__((section(".sdata"))) = {0, 0, 0};
static PhysicsTransitionNames PhysicsTrickNames
    __attribute__((section(".sdata"))) = {
        "OLLIE", "WALL RIDE", "Handplant", "Trick"
    };

/** Writable extra-animation cursor. */
short *pExtraAnim __attribute__((section(".sdata"))) = 0;

/**
 * Filename followed by the fixed trick-name records addressed from it.
 * The 76-byte extent is target-proven and keeps accesses absolute under
 * PHYSICS's recovered -G16 threshold.
 */
static char GTricksFilename[76]
    __attribute__((section(".sdata"))) =
    "tricks.bin\0\0"
    "FOOTPLANT\0\0\0"
    "MANUAL\0\0"
    "NOSE MANUAL\0"
    "FASTPLANT\0\0\0"
    "BONELESS\0\0\0\0"
    "NOLLIE\0";

/** One-time trigger-probe initialization flag. */
static int PhysicsTriggerInitialized
    __attribute__((section(".sdata"))) = 0;

/** Player-one multiplayer wins, losses, streak, and high-tag record. */
int GWins1 __attribute__((section(".sdata"))) = 0;
int GLosses1 __attribute__((section(".sdata"))) = 0;
int GStreak1 __attribute__((section(".sdata"))) = 0;
int GHighTags1 __attribute__((section(".sdata"))) = 0;

/** Player-two multiplayer wins, losses, streak, and high-tag record. */
int GWins2 __attribute__((section(".sdata"))) = 0;
int GLosses2 __attribute__((section(".sdata"))) = 0;
int GStreak2 __attribute__((section(".sdata"))) = 0;
int GHighTags2 __attribute__((section(".sdata"))) = 0;

/** Collision-face masks and active extra-animation count. */
int Face_Air __attribute__((section(".sdata"))) = 0;
int Face_Skateable __attribute__((section(".sdata"))) = 0;
int Face_WallRide __attribute__((section(".sdata"))) = 0;
int Face_Visible __attribute__((section(".sdata"))) = 0;
int Face_Terrain __attribute__((section(".sdata"))) = 0;
int NumExtraAnims __attribute__((section(".sdata"))) = 0;
}

extern "C" int last_valid_401_gp asm("last_valid.401");
#include "ZEROVECTOR.h"
struct Vec3 { int a, b, c; };
// === Readable owner-local names with MAIN.SYM-proven ABI labels ===
extern "C" void Physics_TempTransform(CBrucePhysicsSlice *)
    asm("TempTransform__6CBruce");
extern "C" void Physics_TempRestore(CBrucePhysicsSlice *)
    asm("TempRestore__6CBruce");
extern "C" void Physics_Vibrate(CBrucePhysicsSlice *, int, int, int)
    asm("Vibrate__6CBruceiii");
extern "C" void Physics_StateTransition(CBrucePhysicsSlice *, int, int)
    asm("StateTransition__6CBruce13EPhysicsStatei");
extern "C" void Physics_MaybeFlipAnimationTransition(CBrucePhysicsSlice *)
    asm("MaybeFlipAnimationTransition__6CBruce");
extern "C" void Physics_DebugRunAnim(CBrucePhysicsSlice *, int, int)
    asm("Debug_RunAnim__6CBruceii");
extern "C" void Physics_DebugRunAnim1(CBrucePhysicsSlice *, int, int, int)
    asm("Debug_RunAnim1__6CBruceiii");
extern "C" void Physics_DebugRunAnim2(CBrucePhysicsSlice *, int, int, int, int)
    asm("Debug_RunAnim2__6CBruceiiii");
extern "C" void Physics_DebugRunAnim3(
    CBrucePhysicsSlice *, int, int, int, int, int)
    asm("Debug_RunAnim3__6CBruceiiiii");
extern "C" void Physics_Bail(
    CBrucePhysicsSlice *, int, ELimitBailVel)
    asm("Bail__6CBrucei13ELimitBailVel");
extern "C" void Physics_FrigDarkenObject(CBrucePhysicsSlice *)
    asm("FrigDarkenObject__6CBruce");
extern "C" void Physics_TrickObRail(CBrucePhysicsSlice *)
    asm("TrickObRail__6CBruce");
extern "C" void Physics_StartHandPlant(CBrucePhysicsSlice *)
    asm("StartHandPlant__6CBruce");
extern "C" void Physics_StartGrind(CBrucePhysicsSlice *)
    asm("StartGrind__6CBruce");
extern "C" void Physics_HandleStickToRail(CBrucePhysicsSlice *)
    asm("HandleStickToRail__6CBruce");
extern "C" void Physics_HandleBailOnLanding(CBrucePhysicsSlice *, int)
    asm("HandleBailOnLanding__6CBrucei");
extern "C" void Physics_HandleBailPerpendicular(
    CBrucePhysicsSlice *, SVECTOR)
    asm("HandleBailPerpendicular__6CBruceG7SVECTOR");
extern "C" void Physics_TriggerTrickScript(CBrucePhysicsSlice *, short *)
    asm("TriggerTrick__6CBrucePs");
extern "C" void Physics_TriggerTrickIndex(CBrucePhysicsSlice *, int)
    asm("TriggerTrick__6CBrucei");
extern "C" void Physics_ButtonRecord(
    CBrucePhysicsSlice *, unsigned char, unsigned char)
    asm("ButtonRecord__6CBruceUcUc");
extern "C" void Physics_ButtonRemoveLast(
    CBrucePhysicsSlice *, unsigned char, unsigned char)
    asm("ButtonRemoveLast__6CBruceUcUc");
extern "C" int Physics_GetDirection(CBrucePhysicsSlice *)
    asm("GetDirection__6CBruce");
extern "C" void Physics_UpdateButtons(CBrucePhysicsSlice *)
    asm("UpdateButtons__6CBruce");
extern "C" void Physics_QueueTrick(CBrucePhysicsSlice *, short *, int, int)
    asm("QueueTrick__6CBrucePsii");
extern "C" void Physics_UpdateQueue(CBrucePhysicsSlice *)
    asm("UpdateQueue__6CBruce");
extern "C" int Physics_ActionButtonHeld(CBrucePhysicsSlice *)
    asm("ActionButtonHeld__6CBruce");
extern "C" int Physics_GetPreviousPressedEvent(
    CBrucePhysicsSlice *, int, int, int, int)
    asm("GetPreviousPressedEvent__6CBruceiiii");
extern "C" short Physics_GetScriptFrame(CBrucePhysicsSlice *, int)
    asm("GetScriptFrame__6CBrucei");
extern "C" void Physics_UpdateScript(CBrucePhysicsSlice *)
    asm("UpdateScript__6CBruce");
extern "C" void Physics_HandleTricks(CBrucePhysicsSlice *)
    asm("HandleTricksNew__6CBruce");
extern "C" void Physics_CalcAnimLeanFrame(CBrucePhysicsSlice *)
    asm("CalcAnimLeanFrame__6CBruce");
extern "C" void Physics_HandleSpin(CBrucePhysicsSlice *)
    asm("HandleSpin__6CBruce");
extern "C" void Physics_AlignHeadingOnRail(CBrucePhysicsSlice *)
    asm("AlignHeadingOnRail__6CBruce");
extern "C" void Physics_CreateRailSparks(CBrucePhysicsSlice *)
    asm("CreateRailSparks__6CBruce");
extern "C" void Physics_TweakRailScore(CBrucePhysicsSlice *)
    asm("TweakRailScore__6CBruce");
extern "C" void Physics_HandleGrindOffSound(CBrucePhysicsSlice *)
    asm("HandleGrindOffSound__6CBruce");
extern "C" void Physics_LeaveRail(CBrucePhysicsSlice *)
    asm("LeaveRail__6CBruce");
extern "C" void Physics_SkateOffGround(CBrucePhysicsSlice *)
    asm("SkateOffGround__6CBruce");
extern "C" int Physics_CheckSide(CBrucePhysicsSlice *, int, int, int)
    asm("CheckSide__6CBruceiii");
extern "C" void Physics_DoTerrainEffects(CBrucePhysicsSlice *)
    asm("DoTerrainEffects__6CBruce");
extern "C" void Physics_DoManual(CBrucePhysicsSlice *)
    asm("DoManualPhysics__6CBruce");
extern "C" void Physics_MoveOriginTo(CBrucePhysicsSlice *, int, int, int)
    asm("MoveOriginTo__6CBruceiii");
extern "C" void Physics_HandleTruckSqueak(CBrucePhysicsSlice *)
    asm("HandleTruckSqueakSound__6CBruce");
extern "C" void Physics_HandleLeaningBail(CBrucePhysicsSlice *, SVECTOR)
    asm("HandleLeaningBail__6CBruceG7SVECTOR");
extern "C" void Physics_HitAirPoly(CBrucePhysicsSlice *, SLineInfo *)
    asm("HitAirPoly__6CBruceR9SLineInfo");
extern "C" void Physics_MaybeManual(CBrucePhysicsSlice *, int, int)
    asm("MaybeManual__6CBruceii");
extern "C" void Physics_DoInAir(CBrucePhysicsSlice *)
    asm("DoInAirPhysics__6CBruce");
extern "C" void Physics_DoFootPlant(CBrucePhysicsSlice *)
    asm("DoFootPlantPhysics__6CBruce");
extern "C" void Physics_DoWallRide(CBrucePhysicsSlice *)
    asm("DoWallRidePhysics__6CBruce");
extern "C" void Physics_RotateHeadingAndVelocity(
    CBrucePhysicsSlice *, int, int, int)
    asm("RotateHeadingAndVelocity__6CBruceiii");
extern "C" void Physics_ReflectHeadingVelocityInPlane(
    CBrucePhysicsSlice *, SVECTOR, int)
    asm("ReflectHeadingVelocityInPlane__6CBruceG7SVECTORi");
extern "C" void Physics_AlignToFallLine(CBrucePhysicsSlice *, int)
    asm("AlignToFallLine__6CBrucei");
extern "C" void Physics_UprightSideways(CBrucePhysicsSlice *)
    asm("UprightSideways__6CBruce");
extern "C" void Physics_FRUVectorsFromRotMatrix(CBrucePhysicsSlice *)
    asm("FRUVectorsFromRotMatrix__6CBruce");
extern "C" void Physics_RotMatrixFromFRUVectors(CBrucePhysicsSlice *)
    asm("RotMatrixFromFRUVectors__6CBruce");
extern "C" void Physics_SetAnglesFromHeading(CBrucePhysicsSlice *)
    asm("SetAnglesFromHeading__6CBruce");
extern "C" void Physics_HandleContinuousSounds(CBrucePhysicsSlice *)
    asm("HandleContinuousSounds__6CBruce");
extern "C" void Physics_OrientToNormal(CBrucePhysicsSlice *)
    asm("OrientToNormal__6CBruce");
extern "C" void Physics_HandlePlayerCollisions(CBrucePhysicsSlice *)
    asm("HandlePlayerCollisions__6CBruce");
extern "C" void Physics_LimitFinalSpeed(CBrucePhysicsSlice *)
    asm("LimitFinalSpeed__6CBruce");
extern "C" void Physics_CheckForTriggers(CBrucePhysicsSlice *)
    asm("CheckForTriggers__6CBruce");
extern "C" void Physics_CheckSideCollisions(CBrucePhysicsSlice *)
    asm("CheckSideCollisions__6CBruce");
extern "C" void Physics_HandlePhysicsStates(CBrucePhysicsSlice *)
    asm("HandlePhysicsStates__6CBruce");
extern "C" void Physics_HandleBraking(CBrucePhysicsSlice *)
    asm("HandleBraking__6CBruce");
extern "C" void Physics_HandleFlips(CBrucePhysicsSlice *)
    asm("HandleFlips__6CBruce");
extern "C" void Physics_HandleTimer(CBrucePhysicsSlice *)
    asm("HandleTimer__6CBruce");
extern "C" void Physics_DoPhysics(CBrucePhysicsSlice *)
    asm("DoPhysics__6CBruce");

/**
 * PHYSICS.CPP diagnostic strings in their original owner-local order.
 *
 * The original object keeps the complete diagnostic pool even when a build
 * configuration removes the corresponding debug calls.  Fixed field widths
 * preserve the original padding and keep the pool indivisible.
 */
struct PhysicsDiagnostics {
    char collision[0x1C];
    char script[0x1C];
    char scriptError[0x14];
    char loadTricks[0x14];
    char unloadTricks[0x14];
    char extraAnimations[0x20];
    char manualAnimation[0x2C];
    char newBail[0x28];
    char loopRestart[0x28];
};

typedef char PhysicsDiagnostics_size_must_be_0x110[
    (sizeof(PhysicsDiagnostics) == 0x110) ? 1 : -1];

extern "C" const PhysicsDiagnostics PhysicsDiagnosticPool = {
    "%d: Collision (%d,%d,%d)\n",
    "%d %d %d %d (%d) %d %d %d\n",
    "Script Error %d\n",
    "Loading GTricks\n",
    "UnLoading GTricks\n",
    "Adding Extra animations......\n",
    "Need manual anim!!!!!!!!!!!!!!!!!!!!!!!\n",
    "...............New Bail..............\n",
    "restart at start of main physics loop",
};

// === Function implementations (address order) ===

/**
 * GetFaceFlags(SLineInfo *) — extract collision flags from face data
 *
 * Address: 0x80010000
 * Mangled: GetFaceFlags__FP9SLineInfo
 *
 * @param line_info  Collision result whose face data supplies the flags.
 */
void GetFaceFlags(SLineInfo *line_info)
{
    int face = (int)line_info->pItem;
    if (face != 0)
    {
        int *faceData = (int *)line_info->pFace;
        int header = faceData[3]; // offset 0xC
        int upper = (u32)header >> 16;
        int transp = upper & 0x40;
        int notBit24 = (((u32)header >> 24) ^ 1) & 1;
        int notBit23 = (((u32)header >> 23) ^ 1) & 1;
        int terrain = upper & 0x1E00;
        Face_Air = transp;
        Face_Skateable = notBit24;
        Face_WallRide = notBit23;
        int firstWord = faceData[0];
        Face_Terrain = (u32)terrain >> 9;
        Face_Visible = firstWord & 0x80;
    }
}

/**
 * Ping-pong a frame counter across a range at the requested rate
 *
 * Address: 0x8001006C
 * Mangled: PingPong__Fiii
 *
 * @param a  First frame in the range.
 * @param b  Last frame in the range.
 * @param fps  Playback rate used to advance the range.
 * @return The current frame in the ping-pong cycle.
 */
int PingPong(int a, int b, int fps)
{
    int period = 60 / fps;
    int x = XFrames;
    int cycle = x / period;
    int dif = b - a;
    int doubleRange = dif << 1;
    int pos = cycle % doubleRange;
    if (dif < pos)
    {
        pos = doubleRange - pos;
    }
    return a + pos;
}

/**
 * CBruce::TempTransform(void) — save and temporarily adjust the skater transform
 *
 * Address:   0x800100B8
 * Mangled:   TempTransform__6CBruce
 * Demangled: CBruce::TempTransform(void)
 *
 * Saves the current position and rotation matrix, applies pending flip/spin or
 * rail-lean rotations around the skater, then rebuilds the world transform.
 *
 * @param bruce  Owner-local view of the implicit `this` pointer.
 */
void Physics_TempTransform(CBrucePhysicsSlice *bruce)
{
    MATRIX SurfaceRotMatrix;
    SVECTOR RotAngle;

    bruce->mTempRotationMatrix = bruce->mRotationMatrix;
    bruce->mTempPos = bruce->CSuper.CBody.CItem.mPos;

    if (bruce->mFlipNum || bruce->mRotateTotal[0] || bruce->mRotateTotal[2])
    {
        VECTOR zero;
        VECTOR v;
        VECTOR v2;
        VECTOR v3;

        zero.vz = 0;
        zero.vy = 0;
        zero.vx = 0;
        TransMatrix(&bruce->mRotationMatrix, &zero);
        TransMatrix(&SurfaceRotMatrix, &zero);

        v.vx = 0;
        v.vy = 70 << 12;
        v.vz = 0;
        ApplyMatrixLV(&bruce->mRotationMatrix, &v, &v2);

        RotAngle.vz = 0;
        RotAngle.vy = 0;
        RotAngle.vx = 0;
        if (bruce->mRotateCount[0])
            RotAngle.vx = bruce->mRotateTotal[0] & 0xFFF;
        if (bruce->mRotateCount[2])
            RotAngle.vz = bruce->mRotateTotal[2] & 0xFFF;
        if (bruce->mFlipNum)
            RotAngle.vx = bruce->mFlipAngle & 0xFFF;

        RotMatrixYXZ(&RotAngle, &SurfaceRotMatrix);
        MulMatrix(&bruce->mRotationMatrix, &SurfaceRotMatrix);
        ApplyMatrixLV(&bruce->mRotationMatrix, &v, &v3);

        bruce->CSuper.CBody.CItem.mPos.vx = bruce->CSuper.CBody.CItem.mPos.vx + v2.vx - v3.vx;
        bruce->CSuper.CBody.CItem.mPos.vy = bruce->CSuper.CBody.CItem.mPos.vy + v2.vy - v3.vy;
        bruce->CSuper.CBody.CItem.mPos.vz = bruce->CSuper.CBody.CItem.mPos.vz + v2.vz - v3.vz;
    }

    if (bruce->mManualLean)
    {
        MATRIX Rail_RotMatrix;
        SVECTOR RotAngle;

        RotAngle.vy = 0;
        RotAngle.vz = 0;
        RotAngle.vx = (bruce->mManualLean / 10) & 0xFFF;
        RotMatrixYXZ(&RotAngle, &Rail_RotMatrix);
        MulMatrix(&bruce->mRotationMatrix, &Rail_RotMatrix);
        Physics_FRUVectorsFromRotMatrix(bruce);
    }

    // CBruce begins with the target-proven inherited CSuper object.
    M3d_BuildTransform(reinterpret_cast<CSuper *>(bruce));
}

/* Preserve the target's explicit eight-word copy, including MATRIX padding. */
#define PHYSICS_RAW_MATRIX_WORD(matrix, index) \
    (reinterpret_cast<volatile int *>(&(matrix))[(index)])

/**
 * CBruce::TempRestore(void) — restore saved transform data
 *
 * Address: 0x800102F4
 * Mangled: TempRestore__6CBruce
 * @param bruce  Owner-local view of the implicit `this` pointer.
 */
void Physics_TempRestore(CBrucePhysicsSlice *bruce)
{
    int a;
    int b;
    int c;
    int d;
    a = PHYSICS_RAW_MATRIX_WORD(bruce->mTempRotationMatrix, 0);
    b = PHYSICS_RAW_MATRIX_WORD(bruce->mTempRotationMatrix, 1);
    c = PHYSICS_RAW_MATRIX_WORD(bruce->mTempRotationMatrix, 2);
    d = PHYSICS_RAW_MATRIX_WORD(bruce->mTempRotationMatrix, 3);
    PHYSICS_RAW_MATRIX_WORD(bruce->mRotationMatrix, 0) = a;
    PHYSICS_RAW_MATRIX_WORD(bruce->mRotationMatrix, 1) = b;
    PHYSICS_RAW_MATRIX_WORD(bruce->mRotationMatrix, 2) = c;
    PHYSICS_RAW_MATRIX_WORD(bruce->mRotationMatrix, 3) = d;
    a = PHYSICS_RAW_MATRIX_WORD(bruce->mTempRotationMatrix, 4);
    b = PHYSICS_RAW_MATRIX_WORD(bruce->mTempRotationMatrix, 5);
    c = PHYSICS_RAW_MATRIX_WORD(bruce->mTempRotationMatrix, 6);
    d = PHYSICS_RAW_MATRIX_WORD(bruce->mTempRotationMatrix, 7);
    PHYSICS_RAW_MATRIX_WORD(bruce->mRotationMatrix, 4) = a;
    PHYSICS_RAW_MATRIX_WORD(bruce->mRotationMatrix, 5) = b;
    PHYSICS_RAW_MATRIX_WORD(bruce->mRotationMatrix, 6) = c;
    PHYSICS_RAW_MATRIX_WORD(bruce->mRotationMatrix, 7) = d;
    a = *(volatile int *)&bruce->mTempPos.x;
    b = *(volatile int *)&bruce->mTempPos.y;
    c = *(volatile int *)&bruce->mTempPos.z;
    *(volatile int *)&bruce->CSuper.CBody.CItem.mPos.x = a;
    *(volatile int *)&bruce->CSuper.CBody.CItem.mPos.y = b;
    *(volatile int *)&bruce->CSuper.CBody.CItem.mPos.z = c;
}

#undef PHYSICS_RAW_MATRIX_WORD

/**
 * CBruce::Trick_PlayerInit(unsigned short) — wrapper for Panel_PlayerInit
 *
 * Address: 0x80010354
 * Mangled: Trick_PlayerInit__6CBruceUs
 *
 * @param player_num  Player index whose panel state is initialized.
 */
void CBruce::Trick_PlayerInit(unsigned short player_num)
{
    Panel_PlayerInit(player_num, this);
}

/**
 * Trick_TakeOff__6CBruce
 *
 * Address:   0x8001037C
 * Mangled:   Trick_TakeOff__6CBruce
 * Demangled: CBruce::Trick_TakeOff(void)
 *
 */
void CBruce::Trick_TakeOff()
{
    CBrucePhysicsSlice *bruce =
        reinterpret_cast<CBrucePhysicsSlice *>(this);
    int state = bruce->mPhysicsState;
    if (state == 5)
        return;
    int playTime = bruce->mPlayTime;
    if (playTime == 0)
        return;
    bruce->mPerfectLanding = 0;
    bruce->mSloppyLanding = 0;
    bruce->mTrickTookOff = 1;
    bruce->mNoTrick = 0;
    bruce->mAutoTurn = 0xF;
    if (state != 2)
    {
        reinterpret_cast<CBruce *>(bruce)->ResetTrickBuffers();
    }
    bruce->mNoSpin = 0;
    bruce->mNoSpinScore = 0;
    Panel_TakeOff(reinterpret_cast<CBruce *>(bruce));
    if (GGame == 2)
    {
        Physics_HistoryReset(bruce);
    }
}

/**
 * Trick_Trick__6CBrucePcii
 *
 * Address:   0x80010418
 * Mangled:   Trick_Trick__6CBrucePcii
 * Demangled: CBruce::Trick_Trick(char *, int, int)
 *            Records a trick with the panel, normalizing negative scores as non-flip tricks.
 *
 * @param bruce  Owner-local view of the implicit `this` pointer.
 * @param pName  Asset / object name string.
 * @param Points  Trick score; a negative value marks a non-flip trick.
 * @param Lock  Panel trick-lock mode.
 */
void CBruce::Trick_Trick(char *pName, int Points, int Lock)
{
    CBrucePhysicsSlice *bruce =
        reinterpret_cast<CBrucePhysicsSlice *>(this);
    if (bruce->mTrickTookOff == 0)
        return;

    int game = GGame;
    if (game != 7)
        goto score_phase;
    bruce->mPlayTime = 0;

score_phase:
    int flip;
    if (Points >= 0)
    {
        flip = 1;
    }
    else
    {
        Points = -Points;
        flip = 0;
    }
    bruce->mPanelData.Trick(pName, Points, Lock, flip);
}

/**
 * CBruce::Trick_HoldTrick(int) — wrapper for Panel_HoldTrick
 *
 * Address: 0x80010474
 * Mangled: Trick_HoldTrick__6CBrucei
 *
 * @param Points  Score accumulated for this held trick.
 */
void CBruce::Trick_HoldTrick(int Points)
{
    Panel_HoldTrick(this, Points);
}

/**
 * CBruce::EndManual(void) — end manual if active, call Trick_Land
 *
 * Address: 0x80010494
 * Mangled: EndManual__6CBruce
 *
 */
void CBruce::EndManual()
{
    CBrucePhysicsSlice *bruce =
        reinterpret_cast<CBrucePhysicsSlice *>(this);
    if (bruce->mManual != 0)
    {
        bruce->mManual = 0;
        Trick_Land();
    }
}

/**
 * CBruce::Get180s(void) — count completed 180-degree rotations
 *
 * Address: 0x800104BC
 * Mangled: Get180s__6CBruce
 *
 * @return count
 */
int CBruce::Get180s()
{
    CBrucePhysicsSlice *bruce =
        reinterpret_cast<CBrucePhysicsSlice *>(this);
    int count = bruce->mNum180Spins;
    if (bruce->mManual == 0)
    {
        if (bruce->mNoSpinScore == 0)
        {
            int rot = bruce->mTallyAngles;
            int sign = rot >> 31;
            int absRot = (rot ^ sign) - sign;
            if (absRot >= 0x5DD)
            {
                count++;
                bruce->mTallyAngles = 0;
            }
        }
    }
    return count;
}

/**
 * Trick_Land__6CBruce
 *
 * Address:   0x8001050C
 * Mangled:   Trick_Land__6CBruce
 * Demangled: CBruce::Trick_Land(void)
 *
 * Finalize spin and landing bonuses, refresh the panel's trick queue, award
 * crowd/special-meter feedback, and reset the per-air trick state.
 *
 */
void CBruce::Trick_Land()
{
    CBrucePhysicsSlice *bruce =
        reinterpret_cast<CBrucePhysicsSlice *>(this);
    bruce->mFlipNum = 0;
    bruce->mTricks++;
    bruce->mNum180Spins = Get180s();
    if (bruce->mNum180Spins == 1 && bruce->mOldPhysicsState == 2)
        bruce->mNum180Spins = 0;

    if (bruce->mNum180Spins == 0)
    {
        bruce->mPerfectLanding = 0;
        bruce->mSloppyLanding = 0;
    }
    if (bruce->mPerfectLanding)
        SFX_PlayX(0xBC, 0x2000, 0);
    if (bruce->mSloppyLanding)
        SFX_PlayX(0xBD, 0x2000, 0);

    Physics_PanelRefreshTrickMessage(&bruce->mPanelData);
    int trick_count = bruce->mPanelData.mTQStack;
    if ((trick_count == 0
         || (trick_count > 0
             && bruce->mPanelData.mTQ[trick_count - 1].locked != 0))
        && bruce->mNum180Spins != 0
        && (bruce->mOldPhysicsState != 2 || bruce->mNum180Spins > 1))
    {
        reinterpret_cast<CBruce *>(bruce)->Trick_Trick(
            PhysicsTrickNames.Ollie, bruce->mNum180Spins * -50, 0);
        bruce->mTrickAwarded = 1;
    }

    Panel_Land(reinterpret_cast<CBruce *>(bruce));
    bruce->mNoSpinScore = 0;
    bruce->mNoSpin = 0;
    if (bruce->mTrickAwarded && bruce->mLastSItem)
        bruce->mLastSItem = 0;
    if (bruce->mTrickAwarded && GGame == 7)
        bruce->mPlayTime = 0;

    Crowd_Cheer(bruce->mPanelData.mLandedTotal);
    if (bruce->mTrickAwarded)
    {
        if (bruce->mMagicTime == 0)
        {
            bruce->mMagicPoints += bruce->mPanelData.mLandedTotal;
            if (bruce->mMagicPoints >= 3000)
            {
                bruce->mMagicPoints = 0;
                bruce->mMagicTime = 750;
            }
        }
        else
        {
            bruce->mMagicTime += bruce->mPanelData.mLandedTotal / 5;
            if (bruce->mMagicTime >= 751)
                bruce->mMagicTime = 750;
        }
    }

    bruce->mTrickAwarded = 0;
    bruce->mDoingWhatTrick = 0;
    bruce->mNum180Spins = 0;
    bruce->mTallyAngles = 0;
    bruce->mTrickTookOff = 0;
}

// ====== 0x80010730 Trick_Bail__6CBruce =========================================
// Trick_Bail__6CBruce
// Address: 0x80010730
// Demangled: CBruce::Trick_Bail(void)
void CBruce::Trick_Bail()
{
    CBrucePhysicsSlice *bruce =
        reinterpret_cast<CBrucePhysicsSlice *>(this);
    int game = GGame;
    if (game == 7)
    {
        bruce->mPlayTime = 0;
    }
    int *vel = (int *)&bruce->CSuper.CBody.mVel;
    bruce->mTrickTookOff = 0;
    bruce->mNum180Spins = 0;
    bruce->mTallyAngles = 0;
    bruce->mGaps = 0;
    bruce->mpGrindGapTrick = 0;
    bruce->mFlipNum = 0;
    bruce->mNoSpinScore = 0;
    bruce->mNoSpin = 0;
    bruce->mMagicPoints = 0;
    bruce->mMagicTime = 0;
    int dot = Mips_CVecDotProd((CVector *)vel, (CVector *)vel);
    int root = M3dMaths_SquareRoot0(dot);
    int s1 = root << 6;
    int score = bruce->mPanelData.TotalScore();
    if (score != 0)
    {
        Crowd_Ahh(score);
    }
    else
    {
        Crowd_Oww(s1 >> 12);
    }
    ((Blood *)&bruce->mBlood)->Reset();
    Panel_Bail(reinterpret_cast<CBruce *>(bruce));
    bruce->mTrickAwarded = 0;
    bruce->mDoingWhatTrick = 0;
    Physics_Vibrate(bruce, 8, 1, 0x96);
}

/**
 * Vibrate__6CBruceiii
 *
 * Address:   0x80010800
 * Mangled:   Vibrate__6CBruceiii
 * Demangled: CBruce::Vibrate(int, int, int)
 *
 * @param bruce  Owner-local view of the implicit `this` pointer.
 * @param time  Vibration duration.
 * @param motor  Controller actuator index.
 * @param value  Vibration intensity.
 */
void Physics_Vibrate(
    CBrucePhysicsSlice *bruce, int time, int motor, int value)
{
    int game = GGame;
    int index = bruce->mIndex;
    if (game == 7)
    {
        index ^= GHorsePlayer;
    }
    int padPtr = reinterpret_cast<SCareerOptionsPhysicsOwner *>(&GCareer)->VibrationEnabled[index];
    if (padPtr != 0)
    {
        if (*(int *)&GVInfo1 != 2 && *(int *)&GVInfo2 != 2)
        {
            Pad_ActuatorOn(index & 0xFF, time & 0xFFFF, motor & 0xFF, value & 0xFF);
        }
    }
}

/**
 * StateTransition__6CBruce13EPhysicsStatei — apply transition counters, velocity swap, and matrix setup
 *
 * Address:   0x80010890
 * Mangled:   StateTransition__6CBruce13EPhysicsStatei
 * Demangled: CBruce::StateTransition(EPhysicsState, int)
 *
 * @param bruce  Owner-local view of the implicit `this` pointer.
 * @param NewState  Destination physics state.
 * @param line  Source-line diagnostic token supplied by the caller.
 */
void Physics_StateTransition(CBrucePhysicsSlice *bruce, int NewState, int line)
{
    (void)line;
    if (bruce->mPhysicsState != NewState)
    {
        if (NewState == 1 && bruce->mPhysicsState != 2)
        {
            bruce->mAirs = bruce->mAirs + 1;
        }
        if (NewState == 2 && bruce->mPhysicsState != 1)
        {
            bruce->mAirs = bruce->mAirs + 1;
        }
    }

    if (bruce->mPhysicsState != 2 && NewState == 2)
    {
        bruce->mLastFallLine = bruce->mFront;
        bruce->mLastFallLine.x = -Mips_FixMul(bruce->mLastFallLine.x, 0x32C8);
        bruce->mLastFallLine.y = Mips_FixMul(bruce->mLastFallLine.y, 0x32C8);
        bruce->mLastFallLine.z = -Mips_FixMul(bruce->mLastFallLine.z, 0x32C8);
    }

    if (bruce->mPhysicsState == 5 && NewState == 1)
    {
        CVector v40;
        CVector v30;
        CVector v20;
        reinterpret_cast<CBruce *>(bruce)->Trick_Trick(
            PhysicsTrickNames.WallRide, 0x1F4, 1);
        if (bruce->CSuper.mAnim == 0x23)
        {
            if ((bruce->CSuper.mExtraFlags & 2) == 0)
            {
                v40 = *(CVector *)&bruce->mRight;
                bruce->mRight = bruce->mUp;
                Physics_VectorNegate(&v20, &v40);
                *(CVector *)&bruce->mUp = v20;
                goto done;
            }
        }
        else if ((bruce->CSuper.mExtraFlags & 2) != 0)
        {
            v40 = *(CVector *)&bruce->mRight;
            bruce->mRight = bruce->mUp;
            Physics_VectorNegate(&v30, &v40);
            *(CVector *)&bruce->mUp = v30;
            goto done;
        }
        v40 = *(CVector *)&bruce->mRight;
        Physics_VectorNegate(&v30, (CVector *)&bruce->mUp);
        *(CVector *)&bruce->mRight = v30;
        *(CVector *)&bruce->mUp = v40;
    }

done:
    Physics_RotMatrixFromFRUVectors(bruce);
    bruce->mPhysicsState = NewState;
}

/**
 * MaybeFlipAnimationTransition__6CBruce — clear markers and flip flags when entering flip states
 *
 * Address:   0x80010AE8
 * Mangled:   MaybeFlipAnimationTransition__6CBruce
 * Demangled: CBruce::MaybeFlipAnimationTransition(void)
 *
 * @param bruce  Owner-local view of the implicit `this` pointer.
 */
void Physics_MaybeFlipAnimationTransition(
    CBrucePhysicsSlice *bruce)
{
    if (bruce->CSuper.mAnim == 0x82
        && bruce->CSuper.mFrame >= 0x10)
    {
        bruce->mCess = 0;
        bruce->CSuper.mExtraFlags ^= 2;
    }
    if (bruce->mFlipAfter != 0)
    {
        bruce->mFlipAfter = 0;
        bruce->CSuper.mExtraFlags ^= 2;
    }
    if (bruce->mRotateAfter != 0)
    {
        bruce->mRotateAfter = 0;
        bruce->mFront.x = bruce->mRotationMatrix.m[0][2] =
            -(short)(u16)bruce->mFront.x;
        bruce->mFront.y = bruce->mRotationMatrix.m[1][2] =
            -(short)(u16)bruce->mFront.y;
        bruce->mFront.z = bruce->mRotationMatrix.m[2][2] =
            -(short)(u16)bruce->mFront.z;
        bruce->mRight.x = bruce->mRotationMatrix.m[0][0] =
            -(short)(u16)bruce->mRight.x;
        bruce->mRight.y = bruce->mRotationMatrix.m[1][0] =
            -(short)(u16)bruce->mRight.y;
        bruce->mRight.z = bruce->mRotationMatrix.m[2][0] =
            -(short)(u16)bruce->mRight.z;
    }
}

/**
 * Debug_RunAnim__6CBruceii
 *
 * Address:   0x80010BEC
 * Mangled:   Debug_RunAnim__6CBruceii
 * Demangled: CBruce::Debug_RunAnim(int, int)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 * @param x  Animation index.
 * @param line  Source-line diagnostic token supplied by the caller.
 */
void Physics_DebugRunAnim(
    CBrucePhysicsSlice *bruce, int x, int line)
{
    Physics_MaybeFlipAnimationTransition(bruce);
    int big = 0x10000;
    int neg = -1;
    bruce->CSuper.mAnimSpeed = big;
    reinterpret_cast<CSuper *>(bruce)->RunAnim(x, 0, -1, neg);
}

/**
 * Debug_RunAnim1__6CBruceiii
 *
 * Address:   0x80010C40
 * Mangled:   Debug_RunAnim1__6CBruceiii
 * Demangled: CBruce::Debug_RunAnim1(int, int, int)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 * @param x  Animation index.
 * @param y  Starting animation frame.
 * @param line  Source-line diagnostic token supplied by the caller.
 */
void Physics_DebugRunAnim1(
    CBrucePhysicsSlice *bruce, int x, int y, int line)
{
    (void)line;
    Physics_MaybeFlipAnimationTransition(bruce);
    bruce->CSuper.mAnimSpeed = 0x10000;
    reinterpret_cast<CSuper *>(bruce)->RunAnim(x, y, -1, -1);
}

/**
 * Debug_RunAnim2__6CBruceiiii
 *
 * Address:   0x80010CA0
 * Mangled:   Debug_RunAnim2__6CBruceiiii
 * Demangled: CBruce::Debug_RunAnim2(int, int, int, int)
 *
 * @param bruce  Owner-local view of the implicit `this` pointer.
 * @param x  Animation index.
 * @param y  Starting animation frame.
 * @param z  Ending animation frame.
 * @param line  Source-line diagnostic token supplied by the caller.
 */
void Physics_DebugRunAnim2(
    CBrucePhysicsSlice *bruce, int x, int y, int z, int line)
{
    (void)line;
    Physics_MaybeFlipAnimationTransition(bruce);
    bruce->CSuper.mAnimSpeed = 0x10000;
    reinterpret_cast<CSuper *>(bruce)->RunAnim(x, y, z, -1);
}

/**
 * Debug_RunAnim3__6CBruceiiiii
 *
 * Address:   0x80010D0C
 * Mangled:   Debug_RunAnim3__6CBruceiiiii
 * Demangled: CBruce::Debug_RunAnim3(int, int, int, int, int)
 *
 * @param bruce  Owner-local view of the implicit `this` pointer.
 * @param x  Animation index.
 * @param y  Starting animation frame.
 * @param z  Ending animation frame.
 * @param zz  Final animation-control argument passed to RunAnim.
 * @param line  Source-line diagnostic token supplied by the caller.
 */
void Physics_DebugRunAnim3(
    CBrucePhysicsSlice *bruce, int x, int y, int z, int zz, int line)
{
    (void)line;
    Physics_MaybeFlipAnimationTransition(bruce);
    bruce->CSuper.mAnimSpeed = 0x10000;
    reinterpret_cast<CSuper *>(bruce)->RunAnim(x, y, z, zz);
}

/**
 * CBruce::ResetTrickBuffers(void) — clear trick buffer count
 *
 * Address: 0x80010D80
 *          Target: jr $ra; sw $zero, 0xeb8($a0)
 * Mangled: ResetTrickBuffers__6CBruce
 *
 */
void CBruce::ResetTrickBuffers()
{
    CBrucePhysicsSlice *bruce =
        reinterpret_cast<CBrucePhysicsSlice *>(this);
    bruce->mTapTurn = 0;
}

/**
 * Bail__6CBrucei13ELimitBailVel
 *
 * Address:   0x80010D88
 * Mangled:   Bail__6CBrucei13ELimitBailVel
 * Demangled: CBruce::Bail(int, ELimitBailVel)
 *            Reset trick/physics state for a bail, pick the bail script, and trigger it.
 *
 * @param bruce  Owner-local view of the implicit `this` pointer.
 * @param BailNumber  Bail script/animation selector.
 * @param LimitBailVel  Whether the bail path limits the skater's velocity.
 */
void Physics_Bail(
    CBrucePhysicsSlice *bruce, int BailNumber, ELimitBailVel LimitBailVel)
{
    int frame_pad[2];
    CBruceScriptQueueSlice *script =
        reinterpret_cast<CBruceScriptQueueSlice *>(bruce);
    int bailCount = bruce->mBails;
    int maxValue = 0x64;

    bruce->mLimitBailVel = LimitBailVel;
    bruce->mFlipAngle = 0;
    script->mpRailTrickQueued = 0;
    script->mTricksQueued = 0;
    bruce->mDontStickRail = 0;
    bruce->mOnRail = 0;
    script->mpJumpTrick = 0;
    script->mpOffTrick = 0;
    script->mBailNumber = 0;
    script->mTrickGravity = maxValue;
    bruce->mCess = 0;
    bruce->mManual = 0;
    script->mDoingAScript = 0;
    bruce->mBails = bailCount + 1;
    reinterpret_cast<CBruce *>(bruce)->Trick_Bail();
    bruce->mHeadHit = 0;
    bruce->mInBail = 1;
    bruce->mJumpPending = 0;
    bruce->mJumpStrength = 0;
    reinterpret_cast<CBruce *>(bruce)->ResetTrickBuffers();
    bruce->mDoingATrick = 0;
    bruce->mStickToRail = 0;
    Physics_StateTransition(bruce, 1, 0x411);
    if (BailNumber == 0)
    {
        int dot = Mips_CVecDotProd((CVector *)&bruce->CSuper.CBody.mVel, (CVector *)&bruce->CSuper.CBody.mVel);
        int Speed = M3dMaths_SquareRoot0(dot);
        if ((Speed << 6) <= 0x2CFFF)
        {
            BailNumber = 1;
        }
    }
    short val = GBailsList[BailNumber];
    short half = val / 2;
    Physics_TriggerTrickScript(bruce, GTricks + half);
    Physics_UpdateScript(bruce);
    bruce->mDoingATrick = 0;
}

/**
 * ProjectCVectorToPlane__FP7CVectorG7SVECTOR
 *
 * Address:   0x80010EB4
 * Mangled:   ProjectCVectorToPlane__FP7CVectorG7SVECTOR
 * Demangled: ProjectCVectorToPlane(CVector *, SVECTOR)
 *
 * @param V  Vector projected in place.
 * @param Normal  Plane normal.
 */
void ProjectCVectorToPlane(CVector *V, SVECTOR Normal)
{
    CVector normalExpanded;
    normalExpanded.vx = Normal.vx;
    normalExpanded.vy = Normal.vy;
    normalExpanded.vz = Normal.vz;
    int dot = Mips_CVecDotProd(V, &normalExpanded);
    int subx = Mips_FixMul(dot, Normal.vx);
    V->vx -= subx;
    int suby = Mips_FixMul(dot, Normal.vy);
    V->vy -= suby;
    int subz = Mips_FixMul(dot, Normal.vz);
    V->vz -= subz;
}

/**
 * Projects a vector onto a plane while preserving its original magnitude.
 *
 * Address:   0x80010F50
 * Mangled:   CVectorRotateToPlane__FP7CVectorP7SVECTORT0
 * Demangled: CVectorRotateToPlane(CVector *, SVECTOR *, CVector *)
 *
 * @param InVector Source vector.
 * @param Normal Plane normal.
 * @param OutVector Receives the projected, rescaled vector.
 */
void CVectorRotateToPlane(CVector *InVector, SVECTOR *Normal, CVector *OutVector)
{
    int OldMag = M3dMaths_SquareRoot0(Mips_CVecDotProd(InVector, InVector)) << 6;

    *OutVector = *InVector;

    ProjectCVectorToPlane(OutVector, *Normal);

    int NewMag = M3dMaths_SquareRoot0(Mips_CVecDotProd(OutVector, OutVector)) << 6;
    OldMag >>= 8;
    NewMag >>= 8;
    *OutVector = (*OutVector * OldMag) / NewMag;
}

/**
 * UberFrig__FP6CBruceG7CVectorPc
 *
 * Address: 0x80011070
 * Mangled: UberFrig__FP6CBruceG7CVectorPc
 *
 * @return 0
 */
void UberFrig(CBruce *pSkater, CVector v, char *pc)
{
    (void)pc;
    CBrucePhysicsSlice *bruce = (CBrucePhysicsSlice *)pSkater;
    if (bruce->mOnRail != 0)
        return;

    SLineInfo li;

    // raycast 1
    li.StartCoords.vx = v.vx;
    li.StartCoords.vy = v.vy;
    li.StartCoords.vz = v.vz;
    li.EndCoords = li.StartCoords;
    li.EndCoords.vy += 0x1F40000;
    M3dColij_InitLineInfo(&li);
    M3dZone_LineToItem(&li, 1);
    if (li.pItem != 0)
    {
        Physics_CalculateDynamicLighting(
            bruce, li.pFace, li.pItem);
        return;
    }

    // raycast 2
    li.StartCoords.vx = v.vx;
    li.StartCoords.vy = v.vy;
    li.StartCoords.vz = v.vz;
    li.StartCoords.vy -= 0x12C000;
    li.EndCoords = li.StartCoords;
    li.EndCoords.vy += 0x190000;
    M3dColij_InitLineInfo(&li);
    M3dZone_LineToItem(&li, 1);
    if (li.pItem != 0)
    {
        bruce->CSuper.CBody.CItem.mPos.x = li.Position.vx;
        bruce->CSuper.CBody.CItem.mPos.y = li.Position.vy;
        bruce->CSuper.CBody.CItem.mPos.z = li.Position.vz;
        bruce->CSuper.CBody.CItem.mPos.y -= 0x1E000;
        bruce->CSuper.CBody.mOldPos = bruce->CSuper.CBody.CItem.mPos;
        return;
    }

    // raycast 3
    int startX = bruce->mSafePos.x;
    li.StartCoords.vx = startX;
    li.StartCoords.vy = bruce->CSuper.CBody.CItem.mPos.y;
    li.StartCoords.vz = bruce->CSuper.CBody.CItem.mPos.z;
    li.EndCoords = li.StartCoords;
    int OldX = bruce->CSuper.CBody.CItem.mPos.x;
    int OldOldX = bruce->CSuper.CBody.mOldPos.x;
    bruce->CSuper.CBody.CItem.mPos.x = startX;
    bruce->CSuper.CBody.mOldPos.x = startX;
    li.EndCoords.vy += 0x190000;
    M3dColij_InitLineInfo(&li);
    M3dZone_LineToItem(&li, 1);
    if (li.pItem != 0)
        return;

    // raycast 4
    int e4c = bruce->mSafePos.z;
    li.StartCoords.vx = OldX;
    li.StartCoords.vy = bruce->CSuper.CBody.CItem.mPos.y;
    li.StartCoords.vz = e4c;
    li.EndCoords = li.StartCoords;
    bruce->CSuper.CBody.CItem.mPos.x = OldX;
    bruce->CSuper.CBody.mOldPos.x = OldOldX;
    bruce->CSuper.CBody.CItem.mPos.z = e4c;
    bruce->CSuper.CBody.mOldPos.z = e4c;
    li.EndCoords.vy += 0x190000;
    M3dColij_InitLineInfo(&li);
    M3dZone_LineToItem(&li, 1);
    if (li.pItem != 0)
        return;

    // all raycasts missed
    bruce->CSuper.CBody.CItem.mPos = bruce->mSafePos;
    bruce->CSuper.CBody.mOldPos = bruce->CSuper.CBody.CItem.mPos;
    int iv = -bruce->CSuper.CBody.mVel.z;
    bruce->CSuper.CBody.mVel.x = iv / 2;
    bruce->CSuper.CBody.mVel.z = bruce->CSuper.CBody.mVel.x / 2;
}

/**
 * CBruce::FrigDarkenObject(void) — darken tricked-on object based on score
 *
 * Address: 0x8001131C
 * Mangled: FrigDarkenObject__6CBruce
 *
 * @param bruce  Owner-local view of the implicit `this` pointer.
 */
void Physics_FrigDarkenObject(CBrucePhysicsSlice *bruce)
{
    if (bruce->mLastSItem != 0)
    {
        if (bruce->mLastPSX != 0)
        {
            int score = bruce->mPanelData.TotalScore();
            if (score > 0)
            {
                // The legacy target return is incidental and intentionally ignored.
                TrickOb_TrickedOnItem(
                    bruce->mLastSItem, reinterpret_cast<void *>(bruce), score);
            }
        }
    }
}

/**
 * CBruce::TrickObRail(void) — empty rail-trick hook that preserves
 * the original compiler's empty stack frame.
 *
 * Address: 0x80011378
 *          Target: addiu $sp,-0x10; addiu $sp,0x10; jr $ra; nop
 * Mangled: TrickObRail__6CBruce
 *
 * @param bruce  Owner-local view of the implicit `this` pointer.
 */
void Physics_TrickObRail(CBrucePhysicsSlice *bruce)
{
    int frame[4];
    (void)bruce;
}

// ====== 0x80011388 StartHandPlant__6CBruce =====================================
// StartHandPlant__6CBruce
// Address: 0x80011388
// Demangled: CBruce::StartHandPlant(void)
void Physics_StartHandPlant(CBrucePhysicsSlice *bruce)
{
    bruce->mHandPlantTweaked = 0;
    if (bruce->mTrickTookOff == 0)
    {
        reinterpret_cast<CBruce *>(bruce)->Trick_TakeOff();
        if (bruce->mTrickTookOff == 0)
        {
            bruce->mStickToRail = 0;
            goto epilogue;
        }
    }
    reinterpret_cast<CBruce *>(bruce)->Trick_Trick(
        PhysicsTrickNames.Handplant, 0x3E8, 1);
    Physics_DebugRunAnim1(bruce, 0x22, 8, 0x586);
    bruce->mDoingWhatTrick = bruce->mDoingWhatTrick | 0x4000;
    Physics_StateTransition(bruce, 8, 0x588);
    SFX_PlayX(0xE7, 0x1000, 0);
    bruce->CSuper.CBody.CItem.mPos = bruce->mRailPos;
    bruce->CSuper.CBody.CItem.mPos.y =
        bruce->CSuper.CBody.CItem.mPos.y - 0x1E;
    Physics_TrickObRail(bruce);
    bruce->mDontStickRail = bruce->mCurrentRailStart;
    bruce->CSuper.CBody.mVel.x = 0;
    bruce->CSuper.CBody.mVel.z = 0;
    bruce->CSuper.CBody.mVel.y = -bruce->CSuper.CBody.mVel.y;
epilogue:
    ;
}

/*
 * PHYSICS_RECON_* identifiers are reconstruction-local expression names;
 * original preprocessor identifiers are not present in the target artifacts.
 */
#define PHYSICS_RECON_EFFECTIVE_BALANCE(self)                                      \
    (Cheat_StatsOverride != 0                                                \
         ? Cheat_StatsOverride                                              \
         : Stats[((CBrucePhysicsSlice *)(self))->mSkaterType].Balance        \
               + ((((CBrucePhysicsSlice *)(self))->mMagicTime != 0          \
                   || Cheat_Fire != 0)                                      \
                      ? 3                                                    \
                      : 0)                                                   \
               + (unsigned char)                                             \
                     ((CBrucePhysicsSlice *)(self))->mpBoardStats->balance)

/** Return the branch-free absolute value used by the rail proximity tests. */
static inline int PhysicsAbsolute(int value)
{
    int sign = value >> 31;
    return (value ^ sign) - sign;
}

/** Return the branch-free magnitude of one signed script operand. */
static inline int PhysicsShortAbsolute(unsigned short value)
{
    int sign = (short)value >> 31;
    return (short)((value ^ sign) - sign);
}

/**
 * StartGrind__6CBruce
 *
 * Address:   0x80011478
 * Mangled:   StartGrind__6CBruce
 * Demangled: CBruce::StartGrind(void)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
extern "C" void Physics_StartGrind(CBrucePhysicsSlice *bruce)
{
    int temp;
    int FrontDot;
    int RightDot;
    int VelDot;
    int Direction;
    int RailTrick;

    bruce->mGrindReleasedLR = 0;
    bruce->CSuper.CBody.CItem.mPos = bruce->mRailPos;
    bruce->CSuper.CBody.CItem.mPos.y = bruce->CSuper.CBody.CItem.mPos.y - 0x19000;
    bruce->mStickToRail = 0;
    bruce->mOnRail = 1;
    bruce->mGrindPos = bruce->CSuper.CBody.CItem.mPos;
    bruce->mGrindStartPos = bruce->CSuper.CBody.CItem.mPos;

    /* Same rail as last frame builds the score multiplier; a new one resets it. */
    if (bruce->mCurrentRailStart != bruce->mLastRailStart)
    {
        bruce->mGrindTime = bruce->mGrindTime - 0x1E;
        if (bruce->mGrindTime < 0)
            bruce->mGrindTime = 0;
        bruce->mGrindLeanDir = 0;
        bruce->mLastRailStart = bruce->mCurrentRailStart;
    }
    else
    {
        bruce->mGrindTime = bruce->mGrindTime + 0x28;
    }

    bruce->mGrindLeanDir =
        (((bruce->mGrindLeanDir
           * (100 - PHYSICS_RECON_EFFECTIVE_BALANCE((int)bruce) * 0xF))
          / 100)
         * (bruce->mGrindTime + 0x3C))
        / 0x50;

    bruce->mNumGrinds = bruce->mNumGrinds + 1;

    switch (bruce->mRailType)
    {
    case 0:
        /* Both rail-spark variants use the same demo-build sound ID. */
        if (bruce->mRailSparks != 0)
            SFX_PlayX(5, 0x2000, 0);
        else
            SFX_PlayX(5, 0x2000, 0);
        break;
    case 1:
        if (bruce->mRailSparks != 0)
            SFX_PlayX(0xD5, 0x2000, 0);
        else
            SFX_PlayX(0xD6, 0x2000, 0);
        break;
    case 2:
        /* Both rail-spark variants use the same demo-build sound ID. */
        if (bruce->mRailSparks != 0)
            SFX_PlayX(0xC8, 0x2000, 0);
        else
            SFX_PlayX(0xC8, 0x2000, 0);
        break;
    case 3:
        if (bruce->mRailSparks != 0)
            SFX_PlayX(0xD7, 0x2000, 0);
        else
            SFX_PlayX(0xD6, 0x2000, 0);
        break;
    }

    Physics_StateTransition(bruce, 4, 0x5DE);
    bruce->mDoingATrick = 1;
    bruce->mComboCounter = bruce->mComboCounter + 1;
    bruce->mRails = bruce->mRails + 1;
    Physics_TrickObRail(bruce);

    /* Rail right-hand normal, as the cross of the segment with world up. */
    CVector mSideVel =
        *(CVector *)&bruce->mRailPos - *(CVector *)&bruce->mLastGroundPos;
    mSideVel.vy = 0;
    temp = mSideVel.vx;
    mSideVel.vx = mSideVel.vz;
    mSideVel.vz = -temp;

    FrontDot = Mips_CVecDotProd(
        (CVector *)&bruce->mRailDir, (CVector *)&bruce->mFront);
    RightDot = Mips_CVecDotProd(
        (CVector *)&bruce->mRailDir, (CVector *)&bruce->mRight);
    VelDot = Mips_CVecDotProd(
        (CVector *)&bruce->mRailDir, &mSideVel);

    if (bruce->mDontStickRail != bruce->mCurrentRailStart)
    {
        bruce->mRail_RightOfRail = 0;
        bruce->mRail_FrontSide = 0;
        if (VelDot > 0)
            bruce->mRail_RightOfRail = 1;
        bruce->mRail_SkaterFlipped = 0;
        if ((bruce->CSuper.mExtraFlags & 2) != 0)
            bruce->mRail_SkaterFlipped = 1;
        if (bruce->mRail_RightOfRail != bruce->mRail_SkaterFlipped)
            bruce->mRail_FrontSide = 1;
    }

    bruce->mRail_Backwards = 0;
    bruce->mRail_Parallel = 0;
    if (PhysicsAbsolute(RightDot) < 0xB33)
    {
        bruce->mRail_Parallel = 1;
        if (FrontDot > 0)
            bruce->mRail_Backwards = 1;
    }
    else
    {
        if ((bruce->CSuper.mExtraFlags & 2) != 0)
            RightDot = -RightDot;
        if (RightDot < 0)
            bruce->mRail_Backwards = 1;
    }
    bruce->mDontStickRail = bruce->mCurrentRailStart;

    Direction = Physics_GetDirection(bruce);
    if ((unsigned int)(Direction - 1) < 4)
    {
        int LastEvent = bruce->mEventQueuePos - 1;
        int FoundFirst = 0;
        if (LastEvent < 0)
            LastEvent = 0x1F;

        int Event = Physics_GetPreviousPressedEvent(
            bruce, LastEvent, 0, 0x3C, LastEvent);
        while (Event >= 0)
        {
            if (bruce->mEventQueue[Event].Button == Direction &&
                FoundFirst)
            {
                Direction += 8;
                break;
            }
            if (bruce->mEventQueue[Event].Button == Direction)
                FoundFirst = 1;
            Event = Physics_GetPreviousPressedEvent(
                bruce, Event, 1, 0x3C, LastEvent);
        }
    }

    bruce->mRail_BadLedge = 0;
    if (bruce->mRail_LedgeType != 0)
    {
        bruce->mRail_BadLedge =
            (unsigned int)(bruce->mRail_LedgeType >> 1) ^
            bruce->mRail_RightOfRail ^
            (unsigned int)((bruce->mRail_Direction + 1) >> 1);
    }

    RailTrick =
        bruce->mRail_RightOfRail
        + bruce->mRail_Parallel * 2
        + bruce->mRail_Backwards * 4
        + bruce->mRail_SkaterFlipped * 8
        + Direction * 0x10;
    CVector Vel = *(CVector *)&bruce->mRailDir;

    Vel *=
        (Cheat_StatsOverride != 0
             ? Cheat_StatsOverride
             : Stats[bruce->mSkaterType].Speed
                   + ((bruce->mMagicTime != 0
                       || Cheat_Fire != 0)
                          ? 3
                          : 0)
                   + (unsigned char)
                         bruce->mpBoardStats->speed)
        + 5;
    *(CVector *)&bruce->CSuper.CBody.mVel += Vel;
    bruce->mRailFlip = 1;
    bruce->mRailSparks = 0;

    if (bruce->mpRailTrickQueued != 0 &&
        XFrames - bruce->mRailTrickTime < 0x1E)
    {
        Physics_TriggerTrickScript(bruce, bruce->mpRailTrickQueued);
        bruce->mpRailTrickQueued = 0;
    }
    else
    {
        Physics_TriggerTrickIndex(bruce, RailTrick);
    }
    Physics_UpdateScript(bruce);
}

/**
 * CBruce::HandleStickToRail(void) — resolve pending rail adhesion and bail state
 *
 * Address:   0x80011A4C
 * Mangled:   HandleStickToRail__6CBruce
 * Demangled: CBruce::HandleStickToRail(void)
 *
 * @param bruce  Owner-local view of the skater whose rail state is updated.
 */
void Physics_HandleStickToRail(CBrucePhysicsSlice *bruce)
{
    if (bruce->mPad->Triangle.Pressed == 0)
        bruce->mJumpInhibit = 0;

    if (bruce->mSameRailTimer != 0)
        bruce->mSameRailTimer = bruce->mSameRailTimer - 1;

    if (bruce->mDontStickTimer <= 0)
        goto try_stick;
    bruce->mDontStickTimer = bruce->mDontStickTimer - 1;
    bruce->mOnRail = 0;

cleanup:
    bruce->mStickToRail = 0;
    return;

try_stick:
    if (bruce->mPad->Triangle.Pressed == 0)
        goto cleanup;
    if (bruce->mInBail != 0)
        goto cleanup;
    if (bruce->mPhysicsState == 2 && bruce->CSuper.CBody.mVel.y > 0)
        goto cleanup;
    if (bruce->mRailInhibit != 0)
        goto cleanup;
    if (bruce->mStickToRail == 0)
        return;
    if (bruce->mOnRail != 0)
        goto cleanup;

    bruce->mDontStickTimer = 0;
    if (bruce->mPhysicsState == 0)
    {
        bruce->mNum180Spins = 0;
        bruce->mTallyAngles = 0;
    }

    int Bailing = 0;
    if (bruce->mFlipNum != 0)
    {
        if (bruce->mFlipSpeed > 0)
        {
            int angle = bruce->mFlipAngle;
            angle &= 0xFFF;
            Bailing = angle < 0xED4;
        }
        else if ((bruce->mFlipAngle & 0xFFF) > 300)
        {
            Bailing = 1;
        }
    }

    int d50 = bruce->mDoingATrick;
    if ((d50 != 0 && bruce->mCanStartNextTrick == 0) || Bailing)
    {
        int e64 = bruce->mRailSlack;
        int bail = 1;
        if (e64 != 0)
        {
            if (*(signed char *)((int)&bruce->CSuper.mAnimDir) == 1)
            {
                int current = (int)bruce->CSuper.mFrame;
                bail = current < ((int)*(signed char *)&bruce->CSuper.mTargetFrame - e64);
            }
            else if ((int)bruce->CSuper.mFrame <= ((int)*(signed char *)&bruce->CSuper.mTargetFrame + e64))
            {
                bail = 0;
            }
        }
        if (bail)
        {
            Physics_Bail(
                bruce, (int)bruce->mRailBailNumber, DO_LIMIT);
            ++bruce->mTrickBails;
            bruce->mRailBailNumber = 0;
            return;
        }
    }

    if (d50 != 0)
        bruce->mTrickAwarded = 1;

    if (bruce->CSuper.CBody.mVel.y < 0)
    {
        int x = PhysicsAbsolute(bruce->mUp.y);
        if (x < 3000)
        {
            int y = PhysicsAbsolute((int)(short)*(unsigned short *)((int)&bruce->CSuper.CBody.mNormal.y));
            if (y < 0x5DC)
            {
                int z = PhysicsAbsolute(bruce->mRight.y);
                if (z < 0x5DC && (char)bruce->mPad->Up.Pressed != 0)
                {
                    Physics_StartHandPlant(bruce);
                    return;
                }
            }
        }
    }

    if (bruce->mTrickTookOff != 0)
    {
        Physics_StartGrind(bruce);
        return;
    }
    goto cleanup;
}

/**
 * HandleBailOnLanding__6CBrucei
 *
 * Address:   0x80011CF8
 * Mangled:   HandleBailOnLanding__6CBrucei
 * Demangled: CBruce::HandleBailOnLanding(int)
 *
 * @param bruce  Owner-local view of the implicit `this` pointer.
 * @param line  Source line/debug marker passed by callers.
 */
void Physics_HandleBailOnLanding(CBrucePhysicsSlice *bruce, int line)
{
    if (bruce->mInBail != 0)
        return;

    if (bruce->mDoingATrick == 0)
    {
        register int early_landing asm("$5") = bruce->mFlipNum;
        if (early_landing == 0)
            return;
    }

    register int flags asm("$6") = bruce->mDoingWhatTrick;
    register int landing asm("$5") = bruce->mFlipNum;
    register int bail_a asm("$3") = 0;
    if ((flags & 0x2000) != 0)
    {
        bail_a = bruce->mPad->Square.Pressed != 0;
    }

    register int Bailing asm("$4") = bail_a;
    register int bail_b asm("$3") = 0;
    if ((flags & 0x4000) != 0)
    {
        bail_b = bruce->mPad->Circle.Pressed != 0;
    }
    asm volatile("" : "+r"(flags));

    register int d54_val asm("$2") = bruce->mCanStartNextTrick;
    Bailing = Bailing | bail_b;
    register int d54_zero asm("$2") = (d54_val == 0);
    Bailing = Bailing | d54_zero;
    if (landing != 0)
    {
        if (bruce->mFlipSpeed > 0)
        {
            int angle = bruce->mFlipAngle & 0xFFF;
            if (angle < 0xED4)
                goto set_bail;
            goto check_bail;
        }
        else
        {
            int angle = bruce->mFlipAngle & 0xFFF;
            if (angle < 0x12D)
                goto check_bail;
        }
set_bail:
        Bailing = 1;
    }
check_bail:
    if (Bailing != 0)
    {
        Physics_Bail(
            bruce, (int)bruce->mBailNumber, DO_LIMIT);
        register int bail_count asm("$2") = bruce->mTrickBails;
        bruce->mBailNumber = 0;
        bail_count++;
        bruce->mTrickBails = bail_count;
    }
}

/**
 * HandleBailPerpendicular__6CBruceG7SVECTOR
 *
 * Address: 0x80011E18
 * Mangled: HandleBailPerpendicular__6CBruceG7SVECTOR
 *
 * Projects the skater's velocity and heading into the landing plane, records
 * landing quality, and chooses the lateral bail direction when nearly sideways.
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 * @param Normal  Surface normal used to resolve the perpendicular bail.
 */
void Physics_HandleBailPerpendicular(CBrucePhysicsSlice *bruce, SVECTOR Normal)
{
    CVector InplaneVel;

    bruce->mPerfectLanding = 0;
    bruce->mSloppyLanding = 0;

    InplaneVel = *(CVector *)&bruce->CSuper.CBody.mVel;
    ProjectCVectorToPlane(&InplaneVel, Normal);

    int Speed = M3dMaths_SquareRoot0(
        Mips_CVecDotProd(&InplaneVel, &InplaneVel)) << 6;
    if (Speed > 0x1E000)
    {
        CVector PlaneUnitVector;
        CVector InplaneFront;

        PlaneUnitVector = InplaneVel;
        M3dUtils_NormaliseVector((CVector *)&PlaneUnitVector);

        CVector *Front = (CVector *)&bruce->mFront;
        InplaneFront = *Front;
        ProjectCVectorToPlane(&InplaneFront, Normal);
        M3dUtils_NormaliseVector((CVector *)&InplaneFront);

        int rawDot = Mips_CVecDotProd(
            &PlaneUnitVector, &InplaneFront);
        int Dot = PhysicsAbsolute(rawDot);
        if (Dot >= 0xFC2)
            bruce->mPerfectLanding = 1;
        if (Dot < 0xB50)
            bruce->mSloppyLanding = 1;

        if (Dot < 0x666)
        {
            VECTOR v;
            CVector vv;

            Physics_FlipToVelocity(bruce);
            CVector *Right = (CVector *)&bruce->mRight;
            int RightDot = Mips_CVecDotProd(&InplaneVel, Right);

            vv = *(CVector *)&bruce->CSuper.CBody.mOldPos - *(CVector *)&bruce->mOldPos2;
            v.vx = vv.vx;
            v.vy = vv.vy;
            v.vz = vv.vz;
            int mag = PhysicsAbsolute(vv.vx) + PhysicsAbsolute(vv.vy);
            if (mag + PhysicsAbsolute(vv.vz) > 0x8000)
            {
                v.vx = vv.vx >> 4;
                v.vy = vv.vy >> 4;
                v.vz = vv.vz >> 4;
            }
            VectorNormal(&v, &v);

            {
                register int nvx asm("$3") = -v.vx;
                bruce->mFront.x = nvx;
            }
            bruce->mFront.y = -v.vy;
            bruce->mFront.z = -v.vz;
            bruce->mUp.x = Normal.vx;
            bruce->mUp.y = Normal.vy;
            bruce->mUp.z = Normal.vz;

            gte_ldopv1(Front);
            gte_ldopv2((CVector *)&bruce->mUp);
            gte_op12();
            gte_stlvnl(Right);
            gte_ldopv1((CVector *)&bruce->mUp);
            gte_ldopv2(Right);
            gte_op12();
            gte_stlvnl(Front);
            Physics_RotMatrixFromFRUVectors(bruce);

            if (bruce->CSuper.mExtraFlags & 2)
                RightDot = -RightDot;
            if (RightDot <= 0)
                Physics_Bail(bruce, 2, DO_LIMIT);
            else
                Physics_Bail(bruce, 0, DO_LIMIT);
            bruce->mPerpendicularBails++;
        }
    }
}

/**
 * TriggerTrick__6CBrucePs
 *
 * Address:   0x80012194
 * Mangled:   TriggerTrick__6CBrucePs
 * Demangled: CBruce::TriggerTrick(short *)
 *
 * @param bruce  Owner-local view of the implicit `this` pointer.
 * @param pTrick  Trick to operate on.
 */
void Physics_TriggerTrickScript(
    CBrucePhysicsSlice *bruce, short *pTrick)
{
    CBruceScriptQueueSlice *script =
        reinterpret_cast<CBruceScriptQueueSlice *>(bruce);
    register int one asm("$2") = 1;
    bruce->mDoingATrick = one;
    script->mDoingAScript = one;
    asm volatile("");
    script->CSuper.mAnimSpeed = 0x10000;
    bruce->mCanStartNextTrick = 0;
    script->mpReturn = 0;
    script->mpOffTrick = 0;
    script->mpJumpTrick = 0;
    script->mpTrick = pTrick;
    script->mRailBailNumber = 0;
    script->mBailNumber = 0;
    bruce->mRailSlack = 0;
    script->mTrickPoints = 0x7B;
    script->mpTrickName = PhysicsTrickNames.Trick;
}

/**
 * Trigger indexed grind-table entry by resolving its GTricks script pointer.
 *
 * Address:   0x800121E0
 * Mangled:   TriggerTrick__6CBrucei
 * Demangled: CBruce::TriggerTrick(int)
 *
 * @param bruce  Owner-local view of the implicit `this` pointer.
 * @param Trick  Index into the grind-trick table.
 */
void Physics_TriggerTrickIndex(
    CBrucePhysicsSlice *bruce, int Trick)
{
    short *table1 = GGrindTricksList;
    short val = table1[Trick];
    short half = val / 2;
    short *table2 = GTricks;
    Physics_TriggerTrickScript(bruce, table2 + half);
}

/**
 * ButtonRecord__6CBruceUcUc
 *
 * Address:   0x80012238
 * Mangled:   ButtonRecord__6CBruceUcUc
 * Demangled: CBruce::ButtonRecord(unsigned char, unsigned char)
 * @param bruce  Owner-local view of the implicit `this` pointer.
 * @param Button  Button identifier to record.
 * @param Pressed  New pressed-state value.
 */
void Physics_ButtonRecord(
    CBrucePhysicsSlice *bruce, unsigned char Button, unsigned char Pressed)
{
    CBruceScriptQueueSlice *queue =
        reinterpret_cast<CBruceScriptQueueSlice *>(bruce);
    u8 btn = Button & 0xFF;
    u8 st = Pressed & 0xFF;
    u8 *buf = queue->mCurrentButtonState;
    if (buf[btn] == st)
        return;
    buf[btn] = st;
    {
        int idx = queue->mEventQueuePos;
        queue->mEventQueue[idx].Button = btn;
    }
    {
        int idx = queue->mEventQueuePos;
        queue->mEventQueue[idx].Pressed = st;
    }
    {
        int idx = queue->mEventQueuePos;
        int xframes = XFrames;
        queue->mEventQueue[idx].Time = xframes;
    }
    {
        int limit = 0x20;
        int idx = queue->mEventQueuePos;
        idx = idx + 1;
        queue->mEventQueuePos = idx;
        if (idx == limit)
        {
            queue->mEventQueuePos = 0;
        }
    }
}

/**
 * ButtonRemoveLast__6CBruceUcUc
 *
 * Address:   0x800122BC
 * Mangled:   ButtonRemoveLast__6CBruceUcUc
 * Demangled: CBruce::ButtonRemoveLast(unsigned char, unsigned char)
 *            Walk backward through button history and clear the most recent matching event.
 *
 * @param bruce  Owner-local view of the implicit `this` pointer.
 * @param Button  Button identifier to find.
 * @param Pressed  Pressed-state value to match.
 */
void Physics_ButtonRemoveLast(
    CBrucePhysicsSlice *bruce, unsigned char Button, unsigned char Pressed)
{
    CBruceScriptQueueSlice *queue =
        reinterpret_cast<CBruceScriptQueueSlice *>(bruce);
    char *queueBase = reinterpret_cast<char *>(bruce);
    int idx = queue->mEventQueuePos;
    Button &= 0xFF;
    int Event = idx - 1;
    Pressed &= 0xFF;
    if (Event < 0)
        Event = 0x1F;
    if (Event == idx)
        return;
    int origIdx = idx;
    int offset = Event << 3;
    u8 *p0 = (u8 *)(offset + (int)&((CBruceScriptQueueSlice *)0)->mEventQueue[0].Button);
    p0 = (u8 *)(queueBase + (int)p0);
    offset += (int)&((CBruceScriptQueueSlice *)0)->mEventQueue[0].Pressed;
    u8 *p1 = (u8 *)(offset + reinterpret_cast<int>(queueBase));
    do
    {
        if (*p0 == Button)
        {
            if (*p1 == Pressed)
            {
                *p0 = 0;
                return;
            }
        }
        p0 -= 8;
        Event--;
        p1 -= 8;
        if (Event < 0)
        {
            p0 = &queue->mEventQueue[31].Button;
            p1 = &queue->mEventQueue[31].Pressed;
            Event = 0x1F;
        }
    } while (Event != origIdx);
}

/**
 * GetDirection__6CBruce
 *
 * Address:   0x80012344
 * Mangled:   GetDirection__6CBruce
 * Demangled: CBruce::GetDirection(void)
 *
 * @param bruce  Owner-local view of the implicit `this` pointer.
 * @return DirectionTable[dir]
 */
int Physics_GetDirection(CBrucePhysicsSlice *bruce)
{
    int pad = (int)bruce->mPad;
    SControl *control = (SControl *)pad;
    u8 leftX = control->Down.Pressed;
    u8 dpadUp = control->Up.Pressed;
    u8 dpadLeft = control->Left.Pressed;
    u8 dpadRight = control->Right.Pressed;
    s8 analog1 = bruce->mAY;
    int idx = dpadUp + leftX * 2 + dpadLeft * 4;
    int dir = idx + dpadRight * 8;
    if (analog1 < -40)
    {
        dir |= 1;
    }
    if (analog1 >= 41)
    {
        dir |= 2;
    }
    s8 analog2 = bruce->mAX;
    if (analog2 < -40)
    {
        dir |= 4;
    }
    if (analog2 >= 41)
    {
        dir |= 8;
    }
    return DirectionTable[dir];
}

/**
 * UpdateButtons__6CBruce
 *
 * Address:   0x800123D4
 * Mangled:   UpdateButtons__6CBruce
 * Demangled: CBruce::UpdateButtons(void)
 *
 * @param bruce  Owner-local view of the implicit `this` pointer.
 */
void Physics_UpdateButtons(CBrucePhysicsSlice *bruce)
{
    unsigned int dir = Physics_GetDirection(bruce);
    Physics_ButtonRecord(bruce, 1, (dir ^ 1) < 1u);
    Physics_ButtonRecord(bruce, 2, (dir ^ 2) < 1u);
    Physics_ButtonRecord(bruce, 3, (dir ^ 3) < 1u);
    Physics_ButtonRecord(bruce, 4, (dir ^ 4) < 1u);
    Physics_ButtonRecord(bruce, 5, (dir ^ 5) < 1u);
    Physics_ButtonRecord(bruce, 6, (dir ^ 6) < 1u);
    Physics_ButtonRecord(bruce, 7, (dir ^ 7) < 1u);
    Physics_ButtonRecord(bruce, 8, (dir ^ 8) < 1u);
    int pad = (int)bruce->mPad;
    Physics_ButtonRecord(bruce, 9, ((SControl *)pad)->Circle.Pressed);
    pad = (int)bruce->mPad;
    Physics_ButtonRecord(bruce, 0xA, ((SControl *)pad)->Square.Pressed);
    pad = (int)bruce->mPad;
    Physics_ButtonRecord(bruce, 0xB, ((SControl *)pad)->X.Pressed);
    pad = (int)bruce->mPad;
    Physics_ButtonRecord(bruce, 0xC, ((SControl *)pad)->Triangle.Pressed);
    pad = (int)bruce->mPad;
    Physics_ButtonRecord(bruce, 0xE, ((SControl *)pad)->LeftTwo.Pressed);
    pad = (int)bruce->mPad;
    Physics_ButtonRecord(bruce, 0x10, ((SControl *)pad)->RightTwo.Pressed);
}

/**
 * Adds a trick request to the skater's pending trick queue.
 *
 * Address:   0x8001251C
 * Mangled:   QueueTrick__6CBrucePsii
 * Demangled: CBruce::QueueTrick(short *, int, int)
 *
 * @param bruce  Owner-local view of the implicit `this` pointer.
 * @param pTrick Trick script to queue.
 * @param Button Button associated with the queued trick.
 * @param EventTime Time of the triggering button event.
 */
void Physics_QueueTrick(
    CBrucePhysicsSlice *bruce, short *pTrick, int Button, int EventTime)
{
    CBruceScriptQueueSlice *queue = (CBruceScriptQueueSlice *)bruce;
    if (queue->mTricksQueued == 10)
        return;
    if (Button == 12)
    {
        queue->mpRailTrickQueued = pTrick;
        queue->mRailTrickTime = EventTime;
        return;
    }
    {
        int Head = (queue->mTrickQueueTail + queue->mTricksQueued) % 10;
        queue->mTrickQueue[Head].pTrick = pTrick;
        queue->mTrickQueue[Head].Time = XFrames;
        queue->mTrickQueue[Head].EventTime = EventTime;
        queue->mTrickQueue[Head].Button = Button;
        queue->mTricksQueued++;
    }
}

/**
 * UpdateQueue__6CBruce — process queued tricks when countdown elapses
 *
 * Address:   0x800125B8
 * Mangled:   UpdateQueue__6CBruce
 * Demangled: CBruce::UpdateQueue(void)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_UpdateQueue(CBrucePhysicsSlice *bruce)
{
    CBruceScriptQueueSlice *queue = (CBruceScriptQueueSlice *)bruce;

    if (bruce->mDoingATrick != 0)
        return;
    if (bruce->mInBail != 0)
        return;
    if (queue->mTricksQueued == 0)
        return;

    if ((u32)(bruce->mPhysicsState - 1) >= 2U)
    {
        queue->mTricksQueued = 0;
    }
    else
    {
        int idx = queue->mTrickQueueTail;
        int xframes = XFrames;
        if ((u32)(xframes - queue->mTrickQueue[idx].Time) < 0xB4)
        {
            queue->mActionButton = queue->mTrickQueue[idx].Button;
            queue->mActionButtonEventTime = queue->mTrickQueue[idx].EventTime;
            Physics_TriggerTrickScript(bruce, queue->mTrickQueue[idx].pTrick);
            idx = queue->mTrickQueueTail;
        }
        queue->mTrickQueueTail = (idx + 1) % 10;
        queue->mTricksQueued--;
    }
}

/**
 * ActionButtonHeld__6CBruce — detect held-button replay against last queue entry
 *
 * Address:   0x800126BC
 * Mangled:   ActionButtonHeld__6CBruce
 * Demangled: CBruce::ActionButtonHeld(void)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 * @return 0 if the action event was released after its trigger time; otherwise 1.
 */
int Physics_ActionButtonHeld(CBrucePhysicsSlice *bruce)
{
    CBruceScriptQueueSlice *queue = (CBruceScriptQueueSlice *)bruce;
    int obj = (int)bruce;
    int head = queue->mEventQueuePos;
    int Event = head - 1;
    if (Event < 0)
    {
        Event = 0x1F;
    }
    if (Event != head)
    {
        int last = queue->mActionButton;
        int *timeBase = (int *)&queue->mEventQueue[0].Time;
        int originalHead = head;
        int heldOffset =
            (Event << 3) +
            (int)&((CBruceScriptQueueSlice *)0)->mEventQueue[0].Pressed;
        u8 *heldPtr = (u8 *)(heldOffset + obj);
        int eventOffset = (Event << 3) + 0xAB4;
        u8 *eventPtr = (u8 *)(eventOffset + obj);
        do
        {
            int slot = Event << 3;
            if (*eventPtr == last)
            {
                if (*heldPtr == 0)
                {
                    int slotTime = *(int *)((char *)timeBase + slot);
                    if ((u32)queue->mActionButtonEventTime < (u32)slotTime)
                    {
                        return 0;
                    }
                }
            }
            heldPtr -= 8;
            Event--;
            eventPtr -= 8;
            if (Event < 0)
            {
                heldPtr = &queue->mEventQueue[31].Pressed;
                eventPtr = &queue->mEventQueue[31].Button;
                Event = 0x1F;
            }
        } while (Event != originalHead);
    }
    return 1;
}

/**
 * Finds the previous pressed event within a requested time window.
 *
 * Address:   0x80012768
 * Mangled:   GetPreviousPressedEvent__6CBruceiiii
 * Demangled: CBruce::GetPreviousPressedEvent(int, int, int, int)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 * @param Event Event queue position from which to start searching.
 * @param Skip Number of recent events to skip.
 * @param Time Maximum age of a matching event.
 * @param Top Queue position at which to stop searching.
 * @return The matching queue position, or -1 if none is found.
 */
int Physics_GetPreviousPressedEvent(
    CBrucePhysicsSlice *bruce, int Event, int Skip, int Time, int Top)
{
    char *queueBase = reinterpret_cast<char *>(bruce);
    Event -= Skip;
    if (Event < 0)
    {
        Event += 0x20;
    }
    /* MAIN.SYM records no source locals here; these names are descriptive. */
    u32 *queueTimeBase =
        &((CBruceScriptQueueSlice *)bruce)->mEventQueue[0].Time;
    int current_frame = XFrames;
    int event_offset = (Event * 8) + 0xAB4;
    u8 *event_ptr =
        (u8 *)(event_offset + reinterpret_cast<int>(queueBase));
    for (;;)
    {
        int slot_offset = Event * 8;
        char *event_base = queueBase + slot_offset;
        if (*event_ptr != 0)
        {
            if ((u32)Time <
                (u32)(current_frame -
                      *(u32 *)((char *)queueTimeBase + slot_offset)))
            {
                return -1;
            }
            if (((CBruceScriptQueueSlice *)event_base)
                    ->mEventQueue[0].Pressed != 0)
            {
                return Event;
            }
        }
        Event--;
        event_ptr -= 8;
        if (Event < 0)
        {
            event_ptr =
                &((CBruceScriptQueueSlice *)bruce)->mEventQueue[31].Button;
            Event = 0x1F;
        }
        if (Event == Top)
            return -1;
    }
}

/**
 * Match the recorded button history against the current trick-key table.
 *
 * Address:   0x800127FC
 * Mangled:   MaybeTriggerTrick__6CBruce
 * Demangled: CBruce::MaybeTriggerTrick(void)
 *
 * Match the recent button-event history against the current skater's trick
 * key table and queue the first complete sequence.
 */
void CBruce::MaybeTriggerTrick()
{
    short *pKeys;

    if (mInBail == 0)
    {
        pKeys = (short *)(reinterpret_cast<unsigned char *>(GTricks) +
                         *(short *)(reinterpret_cast<unsigned char *>(GTricksKeys) +
                                   mSkaterType * 2));
        if ((unsigned int)(mPhysicsState - 1) < 3)
        {
            while (*pKeys != 0)
            {
                short NumButtons;
                short *pTrick;
                short Time;
                int Special;
                short *pKey;
                short ActionButton;
                short Button1;
                int LastEvent;
                unsigned int ActionButtonEventTime;

                NumButtons = *pKeys++;
                pKeys += NumButtons;
                pTrick = (short *)(reinterpret_cast<unsigned char *>(GTricks) + *pKeys++);
                Time = *pKeys++;
                Special = Time & 0x8000;
                Time &= 0x0FFF;
                ActionButton = pKeys[-3];
                pKey = pKeys - 5;
                Button1 = pKeys[-4];
                LastEvent = mEventQueuePos - 1;
                if (LastEvent < 0)
                    LastEvent = 31;

                if (Special == 0 || mMagicTime != 0)
                {
                    if (NumButtons == 2 && Button1 > 0 && Button1 < 9)
                    {
                        int Event2;
                        int Direction;
                        int Event1;

                        Event1 = GetPreviousPressedEvent(
                            LastEvent, 0, Time, LastEvent);
                        if (Event1 == -1)
                            continue;

                        Event2 = GetPreviousPressedEvent(
                            Event1, 1, Time, LastEvent);
                        Direction = GetDirection();
                        if (Direction == 0 && Event2 == -1)
                            continue;

                        if (Event2 != -1)
                        {
                            if (mEventQueue[Event1].Button == Button1 &&
                                mEventQueue[Event2].Button == ActionButton)
                            {
                                ActionButtonEventTime =
                                    mEventQueue[Event2].Time;
                                ButtonRemoveLast(Button1, 1);
                            }
                            else if (mEventQueue[Event2].Button == Button1 &&
                                     mEventQueue[Event1].Button == ActionButton)
                            {
                                ButtonRemoveLast(Button1, 1);
                                ActionButtonEventTime =
                                    mEventQueue[Event1].Time;
                            }
                            else if (Direction == Button1)
                            {
                                if (mEventQueue[Event1].Button == ActionButton)
                                {
                                    ActionButtonEventTime =
                                        mEventQueue[Event1].Time;
                                }
                                else if (mEventQueue[Event2].Button == ActionButton)
                                {
                                    ActionButtonEventTime =
                                        mEventQueue[Event2].Time;
                                }
                                else
                                {
                                    continue;
                                }
                            }
                            else
                            {
                                continue;
                            }
                        }
                        else
                        {
                            if (Direction != Button1 ||
                                mEventQueue[Event1].Button != ActionButton)
                                continue;
                            ActionButtonEventTime =
                                mEventQueue[Event1].Time;
                        }

                        ButtonRemoveLast(ActionButton, 1);
                        QueueTrick(pTrick, ActionButton, ActionButtonEventTime);
                        continue;
                    }
                    else
                    {
                        int Event1;
                        int Event2;
                        int OtherButton[10];
                        int OtherButtonIndex;

                        Event1 = GetPreviousPressedEvent(
                            LastEvent, 0, Time, LastEvent);
                        if (Event1 == -1)
                            return;
                        Event2 = GetPreviousPressedEvent(
                            Event1, 1, Time, LastEvent);
                        OtherButtonIndex = 0;
                        if (Event2 != -1)
                        {
                            if ((mEventQueue[Event1].Button == Button1 &&
                                 mEventQueue[Event2].Button == ActionButton) ||
                                (mEventQueue[Event2].Button == Button1 &&
                                 mEventQueue[Event1].Button == ActionButton))
                            {
                                if (mEventQueue[Event1].Button == ActionButton)
                                    ActionButtonEventTime = mEventQueue[Event1].Time;
                                else
                                    ActionButtonEventTime = mEventQueue[Event2].Time;

                                int ButtonsLeft;
                                int EventX;

                                ButtonsLeft = NumButtons - 2;
                                EventX = Event2;
                                while (ButtonsLeft != 0)
                                {
                                    NumButtons = *pKey--;
                                    EventX = GetPreviousPressedEvent(
                                        EventX, 1, Time, LastEvent);
                                    if (EventX == -1 ||
                                        mEventQueue[EventX].Button != NumButtons)
                                        break;
                                    if (OtherButtonIndex < 10)
                                    {
                                        OtherButton[OtherButtonIndex++] =
                                            mEventQueue[EventX].Button;
                                    }
                                    --ButtonsLeft;
                                }

                                if (ButtonsLeft == 0)
                                {
                                    ButtonRemoveLast(Button1, 1);
                                    ButtonRemoveLast(ActionButton, 1);
                                    {
                                        int *pOtherButton;
                                        int i;
                                        i = 0;
                                        pOtherButton = OtherButton;
                                        if (OtherButtonIndex > 0)
                                        {
                                            do
                                            {
                                                ButtonRemoveLast(*pOtherButton++, 1);
                                                ++i;
                                            } while (i < OtherButtonIndex);
                                        }
                                    }
                                    QueueTrick(
                                        pTrick, ActionButton, ActionButtonEventTime);
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

/**
 * GetScriptFrame__6CBrucei
 *
 * Address:   0x80012CE0
 * Mangled:   GetScriptFrame__6CBrucei
 * Demangled: CBruce::GetScriptFrame(int)
 * @param bruce  Owner-local view of the implicit `this` pointer.
 * @param Frame  Animation frame index.
 * @return The resolved frame, clamped to the active animation.
 */
short Physics_GetScriptFrame(CBrucePhysicsSlice *bruce, int Frame)
{
    if (Frame == 0x2711)
    {
        return (signed char)bruce->CSuper.mTargetFrame;
    }
    if (Frame >= 0)
    {
        register int max asm("$3") = bruce->CSuper.mNumFrames - 1;
        if (max < Frame)
        {
            Frame = max;
        }
    }
    else
    {
        int dir = (signed char)bruce->CSuper.mAnimDir;
        if (dir > 0)
        {
            int cur = (signed char)bruce->CSuper.mTargetFrame;
            asm("addu %0,%1,%0" : "+r"(Frame) : "r"(cur));
            if (Frame < 0)
            {
                Frame = 0;
            }
        }
        else
        {
            int cur = (signed char)bruce->CSuper.mTargetFrame;
            Frame = cur - Frame;
            int max = bruce->CSuper.mNumFrames - 1;
            if (max < Frame)
            {
                Frame = max;
            }
        }
    }
    return (short)Frame;
}

/**
 * Advance the active skater-animation script until an opcode requests a wait.
 *
 * Script commands update animation frames, velocity, trick state, sounds, and
 * several owner-local gameplay flags.  Branch commands store byte offsets into
 * the loaded `GTricks` image; command 0x3d additionally saves a return cursor
 * for command 0x3e.
 *
 * Address:   0x80012D78
 * Mangled:   UpdateScript__6CBruce
 * Demangled: CBruce::UpdateScript(void)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_UpdateScript(CBrucePhysicsSlice *bruce)
{
    // Keep each access tied to the owner field.  A persistent pointer-to-field
    // local changes the original allocator's treatment of the skater pointer.
#define scriptCursor (&bruce->mpTrick)
    int Done = 0;

    while (!Done)
    {
        short *command = *scriptCursor;
        switch (*command)
        {
        case 1:
        {
            (*scriptCursor)++;
            unsigned short animation = (unsigned short)**scriptCursor;
            (*scriptCursor)++;
            bruce->CSuper.mFrame = 0;
            *(short *)((int)&bruce->CSuper.mAnim) = animation;
            break;
        }

        case 10:
            (*scriptCursor)++;
            {
                unsigned short points = (unsigned short)**scriptCursor;
                (*scriptCursor)++;
                bruce->mTrickPoints = points;
            }
            break;

        case 0xB:
        {
            (*scriptCursor)++;
            *(char **)((int)&bruce->mpTrickName) = (char *)*scriptCursor;
            char *pSkip = (char *)*scriptCursor;
            if (*pSkip != '\0')
            {
                do
                {
                    ++pSkip;
                } while (*pSkip != '\0');
            }
            pSkip = (char *)(((unsigned int)pSkip + 2U) & ~1U);
            *scriptCursor = (short *)pSkip;
            break;
        }

        case 2:
            (*scriptCursor)++;
            Physics_DebugRunAnim(
                bruce, (unsigned short)*(short *)((int)&bruce->CSuper.mAnim), 0xA64);
            break;

        case 3:
        {
            (*scriptCursor)++;
            short oldEndFrame = bruce->CSuper.mStartFrame;
            bruce->CSuper.mAnimFinished = 0;
            bruce->CSuper.mAnimDir =
                -(signed char)bruce->CSuper.mAnimDir;
            bruce->CSuper.mStartFrame =
                (signed char)*(signed char *)((int)&bruce->CSuper.mTargetFrame);
            *(signed char *)((int)&bruce->CSuper.mTargetFrame) = (signed char)oldEndFrame;
            break;
        }

        case 9:
        {
            (*scriptCursor)++;
            short frameDelta = **scriptCursor;
            (*scriptCursor)++;
            int Frames = *(unsigned short *)((int)&bruce->CSuper.mFrame)
                         + bruce->CSuper.mAnimDir * frameDelta;
            bruce->CSuper.mFrame = Frames;

            if (bruce->CSuper.mAnimDir > 0)
            {
                if ((short)Frames < bruce->CSuper.mStartFrame)
                    bruce->CSuper.mFrame =
                        bruce->CSuper.mStartFrame;
                if (*(signed char *)((int)&bruce->CSuper.mTargetFrame)
                    < bruce->CSuper.mFrame)
                {
                    bruce->CSuper.mFrame =
                        *(signed char *)((int)&bruce->CSuper.mTargetFrame);
                }
            }
            else
            {
                if (bruce->CSuper.mStartFrame < (short)Frames)
                    bruce->CSuper.mFrame =
                        bruce->CSuper.mStartFrame;
                if (bruce->CSuper.mFrame
                    < *(signed char *)((int)&bruce->CSuper.mTargetFrame))
                {
                    bruce->CSuper.mFrame =
                        *(signed char *)((int)&bruce->CSuper.mTargetFrame);
                }
            }
            break;
        }

        case 0xC:
            (*scriptCursor)++;
            if (bruce->mPhysicsState == 4)
            {
                reinterpret_cast<CBruce *>(bruce)->Trick_Trick(
                    bruce->mpTrickName,
                    bruce->mTrickPoints,
                    1);
            }
            else
            {
                reinterpret_cast<CBruce *>(bruce)->Trick_Trick(
                    bruce->mpTrickName,
                    bruce->mTrickPoints,
                    0);
            }
            bruce->mTrickAwarded = 1;
            break;

        case 0xD:
        {
            int targetFrame =
                Physics_GetScriptFrame(bruce, (*scriptCursor)[1]);
            if (bruce->CSuper.mFrame
                        * bruce->CSuper.mAnimDir
                    >= targetFrame
                           * bruce->CSuper.mAnimDir
                || bruce->CSuper.mAnimFinished != 0)
            {
                *scriptCursor += 2;
                break;
            }
            Done = 1;
            break;
        }

        case 7:
            (*scriptCursor)++;
            bruce->mDoingATrick = 0;
            bruce->mDoingAScript = 0;
            bruce->mCanStartNextTrick = 1;
            Done = 1;
            break;

        case 0xE:
            (*scriptCursor)++;
            {
                unsigned short value = (unsigned short)**scriptCursor;
                (*scriptCursor)++;
                bruce->mFrozen = value;
            }
            break;

        case 0xF:
            (*scriptCursor)++;
            bruce->CSuper.CBody.mVel.x = **scriptCursor << 12;
            (*scriptCursor)++;
            bruce->CSuper.CBody.mVel.y = **scriptCursor << 12;
            (*scriptCursor)++;
            bruce->CSuper.CBody.mVel.z = **scriptCursor << 12;
            (*scriptCursor)++;
            break;

        case 0x10:
        {
            int targetFrame =
                Physics_GetScriptFrame(bruce, (*scriptCursor)[1]);
            if (bruce->CSuper.mFrame
                        * bruce->CSuper.mAnimDir
                    < targetFrame
                          * bruce->CSuper.mAnimDir
                    && bruce->CSuper.mAnimFinished == 0
                    && Physics_ActionButtonHeld(bruce) != 0)
            {
                Done = 1;
            }
            else
            {
                *scriptCursor += 2;
            }
            break;
        }

        case 0x17:
            (*scriptCursor)++;
            {
                short points = **scriptCursor;
                (*scriptCursor)++;
                reinterpret_cast<CBruce *>(bruce)->Trick_HoldTrick(points);
            }
            break;

        case 0x11:
            if (Physics_ActionButtonHeld(bruce) != 0)
            {
                reinterpret_cast<CBruce *>(bruce)->Trick_HoldTrick(
                    (*scriptCursor)[1]);
                Done = 1;
            }
            else
            {
                *scriptCursor += 2;
            }
            break;

        case 0x12:
            (*scriptCursor)++;
            {
                short branchOffset = **scriptCursor;
                (*scriptCursor)++;
                if (Physics_ActionButtonHeld(bruce) != 0)
                    *scriptCursor =
                        (short *)(reinterpret_cast<unsigned char *>(GTricks) + branchOffset);
            }
            break;

        case 0x13:
            (*scriptCursor)++;
            bruce->CSuper.mAnimMode = 2;
            break;

        case 0x14:
            (*scriptCursor)++;
            bruce->CSuper.mAnimMode = 0;
            break;

        case 0x16:
            (*scriptCursor)++;
            Done = 1;
            break;

        case 0x15:
            (*scriptCursor)++;
            {
                int Offset = **scriptCursor;
                (*scriptCursor)++;
                *scriptCursor = (short *)(reinterpret_cast<unsigned char *>(GTricks) + Offset);
            }
            break;

        case 0x18:
            (*scriptCursor)++;
            {
                short frame = **scriptCursor;
                (*scriptCursor)++;
                bruce->CSuper.mPingPongEnd =
                    (short)Physics_GetScriptFrame(bruce, frame);
                bruce->CSuper.mPingPongStart = bruce->CSuper.mFrame;
                bruce->CSuper.mPingPongFrame = (short)XFrames;
                bruce->CSuper.mAnimMode = 3;
            }
            break;

        case 0x19:
            (*scriptCursor)++;
            {
                int Offset = **scriptCursor;
                (*scriptCursor)++;
                bruce->mpOffTrick = (short *)(reinterpret_cast<unsigned char *>(GTricks) + Offset);
            }
            break;

        case 0x2A:
            (*scriptCursor)++;
            {
                int Offset = **scriptCursor;
                (*scriptCursor)++;
                bruce->mpJumpTrick = (short *)(reinterpret_cast<unsigned char *>(GTricks) + Offset);
            }
            break;

        case 0x1A:
            if (bruce->mOnRail == 0)
            {
                (*scriptCursor)++;
                break;
            }
            Done = 1;
            break;

        case 0x1B:
            (*scriptCursor)++;
            {
                short value = **scriptCursor;
                (*scriptCursor)++;
                bruce->mRailFlip = value;
            }
            break;

        case 0x1C:
            (*scriptCursor)++;
            bruce->CSuper.mAnimMode = 4;
            break;

        case 0x1D:
            (*scriptCursor)++;
            {
                short value = **scriptCursor;
                (*scriptCursor)++;
                bruce->CSuper.mAnimSpeed =
                    (value * 0x20000) / 0x3C;
            }
            break;

        case 0x1E:
            (*scriptCursor)++;
            Physics_Bail(
                bruce, 0, DO_LIMIT);
            break;

        case 0x1F:
            (*scriptCursor)++;
            {
                short value = **scriptCursor;
                (*scriptCursor)++;
                bruce->mDebugScript = value;
            }
            break;

        case 0x20:
            (*scriptCursor)++;
            bruce->mDoingATrick = 0;
            break;

        case 0x21:
        {
            SVECTOR RotAngle;
            MATRIX RotMatrix;
            MATRIX *pRotation = &RotMatrix;
            (*scriptCursor)++;
            short rotationY = *(*scriptCursor)++;
            RotAngle.vx = RotAngle.vz = 0;
            RotAngle.vy = rotationY & 0xFFF;
            RotMatrixYXZ(&RotAngle, pRotation);
            MulMatrix(&bruce->mRotationMatrix, pRotation);
            Physics_FRUVectorsFromRotMatrix(bruce);
            break;
        }

        case 0x22:
        {
            unsigned int flags = bruce->CSuper.mExtraFlags;
            (*scriptCursor)++;
            bruce->CSuper.mExtraFlags = flags ^ 2;
            break;
        }

        case 0x32:
            (*scriptCursor)++;
            Physics_BoardOn(bruce);
            break;

        case 0x33:
            (*scriptCursor)++;
            Physics_BoardOff(bruce);
            break;

        case 0x23:
            (*scriptCursor)++;
            {
                short value = **scriptCursor;
                (*scriptCursor)++;
                bruce->mGrindTweak = value;
            }
            break;

        case 0x2D:
        {
            (*scriptCursor)++;
            unsigned short encodedValue = (unsigned short)**scriptCursor;
            (*scriptCursor)++;
            int value = (short)encodedValue;
            unsigned int sign = (unsigned int)(value >> 31);
            bruce->mRailSlack = (value ^ sign) - sign;
            break;
        }

        case 0x24:
        {
            (*scriptCursor)++;
            int Offset = **scriptCursor;
            unsigned int condition =
                bruce->mRail_Parallel;
            (*scriptCursor)++;
            if (condition != 0)
                *scriptCursor =
                    (short *)(reinterpret_cast<unsigned char *>(GTricks) + Offset);
            break;
        }

        case 0x25:
        {
            (*scriptCursor)++;
            int Offset = **scriptCursor;
            unsigned int condition =
                bruce->mRail_Backwards;
            (*scriptCursor)++;
            if (condition != 0)
                *scriptCursor =
                    (short *)(reinterpret_cast<unsigned char *>(GTricks) + Offset);
            break;
        }

        case 0x26:
        {
            (*scriptCursor)++;
            int Offset = **scriptCursor;
            unsigned int condition =
                bruce->mRail_RightOfRail;
            (*scriptCursor)++;
            if (condition != 0)
                *scriptCursor =
                    (short *)(reinterpret_cast<unsigned char *>(GTricks) + Offset);
            break;
        }

        case 0x27:
        {
            (*scriptCursor)++;
            int Offset = **scriptCursor;
            unsigned int condition =
                bruce->mRail_FrontSide;
            (*scriptCursor)++;
            if (condition != 0)
                *scriptCursor =
                    (short *)(reinterpret_cast<unsigned char *>(GTricks) + Offset);
            break;
        }

        case 0x28:
        {
            (*scriptCursor)++;
            int Offset = **scriptCursor;
            unsigned int condition =
                bruce->mRail_SkaterFlipped;
            (*scriptCursor)++;
            if (condition != 0)
                *scriptCursor =
                    (short *)(reinterpret_cast<unsigned char *>(GTricks) + Offset);
            break;
        }

        case 0x3A:
        {
            (*scriptCursor)++;
            int Offset = **scriptCursor;
            (*scriptCursor)++;
            int Flipped =
                (bruce->CSuper.mExtraFlags >> 1) & 1;
            if (Flipped
                != (Stats[bruce->mSkaterType].Ability[0xB] == 0))
                *scriptCursor =
                    (short *)(reinterpret_cast<unsigned char *>(GTricks) + Offset);
            break;
        }

        case 0x29:
            (*scriptCursor)++;
            {
                short value = **scriptCursor;
                (*scriptCursor)++;
                bruce->mRailSparks = value;
            }
            break;

        case 0x2B:
        {
            *scriptCursor = command + 1;
            short slot = command[1];
            *scriptCursor = command + 2;
            bruce->mRotateCount[slot] = command[2];
            *scriptCursor = command + 3;
            bruce->mRotateSpeed[slot] = command[3];
            *scriptCursor = command + 4;
            bruce->mRotateTotal[slot] = 0;
            break;
        }

        case 0x2C:
            if (bruce->mRotateCount[0] != 0
                || bruce->mRotateCount[1] != 0
                || bruce->mRotateCount[2] != 0)
            {
                Done = 1;
            }
            else
            {
                (*scriptCursor)++;
            }
            break;

        case 0:
            (*scriptCursor)++;
            break;

        case 0x2E:
            (*scriptCursor)++;
            {
                short value = **scriptCursor;
                (*scriptCursor)++;
                bruce->mHeavyFriction = value;
            }
            break;

        case 0x2F:
        {
            (*scriptCursor)++;
            int Offset = **scriptCursor;
            unsigned int state =
                (unsigned int)(bruce->mPhysicsState - 1);
            (*scriptCursor)++;
            if (state < 2)
                *scriptCursor =
                    (short *)(reinterpret_cast<unsigned char *>(GTricks) + Offset);
            break;
        }

        case 0x30:
            (*scriptCursor)++;
            Physics_BloodSplat(bruce, 0, 0x32);
            break;

        case 0x46:
            (*scriptCursor)++;
            ((Dust *)&bruce->mDust[0])->Reset(bruce->mTerrain, 1);
            break;

        case 0x45:
            (*scriptCursor)++;
            Physics_Vibrate(
                bruce,
                PhysicsShortAbsolute((unsigned short)*(*scriptCursor)++),
                *(*scriptCursor)++,
                *(*scriptCursor)++);
            break;

        case 0x31:
        {
            (*scriptCursor)++;
            bruce->mInBail = 0;
            bruce->mHeavyFriction = 0;
            bruce->mKickLockout = 4;
            if (bruce->mManIsCrouched != 0
                && bruce->mPad->Square.Pressed != 0)
            {
                Physics_DebugRunAnim3(
                    bruce, 8, 0, 0x1A, 0x13, 0xCBE);
            }
            else
            {
                bruce->mManIsCrouched = 0;
                Physics_DebugRunAnim(bruce, 0, 0xCC3);
            }
            bruce->mInBail = 0;
            bruce->CSuper.CBody.mVel.z = 0;
            bruce->CSuper.CBody.mVel.y = 0;
            bruce->CSuper.CBody.mVel.x = 0;
            Physics_BoardOn(bruce);
            break;
        }

        case 0x34:
        {
            (*scriptCursor)++;
            int Offset = **scriptCursor;
            (*scriptCursor)++;
            if (bruce->mRail_BadLedge != 0)
                *scriptCursor =
                    (short *)(reinterpret_cast<unsigned char *>(GTricks) + Offset);
            bruce->mRail_BadLedge = 0;
            break;
        }

        case 0x35:
            (*scriptCursor)++;
            bruce->mFlipAfter = 1;
            break;

        case 0x36:
        {
            (*scriptCursor)++;
            int sfx = **scriptCursor;
            (*scriptCursor)++;
            SFX_PlayX(sfx, 0x2000, 0);
            break;
        }

        case 0x37:
        {
            (*scriptCursor)++;
            int sfx = **scriptCursor;
            (*scriptCursor)++;
            int vol = **scriptCursor;
            (*scriptCursor)++;
            SFX_PlayX(sfx, vol, 0);
            break;
        }

        case 0x38:
        {
            (*scriptCursor)++;
            int sfx = **scriptCursor;
            (*scriptCursor)++;
            int vol = **scriptCursor;
            (*scriptCursor)++;
            int pitch = **scriptCursor;
            (*scriptCursor)++;
            SFX_PlayX(sfx, vol, pitch);
            break;
        }

        case 0x39:
        {
            (*scriptCursor)++;
            int narg = **scriptCursor;
            (*scriptCursor)++;
            Rnd(narg);
            int index = Rnd(narg);
            int Offset = (*scriptCursor)[index];
            *scriptCursor =
                (short *)(reinterpret_cast<unsigned char *>(GTricks) + Offset);
            break;
        }

        case 0x3B:
            (*scriptCursor)++;
            bruce->mNoSpin = 1;
            break;

        case 0x3C:
            (*scriptCursor)++;
            bruce->mNoSpin = 0;
            break;

        case 0x3D:
            (*scriptCursor)++;
            {
                int Offset = **scriptCursor;
                (*scriptCursor)++;
                bruce->mpReturn = *scriptCursor;
                *scriptCursor =
                    (short *)(reinterpret_cast<unsigned char *>(GTricks) + Offset);
            }
            break;

        case 0x3E:
        {
            short *returnAddress = bruce->mpReturn;
            bruce->mpReturn = 0;
            *scriptCursor = returnAddress;
            break;
        }

        case 0x41:
        {
            (*scriptCursor)++;
            int Offset = **scriptCursor;
            unsigned int condition =
                bruce->mRail_LedgeType;
            (*scriptCursor)++;
            if (condition != 0)
                *scriptCursor =
                    (short *)(reinterpret_cast<unsigned char *>(GTricks) + Offset);
            break;
        }

        case 0x3F:
            (*scriptCursor)++;
            {
                unsigned short value = (unsigned short)**scriptCursor;
                (*scriptCursor)++;
                bruce->mRailBailNumber = value;
            }
            break;

        case 0x40:
            (*scriptCursor)++;
            {
                unsigned short value = (unsigned short)**scriptCursor;
                (*scriptCursor)++;
                bruce->mBailNumber = value;
            }
            break;

        case 0x42:
        {
            (*scriptCursor)++;
            int side = **scriptCursor;
            (*scriptCursor)++;
            CVector sidevec = *(CVector *)&bruce->mRight;
            sidevec.vy = 0;
            M3dUtils_NormaliseVector((CVector *)&sidevec);
            *(CVector *)&bruce->CSuper.CBody.CItem.mPos =
                *(CVector *)&bruce->CSuper.CBody.CItem.mPos + sidevec * side;
            break;
        }

        case 0x43:
        {
            (*scriptCursor)++;
            int Offset = **scriptCursor;
            unsigned int condition =
                bruce->CSuper.mExtraFlags & 2;
            (*scriptCursor)++;
            if (condition != 0)
                *scriptCursor =
                    (short *)(reinterpret_cast<unsigned char *>(GTricks) + Offset);
            break;
        }

        case 0x44:
            (*scriptCursor)++;
            {
                unsigned short value = (unsigned short)**scriptCursor;
                (*scriptCursor)++;
                bruce->mTrickGravity = value;
            }
            break;

        case 0x47:
        {
            (*scriptCursor)++;
            int Value = **scriptCursor;
            (*scriptCursor)++;
            int Offset = **scriptCursor;
            (*scriptCursor)++;
            int Spd = M3dMaths_SquareRoot0(
                            Mips_CVecDotProd(
                                (CVector *)&bruce->CSuper.CBody.mVel,
                                (CVector *)&bruce->CSuper.CBody.mVel))
                        << 6;
            if (Value < (Spd >> 12))
                *scriptCursor =
                    (short *)(reinterpret_cast<unsigned char *>(GTricks) + Offset);
            break;
        }

        case 0x48:
        {
            int targetFrame =
                Physics_GetScriptFrame(bruce, (*scriptCursor)[1]);
            short *currentCursor = *scriptCursor;
            *scriptCursor = currentCursor + 2;
            int Offset = **scriptCursor;
            (*scriptCursor)++;
            if (bruce->CSuper.mFrame
                        * bruce->CSuper.mAnimDir
                    >= targetFrame
                           * bruce->CSuper.mAnimDir
                    || bruce->CSuper.mAnimFinished != 0)
            {
                *scriptCursor =
                    (short *)(reinterpret_cast<unsigned char *>(GTricks) + Offset);
            }
            break;
        }

        default:
        {
            short *pCommand = *scriptCursor;
            printf(
                PhysicsDiagnosticPool.script,
                pCommand[-4], pCommand[-3], pCommand[-2], pCommand[-1],
                pCommand[0], pCommand[1], pCommand[2], pCommand[3]);
            bruce->mDoingATrick = 0;
            break;
        }
        }
    }
#undef scriptCursor
#undef object
}

/**
 * Advance a trick-script cursor past one encoded command.
 *
 * Address:   0x80013B28
 * Mangled:   Trick_Skip__FPs
 * Demangled: Trick_Skip(short *)
 *
 * @param pTrick  Cursor at the command opcode.
 * @return Cursor at the following command.
 */
short *Trick_Skip(short *pTrick)
{
    short opcode = *pTrick;
    unsigned short uopcode = *(unsigned short *)pTrick;
    switch (opcode)
    {
    case 1:
    case 9:
    case 10:
    case 0xD:
    case 0xE:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x15:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1B:
    case 0x1D:
    case 0x1F:
    case 0x21:
    case 0x23:
    case 0x24:
    case 0x25:
    case 0x26:
    case 0x27:
    case 0x28:
    case 0x29:
    case 0x2A:
    case 0x2D:
    case 0x2E:
    case 0x2F:
    case 0x34:
    case 0x36:
    case 0x3A:
    case 0x3D:
    case 0x3F:
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
        pTrick = (short *)((int)pTrick + 4);
        break;
    case 0x37:
    case 0x47:
    case 0x48:
        pTrick = (short *)((int)pTrick + 6);
        break;
    case 0xF:
    case 0x2B:
    case 0x38:
    case 0x45:
        pTrick = (short *)((int)pTrick + 8);
        break;
    case 0xB:
    {
        pTrick = (short *)((int)pTrick + 2);
        char *pSkip = (char *)pTrick;
        asm volatile("" : "+r"(pSkip));
        char *v1 = pSkip + 2;
        if (*(unsigned char *)pSkip != 0)
        {
            pSkip = pSkip + 1;
            while (*(unsigned char *)pSkip != 0)
            {
                pSkip = pSkip + 1;
            }
            v1 = pSkip + 2;
        }
        pTrick = (short *)((int)v1 & -2);
        break;
    }
    case 0x39:
    {
        pTrick = (short *)((int)pTrick + 2);
        short v0 = *pTrick;
        pTrick = (short *)((int)pTrick + 2);
        pTrick = (short *)((int)pTrick + (v0 << 1));
        break;
    }
    case 0:
    case 2:
    case 3:
    case 7:
    case 0xC:
    case 0x13:
    case 0x14:
    case 0x16:
    case 0x1A:
    case 0x1C:
    case 0x1E:
    case 0x20:
    case 0x22:
    case 0x2C:
    case 0x30:
    case 0x31:
    case 0x32:
    case 0x33:
    case 0x35:
    case 0x3B:
    case 0x3C:
    case 0x3E:
    case 0x46:
        pTrick = (short *)((int)pTrick + 2);
        break;
    default:
        printf(PhysicsDiagnosticPool.scriptError,
               (int)(short)uopcode);
        pTrick = (short *)((int)pTrick + 2);
        break;
    }
    return pTrick;
}

/**
 * EnsureGTricksLoaded__Fv — cache GTricks tables and offsets on first load
 *
 * Address:   0x80013C04
 * Mangled:   EnsureGTricksLoaded__Fv
 * Demangled: EnsureGTricksLoaded(void)
 *
 * @return 1 when the tables were loaded, or 0 when already resident.
 */
int EnsureGTricksLoaded(void)
{
    if (!GTricks)
    {
        printf(PhysicsDiagnosticPool.loadTricks);
        short *base = reinterpret_cast<short *>(
            FileIO_OpenLoad(GTricksFilename));
        register int ret asm("$2") = 1;
        GTricks = base;
        register int a asm("$4") = base[0];
        register int b asm("$5") = base[1];
        int c = base[2];
        asm volatile("" : "+r"(a), "+r"(b), "+r"(c), "+r"(ret));
        GTricksKeys = (short *)(reinterpret_cast<unsigned char *>(base) + a);
        GGrindTricksList = (short *)(reinterpret_cast<unsigned char *>(base) + b);
        GBailsList = (short *)(reinterpret_cast<unsigned char *>(base) + c);
        return ret;
    }
    return 0;
}

/**
 * UnloadGTricks__Fv — release cached GTricks table memory
 *
 * Address: 0x80013C88
 * Mangled: UnloadGTricks__Fv
 */
void UnloadGTricks(void)
{
    if (GTricks)
    {
        printf(PhysicsDiagnosticPool.unloadTricks);
        Mem_Delete((void *)GTricks);
        GTricks = 0;
    }
}

/**
 * ExtraAnims_Reset(void) — reset extra anims pointer to after default anims
 *
 * Address: 0x80013CD0
 * Mangled: ExtraAnims_Reset__Fv
 *
 */
void ExtraAnims_Reset(void)
{
    pExtraAnim = DefaultAnimations;
    if (*pExtraAnim != -1)
    {
        do
        {
            pExtraAnim++;
        } while (*pExtraAnim != -1);
    }
    short *p = pExtraAnim;
    pExtraAnim = p + 1;
    *(p + 1) = -1;
    asm volatile("");
    NumExtraAnims = 0;
}

/**
 * ExtraAnims_Add(short) — add anim to list and write sentinel
 *
 * Address: 0x80013D2C
 * Mangled: ExtraAnims_Add__Fs
 *
 * @param Anim  Animation index.
 */
void ExtraAnims_Add(short Anim)
{
    short *p = pExtraAnim;
    NumExtraAnims++;
    *p = Anim;
    p++;
    pExtraAnim = p;
    *p = -1;
}

/**
 * ExtraAnims_AddTrick(short *) — walk trick entries and add anim IDs
 *
 * Address: 0x80013D54
 * Mangled: ExtraAnims_AddTrick__FPs
 * @param pTrick  Trick to operate on.
 */
void ExtraAnims_AddTrick(short *pTrick)
{
    short *p = pTrick;
    short val = *p;
    u16 uval = *(u16 *)p;
    if (val != 7)
    {
        do
        {
            int sval = (s16)(u16)uval;
            if (sval == 1)
            {
                ExtraAnims_Add(*(p + 1));
            }
            p = Trick_Skip(p);
            val = *p;
            uval = *(u16 *)p;
        } while (val != 7);
    }
}

/**
 * Walk the trick key table and add each trick's animation entries
 *
 * Address: 0x80013DD4
 * Mangled: ExtraAnims_AddTrickTable__FPs
 *
 * @param pKeys  Keys to operate on.
 */
void ExtraAnims_AddTrickTable(short *pKeys)
{
    short test;
    short trickOffset;
    u16 step;

    goto check;
loop:
    pKeys += (s16)step;
    trickOffset = *pKeys;
    pKeys++;
    ExtraAnims_AddTrick((short *)(reinterpret_cast<unsigned char *>(GTricks) + trickOffset));
    pKeys++;
check:
    test = *pKeys;
    step = *(u16 *)pKeys;
    if (test != 0)
    {
        pKeys++;
        goto loop;
    }
}

/**
 * ExtraAnims_AddSkatersAnims__Fv
 *
 * Address:   0x80013E30
 * Mangled:   ExtraAnims_AddSkatersAnims__Fv
 * Demangled: ExtraAnims_AddSkatersAnims(void)
 *
 */
void ExtraAnims_AddSkatersAnims(void)
{
    int Loaded = EnsureGTricksLoaded();
    printf(PhysicsDiagnosticPool.extraAnimations);
    ExtraAnims_Reset();

    {
        int sel = SelectedSkater;
        unsigned char *tricks = reinterpret_cast<unsigned char *>(GTricks);
        register int offset asm("$5") = GTricksKeys[sel];
        ExtraAnims_AddTrickTable((short *)(tricks + offset));
    }

    if (GNumberOfPlayers == 2)
    {
        int sel = SelectedSkater2;
        unsigned char *tricks = reinterpret_cast<unsigned char *>(GTricks);
        register int offset asm("$5") = GTricksKeys[sel];
        ExtraAnims_AddTrickTable((short *)(tricks + offset));
    }

    if (Loaded != 0)
    {
        UnloadGTricks();
    }
}

/**
 * CBruce::HandleTricksNew(void) — update tricks pipeline
 *
 * Address: 0x80013EE4
 * Mangled: HandleTricksNew__6CBruce
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_HandleTricks(CBrucePhysicsSlice *bruce)
{
    CBruceScriptQueueSlice *queue = (CBruceScriptQueueSlice *)bruce;
    Physics_UpdateButtons(bruce);
    ((CBruce *)bruce)->MaybeTriggerTrick();
    Physics_UpdateQueue(bruce);
    int scriptState = queue->mDoingAScript;
    if (scriptState)
    {
        Physics_UpdateScript(bruce);
    }
}

/**
 * InterpolateFrame__Fii — Interpolate toward target frame
 * Given current frame and target frame, returns an interpolated frame index.
 * The step size depends on distance: if far (>= 13), steps by 5.
 * If medium (4-12), steps by 3. If close (< 4), steps by 1.
 * Direction is determined by sign of (target - current).
 *
 * Address:   0x80013F30, Size: 100 bytes (25 instructions)
 * Mangled:   InterpolateFrame__Fii
 * Demangled: int InterpolateFrame(int, int)
 * @param Current  Current animation frame.
 * @param Dest  Destination animation frame.
 * @return The next frame after stepping toward Dest.
 */
int InterpolateFrame(int Current, int Dest)
{
    int cur = Current;
    int step;
    register int diff asm("$3");

    diff = Dest - cur;

    if (cur == Dest)
    {
        return cur;
    }

    if (diff < 0)
    {
        step = -1;
    }
    else
    {
        step = 1;
    }

    {
        register int sign asm("$2") = diff >> 31;
        diff = diff ^ sign;
        diff = diff - sign;
    }

    if (diff >= 13)
    {
        return cur + step * 5;
    }

    register int small asm("$2") = (diff < 4);

    if (small == 0)
    {
        int step2 = step * 2;
        step2 = step2 + step;
        return cur + step2;
    }

    return cur + step;
}

/**
 * CalcAnimLeanFrame__6CBruce
 *
 * Address:   0x80013F94
 * Mangled:   CalcAnimLeanFrame__6CBruce
 * Demangled: CBruce::CalcAnimLeanFrame(void)
 *
 * Selects and interpolates the regular and crouched lean animation frames,
 * including truck-squeak triggers at the animation limits.
 */
/* Aliases to reach the debug-anim helpers with their trailing __LINE__ arg. */
extern "C" void CalcLean_RunAnim2(int, int, int, int, int)
    asm("Debug_RunAnim2__6CBruceiiii");
extern "C" void CalcLean_RunAnim3(int, int, int, int, int, int)
    asm("Debug_RunAnim3__6CBruceiiiii");

void Physics_CalcAnimLeanFrame(CBrucePhysicsSlice *bruce)
{
    int MaxLeanAngle;
    int mode;

    MaxLeanAngle = 0x2d000;
    if (!(*(signed char *)((int)&bruce->mAY) < 31 &&
          bruce->mPad->Down.Pressed == 0))
        MaxLeanAngle = 0x5a000;

    if (bruce->mAnimLeanAngle == 0)
        goto final_block;

    if (bruce->mManIsCrouched != 0)
        goto after_region1;

    mode = bruce->CSuper.mAnim;
    if (mode == 0)
        goto region1_body;
    if (mode < 0)
        goto region2_start;
    if (mode >= 11)
        goto region2_start;
    if (mode < 6)
        goto after_region1;

region1_body:
{
    int Frame;

    if (bruce->CSuper.mExtraFlags & 2)
    {
        if (bruce->mAnimLeanAngle < 0)
        {
            Frame = -22 * bruce->mAnimLeanAngle / MaxLeanAngle;
            if (Frame >= 23)
                Frame = 22;
            if (mode != 7)
                bruce->CSuper.mFrame = 0;
            bruce->CSuper.mFrame =
                InterpolateFrame(bruce->CSuper.mFrame, Frame);
            CalcLean_RunAnim2(
                (int)bruce, 7, bruce->CSuper.mFrame,
                bruce->CSuper.mFrame, 0xf0c);
        }
        else
        {
            Frame = 22 * bruce->mAnimLeanAngle / MaxLeanAngle;
            if (Frame >= 23)
                Frame = 22;
            if (mode != 6)
                bruce->CSuper.mFrame = 0;
            if (bruce->CSuper.mFrame < Frame)
                bruce->CSuper.mFrame = bruce->CSuper.mFrame + 1;
            bruce->CSuper.mFrame =
                InterpolateFrame(bruce->CSuper.mFrame, Frame);
            CalcLean_RunAnim2(
                (int)bruce, 6, bruce->CSuper.mFrame,
                bruce->CSuper.mFrame, 0xf16);
        }
    }
    else
    {
        if (bruce->mAnimLeanAngle < 0)
        {
            Frame = -22 * bruce->mAnimLeanAngle / MaxLeanAngle;
            if (Frame >= 23)
                Frame = 22;
            if (mode != 6)
                bruce->CSuper.mFrame = 0;
            bruce->CSuper.mFrame =
                InterpolateFrame(bruce->CSuper.mFrame, Frame);
            CalcLean_RunAnim2(
                (int)bruce, 6, bruce->CSuper.mFrame,
                bruce->CSuper.mFrame, 0xf23);
        }
        else
        {
            Frame = 22 * bruce->mAnimLeanAngle / MaxLeanAngle;
            if (Frame >= 23)
                Frame = 22;
            if (mode != 7)
                bruce->CSuper.mFrame = 0;
            bruce->CSuper.mFrame =
                InterpolateFrame(bruce->CSuper.mFrame, Frame);
            CalcLean_RunAnim2(
                (int)bruce, 7, bruce->CSuper.mFrame,
                bruce->CSuper.mFrame, 0xf2c);
        }
    }
}

after_region1:
    if (bruce->mAnimLeanAngle == 0)
        goto final_block;

region2_start:
    if (bruce->mManIsCrouched == 0)
        goto after_region2;

    mode = bruce->CSuper.mAnim;
    if (mode == 0)
        goto region2_body;
    if (mode < 0)
        goto after_region2;
    if (mode >= 11)
        goto after_region2;
    if (mode < 6)
        goto after_region2;

region2_body:
{
    int Frame;

    if (bruce->CSuper.mExtraFlags & 2)
    {
        if (bruce->mAnimLeanAngle < 0)
        {
            Frame = -12 * bruce->mAnimLeanAngle / MaxLeanAngle;
            if (Frame >= 13)
                Frame = 12;
            if (mode == 7)
            {
                bruce->CSuper.mAnim = 10;
                bruce->CSuper.mFrame = Frame;
            }
            if (bruce->CSuper.mAnim != 10)
                bruce->CSuper.mFrame = 0;
            bruce->CSuper.mFrame =
                InterpolateFrame(bruce->CSuper.mFrame, Frame);
            CalcLean_RunAnim2(
                (int)bruce, 10, bruce->CSuper.mFrame,
                bruce->CSuper.mFrame, 0xf50);
            if (bruce->CSuper.mFrame == 12)
                Physics_HandleTruckSqueak(bruce);
        }
        else
        {
            Frame = 15 * bruce->mAnimLeanAngle / MaxLeanAngle;
            if (Frame >= 16)
                Frame = 15;
            if (mode == 6)
            {
                bruce->CSuper.mAnim = 9;
                bruce->CSuper.mFrame = Frame;
            }
            if (bruce->CSuper.mAnim != 9)
                bruce->CSuper.mFrame = 0;
            bruce->CSuper.mFrame =
                InterpolateFrame(bruce->CSuper.mFrame, Frame);
            CalcLean_RunAnim2(
                (int)bruce, 9, bruce->CSuper.mFrame,
                bruce->CSuper.mFrame, 0xf60);
            if (bruce->CSuper.mFrame == 15)
                Physics_HandleTruckSqueak(bruce);
        }
    }
    else
    {
        if (bruce->mAnimLeanAngle < 0)
        {
            Frame = -15 * bruce->mAnimLeanAngle / MaxLeanAngle;
            if (Frame >= 16)
                Frame = 15;
            if (mode == 6)
            {
                bruce->CSuper.mAnim = 9;
                bruce->CSuper.mFrame = Frame;
            }
            if (bruce->CSuper.mAnim != 9)
                bruce->CSuper.mFrame = 0;
            bruce->CSuper.mFrame =
                InterpolateFrame(bruce->CSuper.mFrame, Frame);
            CalcLean_RunAnim2(
                (int)bruce, 9, bruce->CSuper.mFrame,
                bruce->CSuper.mFrame, 0xf75);
            if (bruce->CSuper.mFrame == 15)
                Physics_HandleTruckSqueak(bruce);
        }
        else
        {
            Frame = 12 * bruce->mAnimLeanAngle / MaxLeanAngle;
            if (Frame >= 13)
                Frame = 12;
            if (mode == 7)
            {
                bruce->CSuper.mAnim = 10;
                bruce->CSuper.mFrame = Frame;
            }
            if (bruce->CSuper.mAnim != 10)
                bruce->CSuper.mFrame = 0;
            bruce->CSuper.mFrame =
                InterpolateFrame(bruce->CSuper.mFrame, Frame);
            CalcLean_RunAnim2(
                (int)bruce, 10, bruce->CSuper.mFrame,
                bruce->CSuper.mFrame, 0xf85);
            if (bruce->CSuper.mFrame == 12)
                Physics_HandleTruckSqueak(bruce);
        }
    }
}

after_region2:
    if (bruce->mAnimLeanAngle != 0)
        return;

final_block:
    {
        int st = bruce->CSuper.mAnim;
        if (st < 6)
            return;
        if (st < 8)
        {
            Physics_DebugRunAnim(bruce, 0, 0xf94);
            return;
        }
        if (st >= 11)
            return;
        if (st < 9)
            return;
        CalcLean_RunAnim3((int)bruce, 8, 0x13, 0x1a, 0x13, 0xf9a);
    }
}

/**
 * HandleSpin__6CBruce
 *
 * Address:   0x800144F8
 * Mangled:   HandleSpin__6CBruce
 * Demangled: CBruce::HandleSpin(void)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_HandleSpin(CBrucePhysicsSlice *bruce)
{
    int DeltaAngle;
    int MaxLeanAngle;
    MATRIX SurfaceRotMatrix;
    SVECTOR RotAngle;
    int LeftPressed;
    int RightPressed;

    /* Apply the three timed auto-rotation channels around the board pivot. */
    if (bruce->mRotateCount[0] != 0 || bruce->mRotateCount[1] != 0 ||
        bruce->mRotateCount[2] != 0)
    {
        int ActualRot[3];
        VECTOR zero;
        VECTOR v;
        VECTOR v2;
        VECTOR v3;

        int i = 0;
        int *pCount = &bruce->mRotateCount[0];
        int *pActual = &ActualRot[0];
        int *pSpeed = &bruce->mRotateSpeed[0];

        for (; i < 3; i++)
        {
            int Ofs = i * 4;
            *pActual = 0;
            if (*pCount != 0)
            {
                int *pTotalBase = bruce->mRotateTotal;
                int Scale = TimeScale;
                int Speed =
                    (PhysicsAbsolute(*pSpeed) * Scale)
                    >> 8;
                int Sign = 1;
                if (*pSpeed < 0)
                    Sign = -1;

                if (*pCount >= Speed)
                {
                    *pCount -= Speed;
                    *pActual = (*pSpeed * Scale) >> 8;
                    *(int *)((char *)pTotalBase + Ofs) += *pActual;
                }
                else
                {
                    *pActual = Sign * *pCount;
                    *pCount = 0;
                    *(int *)((char *)pTotalBase + Ofs) = 0;
                }
            }
            pCount++;
            pActual++;
            pSpeed++;
        }

        zero.vz = 0;
        zero.vy = 0;
        zero.vx = 0;
        TransMatrix(&bruce->mRotationMatrix, &zero);
        TransMatrix(&SurfaceRotMatrix, &zero);

        v.vx = 0;
        v.vy = 0x46000;
        v.vz = 0;
        ApplyMatrixLV(&bruce->mRotationMatrix, &v, &v2);

        RotAngle.vx = 0;
        RotAngle.vz = 0;
        RotAngle.vy = (unsigned short)ActualRot[1] & 0xFFF;
        RotMatrixYXZ(&RotAngle, &SurfaceRotMatrix);
        MulMatrix(&bruce->mRotationMatrix, &SurfaceRotMatrix);
        ApplyMatrixLV(&bruce->mRotationMatrix, &v, &v3);

        bruce->CSuper.CBody.CItem.mPos.x =
            (bruce->CSuper.CBody.CItem.mPos.x + v2.vx) - v3.vx;
        bruce->CSuper.CBody.CItem.mPos.y =
            (bruce->CSuper.CBody.CItem.mPos.y + v2.vy) - v3.vy;
        bruce->CSuper.CBody.CItem.mPos.z =
            (bruce->CSuper.CBody.CItem.mPos.z + v2.vz) - v3.vz;
    }

    /* Queued quarter-spins while grinding or lip-stalling. */
    if ((unsigned int)(bruce->mPhysicsState - 1) < 2)
    {
        if (bruce->mTapTurn > 0)
        {
            Physics_RotateHeadingAndVelocity(
                bruce, (TimeScale * 0x96) >> 8, 0, 0);
            bruce->mTallyAngles += (TimeScale * 0x96) >> 8;
            if (bruce->mTallyAngles >= 0x800)
            {
                bruce->mTallyAngles -= 0x800;
                bruce->mTapTurn--;
                if (bruce->mNoSpinScore == 0)
                    bruce->mNum180Spins++;
            }
        }

        if (bruce->mTapTurn < 0)
        {
            Physics_RotateHeadingAndVelocity(
                bruce, -((TimeScale * 0x96) >> 8), 0, 0);
            bruce->mTallyAngles -= (TimeScale * 0x96) >> 8;
            if (bruce->mTallyAngles < -0x7FF)
            {
                bruce->mTallyAngles += 0x800;
                bruce->mTapTurn++;
                if (bruce->mNoSpinScore == 0)
                    bruce->mNum180Spins++;
            }
        }
    }
    else
    {
        bruce->mTapTurn = 0;
    }

    if (bruce->mTapTurn != 0)
    {
        bruce->mLeanAngle = 0;
        return;
    }

    bruce->mTurning = 0;

    if (bruce->mPhysicsState == 0 || bruce->mPhysicsState == 7)
    {
        int RawDelta;
        if (bruce->mpBoardStats->turning != 1)
        {
            if (bruce->mpBoardStats->turning == 3)
                RawDelta = TimeScale * 0x3E00;
            else
                RawDelta = (TimeScale * 0xF) << 11;
        }
        else
        {
            RawDelta = (TimeScale * 5) << 11;
        }
        DeltaAngle = RawDelta >> 8;

        MaxLeanAngle = 0x2D000;
        LeftPressed =
            ((SControl *)bruce->mPad)->Left.Pressed;
        RightPressed =
            ((SControl *)bruce->mPad)->Right.Pressed;
        bruce->mFirstLeftRightPressed = 0;
        if (LeftPressed == 0 && RightPressed == 0)
            bruce->mTruckSqueak = 0;

        if ((signed char)bruce->mAY >= 0x1F ||
            ((SControl *)bruce->mPad)->Down.Pressed != 0)
        {
            bruce->mDoingSharpTurn = 1;
            int Speed =
                (M3dMaths_SquareRoot0(Mips_CVecDotProd(
                     (CVector *)&bruce->CSuper.CBody.mVel,
                     (CVector *)&bruce->CSuper.CBody.mVel))
                 << 6)
                >> 12;
            DeltaAngle *= 2;
            MaxLeanAngle *= 2;
            bruce->CSuper.CBody.mAcc.x -= bruce->CSuper.CBody.mVel.x / Speed;
            bruce->CSuper.CBody.mAcc.y -= bruce->CSuper.CBody.mVel.y / Speed;
            bruce->CSuper.CBody.mAcc.z -= bruce->CSuper.CBody.mVel.z / Speed;
        }
    }
    else
    {
        int RawDelta;
        if (Pad[0].LeftOne.Pressed != 0 || Pad[0].RightOne.Pressed != 0)
            RawDelta = TimeScale * 0x64000;
        else
            RawDelta = TimeScale * 0xA000;
        DeltaAngle = RawDelta >> 8;
        MaxLeanAngle = 0xA0000;

        LeftPressed = 0;
        if (((SControl *)bruce->mPad)->Left.Pressed != 0 ||
            ((SControl *)bruce->mPad)->LeftOne.Pressed != 0)
            LeftPressed = 1;
        RightPressed = 0;
        if (((SControl *)bruce->mPad)->Right.Pressed != 0 ||
            ((SControl *)bruce->mPad)->RightOne.Pressed != 0)
            RightPressed = 1;

        if (LeftPressed != 0 || RightPressed != 0)
        {
            if (bruce->mFirstLeftRightPressed > 0 &&
                XFrames - bruce->mFirstLeftRightPressed < 8)
            {
                LeftPressed = 0;
                RightPressed = 0;
            }
        }
        else
        {
            bruce->mFirstLeftRightPressed = XFrames;
        }

        LeftPressed |= ((SControl *)bruce->mPad)->LeftOne.Pressed;
        RightPressed |= ((SControl *)bruce->mPad)->RightOne.Pressed;
        if (bruce->mNoSpin != 0)
        {
            RightPressed = 0;
            LeftPressed = 0;
        }
        if (LeftPressed == 0 && RightPressed == 0 &&
            (signed char)bruce->mAX == 0)
            bruce->mLeanAngle = 0;
        bruce->mDoingSharpTurn = 0;
    }

    if ((signed char)bruce->mAX != 0 &&
        LeftPressed == 0 && RightPressed == 0)
    {
        int TargetAngle =
            ((signed char)bruce->mAX * MaxLeanAngle) / 128;
        if (TargetAngle < -MaxLeanAngle)
            TargetAngle = -MaxLeanAngle;
        if (TargetAngle > MaxLeanAngle)
            TargetAngle = MaxLeanAngle;

        if (bruce->mLeanAngle < TargetAngle)
        {
            bruce->mTurning = 1;
            int Diff =
                DeltaAngle
                + (DeltaAngle * ((TargetAngle - bruce->mLeanAngle) >> 12))
                      / ((MaxLeanAngle >> 12) / 2);
            bruce->mLeanAngle += Diff;
            if (bruce->mLeanAngle > TargetAngle)
                bruce->mLeanAngle = TargetAngle;
            bruce->mAnimLeanAngle += DeltaAngle;
            if (bruce->mAnimLeanAngle > TargetAngle)
                bruce->mAnimLeanAngle = TargetAngle;
        }

        if (bruce->mLeanAngle > TargetAngle)
        {
            bruce->mTurning = 1;
            int Diff =
                DeltaAngle
                + (DeltaAngle * ((bruce->mLeanAngle - TargetAngle) >> 12))
                      / ((MaxLeanAngle >> 12) / 2);
            bruce->mLeanAngle -= Diff;
            if (bruce->mLeanAngle < TargetAngle)
                bruce->mLeanAngle = TargetAngle;
            bruce->mAnimLeanAngle -= DeltaAngle;
            if (bruce->mAnimLeanAngle < TargetAngle)
                bruce->mAnimLeanAngle = TargetAngle;
        }
    }
    else if (LeftPressed != 0)
    {
        bruce->mTurning = 1;
        bruce->mLeanAngle -= DeltaAngle;
        if (bruce->mLeanAngle < -MaxLeanAngle)
            bruce->mLeanAngle = -MaxLeanAngle;
        if (bruce->mAnimLeanAngle - DeltaAngle >= -MaxLeanAngle)
            bruce->mAnimLeanAngle -= DeltaAngle;
    }
    else if (RightPressed != 0)
    {
        bruce->mTurning = 1;
        bruce->mLeanAngle += DeltaAngle;
        if (bruce->mLeanAngle > MaxLeanAngle)
            bruce->mLeanAngle = MaxLeanAngle;
        if (bruce->mAnimLeanAngle + DeltaAngle <= MaxLeanAngle)
            bruce->mAnimLeanAngle += DeltaAngle;
    }
    else
    {
        int LeanBack;
        if (bruce->mDoingSharpTurn != 0)
            bruce->mLeanAngle -= bruce->mLeanAngle >> 1;
        else
            bruce->mLeanAngle -= bruce->mLeanAngle >> 2;
        if ((bruce->mLeanAngle + 0x800) >> 12 == 0)
            bruce->mLeanAngle = 0;

        LeanBack = bruce->mAnimLeanAngle >> 4;
        if (LeanBack < 0)
        {
            if (LeanBack >= -0x3FF)
                LeanBack = -0x400;
        }
        if ((unsigned int)(LeanBack - 1) < 0x3FF)
            LeanBack = 0x400;
        bruce->mAnimLeanAngle -= LeanBack;
        if ((bruce->mAnimLeanAngle + 0x800) >> 12 == 0)
            bruce->mAnimLeanAngle = 0;
    }

    bruce->mAnimLeanAngle = bruce->mLeanAngle;
    Physics_CalcAnimLeanFrame(bruce);

    /* Nose/tail shift while grinding without grabs. */
    bruce->mPitchAngle = 0;
    if (bruce->mPhysicsState == 1)
    {
        if (((SControl *)bruce->mPad)->Square.Pressed == 0 &&
            ((SControl *)bruce->mPad)->Circle.Pressed == 0)
        {
            if (((SControl *)bruce->mPad)->Up.Pressed != 0 ||
                (signed char)bruce->mAY < -0x28)
                bruce->mPitchAngle = -((TimeScale * 0x14) >> 8);
            if (((SControl *)bruce->mPad)->Down.Pressed != 0 ||
                (signed char)bruce->mAY >= 0x29)
                bruce->mPitchAngle = (TimeScale * 0x14) >> 8;
        }
    }
}

/**
 * AlignHeadingOnRail__6CBruce
 *
 * Address:   0x80014E7C
 * Mangled:   AlignHeadingOnRail__6CBruce
 * Demangled: CBruce::AlignHeadingOnRail(void)
 *            Align heading on rail: rebuild Right/Up vectors orthogonal to Front
 *            using GTE OP (cross product) twice with a world-up reference.
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_AlignHeadingOnRail(CBrucePhysicsSlice *bruce)
{
    CVector *up = (CVector *)&bruce->mUp;
    bruce->mUp.x = 0;
    up->vy = -0x1000;
    up->vz = 0;
    gte_ldopv1((CVector *)&bruce->mFront);
    gte_ldopv2(up);
    gte_op12();
    gte_stlvnl((CVector *)&bruce->mRight);
    M3dUtils_NormaliseVector((CVector *)&bruce->mRight);
    gte_ldopv1((CVector *)&bruce->mRight);
    gte_ldopv2((CVector *)&bruce->mFront);
    gte_op12();
    gte_stlvnl(up);
    Physics_RotMatrixFromFRUVectors(bruce);
}

// ====== 0x80014F58 CreateRailSparks__6CBruce ===================================
// CreateRailSparks__6CBruce
// Address: 0x80014F58
// Demangled: CBruce::CreateRailSparks(void)
#include "BIT2.h"
void Physics_CreateRailSparks(CBrucePhysicsSlice *bruce)
{
    if (BitCount >= 0xC8)
        return;
    int i = 0;
    Physics_SetAnglesFromHeading(bruce);
    Bit_SetSparkTrajectory(*reinterpret_cast<const CSVector *>(
        &bruce->CSuper.CBody.CItem.mAngles));
    CSVector cone;
    register CSVector *conePtr asm("$5") = &cone;
    cone.vx = 0x1C2;
    conePtr->vy = 0x384;
    conePtr->vz = 0;
    Bit_SetSparkTrajectoryCone(*conePtr);
    int sparkY = 0x19000;
    CVector SparkPos;
    UseCacheForBits = 0;
    SparkPos.vz = 0;
    SparkPos.vx = 0;
    SparkPos.vy = sparkY;
    do
    {
        CGrindSpark *bit = static_cast<CGrindSpark *>(
            Bit_AllocateStorage(BIT2_GRIND_SPARK_STORAGE_SIZE));
        if (bit != 0)
        {
            Bit2_ConstructGrindSpark(
                bit, reinterpret_cast<CBody *>(bruce),
                SparkPos,
                -0x28, 0x1000, 5);
        }
        i++;
    } while (i < 9);
    UseCacheForBits = 1;
}

/**
 * TweakRailScore__6CBruce
 *
 * Address:   0x80015034
 * Mangled:   TweakRailScore__6CBruce
 * Demangled: CBruce::TweakRailScore(void)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_TweakRailScore(CBrucePhysicsSlice *bruce)
{
    CVector localVec =
        *(CVector *)&bruce->CSuper.CBody.CItem.mPos -
        *(CVector *)&bruce->mGrindPos;
    int len = localVec.Length();
    if (len >= 0x1F)
    {
        reinterpret_cast<CBruce *>(bruce)->Trick_HoldTrick(
            bruce->mGrindTweak);
        bruce->mGrindPos = bruce->CSuper.CBody.CItem.mPos;
    }
}

/**
 * Play the grind-off sound that matches the current rail type
 *
 * Address:   0x8001509C
 * Mangled:   HandleGrindOffSound__6CBruce
 * Demangled: CBruce::HandleGrindOffSound(void)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_HandleGrindOffSound(CBrucePhysicsSlice *bruce)
{
    int sfx;
    switch (bruce->mRailType)
    {
    case 0:
        if (bruce->mRailSparks == 0) { SFX_PlayX(6, 0x2000, 0); return; }
        sfx = 6; break;
    case 1:
        if (bruce->mRailSparks == 0)
            sfx = -1;
        else
            sfx = 0x10;
        break;
    case 2:
        (void)*(volatile int *)&bruce->mRailSparks;
        sfx = 0xC6;
        break;
    case 3:
        sfx = 0x10;
        break;
    default:
        return;
    }
    SFX_PlayX(sfx, 0x2000, 0);
}

/**
 * LeaveRail__6CBruce
 *
 * Address:   0x8001513C
 * Mangled:   LeaveRail__6CBruce
 * Demangled: CBruce::LeaveRail(void)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_LeaveRail(CBrucePhysicsSlice *bruce)
{
    CBruceScriptQueueSlice *script = (CBruceScriptQueueSlice *)bruce;
    Physics_StateTransition(bruce, 1, 0x1152);
    bruce->mDontStickTimer = 5;
    bruce->mSameRailTimer = 0xF;
    bruce->mOnRail = 0;
    bruce->mStickToRail = 0;
    bruce->mManIsJumping = 1;
    bruce->mDoingATrick = 0;
    bruce->mNum180Spins = 0;
    bruce->mTallyAngles = 0;
    Physics_HandleGrindOffSound(bruce);
    short *trick = script->mpOffTrick;
    script->mpJumpTrick = 0;
    if (trick != 0)
    {
        Physics_TriggerTrickScript(bruce, trick);
        script->mpOffTrick = 0;
    }
    else
    {
        Physics_DebugRunAnim(bruce, 0xE, 0x1168);
        script->mDoingAScript = 0;
    }
}

/**
 * CBruce::DoOnRailPhysics(void) — advance the skater along the rail he is grinding
 *
 * Address:   0x800151D0
 * Mangled:   DoOnRailPhysics__6CBruce
 * Demangled: CBruce::DoOnRailPhysics(void)
 *
 * Each pass round the loop rebuilds the current segment's unit direction,
 * re-projects velocity and acceleration onto it and works out where along the
 * segment the requested movement lands.  Running off either end walks to the
 * neighbouring node when the joint is close enough to parallel and carries the
 * leftover speed into another pass; anything sharper drops the skater off the
 * rail.  Once the position has settled the grind direction, the balance meter
 * and the rail-lean rotation are updated, and the segment just travelled is
 * probed for geometry that should knock the skater off.
 *
 */
void CBruce::DoOnRailPhysics()
{
#define bruce ((CBrucePhysicsSlice *)this)
    CVector SegStart;
    CVector SegEnd;
    CVector SegDir;
    int Speed;
    int LastSpeed;
    int RemainingSpeed;
    int NextSegment;
    int Length;
    int t;
    int MajorAxis;
    int FirstTimeThroughLoop;
    CVector Gravity;
    int AccMagnitude;

    bruce->mManualLean = 0;
    Physics_Vibrate(bruce, 2, 0, 1);
    FirstTimeThroughLoop = 1;
    if (bruce->mRailSparks != 0)
        Physics_CreateRailSparks(bruce);

    ((CVectorPhysicsOwner *)((int)&bruce->CSuper.CBody.mAcc))->Set(0, 0, 0);

    Gravity.vx = 0;
    Gravity.vy = 0x32C8;
    Gravity.vz = 0;

    Physics_TweakRailScore(bruce);

    do
    {
        NextSegment = Rail_NextNode(*(unsigned short *)((int)&bruce->mCurrentRailSegment));
        Trig_GetPosition(SegStart, bruce->mCurrentRailSegment);
        Trig_GetPosition(SegEnd, NextSegment);

        SegDir = SegEnd - SegStart;
        M3dUtils_NormaliseVector((CVector *)&SegDir);

        Speed = Mips_CVecDotProd(
            &SegDir, (CVector *)&bruce->CSuper.CBody.mVel);

        if (FirstTimeThroughLoop)
        {
            FirstTimeThroughLoop = 0;
            *(CVector *)&bruce->CSuper.CBody.mAcc += Gravity;

            if (Speed == 0)
                Speed = 1000;

            /* Ease the grind speed while it is still inside the sane range. */
            if (PhysicsAbsolute(Speed) < 60000)
            {
                if (SegDir.vy != 0)
                {
                    if (SegDir.vy < 0)
                        Speed -= 2000;
                    else
                        Speed += 2000;
                }
                else
                {
                    Speed = (Speed * 5) / 4;
                }
            }
        }

        /* Keep only the component of the motion that runs along the rail. */
        AccMagnitude = Mips_CVecDotProd(
            &SegDir, (CVector *)&bruce->CSuper.CBody.mAcc);
        bruce->CSuper.CBody.mAcc.x = Mips_FixMul(SegDir.vx, AccMagnitude);
        bruce->CSuper.CBody.mAcc.y = Mips_FixMul(SegDir.vy, AccMagnitude);
        bruce->CSuper.CBody.mAcc.z = Mips_FixMul(SegDir.vz, AccMagnitude);
        bruce->CSuper.CBody.mVel.x = Mips_FixMul(SegDir.vx, Speed);
        bruce->CSuper.CBody.mVel.y = Mips_FixMul(SegDir.vy, Speed);
        bruce->CSuper.CBody.mVel.z = Mips_FixMul(SegDir.vz, Speed);

        MajorAxis = 0;
        if (PhysicsAbsolute(SegDir.vy) > PhysicsAbsolute(SegDir.vx))
        {
            MajorAxis = 1;
            if (PhysicsAbsolute(SegDir.vy) < PhysicsAbsolute(SegDir.vz))
            {
                MajorAxis = 2;
            }
        }
        else
        {
            if (PhysicsAbsolute(SegDir.vx) < PhysicsAbsolute(SegDir.vz))
            {
                MajorAxis = 2;
            }
        }

        switch (MajorAxis)
        {
        case 0:
            Length = ((SegEnd.vx - SegStart.vx) / SegDir.vx) << 12;
            t = ((bruce->CSuper.CBody.CItem.mPos.x - SegStart.vx) / SegDir.vx) << 12;
            break;
        case 1:
            Length = ((SegEnd.vy - SegStart.vy) / SegDir.vy) << 12;
            t = ((bruce->CSuper.CBody.CItem.mPos.y - SegStart.vy) / SegDir.vy) << 12;
            break;
        case 2:
        default:
            Length = ((SegEnd.vz - SegStart.vz) / SegDir.vz) << 12;
            t = ((bruce->CSuper.CBody.CItem.mPos.z - SegStart.vz) / SegDir.vz) << 12;
            break;
        }

        t += (LastSpeed = Speed);

        if (t >= 0 && t <= Length)
        {
            CVector Old = mPos;
            mPos +=
                ((mVel * TimeScale) >> 8)
                + (((mAcc * TimeScaleSquared) >> 8) / 2);
            RemainingSpeed = 0;
        }
        else if (t < 0)
        {
            /* Off the near end: step back a node if the joint is shallow. */
            int PreviousSegment =
                Rail_PrevNode(*(unsigned short *)((int)&bruce->mCurrentRailSegment));
            if (PreviousSegment != 0)
            {
                CVector SegStart2;
                CVector SegEnd2;
                CVector SegDir2;

                Trig_GetPosition(SegStart2, PreviousSegment);
                Trig_GetPosition(
                    SegEnd2, bruce->mCurrentRailSegment);
                SegDir2 = SegEnd2 - SegStart2;
                M3dUtils_NormaliseVector((CVector *)&SegDir2);
                if (Mips_CVecDotProd(&SegDir, &SegDir2) > 0xA48)
                {
                    RemainingSpeed = t;
                    bruce->mCurrentRailSegment = PreviousSegment;
                    *(CVector *)&bruce->CSuper.CBody.CItem.mPos = SegStart;
                    bruce->CSuper.CBody.CItem.mPos.y = bruce->CSuper.CBody.CItem.mPos.y - 0x19000;
                    continue;
                }
            }
            Physics_LeaveRail(bruce);
            RemainingSpeed = 0;
        }
        else
        {
            /* Off the far end: step forward a node if the joint is shallow. */
            int NextSegment2 = Rail_NextNode((unsigned short)NextSegment);
            if (NextSegment2 == 0)
            {
                Physics_LeaveRail(bruce);
                RemainingSpeed = 0;
            }
            else
            {
                CVector SegStart2;
                CVector SegEnd2;
                CVector SegDir2;

                Trig_GetPosition(SegStart2, NextSegment);
                Trig_GetPosition(SegEnd2, NextSegment2);
                SegDir2 = SegEnd2 - SegStart2;
                M3dUtils_NormaliseVector((CVector *)&SegDir2);
                if (Mips_CVecDotProd(&SegDir, &SegDir2) < 0xA49)
                {
                    Physics_LeaveRail(bruce);
                    RemainingSpeed = 0;
                }
                else
                {
                    RemainingSpeed = t - Length;
                    bruce->mCurrentRailSegment = NextSegment;
                    *(CVector *)&bruce->CSuper.CBody.CItem.mPos = SegEnd;
                    bruce->CSuper.CBody.CItem.mPos.y = bruce->CSuper.CBody.CItem.mPos.y - 0x19000;
                }
            }
        }
    } while ((Speed = RemainingSpeed) != 0);

    /* The grind direction always opposes the signed travel along the rail. */
    if (LastSpeed >= 0)
    {
        CVector Negated;

        Physics_VectorNegate(&Negated, &SegDir);
        *(CVector *)&bruce->mRailDir = Negated;
    }
    else
    {
        *(CVector *)&bruce->mRailDir = SegDir;
    }

    *(CVector *)&bruce->mFront =
        *(CVector *)&bruce->mRailDir * bruce->mRailFlip;

    Physics_AlignHeadingOnRail(bruce);

    /* Balance meter: decay the lean, then nudge it by the current tilt. */
    bruce->mGrindLean +=
        ((bruce->mGrindLean / (PHYSICS_RECON_EFFECTIVE_BALANCE(bruce) * 3 + 0x1C))
         * (bruce->mGrindTime + 0x3C)) / 0x50;
    bruce->mGrindLean +=
        (bruce->mGrindLeanDir * (bruce->mGrindTime + 0x3C)) / 0x50;

    bruce->mGrindLeanDir +=
        (Rnd(PhysicsAbsolute(bruce->mGrindLeanDir))
         - PhysicsAbsolute(bruce->mGrindLeanDir) / 2) / 10;

    if (Cheat_L2KillsBaddies != 0)
    {
        bruce->mGrindLean = 0;
    }

    if (bruce->mPad->Left.Pressed == 0
        && (signed char)bruce->mAX >= -0x28
        && bruce->mPad->Right.Pressed == 0
        && (signed char)bruce->mAX < 0x29)
    {
        bruce->mGrindReleasedLR = 1;
    }
    if ((bruce->mPad->Left.Pressed != 0
         || (signed char)bruce->mAX < -0x28)
        && bruce->mGrindReleasedLR != 0)
    {
        bruce->mGrindLeanDir = bruce->mGrindLeanDir - 0x14;
    }
    else if ((bruce->mPad->Right.Pressed != 0
              || (signed char)bruce->mAX >= 0x29)
             && bruce->mGrindReleasedLR != 0)
    {
        bruce->mGrindLeanDir = bruce->mGrindLeanDir + 0x14;
    }
    else if (PhysicsAbsolute(bruce->mGrindLeanDir)
             < 0x14 - PHYSICS_RECON_EFFECTIVE_BALANCE(bruce))
    {
        /*
         * The spread is re-expanded on both sides of the call: the original
         * recomputes the balance stat after `Rnd` rather than holding it.
         */
        bruce->mGrindLeanDir =
            Rnd(0x50 - PHYSICS_RECON_EFFECTIVE_BALANCE(bruce) * 3)
            - (0x50 - PHYSICS_RECON_EFFECTIVE_BALANCE(bruce) * 3) / 2;
    }

    if (PhysicsAbsolute(bruce->mGrindLean) > 4000)
    {
        Physics_Bail(
            bruce, bruce->mRailBailNumber, DO_LIMIT);
        bruce->mRailBailNumber = 0;
        bruce->mRailBails = bruce->mRailBails + 1;
    }

    if (bruce->mInBail == 0 && PhysicsAbsolute(LastSpeed) < 7000)
    {
        Physics_Bail(
            bruce, bruce->mRailBailNumber, DO_LIMIT);
        bruce->mRailBailNumber = 0;
    }

    {
        MATRIX Rail_RotMatrix;
        SVECTOR RotAngle;

        RotAngle.vy = 0;
        RotAngle.vx = 0;
        RotAngle.vz = (bruce->mGrindLean / 10) & 0xFFF;
        RotMatrixYXZ(&RotAngle, &Rail_RotMatrix);
        MulMatrix(&bruce->mRotationMatrix, &Rail_RotMatrix);
        Physics_FRUVectorsFromRotMatrix(bruce);

        /* Probe the segment just travelled for geometry that blocks the grind. */
        if (bruce->mOnRail != 0 && bruce->mGrindTime >= 3)
        {
            SLineInfo line_info;

            line_info.StartCoords.Set(
                bruce->CSuper.CBody.mOldPos.x,
                bruce->CSuper.CBody.mOldPos.y,
                bruce->CSuper.CBody.mOldPos.z);
            line_info.EndCoords.Set(
                bruce->CSuper.CBody.CItem.mPos.x, bruce->CSuper.CBody.CItem.mPos.y, bruce->CSuper.CBody.CItem.mPos.z);
            line_info.StartCoords.vy -= 0x19000;
            line_info.EndCoords.vy -= 0x19000;

            CVector vMovement =
                *(CVector *)&bruce->CSuper.CBody.CItem.mPos - *(CVector *)&bruce->CSuper.CBody.mOldPos;

            if (vMovement != ZeroVector)
            {
                int Speed = M3dMaths_SquareRoot0(
                                Mips_CVecDotProd(
                                    &vMovement, &vMovement))
                            << 6;

                if (Speed < (bruce->mInBail != 0 ? 0x64000 : 0x1E000))
                {
                    if (Speed < 0x800)
                        Speed = 0x800;

                    line_info.EndCoords.vx =
                        bruce->CSuper.CBody.mOldPos.x
                        + ((vMovement.vx
                            * (bruce->mInBail != 0 ? 100 : 30))
                           / Speed) * 0x1000;
                    line_info.EndCoords.vy =
                        bruce->CSuper.CBody.mOldPos.y
                        + ((vMovement.vy
                            * (bruce->mInBail != 0 ? 100 : 30))
                           / Speed) * 0x1000;
                    line_info.EndCoords.vz =
                        bruce->CSuper.CBody.mOldPos.z
                        + ((vMovement.vz
                            * (bruce->mInBail != 0 ? 100 : 30))
                           / Speed) * 0x1000;
                }

                int OldBul = BulletCollisionCheck;
                BulletCollisionCheck = 1;
                M3dColij_InitLineInfo(&line_info);
                M3dZone_LineToItem(&line_info, 1);
                BulletCollisionCheck = OldBul;

                if (line_info.pItem != 0)
                {
                    bruce->CSuper.CBody.CItem.mPos = bruce->CSuper.CBody.mOldPos;
                    Physics_LeaveRail(bruce);
                }
            }
        }
    }
}
#undef bruce

/**
 * SkateOffGround__6CBruce
 *
 * Address:   0x80015F04
 * Mangled:   SkateOffGround__6CBruce
 * Demangled: CBruce::SkateOffGround(void)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_SkateOffGround(CBrucePhysicsSlice *bruce)
{
    if (bruce->mPhysicsState == 2)
    {
        Physics_StateTransition(bruce, 1, 0x12E0);
        return;
    }
    Physics_StateTransition(bruce, 1, 0x12E6);
    reinterpret_cast<CBruce *>(bruce)->Trick_TakeOff();
    int bailOnLand = bruce->mInBail;
    bruce->mTallyAngles = 0;
    bruce->mNum180Spins = 0;
    bruce->mDoingATrick = 0;
    bruce->mManIsJumping = 1;
    bruce->mManual = 0;
    bruce->mCess = 0;
    if (bailOnLand != 0)
        return;
    if (bruce->mManIsCrouched != 0)
    {
        Physics_DebugRunAnim(bruce, 0x1A, 0x12F9);
    }
    else
    {
        Physics_DebugRunAnim(bruce, 0x1B, 0x12FB);
    }
}

/**
 * CheckSide__6CBruceiii
 *
 * Address:   0x80015FA8
 * Mangled:   CheckSide__6CBruceiii
 * Demangled: CBruce::CheckSide(int, int, int)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 * @param Side  Signed side of the skater to probe.
 * @param SideCol  Probe distance along the skater's right vector.
 * @param BounceOff  Whether a collision reflects the skater away from the wall.
 * @return 1 when the skater can be moved to the resolved side position; otherwise 0.
 */
int Physics_CheckSide(
    CBrucePhysicsSlice *bruce, int Side, int SideCol, int BounceOff)
{
    SLineInfo line_info;

    line_info.StartCoords.vx =
        bruce->CSuper.CBody.CItem.mPos.x + bruce->mUp.x * 0x1E;
    line_info.EndCoords.vx =
        line_info.StartCoords.vx + Side * bruce->mRight.x * SideCol;
    line_info.StartCoords.vy =
        bruce->CSuper.CBody.CItem.mPos.y + bruce->mUp.y * 0x1E;
    line_info.EndCoords.vy =
        line_info.StartCoords.vy + Side * bruce->mRight.y * SideCol;
    line_info.StartCoords.vz =
        bruce->CSuper.CBody.CItem.mPos.z + bruce->mUp.z * 0x1E;
    line_info.EndCoords.vz =
        line_info.StartCoords.vz + Side * bruce->mRight.z * SideCol;

    M3dColij_InitLineInfo(&line_info);
    M3dZone_LineToItem(&line_info, 1);
    GetFaceFlags(&line_info);

    if (line_info.pItem != 0)
    {
        CVector WallFloorNormal;
        WallFloorNormal.vx = line_info.Normal.vx;
        WallFloorNormal.vy = line_info.Normal.vy;
        WallFloorNormal.vz = line_info.Normal.vz;
        if ((unsigned int)(Mips_CVecDotProd(&WallFloorNormal, (CVector *)&bruce->mUp) + 0x3FF) < 0x7FF)
        {
            int TurnAway = 0;
            bruce->CSuper.CBody.CItem.mPos = bruce->CSuper.CBody.mOldPos;
            CVector OldVel = *(CVector *)&bruce->CSuper.CBody.mVel;

            if (BounceOff != 0)
            {
                if (bruce->CSuper.mAnim != 0xC
                    && OldVel.Length() > 6
                    && bruce->mInBail == 0
                    && bruce->mDoingWhatTrick == 0)
                {
                    CVector WallFloorNormal;
                    WallFloorNormal.vx = line_info.Normal.vx;
                    WallFloorNormal.vy = line_info.Normal.vy;
                    WallFloorNormal.vz = line_info.Normal.vz;
                    if (Mips_CVecDotProd(&WallFloorNormal, (CVector *)&bruce->mFront) > 0xB50)
                    {
                        reinterpret_cast<CBruce *>(bruce)->EndManual();
                        Physics_DebugRunAnim(bruce, 0xC, 0x1338);
                        TurnAway = 0xC8;
                        SFX_Grunt(reinterpret_cast<CBruce *>(bruce));
                    }
                }
                Physics_ReflectHeadingVelocityInPlane(
                    bruce, line_info.Normal, TurnAway);
            }

            line_info.StartCoords.vx = bruce->CSuper.CBody.CItem.mPos.x;
            line_info.StartCoords.vy = bruce->CSuper.CBody.CItem.mPos.y;
            line_info.StartCoords.vz = bruce->CSuper.CBody.CItem.mPos.z;
            line_info.EndCoords.vx =
                line_info.StartCoords.vx + line_info.Normal.vx * SideCol;
            line_info.EndCoords.vy =
                line_info.StartCoords.vy + line_info.Normal.vy * SideCol;
            line_info.EndCoords.vz =
                line_info.StartCoords.vz + line_info.Normal.vz * SideCol;
            M3dColij_InitLineInfo(&line_info);
            M3dZone_LineToItem(&line_info, 1);
            GetFaceFlags(&line_info);
            if (line_info.pItem != 0)
                return 0;
            bruce->CSuper.CBody.CItem.mPos.x = line_info.EndCoords.vx;
            bruce->CSuper.CBody.CItem.mPos.y = line_info.EndCoords.vy;
            bruce->CSuper.CBody.CItem.mPos.z = line_info.EndCoords.vz;
            return 1;
        }
    }
    return 0;
}

/**
 * DoTerrainEffects__6CBruce
 *
 * Address:   0x800162CC
 * Mangled:   DoTerrainEffects__6CBruce
 * Demangled: CBruce::DoTerrainEffects(void)
 *
 * Emit terrain-colored landing sparks and scale controller vibration by the
 * skater's current speed.
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_DoTerrainEffects(CBrucePhysicsSlice *bruce)
{
    CVector *a = (CVector *)&bruce->CSuper.CBody.mVel;
    int Speed = M3dMaths_SquareRoot0(
                    Mips_CVecDotProd(a, a))
                << 6;

    if (((unsigned int)(bruce->mTerrain - 5) < 2 || bruce->mTerrain == 3)
        && BitCount < 200)
    {
        Physics_SetAnglesFromHeading(bruce);
        Bit_SetSparkTrajectory(*reinterpret_cast<const CSVector *>(
            &bruce->CSuper.CBody.CItem.mAngles));
        CSVectorPhysicsOwner cone;
        cone.Set(0x200, 0x1000, 0);
        Bit_SetSparkTrajectoryCone(
            *reinterpret_cast<const CSVector *>(&cone));
        Bit_SetSparkSize(1);
        if (Speed < 0x10001)
            return;

        unsigned char r;
        unsigned char g;
        unsigned char b;
        switch (bruce->mTerrain)
        {
        case 5:
            r = 2;
            g = 0xA0;
            goto common_blue;
        case 6:
            r = 0xA0;
            g = 0xA0;
            b = 0xA0;
            goto set_spark_rgb;
        case 3:
            r = 0x80;
            g = 0x40;
            goto common_blue;
        default:
            goto fade_spark;
        }
    common_blue:
        b = 0x20;
    set_spark_rgb:
        Bit_SetSparkRGB(r, g, b);
    fade_spark:
        Bit_SetSparkFadeRGB(0, 0, 0);

        UseCacheForBits = 0;
        int i = 0;
        do
        {
            if (Mem_GetFree(1) > 10000)
            {
                CBitVelocityPhysicsOwner *pSpark =
                    static_cast<CBitVelocityPhysicsOwner *>(
                        Bit_AllocateStorage(BIT_SPARK_STORAGE_SIZE));
                if (pSpark != 0)
                    pSpark = reinterpret_cast<CBitVelocityPhysicsOwner *>(
                        Physics_ConstructSpark(
                            reinterpret_cast<CSpark *>(pSpark),
                            *(CVector *)&bruce->CSuper.CBody.CItem.mPos,
                            0x14, Speed >> 0x10, 0x14));
                pSpark->mVel += *a;
            }
            i++;
        } while (i < 6);
        UseCacheForBits = 1;
    }

    if (0x10000 < Speed)
    {
        Speed >>= 0xC;
        if (0x40 < Speed)
            Speed = 0x40;
        switch (bruce->mTerrain)
        {
        case 0:
        case 1:
        case 2:
        case 4:
            return;
        case 5:
            Physics_Vibrate(bruce, 8, 1, 0x2580 / Speed);
            break;
        case 6:
            Physics_Vibrate(bruce, 8, 1, 0x1180 / Speed);
            break;
        case 3:
            Physics_Vibrate(bruce, 8, 1, 0x3FC0 / Speed);
            break;
        default:
            return;
        }
    }
}

/**
 * DoManualPhysics__6CBruce
 *
 * Address:   0x800164FC
 * Mangled:   DoManualPhysics__6CBruce
 * Demangled: CBruce::DoManualPhysics(void)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_DoManual(CBrucePhysicsSlice *bruce)
{

    if (bruce->mManual == 0)
    {
        return;
    }

    /* Bleed the manual's stored score off while it is held. */
    if (bruce->mManualCheese != 0)
    {
        int decayed = bruce->mManualCheese - 200;
        bruce->mManualCheese = decayed;
        if (decayed < 0)
        {
            bruce->mManualCheese = 0;
        }
    }

    reinterpret_cast<CBruce *>(bruce)->Trick_HoldTrick(1);

    int manualTime = bruce->mManualTime;
    bruce->mManualTime = manualTime + 1;

    /* Balance stat plus the board's contribution, boosted while fire is lit. */
    int lean = bruce->mManualLean;
    lean = lean +
        ((lean /
          (PHYSICS_RECON_EFFECTIVE_BALANCE(bruce) * 3 + 0x1C)) *
         (manualTime + 0x1F)) / 0x50;
    bruce->mManualLean = lean;

    bruce->mManualLean = lean +
        (bruce->mManualLeanDir * (bruce->mManualTime + 0x1E)) / 0x50;
    if (Cheat_L2KillsBaddies != 0)
    {
        bruce->mManualLean = 0;
    }

    if (bruce->mPad->Up.Pressed == 0)
    {
        if (*(signed char *)((int)&bruce->mAY) >= -0x28 &&
            bruce->mPad->Down.Pressed == 0 &&
            *(signed char *)((int)&bruce->mAY) < 0x29)
        {
            bruce->mManualReleasedLR = 1;
        }
    }

    if ((bruce->mPad->Up.Pressed != 0 ||
         *(signed char *)((int)&bruce->mAY) < -0x28) &&
        bruce->mManualReleasedLR != 0)
    {
        bruce->mManualLeanDir -= 0x14;
    }
    else if ((bruce->mPad->Down.Pressed != 0 ||
              *(signed char *)((int)&bruce->mAY) >= 0x29) &&
             bruce->mManualReleasedLR != 0)
    {
        bruce->mManualLeanDir += 0x14;
    }
    else if (PhysicsAbsolute(bruce->mManualLeanDir) <
             0x14 - PHYSICS_RECON_EFFECTIVE_BALANCE(bruce))
    {
        bruce->mManualLeanDir =
            Rnd(0x50 - PHYSICS_RECON_EFFECTIVE_BALANCE(bruce) * 3) -
            (0x50 - PHYSICS_RECON_EFFECTIVE_BALANCE(bruce) * 3) / 2;
    }
    else
    {
        bruce->mManualLeanDir += Rnd(4) - 2;
    }

    if (PhysicsAbsolute(bruce->mManualLean) > 4000)
    {
        if (bruce->mManual == 1 && bruce->mManualLean > 0)
        {
            bruce->mManualLean = 0;
            Physics_Bail(bruce, 6, DO_LIMIT);
        }
        else if (bruce->mManual == 2 && bruce->mManualLean < 0)
        {
            bruce->mManualLean = 0;
            Physics_Bail(bruce, 0, DO_LIMIT);
        }
        else
        {
            bruce->mManualLean = 0;
            reinterpret_cast<CBruce *>(bruce)->EndManual();
            Physics_DebugRunAnim(bruce, 5, 0x1408);
            return;
        }
    }
}

/**
 * MoveOriginTo__6CBruceiii - Move CBruce's origin toward a requested XYZ using
 * line collision probes, progressively falling back through axis subsets and
 * restoring BulletCollisionCheck before committing the best non-colliding end.
 *
 * Address:   0x800169F4
 * Mangled:   MoveOriginTo__6CBruceiii
 * Demangled: CBruce::MoveOriginTo(int, int, int)
 *            SLineInfo: StartCoords[0..2] at +0, EndCoords[0..2] at +12, pItem at +104
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param z  Z coordinate.
 */
/** Per-skater head-blood frame table addressed absolutely by DoPhysics. */
#define HEADBLOODATFRAME_CONSUMER_DECLARATION
#include "HEADBLOODATFRAME.h"

void Physics_MoveOriginTo(
    CBrucePhysicsSlice *bruce, int x, int y, int z)
{
    SLineInfo line_info;
    int old_bcc = BulletCollisionCheck;
    BulletCollisionCheck = 1;

    // Iter 1: Full 3D move (XYZ)
    line_info.EndCoords.vx = x;
    line_info.EndCoords.vy = y;
    line_info.StartCoords.vx = bruce->CSuper.CBody.CItem.mPos.x;
    line_info.StartCoords.vy = bruce->CSuper.CBody.CItem.mPos.y;
    line_info.StartCoords.vz = bruce->CSuper.CBody.CItem.mPos.z;
    line_info.EndCoords.vz = z;
    M3dColij_InitLineInfo(&line_info);
    M3dZone_LineToItem(&line_info, 1);
    if (line_info.pItem == 0) goto done;

    // Iter 2: YZ only (reset X to current)
    line_info.EndCoords.vx = bruce->CSuper.CBody.CItem.mPos.x;
    M3dColij_InitLineInfo(&line_info);
    M3dZone_LineToItem(&line_info, 1);
    if (line_info.pItem == 0) goto done;

    // Iter 3: XY only (restore X, reset Z to current)
    line_info.EndCoords.vx = x;
    line_info.EndCoords.vz = bruce->CSuper.CBody.CItem.mPos.z;
    M3dColij_InitLineInfo(&line_info);
    M3dZone_LineToItem(&line_info, 1);
    if (line_info.pItem == 0) goto done;

    // Iter 4: XZ (reset Y to current, restore Z to new)
    line_info.EndCoords.vz = z;
    line_info.EndCoords.vy = bruce->CSuper.CBody.CItem.mPos.y;
    M3dColij_InitLineInfo(&line_info);
    M3dZone_LineToItem(&line_info, 1);
    if (line_info.pItem == 0) goto done;

    // Iter 5: X only (reset Y and Z to current)
    line_info.EndCoords.vx = x;
    line_info.EndCoords.vy = bruce->CSuper.CBody.CItem.mPos.y;
    line_info.EndCoords.vz = bruce->CSuper.CBody.CItem.mPos.z;
    M3dColij_InitLineInfo(&line_info);
    M3dZone_LineToItem(&line_info, 1);
    if (line_info.pItem == 0) goto done;

    // Iter 6: Z only (reset X to current, keep Y current, restore Z to new)
    line_info.EndCoords.vx = bruce->CSuper.CBody.CItem.mPos.x;
    line_info.EndCoords.vy = bruce->CSuper.CBody.CItem.mPos.y;
    line_info.EndCoords.vz = z;
    M3dColij_InitLineInfo(&line_info);
    M3dZone_LineToItem(&line_info, 1);
    if (line_info.pItem == 0) goto done;

    // Iter 7: Y only (keep X current, set Y to new, reset Z to current)
    line_info.EndCoords.vx = bruce->CSuper.CBody.CItem.mPos.x;
    line_info.EndCoords.vy = y;
    line_info.EndCoords.vz = bruce->CSuper.CBody.CItem.mPos.z;
    M3dColij_InitLineInfo(&line_info);
    M3dZone_LineToItem(&line_info, 1);
    if (line_info.pItem == 0) goto done;

    // No movement possible — reset Y to current
    line_info.EndCoords.vy = bruce->CSuper.CBody.CItem.mPos.y;

done:
    BulletCollisionCheck = old_bcc;
    bruce->CSuper.CBody.CItem.mPos.x = line_info.EndCoords.vx;
    bruce->CSuper.CBody.CItem.mPos.y = line_info.EndCoords.vy;
    bruce->CSuper.CBody.CItem.mPos.z = line_info.EndCoords.vz;
}

/**
 * HandleTruckSqueakSound__6CBruce
 *
 * Address:   0x80016BE4
 * Mangled:   HandleTruckSqueakSound__6CBruce
 * Demangled: CBruce::HandleTruckSqueakSound(void)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_HandleTruckSqueak(CBrucePhysicsSlice *bruce)
{
    int shouldSkip = 0;
    int val54 = bruce->CSuper.CBody.mVel.x >> 12;
    if ((u32)(val54 + 1) < 3)
    {
        int val5C = bruce->CSuper.CBody.mVel.z >> 12;
        if (val5C < 2)
        {
            int tmp = val5C < -1;
            shouldSkip = (u32)tmp < 1;
        }
    }
    int snd = bruce->mTruckSqueak;
    if (snd != 0)
        return;
    if (shouldSkip != 0)
        return;
    SFX_PlayX(0xE8, 0x2000, 0);
    bruce->mTruckSqueak = 1;
}

#define PHYSICS_RECON_SKATER_STATS_SPEED                                                  \
    ((((Cheat_StatsOverride != 0                                            \
            ? Cheat_StatsOverride                                          \
            : Stats[bruce->mSkaterType].Speed                              \
                  + ((bruce->mMagicTime != 0 || Cheat_Fire != 0)            \
                         ? 3                                                \
                         : 0)                                               \
                  + (unsigned char)bruce->mpBoardStats->speed)              \
          + 0x27)                                                           \
         << 12)                                                             \
        * 45 / 28)

/**
 * Advance ground movement, resolve wall and floor collisions, and apply the
 * acceleration that keeps the skater aligned to the current riding surface.
 *
 * Address:   0x80016C68
 * Mangled:   DoOnGroundPhysics__6CBruce
 * Demangled: CBruce::DoOnGroundPhysics(void)
 *
 */
void CBruce::DoOnGroundPhysics()
{
#define bruce ((CBrucePhysicsSlice *)this)
    int NewMag;
    CVector vMovement;
    CVector InplaneGravity;
    int Speed;
    int TurnAngle;
    int dot;
    SLineInfo line_info;
    int OnInvisible = bruce->mPhysicsState == 2;
    int AutoKickEnabled;
    CVector Gravity;
    CVector Old;

    if (XFrames - bruce->mLandedTime > 20 && GState != 1)
        Physics_MaybeManual(bruce, 15, 1);

    bruce->mLastGroundPos = *(CVectorOwner *)&mPos;

    if (GGame != 7)
        AutoKickEnabled = reinterpret_cast<SCareerOptionsPhysicsOwner *>(&GCareer)->AutoKickEnabled[bruce->mIndex];
    else
        AutoKickEnabled =
            reinterpret_cast<SCareerOptionsPhysicsOwner *>(&GCareer)->AutoKickEnabled[bruce->mIndex ^ GHorsePlayer];

    Gravity.vx = 0;
    Gravity.vy = 0x1964;
    Gravity.vz = 0;

    if (bruce->mPhysicsState == 0)
        bruce->mFlipAngle = 0;

    Physics_DoTerrainEffects(bruce);
    CVector &velocity = mVel;
    Old = mPos;

    mPos +=
        ((velocity * TimeScale) >> 8)
        + (((mAcc
             * TimeScaleSquared) >> 8) / 2);

    Platform_HandleBounce((CBruce *)bruce);

    if (!OnInvisible
        && bruce->mInBail == 0
        && Physics_FlipToVelocity(bruce) != 0
        && bruce->mPhysicsState == 0
        && XFrames - bruce->mLandedTime > 5
        && bruce->mManual != 0)
    {
        reinterpret_cast<CBruce *>(bruce)->EndManual();
        Physics_DebugRunAnim(bruce, 5, 0x14BF);
    }

    if (bruce->mFlipNum != 0
        || bruce->mTapTurn != 0
        || bruce->mNoSpin != 0)
    {
        bruce->mAutoTurn = 0;
    }

    TurnAngle = bruce->mLeanAngle >> 12;
    bruce->mFallLineAdjustment = 0;
    if (bruce->mAutoTurn != 0)
    {
        if (TurnAngle == 0)
        {
            Physics_AlignToFallLine(bruce, 40);
        }
        else
        {
            SControl *pad = (SControl *)bruce->mPad;
            if (pad->Square.Pressed != 0 || pad->Circle.Pressed != 0)
            {
                bruce->mAutoTurn--;
            }
            else
            {
                bruce->mAutoTurn -= 3;
                if (bruce->mAutoTurn < 0)
                    bruce->mAutoTurn = 0;
            }
        }
        TurnAngle += bruce->mFallLineAdjustment;
    }
    else
    {
        bruce->mAutoTurn = 0;
    }

    Physics_RotateHeadingAndVelocity(
        bruce,
        (TurnAngle * TimeScale) >> 8,
        OnInvisible ^ 1,
        0);

    if (bruce->mManual == 0)
        bruce->mTallyAngles +=
            (TurnAngle * TimeScale) >> 8;

    if (bruce->mTallyAngles >= 0x800)
    {
        bruce->mTallyAngles -= 0x800;
        if (bruce->mNoSpinScore == 0)
            bruce->mNum180Spins++;
    }
    if (bruce->mTallyAngles < -0x7FF)
    {
        bruce->mTallyAngles += 0x800;
        if (bruce->mNoSpinScore == 0)
            bruce->mNum180Spins++;
    }

    line_info.StartCoords.Set(
        mOldPos.vx,
        mOldPos.vy,
        mOldPos.vz);
    line_info.EndCoords.Set(
        mPos.vx,
        mPos.vy,
        mPos.vz);

    vMovement = mPos - mOldPos;

    if (vMovement != ZeroVector)
    {
        Speed = M3dMaths_SquareRoot0(
                    Mips_CVecDotProd(
                        &vMovement, &vMovement))
                << 6;
        if ((bruce->mInBail != 0 && Speed < 0x64000)
            || (bruce->mInBail == 0 && Speed < 0x1E000))
        {
            if (Speed < 0x800)
                Speed = 0x800;

            line_info.EndCoords.vx =
                mOldPos.vx
                + ((vMovement.vx
                    * (bruce->mInBail != 0 ? 100 : 30))
                   / Speed) * 0x1000;
            line_info.EndCoords.vy =
                mOldPos.vy
                + ((vMovement.vy
                    * (bruce->mInBail != 0 ? 100 : 30))
                   / Speed) * 0x1000;
            line_info.EndCoords.vz =
                mOldPos.vz
                + ((vMovement.vz
                    * (bruce->mInBail != 0 ? 100 : 30))
                   / Speed) * 0x1000;
        }

        M3dColij_InitLineInfo(&line_info);
        M3dZone_LineToItem(&line_info, 1);
        GetFaceFlags(&line_info);

        if (line_info.pItem != 0
            && Face_Skateable == 0
            && Face_WallRide == 0
            && Face_Air == 0)
        {
                mPos =
                    mOldPos;

                int TurnAway = 0;
                if (mAnim != 12
                    && bruce->mInBail == 0
                    && bruce->mDoingWhatTrick == 0)
                {
                    CVector WallFloorNormal;
                    WallFloorNormal.vx = line_info.Normal.vx;
                    WallFloorNormal.vy = line_info.Normal.vy;
                    WallFloorNormal.vz = line_info.Normal.vz;
                    if (Mips_CVecDotProd(
                            &WallFloorNormal,
                            (CVector *)&bruce->mFront) > 0xB50)
                    {
                        reinterpret_cast<CBruce *>(bruce)->EndManual();
                        Physics_DebugRunAnim(
                            bruce, 12, 0x153C);
                        TurnAway = 200;
                        SFX_Grunt(reinterpret_cast<CBruce *>(bruce));
                    }
                }
                Physics_ReflectHeadingVelocityInPlane(
                    bruce, line_info.Normal, TurnAway);
        }
        else if (line_info.pItem != 0
                 && (Face_Air == 0 || Face_Visible != 0))
        {
                CVector WallFloorNormal;
                WallFloorNormal.vx = line_info.Normal.vx;
                WallFloorNormal.vy = line_info.Normal.vy;
                WallFloorNormal.vz = line_info.Normal.vz;
                CVector *up = (CVector *)&bruce->mUp;
                int UpDot = Mips_CVecDotProd(
                    &WallFloorNormal, up);

                if ((unsigned int)(UpDot + 0x7FF) < 0xFFF)
                {
                    int normalY = -line_info.Normal.vy;
                    if (normalY > PhysicsAbsolute(
                                      (short)(unsigned short)
                                          line_info.Normal.vx)
                        && normalY > PhysicsAbsolute(
                                          (short)(unsigned short)
                                              line_info.Normal.vz))
                    {
                        Physics_MoveOriginTo(
                            bruce,
                            line_info.Position.vx
                                + line_info.Normal.vx * 30,
                            line_info.Position.vy
                                + line_info.Normal.vy * 30,
                            line_info.Position.vz
                                + line_info.Normal.vz * 30);
                        int normalX = line_info.Normal.vx;
                        int normalY = line_info.Normal.vy;
                        int normalZ = line_info.Normal.vz;
                        bruce->mSameNormal = 0;
                        mNormal.vx =
                            normalX;
                        mNormal.vy =
                            normalY;
                        mNormal.vz =
                            normalZ;
                        Physics_OrientToNormal(bruce);

                        if (bruce->mPhysicsState == 2)
                        {
                            int normalX = line_info.Normal.vx;
                            int normalY = line_info.Normal.vy;
                            int normalZ = line_info.Normal.vz;
                            bruce->mSameNormal = 25;
                            mNormal.vx = normalX;
                            mNormal.vy = normalY;
                            mNormal.vz = normalZ;
                            bruce->mOldNormal.vx = normalX;
                            bruce->mOldNormal.vy = normalY;
                            bruce->mOldNormal.vz = normalZ;

                            ProjectCVectorToPlane(
                                (CVector *)&bruce->mFront,
                                line_info.Normal);
                            M3dUtils_NormaliseVector(
                                (CVector *)&bruce->mFront);
                            bruce->mUp.vx = line_info.Normal.vx;
                            bruce->mUp.vy = line_info.Normal.vy;
                            bruce->mUp.vz = line_info.Normal.vz;

                            CVector *front =
                                (CVector *)&bruce->mFront;
                            gte_ldopv1(front);
                            gte_ldopv2(up);
                            gte_op12();
                            gte_stlvnl((CVector *)&bruce->mRight);
                            Physics_RotMatrixFromFRUVectors(bruce);
                        }
                        Physics_StateTransition(
                            bruce, 0, 0x1576);
                    }
                    else
                    {
                        mPos =
                            mOldPos;
                        CVector OldVel =
                            *(CVector *)&mVel;
                        int TurnAway = 0;

                        if (mAnim != 12
                            && OldVel.Length() > 6
                            && bruce->mInBail == 0
                            && bruce->mDoingWhatTrick == 0)
                        {
                            CVector WallFloorNormal;
                            WallFloorNormal.vx = line_info.Normal.vx;
                            WallFloorNormal.vy = line_info.Normal.vy;
                            WallFloorNormal.vz = line_info.Normal.vz;
                            if (Mips_CVecDotProd(
                                    &WallFloorNormal,
                                    (CVector *)&bruce->mFront) > 0xB50)
                            {
                                reinterpret_cast<CBruce *>(bruce)->EndManual();
                                Physics_DebugRunAnim(
                                    bruce, 12, 0x1589);
                                SFX_Grunt(reinterpret_cast<CBruce *>(bruce));
                                TurnAway = 200;
                            }
                        }

                        Physics_ReflectHeadingVelocityInPlane(
                            bruce,
                            line_info.Normal, TurnAway);
                    }
                }
                else if (line_info.Distance <= (Speed >> 12))
                {
                    Physics_MoveOriginTo(
                        bruce,
                        line_info.Position.vx
                            + line_info.Normal.vx * 30,
                        line_info.Position.vy
                            + line_info.Normal.vy * 30,
                        line_info.Position.vz
                            + line_info.Normal.vz * 30);

                    if (line_info.Normal.vx
                                == mNormal.vx
                        && line_info.Normal.vy
                                == mNormal.vy
                        && line_info.Normal.vz
                                == mNormal.vz)
                    {
                        bruce->mSameNormal++;
                    }
                    else
                    {
                        int normalX = line_info.Normal.vx;
                        int normalY = line_info.Normal.vy;
                        int normalZ = line_info.Normal.vz;
                        bruce->mSameNormal = 0;
                        mNormal.vx =
                            normalX;
                        mNormal.vy =
                            normalY;
                        mNormal.vz =
                            normalZ;
                    }
                    Physics_OrientToNormal(bruce);
                }
        }
    }

    NewMag = M3dMaths_SquareRoot0(
                 Mips_CVecDotProd(
                     &velocity, &velocity))
             << 6;
    if (PHYSICS_RECON_SKATER_STATS_SPEED < NewMag)
        bruce->mTooFast = 1;
    else
        bruce->mTooFast = 0;

    Physics_FRUVectorsFromRotMatrix(bruce);
    line_info.StartCoords.vx =
        mPos.vx + bruce->mUp.vx * 70;
    line_info.StartCoords.vy =
        mPos.vy + bruce->mUp.vy * 70;
    line_info.StartCoords.vz =
        mPos.vz + bruce->mUp.vz * 70;
    line_info.EndCoords.vx =
        line_info.StartCoords.vx - bruce->mUp.vx * 256;
    line_info.EndCoords.vy =
        line_info.StartCoords.vy - bruce->mUp.vy * 256;
    line_info.EndCoords.vz =
        line_info.StartCoords.vz - bruce->mUp.vz * 256;

    M3dColij_InitLineInfo(&line_info);
    M3dZone_LineToItem(&line_info, 1);
    GetFaceFlags(&line_info);

    if (line_info.pItem == 0)
    {
        goto missed_ground;
    }
    else
    {
        bruce->mTerrain = Face_Terrain;
        if (Face_Skateable == 0
            && Face_WallRide == 0
            && Face_Air == 0)
        {
            Physics_StateTransition(
                bruce, 1, 0x15ED);
        }
        else if (line_info.pItem == 0)
        {
            goto missed_ground;
        }
        else
        {
            int d;
            int CheckDist;
            d = line_info.Distance;
            if (bruce->mPhysicsState == 2)
            {
                bruce->mWasAir = 20;
                CheckDist = 160;
            }
            else
            {
                CheckDist = bruce->mWasAir + 0x74;
            }

            if (CheckDist < d)
            {
                SControl *pad = (SControl *)bruce->mPad;
                if (pad->Up.Pressed != 0)
                {
                    CVector TmpCV;
                    CVector LastNormal;
                    TmpCV.vx = line_info.Normal.vx;
                    TmpCV.vy = line_info.Normal.vy;
                    TmpCV.vz = line_info.Normal.vz;
                    LastNormal.vx = mNormal.vx;
                    LastNormal.vy = mNormal.vy;
                    LastNormal.vz = mNormal.vz;
                    dot = Mips_CVecDotProd(
                        &LastNormal, &TmpCV);
                    if ((unsigned int)(dot - 1) < 0xED7)
                        CheckDist *= 2;
                }
            }

            if (CheckDist >= d)
            {
                bruce->mIsOnTaggedPoly =
                    ((unsigned short *)line_info.pFace)[7] & 0x20;

                CVector Orig =
                    *(CVector *)&mPos;
                Physics_MoveOriginTo(
                    bruce,
                    line_info.Position.vx + bruce->mUp.vx * 30,
                    line_info.Position.vy + bruce->mUp.vy * 30,
                    line_info.Position.vz + bruce->mUp.vz * 30);
                CVector Move = Orig - mPos;
                if (Mips_CVecDotProd(
                        &Move, &Move) < 0x1000)
                {
                    mPos = Orig;
                }

                if (Face_Air != 0)
                {
                    if (bruce->mPhysicsState != 2)
                    {
                        if (bruce->mJustUp != 0
                            && XFrames - bruce->mJustUp >= 5
                            && Face_Visible == 0
                            && bruce->mInBail == 0)
                        {
                            SVECTOR Push = line_info.Normal;
                            mVel.vx -= Mips_FixMul(
                                Push.vx, NewMag / 4);
                            mVel.vz -= Mips_FixMul(
                                Push.vz, NewMag / 4);
                            mVel.vy /= 2;
                            if (bruce->mPhysicsState == 0)
                            {
                                bruce->mNum180Spins = 0;
                                bruce->mTallyAngles = 0;
                            }
                            Physics_StateTransition(
                                bruce, 1, 0x1646);
                            reinterpret_cast<CBruce *>(bruce)->Trick_TakeOff();
                            bruce->mIsOnTaggedPoly = 0;
                            Physics_DebugRunAnim(
                                bruce, 14, 0x1649);
                            bruce->mManIsJumping = 1;
                        }
                        else
                        {
                            if (bruce->mManIsJumping == 0
                                && bruce->mInBail == 0)
                            {
                                Physics_DebugRunAnim(
                                    bruce, 14, 0x1651);
                                bruce->mManIsJumping = 1;
                            }
                            reinterpret_cast<CBruce *>(bruce)->Trick_TakeOff();
                            bruce->mTallyAngles = 0;
                            bruce->mNum180Spins = 0;
                            Physics_StateTransition(
                                bruce, 2, 0x1658);
                        }
                    }
                    else
                    {
                        CVector NewNormal;
                        NewNormal.vx = line_info.Normal.vx;
                        NewNormal.vy = line_info.Normal.vy;
                        NewNormal.vz = line_info.Normal.vz;
                        CVector VelNormal =
                            *(CVector *)&mVel;
                        M3dUtils_NormaliseVector(
                            (CVector *)&VelNormal);
                        int Angle = Utils_ArcCos(
                                    Mips_CVecDotProd(
                                            &VelNormal, &NewNormal))
                                    - 0x3FF;
                        int absAngle = PhysicsAbsolute(Angle);
                        if (absAngle >= 3
                            && (Angle < 0 ? -1 : 1)
                                != (bruce->mTotalAirSnap < 0 ? -1 : 1))
                            bruce->mTotalAirSnap = Angle;
                        else
                            bruce->mTotalAirSnap += Angle;
                        if (bruce->mTotalAirSnap < -0x400)
                        {
                            Physics_StateTransition(
                                bruce, 1, 0x1670);
                        }
                    }
                }
                else if (Face_Skateable != 0)
                {
                    if (bruce->mPhysicsState == 0)
                    {
                        CVector NewNormal;
                        CVector LastNormal;
                        NewNormal.vx = line_info.Normal.vx;
                        NewNormal.vy = line_info.Normal.vy;
                        NewNormal.vz = line_info.Normal.vz;
                        LastNormal.vx = mNormal.vx;
                        LastNormal.vy = mNormal.vy;
                        LastNormal.vz = mNormal.vz;
                        int UpDot =
                            Mips_CVecDotProd(
                                &LastNormal, &NewNormal);
                        dot = Mips_CVecDotProd(
                            (CVector *)&bruce->mFront,
                            &NewNormal);
                        if (dot < 0
                            && mVel.vy <= 0
                            && UpDot > 0
                            && UpDot < 3000)
                        {
                            Physics_SkateOffGround(bruce);
                        }
                    }
                    else if (bruce->mPhysicsState != 7)
                    {
                        Physics_HandleBailPerpendicular(
                            bruce, line_info.Normal);
                        Physics_StateTransition(
                            bruce, 0, 0x16A5);
                        bruce->mLandedTime = XFrames;
                    }
                    else
                    {
                        mVel.vx = 0;
                        mVel.vy = 0;
                        mVel.vz = 0;
                    }

                    if (!OnInvisible)
                    {
                        CItemOwner *item =
                            (CItemOwner *)line_info.pItem;
                        bruce->mLastSItem = line_info.pItem;
                        bruce->mLastPSX =
                            PSXRegion[item->mRegion].pPSX;
                    }
                }
                else
                {
                    Physics_SkateOffGround(bruce);
                }

                if (bruce->mPhysicsState != 1)
                {
                    if (line_info.Normal.vx
                                == mNormal.vx
                        && line_info.Normal.vy
                                == mNormal.vy
                        && line_info.Normal.vz
                                == mNormal.vz)
                    {
                        bruce->mSameNormal++;
                    }
                    else
                    {
                        int normalX = line_info.Normal.vx;
                        int normalY = line_info.Normal.vy;
                        int normalZ = line_info.Normal.vz;
                        bruce->mSameNormal = 0;
                        mNormal.vx =
                            normalX;
                        mNormal.vy =
                            normalY;
                        mNormal.vz =
                            normalZ;
                    }
                    Physics_OrientToNormal(bruce);
                }
            }
            else
            {
                bruce->mIsOnTaggedPoly =
                    ((unsigned short *)line_info.pFace)[7] & 0x20;
                Physics_SkateOffGround(bruce);
            }
        }
    }

    goto ground_probe_done;

missed_ground:
    Physics_SkateOffGround(bruce);

ground_probe_done:

    if (bruce->mPhysicsState != 1 && line_info.pItem != 0)
    {
        if (bruce->mPhysicsState == 2)
            InplaneGravity =
                *(CVector *)&bruce->mAirGravity;
        else
            InplaneGravity.Set(0, 0x1964, 0);

        ProjectCVectorToPlane(&InplaneGravity, line_info.Normal);
        *(CVector *)&mAcc += InplaneGravity;
        if (bruce->mPhysicsState != 2)
            bruce->mLastFallLine = *(CVectorOwner *)&InplaneGravity;
        bruce->mLastPolyNormal = line_info.Normal;
        CVectorRotateToPlane(
            &velocity, &line_info.Normal, &velocity);
    }
    else
    {
        *(CVector *)&mAcc += Gravity;
    }

    if (bruce->mPhysicsState == 2 || bruce->mInBail != 0)
        return;

    CVector SideVelocity;
    dot = Mips_CVecDotProd(
        &velocity, (CVector *)&bruce->mRight);
    SideVelocity.vx =
        Mips_FixMul(dot, bruce->mRight.vx);
    SideVelocity.vy =
        Mips_FixMul(dot, bruce->mRight.vy);
    SideVelocity.vz =
        Mips_FixMul(dot, bruce->mRight.vz);
    velocity -= SideVelocity;

    CVector ForwardsVelocity;
    dot = Mips_CVecDotProd(
        &velocity, (CVector *)&bruce->mFront);
    ForwardsVelocity.vx =
        Mips_FixMul(dot, bruce->mFront.vx);
    ForwardsVelocity.vy =
        Mips_FixMul(dot, bruce->mFront.vy);
    ForwardsVelocity.vz =
        Mips_FixMul(dot, bruce->mFront.vz);

    if (bruce->mTooFast == 0)
    {
        if (AutoKickEnabled != 0)
        {
            mAcc.vx -=
                Mips_FixMul(8, ForwardsVelocity.vx);
            mAcc.vy -=
                Mips_FixMul(8, ForwardsVelocity.vy);
            mAcc.vz -=
                Mips_FixMul(8, ForwardsVelocity.vz);
        }
    }
    else
    {
        mAcc.vx -=
            Mips_FixMul(0x52, ForwardsVelocity.vx);
        mAcc.vy -=
            Mips_FixMul(0x52, ForwardsVelocity.vy);
        mAcc.vz -=
            Mips_FixMul(0x52, ForwardsVelocity.vz);
    }

    if (bruce->mTerrain == 5)
    {
        mAcc.vx -=
            Mips_FixMul(0x78, ForwardsVelocity.vx);
        mAcc.vy -=
            Mips_FixMul(0x78, ForwardsVelocity.vy);
        mAcc.vz -=
            Mips_FixMul(0x78, ForwardsVelocity.vz);
    }
    if (bruce->mTerrain == 6)
    {
        mAcc.vx -=
            Mips_FixMul(0x78, ForwardsVelocity.vx);
        mAcc.vy -=
            Mips_FixMul(0x78, ForwardsVelocity.vy);
        mAcc.vz -=
            Mips_FixMul(0x78, ForwardsVelocity.vz);
    }
    if (bruce->mTerrain == 3)
    {
        mAcc.vx -=
            Mips_FixMul(0xB4, ForwardsVelocity.vx);
        mAcc.vy -=
            Mips_FixMul(0xB4, ForwardsVelocity.vy);
        mAcc.vz -=
            Mips_FixMul(0xB4, ForwardsVelocity.vz);
    }
    if (bruce->mManual != 0)
    {
        mAcc.vx -=
            Mips_FixMul(0x14, ForwardsVelocity.vx);
        mAcc.vy -=
            Mips_FixMul(0x14, ForwardsVelocity.vy);
        mAcc.vz -=
            Mips_FixMul(0x14, ForwardsVelocity.vz);
    }
#undef bruce
}

/**
 * arccos(int) — always returns 0
 *
 * Address: 0x8001832C
 *          Target: jr $ra; move $v0, $zero
 * Mangled: arccos__Fi
 *
 * @param a  Unused cosine value retained by the original interface.
 * @return 0
 */
int arccos(int a)
{
    return 0;
}

/**
 * HandleLeaningBail__6CBruceG7SVECTOR
 *
 * Address: 0x80018334
 * Mangled: HandleLeaningBail__6CBruceG7SVECTOR
 *
 * Bail when the skater leans too far into the supplied surface normal, then
 * rebuild the front/up basis and redirect velocity along the new front.
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 * @param v     Surface normal used for the lean test.
 */
void Physics_HandleLeaningBail(
    CBrucePhysicsSlice *bruce, SVECTOR v)
{
    if (bruce->mInBail == 0 && bruce->mPhysicsState != 2)
    {
        CVector TmpCV;
        TmpCV.vx = v.vx;
        TmpCV.vy = v.vy;
        TmpCV.vz = v.vz;

        CVector *up = (CVector *)&bruce->mUp;
        if (Mips_CVecDotProd(up, &TmpCV) < 2000
            && bruce->mUp.vy > -2000)
        {
            Physics_Bail(bruce, 3, DO_LIMIT);

            CVector Temp = *(CVector *)&bruce->mFront;
            CVector negated;
            Physics_VectorNegate(&negated, up);
            *(CVector *)&bruce->mFront = negated;
            *(CVector *)&bruce->mUp = Temp;

            Physics_RotMatrixFromFRUVectors(bruce);

            CVector negatedFront;
            Physics_VectorNegate(&negatedFront, (CVector *)&bruce->mFront);
            int scale = 10;
            Physics_VectorMultiply(&negated, &negatedFront, &scale);
            *(CVector *)&bruce->CSuper.CBody.mVel = negated;
            bruce->CSuper.CBody.mVel.vy = 0;
        }
    }
}

/**
 * HitAirPoly__6CBruceR9SLineInfo
 *
 * Address:   0x80018484
 * Mangled:   HitAirPoly__6CBruceR9SLineInfo
 * Demangled: CBruce::HitAirPoly(SLineInfo &)
 *
 * @param bruce      Skater whose airborne collision is being resolved.
 * @param line_info  Collision query containing the face normal.
 */
void Physics_HitAirPoly(
    CBrucePhysicsSlice *bruce, SLineInfo *line_info)
{
    int NewMag;

    if (bruce->mStickToInvisible != 0)
    {
        if (bruce->mJustUp == 0
            || (XFrames - bruce->mJustUp) < 5
            || Face_Visible != 0)
        {
            Physics_StateTransition(bruce, 2, 0x17E0);
            return;
        }
    }

    Physics_StateTransition(bruce, 1, 0x17E7);

    if (bruce->mPad->Up.Pressed != 0
        || *(signed char *)((int)&bruce->mAY) < -0x28)
    {
        NewMag = M3dMaths_SquareRoot0(
                     Mips_CVecDotProd((CVector *)&bruce->CSuper.CBody.mVel,
                                                    (CVector *)&bruce->CSuper.CBody.mVel)) << 6;
        SVECTOR Push = line_info->Normal;
        bruce->CSuper.CBody.mVel.x -= Mips_FixMul(Push.vx, NewMag / 4);
        bruce->CSuper.CBody.mVel.z -= Mips_FixMul(Push.vz, NewMag / 4);
    }

    bruce->mIsOnTaggedPoly = 0;
    if (bruce->mManIsJumping == 0 || bruce->mDoingATrick == 0)
    {
        reinterpret_cast<CBruce *>(bruce)->Trick_TakeOff();
        Physics_DebugRunAnim(bruce, 0xE, 0x17FC);
        bruce->mManIsJumping = 1;
    }
}

/**
 * MaybeManual__6CBruceii
 *
 * Address:   0x80018610
 * Mangled:   MaybeManual__6CBruceii
 * Demangled: CBruce::MaybeManual(int, int)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 * @param Time  Manual-input timing window.
 * @param FromGround  Whether the manual attempt began on the ground.
 */
void Physics_MaybeManual(CBrucePhysicsSlice *bruce, int Time, int FromGround)
{
    if (bruce->mInBail != 0)
        return;
    if (bruce->CSuper.mAnim == 0x82)
        return;
    if (bruce->mManual != 0)
        return;
    if (bruce->mPhysicsState == 2)
        return;

    int Direction;
    int OtherDirection;
    int FoundFirst = 0;

    /* Walk back through the input ring for a matching up/down pair. */
    int LastEvent = bruce->mEventQueuePos - 1;
    if (LastEvent < 0)
        LastEvent = 0x1F;

    int FirstEvent = -1;
    int Event = Physics_GetPreviousPressedEvent(
        bruce, LastEvent, 0, Time, LastEvent);

    while (Event >= 0)
    {
        if (FoundFirst == 0)
        {
            /*
             * The original tests `FoundFirst` with `bnez $s3` and lets the
             * first-time block fall through, so the zero case is the `if`
             * arm here, not an early `break` after the paired case.
             */
            if ((unsigned char)(bruce->mEventQueue[Event].Button - 1) > 1)
                break;
            Direction = bruce->mEventQueue[Event].Button;
            OtherDirection = 1;
            if (Direction == 1)
                OtherDirection = 2;
            FoundFirst = 1;
            FirstEvent = Event;
        }
        else
        {
            if (bruce->mEventQueue[Event].Button == OtherDirection)
            {
                bruce->mManual = (OtherDirection == 1) ? 1 : 2;
                if (FromGround != 0)
                {
                    bruce->mNum180Spins = 0;
                    bruce->mTallyAngles = 0;
                }
                if (bruce->mNum180Spins == 0)
                    bruce->mPerfectLanding = 0;
                bruce->mEventQueue[FirstEvent].Button = 0;
                bruce->mEventQueue[Event].Button = 0;
            }
            break;
        }

        Event = Physics_GetPreviousPressedEvent(
            bruce, Event, 1, Time, LastEvent);
    }

    if (bruce->mManual == 0)
        return;

    bruce->mManualPos = bruce->CSuper.CBody.CItem.mPos;
    bruce->mManualReleasedLR = 0;

    if (bruce->mManual == 1)
    {
        if (bruce->mManualLeanDir
            < PHYSICS_RECON_EFFECTIVE_BALANCE((int)bruce) * 0x1E)
        {
            bruce->mManualLeanDir =
                (PHYSICS_RECON_EFFECTIVE_BALANCE((int)bruce) * 0xF) << 1;
        }
    }
    else
    {
        if (bruce->mManualLeanDir
            > PHYSICS_RECON_EFFECTIVE_BALANCE((int)bruce) * -0x1E)
        {
            bruce->mManualLeanDir =
                (PHYSICS_RECON_EFFECTIVE_BALANCE((int)bruce) * 0xF) * -2;
        }
    }

    /* Damp the lean by the remaining balance headroom. */
    bruce->mManualLeanDir =
        (bruce->mManualLeanDir
         * (100 - PHYSICS_RECON_EFFECTIVE_BALANCE((int)bruce) * 0xF) / 100
         * (bruce->mManualTime + 0x3C)) / 0x50;

    bruce->mManualLean +=
        bruce->mManualCheese
        * (bruce->mManualLean < 0 ? -1 : 1);
    bruce->mManualLean =
        PhysicsAbsolute(bruce->mManualLean)
        * (bruce->mManualLeanDir < 0 ? -1 : 1);
    bruce->mManualCheese = 4000;

    if (bruce->mPhysicsState == 0)
    {
        if (bruce->mManual == 1)
            Physics_DebugRunAnim(bruce, 0x28, 0x188D);
        else
            Physics_DebugRunAnim(bruce, 0x29, 0x188F);
    }
    else
    {
        printf(PhysicsDiagnosticPool.manualAnimation);
    }
}

/** Return the skater's current fixed-point top speed, including board boosts. */
static inline int PhysicsMaxSpeed(CBrucePhysicsSlice *bruce)
{
    if (Cheat_StatsOverride == 0)
    {
        int stat = Stats[bruce->mSkaterType].Speed;
        int board = (unsigned char)bruce->mpBoardStats->speed;
        if (bruce->mMagicTime != 0 || Cheat_Fire != 0)
            return ((board + stat + 0x2A) << 12) * 45 / 28;
        return ((stat + board + 0x27) << 12) * 45 / 28;
    }
    else
    {
        return ((Cheat_StatsOverride + 0x27) << 12) * 45 / 28;
    }
}

/**
 * DoInAirPhysics__6CBruce
 *
 * Address:   0x80018BD8
 * Mangled:   DoInAirPhysics__6CBruce
 * Demangled: CBruce::DoInAirPhysics(void)
 *
 * Integrates the airborne skater one frame, spins the board through the pitch
 * and lean angles, then sweeps the travelled segment for a collision.  A wall
 * becomes a wall ride — or a bail when the approach is too flat, too oblique,
 * or too soon after the last one — a skateable or air poly becomes a landing,
 * and anything else is a crash that reflects the skater off the surface.  On
 * the way down a second short probe along the board's up axis catches air
 * polys the movement segment missed.
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
extern "C" void Physics_DoInAir(CBrucePhysicsSlice *bruce)
{
    CVector vMovement;
    int Speed;
    int TurnAngle;
    MATRIX SurfaceRotMatrix;
    SVECTOR RotAngle;
    SLineInfo line_info;
    CVector Old;
    CVector &position = *(CVector *)&bruce->CSuper.CBody.CItem.mPos;

    Old = position;

    position +=
        ((*(CVector *)&bruce->CSuper.CBody.mVel * TimeScale) >> 8)
        + (((*(CVector *)&bruce->CSuper.CBody.mAcc
              * TimeScaleSquared) >> 8) / 2);

    Platform_HandleBounce((CBruce *)bruce);

    if (bruce->mPitchAngle != 0)
    {
        VECTOR zero;
        VECTOR v;
        VECTOR v2;
        VECTOR v3;

        zero.vz = 0;
        zero.vy = 0;
        zero.vx = 0;
        TransMatrix(&bruce->mRotationMatrix, &zero);
        TransMatrix(&SurfaceRotMatrix, &zero);

        v.vx = 0;
        v.vy = 70 << 12;
        v.vz = 0;
        ApplyMatrixLV(&bruce->mRotationMatrix, &v, &v2);

        RotAngle.vz = 0;
        RotAngle.vy = 0;
        RotAngle.vx = bruce->mPitchAngle & 0xFFF;
        RotMatrixYXZ(&RotAngle, &SurfaceRotMatrix);
        MulMatrix(&bruce->mRotationMatrix, &SurfaceRotMatrix);
        ApplyMatrixLV(&bruce->mRotationMatrix, &v, &v3);

        *(int *)((int)bruce + 4) = *(int *)((int)bruce + 4) + v2.vx - v3.vx;
        *(int *)((int)bruce + 8) = *(int *)((int)bruce + 8) + v2.vy - v3.vy;
        bruce->CSuper.CBody.CItem.mPos.z = bruce->CSuper.CBody.CItem.mPos.z + v2.vz - v3.vz;
    }

    vMovement = position - *(CVector *)&bruce->CSuper.CBody.mOldPos;
    Speed = M3dMaths_SquareRoot0(
                Mips_CVecDotProd(&vMovement, &vMovement))
            << 6;

    int MaxSpeed = PHYSICS_RECON_SKATER_STATS_SPEED;
    if (Speed > MaxSpeed)
        Speed = MaxSpeed;

    TurnAngle = bruce->mLeanAngle >> 12;
    Mips_FixMul(TurnAngle, 0xC00);

    if (TurnAngle != 0)
    {
        RotAngle.vz = 0;
        RotAngle.vx = 0;
        RotAngle.vy = TurnAngle & 0xFFF;
        RotMatrixYXZ(&RotAngle, &SurfaceRotMatrix);
        MulMatrix(&bruce->mRotationMatrix, &SurfaceRotMatrix);

        bruce->mTallyAngles += TurnAngle;
        if (bruce->mTallyAngles >= 0x800)
        {
            bruce->mTallyAngles -= 0x800;
            if (bruce->mNoSpinScore == 0)
                bruce->mNum180Spins++;
        }
        if (bruce->mTallyAngles < -0x7FF)
        {
            bruce->mTallyAngles += 0x800;
            if (bruce->mNoSpinScore == 0)
                bruce->mNum180Spins++;
        }
    }

    if (bruce->mPhysicsState != 2)
        Physics_UprightSideways(bruce);

    line_info.StartCoords.Set(
        bruce->CSuper.CBody.mOldPos.x,
        bruce->CSuper.CBody.mOldPos.y,
        bruce->CSuper.CBody.mOldPos.z);
    line_info.EndCoords.Set(
        *(int *)((int)bruce + 4),
        *(int *)((int)bruce + 8),
        bruce->CSuper.CBody.CItem.mPos.z);
    vMovement = position - *(CVector *)&bruce->CSuper.CBody.mOldPos;

    M3dColij_InitLineInfo(&line_info);
    M3dZone_LineToItem(&line_info, 1);
    GetFaceFlags(&line_info);

    if (line_info.pItem != 0
        && Face_Air != 0
        && Face_Visible == 0
        && bruce->mPhysicsState != 3)
    {
        BulletCollisionCheck = 1;
        M3dColij_InitLineInfo(&line_info);
        M3dZone_LineToItem(&line_info, 1);
        GetFaceFlags(&line_info);
        BulletCollisionCheck = 0;
    }

    if (line_info.pItem != 0)
    {
        if ((Face_Skateable == 0 || (Face_Air != 0 && Face_Visible != 0))
            && Face_WallRide != 0
            && bruce->mInBail == 0
            && (((SControl *)bruce->mPad)->Triangle.Pressed != 0
                || ((SControl *)bruce->mPad)->Triangle.ReleasedTime < 0x14))
        {
            CVector UnitVel;
            CVector FaceNormal;

            int TooFlat = line_info.Normal.vy >= 0xCCE;

            UnitVel = *(CVector *)&bruce->CSuper.CBody.mVel;
            UnitVel.vy = 0;
            M3dUtils_NormaliseVector((CVector *)&UnitVel);

            FaceNormal.vx = line_info.Normal.vx;
            FaceNormal.vy = 0;
            FaceNormal.vz = line_info.Normal.vz;
            M3dUtils_NormaliseVector((CVector *)&FaceNormal);

            int VelNormal =
                Mips_CVecDotProd(&FaceNormal, &UnitVel);

            if (!TooFlat
                && PhysicsAbsolute(VelNormal) < 0xDDB
                && XFrames - bruce->mWallrideTime > 0x28)
            {
                if (bruce->mDoingATrick != 0
                    && bruce->mCanStartNextTrick == 0
                    && bruce->CSuper.mFrame < 10000)
                {
                    Physics_Bail(bruce, 0, DO_LIMIT);
                    bruce->mTrickBails++;
                }
                else
                {
                    bruce->mCanStartNextTrick = 1;
                }

                if (bruce->mInBail == 0)
                {
                    Physics_FrigDarkenObject(bruce);
                    Physics_StateTransition(bruce, 5, 0x19B8);
                    SFX_PlayX(0xB3, 0x2000, 0);

                    bruce->mWallrideTime = XFrames;
                    Physics_MoveOriginTo(
                        bruce,
                        line_info.Position.vx + line_info.Normal.vx * 30,
                        line_info.Position.vy + line_info.Normal.vy * 30,
                        line_info.Position.vz + line_info.Normal.vz * 30);

                    ProjectCVectorToPlane((CVector *)&bruce->mFront,
                                          line_info.Normal);
                    M3dUtils_NormaliseVector((CVector *)&bruce->mFront);
                    bruce->mUp.vx = line_info.Normal.vx;
                    bruce->mUp.vy = line_info.Normal.vy;
                    bruce->mUp.vz = line_info.Normal.vz;
                    gte_ldopv1((CVector *)&bruce->mFront);
                    gte_ldopv2((CVector *)&bruce->mUp);
                    gte_op12();
                    gte_stlvnl((CVector *)&bruce->mRight);
                    Physics_RotMatrixFromFRUVectors(bruce);
                    Physics_FlipToVelocity(bruce);

                    int nx = line_info.Normal.vx;
                    int ny = line_info.Normal.vy;
                    int nz = line_info.Normal.vz;
                    bruce->mSameNormal = 0;
                    bruce->CSuper.CBody.mNormal.vx = nx;
                    bruce->CSuper.CBody.mNormal.vy = ny;
                    bruce->CSuper.CBody.mNormal.vz = nz;
                    bruce->mOldNormal.vx = nx;
                    bruce->mOldNormal.vy = ny;
                    bruce->mOldNormal.vz = nz;
                    ProjectCVectorToPlane(
                        (CVector *)&bruce->CSuper.CBody.mVel,
                        line_info.Normal);

                    gte_ldopv1(&WorldUp);
                    gte_ldopv2((CVector *)&bruce->mUp);
                    gte_op12();
                    CVector WallRight;
                    gte_stlvnl(&WallRight);
                    Physics_RotMatrixFromFRUVectors(bruce);

                    if ((bruce->CSuper.mExtraFlags & 2) != 0)
                    {
                        if (Mips_CVecDotProd(
                                &WallRight,
                                (CVector *)&bruce->CSuper.CBody.mVel) >= 0)
                            Physics_DebugRunAnim(bruce, 0x23, 0x19DF);
                        else
                            Physics_DebugRunAnim(bruce, 0x24, 0x19E3);
                    }
                    else
                    {
                        if (Mips_CVecDotProd(
                                &WallRight,
                                (CVector *)&bruce->CSuper.CBody.mVel) >= 0)
                            Physics_DebugRunAnim(bruce, 0x24, 0x19EA);
                        else
                            Physics_DebugRunAnim(bruce, 0x23, 0x19EE);
                    }
                }
                else
                {
                    bruce->CSuper.CBody.CItem.mPos = bruce->CSuper.CBody.mOldPos;
                }
            }
            else
            {
                bruce->CSuper.CBody.CItem.mPos = bruce->CSuper.CBody.mOldPos;
                printf(PhysicsDiagnosticPool.newBail);
                Physics_Bail(bruce, 0, DO_LIMIT);
            }
        }
        else if (Face_Skateable == 0
                 && (Face_Air == 0 || bruce->mPhysicsState != 3))
        {
            switch (Face_Terrain)
            {
            case 7:
                SFX_PlayX(0x35, 0x2000, 0);
                break;
            case 9:
                if (GLevel == 8)
                    SFX_PlayX(0x89, 0x2000, 0);
                else
                    SFX_PlayX(0x28, 0x2000, 0);
                break;
            }

            if (bruce->mDontStickTimer != 0)
            {
                bruce->mDontStickTimer = 10;
                Physics_RotateHeadingAndVelocity(
                    bruce, Rnd(2) * 0x200 - 0x100, 1, 0);
            }

            bruce->CSuper.CBody.mNormal.vx = 0;
            bruce->CSuper.CBody.mNormal.vy = -0x1000;
            bruce->CSuper.CBody.mNormal.vz = 0;
            setVector(&bruce->mOldNormal, 0, -0x1000, 0);
            bruce->mSameNormal = 0;
            Physics_OrientToNormal(bruce);

            bruce->mNoTrick = 15;
            Physics_MoveOriginTo(
                bruce,
                line_info.Position.vx + line_info.Normal.vx,
                line_info.Position.vy + line_info.Normal.vy,
                line_info.Position.vz + line_info.Normal.vz);
            bruce->mNoTrick = 15;
            Physics_StateTransition(bruce, 1, 0x1A5B);

            bruce->mManIsJumping = 1;
            Physics_ReflectHeadingVelocityInPlane(
                bruce, line_info.Normal, 0);

            bruce->CSuper.CBody.mVel.vx += line_info.Normal.vx / 4;
            bruce->CSuper.CBody.mVel.vy += line_info.Normal.vy / 4;
            bruce->CSuper.CBody.mVel.vz += line_info.Normal.vz / 4;

            if (Rnd(3) == 0)
            {
                bruce->CSuper.CBody.mVel.vx += Rnd(0x4000) - 0x2000;
                bruce->CSuper.CBody.mVel.vz += Rnd(0x4000) - 0x2000;
            }
            return;
        }
        else
        {
            Physics_MoveOriginTo(
                bruce,
                line_info.Position.vx + line_info.Normal.vx * 30,
                line_info.Position.vy + line_info.Normal.vy * 30,
                line_info.Position.vz + line_info.Normal.vz * 30);

            if (bruce->mInBail == 0 && Face_Air != 0)
            {
                Physics_HitAirPoly(bruce, &line_info);
            }
            else
            {
                bruce->mLeanAngle = 0;
                bruce->mpRailTrickQueued = 0;

                if (bruce->mPhysicsState != 3
                    || XFrames - bruce->mJumpFrame > 0x1E)
                {
                    bruce->mTerrain = Face_Terrain;
                    Physics_StateTransition(bruce, 0, 0x1AA3);

                    if (bruce->mInBail == 0)
                    {
                        Physics_HandleBailPerpendicular(
                            bruce, line_info.Normal);
                        Physics_HandleLeaningBail(
                            bruce, line_info.Normal);
                        Physics_HandleBailOnLanding(bruce, 0x1AA9);
                        Physics_MaybeManual(bruce, 0x28, 0);
                    }

                    bruce->mLandedTime = XFrames;
                }
            }

            bruce->mIsOnTaggedPoly =
                *(unsigned short *)((char *)line_info.pFace + 0xE) & 0x20;

            if (bruce->mPhysicsState != 1)
            {
                ProjectCVectorToPlane(
                    (CVector *)&bruce->CSuper.CBody.mVel,
                    line_info.Normal);
                int nx = line_info.Normal.vx;
                int ny = line_info.Normal.vy;
                int nz = line_info.Normal.vz;
                bruce->mSameNormal = 0;
                bruce->CSuper.CBody.mNormal.vx = nx;
                bruce->CSuper.CBody.mNormal.vy = ny;
                bruce->CSuper.CBody.mNormal.vz = nz;
                bruce->mOldNormal.vx = nx;
                bruce->mOldNormal.vy = ny;
                bruce->mOldNormal.vz = nz;
                Physics_OrientToNormal(bruce);
            }
        }
    }
    else
    {
        *(CVector *)&bruce->CSuper.CBody.mAcc +=
            *(CVector *)&bruce->mAirGravity;
    }

    if (bruce->CSuper.CBody.mVel.vy < 0
        && bruce->mPhysicsState == 3
        && XFrames - bruce->mJumpFrame < 0x1E
        && bruce->mInBail == 0)
    {
        line_info.StartCoords.Set(
            bruce->CSuper.CBody.CItem.mPos.x,
            bruce->CSuper.CBody.CItem.mPos.y,
            bruce->CSuper.CBody.CItem.mPos.z);
        line_info.EndCoords.Set(
            bruce->CSuper.CBody.CItem.mPos.x - (bruce->mUp.vx << 7),
            bruce->CSuper.CBody.CItem.mPos.y - (bruce->mUp.vy << 7),
            bruce->CSuper.CBody.CItem.mPos.z - (bruce->mUp.vz << 7));

        M3dColij_InitLineInfo(&line_info);
        M3dZone_LineToItem(&line_info, 1);
        GetFaceFlags(&line_info);

        if (line_info.pItem != 0 && Face_Air != 0)
            Physics_HitAirPoly(bruce, &line_info);
    }
}

#undef PHYSICS_RECON_SKATER_STATS_SPEED

#define PHYSICS_RECON_SKATER_STATS_OLLIE                                             \
    (Cheat_StatsOverride                                              \
         ? Cheat_StatsOverride                                        \
         : (Stats[bruce->mSkaterType].Toughness                       \
            + ((bruce->mMagicTime || Cheat_Fire) ? 3 : 0))            \
               + (unsigned char)bruce->mpBoardStats->ollie)

#define PHYSICS_RECON_SKATER_STATS_AIR                                               \
    (Cheat_StatsOverride                                              \
         ? Cheat_StatsOverride                                        \
         : (Stats[bruce->mSkaterType].Air                             \
            + ((bruce->mMagicTime || Cheat_Fire) ? 3 : 0))            \
               + (unsigned char)bruce->mpBoardStats->air)

#define PHYSICS_RECON_JUMP_MIN (18 - (10 - PHYSICS_RECON_SKATER_STATS_OLLIE) / 3)
#define PHYSICS_RECON_JUMP_MAX (32 - (10 - PHYSICS_RECON_SKATER_STATS_OLLIE) / 2)
#define PHYSICS_RECON_FRAMES_TO_MAX_JUMP_STRENGTH                                    \
    (15 - (PHYSICS_RECON_SKATER_STATS_OLLIE + PHYSICS_RECON_SKATER_STATS_AIR) / 2)

/**
 * Apply the launch velocity and stat-scaled jump force for a foot plant.
 *
 * Address:   0x80019A34
 * Mangled:   DoFootPlantPhysics__6CBruce
 * Demangled: CBruce::DoFootPlantPhysics(void)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_DoFootPlant(CBrucePhysicsSlice *bruce)
{
    CVector launchVelocity;
    CVector reverseHeading;
    int speed[2];

    // "FOOTPLANT" follows "tricks.bin" in PHYSICS's original string table.
    reinterpret_cast<CBruce *>(bruce)->Trick_Trick(
        GTricksFilename + 12, 100, 0);

    bruce->mJumpStrength = PHYSICS_RECON_FRAMES_TO_MAX_JUMP_STRENGTH;
    Physics_StateTransition(bruce, 1, 0x1B24);
    bruce->mManIsJumping = 1;

    Physics_VectorNegate(&reverseHeading, (CVector *)&bruce->mFront);
    speed[0] = reinterpret_cast<const CVector &>(
        bruce->CSuper.CBody.mVel).Length();
    Physics_VectorMultiply(&launchVelocity, &reverseHeading, speed);
    *(CVector *)&bruce->CSuper.CBody.mVel = launchVelocity;
    bruce->CSuper.CBody.mVel.vx += bruce->mFront.vx * 5;
    bruce->CSuper.CBody.mVel.vz += bruce->mFront.vz * 5;

    bruce->CSuper.CBody.mVel.vy -=
        ((bruce->mJumpStrength * ((PHYSICS_RECON_JUMP_MAX - PHYSICS_RECON_JUMP_MIN) << 10))
              / PHYSICS_RECON_FRAMES_TO_MAX_JUMP_STRENGTH
          + PHYSICS_RECON_JUMP_MAX * 0x400)
        * 3;
}

#undef PHYSICS_RECON_FRAMES_TO_MAX_JUMP_STRENGTH
#undef PHYSICS_RECON_JUMP_MAX
#undef PHYSICS_RECON_JUMP_MIN
#undef PHYSICS_RECON_SKATER_STATS_AIR
#undef PHYSICS_RECON_SKATER_STATS_OLLIE

/**
 * DoHandPlantPhysics__6CBruce
 *
 * Address:   0x80019F4C
 * Mangled:   DoHandPlantPhysics__6CBruce
 * Demangled: CBruce::DoHandPlantPhysics(void)
 *
 */
void CBruce::DoHandPlantPhysics()
{
    CBrucePhysicsSlice *bruce = (CBrucePhysicsSlice *)this;
    bruce->mDontStickTimer = 5;

    if (bruce->CSuper.mAnim != 0x22 && bruce->CSuper.mAnim != 0x3F)
    {
        Bail(0, DO_LIMIT);
        return;
    }

    if (bruce->CSuper.mAnim < 0x3E
        && bruce->CSuper.mFrame > 0x13
        && bruce->CSuper.mFrame < 0x19
        && bruce->mHandPlantTweaked >= 0)
    {
        if (*(char *)bruce->mPad != 0)
        {
            bruce->CSuper.mFrame = (short)PingPong(0x14, 0x16, 0xF);
            Trick_HoldTrick(0x14);
            bruce->mHandPlantTweaked++;
            if (bruce->mHandPlantTweaked > 0x14)
            {
                Bail(3, DO_LIMIT);
                return;
            }
        }
        else
        {
            bruce->mHandPlantTweaked = -bruce->mHandPlantTweaked;
        }
    }

    if (bruce->CSuper.mAnimFinished != 0
        || (bruce->CSuper.mAnim < 0x3E && bruce->CSuper.mFrame > 0x1E))
    {
        bruce->mManIsJumping = 0;
        bruce->CSuper.CBody.mVel.y =
            (0x28 - bruce->mHandPlantTweaked * 3) * 0x1000;
        Physics_StateTransition((CBrucePhysicsSlice *)this, 0, 0x1B65);
        bruce->mTrickAwarded = 1;
        reinterpret_cast<CBruce *>(bruce)->Trick_Land();
        bruce->CSuper.CBody.CItem.mPos.x += bruce->mUp.x * 0x1E;
        bruce->CSuper.CBody.CItem.mPos.y += bruce->mUp.y * 0x1E;
        bruce->CSuper.CBody.CItem.mPos.z += bruce->mUp.z * 0x1E;
        if (bruce->CSuper.mAnim >= 0x3E)
        {
            bruce->mFlipAfter = 0;
            bruce->mRotateAfter = 0;
            bruce->CSuper.mExtraFlags ^= 2;
            bruce->mFront.x = bruce->mRotationMatrix.m[0][2] = -bruce->mFront.x;
            bruce->mFront.y = bruce->mRotationMatrix.m[1][2] = -bruce->mFront.y;
            bruce->mFront.z = bruce->mRotationMatrix.m[2][2] = -bruce->mFront.z;
            bruce->mRight.x = bruce->mRotationMatrix.m[0][0] = -bruce->mRight.x;
            bruce->mRight.y = bruce->mRotationMatrix.m[1][0] = -bruce->mRight.y;
            bruce->mRight.z = bruce->mRotationMatrix.m[2][0] = -bruce->mRight.z;
            Physics_DebugRunAnim(bruce, 0, 0x1B7C);
        }
    }
}

/**
 * DoWallRidePhysics__6CBruce
 *
 * Address:   0x8001A1C8
 * Mangled:   DoWallRidePhysics__6CBruce
 * Demangled: CBruce::DoWallRidePhysics(void)
 *
 * Advance the skater along the wall he is riding.  Position is integrated from
 * the time-scaled velocity plus half the time-scaled acceleration, the heading
 * is steered toward the last fall line, and the travelled segment is swept for
 * the wall face: on a hit the skater is re-seated on the surface and, when the
 * face is skateable, the front/right/up basis is rebuilt with the GTE outer
 * product.  A second probe down the wall's own up vector then decides whether
 * the ride continues, whether a near-vertical face throws the skater off, or
 * whether he has left the surface entirely and falls.
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
extern "C" void Physics_DoWallRide(CBrucePhysicsSlice *bruce)
{
    CVector vMovement;
    CVector InplaneGravity;
    int Speed;
    int TurnAngle;
    SLineInfo line_info;
    int OnInvisible;
    CVector Gravity;

    ((CVectorPhysicsOwner *)&Gravity)->Set(0, 0x2328, 0);

    OnInvisible = bruce->mPhysicsState == 2;

    if (bruce->CSuper.mAnimFinished != 0)
    {
        if (bruce->CSuper.mAnim == 0x24)
            bruce->CSuper.mFrame = PingPong(7, 9, 0xF);
        else if (bruce->CSuper.mAnim == 0x23)
            bruce->CSuper.mFrame = PingPong(7, 9, 0xF);
    }

    *(CVector *)&bruce->CSuper.CBody.CItem.mPos +=
        ((*(CVector *)&bruce->CSuper.CBody.mVel * TimeScale) >> 8)
            + (((*(CVector *)&bruce->CSuper.CBody.mAcc
                  * TimeScaleSquared) >> 8) / 2);

    vMovement = *(CVector *)&bruce->CSuper.CBody.CItem.mPos
              - *(CVector *)&bruce->CSuper.CBody.mOldPos;
    Speed = M3dMaths_SquareRoot0(
                Mips_CVecDotProd(&vMovement, &vMovement))
            << 6;
    int MaxSpeed = PhysicsMaxSpeed(bruce);
    if (Speed > MaxSpeed)
        Speed = MaxSpeed;
    TurnAngle = 0;
    if (bruce->mFlipNum == 0 && bruce->mTapTurn == 0)
    {
        Physics_AlignToFallLine(bruce, 4);
        TurnAngle = bruce->mFallLineAdjustment;
    }
    Mips_FixMul(TurnAngle, 0xC00);
    Physics_RotateHeadingAndVelocity(
        bruce,
        (TurnAngle * TimeScale) >> 8, OnInvisible ^ 1, 0);

    line_info.StartCoords.Set(bruce->CSuper.CBody.mOldPos.x,
                              bruce->CSuper.CBody.mOldPos.y,
                              bruce->CSuper.CBody.mOldPos.z);
    line_info.EndCoords.Set(bruce->CSuper.CBody.CItem.mPos.x,
                            bruce->CSuper.CBody.CItem.mPos.y,
                            bruce->CSuper.CBody.CItem.mPos.z);
    vMovement = *(CVector *)&bruce->CSuper.CBody.CItem.mPos
              - *(CVector *)&bruce->CSuper.CBody.mOldPos;

    if (vMovement != ZeroVector)
    {
        Speed = M3dMaths_SquareRoot0(
                    Mips_CVecDotProd(&vMovement, &vMovement))
                << 6;
        if (Speed < (bruce->mInBail != 0 ? 0x64000 : 0x1E000))
        {
            if (Speed < 0x800)
                Speed = 0x800;
            line_info.EndCoords.vx =
                bruce->CSuper.CBody.mOldPos.x
                + ((vMovement.vx * (bruce->mInBail != 0 ? 100 : 30)) / Speed)
                      * 0x1000;
            line_info.EndCoords.vy =
                bruce->CSuper.CBody.mOldPos.y
                + ((vMovement.vy * (bruce->mInBail != 0 ? 100 : 30)) / Speed)
                      * 0x1000;
            line_info.EndCoords.vz =
                bruce->CSuper.CBody.mOldPos.z
                + ((vMovement.vz * (bruce->mInBail != 0 ? 100 : 30)) / Speed)
                      * 0x1000;
        }

        M3dColij_InitLineInfo(&line_info);
        M3dZone_LineToItem(&line_info, 1);
        GetFaceFlags(&line_info);

        if (line_info.pItem != 0 && (Face_Visible != 0 || Face_Air == 0))
        {
            Physics_MoveOriginTo(
                bruce,
                line_info.Position.vx + bruce->mUp.vx * 30,
                line_info.Position.vy + bruce->mUp.vy * 30,
                line_info.Position.vz + bruce->mUp.vz * 30);

            if (!OnInvisible)
            {
                CItemOwner *item = (CItemOwner *)line_info.pItem;
                bruce->mLastSItem = line_info.pItem;
                bruce->mLastPSX = PSXRegion[item->mRegion].pPSX;
            }

            if (line_info.Normal.vx == bruce->CSuper.CBody.mNormal.vx
                && line_info.Normal.vy == bruce->CSuper.CBody.mNormal.vy
                && line_info.Normal.vz == bruce->CSuper.CBody.mNormal.vz)
            {
                bruce->mSameNormal++;
            }
            else
            {
                bruce->mSameNormal = 0;
                bruce->CSuper.CBody.mNormal.Set(line_info.Normal.vx,
                                                line_info.Normal.vy,
                                                line_info.Normal.vz);
            }
            Physics_OrientToNormal(bruce);

            if (Face_Skateable != 0)
            {
                Physics_StateTransition(bruce, 0, 0x1C19);
                ProjectCVectorToPlane(
                    (CVector *)&bruce->mFront, line_info.Normal);
                M3dUtils_NormaliseVector((CVector *)&bruce->mFront);
                bruce->mUp.Set(line_info.Normal.vx,
                               line_info.Normal.vy,
                               line_info.Normal.vz);
                gte_ldopv1(&bruce->mFront);
                gte_ldopv2(&bruce->mUp);
                gte_op12();
                gte_stlvnl(&bruce->mRight);
                Physics_RotMatrixFromFRUVectors(bruce);

                bruce->mSameNormal = 0;
                bruce->CSuper.CBody.mNormal.Set(line_info.Normal.vx,
                                                line_info.Normal.vy,
                                                line_info.Normal.vz);
                bruce->mOldNormal.vx = line_info.Normal.vx;
                bruce->mOldNormal.vy = line_info.Normal.vy;
                bruce->mOldNormal.vz = line_info.Normal.vz;
                ProjectCVectorToPlane(
                    (CVector *)&bruce->CSuper.CBody.mVel, line_info.Normal);
            }
        }
    }

    Physics_FRUVectorsFromRotMatrix(bruce);
    line_info.StartCoords.vx =
        bruce->CSuper.CBody.CItem.mPos.x + bruce->mUp.vx * 70;
    line_info.StartCoords.vy =
        bruce->CSuper.CBody.CItem.mPos.y + bruce->mUp.vy * 70;
    line_info.StartCoords.vz =
        bruce->CSuper.CBody.CItem.mPos.z + bruce->mUp.vz * 70;
    line_info.EndCoords.vx = line_info.StartCoords.vx - bruce->mUp.vx * 128;
    line_info.EndCoords.vy = line_info.StartCoords.vy - bruce->mUp.vy * 128;
    line_info.EndCoords.vz = line_info.StartCoords.vz - bruce->mUp.vz * 128;

    M3dColij_InitLineInfo(&line_info);
    M3dZone_LineToItem(&line_info, 1);
    GetFaceFlags(&line_info);

    if (line_info.pItem != 0)
    {
        int CheckDist = 0x74;
        int d = line_info.Distance;
        if (CheckDist >= d)
        {
            bruce->mIsOnTaggedPoly =
                ((unsigned short *)line_info.pFace)[7] & 0x20;
            Physics_MoveOriginTo(
                bruce,
                line_info.Position.vx + bruce->mUp.vx * 30,
                line_info.Position.vy + bruce->mUp.vy * 30,
                line_info.Position.vz + bruce->mUp.vz * 30);

            if (!OnInvisible)
            {
                CItemOwner *item = (CItemOwner *)line_info.pItem;
                bruce->mLastSItem = line_info.pItem;
                bruce->mLastPSX = PSXRegion[item->mRegion].pPSX;
            }

            if (line_info.Normal.vx == bruce->CSuper.CBody.mNormal.vx
                && line_info.Normal.vy == bruce->CSuper.CBody.mNormal.vy
                && line_info.Normal.vz == bruce->CSuper.CBody.mNormal.vz)
            {
                bruce->mSameNormal++;
            }
            else
            {
                bruce->mSameNormal = 0;
                bruce->CSuper.CBody.mNormal.Set(line_info.Normal.vx,
                                                line_info.Normal.vy,
                                                line_info.Normal.vz);
            }
            Physics_OrientToNormal(bruce);

            if (Face_Skateable != 0)
            {
                Physics_StateTransition(bruce, 0, 0x1C76);
                ProjectCVectorToPlane(
                    (CVector *)&bruce->mFront, line_info.Normal);
                M3dUtils_NormaliseVector((CVector *)&bruce->mFront);
                bruce->mUp.Set(line_info.Normal.vx,
                               line_info.Normal.vy,
                               line_info.Normal.vz);
                gte_ldopv1(&bruce->mFront);
                gte_ldopv2(&bruce->mUp);
                gte_op12();
                gte_stlvnl(&bruce->mRight);
                Physics_RotMatrixFromFRUVectors(bruce);

                bruce->mSameNormal = 0;
                bruce->CSuper.CBody.mNormal.Set(line_info.Normal.vx,
                                                line_info.Normal.vy,
                                                line_info.Normal.vz);
                bruce->mOldNormal.vx = line_info.Normal.vx;
                bruce->mOldNormal.vy = line_info.Normal.vy;
                bruce->mOldNormal.vz = line_info.Normal.vz;
                ProjectCVectorToPlane(
                    (CVector *)&bruce->CSuper.CBody.mVel, line_info.Normal);
            }
            else if (line_info.Normal.vy >= 0xCCE)
            {
                bruce->CSuper.CBody.CItem.mPos = bruce->CSuper.CBody.mOldPos;
                Physics_StateTransition(bruce, 1, 0x1C9C);
                bruce->mManIsJumping = 1;
                *(CVector *)&bruce->CSuper.CBody.mAcc += Gravity;
                Physics_ReflectHeadingVelocityInPlane(
                    bruce, line_info.Normal, 200);
                SFX_Grunt(reinterpret_cast<CBruce *>(bruce));
            }
        }

        InplaneGravity = Gravity;
        ProjectCVectorToPlane(&InplaneGravity, line_info.Normal);
        *(CVector *)&bruce->CSuper.CBody.mAcc += InplaneGravity;
        bruce->mLastFallLine = *(CVectorOwner *)&InplaneGravity;
        bruce->mLastPolyNormal = line_info.Normal;
        CVectorRotateToPlane(
            (CVector *)&bruce->CSuper.CBody.mVel,
            &line_info.Normal,
            (CVector *)&bruce->CSuper.CBody.mVel);
    }
    else
    {
        Physics_StateTransition(bruce, 1, 0x1CC6);
        reinterpret_cast<CBruce *>(bruce)->Trick_TakeOff();
        bruce->mTallyAngles = 0;
        bruce->mNum180Spins = 0;
        bruce->mDoingATrick = 0;
        bruce->mManIsJumping = 1;
        bruce->mManual = 0;
        if (bruce->mInBail == 0)
        {
            if (bruce->mManIsCrouched != 0)
                Physics_DebugRunAnim(bruce, 0x1A, 0x1CE5);
            else
                Physics_DebugRunAnim(bruce, 0x1B, 0x1CE7);
        }
        *(CVector *)&bruce->CSuper.CBody.mAcc += Gravity;
    }
}

#define PHYSICS_RECON_HANDLE_JUMP_OLLIE                                                \
    (Cheat_StatsOverride                                                \
         ? Cheat_StatsOverride                                          \
         : (Stats[bruce->mSkaterType].Toughness                         \
            + ((bruce->mMagicTime || Cheat_Fire) ? 3 : 0))              \
               + (unsigned char)bruce->mpBoardStats->ollie)

#define PHYSICS_RECON_HANDLE_JUMP_AIR                                                  \
    (Cheat_StatsOverride                                                \
         ? Cheat_StatsOverride                                          \
         : (Stats[bruce->mSkaterType].Air                               \
            + ((bruce->mMagicTime || Cheat_Fire) ? 3 : 0))              \
               + (unsigned char)bruce->mpBoardStats->air)

#define PHYSICS_RECON_HANDLE_JUMP_FRAMES                                               \
    (15 - (PHYSICS_RECON_HANDLE_JUMP_OLLIE + PHYSICS_RECON_HANDLE_JUMP_AIR) / 2)
#define PHYSICS_RECON_HANDLE_JUMP_OLLIE_MIN (18 - (10 - PHYSICS_RECON_HANDLE_JUMP_OLLIE) / 3)
#define PHYSICS_RECON_HANDLE_JUMP_OLLIE_MAX (32 - (10 - PHYSICS_RECON_HANDLE_JUMP_OLLIE) / 2)
#define PHYSICS_RECON_HANDLE_JUMP_AIR_MIN (18 - (10 - PHYSICS_RECON_HANDLE_JUMP_AIR) / 3)
#define PHYSICS_RECON_HANDLE_JUMP_AIR_MAX (32 - (10 - PHYSICS_RECON_HANDLE_JUMP_AIR) / 2)

/**
 * HandleJump__6CBruce - Process jump charging and release, landing
 * recovery, rail exits, and the resulting stat-scaled launch velocity.
 *
 * Address:   0x8001AD9C
 * Mangled:   HandleJump__6CBruce
 * Demangled: CBruce::HandleJump(void)
 *
 */
void CBruce::HandleJump()
{
#define bruce ((CBrucePhysicsSlice *)this)
    int Speed;

    if (bruce->mInBail != 0)
    {
        return;
    }

    if (bruce->mManIsCrouched != 0)
    {
        if (bruce->CSuper.mAnim == 0)
        {
            Physics_DebugRunAnim3(
                bruce, 8, 0, 0x1A, 0x13, 0x1D05);
        }
    }
    else if (bruce->CSuper.mAnim == 8)
    {
        if ((signed char)bruce->CSuper.mTargetFrame2 == -1)
        {
            if (bruce->CSuper.mFrame == 0)
                Physics_DebugRunAnim(bruce, 0, 0x1D0D);
        }
        else
        {
            Physics_DebugRunAnim2(
                bruce, 8, 0x13, 0, 0x1D10);
        }
    }

    if (bruce->CSuper.mAnimFinished != 0)
    {
        switch (bruce->CSuper.mAnim)
        {
        case 0x28:
        case 0x29:
            reinterpret_cast<CBruce *>(bruce)->Trick_TakeOff();
            if (bruce->mManual == 1)
                reinterpret_cast<CBruce *>(bruce)->Trick_Trick(
                    GTricksFilename + 0x18, 100, 1);
            else
                reinterpret_cast<CBruce *>(bruce)->Trick_Trick(
                    GTricksFilename + 0x20, 100, 1);
            bruce->CSuper.mAnimMode = 3;
            bruce->CSuper.mPingPongStart = 5;
            bruce->CSuper.mAnimDir = -1;
            bruce->CSuper.mAnimFinished = 0;
            bruce->CSuper.mPingPongEnd = bruce->CSuper.mFrame;
            bruce->CSuper.mPingPongFrame = bruce->CSuper.mFrame;
            break;

        case 8:
            if (bruce->CSuper.mFrame == 0)
                Physics_DebugRunAnim(bruce, 0, 0x1D3D);
            break;

        case 5:
        case 0x19:
            if (bruce->mManIsCrouched != 0)
            {
                Physics_DebugRunAnim3(
                    bruce, 8, 0x13, 0x1A, 0x13, 0x1D4A);
            }
            else
            {
                Physics_DebugRunAnim(bruce, 0, 0x1D4E);
            }
            break;

        case 0x82:
            bruce->mCess = 0;
            if (bruce->mManIsCrouched != 0)
            {
                Physics_DebugRunAnim3(
                    bruce, 8, 0, 0x1A, 0x13, 0x1D56);
            }
            else
            {
                Physics_DebugRunAnim(bruce, 0, 0x1D5A);
            }
            break;

        case 4:
        case 0xD:
            if (bruce->mManIsCrouched == 0)
                Physics_DebugRunAnim(bruce, 0xE, 0x1D65);
            break;

        case 0xE:
            Physics_DebugRunAnim(bruce, 0xE, 0x1D6A);
            break;

        case 0x22:
            if (bruce->mManIsCrouched != 0)
            {
                Physics_DebugRunAnim3(
                    bruce, 8, 0x13, 0x1A, 0x13, 0x1D72);
            }
            else
            {
                Physics_DebugRunAnim2(
                    bruce, 8, 0x13, 0, 0x1D75);
            }
            break;

        case 0:
            if (bruce->mManIsCrouched != 0)
            {
                Physics_DebugRunAnim3(
                    bruce, 8, 0x13, 0x1A, 0x13, 0x1D81);
            }
            else
            {
                Physics_DebugRunAnim(bruce, 0, 0x1D84);
            }
            break;

        default:
            break;
        }
    }
    if (((bruce->mManIsJumping != 0) && (bruce->mOldPhysicsState != 0)) &&
        ((bruce->mPhysicsState == 0 || (bruce->mPhysicsState == 7))))
    {
        bruce->mDoingAScript = 0;
        bruce->mManIsJumping = 0;
        bruce->mStickToInvisible = 0;
        if (bruce->mInBail == 0)
        {
            switch (bruce->mTerrain)
            {
            case 2:
                SFX_PlayX(200, 0x2000, 0);
                break;
            case 10:
            case 0xC:
                SFX_PlayX(0xCB, 0x2000, 0);
                break;
            case 3:
            case 5:
                SFX_PlayX(0xCA, 0x2000, 0);
                break;
            case 6:
                break;
            case 1:
            case 4:
            case 7:
            case 8:
            case 9:
            case 0xB:
            case 0xD:
            case 0xE:
            case 0xF:
            default:
                SFX_PlayX(5, 0x2000, 0);
                break;
            }
            Physics_Vibrate(bruce, 2, 1, 0xFF);
            if (bruce->mManual != 0)
            {
                if (bruce->mManual == 1)
                {
                    Physics_DebugRunAnim(bruce, 0x28, 0x1DC3);
                }
                else
                {
                    Physics_DebugRunAnim(bruce, 0x29, 0x1DC5);
                }
            }
            else
            {
                if (bruce->mManIsCrouched != 0)
                {
                    Physics_DebugRunAnim1(
                        bruce, 0x19, 3, 0x1DCC);
                }
                else
                {
                    Physics_DebugRunAnim1(
                        bruce, 5, 0, 0x1DD0);
                }
            }
            Physics_HandleBailOnLanding(bruce, 0x1DD4);
            if (bruce->mInBail == 0)
            {
                int Boost =
                    bruce->mPanelData.TotalScore() / 100;
                if (Boost > 0x14)
                    Boost = 0x14;
                bruce->mSpeedBoost += Boost;
                if (bruce->mManual == 0)
                {
                    reinterpret_cast<CBruce *>(bruce)->Trick_Land();
                    ((Dust *)&bruce->mDust[0])->Reset(
                        bruce->mTerrain, 0);
                }
            }
        }
        bruce->mDoingATrick = 0;
    }
    if ((bruce->mPad->X.Pressed != 0) &&
        (bruce->mInBail == 0))
    {
        bruce->mJumpStrength++;
        if (PHYSICS_RECON_HANDLE_JUMP_FRAMES < bruce->mJumpStrength)
            bruce->mJumpStrength = PHYSICS_RECON_HANDLE_JUMP_FRAMES;

        int physicsState = bruce->mPhysicsState;
        if ((physicsState == 0 || physicsState == 7) &&
            bruce->mJumpInhibit == 0)
        {
            bruce->mJumpPending = 1;
            if (bruce->mManIsCrouched == 0)
            {
                int lean = bruce->mLeanAngle;
                int sign = lean >> 31;
                if (((lean ^ sign) - sign) < 0xA000 &&
                    (bruce->mLeanAngle = 0,
                     bruce->mManual == 0))
                {
                    Physics_DebugRunAnim3(
                        bruce, 8, 0, 0x1A, 0x13, 0x1E04);
                }
            }
        }
        else
        {
            if (physicsState != 4 || bruce->mJumpInhibit != 0)
            {
                if (physicsState != 5)
                    goto LAB_8001b5a0;
                if (bruce->mJumpInhibit != 0)
                {
                    return;
                }
            }
            bruce->mJumpPending = 1;
        }
    LAB_8001b5a0:
        if (bruce->mJumpInhibit != 0)
        {
            return;
        }
        bruce->mManIsCrouched = 1;
        return;
    }
    bruce->mJumpInhibit = 0;
    bruce->mManIsCrouched = 0;
    if ((0 < bruce->mAirTime) && (0x14 < XFrames - bruce->mAirTime))
    {
        bruce->mJumpPending = 0;
    }
    if (bruce->mJumpPending != 0)
    {
    if ((((GState == 1) && (bruce->mPhysicsState != 4)) && (bruce->mPhysicsState != 5)) &&
        (bruce->mManual == 0))
    {
        bruce->mJumpPending = 0;
        Physics_DebugRunAnim(bruce, 0, 0x1E24);
    }
    else
    {
    int LastPhysics;
    int playedGrindOffSound;
    int JumpBoost;

    bruce->mJumpPending = 0;
    bruce->mManIsJumping = 1;
    bruce->mLastJumpStrength = bruce->mJumpStrength;
    Speed = Mips_CVecDotProd(
        (CVector *)&bruce->CSuper.CBody.mVel,
        (CVector *)&bruce->CSuper.CBody.mVel);
    bruce->mJumpSpeed = M3dMaths_SquareRoot0(Speed) << 6;
    if (bruce->mOnRail != 0)
    {
        bruce->mDoingAScript = 0;
        bruce->mDontStickTimer = 5;
        bruce->mSameRailTimer = 5;
        bruce->mOnRail = 0;
        bruce->mStickToRail = 0;
        bruce->mDoingATrick = 0;
        bruce->mFront = bruce->mRailDir;
        Physics_AlignHeadingOnRail(bruce);
        Speed = Mips_CVecDotProd(
            (CVector *)&bruce->mRailDir,
            (CVector *)&bruce->CSuper.CBody.mVel);
        bruce->CSuper.CBody.mVel.x = Mips_FixMul(bruce->mRailDir.vx, Speed);
        bruce->CSuper.CBody.mVel.y = Mips_FixMul(bruce->mRailDir.vy, Speed);
        bruce->CSuper.CBody.mVel.z = Mips_FixMul(bruce->mRailDir.vz, Speed);
        int Rotate = 0;
        if (bruce->mPad->Left.Pressed != 0)
            Rotate = -200;
        if (bruce->mPad->Right.Pressed != 0)
            Rotate = 200;
        Physics_RotateHeadingAndVelocity(bruce, Rotate, 1, 0);
    }
    bruce->mStickToInvisible = bruce->mIsOnTaggedPoly;
    bruce->mJumpsOffWallRide = (bruce->mPhysicsState == 5);
    LastPhysics = bruce->mPhysicsState;
    if (LastPhysics == 5)
    {
        CVector SurfaceNormal;

        bruce->mJumpStrength = PHYSICS_RECON_HANDLE_JUMP_FRAMES;
        SurfaceNormal.vx = (int)bruce->mLastPolyNormal.vx;
        SurfaceNormal.vy = (int)bruce->mLastPolyNormal.vy;
        SurfaceNormal.vz = (int)bruce->mLastPolyNormal.vz;
        *(CVector *)&bruce->CSuper.CBody.mAcc += SurfaceNormal * 2;
        bruce->CSuper.CBody.mVel.vy += -0xF000;
    }
    playedGrindOffSound = 0;
    if ((bruce->mPhysicsState == 4) && (*(short **)((int)&bruce->mpJumpTrick) != (short *)0x0))
    {
        Physics_TriggerTrickScript(bruce, *(short **)((int)&bruce->mpJumpTrick));
        bruce->mpJumpTrick = 0;
        Physics_HandleGrindOffSound(bruce);
        playedGrindOffSound = 1;
    }
    if (PhysicsAbsolute(bruce->mUp.vy) < 0x9C4)
    {
        JumpBoost =
            -(((bruce->mJumpStrength
                   * ((PHYSICS_RECON_HANDLE_JUMP_AIR_MAX - PHYSICS_RECON_HANDLE_JUMP_AIR_MIN) << 10))
                  / PHYSICS_RECON_HANDLE_JUMP_FRAMES
              + PHYSICS_RECON_HANDLE_JUMP_AIR_MAX * 0x400)
             * 3);
    }
    else
    {
        JumpBoost =
            -(((bruce->mJumpStrength
                   * ((PHYSICS_RECON_HANDLE_JUMP_OLLIE_MAX - PHYSICS_RECON_HANDLE_JUMP_OLLIE_MIN) << 10))
                  / PHYSICS_RECON_HANDLE_JUMP_FRAMES
              + PHYSICS_RECON_HANDLE_JUMP_OLLIE_MAX * 0x400)
             * 3);
    }
    bruce->CSuper.CBody.mVel.vy += JumpBoost;
    bruce->mJumps++;
    if (bruce->mJumpStrength < PHYSICS_RECON_HANDLE_JUMP_FRAMES)
        bruce->mWeakJumps++;
    bruce->mTallyAngles = 0;
    bruce->mNum180Spins = 0;
    if (bruce->mPhysicsState != 2)
    {
        if ((bruce->mStickToInvisible != 0) &&
            (bruce->mPhysicsState == 0))
        {
            Physics_StateTransition(bruce, 3, 0x1E9E);
            bruce->mInAirStickToStart = XFrames;
        }
        else
        {
            Physics_StateTransition(bruce, 1, 0x1EA3);
        }
    }
    if (bruce->mDoingAScript == 0)
    {
        CBruceScriptQueueSlice *script =
            (CBruceScriptQueueSlice *)bruce;
        int Time = 0x14;
        int LastEvent = script->mEventQueuePos - 1;
        if (LastEvent < 0)
            LastEvent = 0x1F;

        int FirstEvent = -1;
        int SecondEvent = -1;
        int Event = Physics_GetPreviousPressedEvent(
            bruce, LastEvent, 0, 0x14, LastEvent);
        while (Event >= 0)
        {
            if (FirstEvent != -1)
            {
                if (script->mEventQueue[Event].Button == 1)
                {
                    SecondEvent = Event;
                    break;
                }
            }
            else if (script->mEventQueue[Event].Button == 1)
            {
                FirstEvent = Event;
                Time = 0x23;
            }

            Event = Physics_GetPreviousPressedEvent(
                bruce, Event, 1, Time, LastEvent);
        }

        if ((((bruce->mPad->LeftTwo.Pressed == 0) && (bruce->mManual == 0)) &&
             (FirstEvent != -1)) &&
            (LastPhysics != 5))
        {
            bruce->mTallyAngles = 0;
            bruce->mNum180Spins = 0;
            if (SecondEvent != -1)
            {
                reinterpret_cast<CBruce *>(bruce)->Trick_TakeOff();
                bruce->CSuper.CBody.mVel.y = bruce->CSuper.CBody.mVel.y + -0x5000;
                reinterpret_cast<CBruce *>(bruce)->Trick_Trick(
                    GTricksFilename + 0x38, 0xFA, 0);
                Physics_DebugRunAnim1(bruce, 0x91, 5, 0x1EE1);
                SFX_PlayX(0xB4, 0x2000, 0);
                Physics_ButtonRemoveLast(
                    bruce, 1, 1);
            }
            else
            {
                reinterpret_cast<CBruce *>(bruce)->Trick_TakeOff();
                reinterpret_cast<CBruce *>(bruce)->Trick_Trick(
                    GTricksFilename + 0x44, 200, 0);
                Physics_DebugRunAnim(bruce, 0xD, 0x1EED);
            }
            Physics_ButtonRemoveLast(
                bruce, 1, 1);
            bruce->mTrickAwarded = 1;
        }
        else
        {
            reinterpret_cast<CBruce *>(bruce)->Trick_TakeOff();
            Physics_DebugRunAnim(bruce, 4, 0x1EF6);
        }
    }
    if (!playedGrindOffSound)
    {
        SFX_PlayX(
            (bruce->mTerrain == 0xC) ? 199 : 6,
            0x2000,
            0);
    }
    bruce->mJumpFrame = XFrames;
    }
    }
    bruce->mJumpStrength = 0;
#undef bruce
    return;
}

#undef PHYSICS_RECON_HANDLE_JUMP_AIR_MAX
#undef PHYSICS_RECON_HANDLE_JUMP_AIR_MIN
#undef PHYSICS_RECON_HANDLE_JUMP_OLLIE_MAX
#undef PHYSICS_RECON_HANDLE_JUMP_OLLIE_MIN
#undef PHYSICS_RECON_HANDLE_JUMP_FRAMES
#undef PHYSICS_RECON_HANDLE_JUMP_AIR
#undef PHYSICS_RECON_HANDLE_JUMP_OLLIE

#define PHYSICS_RECON_KICK_SPEED_VALUE(base)                                      \
    ((((Cheat_StatsOverride != 0                                            \
             ? Cheat_StatsOverride                                         \
             : Stats[((CBrucePhysicsSlice *)(PHYSICS_KICK_ABI_RECEIVER))->mSkaterType].Speed \
                   + ((((CBrucePhysicsSlice *)(PHYSICS_KICK_ABI_RECEIVER))->mMagicTime != 0 \
                           || Cheat_Fire != 0)                              \
                          ? 3                                               \
                          : 0)                                              \
                   + (unsigned char)((CBrucePhysicsSlice *)(PHYSICS_KICK_ABI_RECEIVER))->mpBoardStats->speed) \
           + (base))                                                       \
          << 12)                                                           \
         * 45 / 28)

/**
 * Apply the current kick input and kick-animation impulse.
 *
 * Address:   0x8001C368
 * Mangled:   HandleKick__6CBruce
 * Demangled: CBruce::HandleKick(void)
 */
void CBruce::HandleKick()
{
#define PHYSICS_KICK_ABI_RECEIVER ((int)this)
#define bruce ((CBrucePhysicsSlice *)this)
    int AutoKickEnabled;

    if (GGame != 7)
        AutoKickEnabled = reinterpret_cast<SCareerOptionsPhysicsOwner *>(&GCareer)->AutoKickEnabled[bruce->mIndex];
    else
        AutoKickEnabled =
            reinterpret_cast<SCareerOptionsPhysicsOwner *>(&GCareer)->AutoKickEnabled[bruce->mIndex ^ GHorsePlayer];

    if (bruce->mManual != 0)
        return;

    if (bruce->CSuper.mAnimFinished != 0)
    {
        switch (bruce->CSuper.mAnim)
        {
        case 1:
            Physics_DebugRunAnim(bruce, 3, 0x1F21);
            bruce->CSuper.mAnimSpeed = 0x14000;
            break;
        case 3:
            Physics_DebugRunAnim(bruce, 0, 0x1F26);
            break;
        }
    }

    if (bruce->mPhysicsState != 0)
        return;

    if (bruce->mKickLockout > 0)
        bruce->mKickLockout = bruce->mKickLockout - 1;

    int Speed =
        M3dMaths_SquareRoot0(
            Mips_CVecDotProd(&mVel, &mVel)) << 6;

    CVector mVel2 = mVel;
    ProjectCVectorToPlane(&mVel2, bruce->mLastPolyNormal);
    int x = Mips_CVecDotProd((CVector *)&bruce->mLastFallLine, &mVel2);

    if ((char)bruce->mPad->Square.Pressed != 0 || AutoKickEnabled != 0)
    {
        if (bruce->mDoingSharpTurn == 0 || bruce->mTurning == 0)
        {
            if (bruce->mKickLockout == 0)
            {
                if ((x >= -0x4CC && Speed > bruce->mMaxKickSpeed / 2) ||
                    bruce->mManIsCrouched != 0)
                {
                    bruce->mKickLockout = 0x14;
                }
                else
                {
                    if (bruce->mPhysicsState != 0)
                        goto recompute_speed;

                    {
                        int anim = mAnim;
                        if (anim != 0)
                        {
                            if (anim < 0)
                                goto recompute_speed;
                            if (anim >= 8)
                                goto recompute_speed;
                            if (anim < 5)
                                goto recompute_speed;
                        }
                    }
                    bruce->mMaxKickSpeed =
                        PHYSICS_RECON_KICK_SPEED_VALUE(0x11);
                    bruce->mKickLockout = 0x14;
                    bruce->mFootSound = 1;
                    Physics_DebugRunAnim1(bruce, 1, 0, 0x1F59);
                    bruce->CSuper.mAnimSpeed = 0x14000;
                }
            }

        recompute_speed:
            Speed =
                M3dMaths_SquareRoot0(
                    Mips_CVecDotProd(&mVel, &mVel)) << 6;
            if ((unsigned short)(bruce->CSuper.mAnim - 2) < 2)
            {
                int KickVal = 4;
                if (bruce->CSuper.mFrame > 10 && bruce->mFootSound != 0)
                {
                    int s = 0x22;
                    bruce->mFootSFX = s;
                    SFX_PlayX(s, 0x2000, 0);
                    bruce->mFootSound = 0;
                }
                if ((unsigned short)(*(unsigned short *)((int)&bruce->CSuper.mFrame) - 0xB) < 5)
                {
                    if ((char)bruce->mPad->Square.Pressed != 0)
                        KickVal = 8;
                    if (Speed < bruce->mMaxKickSpeed &&
                        bruce->mPhysicsState != 2)
                    {
                        bruce->CSuper.CBody.mAcc.x = -bruce->mFront.x * KickVal;
                        bruce->CSuper.CBody.mAcc.y = -bruce->mFront.y * KickVal;
                        bruce->CSuper.CBody.mAcc.z = -bruce->mFront.z * KickVal;
                    }
                }
            }
            else if ((unsigned int)(bruce->mKickLockout - 0xB) < 5 &&
                     Speed < bruce->mMaxKickSpeed &&
                     bruce->mPhysicsState != 2)
            {
                bruce->CSuper.CBody.mAcc.x = -bruce->mFront.x * 4;
                bruce->CSuper.CBody.mAcc.y = -bruce->mFront.y * 4;
                bruce->CSuper.CBody.mAcc.z = -bruce->mFront.z * 4;
            }
        }
        if (AutoKickEnabled != 0)
            return;
    }

    if ((char)bruce->mPad->Square.Pressed == 0 || x < -0x4CC ||
        bruce->mManIsCrouched != 0 || bruce->mPhysicsState != 0)
    {
        goto tail;
    }
    {
        int anim = mAnim;
        if (anim != 0)
        {
            if (anim < 0)
                goto tail;
            if (anim >= 8)
                goto tail;
            if (anim < 5)
                goto tail;
        }
    }
    bruce->mMaxKickSpeed = PHYSICS_RECON_KICK_SPEED_VALUE(0x16);
    bruce->mKickLockout = 0x14;
    Physics_DebugRunAnim1(bruce, 1, 0, 0x1FAC);
    bruce->CSuper.mAnimSpeed = 0x14000;
    bruce->mFootSound = 1;

tail:
    {
        Speed =
            M3dMaths_SquareRoot0(
                Mips_CVecDotProd(&mVel, &mVel)) << 6;
        if ((unsigned short)(bruce->CSuper.mAnim - 2) < 2)
        {
            int KickVal = 4;
            if ((unsigned short)(*(unsigned short *)((int)&bruce->CSuper.mFrame) - 0xB) < 5)
            {
                if ((char)bruce->mPad->Square.Pressed != 0)
                    KickVal = 8;
                if (Speed < bruce->mMaxKickSpeed &&
                    bruce->mPhysicsState != 2)
                {
                    bruce->CSuper.CBody.mAcc.x = -bruce->mFront.x * KickVal;
                    bruce->CSuper.CBody.mAcc.y = -bruce->mFront.y * KickVal;
                    bruce->CSuper.CBody.mAcc.z = -bruce->mFront.z * KickVal;
                }
            }
        }
    }
}

#undef bruce
#undef PHYSICS_KICK_ABI_RECEIVER
#undef PHYSICS_RECON_KICK_SPEED_VALUE
/**
 * Rotate the skater heading and optionally rotate the current velocity.
 *
 * Address:   0x8001CA84
 * Mangled:   RotateHeadingAndVelocity__6CBruceiii
 * Demangled: CBruce::RotateHeadingAndVelocity(int, int, int)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 * @param TurnAngle Heading rotation in PSX angle units.
 * @param RotateVelocity Whether the velocity should follow the new heading.
 * @param SlipAngle Heading offset retained by the velocity vector.
 */
void Physics_RotateHeadingAndVelocity(
    CBrucePhysicsSlice *bruce, int TurnAngle, int RotateVelocity, int SlipAngle)
{
    int OldMag;
    int NewMag;
    MATRIX SurfaceRotMatrix;
    SVECTOR RotAngle;

    if (TurnAngle != 0 && bruce->mInBail == 0)
    {
        RotAngle.vz = 0;
        RotAngle.vx = 0;
        RotAngle.vy = (unsigned short)TurnAngle & 0xFFF;
        RotMatrixYXZ(&RotAngle, &SurfaceRotMatrix);
        MulMatrix(&bruce->mRotationMatrix, &SurfaceRotMatrix);
        Physics_FRUVectorsFromRotMatrix(bruce);

        if (RotateVelocity != 0)
        {
            CVector *velocity = (CVector *)&bruce->CSuper.CBody.mVel;
            OldMag = M3dMaths_SquareRoot0(
                Mips_CVecDotProd(velocity, velocity)) << 6;

            MATRIX m;
            MATRIX m2;
            CVector mv;
            CVector NewVel1;

            TransposeMatrix(&bruce->mOldRotationMatrix, &m2);
            RotAngle.vz = 0;
            RotAngle.vx = 0;
            RotAngle.vy = (unsigned short)(TurnAngle - SlipAngle) & 0xFFF;
            RotMatrixYXZ(&RotAngle, &SurfaceRotMatrix);
            MulMatrix(&m2, &SurfaceRotMatrix);

            m = bruce->mOldRotationMatrix;
            MulMatrix(&m, &m2);

            mv.vx = m.m[0][0];
            mv.vy = m.m[0][1];
            mv.vz = m.m[0][2];
            NewVel1.vx = Mips_CVecDotProd(&mv, velocity);

            mv.vx = m.m[1][0];
            mv.vy = m.m[1][1];
            mv.vz = m.m[1][2];
            NewVel1.vy = Mips_CVecDotProd(&mv, velocity);

            mv.vx = m.m[2][0];
            mv.vy = m.m[2][1];
            mv.vz = m.m[2][2];
            NewVel1.vz = Mips_CVecDotProd(&mv, velocity);

            *velocity = NewVel1;

            NewMag = M3dMaths_SquareRoot0(
                Mips_CVecDotProd(velocity, velocity)) << 6;
            OldMag >>= 8;
            NewMag >>= 8;
            *velocity = (*velocity * OldMag) / NewMag;
        }
    }
}

/**
 * ReflectHeadingVelocityInPlane__6CBruceG7SVECTORi
 *
 * Address: 0x8001CCCC
 * Mangled: ReflectHeadingVelocityInPlane__6CBruceG7SVECTORi
 *
 * Removes inward velocity and rotates the skater's heading basis away from a
 * collision plane while preserving an orthonormal front/right/up frame.
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 * @param Normal  Plane normal used for the reflection.
 * @param Angle  Reflection adjustment.
 */
void Physics_ReflectHeadingVelocityInPlane(
    CBrucePhysicsSlice *bruce, SVECTOR Normal, int Angle)
{
    CVectorPhysicsOwner TmpCV;
    int dot;
    MATRIX SurfaceRotMatrix;

    if (Angle == 0)
        Angle = 25;

    TmpCV.vx = Normal.vx;
    TmpCV.vy = Normal.vy;
    TmpCV.vz = Normal.vz;
    dot = Mips_CVecDotProd(
        (CVector *)&bruce->CSuper.CBody.mVel, (CVector *)&TmpCV);
    if (dot < 0)
    {
        bruce->CSuper.CBody.mVel.x -= Mips_FixMul(Normal.vx, dot);
        bruce->CSuper.CBody.mVel.y -= Mips_FixMul(Normal.vy, dot);
        bruce->CSuper.CBody.mVel.z -= Mips_FixMul(Normal.vz, dot);
        bruce->CSuper.CBody.mVel.x += Mips_FixMul(Normal.vx, 205);
        bruce->CSuper.CBody.mVel.y += Mips_FixMul(Normal.vy, 205);
        bruce->CSuper.CBody.mVel.z += Mips_FixMul(Normal.vz, 205);
    }

    TmpCV.vx = -Normal.vx;
    TmpCV.vy = -Normal.vy;
    TmpCV.vz = -Normal.vz;
    dot = Mips_CVecDotProd(
        (CVector *)&bruce->mFront, (CVector *)&TmpCV);
    if (dot < 0)
    {
        int sin = M3dMaths_SquareRoot0(0x1000000 - dot * dot);
        int cos = dot;
        short *pShort = (short *)&SurfaceRotMatrix;
        dot = Mips_CVecDotProd(
            (CVector *)&bruce->mRight, (CVector *)&TmpCV);

        if (dot > 0)
            sin = -sin;

        pShort[0] = cos;
        pShort[1] = 0;
        pShort[2] = -sin;
        pShort[3] = 0;
        pShort[4] = 0x1000;
        pShort[5] = 0;
        pShort[6] = sin;
        pShort[7] = 0;
        pShort[8] = cos;
        MulMatrix(&bruce->mRotationMatrix, &SurfaceRotMatrix);

        SVECTOR RotAngle;
        RotAngle.vz = 0;
        RotAngle.vx = 0;
        if (dot > 0)
            RotAngle.vy = (Angle - 0x400) & 0xFFF;
        else
            RotAngle.vy = (0x400 - Angle) & 0xFFF;
        RotMatrixYXZ(&RotAngle, &SurfaceRotMatrix);
        MulMatrix(&bruce->mRotationMatrix, &SurfaceRotMatrix);
        Physics_FRUVectorsFromRotMatrix(bruce);

        M3dUtils_NormaliseVector((CVector *)&bruce->mFront);
        M3dUtils_NormaliseVector((CVector *)&bruce->mUp);
        gte_ldopv1(&bruce->mFront);
        gte_ldopv2(&bruce->mUp);
        gte_op12();
        gte_stlvnl(&bruce->mRight);
        M3dUtils_NormaliseVector((CVector *)&bruce->mRight);
        gte_ldopv1(&bruce->mUp);
        gte_ldopv2(&bruce->mRight);
        gte_op12();
        gte_stlvnl(&bruce->mFront);
        Physics_RotMatrixFromFRUVectors(bruce);
    }
}

/** Expand the skater's effective fixed-point top speed at each use site. */
#define PHYSICS_RECON_ALIGN_MAX_SPEED(bruce)                                      \
    ((((Cheat_StatsOverride != 0                                            \
            ? Cheat_StatsOverride                                          \
            : Stats[(bruce)->mSkaterType].Speed                            \
                  + (((bruce)->mMagicTime != 0 || Cheat_Fire != 0)          \
                         ? 3                                                \
                         : 0)                                               \
                  + (unsigned char)(bruce)->mpBoardStats->speed)            \
          + 0x27)                                                           \
         << 12)                                                             \
        * 45 / 28)

#define PHYSICS_RECON_LIMIT_MAX_SPEED(self)                                       \
    ((((Cheat_StatsOverride != 0                                            \
            ? Cheat_StatsOverride                                          \
            : Stats[(self)->mSkaterType].Speed                             \
                  + (((self)->mMagicTime != 0                              \
                      || Cheat_Fire != 0)                                   \
                         ? 3                                                \
                         : 0)                                               \
                  + (unsigned char)(self)->mpBoardStats->speed)             \
          + 0x32)                                                           \
         << 12)                                                             \
        * 45 / 28)

/**
 * AlignToFallLine__6CBrucei
 *
 * Address:   0x8001CF9C
 * Mangled:   AlignToFallLine__6CBrucei
 * Demangled: CBruce::AlignToFallLine(int)
 *
 * Computes a speed- and slope-scaled steering adjustment toward the most
 * recent terrain fall line while suppressing repeated turns in one direction.
 *
 * @param bruce  Skater object.
 * @param Amount  Amount by which to align toward the fall line.
 */
void Physics_AlignToFallLine(
    CBrucePhysicsSlice *bruce, int Amount)
{
    bruce->mFallLineAdjustment = 0;
    if (bruce->mInBail != 0)
        return;

    int Speed = M3dMaths_SquareRoot0(
        Mips_CVecDotProd(
            (CVector *)&bruce->CSuper.CBody.mVel,
            (CVector *)&bruce->CSuper.CBody.mVel)) << 6;
    int dot;
    int FallLineLength = M3dMaths_SquareRoot0(
        Mips_CVecDotProd(
            (CVector *)&bruce->mLastFallLine,
            (CVector *)&bruce->mLastFallLine)) << 6;
    int PolyTilt = FallLineLength / 260;
    PolyTilt = (PolyTilt * FallLineLength) / 13000;

    if (PHYSICS_RECON_ALIGN_MAX_SPEED(bruce) < Speed)
        Speed = PHYSICS_RECON_ALIGN_MAX_SPEED(bruce);
    int angle = (PolyTilt *
        ((Amount << 12) + PHYSICS_RECON_ALIGN_MAX_SPEED(bruce) - Speed)) /
        PHYSICS_RECON_ALIGN_MAX_SPEED(bruce);
    CVector FallLinePerp;
    if (angle == 0)
        return;

    if (bruce->mLastXFrames + 10 < XFrames)
        bruce->mLastTurnDirection = 0;
    bruce->mLastXFrames = XFrames;

    CVector FallLineUnit = *(CVector *)&bruce->mLastFallLine;
    M3dUtils_NormaliseVector((CVector *)&FallLineUnit);

    VECTOR mLPN;
    mLPN.vx = bruce->mLastPolyNormal.vx;
    mLPN.vy = bruce->mLastPolyNormal.vy;
    mLPN.vz = bruce->mLastPolyNormal.vz;
    gte_ldopv1(&mLPN);
    gte_ldopv2(&FallLineUnit);
    gte_op12();
    gte_stlvnl(&FallLinePerp);

    CVector *Front = (CVector *)&bruce->mFront;
    CVector FrontInPlane = *Front;
    ProjectCVectorToPlane(
        &FrontInPlane, bruce->mLastPolyNormal);

    dot = Mips_CVecDotProd(
        &FrontInPlane, (CVector *)&FallLineUnit);
    dot = Mips_CVecDotProd(Front, &FallLinePerp);
    if (dot >= 0x2A)
    {
        if (bruce->mLastTurnDirection != 2)
        {
            bruce->mFallLineAdjustment = -angle;
            bruce->mLastTurnDirection = 1;
        }
    }
    else if (dot < -0x29)
    {
        if (bruce->mLastTurnDirection != 1)
        {
            bruce->mFallLineAdjustment = angle;
            bruce->mLastTurnDirection = 2;
        }
    }
    else
    {
        bruce->mLastTurnDirection = 0;
    }
}

/**
 * UprightSideways__6CBruce
 *
 * Address:   0x8001D530
 * Mangled:   UprightSideways__6CBruce
 * Demangled: CBruce::UprightSideways(void)
 *            Apply a small Z-axis roll correction so the skater stays upright
 *            relative to the world-up direction.  Uses GTE OP to compute
 *            cross(mFront, WorldUp), then dots the result with mUp to detect
 *            the lean direction; rotates the body's rotation matrix by
 *            +/-0xB and rebuilds the F/R/U vectors.
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_UprightSideways(CBrucePhysicsSlice *bruce)
{
    CVector *mFront = (CVector *)&bruce->mFront;
    CVector TmpCV;
    MATRIX SurfaceRotMatrix;
    SVECTOR RotAngle;
    VECTOR v;

    gte_ldopv1(mFront);
    gte_ldopv2(&WorldUp);
    gte_op12();
    gte_stlvnl(&v);

    CVector *mUp = (CVector *)&bruce->mUp;
    TmpCV.vx = v.vx;
    TmpCV.vy = v.vy;
    TmpCV.vz = v.vz;
    int dot = Mips_CVecDotProd(mUp, &TmpCV);

    if (dot >= 0x2A)
    {
        RotAngle.vy = 0;
        RotAngle.vx = 0;
        RotAngle.vz = 0xB;
        RotMatrixYXZ(&RotAngle, &SurfaceRotMatrix);
        MulMatrix(&bruce->mRotationMatrix, &SurfaceRotMatrix);
    }
    else if (dot < -0x29)
    {
        RotAngle.vy = 0;
        RotAngle.vx = 0;
        RotAngle.vz = 0xFF5;
        RotMatrixYXZ(&RotAngle, &SurfaceRotMatrix);
        MulMatrix(&bruce->mRotationMatrix, &SurfaceRotMatrix);
    }
    else
    {
        Mips_CVecDotProd(mUp, (CVector *)&WorldUp);
    }
    Physics_FRUVectorsFromRotMatrix(bruce);
}

/**
 * CBruce::FRUVectorsFromRotMatrix(void) — extract F/R/U column vectors from rotation matrix
 *
 * Address: 0x8001D634
 *          Matrix at self+0xE90 is 3x3 shorts (row-major). Outputs are 3 int vectors at 0x10A0/0x10AC/0x10B8.
 * Mangled: FRUVectorsFromRotMatrix__6CBruce
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_FRUVectorsFromRotMatrix(CBrucePhysicsSlice *bruce)
{
    // Column 2 → vector at 0x10A0
    int c20 = bruce->mRotationMatrix.m[0][2];
    int c21 = bruce->mRotationMatrix.m[1][2];
    int c22 = bruce->mRotationMatrix.m[2][2];
    int *dst1 = (int *)&bruce->mFront;
    bruce->mFront.x = c20;
    dst1[1] = c21;
    dst1[2] = c22;
    // Column 0 → vector at 0x10AC
    int c00 = bruce->mRotationMatrix.m[0][0];
    int c01 = bruce->mRotationMatrix.m[1][0];
    int c02 = bruce->mRotationMatrix.m[2][0];
    int *dst2 = (int *)&bruce->mRight;
    bruce->mRight.x = c00;
    dst2[1] = c01;
    dst2[2] = c02;
    // Column 1 → vector at 0x10B8
    int c10 = bruce->mRotationMatrix.m[0][1];
    int c11 = bruce->mRotationMatrix.m[1][1];
    int c12 = bruce->mRotationMatrix.m[2][1];
    bruce->mUp.x = c10;
    int *dst3 = (int *)&bruce->mUp;
    dst3[1] = c11;
    dst3[2] = c12;
}

/**
 * CBruce::RotMatrixFromFRUVectors(void) — write column vectors back to rotation matrix
 *
 * Address: 0x8001D68C
 * Mangled: RotMatrixFromFRUVectors__6CBruce
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_RotMatrixFromFRUVectors(CBrucePhysicsSlice *bruce)
{
    // Load all 9 values (column 0, column 1, column 2)
    u16 r00 = (u16)bruce->mRight.x;
    u16 r10 = (u16)bruce->mRight.y;
    u16 r20 = (u16)bruce->mRight.z;
    u16 r01 = (u16)bruce->mUp.x;
    u16 r11 = (u16)bruce->mUp.y;
    u16 r21 = (u16)bruce->mUp.z;
    u16 r02 = (u16)bruce->mFront.x;
    u16 r12 = (u16)bruce->mFront.y;
    u16 r22 = (u16)bruce->mFront.z;
    // Store to matrix (column by column into rows)
    bruce->mRotationMatrix.m[0][0] = r00;
    bruce->mRotationMatrix.m[1][0] = r10;
    bruce->mRotationMatrix.m[2][0] = r20;
    bruce->mRotationMatrix.m[0][1] = r01;
    bruce->mRotationMatrix.m[1][1] = r11;
    bruce->mRotationMatrix.m[2][1] = r21;
    bruce->mRotationMatrix.m[0][2] = r02;
    bruce->mRotationMatrix.m[1][2] = r12;
    bruce->mRotationMatrix.m[2][2] = r22;
}

/**
 * SetAnglesFromHeading__6CBruce
 *
 * Address:   0x8001D6D8
 * Mangled:   SetAnglesFromHeading__6CBruce
 * Demangled: CBruce::SetAnglesFromHeading(void)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_SetAnglesFromHeading(CBrucePhysicsSlice *bruce)
{
    SVECTOR temp;
    u16 vx = bruce->mRotationMatrix.m[0][1];
    u16 vy = bruce->mRotationMatrix.m[1][1];
    u16 vz = bruce->mRotationMatrix.m[2][1];
    bruce->mRotationMatrix.m[0][1] = -vx;
    bruce->mRotationMatrix.m[1][1] = -vy;
    bruce->mRotationMatrix.m[2][1] = -vz;
    M3dUtils_AnglesFromMatrix(&temp, &bruce->mRotationMatrix);
    u16 vx2 = bruce->mRotationMatrix.m[0][1];
    u16 vy2 = bruce->mRotationMatrix.m[1][1];
    bruce->mRotationMatrix.m[0][1] = -vx2;
    u16 vz2 = bruce->mRotationMatrix.m[2][1];
    bruce->mRotationMatrix.m[1][1] = -vy2;
    s16 r0 = temp.vx;
    s16 r1 = temp.vy;
    s16 r2 = temp.vz;
    bruce->mRotationMatrix.m[2][1] = -vz2;
    bruce->CSuper.CBody.CItem.mAngles.x = r0;
    bruce->CSuper.CBody.CItem.mAngles.y = r1;
    bruce->CSuper.CBody.CItem.mAngles.z = r2;
}

/**
 * HandleContinuousSounds__6CBruce
 *
 * Address:   0x8001D764
 * Mangled:   HandleContinuousSounds__6CBruce
 * Demangled: CBruce::HandleContinuousSounds(void)
 *
 * Maintain the looping wallride, grind, ground, and wind sounds associated
 * with the current physics state.  Ground sounds follow the current terrain;
 * wind volume and pitch scale with velocity and the skater's speed stat.
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
extern "C" void Physics_HandleContinuousSounds(CBrucePhysicsSlice *bruce)
{

    if (bruce->mPhysicsState == 5)
    {
        if (bruce->mSFXWallride == 0)
        {
            bruce->mSFXWallride =
                SFX_PlayX(0x0F, 0x2000, 0);
        }
    }
    else if (bruce->mSFXWallride != 0)
    {
        SFX_Stop(bruce->mSFXWallride);
        bruce->mSFXWallride = 0;
    }

    if (bruce->mPhysicsState == 4)
    {
        if (bruce->mSFXGrind == 0)
        {
            switch (bruce->mRailType)
            {
            case 0:
                if (bruce->mRailSparks != 0)
                    bruce->mSFXGrind =
                        SFX_PlayX(0xCE, 0x2000, 0);
                else
                    bruce->mSFXGrind =
                        SFX_PlayX(0x0B, 0x2000, 0);
                break;
            case 1:
                if (bruce->mRailSparks != 0)
                    bruce->mSFXGrind =
                        SFX_PlayX(0x0C, 0x2000, 0);
                else
                    bruce->mSFXGrind =
                        SFX_PlayX(0xD1, 0x2000, 0);
                break;
            case 2:
                if (bruce->mRailSparks != 0)
                    bruce->mSFXGrind =
                        SFX_PlayX(0xCF, 0x2000, 0);
                else
                    bruce->mSFXGrind =
                        SFX_PlayX(0xD3, 0x2000, 0);
                break;
            case 3:
                if (bruce->mRailSparks != 0)
                    bruce->mSFXGrind =
                        SFX_PlayX(0xD0, 0x2000, 0);
                else
                    bruce->mSFXGrind =
                        SFX_PlayX(0xD4, 0x2000, 0);
                break;
            }
        }
    }
    else if (bruce->mSFXGrind != 0)
    {
        SFX_Stop(bruce->mSFXGrind);
        bruce->mSFXGrind = 0;
    }

    if (bruce->mPhysicsState == 0
        || bruce->mPhysicsState == 7)
    {
        if (bruce->mSFXGround != 0)
        {
            if (bruce->mTerrainSFX != bruce->mTerrain
                || bruce->mInBail != 0)
            {
                SFX_Stop(bruce->mSFXGround);
                bruce->mSFXGround = 0;
            }
        }

        if (bruce->mSFXGround == 0 && bruce->mInBail == 0)
        {
            bruce->mGroundVol = 100;
            bruce->mGroundPitch = 100;
            bruce->mTerrainSFX = bruce->mTerrain;

            switch (bruce->mTerrain)
            {
            case 2:
                bruce->mSFXGround =
                    SFX_PlayX(0x01, 0x2000, 0);
                goto reset_ground_sound;
            case 5:
                bruce->mSFXGround =
                    SFX_PlayX(0x3D, 0x2000, 0);
                break;
            case 3:
                bruce->mSFXGround =
                    SFX_PlayX(0x21, 0x2000, 0);
                break;
            case 13:
                bruce->mSFXGround =
                    SFX_PlayX(0x38, 0x2000, 0);
                goto reset_ground_sound;
            case 4:
                bruce->mSFXGround = SFX_PlayX(
                    GLevel != 7 ? 0x38 : 0xA9, 0x2000, 0);
                goto reset_ground_sound;
            case 6:
                bruce->mSFXGround =
                    SFX_PlayX(0x48, 0x2000, 0);
                break;
            case 8:
                bruce->mSFXGround =
                    SFX_PlayX(0x39, 0x2000, 0);
                goto reset_ground_sound;
            case 7:
            case 9:
            case 11:
                bruce->mSFXGround =
                    SFX_PlayX(0, 0x2000, 0);
                goto reset_ground_sound;
            case 1:
                if (GLevel == 8)
                {
                    bruce->mSFXGround =
                        SFX_PlayX(0xAB, 0x2000, 0);
                    goto reset_ground_sound;
                }
                else
                {
                    bruce->mSFXGround =
                        SFX_PlayX(0x49, 0x2000, 0);
                }
                break;
            case 12:
                bruce->mSFXGround = SFX_PlayX(
                    GLevel != 8 ? 0x37 : 0x8F, 0x2000, 0);
                goto reset_ground_sound;
            case 10:
                bruce->mSFXGround =
                    SFX_PlayX(0x47, 0x2000, 0);
                goto reset_ground_sound;
            case 14:
                bruce->mSFXGround =
                    SFX_PlayX(0x69, 0x2000, 0);
                break;
            case 15:
                bruce->mSFXGround =
                    SFX_PlayX(0x77, 0x2000, 0);
                goto reset_ground_sound;
            default:
                bruce->mSFXGround =
                    SFX_PlayX(0, 0x2000, 0);
                bruce->mGroundVol = 100;
                break;
            }

            goto ground_sound_ready;

        reset_ground_sound:
            bruce->mGroundVol = 100;
            bruce->mGroundPitch = 100;

        ground_sound_ready:
            ;
        }
    }
    else if (bruce->mSFXGround != 0)
    {
        SFX_Stop(bruce->mSFXGround);
        bruce->mSFXGround = 0;
    }

    if (bruce->mPhysicsState == 1
        || bruce->mPhysicsState == 2
        || bruce->mPhysicsState == 8)
    {
        if (bruce->mSFXAir == 0)
        {
            CVector *velocity =
                (CVector *)&bruce->CSuper.CBody.mVel;
            int Spd = M3dMaths_SquareRoot0(
                          Mips_CVecDotProd(
                              velocity, velocity))
                      << 6;
            unsigned short NewPitch;
            unsigned short NewVol;

            if (bruce->mPhysicsState == 8)
                Spd = 0x493E0;

            int numerator = Spd * 200;
            if (Cheat_StatsOverride == 0)
            {
                int stat = Stats[bruce->mSkaterType].Speed;
                unsigned char board = bruce->mpBoardStats->speed;
                if (bruce->mMagicTime != 0 || Cheat_Fire != 0)
                    NewVol = (unsigned short)(
                        numerator
                        / ((((board + stat + 0x35)
                             << 12)
                            * 45)
                           / 448));
                else
                    NewVol = (unsigned short)(
                        numerator
                        / ((((stat + board + 0x32)
                             << 12)
                            * 45)
                           / 448));
            }
            else
            {
                NewVol = (unsigned short)(
                    numerator
                    / ((((Cheat_StatsOverride + 0x32) << 12) * 45)
                       / 448));
            }
            bruce->mSFXAir = SFX_PlayX(
                7, (short)((NewVol * 2) / 3), 0);

            NewPitch =
                Cheat_StatsOverride == 0
                    ? (bruce->mMagicTime != 0 || Cheat_Fire != 0
                           ? (unsigned short)(
                                 (Spd << 11)
                                 / (((((unsigned char)
                                           bruce->mpBoardStats->speed
                                       + Stats[bruce->mSkaterType].Speed
                                       + 0x35)
                                      << 12)
                                     * 45)
                                    / 28))
                           : (unsigned short)(
                                 (Spd << 11)
                                 / ((((Stats[bruce->mSkaterType].Speed
                                       + (unsigned char)
                                           bruce->mpBoardStats->speed
                                       + 0x32)
                                      << 12)
                                     * 45)
                                    / 28)))
                    : (unsigned short)(
                          (Spd << 11)
                          / ((((Cheat_StatsOverride + 0x32) << 12) * 45)
                             / 28));
            if (NewPitch < 0x191)
                NewPitch = 0x190;
            if (NewPitch >= 0x400)
                NewPitch = 0x3FF;

            bruce->mSFXPitch = NewPitch;
            bruce->mSFXVol = NewVol;
            SFX_SetVoicePitch(
                bruce->mSFXAir, NewPitch);
        }
        else if (bruce->mSFXVol != 0)
        {
            int volume =
                bruce->mSFXVol - (bruce->mSFXVol >> 5);
            bruce->mSFXVol = volume;
            SFX_ModifyVol(
                bruce->mSFXAir, volume, volume);
        }
    }
    else if (bruce->mSFXAir != 0)
    {
        SFX_Stop(bruce->mSFXAir);
        bruce->mSFXAir = 0;
    }

    if (bruce->mSFXGround != 0)
    {
        int Spd = M3dMaths_SquareRoot0(
                      Mips_CVecDotProd(
                          (CVector *)&bruce->CSuper.CBody.mVel,
                          (CVector *)&bruce->CSuper.CBody.mVel))
                  << 6;
        int PitchNumerator = Spd << 11;
        int NewPitch;
        if (Cheat_StatsOverride == 0)
        {
            int stat = Stats[bruce->mSkaterType].Speed;
            unsigned char board = bruce->mpBoardStats->speed;
            int denominator;
            if (bruce->mMagicTime != 0 || Cheat_Fire != 0)
                denominator = (((board + stat + 0x35) << 12) * 45) / 28;
            else
                denominator = (((stat + board + 0x32) << 12) * 45) / 28;
            NewPitch = PitchNumerator / denominator;
        }
        else
        {
            NewPitch = PitchNumerator
                / ((((Cheat_StatsOverride + 0x32) << 12) * 45)
                   / 28);
        }

        int VolumeNumerator = Spd * 2024;
        int NewVol;
        if (Cheat_StatsOverride == 0)
        {
            int stat = Stats[bruce->mSkaterType].Speed;
            unsigned char board = bruce->mpBoardStats->speed;
            NewVol =
                bruce->mMagicTime != 0 || Cheat_Fire != 0
                    ? VolumeNumerator
                          / ((((board + stat + 0x35) << 12) * 45) / 448)
                    : VolumeNumerator
                          / ((((stat + board + 0x32) << 12) * 45) / 448);
        }
        else
        {
            NewVol = VolumeNumerator
                / ((((Cheat_StatsOverride + 0x32) << 12) * 45)
                   / 448);
        }
        SFX_ModifyVol(
            bruce->mSFXGround, NewVol, NewVol);

        if (NewPitch < 0x2BD)
            NewPitch = 0x2BC;
        if (NewPitch >= 0x400)
            NewPitch = 0x3FF;

        NewPitch =
            (NewPitch * bruce->mGroundPitch) / 100;
        SFX_SetVoicePitch(
            bruce->mSFXGround, (unsigned short)NewPitch);
        bruce->mSFXPitch = NewPitch;
        bruce->mSFXVol = NewVol;
    }
}

/**
 * OrientToNormal__6CBruce
 *
 * Address:   0x8001E074
 * Mangled:   OrientToNormal__6CBruce
 * Demangled: CBruce::OrientToNormal(void)
 *
 * Smooth the current ground normal, then rebuild the skater's orthogonal
 * front/right/up basis with the PSY-Q GTE outer-product intrinsics.
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_OrientToNormal(CBrucePhysicsSlice *bruce)
{
    VECTOR z1;
    CVector normal;
    CVector x1;

    if (bruce->mSameNormal < 25)
    {
        if (bruce->mSameNormal == 24)
        {
            normal.vx = bruce->CSuper.CBody.mNormal.x;
            normal.vy = bruce->CSuper.CBody.mNormal.y;
            normal.vz = bruce->CSuper.CBody.mNormal.z;
        }
        else
        {
            normal.vx = bruce->mOldNormal.vx
                        + ((bruce->CSuper.CBody.mNormal.x
                            - bruce->mOldNormal.vx)
                           >> 2);
            normal.vy = bruce->mOldNormal.vy
                        + ((bruce->CSuper.CBody.mNormal.y
                            - bruce->mOldNormal.vy)
                           >> 2);
            normal.vz = bruce->mOldNormal.vz
                        + ((bruce->CSuper.CBody.mNormal.z
                            - bruce->mOldNormal.vz)
                           >> 2);
        }

        M3dUtils_NormaliseVector((CVector *)&normal);

        bruce->mFront.Set(bruce->mRotationMatrix.m[0][2],
                          bruce->mRotationMatrix.m[1][2],
                          bruce->mRotationMatrix.m[2][2]);

        gte_ldopv1(&bruce->mFront);
        gte_ldopv2(&normal);
        gte_op12();
        gte_stlvnl(&x1);
        M3dUtils_NormaliseVector((CVector *)&x1);

        gte_ldopv1(&normal);
        gte_ldopv2(&x1);
        gte_op12();
        gte_stlvnl(&z1);

        bruce->mRotationMatrix.m[0][0] = x1.vx;
        bruce->mRight.x = (short)x1.vx;
        bruce->mRotationMatrix.m[1][0] = x1.vy;
        bruce->mRight.y = (short)x1.vy;
        bruce->mRotationMatrix.m[2][0] = x1.vz;
        bruce->mRight.z = (short)x1.vz;

        bruce->mRotationMatrix.m[0][1] = normal.vx;
        bruce->mUp.x = (short)normal.vx;
        bruce->mRotationMatrix.m[1][1] = normal.vy;
        bruce->mUp.y = (short)normal.vy;
        bruce->mRotationMatrix.m[2][1] = normal.vz;
        bruce->mUp.z = (short)normal.vz;

        bruce->mRotationMatrix.m[0][2] = z1.vx;
        bruce->mFront.x = (short)z1.vx;
        bruce->mRotationMatrix.m[1][2] = z1.vy;
        bruce->mFront.y = (short)z1.vy;
        bruce->mRotationMatrix.m[2][2] = z1.vz;
        bruce->mFront.z = (short)z1.vz;

        bruce->mOldNormal.vx = normal.vx;
        bruce->mOldNormal.vy = normal.vy;
        bruce->mOldNormal.vz = normal.vz;
    }
}

/**
 * HandlePlayerCollisions__6CBruce
 *
 * Address:   0x8001E2A4
 * Mangled:   HandlePlayerCollisions__6CBruce
 * Demangled: CBruce::HandlePlayerCollisions(void)
 *
 * Multi-player collision: if my and other skater are alive, and our positions
 * are within 0x3C of each other, normalize the heading vector and decide which
 * skater to bail based on dot products with mVel; if both moving toward each
 * other, bail the slower one.
 */
void Physics_HandlePlayerCollisions(CBrucePhysicsSlice *bruce)
{
    if (bruce->mInBail != 0)
        return;
    CBruce *other = bruce->mpOtherPlayer;
    if (other->mInBail != 0)
        return;

    CVector BetweenPlayers =
        other->mPos - *(CVector *)&bruce->CSuper.CBody.CItem.mPos;
    if (BetweenPlayers.Length() >= 0x3C)
        return;

    BetweenPlayers =
        bruce->mpOtherPlayer->mPos -
        *(CVector *)&bruce->CSuper.CBody.mOldPos;
    BetweenPlayers.Normalize();

    register int Direction1 asm("$17") = Mips_CVecDotProd(
        (CVector *)&bruce->CSuper.CBody.mVel, &BetweenPlayers);
    int Direction2 = Mips_CVecDotProd(
        &bruce->mpOtherPlayer->mVel, &BetweenPlayers);

    if (Direction1 > 0)
    {
        if (Direction2 > 0)
        {
            Physics_Bail(
                reinterpret_cast<CBrucePhysicsSlice *>(bruce->mpOtherPlayer),
                0, DO_LIMIT);
            return;
        }
        int otherLen = bruce->mpOtherPlayer->mVel.Length();
        int myLen = reinterpret_cast<CVector &>(
            bruce->CSuper.CBody.mVel).Length();
        if (myLen < otherLen)
            Physics_Bail(bruce, 0, DO_LIMIT);
        else
            Physics_Bail(
                reinterpret_cast<CBrucePhysicsSlice *>(bruce->mpOtherPlayer),
                0, DO_LIMIT);
        return;
    }
    if (Direction2 > 0)
        return;
    Physics_Bail(bruce, 0, DO_LIMIT);
}

/**
 * LimitFinalSpeed__6CBruce
 *
 * Address:   0x8001E3DC
 * Mangled:   LimitFinalSpeed__6CBruce
 * Demangled: CBruce::LimitFinalSpeed(void)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_LimitFinalSpeed(CBrucePhysicsSlice *bruce)
{
    int Spd;
    int AutoKickEnabled;
    Spd = M3dMaths_SquareRoot0(
              Mips_CVecDotProd(
                  (CVector *)&bruce->CSuper.CBody.mVel,
                  (CVector *)&bruce->CSuper.CBody.mVel))
          << 6;

    if (Spd > PHYSICS_RECON_LIMIT_MAX_SPEED(bruce))
    {
        /* Scale each velocity component back to the cap. */
        bruce->CSuper.CBody.mVel.x =
            (bruce->CSuper.CBody.mVel.x *
             (PHYSICS_RECON_LIMIT_MAX_SPEED(bruce) >> 12)) /
            (Spd >> 12);
        bruce->CSuper.CBody.mVel.y =
            (bruce->CSuper.CBody.mVel.y *
             (PHYSICS_RECON_LIMIT_MAX_SPEED(bruce) >> 12)) /
            (Spd >> 12);
        bruce->CSuper.CBody.mVel.z =
            (bruce->CSuper.CBody.mVel.z *
             (PHYSICS_RECON_LIMIT_MAX_SPEED(bruce) >> 12)) /
            (Spd >> 12);
        Spd = M3dMaths_SquareRoot0(
                  Mips_CVecDotProd(
                      (CVector *)&bruce->CSuper.CBody.mVel,
                      (CVector *)&bruce->CSuper.CBody.mVel))
              << 6;
    }

    if (GGame != 7)
        AutoKickEnabled = reinterpret_cast<SCareerOptionsPhysicsOwner *>(&GCareer)->AutoKickEnabled[bruce->mIndex];
    else
        AutoKickEnabled =
            reinterpret_cast<SCareerOptionsPhysicsOwner *>(&GCareer)->AutoKickEnabled[bruce->mIndex ^ GHorsePlayer];

    if (AutoKickEnabled == 0)
    {
        /* Below walking pace the skater bleeds speed; below a crawl it stops. */
        if (PhysicsAbsolute(Spd) < 0x10000)
        {
            bruce->CSuper.CBody.mVel.x -= bruce->CSuper.CBody.mVel.x / 32;
            bruce->CSuper.CBody.mVel.y -= bruce->CSuper.CBody.mVel.y / 32;
            bruce->CSuper.CBody.mVel.z -= bruce->CSuper.CBody.mVel.z / 32;
        }

        if (PhysicsAbsolute(Spd) < 0x2000)
        {
            bruce->CSuper.CBody.mVel.x -= bruce->CSuper.CBody.mVel.x / 4;
            if (PhysicsAbsolute(bruce->CSuper.CBody.mVel.x) < 0x10)
                bruce->CSuper.CBody.mVel.x = 0;

            bruce->CSuper.CBody.mVel.y -= bruce->CSuper.CBody.mVel.y / 4;
            if (PhysicsAbsolute(bruce->CSuper.CBody.mVel.y) < 0x10)
                bruce->CSuper.CBody.mVel.y = 0;

            bruce->CSuper.CBody.mVel.z -= bruce->CSuper.CBody.mVel.z / 4;
            if (PhysicsAbsolute(bruce->CSuper.CBody.mVel.z) < 0x10)
                bruce->CSuper.CBody.mVel.z = 0;
        }
    }
}

#undef PHYSICS_RECON_LIMIT_MAX_SPEED

/**
 * CheckForTriggers__6CBruce — Probe the skater's movement segment for trigger
 * zones and dispatch command-point collision/execute resets around the raycast.
 *
 * Address:   0x8001E9B8
 * Mangled:   CheckForTriggers__6CBruce
 * Demangled: CBruce::CheckForTriggers(void)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_CheckForTriggers(CBrucePhysicsSlice *bruce)
{
    if (PhysicsTriggerInitialized == 0)
    {
        PhysicsTriggerInitialized = 1;
    }

    CVector check = *(CVector *)&bruce->CSuper.CBody.CItem.mPos - *(CVector *)&bruce->CSuper.CBody.mOldPos;
    SLineInfo LInfo;
    CVector unit;

    if ((check.vx | check.vy | check.vz) != 0)
    {
        int *last_valid = &last_valid_401_gp;
        *(Vec3 *)last_valid = *(Vec3 *)&check;

        Utils_CalcUnit(
            unit, *reinterpret_cast<const CVector *>(last_valid));
        CVector old = *(CVector *)&bruce->CSuper.CBody.mOldPos - unit * 0x10;

        LInfo.StartCoords.Set(old.vx, old.vy, old.vz);
        LInfo.EndCoords.Set(
            bruce->CSuper.CBody.CItem.mPos.x, bruce->CSuper.CBody.CItem.mPos.y, bruce->CSuper.CBody.CItem.mPos.z);
        M3dColij_InitLineInfo(&LInfo);

        LInfo.RecordTriggerZoneHits = 1;
        Trig_ResetCPCollisionFlags(bruce->mIndex + 1);
        pCurrentPlayer = reinterpret_cast<CBruce *>(bruce);
        M3dZone_LineToItemNotSCached(&LInfo, 1);
        Trig_ResetCPExecutedFlags(bruce->mIndex + 1);
        pCurrentPlayer = 0;
    }
}

/**
 * HandleBailStates__6CBruce
 *
 * Address:   0x8001EB00
 * Mangled:   HandleBailStates__6CBruce
 * Demangled: CBruce::HandleBailStates(void)
 *
 */
void CBruce::HandleBailStates()
{
    // The original build allocates a 0x18-byte frame for locals that leave no
    // other trace in the emitted code (same pattern as TrickObRail's empty
    // 0x10 frame). An unreferenced local array reproduces the frame: the
    // prologue's sp adjust lands in the first branch delay slot and the
    // epilogue's restore fills the jr delay slot, with no memory accesses.
    int frame[6];

    CBrucePhysicsSlice *bruce = (CBrucePhysicsSlice *)this;
    if (bruce->mInBail != 0)
    {
        bruce->CSuper.CBody.mAcc.x = 0;
        bruce->CSuper.CBody.mAcc.z = 0;
        bruce->mJumpPending = 0;
        bruce->mManIsJumping = 0;

        if (bruce->mLimitBailVel == 0 || bruce->mPhysicsState == 2)
        {
            if (bruce->CSuper.CBody.mVel.y < 0)
                bruce->CSuper.CBody.mVel.y = 0;
        }

        int friction = bruce->mHeavyFriction;
        if (friction != 0)
        {
            int vx = bruce->CSuper.CBody.mVel.x;
            int dx = vx / friction;
            int vy = bruce->CSuper.CBody.mVel.y;
            int dy = vy / friction;
            int vz = bruce->CSuper.CBody.mVel.z;
            int dz = vz / friction;
            int timeScale = TimeScale;

            bruce->CSuper.CBody.mVel.x = vx - ((dx * timeScale) >> 8);
            bruce->CSuper.CBody.mVel.y = vy - ((dy * timeScale) >> 8);
            bruce->CSuper.CBody.mVel.z = vz - ((dz * timeScale) >> 8);
        }
    }
}

/**
 * CheckSideCollisions__6CBruce
 *
 * Address:   0x8001EBF0
 * Mangled:   CheckSideCollisions__6CBruce
 * Demangled: CBruce::CheckSideCollisions(void)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_CheckSideCollisions(CBrucePhysicsSlice *bruce)
{
    int in_bail = bruce->mInBail;
    CVector Debounce = *(CVector *)&bruce->CSuper.CBody.CItem.mPos;

    int SideDistance = 0xa;
    if (in_bail != 0)
        SideDistance = 0x46;

    int SideCol = Physics_CheckSide(bruce, -1, SideDistance, 1);
    if (SideCol != 0) {
        SideCol = Physics_CheckSide(bruce, 1, SideDistance, 1);
    } else {
        SideCol = Physics_CheckSide(bruce, 1, SideDistance, 1);
        if (SideCol == 0)
            return;
        SideCol = Physics_CheckSide(bruce, -1, SideDistance, 1);
    }
    if (SideCol != 0)
    {
        *(CVector *)&bruce->CSuper.CBody.CItem.mPos = Debounce;
    }
}

/**
 * Dispatch the skater's current physics state.
 *
 * Address:   0x8001ECAC
 * Mangled:   HandlePhysicsStates__6CBruce
 * Demangled: CBruce::HandlePhysicsStates(void)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_HandlePhysicsStates(CBrucePhysicsSlice *bruce)
{
    switch (bruce->mPhysicsState)
    {
    case 0:
        bruce->mGrindLean = 0;
        bruce->mGrindLeanDir = 0;
        bruce->mGrindLeanAcc = 0;
        bruce->mGrindTime = 0;
        bruce->mNumGrinds = 0;
        bruce->mDontStickRail = 0;
        bruce->mGaps = 0;
        bruce->mpGrindGapTrick = 0;
        if (bruce->mBrakeMode == 0 && bruce->mInBail == 0
            && bruce->mManual == 0 && bruce->CSuper.mAnim != 0x82
            && (char)bruce->mPad->RightTwo.Triggered != 0)
        {
            bruce->mPad->RightTwo.Triggered = 0;
            unsigned int sound = (Rnd(0x80) >= 0x40) ? 0xAE : 0xAF;
            SFX_PlayX(sound, 0x2000, 0);
            Physics_DebugRunAnim(bruce, 0x82, 0x2542);
            bruce->mCess = 1;
        }
        ((CBruce *)bruce)->DoOnGroundPhysics();
        Physics_DoManual(bruce);
        goto side_collisions;
    case 7:
        ((CBruce *)bruce)->DoOnGroundPhysics();
        Physics_DoManual(bruce);
        bruce->CSuper.CBody.CItem.mPos = bruce->CSuper.CBody.mOldPos;
    side_collisions:
        Physics_CheckSideCollisions(bruce);
        break;
    case 4:
    {
        SGapTrick *p = bruce->mpGrindGapTrick;
        if (p != 0)
        {
            reinterpret_cast<CBruce *>(bruce)->Trick_Trick(
                p->Name, -(int)p->Score, 2);
            SFX_PlayX(0xA1, 0x2000, 0);
            bruce->mpGrindGapTrick = 0;
        }
        int i = 0;
        int minimumScore = 0x3039;
        int *pScore = (int *)&bruce->mGapChecksum[0];
        int *pCheck = (int *)&bruce->mGapType[0];
        do
        {
            if (*pCheck < 10000)
                *pScore = minimumScore;
            pScore++;
            i++;
            pCheck++;
        } while (i < 10);
        ((CBruce *)bruce)->DoOnRailPhysics();
        break;
    }
    case 8:
        ((CBruce *)bruce)->DoHandPlantPhysics();
        break;
    case 6:
        Physics_DoFootPlant(bruce);
    case 1:
        Physics_DoInAir(bruce);
        break;
    case 5:
        bruce->mDontStickRail = 0;
        Physics_DoWallRide(bruce);
        break;
    case 3:
        Physics_DoInAir(bruce);
        if (bruce->mPhysicsState == 3)
        {
            int xframes = XFrames;
            int landedTime = bruce->mInAirStickToStart;
            if (xframes - landedTime > 0x19)
                Physics_StateTransition(bruce, 1, 0x2589);
        }
        break;
    case 2:
        ((CBruce *)bruce)->DoOnGroundPhysics();
        break;
    }
}

/**
 * HandleBraking__6CBruce
 *
 * Address:   0x8001EF10
 * Mangled:   HandleBraking__6CBruce
 * Demangled: CBruce::HandleBraking(void)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
/*
 * The recovered top-level locals and case order follow MAIN.SYM's lexical
 * scopes and SLD line order.  Signed pad-axis tests, the two braking formulas,
 * and the animation transition arguments are recovered from the target body.
 */
void Physics_HandleBraking(CBrucePhysicsSlice *bruce)
{
    int DownOnly;
    int BrakeVal;
    int Speed;
    int Angle;

    /* The state word is only read once the three early-outs pass, so the
     * condition dereferences it directly rather than hoisting it. */
    if (bruce->mCess != 0 || bruce->mManual != 0 ||
        bruce->mInBail != 0 ||
        (bruce->mPhysicsState != 7 && bruce->mPhysicsState != 0))
    {
        if (bruce->mPhysicsState == 7)
        {
            Physics_StateTransition(bruce, 0, 0x25AA);
            bruce->mBrakeMode = 0;
            return;
        }
    }
    else
    {
        SControl *pad = bruce->mPad;
        DownOnly = 0;
        if (((char)pad->Down.Pressed != 0 && (char)pad->Left.Pressed == 0 &&
             (char)pad->Right.Pressed == 0) ||
            (*(signed char *)&bruce->mAY > 0 &&
             *(signed char *)&bruce->mAX == 0))
        {
            DownOnly = 1;
        }

        if (bruce->mPhysicsState == 7 && !DownOnly)
        {
            Physics_StateTransition(bruce, 0, 0x25BC);
        }

        /* Brake threshold falls off with the board's lean. */
        Angle = (bruce->mLastPolyNormal.vy * -0x1000) >> 12;
        if (Angle < 0x300)
        {
            Angle = 0x300;
        }
        BrakeVal = 0x1000 - Angle;
        BrakeVal = (BrakeVal * 0x1800) / 0xD0;
        if (BrakeVal < 0xA000)
        {
            BrakeVal = 0xA000;
        }

        /* The target keeps this velocity address across the dot product and
         * the later three-component clear. */
        CVector *pVel = (CVector *)&bruce->CSuper.CBody.mVel;
        Speed = M3dMaths_SquareRoot0(
                    Mips_CVecDotProd(pVel, pVel))
                << 6;

        if (bruce->mPhysicsState == 0 && DownOnly)
        {
            if (BrakeVal < Speed)
            {
                Angle = (int)((unsigned int)bruce->mAY << 24) >> 28;
                if ((int)((unsigned int)bruce->mAY << 24) > 0)
                {
                    bruce->CSuper.CBody.mVel.x -=
                        (((bruce->CSuper.CBody.mVel.x * Angle) / 0x80)
                         * TimeScale) >> 8;
                    bruce->CSuper.CBody.mVel.y -=
                        (((bruce->CSuper.CBody.mVel.y * Angle) / 0x80)
                         * TimeScale) >> 8;
                    bruce->CSuper.CBody.mVel.z -=
                        (((bruce->CSuper.CBody.mVel.z * Angle) / 0x80)
                         * TimeScale) >> 8;
                }
                else
                {
                    bruce->CSuper.CBody.mVel.x -=
                        ((bruce->CSuper.CBody.mVel.x >> 4)
                         * TimeScale) >> 8;
                    bruce->CSuper.CBody.mVel.y -=
                        ((bruce->CSuper.CBody.mVel.y >> 4)
                         * TimeScale) >> 8;
                    bruce->CSuper.CBody.mVel.z -=
                        ((bruce->CSuper.CBody.mVel.z >> 4)
                         * TimeScale) >> 8;
                }
            }
            else if (BrakeVal < 0xA001)
            {
                Physics_StateTransition(bruce, 7, 0x25F1);
                bruce->CSuper.CBody.mVel.x = 0;
                ((int *)pVel)[1] = 0;
                ((int *)pVel)[2] = 0;
            }
        }

        int next;
        switch (bruce->mBrakeMode)
        {
        case 0:
            if (!DownOnly)
            {
                return;
            }
            if (BrakeVal < Speed)
            {
                if (bruce->mManIsCrouched == 0 && Speed < 0x30000)
                {
                    Physics_DebugRunAnim(bruce, 0x31, 0x2604);
                    next = 5;
                }
                else
                {
                    Physics_DebugRunAnim(bruce, 0x83, 0x2609);
                    next = 1;
                }
                goto store;
            }
            bruce->mBrakeMode = 0;
            goto lockout;

        case 1:
            if (!DownOnly ||
                Speed < BrakeVal + bruce->CSuper.mFrame * 0x1000)
            {
                bruce->CSuper.mAnimFinished = 0;
                bruce->CSuper.mAnimDir = -1;
                int temp = bruce->CSuper.mStartFrame;
                bruce->CSuper.mStartFrame =
                    (short)(signed char)bruce->CSuper.mTargetFrame;
                bruce->CSuper.mTargetFrame = (unsigned char)temp;
                bruce->mBrakeMode = 3;
                if (DownOnly)
                {
                    unsigned int sound = (Rnd(0x80) >= 0x40) ? 0xB0 : 0xB1;
                    SFX_PlayX(sound, 0x2000, 0);
                    ((Dust *)&bruce->mDust[0])->Reset(
                        bruce->mTerrain, 2);
                }
            }
            goto lockout;

        case 5:
            if (!DownOnly ||
                Speed < BrakeVal + bruce->CSuper.mFrame * 0x1000)
            {
                bruce->CSuper.mAnimFinished = 0;
                bruce->CSuper.mAnimDir = -1;
                int temp = bruce->CSuper.mStartFrame;
                bruce->CSuper.mStartFrame =
                    (short)(signed char)bruce->CSuper.mTargetFrame;
                bruce->CSuper.mTargetFrame = (unsigned char)temp;
                bruce->mBrakeMode = 6;
                if (DownOnly && bruce->CSuper.mFrame > 4)
                {
                    SFX_PlayX(2, 0x2000, 0);
                }
            }
            goto lockout;

        case 3:
            if (bruce->CSuper.mAnimFinished != 0)
            {
                if (bruce->mManIsCrouched == 0)
                {
                    Physics_DebugRunAnim2(bruce, 8, 0x13, 0, 0x2651);
                }
                else
                {
                    Physics_DebugRunAnim3(
                        bruce, 8, 0x13, 0x1A, 0x13, 0x2653);
                }
                next = 4;
                goto store;
            }
            goto lockout;

        case 6:
            if (bruce->CSuper.mAnimFinished != 0)
            {
                Physics_DebugRunAnim(bruce, 0, 0x265D);
                next = 4;
            store:
                bruce->mBrakeMode = next;
            }

        lockout:
            bruce->mKickLockout = 2;
            return;

        case 4:
            if (!DownOnly)
            {
                goto clear;
            }
            return;

        default:
            return;
        }
    }

clear:
    bruce->mBrakeMode = 0;
}

/**
 * HandleFlips__6CBruce — flip rotation update
 *
 * Address:   0x8001F45C
 * Mangled:   HandleFlips__6CBruce
 * Demangled: CBruce::HandleFlips(void)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_HandleFlips(CBrucePhysicsSlice *bruce)
{
    register int flipCount asm("$6") = bruce->mFlipNum;
    if (flipCount != 0)
    {
        int flipSpeed = bruce->mFlipSpeed;
        int timeScale = TimeScale;
        int product = flipSpeed * timeScale;
        int flipAngle = bruce->mFlipAngle;
        int delta = product >> 8;
        int newAngle = flipAngle + delta;
        bruce->mFlipAngle = newAngle;
        if (((newAngle ^ flipAngle) & 0x1000) != 0)
        {
            int newCount = flipCount - 1;
            asm volatile("" : "+r"(flipCount));
            bruce->mFlipNum = newCount;
            if (newCount == 0)
            {
                bruce->mFlipAngle = 0;
                bruce->mFlipSpeed = 0;
            }
        }
    }
}

/**
 * HandleTimer__6CBruce
 *
 * Address:   0x8001F4C0
 * Mangled:   HandleTimer__6CBruce
 * Demangled: CBruce::HandleTimer(void)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_HandleTimer(CBrucePhysicsSlice *bruce)
{
    int game = GGame;
    if (game == 2)
        return;

    int autotest = GAutoTest;
    int timer;
    if (autotest != 0)
    {
        timer = 100 - XFrames;
    }
    else
    {
        int limit = bruce->mPlayTime;
        timer = limit - XFrames;
    }

    if (timer >= 0)
        return;

    if (GState != 0)
        return;

    if (autotest != 0)
    {
        EndCode = ENDCODE_PLAYERQUIT;
        return;
    }

    if (game == 1)
    {
        if (Levels[GLevel].isCompetition != 0)
        {
            SFX_PlayX(0xA0, 0x2000, 0);
        }
    }

    Game_Over();
}

/**
 * DoPhysics__6CBruce
 *
 * Address:   0x8001F5A8
 * Mangled:   DoPhysics__6CBruce
 * Demangled: CBruce::DoPhysics(void)
 *
 * @param bruce Owner-local view of the implicit `this` pointer.
 */
void Physics_DoPhysics(CBrucePhysicsSlice *bruce)
{
    ((Dust *)&bruce->mDust[0])->Update((CBruce *)bruce);
    ((Blood *)&bruce->mBlood[0])->Update((CBruce *)bruce);

    WorldUp.vx = 0;
    WorldUp.vy = -0x1000;
    WorldUp.vz = 0;
    ((CVectorPhysicsOwner *)((int)&bruce->mAirGravity))->Set(
        0, bruce->mTrickGravity * 0x82, 0);

    /* Three per-frame countdowns; the first clamps below zero, the others below one. */
    if (bruce->mWasAir != 0)
    {
        int t = bruce->mWasAir - 5;
        bruce->mWasAir = t;
        if (t < 0)
            bruce->mWasAir = 0;
    }
    if (bruce->mMagicPoints != 0)
    {
        int t = bruce->mMagicPoints - 2;
        bruce->mMagicPoints = t;
        if (t < 1)
            bruce->mMagicPoints = 0;
    }
    if (bruce->mMagicTime != 0)
    {
        int t = bruce->mMagicTime - 2;
        bruce->mMagicTime = t;
        if (t < 1)
            bruce->mMagicTime = 0;
    }

    /* Crouch latch: stamped with the frame counter while the pad is held down. */
    SControl *pad = bruce->mPad;
    if (((char)pad->Up.Pressed != 0 ||
         *(signed char *)&bruce->mAY < -0x28) &&
        (char)pad->Left.Pressed == 0 && (char)pad->Right.Pressed == 0 &&
        *(signed char *)&bruce->mAX == 0)
    {
        if (bruce->mJustUp == 0)
            bruce->mJustUp = XFrames;
    }
    else
        bruce->mJustUp = 0;

    bruce->mLastVel = bruce->CSuper.CBody.mVel;
    BaddyCollisionCheck = 1;
    BulletCollisionCheck = 0;

    reinterpret_cast<CBruce *>(bruce)->Trick_PlayerInit(0);
    Physics_HandleTimer(bruce);
    Physics_HandleFlips(bruce);
    Physics_FRUVectorsFromRotMatrix(bruce);
    Physics_HandleSpin(bruce);

    if (bruce->mPhysicsState != 1)
        bruce->mIgnoreCar = 0;

    bruce->CSuper.CBody.mAcc.z = 0;
    bruce->CSuper.CBody.mAcc.y = 0;
    bruce->CSuper.CBody.mAcc.x = 0;

    ((CBruce *)bruce)->HandleKick();
    ((CBruce *)bruce)->HandleJump();

    if (bruce->mPhysicsState != 5 &&
        bruce->CSuper.mAnimFinished != 0)
    {
        int animation = bruce->CSuper.mAnim;
        if (animation < 0x25)
        {
            if (animation >= 0x23)
            {
                if (bruce->mPhysicsState == 0)
                    Physics_DebugRunAnim(bruce, 0, 0x26FC);
                else
                    Physics_DebugRunAnim(bruce, 0xE, 0x26FE);
            }
        }
    }

    Physics_HandleTricks(bruce);
    Physics_HandleBraking(bruce);

    if (bruce->CSuper.mAnimFinished != 0)
    {
        int animation = bruce->CSuper.mAnim;
        if (animation < 0xD)
        {
            if (animation >= 0xB)
            {
                if (bruce->mManIsCrouched != 0)
                    Physics_DebugRunAnim3(
                        bruce, 8, 0x13, 0x1A, 0x13, 0x2713);
                else
                    Physics_DebugRunAnim(bruce, 0, 0x2717);
            }
        }
    }

    /* Rotate the one-frame history aggregates before this frame overwrites them. */
    bruce->mOldPhysicsState = bruce->mPhysicsState;
    bruce->mOldRotationMatrix = bruce->mRotationMatrix;
    bruce->mOldPos2 = bruce->CSuper.CBody.mOldPos;
    bruce->CSuper.CBody.mOldPos = bruce->CSuper.CBody.CItem.mPos;

    Physics_HandleContinuousSounds(bruce);

    if (bruce->mManIsCrouched != 0)
    {
        int statsOverride = Cheat_StatsOverride;
        if (statsOverride == 0)
        {
            int stat = Stats[bruce->mSkaterType].Speed;
            unsigned char board = bruce->mpBoardStats->speed;
            if (bruce->mMagicTime != 0 || Cheat_Fire != 0)
            {
                int value = board + stat + 0x19;
                bruce->mMaxKickSpeed =
                    ((value << 12) * 45) / 0x1C;
            }
            else
            {
                int value = stat + board + 0x16;
                bruce->mMaxKickSpeed =
                    ((value << 12) * 45) / 0x1C;
            }
        }
        else
        {
            int value = statsOverride + 0x16;
            bruce->mMaxKickSpeed =
                ((value << 12) * 45) / 0x1C;
        }
    }
    else
    {
        int oldMaxKickSpeed = bruce->mMaxKickSpeed;
        if (Cheat_StatsOverride == 0)
        {
            int stat = Stats[bruce->mSkaterType].Speed;
            unsigned char board = bruce->mpBoardStats->speed;
            if (bruce->mMagicTime != 0 || Cheat_Fire != 0)
            {
                int value = board + stat + 0x14;
                if (((value << 12) * 45) / 0x1C <
                    oldMaxKickSpeed)
                    goto lower_max_kick_speed;
                goto keep_max_kick_speed;
            }
            else
            {
                int value = stat + board + 0x11;
                if (((value << 12) * 45) / 0x1C <
                    oldMaxKickSpeed)
                    goto lower_max_kick_speed;
                goto keep_max_kick_speed;
            }
        }
        else
        {
            int value = Cheat_StatsOverride + 0x11;
            if (((value << 12) * 45) / 0x1C >=
                oldMaxKickSpeed)
                goto keep_max_kick_speed;
        }
lower_max_kick_speed:
        bruce->mMaxKickSpeed = oldMaxKickSpeed - 1;
keep_max_kick_speed:;
    }

    if (bruce->mSpeedBoost != 0)
    {
        int Speed = M3dMaths_SquareRoot0(Mips_CVecDotProd(
                        (CVector *)&bruce->CSuper.CBody.mVel, (CVector *)&bruce->CSuper.CBody.mVel));
        Speed = (Speed << 6) >> 12;
        bruce->mSpeedBoost = bruce->mSpeedBoost - 1;
        bruce->CSuper.CBody.mAcc.x = bruce->CSuper.CBody.mAcc.x + bruce->CSuper.CBody.mVel.x / Speed;
        bruce->CSuper.CBody.mAcc.y = bruce->CSuper.CBody.mAcc.y + bruce->CSuper.CBody.mVel.y / Speed;
        bruce->CSuper.CBody.mAcc.z = bruce->CSuper.CBody.mAcc.z + bruce->CSuper.CBody.mVel.z / Speed;
    }

    /* The by-value CVector argument is the skater's current position. */
    UberFrig(
        (CBruce *)bruce,
        *(CVector *)&bruce->CSuper.CBody.CItem.mPos,
        (char *)PhysicsDiagnosticPool.loopRestart);

    bruce->mSafePos = bruce->CSuper.CBody.CItem.mPos;

    if (bruce->mInBail == 0)
        Physics_BoardOn(bruce);
    if (bruce->mPhysicsState != 8)
        Physics_HandleStickToRail(bruce);

    if (bruce->mInBail != 0 &&
        bruce->CSuper.mAnim == 0x2E &&
        bruce->CSuper.mFrame == HeadBloodAtFrame[bruce->mSkaterType])
    {
        Physics_BloodSplat(bruce, 0, 0x32);
        ((Dust *)&bruce->mDust[0])->Reset(bruce->mTerrain, 1);
    }

    bruce->mGrindTime = bruce->mGrindTime + 1;

    if (bruce->mFrozen == 0)
    {
        Physics_HandlePhysicsStates(bruce);
        ((CBruce *)bruce)->HandleBailStates();
    }

    if (bruce->mPhysicsState != 0)
    {
        bruce->mManual = 0;
    }
    else
    {
        if (bruce->mManual == 0)
        {
            bruce->mManualTime = 0;
            bruce->mManualLeanDir = 0;
            bruce->mManualLeanAcc = 0;
            bruce->mManualLean = 0;
            bruce->mManualCheese = 0;
        }
    }

    Physics_HandlePlayerCollisions(bruce);

    if (bruce->mPhysicsState == 0 || bruce->mPhysicsState == 7)
    {
        int savedBullet = BulletCollisionCheck;
        int wallX = bruce->mUp.x;
        BulletCollisionCheck = 1;
        if (wallX != 0 || bruce->mUp.z != 0)
        {
            SLineInfo line_info;
            line_info.StartCoords.vx = bruce->CSuper.CBody.CItem.mPos.x;
            line_info.StartCoords.vy = bruce->CSuper.CBody.CItem.mPos.y;
            line_info.StartCoords.vz = bruce->CSuper.CBody.CItem.mPos.z;
            line_info.EndCoords.vx = line_info.StartCoords.vx
                                   + wallX * 0x3C;
            line_info.EndCoords.vy = line_info.StartCoords.vy
                                   + bruce->mUp.y * 0x3C;
            line_info.EndCoords.vz = line_info.StartCoords.vz
                                   + bruce->mUp.z * 0x3C;
            M3dColij_InitLineInfo(&line_info);
            M3dZone_LineToItem(&line_info, 1);

            if (line_info.pItem != 0)
            {
                CVector *Front = (CVector *)&bruce->mFront;
                bruce->mUp.x = 0;
                bruce->mUp.y = -0x1000;
                bruce->mUp.z = 0;
                M3dUtils_NormaliseVector(Front);
                CVector *Wall = (CVector *)&bruce->mUp;
                M3dUtils_NormaliseVector(Wall);
                gte_ldopv1(Front);
                gte_ldopv2(Wall);
                gte_op12();
                CVector *Right = (CVector *)&bruce->mRight;
                gte_stlvnl(Right);
                M3dUtils_NormaliseVector(Right);
                gte_ldopv1(Wall);
                gte_ldopv2(Right);
                gte_op12();
                gte_stlvnl(Front);
                Physics_RotMatrixFromFRUVectors(bruce);
                Physics_ReflectHeadingVelocityInPlane(
                    bruce, line_info.Normal, 0x200);
                bruce->mSameNormal = 0;
            }
        }

        int lean = bruce->mAnimLeanAngle;
        if (lean != 0)
        {
            int animation = bruce->CSuper.mAnim;
            int MaxFrame = 0;
            if (animation > 5)
            {
                if (animation >= 8)
                {
                    if (animation < 0xB)
                    {
                        if (animation > 8)
                        {
                            MaxFrame = 0xC;
                            if (lean < 0)
                                MaxFrame = -0xC;
                        }
                    }
                }
                else
                {
                    MaxFrame = 0x16;
                    if (lean < 0)
                        MaxFrame = -0x16;
                }
            }
            if (MaxFrame != 0)
            {
                int reach = -((bruce->CSuper.mFrame * 0x5A) / MaxFrame);
                SLineInfo line_info;
                line_info.StartCoords.vx =
                    bruce->CSuper.CBody.CItem.mPos.x + bruce->mUp.x * 100;
                line_info.StartCoords.vy =
                    bruce->CSuper.CBody.CItem.mPos.y + bruce->mUp.y * 100;
                line_info.StartCoords.vz =
                    bruce->CSuper.CBody.CItem.mPos.z + bruce->mUp.z * 100;
                line_info.EndCoords = line_info.StartCoords;
                line_info.EndCoords.vx += bruce->mRight.x * reach;
                line_info.EndCoords.vy += bruce->mRight.y * reach;
                line_info.EndCoords.vz += bruce->mRight.z * reach;
                M3dColij_InitLineInfo(&line_info);
                M3dZone_LineToItemNotSCached(&line_info, 1);

                if (line_info.pItem != 0)
                {
                    int hit = line_info.Distance;
                    int frame = (hit * MaxFrame) / 0x5A;
                    *(unsigned short *)((int)&bruce->CSuper.mFrame) =
                        (unsigned short)PhysicsAbsolute(frame);
                    if (hit < 0x3C)
                    {
                        bruce->CSuper.CBody.CItem.mPos.x += line_info.Normal.vx * 2;
                        bruce->CSuper.CBody.CItem.mPos.y += line_info.Normal.vy * 2;
                        bruce->CSuper.CBody.CItem.mPos.z += line_info.Normal.vz * 2;
                    }
                }
            }
        }
        BulletCollisionCheck = savedBullet;
    }

    if (bruce->mOnRail == 0)
    {
        int x = bruce->CSuper.CBody.CItem.mPos.x;
        int y = bruce->CSuper.CBody.CItem.mPos.y;
        int z = bruce->CSuper.CBody.CItem.mPos.z;
        bruce->CSuper.CBody.CItem.mPos = bruce->CSuper.CBody.mOldPos;
        Physics_MoveOriginTo(bruce, x, y, z);
    }

    Physics_CheckForTriggers(bruce);

    int PhysicsState = bruce->mPhysicsState;
    if (PhysicsState < 4)
    {
        if (PhysicsState > 0)
        {
            if (bruce->mAirTime == 0)
                bruce->mAirTime = XFrames;
        }
        else
        {
            bruce->mAirTime = 0;
            bruce->mTrickGravity = 100;
        }
    }
    else
    {
        bruce->mAirTime = 0;
        bruce->mTrickGravity = 100;
    }

    if (bruce->mFrozen == 0)
    {
        *(CVector *)&bruce->CSuper.CBody.mVel +=
            ((*(CVector *)&bruce->CSuper.CBody.mAcc * TimeScale) >> 8);
        Physics_LimitFinalSpeed(bruce);
    }
    else
    {
        bruce->CSuper.CBody.mAcc.z = 0;
        bruce->CSuper.CBody.mAcc.y = 0;
        bruce->CSuper.CBody.mAcc.x = 0;
    }
}

// ====== 0x80020164 _GLOBAL_.I.DefaultAnimations ================================
// Generated from DefaultAnimations and WorldGravity above.
