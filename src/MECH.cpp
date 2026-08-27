// MECH.cpp -- Reconstructed translation unit
// Original: MECH.CPP
// Functions: 26 (all strict-exact)
// Address range: 0x800428F4 - 0x800440E8
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

// Owner-local declarations attributed to MECH.cpp by MAIN.SYM.

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

/** Return caller-provided storage for an in-place construction. */
inline void *operator new(unsigned int, void *address)
{
    return address;
}

struct CBody;
struct CBodyOwner;
struct CSuper;
struct EditorOutModel;
struct EditorInModel;
struct LevelGenerator;
struct CBruce;
struct CBaddy;
struct SModel;
struct SHook;

#define MECHLIST_OWNER_DECLARATIONS
#include "MECHLIST.h"
#undef MECHLIST_OWNER_DECLARATIONS
extern "C" int NumMechs;

#define BITSTATE_DECLARE_BIT_COUNT
#include "BITSTATE.h"
#include "CAMERALIST.h"
#include "PHYSICSSTATE.h"
#include "GLEVEL.h"
#include "GENERATELEVEL.h"
#define BITSTATE_DECLARE_SPARK_SEMI_TRANS
#include "BITSTATE.h"
#define BITSTATE_DECLARE_USE_CACHE_FOR_BITS
#include "BITSTATE.h"

#include "EXP.h"
#include "SFX.h"
#define MECH_OWNER_DECLARATIONS
#include "MECH.h"
#undef MECH_OWNER_DECLARATIONS
#include "MAIN.h"
#define STATS_UNSIZED_STATS_DECLARATION
#include "STATS.h"
#include "BADDY.h"
#include "FRONT.h"
#include "MEM.h"
#include "MIPS.h"
#include "M3DUTILS.h"
#include "M3DZONE.h"
#include "globals.h"
#include "CITEM.h"
// Owner-local CBruce slices for MECH.cpp.
// These stay local until the field names and offsets are proven across
// multiple owner TUs.

#define OB_LAYOUT_ONLY
#define OB_OWNER_LAYOUT
#include "OB.h"
#include "CBODY.h"
#include "CPOWERUP.h"
#include "POWERUP.h"
#undef OB_OWNER_LAYOUT
#undef OB_LAYOUT_ONLY
#include "PAD.h"
#include "FONTTOOLS.h"
#include "TEXTEFFECTS.h"
#include "PSXREGION.h"
#define M3DCOLIJ_LINE_INFO_LAYOUT
#include "M3DCOLIJ.h"
#undef M3DCOLIJ_LINE_INFO_LAYOUT

/** Face-packet prefix consumed by dynamic-lighting colour sampling. */
struct SFaceLightingMech {
    u16 Flags;
    u8 _padToColour[6];
    union {
        u32 RGB;
        u8 ColourIndices[4];
    } Colour;
};

/**
 * Panel-data slice exposing only the embedded font and text-effect
 * sub-objects destroyed by CBruce::~CBruce (layout from MAIN_STRUCTS.h;
 * the falling-text sub-objects use their shared target-proven layout).
 */
struct CPanelDataMech {
    u8 _padToTimerFont[356];
    FontStorage mTimerFont;                           // 0x164
    FontStorage mTrickFont;                           // 0x1b4
    FontStorage mScoreFont;                           // 0x204
    FallingTextEffect mFallingTextEffect;             // 0x254
    TrickTextEffect mTrickTextEffect;                 // 0x374 (TrickTextEffect)
    TextEffect *mpTextEffect;                         // 0x38c
    FallingTextEffect mOutOfBoundsText;               // 0x390
    u8 _opaqueTail[2276 - 1200];
};

/**
 * Three-component vector view exposing each component's low halfword.  The
 * skater's velocity-flip routine deliberately reads those halfwords before
 * writing the sign-extended values back to the full vector components.
 */
struct CVectorHalfwordMech {
    union { int vx; u16 vxLow; };
    union { int vy; u16 vyLow; };
    union { int vz; u16 vzLow; };
};

/**
 * Complete-size CBruce overlay exposing only fields owned by MECH.cpp.
 * Offsets are from original/MAIN_STRUCTS.h.
 */
struct CBruce {
    struct CSuperOwner CSuper;                        // 0x000
    struct CPanelDataMech mPanelData;                 // 0x190
    u8 _padToDust[3248 - 2676];
    u8 mDust[64];                                     // 0xcb0 (Dust)
    u8 mBlood[92];                                    // 0xcf0 (Blood)
    u8 _padToSkaterType[3496 - 3404];
    int mSkaterType;                                  // 0xda8
    u8 _padToRotationMatrix[3728 - 3500];
    MATRIX mRotationMatrix;                           // 0xe90
    u8 _padToSFXGrind[4164 - 3760];
    u32 mSFXGrind;                                    // 0x1044
    u32 mSFXGround;                                   // 0x1048
    u8 _padToPhysicsState[4204 - 4172];
    int mPhysicsState;                                // 0x106c
    u8 _padToFront[4256 - 4208];
    union {
        struct CVectorOwner mFront;
        struct CVectorHalfwordMech mFrontHalfwords;
    };                                                // 0x10a0
    union {
        struct CVectorOwner mRight;
        struct CVectorHalfwordMech mRightHalfwords;
    };                                                // 0x10ac
    struct CVectorOwner mUp;                          // 0x10b8
    u8 _padToGlow[4380 - 4292];
    u8 mGlowR;                                        // 0x111c
    u8 mGlowG;                                        // 0x111d
    u8 mGlowB;                                        // 0x111e
    u8 mTargetGlowR;                                  // 0x111f
    u8 mTargetGlowG;                                  // 0x1120
    u8 mTargetGlowB;                                  // 0x1121
    u8 mGlowSpeed;                                    // 0x1122
    u8 mGlowDownSpeed;                                // 0x1123
    int mBrightness;                                  // 0x1124
    u8 _opaqueTail[5228 - 4392];

    /** Refresh the orientation basis vectors from the skater matrix. */
    void FRUVectorsFromRotMatrix();
};

/** CBruce vtable prefix through the dynamic-lighting setup callback. */
struct CBruceVtableMech {
    void *reserved[6];
    void (*DoLightingSetup)(struct CBruce *);
};

/**
 * CSpark overlay: velocity inherited from CBit plus the spark's floor
 * plane.  sizeof(CSparkMech) == 0x5C is the allocation size passed to
 * CBit's operator new by BloodSplat.
 */
struct CSparkMech {
    u8 _padToVel[36];
    struct CVectorOwner mVel;                         // 0x24 (CBit.mVel)
    u8 _padToFloorY[88 - 48];
    int mFloorY;                                      // 0x58
};

/** One unlockable board and its five stat bonuses. */
struct SBoardMech {
    char *BoardTexture;
    char *BoardName;
    char TapesToUnlock;
    u8 _padToOllie[3];
    union { int ollie; u8 ollieByte; };
    union { int speed; u8 speedByte; };
    union { int air; u8 airByte; };
    union { int balance; u8 balanceByte; };
    union { int turning; u8 turningByte; };
};

/** Complete skater-stat record used by MECH's board, physics, and UI paths. */
struct SStatsMech {
    char *PSXName;
    char *PSXName2;
    char *FrontName;
    int ComboID;                                      // 0x00c
    u8 Board;                                         // 0x010
    u8 Wheel1;                                        // 0x011
    u8 Wheel2;                                        // 0x012
    u8 Head;
    char Neck;
    u8 _padToToughness[3];
    int Toughness;
    int Speed;
    int Air;
    int Balance;
    int Street;
    char *Name;
    u32 BoardChecksum;
    u32 WheelChecksum;
    u32 TruckChecksum;
    SBoardMech Boards[8];
    u8 Ability[12];
    char *PortraitName;
    char **BioText;
    int Available;
    int FrontEndOrientation;
};

#include "SCHARACTERPROGRESS.h"

/** SCareer prefix through the character-progress table used by MECH. */
struct SCareerMech {
    int Version;
    int MaxTapes;
    SCharacterProgress CharacterProgress[14];
};

/** Board-visibility slice exposing the skater type and inherited display mask. */
struct CBruceBoardSlice {
    struct CSuperOwner CSuper;
    u8 pad_to_skaterType[3096];
    int mSkaterType;
};

/** Controller-input slice exposing the pad and cached analogue axes. */
struct CBruceInputSlice {
    u8 pad_to_pad[3508];
    struct SControl *mPad;
    u8 pad_to_analogue[892];
    u8 mAnalogue;
    char mAX;
    char mAY;
};

/** Fixed-capacity checksum history owned by MECH.cpp. */
struct CBruceHistorySlice {
    u8 pad_to_historyPos[4424];
    int mHistoryPos;
    struct SHistory mHistory[100];
};

/** Dynamic glow state and inherited ellipsoid-lighting fields. */
struct CBruceGlowSlice {
    struct CSuperOwner CSuper;
    u8 pad_to_glow[3980];
    u8 mGlowR;
    u8 mGlowG;
    u8 mGlowB;
    u8 mTargetGlowR;
    u8 mTargetGlowG;
    u8 mTargetGlowB;
    u8 mGlowSpeed;
    u8 mGlowDownSpeed;

    /** Load the aligned four-byte glow state used by the settled test. */
    u32 PackedGlowWord() const {
        return *(const u32 *)&mGlowR;
    }

    /** Load the adjacent target green/blue pair used by the settled test. */
    u16 PackedTargetGlowGB() const {
        return *(const u16 *)&mTargetGlowG;
    }
};

