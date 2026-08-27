// CAMERA.cpp -- Reconstructed translation unit
// Original: CAMERA.CPP
// Functions: 35 (34 exact matches; ReactToCollision remains in progress)
// Address range: 0x800793D0 - 0x8007E614
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#include "CAMERA.h"
#include "MAIN.h"
#include "STATS.h"
#include "BADDY.h"
#include "FRONT.h"
#include "CITEM.h"
#include "M3DZONE.h"

/**
 * Owner-local camera, body, quaternion, and collision layouts recovered from
 * MAIN_STRUCTS. The legacy compiler pragma is load-bearing: removing it
 * changes virtual emission and regresses multiple otherwise exact functions.
 */
#pragma interface

#include <STDDEF.H>
#include <LIBGTE.H>
#include <INLINE_C.H>
#define M3DCOLIJ_LINE_INFO_LAYOUT
#include "M3DCOLIJ.h"
#undef M3DCOLIJ_LINE_INFO_LAYOUT

/** Raw polymorphic-object prefix used by aggregate owner overlays. */
struct CameraClassStorage {
    void **_vf;

    CameraClassStorage() {}
    CameraClassStorage(void **vtable) : _vf(vtable) {}
};

/** Original polymorphic base used by value-passed CQuat objects. */
struct CClass {
    CClass();
    CClass(void **vtable);
    virtual ~CClass();
};

struct SLight;

/** CItem prefix needed for the camera object's inherited position. */
struct CameraCItemOwner {
    u16 mFlags;
    u16 mInquiry;
    struct CVector mPos;
    struct CSVector mAngles;
    u16 mModel;
    s8 mDummyFrame;
    u8 mTintIndex;
    u8 mDummyAnim;
    u8 mRegion;
    struct CameraCItemOwner *mNextItem;
    u32 mRGB;
    struct CSVector mScale;
    u8 padding_after_mScale[2];
    u32 mTRN;
    struct CameraCItemOwner *mPreviousItem;
    u32 mStoredRGB;
    u8 mPredatorEffectType;
    u8 mPredatorEffectFade;
    u16 mType;
    SLight *mpLight;
    u16 mFlashTimer;
    s16 mFlash_dR;
    s16 mFlash_dG;
    s16 mFlash_dB;
    void **_vf;
};

/** Exact-sized CBody overlay; only CItem is inspected in this owner. */
struct CBody : CameraCItemOwner {
    void *mppOriginalList;
    u16 mInputFlags;
    u16 mCBodyFlags;
    struct CVector mVel;
    u8 opaque_before_mRMinor[0xb4 - 0x60];
    u16 mRMinor;
    u8 opaque_after_mRMinor[264 - 0xb6];

    CBody();
    /** Insert this body at the head of a linked body list. */
    void AttachTo(CBody **list);
    /** Remove this body from a linked body list. */
    void DeleteFrom(CBody **list);
};

/** Fixed-point quaternion object (20 bytes). */
struct CQuat : CClass {
    int x;
    int y;
    int z;
    int w;

    CQuat();
};

/** One animated quaternion parameter (28 bytes). */
struct t9AnimParam1Z5CQuat {
    struct CameraClassStorage CClass;
    u32 *mTimer;
    struct CQuat mParam;

    t9AnimParam1Z5CQuat(u32 *timer);
};

/** Camera tripod, lens, and focus snapshot (184 bytes). */
struct CamPos {
    struct CClass CClass;
    void *calc_mTripod;
    struct CBody *mTripod;
    struct CVector mTripodPos;
    void *lock_mStickO;
    struct CQuat mStickO;
    struct CVector mStickOffset;
    s16 mStickLength;
    void *calc_mLenseO;
    struct CQuat mLenseO;
    struct CVector mLenseOffset;
    void *calc_mFocusPos;
    struct CBody *mFocus;
    struct CVector mFocusPos;
    struct CQuat mWorldO;
    struct CVector mWorldPos;
    MATRIX mTrans;

    CamPos();
    ~CamPos();
    /** Draw this camera-pose record's tracked values. */
    void Display(u16 x, u16 y, u8 r, u8 g, u8 b);
    /** Draw the shared headings for camera-pose debug values. */
    static void DisplayLabels(u16 x, u16 y, u8 r, u8 g, u8 b);
};

/** Complete runtime camera object (1668 bytes). */
struct CCamera : CBody {
    /** Enables the camera's owner-local tweak controls. */
    static int Tweaking;
    /** Selected row in the camera tweak menu. */
    static u16 TweakRow __attribute__((section(".sdata")));
    /** Selected camera-position column in the camera tweak menu. */
    static u16 TweakCol __attribute__((section(".sdata")));

    u16 mMikeIndex;
    struct CamPos mPosA;
    struct CamPos mLastPos;
    struct CamPos mCurrentPos;
    struct CamPos mPosB;
    u32 mCamPosDelta;
    u32 mCamPosRate;
    struct CBody *mTripod;
    struct CBody *mOldTripod;
    void *mCalcTripod;
    struct CVector mOldTripodPos;
    void *new_focus;
    struct CVector mTripodPos;
    struct CVector mTripodPosIncrement;
    u32 mTripodTime;
    struct CBody *mFocus;
    void *mCalcFocus;
    struct CVector mOldFocusPos;
    struct CVector mFocusPos;
    struct CVector mFocusPosIncrement;
    u32 mFocusTime;
    int mZoom;
    u16 mZoomTime;
    int mZoomIncrement;
    void *mOnRail;
    struct CVector mRailStart;
    struct CVector mRailEnd;
    s16 mPitchDamper;
    s16 mTripodPosYDamper;
    struct CVector mUp;
    struct CQuat mO;
    struct CQuat mDesiredO;
    struct CQuat mPrevO;
    struct CQuat mPreFocusO;
    struct CQuat mStoppedO;
    void *mStoppedOValid;
    struct CSVector mCamAngles;
    struct CVector mPosOffset;
    struct CVector mDesiredPos;
    struct CVector mDesiredFocusPos;
    int mCollisionRayLR;
    int mCollisionRayBack;
    s16 mCollisionAngLR;
    s16 mCollisionAngBack;
    s16 mCollisionBack;
    int saved_mCollisionRayLR;
    int saved_mCollisionRayBack;
    s16 saved_mCollisionAngLR;
    s16 saved_mCollisionAngBack;
    s16 saved_mCollisionBack;
    struct CSVector mShakeAmplitude;
    struct CFriction mShakeDecay;
    struct CSVector mShakeSpeed;
    int mCameraMode;
    int mCameraModeIncDir;
    int mBossCamSpinRate;
    u32 mTimer;
    struct t9AnimParam1Z5CQuat testQuat;
    s16 mColAngBuffer[8];
    s16 mPrevColAngBuffer[8];
    u8 mColCheckIndex;
    u32 mColCheckCycleCounter;
    int mDownDist;
    int mCompIntroTimer;
    void *mSpeedUpIntro;
    struct CVector mCompIntroEndPos;
    int extra_YRot;
    struct CVector mVFront;
    struct CVector mOldVFront;
    int prev_speed_lag_dist;
    void *big_air;
    int big_air_timer;
    int big_air_end_timer;
    int big_air_mph;
    int new_hang_time;
    int in_pool_timer;
    int jump_acc;
    int jump_uptimer;
    int max_jump_dist;
    int max_jump_dist2;
    int jump_timer;
    int max_timer;
    int landing_timer;
    int landing_pos;
    int turn_delay_timer;
    struct CVector vFront;
    u32 mSpeed;
    int mSpeedBuffer[6];
    s16 mStoppedExtraYRot;
    VECTOR real_cam_offset;
    struct CQuat old_orientation;
    VECTOR old_offset;
    int loose_cam_timer;

    CCamera(struct CBody *tripod, u16 mike_index);
    /** Set the left/right collision-ray distance. */
    void SetCollisionRayLR(int ray);
    /** Set the rear collision-ray distance. */
    void SetCollisionRayBack(int ray);
    /** Set the left/right collision response angle. */
    void SetCollisionAngLR(s16 angle);
    /** Set the rear collision response angle. */
    void SetCollisionAngBack(s16 angle);
    /** Apply a camera-shake envelope from a world-space source. */
    void Shake(const CVector &position, EShakeType magnitude);
    /** Select the complex camera mode. */
    void SetComplexMode(u16 mode);
    /** Copy this camera into the active render-camera record. */
    void LoadIntoMikeCamera();
    /** Rebuild tracking orientation from the followed body. */
    void ResetOrientation();
    void ReactToCollision();
    void MoveToDesiredPos(unsigned int normal_slerp);
    /** Set the camera angle over the requested frame count. */
    void SetCamAngle(s16 angle, u16 frames);
    /** Set the horizontal camera distance over the requested frame count. */
    void SetCamXZDistance(u16 distance, u16 frames);
    /** Set the vertical camera distance over the requested frame count. */
    void SetCamYDistance(s16 distance, u16 frames);
    /** Set the horizontal camera offset over the requested frame count. */
    void SetCamXOffset(s16 offset, u16 frames);
    /** Set the vertical camera offset over the requested frame count. */
    void SetCamYOffset(s16 offset, u16 frames);
    /** Set the depth camera offset over the requested frame count. */
    void SetCamZOffset(s16 offset, u16 frames);
    /** Return the current fixed-point projection zoom. */
    int GetZoom();
    /** Update the owner-local camera tweak menu. */
    void TweakMenu();
    /** Update tracking, tweak controls, and shake response each frame. */
    void AI();
    /** Update ordinary camera tracking. */
    void CM_Normal();
    /** Enter the competition-intro camera mode. */
    void StartCompetitionIntro();
    /** Advance the competition-intro camera mode. */
    void CM_CompetitionIntro();
    /** Return whether the competition-intro camera mode is active. */
    int InCompetitionIntro();
    /** Update the camera without big-air tracking. */
    void CM_NoBigAir();
};

typedef char Camera_CClass_size_must_be_4[(sizeof(CClass) == 4) ? 1 : -1];
typedef char Camera_CItem_size_must_be_76[(sizeof(CameraCItemOwner) == 76) ? 1 : -1];
typedef char Camera_CBody_size_must_be_264[(sizeof(CBody) == 264) ? 1 : -1];
typedef char Camera_Quat_size_must_be_20[(sizeof(CQuat) == 20) ? 1 : -1];
typedef char Camera_AnimQuat_size_must_be_28[
    (sizeof(t9AnimParam1Z5CQuat) == 28) ? 1 : -1];
typedef char Camera_CamPos_size_must_be_184[(sizeof(CamPos) == 184) ? 1 : -1];
typedef char Camera_Camera_size_must_be_1668[(sizeof(CCamera) == 1668) ? 1 : -1];
typedef char Camera_CamPosStruct_size_must_be_16[
    (sizeof(CamPosStruct) == 16) ? 1 : -1];
typedef char Camera_Preset_size_must_be_12[(sizeof(SPresetCamera) == 12) ? 1 : -1];

#include "OB.h"
#include "MIPS.h"
#include "UTILS.h"
#include "PAD.h"
#include "FONT.h"
#include <LIBGPU.H>

// === Forward type declarations ===

/** CBruce slice exposing the controller selected by the camera tripod. */
struct CameraBrucePadOwner : CBody {
    u8 opaque_before_mPad[0xdb4 - sizeof(CBody)];
    SControl *mPad;
};

typedef char Camera_BrucePadOwner_size_must_be_0xdb8[
    (sizeof(CameraBrucePadOwner) == 0xdb8) ? 1 : -1];

/** CBruce state consulted by the camera's movement and pitch dampers. */
struct CameraBruceAiOwner : CBody {
    u8 opaque_before_mInBail[0xf7c - sizeof(CBody)];
    int mInBail;
    u8 opaque_before_mPhysicsState[0x106c - 0xf80];
    EPhysicsState mPhysicsState;
    u8 opaque_before_mInAirTimer[0x1144 - 0x1070];
    u16 mInAirTimer;
};

typedef char Camera_BruceAiOwner_size_must_be_0x1148[
    (sizeof(CameraBruceAiOwner) == 0x1148) ? 1 : -1];

/** CBruce state consulted while selecting the camera's movement response. */
struct CameraBruceMoveOwner : CBody {
    u8 opaque_before_mDoingATrick[0xd50 - sizeof(CBody)];
    int mDoingATrick;
    u8 opaque_before_mManIsJumping[0xe18 - 0xd54];
    int mManIsJumping;
    u8 opaque_before_mRotationMatrix[0xe90 - 0xe1c];
    MATRIX mRotationMatrix;
    u8 opaque_before_mBigAirs[0x1010 - 0xeb0];
    int mBigAirs;
    u8 opaque_before_mPhysicsState[0x106c - 0x1014];
    EPhysicsState mPhysicsState;
    u8 opaque_before_mFront[0x10a0 - 0x1070];
    CVector mFront;
    CVector mRight;
    CVector mUp;
};

typedef char Camera_BruceMoveOwner_size_must_be_0x10c4[
    (sizeof(CameraBruceMoveOwner) == 0x10c4) ? 1 : -1];

#include "SVIEWPORT.h"

/** CBruce fields used to halt the skater during a competition intro. */
struct CameraCompetitionSkater {
    u8 opaque_before_mVel[0x54];
    CVector mVel;
    u8 opaque_before_mFront[0x10a0 - 0x60];
    CVector mFront;
};

typedef char Camera_CompetitionSkater_size_must_be_0x10ac[
    (sizeof(CameraCompetitionSkater) == 0x10ac) ? 1 : -1];

#include "VIDEO.h"

#include "SCAMERA.h"

