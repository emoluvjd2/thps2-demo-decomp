// Smoke-jet construction, positioning, animation, and damage handling.

#define SMOKE_OWNER_DECLARATIONS
#include "SMOKE.h"
#undef SMOKE_OWNER_DECLARATIONS
#include "MECHLIST.h"
#include "MAIN.h"
#include "BADDY.h"
#include "BIT.h"
#include "UTILS.h"

typedef char smoke_api_size_must_match_layout[
    (SMOKE_JET_STORAGE_SIZE == sizeof(CSmokeJet)) ? 1 : -1];

/** Packed fixed-point sine/cosine entry used by the original table macros. */
struct SSinCos
{
    short Sin;
    short Cos;
};

enum
{
    SMOKE_ANGLE_COUNT = 4096,
    SMOKE_ANGLE_MASK = SMOKE_ANGLE_COUNT - 1,
    SMOKE_PERMANENT_LIFETIME = 0xffff,
    SMOKE_COLLISION_SCALE = 4096,
};

#define RCOSSIN_DECL_TYPE int
#define RCOSSIN_SCALAR_VIEW
#include "RCOSSIN.h"

/*
 * Keep the argument expression in the caller: an inline function introduces
 * a distinct GCC 2.x parameter and changes Move's register allocation.
 */
#define Smoke_Sine(angle) \
    (reinterpret_cast<const SSinCos *>( \
        &rcossin_tbl + ((angle) & SMOKE_ANGLE_MASK))->Sin)
CVector operator-(const CVector &, const CVector &);
#include "M3DCOLIJ.h"
extern "C" void Smoke_Destroy(CSmokeJet *, int) asm("_._9CSmokeJet");
extern "C" void Smoke_Move(CSmokeJet *) asm("Move__9CSmokeJet");
extern "C" void Smoke_DisplayRibbon(CTexturedRibbon *)
    asm("Display__15CTexturedRibbon");

/** Exact five-slot virtual table shape emitted for CSmokeJet. */
struct SmokeJetVTable
{
    void (*reserved0)(void);
    void (*reserved1)(void);
    void (*destroy)(CSmokeJet *, int);
    void (*move)(CSmokeJet *);
    void (*display)(CTexturedRibbon *);
};

/** CBody virtual-table prefix through the collision-hit method used here. */
struct SmokeBodyVTable
{
    void (*reserved[7])(void);
    int (*hit)(CBody *, int, CVector *, int);
};

/**
 * Smoke-jet virtual dispatch table in its original read-only owner section.
 */
extern "C" SmokeJetVTable __vt_9CSmokeJet
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        Smoke_Destroy,
        Smoke_Move,
        Smoke_DisplayRibbon,
};

// === Function implementations (address order) ===

/**
 * Initialize a ten-point textured smoke ribbon.
 *
 * The constructor positions the ribbon along its launch direction, initializes
 * each point's horizontal and width wobble, and fades both color banks toward
 * the tip of the jet.
 *
 * Address:   0x800256FC, Size: 868 bytes (217 instructions)
 * Target:    __9CSmokeJetRC7CVectorRC8CSVectoriUsiiiiUcUi
 * Method:    CSmokeJet::CSmokeJet(const CVector &, const CSVector &, int,
 *            unsigned short, int, int, int, int, unsigned char, unsigned int)
 * Match:     PERFECT (217/217)
 *
 * @param BasePos World-space base of the jet.
 * @param Angles Direction used to place the ribbon points.
 * @param Length Jet length in fixed-point world units.
 * @param Duration Ribbon lifetime.
 * @param Damage Damage amount; values at least 1000 enable baddie damage and
 *        store the remainder as the actual damage.
 * @param r Red value at the base of the ribbon.
 * @param g Green value at the base of the ribbon.
 * @param b Blue value at the base of the ribbon.
 * @param ScrollSpeed Texture-scroll increment.
 * @param Checksum Texture checksum.
 */
CSmokeJet::CSmokeJet(
    const CVector &BasePos,
    const CSVector &Angles,
    int Length,
    unsigned short Duration,
    int Damage,
    int r,
    int g,
    int b,
    unsigned char ScrollSpeed,
    unsigned int Checksum)
    : CTexturedRibbon(SMOKE_POINT_COUNT, 0)
{
    CSmokeJet *_this = this;
    _this->_vf = &__vt_9CSmokeJet;
    _this->mAngles = Angles;
    _this->mLength = Length;
    _this->SetPos(BasePos);

    _this->mLifetime = Duration;
    _this->mDamage = Damage;
    if (_this->mDamage >= 1000) {
        _this->mDamageBaddies = 1;
        _this->mDamage = Damage - 1000;
    }

    _this->mScrollSpeed = ScrollSpeed;
    _this->SetTexture(Checksum);

    {
        int i;

        for (i = 0; i < SMOKE_POINT_COUNT; i++) {
            int scaledr;
            int scaledg;
            int scaledb;
            int *amp;

            amp = &_this->mAmp[i];
            _this->mOrgX[i] = _this->mpPoints[i].Pos.vx;
            _this->mPhase[i] = Rnd(SMOKE_ANGLE_COUNT);
            _this->mFreq[i] = 80;

            if (i != 0) {
                *amp = Length / 80;
            } else {
                *amp = 0;
            }

            _this->mWidthWobble[i].TChange = Rnd(16);
            if (i == 0) {
                _this->mWidthWobble[0].TargetWidth = Rnd(60) + 20;
            } else {
                _this->mWidthWobble[i].TargetWidth =
                    _this->mWidthWobble[i - 1].TargetWidth * 148 / 128;
            }

            scaledr = (SMOKE_LAST_POINT - i) * r / SMOKE_LAST_POINT;
            scaledg = (SMOKE_LAST_POINT - i) * g / SMOKE_LAST_POINT;
            scaledb = (SMOKE_LAST_POINT - i) * b / SMOKE_LAST_POINT;

            _this->mpPoints[i].Width =
                _this->mWidthWobble[i].TargetWidth;
            _this->mpPoints[i].WidthB =
                _this->mpPoints[i].Width + 40;
            _this->mpPoints[i].r = scaledr;
            _this->mpPoints[i].g = scaledg;
            _this->mpPoints[i].b = scaledb;

            scaledr = scaledr * 48 / 256;
            scaledg = scaledg * 48 / 256;
            scaledb = scaledb * 48 / 256;

            _this->mpPoints[i].rB = scaledr;
            _this->mpPoints[i].gB = scaledg;
            _this->mpPoints[i].bB = scaledb;
        }
    }
}


