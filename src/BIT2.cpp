// BIT2.cpp -- Reconstructed translation unit
// Original: BIT2.CPP
// Functions: 13 reconstructed entries
// Address range: 0x80036848 - 0x80036FD8
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#define BIT2_OWNER_DECLARATIONS
#include "BIT2.h"
#undef BIT2_OWNER_DECLARATIONS
#include "MEM.h"
#include "UTILS.h"
#include "globals.h"

struct SAnimFrame;

/** Base state shared by the line effects owned by BIT2. */
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
    int *_vf;

    CBit();
    ~CBit();
    void AttachTo(void *list);
    void DeleteFrom(void *list);
    /** Mark this bit for removal. */
    void Die();
    /** Advance this bit for one frame. */
    void Move();
    /** Allocate zero-initialized storage from the transient-bit heap. */
    static void *operator new(unsigned int size);
    /** Releases transient-bit storage through the owning allocator. */
    static void operator delete(void *p);
};

#include "CNONRENDEREDBIT.h"

/** Textured particle state shared with the motion-blur effect. */
struct CFT4Bit : CBit {
    short mTransDecay;
    short mScaleDecay;
    unsigned int mCodeBGR;
    SAnimFrame *mpPSXAnim;
    SAnimFrame *mpPSXFrame;
    unsigned char mBitFlags;
    unsigned char mNumFrames;
    char mFrame;
    unsigned char mFrameFrac;
    short mAnimSpeed;
    short mScale;

    /** Assign the particle's RGB tint. */
    void SetTint(unsigned char r, unsigned char g, unsigned char b);
};

/** Rotation and render-priority state of a flat textured particle. */
struct CFlatBit : CFT4Bit {
    short mAngle;
    short mAngVel;
    short mAngAcc;
    unsigned short mAngFric;
    unsigned int mPostScale;
    unsigned char mForcedPriority;
    unsigned char mSemiTransparencyRate;
    unsigned short mCLUT;
};

#include "CMOTIONBLUR.h"

#include "SLINESEG.h"

#include "CPOLYLINE.h"

#include "CGPOLYLINE.h"

#include "CGLINE.h"

/** Proven prefix of CItem needed to read a body's position. */
struct CItemPositionPrefix {
    unsigned short mFlags;
    unsigned short mInquiry;
    CVector mPos;
};

/** Proven prefix of CBody containing its embedded item state. */
struct CBodyPositionPrefix {
    CItemPositionPrefix CItem;
};

typedef char Bit2_CBit_size_must_be_72[(sizeof(CBit) == 72) ? 1 : -1];
typedef char Bit2_CNonRenderedBit_size_must_be_72[
    (sizeof(CNonRenderedBit) == 72) ? 1 : -1];
typedef char Bit2_CFT4Bit_size_must_be_96[(sizeof(CFT4Bit) == 96) ? 1 : -1];
typedef char Bit2_CFlatBit_size_must_be_112[(sizeof(CFlatBit) == 112) ? 1 : -1];
typedef char Bit2_CMotionBlur_size_must_be_112[
    (sizeof(CMotionBlur) == 112) ? 1 : -1];
typedef char Bit2_SLineSeg_size_must_be_16[(sizeof(SLineSeg) == 16) ? 1 : -1];
typedef char Bit2_CPolyLine_size_must_be_96[(sizeof(CPolyLine) == 96) ? 1 : -1];
typedef char Bit2_CGPolyLine_size_must_be_100[(sizeof(CGPolyLine) == 100) ? 1 : -1];
typedef char Bit2_CGLine_size_must_be_104[(sizeof(CGLine) == 104) ? 1 : -1];
typedef char Bit2_CItem_position_prefix_size_must_be_16[
    (sizeof(CItemPositionPrefix) == 16) ? 1 : -1];
typedef char Bit2_CBody_position_prefix_size_must_be_16[
    (sizeof(CBodyPositionPrefix) == 16) ? 1 : -1];

