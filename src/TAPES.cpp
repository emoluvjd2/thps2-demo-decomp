// Tape pickup ownership, progression messaging, hover physics, and creation.

#include "TAPES.h"
#include "MAIN.h"
#include "OB.h"
#include "CBODY.h"
#include "POWERUP.h"
#include "TRIG.h"
#include "CAREER.h"
#include "SPOOL.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include <INLINE_C.H>
#include "gte_extensions.h"
#include "globals.h"
#include "CITEM.h"
#include "UTILS.h"

/** Return caller-provided storage for construction. */
inline void *operator new(unsigned int, void *storage)
{
    return storage;
}

/** Read one packed 32-bit word for a scalar GTE transfer macro. */
#define TAPES_PACKED_WORD(value, index) \
    (reinterpret_cast<const int *>(&(value))[index])

struct CBody;
struct CTapeGlowLayout;
struct SLight;

/** One progression message associated with a tape-count milestone. */
struct STapeMessage
{
    int essential;
    char *Line1;
    char *Line2;
    char *Line3;
};

typedef void (*CTapeDestroyMethod)(CTape *, int);
typedef void (*CTapeBodyMethod)(CBody *);
typedef int (*CTapeHitMethod)(CBody *, int, const CVector &, int);
typedef void (*CTapeDeleteStuffMethod)(CTape *);

/** Typed dispatch table owned by CTape. */
struct CTapeVTable
{
    int Reserved0;
    int Reserved1;
    CTapeDestroyMethod Destroy;
    CTapeBodyMethod Die;
    CTapeBodyMethod AI;
    CTapeBodyMethod EveryFrame;
    CTapeBodyMethod DoLightingSetup;
    CTapeHitMethod Hit;
    CTapeDeleteStuffMethod DeleteStuff;
};

typedef void (*CTapeGlowDeleteMethod)(CTapeGlowLayout *, int);

/** Prefix of the glow vtable needed by CTape's cleanup path. */
struct CTapeGlowVTable
{
    int Reserved0;
    int Reserved1;
    CTapeGlowDeleteMethod Destroy;
};

#include "SCAMERA.h"

/** Particle prefix used to dispatch deletion of a tape's glow effect. */
struct CTapeBitLayout
{
    CTapeBitLayout *mPrevious;
    CTapeBitLayout *mNext;
    s16 mAge;
    u16 mLifetime;
    CVector mTweenPos;
    CVector mPos;
    CVector mVel;
    CVector mAcc;
    CFriction mFric;
    u8 mDead;
    u16 mFrigDeltaZ;
    u8 mProtected;
    u8 mType;
    CTapeGlowVTable *_vf;
};

/** Glow prefix needed by the tape owner's delete path. */
struct CTapeGlowLayout
{
    CTapeBitLayout CBit;
};

/** CItem fields used directly by tape physics and creation. */
struct CTapeItemLayout
{
    u16 mFlags;                     // 0x00
    u16 mInquiry;                   // 0x02
    CVector mPos;                   // 0x04
    CSVector mAngles;               // 0x10
    u16 mModel;                     // 0x16
    char mDummyFrame;               // 0x18
    u8 mTintIndex;                  // 0x19
    u8 mDummyAnim;                  // 0x1a
    u8 mRegion;                     // 0x1b
    CTapeItemLayout *mNextItem;     // 0x1c
    u32 mRGB;                       // 0x20
    CSVector mScale;                // 0x24
    u8 _pad2a[2];                   // 0x2a
    u32 mTRN;                       // 0x2c
    CTapeItemLayout *mPreviousItem; // 0x30
    u32 mStoredRGB;                 // 0x34
    u8 mPredatorEffectType;         // 0x38
    u8 mPredatorEffectFade;         // 0x39
    u16 mType;                      // 0x3a
    SLight *mpLight;                // 0x3c
    u16 mFlashTimer;                // 0x40
    s16 mFlash_dR;                  // 0x42
    s16 mFlash_dG;                  // 0x44
    s16 mFlash_dB;                  // 0x46
    CTapeVTable *_vf;               // 0x48
};