/** Video recorder state needed to choose live, record, or playback input. */
struct SVideoInfo {
    int VideoMode;
    u8 _opaque[188];
};

/** Inherited skater fields read or updated by the per-frame MECH AI pass. */
struct CBruceAIBaseSlice {
    u8 _padToPosition[4];
    struct CVectorOwner Position;                    // 0x004
    u8 _padToRGB[0x20 - 0x10];
    u32 RGB;                                         // 0x020
    u8 _padToShadowX[0x90 - 0x24];
    int ShadowX;                                     // 0x090
    u8 _padToShadowZ[0x98 - 0x94];
    int ShadowZ;                                     // 0x098
    u8 _padToAnimFlags[0xB8 - 0x9C];
    u16 AnimFlags;                                   // 0x0b8
    s16 GroundAngle;                                 // 0x0ba
    u8 _padToOldPosition[0xC4 - 0xBC];
    struct CVectorOwner OldPosition;                 // 0x0c4
    u8 _padToEnd[0x190 - 0xD0];
};

/**
 * Per-frame AI view of CBruce, naming the small set of inherited fields used
 * by this owner while keeping the remainder opaque.
 */
struct CBruceAISlice {
    struct {
        struct CBruceAIBaseSlice Base;
    } Inherited;
    u8 _padToBoardStats[0xA78 - 0x190];
    struct BoardStats *mpBoardStats;                  // 0x0a78
    u8 _padToSkaterType[0xDA8 - 0xA7C];
    int mSkaterType;                                  // 0x0da8
    int mIndex;                                       // 0x0dac
    int mPlayTime;                                    // 0x0db0
    u8 _padToRecentEvent1[0xE68 - 0xDB4];
    int mRecentEvent1;                                // 0x0e68
    int mRecentEvent2;                                // 0x0e6c
    u8 _padToResponseTimer[0xEDC - 0xE70];
    int mResponseTimer;                               // 0x0edc
    u8 _padToMagicTime[0xF88 - 0xEE0];
    int mMagicTime;                                   // 0x0f88
    u8 _padToInputDelay[0x1014 - 0xF8C];
    u8 mInputDelay;                                   // 0x1014
    u8 _padToShadowY[0x1034 - 0x1015];
    int mShadowY;                                     // 0x1034
    u8 _padToPhysicsState[0x106C - 0x1038];
    int mPhysicsState;                                // 0x106c
    u8 _padToGroundCounter[0x1144 - 0x1070];
    u16 mGroundCounter;                               // 0x1144
    u8 _opaqueTail[5228 - 0x1146];
};

/** Constructor-local form of the inline three-component game vector. */
struct CVectorMechCtor {
    int vx;
    int vy;
    int vz;

    /** Assign all three game-space components. */
    void Set(int x, int y, int z) {
        vx = x;
        vy = y;
        vz = z;
    }
};

#include "SLIGHT.h"

/**
 * Constructor-owned CBruce layout.  Only fields initialized by MECH.cpp are
 * named; the untouched portions stay opaque until their owning methods prove
 * them independently.
 */
struct CBruceCtorSlice {
    struct {
        struct CSuperOwner CSuper;
    } Inherited;                                      // 0x000
    struct CPanelDataMech mPanelData;                 // 0x190
    void *mpCamera;                                   // 0xa74
    struct BoardStats *mpBoardStats;                  // 0xa78
    u8 _padToTrickGravity[2724 - 2684];
    s16 mTrickGravity;                                // 0xaa4
    u8 _padToDust[3248 - 2726];
    u8 mDust[64];                                     // 0xcb0
    u8 mBlood[92];                                    // 0xcf0
    s16 mFrozen;                                      // 0xd4c
    u8 _padToDoingATrick[3408 - 3406];
    int mDoingATrick;                                 // 0xd50
    int mCanStartNextTrick;                           // 0xd54
    u8 _padToSkaterType[3496 - 3416];
    int mSkaterType;                                  // 0xda8
    int mIndex;                                       // 0xdac
    int mPlayTime;                                    // 0xdb0
    struct SControl *mPad;                            // 0xdb4
    u8 _padToIsOnTaggedPoly[3568 - 3512];
    int mIsOnTaggedPoly;                              // 0xdf0
    int mStickToInvisible;                            // 0xdf4
    struct CVectorOwner mLastVel;                     // 0xdf8
    int mMaxKickSpeed;                                // 0xe04
    int mSpeedBoost;                                  // 0xe08
    int mCess;                                        // 0xe0c
    int mManual;                                      // 0xe10
    int mManIsCrouched;                               // 0xe14
    int mManIsJumping;                                // 0xe18
    int mJumpPending;                                 // 0xe1c
    int mAirTime;                                     // 0xe20
    int mJumpStrength;                                // 0xe24
    int mLastJumpStrength;                            // 0xe28
    int mApplyJumpForce;                              // 0xe2c
    u8 _padToOldRotationMatrix[3696 - 3632];
    MATRIX mOldRotationMatrix;                        // 0xe70
    MATRIX mRotationMatrix;                           // 0xe90
    int mDoingSharpTurn;                              // 0xeb0
    int mTurning;                                     // 0xeb4
    int mTapTurn;                                     // 0xeb8
    int mPitchAngle;                                  // 0xebc
    int mNumGrinds;                                   // 0xec0
    int mRailType;                                    // 0xec4
    int mOnRail;                                      // 0xec8
    int mStickToRail;                                 // 0xecc
    u8 _padToDontStickTimer[3832 - 3792];
    int mDontStickTimer;                              // 0xef8
    int mSameRailTimer;                               // 0xefc
    int mDontStickRail;                               // 0xf00
    u8 _padToKickLockout[3936 - 3844];
    int mKickLockout;                                 // 0xf60
    int mJumpSpeed;                                   // 0xf64
    int mJumpFrame;                                   // 0xf68
    int mWallrideTime;                                // 0xf6c
    int mHandPlantTweaked;                            // 0xf70
    int mBailState;                                   // 0xf74
    int mLimitBailVel;                                // 0xf78
    int mInBail;                                      // 0xf7c
    int mHeadHit;                                     // 0xf80
    int mMagicPoints;                                 // 0xf84
    int mMagicTime;                                   // 0xf88
    u8 _padToComboCounter[4136 - 3980];
    int mComboCounter;                                // 0x1028
    u8 _padToSFXGrind[4164 - 4140];
    u32 mSFXGrind;                                    // 0x1044
    u32 mSFXGround;                                   // 0x1048
    u32 mSFXAir;                                      // 0x104c
    u32 mSFXPitch;                                    // 0x1050
    u32 mSFXVol;                                      // 0x1054
    int mTruckSqueak;                                 // 0x1058
    int mFootSound;                                   // 0x105c
    int mFootSFX;                                     // 0x1060
    int mTerrain;                                     // 0x1064
    int mTerrainSFX;                                  // 0x1068
    int mPhysicsState;                                // 0x106c
    int mOldPhysicsState;                             // 0x1070
    MATRIX mTempRotationMatrix;                       // 0x1074
    struct CVectorOwner mTempPos;                     // 0x1094
    struct CVectorOwner mFront;                       // 0x10a0
    struct CVectorOwner mRight;                       // 0x10ac
    struct CVectorOwner mUp;                          // 0x10b8
    struct CVectorMechCtor mLastFallLine;             // 0x10c4
    int mFallLineAdjustment;                          // 0x10d0
    SVECTOR mLastPolyNormal;                          // 0x10d4
    int mSameNormal;                                  // 0x10dc
    VECTOR mOldNormal;                                // 0x10e0
    int mLeanAngle;                                   // 0x10f0
    int mAnimLeanAngle;                               // 0x10f4
    u8 _opaqueTail[5228 - 4344];

    /** Clear the skater's recent-event history. */
    void History_Reset() asm("History_Reset__6CBruce");

    /** Refresh the orientation basis vectors from the skater matrix. */
    void FRUVectorsFromRotMatrix() asm("FRUVectorsFromRotMatrix__6CBruce");

};

/** Camera field selected for the player camera during skater construction. */
struct CCamera {
    u8 _padToCameraMode[1368];
    int mCameraMode;
    u8 _opaqueTail[1668 - 1372];

    /** Select the active gameplay camera mode. */
    void SetMode(int mode) {
        mCameraMode = mode;
    }

    /** Return whether the competition-intro camera mode is active. */
    int InCompetitionIntro();
};

typedef CCamera CCameraMechCtor;

typedef char SFaceLightingMech_size_check[
    (sizeof(struct SFaceLightingMech) == 12) ? 1 : -1];
typedef char CPanelDataMech_size_check[(sizeof(struct CPanelDataMech) == 2276) ? 1 : -1];
typedef char CVectorHalfwordMech_size_check[
    (sizeof(struct CVectorHalfwordMech) == 12) ? 1 : -1];
typedef char CCameraMech_size_check[
    (sizeof(CCamera) == 1668) ? 1 : -1];
typedef char CBruce_mech_size_check[(sizeof(struct CBruce) == 5228) ? 1 : -1];
typedef char CBruceVtableMech_size_check[
    (sizeof(struct CBruceVtableMech) == 28) ? 1 : -1];
typedef char CSparkMech_size_check[(sizeof(struct CSparkMech) == 0x5C) ? 1 : -1];
typedef char SBoardMech_size_check[(sizeof(struct SBoardMech) == 32) ? 1 : -1];
typedef char SStatsMech_size_check[(sizeof(struct SStatsMech) == 344) ? 1 : -1];
typedef char Mech_SCharacterProgress_size_check[
    (sizeof(struct SCharacterProgress) == 32) ? 1 : -1];
