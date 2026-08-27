// Scripted and physics-driven environmental platforms.

#include "PLATFORM.h"
#include "MAIN.h"
#include "MECH.h"
#include "SFX.h"
#include "OB.h"
#include "CBODY.h"
#include "SPOOL.h"
#include "globals.h"
#include "CITEM.h"
#include "M3DZONE.h"
#include "MIPS.h"
#define BADDY_LAYOUT_DECLARATIONS
#include "BADDY.h"
#undef BADDY_LAYOUT_DECLARATIONS
#include <STDDEF.H>
#include <LIBGTE.H>
#include "SMODEL.h"
#include "PSXREGION.h"
#define M3DCOLIJ_LINE_INFO_LAYOUT
#include "M3DCOLIJ.h"
#undef M3DCOLIJ_LINE_INFO_LAYOUT

struct SLight;

/** Composition-only item view used while scanning environmental objects. */
struct PlatformCItemOwner {
    u16 mFlags;
    u16 mInquiry;
    struct CVector mPos;
    struct CSVector mAngles;
    u16 mModel;
    char mDummyFrame;
    u8 mTintIndex;
    u8 mDummyAnim;
    u8 mRegion;
    struct PlatformCItemOwner *mNextItem;
    u32 mRGB;
    struct CSVector mScale;
    u16 _padScale;
    u32 mTRN;
    struct PlatformCItemOwner *mPreviousItem;
    u32 mStoredRGB;
    u8 mPredatorEffectType;
    u8 mPredatorEffectFade;
    u16 mType;
    SLight *mpLight;
    u16 mFlashTimer;
    s16 mFlash_dR;
    s16 mFlash_dG;
    s16 mFlash_dB;
    void *vtable;
};

/** Packed six-halfword interpretation used by platform collision helpers. */
struct PlatformModelBounds {
    short Half[6];
};

/** Scripted bouncing-platform object layout. */
struct CPlatform {
#ifdef __cplusplus
    /** Initialize a scripted platform from one trigger record. */
    CPlatform(short *pInfo, int nodeIndex);
#endif

    struct CBaddy CBaddy;
    u16 mScaleXTimer;
    u16 mScaleYTimer;
    u16 mScaleZTimer;
    s16 mScaleXAdjust;
    s16 mScaleYAdjust;
    s16 mScaleZAdjust;
    s16 mBruceHealthAdjust;
    u16 mNumBounces;
    u16 mType;
    u16 mWidth;
    u16 mHeight;
    u16 mDepth;
    u16 mDeathTimer;
    struct CSVector mTargetAngles;
    u32 mTrodUpon;
    union {
        u32 mNotTrodUponTimer;
        s16 mNotTrodUponScriptValue;
    };
    u32 mLoopingSound;
    int mLoopingSoundUpdate;
    struct CVector mBoundingBox;
#ifdef __cplusplus
    /** Apply this platform's configured bounce response to a skater. */
    void Bounce(CBruce *pSkater, const CVector &direction);
    void BouncyAI();
    /** Advance this platform's translation and rotation. */
    void DoPhysics();
    /** Update this platform's movement target. */
    void MoveTo(CVector &pos);
    /** No-op pattern hook used when script processing is disabled. */
    void ProcessPattern();
#endif
};

/** Exported seven-slot prefix of the platform virtual table. */
struct PlatformPrimaryVTable {
    void (*mReserved0)(void);
    void (*mReserved1)(void);
    void (*mDestructor)(struct CPlatform *, int);
    void (*Die)(struct CPlatform *);
    void (*mAI)(struct CPlatform *);
    void (*mEveryFrame)(struct CBodyOwner *);
    void (*mDoLightingSetup)(struct CBodyOwner *);
};

/** Owner-local five-slot tail placed immediately after the exported prefix. */
struct PlatformVirtualTailTable {
    int (*mHit)(struct CPlatform *, int, const CVector &, int);
    void (*mDeleteStuff)(struct CBodyOwner *);
    int (*mExecuteCommand)(struct CPlatform *, u16);
    void (*mSetVariable)(struct CPlatform *, u16);
    short (*mGetVariable)(struct CPlatform *, u16);
};

/** Combined dispatch view used when calling through either table segment. */
struct PlatformVirtualTable {
    void (*mReserved0)(void);
    void (*mReserved1)(void);
    void (*mDestructor)(struct CPlatform *, int);
    void (*Die)(struct CPlatform *);
    void (*mAI)(struct CPlatform *);
    void (*mEveryFrame)(struct CBodyOwner *);
    void (*mDoLightingSetup)(struct CBodyOwner *);
    int (*mHit)(struct CPlatform *, int, const CVector &, int);
    void (*mDeleteStuff)(struct CBodyOwner *);
    int (*mExecuteCommand)(struct CPlatform *, u16);
    void (*mSetVariable)(struct CPlatform *, u16);
    short (*GetVariable)(struct CPlatform *, u16);
};

/**
 * Skater layout slice exposing the inherited CSuper prefix.
 * The known complete size prevents accidental by-value stride changes.
 */
struct CBruce {
    struct CSuperOwner CSuper;
    u8 _opaqueTail[5228 - 400];
};

typedef char CItem_platform_size_check[
    (sizeof(struct PlatformCItemOwner) == 76) ? 1 : -1];
typedef char Platform_SDK_SVECTOR_size_must_be_8[
    (sizeof(SVECTOR) == 8) ? 1 : -1];
typedef char CPlatform_size_check[(sizeof(struct CPlatform) == 592) ? 1 : -1];
typedef char CPlatform_public_size_check[
    (sizeof(struct CPlatform) == PLATFORM_OBJECT_SIZE) ? 1 : -1];
typedef char PlatformVirtualTable_size_check[
    (sizeof(struct PlatformVirtualTable) == 48) ? 1 : -1];
typedef char PlatformPrimaryVTable_size_check[
    (sizeof(struct PlatformPrimaryVTable) == 28) ? 1 : -1];
typedef char PlatformVirtualTailTable_size_check[
    (sizeof(struct PlatformVirtualTailTable) == 20) ? 1 : -1];
typedef char CBruce_platform_size_check[
    (sizeof(struct CBruce) == 5228) ? 1 : -1];

#include "SHATTER.h"
#include "TRIG.h"
#include "UTILS.h"

/** Return the unsigned magnitude of a signed fixed-angle component. */
static inline int AngleMagnitude(short angle)
{
    int value = angle;
    int sign = value >> 31;
    return (value ^ sign) - sign;
}

/** Return the unsigned magnitude of a signed fixed-point component. */
static inline int VectorMagnitude(int value)
{
    int sign = value >> 31;
    return (value ^ sign) - sign;
}

/** View an owner-local vector layout through its canonical public type. */
static inline CVector &PlatformVector(CVectorOwner &vector)
{
    return reinterpret_cast<CVector &>(vector);
}

/** View an owner-local vector layout through its canonical public type. */
static inline const CVector &PlatformVector(const CVectorOwner &vector)
{
    return reinterpret_cast<const CVector &>(vector);
}

/** View a canonical vector through the identical owner-local layout. */
static inline CVectorOwner &PlatformOwnerVector(CVector &vector)
{
    return reinterpret_cast<CVectorOwner &>(vector);
}

/** View the owner-local item prefix through SHATTER's opaque public type. */
static inline Item *PlatformShatterItem(CItemOwner &item)
{
    return reinterpret_cast<Item *>(&item);
}

/*
 * GCC 2.91 does not inline the equivalent typed helpers inside AI and
 * ExecuteCommand. These owner-view macros keep those zero-code ABI boundaries
 * named without introducing calls in either large function.
 */
#define PLATFORM_VECTOR_VIEW(vector) reinterpret_cast<CVector &>(vector)
#define PLATFORM_CONST_VECTOR_VIEW(vector)                                  \
    reinterpret_cast<const CVector &>(vector)
#define PLATFORM_SHORT_VECTOR_VIEW(vector)                                  \
    reinterpret_cast<CSVector &>(vector)
#define PLATFORM_CONST_SHORT_VECTOR_VIEW(vector)                            \
    reinterpret_cast<const CSVector &>(vector)
#define PLATFORM_OWNER_VECTOR_POINTER(vector)                               \
    reinterpret_cast<CVectorOwner *>(vector)
#define PLATFORM_SHATTER_ITEM_POINTER(item) reinterpret_cast<Item *>(item)

/** Align serialized script data to its next 32-bit word boundary. */
#define PLATFORM_ALIGNED_SCRIPT(type, script)                                \
    reinterpret_cast<type *>(                                                \
        (reinterpret_cast<int>(script) + 3) & ~3)

/** Return the platform's typed virtual dispatch table. */
static inline PlatformVirtualTable *GetPlatformVirtualTable(
    CPlatform *_this)
{
    return reinterpret_cast<PlatformVirtualTable *>(
        _this->CBaddy.CSuper.CBody.CItem.vtable);
}

