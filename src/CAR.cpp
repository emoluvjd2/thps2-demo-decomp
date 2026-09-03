// CAR.cpp -- Reconstructed translation unit
// Original: CAR.CPP
// Functions: 8 reconstructed entries
// Address range: 0x8006341C - 0x800643F8
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#include "CAR.h"
#include "MAIN.h"
#include "CITEM.h"
#include "MECH.h"
#include "MIPS.h"
#include "M3D.h"
#include "M3DUTILS.h"
#define OB_LAYOUT_ONLY
#define OB_OWNER_LAYOUT
#include "OB.h"
#undef OB_OWNER_LAYOUT
#undef OB_LAYOUT_ONLY
#include "CBODY.h"
#include "CSUPER.h"
// Owner-local traffic-car hierarchy used by CAR.cpp.
#include "globals.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include "SMODEL.h"
#include "PSXREGION.h"
#include <INLINE_C.H>
#include "gte_extensions.h"
#define BADDY_LAYOUT_DECLARATIONS
#include "BADDY.h"
#undef BADDY_LAYOUT_DECLARATIONS
#include "BIT.h"

/** Narrow natural interface to the transient-bit allocator owned by BIT. */
struct CBit
{
    /** Allocate zero-initialized storage from the transient-bit heap. */
    static void *operator new(unsigned int size);
};

/** Whether a bail should clamp the skater's velocity. */
enum ELimitBailVel {
    DO_LIMIT = 0,
    DONT_LIMIT = 1
};

/** Opaque skater method surface used by car collisions. */
struct CBruce
{
    /** Adds one named trick to the active scoring sequence. */
    void Trick_Trick(char *name, int points, int lock);
    /** Emit one blood burst from a skater body hook. */
    void BloodSplat(int bodyPart, int maxBloodBits);
    /** Drive one controller vibration actuator for this skater. */
    void Vibrate(int time, int motor, int value);
    /** Copy the current heading matrix into the skater angle vector. */
    void SetAnglesFromHeading();
    /** Reset this skater into the requested bail state. */
    void Bail(int bailNumber, ELimitBailVel limitVelocity);
};
struct CGrindSpark;

/**
 * Access the five packed words consumed by the PSYQ GTE rotation registers.
 * A shared pointer cursor changes 329 instructions and 14 relocations, so the
 * cast stays centralized while each target load remains independently shaped.
 */
#define CAR_ROTATION_WORD(matrix, index) (((const int *)&(matrix))[index])

/** Script subtype codes for the six traffic-car assets. */
enum ECarType {
    CAR_TYPE_TAXI = 0xD5,
    CAR_TYPE_POLICE = 0xD6,
    CAR_TYPE_VAN = 0xD7,
    CAR_TYPE_CABLE = 0xD8,
    CAR_TYPE_KART = 0xD9,
    CAR_TYPE_MAR = 0xDA,
};

/** Named fixed-point and angle constants used by traffic-car simulation. */
enum {
    CAR_FIXED_ONE = 0x1000,
    CAR_FULL_TURN = 0x1000,
    CAR_HALF_TURN = 0x800,
    CAR_ANGLE_MASK = 0xFFF,
    CAR_MODEL_BOX_MARGIN = 20 * CAR_FIXED_ONE,
    CAR_VAN_HEIGHT = 70 * CAR_FIXED_ONE,
    CAR_CABLE_HEIGHT = 100 * CAR_FIXED_ONE,
    CAR_ROAD_HEIGHT = 110 * CAR_FIXED_ONE,
    CAR_MINIMUM_SPEED = 80 * CAR_FIXED_ONE,
    CAR_ACCELERATION = CAR_FIXED_ONE,
    CAR_ANIMATION_SPEED = 16 * CAR_FIXED_ONE,
    CAR_OVERHEAD_Y_IMPULSE = 30 * CAR_FIXED_ONE,
    CAR_SIDE_Y_IMPULSE = 45 * CAR_FIXED_ONE,
    CAR_LAUNCH_Y_OFFSET = 45 * CAR_FIXED_ONE / 2,
    CAR_SOUND_VOLUME = 0x2000,
    CAR_TURN_SNAP_THRESHOLD = 128,
    CAR_RANDOM_SIDE_COUNT = 16,
};

/** Sound-table entries selected by car construction, impact, and horn use. */
enum ECarSound {
    CAR_SOUND_OVERHEAD_HIT = 0x28,
    CAR_SOUND_TAXI_HORN_FIRST = 0x6E,
    CAR_SOUND_TAXI_ENGINE = 0x71,
    CAR_SOUND_BODY_HIT = 0x72,
    CAR_SOUND_TAXI_IMPACT_FIRST = 0x73,
    CAR_SOUND_CABLE = 0x78,
    CAR_SOUND_POLICE_HORN = 0x7A,
    CAR_SOUND_POLICE_IMPACT_FIRST = 0x7D,
    CAR_SOUND_VAN_ENGINE = 0x81,
    CAR_SOUND_VAN_HORN = 0x82,
    CAR_SOUND_VAN_IMPACT = 0x83,
    CAR_SOUND_CABLE_ENGINE = 0x86,
    CAR_SOUND_MAR_ENGINE = 0xDD,
    CAR_SOUND_NONE = -1,
};

