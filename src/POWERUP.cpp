// POWERUP.cpp -- Reconstructed translation unit
// Original: POWERUP.CPP
// Functions: 13 reconstructed
// Address range: 0x8002FB00 - 0x80030F28
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#define __SKIP_CPOWERUP_EXTERNS__

#define POWERUP_OWNER_DECLARATIONS
#include "POWERUP.h"
#undef POWERUP_OWNER_DECLARATIONS
#include "MAIN.h"
#include "globals.h"
#include "CITEM.h"

/** Return caller-provided storage for an owner constructor. */
inline void *operator new(unsigned int, void *address)
{
    return address;
}

struct CQuadBit;
struct SLight;
struct CBit;
struct PowerUpCItemOwner;

/** Deleting-destructor prefix shared by particle vtables. */
struct PowerUpBitVtable {
    void (*Reserved0)(CBit *bit);
    void (*Reserved1)(CBit *bit);
    void (*destroy)(CBit *bit, int flags);
};

/** Runtime interface slots exercised by the power-up owner. */
struct PowerUpItemVtable {
    void (*Reserved0)(PowerUpCItemOwner *item);
    void (*Reserved1)(PowerUpCItemOwner *item);
    void (*destroy)(PowerUpCItemOwner *item, int flags);
    void (*die)(PowerUpCItemOwner *item);
    void (*ai)(PowerUpCItemOwner *item);
    void (*everyFrame)(PowerUpCItemOwner *item);
    void (*doLightingSetup)(PowerUpCItemOwner *item);
    int (*hit)(PowerUpCItemOwner *item, int, const CVector &, int);
    void (*deleteStuff)(PowerUpCItemOwner *item);
    void (*Reserved9)(PowerUpCItemOwner *item);
};

/** Particle-base storage embedded at the start of CGlow. */
struct CBit {
    CBit *mPrevious;
    CBit *mNext;
    short mAge;
    unsigned short mLifetime;
    CVector mTweenPos;
    CVector mPos;
    CVector mVel;
    CVector mAcc;
    CFriction mFric;
    unsigned char mDead;
    unsigned short mFrigDeltaZ;
    unsigned char mProtected;
    unsigned char mType;
    const PowerUpBitVtable *_vf;

    /** Assign the bit's world-space position. */
    void SetPos(const CVector &position);
    static void operator delete(void *) asm("__dl__4CBitPv");
};

#include "CGLOW.h"

#include "LOCALTXT.h"

/** Composition-only item storage used by the power-up owner. */
struct PowerUpCItemOwner {
    unsigned short mFlags;
    unsigned short mInquiry;
    CVector mPos;
    CSVector mAngles;
    unsigned short mModel;
    char mDummyFrame;
    unsigned char mTintIndex;
    unsigned char mDummyAnim;
    unsigned char mRegion;
    PowerUpCItemOwner *mNextItem;
    unsigned int mRGB;
    CSVector mScale;
    unsigned int mTRN;
    PowerUpCItemOwner *mPreviousItem;
    unsigned int mStoredRGB;
    unsigned char mPredatorEffectType;
    unsigned char mPredatorEffectFade;
    unsigned short mType;
    SLight *mpLight;
    unsigned short mFlashTimer;
    short mFlash_dR;
    short mFlash_dG;
    short mFlash_dB;
    const PowerUpItemVtable *_vf;
};

/** Complete 264-byte CBody base embedded by CPowerUp. */
struct CBody {
    PowerUpCItemOwner CItem;
    CBody **mppOriginalList;
    unsigned short mInputFlags;
    unsigned short mCBodyFlags;
    CVector mVel;
    CVector mAcc;
    CFriction mFric;
    CSVector mAngVel;
    CSVector mAngAcc;
    CFriction mAngFric;
    CSVector mNormal;
    unsigned char mEventFlags;
    unsigned int mEllipsoidRGB;
    unsigned short mEllipsoidScale;
    short mEllipsoidOTPushback;
    CVector mShadowPos;
    CSVector mShadowNormal;
    CQuadBit *mpShadow;
    unsigned short mShadowScale;
    short mShadowDist;
    short mShadowThreshold;
    int mPushVal;
    unsigned short mRMinor;
    unsigned short mNode;
    unsigned short mCollision;
    short mHealth;
    unsigned int mPlayerDist;
    unsigned char mAIFrequency;
    unsigned char mAICount;
    CVector mOldPos;
    CVector mTweenPos;
    CVector mNewPos;
    CVector mDPos;
    CSVector mOldAngles;
    CSVector mNewAngles;
    CSVector mDAngles;

    CBody();
    /** Insert this body at the head of a linked body list. */
    void AttachTo(CBody **list);
    /** Remove this body from a linked body list. */
    void DeleteFrom(CBody **list);
    /** Enable shadow rendering for this body. */
    void ShadowOn();
    /** Destroy this body's shadow and clear its shadow flag. */
    void KillShadow();
    /** Return whether this body has been marked dead. */
    int IsDead() const;
};