/** Script opcodes handled directly by CPlatform. */
enum PlatformCommand {
    PlatformCommandDie = 0x4205,
    PlatformCommandMoveTo = 0x4220,
    PlatformCommandMoveToNode = 0x4221,
    PlatformCommandMoveRelative = 0x4222,
    PlatformCommandWaitUntilTrodUpon = 0x4300,
    PlatformCommandShatter = 0x4301,
    PlatformCommandSetBoundingBox = 0x4302,
    PlatformCommandSetBruceHealthAdjust = 0x4303,
    PlatformCommandScale = 0x4304,
    PlatformCommandSetCollision = 0x4305,
    PlatformCommandScaleX = 0x4306,
    PlatformCommandScaleY = 0x4307,
    PlatformCommandScaleZ = 0x4308,
    PlatformCommandSetBounceType = 0x4309,
    PlatformCommandPlayLoopingSound = 0x4507,
    PlatformCommandPlayPositionalLoopingSound = 0x4508,
    PlatformCommandStopLoopingSound = 0x4509,
};

/** Script variables owned by CPlatform rather than its CBaddy base. */
enum PlatformVariable {
    PlatformVariableAngularVelocity = 0x2127,
    PlatformVariableAngularAcceleration = 0x2128,
    PlatformVariableApplyGravity = 0x2123,
    PlatformVariableModelIndex = 0x2124,
    PlatformVariableModelChecksum = 0x212F,
    PlatformVariableVelocity = 0x2134,
    PlatformVariableAngles = 0x2137,
    PlatformVariableNotTrodUponTimer = 0x2200,
};

/** Platform-owned state bits stored in CBaddy::mSpecificFlags. */
enum PlatformStateFlag {
    PlatformMovingToTarget = 1 << 0,
    PlatformHasBoundingBox = 1 << 2,
    PlatformBounceEnabled = 1 << 4,
    PlatformBouncing = 1 << 5,
};

/** CItem flags manipulated by platform construction and script commands. */
enum PlatformItemFlag {
    PlatformItemShattered = 1 << 0,
    PlatformItemCollision = 1 << 3,
    PlatformItemBounceContact = 1 << 5,
    PlatformItemEnvironmental = 1 << 8,
    PlatformItemScaleEnabled = 1 << 9,
};

/** Fixed platform dimensions and initial gameplay values. */
enum PlatformConstant {
    PlatformFixedOne = 1 << 12,
    PlatformHalfTurn = PlatformFixedOne / 2,
    PlatformQuarterTurn = PlatformFixedOne / 4,
    PlatformEighthTurn = PlatformFixedOne / 8,
    PlatformAngleMask = PlatformFixedOne - 1,
    PlatformOrthogonalAngleMask = 3 * PlatformQuarterTurn,
    PlatformGravityPerFrame = 8 * PlatformFixedOne,
    PlatformSoundVolume = 2 * PlatformFixedOne,
    PlatformVelocityRounding = PlatformFixedOne / 2,
    PlatformVelocityDeadZone = PlatformFixedOne + 1,
    PlatformTurnNumerator = 2 * PlatformFixedOne - 32,
    PlatformMaxAngularAcceleration = PlatformFixedOne / 16,
    PlatformLowVerticalSpeed = 12 * PlatformFixedOne,
    PlatformSoftBounceVelocity = -32 * PlatformFixedOne,
    PlatformStandardBounceVelocity = -64 * PlatformFixedOne,
    PlatformStrongBounceVelocity = -80 * PlatformFixedOne,
    PlatformCollisionMargin = 20 * PlatformFixedOne,
    PlatformScriptVariableFlag = 1 << 13,
    PlatformScriptWordMask = (1 << 16) - 1,
    PlatformInitialItemFlags = 0x111,
    PlatformObjectType = 0x192,
    PlatformDefaultMoveSpeed = 0x20,
};

// === Extern vtable/list declarations ===
#define BODYLISTS_DECLARE_ENVIRONMENTAL_OBJECT_LIST
#include "BODYLISTS.h"
#include "MECHLIST.h"
#include "GLEVEL.h"
#include "CURRENTOBJFILE.h"

extern "C" void Platform_InitItem(CItemOwner *, char *)
    asm("InitItem__5CItemPc");

// === Extern "C" forward declarations ===
extern "C" void Platform_CBaddyDestructorAbi(CBaddy *, int)
    asm("_._6CBaddy");
extern "C" void CPlatform_dtor(CPlatform *, int) asm("_._9CPlatform");
extern "C" void CPlatform_AI(CPlatform *) asm("AI__9CPlatform");
extern "C" int CPlatform_Hit(
    CPlatform *, int, const CVector &, int)
    asm("Hit__9CPlatformiRC7CVectori");
extern "C" int CPlatform_ExecuteCommand(CPlatform *, unsigned short)
    asm("ExecuteCommand__9CPlatformUs");
// Typed cross-TU dependencies not yet exposed by narrow shared APIs.
extern "C" void CPlatform_SetVariable(CPlatform *, unsigned short)
    asm("SetVariable__9CPlatformUs");
extern "C" short CPlatform_GetVariable(CPlatform *, unsigned short)
    asm("GetVariable__9CPlatformUs");
extern "C" void Platform_CBaddySetVariable(CBaddy *, u16)
    asm("SetVariable__6CBaddyUs");
extern "C" short Platform_CBaddyGetVariable(CBaddy *, u16)
    asm("GetVariable__6CBaddyUs");
extern "C" void Platform_VectorSubtract(
    CVectorOwner *, const CVectorOwner *, const CVectorOwner *)
    asm("__mi__FRC7CVectorT0");
extern "C" CVector WorldGravity;
#define M3DMATHS_TYPED_API
#include "M3DMATHS.h"
#undef M3DMATHS_TYPED_API
extern "C" CVector Platform_VectorSum(
    const CVector &, const CVector &)
    asm("__pl__FRC7CVectorT0");
extern "C" int Platform_CBaddyExecuteCommand(CBaddy *, unsigned short)
    asm("ExecuteCommand__6CBaddyUs");
extern "C" void Platform_Vibrate(CBruce *, int, int, int)
    asm("Vibrate__6CBruceiii");
extern "C" void Platform_CBaddyDie(CBaddy *) asm("Die__6CBaddy");
extern "C" void Platform_DoLightingSetup(CBodyOwner *)
    asm("DoLightingSetup__5CBody");
extern "C" void Platform_DeleteStuff(CBodyOwner *)
    asm("DeleteStuff__5CBody");

/**
 * Platform virtual dispatch table in its original read-only owner section.
 */
extern "C" PlatformPrimaryVTable __vt_9CPlatform
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        CPlatform_dtor,
        reinterpret_cast<void (*)(CPlatform *)>(Platform_CBaddyDie),
        CPlatform_AI,
        reinterpret_cast<void (*)(CBodyOwner *)>(CBody_EveryFrame),
        Platform_DoLightingSetup,
};

/**
 * Contiguous virtual slots following the target's seven-word exported table.
 *
 * MAIN.SYM ends `__vt_9CPlatform` at 0x800B1584, but target code dispatches
 * through the following owner-local slots, including GetVariable at +0x2c.
 */
static PlatformVirtualTailTable PlatformVirtualTail
    __attribute__((section(".rdata"), aligned(4))) = {
        CPlatform_Hit,
        Platform_DeleteStuff,
        CPlatform_ExecuteCommand,
        CPlatform_SetVariable,
        CPlatform_GetVariable,
};

// === Function implementations (address order) ===

/**
 * Build and initialize a platform from its serialized trigger-node data.
 *
 * @param p_info Cursor into the platform's initialization record.
 * @param node_index Trigger node that owns the platform.
 */
CPlatform::CPlatform(short *p_info, int node_index)
{
    CPlatform *_this = this;
    // The target keeps this address as a scalar across InitItem. A direct
    // typed vtable assignment changes eight constructor instructions.
    int vtbl = reinterpret_cast<int>(&__vt_9CPlatform);
    // Keeping the source pointer live across the preceding field setup is
    // also target-significant; direct use changes eight instructions and one
    // relocation in this constructor.
    char *objFile = pCurrentObjFile;
    // The player snapshot likewise preserves the target's load schedule;
    // direct assignment changes eight constructor instructions.
    CBruce *player = pCurrentPlayer;
    *reinterpret_cast<int *>(
        &_this->CBaddy.CSuper.CBody.CItem.vtable) = vtbl;
    _this->CBaddy.mpCreatedByPlayer = player;
    Platform_InitItem(&_this->CBaddy.CSuper.CBody.CItem, objFile);
    CBody *body = reinterpret_cast<CBody *>(
        &_this->CBaddy.CSuper.CBody);
    body->AttachTo(reinterpret_cast<CBody **>(&EnvironmentalObjectList));
    unsigned short flags = _this->CBaddy.CSuper.CBody.CItem.mFlags;
    _this->mType = PlatformObjectType;
    flags |= PlatformInitialItemFlags;
    flags &= 0xFFFD;
    _this->CBaddy.CSuper.CBody.CItem.mFlags = flags;
    short *pos = body->SquirtPos(p_info);
    short *angles = body->SquirtAngles(pos);
    _this->CBaddy.mpScript = reinterpret_cast<u16 *>(angles);
    _this->CBaddy.CSuper.CBody.mNode =
        static_cast<unsigned short>(node_index);
    _this->mLoopingSoundUpdate = -1;
    _this->CBaddy.mBaddyAttributes[0] = PlatformDefaultMoveSpeed;
    _this->CBaddy.mDoScript = 1;
}