/** Rebuild the fixed-point tripod offset from a rendered camera position. */
inline void Camera_RebuildOldOffset(
    CCamera &camera, const long &positionX, const long &positionY,
    const long &positionZ) {
    camera.old_offset.vx = (positionX << 12) - camera.mTripodPos.vx;
    camera.old_offset.vy = (positionY << 12) - camera.mTripodPos.vy;
    camera.old_offset.vz = (positionZ << 12) - camera.mTripodPos.vz;
}

/** Install a recovered GCC 2.x table pointer on a CClass-derived value. */
inline void Camera_SetClassVtable(CClass &object, int vtable) {
    *(int *)&object = vtable;
}

// === Free function declarations ===
#include "CAMERAAPI.h"
static void SendShake(
    const CVector &pos, CBody *pOb, EShakeType ShakeMagnitude);

// === External function declarations ===
void Competition_ResetMarquee(void);
extern "C" void Camera_DestroyBody(CBody *, int) asm("_._5CBody");
CVector operator+(const CVector &left, const CVector &right);
CVector operator-(const CVector &left, const CVector &right);
CVector Camera_VectorNegate(const CVector &vector) asm("__mi__C7CVector");
CVector operator*(const CVector &vector, const int &scale);
CVector operator*(const int &scale, const CVector &vector);
CVector operator/(const CVector &vector, const int &scale);
CVector operator>>(const CVector &vector, const int &shift);
CVector operator<<(const CVector &vector, const int &shift);
#define CAMERALIST_OWNER_DECLARATIONS
#include "CAMERALIST.h"
#undef CAMERALIST_OWNER_DECLARATIONS
extern "C" void Camera_BodyDie(CBody *) asm("Die__5CBody");
extern "C" void Camera_BodyLightingSetup(CBody *)
    asm("DoLightingSetup__5CBody");
extern "C" int Camera_BodyHit(CBody *, int, const CVector &, int)
    asm("Hit__5CBodyiRC7CVectori");
extern "C" void Camera_BodyDeleteStuff(CBody *)
    asm("DeleteStuff__5CBody");
#include <STDIO.H>
/**
 * Owner-local debug text declared before its users and defined later in the
 * original small-data order. Address-derived labels are bound once here.
 */
static char CameraFollowTextOwner[] asm("DAT_800DBB28");
static char CameraFixedTextOwner[] asm("DAT_800DBB30");
static char CameraQuaternionTextOwner[] asm("DAT_800DBB38");
static char CameraTripodTextOwner[] asm("DAT_800DBB44");
static char CameraXTextOwner[] asm("DAT_800DBB4C");
static char CameraYTextOwner[] asm("DAT_800DBB50");
static char CameraZTextOwner[] asm("DAT_800DBB54");
static char CameraStickOrientationTextOwner[] asm("DAT_800DBB58");
static char CameraStickLengthTextOwner[] asm("DAT_800DBB60");
static char CameraStickOffsetTextOwner[] asm("DAT_800DBB70");
static char CameraPositionATextOwner[] asm("DAT_800DBB00");
static char CameraPositionBTextOwner[] asm("DAT_800DBB08");
struct CBruce;
void Competition_MarqueeDisplay(void);
#include "M3DMATHS.h"
#include "GSKATER_STATE.h"
#include "GENERATELEVEL.h"
extern "C" int __vt_7CCamera[];  // absolute: 0x800B39D0
extern "C" int __vt_t9AnimParam1Z5CQuat;  // gp+0x3ee0
extern "C" int __vt_6CamPos;  // gp+0x3ef0
extern "C" int SubObjBaseVtable asm("DAT_800dbbb0");  // gp+0x3f10
/** Packed fixed-point sine/cosine table used by camera shake. */
struct CameraSinCos {
    s16 Sin;
    s16 Cos;
};
#define RCOSSIN_DECL_TYPE CameraSinCos
#define RCOSSIN_SCALAR_VIEW
#include "RCOSSIN.h"
#define Camera_Sine(angle) \
    ((&rcossin_tbl)[(angle) & 0xfff].Sin)
/** Construct the original polymorphic base. */
inline CClass::CClass() {}

/** Construct the polymorphic base with an owner-selected virtual table. */
inline CClass::CClass(void **vtable) {
    *(void ***)this = vtable;
}

/** Tear down the original polymorphic base. */
inline CClass::~CClass() {}

/** Construct an identity quaternion. */
inline CQuat::CQuat() : CClass(), x(0), y(0), z(0), w(0x1000) {}

/** Natural free-service interface owned by QUAT. */
#include "QUAT_API.h"

/** Bind an animated quaternion to its owning timer and construct its value. */
inline t9AnimParam1Z5CQuat::t9AnimParam1Z5CQuat(u32 *t)
    : CClass((void **)&__vt_t9AnimParam1Z5CQuat), mTimer(t), mParam() {}

/** Owner-local form of CVector's inline three-component constructor. */
struct CameraGlobalVector {
    int vx;
    int vy;
    int vz;

    CameraGlobalVector(int x, int y, int z) : vx(x), vy(y), vz(z) {}
};

/** Owner-local form of CSVector's inline three-component constructor. */
struct CameraGlobalSVector {
    s16 vx;
    s16 vy;
    s16 vz;

    CameraGlobalSVector(s16 x, s16 y, s16 z) : vx(x), vy(y), vz(z) {}
};

/** Base camera distance; also anchors CAMERA.CPP's global initializer name. */
int camDist __attribute__((section(".sdata"))) = 658;
int camPitch __attribute__((section(".sdata"))) = 54;
int camYOffset __attribute__((section(".sdata"))) = -220;

static int camXZDistDeltaTimer __attribute__((section(".sdata"))) = 0;
static int camYDist __attribute__((section(".sdata"))) = -212;
static int camYDistDeltaTimer __attribute__((section(".sdata"))) = 0;
static int camXOffset __attribute__((section(".sdata"))) = 0;
static int camXOffsetDeltaTimer __attribute__((section(".sdata"))) = 0;
static int camYOffsetDeltaTimer __attribute__((section(".sdata"))) = 0;
static int camZOffset __attribute__((section(".sdata"))) = 0;
static int camZOffsetDeltaTimer __attribute__((section(".sdata"))) = 0;
static int Mick_CameraTweak __attribute__((section(".sdata"))) = 0;
int Camera_Preset __attribute__((section(".sdata"))) = 2;

static CameraGlobalVector LoosePosition
    = CameraGlobalVector(0, -0x190000, 0);
static CameraGlobalVector LooseFocus
    = CameraGlobalVector(0, -0x64000, 0);
static CameraGlobalVector StartPosition
    = CameraGlobalVector(0, -0xa0000, -0x3b6000);
static CameraGlobalVector StartFocus
    = CameraGlobalVector(0, -0x64000, 0);
static CameraGlobalVector NormalPosition
    = CameraGlobalVector(0, -0x96000, 0x28a000);
static CameraGlobalVector NormalFocus
    = CameraGlobalVector(0, -0x64000, 0);
static CameraGlobalVector FarPosition
    = CameraGlobalVector(0, -0x44c000, 0x514000);
static CameraGlobalVector FarFocus
    = CameraGlobalVector(0, -0x64000, -0x320000);
static CameraGlobalVector OverheadPosition
    = CameraGlobalVector(0, -0x8ca000, 0x190000);
static CameraGlobalVector OverheadFocus
    = CameraGlobalVector(0, -0x64000, -0x12c000);
static CameraGlobalVector FlyFocus
    = CameraGlobalVector(0, 0x64000, -0x6d60000);
static CameraGlobalVector DemoFocus
    = CameraGlobalVector(0, -0x64000, 0);
static CameraGlobalSVector cameraBigShakeAmp asm("BigShakeAmp")
    = CameraGlobalSVector(25, 0, 50);
static CameraGlobalSVector cameraMediumShakeAmp asm("MediumShakeAmp")
    = CameraGlobalSVector(0, 0, 30);
static CameraGlobalSVector cameraSmallShakeAmp asm("SmallShakeAmp")
    = CameraGlobalSVector(0, 0, 25);
static CFriction cameraLandShakeDecay asm("LandShakeDecay")
    = CFriction(3, 0, 3);
static CameraGlobalSVector cameraLandShakeSpeed asm("LandShakeSpeed")
    = CameraGlobalSVector(600, 0, 600);
static CameraGlobalVector UpDownPosition
    = CameraGlobalVector(0, -0x6e000, 0);

/** Camera presets and their display names, in the original data order. */
SPresetCamera Presets[5] __attribute__((section(".data"))) = {
    {-2000, 654, 6786, "Very Far"},
    {-268, 0, 946, "Far"},
    {-140, 118, 386, "Default"},
    {-60, 80, 202, "Close"},
    {-124, 0, 0, "Headshot"},
};

/** Quaternion probes owned and constructed by CAMERA.CPP. */
CQuat testquat;
CQuat testquat2;
CQuat testquat3;

/** Render-camera storage shared by gameplay and rendering owners. */
SCamera MikeCamera[2] __attribute__((section(".data"))) = {};

/** Scratch matrix used while converting recovered camera orientations. */
MATRIX tm __attribute__((section(".data"))) = {};


// === Extern "C" forward declarations ===
extern "C" void Camera_Destroy(CCamera *, int) asm("_._7CCamera");
/* GCC 2.x receives both `unsigned short` arguments promoted in registers;
 * this ABI shim keeps that promoted view so the target's truncation order is
 * preserved. A true-u16 definition hoists the zoom mask and misses twice. */
extern "C" void Camera_SetZoom(CCamera *, int, int)
    asm("SetZoom__7CCameraUsUs");
/** Raw address view retained solely for the manual CCamera vtable slot. */
extern "C" void Camera_AIVtableSlot(CCamera *) asm("AI__7CCamera");

// === Function implementations (address order) ===

/**
 * Camera_InitialRand__Fi
 *
 * Address:   0x800793D0
 * Mangled:   Camera_InitialRand__Fi
 * Demangled: Camera_InitialRand(int)
 *
 * @param a Initial seed for CAMERA's deterministic random stream.
 */
void Camera_InitialRand(int a) {
    RandSeed = a;
    RandA = 0x12b9b0a1;
    RandB = 0x0aa2fb3f;
}


/** Install CCamera's derived virtual table before initializing its first member. */
static inline u16 Camera_InitializeDerivedBase(CCamera *camera, u16 mikeIndex) {
    camera->_vf = (void **)__vt_7CCamera;
    return mikeIndex;
}

/**
 * Construct a gameplay camera around a tripod body and controller index.
 *
 * @param tripod Body whose position and controller drive the camera.
 * @param mike_index Initial rendered-camera slot; corrected from the tripod's pad.
 */
CCamera::CCamera(struct CBody *tripod, u16 mike_index)
    : mMikeIndex(Camera_InitializeDerivedBase(this, mike_index)),
      mTripod(tripod),
      mCalcTripod((void *)1),
      new_focus(0),
      mFocus(tripod),
      mCalcFocus((void *)1),
      mZoom(0xc0),
      mOnRail(0),
      mPitchDamper(0),
      mTripodPosYDamper(0),
      mCollisionRayLR(-1),
      mCollisionRayBack(-1),
      mCollisionAngLR(0x200),
      mCollisionAngBack(0x310),
      mCollisionBack(0x568),
      mCameraMode(1),
      mCameraModeIncDir(0),
      mBossCamSpinRate(0xf),
      mTimer(0),
      testQuat(&mTimer),
      mColCheckIndex(0),
      mColCheckCycleCounter(0),
      mDownDist(0),
      mSpeedUpIntro(0),
      mStoppedExtraYRot(0),
      loose_cam_timer(0) {
    real_cam_offset.vx = 0;
    real_cam_offset.vy = 0;
    real_cam_offset.vz = 0;

    // Determine mike index from the tripod's control block
    SControl *ctrl =
        ((CameraBrucePadOwner *)mTripod)->mPad;
    if (ctrl == &Pad[0]) {
        mMikeIndex = 0;
    } else if (ctrl == &Pad[1]) {
        mMikeIndex = 1;
    } else {
        printf("camera can't find pad number!, CCamera constructor\n");
    }

    vFront = CVector(0, 0, 0x1000);

    in_pool_timer = 0x3e8;
    big_air = 0;
    extra_YRot = 0;
    {
        // Three ordinary pointer/reference forms shift 21 instructions in the
        // constructor; the recovered compiler keeps this source temporary in v1.
        register CBody *tripod asm("$3") = mTripod;
        mTripodPos = tripod->mPos;
    }
    Camera_Preset = 2;
    mTripodTime = 0;
    mFocusPos = mFocus->mPos;
    mFocusTime = 0;
    mType = 0x63;

    SPresetCamera *base = Presets;
    SPresetCamera *preset = base + 2;
    camDist = preset->Dist;
    camPitch = preset->Pitch;
    camYOffset = base[2].YOffset;
    mFlags = 1;

    LoadIntoMikeCamera();

    // Attach to camera list
    AttachTo(&CameraList);

    NumCameras++;
    mRMinor = 0;
    mCBodyFlags &= 0xfffd;
}

/**
 * Debug label retained by the original CAMERA object between its constructor
 * diagnostic and virtual dispatch table.
 */
static char CameraJumpCompensationLabel[]
    asm("s_jump_compensation_800b39bc")
    __attribute__((section(".rdata"), aligned(4))) =
        "jump compensation";

/**
 * Virtual dispatch table for the gameplay camera specialization.
 *
 * The inherited CBody operations follow CCamera's destructor and AI override
 * in the original GCC 2.x table layout.
 */
int __vt_7CCamera[9] __attribute__((section(".rdata"), aligned(4))) = {
    0,
    0,
    (int)&Camera_Destroy,
    (int)&Camera_BodyDie,
    (int)&Camera_AIVtableSlot,
    (int)&CBody_EveryFrame,
    (int)&Camera_BodyLightingSetup,
    (int)&Camera_BodyHit,
    (int)&Camera_BodyDeleteStuff,
};