/** Absolute-addressed list heads and the shared zero vector used by BIT2. */
#define BITSTATE_DECLARE_POLY_LINE_LIST
#include "BITSTATE.h"
#define BITSTATE_DECLARE_G_LINE_LIST
#include "BITSTATE.h"
#define BITSTATE_DECLARE_G_POLY_LINE_LIST
#include "BITSTATE.h"
#define ZEROVECTOR_SCALAR_DECLARATION
#include "ZEROVECTOR.h"
#undef ZEROVECTOR_SCALAR_DECLARATION

/** Typed dependencies owned by BIT and VECTOR. */
#define BIT_DECLARE_MOTION_BLUR_CONSTRUCTOR
#include "BIT.h"
#define Bit2_ConstructMotionBlur \
    BitMotionBlurConstructorABI::__11CMotionBlurRC7CVectorR7CVectorPciii
#include "CVECTOROPS.h"

/**
 * A short-lived generated line attached to a moving body while grinding.
 *
 * The owner-local base layout supplies the recovered field types without
 * importing unrelated generated aggregate declarations.
 */
struct CGrindSpark : CGLine {
    CVector mOldPos;
    CBody *mpParent;

    CGrindSpark(CBody *pParent, CVector &pos, int vel, int gravity, int life);
    ~CGrindSpark();
    /** Releases bit-owned storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
    void Move();
};


/** Emits a configurable group of short-lived motion-blur particles. */
struct CSmokeGenerator : CNonRenderedBit {
    int mPuffs;
    unsigned char mR;
    unsigned char mG;
    unsigned char mB;
    int mVBase;
    int mVRandom;
    int mScaleBase;
    int mScaleRandom;