#include "CCAR.h"

/** Target-era external compiler view of the traffic-car sound volume. */
extern int CarVol;

/** Target-sized virtual dispatch record owned by CAR.cpp. */
struct CCarVTable {
    void *reservedClass;
    void *reservedRtti;
    void (*destroy)(CCar *, int);
    void (*die)(CBaddy *);
    void (*ai)(CCar *);
    void (*everyFrame)(CBodyOwner *);
    void (*lightingSetup)(CBodyOwner *);
    int (*hit)(CBodyOwner *, int, const CVector &, int);
    void (*deleteStuff)(CBodyOwner *);
    int (*executeCommand)(CCar *, unsigned short);
    void (*setVariable)(CBaddy *, unsigned short);
    short (*getVariable)(CBaddy *, unsigned short);
};

struct CBruceCarOwner {
    CSuperOwner CSuper;
};

/**
 * Player fields consulted while resolving a car collision.
 *
 * This remains an owner-local slice: the intervening gameplay state is opaque
 * until the wider CBruce layout is proven across its other owning systems.
 */
struct CBruceCarCollisionOwner {
    CSuperOwner CSuper;
    unsigned char _padToIgnoreCar[0xA7C - sizeof(CSuperOwner)];
    CCar *mIgnoreCar;
    unsigned char _padToInBail[0xF7C - 0xA80];
    int mInBail;
    unsigned char _padToPhysicsState[0x106C - 0xF80];
    int mPhysicsState;
};

/** Packed six-halfword interpretation used by the car collision helper. */
struct CarModelBounds {
    short Half[6];
};

/** Video state prefix used to reject split-screen playback modes. */
struct SVideoInfo {
    int VideoMode;
    unsigned char _remainingVideoState[188];
};

typedef char car_item_size_must_be_76[(sizeof(CItemOwner) == 76) ? 1 : -1];
typedef char car_body_size_must_be_264[(sizeof(CBodyOwner) == 264) ? 1 : -1];
typedef char car_super_size_must_be_400[(sizeof(CSuperOwner) == 400) ? 1 : -1];
typedef char car_owner_size_must_be_540[(sizeof(CCar) == 540) ? 1 : -1];
typedef char car_vtable_size_must_be_48[(sizeof(CCarVTable) == 48) ? 1 : -1];
typedef char car_bruce_collision_slice_size_must_be_0x1070[
    (sizeof(CBruceCarCollisionOwner) == 0x1070) ? 1 : -1];
typedef char car_video_info_size_must_be_192[(sizeof(SVideoInfo) == 192) ? 1 : -1];

/* CAR-owned virtual methods use readable source names at their target ABIs. */
namespace CarMethodABI
{
extern "C" void AI__4CCar(CCar *);
}
#define CCar_Update CarMethodABI::AI__4CCar

/** Addressable destructor entry used by CAR's owner-local virtual table. */
void CCar_Destroy(CCar *, int) asm("_._4CCar");

/** Reinterpret the common owner-local vector storage as its public type. */
static inline CVector &Car_Vector(CVectorOwner &vector)
{
    return reinterpret_cast<CVector &>(vector);
}

/** Reinterpret a public vector as the identical owner-local storage type. */
static inline CVectorOwner &Car_OwnerVector(CVector &vector)
{
    return reinterpret_cast<CVectorOwner &>(vector);
}

/** Reinterpret the identical owner-local short-vector storage for BIT's API. */
static inline const CSVector &Car_ShortVector(const CSVectorOwner &vector)
{
    return *(const CSVector *)&vector;
}

/** Obtain CAR's collision-specific player view at the same object address. */
#define CAR_COLLISION_SKATER(skater) ((CBruceCarCollisionOwner *)(skater))

/** Obtain the common player prefix used by horn-distance checks. */
#define CAR_SKATER_PREFIX(skater) ((CBruceCarOwner *)(skater))

#include "M3DMATHS.h"

#define CBADDY_DECLARE_DESTRUCTOR
#define CBADDY_DESTRUCTOR_NAME Car_DestroyBaddy
#define CBADDY_DESTRUCTOR_RECEIVER CCar
#include "CBADDYABI.h"
namespace CarInitItemABI
{
extern "C" void InitItem__5CItemPc(CCar *, char *);
}
void Car_InitVanItem(CCar *, char *) asm("InitItem__5CItemPc");
#define Car_InitItem CarInitItemABI::InitItem__5CItemPc

#include "SFX.h"
#include "OB.h"
#include "BIT2.h"
#include "UTILS.h"
#include "TRIG.h"

// === Forward type declarations ===