/** CBody slice preserving the original 264-byte base-class extent. */
struct CTapeBodyLayout
{
    CTapeItemLayout CItem;          // 0x00
    CTapeBodyLayout **mppOriginalList; // 0x4c
    u16 mInputFlags;                // 0x50
    u16 mCBodyFlags;                // 0x52
    CVector mVel;                   // 0x54
    CVector mAcc;                   // 0x60
    CFriction mFric;                // 0x6c
    u8 _pad6f;                      // 0x6f
    CSVector mAngVel;               // 0x70
    u8 _unused76[62];               // 0x76..0xb3
    u16 mRMinor;                    // 0xb4
    u8 _unusedb6[82];               // 0xb6..0x107
};

/** Tape pickup layout recovered from original/MAIN_STRUCTS.h. */
struct CTape
{
    static int mOkayToDisplayContinueButton;
    static int mOkayToDisplayTitle;

    /** Constructs one tape pickup and attaches it to the power-up list. */
    CTape(unsigned short PowerupType, const CVector &Pos,
          const CVector &Pos2, int Size1, int Size2,
          const CVector &Vel, unsigned int Flags, int Lifetime);
    /** Allocates tape storage through the original CItem allocator. */
    static void *operator new(unsigned int size) throw()
    {
        return ::CItem::operator new(size);
    }
    /** Destroys this tape and its owned glow state. */
    ~CTape();
    /** Releases tape storage through the original CItem allocator. */
    static void operator delete(void *p) { ::CItem::operator delete(p); }
    /** Reports whether the tape title may be drawn this frame. */
    static int DisplayTitle();
    /** Reports whether the continue prompt may be drawn this frame. */
    static int DisplayContinueButton();
    /** Advances tape animation and progression messaging. */
    void AI(int tape_no);
    /** Hides this tape and releases its display state. */
    void DontDisplay();
    /** Destroys the glow effect owned by this tape. */
    void DeleteStuff();
    /** Requests the shared particle cache for this tape's 2D effect. */
    void CreateBit();
    /** Advances this tape's hover rotation and wobble. */
    void DoPhysics();

    CTapeBodyLayout CBody;          // 0x000
    CTapeGlowLayout *mpGlow;        // 0x108
    u16 mGlowRadiusOrg;             // 0x10c
    u16 mGlowRadiusAmp;             // 0x10e
    STapeMessage *mMessageTable;    // 0x110
    int mSessionTapeNum;            // 0x114
    int mTapeNum;                   // 0x118
    int mTextX;                     // 0x11c
    int mTextY;                     // 0x120
    int mSize;                      // 0x124
    int mSize1;                     // 0x128
    int mSize2;                     // 0x12c
    CVector mVec1;                  // 0x130
    CVector mVec2;                  // 0x13c
    u8 mHasNode;                    // 0x148
    u8 mIs3d;                       // 0x149
    u8 mDropping;                   // 0x14a
    u8 mDropForever;                // 0x14b
    u8 mHoverHeight;                // 0x14c
    u16 mNodeIndex;                 // 0x14e
    int mGroundY;                   // 0x150
    CVector mOrgPos;                // 0x154
    u16 mT;                         // 0x160
    u16 mMaxWobbleAmplitude;        // 0x162
    u16 mWobbleAmplitude;           // 0x164
    u16 mWobbleSpeed;               // 0x166
    u16 mLifetime;                  // 0x168
    u32 mTimer;                     // 0x16c
};

typedef char tapes_item_size_must_be_76[
    (sizeof(CTapeItemLayout) == 0x4c) ? 1 : -1];
typedef char tapes_body_size_must_be_264[
    (sizeof(CTapeBodyLayout) == 0x108) ? 1 : -1];
typedef char tapes_bit_size_must_be_72[
    (sizeof(CTapeBitLayout) == 0x48) ? 1 : -1];
typedef char tapes_owner_size_must_be_368[
    (sizeof(CTape) == 0x170) ? 1 : -1];
typedef char tapes_vtable_size_must_be_36[
    (sizeof(CTapeVTable) == 36) ? 1 : -1];
typedef char tapes_glow_vtable_prefix_size_must_be_12[
    (sizeof(CTapeGlowVTable) == 12) ? 1 : -1];

/** Power-up type values handled by the tape owner. */
enum TapePowerUpType
{
    TapePowerUpLetterK = 4,
    TapePowerUpLetterS = 5,
    TapePowerUpLetterA = 6,
    TapePowerUpLetterE = 10,
    TapePowerUpLetterT = 15,
    TapePowerUpSecretTape = 16,
    TapePowerUpSpecial18 = 18,
    TapePowerUpMedalA = 0x664,
    TapePowerUpMedalB = 0x665,
    TapePowerUpMedalC = 0x666
};

