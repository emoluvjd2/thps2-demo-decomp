// TRIG.cpp -- Reconstructed translation unit
// Original: TRIG.CPP
// Functions: 34 (34 strict matches)
// Address range: 0x80045704 - 0x80048330
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#include "globals.h"
#include "CITEM.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include "RAIL.h"
#include "CAMPT.h"
#include "MAIN.h"
#include "BADDY.h"
#include "MECH.h"
#include "M3D.h"

/** Return caller-provided storage for an owner constructor. */
inline void *operator new(unsigned int, void *address)
{
    return address;
}

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
struct CNonRenderedBit;
struct SVideoInfo;
struct SCommandPoint;
struct SGapTrick;

/** Runtime trigger-command point stored in checksum and owner lists. */
struct SCommandPoint {
    unsigned short *pCommands;
    unsigned char Collision;
    unsigned char Executed;
    unsigned char NumPulsesSet;
    unsigned char PulsesReceived;
    unsigned short NumPulses;
    unsigned short NodeIndex;
    unsigned int Checksum;
    struct SCommandPoint *pNextSimilar;
    struct SCommandPoint *pNext;
};

#define TRIGSTATE_OWNER_DECLARATION
#define TRIGSTATE_DECLARE_NUM_NODES
#include "TRIGSTATE.h"
extern "C" int NumTrigMenuEntries;
#define TRIGSTATE_OWNER_DECLARATION
#define TRIGSTATE_DECLARE_OFFSET_LIST
#include "TRIGSTATE.h"
#define TOTALCOMMANDPOINTMEMORY_OWNER_DECLARATION
#include "TOTALCOMMANDPOINTMEMORY.h"
#define TRIGSTATE_OWNER_DECLARATION
#define TRIGSTATE_DECLARE_TRIG_FILE
#include "TRIGSTATE.h"
extern "C" int TrigFileSize;
#define CURRENTOBJFILE_OWNER_DECLARATION
#include "CURRENTOBJFILE.h"

#include "CAMERALIST.h"
#include "CURRENTGAMELEVEL.h"
#define BODYLISTS_DECLARE_ENVIRONMENTAL_OBJECT_LIST
#include "BODYLISTS.h"
#define VIDEOINFO_DECLARE_GVINFO1
#include "VIDEOINFO.h"
extern "C" SGapTrick GapTricks[304];
#define M3DCOLOURSTATE_DECLARE_FADE_COLOUR
#include "M3DCOLOURSTATE.h"
#include "MECHLIST.h"
#define BITSTATE_DECLARE_NON_RENDERED_BIT_LIST
#include "BITSTATE.h"
extern "C" unsigned int SuspendDistance
    __attribute__((section(".data")));
#define BODYLISTS_DECLARE_SUSPENDED_LIST
#include "BODYLISTS.h"
#define BITSTATE_DECLARE_USE_CACHE_FOR_BITS
#include "BITSTATE.h"
#include "ZEROVECTOR.h"

// Owner-local gameplay layouts used by TRIG.cpp.
//
// Trigger code only needs the item/body prefix of the gameplay hierarchy. The
// remaining CSuper and CBruce storage stays opaque while retaining the exact
// original sizes recorded in MAIN_STRUCTS.h.

struct SLight;
struct CCamera;

/** TRIG only reads the replay mode at the head of this descriptor. */
struct SVideoInfo
{
    int VideoMode;
};

/** One byte value stored in the low byte of a TRG 16-bit command word. */
struct STriggerByteWord
{
    u8 Value;
    u8 padding;
};

/** Light-node payload view used by the initial TRG parser. */
struct STriggerLightBytes
{
    u16 ClipDist;
    u16 InnerRadius;
    u16 FringeRadius;
    u16 Amp;
    STriggerByteWord r0;
    STriggerByteWord g0;
    STriggerByteWord b0;
    STriggerByteWord r1;
    STriggerByteWord g1;
    STriggerByteWord b1;
};

/** Light-node payload view used when pulsing an already parsed node. */
struct STriggerLightWords
{
    u16 ClipDist;
    u16 InnerRadius;
    u16 FringeRadius;
    u16 Amp;
    u16 r0;
    u16 g0;
    u16 b0;
    u16 r1;
    u16 g1;
    u16 b1;
};

/** Packed payload following a create-background trigger command. */
struct SBackgroundCommandPayload
{
    u32 Checksum;
    CSVector AngularVelocity;
} __attribute__((packed));

/** Composition-only gameplay-item storage used by trigger owner views. */
struct TrigCItemOwner {
    u16 mFlags;
    u16 mInquiry;
    struct CVector mPos;
    struct CSVector mAngles;
    u16 mModel;
    char mDummyFrame;
    u8 mTintIndex;
    u8 mDummyAnim;
    u8 mRegion;
    struct TrigCItemOwner *mNextItem;
    u32 mRGB;
    struct CSVector mScale;
    u32 mTRN;
    struct TrigCItemOwner *mPreviousItem;
    u32 mStoredRGB;
    u8 mPredatorEffectType;
    u8 mPredatorEffectFade;
    u16 mType;
    struct SLight *mpLight;
    u16 mFlashTimer;
    s16 mFlash_dR;
    s16 mFlash_dG;
    s16 mFlash_dB;
    void **_vf;
};

/** Physics body prefix used by restart and kill commands (264 bytes). */
struct CBody {
    struct TrigCItemOwner CItem;
    struct CBody **mppOriginalList;
    u16 mInputFlags;
    u16 mCBodyFlags;
    struct CVector mVel;
    struct CVector mAcc;
    struct CFriction mFric;
    struct CSVector mAngVel;
    struct CSVector mAngAcc;
    struct CFriction mAngFric;
    struct CSVector mNormal;
    u8 mEventFlags;
    u32 mEllipsoidRGB;
    u16 mEllipsoidScale;
    s16 mEllipsoidOTPushback;
    struct CVector mShadowPos;
    struct CSVector mShadowNormal;
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
    struct CVector mOldPos;
    struct CVector mTweenPos;
    struct CVector mNewPos;
    struct CVector mDPos;
    struct CSVector mOldAngles;
    struct CSVector mNewAngles;
    struct CSVector mDAngles;

    /** Move this body from its live list to the suspended list. */
    void Suspend(CBody **originalList);
    /** Restore this body to its saved live list. */
    void UnSuspend();
};

/** Camera fields directly manipulated by trigger commands. */
struct CCamera
{
    struct CBody CBody;
    u8 opaque_before_tripod[1012 - 264];
    struct CBody *mTripod;
    u8 opaque_before_focus[1068 - 1016];
    struct CBody *mFocus;
    u8 opaque_before_pitch_damper[1156 - 1072];
    s16 mPitchDamper;
    s16 mTripodPosYDamper;
    u8 opaque_before_camera_mode[1368 - 1160];
    int mCameraMode;
    u8 opaque_after_camera_mode[1668 - 1372];

    /** Rebuild tracking orientation after a trigger restart. */
    void ResetOrientation();
    /** Set the left/right collision-ray distance. */
    void SetCollisionRayLR(int ray);
    /** Set the rear collision-ray distance. */
    void SetCollisionRayBack(int ray);
    /** Set the left/right collision response angle. */
    void SetCollisionAngLR(s16 angle);
    /** Set the rear collision response angle. */
    void SetCollisionAngBack(s16 angle);
    /** Select the complex camera mode. */
    void SetComplexMode(u16 mode);
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
};

/** Linked effect base used by non-rendered trigger lights. */
struct CBit
{
    struct CBit *mPrevious;
    struct CBit *mNext;
    s16 mAge;
    u16 mLifetime;
    struct CVector mTweenPos;
    struct CVector mPos;
    struct CVector mVel;
    struct CVector mAcc;
    struct CFriction mFric;
    u8 mDead;
    u16 mFrigDeltaZ;
    u8 mProtected;
    u8 mType;
    void **_vf;

    /** Mark this bit for removal. */
    void Die();
    static void operator delete(void *p);
};

#include "CNONRENDEREDBIT.h"
#include "CLIGHT.h"

/** Target-proven trigger view through CLight's on/off byte. */
struct TrigLightPrefix
{
    struct CNonRenderedBit CNonRenderedBit;
    u8 opaque_before_on[14];
    u8 mOn;
};

/** Target-proven storage extent used by the two trigger light allocations. */
enum { TRIG_LIGHT_STORAGE_SIZE = 100 };

/** Power-up layout used for its body-list prefix. */
struct CPowerUp
{
    struct CBody CBody;
    u8 opaque_after_body[312 - 264];

    /** Associate this power-up with a trigger node. */
    void SetNode(int nodeIndex);
};

/** Animation owner prefix; fields beyond CBody are opaque to trigger code. */
struct CSuper {
    /** Start a bounded animation sequence. */
    void RunAnim(int anim, int from, int to, int to2);

    struct CBody CBody;
    u8 opaque_after_CBody[400 - 264];
};

/** Baddy fields used by the platform kill/bounce path. */
struct CBaddy
{
    struct CSuper CSuper;
    u32 mMode;
    u8 mDoScript;
    u8 mReadHooks;
    u8 mApplyGravity;
    u8 mHit;
    u8 mInputSignal;
    u8 mDeathPulseSent;
    u8 mComplexSurfaceInterval;
    u8 mComplexSurfaceTimer;
    u16 mInputFlagsCopy;
    u16 mSubType;
    u16 mVerticalCompliance;
    u16 opaque_before_specific_flags;
    u32 mSpecificFlags;
    u8 opaque_after_specific_flags[532 - 424];
};

/** Platform object; TRIG only needs the CBaddy prefix. */
struct CPlatform
{
    /** Initialize a scripted platform from one trigger record. */
    CPlatform(short *pInfo, int nodeIndex);

    struct CBaddy CBaddy;
    u8 opaque_after_baddy[592 - 532];

    /** Apply this platform's configured bounce response to a skater. */
    void Bounce(CBruce *pSkater, const CVector &direction);
};

/** Script-driven baddy; TRIG only needs its allocation size and base prefix. */
struct CScriptOnlyBaddy
{
    /** Initialize a script-only actor from one trigger record. */
    CScriptOnlyBaddy(short *pInfo, int nodeIndex);

    struct CBaddy CBaddy;
    u8 opaque_after_baddy[556 - 532];
};

#include "CCAR.h"

/** Trigger-created background object. */
struct CBackground
{
    /** Constructs and links one trigger-created background object. */
    CBackground(unsigned int checksum, const CSVector &angVel);
    /** Allocate background storage through CItem's allocator. */
    static void *operator new(unsigned int size) throw()
    {
        return CItem::operator new(size);
    }

    struct CBody CBody;
    u16 mIndex;
    u16 _pad;
};

#define TRICKOB_LAYOUT
#define TRICKOB_LAYOUT_ONLY
#include "TRICKOB.h"
#undef TRICKOB_LAYOUT_ONLY
#undef TRICKOB_LAYOUT

/** Player HUD storage embedded in CBruce. */
struct CPanelData
{
    u8 opaque[2276];

    /** Activate the out-of-bounds panel message. */
    void OutOfBounds();
};

/**
 * Skater layout slice used by gap-trigger bookkeeping.
 *
 * These fields retain their original MAIN_STRUCTS.h offsets while unrelated
 * physics state remains opaque to this owner.
 */
struct CBruce {
    /** Clears accumulated trick state after a bail. */
    void Trick_Bail();
    /** Adds one named trick to the active scoring sequence. */
    void Trick_Trick(char *name, int points, int lock);
    /** Clears the buffered tap-turn state. */
    void ResetTrickBuffers();

    struct CSuper CSuper;
    struct CPanelData mPanelData;
    struct CCamera *mpCamera;
    u8 opaque_before_doing_a_trick[3408 - 2680];
    int mDoingATrick;
    u8 opaque_before_index[3500 - 3412];
    int mIndex;
    u8 opaque_before_jump_pending[3612 - 3504];
    int mJumpPending;
    u8 opaque_before_jump_strength[3620 - 3616];
    int mJumpStrength;
    u8 opaque_before_safe_pos[3652 - 3624];
    struct CVector mSafePos;
    u8 opaque_before_rotation_matrix[3728 - 3664];
    MATRIX mRotationMatrix;
    u8 opaque_before_stick_to_rail[3788 - 3760];
    int mStickToRail;
    u8 opaque_before_in_bail[3964 - 3792];
    int mInBail;
    u8 opaque_before_gap_checksums[3980 - 3968];
    int mGapChecksum[10];
    int mGapType[10];
    int mGaps;
    struct SGapTrick *mpGrindGapTrick;
    u8 opaque_before_front[4256 - 4068];
    struct CVector mFront;
    struct CVector mRight;
    struct CVector mUp;
    u8 opaque_before_ignore_input_timer[4396 - 4292];
    int mIgnoreBruceInputTimer;
    int mIgnoreBruceInputFreezeTimer;
    u8 opaque_after_ignore_input_timers[5228 - 4404];
};

typedef char Trig_CommandPoint_size_must_be_24[
    (sizeof(SCommandPoint) == 24) ? 1 : -1];
typedef char Trig_CItem_size_must_be_76[
    (sizeof(TrigCItemOwner) == 76) ? 1 : -1];
typedef char Trig_CBody_size_must_be_264[(sizeof(CBody) == 264) ? 1 : -1];
typedef char Trig_CCamera_size_must_be_1668[
    (sizeof(CCamera) == 1668) ? 1 : -1];