/** Return the magnitude of a signed scalar without introducing a branch. */
static inline int CarAbs(int value)
{
    int sign = value >> 31;
    return (value ^ sign) - sign;
}

// === Free function declarations ===
static void CollideWithSkater(CCar *pCar, CBruce *pSkater, const MATRIX &tm, short *pBox);
static void CollideWithSkaters(CCar *pCar);
static void HonkAtSkaters(CCar *pCar, CVector Front);
#include "CVECTOROPS.h"
/*
 * ZeroVector really holds a 12-byte CVector, but this TU sees it through a
 * small (<= -G8) declaration.  That is what makes cc1plus emit an unsplit
 * `la` for its address in the spark loop below; a full-size declaration
 * splits the address into a %hi/%lo pair whose %hi half loop-invariant
 * motion then hoists out of the loop into a callee-saved register.
 */
#define ZEROVECTOR_EIGHT_BYTE_DECLARATION
#include "ZEROVECTOR.h"
#undef ZEROVECTOR_EIGHT_BYTE_DECLARATION

/** Recover the complete vector stored behind ZeroVector's small-data view. */
static inline CVector &Car_ZeroVector()
{
    return *(CVector *)&ZeroVector;
}

// === Extern list declarations ===
#include "LOCALTXT.h"

// Car model names loaded from the original small-data contribution.
extern char DAT_800EA844[];
extern char DAT_800EA858[];
extern char DAT_800EA860[];
extern char DAT_800EA868[];
extern char DAT_800EA870[];

extern "C" {
/**
 * Car model names in their original small-data owner contribution.
 *
 * The five incomplete-array aliases above preserve the target absolute
 * address materialization for the eight-byte names.  The twelve-byte police
 * name already exceeds CAR's -G8 threshold and is referenced directly.
 */
namespace CarAddressViewOwners
{
static char DAT_800EA844[8]
    __attribute__((section(".sdata"), aligned(4))) = "c_taxi";
static char DAT_800EA84C[12]
    __attribute__((section(".sdata"), aligned(4))) = "c_police";
static char DAT_800EA858[8]
    __attribute__((section(".sdata"), aligned(4))) = "c_van";
static char DAT_800EA860[8]
    __attribute__((section(".sdata"), aligned(4))) = "c_cable";
static char DAT_800EA868[8]
    __attribute__((section(".sdata"), aligned(4))) = "c_kart";
static char DAT_800EA870[8]
    __attribute__((section(".sdata"), aligned(4))) = "c_mar";
}
}

#include "GSKATER_STATE.h"

/**
 * Car virtual dispatch table in its original read-only owner section.
 */
CCarVTable __vt_4CCar
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        CCar_Destroy,
        (void (*)(CBaddy *))&CBaddy::Die,
        (void (*)(CCar *))&CCar::AI,
        (void (*)(CBodyOwner *))&CBody::EveryFrame,
        (void (*)(CBodyOwner *))&CBody::DoLightingSetup,
        (int (*)(CBodyOwner *, int, const CVector &, int))&CBody::Hit,
        (void (*)(CBodyOwner *))&CBody::DeleteStuff,
        (int (*)(CCar *, unsigned short))&CCar::ExecuteCommand,
        (void (*)(CBaddy *, unsigned short))&CBaddy::SetVariable,
        (short (*)(CBaddy *, unsigned short))&CBaddy::GetVariable,
};

// === Function implementations (address order) ===

/**
 * Construct a traffic car from its script entry.
 *
 * The subtype selects the model and looping engine sound. The car is attached
 * to the baddy list and placed at the first road segment linked from its node.
 *
 * @param pInfo Serialized position and angle data.
 * @param NodeIndex Initial road-node index.
 * @param Type Traffic-car subtype.
 */