/** Proven CBruce slices needed when awarding a pickup. */
struct CBruce {
    /** Start a new airborne trick sequence. */
    void Trick_TakeOff();
    /** Adds one named trick to the active scoring sequence. */
    void Trick_Trick(char *name, int points, int lock);
    /** Finalize the current trick sequence after landing. */
    void Trick_Land();

    CBody CBody;
    unsigned char mStorageBeforeInBail[3964 - sizeof(CBody)];
    int mInBail;
    unsigned char mStorageBeforePhysicsState[236];
    int mPhysicsState;
};

/** Power-up body and its owner-specific state. */
struct CPowerUp {
    CBody CBody;
    CGlow *mpGlow;
    unsigned short mGlowRadiusOrg;
    unsigned short mGlowRadiusAmp;
    unsigned char mHasNode;
    unsigned char mIs3d;
    unsigned char mDropping;
    unsigned char mDropForever;
    unsigned char mHoverHeight;
    unsigned short mNodeIndex;
    int mGroundY;
    CVector mOrgPos;
    unsigned short mT;
    unsigned short mMaxWobbleAmplitude;
    unsigned short mWobbleAmplitude;
    unsigned short mWobbleSpeed;
    unsigned short mLifetime;
    unsigned int mTimer;

    CPowerUp(unsigned short PowerupType, const CVector &Pos, const CVector &Vel,
             unsigned int Flags, int Lifetime);
    ~CPowerUp();
    /** Releases power-up storage through CItem's allocator. */
    static void operator delete(void *p);
    void AI();
    void CheckAge();
    void CreateBit();
    void DeleteStuff();
    void Die();
    void DoPhysics();
    void DontDisplay();
    void SetGravity(int g, int fric);
    void SetNode(int NodeIndex);
    void TakeEffect(struct CBruce *);
};

typedef char powerup_cbit_size_must_be_72[(sizeof(CBit) == 72) ? 1 : -1];
typedef char powerup_bit_vtable_size_must_be_12[
    (sizeof(PowerUpBitVtable) == 12) ? 1 : -1];
typedef char powerup_item_vtable_size_must_be_40[
    (sizeof(PowerUpItemVtable) == 40) ? 1 : -1];
typedef char powerup_cglow_size_must_be_104[(sizeof(CGlow) == 104) ? 1 : -1];
typedef char powerup_local_text_size_must_be_652[
    (sizeof(LocalText) == 652) ? 1 : -1];
typedef char powerup_citem_size_must_be_76[
    (sizeof(PowerUpCItemOwner) == 76) ? 1 : -1];
typedef char powerup_cbody_size_must_be_264[(sizeof(CBody) == 264) ? 1 : -1];
typedef char powerup_cbruce_slice_size_must_be_4208[(sizeof(CBruce) == 4208) ? 1 : -1];
typedef char powerup_cpowerup_size_must_be_312[(sizeof(CPowerUp) == 312) ? 1 : -1];

#include "BIT.h"
#include "SFX.h"
#include "OB.h"
#include "MESS.h"
#include "PANEL.h"
#include "SPOOL.h"
#include "TRIG.h"
#include "UTILS.h"

/** Release power-up storage through the inherited CItem allocator. */
inline void CPowerUp::operator delete(void *p)
{
    ::CItem::operator delete(p);
}

/** Target-proven power-up type values used by this owner. */
enum PowerUpType {
    PowerUpLetterK = 4,
    PowerUpLetterS = 5,
    PowerUpLetterA = 6,
    PowerUpLetterE = 10,
    PowerUpSpecial14 = 14,
    PowerUpLetterT = 15,
    PowerUpSecretTape = 16,
    PowerUpSpecial18 = 18,
    PowerUpTypeBonus100 = 21,
    PowerUpTypeBonus200 = 22,
    PowerUpTypeBonus500 = 23,
    PowerUpTypeBonus20Spot = 24,
    PowerUpTypeBonusFifty = 25,
    PowerUpTypeBonusRich = 26,
    PowerUpTypeBonusRollAgain = 27,
    PowerUpTypeBonusHead = 28,
    PowerUpTypeBonusShoes = 29,
    PowerUpTypeBonusFly = 30,
    PowerUpTypeBonusPants = 31,
    PowerUpTypeBonusBoard = 32,
    PowerUpMedalA = 0x664,
    PowerUpMedalB = 0x665,
    PowerUpMedalC = 0x666
};

/** Flags accepted by the CPowerUp constructor. */
enum PowerUpCreationFlag {
    PowerUpDropToGround = 4,
    PowerUpUseDropVelocity = 8,
    PowerUpDropForever = 0x10
};

/** Availability bits in CItem::mFlags. */
enum PowerUpItemFlag {
    PowerUpUnavailable = 1,
    PowerUpUnavailableToSecondPlayer = 0x40,
    PowerUpUnavailableToEveryone = 0x41
};

