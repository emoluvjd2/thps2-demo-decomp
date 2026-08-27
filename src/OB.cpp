// OB.cpp -- Reconstructed translation unit
// Original: OB.CPP
// Functions: 38 reconstructed entries
// Address range: 0x800441BC - 0x80045704
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#define __SKIP_CSUPER_EXTERNS__

#define __SKIP_CBODY_EXTERNS__

#define __SKIP_CITEM_EXTERNS__

#define OB_OWNER_DECLARATIONS
#define OB_OWNER_LOCAL_SERVICE_DECLS
#include "OB.h"
#undef OB_OWNER_LOCAL_SERVICE_DECLS
#undef OB_OWNER_DECLARATIONS
#define OB_LAYOUT_ONLY
#define OB_OWNER_LAYOUT
#include "OB.h"
#undef OB_OWNER_LAYOUT
#undef OB_LAYOUT_ONLY
#include "MAIN.h"
#include "PSXREGION.h"
#include "CITEM.h"

/** Return caller-provided storage for construction. */
inline void *operator new(unsigned int, void *storage)
{
    return storage;
}

/**
 * Base particle/bit slice used by the CBody shadow quad.
 * Layout recovered from original/MAIN_STRUCTS.h (struct CBit).
 */
struct CBitOwner {
    struct CBitOwner *mPrevious;
    struct CBitOwner *mNext;
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
};

/**
 * Shadow quad billboard owned through CBody::mpShadow.
 * Layout recovered from original/MAIN_STRUCTS.h (struct CQuadBit).
 */
struct CQuadBitOwner {
    struct CBitOwner CBit;
    struct CVector mPosB;
    struct CVector mPosC;
    struct CVector mPosD;
    struct CVector mTweenPosB;
    struct CVector mTweenPosC;
    struct CVector mTweenPosD;
    u32 mCodeBGR;
    void *mpTexture;
    u8 mFlags;
};

/** Narrow natural interface for the shadow quad services owned by BIT.cpp. */
struct CQuadBit {
    void SetTexture(char *name, int frame);
    void FlagOn(u8 flag);
    void SetSemiTransparent();
    void SetTransparency(u8 transparency);
    void OrientUsing(const CVector &position, const SVECTOR &normal,
                     int width, int height, int flags);
};

typedef char CItem_class_size_check[(sizeof(class CItem) == 76) ? 1 : -1];
typedef char CBitOwner_size_check[(sizeof(struct CBitOwner) == 72) ? 1 : -1];
typedef char CQuadBitOwner_size_check[(sizeof(struct CQuadBitOwner) == 156) ? 1 : -1];
typedef char SVECTOR_owner_size_check[(sizeof(SVECTOR) == 8) ? 1 : -1];

#include "BIT.h"
#include "MEM.h"
#include "UTILS.h"

extern "C" int ObZeroVectorFirstComponent asm("ZeroVector");
extern "C" short ObZeroSVectorFirstComponent asm("ZeroSVector");

// === Owner-local natural ABI hierarchy ===

class CBody : public CItem {
public:
    CBody **mppOriginalList;
    u16 mInputFlags;
    u16 mCBodyFlags;
    CVector mVel;
    CVector mAcc;
    CFriction mFric;
    CSVector mAngVel;
    CSVector mAngAcc;
    CFriction mAngFric;
    CSVector mNormal;
    u8 mEventFlags;
    u32 mEllipsoidRGB;
    u16 mEllipsoidScale;
    s16 mEllipsoidOTPushback;
    CVector mShadowPos;
    CSVector mShadowNormal;
    CQuadBitOwner *mpShadow;
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
    CVector mOldPos;
    CVector mTweenPos;
    CVector mNewPos;
    CVector mDPos;
    CSVector mOldAngles;
    CSVector mNewAngles;
    CSVector mDAngles;

    /** Initialize the item base and physical defaults. */
    CBody();
    /** Insert this body at the head of a linked body list. */
    void AttachTo(CBody **list);
    /** Remove this body from a linked body list. */
    void DeleteFrom(CBody **list);
    /** Decode and store one packed position triple. */
    short *SquirtPos(short *info);
    /** Decode and store one packed angle triple. */
    short *SquirtAngles(short *info);
    /** Move this body from its live list to the suspended list. */
    void Suspend(CBody **originalList);
    /** Restore this body to its saved live list. */
    void UnSuspend();
    /** Enable shadow rendering for this body. */
    void ShadowOn();
    /** Destroy this body's shadow and clear its shadow flag. */
    void KillShadow();
    /** Rebuild or retire this body's shadow and refresh its opacity. */
    void UpdateShadow();
    /** Return whether this body has been marked dead. */
    int IsDead() const;
    ~CBody();
};

typedef char CBody_class_size_check[(sizeof(class CBody) == 264) ? 1 : -1];

class CSuper : public CBody {
public:
    /** Start a bounded animation sequence. */
    void RunAnim(int anim, int from, int to, int to2);
    /** Start a looping animation in the requested playback direction. */
    void CycleAnim(int animation, signed char direction);
    /** Advance the active animation by one scaled frame step. */
    void UpdateFrame();
    /** Assign one model hierarchy slot to an animation hierarchy slot. */
    void SetAnimOrder(u8 modelId, u8 animOrder);
    /** Build the animation-order table by matching hierarchy names. */
    void CalculateAnimOrder(char **animHierarchy, int animPartCount,
                            char **modelHierarchy, int modelPartCount);
    ~CSuper();
};

// === External function declarations ===
#include "SPOOL.h"
#include <STRINGS.H>

// $gp-relative globals
extern int __vt_5CItem;

// === Absolute-addressed globals ===
#include "M3DDEFAULTLIGHT.h"

// === Free function declarations ===
#define OB_OWNER_SERVICE_DECLARATIONS
#include "OB.h"
#undef OB_OWNER_SERVICE_DECLARATIONS
void Ob_MaybeUnSuspendOrCull(void);

// === Extern "C" forward declarations ===

extern "C" void CItem_dtor(CItemOwner *, int inCharge) asm("_._5CItem");
extern "C" void CBody_dtor(CBodyOwner *, int inCharge) asm("_._5CBody");
extern "C" void CBody_Die(CBodyOwner *) asm("Die__5CBody");
extern "C" void CBody_AI(CBodyOwner *) asm("AI__5CBody");
extern "C" void CBody_DoLightingSetup(CBodyOwner *)
    asm("DoLightingSetup__5CBody");
extern "C" int CBody_Hit(CBodyOwner *, int Damage, const CVector &knock, int whathit)
    asm("Hit__5CBodyiRC7CVectori");