/**
 * Detach a platform, stop its looping sound, and destroy its baddy state.
 *
 * @param _this Platform being destroyed through the original destructor ABI.
 * @param __in_chrg GCC 2.x in-charge flag; bit zero also releases storage.
 */
void CPlatform_dtor(CPlatform *_this, int __in_chrg)
{
    _this->CBaddy.CSuper.CBody.CItem.vtable = &__vt_9CPlatform;
    reinterpret_cast<CBody *>(&_this->CBaddy.CSuper.CBody)->DeleteFrom(
        reinterpret_cast<CBody **>(&EnvironmentalObjectList));
    if (_this->mLoopingSound)
    {
        SFX_Stop(_this->mLoopingSound);
        _this->mLoopingSound = 0;
    }
    Platform_CBaddyDestructorAbi(&_this->CBaddy, 0);
    if (__in_chrg & 1)
    {
        ::CItem::operator delete(_this);
    }
}

/**
 * Apply the platform's configured bounce response to a skater.
 *
 * @param pSkater Skater receiving the bounce response.
 * @param Direction Incoming horizontal velocity direction.
 */
void CPlatform::Bounce(CBruce *pSkater, const CVector &Direction)
{
    CPlatform *self = this;
    CPlatform *platform = self;
    int flags = platform->CBaddy.mSpecificFlags;
    if (!(flags & PlatformBounceEnabled))
        return;
    if (flags & PlatformBouncing)
        return;

    CVector Dir = Direction;
    Dir.vy = 0;
    int Speed = Dir.Length();

    platform->mNumBounces = 0;
    platform->CBaddy.mSpecificFlags |= PlatformBouncing;
    platform->CBaddy.CSuper.CBody.CItem.mFlags |= PlatformItemBounceContact;
    platform->mTargetAngles.vy = Rnd(PlatformFixedOne);
    int randomAngleX = (Rnd(2) << 11) - PlatformQuarterTurn;
    platform->mTargetAngles.vx =
        platform->CBaddy.CSuper.CBody.CItem.mAngles.vx + randomAngleX;
    int randomAngleZ = (Rnd(2) << 11) - PlatformQuarterTurn;
    platform->mTargetAngles.vz =
        platform->CBaddy.CSuper.CBody.CItem.mAngles.vz + randomAngleZ;

    switch (platform->mType)
    {
    case 1:
        SFX_PlayX(0x68, PlatformSoundVolume, 0);
        goto standard_bounce;
    case 3:
        SFX_PlayX(0x3C, PlatformSoundVolume, 0);
    standard_bounce:
        SFX_Grunt(pSkater);
        Platform_Vibrate(pSkater, 6, 1, 0x78);

        {
            int Height = Speed < 40
                ? PlatformSoftBounceVelocity
                : PlatformStandardBounceVelocity;
            if (Speed < 18)
            {
                platform->mTargetAngles.vy =
                    platform->CBaddy.CSuper.CBody.CItem.mAngles.vy +
                    Rnd(PlatformQuarterTurn) - PlatformEighthTurn;
                platform->mTargetAngles.vx =
                    (platform->CBaddy.CSuper.CBody.CItem.mAngles.vx +
                     PlatformEighthTurn) & PlatformOrthogonalAngleMask;
                platform->mTargetAngles.vz =
                    (platform->CBaddy.CSuper.CBody.CItem.mAngles.vz +
                     PlatformEighthTurn) & PlatformOrthogonalAngleMask;
            }

            CVectorOwner *velocity = &platform->CBaddy.CSuper.CBody.mVel;
            int a = Direction.vx;
            int b = Height;
            int c = Direction.vz;
            velocity->vx = a;
            velocity->vy = b;
            velocity->vz = c;
        }
        return;

    case 2:
    case 4:
    case 5:
    {
        if (platform->mType == 2)
        {
            int sound = GLevel == 0 ? Rnd(2) + 0x53 : 0x6A;
            SFX_PlayX(sound, PlatformSoundVolume, 0);
        }
        else if (platform->mType == 4)
        {
            SFX_Grunt(pSkater);
            SFX_PlayX(0x52, PlatformSoundVolume, 0);
        }

        Platform_Vibrate(pSkater, 2, 1, 0xB4);
        {
            CVectorOwner *velocity = &platform->CBaddy.CSuper.CBody.mVel;
            int a = Direction.vx;
            int c = Direction.vz;
            velocity->vx = a + (a >> 1);
            velocity->vy = PlatformStrongBounceVelocity;
            velocity->vz = c + (c >> 1);
        }

        if (AngleMagnitude(platform->mTargetAngles.vx) == PlatformHalfTurn &&
            platform->mTargetAngles.vz == 0)
        {
            platform->mTargetAngles.vz =
                platform->CBaddy.CSuper.CBody.CItem.mAngles.vz;
        }
        if (platform->mTargetAngles.vx == 0)
        {
            if (AngleMagnitude(platform->mTargetAngles.vz) == PlatformHalfTurn)
            {
                platform->mTargetAngles.vx =
                    platform->CBaddy.CSuper.CBody.CItem.mAngles.vx;
            }
        }
        return;
    }

    default:
        {
            CVectorOwner *velocity = &platform->CBaddy.CSuper.CBody.mVel;
            int a = Direction.vx;
            int c = Direction.vz;
            velocity->vx = a;
            velocity->vy = PlatformStandardBounceVelocity;
            velocity->vz = c;
        }
        return;
    }
}

/**
 * Advance a loose platform through angular motion and collision bounces.
 *
 * The active object advances its angle target, integrates gravity and
 * friction, and resolves up to three collision bounces per frame.
 */