/** Death bit in CBody::mCBodyFlags. */
enum PowerUpBodyFlag {
    PowerUpBodyDead = 0x40
};

enum PowerUpConstant {
    PowerUpPickupVolume = 0x2000,
    PowerUpLetterSound = 0x14,
    PowerUpBonusSound = 0x12,
    PowerUpInfiniteLifetime = 0xFFFF,
    PowerUpDisplayDistance = 8000
};

// === External globals (absolute via incomplete array) ===
#include "MECHLIST.h"

// === External function declarations ===
extern "C" void CItem_InitItem(PowerUpCItemOwner *, char *)
    asm("InitItem__5CItemPc");

// === Extern "C" forward declarations ===
extern "C" void CBody_dtor(CBody *, int) asm("_._5CBody");
extern "C" void CPowerUp_DestructorAbi(CPowerUp *, int) asm("_._8CPowerUp");
extern "C" void CPowerUp_DieEntry(CPowerUp *) asm("Die__8CPowerUp");
extern "C" void CPowerUp_AIEntry(CPowerUp *) asm("AI__8CPowerUp");
extern "C" void CPowerUp_DeleteStuffEntry(CPowerUp *) asm("DeleteStuff__8CPowerUp");
extern "C" void CBody_DoLightingSetup(CBody *) asm("DoLightingSetup__5CBody");
extern "C" int CBody_Hit(CBody *, int, const CVector &, int)
    asm("Hit__5CBodyiRC7CVectori");

// === DoPhysics support declarations ===
#include "RCOSSIN.h"
/**
 * Bonus descriptions retained in POWERUP's read-only data contribution.
 *
 * With the owner's recovered `-G16` threshold, shorter pickup descriptions
 * stay in `.sdata`; these five longer descriptions precede the power-up
 * vtable in `.rdata`, as they do in the original object.
 */
static char PowerUpBonusRich[] __attribute__((section(".rdata"))) =
    "100 Bucks! You're rich!";
static char PowerUpBonusHead[] __attribute__((section(".rdata"))) =
    "You got head - excellent";
static char PowerUpBonusShoes[] __attribute__((section(".rdata"))) =
    "Gotta be the shoes";
static char PowerUpBonusFly[] __attribute__((section(".rdata"))) =
    "You'll look fly in this";
static char PowerUpBonusPants[] __attribute__((section(".rdata"))) =
    "Change yo pants!";

/**
 * Virtual dispatch table emitted by the power-up owner.
 */
extern "C" PowerUpItemVtable const __vt_8CPowerUp
    __attribute__((aligned(8))) = {
    0,
    0,
    (void (*)(PowerUpCItemOwner *, int))CPowerUp_DestructorAbi,
    (void (*)(PowerUpCItemOwner *))CPowerUp_DieEntry,
    (void (*)(PowerUpCItemOwner *))CPowerUp_AIEntry,
    (void (*)(PowerUpCItemOwner *))CBody_EveryFrame,
    (void (*)(PowerUpCItemOwner *))CBody_DoLightingSetup,
    (int (*)(PowerUpCItemOwner *, int, const CVector &, int))CBody_Hit,
    (void (*)(PowerUpCItemOwner *))CPowerUp_DeleteStuffEntry,
    0,
};


// === Function implementations (address order) ===

/**
 * Purpose: Initialize a world power-up, select its model, and add it to the
 * active power-up list.
 *
 * Address: 0x8002FB00
 * Mangled: __8CPowerUpUsRC7CVectorT2Uii
 *
 * @return `this` pointer (GCC 2.x ctor convention).
 */