extern "C" void CBody_DeleteStuff(CBodyOwner *) asm("DeleteStuff__5CBody");
extern "C" CSuperOwner *CSuper_ctor(CSuperOwner *) asm("__6CSuper");
extern "C" void CSuper_dtor(CSuperOwner *, int inCharge) asm("_._6CSuper");
extern "C" char Ob_ShadowTextureName[] asm("s_Shadow_800d9b4c");
#include "GSKATER_STATE.h"
/** Lowered aggregate-return ABI for CVector subtraction. */
extern "C" void Ob_VectorSubtract(
    CVectorOwner *, const CVectorOwner *, const CVectorOwner *)
    asm("__mi__FRC7CVectorT0");
extern "C" void Ob_VectorDivide(
    CVectorOwner *, const CVectorOwner *, const int *)
    asm("__dv__FRC7CVectorRCi");
extern "C" void Ob_SVectorSubtract(
    CSVectorOwner *, const CSVectorOwner *, const CSVectorOwner *)
    asm("__mi__FRC8CSVectorT0");
/** Typed views of the CQuadBit methods used by the body shadow. */
extern "C" CQuadBitOwner *CQuadBit_ctor(CQuadBitOwner *) asm("__8CQuadBit");

extern "C" {

/** Active and suspended environmental-object list heads. */
CBody *EnvironmentalObjectList
    __attribute__((section(".sdata"))) = 0;
CBody *SuspendedList
    __attribute__((section(".sdata"))) = 0;

/** Canonical zero vectors initialized again by the TU global initializer. */
CVector ZeroVector
    __attribute__((section(".sdata"))) = {0, 0, 0};
CSVector ZeroSVector
    __attribute__((section(".sdata"))) = {0, 0, 0};

/** Distance beyond which environmental objects may be suspended. */
unsigned int SuspendDistance
    __attribute__((section(".sdata"))) = 13000;

/** Texture name used by the body shadow quad. */
static char Ob_ShadowTextureName_owner[8] asm("s_Shadow_800d9b4c")
    __attribute__((section(".sdata"))) = "Shadow";

/** Base item dispatch table: header words followed by its destructor. */
int Ob_CItemVtable_owner[3] asm("__vt_5CItem")
    __attribute__((section(".sdata"))) = {
        0,
        0,
        (int)&CItem_dtor,
};

}

/**
 * Super and body virtual dispatch tables in their original read-only order.
 *
 * Each table contains the two GCC 2.x header words and seven virtual slots.
 * Eight-byte alignment contributes one zero padding word between the tables.
 */
extern "C" {
int __vt_6CSuper[9]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&CSuper_dtor,
        (int)&CBody_Die,
        (int)&CBody_AI,
        (int)&CBody_EveryFrame,
        (int)&CBody_DoLightingSetup,
        (int)&CBody_Hit,
        (int)&CBody_DeleteStuff,
};

int __vt_5CBody[9]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&CBody_dtor,
        (int)&CBody_Die,
        (int)&CBody_AI,
        (int)&CBody_EveryFrame,
        (int)&CBody_DoLightingSetup,
        (int)&CBody_Hit,
        (int)&CBody_DeleteStuff,
};
}

// === Virtual dispatch helpers ===
//
// GCC 2.x vtables carry two header words, so slot k of the tables above is
// method k-2: [2] destructor, [3] Die, [4] AI, [5] EveryFrame,
// [6] DoLightingSetup, [7] Hit, [8] DeleteStuff.

/** Invoke a CBit's virtual destructor with in-charge flag 3 (also frees). */
static inline void DeleteBit(CBitOwner *pBit) {
    ((void (*)(CBitOwner *, int))((int *)pBit->_vf)[2])(pBit, 3);
}

/** Invoke a CBody's virtual destructor with in-charge flag 3 (also frees). */
static inline void DeleteBody(CBodyOwner *pBody) {
    ((void (*)(CBodyOwner *, int))((int *)pBody->CItem.vtable)[2])(pBody, 3);
}

/** Dispatch CBody::AI() through the object's vtable. */
static inline void CallAI(CBodyOwner *pBody) {
    ((void (*)(CBodyOwner *))((int *)pBody->CItem.vtable)[4])(pBody);
}

/** Dispatch CBody::EveryFrame() through the object's vtable. */
static inline void CallEveryFrame(CBodyOwner *pBody) {
    ((void (*)(CBodyOwner *))((int *)pBody->CItem.vtable)[5])(pBody);
}

/** Dispatch CBody::DeleteStuff() through the object's vtable. */
static inline void CallDeleteStuff(CBodyOwner *pBody) {
    ((void (*)(CBodyOwner *))((int *)pBody->CItem.vtable)[8])(pBody);
}

// === Function implementations (address order) ===

/**
 * CItem::operator new(unsigned int) — allocate and zero-fill
 *
 * Address: 0x800441BC
 * Mangled: __nw__5CItemUi
 *
 * @param size  Buffer size in bytes.
 * @return Newly allocated, zero-filled storage.
 */
void *CItem::operator new(unsigned int size)
{
    void *pnew = Mem_New(size, 0, 1);
    int *p = (int *)pnew;
    unsigned int words = (size + 3) >> 2;
    unsigned int i = 0;
    while (i < words) {
        *p = 0;
        i++;
        p++;
    }
    return pnew;
}


/**
 * CItem::operator delete(void *) — calls Mem_Delete
 *
 * Address:   0x80044218
 * Mangled:   __dl__5CItemPv
 * Demangled: CItem::operator delete(void *)
 *
 * @param p  Allocation to release.
 */
void CItem::operator delete(void *p)
{
    Mem_Delete(p);
}


/**
 * CItem constructor — set vtable, default light, and color values
 *
 * Address: 0x80044238
 * Mangled: __5CItem
 *
 * @return `this` pointer (GCC 2.x ctor convention).
 */
CItem::CItem()
{
    _vf = (void **)&__vt_5CItem;
    CSVector *scale = &mScale;
    scale->vx = 0x1000;
    scale->vy = 0x1000;
    scale->vz = 0x1000;
    mpLight = &M3d_DefaultLight;
}


/**
 * Restore CItem's base vtable during destruction.
 *
 * Address:   0x80044268
 * Mangled:   _._5CItem
 * Demangled: CItem::~CItem()
 *
 */
CItem::~CItem()
{
    _vf = (void **)&__vt_5CItem;
}


/**
 * CItem::InitItem(char *name) — find region and store slot
 *
 * Address: 0x80044298
 * Mangled: InitItem__5CItemPc
 *
 * @param pName  Asset / object name string.
 */
void CItem::InitItem(char *pName)
{
    mRegion = Spool_FindRegion(pName);
    mModel = 0;
}


