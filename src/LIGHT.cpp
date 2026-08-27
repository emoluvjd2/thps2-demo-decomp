// LIGHT.cpp -- Reconstructed translation unit
// Original: LIGHT.CPP
// Functions: 3 reconstructed entries
// Address range: 0x8002F7F0 - 0x8002F964

#include "globals.h"

struct CBit;

/** GCC 2.x virtual-table prefix shared by the bit hierarchy. */
struct LightBitVTable {
    int baseOffset;
    int typeInfo;
    void (*Destroy)(CBit *bit, int __in_chrg);
};

/** Base particle record embedded in non-rendered light objects. */
struct CBit {
    struct CBit *mPrevious;
    struct CBit *mNext;
    short mAge;
    unsigned short mLifetime;
    struct CVector mTweenPos;
    struct CVector mPos;
    struct CVector mVel;
    struct CVector mAcc;
    struct CFriction mFric;
    unsigned char mDead;
    unsigned short mFrigDeltaZ;
    unsigned char mProtected;
    unsigned char mType;
    LightBitVTable *_vf;

    static void operator delete(void *p);
};

#include "CNONRENDEREDBIT.h"

#include "CGLOW.h"

#include "CWOBBLYGLOW.h"

#include "CLIGHT.h"

typedef char Light_CBit_size_must_be_72[(sizeof(CBit) == 72) ? 1 : -1];
typedef char Light_BitVTable_prefix_size_must_be_12[
    (sizeof(LightBitVTable) == 12) ? 1 : -1];
typedef char Light_CNonRenderedBit_size_must_be_72[
    (sizeof(CNonRenderedBit) == 72) ? 1 : -1];
typedef char Light_CGlow_size_must_be_104[(sizeof(CGlow) == 104) ? 1 : -1];
typedef char Light_CWobblyGlow_size_must_be_176[
    (sizeof(CWobblyGlow) == 176) ? 1 : -1];
typedef char CLight_size_must_be_100[(sizeof(CLight) == 100) ? 1 : -1];

#include "BIT.h"

/** First two members of the CBody records MechList points at. */
struct LightCItemPrefix
{
    unsigned short mFlags;
    unsigned short mInquiry;
    CVector mPos;
};

struct CBody
{
    LightCItemPrefix CItem;
};

// === $gp-relative globals ===
// The vtable is 12 bytes, but LIGHT addresses it through the target's
// one-word small-data anchor.  A full-table declaration selects absolute
// addressing and changes both constructor and destructor scheduling.
extern int __vt_6CLight;

static inline LightBitVTable *Light_VTable()
{
    return reinterpret_cast<LightBitVTable *>(&__vt_6CLight);
}

// === External globals ===
// MECH owns this small object, but LIGHT's target uses an absolute address.
#include "MECHLIST.h"
// LIGHT's target accesses this BIT-owned small object by absolute address.
// The section-qualified redeclaration suppresses GCC's GP-relative extern.
#define BITSTATE_DECLARE_USE_CACHE_FOR_BITS
#include "BITSTATE.h"

/** Delete a bit through the target's typed GCC 2.x vtable prefix. */
static inline void DeleteBit(CBit *bit)
{
    bit->_vf->Destroy(bit, 3);
}

// === Function implementations (address order) ===

/**
 * CLight::CLight
 *
 * Address:   0x8002F7F0
 * Mangled:   __6CLightRC7CVectoriiiiiUcUcUcUcUcUc
 *
 * @param Pos  Position vector.
 * @param Node  Rail / graph node identifier, kept in CBit's lifetime slot.
 * @param ClipDist  Distance beyond which the light stops spawning a glow.
 * @param InnerRadius  Inner glow radius.
 * @param FringeRadius  Fringe (outer falloff) radius.
 * @param Amp  Wobble amplitude.
 * @param r0,g0,b0  Centre colour.
 * @param r1,g1,b1  Fringe colour.
 */
CLight::CLight(const CVector &Pos, int Node, int ClipDist,
               int InnerRadius, int FringeRadius, int Amp,
               unsigned char r0, unsigned char g0, unsigned char b0,
               unsigned char r1, unsigned char g1, unsigned char b1)
{
    _vf = Light_VTable();
    mPos = Pos;
    mClipDist = ClipDist;
    mType = 2;
    mLifetime = static_cast<unsigned short>(Node);
    mOn = 1;
    mInnerRadius = InnerRadius;
    mFringeRadius = FringeRadius;
    mr0 = r0;
    mg0 = g0;
    mb0 = b0;
    mr1 = r1;
    mg1 = g1;
    mb1 = b1;
    mAmp = Amp;
}

/**
 * CLight::~CLight
 *
 * Address:   0x8002F8F0
 * Mangled:   _._6CLight
 */
CLight::~CLight()
{
    _vf = Light_VTable();
    if (mpGlow != 0)
        DeleteBit(static_cast<CBit *>(mpGlow));
}

/**
 * CLight::Move
 *
 * Address:   0x8002F964
 * Mangled:   Move__6CLight
 *
 * Spawns a wobbly glow when the skater is within clip distance and feeds it
 * a distance-scaled angle; kills the glow when out of range or switched off.
 */
// Shared CLight member function
void CLight::Move()
{
    if (mOn == 0)
        goto cleanup;
    {
        int dx = (mPos.vx -
                  MechList->CItem.mPos.vx) >> 12;
        if (dx < 0)
            dx = -dx;
        int dz = (mPos.vz -
                  MechList->CItem.mPos.vz) >> 12;
        if (dz < 0)
            dz = -dz;
        int Dist;
        if (dx < dz)
        {
            Dist = dz + dx / 2;
        }
        else
        {
            Dist = dx + dz / 2;
        }
        if (Dist >= mClipDist)
            goto cleanup;
        if (mpGlow == 0)
        {
            UseCacheForBits = 0;
            // Writing allocation/construction results directly through
            // mpGlow changes thirty-nine instructions and three relocations.
            CWobblyGlow *obj = static_cast<CWobblyGlow *>(
                Bit_AllocateStorage(sizeof(CWobblyGlow)));
            if (obj != 0)
            {
                obj = CWobblyGlow_Construct(
                    obj, mPos, mInnerRadius,
                    mFringeRadius, mAmp, mr0, mg0, mb0, mr1, mg1, mb1);
            }
            mpGlow = obj;
            UseCacheForBits = 1;
            obj->mProtected = 1;
        }
        mpGlow->mAngle = static_cast<short>(Dist * 10 / 16);
    }
    return;

cleanup:
    {
        if (mpGlow != 0)
        {
            DeleteBit(static_cast<CBit *>(mpGlow));
            mpGlow = 0;
        }
    }
    return;
}