void CPlatform::BouncyAI()
{
    MATRIX m;
    SLineInfo LInfo;
    CVector NewPos;
    CVector NewVel;
    CFriction Fric;
    int ObjectExtent;

    if (this->mDeathTimer != 0)
    {
        this->mDeathTimer--;
        if (this->mDeathTimer == 0)
        {
            PlatformVirtualTable *vtable = GetPlatformVirtualTable(this);
            vtable->Die(this);
            return;
        }
    }

    if ((this->CBaddy.mSpecificFlags & PlatformBouncing) == 0)
        return;

    this->mTargetAngles.Mask();

    this->CBaddy.CSuper.CBody.mAngVel.vx =
        static_cast<short>((this->mTargetAngles.vx -
        this->CBaddy.CSuper.CBody.CItem.mAngles.vx) >> 5);
    this->CBaddy.CSuper.CBody.mAngVel.vy =
        static_cast<short>((this->mTargetAngles.vy -
        this->CBaddy.CSuper.CBody.CItem.mAngles.vy) >> 5);
    this->CBaddy.CSuper.CBody.mAngVel.vz =
        static_cast<short>((this->mTargetAngles.vz -
        this->CBaddy.CSuper.CBody.CItem.mAngles.vz) >> 5);
    PLATFORM_SHORT_VECTOR_VIEW(
        this->CBaddy.CSuper.CBody.mAngVel).KillSmall();

    if (AngleMagnitude(this->CBaddy.CSuper.CBody.mAngVel.vx) < 40 &&
        VectorMagnitude(this->mTargetAngles.vx -
            this->CBaddy.CSuper.CBody.CItem.mAngles.vx) > 40)
    {
        this->CBaddy.CSuper.CBody.mAngVel.vx =
            this->CBaddy.CSuper.CBody.mAngVel.vx < 0 ? -40 : 40;
    }
    if (AngleMagnitude(this->CBaddy.CSuper.CBody.mAngVel.vy) < 40 &&
        VectorMagnitude(this->mTargetAngles.vy -
            this->CBaddy.CSuper.CBody.CItem.mAngles.vy) > 40)
    {
        this->CBaddy.CSuper.CBody.mAngVel.vy =
            this->CBaddy.CSuper.CBody.mAngVel.vy < 0 ? -40 : 40;
    }
    if (AngleMagnitude(this->CBaddy.CSuper.CBody.mAngVel.vz) < 40 &&
        VectorMagnitude(this->mTargetAngles.vz -
            this->CBaddy.CSuper.CBody.CItem.mAngles.vz) > 40)
    {
        this->CBaddy.CSuper.CBody.mAngVel.vz =
            this->CBaddy.CSuper.CBody.mAngVel.vz < 0 ? -40 : 40;
    }

    PLATFORM_SHORT_VECTOR_VIEW(this->CBaddy.CSuper.CBody.CItem.mAngles) +=
        PLATFORM_CONST_SHORT_VECTOR_VIEW(this->CBaddy.CSuper.CBody.mAngVel);
    PLATFORM_SHORT_VECTOR_VIEW(
        this->CBaddy.CSuper.CBody.CItem.mAngles).Mask();
    M3dMaths_RotMatrixYXZ(
        reinterpret_cast<SVECTOR *>(
            &this->CBaddy.CSuper.CBody.CItem.mAngles),
        &m);

    ObjectExtent =
        (this->mWidth * AngleMagnitude(m.m[1][0]) +
         this->mHeight * AngleMagnitude(m.m[1][1]) +
         this->mDepth * AngleMagnitude(m.m[1][2])) >> 1;

    NewVel = PlatformVector(this->CBaddy.CSuper.CBody.mVel);
    NewVel.vy += PlatformGravityPerFrame;
    Fric = CFriction(6, 6, 6);

    NewVel %= Fric;
    NewVel.KillSmall();
    NewPos = Platform_VectorSum(
        PlatformVector(this->CBaddy.CSuper.CBody.CItem.mPos), NewVel);

    CVector Dir = NewVel;
    Dir.vy = 0;
    int Speed = Dir.Length();

    for (int NumChecks = 0; NumChecks < 3; NumChecks++)
    {
        LInfo.StartCoords.vx =
            this->CBaddy.CSuper.CBody.CItem.mPos.vx;
        LInfo.StartCoords.vy =
            this->CBaddy.CSuper.CBody.CItem.mPos.vy;
        LInfo.StartCoords.vz =
            this->CBaddy.CSuper.CBody.CItem.mPos.vz;
        LInfo.EndCoords.vx = NewPos.vx;
        LInfo.EndCoords.vy = NewPos.vy;
        LInfo.EndCoords.vz = NewPos.vz;
        M3dColij_InitLineInfo(&LInfo);
        M3dZone_LineToItem(&LInfo, 1);

        if (LInfo.pItem == 0)
        {
            if (NewVel.vy > 0)
            {
                LInfo.StartCoords.vx = NewPos.vx;
                LInfo.StartCoords.vy = NewPos.vy;
                LInfo.StartCoords.vz = NewPos.vz;
                LInfo.EndCoords = LInfo.StartCoords;
                LInfo.StartCoords.vy -= ObjectExtent;
                LInfo.EndCoords.vy += ObjectExtent;
                M3dColij_InitLineInfo(&LInfo);
                M3dZone_LineToItem(&LInfo, 1);
            }

            if (LInfo.pItem == 0)
            {
                this->CBaddy.CSuper.CBody.CItem.mPos =
                    PlatformOwnerVector(NewPos);
                this->CBaddy.CSuper.CBody.mVel =
                    PlatformOwnerVector(NewVel);
                break;
            }
        }

        unsigned short NumBounces = this->mNumBounces;
        if (NumBounces < 5)
        {
            switch (this->mType)
            {
            case 1:
                if (Rnd(2) != 0)
                    SFX_PlayX(0x66, PlatformSoundVolume, 0);
                else
                    SFX_PlayX(0x67, PlatformSoundVolume, 0);
                break;
            case 3:
                SFX_PlayX(0x45, PlatformSoundVolume, 0);
                break;
            case 2:
                if (GLevel != 0)
                {
                    if (Rnd(2) != 0)
                        SFX_PlayX(0x6A, PlatformSoundVolume, 0);
                    else
                        SFX_PlayX(0x6B, PlatformSoundVolume, 0);
                    break;
                }
                goto type_five_sound;
            case 4:
                if (Rnd(2) != 0)
                    SFX_PlayX(0x50, PlatformSoundVolume, 0);
                else
                    SFX_PlayX(0x51, PlatformSoundVolume, 0);
                break;
            case 5:
            type_five_sound:
                SFX_PlayX(Rnd(2) + 0x53, PlatformSoundVolume, 0);
                break;
            default:
                goto no_bounce_sound;
            }
            NumBounces = this->mNumBounces;
        }

    no_bounce_sound:
        CVector Normal;
        Normal.vx = LInfo.Normal.vx;
        Normal.vy = LInfo.Normal.vy;
        Normal.vz = LInfo.Normal.vz;
        this->mNumBounces = NumBounces + 1;

        int dot = Mips_CVecDotProd(&NewVel, &Normal);
        NewVel.vx -= Mips_FixMul(Normal.vx, dot) << 1;
        NewVel.vy -= Mips_FixMul(Normal.vy, dot) << 1;
        NewVel.vz -= Mips_FixMul(Normal.vz, dot) << 1;

        Fric = CFriction(3, 2, 3);
        if (VectorMagnitude(NewVel.vy) < PlatformLowVerticalSpeed)
        {
            Fric.vy = 0;
            Fric.vx = Fric.vz = 2;
        }
        else
        {
            this->mTargetAngles.vy = Rnd(PlatformFixedOne);

            if (this->mNumBounces == 1)
            {
                this->mTargetAngles.vx +=
                    static_cast<short>(
                        (Rnd(2) << 11) - PlatformQuarterTurn);
                if ((this->mType == 2 || this->mType == 4) &&
                    ((this->mTargetAngles.vx == PlatformHalfTurn &&
                      this->mTargetAngles.vz == 0) ||
                     (this->mTargetAngles.vx == 0 &&
                      this->mTargetAngles.vz == PlatformHalfTurn)))
                {
                    this->mTargetAngles.vx =
                        (Rnd(2) << 11) - PlatformQuarterTurn;
                }
            }

            if (this->mNumBounces == 1)
            {
                this->mTargetAngles.vz += (Rnd(3) - 1) << 10;
                if ((this->mType == 2 || this->mType == 4) &&
                    ((this->mTargetAngles.vx == PlatformHalfTurn &&
                      this->mTargetAngles.vz == 0) ||
                     (this->mTargetAngles.vx == 0 &&
                      this->mTargetAngles.vz == PlatformHalfTurn)))
                {
                    this->mTargetAngles.vz =
                        (Rnd(2) << 11) - PlatformQuarterTurn;
                }
            }

            if (this->mNumBounces == 1 && Speed < 18)
            {
                this->mTargetAngles.vy =
                    this->CBaddy.CSuper.CBody.CItem.mAngles.vy +
                    Rnd(PlatformQuarterTurn) - PlatformEighthTurn;
                this->mTargetAngles.vx =
                    (this->CBaddy.CSuper.CBody.CItem.mAngles.vx +
                     PlatformEighthTurn) & PlatformOrthogonalAngleMask;
                this->mTargetAngles.vz =
                    (this->CBaddy.CSuper.CBody.CItem.mAngles.vz +
                     PlatformEighthTurn) & PlatformOrthogonalAngleMask;
            }
        }

        NewVel %= Fric;
        NewVel.KillSmall();

        if (this->mNumBounces > 18)
        {
            this->CBaddy.CSuper.CBody.CItem.mAngles.vx =
                this->mTargetAngles.vx;
            this->CBaddy.CSuper.CBody.CItem.mAngles.vz =
                this->mTargetAngles.vz;
            M3dMaths_RotMatrixYXZ(
                reinterpret_cast<SVECTOR *>(
                    &this->CBaddy.CSuper.CBody.CItem.mAngles),
                &m);
            NewVel.vx = NewVel.vy = NewVel.vz = 0;
            this->mDeathTimer = 10;
            this->CBaddy.mSpecificFlags &= ~PlatformBouncing;
            return;
        }

        NewPos.vx = LInfo.Position.vx +
            Mips_FixMul(LInfo.Normal.vx, ObjectExtent);
        NewPos.vy = LInfo.Position.vy +
            Mips_FixMul(LInfo.Normal.vy, ObjectExtent);
        NewPos.vz = LInfo.Position.vz +
            Mips_FixMul(LInfo.Normal.vz, ObjectExtent);
    }

    if (this->mNumBounces > 18)
    {
        NewVel.vx = NewVel.vy = NewVel.vz = 0;
        this->CBaddy.CSuper.CBody.CItem.mAngles.vx =
            this->mTargetAngles.vx;
        this->CBaddy.CSuper.CBody.CItem.mAngles.vz =
            this->mTargetAngles.vz;
        this->mDeathTimer = 10;
        this->CBaddy.mSpecificFlags &= ~PlatformBouncing;
    }
}

/**
 * Advances scripted platform scale, movement, steering, physics, and sound.
 *
 * @param platform  Platform instance being updated.
 */