/**
 * Configures a timed color flash on this item.
 *
 * Address:   0x800442C8
 * Mangled:   Flash__5CItemiUcUcUc
 * Demangled: CItem::Flash(int, unsigned char, unsigned char, unsigned char)
 *
 * @param frames  Frame count.
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 */
void CItem::Flash(int frames, unsigned char r, unsigned char g, unsigned char b)
{
    unsigned int rr = r;
    unsigned int bb = b;
    unsigned int gg = g;
    unsigned short flags;
    if (frames == 0)
    {
        flags = mFlags;
        mFlashTimer = 0;
        mRGB = (bb << 16) | (gg << 8) | rr;
        flags |= 0x400;
        mFlags = flags;
        return;
    }
    else
    {
        if (frames < 0)
        {
            // duration < 0: fade from center
            mFlashTimer = (unsigned short)-frames;
            mFlash_dR = (short)((int)(rr - 0x80) / (int)mFlashTimer);
            mFlash_dG = (short)((int)(gg - 0x80) / (int)mFlashTimer);
            mFlash_dB = (short)((int)(bb - 0x80) / (int)mFlashTimer);
            mRGB = 0x808080;
            mFlags = mFlags | 0x400;
            return;
        }
        // duration > 0: fade to center
        mFlashTimer = (short)frames + 1;
        mFlash_dR = (short)((int)(0x80 - rr) / frames);
        mFlash_dG = (short)((int)(0x80 - gg) / frames);
        mFlash_dB = (short)((int)(0x80 - bb) / frames);
        if ((mStoredRGB & 0x1000000) == 0)
        {
            unsigned int sv = (mRGB & 0xffffff) | 0x1000000;
            mStoredRGB = sv;
            if ((mFlags & 0x400) != 0)
            {
                mStoredRGB = sv | 0x2000000;
            }
        }
        flags = mFlags;
    }
    mRGB = (bb << 16) | (gg << 8) | rr;
    flags |= 0x400;
    mFlags = flags;
}


/**
 * Advances an active item flash and restores the saved color when it expires.
 *
 * Address:   0x80044408
 * Mangled:   FlashUpdate__5CItem
 * Demangled: CItem::FlashUpdate(void)
 *
 */
void CItem::FlashUpdate()
{
    if (mFlashTimer != 0)
    {
        mFlashTimer--;

        unsigned int color = mRGB;
        int r = color & 0xff;
        int g = (color >> 8) & 0xff;
        int b = (color >> 16) & 0xff;
        r += mFlash_dR;
        g += mFlash_dG;
        b += mFlash_dB;
        mRGB = ((b & 0xff) << 16) | ((g & 0xff) << 8) | (r & 0xff);

        mFlags |= 0x400;
        if (mFlashTimer == 0)
        {
            if ((mStoredRGB & 0x02000000) == 0)
                mFlags &= 0xfbff;
            else
                mFlags |= 0x400;
            mRGB = mStoredRGB;
            mStoredRGB = 0;
        }
    }
}


/**
 * Initialize CBody's base item and physical defaults.
 *
 * Address:   0x800444B4
 * Mangled:   __5CBody
 * Demangled: CBody::(void)
 *
 */
CBody::CBody() {
    CBodyOwner *body = reinterpret_cast<CBodyOwner *>(this);
    body->CItem.vtable = (void *)&__vt_5CBody[0];
    body->mFric.Set(1, 1, 1);
    body->mAngFric.Set(1, 1, 1);
    body->mPushVal = 10;
    body->mNode = 0xFFFF;
    body->mShadowScale = 50;
    body->mShadowThreshold = 300;
    body->mRMinor = 50;
    body->mCBodyFlags = body->mCBodyFlags | 0x12;
}


/**
 * Destroy CBody-owned shadow state.
 *
 * Address:   0x8004453C
 * Mangled:   _._5CBody
 * Demangled: CBody::~CBody()
 *
 * The compiler supplies the original hidden in-charge flag and emits the
 * target `_._5CBody` ABI entry.
 */
CBody::~CBody() {
    CBodyOwner *body = (CBodyOwner *)this;
    body->CItem.vtable = (void *)&__vt_5CBody[0];
    CQuadBitOwner *shadow = (CQuadBitOwner *)body->mpShadow;
    if (shadow != 0) {
        DeleteBit(&shadow->CBit);
    }
}


/**
 * CBody::AttachTo(CBody **list) — insert this at head of linked list
 *
 * Address: 0x800445B4
 * Mangled: AttachTo__5CBodyPP5CBody
 *
 * @param ppList  List head to update.
 */
void CBody::AttachTo(CBody **list) {
    CBodyOwner *body = reinterpret_cast<CBodyOwner *>(this);
    CBodyOwner **ppList = reinterpret_cast<CBodyOwner **>(list);
    CBodyOwner *oldHead = *ppList;
    body->CItem.mPreviousItem = 0;
    body->CItem.mNextItem = &oldHead->CItem;
    *ppList = body;                      // *ppList = this
    CBodyOwner *next = (CBodyOwner *)body->CItem.mNextItem;
    if (next) {
        next->CItem.mPreviousItem = &body->CItem;
    }
}


/**
 * CBody::DeleteFrom(CBody **list) — remove from doubly-linked list
 *
 * Address: 0x800445E0
 * Mangled: DeleteFrom__5CBodyPP5CBody
 *
 * @param ppList  List head to update.
 */
void CBody::DeleteFrom(CBody **ppList) {
    if ((mCBodyFlags & 1) != 0) {
        if (ppList != &SuspendedList) {
            UnSuspend();
        }
    }
    CBody *next = (CBody *)mNextItem;
    if (next) {
        next->mPreviousItem = mPreviousItem;
    }
    CBody *prev = (CBody *)mPreviousItem;
    if (prev) {
        prev->mNextItem = mNextItem;
    }
    if (*ppList == this) {
        *ppList = (CBody *)mNextItem;
    }
}


/**
 * CBody::SquirtPos(short *src) — copy 3 positions with <<12 shift, store to pos + savePos
 *
 * Address: 0x80044684
 * Mangled: SquirtPos__5CBodyPs
 *
 * @param p_info  Packed position stream cursor.
 * @return Cursor immediately after the consumed position words.
 */
short *CBody::SquirtPos(short *p_info) {
    CBodyOwner *body = reinterpret_cast<CBodyOwner *>(this);
    int *src = (int *)p_info;
    int x = src[0] << 12;
    body->mTweenPos.x = x;
    body->CItem.mPos.x = x;
    int y = src[1] << 12;
    body->mTweenPos.y = y;
    body->CItem.mPos.y = y;
    int z = src[2] << 12;
    body->mTweenPos.z = z;
    body->CItem.mPos.z = z;
    return (short *)(src + 3);
}