CCar::CCar(short *pInfo, int NodeIndex, int Type)
{
    CSuper.CBody.CItem.mType = Type;
    /*
     * GCC otherwise selects $2 for this address and commutes the adjacent
     * type/update stores.  Direct assignment, an ordinary local, and both
     * store orders were measured; the best clean form still differs in three
     * instructions and two relocation positions.
     */
    register CCarVTable *carVtable asm("$3") = &__vt_4CCar;
    CSuper.CBody.CItem._vf = carVtable;
    mLoopingSoundUpdate = 1;

    int soundId;
    switch (Type)
    {
    case CAR_TYPE_TAXI:
        Car_InitItem(this, DAT_800EA844);
        soundId = CAR_SOUND_TAXI_ENGINE;
        goto play_sound;
    case CAR_TYPE_POLICE:
        mLoopingSoundUpdate = 0;
        Car_InitItem(this, CarAddressViewOwners::DAT_800EA84C);
        break;
    case CAR_TYPE_VAN:
        Car_InitVanItem(this, DAT_800EA858);
        soundId = CAR_SOUND_VAN_ENGINE;
        goto play_sound;
    case CAR_TYPE_CABLE:
        Car_InitItem(this, DAT_800EA860);
        soundId = CAR_SOUND_CABLE_ENGINE;
        goto play_sound;
    case CAR_TYPE_KART:
        Car_InitItem(this, DAT_800EA868);
        soundId = CAR_SOUND_VAN_ENGINE;
        goto play_sound;
    case CAR_TYPE_MAR:
        Car_InitItem(this, DAT_800EA870);
        soundId = CAR_SOUND_MAR_ENGINE;
        goto play_sound;
    }
    goto after_sound;

play_sound:
    mLoopingSound = SFX_PlayPos(
        soundId, Car_Vector(CSuper.CBody.CItem.mPos), 0);

after_sound:
    if (mLoopingSoundUpdate != 0)
    {
        SFX_ModifyVol(mLoopingSound, 0, 0);
    }

    CarVol = 300;
    ::CBody *body = reinterpret_cast< ::CBody *>(this);
    body->AttachTo(&BaddyList);
    CSuper.mAnimSpeed = CAR_ANIMATION_SPEED;
    reinterpret_cast< ::CSuper *>(&CSuper)->CycleAnim(0, 1);

    mGroundToOrigin = 52;
    mOriginToEye = 32;
    CSuper.CBody.mHealth = 60;
    short *pos = body->SquirtPos(pInfo);
    body->SquirtAngles(pos);

    CSuper.CBody.mNode = NodeIndex;
    unsigned short *links = Trig_GetLinksPointer(NodeIndex);
    mCurrentRoadSegment = links[1];
    Trig_GetPosition(
        Car_Vector(CSuper.CBody.CItem.mPos),
        mCurrentRoadSegment);

    CSuper.CBody.CItem.mPos.y -= CAR_ROAD_HEIGHT;
}

/**
 * Destroy a traffic car and detach it from the global baddy list.
 *
 * GCC 2.x supplies the hidden in-charge flag and invokes the class allocator
 * when destruction also owns the car's storage.
 *
 * @param self Car being destroyed.
 * @param __in_chrg Hidden GCC ownership flags.
 */
void CCar_Destroy(CCar *self, int __in_chrg)
{
    self->CSuper.CBody.CItem._vf = &__vt_4CCar;
    reinterpret_cast<CBody *>(self)->DeleteFrom(&BaddyList);
    Car_DestroyBaddy(self, 0);
    if (__in_chrg & 1)
    {
        ::CItem::operator delete(self);
    }
}

/**
 * Compute the shortest signed difference between two 12-bit angles.
 *
 * @param StartAngle Starting angle.
 * @param EndAngle Destination angle.
 * @return Wrapped difference in the half-open range [-2048, 2048).
 */
int GetDeltaAngle(int StartAngle, int EndAngle)
{
    int d = EndAngle - StartAngle;
    if (d < -CAR_HALF_TURN)
    {
        d += CAR_FULL_TURN;
    }
    if (d >= CAR_HALF_TURN)
    {
        d -= CAR_FULL_TURN;
    }
    return d;
}

/**
 * Collide with a single skater against the car's transformed model box.
 *
 * @param pCar  Car to operate on.
 * @param pSkater  Active skater being processed.
 * @param tm Inverse car rotation used to transform the skater path.
 * @param pBox Six signed model-box limits in min/max axis pairs.
 */