typedef char Trig_CBit_size_must_be_72[(sizeof(CBit) == 72) ? 1 : -1];
typedef char Trig_CNonRenderedBit_size_must_be_72[
    (sizeof(CNonRenderedBit) == 72) ? 1 : -1];
typedef char Trig_LightPrefix_size_must_be_88[
    (sizeof(TrigLightPrefix) == 88) ? 1 : -1];
typedef char Trig_LightPrefix_on_offset_must_be_86[
    ((unsigned long)&(((TrigLightPrefix *)0)->mOn) == 86) ? 1 : -1];
typedef char Trig_Light_storage_size_must_be_100[
    (TRIG_LIGHT_STORAGE_SIZE == 100) ? 1 : -1];
typedef char Trig_CPowerUp_size_must_be_312[
    (sizeof(CPowerUp) == 312) ? 1 : -1];
typedef char Trig_CSuper_size_must_be_400[(sizeof(CSuper) == 400) ? 1 : -1];
typedef char Trig_CBaddy_size_must_be_532[(sizeof(CBaddy) == 532) ? 1 : -1];
typedef char Trig_CPlatform_size_must_be_592[
    (sizeof(CPlatform) == 592) ? 1 : -1];
typedef char Trig_CScriptOnlyBaddy_size_must_be_556[
    (sizeof(CScriptOnlyBaddy) == 556) ? 1 : -1];
typedef char Trig_CCar_size_must_be_540[(sizeof(CCar) == 540) ? 1 : -1];
typedef char Trig_CBackground_size_must_be_268[
    (sizeof(CBackground) == 268) ? 1 : -1];
typedef char Trig_CTrickOb_size_must_be_24[
    (sizeof(CTrickOb) == 24) ? 1 : -1];
typedef char Trig_CPanelData_size_must_be_2276[
    (sizeof(CPanelData) == 2276) ? 1 : -1];
typedef char Trig_CBruce_size_must_be_5228[(sizeof(CBruce) == 5228) ? 1 : -1];
typedef char Trig_STriggerLightBytes_size_must_be_20[
    (sizeof(STriggerLightBytes) == 20) ? 1 : -1];
typedef char Trig_STriggerLightWords_size_must_be_20[
    (sizeof(STriggerLightWords) == 20) ? 1 : -1];
typedef char Trig_SBackgroundCommandPayload_size_must_be_10[
    (sizeof(SBackgroundCommandPayload) == 10) ? 1 : -1];
typedef char Trig_MATRIX_size_must_be_32[(sizeof(MATRIX) == 32) ? 1 : -1];
typedef char Trig_rotation_matrix_offset_must_be_0xe90[
    ((unsigned long)&(((CBruce *)0)->mRotationMatrix) == 0xE90) ? 1 : -1];

#define TRIG_OWNER_DECLARATIONS
#include "TRIG.h"
#undef TRIG_OWNER_DECLARATIONS
#include "SFX.h"
#include "OB.h"
#include "BIT.h"
#include "MESS.h"
#include "UTILS.h"
#include "CAR.h"
#include "POWERUP.h"
#include "BACKGRND.h"
#include "PLATFORM.h"
#include "EXP.h"
#include "M3DINIT.h"
#include "REDBOOK.h"
#include "SPOOL.h"
#include "TRICKOB.h"

/** Gap definitions and score/name metadata used by trigger command 0xC9. */
SGapTrick GapTricks[304] __attribute__((section(".data"))) = {
    {0, 250, "[WIMPY GAP]"},
    {1, 500, "[GAP]"},
    {2, 1000, "[PHAT GAP]"},
    {1001, 200, "[TRANSFER]"},
    {1005, 600, "[TAXI GAP]"},
    {1006, 100, "[KICKER GAP]"},
    {1007, 300, "[OVER THE PIPE]"},
    {1008, 300, "[SECRET ROOM]"},
    {1009, 400, "[FACEPLANT]"},
    {1010, 1000, "[ACID DROP]"},
    {1100, 10, "[10 Feet]"},
    {1101, 10, "[20 Feet]"},
    {1102, 10, "[30 Feet]"},
    {1103, 10, "[40 Feet]"},
    {1104, 10, "[50 Feet]"},
    {1105, 10, "[60 Feet]"},
    {1106, 10, "[70 Feet]"},
    {1107, 10, "[80 Feet]"},
    {1108, 10, "[90 Feet]"},
    {1109, 10, "[100 Feet]"},
    {1200, 1000, "[WHOOP GAP]"},
    {1201, 100, "[WALL GAP]"},
    {1202, 100, "[OVER THE BOX]"},
    {1203, 2000, "[OVER THE RAFTERS]"},
    {1204, 700, "[OVER THE PIPE]"},
    {1205, 500, "[POOL HIP]"},
    {1206, 700, "[POOL 2 WALKWAY]"},
    {1207, 250, "[HP TRANSFER]"},
    {1300, 1000, "[BRIDGE GAP]"},
    {1301, 700, "[VERT WALL GAP]"},
    {1302, 700, "[TWINKIE TRANSFER]"},
    {1303, 800, "[OVER DA POOL]"},
    {2000, 500, "[ROOF 2 ROOF GAP]"},
    {2001, 1000, "[SWIM TEAM GAP]"},
    {2002, 125, "[AWNING GAP]"},
    {2003, 50, "[GARBAGE OLLIE]"},
    {2004, 750, "[ROOF TO AWNING GAP]"},
    {2005, 250, "[DITCH SLAP]"},
    {2006, 750, "[OVER THE AIR CONDITIONER]"},
    {2007, 1000, "[OVER A FOOTBRIDGE]"},
    {2008, 500, "[PARK GAP]"},
    {2009, 250, "[MINI GAP]"},
    {2010, 100, "[PLANTER GAP]"},
    {2011, 100, "[KICKER GAP]"},
    {2100, 100, "[SS NORAIL GAP 1]"},
    {2101, 100, "[SS NORAIL GAP 2]"},
    {2102, 100, "[SS NORAIL GAP 3]"},
    {2103, 100, "[SS NORAIL GAP 4]"},
    {2104, 100, "[SS NORAIL GAP 5]"},
    {2105, 100, "[SS NORAIL GAP 6]"},
    {2106, 100, "[SS NORAIL GAP 7]"},
    {2107, 100, "[SS NORAIL GAP 8]"},
    {2108, 100, "[SS NORAIL GAP 9]"},
    {2109, 100, "[SS NORAIL GAP 10]"},
    {2300, 250, "[OVER A 16 STAIR SET]"},
    {2301, 2000, "[OVER A HUGE 32 STAIR GAP]"},
    {2302, 500, "[]"},
    {2303, 500, "[SKATER ESCALATOR GAP]"},
    {2304, 2500, "[32 STEPS OFF A MEZZANINE]"},
    {2305, 100, "[THE FLYING LEAP]"},
    {2306, 100, "[PLANTER GAP]"},
    {2307, 250, "[RAMP GAP]"},
    {2308, 250, "[GOING UP GAP]"},
    {2309, 250, "[GOING DOWN GAP]"},
    {2310, 250, "[FOUNTAIN GAP]"},
    {2400, 250, "[PIGEON SHIT GAP]"},
    {2500, 500, "[LOVE FOUNTAIN GAP]"},
    {2501, 500, "[CHILLIN' ON THE BALCONY]"},
    {2502, 100, "[LOVE STAIR SET]"},
    {2503, 100, "[UP THE SMALL STEP SET]"},
    {2504, 100, "[BENCH GAP]"},
    {2505, 250, "[PHILLYSIDE HP TRANSFER]"},
    {2506, 500, "[SPECTRUM SET]"},
    {2507, 500, "[SHOT...]"},
    {2508, 500, "[SCORE...]"},
    {2509, 500, "[AND THE CROWD GOES WILD!!!]"},
    {2510, 100, "[WORLDS MOST OBVIOUS GAP]"},
    {2600, 250, "[RAGING GORGE GAP]"},
    {2602, 1000, "[HUGE WATER HAZARD GAP]"},
    {2603, 50, "[50 FEET]"},
    {2604, 100, "[100 FEET]"},
    {2605, 150, "[150 FEET]"},
    {2606, 200, "[200 FEET]"},
    {2607, 250, "[250 FEET]"},
    {2609, 250, "[SMALL WATER HAZARD GAP]"},
    {2610, 500, "[BIG WATER HAZARD GAP]"},
    {2611, 25, "[25 FEET]"},
    {2612, 75, "[75 FEET]"},
    {2613, 125, "[125 FEET]"},
    {2614, 175, "[175 FEET]"},
    {2615, 225, "[225 FEET]"},
    {3000, 250, "[OVER THE GATE]"},
    {3001, 500, "[OVER THE CROSSBAR]"},
    {3002, 100, "[MAIN HIP]"},
    {3003, 100, "[BIG BOWL HIP]"},
    {3004, 500, "[FREAKIN' HUGE HIP]"},
    {3005, 200, "[DUMPSTER POP]"},
    {3006, 250, "[TABLE POP]"},
    {3007, 250, "[2 THE BOX]"},
    {3008, 250, "[OVER THE TABLE]"},
    {3009, 250, "[OVER THE SPEAKERS]"},
    {3010, 250, "[OVER THE DUMPSTER]"},
    {3011, 250, "[BOX 2 BOX ACTION]"},
    {3012, 500, "[OVER THE BOX]"},
    {3013, 5000, "[HUMPTEY HUMPS!!!]"},
    {3014, 150, "[SHORTY DUMPSTER POP]"},
    {3015, 150, "[SHORTY TABLE POP]"},
    {3016, 150, "[SHORTY SPEAKER POP]"},
    {3017, 100, "[SPEAKERS POP]"},
    {3018, 100, "[OVER THE LIL' 4]"},
    {3019, 250, "[UP THE LIL' 4]"},
    {3020, 500, "[BIG OL' STANKY GAP]"},
    {3021, 250, "[WATER UP LE BACKSIDE]"},
    {3022, 1000, "[BIG MOUTH GAP]"},
    {3023, 250, "[UP!]"},
    {3024, 500, "[UP!!]"},
    {3025, 1000, "[AND AWAY!!!]"},
    {3300, 500, "[LEAP OF FAITH!]"},
    {3600, 500, "[MUSKA'S GAP']"},
    {3601, 100, "[TABLE POP]"},
    {4000, 250, "[DROPPING IN ON TONY]"},
    {4001, 1000, "[UP 2 COMBI]"},
    {4002, 50, "[DOWN TO TONYS ISLAND]"},
    {4003, 50, "[DOWN TO TONYS ISLAND]"},
    {4004, 1500, "[SNAKE GAP]"},
    {4005, 1500, "[SNAKE GAP]"},
    {4006, 1500, "[SNAKE GAP]"},
    {4007, 2000, "[SNAKE GAP]"},
    {4008, 2000, "[SNAKE GAP]"},
    {4009, 2000, "[SNAKE GAP]"},
    {4010, 300, "[GAP]"},
    {4011, 150, "[GAP]"},
    {4012, 500, "[GAP]"},
    {4013, 50, "[GAP]"},
    {4014, 300, "[GAP]"},
    {4015, 2000, "[TIGHT LANDING]"},
    {4016, 100, "[SAN DIEGUITO TEN SET]"},
    {4017, 400, "[BIG FAT GRASSY ASS]"},
    {4018, 50, "[GRASSY ASS]"},
    {4019, 2000, "[FEED ME!!!]"},
    {4020, 0, "[NO SOUP FOR YOU!!!]"},
    {4021, 0, "[DON'T LOOK DOWN!!!]"},
    {4022, 0, "[GIVE IT UP! TAKE UP ROLLERBLADING!]"},
    {4023, 500, "[PUSSY SNAKE GAP]"},
    {4024, 500, "[PUSSY SNAKE GAP]"},
    {4025, 500, "[PUSSY SNAKE GAP]"},
    {4026, 300, "[GAP]"},
    {4027, 300, "[GAP]"},
    {4028, 1000, "[GAP]"},
    {4029, 1000, "[OVER THE DOME]"},
    {4030, 1000, "[CLEARING THE SWINGS]"},
    {4031, 1000, "[GAP]"},
    {4032, 1000, "[TUNNEL OF LUVIN]"},
    {11000, 250, "[CHANNEL GAP]"},
    {11001, 200, "[KICKER 2 LEDGE]"},
    {11002, 1000, "[GAP]"},
    {11100, 100, "[FREAKIN POOL GRIND]"},
    {11101, 100, "[GAP]"},
    {11102, 100, "[GAP]"},
    {11200, 1000, "[ROLL IN CHANNEL GAP]"},
    {11201, 500, "[CHANNEL GAP]"},
    {12000, 1000, "[HALL PASS GAP]"},
    {12001, 1000, "[GAP]"},
    {12002, 1000, "[GAP]"},
    {12100, 100, "[SS EITHER GAP 1]"},
    {12101, 100, "[SS EITHER GAP 2]"},
    {12102, 100, "[SS EITHER GAP 3]"},
    {12103, 100, "[SS EITHER GAP 4]"},
    {12104, 100, "[SS EITHER GAP 5]"},
    {12105, 100, "[SS EITHER GAP 6]"},
    {12106, 100, "[SS EITHER GAP 7]"},
    {12107, 100, "[SS EITHER GAP 8]"},
    {12108, 100, "[SS EITHER GAP 9]"},
    {12109, 100, "[SS EITHER GAP 10]"},
    {12400, 100, "[WELCOME TO BROOKLYN BANKS]"},
    {12500, 500, "[GRIND UP DEM STAIRS]"},
    {12501, 500, "[SHREDDIN UP THE SPECTRUM SET]"},
    {13000, 1000, "[GAP]"},
    {13001, 1000, "[GAP]"},
    {13002, 1000, "[GAP]"},
    {14000, 1000, "[GAP]"},
    {21000, 200, "[BIG RAIL]"},
    {21001, 300, "[DECK 2 RAIL]"},
    {21002, 500, "[TAXI 2 LEDGE]"},
    {21003, 1000, "[TAXI 2 RAIL]"},
    {21004, 200, "[KICKER 2 LEDGE]"},
    {21005, 500, "[MONSTER GRIND]"},
    {21006, 200, "[HIGH RAIL]"},
    {21007, 3000, "[HOLY SHI...]"},
    {21008, 400, "[TRANSITION GRIND]"},
    {21200, 100, "[KICKER 2 EDGE]"},
    {21201, 200, "[BS GRIND]"},
    {21202, 750, "[RAIL 2 RAIL TRANSFER]"},
    {21203, 500, "[BILLBOARD GRIND]"},
    {21204, 3000, "[DIRTY RAIL]"},
    {21205, 2000, "[DEATH GRIND]"},
    {21300, 1000, "[TRIPLE RAIL]"},
    {21301, 800, "[BRIDGE GRIND]"},
    {21302, 1000, "[HAWK BRIDGE GRIND]"},
    {21400, 1000, "[RAFTER RAIL]"},
    {21401, 1000, "[PIPE 2 BOX GRIND]"},
    {21402, 500, "[LIGHT GRIND]"},
    {21403, 700, "[WALKWAY RAIL TRANS]"},
    {21404, 1000, "[POOL RAIL TRANS]"},
    {21500, 1000, "[ET GRIND]"},
    {21501, 1000, "[BHOUSE RAIL]"},
    {21502, 2000, "[POOL GRIND]"},
    {21503, 800, "[DECK GRIND]"},
    {21504, 2000, "[MB EMERSON GRIND]"},
    {22000, 250, "[DUMPSTER RAIL GAP]"},
    {22001, 500, "[PLAYGROUND RAIL]"},
    {22002, 750, "[RAIL TO RAIL TRANSFER]"},
    {22003, 1000, "[HUGE RAIL]"},
    {22004, 2500, "[LONG ASS RAIL]"},
    {22005, 250, "[FUNBOX TO RAIL TRANSFER]"},
    {22006, 500, "[FUNBOX TO TABLE TRANSFER]"},
    {22007, 50, "[GIMME GAP]"},
    {22008, 500, "[HANDICAP RAMP RAIL]"},
    {22100, 100, "[SS MUST GAP 1]"},
    {22101, 100, "[SS MUST GAP 2]"},
    {22102, 100, "[SS MUST GAP 3]"},
    {22103, 100, "[SS MUST GAP 4]"},
    {22104, 100, "[SS MUST GAP 5]"},
    {22105, 100, "[SS MUST GAP 6]"},
    {22106, 100, "[SS MUST GAP 7]"},
    {22107, 100, "[SS MUST GAP 8]"},
    {22108, 100, "[SS MUST GAP 9]"},
    {22109, 100, "[SS MUST GAP 10]"},
    {22300, 1000, "[COFFEE GRIND]"},
    {22301, 500, "[FOR THE WHOLE ATRIUM]"},
    {22302, 500, "[RAIL COMBO]"},
    {22400, 100, "[BENCH-HOPPIN]"},
    {22401, 100, "[GET OFF OUR STATUE!]"},
    {22402, 100, "[YOULL HURT YOURSELF!!!]"},
    {22403, 100, "[PARKS CLOSED AFTER DARK, KID]"},
    {22404, 100, "[PARKING METER GAP]"},
    {22405, 100, "[CENTRAL PARK CRACK DEALER GAP]"},
    {22406, 100, "[GO ASK ALICE]"},
    {22407, 100, "[YOU LUCKY FUCKER]"},
    {22500, 500, "[HOLY SH#! - YOU MADE IT TO THE CORNER?]"},
    {22501, 500, "[VISITOR CENTER RAIL TRANSFER]"},
    {22502, 750, "[TELEPHONE CO. GAP]"},
    {22503, 750, "[WORLDS SECOND MOST OBVIOUS GAP]"},
    {22601, 1500, "[NEVERSOFT ELEC CO GAP]"},
    {23000, 2000, "[DOWN THE SPIRAL]"},
    {23001, 250, "[LOMBARD LEDGE]"},
    {23002, 500, "[HUBBA LEDGE]"},
    {23003, 750, "[HOOK RAIL]"},
    {23004, 500, "[RAIL 2 RAIL]"},
    {23005, 250, "[BACKWOODS LEDGE]"},
    {23006, 500, "[BENDY'S LIP]"},
    {23007, 3000, "[ARE YOU KIDDING?]"},
    {23100, 250, "[RAIL 2 LEDGE]"},
    {23101, 250, "[LEDGE 2 RAIL]"},
    {23102, 2000, "[LAMP STOMP]"},
    {23103, 250, "[LAMP 2 RAIL]"},
    {23104, 50, "[RAIL 2 RAIL]"},
    {23105, 2000, "[KNUCKLIN' FUTS!!!]"},
    {23106, 250, "[CROSSBAR 2 LAMP]"},
    {23107, 1000, "[DUMPSTER STOMP]"},
    {23108, 1000, "[KINK CLANK]"},
    {23109, 1000, "[BUMP 'N' GRIND]"},
    {23110, 1000, "[KINK STOMP]"},
    {23600, 500, "[BENCH TRIPPIN']"},
    {23601, 500, "[LEDGE 2 LEDGE]"},
    {24000, 2000, "[RADRAMP 2 SNAKERUN]"},
    {24001, 400, "[RAIL 2 RAIL]"},
    {24002, 250, "[RAIL 2 RAIL]"},
    {24003, 1000, "[UP 2 PIPE RAIL]"},
    {24004, 2000, "[LONGRAIL]"},
    {24005, 50, "[RAIL 2 RAIL]"},
    {24006, 300, "[RAIL 2 RAIL]"},
    {24007, 300, "[RAIL 2 RAIL]"},
    {24008, 75, "[RAIL 2 RAIL]"},
    {24009, 500, "[RAIL 2 RAIL]"},
    {24010, 1000, "[RAIL 2 RAIL 2 RAIL]"},
    {24011, 2000, "[OFF THE ROOF 2 RAIL]"},
    {24012, 4000, "[TIME 2 FEED THE VOLCANO!!!]"},
    {24013, 600, "[GAP 2 RAIL]"},
    {24014, 1000, "[OUT THE WINDOW 2 RAIL]"},
    {24015, 500, "[GAP 2 RAIL]"},
    {24016, 600, "[DOWN 2 RAIL]"},
    {24017, 2000, "[RAIL SERIES]"},
    {24018, 900, "[HALF RAIL SERIES]"},
    {24019, 900, "[HALF RAIL SERIES]"},
    {24020, 2000, "[TOP OF DA WORLD MA!!!]"},
    {24021, 2000, "[PLANK RAIL SERIES]"},
    {24022, 450, "[RAIL 2 RAIL]"},
    {24023, 2000, "[SWINGING THE SET]"},
    {24024, 2000, "[RAIL SERIES]"},
    {24025, 900, "[RAIL]"},
    {24026, 900, "[RAIL]"},
    {24027, 350, "[RAIL 2 RAIL]"},
    {24028, 200, "[RAIL 2 RAIL]"},
    {24029, 200, "[RAIL 2 RAIL]"},
    {24030, 300, "[RAIL 2 RAIL]"},
    {24031, 300, "[RAIL 2 RAIL]"},
    {24032, 2000, "[LONG ASS RAIL RUN]"},
    {24033, 700, "[CANT RAIL THE BIG ONE EH?]"},
    {24034, 1500, "[KICKER 2 RAIL]"},
    {24035, 1500, "[KICKER 2 RAIL]"},
    {24036, 1500, "[KICKER 2 RAIL 2 RAIL]"},
    {24037, 300, "[CHIN RAIL SERIES]"},
    {-1, 0, 0},
};