void CPlatform_AI(CPlatform *platform)
{
    if (platform->mScaleXTimer != 0)
    {
        platform->mScaleXTimer--;
        platform->CBaddy.CSuper.CBody.CItem.mScale.vx +=
            platform->mScaleXAdjust;
    }
    if (platform->mScaleYTimer != 0)
    {
        platform->mScaleYTimer--;
        platform->CBaddy.CSuper.CBody.CItem.mScale.vy +=
            platform->mScaleYAdjust;
    }
    if (platform->mScaleZTimer != 0)
    {
        platform->mScaleZTimer--;
        platform->CBaddy.CSuper.CBody.CItem.mScale.vz +=
            platform->mScaleZAdjust;
    }

    platform->CBaddy.CSuper.CBody.mAcc.vx =
        platform->CBaddy.CSuper.CBody.mAcc.vy =
        platform->CBaddy.CSuper.CBody.mAcc.vz = 0;
    if (platform->mTrodUpon == 0)
        platform->mNotTrodUponTimer++;

    unsigned short inputFlags = platform->CBaddy.CSuper.CBody.mInputFlags;
    platform->CBaddy.mInputFlagsCopy = inputFlags;
    if ((inputFlags & 1) != 0)
    {
        platform->CBaddy.CSuper.CBody.mInputFlags = inputFlags & 0xFFFE;
        platform->CBaddy.mInputSignal++;
    }

    if ((platform->CBaddy.mSpecificFlags & PlatformBounceEnabled) != 0)
    {
        platform->BouncyAI();
        return;
    }

    if (platform->CBaddy.mDoScript != 0)
    {
        if (platform->CBaddy.mWaitTimer != 0)
            platform->CBaddy.mWaitTimer--;
        else
            platform->CBaddy.ParseScript(platform->CBaddy.mpScript);
    }
    else
    {
        platform->ProcessPattern();
    }

    if ((platform->CBaddy.mSpecificFlags & PlatformMovingToTarget) != 0)
    {
        CVectorOwner distance;
        Platform_VectorSubtract(
            &distance, &platform->CBaddy.CSuper.CBody.CItem.mPos,
            &platform->CBaddy.mTargetPos);
        int distanceToTarget = PlatformVector(distance).Length();
        int reachedTarget = !(
            static_cast<int>(platform->CBaddy.mBaddyAttributes[0]) <
            distanceToTarget);
        if (reachedTarget != 0)
        {
            platform->CBaddy.CSuper.CBody.CItem.mPos =
                platform->CBaddy.mTargetPos;
            if (platform->CBaddy.mBaddyAttributes[2] < 2)
            {
                CVectorOwner *velocity =
                    &platform->CBaddy.CSuper.CBody.mVel;
                velocity->vx = velocity->vy = velocity->vz = 0;
            }
            platform->CBaddy.mSpecificFlags &= ~PlatformMovingToTarget;
        }
        else
        {
            unsigned short movementMode =
                platform->CBaddy.mBaddyAttributes[2];
            if (movementMode == 0)
            {
                CSVectorOwner ideal_angles;
                Utils_CalcAim(
                    PLATFORM_SHORT_VECTOR_VIEW(ideal_angles),
                    PLATFORM_CONST_VECTOR_VIEW(
                        platform->CBaddy.CSuper.CBody.CItem.mPos),
                    PLATFORM_CONST_VECTOR_VIEW(
                        platform->CBaddy.mTargetPos));
                Utils_GetVecFromMagDir(
                    PLATFORM_VECTOR_VIEW(
                        platform->CBaddy.CSuper.CBody.mVel),
                    platform->CBaddy.mBaddyAttributes[0],
                    PLATFORM_CONST_SHORT_VECTOR_VIEW(ideal_angles));
            }
            else if (movementMode == 1)
            {
                CSVectorOwner ideal_angles;
                Utils_CalcAim(
                    PLATFORM_SHORT_VECTOR_VIEW(ideal_angles),
                    PLATFORM_CONST_VECTOR_VIEW(
                        platform->CBaddy.CSuper.CBody.CItem.mPos),
                    PLATFORM_CONST_VECTOR_VIEW(
                        platform->CBaddy.mTargetPos));
                int ang_acc;
                {
                    CVectorOwner remaining;
                    Platform_VectorSubtract(
                        &remaining, &platform->CBaddy.mTargetPos,
                        &platform->CBaddy.CSuper.CBody.CItem.mPos);
                    ang_acc = PlatformTurnNumerator /
                        PlatformVector(remaining).Length();
                }
                if (ang_acc < -PlatformMaxAngularAcceleration)
                    ang_acc = -PlatformMaxAngularAcceleration;
                else if (ang_acc > PlatformMaxAngularAcceleration)
                    ang_acc = PlatformMaxAngularAcceleration;
                else if (ang_acc > 0 && ang_acc < 4)
                    ang_acc = 4;
                else if (ang_acc < 0 && ang_acc > -4)
                    ang_acc = -4;

                Utils_TurnTowards(
                    PLATFORM_CONST_SHORT_VECTOR_VIEW(
                        platform->CBaddy.CSuper.CBody.CItem.mAngles),
                    PLATFORM_SHORT_VECTOR_VIEW(
                        platform->CBaddy.CSuper.CBody.mAngVel),
                    PLATFORM_SHORT_VECTOR_VIEW(
                        platform->CBaddy.CSuper.CBody.mAngAcc),
                    PLATFORM_CONST_SHORT_VECTOR_VIEW(ideal_angles),
                    ang_acc);
                platform->CBaddy.CSuper.CBody.CItem.mAngles.vx = 0;
                Utils_GetVecFromMagDir(
                    PLATFORM_VECTOR_VIEW(
                        platform->CBaddy.CSuper.CBody.mAcc),
                    platform->CBaddy.mBaddyAttributes[0],
                    PLATFORM_CONST_SHORT_VECTOR_VIEW(ideal_angles));

                short roll = 0;
                if ((platform->CBaddy.mSpecificFlags & PlatformBouncing) != 0)
                {
                    roll = static_cast<short>((ideal_angles.vy -
                        platform->CBaddy.CSuper.CBody.CItem.mAngles.vy) &
                        PlatformAngleMask);
                }
                if (roll > PlatformHalfTurn)
                    roll = static_cast<short>(roll - PlatformFixedOne);
                if (roll > PlatformQuarterTurn)
                    roll = PlatformQuarterTurn;
                else if (roll < -PlatformQuarterTurn)
                    roll = -PlatformQuarterTurn;

                roll = static_cast<short>((-roll >> 1) & PlatformAngleMask);
                short rolldiff = static_cast<short>(roll -
                    platform->CBaddy.CSuper.CBody.CItem.mAngles.vz);
                if (rolldiff < -PlatformHalfTurn)
                    rolldiff = static_cast<short>(
                        rolldiff + PlatformFixedOne);
                if (rolldiff > PlatformHalfTurn)
                    rolldiff = static_cast<short>(
                        rolldiff - PlatformFixedOne);
                if (rolldiff != 0)
                    platform->CBaddy.CSuper.CBody.mAngAcc.vz =
                        static_cast<short>(rolldiff >> 3);
                else
                    platform->CBaddy.CSuper.CBody.mAngVel.vz = 0;
            }
            else if (movementMode == 2)
            {
                CSVectorOwner ideal_angles;
                Utils_CalcAim(
                    PLATFORM_SHORT_VECTOR_VIEW(ideal_angles),
                    PLATFORM_CONST_VECTOR_VIEW(
                        platform->CBaddy.CSuper.CBody.CItem.mPos),
                    PLATFORM_CONST_VECTOR_VIEW(
                        platform->CBaddy.mTargetPos));
                int ang_acc;
                {
                    CVectorOwner remaining;
                    Platform_VectorSubtract(
                        &remaining, &platform->CBaddy.mTargetPos,
                        &platform->CBaddy.CSuper.CBody.CItem.mPos);
                    ang_acc = PlatformTurnNumerator /
                        PlatformVector(remaining).Length();
                }
                if (ang_acc < -PlatformMaxAngularAcceleration)
                    ang_acc = -PlatformMaxAngularAcceleration;
                else if (ang_acc > PlatformMaxAngularAcceleration)
                    ang_acc = PlatformMaxAngularAcceleration;
                else if (ang_acc > 0 && ang_acc < 4)
                    ang_acc = 4;
                else if (ang_acc < 0 && ang_acc > -4)
                    ang_acc = -4;

                Utils_TurnTowards(
                    PLATFORM_CONST_SHORT_VECTOR_VIEW(
                        platform->CBaddy.CSuper.CBody.CItem.mAngles),
                    PLATFORM_SHORT_VECTOR_VIEW(
                        platform->CBaddy.CSuper.CBody.mAngVel),
                    PLATFORM_SHORT_VECTOR_VIEW(
                        platform->CBaddy.CSuper.CBody.mAngAcc),
                    PLATFORM_CONST_SHORT_VECTOR_VIEW(ideal_angles),
                    ang_acc);
                platform->CBaddy.CSuper.CBody.CItem.mAngles.vx = 0;
                Utils_GetVecFromMagDir(
                    PLATFORM_VECTOR_VIEW(
                        platform->CBaddy.CSuper.CBody.mAcc),
                    platform->CBaddy.mBaddyAttributes[0],
                    PLATFORM_CONST_SHORT_VECTOR_VIEW(ideal_angles));
            }
        }
    }
    else if (platform->CBaddy.mBaddyAttributes[2] == 1)
    {
        short rolldiff = static_cast<short>(
            -platform->CBaddy.CSuper.CBody.CItem.mAngles.vz);
        if (rolldiff < -PlatformHalfTurn)
            rolldiff = static_cast<short>(rolldiff + PlatformFixedOne);
        if (rolldiff > PlatformHalfTurn)
            rolldiff = static_cast<short>(rolldiff - PlatformFixedOne);
        if (rolldiff == 0)
            platform->CBaddy.CSuper.CBody.mAngVel.vz = 0;
        else
            platform->CBaddy.CSuper.CBody.mAngAcc.vz =
                static_cast<short>(rolldiff >> 3);
    }

    if (platform->CBaddy.mApplyGravity != 0)
    {
        // These inherited scheduling boundaries are load-bearing under GCC
        // 2.91. Removing the first barrier changes 28 instructions and four
        // relocations; dropping `volatile` from the second changes 28
        // instructions and three relocations. Basic asm is already volatile,
        // so the first barrier needs no explicit qualifier.
        asm("");
        CVector *gravity = &WorldGravity;
        asm volatile("" : "+r"(gravity));
        PLATFORM_VECTOR_VIEW(platform->CBaddy.CSuper.CBody.mAcc) += *gravity;
    }

    platform->DoPhysics();
    if (platform->mLoopingSoundUpdate != -1 &&
        TTime % platform->mLoopingSoundUpdate == 0)
    {
        SFX_ModifyPos(
            platform->mLoopingSound,
            PlatformVector(platform->CBaddy.CSuper.CBody.CItem.mPos),
            0);
    }
    platform->mTrodUpon = 0;

}