/**
 * Destroy a smoke jet and optionally release its CBit-owned storage.
 *
 * Address:   0x80025A60
 * Target:    _._9CSmokeJet
 * Method:    CSmokeJet::~CSmokeJet()
 *
 */
CSmokeJet::~CSmokeJet()
{
    _vf = &__vt_9CSmokeJet;
}


/**
 * Reposition all ribbon points from a base position and launch direction.
 *
 * Address:   0x80025AB4
 * Target:    SetPos__9CSmokeJetRC7CVector
 * Method:    CSmokeJet::SetPos(const CVector &)
 *
 * @param Pos World-space base position.
 */
void CSmokeJet::SetPos(const CVector &Pos)
{
    CSmokeJet *_this = this;
    CVector Off;
    CVector pos;

    Utils_GetVecFromMagDir(Off, _this->mLength, _this->mAngles);
    Off /= SMOKE_LAST_POINT;
    pos = Pos;

    for (int i = 0; i < SMOKE_POINT_COUNT; i++)
    {
        _this->mpPoints[i].Pos = pos;
        pos += Off;
    }
}


/**
 * Advance smoke jet animation, lifetime, and collision-driven damage.
 *
 * Address:   0x80025B74
 * Target:    Move__9CSmokeJet
 * Method:    CSmokeJet::Move(void)
 *
 */
void CSmokeJet::Move()
{
    if (mLifetime ==
        SMOKE_PERMANENT_LIFETIME) {
        int dx = (mpPoints[0].Pos.vx -
                  MechList->CItem.mPos.vx) >> 12;
        if (dx < 0) dx = -dx;

        int dz = (mpPoints[0].Pos.vz -
                  MechList->CItem.mPos.vz) >> 12;
        if (dz < 0) dz = -dz;

        int Dist;
        if (dx < dz) {
            Dist = dz + dx / 2;
        } else {
            Dist = dx + dz / 2;
        }

        if (Dist > 10000) {
            mDontDisplay = 1;
            return;
        }
    } else if (mLifetime == 0 ||
               --mLifetime == 0) {
        Die();
    }

    mDontDisplay = 0;
    mU =
        Smoke_Sine(TTime << 4) / 128;
    mV += mScrollSpeed;

    for (int i = 0; i < SMOKE_POINT_COUNT; i++) {
        mpPoints[i].Pos.vx =
            mOrgX[i] +
            ((mAmp[i] *
              Smoke_Sine(mFreq[i] * TTime + mPhase[i])) /
             SMOKE_ANGLE_COUNT << 12);
    }

    mpPoints[0].Width = 20;
    mpPoints[0].WidthB = 60;
    mWidthWobble[0].TargetWidth = Rnd(60) + 20;

    for (int i = 1; i < 10; i++) {
        mpPoints[i].Width +=
            (mWidthWobble[i].TargetWidth -
             mpPoints[i].Width) / 8;
        mpPoints[i].WidthB =
            mpPoints[i].Width + 40;

        if ((TTime & 0xf) == mWidthWobble[i].TChange) {
            mWidthWobble[i].TChange = Rnd(16);
            mWidthWobble[i].TargetWidth =
                mWidthWobble[i - 1].TargetWidth * 148 / 128;
        }
    }

    if ((mDamageTimer == 0 || --mDamageTimer == 0) &&
        mDamage) {
        CVector Start = mpPoints[0].Pos;
        CVector End = mpPoints[SMOKE_LAST_POINT].Pos;
        CVector Unit = End - Start;
        CVector dummy;

        Utils_CalcUnit(Unit, Unit);

        CBody *pBody = M3dColij_LineToSphere(
            Start, End, dummy, MechList, 0, SMOKE_COLLISION_SCALE);
        if (pBody != 0) {
            pBody->CItem._vf->hit(pBody, mDamage, &Unit, 31);
            mDamageTimer = 15;
        }

        if (mDamageBaddies != 0) {
            CBody *pBody = M3dColij_LineToSphere(
                Start, End, dummy, BaddyList, 0, SMOKE_COLLISION_SCALE);
            if (pBody != 0) {
                pBody->CItem._vf->hit(
                    pBody, mDamage, &Unit, 31);
                mDamageTimer = 15;
            }
        }
    }
}