    ~CSmokeGenerator();
    /** Releases bit-owned storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
    void Move();
};

typedef char Bit2_CGrindSpark_class_size_must_be_120[(sizeof(CGrindSpark) == 120) ? 1 : -1];
typedef char Bit2_CSmokeGenerator_size_must_be_96[(sizeof(CSmokeGenerator) == 96) ? 1 : -1];
// === Extern vtable symbols ===

/** Returns the full vector stored at the linker-visible `ZeroVector` base. */
static inline const CVector &Bit2_ZeroVector()
{
    return *reinterpret_cast<const CVector *>(&ZeroVector);
}

/** Returns the proven CItem-position prefix of a body. */
static inline const CVector &Bit2_BodyPosition(const CBody *body)
{
    return reinterpret_cast<const CBodyPositionPrefix *>(body)->CItem.mPos;
}
#define BIT2_DECLARE_MOTION_BLUR_ANIMATION_VIEW
#include "BIT2.h"

/** Animation name used by the motion-blur bit effect. */
namespace Bit2AddressViewOwners
{
extern "C"
{
static char DAT_800d98b8[8]
    __attribute__((section(".sdata"))) = "Smoke";
}
}

/*
 * This owner's five CBit subclass vtables, which directly follow the animation
 * name in the original small-data contribution.  C++ forbids taking a
 * destructor's address, so each dispatch slot goes through an addressable
 * declaration of the symbol this TU already defines; `Move__4CBit` is BIT's.
 *
 * Each table is four consecutive eight-byte-aligned small-data words rather
 * than an array, so -G8 keeps emitting the target's `addiu $vX, $gp, off`
 * instead of an absolute lui/addiu pair.
 */
/** Addressable destructor entries used by this owner's five manual vtables. */
void Bit2_GrindSparkDtorAbi(CGrindSpark *, int) asm("_._11CGrindSpark");
void Bit2_GLineDtorAbi(CGLine *, int) asm("_._6CGLine");
void Bit2_SmokeGeneratorDtorAbi(CSmokeGenerator *, int)
    asm("_._15CSmokeGenerator");
void Bit2_GPolyLineDtorAbi(CGPolyLine *, int) asm("_._10CGPolyLine");
void Bit2_PolyLineDtorAbi(CPolyLine *, int) asm("_._9CPolyLine");

int __vt_11CGrindSpark
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int Bit2_GrindSparkVTableIndex = 0;
static void (*Bit2_GrindSparkVTableDtor)(CGrindSpark *, int) =
    &Bit2_GrindSparkDtorAbi;
static void (*Bit2_GrindSparkVTableMove)(CGrindSpark *) =
    (void (*)(CGrindSpark *))&CGrindSpark::Move;

int __vt_6CGLine
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int Bit2_GLineVTableIndex = 0;
static void (*Bit2_GLineVTableDtor)(CGLine *, int) = &Bit2_GLineDtorAbi;
static void (*Bit2_GLineVTableMove)(CBit *) =
    (void (*)(CBit *))&CBit::Move;

int __vt_15CSmokeGenerator
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int Bit2_SmokeVTableIndex = 0;
static void (*Bit2_SmokeVTableDtor)(CSmokeGenerator *, int) =
    &Bit2_SmokeGeneratorDtorAbi;
static void (*Bit2_SmokeVTableMove)(CSmokeGenerator *) =
    (void (*)(CSmokeGenerator *))&CSmokeGenerator::Move;

int __vt_10CGPolyLine
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int Bit2_GPolyLineVTableIndex = 0;
static void (*Bit2_GPolyLineVTableDtor)(CGPolyLine *, int) =
    &Bit2_GPolyLineDtorAbi;
static void (*Bit2_GPolyLineVTableMove)(CBit *) =
    (void (*)(CBit *))&CBit::Move;

int __vt_9CPolyLine
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int Bit2_PolyLineVTableIndex = 0;
static void (*Bit2_PolyLineVTableDtor)(CPolyLine *, int) =
    &Bit2_PolyLineDtorAbi;
static void (*Bit2_PolyLineVTableMove)(CBit *) =
    (void (*)(CBit *))&CBit::Move;

/** Sets the semitransparency bit on one packed line-segment command. */
static inline void Bit2_SetSegmentSemiTransparent(
    int index, SLineSeg *segments)
{
    segments[index].code |= 2;
}

// === Function implementations (address order) ===

/**
 *
 * Address:   0x80036848
 * Mangled:   __9CPolyLinei
 * Demangled: CPolyLine::CPolyLine(int)
 *            Purpose: allocate the segment buffer, initialize segments, and attach to the polyline list.
 *
 * @param numsegs  Number of line segments to allocate and initialize.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
CPolyLine::CPolyLine(int numsegs) {
    int blockSize;
    int i;
    _vf = &__vt_9CPolyLine;
    mSegs = static_cast<SLineSeg *>(Mem_New(numsegs << 4, 0, 1));
    blockSize = Mem_GetBlockSize(mSegs);
    TotalBitUsage += 4 + blockSize;
    for (i = 0; i < numsegs; i++) {
        *(unsigned int *)&mSegs[i].r = 0x40FFFFFFu;
        mSegs[i].End = Bit2_ZeroVector();
    }
    mNumSegs = numsegs;
    AttachTo(&PolyLineList);
}


/**
 *
 * Address:   0x80036928
 * Mangled:   _._9CPolyLine
 * Demangled: CPolyLine::~CPolyLine()
 *            Purpose: release the segment buffer, detach from the polyline list, and chain to the base destructor.
 *
 */
CPolyLine::~CPolyLine() {
    SLineSeg *segments;
    CPolyLine **listHead;
    int blockSize;
    int remainingUsage;
    SLineSeg *segmentsToFree;
    _vf = &__vt_9CPolyLine;
    segments = mSegs;
    blockSize = Mem_GetBlockSize(segments);
    remainingUsage = TotalBitUsage;
    segmentsToFree = mSegs;
    remainingUsage = remainingUsage - 4;
    remainingUsage = remainingUsage - blockSize;
    TotalBitUsage = remainingUsage;
    Mem_Delete(segmentsToFree);
    listHead = &PolyLineList;
    DeleteFrom(listHead);
}


/**
 * CPolyLine::SetSemiTransparent()
 *
 * Address:   0x800369B4
 * Mangled:   SetSemiTransparent__9CPolyLine
 * Demangled: CPolyLine::SetSemiTransparent(void)
 *            Marks every packed segment command as semitransparent.
 *
 */
void CPolyLine::SetSemiTransparent() {
    for (int i = 0; i < mNumSegs; i++) {
        Bit2_SetSegmentSemiTransparent(i, mSegs);
    }
}


/**
 *
 * Address:   0x80036A00
 * Mangled:   __10CGPolyLinei
 * Demangled: CGPolyLine::CGPolyLine(int)
 *            Purpose: allocate the segment buffer, initialize segments, and attach to the generated polyline list.
 *
 * @param numsegs  Number of generated line segments to allocate and initialize.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
CGPolyLine::CGPolyLine(int numsegs) {
    int blockSize;
    int i;
    _vf = &__vt_10CGPolyLine;
    mSegs = static_cast<SLineSeg *>(Mem_New(numsegs << 4, 0, 1));
    blockSize = Mem_GetBlockSize(mSegs);
    TotalBitUsage += 4 + blockSize;
    for (i = 0; i < numsegs; i++) {
        *(unsigned int *)&mSegs[i].r = 0x50FFFFFFu;
        mSegs[i].End = Bit2_ZeroVector();
    }
    mNumSegs = numsegs;
    AttachTo(&GPolyLineList);
    mStartR = 0xff;
    mStartG = 0xff;
    mStartB = 0xff;
    mCode = 0x50;
}


/**
 *
 * Address:   0x80036AF8
 * Mangled:   _._10CGPolyLine
 * Demangled: CGPolyLine::~CGPolyLine()
 *            Purpose: release the segment buffer, detach from the generated polyline list, and chain to the base destructor.
 *
 */
CGPolyLine::~CGPolyLine() {
    SLineSeg *segments;
    CGPolyLine **listHead;
    int blockSize;
    int remainingUsage;
    SLineSeg *segmentsToFree;
    _vf = &__vt_10CGPolyLine;
    segments = mSegs;
    blockSize = Mem_GetBlockSize(segments);
    remainingUsage = TotalBitUsage;
    segmentsToFree = mSegs;
    remainingUsage = remainingUsage - 4;
    remainingUsage = remainingUsage - blockSize;
    TotalBitUsage = remainingUsage;
    Mem_Delete(segmentsToFree);
    listHead = &GPolyLineList;
    DeleteFrom(listHead);
}


/**
 * CGPolyLine::SetSemiTransparent()
 *
 * Address:   0x80036B84
 * Mangled:   SetSemiTransparent__10CGPolyLine
 * Demangled: CGPolyLine::SetSemiTransparent(void)
 *            Marks every segment and the line's own draw command as
 *            semitransparent.
 *
 */
void CGPolyLine::SetSemiTransparent() {
    for (int i = 0; i < mNumSegs; i++) {
        Bit2_SetSegmentSemiTransparent(i, mSegs);
    }
    mCode = mCode | 2;
}


/**
 * CSmokeGenerator::~CSmokeGenerator()
 *
 * Address:   0x80036BDC
 * Mangled:   _._15CSmokeGenerator
 * Demangled: CSmokeGenerator::~CSmokeGenerator()
 *
 */
CSmokeGenerator::~CSmokeGenerator()
{
    _vf = &__vt_15CSmokeGenerator;
}


/**
 *
 * Address:   0x80036C2C
 * Mangled:   Move__15CSmokeGenerator
 * Demangled: CSmokeGenerator::Move(void)
 *            Purpose: spawn and tint smoke motion-blur particles, age the generator, and kill it when its lifetime expires.
 *
 */
void CSmokeGenerator::Move()
{
    int i = 0;
    CVector upvel;
    upvel.vx = 0;
    int count = mPuffs;
    CVector *upvelPtr = &upvel;
    upvelPtr->vy = 0;
    upvelPtr->vz = 0;
    if (count > 0) {
        int defaultAge = 100;
        int loopCount;
        do {
            int r = Rnd(mVRandom);
            int yoff = mVBase + r;
            yoff = -yoff;
            upvel.vy = yoff << 12;
            CMotionBlur *foo = static_cast<CMotionBlur *>(
                CBit::operator new(sizeof(CMotionBlur)));
            if (foo != 0) {
                int r2 = Rnd(mScaleRandom);
                int animLen = mScaleBase + r2;
                foo = Bit2_ConstructMotionBlur(
                    foo, &mPos, &upvel,
                    DAT_800d98b8, animLen, 0, 10);
            }
            foo->mFrigDeltaZ = defaultAge;
            int randFrame = Rnd(0x1000);
            foo->mAngle = static_cast<short>(randFrame);
            unsigned int cr = mR;
            unsigned int cg = mG;
            unsigned int cb = mB;
            foo->SetTint(cr, cg, cb);
            loopCount = mPuffs;
            i++;
        } while (i < loopCount);
    }
    int age = static_cast<unsigned short>(mAge);
    int maxAge = mLifetime;
    age = age + 1;
    mAge = static_cast<short>(age);
    short sAge = static_cast<short>(age);
    if (sAge == maxAge) {
        if (static_cast<unsigned short>(sAge) != 0xffff) {
            Die();
        }
    }
}


/**
 * CGLine::CGLine()
 *
 * Address:   0x80036D6C
 * Mangled:   __6CGLine
 * Demangled: CGLine::CGLine(void)
 *            Constructor: calls CBit parent, sets colors, vtable, attaches to GLineList (absolute address)
 *
 */
CGLine::CGLine() {
    mCodeBGR0 = 0x50808080;
    {
        register u32 color2 asm("$3") = 0x55808080;
        int *vtable = &__vt_6CGLine;
        _vf = vtable;
        mPadBGR1 = color2;
        AttachTo(&GLineList);
    }
}


/**
 * CGLine::~CGLine()
 *
 * Address:   0x80036DC4
 * Mangled:   _._6CGLine
 * Demangled: CGLine::~CGLine()
 *            Destructor: sets vtable, calls DeleteFrom, calls parent ~CBit(flag=0), conditionally deletes
 *
 */
CGLine::~CGLine() {
    _vf = &__vt_6CGLine;
    DeleteFrom(&GLineList);
}


/**
 *
 * Address:   0x80036E24
 * Mangled:   __11CGrindSparkP5CBodyR7CVectoriii
 * Demangled: CGrindSpark::CGrindSpark(CBody *, CVector &, int, int, int)
 *            Builds a grind spark line in body-relative coordinates.
 *
 * @param pParent  Body whose position offsets the generated line.
 * @param pos  Spark position relative to the parent body.
 * @param vel  Signed random velocity range.
 * @param gravity  Vertical acceleration applied to the spark.
 * @param life  Exclusive upper bound used to randomize the lifetime.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
CGrindSpark::CGrindSpark(CBody *pParent, CVector &pos, int vel, int gravity, int life)
{
    _vf = &__vt_11CGrindSpark;

    int vel2;
    if (vel < 0) {
        int magnitude = -vel;
        vel2 = -((magnitude >> 1) + Rnd(magnitude));
    } else {
        vel2 = (vel >> 1) + Rnd(vel);
    }
    Bit_CalculateSparkVelocity(mVel, vel2);

    mAcc.vy = gravity;
    mFric.Set(3, 3, 3);
    mLifetime = static_cast<unsigned short>(Rnd(life) + 1);
    mpParent = pParent;
    mOldPos = pos;
    mPos = pos;

    CVector end = pos + Bit2_BodyPosition(mpParent);
    mEnd = end;
    mStart = mEnd;
    mCodeBGR0 = 0x52000080;
    int color = Rnd(0x9f);
    mPadBGR1 = 0x55000000;
    mCodeBGR0 |= ((color >> 5) + 2) << 12;
}


/**
 * CGrindSpark::~CGrindSpark()
 *
 * Address:   0x80036F88
 * Mangled:   _._11CGrindSpark
 * Demangled: CGrindSpark::~CGrindSpark()
 *
 */
CGrindSpark::~CGrindSpark()
{
    _vf = &__vt_11CGrindSpark;
}


/**
 *
 * Address:   0x80036FD8
 * Mangled:   Move__11CGrindSpark
 * Demangled: CGrindSpark::Move(void)
 *
 */
void CGrindSpark::Move()
{
    CGrindSpark *spark = this;
    register int px asm("$7") = spark->mPos.vx;
    register int py asm("$8") = spark->mPos.vy;
    register int velX asm("$4") = spark->mVel.vx;
    register int velY asm("$10") = spark->mVel.vy;
    register int pz asm("$6") = spark->mPos.vz;
    const CBodyPositionPrefix *parent =
        reinterpret_cast<const CBodyPositionPrefix *>(spark->mpParent);
    register int velZ asm("$9") = spark->mVel.vz;
    unsigned short age = static_cast<unsigned short>(spark->mAge);
    int accY = spark->mAcc.vy;
    asm volatile("" : "+r"(px), "+r"(py), "+r"(velX), "+r"(velY),
                      "+r"(pz));
    register int nx asm("$17") = px + velX;
    register int ny asm("$18") = py + velY;
    register int nz asm("$25") = pz + velZ;
    asm volatile("" : "+r"(nx), "+r"(ny), "+r"(nz));
    int sx = parent->CItem.mPos.vx;
    int sy = parent->CItem.mPos.vy;
    int sz = parent->CItem.mPos.vz;
    int accX = spark->mAcc.vx;
    int newAge = static_cast<int>(age) + 1;
    spark->mAge = static_cast<short>(newAge);
    int ageShifted = newAge << 16;
    spark->mOldPos.vx = px;
    spark->mOldPos.vy = py;
    spark->mOldPos.vz = pz;
    spark->mPos.vx = nx;
    spark->mPos.vy = ny;
    spark->mPos.vz = nz;
    register int startX asm("$16") = nx + sx;
    register int newVelX asm("$17") = velX + accX;
    register int startY asm("$12") = ny + sy;
    register int newVelY asm("$18") = velY + accY;
    register int startZ asm("$11") = nz + sz;
    asm volatile("" : "+r"(startX), "+r"(newVelX), "+r"(startY),
                      "+r"(newVelY), "+r"(startZ));
    newVelX = newVelX - (newVelX >> 3);
    newVelY = newVelY - (newVelY >> 3);
    int endX = px + sx;
    int endY = py + sy;
    register int accZ asm("$4") = spark->mAcc.vz;
    int endZ = pz + sz;
    spark->mEnd.vx = endX;
    spark->mEnd.vy = endY;
    spark->mEnd.vz = endZ;
    spark->mStart.vx = startX;
    spark->mStart.vy = startY;
    spark->mStart.vz = startZ;
    spark->mVel.vx = newVelX;
    spark->mVel.vy = newVelY;
    register int newVelZ asm("$25") = velZ + accZ;
    newVelZ = newVelZ - (newVelZ >> 3);
    spark->mVel.vz = newVelZ;
    asm volatile("" : "+r"(newVelZ));
    unsigned short maxAge = spark->mLifetime;
    int signedAge = ageShifted >> 16;
    if (signedAge >= static_cast<int>(maxAge)) {
        spark->Die();
    }
}