static void CollideWithSkater(CCar *pCar, CBruce *pSkater, const MATRIX &tm, short *pBox)
{
    CBruceCarCollisionOwner *skater = CAR_COLLISION_SKATER(pSkater);

    if (pCar == skater->mIgnoreCar)
        return;
    if (skater->mInBail != 0 && skater->mPhysicsState != 0)
        return;
    // The original uses an unsigned range check, which also rejects a
    // negative sentinel returned by the coarse distance helper.
    if ((unsigned int)Utils_CrapDist(
            Car_Vector(skater->CSuper.CBody.CItem.mPos),
            Car_Vector(pCar->CSuper.CBody.CItem.mPos)) >= 500)
        return;

    int OldX;
    int OldY;
    int OldZ;
    int NewX;
    int NewY;
    int NewZ;

    OldX = skater->CSuper.CBody.mOldPos.x -
        pCar->CSuper.CBody.CItem.mPos.x;
    OldY = skater->CSuper.CBody.mOldPos.y -
        pCar->CSuper.CBody.CItem.mPos.y;
    OldZ = skater->CSuper.CBody.mOldPos.z -
        pCar->CSuper.CBody.CItem.mPos.z;
    NewX = skater->CSuper.CBody.CItem.mPos.x -
        pCar->CSuper.CBody.CItem.mPos.x;
    NewY = skater->CSuper.CBody.CItem.mPos.y -
        pCar->CSuper.CBody.CItem.mPos.y;
    NewZ = skater->CSuper.CBody.CItem.mPos.z -
        pCar->CSuper.CBody.CItem.mPos.z;

    gte_ldrot0(CAR_ROTATION_WORD(tm, 0));
    gte_ldrot1(CAR_ROTATION_WORD(tm, 1));
    gte_ldrot2(CAR_ROTATION_WORD(tm, 2));
    gte_ldrot3(CAR_ROTATION_WORD(tm, 3));
    gte_ldrot4(CAR_ROTATION_WORD(tm, 4));

    VECTOR OldPos;
    OldX >>= 12;
    gte_ldir1(OldX);
    OldY >>= 12;
    gte_ldir2(OldY);
    OldZ >>= 12;
    gte_ldir3(OldZ);
    gte_rtir_sf0();
    gte_stlvnl(&OldPos);

    VECTOR Pos;
    NewX >>= 12;
    gte_ldir1(NewX);
    NewY >>= 12;
    gte_ldir2(NewY);
    NewZ >>= 12;
    gte_ldir3(NewZ);
    gte_rtir_sf0();
    gte_stlvnl(&Pos);

    int XMin = OldPos.vx;
    int XMax = Pos.vx;
    if (XMax < XMin)
    {
        int tmp = XMin;
        XMin = XMax;
        XMax = tmp;
    }
    XMin -= CAR_MODEL_BOX_MARGIN;
    XMax += CAR_MODEL_BOX_MARGIN;

    int YMin = OldPos.vy;
    int YMax = Pos.vy;
    if (YMax < YMin)
    {
        int tmp = YMin;
        YMin = YMax;
        YMax = tmp;
    }
    YMin -= CAR_MODEL_BOX_MARGIN;
    YMax += CAR_MODEL_BOX_MARGIN;

    int ZMin = OldPos.vz;
    int ZMax = Pos.vz;
    if (ZMax < ZMin)
    {
        int tmp = ZMin;
        ZMin = ZMax;
        ZMax = tmp;
    }
    ZMin -= CAR_MODEL_BOX_MARGIN;
    ZMax += CAR_MODEL_BOX_MARGIN;

    if (XMax < pBox[1] * CAR_FIXED_ONE ||
        pBox[0] * CAR_FIXED_ONE < XMin ||
        ZMax < pBox[5] * CAR_FIXED_ONE ||
        pBox[4] * CAR_FIXED_ONE < ZMin ||
        YMax < pBox[3] * CAR_FIXED_ONE ||
        pBox[2] * CAR_FIXED_ONE < YMin)
        return;

    int CarHeight = 0;
    if (pCar->CSuper.CBody.CItem.mType == CAR_TYPE_VAN)
        CarHeight = CAR_VAN_HEIGHT;
    else if (pCar->CSuper.CBody.CItem.mType == CAR_TYPE_CABLE)
        CarHeight = CAR_CABLE_HEIGHT;

    if (skater->mInBail == 0 &&
        skater->mPhysicsState == 1 &&
        CarHeight <
            pCar->CSuper.CBody.CItem.mPos.y -
            skater->CSuper.CBody.CItem.mPos.y)
    {
        skater->CSuper.CBody.mVel.x >>= 1;
        skater->CSuper.CBody.mVel.y >>= 1;
        skater->CSuper.CBody.mVel.z >>= 1;
        skater->CSuper.CBody.mVel.y -= CAR_OVERHEAD_Y_IMPULSE;

        pSkater->Vibrate(8, 1, 150);
        SFX_Grunt(pSkater);
        SFX_PlayX(CAR_SOUND_OVERHEAD_HIT, CAR_SOUND_VOLUME, 0);
        pSkater->Trick_Trick(GameText.pCarPlant, -1000, 0);
        int canSpawnBits = BitCount < 200;
        skater->mIgnoreCar = pCar;

        if (canSpawnBits)
        {
            pSkater->SetAnglesFromHeading();
            Bit_SetSparkTrajectory(
                Car_ShortVector(skater->CSuper.CBody.CItem.mAngles));

            CSVector cone;
            cone.vx = 450;
            cone.vy = 900;
            cone.vz = 0;
            Bit_SetSparkTrajectoryCone(cone);

            UseCacheForBits = 0;
            for (int i = 0; i < 20; i++)
            {
                CGrindSpark *spark =
                    static_cast<CGrindSpark *>(CBit::operator new(
                        BIT2_GRIND_SPARK_STORAGE_SIZE));
                if (spark != 0)
                {
                    Bit2_ConstructGrindSpark(
                        spark, reinterpret_cast<CBody *>(pSkater), Car_ZeroVector(),
                        -40, CAR_FIXED_ONE, 5);
                }
            }
            UseCacheForBits = 1;
        }
        return;
    }

    int t = Rnd(CAR_RANDOM_SIDE_COUNT) >> 3;
    switch (t)
    {
    case 0:
    {
        int vx = pCar->CSuper.CBody.mVel.x;
        int vz = pCar->CSuper.CBody.mVel.z;
        int absX = CarAbs(vx);
        int absZ = CarAbs(vz);
        if (absZ < absX)
        {
            vx += vx >> 1;
            int offset = Rnd(vx >> 1) + (vx >> 2);
            if ((Rnd(CAR_RANDOM_SIDE_COUNT) >> 3) != 0)
                offset = -offset;
            vz += offset;
        }
        else
        {
            vz += vz >> 1;
            int offset = Rnd(vz >> 1) + (vz >> 2);
            if ((Rnd(CAR_RANDOM_SIDE_COUNT) >> 3) != 0)
                offset = -offset;
            vx += offset;
        }
        skater->CSuper.CBody.mVel.x = vx;
        // Direct member use changes the target's three-instruction load order.
        int carVelY = pCar->CSuper.CBody.mVel.y;
        skater->CSuper.CBody.mVel.z = vz;
        skater->CSuper.CBody.mVel.y = carVelY - CAR_LAUNCH_Y_OFFSET;
        break;
    }
    case 1:
    {
        // Folding this load changes 69 instructions and 14 relocations.
        int carVelX = pCar->CSuper.CBody.mVel.x;
        skater->CSuper.CBody.mVel.y = -CAR_SIDE_Y_IMPULSE;
        skater->CSuper.CBody.mVel.x = -carVelX >> 2;
        skater->CSuper.CBody.mVel.z =
            -pCar->CSuper.CBody.mVel.z >> 2;
        break;
    }
    }

    skater->mIgnoreCar = pCar;
    pSkater->Bail(1, DO_LIMIT);
    pSkater->BloodSplat(1, 100);
    pSkater->Vibrate(12, 1, 255);
    SFX_Grunt(pSkater);
    SFX_PlayX(CAR_SOUND_BODY_HIT, CAR_SOUND_VOLUME, 0);

    /*
     * The impact sound is chosen from the car model type, which is biased
     * down to a zero-based index first.  The subtraction is a separate
     * statement so it happens in place, in the register the type was loaded
     * into, exactly as the original does.
     */
    int carType = pCar->CSuper.CBody.CItem.mType;
    carType -= CAR_TYPE_TAXI;
    switch (carType)
    {
    case CAR_TYPE_TAXI - CAR_TYPE_TAXI:
        SFX_PlayX(
            Rnd(4) + CAR_SOUND_TAXI_IMPACT_FIRST, CAR_SOUND_VOLUME, 0);
        break;
    case CAR_TYPE_POLICE - CAR_TYPE_TAXI:
        SFX_PlayX(
            Rnd(4) + CAR_SOUND_POLICE_IMPACT_FIRST, CAR_SOUND_VOLUME, 0);
        break;
    case CAR_TYPE_VAN - CAR_TYPE_TAXI:
        SFX_PlayX(CAR_SOUND_VAN_IMPACT, CAR_SOUND_VOLUME, 0);
        break;
    case CAR_TYPE_CABLE - CAR_TYPE_TAXI:
        SFX_PlayX(CAR_SOUND_CABLE, CAR_SOUND_VOLUME, 0);
        break;
    case CAR_TYPE_KART - CAR_TYPE_TAXI:
        SFX_PlayX(CAR_SOUND_VAN_IMPACT, CAR_SOUND_VOLUME, 0);
        break;
    case CAR_TYPE_MAR - CAR_TYPE_TAXI:
        SFX_PlayX(CAR_SOUND_NONE, CAR_SOUND_VOLUME, 0);
        break;
    }
}

