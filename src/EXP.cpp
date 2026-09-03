// Environmental explosion damage and expanding flat-ripple effects.

#include "EXP.h"

// Owner-local layouts used by this translation unit.
//
// These records remain local to the explosion/ripple owner until their
// definitions can be shared cleanly across every consuming translation unit.
// Field names and offsets come from original/MAIN_STRUCTS.h.
#include "globals.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include "SMODEL.h"

#define ENVIROLIST_ABSOLUTE_DECLARATION
#include "ENVIROLIST.h"

/** Base particle state embedded in a polyline effect. */
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
    void *_vf;

    /** Mark this bit for removal. */
    void Die();
    /** Release bit storage through the shared CBit allocator. */
    static void operator delete(void *storage);
};

#include "CFLATRIPPLE.h"

/** One sine/cosine pair in the fixed PSX trigonometry table. */
struct ExpTrigPair {
    short sine;
    short cosine;
};

enum ExpRippleConstant {
    ExpModelDestructible = 2,
    ExpAngleMask = 0xFFF,
    ExpFullTurn = 0x1000,
    ExpForceDamage = 0xFFFF
};

// Macros preserve the target compiler's signed literal treatment here; enum
// constants change the address-folding path even though their values match.
#define EXP_TRIG_TABLE_PAGE 0x800D0000
#define EXP_TRIG_TABLE_PAGE_OFFSET 0x43C

typedef char Exp_SDK_SVECTOR_size_must_be_8[
    (sizeof(SVECTOR) == 8) ? 1 : -1];
typedef char Exp_CBit_size_must_be_72[(sizeof(CBit) == 72) ? 1 : -1];
typedef char Exp_SLineSeg_size_must_be_16[(sizeof(SLineSeg) == 16) ? 1 : -1];
typedef char Exp_TrigPair_size_must_be_4[(sizeof(ExpTrigPair) == 4) ? 1 : -1];
typedef char Exp_CPolyLine_size_must_be_96[(sizeof(CPolyLine) == 96) ? 1 : -1];
typedef char Exp_CFlatRipple_size_must_be_116[(sizeof(CFlatRipple) == 116) ? 1 : -1];

#include "BIT.h"
#include "PSXREGION.h"
#include "SHATTER.h"
#include "UTILS.h"

// === Forward declarations ===
/** One-word target anchor used for the flat-ripple dispatch table. */
extern void *__vt_11CFlatRipple;
// === Function implementations (address order) ===

/**
 * Confirm an item belongs to EnviroList, then shatter its face or full model.
 *
 * @param pItem Environment item receiving the hit.
 * @param pFace Optional face to shatter when the whole model is not destructible.
 * @param Damage Damage applied to the item; ExpForceDamage forces a shatter.
 */
void Exp_HitEnvItem(Item *pItem, unsigned int *pFace, int Damage)
{
    Item *pScan;
    SModel *pModel;

    if (pItem == 0) return;
    pScan = EnviroList;
    if (pScan == 0) return;
loop_search:
    if (pScan == pItem) goto found;
    pScan = pScan->NextItem;
    if (pScan != 0) goto loop_search;
found:
    if (pScan == 0) return;

    {
        pModel = PSXRegion[pItem->Region].ppModels[pItem->Model];

        if ((pModel->Flags & ExpModelDestructible) != 0)
        {
            unsigned char Life;

            // The legacy life value occupies the high word of the model flags.
            // SModel::Flags is 16-bit in this PSX build, so the extracted value is zero.
            Life = pModel->Flags >> 16;
            if (Life != 0 || Damage == ExpForceDamage)
            {
                if (Damage < Life) return;
                Shatter_Item(pItem, 1, 1);
            }
            return;
        }
        if (pFace == 0) return;
        Shatter_Face(pItem, pFace, 1, 1, 1, 1, 1);
    }
    return;
}

/**
 * Build a flat ripple, seed its segments, and randomize its start angle.
 *
 * @param Pos  Centre of the ripple.
 * @param MaxRadius  Radius at which the ripple expires.
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 * @param NumSegs  Number of ring segments.
 * @param GrowRate  Radius added on each update.
 */
