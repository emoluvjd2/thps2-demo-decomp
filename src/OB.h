// Shared declarations and opt-in owner layouts for OB.cpp.

#ifndef OB_LAYOUT_ONLY

#ifndef OB_INC_H
#define OB_INC_H

struct CBody;

#ifndef OB_OWNER_LOCAL_SERVICE_DECLS

/** Clear transient input flags for every body in a linked list. */
void Ob_ClearInputFlags(CBody *pBody);

/** Run distance-aware AI for every body in a linked list. */
void Ob_AI(CBody **ppBody);

/** Run the lightweight AI path for every body in a linked list. */
void Ob_QuickAI(CBody **ppBody);

#endif // OB_OWNER_LOCAL_SERVICE_DECLS

/** Interleave expensive body AI samples across frames. */
extern "C" void CBody_InterleaveAI(CBody *body)
    asm("InterleaveAI__5CBody");

/** Run the base body's empty per-frame virtual default. */
extern "C" void CBody_EveryFrame(CBody *body)
    asm("EveryFrame__5CBody");

#ifdef OB_OWNER_DECLARATIONS

#include "globals.h"

#ifdef __cplusplus
extern "C" {
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

// Forward declarations for large/opaque types
struct CBody;
struct CSuper;
struct EditorOutModel;
struct EditorInModel;
struct LevelGenerator;
struct CBruce;
struct CBaddy;
struct SModel;
struct SHook;
struct CPowerUp;
struct SLight;

struct SLink {
    unsigned short Part;
    unsigned short ParentPart;
    struct SVector Pivot;
    unsigned short ParentLink;
};

struct SJoint {
    struct SVector Angles;
    struct SVector Displacement;
};

#ifdef __cplusplus
}
#endif

// === Variables (gp-relative in this TU) ===
extern "C" unsigned int SuspendDistance;       // 0x800D9B48
extern "C" CSVector ZeroSVector;       // 0x800D9B40
extern "C" CVector ZeroVector;       // 0x800D9B34

// === Variables (absolute in this TU) ===
#define BITSTATE_DECLARE_USE_CACHE_FOR_BITS
#include "BITSTATE.h"

#endif // OB_OWNER_DECLARATIONS

#endif // OB_INC_H

#endif // OB_LAYOUT_ONLY

#if defined(OB_OWNER_LAYOUT) && !defined(OB_OWNER_LAYOUT_EMITTED)
#define OB_OWNER_LAYOUT_EMITTED

#include <STDDEF.H>
#include <LIBGTE.H>

struct SLight;

/** Game-space vector with aliases used by both recovered naming conventions. */
struct CVectorOwner {
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

/** Short game-space vector with recovered x/vx naming aliases. */
struct CSVectorOwner {
    union { short x; short vx; };
    union { short y; short vy; };
    union { short z; short vz; };