enum TapeConstant
{
    TapeUnavailableToEveryone = 0x41,
    TapeInfiniteLifetime = 0xFFFF,
    TapeFixedFractionBits = 12,
    TapeTrigAngleMask = 0xFFF,
    TapeTrigEntryStride = 2
};

// === External globals ===
#include "CURRENTCAMERA.h"
#include "ZEROVECTOR.h"

// === External function declarations ===
extern "C" void Tape_DestroyBody(CTapeBodyLayout *, int)
    asm("_._5CBody");
extern "C" void Tape_InitItem(CTapeItemLayout *, const char *)
    asm("InitItem__5CItemPc");

CVector operator+(const CVector &, const CVector &);
CVector operator-(const CVector &, const CVector &);
CVector operator*(const CVector &, const int &);
CVector operator/(const CVector &, const int &);
#include "MESS.h"

extern "C" void Tape_BodyDie(CBody *) asm("Die__5CBody");
extern "C" void Tape_BodyAI(CBody *) asm("AI__5CBody");
extern "C" void Tape_BodyLighting(CBody *) asm("DoLightingSetup__5CBody");
extern "C" int Tape_BodyHit(CBody *, int, const CVector &, int)
    asm("Hit__5CBodyiRC7CVectori");

// === External globals (vtable, linked list) ===
extern "C" CTape *GTape[5] __attribute__((section(".data")));
#define BITSTATE_DECLARE_USE_CACHE_FOR_BITS
#include "BITSTATE.h"

// === External globals (absolute via incomplete array) ===
#include "RCOSSIN.h"


// === Extern "C" forward declarations ===
extern "C" void Tape_DestructorAbi(CTape *, int) asm("_._5CTape");
extern "C" void Tape_DeleteStuffAbi(CTape *) asm("DeleteStuff__5CTape");

/**
 * Career-progression messages displayed after collecting level tapes.
 *
 * Repeated lines deliberately share their literals, matching the original
 * compiler's constant pooling and the pointer relationships in MAIN.SYM.
 */
extern "C" STapeMessage TapeMessage[30] = {
    {0, "Great run! Earn more tapes", "by completing the five",
     "challenges in each level."},
    {1, "New level! You've unlocked", "the SCHOOL. ", 0},
    {1, "NEW DECK now available.", "Stats increased! ", 0},
    {0, "Collect 1 more tape to", "visit the MALL. ", 0},
    {1, "Congratulations! You've", "unlocked the MALL. ", 0},
    {0, "Just two more tapes earns", "an invite to COMPETITION #1. ", 0},
    {1, "NEW DECK now available.", "Stats increased! ", 0},
    {1, "New level! Head to the SKATEPARK",
     "to enter the Windy City Slam. ", 0},
    {0, "4 more tapes for a trip", "to the DOWNHILL JAM.", 0},
    {1, "NEW DECK now available.", "Stats increased! ", 0},
    {0, "Just 2 more tapes to unlock", "the next level... ", 0},
    {0, "One more tape and you're", "off to the DOWNHILL JAM.", 0},
    {1, "New level! You've unlocked", "the DOWNHILL JAM. ", 0},
    {0, "Five more tapes and it's a", "road trip to BURNSIDE.", 0},
    {1, "NEW DECK now available.", "Stats increased! ", 0},
    {0, "2 more tapes and you'll be", "airing it out over BURNSIDE.", 0},
    {0, "Just 1 more tape earns a trip", "to Portland. BURNSIDE awaits.", 0},
    {1, "Congratulations! You've", "unlocked BURNSIDE! ", 0},
    {0, "Rumors of a top secret", "competition abound. 7 to go.", 0},
    {0, "Location of secret ramp unknown.", "6 more tapes should help.", 0},
    {1, "NEW DECK now available. ", "Stats increased! ", 0},
    {0, "The truth is out there", "at 4 more tapes.", 0},
    {0, "Transmissions coming from the", "Southwest. 3 more tapes to go.", 0},
    {1, "The FINAL DECK is now ", "available. Stats at maximum! ", 0},
    {0, "Almost there! Just 1 more tape",
     "earns a secret competition invite!", 0},
    {1, "Congratulations! Prepare", "yourself for ROSWELL.", 0},
    {0, "Only 3 tapes remain", "in the entire game!", 0},
    {0, "The law is on to you and",
     "moving fast. Just 2 tapes left\x85 ", 0},
    {0, "The cops are closing in.", "Stay alert. Only 1 tape to go\x85 ", 0},
    {1, "You're busted!", "OFFICER DICK has arrived!", 0},
};