/**
 * CBody::SquirtAngles(short *) — copy 3 angles from source, return ptr past data
 *
 * Address: 0x800446C4
 * Mangled: SquirtAngles__5CBodyPs
 *
 * @param p_info  Packed angle stream cursor.
 * @return Cursor immediately after the consumed angle words.
 */
short *CBody::SquirtAngles(short *p_info) {
    CBodyOwner *body = reinterpret_cast<CBodyOwner *>(this);
    body->CItem.mAngles.x = *p_info++;
    body->CItem.mAngles.y = *p_info++;
    body->CItem.mAngles.z = *p_info;
    return p_info + 1;
}


/**
 * CBody::Suspend(CBody **list) — move from list to SuspendedList
 *
 * Address: 0x800446EC
 * Mangled: Suspend__5CBodyPP5CBody
 *
 * @param ppOriginalList  Original list head saved for restoration.
 */
void CBody::Suspend(CBody **originalList) {
    CBodyOwner *body = reinterpret_cast<CBodyOwner *>(this);
    CBodyOwner **ppOriginalList =
        reinterpret_cast<CBodyOwner **>(originalList);
    CallDeleteStuff(body);
    body->mppOriginalList = ppOriginalList;
    reinterpret_cast<CBody *>(body)->DeleteFrom(
        reinterpret_cast<CBody **>(ppOriginalList));
    reinterpret_cast<CBody *>(body)->AttachTo(&SuspendedList);
    u16 flags = body->mCBodyFlags;
    body->mCBodyFlags = flags | 1;
}


/**
 * CBody::UnSuspend(void) — move from SuspendedList back to saved list
 *
 * Address: 0x80044754
 * Mangled: UnSuspend__5CBody
 *
 */
void CBody::UnSuspend() {
    CBodyOwner *body = reinterpret_cast<CBodyOwner *>(this);
    if (((body->mCBodyFlags ^ 1) & 1) == 0) {
        reinterpret_cast<CBody *>(body)->DeleteFrom(&SuspendedList);
        CBodyOwner **savedList = body->mppOriginalList;
        reinterpret_cast<CBody *>(body)->AttachTo(
            reinterpret_cast<CBody **>(savedList));
        u16 flags = body->mCBodyFlags;
        body->mCBodyFlags = flags & 0xFFFE;
    }
}


/**
 * CBody::ShadowOn(void) — set shadow flag (bit 3)
 *
 * Address: 0x800447B4
 *          Target: lhu $v0,0x52($a0); nop; ori $v0,8; jr $ra; sh $v0,0x52($a0)
 * Mangled: ShadowOn__5CBody
 */
void CBody::ShadowOn() {
    CBodyOwner *body = reinterpret_cast<CBodyOwner *>(this);
    body->mCBodyFlags = body->mCBodyFlags | 8;
}


/**
 * CBody::KillShadow(void) — destroy shadow object, clear flag
 *
 * Address: 0x800447C8
 * Mangled: KillShadow__5CBody
 */
void CBody::KillShadow() {
    CBodyOwner *body = reinterpret_cast<CBodyOwner *>(this);
    body->mCBodyFlags = body->mCBodyFlags & 0xFFF7;
    CQuadBitOwner *shadow = (CQuadBitOwner *)body->mpShadow;
    if (shadow) {
        DeleteBit(&shadow->CBit);
        body->mpShadow = 0;
    }
}


/**
 * Rebuild or retire the shadow quad bit and refresh its opacity.
 *
 * Address:   0x80044818
 * Mangled:   UpdateShadow__5CBody
 * Demangled: CBody::UpdateShadow(void)
 *
 */
void CBody::UpdateShadow() {
    CBodyOwner *_this = reinterpret_cast<CBodyOwner *>(this);
    SVECTOR up;

    if ((_this->mCBodyFlags & 8) != 0) {
        if (_this->mpShadow == (void *)0x0) {
            UseCacheForBits = 0;
            CQuadBitOwner *shadow = static_cast<CQuadBitOwner *>(
                Bit_AllocateStorage(sizeof(CQuadBitOwner)));
            if (shadow != (CQuadBitOwner *)0x0) {
                shadow = CQuadBit_ctor(shadow);
            }
            _this->mpShadow = shadow;
            UseCacheForBits = 1;
            reinterpret_cast<CQuadBit *>(shadow)->SetTexture(
                Ob_ShadowTextureName, 0);
            reinterpret_cast<CQuadBit *>(_this->mpShadow)->FlagOn(4);
            reinterpret_cast<CQuadBit *>(_this->mpShadow)
                ->SetSemiTransparent();
            ((CQuadBitOwner *)_this->mpShadow)->CBit.mFrigDeltaZ = 0x20;
            ((CQuadBitOwner *)_this->mpShadow)->CBit.mProtected = 1;
        }
        if (_this->mShadowNormal.y < -0x80) {
            up.vx = _this->mShadowNormal.x;
            up.vz = _this->mShadowNormal.z;
            up.vy = _this->mShadowNormal.y;
            reinterpret_cast<CQuadBit *>(_this->mpShadow)->OrientUsing(
                *(const CVector *)&_this->mShadowPos, up,
                _this->mShadowScale, _this->mShadowScale, 0);
        }
        int trans = (((int)_this->mShadowThreshold - (int)_this->mShadowDist) * 0x80) / (int)_this->mShadowThreshold;
        if (trans < 0) {
            trans = 0;
        }
        trans /= 6;
        reinterpret_cast<CQuadBit *>(_this->mpShadow)
            ->SetTransparency((u8)trans);
    } else {
        reinterpret_cast<CBody *>(_this)->KillShadow();
    }
}


/**
 * CBody::Die(void) — mark as dead if not already
 *
 * Address: 0x80044974
 * Mangled: Die__5CBody
 *
 * @param body  Body marked for deletion.
 */
void CBody_Die(CBodyOwner *body) {
    if (!reinterpret_cast<const CBody *>(body)->IsDead()) {
        body->mCBodyFlags = body->mCBodyFlags | 0x40;
    }
}


/**
 * CBody::IsDead(void) const — check if dead flag is set
 *
 * Address: 0x800449B0
 * Mangled: IsDead__C5CBody
 *
 * @return flags != 0
 */
int CBody::IsDead() const {
    const CBodyOwner *bodyView =
        reinterpret_cast<const CBodyOwner *>(this);
    u32 flags = bodyView->mCBodyFlags;
    flags = flags & 0x40;
    return flags != 0;
}