CPowerUp::CPowerUp(unsigned short PowerupType, const CVector &Pos, const CVector &Vel,
                   unsigned int Flags, int Lifetime)
{
    CBody.CItem._vf = &__vt_8CPowerUp;
    mTimer = 0;
    CBody.CItem.mType = PowerupType;
    mHoverHeight = 0;
    mLifetime = Lifetime;
    mOrgPos = Pos;
    CBody.CItem.mPos = Pos;

    if (Flags & PowerUpUseDropVelocity)
        CBody.mVel.vy = -(60 << 12);
    else
        CBody.mVel = Vel;

    CBody.mFric.Set(5, 5, 5);
    CBody.mAngVel.vy = 100;
    CBody.mAcc.vy = 8 << 12;

    mWobbleSpeed = Rnd(20) + 90;
    mMaxWobbleAmplitude = 50;
    mWobbleAmplitude = 50;

    if (Flags & PowerUpDropForever)
    {
        mDropping = 1;
        mDropForever = 1;
    }
    else if (Flags & PowerUpDropToGround)
    {
        mDropping = 1;
        mGroundY = Utils_GetGroundHeight(mOrgPos, 0, PowerUpDisplayDistance);
    }

    CBody.mRMinor = 300;
    CBody.ShadowOn();
    CBody.mShadowThreshold = 300;
    CItem_InitItem(&CBody.CItem, "items");

    switch (CBody.CItem.mType)
    {
    case PowerUpMedalA:
        CItem_InitItem(&CBody.CItem, "skmedals");
        mIs3d = 1;
        CBody.CItem.mModel = Spool_GetModel(0x54636518, CurrentMedalsFileRegion);
        break;
    case PowerUpMedalB:
        CItem_InitItem(&CBody.CItem, "skmedals");
        mIs3d = 1;
        CBody.CItem.mModel = Spool_GetModel(0xBA6D0434, CurrentMedalsFileRegion);
        break;
    case PowerUpMedalC:
        CItem_InitItem(&CBody.CItem, "skmedals");
        mIs3d = 1;
        CBody.CItem.mModel = Spool_GetModel(0x2364558E, CurrentMedalsFileRegion);
        break;
    case PowerUpLetterS:
        mIs3d = 1;
        CBody.CItem.mModel = Spool_GetModel(0x311D55D4, CurrentItemsFileRegion);
        break;
    case PowerUpLetterK:
        mIs3d = 1;
        CBody.CItem.mModel = Spool_GetModel(0x2328A71C, CurrentItemsFileRegion);
        break;
    case PowerUpLetterA:
        mIs3d = 1;
        CBody.CItem.mModel = Spool_GetModel(0x2EBF22CA, CurrentItemsFileRegion);
        break;
    case PowerUpLetterT:
        mIs3d = 1;
        CBody.CItem.mModel = Spool_GetModel(0x34524351, CurrentItemsFileRegion);
        break;
    case PowerUpLetterE:
        mIs3d = 1;
        CBody.CItem.mModel = Spool_GetModel(0x29B68A16, CurrentItemsFileRegion);
        break;
    case PowerUpSecretTape:
    case PowerUpSpecial18:
        CBody.mAngVel.vy = 150;
        mIs3d = 1;
        CBody.CItem.mModel = Spool_GetModel(0x7C1B2C4A, CurrentItemsFileRegion);
        break;
    case PowerUpTypeBonus100:
        CBody.mAngVel.vy = 200;
        mIs3d = 1;
        CBody.CItem.mModel = Spool_GetModel(0x6F0F6B9F, CurrentItemsFileRegion);
        break;
    case PowerUpTypeBonus200:
        CBody.mAngVel.vy = 200;
        mIs3d = 1;
        CBody.CItem.mModel = Spool_GetModel(0xF6063A25, CurrentItemsFileRegion);
        break;
    case PowerUpTypeBonus500:
        CBody.mAngVel.vy = 200;
        mIs3d = 1;
        CBody.CItem.mModel = Spool_GetModel(0x81010AB3, CurrentItemsFileRegion);
        break;
    case PowerUpTypeBonus20Spot:
        CBody.mAngVel.vy = 200;
        mIs3d = 1;
        CBody.CItem.mModel = Spool_GetModel(0x694ED947, CurrentItemsFileRegion);
        break;
    case PowerUpTypeBonusFifty:
        CBody.mAngVel.vy = 200;
        mIs3d = 1;
        CBody.CItem.mModel = Spool_GetModel(0x260F4F80, CurrentItemsFileRegion);
        break;
    case PowerUpTypeBonusRich:
        CBody.mAngVel.vy = 200;
        mIs3d = 1;
        CBody.CItem.mModel = Spool_GetModel(0xCC4E141F, CurrentItemsFileRegion);
        break;
    case PowerUpTypeBonusRollAgain:
        CBody.mAngVel.vy = 200;
        mIs3d = 1;
        CBody.CItem.mModel = Spool_GetModel(0x483701E6, CurrentItemsFileRegion);
        break;
    case PowerUpTypeBonusHead:
        CBody.mAngVel.vy = 200;
        mIs3d = 1;
        CBody.CItem.mModel = Spool_GetModel(0x608E06C7, CurrentItemsFileRegion);
        break;
    case PowerUpTypeBonusShoes:
        CBody.mAngVel.vy = 200;
        mIs3d = 1;
        CBody.CItem.mModel = Spool_GetModel(0x10122D2A, CurrentItemsFileRegion);
        break;
    case PowerUpTypeBonusFly:
        CBody.mAngVel.vy = 200;
        mIs3d = 1;
        CBody.CItem.mModel = Spool_GetModel(0xC12ED64B, CurrentItemsFileRegion);
        break;
    case PowerUpTypeBonusPants:
        CBody.mAngVel.vy = 200;
        mIs3d = 1;
        CBody.CItem.mModel = Spool_GetModel(0x22908B06, CurrentItemsFileRegion);
        break;
    case PowerUpTypeBonusBoard:
        CBody.mAngVel.vy = 200;
        mIs3d = 1;
        CBody.CItem.mModel = Spool_GetModel(0x56891A69, CurrentItemsFileRegion);
        break;
    }

    if (GGame != 1)
    {
        switch (CBody.CItem.mType)
        {
        case PowerUpLetterK:
        case PowerUpLetterS:
        case PowerUpLetterA:
        case PowerUpLetterE:
        case PowerUpLetterT:
        case PowerUpSecretTape:
            mLifetime = 1;
            break;
        }
    }
    else
    {
        switch (CBody.CItem.mType)
        {
        case PowerUpLetterS:     GPowS = this; break;
        case PowerUpLetterK:     GPowK = this; break;
        case PowerUpLetterA:     GPowA = this; break;
        case PowerUpLetterT:     GPowT = this; break;
        case PowerUpLetterE:     GPowE = this; break;
        case PowerUpSecretTape:  GPowTape = this; break;
        }
    }

    CBody.AttachTo(&PowerUpList);
    if (mIs3d == 0)
        DontDisplay();

}