typedef char CBruceBoardSlice_size_check[
    (sizeof(struct CBruceBoardSlice) == 3500) ? 1 : -1];
typedef char CBruceInputSlice_size_check[
    (sizeof(struct CBruceInputSlice) == 4408) ? 1 : -1];
typedef char CBruceHistorySlice_size_check[
    (sizeof(struct CBruceHistorySlice) == 5228) ? 1 : -1];
typedef char CBruceHistorySlice_history_pos_offset_check[
    ((unsigned long)&(((CBruceHistorySlice *)0)->mHistoryPos) == 0x1148)
        ? 1 : -1];
typedef char CBruceHistorySlice_history_offset_check[
    ((unsigned long)&(((CBruceHistorySlice *)0)->mHistory) == 0x114C)
        ? 1 : -1];
typedef char CBruceGlowSlice_size_check[
    (sizeof(struct CBruceGlowSlice) == 4388) ? 1 : -1];
typedef char SVideoInfo_size_check[
    (sizeof(struct SVideoInfo) == 192) ? 1 : -1];
typedef char CBruceAIBaseSlice_size_check[
    (sizeof(struct CBruceAIBaseSlice) == 400) ? 1 : -1];
typedef char CBruceAISlice_size_check[
    (sizeof(struct CBruceAISlice) == 5228) ? 1 : -1];
typedef char SDK_MATRIX_mech_size_check[
    (sizeof(MATRIX) == 32) ? 1 : -1];
typedef char CBruce_rotation_matrix_offset_check[
    ((unsigned long)&(((CBruce *)0)->mRotationMatrix) == 0xE90) ? 1 : -1];
typedef char CBruceCtor_old_rotation_matrix_offset_check[
    ((unsigned long)&(((CBruceCtorSlice *)0)->mOldRotationMatrix) == 0xE70)
        ? 1 : -1];
typedef char CBruceCtor_rotation_matrix_offset_check[
    ((unsigned long)&(((CBruceCtorSlice *)0)->mRotationMatrix) == 0xE90)
        ? 1 : -1];
typedef char CBruceCtor_temp_rotation_matrix_offset_check[
    ((unsigned long)&(((CBruceCtorSlice *)0)->mTempRotationMatrix) == 0x1074)
        ? 1 : -1];
typedef char SDK_VECTOR_mech_size_check[
    (sizeof(VECTOR) == 16) ? 1 : -1];
typedef char CVectorMechCtor_size_check[
    (sizeof(struct CVectorMechCtor) == 12) ? 1 : -1];
typedef char SDK_SVECTOR_mech_size_check[
    (sizeof(SVECTOR) == 8) ? 1 : -1];
typedef char SDK_CVECTOR_mech_size_check[
    (sizeof(CVECTOR) == 4) ? 1 : -1];
typedef char CBruceCtorSlice_size_check[
    (sizeof(struct CBruceCtorSlice) == 5228) ? 1 : -1];
typedef char Mech_CFriction_size_check[
    (sizeof(struct CFriction) == 3) ? 1 : -1];
typedef char Mech_SLight_size_check[
    (sizeof(SLight) == 52) ? 1 : -1];

#include "OB.h"
#include "FONT.h"
#include "UTILS.h"
#define BIT_DUST_MANAGER_LAYOUT
#define BIT_BLOOD_MANAGER_LAYOUT
#include "BIT.h"
#undef BIT_BLOOD_MANAGER_LAYOUT
#undef BIT_DUST_MANAGER_LAYOUT
#include "CFLATRIPPLE.h"
typedef char CFlatRipple_size_check[
    (sizeof(CFlatRipple) == 0x74) ? 1 : -1];

// === External function declarations ===
extern "C" void CItem_InitItem(CItemOwner *, char *) asm("InitItem__5CItemPc");
#include "VIDEO.h"
extern "C" CSparkMech *CSpark_ctor(
    CSparkMech *, CVector &, int, int, int)
    asm("__6CSparkR7CVectoriii");
extern "C" void CSuper_dtor(CSuperOwner *, int) asm("_._6CSuper");
extern "C" int __vt_6CBruce[];
extern "C" CSuperOwner *CSuper_ctor(CSuperOwner *) asm("__6CSuper");
extern "C" CPanelDataMech *CPanelData_ctor(
    CPanelDataMech *, CBruceCtorSlice *) asm("__10CPanelDataP6CBruce");
extern "C" void CBruce_HistoryReset(CBruceHistorySlice *)
    asm("History_Reset__6CBruce");
extern "C" void CBruce_DoPhysics(CBruce *) asm("DoPhysics__6CBruce");
#include <STDIO.H>

#include "GSKATER_STATE.h"
#define VIDEOINFO_DECLARE_GVINFO1
#define VIDEOINFO_DECLARE_GVINFO2
#include "VIDEOINFO.h"

/**
 * Compact diagnostic format retained in the original small-data section.
 */
static char Mech_StatsFormat[12]
    __attribute__((section(".sdata"), aligned(4))) = "%d %d %d %d";

/** Active skater used by script and collision systems. */
CBruce *pCurrentPlayer __attribute__((section(".sbss")));

/** Default skater light used by ordinary levels. */
SLight Skater_DefaultLight __attribute__((section(".data"))) = {
    {
        {0, 0x1000, 0},
        {0, -0x1000, 0},
        {-3857, 0, 1380},
    },
    0,
    {
        {640, 2944, 320},
        {640, 2944, 320},
        {640, 2944, 320},
    },
    0,
    {2176, 2176, 2176},
};

/** Warmer skater light selected by the generated Mars level. */
SLight Skater_MarsLight __attribute__((section(".data"))) = {
    {
        {0, 0x1000, 0},
        {0, -0x1000, 0},
        {-3857, 0, 1380},
    },
    0,
    {
        {192, 786, 3072},
        {192, 705, 2628},
        {192, 1088, 1992},
    },
    0,
    {1664, 1664, 1664},
};

/** Per-skater hook indices loaded after the skater model is initialized. */
s16 SkaterHooks[14][10] __attribute__((section(".data"))) = {
    {0, 1, 0, 0, 0, 18, 0, 0, 0, 3},
    {0, 1, 0, 0, 0, 12, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 12, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 12, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 12, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 12, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 12, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 12, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 12, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 12, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 12, 0, 0, 0, 0},
    {},
    {},
    {},
};

#define HEADBLOODATFRAME_OWNER_DECLARATION
#include "HEADBLOODATFRAME.h"

/** Animation frame that emits head blood for each skater model. */
s16 HeadBloodAtFrame[14] __attribute__((section(".data"))) = {
    19, -1, 17, 17, 17, 17, 17, 17, 17, 17, 17, 0, 0, 0,
};

/** Board-stat bonuses filled for the active skaters at run setup. */
BoardStats GBoardStats[2] __attribute__((section(".sdata"))) = {};

/** Branch-free signed magnitude used by the original fixed-point helpers. */
static inline int MechAbs(int value)
{
    int sign = value >> 31;
    return (value ^ sign) - sign;
}

// === Extern "C" forward declarations ===
extern "C" void CBruce_BoardOff(CBruce *) asm("BoardOff__6CBruce");
extern "C" void CBruce_BoardOn(CBruce *) asm("BoardOn__6CBruce");
extern "C" void CBruce_GetStats(CBruce *, int) asm("GetStats__6CBrucei");
extern "C" CBruceCtorSlice *CBruce_ctor(CBruceCtorSlice *) asm("__6CBruce");
extern "C" int CBruce_HistoryFind(CBruceHistorySlice *, char *, int)
    asm("History_Find__6CBrucePci");
extern "C" int CBruce_HistoryAdd(CBruceHistorySlice *, char *, int)
    asm("History_Add__6CBrucePci");
extern "C" void CBruce_dtor(CBruce *, int) asm("_._6CBruce");
extern "C" void CBruce_GlowUpdate(CBruceGlowSlice *)
    asm("GlowUpdate__6CBruce");
extern "C" void CBruce_DoLightingSetup(CBruce *)
    asm("DoLightingSetup__6CBruce");
extern "C" void CBruce_CalculateDynamicLighting(
    CBruce *, unsigned int *, Item *)
    asm("CalculateDynamicLighting__6CBrucePUiP4Item");
extern "C" void CBruce_ReadAnalogueInput(CBruceInputSlice *)
    asm("ReadAnalogueInput__6CBruce");
extern "C" void CBruce_GetHookPosition(CBruce *, CVector &, u8)
    asm("GetHookPosition__6CBruceR7CVectorUc");
extern "C" void CBruce_Die(CBruce *) asm("Die__6CBruce");
extern "C" void CBruce_DeleteStuff(CBruce *) asm("DeleteStuff__6CBruce");
extern "C" int CBruce_Hit(CBruce *, int, const CVector &, int)
    asm("Hit__6CBruceiRC7CVectori");
extern "C" void CBruce_CollideWithObject(CBruce *, CBody *)
    asm("CollideWithObject__6CBruceP5CBody");
extern "C" void CBruce_DoShadowCheck(CBruce *) asm("DoShadowCheck__6CBruce");
extern "C" void CBruce_InitItem(CBruce *, char *) asm("InitItem__6CBrucePc");
extern "C" int CBruce_FlipToVelocity(CBruce *)
    asm("FlipToVelocity__6CBruce");