/**
 * Damage a platform, flashing it while alive or shattering it on death.
 *
 * @param self Platform receiving the hit.
 * @param damage Health removed by the hit.
 * @param knock Knockback vector retained by the virtual ABI.
 * @param whathit Hit-category value retained by the virtual ABI.
 * @return One after processing the hit.
 */
int CPlatform_Hit(
    CPlatform *self, int damage, const CVector &knock, int whathit)
{
    u8 hitCount = self->CBaddy.mHit;
    u16 health = self->CBaddy.mBaddyAttributes[1];
    hitCount++;
    self->CBaddy.mHit = hitCount;
    if (health != 0)
    {
        s16 hp = self->CBaddy.CSuper.CBody.mHealth;
        u16 hpu = static_cast<u16>(self->CBaddy.CSuper.CBody.mHealth);
        if (hp > 0)
        {
            self->CBaddy.CSuper.CBody.mHealth = hpu - damage;
            if (self->CBaddy.CSuper.CBody.mHealth <= 0)
            {
                PlatformVirtualTable *vtbl =
                    GetPlatformVirtualTable(self);
                vtbl->Die(self);
                Shatter_Item(
                    PlatformShatterItem(
                        self->CBaddy.CSuper.CBody.CItem),
                    0, 1);
            }
            else
            {
                reinterpret_cast<CItem *>(
                    &self->CBaddy.CSuper.CBody.CItem)
                    ->Flash(3, 0xFF, 0xFF, 0xFF);
            }
        }
    }
    return 1;
}

/**
 * Advance platform translation and rotation in the current physics mode.
 *
 */
void CPlatform::DoPhysics()
{
    CPlatform *self = this;
    int mode = self->CBaddy.mBaddyAttributes[2];

    self->CBaddy.CSuper.CBody.mNormal.vz = 0;
    self->CBaddy.CSuper.CBody.mNormal.vy = 0;
    self->CBaddy.CSuper.CBody.mNormal.vx = 0;

    if (mode == 0)
    {
        // GCC 2.91 otherwise chooses a different integration schedule. The
        // measured all-pin cleanup changes 97 instructions and 3 relocs;
        // removing the translation, angle, or position groups separately
        // changes 19, 55, and 38 instructions respectively.
        int oldVelX = self->CBaddy.CSuper.CBody.mVel.vx;
        int accX = self->CBaddy.CSuper.CBody.mAcc.vx;
        register int oldVelY asm("$6") = self->CBaddy.CSuper.CBody.mVel.vy;
        register int accY asm("$4") = self->CBaddy.CSuper.CBody.mAcc.vy;
        register int oldVelZ asm("$5") = self->CBaddy.CSuper.CBody.mVel.vz;
        int vx = oldVelX + accX;
        register int vy asm("$11") = oldVelY + accY;
        accX = vx + PlatformVelocityRounding;
        oldVelX = self->CBaddy.CSuper.CBody.mAcc.vz;
        register int vz asm("$13") = oldVelZ + oldVelX;

        if (static_cast<u32>(accX) < PlatformVelocityDeadZone)
        {
            vx = 0;
        }
        if (static_cast<u32>(vy + PlatformVelocityRounding) <
            PlatformVelocityDeadZone)
        {
            vy = 0;
        }
        if (static_cast<u32>(vz + PlatformVelocityRounding) <
            PlatformVelocityDeadZone)
        {
            vz = 0;
        }

        register int sx asm("$9") = self->CBaddy.CSuper.CBody.mAngVel.vx;
        register int sy asm("$10") = self->CBaddy.CSuper.CBody.mAngVel.vy;
        register int sz asm("$8") = self->CBaddy.CSuper.CBody.mAngVel.vz;
        register int angleX asm("$7") =
            static_cast<u16>(self->CBaddy.CSuper.CBody.CItem.mAngles.vx);
        register int angAccX asm("$2") =
            static_cast<u16>(self->CBaddy.CSuper.CBody.mAngAcc.vx);
        register int angleY asm("$6") =
            static_cast<u16>(self->CBaddy.CSuper.CBody.CItem.mAngles.vy);
        register int angAccY asm("$3") =
            static_cast<u16>(self->CBaddy.CSuper.CBody.mAngAcc.vy);
        register int angleZ asm("$5") =
            static_cast<u16>(self->CBaddy.CSuper.CBody.CItem.mAngles.vz);
        register int angAccZ asm("$4") =
            static_cast<u16>(self->CBaddy.CSuper.CBody.mAngAcc.vz);

        self->CBaddy.CSuper.CBody.mVel.vx = vx;
        self->CBaddy.CSuper.CBody.mVel.vy = vy;
        self->CBaddy.CSuper.CBody.mVel.vz = vz;

        angleX = sx + angleX;
        sx = static_cast<s16>(sx + angAccX);
        angleY = sy + angleY;
        sy = static_cast<s16>(sy + angAccY);
        angleZ = sz + angleZ;
        sz = static_cast<s16>(sz + angAccZ);
        angleX &= PlatformAngleMask;
        angleY &= PlatformAngleMask;
        // Ordinary loads here move all three reads past the angle stores and
        // change 36 instructions; even relaxing only Z has the same result.
        register int posX asm("$2") =
            *reinterpret_cast<volatile int *>(
                &self->CBaddy.CSuper.CBody.CItem.mPos.vx);
        register int posY asm("$3") =
            *reinterpret_cast<volatile int *>(
                &self->CBaddy.CSuper.CBody.CItem.mPos.vy);
        int posZ =
            *reinterpret_cast<volatile int *>(
                &self->CBaddy.CSuper.CBody.CItem.mPos.vz);
        angleZ &= PlatformAngleMask;

        self->CBaddy.CSuper.CBody.CItem.mAngles.vx = angleX;
        self->CBaddy.CSuper.CBody.CItem.mAngles.vy = angleY;
        self->CBaddy.CSuper.CBody.CItem.mAngles.vz = angleZ;

        posX += vx;
        posY += vy;
        posZ += vz;
        self->CBaddy.CSuper.CBody.CItem.mPos.vx = posX;
        self->CBaddy.CSuper.CBody.CItem.mPos.vy = posY;
        self->CBaddy.CSuper.CBody.CItem.mPos.vz = posZ;

        if (static_cast<u16>(sx + 1) < 3)
        {
            sx = 0;
        }
        if (static_cast<u16>(sy + 1) < 3)
        {
            sy = 0;
        }
        if (static_cast<u16>(sz + 1) < 3)
        {
            sz = 0;
        }

        self->CBaddy.CSuper.CBody.mAngVel.vx = sx;
        self->CBaddy.CSuper.CBody.mAngVel.vy = sy;
        self->CBaddy.CSuper.CBody.mAngVel.vz = sz;
    }
    else
    {
        // Removing this final mode selector pin changes 35 instructions and
        // two relocations even after the surrounding accesses are typed.
        register u16 activeMode asm("$2") = mode - 1;
        if (activeMode >= 2)
        {
            return;
        }

        CVectorOwner *velocity = &self->CBaddy.CSuper.CBody.mVel;
        PLATFORM_VECTOR_VIEW(*velocity) +=
            PLATFORM_CONST_VECTOR_VIEW(self->CBaddy.CSuper.CBody.mAcc);
        PLATFORM_VECTOR_VIEW(*velocity) %=
            self->CBaddy.CSuper.CBody.mFric;
        PLATFORM_VECTOR_VIEW(*velocity).KillSmall();
        PLATFORM_VECTOR_VIEW(self->CBaddy.CSuper.CBody.CItem.mPos) +=
            PLATFORM_CONST_VECTOR_VIEW(*velocity);

        CSVectorOwner *angles =
            &self->CBaddy.CSuper.CBody.CItem.mAngles;
        CSVectorOwner *angVel = &self->CBaddy.CSuper.CBody.mAngVel;
        PLATFORM_SHORT_VECTOR_VIEW(*angles) +=
            PLATFORM_CONST_SHORT_VECTOR_VIEW(*angVel);
        PLATFORM_SHORT_VECTOR_VIEW(*angles).Mask();
        PLATFORM_SHORT_VECTOR_VIEW(*angVel) +=
            PLATFORM_CONST_SHORT_VECTOR_VIEW(
                self->CBaddy.CSuper.CBody.mAngAcc);
        PLATFORM_SHORT_VECTOR_VIEW(*angVel) %=
            self->CBaddy.CSuper.CBody.mAngFric;
        PLATFORM_SHORT_VECTOR_VIEW(*angVel).KillSmall();
    }
}

/**
 * Update the cached target and mark the platform as moving when it changes.
 *
 * @param pos New target position.
 */