/**
 * _._7CCamera (typed alias for the original destructor ABI)
 *
 * Address:   0x800796CC
 * Mangled:   _._7CCamera
 * Demangled: CCamera::~CCamera()
 *
 * @param camera Camera being destroyed.
 * @param __in_chrg GCC 2.x in-charge flag; bit 0 also frees the object.
 */
void Camera_Destroy(CCamera *camera, int __in_chrg) {
    camera->_vf = (void **)__vt_7CCamera;
    camera->DeleteFrom(&CameraList);
    int vt = (int)&SubObjBaseVtable;
    int nc = NumCameras;
    int vt2 = (int)&__vt_t9AnimParam1Z5CQuat;
    Camera_SetClassVtable(camera->old_orientation, vt);
    camera->testQuat.CClass._vf = (void **)vt2;
    Camera_SetClassVtable(camera->testQuat.mParam, vt);
    camera->testQuat.CClass._vf = (void **)vt;
    Camera_SetClassVtable(camera->mStoppedO, vt);
    Camera_SetClassVtable(camera->mPreFocusO, vt);
    Camera_SetClassVtable(camera->mPrevO, vt);
    Camera_SetClassVtable(camera->mDesiredO, vt);
    Camera_SetClassVtable(camera->mO, vt);
    NumCameras = nc - 1;
    camera->mPosB.~CamPos();
    camera->mCurrentPos.~CamPos();
    camera->mLastPos.~CamPos();
    camera->mPosA.~CamPos();
    Camera_DestroyBody(camera, 0);
    if (__in_chrg & 1) {
        ::CItem::operator delete(camera);
    }
}


/**
 * SetCollisionRayLR__7CCamerai
 *
 * Address:   0x80079794
 * Mangled:   SetCollisionRayLR__7CCamerai
 * Demangled: CCamera::SetCollisionRayLR(int)
 *            ASM: jr $ra; sw $a1, 0x528($a0)
 *
 * @param r  Red component (0-255).
 */
void CCamera::SetCollisionRayLR(int r) {
    mCollisionRayLR = r;
}


/**
 * SetCollisionRayBack__7CCamerai
 *
 * Address:   0x8007979C
 * Mangled:   SetCollisionRayBack__7CCamerai
 * Demangled: CCamera::SetCollisionRayBack(int)
 *            ASM: jr $ra; sw $a1, 0x52c($a0)
 *
 * @param r  Red component (0-255).
 */
void CCamera::SetCollisionRayBack(int r) {
    mCollisionRayBack = r;
}


/**
 * SetCollisionAngLR__7CCameras
 *
 * Address:   0x800797A4
 * Mangled:   SetCollisionAngLR__7CCameras
 * Demangled: CCamera::SetCollisionAngLR(short)
 *            ASM: jr $ra; sh $a1, 0x530($a0)
 *
 * @param a Left/right collision response angle.
 */
void CCamera::SetCollisionAngLR(s16 a) {
    mCollisionAngLR = a;
}


/**
 * SetCollisionAngBack__7CCameras
 *
 * Address:   0x800797AC
 * Mangled:   SetCollisionAngBack__7CCameras
 * Demangled: CCamera::SetCollisionAngBack(short)
 *            ASM: jr $ra; sh $a1, 0x532($a0)
 *
 * @param a Rear collision response angle.
 */
void CCamera::SetCollisionAngBack(s16 a) {
    mCollisionAngBack = a;
}


/**
 * SendShake__FRC7CVectorP5CBody10EShakeType
 *
 * Address:   0x800797B4
 * Mangled:   SendShake__FRC7CVectorP5CBody10EShakeType
 * Demangled: SendShake(const CVector &, CBody *, EShakeType)
 *
 * @param pos  Position vector.
 * @param pOb  Ob to operate on.
 * @param ShakeMagnitude Shake envelope to propagate through the list.
 */
static void SendShake(
    const CVector &pos, CBody *pOb, EShakeType ShakeMagnitude) {
    while (pOb) {
        pOb->mInputFlags = pOb->mInputFlags | 2;
        pOb = (CBody *)pOb->mNextItem;
    }
}


/**
 * Shake__7CCameraRC7CVector10EShakeType
 *
 * Address:   0x800797E4
 * Mangled:   Shake__7CCameraRC7CVector10EShakeType
 * Demangled: CCamera::Shake(const CVector &, EShakeType)
 *
 * @param pos  Position vector.
 * @param ShakeMagnitude Requested camera-shake envelope.
 */
void CCamera::Shake(
    const CVector &pos, EShakeType ShakeMagnitude) {
    CCamera *camera = this;
    switch (ShakeMagnitude) {
    case 0: {
        camera->mShakeAmplitude = BigShakeAmp;
        break;
    }
    case 1: {
        camera->mShakeAmplitude = MediumShakeAmp;
        break;
    }
    case 2: {
        camera->mShakeAmplitude = SmallShakeAmp;
        break;
    }
    default:
        goto send_shakes;
    }

    camera->mShakeDecay = LandShakeDecay;
    camera->mShakeSpeed = LandShakeSpeed;

send_shakes:
    {
        EShakeType st = ShakeMagnitude;
        SendShake(pos, BaddyList, st);
        SendShake(pos, EnvironmentalObjectList, st);
    }
}


/**
 * SetComplexMode__7CCameraUs (intentional no-op in this build)
 *
 * Address:   0x80079908
 * Mangled:   SetComplexMode__7CCameraUs
 * Demangled: CCamera::SetComplexMode(unsigned short)
 *            ASM: jr $ra; nop
 *
 * @param mode Requested complex camera mode.
 */
void CCamera::SetComplexMode(u16 mode) {
}


/**
 * LoadIntoMikeCamera__7CCamera
 *
 * Address:   0x80079910
 * Mangled:   LoadIntoMikeCamera__7CCamera
 * Demangled: CCamera::LoadIntoMikeCamera(void)
 *
 */
void CCamera::LoadIntoMikeCamera() {
    CCamera *camera = this;
    VECTOR v;

    if (((CameraBruceMoveOwner *)camera->mTripod)->mPhysicsState ==
        PHYSICS_IN_WALLRIDE) {
        MikeCamera[Camera_CurrentMikeCamera].Position.vx =
            (camera->mTripodPos.vx + camera->old_offset.vx) >> 12;
        MikeCamera[Camera_CurrentMikeCamera].Position.vy =
            (camera->mTripodPos.vy + camera->old_offset.vy) >> 12;
        MikeCamera[Camera_CurrentMikeCamera].Position.vz =
            (camera->mTripodPos.vz + camera->old_offset.vz) >> 12;

        camera->mO = camera->old_orientation;
    } else {
        MikeCamera[Camera_CurrentMikeCamera].Position.vx =
            (camera->mPos.vx + camera->real_cam_offset.vx) >> 12;
        MikeCamera[Camera_CurrentMikeCamera].Position.vy =
            (camera->mPos.vy + camera->real_cam_offset.vy) >> 12;
        MikeCamera[Camera_CurrentMikeCamera].Position.vz =
            (camera->mPos.vz + camera->real_cam_offset.vz) >> 12;

        camera->old_orientation = camera->mO;
        Camera_RebuildOldOffset(
            *camera,
            MikeCamera[Camera_CurrentMikeCamera].Position.vx,
            MikeCamera[Camera_CurrentMikeCamera].Position.vy,
            MikeCamera[Camera_CurrentMikeCamera].Position.vz);
    }

    MikeCamera[Camera_CurrentMikeCamera].Focus.vx =
        (camera->mFocusPos.vx + Camera_CamOffset.vx) >> 12;
    MikeCamera[Camera_CurrentMikeCamera].Focus.vy =
        (camera->mFocusPos.vy + Camera_CamOffset.vy) >> 12;
    MikeCamera[Camera_CurrentMikeCamera].Focus.vz =
        (camera->mFocusPos.vz + Camera_CamOffset.vz) >> 12;

    QToM(
        camera->mO,
        MikeCamera[Camera_CurrentMikeCamera].Transform);
    TransMatrix(
        &MikeCamera[Camera_CurrentMikeCamera].Transform,
        &MikeCamera[Camera_CurrentMikeCamera].Position);
}


/**
 *
 * Address:   0x80079B94
 * Mangled:   ResetOrientation__7CCamera
 * Demangled: CCamera::ResetOrientation(void)
 *            Rebuilds the camera's forward/up state from the followed body, resets the
 *            position history, and pushes the refreshed orientation into MikeCamera.
 *
 */
void CCamera::ResetOrientation() {
    CCamera *camera = this;
    VECTOR vtemp1;
    VECTOR vtemp2;
    CameraBruceMoveOwner *pBruce =
        (CameraBruceMoveOwner *)camera->mTripod;

    vtemp1.vx = 0;
    vtemp1.vy = 0;
    vtemp1.vz = -0x1000;
    ApplyMatrixLV(
        &pBruce->mRotationMatrix,
        &vtemp1, &vtemp2);

    camera->vFront.vx = vtemp2.vx;
    camera->vFront.vy = vtemp2.vy;
    camera->vFront.vz = vtemp2.vz;

    camera->mOldVFront = camera->vFront << 12;
    camera->mVFront = camera->mOldVFront;

    camera->mTripodPos = camera->mTripod->mPos;
    camera->mOldTripodPos = camera->mTripodPos;

    Camera_CurrentMikeCamera = camera->mMikeIndex;

    camera->CM_Normal();

    camera->mStoppedO = camera->mPrevO = camera->mO = camera->mDesiredO;
    camera->mStoppedOValid = (void *)1;

    camera->LoadIntoMikeCamera();
}


/**
 * Probe the camera's collision rays and apply orientation corrections.
 *
 * Address:   0x80079D3C
 * Mangled:   ReactToCollision__7CCamera
 * Demangled: CCamera::ReactToCollision(void)
 *
 * @param self Camera whose desired orientation and collision history are updated.
 */