/** Restart and trigger-menu name tables populated while parsing TRG files. */
char *CheatRestarts[20] __attribute__((section(".data"))) = {0};
char *HorseRestarts[20] __attribute__((section(".data"))) = {0};
char *MenuFilenamePointers[40] __attribute__((section(".data"))) = {0};

/** Trigger-file state and restart counters initialized before parsing. */
int NumCheatRestarts __attribute__((section(".sdata"))) = 0;
int NumHorseRestarts __attribute__((section(".sdata"))) = 0;
unsigned short *TrigFile __attribute__((section(".sdata"))) = 0;
int RestartNode __attribute__((section(".sdata"))) = 0xFFFF;
short **OffsetList __attribute__((section(".sdata"))) = 0;
int NumNodes __attribute__((section(".sdata"))) = 0;
char *pCurrentObjFile __attribute__((section(".sdata"))) = 0;
int NumTrigMenuEntries __attribute__((section(".sdata"))) = 0;
static SCommandPoint *CommandPoints __attribute__((section(".sdata"))) = 0;
int TotalCommandPointMemory __attribute__((section(".sdata"))) = 0;

/** Polymorphic CItem prefix used to recover CBody's native virtual-call ABI. */
struct CItemKillView
{
    u8 opaque_before_next[28];
    CItemKillView *mNextItem;
    u8 opaque_after_next[40];

    virtual ~CItemKillView();
};

/** CBody layout slice and original virtual interface needed by KillInList. */
struct CBodyKillView : CItemKillView
{
    u8 opaque_before_node[106];
    u16 mNode;
    u16 mCollision;
    s16 mHealth;

    virtual void Die();
    virtual void AI();
    virtual void EveryFrame();
    virtual void DoLightingSetup();
    virtual int Hit(int, const CVector &, int);
    virtual void DeleteStuff();
};

typedef char Trig_CItemKillView_size_must_be_76[
    (sizeof(CItemKillView) == 76) ? 1 : -1];

// === Extern "C" absolute-addressed globals ===
// MAIN_STRUCTS identifies this as TRIG's file-local command-point hash table.
static SCommandPoint *HashTable[256]
    __attribute__((section(".bss")));

// === External function declarations ===
#include "MEM.h"
#include "FILEIO.h"
CVector operator*(const CVector &, const int &);
// Trig_SendPulse is defined later in this TU

// ExecuteCommandList extern functions
extern "C" void Camera_SetZoom(CCamera *, int, int)
    asm("SetZoom__7CCameraUsUs");
#include "FRONT.h"
#include <STDIO.H>
#include "DB.h"

// === Owner-local free function declarations ===
static void CreateCommandPoint(
    unsigned int checksum, unsigned short node, unsigned short *pCommands);
static SCommandPoint *GetCommandPoint(int Node);
static void KillInList(int Node, CBody *pList, int How);
static void SendVisibleFromNode(int node, int visible);
static void SendKillFromNode(int Node, int How);
static unsigned short *SkipCommand(unsigned short *pCommand);
static void ExecuteCommandList(
    unsigned short *pCommands, int Node, int WaitForSpooling);
// === Function implementations (address order) ===

/**
 * Trig_ClearTrigMenu__Fv
 *
 * Address:   0x80045704
 * Mangled:   Trig_ClearTrigMenu__Fv
 * Demangled: Trig_ClearTrigMenu(void)
 *
 */
void Trig_ClearTrigMenu(void)
{
    int i = 0;
    char **pFilename = MenuFilenamePointers;
    do
    {
        *pFilename = 0;
        i++;
        pFilename++;
    } while (i < 40);
    NumTrigMenuEntries = 0;
}

/**
 * CreateCommandPoint__FUiUsPUs
 *
 * Address:   0x80045730
 * Mangled:   CreateCommandPoint__FUiUsPUs
 * Demangled: CreateCommandPoint(unsigned int, unsigned short, unsigned short *)
 *
 * @param checksum  Asset hash for content lookup.
 * @param node  Rail / graph node identifier.
 * @param pCommands  Commands to operate on.
 */
static void CreateCommandPoint(unsigned int checksum, unsigned short node, unsigned short *pCommands)
{
    SCommandPoint *pNewCommandPoint =
        (SCommandPoint *)Mem_New(sizeof(SCommandPoint), 0, 1);

    pNewCommandPoint->pNext = CommandPoints;
    CommandPoints = pNewCommandPoint;
    TotalCommandPointMemory += sizeof(SCommandPoint) + sizeof(unsigned int);
    pNewCommandPoint->pNextSimilar = HashTable[checksum & 0xFF];
    HashTable[checksum & 0xFF] = pNewCommandPoint;
    pNewCommandPoint->Collision = 0;
    pNewCommandPoint->Executed = 0;
    pNewCommandPoint->NodeIndex = node;
    pNewCommandPoint->pCommands = pCommands;
    pNewCommandPoint->Checksum = checksum;
    pNewCommandPoint->PulsesReceived = 0;
    pNewCommandPoint->NumPulsesSet = 0;
    pNewCommandPoint->NumPulses = 0;
}