void CPlatform::MoveTo(CVector &pos)
{
    CPlatform *self = this;
    if (PLATFORM_VECTOR_VIEW(self->CBaddy.mTargetPos) != pos)
    {
        self->CBaddy.mTargetPos =
            PlatformOwnerVector(pos);
        self->CBaddy.mSpecificFlags |= PlatformMovingToTarget;
    }
}

/** No-op pattern hook used by platforms whose script processing is disabled. */
void CPlatform::ProcessPattern()
{
}

/**
 * Execute a platform-specific script opcode.
 *
 * @param self Platform executing the script command.
 * @param command  Script opcode to consume.
 * @return One while script execution may continue, or zero after a blocking
 *         wait or destructive command.
 */
int CPlatform_ExecuteCommand(
    CPlatform *self, unsigned short command)
{
    struct CPlatform *plat = self;
    struct CItemOwner *item = &plat->CBaddy.CSuper.CBody.CItem;

    /* Scale commands only take effect once the per-item scale is enabled. */
#define PLATFORM_ENABLE_SCALE()                                                \
    do {                                                                       \
        int flags = item->mFlags;                                              \
        if ((flags & PlatformItemScaleEnabled) == 0) {                         \
            flags |= PlatformItemScaleEnabled;                                \
            item->mFlags = flags;                                              \
            item->mScale.vz = PlatformFixedOne;                                \
            item->mScale.vy = PlatformFixedOne;                                \
            item->mScale.vx = PlatformFixedOne;                                \
        }                                                                      \
    } while (0)

    switch (command)
    {
    case PlatformCommandSetBounceType:
    {
        unsigned short *pc = plat->CBaddy.mpScript;
        unsigned short type = pc[0];
        plat->CBaddy.mpScript = pc + 1;
        plat->mType = type;
        if (type != 0)
            plat->CBaddy.mSpecificFlags |= PlatformBounceEnabled;
        else
            plat->CBaddy.mSpecificFlags &= ~PlatformBounceEnabled;
        struct SModel *m =
            PSXRegion[item->mRegion].ppModels[item->mModel];
        short *pBox = ((PlatformModelBounds *)&m->Box)->Half;
        plat->mWidth = pBox[0] - pBox[1];
        plat->mHeight = pBox[2] - pBox[3];
        plat->mDepth = pBox[4] - pBox[5];
        break;
    }

    case PlatformCommandSetCollision:
    {
        unsigned short *pc = plat->CBaddy.mpScript;
        unsigned short on = pc[0];
        plat->CBaddy.mpScript = pc + 1;
        if (on != 0)
            item->mFlags |= PlatformItemCollision;
        else
            item->mFlags &= ~PlatformItemCollision;
        break;
    }

    case PlatformCommandPlayLoopingSound:
    {
        if (plat->mLoopingSound != 0)
            SFX_Stop(plat->mLoopingSound);
        unsigned short *pc = plat->CBaddy.mpScript;
        unsigned short sound = pc[0];
        plat->CBaddy.mpScript = pc + 1;
        plat->mLoopingSound = SFX_PlayX(sound, PlatformSoundVolume, 0);
        plat->mLoopingSoundUpdate = -1;
        break;
    }

    case PlatformCommandPlayPositionalLoopingSound:
    {
        if (plat->mLoopingSound != 0)
            SFX_Stop(plat->mLoopingSound);
        unsigned short *pc = plat->CBaddy.mpScript;
        unsigned short sound = *pc++;
        plat->CBaddy.mpScript = pc;
        unsigned short update = *pc++;
        plat->CBaddy.mpScript = pc;
        plat->mLoopingSoundUpdate = update;
        plat->mLoopingSound = SFX_PlayPos(
            sound, PLATFORM_CONST_VECTOR_VIEW(item->mPos), 0);
        break;
    }

    case PlatformCommandStopLoopingSound:
        if (plat->mLoopingSound != 0)
            SFX_Stop(plat->mLoopingSound);
        plat->mLoopingSound = 0;
        break;

    case PlatformCommandScale:
    {
        PLATFORM_ENABLE_SCALE();
        unsigned short *pc = plat->CBaddy.mpScript;
        short p = *pc++;
        plat->CBaddy.mpScript = pc;
        plat->mScaleXTimer = plat->mScaleYTimer =
            plat->mScaleZTimer = *pc++;
        plat->CBaddy.mpScript = pc;
        if (plat->mScaleXTimer != 0)
        {
            plat->mScaleYAdjust = plat->mScaleZAdjust =
                plat->mScaleXAdjust = (static_cast<int>(p) << 12) /
                    static_cast<int>(
                        static_cast<unsigned int>(plat->mScaleXTimer) * 100);
        }
        else
        {
            plat->mScaleYAdjust = plat->mScaleZAdjust =
                plat->mScaleXAdjust = (static_cast<int>(p) << 12) / 100;
        }
        item->mScale.vx += plat->mScaleXAdjust;
        item->mScale.vy += plat->mScaleYAdjust;
        item->mScale.vz += plat->mScaleZAdjust;
        break;
    }

    case PlatformCommandScaleX:
    {
        PLATFORM_ENABLE_SCALE();
        unsigned short *pc = plat->CBaddy.mpScript;
        short p = *pc++;
        plat->CBaddy.mpScript = pc;
        plat->mScaleXTimer = *pc++;
        plat->CBaddy.mpScript = pc;
        if (plat->mScaleXTimer != 0)
            plat->mScaleXAdjust = (static_cast<int>(p) << 12) /
                static_cast<int>(
                    static_cast<unsigned int>(plat->mScaleXTimer) * 100);
        else
            plat->mScaleXAdjust = (static_cast<int>(p) << 12) / 100;
        item->mScale.vx += plat->mScaleXAdjust;
        break;
    }

    case PlatformCommandScaleY:
    {
        PLATFORM_ENABLE_SCALE();
        unsigned short *pc = plat->CBaddy.mpScript;
        short p = *pc++;
        plat->CBaddy.mpScript = pc;
        plat->mScaleYTimer = *pc++;
        plat->CBaddy.mpScript = pc;
        if (plat->mScaleYTimer != 0)
            plat->mScaleYAdjust = (static_cast<int>(p) << 12) /
                static_cast<int>(
                    static_cast<unsigned int>(plat->mScaleYTimer) * 100);
        else
            plat->mScaleYAdjust = (static_cast<int>(p) << 12) / 100;
        item->mScale.vy += plat->mScaleYAdjust;
        break;
    }

    case PlatformCommandScaleZ:
    {
        PLATFORM_ENABLE_SCALE();
        unsigned short *pc = plat->CBaddy.mpScript;
        short p = *pc++;
        plat->CBaddy.mpScript = pc;
        plat->mScaleZTimer = *pc++;
        plat->CBaddy.mpScript = pc;
        if (plat->mScaleZTimer != 0)
            plat->mScaleZAdjust = (static_cast<int>(p) << 12) /
                static_cast<int>(
                    static_cast<unsigned int>(plat->mScaleZTimer) * 100);
        else
            plat->mScaleZAdjust = (static_cast<int>(p) << 12) / 100;
        item->mScale.vz += plat->mScaleZAdjust;
        break;
    }

    case PlatformCommandSetBruceHealthAdjust:
    {
        unsigned short *pc = plat->CBaddy.mpScript;
        plat->mBruceHealthAdjust = pc[0];
        plat->CBaddy.mpScript = pc + 1;
        break;
    }

    case PlatformCommandSetBoundingBox:
    {
        unsigned int flags = plat->CBaddy.mSpecificFlags;
        unsigned short x = *plat->CBaddy.mpScript++;
        unsigned short y = *plat->CBaddy.mpScript++;
        unsigned short z = *plat->CBaddy.mpScript++;
        plat->CBaddy.mSpecificFlags = flags | PlatformHasBoundingBox;
        plat->mBoundingBox.vx = static_cast<unsigned int>(x) << 11;
        plat->mBoundingBox.vy = static_cast<unsigned int>(y) << 11;
        plat->mBoundingBox.vz = static_cast<unsigned int>(z) << 11;
        break;
    }

    case PlatformCommandWaitUntilTrodUpon:
        if (plat->mTrodUpon == 0)
        {
            plat->CBaddy.mpScript = plat->CBaddy.mpScript - 1;
            return 0;
        }
        break;

    case PlatformCommandMoveTo:
    {
        int *p_pos = PLATFORM_ALIGNED_SCRIPT(
            int, plat->CBaddy.mpScript);
        CVector pos;
        pos.vx = *p_pos++;
        pos.vy = *p_pos++;
        pos.vz = *p_pos++;
        int shift = 12;
        pos <<= shift;
        plat->CBaddy.mpScript = reinterpret_cast<unsigned short *>(p_pos);
        self->MoveTo(pos);
        break;
    }

    case PlatformCommandMoveToNode:
    {
        unsigned short *pc = plat->CBaddy.mpScript;
        unsigned short node = pc[0];
        plat->CBaddy.mpScript = pc + 1;
        if ((node & PlatformScriptVariableFlag) != 0)
        {
            PlatformVirtualTable *vtable =
                GetPlatformVirtualTable(self);
            node = vtable->GetVariable(self, node) & PlatformScriptWordMask;
        }
        CVector pos;
        Trig_GetPosition(pos, node);
        self->MoveTo(pos);
        break;
    }

    case PlatformCommandMoveRelative:
    {
        int *p_pos = PLATFORM_ALIGNED_SCRIPT(
            int, plat->CBaddy.mpScript);
        CVector pos;
        pos.vx = *p_pos++;
        pos.vy = *p_pos++;
        pos.vz = *p_pos++;
        int shift = 12;
        pos <<= shift;
        pos += PLATFORM_CONST_VECTOR_VIEW(item->mPos);
        plat->CBaddy.mpScript = reinterpret_cast<unsigned short *>(p_pos);
        self->MoveTo(pos);
        break;
    }

    case PlatformCommandDie:
    {
        PlatformVirtualTable *vtable = GetPlatformVirtualTable(self);
        vtable->Die(self);
        return 0;
    }

    case PlatformCommandShatter:
    {
        item->mFlags |= PlatformItemShattered;
        PlatformVirtualTable *vtable = GetPlatformVirtualTable(self);
        vtable->Die(self);
        Shatter_Item(
            PLATFORM_SHATTER_ITEM_POINTER(
                &self->CBaddy.CSuper.CBody.CItem),
            0, 0);
        return 0;
    }

    default:
        return Platform_CBaddyExecuteCommand(&plat->CBaddy, command);
    }
#undef PLATFORM_ENABLE_SCALE
    return 1;
}