/**
 * GCC 2.x deleting-destructor entry for CPowerUp.
 *
 * Address:   0x8003018C
 * Mangled:   _._8CPowerUp
 * Demangled: CPowerUp::~CPowerUp()
 *
 */
CPowerUp::~CPowerUp()
{
    CPowerUp *self = this;
    self->CBody.CItem._vf = &__vt_8CPowerUp;
    self->CBody.DeleteFrom(&PowerUpList);
    self->DeleteStuff();
    CBody_dtor(&self->CBody, 0);
}

/**
 * Mark this power-up unavailable and release its display resources.
 *
 * Address:   0x800301F4
 * Mangled:   DontDisplay__8CPowerUp
 * Demangled: CPowerUp::DontDisplay(void)
 *
 */
void CPowerUp::DontDisplay()
{
    u16 flags = CBody.CItem.mFlags;
    flags |= PowerUpUnavailableToEveryone;
    CBody.CItem.mFlags = flags;
    CBody.CItem._vf->deleteStuff(&CBody.CItem);
}

/**
 * Set the vertical acceleration and damping shift.
 *
 * Address:   0x8003022C
 * Mangled:   SetGravity__8CPowerUpii
 * Demangled: CPowerUp::SetGravity(int, int)
 *
 * @param g  Vertical acceleration in world fixed-point units.
 * @param fric  Vertical velocity damping shift.
 */
void CPowerUp::SetGravity(int g, int fric)
{
    this->CBody.mAcc.vy = g;
    this->CBody.mFric.vy = fric;
}

/**
 * Request deferred creation of power-up particle effects.
 *
 * Address:   0x80030238
 * Mangled:   CreateBit__8CPowerUp
 * Demangled: CPowerUp::CreateBit(void)
 *
 */
void CPowerUp::CreateBit()
{
    UseCacheForBits = 1;
}

/**
 * Release the shadow and glow owned by this power-up.
 *
 * Address:   0x8003024C
 * Mangled:   DeleteStuff__8CPowerUp
 * Demangled: CPowerUp::DeleteStuff(void)
 *
 */
void CPowerUp::DeleteStuff()
{
    CBody.KillShadow();
    CGlow *glow = mpGlow;
    if (glow != 0)
    {
        glow->_vf->destroy(static_cast<CBit *>(glow), 3);
    }
    mpGlow = 0;
}

/**
 * Associate the power-up with a trigger node.
 *
 * Address:   0x8003029C
 * Mangled:   SetNode__8CPowerUpi
 * Demangled: CPowerUp::SetNode(int)
 *
 * @param NodeIndex  Rail node index.
 */
void CPowerUp::SetNode(int NodeIndex)
{
    mNodeIndex = NodeIndex;
    mHasNode = 1;
}

/**
 * Mark the body dead and hide its display resources once.
 *
 * Address:   0x800302AC
 * Mangled:   Die__8CPowerUp
 * Demangled: CPowerUp::Die(void)
 *
 */
void CPowerUp::Die()
{
    if (CBody.IsDead() == 0)
    {
        u16 flags = this->CBody.mCBodyFlags;
        flags |= PowerUpBodyDead;
        this->CBody.mCBodyFlags = flags;
        this->DontDisplay();
    }
}

/**
 * Advance falling or hover motion and refresh the pickup shadow.
 *
 * Address:   0x800302EC
 * Mangled:   DoPhysics__8CPowerUp
 * Demangled: CPowerUp::DoPhysics(void)
 *            Advances rotation, falling and hover motion, then updates the
 *            power-up's shadow position, distance, and scale.
 *
 */