void CCamera::ReactToCollision() {
    CCamera *camera = this;
    SLineInfo line_info;
    MATRIX m;
    VECTOR vLeft;
    VECTOR vRight;
    VECTOR vBack;
    VECTOR vTemp;
    CVector vVel;
    CVector vStick;
    CVector vColCheck;
    VECTOR vOffset;

    if (GamePaused != 0) {
        return;
    }
    if (GameFrozen != 0) {
        return;
    }
    if (camera->mTimer < 13) {
        return;
    }

    QToM(camera->mDesiredO, m);

    ++camera->mColCheckIndex;
    if (camera->mColCheckIndex > 2) {
        camera->mColCheckIndex = 0;
        ++camera->mColCheckCycleCounter;
    }

    if (camera->mColCheckIndex == 2) {
        camera->mPrevColAngBuffer[2] = camera->mColAngBuffer[2];
        line_info.StartCoords.vx = camera->mPos.vx;
        line_info.StartCoords.vy = camera->mPos.vy;
        line_info.StartCoords.vz = camera->mPos.vz;
        line_info.EndCoords.Set(
            camera->mPos.vx, camera->mPos.vy, camera->mPos.vz);
        line_info.EndCoords.vy += 0xbb8000;
        M3dColij_InitLineInfo(&line_info);
        line_info.RecordTriggerZoneHits = 0;
        M3dZone_LineToItem(&line_info, 1);

        if (line_info.pItem != 0) {
            camera->mDownDist = line_info.Distance;
            if (line_info.Distance < 0x78) {
                int ratio =
                    ((0x78 - line_info.Distance) * 0x1000) / 0x78;
                camera->mColAngBuffer[2] =
                    (s16)((ratio * ratio) >> 15);
            } else {
                camera->mColAngBuffer[2] = 0;
            }
        } else {
            if (((CameraBruceAiOwner *)camera->mTripod)->mPhysicsState ==
                PHYSICS_IN_AIR) {
                camera->mColAngBuffer[2] = 0x200;
            } else {
                camera->mColAngBuffer[2] = camera->mPrevColAngBuffer[2];
            }
            camera->mDownDist = -1;
        }

        if (((CameraBruceAiOwner *)camera->mTripod)->mPhysicsState ==
            PHYSICS_ON_INVISIBLE) {
            setVector(&vTemp, 0, Camera_CamDist << 13, 0);
            ApplyMatrixLV(&m, &vTemp, &vBack);

            line_info.StartCoords.vx = camera->mPos.vx;
            line_info.StartCoords.vy = camera->mPos.vy;
            line_info.StartCoords.vz = camera->mPos.vz;
            line_info.EndCoords.vx = line_info.StartCoords.vx + vBack.vx;
            line_info.EndCoords.vy = line_info.StartCoords.vy + vBack.vy;
            line_info.EndCoords.vz = line_info.StartCoords.vz + vBack.vz;
            M3dColij_InitLineInfo(&line_info);
            line_info.RecordTriggerZoneHits = 0;
            CameraCollisionCheck = 1;
            M3dZone_LineToItem(&line_info, 1);
            CameraCollisionCheck = 0;
            camera->mColAngBuffer[2] = 0;

            if (line_info.pItem != 0) {
                if (line_info.Length == 0) {
                    line_info.Length = 1;
                }
                int ratio =
                    ((line_info.Length - line_info.Distance) * 0x1000) /
                    line_info.Length;
                ratio *= ratio;
                ratio >>= 12;
                int ang = 300;
                ang *= ratio;
                ang >>= 12;
                camera->mColAngBuffer[2] += (s16)ang;
                camera->mDownDist = line_info.Distance;
            } else {
                camera->mDownDist = Camera_CamDist;
            }
        }
    }

    vTemp.vx = Camera_CamDist * -0x2000;
    vTemp.vz = 0;
    vTemp.vy = 0;
    ApplyMatrixLV(&m, &vTemp, &vLeft);
    vRight.vx = -vLeft.vx;
    vRight.vy = -vLeft.vy;
    vRight.vz = -vLeft.vz;

    if (camera->mCollisionRayLR != 0) {
        if (camera->mColCheckIndex == 0) {
            camera->mPrevColAngBuffer[0] = camera->mColAngBuffer[0];
            if ((camera->mColCheckCycleCounter & 1) != 0) {
                line_info.StartCoords.vx = camera->mPos.vx;
                line_info.StartCoords.vy = camera->mPos.vy;
                line_info.StartCoords.vz = camera->mPos.vz;
                line_info.EndCoords.vx =
                    line_info.StartCoords.vx + vLeft.vx;
                line_info.EndCoords.vy =
                    line_info.StartCoords.vy + vLeft.vy;
                line_info.EndCoords.vz =
                    line_info.StartCoords.vz + vLeft.vz;
            } else {
                line_info.StartCoords.vx = camera->mTripodPos.vx;
                line_info.StartCoords.vy = camera->mTripodPos.vy;
                line_info.StartCoords.vz = camera->mTripodPos.vz;
                line_info.EndCoords.vx =
                    ((camera->mPos.vx + line_info.StartCoords.vx) >> 1) +
                    vLeft.vx;
                line_info.EndCoords.vy =
                    ((camera->mPos.vy + line_info.StartCoords.vy) >> 1) +
                    vLeft.vy;
                line_info.EndCoords.vz =
                    ((camera->mPos.vz + line_info.StartCoords.vz) >> 1) +
                    vLeft.vz;
            }
            M3dColij_InitLineInfo(&line_info);
            line_info.RecordTriggerZoneHits = 0;
            CameraCollisionCheck = 1;
            M3dZone_LineToItem(&line_info, 1);
            CameraCollisionCheck = 0;
            camera->mColAngBuffer[0] = 0;
            if (line_info.pItem != 0) {
                if (line_info.Length == 0) {
                    line_info.Length = 1;
                }
                int ratio =
                    ((line_info.Length - line_info.Distance) * 0x1000) /
                    line_info.Length;
                camera->mColAngBuffer[0] =
                    (s16)((camera->mCollisionAngLR *
                           ((ratio * ratio) >> 12)) >> 12);
            }
        }

        if (camera->mColCheckIndex == 1) {
            camera->mPrevColAngBuffer[1] = camera->mColAngBuffer[1];
            if ((camera->mColCheckCycleCounter & 1) != 0) {
                line_info.StartCoords.vx = camera->mPos.vx;
                line_info.StartCoords.vy = camera->mPos.vy;
                line_info.StartCoords.vz = camera->mPos.vz;
                line_info.EndCoords.vx =
                    line_info.StartCoords.vx + vRight.vx;
                line_info.EndCoords.vy =
                    line_info.StartCoords.vy + vRight.vy;
                line_info.EndCoords.vz =
                    line_info.StartCoords.vz + vRight.vz;
            } else {
                line_info.StartCoords.vx = camera->mTripodPos.vx;
                line_info.StartCoords.vy = camera->mTripodPos.vy;
                line_info.StartCoords.vz = camera->mTripodPos.vz;
                line_info.EndCoords.vx =
                    ((camera->mPos.vx + line_info.StartCoords.vx) >> 1) +
                    vRight.vx;
                line_info.EndCoords.vy =
                    ((camera->mPos.vy + line_info.StartCoords.vy) >> 1) +
                    vRight.vy;
                line_info.EndCoords.vz =
                    ((camera->mPos.vz + line_info.StartCoords.vz) >> 1) +
                    vRight.vz;
            }
            M3dColij_InitLineInfo(&line_info);
            line_info.RecordTriggerZoneHits = 0;
            CameraCollisionCheck = 1;
            M3dZone_LineToItem(&line_info, 1);
            CameraCollisionCheck = 0;
            camera->mColAngBuffer[1] = 0;
            if (line_info.pItem != 0) {
                if (line_info.Length == 0) {
                    line_info.Length = 1;
                }
                int ratio =
                    ((line_info.Length - line_info.Distance) * 0x1000) /
                    line_info.Length;
                camera->mColAngBuffer[1] =
                    -(s16)((camera->mCollisionAngLR *
                            ((ratio * ratio) >> 12)) >> 12);
            }
        }
    }

    setVector(&vTemp, 0, 0, Camera_CamDist * -0x1000);
    ApplyMatrixLV(&m, &vTemp, &vBack);
    setVector(&vTemp, 0x64000, 0, 0);
    ApplyMatrixLV(&m, &vTemp, &vOffset);

    if (camera->big_air_timer < 1 &&
        ((CameraBruceAiOwner *)camera->mTripod)->mPhysicsState !=
            PHYSICS_ON_INVISIBLE &&
        camera->mCollisionBack != 0) {
        u32 ratioL;
        u32 ratioR = 0;
        int ang;
        camera->mPrevColAngBuffer[3] = camera->mColAngBuffer[3];

        line_info.StartCoords.vx =
            camera->mTripodPos.vx + (vOffset.vx >> 1);
        line_info.StartCoords.vy =
            camera->mTripodPos.vy + (vOffset.vy >> 1);
        line_info.StartCoords.vz =
            camera->mTripodPos.vz + (vOffset.vz >> 1);
        line_info.EndCoords.vx =
            camera->mTripodPos.vx - vOffset.vx +
            vBack.vx * 2;
        line_info.EndCoords.vy =
            camera->mTripodPos.vy - vOffset.vy +
            vBack.vy * 2;
        line_info.EndCoords.vz =
            camera->mTripodPos.vz - vOffset.vz +
            vBack.vz * 2;
        M3dColij_InitLineInfo(&line_info);
        line_info.RecordTriggerZoneHits = 0;
        CameraCollisionCheck = 1;
        M3dZone_LineToItem(&line_info, 1);
        CameraCollisionCheck = 0;

        ratioL = 0;
        if (line_info.pItem != 0) {
            if (line_info.Length == 0) {
                line_info.Length = 1;
            }
            ratioL =
                ((line_info.Length - line_info.Distance) * 0x1000) / line_info.Length;
            ratioL *= ratioL;
            ratioL >>= 12;
        }

        line_info.StartCoords.vx =
            camera->mTripodPos.vx - (vOffset.vx >> 1);
        line_info.StartCoords.vy =
            camera->mTripodPos.vy - (vOffset.vy >> 1);
        line_info.StartCoords.vz =
            camera->mTripodPos.vz - (vOffset.vz >> 1);
        line_info.EndCoords.vx =
            camera->mTripodPos.vx + vOffset.vx +
            vBack.vx * 2;
        line_info.EndCoords.vy =
            camera->mTripodPos.vy + vOffset.vy +
            vBack.vy * 2;
        line_info.EndCoords.vz =
            camera->mTripodPos.vz + vOffset.vz +
            vBack.vz * 2;
        M3dColij_InitLineInfo(&line_info);
        line_info.RecordTriggerZoneHits = 0;
        CameraCollisionCheck = 1;
        M3dZone_LineToItem(&line_info, 1);
        CameraCollisionCheck = 0;

        if (line_info.pItem != 0) {
            if (line_info.Length == 0) {
                line_info.Length = 1;
            }
            ratioR =
                ((line_info.Length - line_info.Distance) * 0x1000) / line_info.Length;
            ratioR *= ratioR;
            ratioR >>= 12;
        }

        ang = 0x568;
        if (((CameraBruceAiOwner *)camera->mTripod)->mPhysicsState ==
            PHYSICS_ON_RAIL) {
            ratioR = 0;
            ratioL = 0;
        }
        camera->mColAngBuffer[3] = 0;
        if (ratioR != 0 || ratioL != 0) {
            if (((camera->mPrevColAngBuffer[1] +
                  camera->mColAngBuffer[1]) >> 1) != 0 &&
                ((camera->mPrevColAngBuffer[0] +
                  camera->mColAngBuffer[0]) >> 1) <
                    -((camera->mPrevColAngBuffer[1] +
                       camera->mColAngBuffer[1]) >> 1)) {
                ang *= ratioR;
                ang >>= 12;
                camera->mColAngBuffer[3] = -(s16)ang;
            }
            if (((camera->mPrevColAngBuffer[0] +
                  camera->mColAngBuffer[0]) >> 1) != 0) {
                ang *= ratioL;
                if (-((camera->mPrevColAngBuffer[1] +
                       camera->mColAngBuffer[1]) >> 1) <
                    ((camera->mPrevColAngBuffer[0] +
                      camera->mColAngBuffer[0]) >> 1)) {
                    ang >>= 12;
                    camera->mColAngBuffer[3] = (s16)ang;
                }
            }
        }
    }

    if (camera->big_air_timer > 0 ||
        ((CameraBruceAiOwner *)camera->mTripod)->mPhysicsState ==
            PHYSICS_ON_INVISIBLE) {
        camera->mColAngBuffer[3] = 0;
        camera->mPrevColAngBuffer[3] = 0;
    }

    Utils_CalcUnit(vVel, camera->mVel);
    if (camera->mColCheckIndex == 4) {
        camera->mPrevColAngBuffer[4] = camera->mColAngBuffer[4];
        vVel *= Camera_CamDist;
        line_info.EndCoords.vx = camera->mPos.vx + vVel.vx;
        line_info.EndCoords.vy = camera->mPos.vy + vVel.vy;
        line_info.EndCoords.vz = camera->mPos.vz + vVel.vz;
        line_info.StartCoords.vx = camera->mPos.vx;
        line_info.StartCoords.vy = camera->mPos.vy;
        line_info.StartCoords.vz = camera->mPos.vz;
        M3dColij_InitLineInfo(&line_info);
        line_info.RecordTriggerZoneHits = 0;
        CameraCollisionCheck = 1;
        M3dZone_LineToItem(&line_info, 1);
        CameraCollisionCheck = 0;

        Utils_CalcUnit(vVel, camera->mVel);
        CVector vworlddown = camera->mTripod->mPos - camera->mPos;
        Utils_CalcUnit(vStick, vworlddown);
        camera->mColAngBuffer[4] = 0;
        if (line_info.pItem != 0) {
            if (line_info.Length == 0) {
                line_info.Length = 1;
            }
            int ratio =
                ((line_info.Length - line_info.Distance) * 0x1000) / line_info.Length;
            camera->mColAngBuffer[4] =
                (s16)((ratio * ratio) >> 16);
        }
    }

    int ang =
        (camera->mPrevColAngBuffer[2] + camera->mColAngBuffer[2]) >> 1;
    if (ang != 0) {
        camera->mDesiredO = operator*(
            camera->mDesiredO, QFromXRot((s16)ang));
    }

    ang =
        (camera->mPrevColAngBuffer[0] + camera->mColAngBuffer[0]) >> 1;
    if (ang != 0) {
        camera->mDesiredO = operator*(
            camera->mDesiredO, QFromYRot((s16)ang));
    }

    ang =
        (camera->mPrevColAngBuffer[1] + camera->mColAngBuffer[1]) >> 1;
    if (ang != 0) {
        camera->mDesiredO = operator*(
            camera->mDesiredO, QFromYRot((s16)ang));
    }

    if (camera->mColAngBuffer[4] != 0) {
        CVector vworlddown = camera->mTripod->mPos - camera->mPos;
        Utils_CalcUnit(vStick, vworlddown);

        gte_ldopv1(&vVel);
        gte_ldopv2(&vStick);
        gte_op0();
        gte_stlvnl(&vColCheck);
        int correction = camera->mColAngBuffer[4];
        vColCheck.vx *= correction;
        vColCheck.vy *= correction;
        vColCheck.vz *= correction;

        CQuat rotation = QFromV(vColCheck);
        CQuat &desired_o = camera->mDesiredO;
        int ox = desired_o.x;
        int oy = desired_o.y;
        int oz = desired_o.z;
        int ow = desired_o.w;
        int nw =
            (ow * rotation.w - ox * rotation.x - oy * rotation.y -
             oz * rotation.z) >> 12;
        desired_o.x =
            (ow * rotation.x + ox * rotation.w + oy * rotation.z -
             oz * rotation.y) >> 12;
        desired_o.y =
            (ow * rotation.y + oy * rotation.w + oz * rotation.x -
             ox * rotation.z) >> 12;
        desired_o.z =
            (ow * rotation.z + oz * rotation.w + ox * rotation.y -
             oy * rotation.x) >> 12;
        desired_o.w = nw;
    }

    ang =
        (camera->mPrevColAngBuffer[3] + camera->mColAngBuffer[3]) >> 1;
    if (ang != 0) {
        CQuat rotation = QFromYRot((s16)ang);
        CQuat &desired_o = camera->mDesiredO;
        int ox = desired_o.x;
        int oy = desired_o.y;
        int oz = desired_o.z;
        int ow = desired_o.w;
        int nw =
            (ow * rotation.w - ox * rotation.x - oy * rotation.y -
             oz * rotation.z) >> 12;
        desired_o.x =
            (ow * rotation.x + ox * rotation.w + oy * rotation.z -
             oz * rotation.y) >> 12;
        desired_o.y =
            (ow * rotation.y + oy * rotation.w + oz * rotation.x -
             ox * rotation.z) >> 12;
        desired_o.z =
            (ow * rotation.z + oz * rotation.w + ox * rotation.y -
             oy * rotation.x) >> 12;
        desired_o.w = nw;
    }

    if (camera->mStoppedOValid != 0) {
        CVector vworlddown(0, 0x1000, 0);
        VECTOR v_temp;
        VECTOR v_temp2;
        CVector vdown;
        CVector vstop;
        MATRIX mstop;

        v_temp.vx = 0;
        v_temp.vy = 0x1000;
        v_temp.vz = 0;
        ApplyMatrixLV(&m, &v_temp, &v_temp2);
        vdown.vx = v_temp2.vx;
        vdown.vy = v_temp2.vy;
        vdown.vz = v_temp2.vz;

        QToM(camera->mStoppedO, mstop);
        v_temp.vx = 0;
        v_temp.vy = 0;
        v_temp.vz = 0x1000;
        ApplyMatrixLV(&mstop, &v_temp, &v_temp2);
        vstop.vx = v_temp2.vx;
        vstop.vy = v_temp2.vy;
        vstop.vz = v_temp2.vz;

        // The original branchless ABS macro evaluates its argument three times.
        int dot_stop =
            (Mips_CVecDotProd(&vdown, &vstop) ^
             (Mips_CVecDotProd(&vdown, &vstop) >> 31)) -
            (Mips_CVecDotProd(&vdown, &vstop) >> 31);
        (void)((Mips_CVecDotProd(
                    &vworlddown, &vstop) ^
                (Mips_CVecDotProd(
                    &vworlddown, &vstop) >> 31)) -
               (Mips_CVecDotProd(
                    &vworlddown, &vstop) >> 31));
        if (camera->mDownDist < 0 || dot_stop > 0x76c) {
            camera->mStoppedOValid = 0;
        }
    }

    if (camera->mSpeed < 4 ||
        camera->mTripodPos == camera->mOldTripodPos ||
        ((CameraBruceAiOwner *)camera->mTripod)->mInBail != 0) {
        if (camera->mStoppedOValid == 0 && camera->mDownDist >= 0) {
            Quat_Slerp(
                camera->mO, camera->mDesiredO, 0x600, camera->mStoppedO);
            camera->mStoppedOValid = (void *)1;
        }

        if (GState == 0 && camera->mStoppedOValid != 0) {
            if (((camera->mPrevColAngBuffer[0] +
                  camera->mColAngBuffer[0]) >> 1) == 0 &&
                ((camera->mPrevColAngBuffer[1] +
                  camera->mColAngBuffer[1]) >> 1) == 0 &&
                ((camera->mPrevColAngBuffer[3] +
                  camera->mColAngBuffer[3]) >> 1) == 0) {
                if (GenerateLevel == 0) {
                    Quat_Slerp(
                        camera->mO, camera->mDesiredO, 0x5e7,
                        camera->mStoppedO);
                }
            }

            camera->mDesiredO = camera->mStoppedO;

            int stopped_angle =
                (camera->mPrevColAngBuffer[1] +
                 camera->mColAngBuffer[1]) >> 1;
            if (stopped_angle != 0 &&
                ((camera->mPrevColAngBuffer[0] +
                  camera->mColAngBuffer[0]) >> 1) == 0 &&
                ((camera->mPrevColAngBuffer[3] +
                  camera->mColAngBuffer[3]) >> 1) == 0) {
                camera->mDesiredO = operator*(
                    camera->mDesiredO,
                    QFromYRot((s16)stopped_angle));
            } else if (
                ((camera->mPrevColAngBuffer[1] +
                  camera->mColAngBuffer[1]) >> 1) == 0 &&
                ((camera->mPrevColAngBuffer[0] +
                  camera->mColAngBuffer[0]) >> 1) != 0 &&
                ((camera->mPrevColAngBuffer[3] +
                  camera->mColAngBuffer[3]) >> 1) == 0) {
                camera->mDesiredO = operator*(
                    camera->mDesiredO,
                    QFromYRot((s16)(
                        (camera->mPrevColAngBuffer[0] +
                         camera->mColAngBuffer[0]) >> 1)));
            }
        }
    } else {
        camera->mStoppedOValid = 0;
        camera->mStoppedExtraYRot = 0;
    }

    if (camera->mStoppedExtraYRot != 0) {
        CQuat rotation = QFromYRot(camera->mStoppedExtraYRot);
        CQuat &desired_o = camera->mDesiredO;
        int ox = desired_o.x;
        int oy = desired_o.y;
        int oz = desired_o.z;
        int ow = desired_o.w;
        int nw =
            (ow * rotation.w - ox * rotation.x - oy * rotation.y -
             oz * rotation.z) >> 12;
        desired_o.x =
            (ow * rotation.x + ox * rotation.w + oy * rotation.z -
             oz * rotation.y) >> 12;
        desired_o.y =
            (ow * rotation.y + oy * rotation.w + oz * rotation.x -
             ox * rotation.z) >> 12;
        desired_o.z =
            (ow * rotation.z + oz * rotation.w + ox * rotation.y -
             oy * rotation.x) >> 12;
        desired_o.w = nw;
    }
}