/**
 * Check the car against every active skater in the scene.
 *
 * @param pCar  Car to operate on.
 */
static void CollideWithSkaters(CCar *pCar)
{
    MATRIX tm;
    SModel *m;
    short *pBox;

    if ((GVInfo1.VideoMode != 2) && (GVInfo2.VideoMode != 2)) {
        m = PSXRegion[pCar->CSuper.CBody.CItem.mRegion].ppModels[4];
        pBox = ((CarModelBounds *)&m->Box)->Half;
        M3dMaths_TransposeMat(
            reinterpret_cast<unsigned short *>(&pCar->CSuper.mTransform),
            reinterpret_cast<unsigned short *>(&tm));
        CollideWithSkater(pCar, GSkater[0], tm, pBox);
        if (GSkater[1] != 0) {
            CollideWithSkater(pCar, GSkater[1], tm, pBox);
        }
    }
    return;
}

/**
 * Sounds the car horn when a skater enters the wedge in front of the car.
 *
 * The lock prevents the horn from repeatedly sounding while a skater remains
 * in range; it is cleared after every skater leaves the wedge.
 *
 * @param pCar Car whose position, type, and horn lock are tested.
 * @param Front Forward vector defining the horn's detection wedge.
 */
static void HonkAtSkaters(CCar *pCar, CVector Front)
{
    int Honk = 0;

    if (GVInfo1.VideoMode == 2)
        return;
    if (GVInfo2.VideoMode == 2)
        return;

    int Skater = 0;
    if (GNumberOfPlayers > 0)
    {
        // Indexing GSkater instead changes 93 instructions and four relocs;
        // the target advances this pointer cursor directly.
        CBruce **ppSkater = GSkater;
        for (; Skater < GNumberOfPlayers; ppSkater++, Skater++)
        {
            if (static_cast<int>(Utils_CrapDist(
                    Car_Vector(pCar->CSuper.CBody.CItem.mPos),
                    Car_Vector(CAR_SKATER_PREFIX(*ppSkater)->CSuper.CBody.CItem.mPos))) < 2000)
            {
                int LeftZ = Front.vz;
                int VecX = CAR_SKATER_PREFIX(*ppSkater)->CSuper.CBody.CItem.mPos.x -
                    pCar->CSuper.CBody.CItem.mPos.x;
                int VecZ = CAR_SKATER_PREFIX(*ppSkater)->CSuper.CBody.CItem.mPos.z -
                    pCar->CSuper.CBody.CItem.mPos.z;
                int RightX;
                if (Mips_DotProd2Sign(
                        Front.vx - LeftZ, VecX, Front.vx + LeftZ, VecZ) > 0)
                {
                    RightX = Front.vz + Front.vx;
                    if (Mips_DotProd2Sign(RightX, VecX, Front.vz - Front.vx, VecZ) > 0)
                    {
                        Honk = 1;
                        break;
                    }
                }
            }
        }
    }

    if (Honk)
    {
        if (pCar->mHonkLock == 0)
        {
            switch (pCar->CSuper.CBody.CItem.mType)
            {
            case CAR_TYPE_TAXI:
                SFX_PlayX(
                    Rnd(3) + CAR_SOUND_TAXI_HORN_FIRST,
                    CAR_SOUND_VOLUME, 0);
                break;
            case CAR_TYPE_POLICE:
                SFX_PlayX(CAR_SOUND_POLICE_HORN, CAR_SOUND_VOLUME, 0);
                break;
            case CAR_TYPE_VAN:
                SFX_PlayX(CAR_SOUND_VAN_HORN, CAR_SOUND_VOLUME, 0);
                break;
            case CAR_TYPE_CABLE:
                SFX_PlayX(CAR_SOUND_CABLE, CAR_SOUND_VOLUME, 0);
                break;
            case CAR_TYPE_KART:
                SFX_PlayX(CAR_SOUND_VAN_HORN, CAR_SOUND_VOLUME, 0);
                break;
            case CAR_TYPE_MAR:
                SFX_PlayX(CAR_SOUND_NONE, CAR_SOUND_VOLUME, 0);
                break;
            default:
                goto set_lock;
            }
        set_lock:
            pCar->mHonkLock = 1;
        }
    }
    else
    {
        pCar->mHonkLock = 0;
    }
}