/**
 * CBody::Hit(int, const CVector &, int) — always returns 1
 *
 * Address: 0x800449C4
 *          Target: jr $ra; addiu $v0, $zero, 1
 * Mangled: Hit__5CBodyiRC7CVectori
 *
 * @param body  Body receiving the hit.
 * @param Damage  Damage amount.
 * @param knock  Impact direction.
 * @param whathit  Impact source identifier.
 * @return 1
 */
int CBody_Hit(
    CBodyOwner *body, int Damage, const CVector &knock, int whathit) {
    (void)body;
    (void)Damage;
    (void)knock;
    (void)whathit;
    return 1;
}


/**
 * Initialize CSuper's body base and animation defaults.
 *
 * Address:   0x800449CC
 * Mangled:   __6CSuper
 * Demangled: CSuper::(void)
 *
 * @param super  Animated body being initialized.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
CSuperOwner *CSuper_ctor(CSuperOwner *super) {
    ::new (reinterpret_cast<CBody *>(&super->CBody)) CBody;
    super->CBody.CItem.vtable = (void *)&__vt_6CSuper[0];
    super->mExtraFlags = 0;
    super->mAnimSpeed = 0x10000;
    super->mNumFrames = 1;
    super->mAnimFinished = 1;
    super->mUseAnimOrder = 0;
    super->CBody.CItem.mFlags = super->CBody.CItem.mFlags | 2;
    return super;
}


/**
 * Destroy CSuper-owned animation buffers.
 *
 * Address:   0x80044A28
 * Mangled:   _._6CSuper
 * Demangled: CSuper::~CSuper()
 *
 * The compiler supplies the original hidden in-charge flag and emits the
 * target `_._6CSuper` ABI entry.
 */
CSuper::~CSuper() {
    CSuperOwner *super = (CSuperOwner *)this;
    super->CBody.CItem.vtable = (void *)&__vt_6CSuper[0];
    if (super->mpPoseBuffer != 0) {
        Mem_Delete((void *)super->mpPoseBuffer);
    }
    if (super->mpJoints != 0) {
        Mem_Delete((void *)super->mpJoints);
    }
    if (super->mpDecompressedFrame != 0) {
        Mem_Delete((void *)super->mpDecompressedFrame);
    }
    if (super->mpCalculationOrder != 0) {
        Mem_Delete((void *)super->mpCalculationOrder);
    }
}


/**
 * Start a bounded animation sequence.
 *
 * Address:   0x80044AD8
 * Mangled:   RunAnim__6CSuperiiii
 * Demangled: CSuper::RunAnim(int, int, int, int)
 *
 * @param anim  Animation index.
 * @param from  Initial frame, or -1 to select the final frame.
 * @param to  Target frame, or -1 to select the final frame.
 * @param to2  Secondary target-frame value.
 */
void CSuper::RunAnim(int anim, int from, int to, int to2) {
    CSuperOwner *super = reinterpret_cast<CSuperOwner *>(this);
    int region = super->CBody.CItem.mRegion;
    // These remaining pins/barriers preserve the target's GCC 2.x allocation;
    // natural unpinned forms were measured and change this exact routine.
    register unsigned int *tbl asm("$4");
    u32 idx;
    int frameCount;

    super->mAnim = anim;
    tbl = PSXRegion[region].pAnimFile;
    asm volatile("" : "+r"(super), "+r"(region));
    idx = super->mAnim;
    if (!(idx < *tbl)) {
        idx = 46;
        asm("" : "+r"(idx));
        super->mAnim = 46;
    }

    tbl = PSXRegion[region].pAnimFile;
    int minusOne = -1;
    {
        // Integer addition preserves the target's base/offset operand order.
        int animationEntry = (idx << 3) + (int)tbl;
        register int rawFrame asm("$2") =
            ((ObAnimationTableOwner *)animationEntry)
                ->mAnimations[0].mNumFrames;
        super->mNumFrames = rawFrame;

        if (from == minusOne) {
            asm("" : "+r"(rawFrame));
            from = (rawFrame & 0xFF) - 1;
            asm volatile("" : "+r"(rawFrame));
        }
    }

    frameCount = super->mNumFrames;
    if (to == minusOne) {
        to = frameCount - 1;
    }

    if (from < 0) {
        from = 0;
    }
    if (to < 0) {
        to = 0;
    }
    if (!(from < frameCount)) {
        from = frameCount - 1;
    }
    if (!(to < frameCount)) {
        to = frameCount - 1;
    }

    super->mAnimMode = 0;
    if (to <= from) {
        if (to < from) {
            super->mAnimDir = minusOne;
        } else {
            super->mAnimDir = 0;
        }
    } else {
        super->mAnimDir = 1;
    }

    int tf = to << 24;
    super->mStartFrame = from;
    super->mFrame = from;
    super->mTargetFrame = to;
    asm("");
    {
        register int spd asm("$2") = to2;
        tf >>= 24;
        super->mFrameFrac = 0;
        super->mTargetFrame2 = spd;
    }
    register int sf asm("$2") = (s16)from;
    super->mAnimFinished = (sf == tf);
}


/**
 * Starts a cycled animation and caches the frame count from the active
 * region's animation table.
 *
 * Address: 0x80044BF4
 * Mangled: CycleAnim__6CSuperiSc
 *
 * @param anim  Animation index.
 * @param animdir  Initial playback direction.
 */
void CSuper::CycleAnim(int anim, signed char animdir) {
    CSuperOwner *super = reinterpret_cast<CSuperOwner *>(this);
    // The two pins keep the region-table base and animation index in the
    // target registers; removing either changes nine instructions.
    register int regionBase asm("$7");
    int regionOffset;
    register int animIndex asm("$2");
    int animTable;
    u8 region;

    region = super->CBody.CItem.mRegion;
    regionBase = (int)PSXRegion;
    super->mAnim = (u16)anim;
    animIndex = super->mAnim;
    regionOffset = region << 6;
    /**
     * Keep the accumulated region pointer in the same register that is reused
     * for the frame-count byte; ordinary pointer addition changes two target
     * instructions in this otherwise exact routine.
     */
    asm("addu %0,%0,%1" : "+r"(regionBase) : "r"(regionOffset));
    animTable = (int)((SPSXRegion *)regionBase)->pAnimFile;
    animIndex = animIndex << 3;
    animIndex = animIndex + animTable;
    {
        int mode = 1;
        regionBase =
            ((ObAnimationTableOwner *)animIndex)->mAnimations[0].mNumFrames;

        super->mAnimMode = (u8)mode;
        super->mAnimDir = animdir;
        super->mFrame = 0;
        super->mFrameFrac = 0;
        super->mAnimFinished = 0;
        super->mNumFrames = (u8)regionBase;
    }
}