/**
 * Trig_DeleteCommandPoints__Fv
 *
 * Address:   0x800457D0
 * Mangled:   Trig_DeleteCommandPoints__Fv
 * Demangled: Trig_DeleteCommandPoints(void)
 *
 */
void Trig_DeleteCommandPoints(void)
{
    int i = 0;
    SCommandPoint **pHash = HashTable;
    do
    {
        *pHash = 0;
        i++;
        pHash++;
    } while (i < 256);

    SCommandPoint *pCP = CommandPoints;
    while (pCP != 0)
    {
        SCommandPoint *pNext = pCP->pNext;
        Mem_Delete(pCP);
        TotalCommandPointMemory -=
            sizeof(SCommandPoint) + sizeof(unsigned int);
        pCP = pNext;
    }
    CommandPoints = 0;
}

/**
 * GetCommandPoint__Fi — Return the live command-point entry for a trigger node.
 * Validates that OffsetList[Node] is a command-point record (type 6), then walks
 * the live CommandPoints chain and returns the entry whose NodeIndex matches
 * Node. Returns 0 if the node is invalid or no live command point exists.
 *
 * Address:   0x80045844
 * Mangled:   GetCommandPoint__Fi
 * Demangled: GetCommandPoint(int)
 *
 * @param Node  Rail / graph node identifier.
 * @return Matching live command point, or null when the node is invalid.
 */
static SCommandPoint *GetCommandPoint(int Node)
{
    if (Node != 0xFFFF)
    {
        if (*OffsetList[Node] == 6)
        {
            SCommandPoint *pCP = CommandPoints;
            while (pCP != 0)
            {
                if (pCP->NodeIndex == Node)
                    return pCP;
                pCP = pCP->pNext;
            }
        }
    }
    return 0;
}

/**
 * Trig_ResetCPCollisionFlags__Fi — Clear selected collision bits on all command points.
 *
 * Address:   0x800458B8
 * Mangled:   Trig_ResetCPCollisionFlags__Fi
 * Demangled: Trig_ResetCPCollisionFlags(int)
 *
 * @param Player  Player-bit mask to clear.
 */
void Trig_ResetCPCollisionFlags(int Player)
{
    SCommandPoint *pCP = CommandPoints;
    while (pCP != 0)
    {
        pCP->Collision &= ~Player;
        pCP = pCP->pNext;
    }
}

/**
 * Trig_ResetCPExecutedFlags__Fi — Clear executed bits once a player leaves a command point.
 *
 * Address:   0x800458F0
 * Mangled:   Trig_ResetCPExecutedFlags__Fi
 * Demangled: Trig_ResetCPExecutedFlags(int)
 *
 * @param Player  Player-bit mask whose completed state may be cleared.
 */
void Trig_ResetCPExecutedFlags(int Player)
{
    SCommandPoint *pCP = CommandPoints;
    while (pCP != 0)
    {
        u8 executed = pCP->Executed;
        if (executed & Player)
        {
            u8 collision = pCP->Collision;
            if (!(collision & Player))
            {
                pCP->Executed = executed & ~Player;
            }
        }
        pCP = pCP->pNext;
    }
}

/**
 * Trig_TriggerCommandPoint__FUib
 *
 * Address:   0x80045944
 * Mangled:   Trig_TriggerCommandPoint__FUib
 * Demangled: Trig_TriggerCommandPoint(unsigned int, bool)
 *
 * @param checksum  Asset hash for content lookup.
 * @param assert  Missing-trigger assertion request; unused by this prototype.
 * @return Triggered command point, or null when no eligible point is found.
 */
SCommandPoint *Trig_TriggerCommandPoint(unsigned int checksum, bool assert)
{
    SCommandPoint *pSearch = HashTable[checksum & 0xFF];
    SCommandPoint *result = 0;
    while (pSearch != 0)
    {
        if (pSearch->Checksum != checksum)
            goto next;
        {
            u8 collision = pSearch->Collision;
            u8 executed = pSearch->Executed;
            int Player = pCurrentPlayer->mIndex + 1;
            collision = collision | Player;
            pSearch->Collision = collision;
            if ((executed & Player) != 0)
                goto next;
            {
                unsigned short *pCommands = pSearch->pCommands;
                unsigned short nodeIdx = pSearch->NodeIndex;
                ExecuteCommandList(pCommands, nodeIdx, 0);
                u8 exec2 = pSearch->Executed;
                result = pSearch;
                exec2 = exec2 | Player;
                pSearch->Executed = exec2;
                return result;
            }
        }
    next:
        pSearch = pSearch->pNextSimilar;
        result = 0;
    }
    return result;
}

/**
 * Restore the restart node by matching a restart trigger name.
 *
 * Address:   0x800459FC
 * Mangled:   Trig_SetRestart__FPc
 * Demangled: Trig_SetRestart(char *)
 *
 * @param pName  Asset / object name string.
 */
void Trig_SetRestart(char *pName)
{
    int i = 0;
    RestartNode = 0xFFFF;
    if (NumNodes > 0)
    {
        do
        {
            short *pNode = OffsetList[i];
            if (*pNode == 8)
            {
                CVector Dummy;
                if (Utils_CompareStrings(
                        (char *)(Trig_GetPosition(Dummy, i) + 3), pName, -1))
                {
                    RestartNode = i;
                    return;
                }
            }
            i++;
        } while (i < NumNodes);
    }
}

/**
 * Recompute Bruce's facing vectors from the current heading.
 *
 * Address:   0x80045AA8
 * Mangled:   FudgeAngles__FP6CBruce
 * Demangled: FudgeAngles(CBruce *)
 *
 * @param pBruce  Active skater (CBruce) being processed.
 */
void FudgeAngles(CBruce *pBruce)
{
    CVector Temp1;
    CVector Temp2;
    pBruce->CSuper.CBody.CItem.mAngles.vz = 0;
    pBruce->CSuper.CBody.CItem.mAngles.vx = 0;
    pBruce->CSuper.CBody.CItem.mAngles.vy =
        ((unsigned short)pBruce->CSuper.CBody.CItem.mAngles.vy + 0x800) &
        0xFFF;
    Temp1.vx = -0x1000;
    Temp1.vy = 0;
    Temp1.vz = 0;
    Utils_RotateY(Temp2, Temp1,
                  pBruce->CSuper.CBody.CItem.mAngles.vy);
    pBruce->mRotationMatrix.m[0][0] = Temp2.vx;
    pBruce->mRotationMatrix.m[1][0] = Temp2.vy;
    pBruce->mRotationMatrix.m[2][0] = Temp2.vz;
    Temp1.vx = 0;
    Temp1.vy = 0;
    Temp1.vz = -0x1000;
    Utils_RotateY(Temp2, Temp1,
                  pBruce->CSuper.CBody.CItem.mAngles.vy);
    pBruce->mFront.vx = pBruce->mRotationMatrix.m[0][2] = Temp2.vx;
    pBruce->mFront.vy = pBruce->mRotationMatrix.m[1][2] = Temp2.vy;
    pBruce->mFront.vz = pBruce->mRotationMatrix.m[2][2] = Temp2.vz;
    pBruce->mUp.vx = pBruce->mRotationMatrix.m[0][1] = 0;
    pBruce->mUp.vy = pBruce->mRotationMatrix.m[1][1] = -0x1000;
    pBruce->mUp.vz = pBruce->mRotationMatrix.m[2][1] = 0;
}

/**
 * Move mechanics to the selected restart node and execute its command list.
 *
 * Address:   0x80045B9C
 * Mangled:   Trig_ExecuteRestart__Fv
 * Demangled: Trig_ExecuteRestart(void)
 *
 */
void Trig_ExecuteRestart(void)
{
    CVector StartPos;
    unsigned short *pAngles = Trig_GetPosition(StartPos, RestartNode);
    CBody *MechListCopy = MechList;

    while (MechListCopy != 0)
    {
        MechListCopy->CItem.mPos = StartPos;
        MechListCopy->mOldPos = StartPos;
        MechListCopy->CItem.mAngles = *(CSVector *)pAngles;
        FudgeAngles((CBruce *)MechListCopy);
        MechListCopy = (CBody *)MechListCopy->CItem.mNextItem;
    }

    // Skip name string after position data
    char *pName = (char *)(pAngles + 3);
    if (*(u8 *)pName != 0)
    {
        do
        {
            pName++;
        } while (*(u8 *)pName != 0);
    }
    pName++;
    unsigned short *cmds = (unsigned short *)((int)pName + ((int)pName & 1));
    ExecuteCommandList(cmds, RestartNode, 1);

    if (GNumberOfPlayers != 2)
        return;
    if (GGame == 7)
        return;

    // Rotation + position update
    CBruce *pMan = (CBruce *)MechList;
    CVector Dummy = pMan->mFront * 100;
    pMan->CSuper.CBody.CItem.mPos += Dummy;

    if (NumNodes <= 0)
        return;
    int i = 0;
    do
    {
        short *pNode = OffsetList[i];
        if (*pNode == 8 &&
            Utils_CompareStrings(
                (char *)(Trig_GetPosition(Dummy, i) + 3), "RE_2P", -1))
        {
            CVector StartPos;
            unsigned short *restartAngles = Trig_GetPosition(StartPos, i);
            pMan->CSuper.CBody.CItem.mPos = StartPos;
            pMan->CSuper.CBody.mOldPos = StartPos;
            pMan->CSuper.CBody.CItem.mAngles = *(CSVector *)restartAngles;
            FudgeAngles(pMan);
        }
        i++;
    } while (i < NumNodes);
}

/**
 * Trig_DeleteTrigFile__Fv
 *
 * Address:   0x80045DA8
 * Mangled:   Trig_DeleteTrigFile__Fv
 * Demangled: Trig_DeleteTrigFile(void)
 *
 */
void Trig_DeleteTrigFile(void)
{
    if (TrigFile != 0)
    {
        Mem_Delete((void *)TrigFile);
        TrigFile = 0;
    }
    NumCheatRestarts = 0;
    NumHorseRestarts = 0;
}

/**
 * Trig_LoadTRG__FPc
 *
 * Address:   0x80045DD8
 * Mangled:   Trig_LoadTRG__FPc
 * Demangled: Trig_LoadTRG(char *)
 *
 * @param Name  Base filename loaded with the `.trg` extension.
 */
void Trig_LoadTRG(char *Name)
{
    char *p = ".trg";
    char Buf[30];
    char *pFrom = Name;
    char *pTo = Buf;
    u8 ch = *(u8 *)pFrom;
    if (ch != 0)
    {
        do
        {
            pFrom++;
            *pTo = ch;
            ch = *(u8 *)pFrom;
            pTo++;
        } while (ch != 0);
    }
    u8 ech = *(u8 *)p;
    if (ech != 0)
    {
        do
        {
            p++;
            *pTo = ech;
            ech = *(u8 *)p;
            pTo++;
        } while (ech != 0);
    }
    *pTo = 0;
    TrigFile = (u16 *)Mem_New(FileIO_Open(Buf), 0, 1);
    FileIO_Load(TrigFile);
    FileIO_Sync();

    int *pTRG = (int *)TrigFile;
    unsigned short n = *((unsigned short *)pTRG + 4);
    pTRG += 3;
    OffsetList = (short **)pTRG;
    NumNodes = n;
    int i = 0;
    if (n != 0)
    {
        do
        {
            int val = *pTRG;
            int base = (int)TrigFile;
            i++;
            *pTRG = val + base;
            pTRG++;
        } while (i < NumNodes);
    }
    register char *pStart asm("$5") =
        (char *)OffsetList[NumNodes - 1];
    short lastType = *(short *)pStart;
    int v1Ff = 0xFF;
    if (lastType != v1Ff)
        return;
    int mask = -4;
    int base = (int)TrigFile;
    register int used asm("$16") = pStart - (char *)base;
    register int used5 asm("$3") = used + 5;
    asm("" : "+r"(used5));
    int newsize = used5 & mask;
    Mem_Shrink((void *)base, newsize);
    TrigFileSize = newsize;

}

/** Diagnostic messages retained by the original trigger parser. */
static char TrigFakeTRGMessage1[] __attribute__((section(".sdata"))) =
    "Trig_FakeTRG 1\n";
static char TrigFakeTRGMessage2[] __attribute__((section(".sdata"))) =
    "Trig_FakeTRG 2\n";

/** Scratch allocation used while commands wait for asynchronous spooling. */
static unsigned int *pPSXDualBuffer __attribute__((section(".sdata"))) = 0;

/** Default inline command list attached to command-point nodes. */
static unsigned short CrateCommands[2] __attribute__((section(".sdata"))) = {
    3, 0xFFFF
};

/** Object-file regions and the compacted trigger-file size. */
unsigned char CurrentObjFileRegion __attribute__((section(".sdata"))) = 0;
unsigned char CurrentItemsFileRegion __attribute__((section(".sdata"))) = 0;
unsigned char CurrentMedalsFileRegion __attribute__((section(".sdata"))) = 0;

/**
 * GetFlag__FUcPUc
 *
 * Address:   0x80045F0C
 * Mangled:   GetFlag__FUcPUc
 * Demangled: GetFlag(unsigned char, unsigned char *)
 *
 * @param flag  Flag identifier to search for.
 * @param pFlags  Flags to operate on.
 * @return 1 when the identifier is present; otherwise 0.
 */