/**
 * Consume platform-specific script variables, updating movement/orientation
 * fields directly and delegating unknown variables to CBaddy.
 *
 * @param self  Platform consuming the script variable.
 * @param variable Script variable identifier to consume.
 */
void CPlatform_SetVariable(
    CPlatform *self, unsigned short variable)
{
    variable &= PlatformScriptWordMask;
    CSVectorOwner *vecBase;
    int vecY;
    int vecZ;

    switch (variable)
    {
    case PlatformVariableApplyGravity:
    {
        u16 *script = self->CBaddy.mpScript;
        u8 value = *reinterpret_cast<u8 *>(script);
        self->CBaddy.mpScript = script + 1;
        self->CBaddy.mApplyGravity = value;
        break;
    }
    case PlatformVariableModelIndex:
    {
        u16 *script = self->CBaddy.mpScript;
        u16 value = *script;
        self->CBaddy.mpScript = script + 1;
        self->CBaddy.CSuper.CBody.CItem.mModel = value;
        break;
    }
    case PlatformVariableModelChecksum:
    {
        unsigned int *p_checksum = PLATFORM_ALIGNED_SCRIPT(
            unsigned int, self->CBaddy.mpScript);
        unsigned int model = Spool_GetModel(
            *p_checksum,
            self->CBaddy.CSuper.CBody.CItem.mRegion);
        u16 flags = self->CBaddy.CSuper.CBody.CItem.mFlags;
        self->CBaddy.CSuper.CBody.CItem.mModel = model;
        self->CBaddy.mpScript = reinterpret_cast<u16 *>(p_checksum + 1);
        flags &= 0xFFFE;
        self->CBaddy.CSuper.CBody.CItem.mFlags = flags;
        break;
    }
    case PlatformVariableVelocity:
    {
        s16 x = self->CBaddy.GetScriptValue();
        s16 y = self->CBaddy.GetScriptValue();
        s16 z = self->CBaddy.GetScriptValue();
        int vx = static_cast<int>(x) << 12;
        int vy = static_cast<int>(y) << 12;
        int vz = static_cast<int>(z) << 12;
        CVectorOwner *dest = &self->CBaddy.CSuper.CBody.mVel;
        dest->vx = vx;
        dest->vy = vy;
        dest->vz = vz;
        break;
    }
    case PlatformVariableAngles:
    {
        s16 x = self->CBaddy.GetScriptValue();
        vecY = self->CBaddy.GetScriptValue();
        // Each of these three script-vector cases needs the inherited $a0
        // binding; stripping the group changes nine instructions.
        register CBaddy *arg asm("$4") = &self->CBaddy;
        vecY <<= 16;
        vecY >>= 16;
        vecZ = arg->GetScriptValue();
        vecBase = &self->CBaddy.CSuper.CBody.CItem.mAngles;
        self->CBaddy.CSuper.CBody.CItem.mAngles.vx = x;
        goto storeShortVector;
    }
    case PlatformVariableAngularVelocity:
    {
        s16 x = self->CBaddy.GetScriptValue();
        vecY = self->CBaddy.GetScriptValue();
        register CBaddy *arg asm("$4") = &self->CBaddy;
        vecY <<= 16;
        vecY >>= 16;
        vecZ = arg->GetScriptValue();
        vecBase = &self->CBaddy.CSuper.CBody.mAngVel;
        self->CBaddy.CSuper.CBody.mAngVel.vx = x;
        goto storeShortVector;
    }
    case PlatformVariableAngularAcceleration:
    {
        s16 x = self->CBaddy.GetScriptValue();
        vecY = self->CBaddy.GetScriptValue();
        register CBaddy *arg asm("$4") = &self->CBaddy;
        vecY <<= 16;
        vecY >>= 16;
        vecZ = arg->GetScriptValue();
        vecBase = &self->CBaddy.CSuper.CBody.mAngAcc;
        self->CBaddy.CSuper.CBody.mAngAcc.vx = x;
        goto storeShortVector;
    }
storeShortVector:
        vecBase->vy = vecY;
        vecBase->vz = vecZ;
        break;
    default:
        Platform_CBaddySetVariable(&self->CBaddy, variable);
        break;
    }
}

/**
 * Read a platform-specific script variable or delegate it to CBaddy.
 *
 * @param self Platform supplying the requested script variable.
 * @param variable Script variable identifier.
 * @return The signed script value.
 */
short CPlatform_GetVariable(
    CPlatform *self, unsigned short variable)
{
    variable &= PlatformScriptWordMask;
    if (variable == PlatformVariableNotTrodUponTimer)
    {
        return self->mNotTrodUponScriptValue;
    }
    return Platform_CBaddyGetVariable(&self->CBaddy, variable);
}

/**
 * Test the skater's swept bounds against every active bouncing platform.
 *
 * @param pSkater Active skater being processed.
 */
void Platform_HandleBounce(CBruce *pSkater)
{
    int LP1 = pSkater->CSuper.CBody.mOldPos.vx;
    int RP1 = pSkater->CSuper.CBody.CItem.mPos.vx;
    if (RP1 < LP1)
    {
        int tmp = LP1;
        LP1 = RP1;
        RP1 = tmp;
    }
    LP1 -= PlatformCollisionMargin;
    RP1 += PlatformCollisionMargin;

    int UP1 = pSkater->CSuper.CBody.mOldPos.vy;
    int DP1 = pSkater->CSuper.CBody.CItem.mPos.vy;
    if (DP1 < UP1)
    {
        int tmp = UP1;
        UP1 = DP1;
        DP1 = tmp;
    }
    UP1 -= PlatformCollisionMargin;
    DP1 += PlatformCollisionMargin;

    int FP1 = pSkater->CSuper.CBody.mOldPos.vz;
    int BP1 = pSkater->CSuper.CBody.CItem.mPos.vz;
    if (BP1 < FP1)
    {
        int tmp = FP1;
        FP1 = BP1;
        BP1 = tmp;
    }
    FP1 -= PlatformCollisionMargin;
    BP1 += PlatformCollisionMargin;

    PlatformCItemOwner *pItem =
        reinterpret_cast<PlatformCItemOwner *>(EnvironmentalObjectList);
    while (pItem != 0)
    {
        if (pItem->mFlags & PlatformItemEnvironmental)
        {
            CPlatform *platform = reinterpret_cast<CPlatform *>(pItem);
            int tmp = platform->CBaddy.mSpecificFlags;
            if (tmp & PlatformBounceEnabled)
            {
                pItem->mFlags |= PlatformItemBounceContact;
                if (!(tmp & PlatformBouncing))
                {
                    SModel *m = PSXRegion[pItem->mRegion].ppModels[pItem->mModel];
                    short *pBox = ((PlatformModelBounds *)&m->Box)->Half;
                    int x = pItem->mPos.vx;
                    if (RP1 >= x + (static_cast<int>(pBox[1]) << 12) &&
                        x + (static_cast<int>(pBox[0]) << 12) >= LP1)
                    {
                        int z = pItem->mPos.vz;
                        if (BP1 >= z + (static_cast<int>(pBox[5]) << 12) &&
                            z + (static_cast<int>(pBox[4]) << 12) >= FP1)
                        {
                            int y = pItem->mPos.vy;
                            if (DP1 >= y + (static_cast<int>(pBox[3]) << 12) &&
                                y + (static_cast<int>(pBox[2]) << 12) >= UP1)
                            {
                                platform->Bounce(
                                    reinterpret_cast<CBruce *>(MechList),
                                    PLATFORM_CONST_VECTOR_VIEW(
                                        pSkater->CSuper.CBody.mVel));
                            }
                        }
                    }
                }
            }
        }
        pItem = pItem->mNextItem;
    }
}