/**
 * Advance the active animation by one scaled frame step.
 *
 * Address:   0x80044C40
 * Mangled:   UpdateFrame__6CSuper
 * Demangled: CSuper::UpdateFrame(void)
 *
 * Advances the active animation frame according to mode, direction,
 * target frames, and the global time scale.
 */
void CSuper::UpdateFrame() {
    // The receiver pin and barrier preserve target scheduling (65 and 13
    // instruction mismatches respectively when expressed naturally).
    register CSuperOwner *p asm("$7") =
        reinterpret_cast<CSuperOwner *>(this);
    asm("" : "+r"(p));
// Named-member views retain the target's deliberate signed/unsigned loads.
#define SUPER_S8(member)  (*(signed char *)&p->member)
#define SUPER_U8(member)  (*(u8 *)&p->member)
#define SUPER_S16(member) (*(s16 *)&p->member)
#define SUPER_U16(member) (*(u16 *)&p->member)
#define SUPER_S32(member) (*(int *)&p->member)

    int t;
    int cmp;
    int dir;
    u8 rawDirection;
    u16 rawFrame;
    int speed;
    int AdjustedSpeed;
    int f;
    int frame;

    if (SUPER_U8(mAnimMode) == 0 || SUPER_U8(mAnimMode) == 2) {
        int d = SUPER_S8(mAnimDir);
        if ((d == 1 &&
             (t = SUPER_U8(mTargetFrame),
              SUPER_S16(mFrame) >= SUPER_S8(mTargetFrame))) ||
            (d == -1 &&
             (t = SUPER_U8(mTargetFrame),
              SUPER_S16(mFrame) <= SUPER_S8(mTargetFrame)))) {
            if (SUPER_S8(mTargetFrame2) > 0) {
                SUPER_U8(mTargetFrame) = SUPER_U8(mTargetFrame2);
                SUPER_U8(mTargetFrame2) = t;
                SUPER_U8(mAnimDir) = -SUPER_U8(mAnimDir);
            } else {
                SUPER_U8(mAnimFinished) = 1;
            }
        }
    }

    speed = SUPER_S32(mAnimSpeed);
    {
        unsigned int m;
        AdjustedSpeed = speed * TimeScale >> 8;
        m = SUPER_U8(mAnimMode);
        rawDirection = SUPER_U8(mAnimDir);
        rawFrame = SUPER_U16(mFrame);
        if (m - 2 < 2u) {
            AdjustedSpeed = 0;
        }
    }

    {
        int lo = SUPER_U16(mFrameFrac);
        int hi = rawFrame << 16;
        dir = (signed char)rawDirection;
        f = hi | lo;
    }
    if (dir == 1) {
        f += AdjustedSpeed;
    }
    if (dir == -1) {
        f -= AdjustedSpeed;
    }

    frame = f >> 16;
    SUPER_S16(mFrame) = (s16)frame;
    AdjustedSpeed = SUPER_U8(mAnimMode);
    SUPER_U16(mFrameFrac) = (u16)f;

    switch (AdjustedSpeed) {
    case 0:
        if (dir != 1 ||
            (cmp = frame < SUPER_S8(mTargetFrame),
             t = SUPER_U8(mTargetFrame), cmp)) {
            if (dir != -1) {
                return;
            }
            cmp = SUPER_S8(mTargetFrame) < frame;
            t = SUPER_U8(mTargetFrame);
            if (cmp) {
                return;
            }
        }
        SUPER_S16(mFrame) = (signed char)t;
        return;

    case 4:
        if (dir != 1 ||
            (cmp = frame < SUPER_S8(mTargetFrame),
             t = SUPER_U8(mTargetFrame), cmp)) {
            if (dir != -1) {
                return;
            }
            cmp = SUPER_S8(mTargetFrame) < frame;
            t = SUPER_U8(mTargetFrame);
            if (cmp) {
                return;
            }
        }
        {
            u8 od = SUPER_U8(mAnimDir);
            u8 oldStart = SUPER_U8(mStartFrame);
            SUPER_S16(mFrame) = (signed char)t;
            SUPER_S16(mStartFrame) = (signed char)t;
            SUPER_U8(mAnimDir) = -od;
            SUPER_U8(mTargetFrame) = oldStart;
        }
        return;

    case 3: {
        int dif = SUPER_S16(mPingPongEnd) - SUPER_S16(mPingPongStart);
        if (dif > 0) {
            int width = dif * 2;
            int x =
                ((XFrames - SUPER_S16(mPingPongFrame)) /
                 (0x20000 / speed)) % width;
            if (dif < x) {
                x = width - x;
            }
            SUPER_S16(mFrame) = SUPER_U16(mPingPongStart) + x;
        } else {
            int width;
            int x;
            dif = -dif;
            width = dif * 2;
            x = ((XFrames - SUPER_S16(mPingPongFrame)) /
                 (0x20000 / speed)) % width;
            if (dif < x) {
                x = width - x;
            }
            SUPER_S16(mFrame) = SUPER_U16(mPingPongStart) - x;
        }
        return;
    }

    case 1:
        if (SUPER_U8(mNumFrames) <= frame) {
            SUPER_S16(mFrame) = 0;
        }
        if (SUPER_S16(mFrame) < 0) {
            SUPER_S16(mFrame) = SUPER_U8(mNumFrames) - 1;
        }
        return;
    }

#undef SUPER_S8
#undef SUPER_U8
#undef SUPER_S16
#undef SUPER_U16
#undef SUPER_S32
}


/**
 * CSuper::SetAnimOrder(unsigned char slot, unsigned char order)
 *
 * Address: 0x80044F24
 * Mangled: SetAnimOrder__6CSuperUcUc
 *
 * @param modelId  Model hierarchy slot.
 * @param animOrder  Matching animation hierarchy slot.
 */
void CSuper::SetAnimOrder(u8 modelId, u8 animOrder) {
    CSuperOwner *super = reinterpret_cast<CSuperOwner *>(this);
    super->mAnimOrder[modelId] = animOrder;
    super->mUseAnimOrder = 1;
}


/**
 * Build the animation-order table by matching hierarchy names.
 *
 * Address:   0x80044F3C
 * Mangled:   CalculateAnimOrder__6CSuperPPciT1i
 * Demangled: CSuper::CalculateAnimOrder(char **, int, char **, int)
 *
 * @param pAnimHierarchy  Animation hierarchy names.
 * @param animPartCount  Number of animation hierarchy names.
 * @param pModelHierarchy  Model hierarchy names.
 * @param modelPartCount  Number of model hierarchy names.
 */