/**
 * Tape virtual dispatch table in its original read-only owner section.
 */
extern "C" CTapeVTable __vt_5CTape
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        Tape_DestructorAbi,
        Tape_BodyDie,
        Tape_BodyAI,
        CBody_EveryFrame,
        Tape_BodyLighting,
        Tape_BodyHit,
        Tape_DeleteStuffAbi,
};

// === Function implementations (address order) ===

/**
 * Reports whether the tape title may be drawn this frame.
 *
 * @return Nonzero while title display is enabled.
 */
int CTape::DisplayTitle()
{
    return CTape::mOkayToDisplayTitle != 0;
}

/**
 * Reports whether the continue prompt may be drawn this frame.
 *
 * @return Nonzero while continue-prompt display is enabled.
 */
int CTape::DisplayContinueButton()
{
    return CTape::mOkayToDisplayContinueButton != 0;
}

/**
 * Constructs a tape pickup in caller-provided storage.
 *
 * @param PowerupType Pickup type to initialize.
 * @param Pos Initial hover position.
 * @param Pos2 Final interpolation position.
 * @param Size1 Initial scale.
 * @param Size2 Final interpolation scale.
 * @param Vel Initial velocity.
 * @param Flags Reserved construction flags.
 * @param Lifetime Initial lifetime counter.
 */
CTape::CTape(
    unsigned short PowerupType, const CVector &Pos, const CVector &Pos2,
    int Size1, int Size2, const CVector &Vel, unsigned int Flags,
    int Lifetime)
{
    CTape *self = this;
    (void)Flags;

    ::new (reinterpret_cast< ::CBody *>(&self->CBody)) ::CBody;
    self->CBody.CItem._vf = &__vt_5CTape;

    self->mSize = Size1;
    self->mSize1 = Size1;
    short Scale = static_cast<short>(Size1);
    self->mSize2 = Size2;
    self->mVec1 = Pos;
    self->mVec2 = Pos2;
    CSVector *scale = &self->CBody.CItem.mScale;
    scale->vx = Scale;
    scale->vy = Scale;
    scale->vz = Scale;
    self->mTimer = 0;
    self->CBody.CItem.mType = PowerupType;
    self->mLifetime = Lifetime;
    self->mTapeNum = 0;
    self->mSessionTapeNum = 0;
    self->mOrgPos = Pos;
    self->CBody.CItem.mPos = Pos;
    self->CBody.mVel = Vel;
    self->CBody.mFric.Set(5, 5, 5);
    self->CBody.mAngVel.vy = 100;
    self->CBody.mAcc.vy = 8 << TapeFixedFractionBits;
    self->mWobbleSpeed = Rnd(20) + 90;
    self->mMaxWobbleAmplitude = 50;
    self->mWobbleAmplitude = 50;
    self->CBody.mRMinor = 300;

    Tape_InitItem(&self->CBody.CItem, "items");

    switch (self->CBody.CItem.mType)
    {
    case TapePowerUpMedalA:
        Tape_InitItem(&self->CBody.CItem, "skmedals");
        self->mIs3d = 1;
        self->CBody.CItem.mModel =
            Spool_GetModel(0x54636518, CurrentMedalsFileRegion);
        break;

    case TapePowerUpMedalB:
        Tape_InitItem(&self->CBody.CItem, "skmedals");
        self->mIs3d = 1;
        self->CBody.CItem.mModel =
            Spool_GetModel(0xba6d0434, CurrentMedalsFileRegion);
        break;

    case TapePowerUpMedalC:
        Tape_InitItem(&self->CBody.CItem, "skmedals");
        self->mIs3d = 1;
        self->CBody.CItem.mModel =
            Spool_GetModel(0x2364558e, CurrentMedalsFileRegion);
        break;

    case TapePowerUpSecretTape:
    case TapePowerUpSpecial18:
        self->mMessageTable = TapeMessage;
        self->CBody.mAngVel.vy = 150;
        self->mIs3d = 1;
        self->CBody.CItem.mModel =
            Spool_GetModel(0x7c1b2c4a, CurrentItemsFileRegion);
        break;
    }

    if (GGame != 1)
    {
        switch (self->CBody.CItem.mType)
        {
        case TapePowerUpLetterK:
        case TapePowerUpLetterS:
        case TapePowerUpLetterA:
        case TapePowerUpLetterE:
        case TapePowerUpLetterT:
        case TapePowerUpSecretTape:
            self->mLifetime = 1;
            break;
        }
    }
    else
    {
        switch (self->CBody.CItem.mType)
        {
        case TapePowerUpLetterK:
        case TapePowerUpLetterS:
        case TapePowerUpLetterA:
        case TapePowerUpLetterE:
        case TapePowerUpLetterT:
            if (Career_Got(2) != 0)
                self->mLifetime = 1;
            break;

        case TapePowerUpSecretTape:
            if (Career_Got(3) != 0)
                self->mLifetime = 1;
            break;
        }
    }

    reinterpret_cast< ::CBody *>(&self->CBody)->AttachTo(&PowerUpList);
    if (self->mIs3d == 0)
        self->DontDisplay();
}