unsigned char GetFlag(unsigned char flag, unsigned char *pFlags)
{
    while (*pFlags != 0xFF)
    {
        if (*pFlags == flag)
        {
            return 1;
        }
        pFlags++;
    }
    return 0;
}

/**
 * SkipFlags__FPUc
 *
 * Address:   0x80045F44
 * Mangled:   SkipFlags__FPUc
 * Demangled: SkipFlags(unsigned char *)
 *
 * @param pFlags  Flags to operate on.
 * @return First byte following the 0xFF flag-list terminator.
 */
char *SkipFlags(unsigned char *pFlags)
{
    while (*pFlags != 0xFF)
        pFlags++;
    return (char *)(pFlags + 1);
}

/**
 * Trig_CreateObject__Fi
 *
 * Address:   0x80045F74
 * Mangled:   Trig_CreateObject__Fi
 * Demangled: Trig_CreateObject(int)
 *
 * @param NodeIndex  Rail node index.
 * @return Newly created body, or null for unsupported or failed creation.
 */
CBody *Trig_CreateObject(int NodeIndex)
{
    int ManualSuspendControl = 0;
    int Seedable = 0;
    CBody *NewObject = 0;
    short *pInfo = OffsetList[NodeIndex];
    int NodeType = *pInfo;
    pInfo++;
    int NotSuspendedAtStart;
    int Type;
    CVector Pos;
    unsigned short *pStuff;
    int LifeTime;

    if (NodeType == 5)
        goto case5;
    if (NodeType < 6)
    {
        if (NodeType != 1)
            goto done;
        // NodeType == 1 (Seedable stays 0)
    }
    else
    {
        if (NodeType != 7)
            goto done;
        Seedable = 1;
    }
    // Shared body for NodeType 1 and 7
    {
        Type = *pInfo;
        pInfo += 2;
        short linkCount = *pInfo;
        pInfo += linkCount + 1;
        NotSuspendedAtStart = GetFlag(2, (unsigned char *)pInfo);
        ManualSuspendControl = GetFlag(4, (unsigned char *)pInfo);
        pInfo = (short *)(((int)SkipFlags((unsigned char *)pInfo) + 3) & ~3);

        if (Type < 0xDB)
        {
            if (Type < 0xD5)
            {
                if (Type == 0xCB)
                    goto makeScriptOnly;
                goto done;
            }
            goto makeCCar;    // 0xD5-0xDA range
        }
        if (Type == 0x192)
            goto makeCPlatform;
        goto done;
    }

makeScriptOnly:
    NewObject = (CBody *)CItem::operator new(sizeof(CScriptOnlyBaddy));
    if (NewObject == 0)
        goto done_null;
    NewObject = (CBody *)::new ((CScriptOnlyBaddy *)NewObject)
        CScriptOnlyBaddy(pInfo, NodeIndex);
    goto done;
makeCPlatform:
    NewObject = (CBody *)CItem::operator new(sizeof(CPlatform));
    if (NewObject == 0)
        goto done_null;
    NewObject = (CBody *)::new ((CPlatform *)NewObject)
        CPlatform(pInfo, NodeIndex);
    goto done;
makeCCar:
    NewObject = (CBody *)CItem::operator new(sizeof(CCar));
    if (NewObject == 0)
        goto done_null;
    NewObject = (CBody *)::new ((CCar *)NewObject)
        CCar(pInfo, NodeIndex, Type);
    goto done;

case5:
    {
        Type = *pInfo;
        pStuff = Trig_GetPosition(Pos, NodeIndex);
        pStuff++;
        // Required to retain the target's source-order argument scheduling.
        asm volatile("");
        unsigned int flags = pStuff[0];
        LifeTime = pStuff[1];
        // CPowerUp's target-proven CBody prefix begins at offset zero.
        NewObject = reinterpret_cast<CBody *>(PowerUp_Create(
            Type & 0xFFFF, Pos, (flags == 0) << 2,
            LifeTime, ZeroVector));
        int links = Trig_GetLinksPointer(NodeIndex);
        unsigned short linkCount = *(unsigned short *)links;
        if (linkCount != 0)
        {
            reinterpret_cast<CPowerUp *>(NewObject)->SetNode(NodeIndex);
        }
        NotSuspendedAtStart = 1;
    }
    goto done;

done:
    if (NewObject == 0)
        return 0;
    else
    {
        NewObject->mNode = NodeIndex;
        if (ManualSuspendControl != 0)
        {
            NewObject->mCBodyFlags &= ~2;
        }
        if (Seedable != 0)
        {
            NewObject->mCBodyFlags |= 4;
        }
        if (NotSuspendedAtStart == 0)
        {
            // Preserve the list that owned the object before suspending it.
            CBody **ppList = 0;
            if (NewObject == BaddyList)
                ppList = &BaddyList;
            if (NewObject == EnvironmentalObjectList)
                ppList = &EnvironmentalObjectList;
            if (NewObject == PowerUpList)
                ppList = &PowerUpList;
            NewObject->Suspend(ppList);
        }
    }
    return NewObject;
done_null:
    return 0;
}

/**
 * Trig_SendPulseToNode__Fi
 *
 * Address:   0x8004622C
 * Mangled:   Trig_SendPulseToNode__Fi
 * Demangled: Trig_SendPulseToNode(int)
 *
 * @param NodeIndex  Rail node index.
 */
void Trig_SendPulseToNode(int NodeIndex)
{
    short *pNode = OffsetList[NodeIndex];
    int nodeType = *pNode;
    CVector Pos;

    if (nodeType < 0xc)
    {
        if (nodeType >= 0xa)
            return;
        if (nodeType == 5)
            goto createObj;
        if (nodeType < 6)
        {
            if (nodeType == 1)
                goto createObj;
            return;
        }
        if (nodeType == 6)
            goto cmdPoint;
        if (nodeType == 7)
            goto createObj;
        return;
    }
    if (nodeType == 0xe)
        goto trickOb;
    if (nodeType < 0xf)
    {
        if (nodeType == 0xc)
            goto trickOb;
        return;
    }
    if (nodeType == 0x1F5)
        goto light;
    return;

createObj: // case 1, 5, 7
    Trig_CreateObject(NodeIndex);
    return;
cmdPoint: // case 6
    {
        SCommandPoint *pCommandPoint = GetCommandPoint(NodeIndex);
        unsigned short *pCommands = pCommandPoint->pCommands;
        u8 pulseCount = pCommandPoint->PulsesReceived;
        pulseCount++;
        pCommandPoint->PulsesReceived = pulseCount;
        ExecuteCommandList(pCommands, NodeIndex, 0);
    }
    return;
light: // case 0x1F5
    {
        STriggerLightWords *pStuff =
            (STriggerLightWords *)Trig_GetPosition(Pos, NodeIndex);
        UseCacheForBits = 0;
        CLight *lightObj = static_cast<CLight *>(
            Bit_AllocateStorage(TRIG_LIGHT_STORAGE_SIZE));
        if (lightObj != 0)
        {
            ::new (lightObj) CLight(
                Pos, NodeIndex,
                pStuff->ClipDist, pStuff->InnerRadius,
                pStuff->FringeRadius, pStuff->Amp,
                pStuff->r0, pStuff->g0, pStuff->b0,
                pStuff->r1, pStuff->g1, pStuff->b1);
        }
        UseCacheForBits = 1;
    }
    return;
trickOb: // case 0xc, 0xe
    TrickOb_DarkenNode(NodeIndex);
}

/**
 * Trig_SendPulse__FPUs
 *
 * Address:   0x800463E8
 * Mangled:   Trig_SendPulse__FPUs
 * Demangled: Trig_SendPulse(unsigned short *)
 *            Dispatch each linked trigger node stored after the pulse-count header.
 *
 * @param pLinkInfo  LinkInfo to operate on.
 */
void Trig_SendPulse(unsigned short *pLinkInfo)
{
    unsigned short *pLink = pLinkInfo + 1;
    unsigned short NumLinks = *pLinkInfo;
    int i = 0;

    if (NumLinks != 0)
    {
        do
        {
            i = i + 1;
            Trig_SendPulseToNode(*pLink);
            pLink++;
        } while (i < NumLinks);
    }
}

/**
 * KillInList__FiP5CBodyi
 *
 * Address:   0x80046444
 * Mangled:   KillInList__FiP5CBodyi
 * Demangled: KillInList(int, CBody *, int)
 *            Walk a body list and invoke the selected virtual kill path for
 *            every body associated with the requested node.
 *
 * @param Node  Rail / graph node identifier.
 * @param pList  List to operate on.
 * @param How  Zero invokes `Die`; one applies a full-health `Hit` with no knockback.
 */
static void KillInList(int Node, CBody *pList, int How)
{
    CBodyKillView *body = (CBodyKillView *)pList;
    if (body == 0)
        return;
    do
    {
        if (body->mNode == (unsigned int)Node)
        {
            switch (How)
            {
            case 0:
                body->Die();
                break;
            case 1:
                body->Hit(
                    body->mHealth,
                    ZeroVector,
                    0);
                break;
            }
        }
        body = (CBodyKillView *)body->mNextItem;
    } while (body != 0);
}

/**
 * SendVisibleFromNode__Fii
 *
 * Address:   0x80046504
 * Mangled:   SendVisibleFromNode__Fii
 * Demangled: SendVisibleFromNode(int, int)
 *
 * @param node  Rail / graph node identifier.
 * @param visible  Nonzero to show linked objects; zero to hide them.
 */
static void SendVisibleFromNode(int node, int visible)
{
    int SetFlags;
    int ClearFlags;
    unsigned short *pNodes;
    unsigned short NumLinks;

    // Determine visibility mask based on player
    CBruce *pPlayer = pCurrentPlayer;
    if (pPlayer == 0 || GNumberOfPlayers == 1)
    {
        SetFlags = 0x41;
    }
    else
    {
        SetFlags = 1;
        if ((CBody *)pPlayer == MechList)
            SetFlags = 0x40;
    }
    ClearFlags = ~SetFlags;
    if (visible != 0)
        SetFlags = 0;
    else
        ClearFlags = -1;

    {
        int i;

        pNodes = Trig_GetLinksPointer(node);
        NumLinks = *pNodes;
        pNodes++;

        for (i = 0; i < NumLinks; i++)
        {
            unsigned short VisNode = *pNodes;
            short *pVisNode = OffsetList[VisNode];
            pNodes++;

            switch (*pVisNode)
            {
                CPowerUp *pUp;
                CBit *pBit;

            case 5:
                // PowerUp — iterate PowerUpList, match on node.
                pUp = (CPowerUp *)PowerUpList;
                for (; pUp != 0;
                     pUp = (CPowerUp *)pUp->CBody.CItem.mNextItem)
                {
                    if (pUp->CBody.mNode == VisNode)
                    {
                        unsigned short flags = pUp->CBody.CItem.mFlags;
                        pUp->CBody.CItem.mFlags =
                            (flags & ClearFlags) | SetFlags;
                        break;
                    }
                }
                break;

            case 0x1F5:
                // NonRenderedBitList.
                pBit = (CBit *)NonRenderedBitList;
                for (; pBit != 0; pBit = pBit->mNext)
                {
                    if (pBit->mType == 2 && pBit->mLifetime == VisNode)
                    {
                        if (visible != 0)
                            ((TrigLightPrefix *)pBit)->mOn = 1;
                        else if (GNumberOfPlayers == 1)
                            ((TrigLightPrefix *)pBit)->mOn = 0;
                        break;
                    }
                }
                break;

            case 2:
            case 9:
            case 14:
            case 12:
                pVisNode++;
                pVisNode += *pVisNode + 1;
                if ((int)pVisNode & 2)
                    pVisNode++;
                {
                    Item *pItem = Spool_FindEnviroItem(*(int *)pVisNode);
                    if (pItem != 0)
                    {
                        unsigned short flags = pItem->Flags;
                        pItem->Flags = (flags & ClearFlags) | SetFlags;
                    }
                }
                break;
            }
        }
    }
}

/**
 * SendKillFromNode__Fii
 *
 * Address:   0x8004675C
 * Mangled:   SendKillFromNode__Fii
 * Demangled: SendKillFromNode(int, int)
 *
 * @param Node  Rail / graph node identifier.
 * @param How  Kill mode: zero destroys linked objects; one applies hit behavior.
 */