/**
 * Move the camera toward its collision-adjusted desired position.
 *
 * Address:   0x8007B284
 * Mangled:   MoveToDesiredPos__7CCameraUi
 * Demangled: CCamera::MoveToDesiredPos(unsigned int)
 *
 * @param normal_slerp  Orientation interpolation used outside special modes.
 */
void CCamera::MoveToDesiredPos(unsigned int normal_slerp) {
    mPrevO = mO;

    if (mTripod == 0) {
        CVector U;
        CVector V;
        CVector N;
        int dot;

        Utils_CalcUnit(N, mFocusPos - mTripodPos);

        V.vx = 0;
        V.vy = 0x1000;
        V.vz = 0;
        dot =
            (Mips_CVecDotProd(&N, &V) ^
             (Mips_CVecDotProd(&N, &V) >> 31)) -
            (Mips_CVecDotProd(&N, &V) >> 31);

        if (dot >= 0xfe0) {
            U.vx = 0x1000;
            U.vy = 0;
            U.vz = 0;

            gte_ldopv1(&N);
            gte_ldopv2(&U);
            gte_op0();
            gte_stlvnl(&V);
            V.vx >>= 12;
            V.vy >>= 12;
            V.vz >>= 12;

            gte_ldopv1(&V);
            gte_ldopv2(&N);
            gte_op0();
            gte_stlvnl(&U);
            U.vx >>= 12;
            U.vy >>= 12;
            U.vz >>= 12;
        } else {
            gte_ldopv1(&V);
            gte_ldopv2(&N);
            gte_op0();
            gte_stlvnl(&U);
            U.vx >>= 12;
            U.vy >>= 12;
            U.vz >>= 12;

            gte_ldopv1(&N);
            gte_ldopv2(&U);
            gte_op0();
            gte_stlvnl(&V);
            V.vx >>= 12;
            V.vy >>= 12;
            V.vz >>= 12;
        }

        tm.m[0][0] = (s16)U.vx;
        tm.m[1][0] = (s16)U.vy;
        tm.m[2][0] = (s16)U.vz;
        tm.m[0][1] = (s16)V.vx;
        tm.m[1][1] = (s16)V.vy;
        tm.m[2][1] = (s16)V.vz;
        tm.m[0][2] = (s16)N.vx;
        tm.m[1][2] = (s16)N.vy;
        tm.m[2][2] = (s16)N.vz;
        CQuat tempO;
        MToQ(tm, tempO);

        if (new_focus != 0) {
            mO = tempO;
            new_focus = 0;
        } else {
            Quat_Slerp(
                mPrevO, tempO, 0x7ff, mO);
        }

        mPos = mTripodPos;
        return;
    }

    CVector old_pos;
    CVector Dist;
    int Spd;
    int Frames;
    int mph;
    VECTOR v1;
    VECTOR v2;
    int speed_lag_dist;
    int temptime;

    old_pos = mPos;
    Dist = mTripod->mVel;

    if (((CameraBruceMoveOwner *)mTripod)->mPhysicsState ==
            PHYSICS_ON_GROUND ||
        ((CameraBruceMoveOwner *)mTripod)->mPhysicsState ==
            PHYSICS_ON_RAIL) {
        for (int i = 5; i > 0; i--) {
            mSpeedBuffer[i] = mSpeedBuffer[i - 1];
        }
        if (Dist.Length() > 100) {
            Dist.vx = 0x64000;
            Dist.vy = 0;
            Dist.vz = 0;
        }
        mSpeedBuffer[0] =
            M3dMaths_SquareRoot0(Mips_CVecDotProd(&Dist, &Dist)) << 6;
    }

    Spd = 0;
    for (int i = 0; i < 6; i++) {
        Spd += mSpeedBuffer[i];
    }

    Spd /= 6;
    Frames = 2;
    mph = Spd / 34 * 60 / Frames * 3600 / 5278;
    mph >>= 12;
    mSpeed = mph;

    v1.vx = 0;
    v1.vy = 0;
    speed_lag_dist =
        (Camera_CamDist + mph * 4 + prev_speed_lag_dist) >> 1;
    prev_speed_lag_dist = speed_lag_dist;
    v1.vz = -(speed_lag_dist << 12);

    if (turn_delay_timer >= 4) {
        CQuat wouldbeO = operator*(
            mDesiredO, QFromXRot(-450));
        MATRIX m;
        VECTOR v;
        VECTOR vCheck;
        SLineInfo line_info;

        v.vx = 0;
        v.vy = 0;
        v.vz = -(Camera_CamDist << 12);
        QToM(wouldbeO, m);
        ApplyMatrixLV(&m, &v, &vCheck);

        line_info.StartCoords.vx = mTripodPos.vx;
        line_info.StartCoords.vy = mTripodPos.vy;
        line_info.StartCoords.vz = mTripodPos.vz;
        line_info.EndCoords.vx = mTripodPos.vx + vCheck.vx;
        line_info.EndCoords.vy = mTripodPos.vy + vCheck.vy;
        line_info.EndCoords.vz = mTripodPos.vz + vCheck.vz;
        M3dColij_InitLineInfo(&line_info);
        line_info.RecordTriggerZoneHits = 0;
        CameraCollisionCheck = 1;
        M3dZone_LineToItem(&line_info, 1);
        CameraCollisionCheck = 0;

        if (line_info.pItem == 0) {
            mDesiredO = wouldbeO;
        }
    }

    if (Mick_CameraTweak == 0 &&
        ((big_air != 0 &&
          ((CameraBruceMoveOwner *)mTripod)->mDoingATrick != 0) ||
         big_air_timer > 0)) {
        ReactToCollision();

        if (big_air_timer == 0) {
            big_air_mph = mph;
        }
        if (big_air_mph > 30) {
            big_air_mph = 30;
        }

        if (big_air == 0) {
            temptime = big_air_timer;
            if (temptime <= 0) {
                temptime = 0;
            } else if (big_air_end_timer == 0) {
                if (temptime >= 8) {
                    temptime = 7;
                }
                big_air_end_timer = temptime + 1;
            } else {
                temptime = big_air_end_timer - 1;
            }

            if (temptime == 0) {
                big_air_end_timer = 1;
                big_air_timer = -1;
                new_hang_time = 0;
            }
            temptime = temptime * temptime;
            --big_air_end_timer;
            int camera_lift = big_air_mph * 8000;
            Camera_CamOffset.vy +=
                (temptime * camera_lift) / 30;
            int scaled_time = temptime * 75;
            v1.vz += (big_air_mph * 8) * scaled_time;
        } else {
            big_air_end_timer = 0;
            if (big_air_timer >= 7) {
                temptime = 49;
                int camera_lift = big_air_mph * 8000;
                Camera_CamOffset.vy +=
                    (temptime * camera_lift) / 30;
                int scaled_time = temptime * 600;
                v1.vz += scaled_time * big_air_mph;
                ++new_hang_time;
            } else if (big_air_timer >= 0) {
                new_hang_time = 0;
                temptime = big_air_timer * big_air_timer;
                int camera_lift = big_air_mph * 8000;
                Camera_CamOffset.vy +=
                    (temptime * camera_lift) / 30;
                int scaled_time = temptime * 75;
                v1.vz += (big_air_mph * 8) * scaled_time;
            }
        }

        Quat_Slerp(
            mPrevO, mDesiredO, 0xde, mO);
        ++big_air_timer;
    } else {
        void *loosen_up = 0;
        if (loose_cam_timer > 0) {
            loosen_up = (void *)1;
            --loose_cam_timer;
        }

        mDesiredO = operator*(
            mDesiredO, QFromXRot((s16)Camera_CamPitch));
        ReactToCollision();

        if (((CameraBruceMoveOwner *)mTripod)->mManIsJumping != 0) {
            if (((CameraBruceMoveOwner *)mTripod)->mPhysicsState ==
                PHYSICS_IN_AIR_STICK_TO) {
                loosen_up = (void *)1;
                loose_cam_timer = 20;
            } else if (
                ((CameraBruceMoveOwner *)mTripod)->mPhysicsState ==
                PHYSICS_IN_AIR) {
                mDesiredO = operator*(
                    mDesiredO, QFromXRot(200));
            }
        }

        if ((u32)(in_pool_timer - 2) < 18) {
            Quat_Slerp(
                mPrevO, mDesiredO, 0x137, mO);
        } else {
            if (loosen_up) {
                int loosen_slerp =
                    (20 - loose_cam_timer) * 13 + 51;
                Quat_Slerp(
                    mPrevO, mDesiredO, loosen_slerp, mO);
            } else {
                Quat_Slerp(
                    mPrevO, mDesiredO, normal_slerp,
                    mO);
            }
        }
    }

    if (mTimer < 12) {
        mPrevO = mO = mDesiredO;
    }

    MATRIX m;
    QToM(mO, m);
    ApplyMatrixLV(&m, &v1, &v2);
    v1.vx = Camera_CamOffset.vx;
    v1.vy = Camera_CamOffset.vy;
    v1.vz = Camera_CamOffset.vz;
    ApplyMatrixLV(&m, &v1, &real_cam_offset);

    mPos.vx = mTripodPos.vx + v2.vx;
    mPos.vy = mTripodPos.vy + v2.vy;
    mPos.vz = mTripodPos.vz + v2.vz;
    mVel = mPos - old_pos;
}