void CSuper::CalculateAnimOrder(
    char **pAnimHierarchy, int animPartCount,
    char **pModelHierarchy, int modelPartCount) {
    // The saved-register assignment is load-bearing: removing these pins
    // changes 51 instructions even though the control flow is unchanged.
    register int oc asm("$22") = modelPartCount;
    register int i asm("$18") = 0;
    register CSuperOwner *s asm("$23") =
        reinterpret_cast<CSuperOwner *>(this);
    register char **np asm("$30") = pAnimHierarchy;
    register int nc asm("$21") = animPartCount;
    register char **op asm("$20");
    if (oc <= 0) goto done;
    op = pModelHierarchy;
    // Preserve the target's saved-register initialization order.
    asm("" : "+r"(oc), "+r"(i), "+r"(np), "+r"(s), "+r"(nc));
loop_outer:
    {
        register int j asm("$17");
        register char **oSrc asm("$19");
        register char **nSrc asm("$16");
        if (nc <= 0) goto skip_inner;
        j = 0;
        oSrc = op;
        nSrc = np;
    loop_inner:
        if (strcmp(*oSrc, *nSrc) != 0) goto next_inner;
        reinterpret_cast<CSuper *>(s)->SetAnimOrder(
            i & 0xFF, j & 0xFF);
        op++;
        goto next_outer;
    next_inner:
        j++;
        if (j < nc) {
            nSrc++;
            goto loop_inner;
        }
    skip_inner:
        op++;
    }
next_outer:
    i++;
    if (i < oc) goto loop_outer;
done:
    ;
}


/**
 * Interleave expensive AI samples across frames.
 *
 * Address:   0x80045014
 * Mangled:   InterleaveAI__5CBody
 * Demangled: CBody::InterleaveAI(void)
 *
 * Runs body AI at its requested cadence and interpolates position/angles
 * between full AI samples.
 */
void CBody_InterleaveAI(CBody *body) {
    CSuperOwner *super = reinterpret_cast<CSuperOwner *>(body);
    if ((super->CBody.CItem.mFlags & 2) != 0) {
        reinterpret_cast<CSuper *>(super)->UpdateFrame();
        CallEveryFrame(&super->CBody);
        if (super->CBody.mAIFrequency < 2) {
            super->mNewFrame = *(u16 *)&super->mFrame;
            CallAI(&super->CBody);
            super->mOldFrame = *(u16 *)&super->mFrame;
            super->mOldAnim = *(u8 *)&super->mAnim;
            super->mOldAnimDir = *(u8 *)&super->mAnimDir;
            return;
        }
    } else {
        CallEveryFrame(&super->CBody);
        if (super->CBody.mAIFrequency < 2) {
            CallAI(&super->CBody);
            return;
        }
    }

    int cnt = super->CBody.mAICount + 1;
    super->CBody.mAICount = (u8)cnt;
    if ((u8)cnt >= super->CBody.mAIFrequency) {
        super->CBody.mAICount = 0;
        super->CBody.CItem.mPos = super->CBody.mNewPos;
        super->CBody.mOldPos = super->CBody.CItem.mPos;
        super->CBody.CItem.mAngles = super->CBody.mNewAngles;
        super->CBody.mOldAngles = super->CBody.CItem.mAngles;

        if ((super->CBody.CItem.mFlags & 2) != 0) {
            super->mNewFrame = *(u16 *)&super->mFrame;
            CallAI(&super->CBody);
            super->mOldFrame = *(u16 *)&super->mFrame;
            super->mOldAnim = *(u8 *)&super->mAnim;
            super->mOldAnimDir = *(u8 *)&super->mAnimDir;
        } else {
            CallAI(&super->CBody);
        }

        {
            CVectorOwner stepPos;
            CVectorOwner deltaPos;
            int divisor;

            super->CBody.mNewPos = super->CBody.CItem.mPos;
            super->CBody.CItem.mPos = super->CBody.mOldPos;
            Ob_VectorSubtract(&deltaPos, &super->CBody.mNewPos, &super->CBody.mOldPos);
            divisor = super->CBody.mAIFrequency;
            Ob_VectorDivide(&stepPos, &deltaPos, &divisor);
            super->CBody.mDPos = stepPos;
        }

        {
            CSVectorOwner deltaAngles;

            super->CBody.mNewAngles = super->CBody.CItem.mAngles;
            super->CBody.CItem.mAngles = super->CBody.mOldAngles;
            Ob_SVectorSubtract(&deltaAngles, &super->CBody.mNewAngles, &super->CBody.mOldAngles);
            super->CBody.mDAngles = deltaAngles;

            if (super->CBody.mDAngles.x < -0x800) super->CBody.mDAngles.x += 0x1000;
            if (super->CBody.mDAngles.x > 0x800) super->CBody.mDAngles.x -= 0x1000;
            if (super->CBody.mDAngles.y < -0x800) super->CBody.mDAngles.y += 0x1000;
            if (super->CBody.mDAngles.y > 0x800) super->CBody.mDAngles.y -= 0x1000;
            if (super->CBody.mDAngles.z < -0x800) super->CBody.mDAngles.z += 0x1000;
            if (super->CBody.mDAngles.z > 0x800) super->CBody.mDAngles.z -= 0x1000;

            reinterpret_cast<CSVector &>(super->CBody.mDAngles) /=
                super->CBody.mAIFrequency;
        }
        return;
    }

    *(CVector *)&super->CBody.CItem.mPos +=
        *(const CVector *)&super->CBody.mDPos;
    reinterpret_cast<CSVector &>(super->CBody.CItem.mAngles) +=
        reinterpret_cast<const CSVector &>(super->CBody.mDAngles);
    reinterpret_cast<CSVector &>(super->CBody.CItem.mAngles).Mask();
}


/**
 * Ob_ClearInputFlags(CBody *) — walk linked list clearing input flags (keep bit 0)
 *
 * Address: 0x80045394
 * Mangled: Ob_ClearInputFlags__FP5CBody
 *
 * @param pBody  Body to operate on.
 */
void Ob_ClearInputFlags(CBody * pBody) {
    CBodyOwner *body = (CBodyOwner *)pBody;
    if (!body) return;
    CBodyOwner *pNext = (CBodyOwner *)body->CItem.mNextItem;
loop:
    body->mInputFlags = body->mInputFlags & 1;
    // Prevent the store from moving into the null-check delay slot.
    asm("");
    if (!pNext) return;
    body = pNext;
    pNext = (CBodyOwner *)pNext->CItem.mNextItem;
    goto loop;
}