static void SendKillFromNode(int Node, int How)
{
    unsigned short *pNodes = Trig_GetLinksPointer(Node);
    unsigned short NumLinks = *pNodes;
    pNodes++;
    int Bounced;

    {
        for (int i = 0; i < NumLinks; i++)
        {
            unsigned short KillNode;
            short *pKillNode;
            CBody *pList;

            KillNode = *pNodes++;
            pKillNode = OffsetList[KillNode];
            Bounced = 0;

            switch (*pKillNode)
            {
            case 1:
                goto body_kill;
            case 2:
            case 9:
                goto enviro_kill;
            case 10:
            case 11:
                goto rail_kill;
            case 12:
            case 14:
                goto next_link;
            case 0x1F4:
            case 0x1F5:
                goto bit_kill;
            default:
                goto next_link;
            }

        body_kill:
        {
            // Kill in EnvironmentalObjectList
            pList = EnvironmentalObjectList;
            if (pList == 0)
                goto after_enviro;
            do
            {
                CPlatform *pPlatform = (CPlatform *)pList;
                unsigned short bodyNode = pList->mNode;
                if (bodyNode != KillNode)
                    goto next_body;
                {
                    unsigned short status = pList->CItem.mFlags;
                    if (!(status & 0x100))
                        goto next_body;
                    int flags2 = pPlatform->CBaddy.mSpecificFlags;
                    if (!(flags2 & 0x10))
                        goto next_body;
                    pList->CItem.mFlags = status | 0x20;
                    if (flags2 & 0x20)
                        goto next_body;
                    // Kill: send pulse, set kill flag, call Bounce
                    unsigned short bn = pList->mNode;
                    int lp = Trig_GetLinksPointer(bn);
                    Trig_SendPulse((unsigned short *)lp);
                    pPlatform->CBaddy.mDeathPulseSent = 1;
                    pPlatform->Bounce(
                        pCurrentPlayer,
                        pCurrentPlayer->CSuper.CBody.mVel);
                }
                Bounced = 1;
            next_body:
                pList = (CBody *)pList->CItem.mNextItem;
            } while (pList != 0);
        after_enviro:
            if (Bounced != 0)
                goto next_link;
            KillInList(KillNode, BaddyList, How);
            KillInList(KillNode, EnvironmentalObjectList, How);
        }
        goto next_link;

    enviro_kill:
        {
            Item *pItem;

            pKillNode++;
            pKillNode += *pKillNode + 1;
            if ((int)pKillNode & 2)
                pKillNode++;
            pItem = Spool_FindEnviroItem(*(int *)pKillNode);
            if (pItem == 0)
                goto next_link;
            if (How == 1)
            {
                Exp_HitEnvItem(pItem, 0, 0xFFFF);
                goto next_link;
            }
            pItem->Flags |= 0x41;
            goto next_link;
        }

    bit_kill:
        {
            CBit *pBit = (CBit *)NonRenderedBitList;
            if (pBit == 0)
                goto next_link;
            do
            {
                if (pBit->mType == 2 && pBit->mLifetime == KillNode)
                {
                    pBit->Die();
                    goto next_link;
                }
                pBit = pBit->mNext;
            } while (pBit != 0);
            goto next_link;
        }

    rail_kill:
        Rail_KillRail(KillNode);

    next_link:
        ;
        }
    }
}

/**
 * SendSignalToNode__FP5CBodyi
 *
 * Address:   0x80046A08
 * Mangled:   SendSignalToNode__FP5CBodyi
 * Demangled: SendSignalToNode(CBody *, int)
 *
 * @param pBody  Body to operate on.
 * @param NodeIndex  Rail node index.
 */
void SendSignalToNode(CBody *pBody, int NodeIndex)
{
    while (pBody != 0)
    {
        if (pBody->mNode == NodeIndex)
        {
            pBody->mInputFlags |= 1;
        }
        pBody = (CBody *)pBody->CItem.mNextItem;
    }
}

/**
 * SendSignalToLinks__FPUs
 *
 * Address:   0x80046A48
 * Mangled:   SendSignalToLinks__FPUs
 * Demangled: SendSignalToLinks(unsigned short *)
 *
 * @param pLinkInfo  LinkInfo to operate on.
 */
void SendSignalToLinks(unsigned short *pLinkInfo)
{
    unsigned short *pLink = pLinkInfo + 1;
    unsigned short NumLinks = *pLinkInfo;
    int i = 0;
    if (NumLinks != 0)
    {
        do
        {
            unsigned short NodeIndex = *pLink;
            short *pNode = OffsetList[NodeIndex];
            if (*pNode == 1 || *pNode == 7)
            {
                SendSignalToNode(BaddyList, NodeIndex);
                SendSignalToNode(EnvironmentalObjectList, NodeIndex);
            }
            i++;
            pLink++;
        } while (i < NumLinks);
    }
}

/**
 * SendSuspend__FPP5CBodyi
 *
 * Address:   0x80046B08
 * Mangled:   SendSuspend__FPP5CBodyi
 * Demangled: SendSuspend(CBody **, int)
 *
 * @param ppList  Address of the body-list head to scan and update.
 * @param NodeIndex  Rail node index.
 */
void SendSuspend(CBody **ppList, int NodeIndex)
{
    CBody *pList = *ppList;
    while (pList != 0)
    {
        if (pList->mNode == NodeIndex)
        {
            pList->Suspend(ppList);
        }
        pList = (CBody *)pList->CItem.mNextItem;
    }
}

/**
 * SendUnSuspend__Fi
 *
 * Address:   0x80046B70
 * Mangled:   SendUnSuspend__Fi
 * Demangled: SendUnSuspend(int)
 *
 * @param NodeIndex  Rail node index.
 */
void SendUnSuspend(int NodeIndex)
{
    CBody *pList = SuspendedList;
    while (pList != 0)
    {
        if (pList->mNode == NodeIndex)
        {
            pList->UnSuspend();
        }
        pList = (CBody *)pList->CItem.mNextItem;
    }
}

/**
 * SendSuspendOrActivate__FPUsi
 *
 * Address:   0x80046BCC
 * Mangled:   SendSuspendOrActivate__FPUsi
 * Demangled: SendSuspendOrActivate(unsigned short *, int)
 *
 * @param pLinkInfo  LinkInfo to operate on.
 * @param signaltype  Signal 5 suspends linked bodies; other values resume them.
 */
void SendSuspendOrActivate(unsigned short *pLinkInfo, int signaltype)
{
    unsigned short *pLink = pLinkInfo + 1;
    int i = 0;
    unsigned short NumLinks = *pLinkInfo;
    if (NumLinks != 0)
    {
        do
        {
            unsigned short NodeIndex = *pLink;
            short *pNode = OffsetList[NodeIndex];
            if (*pNode == 1 || *pNode == 7)
            {
                if (signaltype == 5)
                {
                    SendSuspend(&BaddyList, NodeIndex);
                    SendSuspend(&EnvironmentalObjectList, NodeIndex);
                }
                else
                {
                    SendUnSuspend(NodeIndex);
                }
            }
            i++;
            pLink++;
        } while (i < NumLinks);
    }
}

/**
 * SkipString__FPc
 *
 * Address:   0x80046CBC
 * Mangled:   SkipString__FPc
 * Demangled: SkipString(char *)
 *
 * @param pText  Text to operate on.
 * @return Halfword-aligned pointer immediately after the terminated string.
 */
unsigned short *SkipString(char *pText)
{
    if (*(u8 *)pText != 0)
    {
        do
        {
            pText++;
        } while (*(u8 *)pText != 0);
    }
    pText++;
    return (unsigned short *)((int)pText + ((int)pText & 1));
}

/**
 * SkipCommand__FPUs
 *
 * Address:   0x80046CF4
 * Mangled:   SkipCommand__FPUs
 * Demangled: SkipCommand(unsigned short *)
 *
 * @param pCommand  Command to operate on.
 * @return Pointer to the next command, or null for the end marker.
 */
static unsigned short *SkipCommand(unsigned short *pCommand)
{
    switch (*pCommand++)
    {
    case 0xFFFF:
        return 0;

    case 0x82:
    case 0x87:
    case 0x8B:
    case 0x8F:
    case 0x90:
    case 0x91:
    case 0x92:
    case 0xA7:
    case 0xAE:
    case 0xC8:
    case 0xCA:
        pCommand += 2;
        break;

    case 0x68:
        pCommand += 3;
        break;

    case 0x73:
    case 0x77:
    case 0x7E:
    case 0x7F:
    case 0x80:
    case 0x8C:
    case 0x8E:
    case 0xB0:
    case 0xB2:
        pCommand = SkipString((char *)pCommand);
        break;

    case 2:
    {
        if (*(u8 *)pCommand != 0)
        {
            do
            {
                pCommand = SkipString((char *)pCommand);
            } while (*(u8 *)pCommand != 0);
        }
        pCommand++;
        break;
    }

    case 0xAB:
    {
        unsigned int *p_checksum =
            (unsigned int *)(((int)pCommand + 3) & ~3);
        pCommand = (unsigned short *)
            ((char *)p_checksum + sizeof(SBackgroundCommandPayload));
        break;
    }

    case 0xC9:
    {
        unsigned int *p_checksum =
            (unsigned int *)(((int)pCommand + 3) & ~3);
        pCommand = (unsigned short *)((char *)p_checksum + 6);
        break;
    }

    case 0x8D:
        pCommand += 2;
        // Fall through: both formats end in the same aligned entry list.
    case 0x85:
    {
        int *p_pos;
        while (*pCommand != 0xFF)
        {
            p_pos = (int *)((int)pCommand + 3);
            p_pos = (int *)((int)p_pos & ~3);
            p_pos += 6;
            pCommand = (unsigned short *)p_pos;
        }
        pCommand++;
        break;
    }

    case 0x8A:
    case 0x0D:
    case 0x69:
    case 0x6A:
    case 0x83:
    case 0x84:
    case 0x86:
    case 0x93:
    case 0x94:
    case 0x99:
    case 0x9A:
    case 0x9B:
    case 0x9C:
    case 0xA0:
    case 0xA3:
    case 0xA4:
    case 0xA5:
    case 0xA6:
    case 0xA8:
    case 0xA9:
    case 0xAA:
    case 0xAC:
    case 0xB1:
        pCommand++;
        break;

    // Commands without inline payload data already point at the next command.
    case 3:
    case 4:
    case 5:
    case 0x0A:
    case 0x0B:
    case 0x0C:
    case 0x66:
    case 0x67:
    case 0x79:
    case 0x7A:
    case 0x81:
    case 0x88:
    case 0x89:
    case 0xAD:
    case 0xAF:
        break;

    // End-if already points at the following command.
    case 0x95:
    default:
        return pCommand;
    }

    return pCommand;
}

/**
 * Trig_AllocateDualBuffer__FUi
 *
 * Address:   0x8004701C
 * Mangled:   Trig_AllocateDualBuffer__FUi
 * Demangled: Trig_AllocateDualBuffer(unsigned int)
 *
 * @param Size Requested scratch-buffer size; intentionally unused here.
 */
void Trig_AllocateDualBuffer(unsigned int Size)
{
}

/**
 * Trig_RemoveDualBuffer__Fv
 *
 * Address:   0x80047024
 * Mangled:   Trig_RemoveDualBuffer__Fv
 * Demangled: Trig_RemoveDualBuffer(void)
 *
 */
void Trig_RemoveDualBuffer(void)
{
    if (pPSXDualBuffer != 0)
    {
        Mem_Delete((void *)pPSXDualBuffer);
    }
    pPSXDualBuffer = 0;
}

/**
 * ExecuteCommandList__FPUsii
 *
 * Address:   0x8004704C
 * Mangled:   ExecuteCommandList__FPUsii
 * Demangled: ExecuteCommandList(unsigned short *, int, int)
 * Match:     PERFECT (828/828)
 *
 * Interprets one node's trigger-command stream, dispatching gameplay,
 * presentation, audio, camera, and level-state operations until its end marker.
 *
 * @param pCommands  Commands to operate on.
 * @param Node  Rail / graph node identifier.
 * @param WaitForSpooling  Nonzero to block after commands that begin asset spooling.
 */