/**
 * Detaches and destroys a tape pickup and its owned glow.
 *
 */
CTape::~CTape()
{
    CTape *self = this;
    self->CBody.CItem._vf = &__vt_5CTape;
    reinterpret_cast< ::CBody *>(&self->CBody)->DeleteFrom(&PowerUpList);
    self->DeleteStuff();
    Tape_DestroyBody(&self->CBody, 0);
}

/**
 * Makes a tape unavailable to both players and releases display state.
 *
 */
void CTape::DontDisplay()
{
    CTape *self = this;
    u16 flags = self->CBody.CItem.mFlags;
    flags |= TapeUnavailableToEveryone;
    self->CBody.CItem.mFlags = flags;
    self->CBody.CItem._vf->DeleteStuff(self);
}

/**
 * Requests the shared particle cache used for the tape's 2D effect.
 *
 */
void CTape::CreateBit()
{
    CTape *self = this;
    (void)self;
    UseCacheForBits = 1;
}

/**
 * Destroys the glow effect owned by a tape.
 *
 */
void CTape::DeleteStuff()
{
    CTape *self = this;
    if (self->mpGlow != 0)
    {
        self->mpGlow->CBit._vf->Destroy(self->mpGlow, 3);
    }
    self->mpGlow = 0;
}

/**
 * Restores the hover origin, advances rotation, and applies sine wobble.
 *
 */
void CTape::DoPhysics()
{
    CTape *self = this;
    CTape *tape = self;
    tape->CBody.CItem.mAngles += tape->CBody.mAngVel;
    tape->CBody.CItem.mAngles.Mask();
    short *table = rcossin_tbl;
    self->CBody.CItem.mPos = self->mOrgPos;
    u16 angle = self->mT;
    u16 speed = self->mWobbleAmplitude;
    short sinVal =
        table[(angle & TapeTrigAngleMask) * TapeTrigEntryStride];
    int product = speed * sinVal;
    if (product < 0)
        product += TapeTrigAngleMask;
    int yDelta =
        (product >> TapeFixedFractionBits) << TapeFixedFractionBits;
    int curY = self->CBody.CItem.mPos.vy;
    u16 angleStep = self->mWobbleSpeed;
    self->CBody.CItem.mPos.vy = curY + yDelta;
    self->mT = angle + angleStep;
}

/**
 * Advances tape animation and displays queued career-progression messages.
 *
 * @param tape_no Tape AI index retained by the public ABI.
 */