void CPowerUp::DoPhysics()
{
    this->CBody.CItem.mAngles += this->CBody.mAngVel;
    this->CBody.CItem.mAngles.Mask();

    if (this->mDropping != 0) {
        if (this->mDropForever == 0 && this->mGroundY < 0 && this->mGroundY > -5) {
            if (TTime & 1) {
                int ground = Utils_GetGroundHeight(this->mOrgPos, 0,
                                                   PowerUpDisplayDistance);
                if (ground == -1) {
                    this->mGroundY--;
                    if (this->mGroundY < -4) this->mDropForever = 1;
                } else {
                    this->mGroundY = ground;
                }
            }
        }
        else {
            CVector *vel = &this->CBody.mVel;
            this->mOrgPos += *vel;
            this->CBody.mVel.vy += this->CBody.mAcc.vy;
            this->CBody.mVel.vy -= this->CBody.mVel.vy >> this->CBody.mFric.vy;
            if (this->mDropForever == 0) {
                if (this->mGroundY - (this->mHoverHeight << 12) < this->mOrgPos.vy &&
                    this->CBody.mVel.vy > 0) {
                    this->mOrgPos.vy = this->mGroundY - (this->mHoverHeight << 12);
                    vel->vx = 0;
                    vel->vy = 0;
                    vel->vz = 0;
                    this->mDropping = 0;
                }
            }
            this->CBody.CItem.mPos = this->mOrgPos;
        }
    }
    else {
        this->CBody.CItem.mPos = this->mOrgPos;
        this->CBody.CItem.mPos.vy +=
            ((this->mWobbleAmplitude * rcossin_tbl[(this->mT & 0xFFF) * 2]) / 4096) << 12;
        this->mT += this->mWobbleSpeed;
    }

    this->CBody.mShadowPos = this->mOrgPos;
    this->CBody.mShadowPos.vy = this->mGroundY;
    this->CBody.mShadowDist = (this->mGroundY - this->CBody.CItem.mPos.vy) >> 12;
    if (this->CBody.mShadowDist != 0) {
        this->CBody.mShadowScale = 140 - this->CBody.mShadowDist * 60 / 128;
    } else {
        this->CBody.mShadowScale = 0;
    }
}

/**
 * Age the pickup, animate its expiry blink, and destroy it at zero lifetime.
 *
 * Address:   0x80030530
 * Mangled:   CheckAge__8CPowerUp
 * Demangled: CPowerUp::CheckAge(void)
 *
 */
void CPowerUp::CheckAge()
{
    mTimer++;

    if (mLifetime == PowerUpInfiniteLifetime)
        return;

    if (mLifetime != 0)
        mLifetime--;

    if (mLifetime < 60)
        mWobbleAmplitude = mMaxWobbleAmplitude * mLifetime / 60;

    if (mLifetime > 30 && mLifetime < 60)
    {
        if (TTime & 2)
            CBody.CItem.mFlags &= ~PowerUpUnavailable;
        else
            DontDisplay();
    }

    if (mLifetime < 31)
    {
        if (TTime & 1)
            CBody.CItem.mFlags &= ~PowerUpUnavailable;
        else
            DontDisplay();
    }

    if (mLifetime == 0)
    {
        CBody.CItem._vf->die(&CBody.CItem);
    }
}

/**
 * Apply this pickup's letter, tape, bonus, and trigger effects.
 *
 * Address:   0x8003068C
 * Mangled:   TakeEffect__8CPowerUpP6CBruce
 * Demangled: CPowerUp::TakeEffect(CBruce *)
 *
 * Applies letter, tape, and bonus pickups to a skater, then pulses any linked
 * trigger and consumes pickups which had an effect.
 */