/**
 * Ob_AI(CBody **) — update AI for every body in a list.
 *
 * Address:   0x800453CC
 * Mangled:   Ob_AI__FPP5CBody
 * Demangled: Ob_AI(CBody **)
 *
 * @param ppBody  List head pointer to process; suspended bodies may be removed.
 */
void Ob_AI(CBody ** ppBody) {
    CBodyOwner *pBody = (CBodyOwner *)*ppBody;

    while (pBody) {
        CBodyOwner *pNext = (CBodyOwner *)pBody->CItem.mNextItem;

        if (pBody->CItem.mFlashTimer != 0) {
            ((CItem *)pBody)->FlashUpdate();
        }

        unsigned short flags = pBody->mCBodyFlags;
        if ((flags & 0x40) != 0) {
            if ((flags & 0x80) != 0) {
                if (pBody != 0) {
                    DeleteBody(pBody);
                }
            }
            else {
                pBody->mCBodyFlags = flags | 0x80;
            }
        }
        else {
            unsigned int dist = Utils_CrapDist(
                *(CVector *)&reinterpret_cast<CBrucePositionOwner *>(GSkater[0])
                    ->CSuper.CBody.CItem.mPos,
                *(CVector *)&pBody->CItem.mPos);

            if (GNumberOfPlayers == 2) {
                unsigned int dist2 = Utils_CrapDist(
                    *(CVector *)&reinterpret_cast<CBrucePositionOwner *>(GSkater[1])
                        ->CSuper.CBody.CItem.mPos,
                    *(CVector *)&pBody->CItem.mPos);
                if (dist2 < dist) {
                    dist = dist2;
                }
            }

            pBody->mPlayerDist = dist;
            if ((pBody->mCBodyFlags & 2) != 0 && dist > SuspendDistance) {
                reinterpret_cast<CBody *>(pBody)->Suspend(ppBody);
            }
            else {
                CBody_InterleaveAI(reinterpret_cast<CBody *>(pBody));
                if ((pBody->mCBodyFlags & 8) != 0) {
                    reinterpret_cast<CBody *>(pBody)->UpdateShadow();
                }
            }
        }

        pBody = pNext;
    }
}


/**
 * Ob_QuickAI__FPP5CBody
 *
 * Address:   0x80045540
 * Mangled:   Ob_QuickAI__FPP5CBody
 * Demangled: Ob_QuickAI(CBody **)
 *
 * @param ppBody  List head pointer to process.
 */
void Ob_QuickAI(CBody ** ppBody) {
    CBodyOwner *pBody = (CBodyOwner *)*ppBody;
    CBodyOwner *pNext;
    unsigned short flags;

    if (!pBody) goto end;
    pNext = (CBodyOwner *)pBody->CItem.mNextItem;

top:
    flags = pBody->mCBodyFlags;

    if (flags & 0x40) {
        if (flags & 0x80) {
            if (pBody) {
                DeleteBody(pBody);
            }
        } else {
            pBody->mCBodyFlags = flags | 0x80;
        }
    } else {
        CallAI(pBody);
    }

    pBody = pNext;
    if (!pNext) goto end;
    pNext = (CBodyOwner *)pNext->CItem.mNextItem;
    goto top;

end:
    ;
}


/**
 * Restore nearby suspended bodies to their original lists.
 *
 * Address:   0x800455EC
 * Mangled:   Ob_MaybeUnSuspendOrCull__Fv
 * Demangled: Ob_MaybeUnSuspendOrCull(void)
 */
#include "MECHLIST.h"
void Ob_MaybeUnSuspendOrCull(void)
{
    // These list-cursor pins retain the target loop allocation; unpinned
    // forms change 14 and 11 instructions respectively.
    register CBodyOwner *pBody asm("$17") = (CBodyOwner *)SuspendedList;
    if (pBody == (CBodyOwner *)0) return;
    register CBodyOwner *pNext asm("$19") =
        (CBodyOwner *)pBody->CItem.mNextItem;
top:
    if ((pBody->mCBodyFlags & 2) != 0) {
        CVector *pBodyPos = (CVector *)&pBody->CItem.mPos;
        unsigned int dist = (unsigned int)Utils_CrapDist(
            *(CVector *)&((CBodyOwner *)MechList)->CItem.mPos, *pBodyPos);
        if (GNumberOfPlayers == 2) {
            unsigned int dist2 = (unsigned int)Utils_CrapDist(
                *(CVector *)&((CBodyOwner *)((CBodyOwner *)MechList)
                                  ->CItem.mNextItem)
                    ->CItem.mPos,
                *pBodyPos);
            if (dist2 < dist) {
                dist = dist2;
            }
        }
        if (SuspendDistance >= dist) {
            reinterpret_cast<CBody *>(pBody)->UnSuspend();
        }
    }
    if (pNext == (CBodyOwner *)0) return;
    pBody = pNext;
    pNext = (CBodyOwner *)pNext->CItem.mNextItem;
    goto top;
}


/**
 * Initialize the translation unit's global zero vectors.
 *
 * Address: 0x800456C4
 * Mangled: _GLOBAL_.I.EnvironmentalObjectList
 */
extern "C" void Ob_GlobalInit_EnvironmentalObjectList(void) asm("_GLOBAL_.I.EnvironmentalObjectList");
void Ob_GlobalInit_EnvironmentalObjectList(void)
{
    (&ObZeroVectorFirstComponent)[0] = 0;
    (&ObZeroVectorFirstComponent)[1] = 0;
    (&ObZeroVectorFirstComponent)[2] = 0;
    (&ObZeroSVectorFirstComponent)[0] = 0;
    (&ObZeroSVectorFirstComponent)[1] = 0;
    (&ObZeroSVectorFirstComponent)[2] = 0;
}

/**
 * `.ctors` table slot for this owner's hand-written global constructor.
 *
 * GCC emits this entry itself for a compiler-generated constructor, but this
 * owner's hook is written by hand, so the slot is declared explicitly. The
 * recovered application object order reproduces the target's table sequence.
 */
static void (*Ob_GlobalCtorSlot)(void)
    __attribute__((section(".ctors"))) = Ob_GlobalInit_EnvironmentalObjectList;

/** Empty default used by bodies with no owner-specific teardown work. */
void CBody_DeleteStuff(CBodyOwner *)
{
}

/** Empty default used by bodies with no owner-specific lighting setup. */
void CBody_DoLightingSetup(CBodyOwner *)
{
}

/** Empty per-frame default supplied by the original inline body. */
void CBody_EveryFrame(CBody *)
{
}

/** Empty AI default supplied by the original inline body. */
void CBody_AI(CBodyOwner *)
{
}