void CTape::AI(int tape_no)
{
    CTape *tape = this;
    (void)tape_no;

    tape->mTimer++;
    if (tape->mTimer < 60)
        CTape::mOkayToDisplayTitle = 1;
    else
        CTape::mOkayToDisplayTitle = 0;

    if (tape->mSessionTapeNum * 20 + 80 < tape->mTimer)
        CTape::mOkayToDisplayContinueButton = 1;
    else
        CTape::mOkayToDisplayContinueButton = 0;

    if (tape->mTimer > 60)
    {
        int temptime = tape->mTimer - 60;
        CVector pos_offset =
            ((tape->mVec2 - tape->mVec1) * temptime) / 20;

        int size_offset = (tape->mSize2 - tape->mSize1) * temptime / 20;
        if (temptime < 20)
        {
            tape->mOrgPos = pos_offset + tape->mVec1;
            tape->mSize = size_offset + tape->mSize1;
            tape->mWobbleAmplitude =
                tape->mMaxWobbleAmplitude -
                (tape->mMaxWobbleAmplitude * temptime / 20);
        }
        else
        {
            tape->mOrgPos = tape->mVec2;
            tape->mWobbleAmplitude = 0;
            tape->mSize = tape->mSize2;
        }
    }

    int message_count = 0;
    STapeMessage *messages[5];
    messages[0] = 0;
    messages[1] = 0;
    messages[2] = 0;
    messages[3] = 0;
    messages[4] = 0;

    short Scale = static_cast<short>(tape->mSize);
    CSVector &scale = tape->CBody.CItem.mScale;
    scale.vx = Scale;
    scale.vy = Scale;
    scale.vz = Scale;

    for (int i = 0; i < 5; i++)
    {
        if (GTape[i] != 0)
        {
            if (i == 4)
            {
                messages[4] = &tape->mMessageTable[GTape[4]->mTapeNum];
            }
            else if (tape->mMessageTable[GTape[i]->mTapeNum].essential == 1)
            {
                messages[i] = &tape->mMessageTable[GTape[i]->mTapeNum];
            }
        }
        else
        {
            messages[i - 1] = &tape->mMessageTable[GTape[i - 1]->mTapeNum];
            break;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        if (messages[i] != 0)
            message_count++;
    }

    int message_no = 0;
    for (int i = 0; i < tape->mSessionTapeNum; i++)
    {
        if (messages[i] != 0)
            message_no++;
    }

    if (message_no * 20 + 80 < tape->mTimer &&
        messages[tape->mSessionTapeNum] != 0)
    {
        switch (message_count)
        {
        case 1:
            tape->mTextX = 130;
            tape->mTextY = 120;
            break;

        case 2:
            switch (message_no)
            {
            case 0:
                tape->mTextX = 130;
                tape->mTextY = 85;
                break;
            case 1:
                tape->mTextX = 130;
                tape->mTextY = 155;
                break;
            }
            break;

        case 3:
            switch (message_no)
            {
            case 0:
                tape->mTextX = 130;
                tape->mTextY = 64;
                break;
            case 1:
                tape->mTextX = 130;
                tape->mTextY = 120;
                break;
            case 2:
                tape->mTextX = 130;
                tape->mTextY = 176;
                break;
            }
            break;

        case 4:
            switch (message_no)
            {
            case 0:
                tape->mTextX = 130;
                tape->mTextY = 50;
                break;
            case 1:
                tape->mTextX = 150;
                tape->mTextY = 92;
                break;
            case 2:
                tape->mTextX = 130;
                tape->mTextY = 134;
                break;
            case 3:
                tape->mTextX = 150;
                tape->mTextY = 176;
                break;
            }
            break;

        case 5:
            switch (message_no)
            {
            case 0:
                tape->mTextX = 130;
                tape->mTextY = 50;
                break;
            case 1:
                tape->mTextX = 150;
                tape->mTextY = 85;
                break;
            case 2:
                tape->mTextX = 130;
                tape->mTextY = 120;
                break;
            case 3:
                tape->mTextX = 150;
                tape->mTextY = 155;
                break;
            case 4:
                tape->mTextX = 130;
                tape->mTextY = 190;
                break;
            }
            break;
        }

        Mess_SetTextJustify(1);
        Mess_SetScale(0xe0);
        Mess_SetRGB(0x7a, 0x7a, 0x7a, 0);

        if (messages[tape->mSessionTapeNum]->Line2 == 0)
        {
            Mess_DrawText(
                tape->mTextX, tape->mTextY,
                messages[tape->mSessionTapeNum]->Line1,
                0, 0, 0x100, 0);
        }
        else if (messages[tape->mSessionTapeNum]->Line3 == 0)
        {
            Mess_DrawText(
                tape->mTextX, tape->mTextY - 8,
                messages[tape->mSessionTapeNum]->Line1,
                0, 0, 0x100, 0);
            Mess_DrawText(
                tape->mTextX, tape->mTextY + 8,
                messages[tape->mSessionTapeNum]->Line2,
                0, 0, 0x100, 0);
        }
        else
        {
            Mess_DrawText(
                tape->mTextX, tape->mTextY - 16,
                messages[tape->mSessionTapeNum]->Line1,
                0, 0, 0x100, 0);
            Mess_DrawText(
                tape->mTextX, tape->mTextY,
                messages[tape->mSessionTapeNum]->Line2,
                0, 0, 0x100, 0);
            Mess_DrawText(
                tape->mTextX, tape->mTextY + 16,
                messages[tape->mSessionTapeNum]->Line3,
                0, 0, 0x100, 0);
        }
    }

    tape->DoPhysics();
    if (tape->mIs3d == 0)
        tape->CreateBit();
}

/**
 * Create a tape pickup from two camera-relative endpoints.
 *
 * @param x First camera-relative X coordinate.
 * @param y First camera-relative Y coordinate.
 * @param z First camera-relative Z coordinate.
 * @param Size Initial pickup scale.
 * @param x2 Second camera-relative X coordinate.
 * @param y2 Second camera-relative Y coordinate.
 * @param z2 Second camera-relative Z coordinate.
 * @param Size2 Final interpolation scale.
 * @return Newly created tape pickup.
 */
CTape *Tape_Create(int x, int y, int z, int Size, int x2, int y2, int z2, int Size2)
{
    SVECTOR vec;
    VECTOR vec2;
    CVector TapePos;
    CVector TapePos2;

    vec.vz = static_cast<short>(z);
    vec.vx = static_cast<short>(x);
    vec.vy = static_cast<short>(y);

    gte_ldrot0(TAPES_PACKED_WORD(pCurrentCamera->Transform, 0));
    gte_ldrot1(TAPES_PACKED_WORD(pCurrentCamera->Transform, 1));
    gte_ldrot2(TAPES_PACKED_WORD(pCurrentCamera->Transform, 2));
    gte_ldrot3(TAPES_PACKED_WORD(pCurrentCamera->Transform, 3));
    gte_ldrot4(TAPES_PACKED_WORD(pCurrentCamera->Transform, 4));
    gte_ldvxy0(TAPES_PACKED_WORD(vec, 0));
    gte_ldvz0(TAPES_PACKED_WORD(vec, 1));
    gte_mvmva(0, 0, 0, 3, 0);
    gte_stlvnl(&vec2);

    TapePos.vx = vec2.vx +
        (pCurrentCamera->Position.vx << TapeFixedFractionBits);
    TapePos.vy = vec2.vy +
        (pCurrentCamera->Position.vy << TapeFixedFractionBits);
    TapePos.vz = vec2.vz +
        (pCurrentCamera->Position.vz << TapeFixedFractionBits);

    vec.vx = static_cast<short>(x2);
    vec.vy = static_cast<short>(y2);
    vec.vz = static_cast<short>(z2);
    vec2.vx = 0;
    vec2.vy = 0;
    vec2.vz = 0;

    gte_ldrot0(TAPES_PACKED_WORD(pCurrentCamera->Transform, 0));
    gte_ldrot1(TAPES_PACKED_WORD(pCurrentCamera->Transform, 1));
    gte_ldrot2(TAPES_PACKED_WORD(pCurrentCamera->Transform, 2));
    gte_ldrot3(TAPES_PACKED_WORD(pCurrentCamera->Transform, 3));
    gte_ldrot4(TAPES_PACKED_WORD(pCurrentCamera->Transform, 4));
    gte_ldvxy0(TAPES_PACKED_WORD(vec, 0));
    gte_ldvz0(TAPES_PACKED_WORD(vec, 1));
    gte_mvmva(0, 0, 0, 3, 0);
    gte_stlvnl(&vec2);

    TapePos2.vx = vec2.vx +
        (pCurrentCamera->Position.vx << TapeFixedFractionBits);
    TapePos2.vy = vec2.vy +
        (pCurrentCamera->Position.vy << TapeFixedFractionBits);
    TapePos2.vz = vec2.vz +
        (pCurrentCamera->Position.vz << TapeFixedFractionBits);

    CTape *Tape = new CTape(
        TapePowerUpSpecial18, TapePos, TapePos2, Size, Size2,
        ZeroVector, 0, TapeInfiniteLifetime);

    Tape->mWobbleAmplitude = 10;
    Tape->mMaxWobbleAmplitude = 10;
    Tape->mWobbleSpeed = 80;
    Tape->CBody.CItem.mFlags |= 0x200;
    return Tape;
}