/**
 * Advances a traffic car along the linked road-node graph.  The car retains
 * any movement left after crossing a segment, rebuilds its velocity for the
 * next segment, eases its speed near a node, and turns its body toward the
 * resulting road direction before testing whether it should sound its horn.
 *
 * @param car Traffic car being advanced.
 */
void CCar_Update(CCar *car)
{
    CVector SegStart;
    CVector SegEnd;
    CVector SegDir;
    int Speed;
    int InitialSpeed;
    int RemainingSpeed;
    int NextSegment;
    int Length;
    int t;
    int MajorAxis;
    int FirstTimeThroughLoop;

    if (car->mLoopingSoundUpdate != -1)
    {
        // Reloading the member at the call changes 284 instructions and
        // three relocations by disrupting the method's register allocation.
        unsigned int loopingSound = car->mLoopingSound;
        if (loopingSound != 0)
        {
            int remainder = XFrames % car->mLoopingSoundUpdate;
            int t = SFXLEVEL;
            SFXLEVEL = SFXLEVEL * CarVol / 100;
            if (remainder == 0)
            {
                SFX_ModifyPos(
                    loopingSound,
                    Car_Vector(car->CSuper.CBody.CItem.mPos), 0);
            }
            SFXLEVEL = t;
        }
    }

    FirstTimeThroughLoop = 1;
    // CSuperOwner is CAR's target-proven view of the inherited CSuper object.
    M3d_BuildTransform(reinterpret_cast<CSuper *>(&car->CSuper));
    CollideWithSkaters(car);

    do
    {
        unsigned short *links =
            Trig_GetLinksPointer(car->mCurrentRoadSegment);
        NextSegment = links[1];

        Trig_GetPosition(SegStart, car->mCurrentRoadSegment);
        SegStart.vy -= CAR_ROAD_HEIGHT;
        Trig_GetPosition(SegEnd, NextSegment);
        SegEnd.vy -= CAR_ROAD_HEIGHT;

        SegDir = SegEnd - SegStart;
        M3dUtils_NormaliseVector(&SegDir);

        if (FirstTimeThroughLoop)
        {
            FirstTimeThroughLoop = 0;
            Speed = InitialSpeed = Mips_CVecDotProd(
                &SegDir, reinterpret_cast<CVector *>(
                    &car->CSuper.CBody.mVel));
            if (Speed < 0)
            {
                Speed = 0;
            }
            if (Speed < CAR_MINIMUM_SPEED)
            {
                Speed += CAR_ACCELERATION;
            }

            car->CSuper.CBody.mVel.x =
                Mips_FixMul(SegDir.vx, Speed);
            car->CSuper.CBody.mVel.y =
                Mips_FixMul(SegDir.vy, Speed);
            car->CSuper.CBody.mVel.z =
                Mips_FixMul(SegDir.vz, Speed);
        }
        else
        {
            car->CSuper.CBody.mVel.x =
                Mips_FixMul(SegDir.vx, InitialSpeed);
            car->CSuper.CBody.mVel.y =
                Mips_FixMul(SegDir.vy, InitialSpeed);
            car->CSuper.CBody.mVel.z =
                Mips_FixMul(SegDir.vz, InitialSpeed);
        }

        MajorAxis = 0;
        if (CarAbs(SegDir.vy) > CarAbs(SegDir.vx))
        {
            MajorAxis = 1;
            if (CarAbs(SegDir.vy) < CarAbs(SegDir.vz))
            {
                MajorAxis = 2;
            }
        }
        else
        {
            if (CarAbs(SegDir.vx) < CarAbs(SegDir.vz))
            {
                MajorAxis = 2;
            }
        }

        switch (MajorAxis)
        {
        case 0:
            Length = ((SegEnd.vx - SegStart.vx) / SegDir.vx) << 12;
            t = ((car->CSuper.CBody.CItem.mPos.x - SegStart.vx) /
                 SegDir.vx) << 12;
            break;

        case 1:
            Length = ((SegEnd.vy - SegStart.vy) / SegDir.vy) << 12;
            t = ((car->CSuper.CBody.CItem.mPos.y - SegStart.vy) /
                 SegDir.vy) << 12;
            break;

        case 2:
            Length = ((SegEnd.vz - SegStart.vz) / SegDir.vz) << 12;
            t = ((car->CSuper.CBody.CItem.mPos.z - SegStart.vz) /
                 SegDir.vz) << 12;
            break;
        }

        if (Length - (Length >> 4) < t)
        {
            Speed -= Speed >> 4;
            car->CSuper.CBody.mVel.x -=
                car->CSuper.CBody.mVel.x >> 4;
            car->CSuper.CBody.mVel.y -=
                car->CSuper.CBody.mVel.y >> 4;
            car->CSuper.CBody.mVel.z -=
                car->CSuper.CBody.mVel.z >> 4;
        }

        t += Speed;
        if (t >= 0 && t <= Length)
        {
            car->CSuper.CBody.CItem.mPos.x +=
                Mips_FixMul(SegDir.vx, Speed);
            car->CSuper.CBody.CItem.mPos.y +=
                Mips_FixMul(SegDir.vy, Speed);
            car->CSuper.CBody.CItem.mPos.z +=
                Mips_FixMul(SegDir.vz, Speed);
            RemainingSpeed = 0;
        }
        else
        {
            Trig_GetLinksPointer(NextSegment);
            RemainingSpeed = t - Length;
            car->mCurrentRoadSegment = NextSegment;
            car->CSuper.CBody.CItem.mPos = Car_OwnerVector(SegEnd);
        }

    } while ((Speed = RemainingSpeed) != 0);

    int TargetAngle = ratan2(-SegDir.vx, -SegDir.vz) & CAR_ANGLE_MASK;
    int d = GetDeltaAngle(
        car->CSuper.CBody.CItem.mAngles.vy, TargetAngle);
    if (CarAbs(d) < CAR_TURN_SNAP_THRESHOLD)
    {
        car->CSuper.CBody.CItem.mAngles.vy = TargetAngle;
    }
    else
    {
        car->CSuper.CBody.CItem.mAngles.vy =
            (car->CSuper.CBody.CItem.mAngles.vy + (d >> 2)) &
            CAR_ANGLE_MASK;
    }

    TargetAngle = ratan2(SegDir.vy, CAR_FIXED_ONE) & CAR_ANGLE_MASK;
    d = GetDeltaAngle(
        car->CSuper.CBody.CItem.mAngles.vx, TargetAngle);
    if (CarAbs(d) < CAR_TURN_SNAP_THRESHOLD)
    {
        car->CSuper.CBody.CItem.mAngles.vx = TargetAngle;
    }
    else
    {
        car->CSuper.CBody.CItem.mAngles.vx =
            (car->CSuper.CBody.CItem.mAngles.vx + (d >> 3)) &
            CAR_ANGLE_MASK;
    }

    HonkAtSkaters(car, SegDir);
}

/**
 * Accept a scripted car command.
 *
 * Traffic cars currently acknowledge every command without additional work.
 *
 * @param command Command identifier.
 * @return One to report that the command was handled.
 */
int CCar::ExecuteCommand(unsigned short command)
{
    (void)this;
    (void)command;
    return 1;
}