/**
 * SetCamAngle__7CCamerasUs (intentional no-op in this build)
 *
 * Address:   0x8007BFAC
 * Mangled:   SetCamAngle__7CCamerasUs
 * Demangled: CCamera::SetCamAngle(short, unsigned short)
 *            ASM: jr $ra; nop
 *
 * @param y  Y coordinate.
 * @param frames  Frame count.
 */
void CCamera::SetCamAngle(s16 y, u16 frames) {
}


/**
 * SetCamXZDistance__7CCameraUsUs (intentional no-op in this build)
 *
 * Address:   0x8007BFB4
 * Mangled:   SetCamXZDistance__7CCameraUsUs
 * Demangled: CCamera::SetCamXZDistance(unsigned short, unsigned short)
 *            ASM: jr $ra; nop
 *
 * @param dist Requested horizontal camera distance.
 * @param frames  Frame count.
 */
void CCamera::SetCamXZDistance(u16 dist, u16 frames) {
}


/**
 * SetCamYDistance__7CCamerasUs (intentional no-op in this build)
 *
 * Address:   0x8007BFBC
 * Mangled:   SetCamYDistance__7CCamerasUs
 * Demangled: CCamera::SetCamYDistance(short, unsigned short)
 *            ASM: jr $ra; nop
 *
 * @param dist Requested vertical camera distance.
 * @param frames  Frame count.
 */
void CCamera::SetCamYDistance(s16 dist, u16 frames) {
}


/**
 * SetCamXOffset__7CCamerasUs (intentional no-op in this build)
 *
 * Address:   0x8007BFC4
 * Mangled:   SetCamXOffset__7CCamerasUs
 * Demangled: CCamera::SetCamXOffset(short, unsigned short)
 *            ASM: jr $ra; nop
 *
 * @param offset Requested horizontal camera offset.
 * @param frames  Frame count.
 */
void CCamera::SetCamXOffset(s16 offset, u16 frames) {
}


/**
 * SetCamYOffset__7CCamerasUs (intentional no-op in this build)
 *
 * Address:   0x8007BFCC
 * Mangled:   SetCamYOffset__7CCamerasUs
 * Demangled: CCamera::SetCamYOffset(short, unsigned short)
 *            ASM: jr $ra; nop
 *
 * @param offset Requested vertical camera offset.
 * @param frames  Frame count.
 */
void CCamera::SetCamYOffset(s16 offset, u16 frames) {
}


/**
 * SetCamZOffset__7CCamerasUs (intentional no-op in this build)
 *
 * Address:   0x8007BFD4
 * Mangled:   SetCamZOffset__7CCamerasUs
 * Demangled: CCamera::SetCamZOffset(short, unsigned short)
 *            ASM: jr $ra; nop
 *
 * @param offset Requested depth camera offset.
 * @param frames  Frame count.
 */
void CCamera::SetCamZOffset(s16 offset, u16 frames) {
}


/**
 *
 * Address:   0x8007BFDC
 * Mangled:   SetZoom__7CCameraUsUs
 * Demangled: CCamera::SetZoom(unsigned short, unsigned short)
 *            Store a new zoom target immediately, or compute a per-frame zoom delta.
 *
 * @param camera Camera whose zoom is changed.
 * @param zoom Target fixed-point projection zoom.
 * @param time Number of frames over which to interpolate.
 */
void Camera_SetZoom(CCamera *camera, int zoom, int time) {
    CCamera *self = camera;
    register int frameCount asm("$6") = (u16)time;
    asm volatile("" : "+r"(self), "+r"(frameCount));
    self->mZoomTime = frameCount;
    zoom = (u16)zoom;
    if (frameCount != 0) {
        int current = self->mZoom;
        register int den asm("$3") = (u16)frameCount;
        self->mZoomIncrement = ((int)zoom - current) / (int)den;
    } else {
        self->mZoom = zoom;
    }
}


/**
 * GetZoom__7CCamera
 *
 * Address:   0x8007C014
 * Mangled:   GetZoom__7CCamera
 * Demangled: CCamera::GetZoom(void)
 *
 * @return Current fixed-point projection zoom.
 */
int CCamera::GetZoom() {
    return mZoom;
}


/**
 * TweakMenu__7CCamera
 *
 * Address:   0x8007C020
 * Mangled:   TweakMenu__7CCamera
 * Demangled: CCamera::TweakMenu(void)
 *
 */
void CCamera::TweakMenu() {
    CCamera *camera = this;
    u8 c8 = 0xc8;

    // Display labels and camera positions
    CamPos::DisplayLabels(0x64, 0x32, c8, c8, c8);
    Font_String(
        0xc8, 0x28, CameraPositionATextOwner, c8, c8, c8, 0);
    CamPos *camPos0 = &camera->mPosA;
    camPos0->Display(0xc8, 0x32, 0x50, 0, 0);
    Font_String(
        0x12c, 0x28, CameraPositionBTextOwner, c8, c8, c8, 0);
    CamPos *camPos1 = &camera->mPosB;
    camPos1->Display(0x12c, 0x32, 0x50, 0, 0);

    // Check Right.Triggered (Pad[1])
    if (Pad[1].Right.Triggered != 0) {
        u16 col = TweakCol;
        switch (col) {
        case 0:
            TweakCol = 1;
            break;
        case 1:
            TweakCol = col;
            break;
        default:
            Pad[1].Right.Triggered = 0;
            goto check_left;
        }
        Pad[1].Right.Triggered = 0;
    }
check_left:
    // Check Left.Triggered (Pad[1])
    if (Pad[1].Left.Triggered != 0) {
        u16 col = TweakCol;
        switch (col) {
        case 0:
            TweakCol = 0;
            break;
        case 1:
            TweakCol = 0;
            break;
        }
        Pad[1].Left.Triggered = 0;
    }

    // Check Down.Triggered
    if (Pad[1].Down.Triggered != 0) {
        u16 row = TweakRow;
        if (row < 11) {
            switch (row) {
            case 0: TweakRow = 1; break;
            case 1: TweakRow = 2; break;
            case 2: TweakRow = 3; break;
            case 3: TweakRow = 5; break;
            case 4: break;
            case 5: TweakRow = 6; break;
            case 6: TweakRow = 8; break;
            case 7: break;
            case 8: TweakRow = 9; break;
            case 9: TweakRow = 10; break;
            case 10: TweakRow = 0; break;
            }
        }
        Pad[1].Down.Triggered = 0;
    }

    // Check Up.Triggered
    if (Pad[1].Up.Triggered != 0) {
        u16 row = TweakRow;
        if (row < 11) {
            switch (row) {
            case 0: TweakRow = 10; break;
            case 1: TweakRow = 0;
                goto clear_up;
            case 2: TweakRow = 1; break;
            case 3: TweakRow = 2; break;
            case 4: break;
            case 5: TweakRow = 3; break;
            case 6: TweakRow = 5; break;
            case 7: break;
            case 8: TweakRow = 6; break;
            case 9: TweakRow = 8; break;
            case 10: TweakRow = 9; break;
            }
        }
    clear_up:
        Pad[1].Up.Triggered = 0;
    }

    // Select active CamPos based on TweakCol
    CamPos *cp;
    u16 col = TweakCol;
    switch (col) {
    case 0:
        cp = camPos0;
        break;
    case 1:
        cp = camPos1;
        break;
    }
    // Triangle.Pressed (Pad[1], offset 0x16c)
    if (Pad[1].Triangle.Pressed != 0) {
        switch (TweakRow) {
        case 5:
            cp->mStickO = operator*(
                cp->mStickO, QFromXRot(0xa));
            break;
        case 6:
            cp->mStickLength -= 10;
            break;
        }
    }

    // X.Pressed (Pad[1], offset 0x19c)
    if (Pad[1].X.Pressed != 0) {
        switch (TweakRow) {
        case 5:
            cp->mStickO = operator*(
                cp->mStickO, QFromXRot(-0xa));
            break;
        case 6:
            cp->mStickLength += 10;
            break;
        }
    }

    // Square.Pressed (Pad[1], offset 0x17c)
    if (Pad[1].Square.Pressed != 0 && TweakRow == 5) {
        cp->mStickO = operator*(
            cp->mStickO, QFromYRot(-0xa));
    }

    // Circle.Pressed (Pad[1], offset 0x18c)
    if (Pad[1].Circle.Pressed != 0 && TweakRow == 5) {
        cp->mStickO = operator*(
            cp->mStickO, QFromYRot(0xa));
    }

    // RightOne.Pressed (Pad[1], offset 0x1cc)
    if (Pad[1].RightOne.Pressed != 0 && TweakRow == 5) {
        cp->mStickO = operator*(
            cp->mStickO, QFromZRot(0xa));
    }

    // RightTwo.Pressed (Pad[1], offset 0x1dc)
    if (Pad[1].RightTwo.Pressed != 0) {
        if (TweakRow == 5) {
            cp->mStickO = operator*(
                cp->mStickO, QFromZRot(-0xa));
        }
    }
}


// === CAMERA-owned small-data prefix needed by CCamera::AI ===

/*
 * CAMERA's initialized render values and list state. The public scalar
 * declarations above retain the target's proven gp-relative access pattern.
 */
static int CameraOwnerCamPitch
    __attribute__((section(".sdata"))) = 54;
static int CameraOwnerCamDist
    __attribute__((section(".sdata"))) = 850;
static int CameraOwnerCamOffset[3]
    __attribute__((section(".sdata"))) = {0, 0, 0};
static int CameraOwnerNumCameras
    __attribute__((section(".sdata"))) = 0;
static CBody *CameraOwnerList
    __attribute__((section(".sdata"))) = 0;
static u32 CameraOwnerCurrentMike
    __attribute__((section(".sdata"))) = 0;

/** Camera-mode diagnostics retained by the original debug build. */
static char CameraBigAirOverText[]
    __attribute__((section(".sdata"), aligned(4))) = "big air over";
static char CameraBigAirText[]
    __attribute__((section(".sdata"), aligned(4))) = "big air";
static char CameraResettingText[]
    __attribute__((section(".sdata"), aligned(4))) = "reseting";
static char CameraRampingText[]
    __attribute__((section(".sdata"), aligned(4))) = "ramping";
static char CameraNormalText[]
    __attribute__((section(".sdata"), aligned(4))) = "normal";
static char CameraLooseText[]
    __attribute__((section(".sdata"), aligned(4))) = "loose cam";

/** Initial state for CCamera's owner-local tweak controls. */
int CCamera::Tweaking
    __attribute__((section(".sdata"))) = 0;
/*
 * Keep the row and column backing bytes at their target-proven .sdata offsets.
 * Their natural static-member references must remain unresolved here: ASPSX
 * expands same-TU definitions into absolute sequences, while the target uses
 * GP-relative halfword relocations for all 25 accesses in TweakMenu.
 */
static u16 CameraOwnerTweakRow
    __attribute__((section(".sdata"))) = 0;
static u16 CameraOwnerTweakCol
    __attribute__((section(".sdata"))) = 0;

/** Headings used by CCamera and CamPos's debug displays. */
static char CameraPositionATextOwner[]
    __attribute__((section(".sdata"), aligned(4))) = "Pos A";
static char CameraPositionBTextOwner[]
    __attribute__((section(".sdata"), aligned(4))) = "Pos B";

/* Owner-private AI state immediately follows the tweak labels in the target. */
namespace {
static int Released
    __attribute__((section(".sdata"))) = 1;
static int Mick_DisplayCameraValues
    __attribute__((section(".sdata"))) = 1;
static int old_GlobalZoom
    __attribute__((section(".sdata"))) = 0x100;
static int Mick_Select_Triggered
    __attribute__((section(".sdata"))) = 0;
static int Mick_Select_Released
    __attribute__((section(".sdata"))) = 0;
}

/**
 * Update the active camera's tracking, tweak controls, and shake response.
 *
 * Address:   0x8007C6C4
 * Mangled:   AI__7CCamera
 * Demangled: CCamera::AI(void)
 *
 */
