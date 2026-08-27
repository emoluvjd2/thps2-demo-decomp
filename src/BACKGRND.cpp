// BACKGRND.cpp -- Reconstructed translation unit
// Original: BACKGRND.CPP
// Functions: 5
// Address range: 0x800790B0 - 0x8007924C
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#include "BACKGRND.h"
#include "globals.h"
#include "CITEM.h"

/** Body storage used by the background object's inherited CBody state. */
struct CBody : public CItem {
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
    void *mpShadow;
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
    ~CBody();
    void AttachTo(CBody **list);
    void DeleteFrom(CBody **list);
};

/** Background body plus the monotonically assigned background-list index. */
struct CBackground : public CBody {
    unsigned short mIndex;

    CBackground(unsigned int checksum, const CSVector &ang_vel);
    ~CBackground();
    void AI();
};

typedef char CItem_size_check[(sizeof(CItem) == 76) ? 1 : -1];
typedef char CBody_size_check[(sizeof(CBody) == 264) ? 1 : -1];
typedef char CBackground_size_check[(sizeof(CBackground) == 268) ? 1 : -1];

#include "OB.h"
#include "SPOOL.h"

// Linked list node for background objects -- gp-relative head pointer
CBody *BackgroundList = 0; // gp+0x3D88
static u16 CurrentIndex = 0; // gp+0x3D8C

enum {
    BACKGROUND_DISABLED_FLAG = 1,
    BACKGROUND_ITEM_FLAGS_CLEARED_ON_INIT = 2,
    BACKGROUND_BODY_FLAGS_CLEARED_ON_INIT = 6
};

// === Extern "C" forward declarations ===
extern "C" void Background_DestructorAbi(CBackground *, int)
    asm("_._11CBackground");
extern "C" void Background_AIAbi(CBackground *) asm("AI__11CBackground");
extern "C" void Background_CBodyDie(CBody *) asm("Die__5CBody");
extern "C" void Background_CBodyDoLightingSetup(CBody *)
    asm("DoLightingSetup__5CBody");
extern "C" int Background_CBodyHit(
    CBody *, int damage, const CVector &knock, int whatHit)
    asm("Hit__5CBodyiRC7CVectori");
extern "C" void Background_CBodyDeleteStuff(CBody *)
    asm("DeleteStuff__5CBody");

/** Typed dispatch-table layout emitted for CBackground. */
struct BackgroundVtable {
    unsigned int reserved[2];
    void (*destructor)(CBackground *, int);
    void (*die)(CBody *);
    void (*ai)(CBackground *);
    void (*everyFrame)(CBody *);
    void (*doLightingSetup)(CBody *);
    int (*hit)(CBody *, int, const CVector &, int);
    void (*deleteStuff)(CBody *);
};

typedef char BackgroundVtable_size_check[
    (sizeof(BackgroundVtable) == 36) ? 1 : -1];

/** Virtual dispatch table for the background-object specialization. */
BackgroundVtable __vt_11CBackground
    __attribute__((section(".rdata"))) = {
        {0, 0},
        Background_DestructorAbi,
        Background_CBodyDie,
        Background_AIAbi,
        CBody_EveryFrame,
        Background_CBodyDoLightingSetup,
        Background_CBodyHit,
        Background_CBodyDeleteStuff,
    };

// === Function implementations (address order) ===

/**
 * Backgrnd_On__FUs
 *
 * Address:   0x800790B0
 * Mangled:   Backgrnd_On__FUs
 * Demangled: Backgrnd_On(unsigned short)
 *            Traverses the background list and clears the disabled bit on the
 *            object whose index matches.
 *
 * @param index  Background-object index.
 */
void Backgrnd_On(unsigned short index) {
    CBackground *p_back = static_cast<CBackground *>(BackgroundList);
    while (p_back != 0) {
        if (p_back->mIndex == index) {
            p_back->mFlags &= ~BACKGROUND_DISABLED_FLAG;
            return;
        }
        p_back = static_cast<CBackground *>(p_back->mNextItem);
    }
}


/**
 * Backgrnd_Off__FUs
 *
 * Address:   0x800790FC
 * Mangled:   Backgrnd_Off__FUs
 * Demangled: Backgrnd_Off(unsigned short)
 *
 * @param index  Background-object index.
 */
void Backgrnd_Off(unsigned short index) {
    CBackground *p_back = static_cast<CBackground *>(BackgroundList);
    while (p_back != 0) {
        if (p_back->mIndex == index) {
            p_back->mFlags |= BACKGROUND_DISABLED_FLAG;
            return;
        }
        p_back = static_cast<CBackground *>(p_back->mNextItem);
    }
}


/**
 * __11CBackgroundUiRC8CSVector
 *
 * Address:   0x80079148
 * Mangled:   __11CBackgroundUiRC8CSVector
 * Demangled: CBackground::CBackground(unsigned int, const CSVector &)
 *            Initialize the object and link it into the background list.
 *
 * @param checksum  Asset hash for content lookup.
 * @param ang_vel  Reserved angular-velocity argument; not read here.
 */
CBackground::CBackground(unsigned int checksum, const CSVector &ang_vel) {
    char *pName = pCurrentObjFile;
    _vf = (void **)&__vt_11CBackground;
    InitItem(pName);
    mModel = Spool_GetModel(checksum, mRegion);
    mFlags &= ~BACKGROUND_ITEM_FLAGS_CLEARED_ON_INIT;
    mCBodyFlags &= ~BACKGROUND_BODY_FLAGS_CLEARED_ON_INIT;
    AttachTo(&BackgroundList);
    mIndex = CurrentIndex++;
}


/**
 * _._11CBackground
 *
 * Address:   0x800791DC
 * Mangled:   _._11CBackground
 * Demangled: CBackground::~CBackground()
 *
 * GCC 2.x supplies the hidden in-charge flag and invokes the class allocator
 * when destruction also owns the object's storage.
 */
CBackground::~CBackground() {
    _vf = (void **)&__vt_11CBackground;
    DeleteFrom(&BackgroundList);
    CurrentIndex--;
}


/**
 * AI__11CBackground
 *
 * Address:   0x8007924C
 * Mangled:   AI__11CBackground
 * Demangled: CBackground::AI(void)
 *            Advances the object's angles by its angular velocity.
 */
void CBackground::AI() {
    mAngles += mAngVel;
}