void CPowerUp::TakeEffect(CBruce *pMech)
{
    unsigned short unavailable;
    if (GNumberOfPlayers != 1 && &pMech->CBody == MechList)
        unavailable = CBody.CItem.mFlags & PowerUpUnavailableToSecondPlayer;
    else
        unavailable = CBody.CItem.mFlags & PowerUpUnavailable;

    int HadEffect = 0;
    if (unavailable)
        return;

    int sound = 0;
    switch (CBody.CItem.mType)
    {
    case PowerUpLetterK:
    case PowerUpLetterS:
    case PowerUpLetterA:
    case PowerUpLetterE:
    case PowerUpSpecial14:
    case PowerUpLetterT:
    case PowerUpSecretTape:
    {
        HadEffect = 1;
        Mess_DeleteAll();

        char *pMess = 0;
        switch (CBody.CItem.mType)
        {
        case PowerUpLetterS:
            SFX_PlayX(PowerUpLetterSound, PowerUpPickupVolume, 0);
            if (!Panel_Letter(0))
                pMess = GameText.pSPickup;
            break;
        case PowerUpLetterK:
            SFX_PlayX(PowerUpLetterSound, PowerUpPickupVolume, 0);
            if (!Panel_Letter(1))
                pMess = GameText.pKPickup;
            break;
        case PowerUpLetterA:
            SFX_PlayX(PowerUpLetterSound, PowerUpPickupVolume, 0);
            if (!Panel_Letter(2))
                pMess = GameText.pAPickup;
            break;
        case PowerUpLetterT:
            SFX_PlayX(PowerUpLetterSound, PowerUpPickupVolume, 0);
            if (!Panel_Letter(3))
                pMess = GameText.pTPickup;
            break;
        case PowerUpLetterE:
            SFX_PlayX(PowerUpLetterSound, PowerUpPickupVolume, 0);
            if (!Panel_Letter(4))
                pMess = GameText.pEPickup;
            break;
        case PowerUpSecretTape:
            Panel_Tape(3);
            pMess = 0;
            break;
        }

        if (pMess)
            Mess_Message(pMess, Messprog_Zoom, 1, 0, 0);
        break;
    }

    case PowerUpTypeBonus100:
    case PowerUpTypeBonus200:
    case PowerUpTypeBonus500:
    case PowerUpTypeBonus20Spot:
    case PowerUpTypeBonusFifty:
    case PowerUpTypeBonusRich:
    case PowerUpTypeBonusRollAgain:
    case PowerUpTypeBonusHead:
    case PowerUpTypeBonusShoes:
    case PowerUpTypeBonusFly:
    case PowerUpTypeBonusPants:
    case PowerUpTypeBonusBoard:
    {
        if (pMech->mInBail)
            break;

        HadEffect = 1;
        if (pMech->mPhysicsState == 0)
            pMech->Trick_TakeOff();

        int Pong = SFX_PlayX(PowerUpBonusSound, PowerUpPickupVolume, 0);
        switch (CBody.CItem.mType)
        {
        case PowerUpTypeBonus100:
            pMech->Trick_Trick("Bonus 100", -100, 2);
            SFX_SetVoicePitch(Pong, 900);
            break;
        case PowerUpTypeBonus200:
            pMech->Trick_Trick("Bonus 200", -200, 2);
            SFX_SetVoicePitch(Pong, 1114);
            break;
        case PowerUpTypeBonus500:
            pMech->Trick_Trick("Bonus 500", -500, 2);
            SFX_SetVoicePitch(Pong, 1500);
            break;
        case PowerUpTypeBonus20Spot:
            pMech->Trick_Trick("20 Spot", -100, 2);
            SFX_SetVoicePitch(Pong, 900);
            break;
        case PowerUpTypeBonusFifty:
            pMech->Trick_Trick("Fitty Bucks", -200, 2);
            SFX_SetVoicePitch(Pong, 1114);
            break;
        case PowerUpTypeBonusRich:
            pMech->Trick_Trick(PowerUpBonusRich, -500, 2);
            SFX_SetVoicePitch(Pong, 1500);
            break;
        case PowerUpTypeBonusRollAgain:
            pMech->Trick_Trick("Roll Another", -200, -2);
            SFX_SetVoicePitch(Pong, 1114);
            break;
        case PowerUpTypeBonusHead:
            pMech->Trick_Trick(PowerUpBonusHead, -200, 2);
            SFX_SetVoicePitch(Pong, 1114);
            break;
        case PowerUpTypeBonusShoes:
            pMech->Trick_Trick(PowerUpBonusShoes, -200, 2);
            SFX_SetVoicePitch(Pong, 1114);
            break;
        case PowerUpTypeBonusFly:
            pMech->Trick_Trick(PowerUpBonusFly, -200, 2);
            SFX_SetVoicePitch(Pong, 1114);
            break;
        case PowerUpTypeBonusPants:
            pMech->Trick_Trick(PowerUpBonusPants, -200, 2);
            SFX_SetVoicePitch(Pong, 1114);
            break;
        case PowerUpTypeBonusBoard:
            pMech->Trick_Trick("I'm Board", -200, 2);
            SFX_SetVoicePitch(Pong, 1114);
            break;
        }

        if (pMech->mPhysicsState == 0)
            pMech->Trick_Land();
        break;
    }
    }

    if (mHasNode)
        Trig_SendPulse(Trig_GetLinksPointer(mNodeIndex));

    if (HadEffect)
    {
        CBody.CItem._vf->die(&CBody.CItem);

        if (sound)
            SFX_PlayX(0, PowerUpPickupVolume, 0);
    }
}

/**
 * Update visibility, physics, and glow animation.
 *
 * Address:   0x80030AAC
 * Mangled:   AI__8CPowerUp
 * Demangled: CPowerUp::AI(void)
 *
 * Updates range culling and physics, creates the pickup glow on demand, and
 * animates its fringe radius.
 */