extern "C" void CBruce_BloodSplat(CBruce *, int, int)
    asm("BloodSplat__6CBruceii");
extern "C" void CBruce_AI(CBruceAISlice *) asm("AI__6CBruce");
extern "C" void CBruce_HandleWater(CBruce *) asm("HandleWater__6CBruce");
#include "RCOSSIN.h"

/**
 * Skater virtual dispatch table in its original read-only owner section.
 */
int __vt_6CBruce[9]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&CBruce_dtor,
        (int)&CBruce_Die,
        (int)&CBruce_AI,
        (int)&CBody_EveryFrame,
        (int)&CBruce_DoLightingSetup,
        (int)&CBruce_Hit,
        (int)&CBruce_DeleteStuff,
};


// === Function implementations (address order) ===

/**
 * BoardOff__6CBruce
 *
 * Address:   0x800428F4
 * Mangled:   BoardOff__6CBruce
 * Demangled: CBruce::BoardOff(void)
 *
 * @param bruce  Skater instance whose board should be hidden.
 */
void CBruce_BoardOff(CBruce *bruce)
{
    register SStats *stats asm("$5") = Stats;
    int idx = bruce->mSkaterType;
    stats += idx;
    int bit = 1;
    int b0 = stats->Board;
    int b1 = stats->Wheel1;
    int b2 = stats->Wheel2;
    int flags = bruce->CSuper.mSubObjectDisplayMask;
    int m0 = bit << b0;
    int m1 = bit << b1;
    m0 = m0 | m1;
    int m2 = bit << b2;
    m0 = m0 | m2;
    bruce->CSuper.mSubObjectDisplayMask = flags | m0;
}

/**
 * BoardOn__6CBruce
 *
 * Address:   0x80042954
 * Mangled:   BoardOn__6CBruce
 * Demangled: CBruce::BoardOn(void)
 *
 * @param bruce  Skater instance whose board should be shown.
 */
void CBruce_BoardOn(CBruce *bruce)
{
    int idx = bruce->mSkaterType;
    register SStats *stats asm("$5") = Stats;
    stats += idx;
    int bit = 1;
    int b0 = stats->Board;
    int b1 = stats->Wheel1;
    int b2 = stats->Wheel2;
    int flags = bruce->CSuper.mSubObjectDisplayMask;
    b0 = bit << b0;
    b1 = bit << b1;
    b0 = b0 | b1;
    int m2 = bit << b2;
    m2 = ~(b0 | m2);
    bruce->CSuper.mSubObjectDisplayMask = flags & m2;
}

/**
 * GetStats__6CBrucei - Fill one 4-byte stat display slot for the current skater.
 * In single-player career it chooses the highest unlocked stat tier for the
 * skater from GCareer progress, then writes the four displayed stat bytes into
 * the per-slot output buffer at 0x800D9B20 + slot * 5. In multiplayer it clears
 * that output slot instead.
 *
 * Address:   0x800429B4
 * Mangled:   GetStats__6CBrucei
 * Demangled: CBruce::GetStats(int)
 *
 * @param bruce  Skater instance whose stat display is being filled.
 * @param Skater  Output-panel slot to update.
 */
void CBruce_GetStats(CBruce *bruce, int Skater)
{
    CBruce *selfReg = bruce;
    register char *gp asm("$28");

    // This target load embeds GNumberOfPlayers' address without a relocation.
    if (*(int *)0x800D95E0 != 1)
        goto clear_stats;

    {
        register int highest_board asm("$7") = 0;
        register SStatsMech *stats asm("$9");
        {
            // The target object carries these absolute addresses as split
            // constants rather than external-symbol relocations.
            register int base asm("$3") = 0x800B0000;
            asm volatile("" : "+r"(base));
            stats = reinterpret_cast<SStatsMech *>(base + 0x5494);
        }
        register int careerBase asm("$3") = 0x800B0000;
        int skater = selfReg->mSkaterType;
        asm volatile("" : "+r"(careerBase));
        careerBase += 0x7834;
        register int calc asm("$2") = skater << 1;
        calc += skater;
        calc <<= 2;
        calc -= skater;
        calc <<= 2;
        calc -= skater;
        int statsOffset = calc << 3;
        asm volatile("" : "+r"(statsOffset));
        skater <<= 5;
        skater += careerBase;
        unsigned int progress =
            reinterpret_cast<SCareerMech *>(skater)
                ->CharacterProgress[0].NumTapes;
        asm volatile("" : "+r"(highest_board));
        int next = highest_board + 1;

    tier_loop:
        calc = next << 5;
        calc += statsOffset;
        calc += (int)stats;
        unsigned int threshold =
            ((SStatsMech *)calc)->Boards[0].TapesToUnlock;
        if (progress < threshold)
            goto tier_done;
        highest_board = next;
        if (next < 7)
        {
            next = highest_board + 1;
            goto tier_loop;
        }

    tier_done:
        int slotOffset = Skater << 2;
        slotOffset += Skater;
        register char *dst0 asm("$4") = gp + 0x1E80;
        asm volatile("" : "+r"(dst0));
        highest_board <<= 5;
        calc = highest_board + statsOffset;
        calc = (int)stats + calc;
        int value0 = ((SStatsMech *)calc)->Boards[0].ollieByte;
        dst0 = (char *)(slotOffset + (int)dst0);
        *(u8 *)dst0 = value0;

        register int skater2 asm("$3") = selfReg->mSkaterType;
        register char *dst1 asm("$6") = gp + 0x1E81;
        calc = skater2 << 1;
        calc += skater2;
        calc <<= 2;
        calc -= skater2;
        calc <<= 2;
        calc -= skater2;
        calc <<= 3;
        calc = highest_board + calc;
        calc = (int)stats + calc;
        int value1 = ((SStatsMech *)calc)->Boards[0].speedByte;
        dst1 = (char *)(slotOffset + (int)dst1);
        *(volatile u8 *)dst1 = value1;

        skater2 = selfReg->mSkaterType;
        register char *dst2 asm("$5") = gp + 0x1E82;
        calc = skater2 << 1;
        calc += skater2;
        calc <<= 2;
        calc -= skater2;
        calc <<= 2;
        calc -= skater2;
        calc <<= 3;
        calc = highest_board + calc;
        calc = (int)stats + calc;
        int value2 = ((SStatsMech *)calc)->Boards[0].airByte;
        dst2 = (char *)(slotOffset + (int)dst2);
        *(volatile u8 *)dst2 = value2;

        skater2 = selfReg->mSkaterType;
        register int dst3 asm("$6") = (int)(gp + 0x1E83);
        calc = skater2 << 1;
        calc += skater2;
        calc <<= 2;
        calc -= skater2;
        calc <<= 2;
        calc -= skater2;
        calc <<= 3;
        highest_board += calc;
        highest_board = (int)stats + highest_board;
        skater2 =
            ((SStatsMech *)highest_board)->Boards[0].balanceByte;
        asm volatile("" : "+r"(skater2));
        slotOffset = slotOffset + dst3;
        *(u8 *)slotOffset = skater2;
        return;
    }

clear_stats:
    {
        int slotOffset = Skater << 2;
        slotOffset += Skater;
        register char *dst0 asm("$2") = gp + 0x1E80;
        dst0 = (char *)(slotOffset + (int)dst0);
        asm volatile("" : "+r"(dst0));
        register char *dst1 asm("$3") = gp + 0x1E81;
        dst1 = (char *)(slotOffset + (int)dst1);
        asm volatile("" : "+r"(dst1));
        *(u8 *)dst0 = 0;
        register char *dst2 asm("$2") = gp + 0x1E82;
        dst2 = (char *)(slotOffset + (int)dst2);
        asm volatile("" : "+r"(dst2));
        *(u8 *)dst1 = 0;
        register char *dst3 asm("$3") = gp + 0x1E83;
        slotOffset = slotOffset + (int)dst3;
        *(u8 *)dst2 = 0;
        *(u8 *)slotOffset = 0;
    }
}

/**
 * Construct a skater and establish its gameplay, orientation, shadow, and
 * controller defaults before attaching it to the live mech list.
 *
 * Address:   0x80042B44
 * Mangled:   __6CBruce
 * Demangled: CBruce::CBruce(void)
 *
 * @param bruce Storage supplied for the new skater.
 * @return `bruce` (the GCC 2.x constructor return convention).
 */