void CCamera::AI() {
    CCamera *camera = this;
    static int old_camYOffset;
    static int old_camDist;
    static int old_camPitch;

    Camera_CurrentMikeCamera = camera->mMikeIndex;

    if (camera->mCalcTripod != 0) {
        camera->mOldTripodPos = camera->mTripodPos;
        if (camera->new_focus != 0) {
            camera->mTripodPos = camera->mTripod->mPos;
        } else {
            camera->mTripodPos =
                ((camera->mTripod->mPos << 1) + camera->mOldTripodPos) / 3;
        }
    }

    if (camera->mCalcFocus != 0) {
        camera->mOldFocusPos = camera->mFocusPos;
        if (camera->new_focus != 0) {
            camera->mFocusPos = camera->mFocus->mPos;
        } else {
            camera->mFocusPos =
                ((camera->mFocus->mPos << 1) + camera->mOldFocusPos) / 3;
        }
    }

    if (Pad[0].Select.Pressed != 0) {
        if (Mick_Select_Released != 0) {
            Mick_Select_Triggered = 1;
        }
        Mick_Select_Released = 0;
    } else {
        Mick_Select_Released = 1;
    }

    if (Cheat_ScreenShots != 0 &&
        Mick_Select_Triggered != 0 &&
        Pad[0].Select.Pressed != 0) {
        Pad[1].RightOne.Triggered = 0;
        Pad[1].RightTwo.Triggered = 0;
        Mick_DisplayCameraValues = 0;
        Pad[0].LeftTwo.Triggered = 0;
        Mick_Select_Triggered = 0;
        Pad[0].Select.Triggered = 0;
        Pad[0].Start.Triggered = 0;
        Mick_CameraTweak ^= 1;
        GameFrozen = Mick_CameraTweak;

        if (Mick_CameraTweak != 0) {
            old_camYOffset = camYOffset;
            old_camDist = camDist;
            old_camPitch = camPitch;
            old_GlobalZoom = camera->mZoom;
        }
    }

    if (Mick_CameraTweak != 0) {
        if (Pad[1].Start.Pressed != 0) {
            camYOffset = old_camYOffset;
            camDist = old_camDist;
            camPitch = old_camPitch;
            camera->mZoom = old_GlobalZoom;
        }
        if (Pad[0].Square.Pressed != 0) {
            --camera->mZoom;
        }
        if (Pad[0].Circle.Pressed != 0) {
            ++camera->mZoom;
        }
        if (Pad[0].Triangle.Pressed != 0) {
            camera->mZoom = 0x100;
        }
        if (Pad[1].Square.Pressed != 0) {
            camera->extra_YRot -= 10;
        }
        if (Pad[1].Circle.Pressed != 0) {
            camera->extra_YRot += 10;
        }
        if (Pad[1].Left.Pressed != 0) {
            camDist -= Pad[1].Select.Pressed != 0 ? 32 : 8;
        }
        if (Pad[1].Right.Pressed != 0) {
            camDist += Pad[1].Select.Pressed != 0 ? 32 : 8;
        }
        if (Pad[1].Up.Pressed != 0) {
            camPitch -= Pad[1].Select.Pressed != 0 ? 32 : 8;
        }
        if (Pad[1].Down.Pressed != 0) {
            camPitch += Pad[1].Select.Pressed != 0 ? 32 : 8;
        }
        camPitch &= 0xfff;
        if (Pad[1].LeftOne.Pressed != 0) {
            camYOffset -= Pad[1].Select.Pressed != 0 ? 32 : 8;
        }
        if (Pad[1].LeftTwo.Pressed != 0) {
            camYOffset += Pad[1].Select.Pressed != 0 ? 32 : 8;
        }
    }

    Camera_CamDist = camDist;
    Camera_CamPitch = camPitch;
    Camera_CamOffset.vx = 0;
    Camera_CamOffset.vy = camYOffset << 12;
    Camera_CamOffset.vz = 0;

    if (GamePaused != 0) {
        if (Mick_CameraTweak == 0) {
            return;
        }
        camera->CM_Normal();
        camera->MoveToDesiredPos(0x137);
        return;
    }

    ++camera->mTimer;
    if (camera->mZoomTime != 0) {
        --camera->mZoomTime;
        camera->mZoom += camera->mZoomIncrement;
    }
    pCurrentViewport->Zoom = (u16)camera->mZoom;

    Utils_CalcAim(camera->mAngles, camera->mPos, camera->mFocusPos);

    if (((CameraBruceAiOwner *)camera->mTripod)->mInAirTimer != 0 &&
        camera->mTripodPosYDamper != 0) {
        if (((CameraBruceAiOwner *)camera->mTripod)->mVel.vy < 0) {
            ++camera->jump_uptimer;
        } else {
            --camera->jump_uptimer;
        }
        camera->jump_acc +=
            ((-((CameraBruceAiOwner *)camera->mTripod)->mVel.vy >> 12) *
             camera->mTripodPosYDamper);
        if (camera->jump_uptimer > 0) {
            camera->mTripodPos.vy += camera->jump_acc;
        }
    } else {
        camera->jump_acc = 0;
        camera->jump_uptimer = 0;
    }

    if (camera->mPitchDamper != 0) {
        if (((CameraBruceAiOwner *)camera->mTripod)->mVel.vy < 0) {
            Camera_CamPitch -= camera->mPitchDamper;
        }
        if (((CameraBruceAiOwner *)camera->mTripod)->mVel.vy > 0) {
            Camera_CamPitch -= camera->mPitchDamper >> 1;
        }
        Camera_CamPitch &= 0xfff;
    }

    switch (camera->mCameraMode) {
    case 1:
        camera->CM_Normal();
        break;
    case 2:
        camera->CM_NoBigAir();
        break;
    case 0x17:
        camera->CM_CompetitionIntro();
        camera->LoadIntoMikeCamera();
        return;
    default:
        break;
    }

    if (((CameraBruceAiOwner *)camera->mTripod)->mInBail == 0) {
        if (camera->mTripodPos == camera->mOldTripodPos) {
            camera->MoveToDesiredPos(0x97);
        } else if (camera->mSpeed < 4) {
            camera->MoveToDesiredPos(0x97);
        } else if (((CameraBruceAiOwner *)camera->mTripod)->mPhysicsState !=
                   PHYSICS_IN_AIR) {
            camera->MoveToDesiredPos(0x137);
        } else if (camera->mTimer < 0x12) {
            camera->MoveToDesiredPos(0x137);
        } else {
            camera->MoveToDesiredPos(0x97);
        }
    } else {
        camera->MoveToDesiredPos(0x137);
    }
    Utils_CalcAim(camera->mAngles, camera->mPos, camera->mFocusPos);

    if (camera->mShakeAmplitude.vx != 0 ||
        camera->mShakeAmplitude.vy != 0 ||
        camera->mShakeAmplitude.vz != 0) {
        camera->mO = operator*(
            operator*(
                operator*(
                    QFromYRot((s16)(
                        (camera->mShakeAmplitude.vy *
                         Camera_Sine(camera->mShakeSpeed.vy * TTime)) /
                        0x1000)),
                    camera->mO),
                QFromXRot((s16)(
                    (camera->mShakeAmplitude.vx *
                     Camera_Sine(camera->mShakeSpeed.vx * TTime)) /
                    0x1000))),
            QFromZRot((s16)(
                (camera->mShakeAmplitude.vz *
                 Camera_Sine(camera->mShakeSpeed.vz * TTime)) /
                0x1000)));

        CSVector oldamp = camera->mShakeAmplitude;
        if (camera->mShakeAmplitude.vx < 0) {
            camera->mShakeAmplitude.vx += camera->mShakeDecay.vx;
        } else {
            camera->mShakeAmplitude.vx -= camera->mShakeDecay.vx;
        }
        if (camera->mShakeAmplitude.vy < 0) {
            camera->mShakeAmplitude.vy += camera->mShakeDecay.vy;
        } else {
            camera->mShakeAmplitude.vy -= camera->mShakeDecay.vy;
        }
        if (camera->mShakeAmplitude.vz < 0) {
            camera->mShakeAmplitude.vz += camera->mShakeDecay.vz;
        } else {
            camera->mShakeAmplitude.vz -= camera->mShakeDecay.vz;
        }

        if (((u16)oldamp.vx ^ (u16)camera->mShakeAmplitude.vx) & 0x8000) {
            camera->mShakeAmplitude.vx = 0;
        }
        if (((u16)oldamp.vy ^ (u16)camera->mShakeAmplitude.vy) & 0x8000) {
            camera->mShakeAmplitude.vy = 0;
        }
        if (((u16)oldamp.vz ^ (u16)camera->mShakeAmplitude.vz) & 0x8000) {
            camera->mShakeAmplitude.vz = 0;
        }
    }

    camera->LoadIntoMikeCamera();
}


/**
 * CM_Normal__7CCamera
 *
 * Address:   0x8007D138
 * Mangled:   CM_Normal__7CCamera
 * Demangled: CCamera::CM_Normal(void)
 */
void CCamera::CM_Normal() {
    CCamera *camera = this;
    CVector vUp;
    CVector vRight;
    CSVector ang1;
    int dot_front;
    CVector temp_vel = camera->mTripodPos - camera->mOldTripodPos;

    vUp = ((CameraBruceMoveOwner *)camera->mTripod)->mUp;

    CVector Down(0, 0x1000, 0);
    CVector Zero(0, 0, 0);

    // The original branchless ABS macro evaluates its argument three times.
    int dot_up =
        (Mips_CVecDotProd(&Down, &vUp) ^
         (Mips_CVecDotProd(&Down, &vUp) >> 31)) -
        (Mips_CVecDotProd(&Down, &vUp) >> 31);

    Utils_CalcAim(ang1, Zero, temp_vel);
    CVector temp_front;
    Utils_GetVecFromMagDir(temp_front, 0x1000, ang1);
    dot_front =
        (Mips_CVecDotProd(&temp_front, &vUp) ^
         (Mips_CVecDotProd(&temp_front, &vUp) >> 31)) -
        (Mips_CVecDotProd(&temp_front, &vUp) >> 31);

    ++camera->in_pool_timer;
    if (dot_up < 0xdf4) {
        if (((CameraBruceMoveOwner *)camera->mTripod)->mPhysicsState ==
                PHYSICS_ON_INVISIBLE ||
            ((CameraBruceMoveOwner *)camera->mTripod)->mPhysicsState ==
                PHYSICS_IN_HANDPLANT) {
            if (camera->big_air == 0 &&
                ((CameraBruceMoveOwner *)camera->mTripod)->mManIsJumping != 0) {
                ++((CameraBruceMoveOwner *)camera->mTripod)->mBigAirs;
            }

            ++camera->turn_delay_timer;
            if (camera->turn_delay_timer >= 4) {
                CVector U(0, 0x1000000, 0);
                camera->vFront = U;
            } else {
                Utils_GetVecFromMagDir(
                    camera->vFront, 0x1000, ang1);
            }
            camera->big_air = (void *)1;
            camera->in_pool_timer = 0;
            goto front_selected;
        }
    }

    if ((dot_front >> 12) >= 0x7d1 ||
        temp_vel.Length() < 5) {
        camera->turn_delay_timer = 0;
        if (dot_up < 0xdf4 && camera->big_air == (void *)1) {
            camera->in_pool_timer = 0;
        }
        camera->big_air = 0;
    } else {
        camera->turn_delay_timer = 0;
        Utils_GetVecFromMagDir(
            camera->vFront, 0x1000, ang1);
        if (dot_up < 0xdf4 && camera->big_air == (void *)1) {
            camera->in_pool_timer = 0;
        }
        camera->big_air = 0;
    }

front_selected:
    camera->mOldVFront = camera->mVFront;
    if (camera->mTripodPos == camera->mOldTripodPos) {
        camera->mVFront = camera->mOldVFront;
    } else {
        camera->mVFront = (camera->mOldVFront + camera->vFront) >> 1;
    }

    if (camera->mTimer < 12) {
        camera->mVFront = Camera_VectorNegate(
        reinterpret_cast<CameraCompetitionSkater *>(GSkater[0])->mFront) << 12;
        camera->mOldVFront = camera->mVFront;
        camera->vFront = camera->mOldVFront;
    }

    camera->vFront = camera->mVFront;
    CVector U;
    CVector V;
    CVector N;
    N = camera->vFront >> 12;
    V.vx = -vUp.vx;
    V.vy = -vUp.vy;
    V.vz = -vUp.vz;

    gte_ldopv1(&V);
    gte_ldopv2(&N);
    gte_op0();
    gte_stlvnl(&U);
    U.vx >>= 12;
    U.vy >>= 12;
    U.vz >>= 12;

    gte_ldopv1(&U);
    gte_ldopv2(&V);
    gte_op0();
    gte_stlvnl(&N);
    N.vx >>= 12;
    N.vy >>= 12;
    N.vz >>= 12;

    tm.m[0][0] = (s16)U.vx;
    tm.m[1][0] = (s16)U.vy;
    tm.m[2][0] = (s16)U.vz;
    tm.m[0][1] = (s16)V.vx;
    tm.m[1][1] = (s16)V.vy;
    tm.m[2][1] = (s16)V.vz;
    tm.m[0][2] = (s16)N.vx;
    tm.m[1][2] = (s16)N.vy;
    tm.m[2][2] = (s16)N.vz;
    MToQ(tm, camera->mDesiredO);

    camera->mDesiredO = operator*(
        camera->mDesiredO, QFromYRot((s16)camera->extra_YRot));
}


/**
 * StartCompetitionIntro__7CCamera
 *
 * Address:   0x8007D708
 * Mangled:   StartCompetitionIntro__7CCamera
 * Demangled: CCamera::StartCompetitionIntro(void)
 *
 */
void CCamera::StartCompetitionIntro() {
    CCamera *camera = this;
    camera->mCompIntroTimer = 0;
    camera->mCameraMode = 0x17;
    Competition_ResetMarquee();
}


/**
 * Advance the competition-intro camera and transition into the run video.
 *
 * Address:   0x8007D730
 * Mangled:   CM_CompetitionIntro__7CCamera
 * Demangled: CCamera::CM_CompetitionIntro(void)
 *
 */