static void ExecuteCommandList(unsigned short *pCommands, int Node, int WaitForSpooling)
{
    int NewFoggingParameters = 0;
    int ZHither, ZDpqMin;
    unsigned int Range;
    SCommandPoint *pCommandPoint = GetCommandPoint(Node);

    /* Read-only traces retained from the disabled fake-TRG diagnostic path. */
    static const char fakeTrgEnter[] asm("s_Enter_Trig_FakeTRG_800B2510") =
        "\nEnter Trig_FakeTRG\n";
    static const char fakeTrgExit[] asm("s_Exit_Trig_FakeTRG_800B2528") =
        "Exit Trig_FakeTRG\n";

    while (*pCommands != 0xFFFF)
    {
        unsigned short Command = *pCommands;
        pCommands++;
        if ((unsigned int)(Command - 2) >= 0xC9)
        {
            goto next_cmd;
        }

        switch (Command)
        {
        case 0x02:
        {
            // Define cheat restart strings
            NumCheatRestarts = 0;
            if (*(unsigned char *)pCommands == 0)
                goto case_02_done;
            do
            {
                CheatRestarts[NumCheatRestarts++] = (char *)pCommands;
                pCommands = (unsigned short *)SkipString((char *)pCommands);
            } while (*(unsigned char *)pCommands != 0);
        case_02_done:
            pCommands++;
            break;
        }

        case 0x97:
        {
            // Allocate dual buffer
            Trig_RemoveDualBuffer();
            unsigned short val = *pCommands;
            pCommands++;
            unsigned int size = ((((val << 5) - val) << 2) + val) << 3;
            Trig_AllocateDualBuffer(size);
            break;
        }

        case 0x8C:
        case 0xB0:
        {
            // Set restart
            int OldRestart = RestartNode;
            Trig_SetRestart(GGame == 7 ? HorseRestarts[GHorseRestart]
                                       : (char *)pCommands);
            pCommands = (unsigned short *)SkipString((char *)pCommands);
            if (OldRestart == 0xFFFF)
                break;
            if (RestartNode == OldRestart)
                break;
            if (Command != 0xB0)
            {
                Mess_DeleteAll();
            }
            Front_SaveGameState();
            break;
        }

        case 0xB2:
        {
            // Set restart (2-player only)
            if (GNumberOfPlayers == 2)
            {
                Trig_SetRestart(GGame == 7 ? HorseRestarts[GHorseRestart]
                                           : (char *)pCommands);
            }
            goto skip_string;
        }

        case 0x7E:
            Spool_PSX((char *)pCommands, 0, 0, 1);
            if (WaitForSpooling)
            {
                Spool_Sync();
            }
            pCommands = (unsigned short *)SkipString((char *)pCommands);
            break;

        case 0x7F:
        {
            // Spool clear PSX
            Spool_ClearPSX((char *)pCommands);
            pCommands = (unsigned short *)SkipString((char *)pCommands);
            break;
        }

        case 0x80:
            Spool_PSX((char *)pCommands, 1, 0, 1);
            if (WaitForSpooling)
            {
                Spool_Sync();
            }
            pCommands = (unsigned short *)SkipString((char *)pCommands);
            break;

        case 0x81:
        {
            // Spool sync
            Spool_Sync();
            break;
        }

        case 0x9F:
        {
            // SFX spool MIDI
            SFX_SpoolInMIDI((char *)pCommands);
            goto skip_string;
        }

        case 0x8E:
        {
            // Store obj file + find region
            pCurrentObjFile = (char *)pCommands;
            pCommands = (unsigned short *)SkipString((char *)pCommands);
            CurrentObjFileRegion = Spool_FindRegion(pCurrentObjFile);
            break;
        }

        case 0x85:
        {
            CVector min, max;
            while (*pCommands != 0xFF)
            {
                int *p_pos = (int *)(((int)pCommands + 3) & ~3);
                min.vx = *p_pos++ << 12;
                min.vy = *p_pos++ << 12;
                min.vz = *p_pos++ << 12;
                max.vx = *p_pos++ << 12;
                max.vy = *p_pos++ << 12;
                max.vz = *p_pos++ << 12;
                Utils_KillEverythingInBox(min, max);
                pCommands = (unsigned short *)p_pos;
            }
            pCommands++;
            break;
        }

        case 0x8A:
            pCommands++;
            break;

        case 0x8D:
        {
            // Set visibility in box loop
            unsigned short on = *pCommands++;
            unsigned short in = *pCommands++;
            CVector min, max;
            while (*pCommands != 0xFF)
            {
                int *p_pos = (int *)(((int)pCommands + 3) & ~3);
                min.vx = *p_pos++ << 12;
                min.vy = *p_pos++ << 12;
                min.vz = *p_pos++ << 12;
                max.vx = *p_pos++ << 12;
                max.vy = *p_pos++ << 12;
                max.vz = *p_pos++ << 12;
                Utils_SetVisibilityInBox(min, max, on, in);
                pCommands = (unsigned short *)p_pos;
            }
            pCommands++;
            break;
        }

        case 0xAB:
        {
            // Create background
            unsigned int *p_checksum =
                (unsigned int *)(((int)pCommands + 3) & ~3);
            unsigned int checksum = *p_checksum++;
            pCommands = (unsigned short *)p_checksum;
            CSVector ang_vel;
            ang_vel.vx = *pCommands++;
            ang_vel.vy = *pCommands++;
            ang_vel.vz = *pCommands++;
            CBackground *pBackground =
                new CBackground(checksum, ang_vel);
            if (pBackground == 0)
                break;
            break;
        }

        case 0x83:
        {
            // Backgrnd_On
            Backgrnd_On(*pCommands);
            pCommands++;
            break;
        }

        case 0x84:
        {
            // Backgrnd_Off
            Backgrnd_Off(*pCommands);
            pCommands++;
            break;
        }

        case 0x68:
        {
            // Set fog params (deferred)
            ZHither = *pCommands++;
            NewFoggingParameters = 1;
            ZDpqMin = *pCommands++;
            Range = *pCommands++;
            break;
        }

        case 0xC8:
        {
            // Set fade colour
            unsigned int fadecolor = *pCommands++ << 16;
            fadecolor += *pCommands++;
            M3d_FadeColour = fadecolor;
            break;
        }

        case 0xCA:
        {
            // Set sky color + update
            unsigned int skycolor = *pCommands++ << 16;
            skycolor += *pCommands++;
            Db_SkyColor = skycolor;
            Db_UpdateSky();
            break;
        }

        case 0xC9:
        {
            // Gap trick assignment
            unsigned int *pChecksum = (unsigned int *)(((int)pCommands + 3) & ~3);
            unsigned int Checksum = *pChecksum++;
            pCommands = (unsigned short *)pChecksum;
            int GapType = *pCommands++;
            if (GVInfo1.VideoMode == 2)
                break;

            int i;
            for (i = 0; i < pCurrentPlayer->mGaps; i++)
            {
                if (pCurrentPlayer->mGapChecksum[i] != 0)
                {
                    if (pCurrentPlayer->mGapChecksum[i] ==
                        pCommandPoint->Checksum)
                    {
                        pCurrentPlayer->mGapChecksum[i] = 0;
                        if (pCurrentPlayer->mInBail == 0)
                        {
                            SGapTrick *pGapTrick = GapTricks;
                            while (pGapTrick->Type != -1 &&
                                   pGapTrick->Type !=
                                       pCurrentPlayer->mGapType[i])
                            {
                                pGapTrick++;
                            }

                            if (pGapTrick->Type != -1)
                            {
                                if (pGapTrick->Type < 20000)
                                {
                                    pCurrentPlayer->Trick_Trick(
                                        pGapTrick->Name,
                                        -pGapTrick->Score,
                                        0);
                                    SFX_PlayX(0xA1, 0x2000, 0);
                                }
                                else
                                {
                                    pCurrentPlayer->mpGrindGapTrick =
                                        pGapTrick;
                                }
                            }
                        }
                    }
                }
            }

            if (Checksum == 0)
                break;
            if (pCurrentPlayer->mGaps < 10)
            {
                pCurrentPlayer->mGapChecksum[pCurrentPlayer->mGaps] =
                    Checksum;
                pCurrentPlayer->mGapType[pCurrentPlayer->mGaps] = GapType;
                pCurrentPlayer->mGaps++;
            }
            else
            {
                // The ten pending gap slots are full; report their contents.
                i = 0;
                do
                {
                    printf("Checksum %lx, type %d\n",
                           pCurrentPlayer->mGapChecksum[i],
                           pCurrentPlayer->mGapType[i]);
                    i++;
                } while (i < 10);
            }
            break;
        }

        case 0x69:
        {
            // SFX_PlayX
            short sfxId = *(short *)pCommands;
            pCommands++;
            SFX_PlayX(sfxId, 0x2000, 0);
            break;
        }

        case 0x6A:
        {
            // SFX_Stop
            short sfxId = *(short *)pCommands;
            SFX_Stop(sfxId);
            pCommands++;
            break;
        }

        case 0x96:
        {
            // SFX MIDI channel mute
            SFX_MIDIChannelMute(*pCommands);
            pCommands++;
            break;
        }

        case 0x9B:
        {
            // SFX MIDI fade in
            SFX_MIDIFadeIn(*pCommands);
            pCommands++;
            break;
        }

        case 0x9C:
        {
            // SFX MIDI fade out
            SFX_MIDIFadeOut(*pCommands);
            pCommands++;
            break;
        }

        case 0x9D:
        {
            // SFX set reverb type
            SFX_SetReverbType(*(unsigned char *)pCommands);
            pCommands++;
            break;
        }

        case 0x9E:
        {
            // Game over
            Game_Over();
            break;
        }

        case 0xA3:
        {
            // Set MechList anim speed
            CBruce *pBruce = (CBruce *)MechList;
            if (pBruce == 0)
                break;
            unsigned short val = *pCommands;
            pCommands++;
            pBruce->mIgnoreBruceInputTimer = val;
            break;
        }

        case 0xB1:
        {
            // Set MechList anim param
            CBruce *pBruce = (CBruce *)MechList;
            if (pBruce == 0)
                break;
            unsigned short val = *pCommands;
            pCommands++;
            pBruce->mIgnoreBruceInputFreezeTimer = val;
            break;
        }

        case 0x98:
        {
            // Teleport player
            CVector StartPos;
            CVector BestStartPos;
            CSVector *pBestAngles = (CSVector *)Trig_GetPosition(
                BestStartPos, Trig_GetLinksPointer(Node)[1]);
            CBruce *pBruce = pCurrentPlayer;
            CBody &body = pBruce->CSuper.CBody;
            body.CItem.mPos = BestStartPos;
            body.mVel.vx = 0;
            body.mVel.vy = -0x4000;
            body.mVel.vz = 0;
            body.mAcc.vx = 0;
            body.mAcc.vy = 0;
            body.mAcc.vz = 0;
            body.CItem.mPos.vy += (int)0xFFFE7000;
            pBruce->Trick_Bail();
            pBruce->mDoingATrick = 0;
            pBruce->mJumpPending = 0;
            pBruce->mJumpStrength = 0;
            pBruce->ResetTrickBuffers();
            pBruce->mStickToRail = 0;
            // Restore the linked node's angular velocity.
            body.CItem.mAngles = *pBestAngles;
            FudgeAngles(pBruce);
            body.mOldPos = body.CItem.mPos;
            // Preserve this position as Bruce's next safe restart point.
            pBruce->mSafePos = body.CItem.mPos;
            pBruce->mInBail = 0;
            pBruce->mpCamera->ResetOrientation();
            pBruce->CSuper.RunAnim(0, 0, -1, -1);
            pBruce->mPanelData.OutOfBounds();
            break;
        }

        case 0x94:
        {
            // Conditional (if pulse count != value)
            if (pCommandPoint->PulsesReceived == *pCommands++)
                break;
            // Check for endif (0x95) or skip commands until 0x95
            if (*pCommands == 0x95)
            {
                pCommands++;
                break;
            }
            do
            {
                pCommands = (unsigned short *)SkipCommand(pCommands);
            } while (*pCommands != 0x95);
            pCommands++;
            break;
        }

        case 0x86:
        {
            // Set executed flag + repeat counter
            if (pCommandPoint->NumPulsesSet == 0)
            {
                pCommandPoint->NumPulses = *pCommands;
                pCommandPoint->NumPulsesSet = 1;
            }
            pCommands++;
            break;
        }

        case 0x03:
        {
            // End-if / repeat trigger
            if (pCommandPoint != 0)
            {
                unsigned short repeat = pCommandPoint->NumPulses;
                if (repeat != 0)
                {
                    Trig_SendPulse((unsigned short *)Trig_GetLinksPointer(Node));
                    unsigned short rep = pCommandPoint->NumPulses;
                    if (rep != 0xFFFF)
                        pCommandPoint->NumPulses = rep - 1;
                }
            }
            else
            {
                Trig_SendPulse((unsigned short *)Trig_GetLinksPointer(Node));
            }
            break;
        }

        case 0x04:
        case 0x05:
        {
            // Send signal to links with param
            int links = Trig_GetLinksPointer(Node);
            SendSuspendOrActivate((unsigned short *)links, Command);
            break;
        }

        case 0x0A:
        {
            // Send signal to links
            int links = Trig_GetLinksPointer(Node);
            SendSignalToLinks((unsigned short *)links);
            break;
        }

        case 0x0D:
        {
            // Send visible from node
            SendVisibleFromNode(Node, *pCommands);
            pCommands++;
            break;
        }

        case 0x0B:
        {
            // Send kill from node (0)
            SendKillFromNode(Node, 0);
            break;
        }

        case 0x0C:
        {
            // Send kill from node (1)
            SendKillFromNode(Node, 1);
            break;
        }

        case 0x77:
        case 0xA2:
        skip_string:
            pCommands = (unsigned short *)SkipString((char *)pCommands);
            break;

        case 0xAA:
        {
            // Set suspend distance
            SuspendDistance = *pCommands++;
            break;
        }

        case 0xA6:
        {
            // Set OT pushback
            M3d_OTPushback = *pCommands++;
            break;
        }

        case 0xA9:
        {
            // Set OT pushback 2
            M3d_OTPushback2 = *pCommands++;
            break;
        }

        case 0xAD:
        {
            // Camera save orient
            CCamera *pCamera = (CCamera *)CameraList;
            if (pCamera == 0)
                break;
            pCamera->mFocus = pCamera->mTripod;
            break;
        }

        case 0xA7:
        {
            // Camera set zoom
            CCamera *pCamera = (CCamera *)CameraList;
            if (pCamera == 0)
            {
                pCommands += 2;
                break;
            }
            Camera_SetZoom(pCamera, pCommands[0], pCommands[1]);
            pCommands += 2;
            break;
        }

        case 0xA8:
        {
            // Camera heading set
            CCamera *pCamera = (CCamera *)CameraList;
            if (pCamera != 0)
            {
                int heading = *(short *)pCommands;
                pCamera->mPitchDamper = heading;
            }
            pCommands++;
            break;
        }

        case 0xAC:
        {
            // Camera pitch set
            CCamera *pCamera = (CCamera *)CameraList;
            if (pCamera != 0)
            {
                int pitch = *(short *)pCommands;
                pCamera->mTripodPosYDamper = pitch;
            }
            pCommands++;
            break;
        }

        case 0x82:
        {
            // Camera set angle
            CCamera *pCamera = (CCamera *)CameraList;
            if (pCamera == 0)
            {
                pCommands += 2;
                break;
            }
            pCamera->SetCamAngle(*(short *)pCommands, pCommands[1]);
            pCommands += 2;
            break;
        }

        case 0x87:
        {
            // Camera set XZ distance
            CCamera *pCamera = (CCamera *)CameraList;
            if (pCamera == 0)
            {
                pCommands += 2;
                break;
            }
            pCamera->SetCamXZDistance(pCommands[0], pCommands[1]);
            pCommands += 2;
            break;
        }

        case 0x8F:
        {
            // Camera set Y distance
            CCamera *pCamera = (CCamera *)CameraList;
            if (pCamera == 0)
            {
                pCommands += 2;
                break;
            }
            pCamera->SetCamYDistance(*(short *)pCommands, pCommands[1]);
            pCommands += 2;
            break;
        }

        case 0x90:
        {
            // Camera set X offset
            CCamera *pCamera = (CCamera *)CameraList;
            if (pCamera == 0)
            {
                pCommands += 2;
                break;
            }
            pCamera->SetCamXOffset(*(short *)pCommands, pCommands[1]);
            pCommands += 2;
            break;
        }

        case 0x91:
        {
            // Camera set Y offset
            CCamera *pCamera = (CCamera *)CameraList;
            if (pCamera == 0)
            {
                pCommands += 2;
                break;
            }
            pCamera->SetCamYOffset(*(short *)pCommands, pCommands[1]);
            pCommands += 2;
            break;
        }

        case 0x92:
        {
            // Camera set Z offset
            CCamera *pCamera = (CCamera *)CameraList;
            if (pCamera == 0)
            {
                pCommands += 2;
                break;
            }
            pCamera->SetCamZOffset(*(short *)pCommands, pCommands[1]);
            pCommands += 2;
            break;
        }

        case 0x99:
        {
            // Camera collision ray LR
            CCamera *pCamera = (CCamera *)CameraList;
            if (pCamera == 0)
            {
                pCommands++;
                break;
            }
            pCamera->SetCollisionRayLR(*(short *)pCommands);
            pCommands++;
            break;
        }

        case 0x9A:
        {
            // Camera collision ray back
            CCamera *pCamera = (CCamera *)CameraList;
            if (pCamera == 0)
            {
                pCommands++;
                break;
            }
            pCamera->SetCollisionRayBack(*(short *)pCommands);
            pCommands++;
            break;
        }

        case 0xA4:
        {
            // Camera collision angle LR
            CCamera *pCamera = (CCamera *)CameraList;
            if (pCamera == 0)
            {
                pCommands++;
                break;
            }
            pCamera->SetCollisionAngLR(*(short *)pCommands);
            pCommands++;
            break;
        }

        case 0xA5:
        {
            // Camera collision angle back
            CCamera *pCamera = (CCamera *)CameraList;
            if (pCamera == 0)
            {
                pCommands++;
                break;
            }
            pCamera->SetCollisionAngBack(*(short *)pCommands);
            pCommands++;
            break;
        }

        case 0xA0:
        {
            // Camera set look-at flag
            CCamera *pCamera = (CCamera *)CameraList;
            if (pCamera != 0)
                pCamera->mCameraMode = *pCommands;
            pCommands++;
            break;
        }

        case 0xA1:
        {
            // Camera set complex mode
            CCamera *pCamera = (CCamera *)CameraList;
            if (pCamera == 0)
            {
                pCommands++;
                break;
            }
            pCamera->SetComplexMode(*pCommands);
            pCommands++;
            break;
        }

        case 0x93:
        {
            // Set current game level
            CurrentGameLevel = *pCommands++;
            break;
        }

        case 0x88:
        {
            // XA allow on
            Redbook_XAAllow(true);
            break;
        }

        case 0x89:
        {
            // XA allow off
            Redbook_XAAllow(false);
            break;
        }

        case 0x8B:
        case 0xAE:
            pCommands += 2;
            break;

        default:
            break;
        }

    next_cmd:;
    }

    // Post-loop: apply fog if flag was set
    if (NewFoggingParameters != 0)
    {
        M3dInit_SetFoggingParams(ZHither, ZDpqMin, Range);
    }
}