CBruceCtorSlice *CBruce_ctor(CBruceCtorSlice *bruce)
{
    int maxKickSpeed;

    CSuper_ctor(&bruce->Inherited.CSuper);
    bruce->Inherited.CSuper.CBody.CItem.vtable = (void *)__vt_6CBruce;
    CPanelData_ctor(&bruce->mPanelData, bruce);
    ::new ((Dust *)bruce->mDust) Dust;
    ::new ((Blood *)bruce->mBlood) Blood;

    bruce->mTrickGravity = 100;
    bruce->Inherited.CSuper.mAnimSpeed = 0x10000;
    bruce->mpBoardStats = (BoardStats *)-1;

    if (Levels[GLevel].isCompetition && GGame == 1)
        bruce->mPlayTime = 3630;
    else if (GGame == 7)
        bruce->mPlayTime = 630;
    else
        bruce->mPlayTime = 7230;

    bruce->Inherited.CSuper.CBody.CItem.mFlags |= 0x200;
    int statsOverride = Cheat_StatsOverride;
    bruce->Inherited.CSuper.CBody.CItem.mScale.Set(0x1000, 0x1000, 0x1000);

    if (!statsOverride)
    {
        int statsSpeed = Stats[bruce->mSkaterType].Speed;
        int boardSpeed = bruce->mpBoardStats->speed;
        if (bruce->mMagicTime || Cheat_Fire)
        {
            maxKickSpeed =
                ((boardSpeed + statsSpeed + 20) << 12) * 45 / 28;
        }
        else
        {
            maxKickSpeed =
                ((statsSpeed + boardSpeed + 17) << 12) * 45 / 28;
        }
    }
    else
    {
        maxKickSpeed =
            ((statsOverride + 17) << 12) * 45 / 28;
    }
    bruce->mMaxKickSpeed = maxKickSpeed;

    bruce->mOnRail = 0;
    bruce->mStickToRail = 0;
    bruce->mSameRailTimer = 0;
    bruce->mDontStickTimer = 0;
    bruce->mDontStickRail = -1;
    bruce->mSFXGrind = 0;
    bruce->mSFXGround = 0;
    bruce->mTruckSqueak = 0;
    bruce->mPhysicsState = 0;
    bruce->mOldPhysicsState = 0;
    bruce->mManIsCrouched = 0;
    bruce->mManIsJumping = 0;
    bruce->mJumpPending = 0;
    bruce->mDoingATrick = 0;
    bruce->mApplyJumpForce = 0;
    bruce->mComboCounter = 0;
    bruce->Inherited.CSuper.mExtraFlags |= 1;

    bruce->History_Reset();

    bruce->mStickToInvisible = 0;
    bruce->mIsOnTaggedPoly = 0;
    bruce->mLastFallLine.Set(0, 0, 0);
    bruce->mFallLineAdjustment = 0;
    bruce->mLastPolyNormal.vx = 0;
    bruce->mLastPolyNormal.vy = -0x1000;
    bruce->mLastPolyNormal.vz = 0;
    bruce->mPitchAngle = 0;
    bruce->mLeanAngle = 0;
    bruce->mAnimLeanAngle = 0;
    bruce->mKickLockout = 0;

    bruce->mRotationMatrix.m[0][0] = -0x1000;
    bruce->mRotationMatrix.m[1][0] = 0;
    bruce->mRotationMatrix.m[2][0] = 0;
    bruce->mRotationMatrix.m[0][1] = 0;
    bruce->mRotationMatrix.m[1][1] = -0x1000;
    bruce->mRotationMatrix.m[2][1] = 0;
    bruce->mRotationMatrix.m[0][2] = 0;
    bruce->mRotationMatrix.m[1][2] = 0;
    bruce->mRotationMatrix.m[2][2] = -0x1000;

    bruce->FRUVectorsFromRotMatrix();
    bruce->mOldRotationMatrix = bruce->mRotationMatrix;

    {
        int normX = bruce->mRotationMatrix.m[0][1];
        int normY = bruce->mRotationMatrix.m[1][1];
        int normZ = bruce->mRotationMatrix.m[2][1];

        bruce->mSameNormal = 0;
        bruce->mOldNormal.vx = normX;
        bruce->mOldNormal.vy = normY;
        bruce->mOldNormal.vz = normZ;
        bruce->Inherited.CSuper.CBody.mNormal.x = normX;
        bruce->Inherited.CSuper.CBody.mNormal.y = normY;
        bruce->Inherited.CSuper.CBody.mNormal.z = normZ;
    }

    reinterpret_cast<CBody *>(&bruce->Inherited.CSuper.CBody)->AttachTo(
        reinterpret_cast<CBody **>(&MechList));
    u16 bodyFlags =
        bruce->Inherited.CSuper.CBody.mCBodyFlags & (u16)~2;
    int numMechs = ++NumMechs;
    bruce->Inherited.CSuper.CBody.mCBodyFlags = bodyFlags;
    if (numMechs == 1)
        bruce->mPad = &Pad[0];
    else
        bruce->mPad = &Pad[1];
    bruce->Inherited.CSuper.CBody.mAngFric.Set(5, 1, 5);

    if (GNumberOfPlayers == 1)
    {
        bruce->Inherited.CSuper.mShadowLights = 1;
        bruce->Inherited.CSuper.mpShadowSource =
            static_cast<SVECTOR *>(Mem_New(32, 0, 1));
        bruce->Inherited.CSuper.mpShadowRGB =
            static_cast<CVECTOR *>(Mem_New(16, 0, 1));

        bruce->Inherited.CSuper.mpShadowSource->vx = 0;
        bruce->Inherited.CSuper.mpShadowSource->vy = -0x1000;
        bruce->Inherited.CSuper.mpShadowSource->vz = 0;
        bruce->Inherited.CSuper.mpShadowRGB->r = 0x80;
        bruce->Inherited.CSuper.mpShadowRGB->g = 0x80;
        bruce->Inherited.CSuper.mpShadowRGB->b = 0x80;
    }
    else
    {
        reinterpret_cast<CBody *>(&bruce->Inherited.CSuper.CBody)->ShadowOn();
        reinterpret_cast<CBody *>(
            &bruce->Inherited.CSuper.CBody)->UpdateShadow();
    }

    SLight *defaultLight = &Skater_DefaultLight;
    CCameraMechCtor *camera =
        reinterpret_cast<CCameraMechCtor *>(CameraList);
    void *generateLevel = GenerateLevel;
    camera->SetMode(1);
    bruce->Inherited.CSuper.CBody.CItem.mpLight = defaultLight;
    if (!generateLevel)
        bruce->Inherited.CSuper.CBody.CItem.mpLight =
            &Skater_MarsLight;

    return bruce;
}

/**
 * _._6CBruce
 *
 * Address:   0x80042F44
 * Mangled:   _._6CBruce
 * Demangled: CBruce::~CBruce()
 *
 * @param bruce  Skater instance being destroyed.
 * @param __in_chrg  GCC 2.x in-charge flag (bit 0 also deletes `bruce`).
 */
void CBruce_dtor(CBruce *bruce, int __in_chrg)
{
    bruce->CSuper.CBody.CItem._vf = (void *)__vt_6CBruce;

    if (bruce->mSFXGrind != 0)
    {
        SFX_Stop(bruce->mSFXGrind);
        bruce->mSFXGrind = 0;
    }

    if (bruce->mSFXGround != 0)
    {
        SFX_Stop(bruce->mSFXGround);
        bruce->mSFXGround = 0;
    }

    if (bruce->CSuper.mpShadowSource != 0)
    {
        Mem_Delete(bruce->CSuper.mpShadowSource);
    }

    if (bruce->CSuper.mpShadowRGB != 0)
    {
        Mem_Delete(bruce->CSuper.mpShadowRGB);
    }

    reinterpret_cast<CBody *>(&bruce->CSuper.CBody)->DeleteFrom(
        reinterpret_cast<CBody **>(&MechList));
    NumMechs = NumMechs - 1;
    CBruce_DeleteStuff(bruce);

    ((Blood *)bruce->mBlood)->~Blood();
    ((Dust *)bruce->mDust)->~Dust();
    bruce->mPanelData.mOutOfBoundsText.~FallingTextEffect();
    TrickTextEffect_DestroyAbi(&bruce->mPanelData.mTrickTextEffect, 2);
    bruce->mPanelData.mFallingTextEffect.~FallingTextEffect();
    FontFromStorage(&bruce->mPanelData.mScoreFont)->~Font();
    FontFromStorage(&bruce->mPanelData.mTrickFont)->~Font();
    FontFromStorage(&bruce->mPanelData.mTimerFont)->~Font();

    CSuper_dtor(&bruce->CSuper, 0);

    if (__in_chrg & 1)
    {
        ::CItem::operator delete(bruce);
    }
}

/**
 * History_Checksum(char *str, int seed) — rotate-left-1 + xor string hash
 *
 * Address: 0x80043080
 * Mangled: History_Checksum__FPci
 *
 * @param pName  Asset / object name string.
 * @param Spins  Initial low-bit seed mixed into the checksum.
 * @return hash
 */
int History_Checksum(char *pName, int Spins)
{
    int c = Spins & 1;
    if (*pName != '\0')
    {
        do
        {
            int top_bit = ((u32)c & 0x80000000) >> 31;
            c = (c << 1) + top_bit;
            c ^= *(u8 *)pName++;
        } while (*pName != '\0');
    }
    return c;
}

/**
 * History_Find__6CBrucePci
 *
 * Address:   0x800430C8
 * Mangled:   History_Find__6CBrucePci
 * Demangled: CBruce::History_Find(char *, int)
 *
 * @param bruce  Skater whose recent-event history is searched.
 * @param pName  Asset / object name string.
 * @param Spins  Initial low-bit seed mixed into the checksum.
 * @return Matching history index, or -1 when absent.
 */
int CBruce_HistoryFind(
    CBruceHistorySlice *bruce, char *pName, int Spins)
{
    int c = History_Checksum(pName, Spins);
    int i = 0;
    register int count asm("$4") = bruce->mHistoryPos;
    if (count > 0)
    {
        int limit = count;
        SHistory *entry = bruce->mHistory;
        do
        {
            if (entry->Checksum == c)
            {
                return i;
            }
            i++;
            entry++;
        } while (i < limit);
    }
    return -1;
}

/**
 * CBruce::History_Reset(void) — clear history count
 *
 * Address: 0x80043134
 *          Target: jr $ra; sw $zero, 0x1148($a0)
 * Mangled: History_Reset__6CBruce
 *
 * @param bruce  Skater whose recent-event history is cleared.
 */
void CBruce_HistoryReset(CBruceHistorySlice *bruce)
{
    bruce->mHistoryPos = 0;
}