void CCamera::CM_CompetitionIntro() {
    CCamera *camera = this;

    if (camera->mCompIntroTimer == 0) {
        VECTOR v1;
        VECTOR v2;
        int speed_lag_dist;
        MATRIX m;

        camera->mCameraMode = 1;
        reinterpret_cast<CameraCompetitionSkater *>(GSkater[0])->mVel.vx =
            reinterpret_cast<CameraCompetitionSkater *>(GSkater[0])->mFront.vx * 20;
        reinterpret_cast<CameraCompetitionSkater *>(GSkater[0])->mVel.vy =
            reinterpret_cast<CameraCompetitionSkater *>(GSkater[0])->mFront.vy * 20;
        reinterpret_cast<CameraCompetitionSkater *>(GSkater[0])->mVel.vz =
            reinterpret_cast<CameraCompetitionSkater *>(GSkater[0])->mFront.vz * 20;

        int i = 0;
        do {
            ++i;
            Ob_AI(&CameraList);
        } while (i < 10);

        reinterpret_cast<CameraCompetitionSkater *>(GSkater[0])->mVel.vx = 0;
        reinterpret_cast<CameraCompetitionSkater *>(GSkater[0])->mVel.vy = 0;
        speed_lag_dist = Camera_CamDist >> 1;
        reinterpret_cast<CameraCompetitionSkater *>(GSkater[0])->mVel.vz = 0;
        camera->mCameraMode = 0x17;

        camera->prev_speed_lag_dist = speed_lag_dist;
        v1.vx = 0;
        v1.vy = 0;
        v1.vz = -(speed_lag_dist << 12);

        QToM(camera->mO, m);
        ApplyMatrixLV(&m, &v1, &v2);
        v1.vx = Camera_CamOffset.vx;
        v1.vy = Camera_CamOffset.vy;
        v1.vz = Camera_CamOffset.vz;
        ApplyMatrixLV(&m, &v1, &camera->real_cam_offset);

        camera->mPos.vx = camera->mTripodPos.vx + v2.vx;
        camera->mPos.vy = camera->mTripodPos.vy + v2.vy;
        camera->mCompIntroEndPos.vx = camera->mPos.vx;
        camera->mPos.vz = camera->mTripodPos.vz + v2.vz;
        camera->mCompIntroEndPos.vy = camera->mPos.vy;
        camera->mCompIntroEndPos.vz = camera->mPos.vz;
    }

    /* The trailing legacy barrier keeps the two automatic CQuat destructor
     * stores ahead of register restores. Removing it, adding `return`, and a
     * do/while scope produced 6, 6, and 28 real mismatches respectively. */
    {
    CQuat begin_o;
    CQuat end_o;
    CVector begin_pos;
    end_o = camera->mDesiredO;
    begin_o = operator*(
        operator*(
            QFromYRot(
                Levels[GLevel].IntroStartAngY),
            QFromXRot(
                Levels[GLevel].IntroStartAngX)),
        QFromZRot(
            Levels[GLevel].IntroStartAngZ));

    int slerp_time =
        (int)(((u32)camera->mCompIntroTimer << 12) / 130u);
    Quat_Slerp(begin_o, end_o, slerp_time, camera->mO);

    if ((u32)camera->mCompIntroTimer >= 131u) {
        Video_Start(
            GSkater[0],
            VIDEO_RECORD, 1, 0x7fffffff);
        camera->mCameraMode = 1;
    } else {
        Competition_MarqueeDisplay();
        begin_pos.vx =
            Levels[GLevel].IntroStartPosX << 12;
        begin_pos.vy =
            Levels[GLevel].IntroStartPosY << 12;
        begin_pos.vz =
            Levels[GLevel].IntroStartPosZ << 12;
        CVector offset_inc;
        offset_inc = (begin_pos - camera->mCompIntroEndPos) / 130;
        CVector offset =
            (offset_inc * 130) -
            (camera->mCompIntroTimer * offset_inc);

        camera->mPos.vx = camera->mCompIntroEndPos.vx + offset.vx;
        camera->mPos.vy = camera->mCompIntroEndPos.vy + offset.vy;
        camera->mPos.vz = camera->mCompIntroEndPos.vz + offset.vz;

        int activated = 0;
        if (Pad[0].X.Pressed != 0 || Pad[1].X.Pressed != 0) {
            activated = 1;
        }
        if (activated) {
            camera->mSpeedUpIntro = (void *)1;
        }

        if (camera->mSpeedUpIntro != 0) {
            camera->mCompIntroTimer += 6;
        } else {
            camera->mCompIntroTimer += 1;
        }
    }
    }
    asm volatile("");
}


/**
 * InCompetitionIntro__7CCamera
 *
 * Address:   0x8007DC78
 * Mangled:   InCompetitionIntro__7CCamera
 * Demangled: CCamera::InCompetitionIntro(void)
 *
 * @return Nonzero while competition-intro mode is active.
 */
int CCamera::InCompetitionIntro() {
    return mCameraMode == 0x17;
}


/**
 * CM_NoBigAir__7CCamera
 *
 * Address:   0x8007DC8C
 * Mangled:   CM_NoBigAir__7CCamera
 * Demangled: CCamera::CM_NoBigAir(void)
 *
 */
void CCamera::CM_NoBigAir() {
    CCamera *camera = this;
    CVector vUp;
    CVector vRight;
    CSVector ang1;
    int dot_front;
    CVector temp_vel = camera->mTripodPos - camera->mOldTripodPos;

    if (camera->mTripod == 0) {
        vUp = CVector(0, -0x1000, 0);
    } else {
        vUp = ((CameraBruceMoveOwner *)camera->mTripod)->mUp;
    }

    CVector Down(0, 0x1000, 0);
    CVector Zero(0, 0, 0);

    // The original branchless ABS macro evaluates its argument three times.
    int dot_up =
        ((Mips_CVecDotProd(&Down, &vUp) ^
          (Mips_CVecDotProd(&Down, &vUp) >> 31)) -
         (Mips_CVecDotProd(&Down, &vUp) >> 31)) >> 12;
    (void)dot_up;

    Utils_CalcAim(ang1, Zero, temp_vel);
    CVector temp_front;
    Utils_GetVecFromMagDir(temp_front, 0x1000, ang1);
    dot_front =
        ((Mips_CVecDotProd(&temp_front, &vUp) ^
          (Mips_CVecDotProd(&temp_front, &vUp) >> 31)) -
         (Mips_CVecDotProd(&temp_front, &vUp) >> 31)) >> 12;

    if (dot_front >= 0x7d1) {
        camera->big_air = 0;
    } else if (temp_vel.Length() < 5) {
        camera->big_air = 0;
    } else {
        camera->big_air = 0;
        Utils_GetVecFromMagDir(
            camera->vFront, 0x1000, ang1);
    }

    camera->mOldVFront = camera->mVFront;
    if (camera->mTripodPos == camera->mOldTripodPos) {
        camera->mVFront = camera->mOldVFront;
    } else {
        camera->mVFront = (camera->mOldVFront + camera->vFront) >> 1;
    }

    camera->vFront = camera->mVFront;
    CVector U;
    CVector V;
    CVector N;
    N = camera->vFront >> 12;
    V.vx = -vUp.vx;
    V.vy = -vUp.vy;
    V.vz = -vUp.vz;

    gte_ldopv1(&V);
    gte_ldopv2(&N);
    gte_op0();
    gte_stlvnl(&U);
    U.vx >>= 12;
    U.vy >>= 12;
    U.vz >>= 12;

    gte_ldopv1(&U);
    gte_ldopv2(&V);
    gte_op0();
    gte_stlvnl(&N);
    N.vx >>= 12;
    N.vy >>= 12;
    N.vz >>= 12;

    tm.m[0][0] = (s16)U.vx;
    tm.m[1][0] = (s16)U.vy;
    tm.m[2][0] = (s16)U.vz;
    tm.m[0][1] = (s16)V.vx;
    tm.m[1][1] = (s16)V.vy;
    tm.m[2][1] = (s16)V.vz;
    tm.m[0][2] = (s16)N.vx;
    tm.m[1][2] = (s16)N.vy;
    tm.m[2][2] = (s16)N.vz;
    MToQ(tm, camera->mDesiredO);

    camera->mDesiredO = operator*(
        camera->mDesiredO, QFromYRot((s16)camera->extra_YRot));
    camera->big_air = 0;
}


/**
 * Construct a camera-pose record and its embedded quaternion values.
 *
 * Address:   0x8007E0E8
 * Mangled:   __6CamPos
 * Demangled: CamPos::(void)
 *
 */
CamPos::CamPos() : CClass((void **)&__vt_6CamPos) {}


/**
 * Restore CamPos's table while its embedded quaternion members unwind.
 *
 * Address:   0x8007E148
 * Mangled:   _._6CamPos
 * Demangled: CamPos::~CamPos()
 *
 * The compiler supplies the original hidden in-charge flag and emits the
 * target `_._6CamPos` ABI entry.
 */
CamPos::~CamPos() {
    Camera_SetClassVtable(CClass, (int)&__vt_6CamPos);
}


/**
 * Draw this camera-pose record's tracked tripod and stick values.
 *
 * Address: 0x8007E18C
 * Demangled: CamPos::Display(unsigned short, unsigned short, unsigned char, unsigned char, unsigned char)
 */
void CamPos::Display(u16 x, u16 y, u8 r, u8 g, u8 b) {
    CamPos *camPos = this;
    if (camPos->calc_mTripod != 0) {
        Font_String(
            x, y, CameraFollowTextOwner, r, g, b, 0);
        Font_Number(
            x, (u16)(y + 8), camPos->mTripod->mPos.vx >> 12,
            r, g, b, 0);
        Font_Number(
            x, (u16)(y + 16), camPos->mTripod->mPos.vy >> 12,
            r, g, b, 0);
        Font_Number(
            x, (u16)(y + 24), camPos->mTripod->mPos.vz >> 12,
            r, g, b, 0);
    } else {
        Font_String(
            x, y, CameraFixedTextOwner, r, g, b, 0);
        Font_Number(
            x, (u16)(y + 8), camPos->mTripodPos.vx >> 12, r, g, b, 0);
        Font_Number(
            x, (u16)(y + 16), camPos->mTripodPos.vy >> 12, r, g, b, 0);
        Font_Number(
            x, (u16)(y + 24), camPos->mTripodPos.vz >> 12, r, g, b, 0);
    }
    Font_String(
        x, (u16)(y + 40), CameraQuaternionTextOwner, r, g, b, 0);
    Font_Number(
        x, (u16)(y + 48), camPos->mStickLength, r, g, b, 0);
    Font_Number(
        x, (u16)(y + 64), camPos->mStickOffset.vx >> 12, r, g, b, 0);
    Font_Number(
        x, (u16)(y + 72), camPos->mStickOffset.vy >> 12, r, g, b, 0);
    Font_Number(
        x, (u16)(y + 80), camPos->mStickOffset.vz >> 12, r, g, b, 0);
}


/**
 * DisplayLabels__6CamPosUsUsUcUcUc - Draw the static headings for the camera
 * position debug readout; CamPos::Display prints the matching numeric values.
 *
 * Address:   0x8007E3E8
 * Mangled:   DisplayLabels__6CamPosUsUsUcUcUc
 * Demangled: CamPos::DisplayLabels(unsigned short, unsigned short, unsigned char, unsigned char, unsigned char)
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 */
void CamPos::DisplayLabels(u16 x, u16 y, u8 r, u8 g, u8 b) {
    Font_String(x, y, CameraTripodTextOwner, r, g, b, 0);
    u16 x2 = (u16)(x + 0x14);
    Font_String(x2, (u16)(y + 8), CameraXTextOwner, r, g, b, 0);
    Font_String(x2, (u16)(y + 0x10), CameraYTextOwner, r, g, b, 0);
    Font_String(x2, (u16)(y + 0x18), CameraZTextOwner, r, g, b, 0);
    Font_String(x, (u16)(y + 0x28), CameraStickOrientationTextOwner, r, g, b, 0);
    Font_String(x, (u16)(y + 0x30), CameraStickLengthTextOwner, r, g, b, 0);
    Font_String(x, (u16)(y + 0x38), CameraStickOffsetTextOwner, r, g, b, 0);
    Font_String(x2, (u16)(y + 0x40), CameraXTextOwner, r, g, b, 0);
    Font_String(x2, (u16)(y + 0x48), CameraYTextOwner, r, g, b, 0);
    Font_String(x2, (u16)(y + 0x50), CameraZTextOwner, r, g, b, 0);
}


/**
 * _GLOBAL_.D.camDist [SKIP] — compiler-generated static init
 *
 *
 * ====== 0x8007E614 _GLOBAL_.I.camDist ==========================================
 * _GLOBAL_.I.camDist [SKIP] — compiler-generated static init
 *
 * Shared CBruce member function
 *
 * Address: 0x8007E5EC
 * Mangled: _GLOBAL_.D.camDist
 *
 */

int yangle __attribute__((section(".sdata"))) = 0;

static char CameraFollowTextOwner[]
    __attribute__((section(".sdata"), aligned(4))) = "Follow";
static char CameraFixedTextOwner[]
    __attribute__((section(".sdata"), aligned(4))) = "Fixed";
static char CameraQuaternionTextOwner[]
    __attribute__((section(".sdata"), aligned(4))) = "quaternion";
static char CameraTripodTextOwner[]
    __attribute__((section(".sdata"), aligned(4))) = "Tripod";
static char CameraXTextOwner[]
    __attribute__((section(".sdata"), aligned(4))) = "x";
static char CameraYTextOwner[]
    __attribute__((section(".sdata"), aligned(4))) = "y";
static char CameraZTextOwner[]
    __attribute__((section(".sdata"), aligned(4))) = "z";
static char CameraStickOrientationTextOwner[]
    __attribute__((section(".sdata"), aligned(4))) = "Stick O";
static char CameraStickLengthTextOwner[]
    __attribute__((section(".sdata"), aligned(4))) = "Stick Length";
static char CameraStickOffsetTextOwner[]
    __attribute__((section(".sdata"), aligned(4))) = "Stick Offset";

extern "C" void CameraAnimParamDestructor(void)
    asm("__DOT__t9AnimParam1Z5CQuat");
/** Raw address view retained solely for CameraCamPosVtableImage. */
extern "C" void CameraCamPosDestructor(void)
    asm("_._6CamPos");
extern "C" void CameraQuatDestructor(void)
    asm("__DOT__5CQuat_8007E81C");

/** Destructor-only dispatch tables for CAMERA's embedded value types. */
static unsigned int CameraAnimParamVtableImage[4]
    __attribute__((section(".sdata"), aligned(4))) = {
        0, 0, (unsigned int)&CameraAnimParamDestructor, 0,
    };
static unsigned int CameraCamPosVtableImage[4]
    __attribute__((section(".sdata"), aligned(4))) = {
        0, 0, (unsigned int)&CameraCamPosDestructor, 0,
    };
static unsigned int CameraQuatVtableImage[4]
    __attribute__((section(".sdata"), aligned(4))) = {
        0, 0, (unsigned int)&CameraQuatDestructor, 0,
    };