// Build the restart tables and fire initial command nodes after parsing TRGs.
/**
 * Trig_InitialParseTRGFile__Fv
 *
 * Address:   0x80047D3C
 * Mangled:   Trig_InitialParseTRGFile__Fv
 * Demangled: Trig_InitialParseTRGFile(void)
 * Match:     PERFECT (130/130)
 */
void Trig_InitialParseTRGFile(void)
{
    NumCheatRestarts = 0;
    int i = 0;
    if (i < NumNodes)
    {
        do
        {
            short *pNode = OffsetList[i];
            CVector Dummy;
            if (*pNode == 8)
            {
                char *pNodeName = (char *)Trig_GetPosition(Dummy, i) + 6;
                CheatRestarts[NumCheatRestarts++] = pNodeName;
            }
            i++;
        } while (i < NumNodes);
    }

    NumHorseRestarts = 0;
    i = 0;
    if (i < NumNodes)
    {
        do
        {
            short *pNode = OffsetList[i];
            CVector Dummy;
            if (*pNode == 8)
            {
                char *pNodeName = (char *)Trig_GetPosition(Dummy, i);
                pNodeName += 6;
                if (*pNodeName == 'H')
                    HorseRestarts[NumHorseRestarts++] = pNodeName;
            }
            i++;
        } while (i < NumNodes);
    }

    RestartNode = 0xFFFF;
    int Found = 0;
    if (GNumberOfPlayers == 2)
    {
        i = 0;
        if (i < NumNodes)
        {
            do
            {
                short *pNode = OffsetList[i];
                if (*pNode == 0xF)
                {
                    Found = 1;
                    ExecuteCommandList((unsigned short *)(pNode + 1), i, 1);
                }
                i++;
            } while (i < NumNodes);
        }
    }

    if (Found == 0)
    {
        i = 0;
        if (i < NumNodes)
        {
            do
            {
                short *pNode = OffsetList[i];
                if (*pNode == 4)
                    ExecuteCommandList((unsigned short *)(pNode + 1), i, 1);
                i++;
            } while (i < NumNodes);
        }
    }
}

/**
 * Trig_ParseTRGFile__Fv
 *
 * Address:   0x80047F44
 * Mangled:   Trig_ParseTRGFile__Fv
 * Demangled: Trig_ParseTRGFile(void)
 *
 */
void Trig_ParseTRGFile(void)
{
    {
        int i = 0;
        if (NumNodes <= 0)
            goto phase2;

        do
        {
            short *pNode = OffsetList[i];
            CVector Pos;
            unsigned int checksum;

            switch (*pNode)
            {
            case 7:
                goto case7;
            case 1:
                goto case1;
            case 2:
            case 9:
                goto cpDefault;
            case 5:
                goto case5;
            case 6:
                goto case6;
            case 12:
            case 14:
                goto trickOb;
            case 13:
                goto camPt;
            case 0x1F4:
                goto light;
            default:
                goto next;
            }

    case7:
        {
            short linkCount = pNode[3];
            int flagOffset = (linkCount << 1) + 8;
            GetFlag(1, (unsigned char *)pNode + flagOffset);
        }
        goto next;
    case1:
        {
            short linkCount = pNode[3];
            int flagOffset = (linkCount << 1) + 8;
            if (GetFlag(1, (unsigned char *)pNode + flagOffset) == 0)
                goto next;
        }
        goto createObject;
    case5:
        if (*Trig_GetPosition(Pos, i) != 1)
            goto next;
        // fall through to shared create
    createObject:
        Trig_CreateObject(i);
        goto next;
    case6:
        {
            pNode++;
            pNode += *pNode + 1;
            unsigned short nodeIndex = i;
            if ((int)pNode & 2)
                pNode++;
            checksum = *(unsigned int *)pNode;
            CreateCommandPoint(checksum, nodeIndex,
                               (unsigned short *)(pNode + 2));
        }
        goto next;
    cpDefault: // case 2 and 9
        {
            pNode++;
            pNode += *pNode + 1;
            if ((int)pNode & 2)
                pNode++;
            checksum = *(unsigned int *)pNode;
            CreateCommandPoint(checksum, i & 0xFFFF, CrateCommands);
        }
        goto next;
    light: // case 0x1F4
        {
            STriggerLightBytes *pStuff =
                (STriggerLightBytes *)Trig_GetPosition(Pos, i);
            UseCacheForBits = 0;
            CLight *pLight = static_cast<CLight *>(
                Bit_AllocateStorage(TRIG_LIGHT_STORAGE_SIZE));
            if (pLight != 0)
            {
                ::new (pLight) CLight(
                    Pos, i,
                    pStuff->ClipDist, pStuff->InnerRadius,
                    pStuff->FringeRadius, pStuff->Amp,
                    pStuff->r0.Value, pStuff->g0.Value,
                    pStuff->b0.Value, pStuff->r1.Value,
                    pStuff->g1.Value, pStuff->b1.Value);
            }
            UseCacheForBits = 1;
        }
        goto next;
    trickOb: // case 0xc and 0xe
        {
            CTrickOb *pTrickOb =
                (CTrickOb *)CClass::operator new(sizeof(CTrickOb));
            if (pTrickOb != 0)
                CTrickOb_Construct(pTrickOb, i & 0xFFFF, 0);
        }
        goto next;
    camPt: // case 0xd
        CamPt_New(i);
    next:
            i++;
        } while (i < NumNodes);
    }

phase2:
    // Phase 2: clear trig menu and collect restart names
    Trig_ClearTrigMenu();
    int i = 0;
    int numN = NumNodes;
    if (numN <= 0)
        return;
    do
    {
        short *pNode = OffsetList[i];
        if (*pNode == 8)
        {
            CVector Dummy;
            char *pNodeName = (char *)Trig_GetPosition(Dummy, i) + 6;
            numN = NumNodes;
            MenuFilenamePointers[NumTrigMenuEntries++] = (int)pNodeName;
        }
        i++;
    } while (i < numN);
}

/**
 * Return the link-list payload pointer for a trigger node.
 *
 * Address:   0x8004827C
 * Mangled:   Trig_GetLinksPointer__Fi
 * Demangled: Trig_GetLinksPointer(int)
 *
 * @param node  Rail / graph node identifier.
 * @return Pointer to the node's link payload for node types that carry links.
 */
unsigned short *Trig_GetLinksPointer(int node)
{
    unsigned short *pNode = (unsigned short *)OffsetList[node];

    switch (*pNode)
    {
    case 1:
        return pNode + 3;
    case 5:
        return pNode + 2;
    case 2:
    case 3:
    case 6:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 1000:
    case 1001:
        return pNode + 1;
    }
}

/**
 * Return the aligned data pointer for a trigger node's stored position.
 * Trig_GetPosition__FR7CVectori
 *
 * Address:   0x80048330
 * Mangled:   Trig_GetPosition__FR7CVectori
 * Demangled: Trig_GetPosition(CVector &, int)
 * Match:     PERFECT (93/93)
 *
 * Reads the node's position payload (offset varies by node type), stores the
 * 12.20 fixed-point position into `pos`, and returns the pointer just past it.
 * The default arm returns through `done:` with pLong uninitialized (matching
 * the original binary, which returns garbage+12 for unknown node types).
 * `mask` doubles as the 0xFF sentinel and the ~3 alignment mask (one pseudo ->
 * $v1 for both, as in the target). The scoped $a0 pins keep pPos/flag in $a0,
 * which pushes `pos` to $a3 (the target's `move $a3,$a0`).
 *
 * @param pos  Position vector (output, 12.20 fixed point).
 * @param node  Rail / graph node identifier.
 * @return Pointer past the position words (angles/data that follow).
 */
unsigned short *Trig_GetPosition(CVector &pos, int node)
{
    ushort *pNode = (ushort *)OffsetList[node];
    ushort *pPos;
    int *pLong;
    unsigned char *pFlags;
    uint mask;

    switch (*pNode)
    {
    case 1:
    case 7:
    {
        pNode += pNode[3] + 4;
        pFlags = (unsigned char *)pNode;
        register uint flag asm("$4") = *pFlags;
        asm volatile("" : "+r"(flag));
        mask = 0xFF;
        if (flag != mask)
        {
            do
            {
                pFlags++;
            } while (*pFlags != mask);
        }
        pPos = (ushort *)(pFlags + 4);
        break;
    }
    case 3:
    case 8:
    case 10:
    case 11:
    case 13:
    case 1000:
    case 1001:
        mask = 0xFFFFFFFC;
        pPos = (ushort *)((uint)pNode[1] * 2 + (int)pNode + 7);
        pLong = (int *)((uint)pPos & mask);
        goto store;
    case 5:
        mask = 0xFFFFFFFC;
        pPos = (ushort *)((uint)pNode[2] * 2 + (int)pNode + 9);
        pLong = (int *)((uint)pPos & mask);
        goto store;
    case 500:
    case 501:
    {
        register ushort *t asm("$4") =
            (ushort *)((char *)pNode + 5);
        asm("" : "+r"(t));
        pPos = t;
        break;
    }
    default:
        goto done;
    }
    mask = 0xFFFFFFFC;
    pLong = (int *)((uint)pPos & mask);
store:
    pos.vx = pLong[0] << 12;
    pos.vy = pLong[1] << 12;
    pos.vz = pLong[2] << 12;
done:
    pPos = (ushort *)(pLong + 3);
    return pPos;
}