/**
 * History_Add__6CBrucePci
 *
 * Address:   0x8004313C
 * Mangled:   History_Add__6CBrucePci
 * Demangled: CBruce::History_Add(char *, int)
 *
 * @param bruce  Skater whose recent-event history is updated.
 * @param pName  Asset / object name string.
 * @param Spins  Initial low-bit seed mixed into the checksum.
 * @return Updated occurrence count for the history entry.
 */
int CBruce_HistoryAdd(
    CBruceHistorySlice *bruce, char *pName, int Spins)
{
    int i = CBruce_HistoryFind(bruce, pName, Spins);
    if (i != -1)
        goto done;
    {
        int count = bruce->mHistoryPos;
        if (count < 100)
        {
            i = count;
            bruce->mHistoryPos = i + 1;
        }
        else
        {
            i = 0;
        }
        {
            int checksum = History_Checksum(pName, Spins);
            bruce->mHistory[i].Checksum = checksum;
            bruce->mHistory[i].Count = 0;
        }
    }
done:
{
    int hitCount = bruce->mHistory[i].Count;
    hitCount++;
    bruce->mHistory[i].Count = hitCount;
    return bruce->mHistory[i].Count;
}
}

/**
 * GlowTowards(int current, int target, int step) — move current toward target by step
 *
 * Address: 0x800431EC
 *          PERFECT — asm volatile("+r") forces slt to read from $v1 not $v0
 * Mangled: GlowTowards__Fiii
 *
 * @param A  Current glow component.
 * @param B  Target glow component.
 * @param Speed  Maximum change for this update.
 * @return Component value moved toward the target.
 */
int GlowTowards(int A, int B, int Speed)
{
    int newVal = A + Speed;
    if (A < B)
    {
        register int result asm("$2") = newVal;
        asm volatile("" : "+r"(newVal));
        if (B < newVal)
        {
            result = B;
        }
        return result;
    }
    else
    {
        A = A - Speed;
        if (A < B)
        {
            return B;
        }
        return A;
    }
}

/**
 * GlowUpdate__6CBruce
 *
 * Address:   0x8004322C
 * Mangled:   GlowUpdate__6CBruce
 * Demangled: CBruce::GlowUpdate(void)
 *            Drive the skater glow toward its target color and clear the cycle when settled.
 *
 * @param bruce  Skater whose glow state is advanced.
 */
void CBruce_GlowUpdate(CBruceGlowSlice *bruce)
{
    int glow = GlowTowards(bruce->mGlowR, bruce->mTargetGlowR, bruce->mGlowSpeed);
    bruce->mGlowR = glow;
    glow = GlowTowards(bruce->mGlowG, bruce->mTargetGlowG, bruce->mGlowSpeed);
    bruce->mGlowG = glow;
    glow = GlowTowards(bruce->mGlowB, bruce->mTargetGlowB, bruce->mGlowSpeed);
    bruce->mGlowB = glow;
    if (bruce->mGlowR == bruce->mTargetGlowR)
    {
        if (((bruce->PackedGlowWord() >> 8) & 0xFFFF) ==
            bruce->PackedTargetGlowGB())
        {
            u8 cycle = bruce->mGlowDownSpeed;
            if (cycle != 0)
            {
                bruce->mTargetGlowR = 0;
                bruce->mTargetGlowG = 0;
                bruce->mTargetGlowB = 0;
                bruce->mGlowSpeed = cycle;
                bruce->mGlowDownSpeed = 0;
            }
        }
    }
    if ((bruce->mGlowR | bruce->mGlowG | bruce->mGlowB) != 0)
    {
        bruce->CSuper.CBody.mEllipsoidScale = 0x2000;
        u32 green = bruce->mGlowG;
        bruce->CSuper.CBody.mEllipsoidOTPushback = 0xA;
        u32 red = bruce->mGlowR;
        u8 blue = bruce->mGlowB;
        u32 color = (green << 8) | 0x02000000;
        u32 packed = red | color;
        packed |= (u32)blue << 16;
        bruce->CSuper.CBody.mEllipsoidRGB = packed;
    }
    else
    {
        bruce->CSuper.CBody.mEllipsoidRGB = 0;
    }
}

/**
 * CBruce::DoLightingSetup(void) — empty virtual method override
 *
 * Address: 0x80043330
 *          Target: jr $ra; nop
 * Mangled: DoLightingSetup__6CBruce
 *
 * @param bruce  Skater receiving the lighting setup.
 */
void CBruce_DoLightingSetup(CBruce *bruce)
{
    (void)bruce;
}

/**
 * CalculateDynamicLighting__6CBrucePUiP4Item
 *
 * Address:   0x80043338
 * Mangled:   CalculateDynamicLighting__6CBrucePUiP4Item
 * Demangled: CBruce::CalculateDynamicLighting(unsigned int *, Item *)
 *
 * For indexed-colour faces, samples the region colour table for each vertex
 * and averages the RGB components. Otherwise averages the face's packed RGB.
 * The resulting brightness is scaled by three before the lighting callback.
 */
void CBruce_CalculateDynamicLighting(
    CBruce *bruce, unsigned int *p_face, Item *p_item)
{
    register CBruce *self asm("$10") = bruce;
    register SFaceLightingMech *pFace asm("$11") =
        (SFaceLightingMech *)p_face;
    asm volatile("" : "+r"(self), "+r"(pFace));
    unsigned int flags = pFace->Flags;

    if ((flags & 0x800) != 0)
    {
        unsigned int *pColourTable = PSXRegion[p_item->Region].pColourTable;
        int vertices = 4;
        self->mBrightness = 0;
        if ((flags & 0x10) != 0)
        {
            vertices = 3;
        }
        if (vertices != 0)
        {
            int i = 0;
            unsigned int mask = 0xffffff;
            register unsigned char *bytePtr asm("$6") =
                pFace->Colour.ColourIndices;
            do
            {
                unsigned int idx = *bytePtr;
                bytePtr++;
                i++;
                register unsigned int rgb asm("$4") = pColourTable[idx];
                int sum = self->mBrightness;
                register unsigned int masked asm("$5") = rgb & mask;
                sum += rgb & 0xff;
                sum += (masked >> 8) & 0xff;
                sum += masked >> 16;
                self->mBrightness = sum;
            } while (i < vertices);
        }
        self->mBrightness = self->mBrightness / (vertices * 3);
    }
    else
    {
        unsigned int rgb = pFace->Colour.RGB;
        unsigned int masked = rgb & 0xffffff;
        int sum = (rgb & 0xff) + ((masked >> 8) & 0xff) + (masked >> 16);
        self->mBrightness = sum / 3;
    }
    int b = self->mBrightness;
    self->mBrightness = b * 3;
    register CBruceVtableMech *vtbl asm("$6") =
        (CBruceVtableMech *)self->CSuper.CBody.CItem.vtable;
    register void (*method)(CBruce *) asm("$5") =
        vtbl->DoLightingSetup;
    method(self);
}

/**
 * CBruce::ReadAnalogueInput(void) — read analog stick from pad, optionally flip Y
 *
 * Address: 0x80043474
 *          PERFECT
 * Mangled: ReadAnalogueInput__6CBruce
 *
 * @param bruce  Skater receiving the cached analogue input.
 */
void CBruce_ReadAnalogueInput(CBruceInputSlice *bruce)
{
    bruce->mAY =
        *(u8 *)&bruce->mPad->AnalogueMoveForwardsBackwards;
    bruce->mAX = *(u8 *)&bruce->mPad->AnalogueMoveLeftRight;
    if (Cheat_FlipScreen != 0)
        bruce->mAX = -bruce->mAX;
}

/**
 * CBruce::GetHookPosition(CVector &, unsigned char) — wrapper for M3dUtils
 *
 * Address: 0x800434B4
 *          Calls M3dUtils_GetHookPosition(vec, this, hookIndex & 0xFF)
 * Mangled: GetHookPosition__6CBruceR7CVectorUc
 *
 * @param bruce  Skater whose model hook is queried.
 * @param pos  Position vector.
 * @param hook  Model hook index.
 */
void CBruce_GetHookPosition(
    CBruce *bruce, CVector &pos, u8 hook)
{
    M3dUtils_GetHookPosition(
        reinterpret_cast<VECTOR *>(&pos),
        reinterpret_cast<CSuper *>(bruce), hook);
}

/**
 * CBruce::Die(void) — mark as dead if not already
 *
 * Address: 0x800434E0
 * Mangled: Die__6CBruce
 *
 * @param bruce  Skater being marked dead.
 */
void CBruce_Die(CBruce *bruce)
{
    if (!reinterpret_cast<const CBody *>(&bruce->CSuper.CBody)->IsDead())
    {
        bruce->CSuper.CBody.mCBodyFlags = bruce->CSuper.CBody.mCBodyFlags | 0x40;
        bruce->CSuper.CBody.CItem.mFlags = bruce->CSuper.CBody.CItem.mFlags | 0x41;
    }
}

/**
 * CBruce::DeleteStuff(void) — empty virtual method override
 *
 * Address: 0x80043524
 *          Target: jr $ra; nop
 * Mangled: DeleteStuff__6CBruce
 */
void CBruce_DeleteStuff(CBruce *bruce)
{
    (void)bruce;
}

