// Owner-local layout overlay for SMOKE.cpp.
#ifndef SMOKE_INC_H
#define SMOKE_INC_H

#include "globals.h"

struct CSmokeJet;

/** Target-proven storage size of one smoke jet. */
enum { SMOKE_JET_STORAGE_SIZE = 364 };

enum
{
    SMOKE_POINT_COUNT = 10,
    SMOKE_LAST_POINT = SMOKE_POINT_COUNT - 1,
};

/** Construct a smoke jet in caller-provided CBit storage. */
extern "C" CSmokeJet *Smoke_Construct(
    CSmokeJet *storage, const CVector &BasePos, const CSVector &Angles,
    int Length, unsigned short Duration, int Damage,
    int r, int g, int b, unsigned char ScrollSpeed, unsigned int Checksum)
    asm("__9CSmokeJetRC7CVectorRC8CSVectoriUsiiiiUcUi");

#ifdef SMOKE_OWNER_DECLARATIONS

struct Texture;
struct SLight;
struct SmokeJetVTable;
struct SmokeBodyVTable;

/** Base particle record used by special-display effects. */
struct CBit
{
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
    SmokeJetVTable *_vf;

    /** Mark this bit for removal. */
    void Die();
    /** Release transient-bit storage through the owning allocator. */
    static void operator delete(void *);
};

#include "CSPECIALDISPLAY.h"

#include "SRIBBONPOINT.h"

#include "CTEXTUREDRIBBON.h"

#include "CSMOKEJET.h"

/** Composition-only item view used for smoke-jet range and damage checks. */
struct SmokeCItemOwner
{
    unsigned short mFlags;
    unsigned short mInquiry;
    CVector mPos;
    CSVector mAngles;
    unsigned short mModel;
    char mDummyFrame;
    unsigned char mTintIndex;
    unsigned char mDummyAnim;
    unsigned char mRegion;
    SmokeCItemOwner *mNextItem;
    unsigned int mRGB;
    CSVector mScale;
    unsigned int mTRN;
    SmokeCItemOwner *mPreviousItem;
    unsigned int mStoredRGB;
    unsigned char mPredatorEffectType;
    unsigned char mPredatorEffectFade;
    unsigned short mType;
    SLight *mpLight;
    unsigned short mFlashTimer;
    short mFlash_dR;
    short mFlash_dG;
    short mFlash_dB;
    SmokeBodyVTable *_vf;
};

/**
 * Body slice used by SMOKE.cpp.
 *
 * Only CItem fields are observed here; the reserved tail preserves CBody's
 * canonical 264-byte size without importing unrelated collision/render state.
 */
struct CBody
{
    SmokeCItemOwner CItem;
    unsigned char mBodyTail[264 - 76];
};

typedef char smoke_CBit_size_must_be_72[(sizeof(CBit) == 72) ? 1 : -1];
typedef char smoke_SRibbonPoint_size_must_be_36[(sizeof(SRibbonPoint) == 36) ? 1 : -1];
typedef char smoke_CTexturedRibbon_size_must_be_108[(sizeof(CTexturedRibbon) == 108) ? 1 : -1];
typedef char smoke_SWidthWobble_size_must_be_8[(sizeof(SWidthWobble) == 8) ? 1 : -1];
typedef char smoke_CItem_size_must_be_76[
    (sizeof(SmokeCItemOwner) == 76) ? 1 : -1];
typedef char smoke_CBody_size_must_be_264[(sizeof(CBody) == 264) ? 1 : -1];
typedef char smoke_CSmokeJet_size_must_be_364[(sizeof(CSmokeJet) == 364) ? 1 : -1];

#endif // SMOKE_OWNER_DECLARATIONS

#endif // SMOKE_INC_H