CFlatRipple::CFlatRipple(
    const CVector &Pos, int MaxRadius,
    unsigned char r, unsigned char g, unsigned char b,
    int NumSegs, int GrowRate)
    : CPolyLine(NumSegs)
{
    int i;

    _vf = &__vt_11CFlatRipple;

    mPos = Pos;
    mMaxRadius = MaxRadius;
    mGrowRate = GrowRate;

    mStart = Pos;

    mR = r;
    mG = g;
    mB = b;

    i = 0;
    if (0 < mNumSegs)
    {
        do
        {
            mSegs[i].r = r;
            mSegs[i].g = g;
            mSegs[i].b = b;
            mSegs[i].End = Pos;

            i++;
        } while (i < mNumSegs);
    }

    mAngle = Rnd(ExpFullTurn);
    SetSemiTransparent();
}

/** Restore the derived dispatch table before the base destructor runs. */
CFlatRipple::~CFlatRipple()
{
    _vf = &__vt_11CFlatRipple;
}

/**
 * Expand a flat ripple, fade its colour, and rebuild its ring segments.
 *
 */
void CFlatRipple::Move()
{
    mRadius += mGrowRate;

    if (mRadius >= mMaxRadius)
    {
        mRadius = mMaxRadius;
        Die();
    }

    int intensity = ((mMaxRadius - mRadius) << 8) / mMaxRadius;
    int a = mAngle;
    // Without this constraint cc1plus hoists the table-base load ahead of the
    // target's angle load (2 of 140 instructions differ).
    asm("" : "+r"(a));
    int trigTableAddress = EXP_TRIG_TABLE_PAGE;
    // Keeping the page opaque preserves the target's alias and loop schedule;
    // exposing the complete absolute address changes 132 instructions and six
    // relocations.
    asm("" : "+r"(trigTableAddress));
    trigTableAddress += EXP_TRIG_TABLE_PAGE_OFFSET;
    // Ordinary allocation chooses v0 and changes five instructions. Three
    // alternative source shapes still changed at least three instructions.
    register int trigOffset asm("$3") = a & ExpAngleMask;
    trigOffset <<= 2;
    ExpTrigPair *initialTrig = reinterpret_cast<ExpTrigPair *>(
        trigOffset + trigTableAddress);
    int angleStep = ExpFullTurn / mNumSegs;
    mStart.vx =
        mPos.vx +
        (((mRadius * initialTrig->cosine) / ExpFullTurn) << 12);

    int i = 0;
    mStart.vz =
        mPos.vz +
        (((mRadius * initialTrig->sine) / ExpFullTurn) << 12);

    if (mNumSegs <= 0)
        return;

    do
    {
        int offset = i << 4;
        mSegs[i].r = static_cast<u8>((mR * intensity) >> 8);
        mSegs[i].g = static_cast<u8>((mG * intensity) >> 8);
        mSegs[i].b = static_cast<u8>((mB * intensity) >> 8);
        a += angleStep;

        if (i == mNumSegs - 1)
        {
            mSegs[i].End = mStart;
        }
        else
        {
            ExpTrigPair *loopTrig = reinterpret_cast<ExpTrigPair *>(
                ((a & ExpAngleMask) << 2) + trigTableAddress);
            // Direct typed indexing commutes the target's address operands and
            // changes 21 instructions plus two branch relocations.
            SLineSeg *segment = reinterpret_cast<SLineSeg *>(
                offset + reinterpret_cast<int>(mSegs));
            segment->End.vx =
                mPos.vx +
                (((mRadius * loopTrig->cosine) / ExpFullTurn) << 12);

            segment = reinterpret_cast<SLineSeg *>(
                offset + reinterpret_cast<int>(mSegs));
            segment->End.vz =
                mPos.vz +
                (((mRadius * loopTrig->sine) / ExpFullTurn) << 12);
        }

        i++;
    } while (i < mNumSegs);

    return;
}