/**
 * CBruce::Hit(int, const CVector &, int) — always returns 1
 *
 * Address: 0x8004352C
 *          Target: jr $ra; addiu $v0, $zero, 1
 * Mangled: Hit__6CBruceiRC7CVectori
 *
 * @param bruce  Skater receiving the hit.
 * @param Damage  Damage applied by the hit.
 * @param knock  Knockback vector supplied by the caller.
 * @param whathit  Hit-kind identifier.
 * @return 1
 */
int CBruce_Hit(
    CBruce *bruce, int Damage, const CVector &knock, int whathit)
{
    (void)bruce;
    (void)Damage;
    (void)knock;
    (void)whathit;
    return 1;
}

/**
 * CBruce::CollideWithObject(CBody *) — empty virtual method override
 *
 * Address: 0x80043534
 *          Target: jr $ra; nop
 * Mangled: CollideWithObject__6CBruceP5CBody
 *
 * @param bruce  Skater participating in the collision.
 * @param pOb  Body with which the skater collided.
 */
void CBruce_CollideWithObject(CBruce *bruce, CBody *pOb)
{
    (void)bruce;
    (void)pOb;
}

/**
 * DoShadowCheck__6CBruce
 *
 * Address:   0x8004353C
 * Mangled:   DoShadowCheck__6CBruce
 * Demangled: CBruce::DoShadowCheck(void)
 *            Cast a downward shadow ray and update the skater shadow when it finds valid ground.
 *
 * @param bruce  Skater whose shadow is refreshed.
 */
void CBruce_DoShadowCheck(CBruce *bruce)
{
    SLineInfo line_info;

    bruce->FRUVectorsFromRotMatrix();

    if (GNumberOfPlayers == 1)
    {
        line_info.StartCoords.vx =
            bruce->CSuper.CBody.CItem.mPos.vx + bruce->mUp.vx * 50;
        line_info.StartCoords.vy =
            bruce->CSuper.CBody.CItem.mPos.vy + bruce->mUp.vy * 50;
        line_info.StartCoords.vz =
            bruce->CSuper.CBody.CItem.mPos.vz + bruce->mUp.vz * 50;
    }
    else
    {
        line_info.StartCoords.vx = bruce->CSuper.CBody.CItem.mPos.vx;
        line_info.StartCoords.vy = bruce->CSuper.CBody.CItem.mPos.vy;
        line_info.StartCoords.vz = bruce->CSuper.CBody.CItem.mPos.vz;
    }

    line_info.EndCoords.vx = line_info.StartCoords.vx;
    line_info.EndCoords.vy = line_info.StartCoords.vy + 0xFA0000;
    line_info.EndCoords.vz = line_info.StartCoords.vz;
    M3dColij_InitLineInfo(&line_info);
    M3dZone_LineToItem(&line_info, 1);

    if (line_info.pItem &&
        line_info.Normal.vy < -1900 &&
        bruce->mPhysicsState != 4 &&
        bruce->mPhysicsState != 8)
    {
        int normalY = line_info.Normal.vy;
        register int normalX asm("$5") = line_info.Normal.vx;
        int shadowX = line_info.Position.vx;
        int shadowY = line_info.Position.vy;
        int shadowZ = line_info.Position.vz;
        int normalZ = line_info.Normal.vz;
        asm volatile("" : "+r"(normalY), "+r"(normalX), "+r"(shadowX) : : "memory");
        asm volatile("" : "+r"(shadowY), "+r"(shadowZ), "+r"(normalZ) : : "memory");

        bruce->CSuper.CBody.mShadowNormal.vx = normalX;
        {
            int players = GNumberOfPlayers;
            bruce->CSuper.CBody.mShadowNormal.vy = normalY;
            asm volatile("" : : : "memory");
            bruce->CSuper.CBody.mShadowPos.vx = shadowX;
            {
                int one = 1;
                bruce->CSuper.CBody.mShadowPos.vy = shadowY;
                bruce->CSuper.CBody.mShadowPos.vz = shadowZ;
                bruce->CSuper.CBody.mShadowNormal.vz = normalZ;

                if (players == one)
                {
                    int shade = ((4300 - line_info.Distance) << 7) / 4000;
                    CVECTOR *rgb = bruce->CSuper.mpShadowRGB;
                    bruce->CSuper.mShadowLights = players;
                    rgb->b = shade;
                    rgb->g = shade;
                    rgb->r = shade;
                }
            }
        }
    }
    else
    {
        bruce->CSuper.mShadowLights = 0;
    }
}

/**
 * CBruce::InitItem(char *) — call parent InitItem, then read hooks packet
 *
 * Address: 0x800436F0
 * Mangled: InitItem__6CBrucePc
 *
 * @param bruce  Skater whose item data is initialized.
 * @param pName  Asset / object name string.
 */
void CBruce_InitItem(CBruce *bruce, char *pName)
{
    CItem_InitItem(&bruce->CSuper.CBody.CItem, pName);
    int idx = bruce->mSkaterType;
    M3dUtils_ReadHooksPacket(
        reinterpret_cast<CSuper *>(bruce), SkaterHooks[idx]);
}

/**
 * FlipToVelocity__6CBruce
 *
 * Address:   0x80043738
 * Mangled:   FlipToVelocity__6CBruce
 * Demangled: CBruce::FlipToVelocity(void)
 *
 * @param bruce  Skater whose orientation is flipped.
 * @return 1 when the orientation was flipped, otherwise 0.
 */
int CBruce_FlipToVelocity(CBruce *bruce)
{
    if (bruce->CSuper.mAnim == 0x22)
        return 0;

    if (Mips_CVecDotProd(
            reinterpret_cast<CVector *>(&bruce->mFront),
            reinterpret_cast<CVector *>(
                &bruce->CSuper.CBody.mVel)) > 0x5000)
    {
        register int v0 asm("$3");
        register int v1 asm("$4");
        v0 = bruce->mFrontHalfwords.vxLow;
        v1 = bruce->mFrontHalfwords.vyLow;
        v0 = -v0;
        bruce->mRotationMatrix.m[0][2] = v0;
        v0 = (s16)v0;
        v1 = -v1;
        bruce->mRotationMatrix.m[1][2] = v1;
        v1 = (s16)v1;
        bruce->mFront.vx = v0;
        bruce->mFront.vy = v1;

        v0 = bruce->mFrontHalfwords.vzLow;
        v1 = bruce->mRightHalfwords.vxLow;
        v0 = -v0;
        bruce->mRotationMatrix.m[2][2] = v0;
        v0 = (s16)v0;
        v1 = -v1;
        bruce->mRotationMatrix.m[0][0] = v1;
        v1 = (s16)v1;
        bruce->mFront.vz = v0;
        bruce->mRight.vx = v1;

        v0 = bruce->mRightHalfwords.vyLow;
        v1 = bruce->mRightHalfwords.vzLow;
        v0 = -v0;
        bruce->mRotationMatrix.m[1][0] = v0;
        v0 = (s16)v0;
        v1 = -v1;
        bruce->mRotationMatrix.m[2][0] = v1;
        bruce->mRight.vy = v0;
        v0 = bruce->CSuper.mExtraFlags;
        v1 = (s16)v1;
        bruce->mRight.vz = v1;
        v0 ^= 2;
        bruce->CSuper.mExtraFlags = v0;
        return 1;
    }

    return 0;
}

/**
 * BloodSplat__6CBruceii
 *
 * Address:   0x80043820
 * Mangled:   BloodSplat__6CBruceii
 * Demangled: CBruce::BloodSplat(int, int)
 *
 * @param bruce  Skater emitting the blood effect.
 * @param BodyPart  Hook index used as the emission point.
 * @param MaxBloodBits  Maximum number of spark particles to create.
 */
void CBruce_BloodSplat(
    CBruce *bruce, int BodyPart, int MaxBloodBits)
{
    CSVector v;
    CVector Pos;
    CSVector Cone;
    SLineInfo BloodLine;

    if (Cheat_Blood == 0)
        return;

    Video_BloodSplat(bruce, BodyPart, MaxBloodBits);

    if (BitCount >= 200)
        return;

    v.vx = -0x400;
    v.vy = 0;
    v.vz = 0;
    Bit_SetSparkTrajectory(v);

    switch (BodyPart)
    {
    case 1:
        Cone.vx = 0x640;
        Cone.vy = 0x640;
        Cone.vz = 0;
        break;
    case 0:
    default:
        Cone.vx = 0x4B0;
        Cone.vy = 0x320;
        Cone.vz = 0;
        break;
    }
    Bit_SetSparkTrajectoryCone(Cone);
    Bit_SetSparkRGB(0x48, 0, 0);
    Bit_SetSparkFadeRGB(0, 0, 0);
    CBruce_GetHookPosition(bruce, Pos, (u8)BodyPart);

    SparkSemiTrans = reinterpret_cast<void *>(0);
    UseCacheForBits = 0;

    int Bits = 200 - BitCount;
    if (MaxBloodBits < Bits)
        Bits = MaxBloodBits;

    BloodLine.StartCoords.vx = Pos.vx;
    BloodLine.StartCoords.vy = Pos.vy - 0x28000;
    BloodLine.StartCoords.vz = Pos.vz;
    BloodLine.EndCoords.vx = Pos.vx;
    BloodLine.EndCoords.vy = Pos.vy + 0x320000;
    BloodLine.EndCoords.vz = Pos.vz;
    M3dColij_InitLineInfo(&BloodLine);
    M3dZone_LineToItem(&BloodLine, 1);

    int FloorY;
    if (BloodLine.pItem != 0)
        FloorY = BloodLine.Position.vy - 0x4000;
    else
        FloorY = Pos.vy - 0x4000;

    int i;
    Pos.vy = Pos.vy - 0x1E000;
    i = 0;

    if (Bits > 0)
    {
        int gravity = 0x2800;
        do
        {
            if (Mem_GetFree(1) < 10000)
                return;
            Bit_SetSparkSize((i & 1) + 1);
            CSparkMech *pSpark = static_cast<CSparkMech *>(
                Bit_AllocateStorage(sizeof(CSparkMech)));
            if (pSpark != 0)
                pSpark = CSpark_ctor(
                    pSpark, Pos, (i >> 2) + 0xE,
                    gravity, (i & 7) + 0x32);
            reinterpret_cast<CVector &>(pSpark->mVel) +=
                reinterpret_cast<const CVector &>(
                    bruce->CSuper.CBody.mVel);
            pSpark->mFloorY = FloorY;
            i++;
            gravity += 0x100;
        } while (i < Bits);
    }

    UseCacheForBits = 1;
    SparkSemiTrans = reinterpret_cast<void *>(1);
}