    /** Assign all three fixed-point components. */
    void Set(int _x, int _y, int _z) {
        x = _x;
        y = _y;
        z = _z;
    }
};

#include "CFRICTION.h"

/** One eight-byte entry in a region's packed animation lookup table. */
struct ObAnimationEntryOwner {
    u8 mNumFrames;
    u8 _reserved[7];
};

/** Header and entries addressed by `SPSXRegion::pAnimFile`. */
struct ObAnimationTableOwner {
    u32 mAnimationCount;
    u32 _reserved;
    ObAnimationEntryOwner mAnimations[1];
};

/** CItem data layout shared by base-object owner routines. */
struct CItemOwner {
    u16 mFlags;
    u16 mInquiry;
    struct CVectorOwner mPos;
    struct CSVectorOwner mAngles;
    u16 mModel;
    char mDummyFrame;
    u8 mTintIndex;
    u8 mDummyAnim;
    u8 mRegion;
    struct CItemOwner *mNextItem;
    u32 mRGB;
    struct CSVectorOwner mScale;
    u32 mTRN;
    struct CItemOwner *mPreviousItem;
    u32 mStoredRGB;
    u8 mPredatorEffectType;
    u8 mPredatorEffectFade;
    u16 mType;
    struct SLight *mpLight;
    u16 mFlashTimer;
    s16 mFlash_dR;
    s16 mFlash_dG;
    s16 mFlash_dB;
    union { void *vtable; void *_vf; };
};

/** CBody data layout implemented by OB.cpp. */
struct CBodyOwner {
    struct CItemOwner CItem;
    struct CBodyOwner **mppOriginalList;
    u16 mInputFlags;
    u16 mCBodyFlags;
    struct CVectorOwner mVel;
    struct CVectorOwner mAcc;
    struct CFriction mFric;
    struct CSVectorOwner mAngVel;
    struct CSVectorOwner mAngAcc;
    struct CFriction mAngFric;
    struct CSVectorOwner mNormal;
    u8 mEventFlags;
    u32 mEllipsoidRGB;
    u16 mEllipsoidScale;
    s16 mEllipsoidOTPushback;
    struct CVectorOwner mShadowPos;
    struct CSVectorOwner mShadowNormal;
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
    struct CVectorOwner mOldPos;
    struct CVectorOwner mTweenPos;
    struct CVectorOwner mNewPos;
    struct CVectorOwner mDPos;
    struct CSVectorOwner mOldAngles;
    struct CSVectorOwner mNewAngles;
    struct CSVectorOwner mDAngles;
};

/** Animated CSuper data layout implemented by OB.cpp. */
struct CSuperOwner {
    struct CBodyOwner CBody;
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

/** CBruce prefix needed to read the inherited CItem position. */
struct CBrucePositionOwner {
    CSuperOwner CSuper;
};

/**
 * Owner-local CBruce slice covering the player index used for vibration.
 */
struct CBruceVibrationOwner {
    u8 _padToIndex[0xDAC];
    int mIndex;
};

/**
 * Owner-local CBruce slice covering trick-state fields used in OB.cpp.
 */
struct CBruceTrickOwner {
    u8 _padToNoSpin[0xD68];
    int mNoSpin;
    int mNoSpinScore;
    u8 _padToNoTrick[0x34];
    int mNoTrick;
    u8 _padToPlayTime[0x08];
    int mPlayTime;
    u8 _padToAutoTurn[0x18];
    int mAutoTurn;
    u8 _padToManual[0x40];
    int mManual;
    u8 _padToTrickTookOff[0x208];
    int mTrickTookOff;
    int mTallyAngles;
    int mNum180Spins;
    int mComboCounter;
    int mPerfectLanding;
    int mSloppyLanding;
    u8 _padToPhysicsState[0x38];
    int mPhysicsState;
};

typedef char CVectorOwner_size_check[(sizeof(struct CVectorOwner) == 12) ? 1 : -1];
typedef char CSVectorOwner_size_check[(sizeof(struct CSVectorOwner) == 6) ? 1 : -1];
typedef char SDK_VECTOR_owner_size_check[(sizeof(VECTOR) == 16) ? 1 : -1];
typedef char SDK_SVECTOR_owner_size_check[(sizeof(SVECTOR) == 8) ? 1 : -1];
typedef char SDK_CVECTOR_owner_size_check[(sizeof(CVECTOR) == 4) ? 1 : -1];
typedef char ObAnimationEntryOwner_size_check[
    (sizeof(struct ObAnimationEntryOwner) == 8) ? 1 : -1];
typedef char ObAnimationTableOwner_size_check[
    (sizeof(struct ObAnimationTableOwner) == 16) ? 1 : -1];
typedef char CItemOwner_size_check[(sizeof(struct CItemOwner) == 76) ? 1 : -1];
typedef char CBodyOwner_size_check[(sizeof(struct CBodyOwner) == 264) ? 1 : -1];
typedef char MATRIX_owner_size_check[(sizeof(MATRIX) == 32) ? 1 : -1];
typedef char CSuperOwner_shadow_source_offset_check[
    ((unsigned long)&((struct CSuperOwner *)0)->mpShadowSource == 316) ? 1 : -1];
typedef char CSuperOwner_shadow_rgb_offset_check[
    ((unsigned long)&((struct CSuperOwner *)0)->mpShadowRGB == 320) ? 1 : -1];
typedef char CSuperOwner_transform_offset_check[
    ((int)&((struct CSuperOwner *)0)->mTransform == 324) ? 1 : -1];
typedef char CSuperOwner_pose_buffer_offset_check[
    ((int)&((struct CSuperOwner *)0)->mpPoseBuffer == 356) ? 1 : -1];
typedef char CSuperOwner_size_check[(sizeof(struct CSuperOwner) == 400) ? 1 : -1];
typedef char CBrucePositionOwner_prefix_size_check[
    (sizeof(struct CBrucePositionOwner) == 400) ? 1 : -1];
typedef char CBruceVibrationOwner_size_check[
    (sizeof(struct CBruceVibrationOwner) == 3504) ? 1 : -1];
typedef char CBruceTrickOwner_size_check[
    (sizeof(struct CBruceTrickOwner) == 4208) ? 1 : -1];

#endif // OB_OWNER_LAYOUT

/*
 * OB.cpp suppresses these same-TU service declarations during its first
 * owner include so they retain their target-proven encounter point.  This
 * re-entrant owner view exposes only that service surface at the later site.
 */
#if defined(OB_OWNER_SERVICE_DECLARATIONS) && \
    !defined(OB_OWNER_SERVICE_DECLARATIONS_EMITTED)
#define OB_OWNER_SERVICE_DECLARATIONS_EMITTED

struct CBody;

/** Clear transient input flags for every body in a linked list. */
void Ob_ClearInputFlags(CBody *pBody);

/** Run distance-aware AI for every body in a linked list. */
void Ob_AI(CBody **ppBody);

/** Run the lightweight AI path for every body in a linked list. */
void Ob_QuickAI(CBody **ppBody);

#endif // OB_OWNER_SERVICE_DECLARATIONS