void CPowerUp::AI()
{
    CheckAge();

    if (GNumberOfPlayers == 1)
    {
        if (CBody.CItem.mFlags & PowerUpUnavailable)
            return;
        if (CBody.mPlayerDist > PowerUpDisplayDistance)
        {
            CBody.CItem._vf->deleteStuff(&CBody.CItem);
            return;
        }
    }
    else
    {
        if ((CBody.CItem.mFlags & PowerUpUnavailableToEveryone) ==
            PowerUpUnavailableToEveryone)
            return;

        if (CBody.mPlayerDist > PowerUpDisplayDistance &&
            Utils_CrapDist(
                MechList->CItem.mNextItem->mPos,
                CBody.CItem.mPos) > PowerUpDisplayDistance)
        {
            CBody.CItem._vf->deleteStuff(&CBody.CItem);
            return;
        }
    }

    DoPhysics();
    if (!mIs3d)
        CreateBit();

    if (!mpGlow)
    {
        // The target keeps base allocation and conditional construction separate.
        CGlow *glow = static_cast<CGlow *>(
            Bit_AllocateStorage(sizeof(CGlow)));
        if (glow)
            glow = ::new (glow) CGlow(4, 2);
        mpGlow = glow;

        mpGlow->mSkipTriangles = 1;
        mpGlow->SetCentreRGB(0, 0, 0);
        mpGlow->SetRGB(0, 0, 0);
        mpGlow->SetFringeRGB(1, 0, 0, 0);
        mpGlow->SetRadius(0);
        mpGlow->mProtected = 1;

        switch (CBody.CItem.mType)
        {
        case PowerUpSpecial18:
            for (int i = 0; i < mpGlow->mNumSections; i++)
            {
                if (i & 1)
                    mpGlow->SetRadialRGB(i, 0x60, 0x60, 0);
                else
                    mpGlow->SetRadialRGB(i, 0x40, 0x40, 0x40);
            }
            mpGlow->SetRGB(0x20, 0, 0);
            mpGlow->SetFringeRGB(1, 0, 0, 0);
            mGlowRadiusOrg = 10;
            mGlowRadiusAmp = 3;
            break;

        case PowerUpLetterK:
        case PowerUpLetterS:
        case PowerUpLetterA:
        case PowerUpLetterE:
        case PowerUpLetterT:
            for (int i = 0; i < mpGlow->mNumSections; i++)
            {
                if (i & 1)
                    mpGlow->SetRadialRGB(i, 0, 0, 0x60);
                else
                    mpGlow->SetRadialRGB(i, 0, 0, 0);
            }
            mpGlow->SetRGB(0, 0, 0);
            mpGlow->SetFringeRGB(1, 0, 0, 0);
            mGlowRadiusOrg = 60;
            mGlowRadiusAmp = 10;
            break;

        case PowerUpSecretTape:
            for (int i = 0; i < mpGlow->mNumSections; i++)
            {
                if (i & 1)
                    mpGlow->SetRadialRGB(i, 0x60, 0x60, 0);
                else
                    mpGlow->SetRadialRGB(i, 0x40, 0x40, 0x40);
            }
            mpGlow->SetRGB(0, 0, 0);
            mpGlow->SetFringeRGB(1, 0, 0, 0);
            mGlowRadiusOrg = 60;
            mGlowRadiusAmp = 10;
            break;

        case PowerUpTypeBonus100:
            mpGlow->SetFringeRGB(0, 0x30, 0x30, 0);
            mGlowRadiusOrg = 60;
            mGlowRadiusAmp = 10;
            break;

        case PowerUpTypeBonusFifty:
            mpGlow->SetFringeRGB(0, 0x30, 0x30, 0);
            mGlowRadiusOrg = 60;
            mGlowRadiusAmp = 10;
            break;

        case PowerUpTypeBonus200:
        case PowerUpTypeBonusRich:
            mpGlow->SetFringeRGB(0, 0x40, 0x20, 8);
            mGlowRadiusOrg = 60;
            mGlowRadiusAmp = 10;
            break;

        case PowerUpTypeBonus500:
        case PowerUpTypeBonus20Spot:
            mpGlow->SetFringeRGB(0, 0x30, 0, 0);
            mGlowRadiusOrg = 60;
            mGlowRadiusAmp = 10;
            break;
        }
    }

    static_cast<CBit *>(mpGlow)->SetPos(CBody.CItem.mPos);
    int m = TTime * 150;
    int width = mGlowRadiusOrg +
                mGlowRadiusAmp * rcossin_tbl[(m & 0xFFF) * 2] / 4096;
    mpGlow->SetFringeWidth(0, width);
    mpGlow->SetFringeWidth(1, width);
}

/**
 * Allocate and construct one world power-up.
 *
 * Address:   0x80030F28
 * Mangled:   PowerUp_Create__FUsRC7CVectorUiiR7CVector
 * Demangled: PowerUp_Create(unsigned short, const CVector &, unsigned int, int, CVector &)
 *
 * @param Type  Type code.
 * @param Pos  Position vector.
 * @param Flags  Creation behavior flags.
 * @param LifeTime  Number of frames to keep the pickup alive.
 * @param Vel  Initial velocity for a moving pickup.
 * @return The constructed power-up, or null when allocation fails.
 */
CPowerUp *PowerUp_Create(unsigned short Type, const CVector &Pos,
                         unsigned int Flags, int LifeTime, CVector &Vel)
{
    // Preserve the target's explicit null check before invoking the constructor.
    CPowerUp *pNewPowerUp = static_cast<CPowerUp *>(
        ::CItem::operator new(sizeof(CPowerUp)));
    if (pNewPowerUp)
    {
        pNewPowerUp = ::new (pNewPowerUp)
            CPowerUp(Type, Pos, Vel, Flags, LifeTime);
    }
    return pNewPowerUp;
}