/**
 * Run the skater's per-frame control, physics, collision, shadow, and debug
 * display updates.
 *
 * Address:   0x80043A74
 * Mangled:   AI__6CBruce
 * Demangled: CBruce::AI(void)
 *
 * @param bruce  Skater receiving its per-frame update.
 */
void CBruce_AI(CBruceAISlice *bruce)
{
    if (GamePaused != 0)
        return;

    if (((CCamera *)CameraList)->InCompetitionIntro() != 0)
    {
        bruce->mPlayTime = XFrames + 3630;
        return;
    }

    if (bruce->mRecentEvent1 != 0 &&
        XFrames - bruce->mRecentEvent1 > 10)
    {
        bruce->mRecentEvent1 = 0;
    }
    if (bruce->mRecentEvent2 != 0 &&
        XFrames - bruce->mRecentEvent2 > 10)
    {
        bruce->mRecentEvent2 = 0;
    }

    SVideoInfo *VInfo;
    if ((CBodyOwner *)&bruce->Inherited.Base == (CBodyOwner *)MechList)
        VInfo = &GVInfo1;
    else
        VInfo = &GVInfo2;

    switch (VInfo->VideoMode)
    {
    case 1:
        Video_Record((CBruce *)bruce);
        break;
    case 2:
        Video_Playback((CBruce *)bruce);
        return;
    }

    bruce->mResponseTimer = 100;
    bruce->Inherited.Base.RGB = 0x404040;
    CBruce_ReadAnalogueInput((CBruceInputSlice *)bruce);
    CBruce_DoPhysics((CBruce *)bruce);
    CBruce_HandleWater((CBruce *)bruce);

    if ((((bruce->Inherited.Base.AnimFlags >> 1) ^ 1) & 1) != 0)
        ++bruce->mGroundCounter;
    else
        bruce->mGroundCounter = 0;

    if (bruce->mInputDelay != 0)
        --bruce->mInputDelay;

    CVector nearest;
    CBody *pOb = M3dColij_LineToSphere(
        reinterpret_cast<const CVector &>(
            bruce->Inherited.Base.OldPosition),
        reinterpret_cast<const CVector &>(
            bruce->Inherited.Base.Position),
        nearest, BaddyList, 0, 0x1000);
    if (pOb != 0)
        CBruce_CollideWithObject((CBruce *)bruce, pOb);

    pOb = M3dColij_LineToSphere(
        reinterpret_cast<const CVector &>(
            bruce->Inherited.Base.OldPosition),
        reinterpret_cast<const CVector &>(
            bruce->Inherited.Base.Position),
        nearest, PowerUpList, 0, 0x1000);
    if (pOb != 0)
        reinterpret_cast<CPowerUp *>(pOb)->TakeEffect((CBruce *)bruce);

    if (bruce->mPhysicsState != 0 ||
        MechAbs(bruce->Inherited.Base.Position.vy - bruce->mShadowY) >= 65)
    {
        bruce->mShadowY = bruce->Inherited.Base.Position.vy;
        CBruce_DoShadowCheck((CBruce *)bruce);
    }
    else
    {
        bruce->Inherited.Base.ShadowX = bruce->Inherited.Base.Position.vx;
        bruce->Inherited.Base.ShadowZ = bruce->Inherited.Base.Position.vz;
    }

    if (bruce->Inherited.Base.GroundAngle < 0)
        bruce->Inherited.Base.GroundAngle = 0;

    if ((Cheat_FPS != 0 || Cheat_StatsOverride != 0) &&
        bruce == reinterpret_cast<CBruceAISlice *>(GSkater[0]))
    {
        static char lm[30];

        sprintf(
            lm,
            Mech_StatsFormat,
            Cheat_StatsOverride != 0
                ? Cheat_StatsOverride
                : (Stats[bruce->mSkaterType].Toughness +
                       ((bruce->mMagicTime != 0 || Cheat_Fire != 0)
                            ? 3
                            : 0)) +
                      bruce->mpBoardStats->ollie,
            Cheat_StatsOverride != 0
                ? Cheat_StatsOverride
                : (Stats[bruce->mSkaterType].Speed +
                       ((bruce->mMagicTime != 0 || Cheat_Fire != 0)
                            ? 3
                            : 0)) +
                      bruce->mpBoardStats->speed,
            Cheat_StatsOverride != 0
                ? Cheat_StatsOverride
                : (Stats[bruce->mSkaterType].Air +
                       ((bruce->mMagicTime != 0 || Cheat_Fire != 0)
                            ? 3
                            : 0)) +
                      bruce->mpBoardStats->air,
            Cheat_StatsOverride != 0
                ? Cheat_StatsOverride
                : (Stats[bruce->mSkaterType].Balance +
                       ((bruce->mMagicTime != 0 || Cheat_Fire != 0)
                            ? 3
                            : 0)) +
                      bruce->mpBoardStats->balance);
        Font_String(
            0x1A4, 200, lm, 0x5A, 0x5A, 0x64, 0);
    }
}

/**
 * Update the skater's water flag and periodically emit a surface ripple.
 *
 * Address:   0x80043F38
 * Mangled:   HandleWater__6CBruce
 * Demangled: CBruce::HandleWater(void)
 *
 * @param bruce  Skater whose water interaction is updated.
 */
void CBruce_HandleWater(CBruce *bruce)
{
    if (GNumberOfPlayers == 2)
        return;

    int WaterLevel;
    if (&bruce->CSuper.CBody == MechList)
    {
        WaterLevel = CurrentWaterLevel;
    }
    else
    {
        WaterLevel = CurrentWaterLevel2;
    }

    int bottom = bruce->CSuper.CBody.CItem.mPos.vy + 0x19000;
    if (WaterLevel < bottom)
    {
        int flags = bruce->CSuper.CBody.CItem.mFlags;
        int splashSfx = 0x2B;
        if ((flags & 8) == 0)
        {
            SFX_PlayX(splashSfx, 0x2000, 0);
            flags = bruce->CSuper.CBody.CItem.mFlags;
        }

        flags = flags | 8;

        int timer = TTime;
        timer &= 3;
        bruce->CSuper.CBody.CItem.mFlags = flags;
        if (timer != 0)
            return;

        CVector p;
        p = reinterpret_cast<const CVector &>(
            bruce->CSuper.CBody.CItem.mPos);
        p.vy = WaterLevel;

        int offset = Rnd(24) + 16;
        int angle = Rnd(0x1000) & 0xFFF;
        s16 *tbl = rcossin_tbl;
        s16 *base = &tbl[angle * 2];
        p.vx += offset * base[0];
        p.vz += offset * base[1];

        CFlatRipple *ripple =
            static_cast<CFlatRipple *>(
                Bit_AllocateStorage(sizeof(CFlatRipple)));
        if (ripple != 0)
        {
            int radius = Rnd(24) + 40;
            int segments = Rnd(3) + 4;
            ::new (ripple) CFlatRipple(
                p, radius,
                0xDD, 0xEE, 0xFF, 12, segments);
        }
    }
    else
    {
        bruce->CSuper.CBody.CItem.mFlags =
            bruce->CSuper.CBody.CItem.mFlags & ~8;
    }
}

/**
 * SFX_Grunt__FP6CBruce
 *
 * Address:   0x800440E8
 * Mangled:   SFX_Grunt__FP6CBruce
 * Demangled: SFX_Grunt(CBruce *)
 *
 * @param pSkater  Active skater being processed.
 */
void SFX_Grunt(CBruce *pSkater)
{
    int terrain = Face_Terrain;
    int sfxId;

    if (terrain == 7) goto case_7;
    if (terrain == 9) goto case_9;
    goto after_sfx;
case_7:
    sfxId = 0x35;
    goto do_play1;
case_9:
    sfxId = 0x28;
    if (GLevel != 8) goto do_play1;
    sfxId = 0x89;
do_play1:
    SFX_PlayX(sfxId, 0x2000, 0);

after_sfx:;
    int playerIdx = pSkater->mSkaterType;
    int offset = playerIdx * sizeof(SStats);
    register SStats *statsBase asm("$4") = Stats;
    asm volatile("" : "+r"(statsBase));
    statsBase = (SStats *)((char *)statsBase + offset);
    int field = statsBase->ComboID;

    if (field == 0xA000) goto set_1f;
    if (field != 0xC000) goto rnd_path;
set_1f:
    sfxId = 0x1F;
    goto do_play2;
rnd_path:
    sfxId = Rnd(3) + 0x1C;
do_play2:
    SFX_PlayX(sfxId, 0x2000, 0);
}
