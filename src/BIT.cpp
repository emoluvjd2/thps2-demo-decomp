// BIT.cpp -- Reconstructed translation unit
// Original: BIT.CPP
// Functions: 108 reconstructed entries
// Address range: 0x800370EC - 0x8003E690
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#define __SKIP_CBIT_EXTERNS__

#define __SKIP_CQUADBIT_EXTERNS__

#define __SKIP_CRIBBON_EXTERNS__

#define __SKIP_CCHUNKBIT_EXTERNS__

#define BIT_GLOW_ELEMENT_LAYOUT
#define BIT_OWNER_DECLARATIONS
#include "BIT.h"
#undef BIT_OWNER_DECLARATIONS
#undef BIT_GLOW_ELEMENT_LAYOUT
#include "MAIN.h"
#include "DB.h"
#include "globals.h"
#include "UTILS.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include <INLINE_C.H>
#include "gte_extensions.h"

/** Return caller-provided storage for an in-place construction. */
inline void *operator new(unsigned int, void *storage)
{
    return storage;
}

struct tag_S_Pal;
struct CBit;

/** Virtual slots shared by every transient bit-effect subtype. */
struct CBitVTable {
    void *mReserved[2];
    void (*Destroy)(CBit *, int);
    void (*Move)(CBit *);
    void (*Display)(CBit *);
};

#include "TEXTURE.h"

#include "SANIMFRAME.h"

/** Base state shared by every transient bit effect. */
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
    void **_vf;

    /** Construct the base transient-bit state. */
    CBit();
    /** Allocate zero-initialized storage from the transient-bit heap. */
    static void *operator new(unsigned int size);
    ~CBit();
    /** Insert this bit at the head of a transient-bit list. */
    void AttachTo(void *list);
    /** Remove this bit from a transient-bit list. */
    void DeleteFrom(void *list);
    /** Mark this bit for removal. */
    void Die();
    /** Assign the bit's world-space position. */
    void SetPos(const CVector &position);
    /** Default movement hook for transient bits. */
    void Move();
    /** Releases transient-bit storage through the owning allocator. */
    static void operator delete(void *p);
};

#include "CSPECIALDISPLAY.h"

#include "CNONRENDEREDBIT.h"

/** Textured four-point particle state. */
struct CFT4Bit : CBit {
    short mTransDecay;
    short mScaleDecay;
    unsigned int mCodeBGR;
    struct SAnimFrame *mpPSXAnim;
    struct SAnimFrame *mpPSXFrame;
    unsigned char mBitFlags;
    unsigned char mNumFrames;
    char mFrame;
    unsigned char mFrameFrac;
    short mAnimSpeed;
    short mScale;

    /** Construct the shared textured-bit state. */
    CFT4Bit();
    ~CFT4Bit();
    /** Assign the particle's fixed-point display scale. */
    void SetScale(unsigned short scale);
    /** Enable semi-transparent primitive rendering. */
    void SetSemiTransparent();
    /** Assign the particle's RGB tint. */
    void SetTint(unsigned char r, unsigned char g, unsigned char b);
    /** Assign the per-frame transparency decay. */
    void SetTransDecay(int decay);
    /** Select an animation by name. */
    void SetAnim(char *name);
    /** Select one animation frame and reset its fractional step. */
    void SetFrame(int frame);
    /** Advance one animation frame without wrapping. */
    void IncFrame();
    /** Advance one animation frame and wrap at the animation end. */
    void IncFrameWithWrap();
    /** Select a random frame from the current animation. */
    void ChooseRandomFrame();
    /** Reduce the display scale and optionally retire the particle. */
    int Shrink(int die);
    /** Fade the particle tint and optionally retire the particle. */
    int Fade(int die);
    /** Releases textured-bit storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
};

/** Four-corner world-space particle. */
struct CQuadBit : CBit {
    struct CVector mPosB;
    struct CVector mPosC;
    struct CVector mPosD;
    struct CVector mTweenPosB;
    struct CVector mTweenPosC;
    struct CVector mTweenPosD;
    unsigned int mCodeBGR;
    struct Texture *mpTexture;
    unsigned char mFlags;

    ~CQuadBit();
    /** Select the texture frame used by this world-space quad. */
    void SetTexture(char *animName, int frame);
    /** Enable one or more quad-rendering flags. */
    void FlagOn(unsigned char flag);
    /** Enable semi-transparent primitive rendering. */
    void SetSemiTransparent();
    /** Assign a grayscale transparency value to the quad. */
    void SetTransparency(unsigned char transparency);
    /** Releases quad storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
    void OrientUsing(const struct CVector &position,
                     const SVECTOR &normal,
                     int dx, int dy, int angle);
};

struct CFlatBit : CFT4Bit {
    short mAngle;
    short mAngVel;
    short mAngAcc;
    unsigned short mAngFric;
    unsigned int mPostScale;
    unsigned char mForcedPriority;
    unsigned char mSemiTransparencyRate;
    unsigned short mCLUT;

    /** Construct and attach a flat textured particle. */
    CFlatBit();
    ~CFlatBit();
    /** Releases flat-bit storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
};

struct CLinked2EndedBit : CFT4Bit {
    struct CVector mEndPosA;
    struct CVector mEndPosB;

    /** Construct and attach a linked two-ended transient bit. */
    CLinked2EndedBit();
    ~CLinked2EndedBit();
    /** Releases linked-bit storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
};

struct CPixel : CBit {
    unsigned int tag;
    unsigned char r0;
    unsigned char g0;
    unsigned char b0;
    unsigned char code;
    unsigned int mWidthHeight;

    CPixel();
    ~CPixel();
    /** Releases pixel storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
};

struct CSpark : CPixel {
    unsigned char mFadeR;
    unsigned char mFadeG;
    unsigned char mFadeB;
    int mFloorY;

    CSpark(struct CVector &position, int velocity, int gravity, int life);
    ~CSpark();
    /** Advance the spark and fade its colour channels. */
    void Move();
    /** Releases spark storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
};

struct CSimpleAnim : CFlatBit {
    int mDie;
    int mDieFrame;

    ~CSimpleAnim();
    /** Advance the animation and retire it at the configured frame. */
    void Move();
    /** Releases animated-bit storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
};

#include "CMOTIONBLUR.h"

struct CMotionBlurImmortal : CMotionBlur {
    ~CMotionBlurImmortal();
    /** Advance the immortal blur without retiring it on shrink or fade. */
    void Move();
    /** Releases immortal motion-blur storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
};

/** One animated segment owned by a ribbon effect. */
struct CRibbonBit : CLinked2EndedBit {
    CRibbonBit();
    ~CRibbonBit();
    /** Advance the ribbon segment's animation frame. */
    void Move();
    /** Releases ribbon-segment storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
};

#include "SRIBBONPOINT.h"

#include "CRIBBON.h"

#include "CGLOW.h"

#include "CWOBBLYGLOW.h"

#include "SLINESEG.h"

#include "CPOLYLINE.h"

#include "CGPOLYLINE.h"

#include "CGLINE.h"

/**
 * Scratchpad layout used by M3dAsm_ClipLine.
 *
 * The assembly consumes two input vectors at offsets 0x00 and 0x0c, exposes
 * the two screen words used by BIT at 0x18 and 0x1c, and uses the remaining
 * words through offset 0x5c as private clipping workspace.
 */
struct SClipLineBuf {
    struct CVector Input[2];
    unsigned int Output[2];
    unsigned int Workspace[16];
};

#include "CCHUNKBIT.h"

#include "CSMOKETRAIL.h"

struct CGenPoly : CBit {
    unsigned short mGouraud;
    unsigned short mPad;
    SVECTOR mPosA;
    SVECTOR mPosB;
    SVECTOR mPosC;
    SVECTOR mPosD;
    struct CVector mWorldPosA;
    struct CVector mWorldPosB;
    struct CVector mWorldPosC;
    struct CVector mWorldPosD;
    struct CSVector mAngles;
    unsigned int mClutV0U0;
    unsigned int mTPageV1U1;
    unsigned short mV2U2;
    unsigned short mV3U3;
    unsigned int mB0G0R0;
    unsigned int mPadB1G1R1;
    unsigned int mPadB2G2R2;
    unsigned int mPadB3G3R3;
    unsigned int mTexWin;

    ~CGenPoly();
    /** Releases generated-polygon storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
};

struct CFrag : CFlatBit {
    ~CFrag();
    /** Advance fragment physics and retire it at the end of its lifetime. */
    void Move();
    /** Releases fragment storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
};

struct CGouraudRibbon : CSpecialDisplay {
    int mTrail;
    int mNumPoints;
    struct SRibbonPoint *mpPoints;

    ~CGouraudRibbon();
    /** Releases Gouraud-ribbon storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
};

struct CGlassBit : CBit {
    struct CVector mPosA;
    struct CVector mPosB;
    struct CVector mPosC;
    int mGroundY;
    unsigned char mDefaultR;
    unsigned char mDefaultG;
    unsigned char mDefaultB;
    unsigned char mR;
    unsigned char mG;
    unsigned char mB;
    unsigned char mFadeRate;

    ~CGlassBit();
    /** Advance fragment physics and its bounce/fade lifecycle. */
    void Move();
    /** Releases glass-fragment storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
};

#include "CTEXTUREDRIBBON.h"

/** Scratchpad projection record used by both ribbon display routines. */
struct SCalcBuffer {
    union {
        int Pos;
        struct {
            short X;
            short Y;
        } Screen;
    };
    int NormalX;
    int NormalY;
    long Zed;
    int ScrWidth;
    int ScrWidthB;
    int Bad;
};

typedef char Bit_Texture_size_must_be_40[(sizeof(Texture) == 40) ? 1 : -1];
typedef char Bit_CBit_size_must_be_72[(sizeof(CBit) == 72) ? 1 : -1];
typedef char Bit_CFT4Bit_size_must_be_96[(sizeof(CFT4Bit) == 96) ? 1 : -1];
typedef char Bit_CQuadBit_size_must_be_156[(sizeof(CQuadBit) == 156) ? 1 : -1];
typedef char Bit_CFlatBit_size_must_be_112[(sizeof(CFlatBit) == 112) ? 1 : -1];
typedef char Bit_CLinked2EndedBit_size_must_be_120[(sizeof(CLinked2EndedBit) == 120) ? 1 : -1];
typedef char Bit_CPixel_size_must_be_84[(sizeof(CPixel) == 84) ? 1 : -1];
typedef char Bit_CSpark_size_must_be_92[(sizeof(CSpark) == 92) ? 1 : -1];
typedef char Bit_CSimpleAnim_size_must_be_120[(sizeof(CSimpleAnim) == 120) ? 1 : -1];
typedef char Bit_CRibbon_size_must_be_96[(sizeof(CRibbon) == 96) ? 1 : -1];
typedef char Bit_SRibbonPoint_size_must_be_36[(sizeof(SRibbonPoint) == 36) ? 1 : -1];
typedef char Bit_CGlow_size_must_be_104[(sizeof(CGlow) == 104) ? 1 : -1];
typedef char Bit_CWobblyGlow_size_must_be_176[(sizeof(CWobblyGlow) == 176) ? 1 : -1];
typedef char Bit_SLineSeg_size_must_be_16[(sizeof(SLineSeg) == 16) ? 1 : -1];
typedef char Bit_CPolyLine_size_must_be_96[(sizeof(CPolyLine) == 96) ? 1 : -1];
typedef char Bit_CGPolyLine_size_must_be_100[(sizeof(CGPolyLine) == 100) ? 1 : -1];
typedef char Bit_CGLine_size_must_be_104[(sizeof(CGLine) == 104) ? 1 : -1];
typedef char Bit_SClipLineBuf_size_must_be_0x60[
    (sizeof(SClipLineBuf) == 0x60) ? 1 : -1];
typedef char Bit_CChunkBit_size_must_be_192[(sizeof(CChunkBit) == 192) ? 1 : -1];
typedef char Bit_CSmokeTrail_size_must_be_100[(sizeof(CSmokeTrail) == 100) ? 1 : -1];
typedef char Bit_CGenPoly_size_must_be_196[(sizeof(CGenPoly) == 196) ? 1 : -1];
typedef char Bit_CGouraudRibbon_size_must_be_84[(sizeof(CGouraudRibbon) == 84) ? 1 : -1];
typedef char Bit_CGlassBit_size_must_be_120[(sizeof(CGlassBit) == 120) ? 1 : -1];
typedef char Bit_CTexturedRibbon_size_must_be_108[(sizeof(CTexturedRibbon) == 108) ? 1 : -1];
typedef char Bit_SCalcBuffer_size_must_be_28[(sizeof(SCalcBuffer) == 28) ? 1 : -1];

#include "MEM.h"
#include <LIBGPU.H>

/** Enable the transient bit-effect cache by default. */
int UseCacheForBits __attribute__((section(".sdata"))) = 1;

// === Extern "C" global variables ===
/*
 * Keep the constructor-facing declaration opaque while the owner-local
 * definition below supplies the same assembler label without an XDEF.
 */
extern "C" char Bit_RibbonName[] asm("DAT_800d9970");

// Absolute-addressed globals (force lui/addiu, not gp-relative)
#define RCOSSIN_DECL_TYPE int
#define RCOSSIN_SCALAR_VIEW
#include "RCOSSIN.h"

/** Packed fixed-point sine/cosine entry used by the original table macros. */
struct SSinCos
{
    short Sin;
    short Cos;
};
#define Sine(angle) \
    ((*(SSinCos *)(&rcossin_tbl + ((angle) & 0xfff))).Sin)
#define Cosine(angle) \
    ((*(SSinCos *)(&rcossin_tbl + ((angle) & 0xfff))).Cos)

/** Return a word-addressed pointer into the PlayStation scratchpad. */
#ifndef getScratchAddr
#define getScratchAddr(offset) ((u32 *)(0x1f800000 + (offset) * 4))
#endif

// === Extern vtable symbols ===
extern "C" int __vt_4CBit;
extern "C" int __vt_15CNonRenderedBit;
extern "C" int __vt_7CFT4Bit;
extern "C" int __vt_16CLinked2EndedBit;
extern "C" int __vt_6CPixel;
extern "C" int __vt_10CRibbonBit;
extern "C" int __vt_8CQuadBit;
extern "C" int __vt_8CFlatBit;
extern "C" int __vt_11CMotionBlur;
extern "C" int __vt_19CMotionBlurImmortal;
extern "C" int __vt_5CFrag;
extern "C" int __vt_6CSpark;
extern "C" int __vt_11CSimpleAnim;
extern "C" int __vt_11CSmokeTrail;
extern "C" int __vt_8CGenPoly;
extern "C" int __vt_9CChunkBit;
extern "C" int __vt_9CGlassBit;
extern "C" int __vt_5CGlow;
extern "C" int __vt_7CRibbon;
extern "C" int __vt_11CWobblyGlow;

// BIT init tables and their small-data animation-name strings.
static char *AnimNames[6] = {
    "SHADOW",
    "SMOKE",
    "orbit",
    "ribbon",
    "smtile",
    "Buttons",
};
#define QUICKANIMLOOKUP_OWNER_DECLARATION
#include "QUICKANIMLOOKUP.h"
SAnimFrame *QuickAnimLookup[6] = {
    0, 0, 0, 0, 0, 0,
};

/**
 * Spark defaults and the ribbon texture name owned by BIT.
 *
 * The two six-byte vectors are naturally padded to four-byte boundaries by
 * the original compiler, which preserves the target offsets of the scalar and
 * colour defaults that follow them.
 */
Texture *pShadowTexture __attribute__((section(".sdata"))) = 0;
CSVector SparkTrajectory __attribute__((section(".sdata"))) = {0, 0, 0};
CSVector SparkTrajectoryCone __attribute__((section(".sdata"))) = {0, 0, 0};
unsigned int SparkSize __attribute__((section(".sdata"))) = 1;
void *SparkSemiTrans __attribute__((section(".sdata"))) = (void *)1;
CVECTOR SparkRGB __attribute__((section(".sdata"))) = {0x80, 0x80, 0x80, 0};
CVECTOR SparkFadeRGB __attribute__((section(".sdata"))) = {4, 4, 4, 8};
static char Bit_RibbonNameOwner[8]
    asm("DAT_800d9970")
    __attribute__((section(".sdata"))) = "Ribbon";

#include "SCAMERA.h"

#include "SVIEWPORT.h"

/** Proven display-buffer slice containing the viewport ordering table. */
struct SDoubleBuffer
{
    char beforeViewportOT[0x8c];
    u32 *ViewportOT;
};

// === Shared and external function declarations ===
#include "SPOOL.h"
#define M3DMATHS_TYPED_API
#include "M3DMATHS.h"
#undef M3DMATHS_TYPED_API
#include "M3DASM.h"

// === Free function declarations ===
long Transform(const CVector &Pos, int &yx);
void Bit_Init(void);
void DeleteBitList(CBit *pBitList);
static void RemoveDeadBits(CBit *pBit);
void Bit_RemoveDeadBits(void);
void MoveList(CBit *pBit);
void Bit_Move(void);
void Bit_Display(void);
static void CalcScreenNormal(SCalcBuffer *pBuf, int &UnitX, int &UnitY, int ClosenessLimit);
CVector operator+(const CVector &left, const CVector &right);
CVector operator-(const CVector &left, const CVector &right);
CVector operator*(const CVector &vector, const int &scale);
CVector operator/(const CVector &vector, const int &scale);
CVector operator>>(const CVector &vector, const int &shift);

// === Extern "C" forward declarations ===
extern "C" CBit *CBit_ctor(CBit *) asm("__4CBit");
extern "C" void CSpecialDisplay_dtor(CSpecialDisplay *, int)
    asm("_._15CSpecialDisplay");
extern "C" CFT4Bit *CFT4Bit_ctor(CFT4Bit *) asm("__7CFT4Bit");
extern "C" CQuadBit *CQuadBit_ctor(CQuadBit *) asm("__8CQuadBit");
extern "C" CLinked2EndedBit *CLinked2EndedBit_ctor(CLinked2EndedBit *)
    asm("__16CLinked2EndedBit");
extern "C" CMotionBlur *CMotionBlur_ctor(
    CMotionBlur *, CVector *, CVector *, char *, int, int, int)
    asm("__11CMotionBlurRC7CVectorR7CVectorPciii");
extern "C" CRibbonBit *CRibbonBit_ctor(CRibbonBit *) asm("__10CRibbonBit");
extern "C" void CGouraudRibbon_dtor(CGouraudRibbon *, int)
    asm("_._14CGouraudRibbon");
extern "C" void CGouraudRibbon_Display(CGouraudRibbon *)
    asm("Display__14CGouraudRibbon");
extern "C" void CTexturedRibbon_dtor(CTexturedRibbon *, int)
    asm("_._15CTexturedRibbon");
extern "C" void CTexturedRibbon_Display(CTexturedRibbon *)
    asm("Display__15CTexturedRibbon");
extern "C" void CSpecialDisplay_Display(CSpecialDisplay *)
    asm("Display__15CSpecialDisplay");
extern "C" void CBit_Move(CBit *) asm("Move__4CBit");

/**
 * Ribbon and special-display dispatch tables in the BIT owner.
 *
 * GCC 2.x aligns each five-word table to eight bytes, leaving one zero word
 * between adjacent tables. The last table ends without trailing owner data.
 */
int __vt_15CTexturedRibbon[5]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&CTexturedRibbon_dtor,
        (int)&CBit_Move,
        (int)&CTexturedRibbon_Display,
};

int __vt_14CGouraudRibbon[5]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&CGouraudRibbon_dtor,
        (int)&CBit_Move,
        (int)&CGouraudRibbon_Display,
};

int __vt_15CSpecialDisplay[5]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&CSpecialDisplay_dtor,
        (int)&CBit_Move,
        (int)&CSpecialDisplay_Display,
};

// === Function implementations (address order) ===

/**
 *
 * Address:   0x800370EC
 * Mangled:   Transform__FRC7CVectorRi
 * Demangled: Transform(const CVector &, int &)
 *
 * @param Pos  Position vector.
 * @param yx  Receives the packed projected screen coordinates.
 * @return depth
 */
long Transform(const CVector &Pos, int &yx)
{
    CVector Temp;
    Temp.vx = (Pos.vx >> 12) - MikeCamera[Camera_CurrentMikeCamera].Position.vx;
    Temp.vy = (Pos.vy >> 12) - MikeCamera[Camera_CurrentMikeCamera].Position.vy;
    Temp.vz = (Pos.vz >> 12) - MikeCamera[Camera_CurrentMikeCamera].Position.vz;

    long Zed;
    gte_ldlv0(&Temp);
    gte_rtps();
    gte_stsxy(&yx);
    gte_stlvnl2(&Zed);
    return Zed;
}

/**
 * Bit_Init__Fv
 *
 * Address:   0x80037198
 * Mangled:   Bit_Init__Fv
 * Demangled: Bit_Init(void)
 */
void Bit_Init(void)
{
    BitCount = 0;
    NonRenderedBitList = 0;
    FlatBitList = 0;
    Linked2EndedBitList = 0;
    PixelList = 0;
    PolyLineList = 0;
    GPolyLineList = 0;
    QuadBitList = 0;
    GenPolyList = 0;
    ChunkBitList = 0;
    GlowList = 0;
    GlassList = 0;
    GLineList = 0;
    SpecialDisplayList = 0;
    int tpage = GetTPage(0, 1, 0, 0);
    SetDrawTPage(&StoredTPagePrim, 1, 1, tpage);
    int i = 0;
    SAnimFrame **dst = QuickAnimLookup;
    char **src = AnimNames;
    do
    {
        char *name = *src;
        src++;
        i++;
        SAnimFrame *anim = Spool_FindAnim(name);
        *dst = anim;
        dst++;
    } while (i < 6);
    ButtonsAnim = Spool_FindAnim("Buttons");
    pShadowTexture = Spool_FindTextureEntry(0x2A3B2550);
}

/**
 * DeleteBitList__FP4CBit
 *
 * Address:   0x8003727C
 * Mangled:   DeleteBitList__FP4CBit
 * Demangled: DeleteBitList(CBit *)
 *
 * @param pBitList  BitList to operate on.
 */
void DeleteBitList(CBit *pList)
{
    CBit *pNext;
    if (pList == 0)
        goto done;
    pNext = pList->mNext;
    do
    {
        if (pList->mProtected == 0 && pList != 0)
        {
            CBitVTable *vtable = (CBitVTable *)pList->_vf;
            vtable->Destroy(pList, 3);
        }
        if (pNext == 0)
            goto done;
        pList = pNext;
        pNext = pNext->mNext;
    } while (1);
done:;
}

/**
 * Bit_DeleteAll__Fv
 *
 * Address:   0x800372E4
 * Mangled:   Bit_DeleteAll__Fv
 * Demangled: Bit_DeleteAll(void)
 *
 */
void Bit_DeleteAll(void)
{
    DeleteBitList((CBit *)NonRenderedBitList);
    DeleteBitList((CBit *)FlatBitList);
    DeleteBitList((CBit *)Linked2EndedBitList);
    DeleteBitList((CBit *)PixelList);
    DeleteBitList((CBit *)PolyLineList);
    DeleteBitList((CBit *)GPolyLineList);
    DeleteBitList((CBit *)QuadBitList);
    DeleteBitList((CBit *)GenPolyList);
    DeleteBitList((CBit *)ChunkBitList);
    DeleteBitList((CBit *)GlowList);
    DeleteBitList((CBit *)GlassList);
    DeleteBitList((CBit *)GLineList);
    DeleteBitList((CBit *)SpecialDisplayList);
}

/**
 * RemoveDeadBits__FP4CBit
 *
 * Address:   0x80037398
 * Mangled:   RemoveDeadBits__FP4CBit
 * Demangled: RemoveDeadBits(CBit *)
 *
 * @param pBit  Bit to operate on.
 */
static void RemoveDeadBits(CBit *pBit)
{
    CBit *pNext;
    if (pBit == 0)
        goto done;
    pNext = pBit->mNext;
    do
    {
        if (pBit->mDead != 0 && pBit != 0)
        {
            CBitVTable *vtable = (CBitVTable *)pBit->_vf;
            vtable->Destroy(pBit, 3);
        }
        if (pNext == 0)
            goto done;
        pBit = pNext;
        pNext = pNext->mNext;
    } while (1);
done:;
}

/**
 * Bit_RemoveDeadBits__Fv
 *
 * Address:   0x80037400
 * Mangled:   Bit_RemoveDeadBits__Fv
 * Demangled: Bit_RemoveDeadBits(void)
 *
 */
void Bit_RemoveDeadBits(void)
{
    RemoveDeadBits((CBit *)NonRenderedBitList);
    RemoveDeadBits((CBit *)FlatBitList);
    RemoveDeadBits((CBit *)Linked2EndedBitList);
    RemoveDeadBits((CBit *)PixelList);
    RemoveDeadBits((CBit *)PolyLineList);
    RemoveDeadBits((CBit *)GPolyLineList);
    RemoveDeadBits((CBit *)QuadBitList);
    RemoveDeadBits((CBit *)GenPolyList);
    RemoveDeadBits((CBit *)ChunkBitList);
    RemoveDeadBits((CBit *)GlowList);
    RemoveDeadBits((CBit *)GlassList);
    RemoveDeadBits((CBit *)GLineList);
    RemoveDeadBits((CBit *)SpecialDisplayList);
}

/**
 * __nw__4CBitUi
 *
 * Address:   0x800374B4
 * Mangled:   __nw__4CBitUi
 * Demangled: CBit::operator new(unsigned int)
 *            Checks UseCacheForBits before allocating and zeroing storage,
 *            then updates TotalBitUsage
 *
 * @param size  Buffer size in bytes.
 * @return Zero-initialized storage allocated for the bit object.
 */
void *CBit::operator new(unsigned int size)
{
    void *pnew;
    if (UseCacheForBits != 0) {
        pnew = Mem_New(size, 0, 1);
    } else {
        pnew = Mem_New(size, 0, 1);
    }
    size = (size + 3) & ~3;
    unsigned int *p = (unsigned int *)pnew;
    int usage = TotalBitUsage;
    u32 words = size >> 2;
    usage = usage + 4;
    usage = usage + size;
    TotalBitUsage = usage;
    u32 i = 0;
    if (words != 0)
    {
        do
        {
            *p = 0;
            i++;
            p++;
        } while (i < words);
    }
    return pnew;
}

/**
 * CBit::operator delete(void *)
 *
 * Address:   0x80037534
 * Mangled:   __dl__4CBitPv
 * Demangled: CBit::operator delete(void *)
 *            Gets block size, subtracts (size+4) from TotalBitUsage, frees memory
 *
 * @param p Allocated bit storage to release.
 */
void CBit::operator delete(void *p)
{
    int size = Mem_GetBlockSize(p);
    int usage = TotalBitUsage;
    usage = usage - 4;
    usage = usage - size;
    TotalBitUsage = usage;
    Mem_Delete(p);
}

/**
 * AttachTo__4CBitPv
 *
 * Address:   0x80037574
 * Mangled:   AttachTo__4CBitPv
 * Demangled: CBit::AttachTo(void *)
 *            Doubly-linked list insertion: insert _this at head of *list
 *            ASM: lw $v0,($a1); sw $zero,($a0); sw $v0,4($a0); sw $a0,($a1);
 *            lw $v0,4($a0); beqz $v0,.end; sw $a0,($v0); .end: jr $ra
 *
 * @param p Address of the list head receiving this bit.
 */
void CBit::AttachTo(void *p)
{
    CBit *self = this;
    int *pList = (int *)p;
    int next = *pList;
    self->mPrevious = 0;          // prev = NULL
    self->mNext = (CBit *)next;   // next = old head
    *pList = (int)self;           // head = this
    CBit *n = self->mNext;
    if (n != 0)
    {
        n->mPrevious = self; // next->prev = this
    }
}

/**
 * DeleteFrom__4CBitPv
 *
 * Address:   0x800375A0
 * Mangled:   DeleteFrom__4CBitPv
 * Demangled: CBit::DeleteFrom(void *)
 *            Doubly-linked list removal: unlink _this from *list
 *
 * @param p Address of the list head from which this bit is removed.
 */
void CBit::DeleteFrom(void *p)
{
    int *pList = (int *)p;
    CBit *next = mNext;
    if (next != 0)
    {
        CBit *prev = mPrevious;
        next->mPrevious = prev;
    }
    CBit *prev = mPrevious;
    if (prev != 0)
    {
        CBit *next2 = mNext;
        prev->mNext = next2;
    }
    if (*pList == (int)this)
    {
        /* Re-read the member from `this`, not from the list head compared. */
        int next3 = (int)mNext;
        *pList = next3;
    }
}

/**
 * Die__4CBit
 *
 * Address:   0x800375FC
 * Mangled:   Die__4CBit
 * Demangled: CBit::Die(void)
 *            ASM: addiu $v0,$zero,1; jr $ra; sb $v0, 0x3f($a0)
 *
 */
void CBit::Die()
{
    mDead = 1;
}

/**
 * SetPos__4CBitRC7CVector
 *
 * Address:   0x80037608
 * Mangled:   SetPos__4CBitRC7CVector
 * Demangled: CBit::SetPos(const CVector &)
 *            ASM: lw $v0,0($a1); lw $v1,4($a1); lw $a2,8($a1); sw $v0,0x18($a0); sw $v1,0x1c($a0); sw $a2,0x20($a0); jr $ra; nop
 *
 * @param pos  Position vector.
 */
void CBit::SetPos(const CVector &pos)
{
    mPos = pos;
}

/**
 * __4CBit_default_ctor
 *
 * Address:   0x80037628
 * Mangled:   __4CBit
 * Demangled: CBit::CBit(void)
 *            Constructor: sets vtable at +0x44, sets +0x3c/+0x3d/+0x3e to 1, increments BitCount
 *
 * @param _this  Object being operated on.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
CBit *CBit_ctor(CBit *self)
{
    self->_vf = (void **)&__vt_4CBit;
    self->mFric.Set(1, 1, 1);
    int cnt = BitCount;
    BitCount = cnt + 1;
    return self;
}

/**
 * CBit::~CBit()
 *
 * Address:   0x80037660
 * Mangled:   _._4CBit
 * Demangled: CBit::~CBit()
 *            Destructor: sets vtable, decrements BitCount, conditionally calls operator delete
 *
 */
CBit::~CBit()
{
    int cnt = BitCount;
    _vf = (void **)&__vt_4CBit;
    BitCount = cnt - 1;
}

/**
 * __15CSpecialDisplay_default_ctor
 *
 * Address:   0x800376A0
 * Mangled:   __15CSpecialDisplay
 * Demangled: CSpecialDisplay::CSpecialDisplay(void)
 *            Constructor: calls CBit(), sets vtable (absolute addr), attaches to SpecialDisplayList
 *
 */
CSpecialDisplay::CSpecialDisplay()
{
    _vf = (void **)__vt_15CSpecialDisplay;
    AttachTo(&SpecialDisplayList);
}

/**
 * CSpecialDisplay::~CSpecialDisplay()
 *
 * Address:   0x800376E0
 * Mangled:   _._15CSpecialDisplay
 * Demangled: CSpecialDisplay::~CSpecialDisplay()
 *            Destructor: sets absolute vtable, calls DeleteFrom, calls parent ~CBit(flag=0), conditionally deletes
 *
 */
CSpecialDisplay::~CSpecialDisplay()
{
    _vf = (void **)__vt_15CSpecialDisplay;
    DeleteFrom(&SpecialDisplayList);
}

/**
 * __15CNonRenderedBit_default_ctor
 *
 * Address:   0x80037740
 * Mangled:   __15CNonRenderedBit
 * Demangled: CNonRenderedBit::CNonRenderedBit(void)
 *            Constructor: calls CBit parent, sets vtable, attaches to NonRenderedBitList
 *
 */
CNonRenderedBit::CNonRenderedBit()
{
    _vf = (void **)&__vt_15CNonRenderedBit;
    AttachTo(&NonRenderedBitList);
}

/**
 * CNonRenderedBit::~CNonRenderedBit()
 *
 * Address:   0x8003777C
 * Mangled:   _._15CNonRenderedBit
 * Demangled: CNonRenderedBit::~CNonRenderedBit()
 *            Destructor: sets vtable, calls DeleteFrom, calls parent ~CBit(flag=0), conditionally deletes
 *
 */
CNonRenderedBit::~CNonRenderedBit()
{
    _vf = (void **)&__vt_15CNonRenderedBit;
    DeleteFrom(&NonRenderedBitList);
}

/**
 * __7CFT4Bit_default_ctor
 *
 * Address:   0x800377D8
 * Mangled:   __7CFT4Bit
 * Demangled: CFT4Bit::CFT4Bit(void)
 *            Constructor: calls CBit parent, sets vtable, speed=128, scale=400, tint=0x2C808080
 *
 * @param self  Object being operated on.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
CFT4Bit *CFT4Bit_ctor(CFT4Bit *self)
{
    CBit_ctor(self);
    self->_vf = (void **)&__vt_7CFT4Bit;
    self->mAnimSpeed = 0x80;
    self->mScale = 0x190;
    self->mCodeBGR = 0x2C808080;
    return self;
}

/**
 * _._7CFT4Bit
 *
 * Address:   0x80037824
 * Mangled:   _._7CFT4Bit
 * Demangled: CFT4Bit::~CFT4Bit()
 *            Destructor: sets vtable, calls parent ~CBit(flag=0), conditionally deletes
 *
 * @param _this  Object being operated on.
 * @param flag  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
CFT4Bit::~CFT4Bit()
{
    _vf = (void **)&__vt_7CFT4Bit;
}

/**
 * SetScale__7CFT4BitUs
 *
 * Address:   0x80037874
 * Mangled:   SetScale__7CFT4BitUs
 * Demangled: CFT4Bit::SetScale(unsigned short)
 *            ASM: jr $ra; sh $a1, 0x5e($a0)
 *
 * @param s  New fixed-point scale.
 */
void CFT4Bit::SetScale(u16 s)
{
    mScale = s;
}

/**
 * SetSemiTransparent__7CFT4Bit
 *
 * Address:   0x8003787C
 * Mangled:   SetSemiTransparent__7CFT4Bit
 * Demangled: CFT4Bit::SetSemiTransparent(void)
 *            ASM: lw $v0,0x4c($a0); lui $v1,0x200; or $v0,$v0,$v1; jr $ra; sw $v0,0x4c($a0)
 *
 */
void CFT4Bit::SetSemiTransparent()
{
    mCodeBGR = mCodeBGR | 0x02000000;
}

/**
 * SetTint__7CFT4BitUcUcUc
 *
 * Address:   0x80037890
 * Mangled:   SetTint__7CFT4BitUcUcUc
 * Demangled: CFT4Bit::SetTint(unsigned char, unsigned char, unsigned char)
 *            ASM: andi $a1,0xff; andi $a2,0xff; andi $a3,0xff; lui $v1,0xff00; sll $a3,16;
 *            lw $v0,0x4c($a0); sll $a2,8; and $v0,$v0,$v1; or $v0,$v0,$a3; or $v0,$v0,$a2;
 *            or $v0,$v0,$a1; jr $ra; sw $v0,0x4c($a0)
 *
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 */
void CFT4Bit::SetTint(u8 r, u8 g, u8 b)
{
    u32 mask = 0xFF000000;
    register u32 val asm("$2") = mCodeBGR;
    val = (val & mask) | ((u32)b << 16) | ((u32)g << 8) | r;
    mCodeBGR = val;
}

/**
 * SetTransDecay__7CFT4Biti
 *
 * Address:   0x800378C4
 * Mangled:   SetTransDecay__7CFT4Biti
 * Demangled: CFT4Bit::SetTransDecay(int)
 *            ASM: jr $ra; sh $a1, 0x48($a0)
 *
 * @param decay  Amount subtracted from each tint channel per update.
 */
void CFT4Bit::SetTransDecay(int decay)
{
    mTransDecay = (s16)decay;
}

/**
 * SetAnim__7CFT4BitPc
 *
 * Address:   0x800378CC
 * Mangled:   SetAnim__7CFT4BitPc
 * Demangled: CFT4Bit::SetAnim(char *)
 *            Calls Spool_FindAnim, stores anim ptr, reads frameCount from -4 offset
 *
 * @param pName  Asset / object name string.
 */
void CFT4Bit::SetAnim(char *pName)
{
    SAnimFrame *animPtr = Spool_FindAnim(pName);
    mpPSXAnim = animPtr;
    u8 frameCount = *((u8 *)animPtr - 4);
    mFrame = 0;
    mFrameFrac = 0;
    mpPSXFrame = animPtr;
    mNumFrames = frameCount;
}

/**
 * SetFrame__7CFT4Biti
 *
 * Address:   0x8003790C
 * Mangled:   SetFrame__7CFT4Biti
 * Demangled: CFT4Bit::SetFrame(int)
 *            ASM: sb $a1,0x5a($a0); sll $a1,$a1,0x18; lw $v0,0x50($a0); sra $a1,$a1,0x15;
 *            sb $zero,0x5b($a0); addu $v0,$v0,$a1; jr $ra; sw $v0,0x54($a0)
 *
 * @param frame  Animation frame index.
 */
void CFT4Bit::SetFrame(int frame)
{
    mFrame = (char)frame;
    mFrameFrac = 0;
    mpPSXFrame = mpPSXAnim + (s8)frame;
}

/**
 * IncFrame__7CFT4Bit
 *
 * Address:   0x8003792C
 * Mangled:   IncFrame__7CFT4Bit
 * Demangled: CFT4Bit::IncFrame(void)
 *            Increments fractional frame counter using TimeScale * speed, updates anim pointer
 *
 */
void CFT4Bit::IncFrame()
{
    s16 speed = mAnimSpeed;
    int timer = TimeScale;
    int increment = speed * timer;
    u8 hi = *(u8 *)&mFrame;
    u8 lo = mFrameFrac;
    int frame16 = ((int)((u32)hi << 24) >> 16) | lo;
    SAnimFrame *basePtr = mpPSXAnim;
    frame16 = frame16 + (increment >> 8);
    frame16 = frame16 << 16;
    int frameHi = frame16 >> 24;
    int frameLo = frame16 >> 16;
    mFrame = (char)frameHi;
    mFrameFrac = (u8)frameLo;
    mpPSXFrame = basePtr + frameHi;
}

/**
 * IncFrameWithWrap__7CFT4Bit
 *
 * Address:   0x8003798C
 * Mangled:   IncFrameWithWrap__7CFT4Bit
 * Demangled: CFT4Bit::IncFrameWithWrap(void)
 *            Like IncFrame but wraps frame to 0 when >= maxFrames.
 *            Wraps the frame counter and refreshes the PSX frame pointer.
 *
 */
void CFT4Bit::IncFrameWithWrap()
{
    s16 speed = mAnimSpeed;
    int timer = TimeScale;
    int increment = speed * timer;
    u8 hi = *(u8 *)&mFrame;
    u8 maxFrames = mNumFrames;
    u8 lo = mFrameFrac;
    int frame16 = ((int)((u32)hi << 24) >> 16) | lo;
    frame16 = frame16 + (increment >> 8);
    frame16 = frame16 << 16;
    int frameHi = frame16 >> 24;
    int frameLo = frame16 >> 16;
    mFrame = (char)frameHi;
    mFrameFrac = (u8)frameLo;
    if (frameHi >= (int)maxFrames)
    {
        mFrame = 0;
    }
    int frame = (signed char)mFrame;
    SAnimFrame *basePtr = mpPSXAnim;
    mpPSXFrame = basePtr + frame;
}

/**
 * ChooseRandomFrame__7CFT4Bit
 *
 * Address:   0x80037A00
 * Mangled:   ChooseRandomFrame__7CFT4Bit
 * Demangled: CFT4Bit::ChooseRandomFrame(void)
 *            Picks random frame: Rnd(totalFrames), sign-extend×8 + basePtr
 *
 */
void CFT4Bit::ChooseRandomFrame()
{
    int frame = Rnd(mNumFrames);
    mFrame = (char)frame;
    SAnimFrame *basePtr = mpPSXAnim;
    mpPSXFrame = basePtr + (s8)frame;
}

/**
 * Shrink__7CFT4Biti
 *
 * Address:   0x80037A44
 * Mangled:   Shrink__7CFT4Biti
 * Demangled: CFT4Bit::Shrink(int)
 *            Decreases scale by shrink rate; if scale was zero and param!=0, dies; clamps to 0
 *
 * @param die  Nonzero to destroy the bit once its scale reaches zero.
 * @return 1 when shrinking is complete; otherwise 0.
 */
int CFT4Bit::Shrink(int die)
{
    s16 scale = mScale;
    u16 scaleU = *(u16 *)&mScale;
    if (scale == 0)
    {
        if (die == 0)
            return 1;
        Die();
        return 1;
    }
    u16 rate = *(u16 *)&mScaleDecay;
    int newScale = (int)(scaleU - rate);
    mScale = (s16)newScale;
    if ((newScale << 16) < 0)
    {
        mScale = 0;
    }
    return 0;
}

/**
 * Fade__7CFT4Biti
 *
 * Address:   0x80037AA0
 * Mangled:   Fade__7CFT4Biti
 * Demangled: CFT4Bit::Fade(int)
 *            Fades tint RGB towards zero by decay rate at +0x48; if fully faded and param!=0, dies
 *
 * @param die  Nonzero to destroy the bit once its tint reaches black.
 * @return 1 when fading is complete; otherwise 0.
 */
int CFT4Bit::Fade(int die)
{
    u32 color = mCodeBGR;
    u32 rgb = color & 0x00FFFFFF;
    if (rgb == 0)
    {
        if (die != 0)
        {
            Die();
        }
        return 1;
    }
    int g = (color >> 8) & 0xff;
    int b = (color >> 16) & 0xff;
    int r = *(u8 *)&mCodeBGR;
    int decay = mTransDecay;
    u16 decay_u = *(u16 *)&mTransDecay;
    if (r < decay)
    {
        r = 0;
    }
    else
    {
        int d = *(u8 *)&mTransDecay;
        r = (r - d) & 0xff;
    }
    asm("" : "+r"(decay_u));
    int decay2 = (int)(s16)decay_u;
    if (g < decay2)
    {
        g = 0;
    }
    else
    {
        int d = *(u8 *)&mTransDecay;
        g = (g - d) & 0xff;
    }
    asm("" : "+r"(decay_u));
    int decay3 = (int)(s16)decay_u;
    if (b < decay3)
    {
        b = 0;
    }
    else
    {
        int d = *(u8 *)&mTransDecay;
        b = (b - d) & 0xff;
    }
    u32 newColor = (color & 0xFF000000) | (b << 16) | (g << 8) | r;
    mCodeBGR = newColor;
    return 0;
}

/**
 * __8CQuadBit_default_ctor
 *
 * Address:   0x80037BA4
 * Mangled:   __8CQuadBit
 * Demangled: CQuadBit::CQuadBit(void)
 *            Constructor: calls CBit parent, sets vtable, stores 0x2C808080 at +0x90, attaches to QuadBitList
 *
 * @param self  Object being operated on.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
CQuadBit *CQuadBit_ctor(CQuadBit *self)
{
    CBit_ctor(self);
    self->_vf = (void **)&__vt_8CQuadBit;
    self->mCodeBGR = 0x2C808080;
    self->AttachTo(&QuadBitList);
    return self;
}

/**
 * _._8CQuadBit
 *
 * Address:   0x80037BEC
 * Mangled:   _._8CQuadBit
 * Demangled: CQuadBit::~CQuadBit()
 *            Destructor: sets vtable, calls DeleteFrom, calls parent ~CBit(flag=0), conditionally deletes
 *
 * @param _this  Object being operated on.
 * @param flag  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
CQuadBit::~CQuadBit()
{
    _vf = (void **)&__vt_8CQuadBit;
    DeleteFrom(&QuadBitList);
}

/**
 * SetTexture__8CQuadBitPci
 *
 * Address:   0x80037C48
 * Mangled:   SetTexture__8CQuadBitPci
 * Demangled: CQuadBit::SetTexture(char *, int)
 *            Calls Spool_FindAnim, indexes into frame array, stores texture info
 *
 * @param self  Object being operated on.
 * @param pAnimName  AnimName to operate on.
 * @param Frame  Animation frame index.
 */
void CQuadBit::SetTexture(char *pAnimName, int Frame)
{
    SAnimFrame *anim = Spool_FindAnim(pAnimName);
    SAnimFrame *entry =
        (SAnimFrame *)(Frame * sizeof(SAnimFrame) + (int)anim);
    Texture *texInfo = entry->pTexture;
    mpTexture = texInfo;
}

/**
 * FlagOn__8CQuadBitUc
 *
 * Address:   0x80037C8C
 * Mangled:   FlagOn__8CQuadBitUc
 * Demangled: CQuadBit::FlagOn(unsigned char)
 *            ASM: lbu $v0,0x98($a0); nop; or $v0,$v0,$a1; jr $ra; sb $v0,0x98($a0)
 *
 * @param self  Object being operated on.
 * @param b  Blue component (0-255).
 */
void CQuadBit::FlagOn(u8 b)
{
    mFlags = mFlags | b;
}

/**
 * SetSemiTransparent__8CQuadBit
 *
 * Address:   0x80037CA0
 * Mangled:   SetSemiTransparent__8CQuadBit
 * Demangled: CQuadBit::SetSemiTransparent(void)
 *            ASM: lw $v0,0x90($a0); lui $v1,0x200; or $v0,$v0,$v1; jr $ra; sw $v0,0x90($a0)
 *
 * @param self  Object being operated on.
 */
void CQuadBit::SetSemiTransparent()
{
    mCodeBGR = mCodeBGR | 0x02000000;
}

/**
 * SetTransparency__8CQuadBitUc
 *
 * Address:   0x80037CB4
 * Mangled:   SetTransparency__8CQuadBitUc
 * Demangled: CQuadBit::SetTransparency(unsigned char)
 *
 * @param self  Object being operated on.
 * @param t  Grayscale intensity copied into all three tint channels.
 */
void CQuadBit::SetTransparency(u8 t)
{
    register u32 byte asm("$5") = t;
    asm("" : "+r"(byte));
    u32 mask = 0xFF000000;
    u32 color = mCodeBGR;
    u32 hi = byte << 16;
    color = color & mask;
    color = color | hi;
    color = color | (byte << 8);
    color = color | byte;
    mCodeBGR = color;
}

/**
 * OrientUsing__8CQuadBitRC7CVectorRC7SVECTORiii
 *
 * Address:   0x80037CE0
 * Mangled:   OrientUsing__8CQuadBitRC7CVectorRC7SVECTORiii
 * Demangled: CQuadBit::OrientUsing(const CVector &, const SVECTOR &, int, int, int)
 *            Purpose: Build this quad from an origin and normal using normalized basis vectors.
 *
 * @param _this  Object being operated on.
 * @param Position  Quad center position.
 * @param Normal  Surface normal used to derive the quad basis.
 * @param dx  Scale along the first derived basis vector.
 * @param dy  Scale along the second derived basis vector.
 * @param Angle  Third scalar argument from the original signature.
 */
void CQuadBit::OrientUsing(const CVector &Position, const SVECTOR &Normal,
                           int dx, int dy, int Angle)
{
    int l1;
    int l2;
    CVector b1;
    CVector b2;

    b1.vz = (int)Normal.vy;
    b1.vx = 0;
    b1.vy = -(int)Normal.vz;
    b2.vx = b1.vz * b1.vz + (int)Normal.vz * (int)Normal.vz;
    b2.vy = -(int)Normal.vx * (int)Normal.vy;
    b2.vz = b1.vy * (int)Normal.vx;
    l1 = b1.Length();
    l2 = b2.Length();
    b1 = (b1 * dx) / l1;
    b2 = (b2 * dy) / l2;
    mPos = Position - b1 - b2;
    mPosB = Position + b1 - b2;
    mPosC = Position - b1 + b2;
    mPosD = Position + b1 + b2;
}

/**
 * _._8CGenPoly
 *
 * Address:   0x80037F14
 * Mangled:   _._8CGenPoly
 * Demangled: CGenPoly::~CGenPoly()
 *
 * @param _this  Object being operated on.
 * @param flag  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
CGenPoly::~CGenPoly()
{
    _vf = (void **)&__vt_8CGenPoly;
    DeleteFrom(&GenPolyList);
}

/**
 * Owner-local view of `CVector`'s original inline three-axis constructor.
 *
 * The shared layout currently exposes the vector fields but not that constructor.
 * Inheriting without adding fields preserves both the `CVector` representation and
 * its original component-initialization semantics until the declaration can be
 * promoted with a repo-wide regression audit.
 */
struct Bit_Vector : CVector
{
    Bit_Vector(int x, int y, int z)
    {
        vx = x;
        vy = y;
        vz = z;
    }
};

/**
 * __9CChunkBitRC8CSVectorN21
 *
 * Address:   0x80037F70
 * Mangled:   __9CChunkBitRC8CSVectorN21
 * Demangled: CChunkBit::CChunkBit(const CSVector &, const CSVector &,
 *                                 const CSVector &)
 *
 * Builds the fourth corner from the normalized cross product of the two triangle
 * edges, with a randomized displacement, then attaches the fragment to the global
 * chunk-bit list.
 *
 * @param a First triangle corner.
 * @param b Second triangle corner.
 * @param c Third triangle corner.
 */
CChunkBit::CChunkBit(const CSVector &a, const CSVector &b, const CSVector &c)
{
    _vf = (void **)&__vt_9CChunkBit;
    mType = 6;
    mPosA.vx = a.vx;
    mPosA.vy = a.vy;
    mPosA.vz = a.vz;
    mPosB.vx = b.vx;
    mPosB.vy = b.vy;
    mPosB.vz = b.vz;
    mPosC.vx = c.vx;
    mPosC.vy = c.vy;
    mPosC.vz = c.vz;

    Bit_Vector ca(c.vx - a.vx, c.vy - a.vy, c.vz - a.vz);
    Bit_Vector ba(b.vx - a.vx, b.vy - a.vy, b.vz - a.vz);
    CVector cprod;
    CVector norm;
    cprod.vx = ca.vy * ba.vz - ca.vz * ba.vy;
    cprod.vy = ca.vz * ba.vx - ca.vx * ba.vz;
    cprod.vz = ca.vx * ba.vy - ca.vy * ba.vx;
    Utils_CalcUnit(norm, cprod);
    int mul = Rnd(0x60) + 0x80;
    mPosD.vx = (short)((norm.vx * mul) >> 12);
    mPosD.vy = (short)((norm.vy * mul) >> 12);
    mPosD.vz = (short)((norm.vz * mul) >> 12);
    AttachTo(&ChunkBitList);
}

/**
 * CChunkBit::~CChunkBit()
 *
 * Address:   0x80038168
 * Mangled:   _._9CChunkBit
 * Demangled: CChunkBit::~CChunkBit()
 *
 */
CChunkBit::~CChunkBit()
{
    _vf = (void **)&__vt_9CChunkBit;
    DeleteFrom(&ChunkBitList);
}

/**
 * SetRGB__9CChunkBitUcUcUc
 *
 * Address:   0x800381C4
 * Mangled:   SetRGB__9CChunkBitUcUcUc
 * Demangled: CChunkBit::SetRGB(unsigned char, unsigned char, unsigned char)
 *
 * @param self  Object being operated on.
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 */
void CChunkBit::SetRGB(u8 r, u8 g, u8 b)
{
    CChunkBit *self = this;
    int rr = r & 0xff;
    int gg = g & 0xff;
    int bb = b & 0xff;
    u32 baseColor = (bb << 16) | (gg << 8) | rr;
    self->mB0G0R0 = baseColor;
    int mul = Rnd(0x1000);
    int c0 = ((bb * mul) >> 12) << 16;
    int c1 = ((gg * mul) >> 12) << 8;
    c0 = c0 | c1;
    int c2 = (rr * mul) >> 12;
    c0 = c0 | c2;
    self->mB1G1R1 = c0;
    mul = Rnd(0x1000);
    c0 = ((bb * mul) >> 12) << 16;
    c1 = ((gg * mul) >> 12) << 8;
    c0 = c0 | c1;
    c2 = (rr * mul) >> 12;
    c0 = c0 | c2;
    self->mB2G2R2 = c0;
    mul = Rnd(0x1000);
    int d0 = (bb * mul) >> 12;
    d0 = d0 << 16;
    int d1 = (gg * mul) >> 12;
    d1 = d1 << 8;
    d0 = d0 | d1;
    int d2 = (rr * mul) >> 12;
    d0 = d0 | d2;
    self->mB3G3R3 = d0;
}

/**
 * SetUVs__9CChunkBitUsUsUcUcUcUcUcUc
 *
 * Address:   0x80038318
 * Mangled:   SetUVs__9CChunkBitUsUsUcUcUcUcUcUc
 * Demangled: CChunkBit::SetUVs(unsigned short, unsigned short, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char)
 *            Packs tpage/clut + UV pairs into words at offsets 0xA0, 0xA4, 0xA8
 *
 * @param Clut  CLUT identifier packed with the first UV coordinate.
 * @param TPage  Texture-page identifier packed with the second UV coordinate.
 * @param u0  First vertex U coordinate.
 * @param v0  First vertex V coordinate.
 * @param u1  Second vertex U coordinate.
 * @param v1  Second vertex V coordinate.
 * @param u2  Third vertex U coordinate.
 * @param v2  Third vertex V coordinate.
 */
void CChunkBit::SetUVs(u16 Clut, u16 TPage,
                      u8 u0, u8 v0, u8 u1, u8 v1,
                      u8 u2, u8 v2)
{
    CChunkBit *self = this;
    self->mClutV0U0 = ((u32)Clut << 16) | ((u32)v0 << 8) | u0;
    self->mTPageV1U1 = ((u32)TPage << 16) | ((u32)v1 << 8) | u1;
    self->mV2U2 = (u16)((u32)v2 << 8 | u2);
}

/**
 * CalculateWorldCoords__9CChunkBit
 *
 * Address:   0x80038368
 * Mangled:   CalculateWorldCoords__9CChunkBit
 * Demangled: CChunkBit::CalculateWorldCoords(void)
 *            Builds the rotation matrix from mAngles, translates by mPos,
 *            and transforms mPosA..mPosD into mWorldPosA..mWorldPosD.
 *
 */
void CChunkBit::CalculateWorldCoords()
{
    CChunkBit *self = this;
    MATRIX Transform;
    M3dMaths_RotMatrixYXZ((SVECTOR *)&self->mAngles, &Transform);
    SetRotMatrix(&Transform);
    M3dAsm_SetTransVector(reinterpret_cast<Vector *>(&self->mPos));

    int *src;
    int *dst;

    // Transform vertex 0: mPosA -> mWorldPosA
    src = (int *)&self->mPosA;
    asm(
        "lwc2 $0, 0(%0)\n\t"
        "lwc2 $1, 4(%0)\n\t"
        "nop\n\t"
        "nop\n\t"
        ".word 0x4A480012\n\t"
        : "+r"(src));
    dst = (int *)&self->mWorldPosA;
    asm(
        "swc2 $25, 0(%0)\n\t"
        "swc2 $26, 4(%0)\n\t"
        "swc2 $27, 8(%0)\n\t"
        : : "r"(dst), "r"(src));

    // Transform vertex 1: mPosB -> mWorldPosB
    src = (int *)&self->mPosB;
    asm(
        "lwc2 $0, 0(%0)\n\t"
        "lwc2 $1, 4(%0)\n\t"
        "nop\n\t"
        "nop\n\t"
        ".word 0x4A480012\n\t"
        : "+r"(src));
    dst = (int *)&self->mWorldPosB;
    asm(
        "swc2 $25, 0(%0)\n\t"
        "swc2 $26, 4(%0)\n\t"
        "swc2 $27, 8(%0)\n\t"
        : : "r"(dst), "r"(src));

    // Transform vertex 2: mPosC -> mWorldPosC
    src = (int *)&self->mPosC;
    asm(
        "lwc2 $0, 0(%0)\n\t"
        "lwc2 $1, 4(%0)\n\t"
        "nop\n\t"
        "nop\n\t"
        ".word 0x4A480012\n\t"
        : "+r"(src));
    dst = (int *)&self->mWorldPosC;
    asm(
        "swc2 $25, 0(%0)\n\t"
        "swc2 $26, 4(%0)\n\t"
        "swc2 $27, 8(%0)\n\t"
        : : "r"(dst), "r"(src));

    // Transform vertex 3: mPosD -> mWorldPosD
    src = (int *)&self->mPosD;
    asm(
        "lwc2 $0, 0(%0)\n\t"
        "lwc2 $1, 4(%0)\n\t"
        "nop\n\t"
        "nop\n\t"
        ".word 0x4A480012\n\t"
        : : "r"(src));
    self = (CChunkBit *)&self->mWorldPosD;
    asm(
        "swc2 $25, 0(%0)\n\t"
        "swc2 $26, 4(%0)\n\t"
        "swc2 $27, 8(%0)\n\t"
        : : "r"(self));
}


/**
 * __5CGlowUiUi
 *
 * Address:   0x80038444
 * Mangled:   __5CGlowUiUi
 * Demangled: CGlow::(unsigned int, unsigned int)
 *            Purpose: Allocate glow sections/fringes and initialize default colour and fringe state.
 *
 * @param NumPoints  Number of points around half of the glow ring.
 * @param NumFringes  Number of outer fringe rings.
 */
CGlow::CGlow(u32 NumPoints, u32 NumFringes)
    : CBit()
{
    CGlow *glow = this;
    int numSections = NumPoints * 2;
    glow->_vf = (void **)&__vt_5CGlow;
    glow->mNumSections = numSections;
    glow->mNumFringes = NumFringes;
    glow->mStepAngle = (short)(0x1000u / (u32)numSections);
    // One SSection ring plus one SFringeQuad ring per fringe.
    SSection *sections = (SSection *)Mem_New(
        (numSections + numSections * NumFringes) * sizeof(SSection), 0, 1);
    glow->mpSections = sections;
    int allocSize = Mem_GetBlockSize(sections);
    int i = 0;
    register int count asm("$4") = glow->mNumSections * glow->mNumFringes;
    int usage = TotalBitUsage;
    usage = usage + 4;
    usage = usage + allocSize;
    TotalBitUsage = usage;
    glow->mpFringes = (SFringeQuad *)(glow->mpSections + glow->mNumSections);
    if (count != 0)
    {
        u32 fringeCode = 0x3a000000;
        do
        {
            glow->mpFringes[i].CodeBGR = fringeCode;
            i++;
            int loopSections = glow->mNumSections;
            register int loopFringes asm("$3") = glow->mNumFringes;
            count = loopSections * loopFringes;
        } while ((u32)i < (u32)count);
    }
    glow->mCentreCodeBGR = 0x32000000;
    glow->mMask = -1;
    glow->AttachTo(&GlowList);
}

/**
 * __5CGlowRC7CVectoriiUcUcUcUcUcUc — Build a fixed 8-spoke glow at the given
 * world position, seeding inner radius/color and one outer fringe ring.
 *
 * Address:   0x80038568
 * Mangled:   __5CGlowRC7CVectoriiUcUcUcUcUcUc
 * Demangled: CGlow::(const CVector &, int, int, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char)
 *            Purpose: Build a fixed 8-spoke glow from a world position.
 *
 * @param Pos  Position vector.
 * @param InnerRadius  Radius of the inner glow ring.
 * @param FringeRadius  Radius of the outer fringe ring.
 * @param r0  Red component for vertex/component 0.
 * @param g0  Green component for vertex/component 0.
 * @param b0  Blue component for vertex/component 0.
 * @param r1  Red component for vertex/component 1.
 * @param g1  Green component for vertex/component 1.
 * @param b1  Blue component for vertex/component 1.
 */
CGlow::CGlow(
    const CVector &Pos, int InnerRadius, int FringeRadius,
    u8 r0, u8 g0, u8 b0, u8 r1, u8 g1, u8 b1)
    : CBit()
{
    int self = (int)this;
    CGlow *glow = (CGlow *)self;
    int rad = InnerRadius;
    u8 cR = r0;
    u8 cG = g0;
    u8 cB = b0;
    u8 fR = r1;
    u8 fG = g1;
    u8 fB = b1;

    glow->_vf = (void **)&__vt_5CGlow;
    glow->mNumSections = 8;
    glow->mStepAngle = 0x200;
    glow->mNumFringes = 1;

    // Eight spokes: one SSection ring plus one SFringeQuad ring.
    glow->mpSections = (SSection *)Mem_New(16 * sizeof(SSection), 0, 1);
    TotalBitUsage += Mem_GetBlockSize(glow->mpSections) + 4;
    glow->mpFringes =
        (SFringeQuad *)(glow->mpSections + glow->mNumSections);
    glow->AttachTo(&GlowList);

    glow->mPos = Pos;

    /*
     * Keep the ring fields representation-addressed after initialization.
     * The target reuses one index for all three rings and initializes the
     * centre colour before taking the section-count snapshot for the next
     * phase.
     */
    u32 i = 0;
    if (*(u32 *)(self + 0x50) != 0)
    {
        do
        {
            int base = *(int *)(self + 0x48);
            *(int *)(i * 8 + base) = rad;
            i++;
        } while (i < *(u32 *)(self + 0x50));
    }

    i = 0;
    u32 centreBlue = (u32)cB << 16;
    u32 centreGreen = ((u32)cG << 8) | 0x32000000;
    u32 centreGB = centreBlue | centreGreen;
    u32 centreColor = centreGB | cR;
    u32 PadBGR = ((u32)fB << 16) | ((u32)fG << 8) | fR;
    *(u32 *)(self + 0x58) = centreColor;
    u32 count = *(u32 *)(self + 0x50);
    u32 fringeCount = count;
    PadBGR = PadBGR | fR;

    if (count == 0)
    {
        goto check_fringes;
    }

    do
    {
        int base = *(int *)(self + 0x48);
        *(u32 *)(i * 8 + base + 4) = PadBGR;
        count = *(u32 *)(self + 0x50);
        fringeCount = count;
        i++;
    } while (i < count);

check_fringes:
    i = 0;
    if (fringeCount != 0)
    {
        do
        {
            int offset = i << 3;
            *(int *)(offset + *(int *)(self + 0x4c)) = FringeRadius;
            offset = offset + *(int *)(self + 0x4c);
            *(u32 *)(offset + 4) = 0x3A000000;
            i++;
        } while (i < *(u32 *)(self + 0x50));
    }

    glow->mMask = -1;
}

/**
 * _._5CGlow
 *
 * Address:   0x80038754
 * Mangled:   _._5CGlow
 * Demangled: CGlow::~CGlow()
 *
 * @param _this  Object being operated on.
 * @param flag  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
CGlow::~CGlow()
{
    _vf = (void **)&__vt_5CGlow;
    int size = Mem_GetBlockSize(mpSections);
    int usage = TotalBitUsage;
    SSection *ptr2 = mpSections;
    usage = usage - 4;
    usage = usage - size;
    TotalBitUsage = usage;
    Mem_Delete(ptr2);
    DeleteFrom(&GlowList);
}

/**
 * SetRadius__5CGlowi
 *
 * Address:   0x800387D4
 * Mangled:   SetRadius__5CGlowi
 * Demangled: CGlow::SetRadius(int)
 *            Sets radius for all fringe segments (array at 0x48, stride 8, +0)
 *
 * @param Radius  Radius assigned to every glow section.
 */
void CGlow::SetRadius(int Radius)
{
    CGlow *self = this;
    u32 count = self->mNumSections;
    if (count == 0)
        return;
    u32 i = 0;
    do
    {
        self->mpSections[i].Radius = Radius;
        i++;
    } while (i < self->mNumSections);
}

/**
 * SetFringeWidth__5CGlowUiUi
 *
 * Address:   0x80038814
 * Mangled:   SetFringeWidth__5CGlowUiUi
 * Demangled: CGlow::SetFringeWidth(unsigned int, unsigned int)
 *            Set every segment width in one fringe ring of the glow mesh.
 *
 * @param Fringe  Fringe ring index.
 * @param Width  New width for each segment in the ring.
 */
void CGlow::SetFringeWidth(u32 Fringe, u32 Width)
{
    CGlow *self = this;
    u32 count = self->mNumSections;
    u32 i = 0;
    SFringeQuad *fringe = self->mpFringes + Fringe * count;
    if (count != 0)
    {
        SFringeQuad *pFringe = fringe;
        do
        {
            pFringe->Width = Width;
            pFringe++;
        } while (++i < self->mNumSections);
    }
}

/**
 * SetCentreRGB__5CGlowUcUcUc
 *
 * Address:   0x8003885C
 * Mangled:   SetCentreRGB__5CGlowUcUcUc
 * Demangled: CGlow::SetCentreRGB(unsigned char, unsigned char, unsigned char)
 *            Packs RGB with 0x32 alpha prefix into word at offset 0x58
 *
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 */
void CGlow::SetCentreRGB(u8 r, u8 g, u8 b)
{
    CGlow *self = this;
    u32 bb = (u32)b << 16;
    u32 gg = ((u32)g << 8) | 0x32000000;
    u32 color = bb | gg;
    self->mCentreCodeBGR = color | r;
}

/**
 * SetRGB__5CGlowUcUcUc
 *
 * Address:   0x80038888
 * Mangled:   SetRGB__5CGlowUcUcUc
 * Demangled: CGlow::SetRGB(unsigned char, unsigned char, unsigned char)
 *            Sets packed RGB color for all fringe segments (array at 0x48, stride 8, +4)
 *
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 */
void CGlow::SetRGB(u8 r, u8 g, u8 b)
{
    u32 i = 0;
    register CGlow *self asm("$9") = this;
    asm volatile("" : "+r"(self));
    u32 rr = r & 0xff;
    u32 gg = g & 0xff;
    u32 bb = b & 0xff;
    bb = bb << 16;
    gg = gg << 8;
    u32 PadBGR = bb | gg;
    u32 count = self->mNumSections;
    PadBGR = PadBGR | rr;
    if (count == 0)
        return;
    do
    {
        self->mpSections[i].PadBGR = PadBGR;
        i++;
    } while (i < self->mNumSections);
}

/**
 * SetFringeRGB__5CGlowUiUcUcUc
 *
 * Address:   0x800388E4
 * Mangled:   SetFringeRGB__5CGlowUiUcUcUc
 * Demangled: CGlow::SetFringeRGB(unsigned int, unsigned char, unsigned char, unsigned char)
 *            Copy packed RGB+alpha to every segment in a fringe ring
 *
 * @param Fringe  Fringe-ring index to recolor.
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 */
void CGlow::SetFringeRGB(u32 Fringe, u8 r, u8 g, u8 b)
{
    CGlow *self = this;
    u32 count = self->mNumSections;
    u32 i = 0;
    u32 bb = (u32)b << 16;
    u32 gg = ((u32)g << 8) | 0x3A000000;
    u32 color = bb | gg;
    u32 CodeBGR = color | r;
    SFringeQuad *pFringe = self->mpFringes + Fringe * count;
    if (count != 0)
    {
        u32 *pCodeBGR = &pFringe->CodeBGR;
        do
        {
            *pCodeBGR = CodeBGR;
            pCodeBGR += sizeof(SFringeQuad) / sizeof(u32);
        } while (++i < self->mNumSections);
    }
}

/**
 * SetRadialRGB__5CGlowiUcUcUc
 *
 * Address:   0x80038950
 * Mangled:   SetRadialRGB__5CGlowiUcUcUc
 * Demangled: CGlow::SetRadialRGB(int, unsigned char, unsigned char, unsigned char)
 *            Copy packed RGB+alpha to the radial spoke and all fringe rings
 *
 * @param Spoke  Radial section index to recolor.
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 */
void CGlow::SetRadialRGB(int Spoke, u8 r, u8 g, u8 b)
{
    CGlow *self = this;
    u32 i = 0;
    u32 bb = (u32)b << 16;
    u32 gg = ((u32)g << 8) | 0x3A000000;
    u32 packed = bb | gg;
    u32 CodeBGR = packed | r;
    self->mpSections[Spoke].PadBGR = CodeBGR;
    SFringeQuad *pFringe = self->mpFringes + Spoke;
    if (self->mNumFringes != 0)
    {
        do
        {
            i++;
            pFringe->CodeBGR = CodeBGR;
            pFringe += self->mNumSections;
        } while (i < self->mNumFringes);
    }
}

// ====== 0x800389C8 __11CWobblyGlowRC7CVectoriiiUcUcUcUcUcUc ====================
// __11CWobblyGlowRC7CVectoriiiUcUcUcUcUcUc
// Address: 0x800389C8
// Demangled: CWobblyGlow::(const CVector &, int, int, int, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char)
// Wraps CGlow construction and seeds per-section wobble phase and velocity arrays.
CWobblyGlow::CWobblyGlow(
    const CVector &Pos,
    int InnerRadius,
    int FringeRadius,
    int Amp,
    u8 r0,
    u8 g0,
    u8 b0,
    u8 r1,
    u8 g1,
    u8 b1)
    : CGlow(Pos, InnerRadius, FringeRadius, r0, g0, b0, r1, g1, b1)
{
    CWobblyGlow *glow = this;
    int product = InnerRadius * Amp;
    glow->_vf = (void **)&__vt_11CWobblyGlow;
    glow->mInnerRadius = InnerRadius;
    if (product < 0)
    {
        product = product + 0xFF;
    }
    int i = 0;
    int sections = glow->mNumSections;
    glow->mAmplitude = product >> 8;
    if (sections != 0)
    {
        int *velocities = glow->mInc;
        int *angles = glow->mT;
        int sections_lim;
        do
        {
            int phase = Rnd(0x1000);
            *angles = phase;
            int velRnd = Rnd(0x32);
            *velocities = velRnd + 0x32;
            velocities++;
            angles++;
            ++i;
            sections_lim = glow->mNumSections;
        } while ((u32)i < (u32)sections_lim);
    }
}

/**
 * Move__11CWobblyGlow
 *
 * Address:   0x80038AAC
 * Mangled:   Move__11CWobblyGlow
 * Demangled: CWobblyGlow::Move(void)
 *            Wobble effect: updates angles, looks up sine table, modulates radius per segment
 *
 * @param self  Object being operated on.
 */
void CWobblyGlow::Move()
{
    u32 count = mNumSections;
    u32 i = 0;
    if (count == 0)
        return;
    register s16 *sineBase asm("$10") = (s16 *)&rcossin_tbl;
    int *angles = mT;
    int *velocities = mInc;
    do
    {
        int angle = *angles + *velocities;
        *angles = angle;
        int byteOff = (angle & 0xFFF) << 2;
        s16 sine = *(s16 *)(byteOff + (int)sineBase);
        int amplitude = mAmplitude;
        int product = amplitude * sine;
        int radiusBase = (int)mpSections;
        int *radiusPtr = (int *)(i * 8 + radiusBase);
        if (product < 0)
        {
            product = product + 0xFFF;
        }
        angles++;
        int baseRadius = mInnerRadius;
        int wobble = product >> 12;
        *radiusPtr = baseRadius + wobble;

        register u32 reloadCount asm("$4") = mNumSections;
        asm volatile("" : "+r"(reloadCount));
        count = reloadCount;
        i++;
        velocities++;
    } while (i < count);
}

/**
 * __8CFlatBit_default_ctor
 *
 * Address:   0x80038B48
 * Mangled:   __8CFlatBit
 * Demangled: CFlatBit::CFlatBit(void)
 *            Constructor: calls CFT4Bit parent, sets vtable, attaches to FlatBitList, inits fields
 *
 */
CFlatBit::CFlatBit()
{
    CFlatBit *self = this;
    self->_vf = (void **)&__vt_8CFlatBit;
    self->AttachTo(&FlatBitList);
    self->mSemiTransparencyRate = 0x20;
    self->mAngFric = 1;
    self->mPostScale = 0x10001000;
}

/**
 * _._8CFlatBit
 *
 * Address:   0x80038BA0
 * Mangled:   _._8CFlatBit
 * Demangled: CFlatBit::~CFlatBit()
 *            Destructor: sets vtable, calls DeleteFrom, calls parent ~CFT4Bit(flag=0), conditionally deletes
 *
 * @param _this  Object being operated on.
 * @param flag  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
CFlatBit::~CFlatBit()
{
    _vf = (void **)&__vt_8CFlatBit;
    DeleteFrom(&FlatBitList);
}

/**
 * __16CLinked2EndedBit_default_ctor
 *
 * Address:   0x80038BFC
 * Mangled:   __16CLinked2EndedBit
 * Demangled: CLinked2EndedBit::CLinked2EndedBit(void)
 *            Constructor: calls CFT4Bit parent, sets vtable, attaches to Linked2EndedBitList
 *
 * @param self  Object being operated on.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
CLinked2EndedBit *CLinked2EndedBit_ctor(CLinked2EndedBit *self)
{
    CFT4Bit_ctor(self);
    self->_vf = (void **)&__vt_16CLinked2EndedBit;
    self->AttachTo(&Linked2EndedBitList);
    return self;
}

/**
 * _._16CLinked2EndedBit
 *
 * Address:   0x80038C38
 * Mangled:   _._16CLinked2EndedBit
 * Demangled: CLinked2EndedBit::~CLinked2EndedBit()
 *            Destructor: sets vtable, calls DeleteFrom, calls parent ~CFT4Bit(flag=0), conditionally deletes
 *
 * @param _this  Object being operated on.
 * @param flag  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
CLinked2EndedBit::~CLinked2EndedBit()
{
    _vf = (void **)&__vt_16CLinked2EndedBit;
    DeleteFrom(&Linked2EndedBitList);
}

/**
 * __6CPixel_default_ctor
 *
 * Address:   0x80038C94
 * Mangled:   __6CPixel
 * Demangled: CPixel::CPixel(void)
 *            Constructor: calls CBit parent, sets vtable, attaches to PixelList
 *
 * @param self  Object being operated on.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
CPixel::CPixel()
{
    _vf = (void **)&__vt_6CPixel;
    AttachTo(&PixelList);
}

/**
 * _._6CPixel
 *
 * Address:   0x80038CD0
 * Mangled:   _._6CPixel
 * Demangled: CPixel::~CPixel()
 *            Destructor: sets vtable, calls DeleteFrom, calls parent ~CBit(flag=0), conditionally deletes
 *
 */
CPixel::~CPixel()
{
    _vf = (void **)&__vt_6CPixel;
    DeleteFrom(&PixelList);
}

/**
 * Bit_SetSparkTrajectory__FRC8CSVector
 *
 * Address:   0x80038D2C
 * Mangled:   Bit_SetSparkTrajectory__FRC8CSVector
 * Demangled: Bit_SetSparkTrajectory(const CSVector &)
 *            Struct copy of 6-byte CSVector to SparkTrajectory global
 *
 * @param trajectory  Base direction used for subsequently generated sparks.
 */
void Bit_SetSparkTrajectory(const CSVector &trajectory)
{
    SparkTrajectory = trajectory;
}

/**
 * Bit_SetSparkTrajectoryCone__FRC8CSVector
 *
 * Address:   0x80038D50
 * Mangled:   Bit_SetSparkTrajectoryCone__FRC8CSVector
 * Demangled: Bit_SetSparkTrajectoryCone(const CSVector &)
 *            Struct copy of 6-byte CSVector to SparkTrajectoryCone global
 *
 * @param cone  Per-axis random angular range around the base trajectory.
 */
void Bit_SetSparkTrajectoryCone(const CSVector &cone)
{
    SparkTrajectoryCone = cone;
}

/**
 * Bit_CalculateSparkVelocity__FR7CVectori
 *
 * Address:   0x80038D74
 * Mangled:   Bit_CalculateSparkVelocity__FR7CVectori
 * Demangled: Bit_CalculateSparkVelocity(CVector &, int)
 *
 * @param velocity  Receives the generated spark velocity.
 * @param magnitude  Requested velocity magnitude.
 */
void Bit_CalculateSparkVelocity(CVector &velocity, int magnitude)
{
    CSVector angles = SparkTrajectory;

    if (SparkTrajectoryCone.vx != 0)
    {
        int r = Rnd(SparkTrajectoryCone.vx);
        unsigned short cur = angles.vx;
        short half = (short)(unsigned short)SparkTrajectoryCone.vx >> 1;
        angles.vx = cur + (r - half);
    }

    if (SparkTrajectoryCone.vy != 0)
    {
        int r = Rnd(SparkTrajectoryCone.vy);
        unsigned short cur = angles.vy;
        short half = (short)(unsigned short)SparkTrajectoryCone.vy >> 1;
        angles.vy = cur + (r - half);
    }

    if (SparkTrajectoryCone.vz != 0)
    {
        int r = Rnd(SparkTrajectoryCone.vz);
        unsigned short cur = angles.vz;
        short half = (short)(unsigned short)SparkTrajectoryCone.vz >> 1;
        angles.vz = cur + (r - half);
    }

    Utils_GetVecFromMagDir(velocity, magnitude, angles);
}

/**
 * Bit_SetSparkSize__FUi
 *
 * Address:   0x80038E6C
 * Mangled:   Bit_SetSparkSize__FUi
 * Demangled: Bit_SetSparkSize(unsigned int)
 *            ASM: sll $v0,$a0,0x10; or $v0,$v0,$a0; sw $v0,0x1cc0($gp); jr $ra; nop
 *
 * @param size  Buffer size in bytes.
 */
void Bit_SetSparkSize(unsigned int size)
{
    SparkSize = (size << 16) | size;
}

/**
 * Bit_SetSparkRGB__FUcUcUc
 *
 * Address:   0x80038E80
 * Mangled:   Bit_SetSparkRGB__FUcUcUc
 * Demangled: Bit_SetSparkRGB(unsigned char, unsigned char, unsigned char)
 *            ASM: sb $a0,0x1cc8($gp); sb $a1,0x1cc9($gp); sb $a2,0x1cca($gp); jr $ra; nop
 *
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 */
void Bit_SetSparkRGB(unsigned char r, unsigned char g, unsigned char b)
{
    SparkRGB.r = r;
    SparkRGB.g = g;
    SparkRGB.b = b;
}

/**
 * Bit_SetSparkFadeRGB__FUcUcUc
 *
 * Address:   0x80038E94
 * Mangled:   Bit_SetSparkFadeRGB__FUcUcUc
 * Demangled: Bit_SetSparkFadeRGB(unsigned char, unsigned char, unsigned char)
 *            ASM: sb $a0,0x1ccc($gp); sb $a1,0x1ccd($gp); sb $a2,0x1cce($gp); jr $ra; nop
 *
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 */
void Bit_SetSparkFadeRGB(unsigned char r, unsigned char g, unsigned char b)
{
    SparkFadeRGB.r = r;
    SparkFadeRGB.g = g;
    SparkFadeRGB.b = b;
}

/**
 * __6CSparkR7CVectoriii
 *
 * Address:   0x80038EA8
 * Mangled:   __6CSparkR7CVectoriii
 * Demangled: CSpark::(CVector &, int, int, int)
 *
 * @param self  Object being operated on.
 * @param pos  Position vector.
 * @param vel  Initial speed magnitude.
 * @param gravity  Vertical acceleration applied each update.
 * @param life  Exclusive upper bound for the randomized lifetime.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
CSpark::CSpark(CVector &pos, int vel, int gravity, int life)
{
    int sizeMode = SparkSize & 0xf;
    _vf = (void **)&__vt_6CSpark;
    if (sizeMode == 1)
    {
        code = 0x68;
        tag = 0x02000000;
        mWidthHeight = 1;
    }
    else
    {
        code = 0x60;
        tag = 0x03000000;
        mWidthHeight = SparkSize;
    }
    r0 = SparkRGB.r;
    g0 = SparkRGB.g;
    b0 = SparkRGB.b;
    mFadeR = SparkFadeRGB.r;
    mFadeG = SparkFadeRGB.g;
    mFadeB = SparkFadeRGB.b;
    mPos = pos;
    Bit_CalculateSparkVelocity(mVel, vel);
    mAcc.vy = gravity;
    mFric.Set(3, 3, 3);
    int rndResult = Rnd(life);
    mLifetime = (u16)rndResult;
    if (SparkSemiTrans != 0)
    {
        code |= 2;
    }
}

/**
 * _._6CSpark
 *
 * Address:   0x80038FE4
 * Mangled:   _._6CSpark
 * Demangled: CSpark::~CSpark()
 *
 */
CSpark::~CSpark()
{
    _vf = (void **)&__vt_6CSpark;
}

/**
 * Move__6CSpark
 *
 * Address:   0x80039034
 * Mangled:   Move__6CSpark
 * Demangled: CSpark::Move(void)
 *            Purpose: Apply acceleration, advance life, and fade spark channels.
 *
 */
void CSpark::Move()
{
    CSpark *spark = this;
    int x = spark->mPos.vx + spark->mVel.vx;
    int y = spark->mPos.vy + spark->mVel.vy;
    int z = spark->mPos.vz + spark->mVel.vz;
    int floor = spark->mFloorY;
    if (floor != 0 && floor < y)
    {
        spark->mPos.vy = floor;
    }
    else
    {
        spark->mPos.vx = x;
        spark->mPos.vy = y;
        spark->mPos.vz = z;
        x = spark->mVel.vx + spark->mAcc.vx;
        y = spark->mVel.vy + spark->mAcc.vy;
        z = spark->mVel.vz + spark->mAcc.vz;
        x -= x >> 3;
        y -= y >> 3;
        z -= z >> 3;
        spark->mVel.vx = x;
        spark->mVel.vy = y;
        spark->mVel.vz = z;
    }
    unsigned int life = (u16)spark->mAge;
    int maxlife = spark->mLifetime;
    life = life + 1;
    spark->mAge = (s16)life;
    if ((s16)life >= maxlife)
    {
        spark->Die();
    }
    u8 r = spark->r0;
    u8 fr = spark->mFadeR;
    u8 g = spark->g0;
    u8 fb = spark->mFadeB;
    r = r - fr;
    u8 fg = spark->mFadeG;
    spark->r0 = r;
    u8 b = spark->b0;
    g = g - fg;
    b = b - fb;
    spark->g0 = g;
    spark->b0 = b;
}

/**
 * _._11CSimpleAnim
 *
 * Address:   0x80039144
 * Mangled:   _._11CSimpleAnim
 * Demangled: CSimpleAnim::~CSimpleAnim()
 *
 * @param _this  Object being operated on.
 * @param flag  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
CSimpleAnim::~CSimpleAnim()
{
    _vf = (void **)&__vt_11CSimpleAnim;
}

/**
 * Move__11CSimpleAnim
 *
 * Address:   0x80039194
 * Mangled:   Move__11CSimpleAnim
 * Demangled: CSimpleAnim::Move(void)
 *            Calls IncFrame, then checks loop mode (+0x70) and frame limits
 *
 */
void CSimpleAnim::Move()
{
    CSimpleAnim *self = this;
    self->IncFrame();
    {
        int loopMode = self->mDie;
        if (loopMode != 0)
        {
            s8 frame = (signed char)self->mFrame;
            int limit = self->mDieFrame;
            if (frame >= limit)
            {
                self->Die();
            }
        }
        else
        {
            s8 frame = (signed char)self->mFrame;
            u8 maxFrames = self->mNumFrames;
            if (frame < (int)maxFrames)
                goto done_anim;
            {
                int negTwo = -2;
                int mode = self->mDieFrame;
                if (mode == negTwo)
                {
                    self->SetFrame(maxFrames - 1);
                }
                else
                {
                    self->SetFrame(0);
                }
            }
        }
    }
done_anim:;
}

/**
 * __11CMotionBlurRC7CVectorR7CVectorPciii
 *
 * Address:   0x8003922C
 * Mangled:   __11CMotionBlurRC7CVectorR7CVectorPciii
 * Demangled: CMotionBlur::(const CVector &, CVector &, char *, int, int, int)
 *            Purpose: Initialize a flat motion-blur bit from position, velocity, animation, and fade parameters.
 *
 * @param _this  Object being operated on.
 * @param pos  Position vector.
 * @param vel  Initial velocity.
 * @param pAnimName  AnimName to operate on.
 * @param initialscale  Initial fixed-point sprite scale.
 * @param scaledecay  Scale removed per update.
 * @param transdecay  Tint removed per update.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
CMotionBlur *CMotionBlur_ctor(
    CMotionBlur *self, CVector *pos, CVector *vel, char *pAnimName,
    int initialscale, int scaledecay, int transdecay)
{
    ::new (self) CFlatBit();
    self->_vf = (void **)&__vt_11CMotionBlur;
    self->mPos = *pos;
    self->mVel = *vel;
    self->SetAnim(pAnimName);
    self->SetSemiTransparent();
    self->mScale = (s16)initialscale;
    self->mScaleDecay = (s16)scaledecay;
    self->mTransDecay = (s16)transdecay;
    return self;
}

/**
 * _._11CMotionBlur
 *
 * Address:   0x800392F4
 * Mangled:   _._11CMotionBlur
 * Demangled: CMotionBlur::~CMotionBlur()
 *
 * @param _this  Object being operated on.
 * @param flag  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
CMotionBlur::~CMotionBlur()
{
    _vf = (void **)&__vt_11CMotionBlur;
}

/**
 * Move__11CMotionBlur
 *
 * Address:   0x80039344
 * Mangled:   Move__11CMotionBlur
 * Demangled: CMotionBlur::Move(void)
 *            Copies velocity to position, then shrink/fade/incFrameWithWrap chain
 *
 */
void CMotionBlur::Move()
{
    CMotionBlur *self = this;
    self->mPos += self->mVel;
    if (self->Shrink(1))
        return;
    if (self->Fade(1))
        return;
    self->IncFrameWithWrap();
}

/**
 * _._19CMotionBlurImmortal
 *
 * Address:   0x800393A0
 * Mangled:   _._19CMotionBlurImmortal
 * Demangled: CMotionBlurImmortal::~CMotionBlurImmortal()
 *            Destructor: sets vtable, calls parent ~CMotionBlur(flag=0), conditionally deletes
 *            NOTE: Must immediately follow Move__11CMotionBlur (merged .s file)
 *
 * @param _this  Object being operated on.
 * @param flag  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
CMotionBlurImmortal::~CMotionBlurImmortal()
{
    _vf = (void **)&__vt_19CMotionBlurImmortal;
}

/**
 * Move__19CMotionBlurImmortal
 *
 * Address:   0x800393F0
 * Mangled:   Move__19CMotionBlurImmortal
 * Demangled: CMotionBlurImmortal::Move(void)
 *            Same as Move__11CMotionBlur but with param=0 (immortal, no death on shrink/fade)
 *
 */
void CMotionBlurImmortal::Move()
{
    CMotionBlurImmortal *self = this;
    self->mPos += self->mVel;
    if (self->Shrink(0))
        return;
    if (self->Fade(0))
        return;
    self->IncFrameWithWrap();
}

/**
 * _._5CFrag
 *
 * Address:   0x8003944C
 * Mangled:   _._5CFrag
 * Demangled: CFrag::~CFrag()
 *            Destructor: sets vtable, calls parent ~CFlatBit(flag=0), conditionally deletes
 *            NOTE: Must immediately follow Move__19CMotionBlurImmortal (merged .s file)
 *
 * @param _this  Object being operated on.
 * @param flag  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
CFrag::~CFrag()
{
    _vf = (void **)&__vt_5CFrag;
}

/**
 * Move__5CFrag
 *
 * Address:   0x8003949C
 * Mangled:   Move__5CFrag
 * Demangled: CFrag::Move(void)
 *            Purpose: Advance fragment physics, damp velocity, update animation frame, and expire by lifetime.
 *
 */
void CFrag::Move()
{
    CFrag *_this = this;
    register int v0 asm("$2") = _this->mPos.vx;
    int a1 = _this->mVel.vx;
    register int v1 asm("$3") = _this->mPos.vy;
    int a2 = _this->mVel.vy;
    register int a3 asm("$7") = _this->mVel.vz;
    register int a0 asm("$4") = _this->mAcc.vy;
    v0 = v0 + a1;
    v1 = v1 + a2;
    _this->mPos.vx = v0;
    v0 = _this->mPos.vz;
    a2 = a2 + a0;
    _this->mPos.vy = v1;
    v1 = _this->mAcc.vx;
    a0 = _this->mFric.vz;
    a1 = a1 + v1;
    v1 = _this->mAcc.vz;
    v0 = v0 + a3;
    _this->mPos.vz = v0;
    v0 = _this->mFric.vx;
    _this->mVel.vy = a2;
    _this->mVel.vx = a1;
    a3 = a3 + v1;
    v0 = a1 >> v0;
    a1 = a1 - v0;
    v1 = _this->mFric.vy;
    v0 = *(u8 *)&_this->mFrame;
    a0 = a3 >> a0;
    _this->mVel.vz = a3;
    a3 = a3 - a0;
    _this->mVel.vx = a1;
    _this->mVel.vz = a3;
    v1 = a2 >> v1;
    a2 = a2 - v1;
    v0 = v0 + 1;
    _this->mFrame = v0;
    v0 = (s8)v0;
    v1 = _this->mNumFrames;
    _this->mVel.vy = a2;
    if (v0 >= v1)
    {
        v0 = v1 - 1;
        _this->mFrame = v0;
    }
    v0 = *(u16 *)&_this->mAge;
    v1 = _this->mLifetime;
    v0 = v0 + 1;
    _this->mAge = v0;
    v0 = (s16)v0;
    if (v0 >= v1)
    {
        _this->Die();
    }
}

/**
 * __10CRibbonBit — Construct one animated ribbon segment.
 *
 * Address:   0x80039590
 * Mangled:   __10CRibbonBit
 * Demangled: CRibbonBit::CRibbonBit(void)
 *            Constructor: calls CLinked2EndedBit parent, sets vtable (gp-relative)
 *
 */
CRibbonBit::CRibbonBit()
{
    _vf = (void **)&__vt_10CRibbonBit;
}

/**
 * _._10CRibbonBit
 *
 * Address:   0x800395C0
 * Mangled:   _._10CRibbonBit
 * Demangled: CRibbonBit::~CRibbonBit()
 *            Destructor: sets vtable, calls parent ~CLinked2EndedBit(flag=0), conditionally deletes
 *
 */
CRibbonBit::~CRibbonBit()
{
    _vf = (void **)&__vt_10CRibbonBit;
}

/**
 * Move__10CRibbonBit
 *
 * Address:   0x80039610
 * Mangled:   Move__10CRibbonBit
 * Demangled: CRibbonBit::Move(void)
 *            Tail call to IncFrameWithWrap (but not optimized — saves/restores $ra)
 *
 */
void CRibbonBit::Move()
{
    CRibbonBit *self = this;
    self->IncFrameWithWrap();
}

/**
 * __7CRibbonRC7CVectoriiPcT4ii — Construct a history-backed ribbon effect.
 *
 * Address:   0x80039630
 * Mangled:   __7CRibbonRC7CVectoriiPcT4ii
 * Demangled: CRibbon::CRibbon(const CVector &, int, int, char *, char *,
 *                             int, int)
 *
 * Allocates the position history and child-bit arrays, initializes every
 * history entry at `pos`, and configures the middle and end animations.
 *
 * @param pos Initial position for the complete ribbon history.
 * @param numbits Number of animated ribbon segments.
 * @param pointsperbit Number of history samples separating adjacent segments.
 * @param pMiddleAnimName Animation used by the interior segments.
 * @param pEndAnimName Animation used by the leading segment.
 * @param scale Initial segment scale.
 * @param semitrans Nonzero to enable semi-transparency on every segment.
 */
CRibbon::CRibbon(const CVector &pos, int numbits, int pointsperbit,
                 char *pMiddleAnimName, char *pEndAnimName, int scale,
                 int semitrans)
{
    _vf = (void **)&__vt_7CRibbon;
    mNumBits = numbits;
    mPointsPerBit = pointsperbit - 1;
    mNumPoints = numbits * (pointsperbit - 1) + 1;
    mPoints = (CVector *)Mem_New(mNumPoints * sizeof(CVector), 0, 1);
    TotalBitUsage += Mem_GetBlockSize(mPoints) + 4;

    if (mNumPoints > 0)
    {
        int i = 0;
        int offset = 0;
        do
        {
            CVector *point = (CVector *)(offset + (int)mPoints);
            *point = pos;
            i++;
            offset += sizeof(CVector);
        } while (i < mNumPoints);
    }

    mBits = (CRibbonBit **)Mem_New(numbits * sizeof(CRibbonBit *), 0, 1);
    TotalBitUsage += Mem_GetBlockSize(mBits) + 4;

    for (int i = mNumBits - 1; i >= 0; i--)
    {
        unsigned short childScale = (unsigned short)scale;
        CRibbonBit **slot = (CRibbonBit **)((i * sizeof(CRibbonBit *)) +
                                            (int)mBits);
        CRibbonBit *child = static_cast<CRibbonBit *>(
            Bit_AllocateStorage(sizeof(CRibbonBit)));
        if (child != 0)
        {
            child = CRibbonBit_ctor(child);
        }
        *slot = child;
        ((CBit *)mBits[i])->mProtected = 1;
        mBits[i]->SetAnim(pMiddleAnimName);
        mBits[i]->SetScale(childScale);
        if (semitrans != 0)
        {
            mBits[i]->SetSemiTransparent();
        }
    }
    mBits[0]->mBitFlags |= 0x10;
    mBits[0]->SetAnim(pEndAnimName);
}

/**
 * _._7CRibbon
 *
 * Address:   0x80039848
 * Mangled:   _._7CRibbon
 * Demangled: CRibbon::~CRibbon()
 *
 * @param _this  Object being operated on.
 * @param flag  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
CRibbon::~CRibbon()
{
    int i = 0;
    _vf = (void **)&__vt_7CRibbon;
    int count = mNumBits;
    if (count > 0)
    {
        do
        {
            CRibbonBit *child = mBits[i];
            if (child != 0)
            {
                CBitVTable *vtable = (CBitVTable *)child->_vf;
                vtable->Destroy(child, 3);
                count = mNumBits;
            }
            i++;
        } while (i < count);
    }
    // Free children array and position buffer
    int size1 = Mem_GetBlockSize(mBits);
    int usage1 = TotalBitUsage;
    CVector *buf2 = mPoints;
    usage1 = usage1 - 4;
    usage1 = usage1 - size1;
    TotalBitUsage = usage1;
    int size2 = Mem_GetBlockSize(buf2);
    int usage2 = TotalBitUsage;
    CRibbonBit **buf1 = mBits;
    usage2 = usage2 - 4;
    usage2 = usage2 - size2;
    TotalBitUsage = usage2;
    Mem_Delete(buf1);
    Mem_Delete(mPoints);
}

/**
 * SetTint__7CRibbonUcUcUc
 *
 * Address:   0x80039948
 * Mangled:   SetTint__7CRibbonUcUcUc
 * Demangled: CRibbon::SetTint(unsigned char, unsigned char, unsigned char)
 *            Loops over children array at 0x5c, count at 0x48, calling SetTint__7CFT4BitUcUcUc
 *
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 */
void CRibbon::SetTint(u8 r, u8 g, u8 b)
{
    CRibbon *self = this;
    int i = 0;
    int count = self->mNumBits;
    if (count > 0)
    {
        do
        {
            self->mBits[i]->SetTint(r, g, b);
            count = self->mNumBits;
            i++;
        } while (i < count);
    }
}

/**
 * SetScale__7CRibboni
 *
 * Address:   0x800399D8
 * Mangled:   SetScale__7CRibboni
 * Demangled: CRibbon::SetScale(int)
 *            Loops over children array at 0x5c, count at 0x48, calling SetScale__7CFT4BitUs
 *
 * @param Scale  Scale assigned to each child ribbon bit.
 */
void CRibbon::SetScale(int Scale)
{
    CRibbon *self = this;
    int i = 0;
    int count = self->mNumBits;
    if (count > 0)
    {
        do
        {
            self->mBits[i]->SetScale((u16)Scale);
            count = self->mNumBits;
            i++;
        } while (i < count);
    }
}

/**
 * SetPos__7CRibbonR7CVector
 *
 * Address:   0x80039A48
 * Mangled:   SetPos__7CRibbonR7CVector
 * Demangled: CRibbon::SetPos(CVector &)
 *            Advances the ribbon sample index and rewrites segment endpoints from ring buffer.
 *
 * @param pos  Position vector.
 */
void CRibbon::SetPos(CVector &pos)
{
    CRibbon *self = this;
    int next = self->mCurrent + 1;
    self->mCurrent = next;
    if (next == self->mNumPoints)
    {
        self->mCurrent = 0;
    }

    {
        int cur0 = self->mCurrent;
        CVector *slot = (CVector *)(cur0 * 12 + (int)self->mPoints);
        *slot = pos;
    }

    int numBits = self->mNumBits;
    int current = self->mCurrent;
    int i = numBits - 1;
    if (i >= 0)
    {
        do
        {
            CRibbonBit *bitB = self->mBits[i];
            CVector *srcB = (CVector *)(current * 12 + (int)self->mPoints);
            bitB->mEndPosB = *srcB;

            current = current - self->mPointsPerBit;
            if (current < 0)
            {
                current = current + self->mNumPoints;
            }
            CRibbonBit *bitA = self->mBits[i];
            CVector *srcA = (CVector *)(current * 12 + (int)self->mPoints);
            bitA->mEndPosA = *srcA;
            i--;
        } while (i >= 0);
    }
}

/**
 * __11CSmokeTrailR7CVectoriiii
 *
 * Address:   0x80039B4C
 * Mangled:   __11CSmokeTrailR7CVectoriiii
 * Demangled: CSmokeTrail::(CVector &, int, int, int, int)
 *            Builds a smoke trail from a ribbon and linearly ramps initial per-segment tint.
 *
 * @param pos  Position vector.
 * @param numbits  Number of child ribbon segments.
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 */
CSmokeTrail::CSmokeTrail(
    CVector &pos, int numbits, int r, int g, int b)
    : CRibbon(pos, numbits, 2,
              Bit_RibbonName, Bit_RibbonName, 0x190, 1)
{
    CSmokeTrail *self = this;
    int count = self->mNumBits;
    int decr = r / count;
    int decg = g / count;
    int decb = b / count;
    int i = 0;
    self->_vf = (void **)&__vt_11CSmokeTrail;
    if (count > 0)
    {
        do
        {
            int k = i + 1;
            int rem = count - k;
            self->mBits[i]->SetTint(
                (char)(r - rem * decr),
                (char)(g - rem * decg),
                (char)(b - rem * decb));
            self->mBits[i]->SetTransDecay(8);
            count = self->mNumBits;
            i = k;
        } while (i < count);
    }
}

/**
 * _._11CSmokeTrail
 *
 * Address:   0x80039CA4
 * Mangled:   _._11CSmokeTrail
 * Demangled: CSmokeTrail::~CSmokeTrail()
 *
 * @param _this  Object being operated on.
 * @param flag  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
CSmokeTrail::~CSmokeTrail()
{
    _vf = (void **)&__vt_11CSmokeTrail;
}

/**
 * Move__11CSmokeTrail
 *
 * Address:   0x80039CF4
 * Mangled:   Move__11CSmokeTrail
 * Demangled: CSmokeTrail::Move(void)
 *            Loops over children calling Fade, dies if all children fully faded
 *
 */
void CSmokeTrail::Move()
{
    CSmokeTrail *self = this;
    int active = self->mFadeAway;
    int die = 1;
    if (active != 0)
    {
        int count = self->mNumBits;
        int i = 0;
        if (count > 0)
        {
            do
            {
                CRibbonBit *child = self->mBits[i];
                int result = child->Fade(0);
                if (result == 0)
                {
                    die = 0;
                }
                count = self->mNumBits;
                i++;
            } while (i < count);
        }
        if (die != 0)
        {
            self->Die();
        }
    }
}

/**
 * __9CGlassBitRC7CVectorT1iUcUcUciii
 *
 * Address: 0x80039D8C
 *          Spawns a glass fragment with randomised initial triangle offsets and color state.
 * Mangled: __9CGlassBitRC7CVectorT1iUcUcUciii
 *
 * @param self  Object being operated on.
 * @param Pos  Position vector.
 * @param Vel  Initial fragment velocity.
 * @param GroundY  Ground height where the fragment stops falling.
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 * @param dx  Random X extent around the origin.
 * @param dy  Random Y extent around the origin.
 * @param dz  Random Z extent around the origin.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
CGlassBit *CGlassBit_Construct(
    CGlassBit *self, const CVector &Pos, const CVector &Vel, int GroundY,
    u8 r, u8 g, u8 b, int dx, int dy, int dz)
{
    CBit_ctor(self);
    self->_vf = (void **)&__vt_9CGlassBit;
    self->AttachTo(&GlassList);
    int u = dx * 2 + 1;
    int v = dy * 2 + 1;
    int w = dz * 2 + 1;

    self->mPos = Pos;

    int rnd = Rnd(u);
    self->mPosA.vx = Pos.vx + (rnd - dx) * 0x1000;
    rnd = Rnd(v);
    self->mPosA.vy = Pos.vy + (rnd - dy) * 0x1000;
    rnd = Rnd(w);
    self->mPosA.vz = Pos.vz + (rnd - dz) * 0x1000;

    rnd = Rnd(u);
    self->mPosB.vx = Pos.vx + (rnd - dx) * 0x1000;
    rnd = Rnd(v);
    self->mPosB.vy = Pos.vy + (rnd - dy) * 0x1000;
    rnd = Rnd(w);
    self->mPosB.vz = Pos.vz + (rnd - dz) * 0x1000;

    rnd = Rnd(u);
    self->mPosC.vx = Pos.vx + (rnd - dx) * 0x1000;
    rnd = Rnd(v);
    self->mPosC.vy = Pos.vy + (rnd - dy) * 0x1000;
    rnd = Rnd(w);
    self->mPosC.vz = Pos.vz + (rnd - dz) * 0x1000;

    self->mVel = Vel;
    self->mGroundY = GroundY;
    self->mDefaultR = r >> 2;
    self->mDefaultG = g >> 2;
    self->mDefaultB = b >> 2;
    self->mR = r >> 2;
    self->mG = g >> 2;
    self->mB = b >> 2;
    self->mLifetime = (u16)(Rnd(30) + 30);
    self->mFadeRate = 4;
    return self;
}

/**
 * _._9CGlassBit
 *
 * Address:   0x80039FD0
 * Mangled:   _._9CGlassBit
 * Demangled: CGlassBit::~CGlassBit()
 *
 * @param _this  Object being operated on.
 * @param flag  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
CGlassBit::~CGlassBit()
{
    _vf = (void **)&__vt_9CGlassBit;
    DeleteFrom(&GlassList);
}

/**
 * Move__9CGlassBit
 *
 * Address:   0x8003A02C
 * Mangled:   Move__9CGlassBit
 * Demangled: CGlassBit::Move(void)
 *            Purpose: Steps glass fragment position/velocity, handles ground bounce and fade lifecycle.
 *
 */
void CGlassBit::Move()
{
    CGlassBit *glass = this;
    int vx = glass->mVel.vx;
    int vy = glass->mVel.vy;
    int vz = glass->mVel.vz;

    glass->mPosA.vx += vx;
    glass->mPosA.vy += vy;
    glass->mPosA.vz += vz;
    glass->mPosB.vx += vx;
    glass->mPosB.vy += vy;
    glass->mPosB.vz += vz;
    glass->mPosC.vx += vx;
    glass->mPosC.vy += vy;
    glass->mPosC.vz += vz;

    vy += 10000;
    if (glass->mGroundY < glass->mPosA.vy)
    {
        if (glass->mAge < 2)
        {
            glass->mPosA.vy = glass->mGroundY - (Rnd(0x32) << 0xC);
            glass->mPosB.vy = glass->mGroundY - (Rnd(0x32) << 0xC);
            vy = -vy;
            vy = vy >> 2;
            glass->mPosC.vy = glass->mGroundY - (Rnd(0x32) << 0xC);
            *(u16 *)&glass->mAge = *(u16 *)&glass->mAge + 1;
        }
        else
        {
            vy = 0;
        }
        vx = vx >> 1;
        vz = vz >> 1;
    }
    glass->mVel.vx = vx;
    glass->mVel.vy = vy;
    glass->mVel.vz = vz;

    if ((glass->mAge < 2) && (Rnd(5) == 0))
    {
        glass->mR = 0x30;
        glass->mG = 0x30;
        glass->mB = 0x40;
    }
    else
    {
        glass->mR = glass->mDefaultR;
        glass->mG = glass->mDefaultG;
        glass->mB = glass->mDefaultB;
    }

    int life = glass->mLifetime;
    if ((life == 0) || (life = life - 1, glass->mLifetime = (u16)life, (u16)life == 0))
    {
        glass->mDefaultR = glass->mDefaultR > glass->mFadeRate
            ? glass->mDefaultR - glass->mFadeRate
            : 0;
        glass->mDefaultG = glass->mDefaultG > glass->mFadeRate
            ? glass->mDefaultG - glass->mFadeRate
            : 0;
        glass->mDefaultB = glass->mDefaultB > glass->mFadeRate
            ? glass->mDefaultB - glass->mFadeRate
            : 0;
    }
    if ((glass->mDefaultR | glass->mDefaultG | glass->mDefaultB) == 0)
    {
        glass->Die();
    }
    return;
}

/**
 * MoveList__FP4CBit
 *
 * Address:   0x8003A25C
 * Mangled:   MoveList__FP4CBit
 * Demangled: MoveList(CBit *)
 *
 * @param pBit  Bit to operate on.
 */
void MoveList(CBit *pBit)
{
    while (pBit != 0)
    {
        if (pBit->mDead == 0)
        {
            CBitVTable *vtable = (CBitVTable *)pBit->_vf;
            vtable->Move(pBit);
        }
        pBit = pBit->mNext;
    }
}

/**
 * Bit_Move__Fv
 *
 * Address:   0x8003A2B8
 * Mangled:   Bit_Move__Fv
 * Demangled: Bit_Move(void)
 *
 */
void Bit_Move(void)
{
    MoveList((CBit *)PixelList);
    MoveList((CBit *)FlatBitList);
    MoveList((CBit *)GlassList);
    MoveList((CBit *)GenPolyList);
    MoveList((CBit *)ChunkBitList);
    MoveList((CBit *)QuadBitList);
    MoveList((CBit *)GlowList);
    MoveList((CBit *)NonRenderedBitList);
    MoveList((CBit *)Linked2EndedBitList);
    MoveList((CBit *)PolyLineList);
    MoveList((CBit *)GPolyLineList);
    MoveList((CBit *)GLineList);
    MoveList((CBit *)SpecialDisplayList);
}

/**
 * Bit_Display__Fv
 *
 * Address:   0x8003A36C
 * Mangled:   Bit_Display__Fv
 * Demangled: Bit_Display(void)
 *            Purpose: Project and submit every active transient-effect list to
 *            the current viewport ordering table.
 */
void Bit_Display(void)
{
    CVector Temp;
    CVector camera_pos;
    int GTEZedLimit;
    CSpecialDisplay *pSpecial;
    CGlow *pGlow;
    CGPolyLine *pGPolyLine;
    CPolyLine *pPolyLine;
    CFlatBit *pFlatBit;
    CGlassBit *pGlass;
    CGLine *pGLine;
    CQuadBit *pQuad;
    CChunkBit *p_chunk;
    CPixel *pPixel;

    gte_SetRotMatrix(&MikeCamera[Camera_CurrentMikeCamera].View);
    gte_ZeroTransVector();

    camera_pos.vx = MikeCamera[Camera_CurrentMikeCamera].Position.vx;
    camera_pos.vy = MikeCamera[Camera_CurrentMikeCamera].Position.vy;
    camera_pos.vz = MikeCamera[Camera_CurrentMikeCamera].Position.vz;
    GTEZedLimit = pCurrentViewport->Zoom >> 1;

    for (pSpecial = SpecialDisplayList;
         pSpecial != 0;
         pSpecial = (CSpecialDisplay *)pSpecial->mNext)
    {
        CBitVTable *vtable = (CBitVTable *)pSpecial->_vf;
        vtable->Display(pSpecial);
    }

    for (pGlow = GlowList;
         pGlow != 0;
         pGlow = (CGlow *)pGlow->mNext)
    {
        unsigned int y0x0;
        VECTOR RotCoords;
        CVector Centre = (pGlow->mPos >> 12) - camera_pos;
        gte_ldlv0(&Centre);
        gte_rtps();

        DR_TPAGE *ptpage = (DR_TPAGE *)pPoly;
        unsigned char *newppoly = (unsigned char *)(ptpage + 1);
        if (PolyBufferEnd < newppoly)
            return;
        pPoly = (u32 *)newppoly;
        u32 a = StoredTPagePrim.tag;
        u32 b = StoredTPagePrim.code[0];
        ptpage->tag = a;
        ptpage->code[0] = b;

        int n = pGlow->mNumSections;
        int Step = pGlow->mStepAngle;
        int Ang = pGlow->mAngle;
        SSection *pSection = pGlow->mpSections;
        short CentreX;
        short CentreY;
        gte_stlvnl(&RotCoords);
        if ((u32)RotCoords.vz >= 20001)
            continue;

        int RGBFade = 0;
        if (RotCoords.vz < 700)
            RGBFade = (RotCoords.vz * 255) / 700 + 1;

        if (RotCoords.vz < GTEZedLimit)
        {
            CentreX = pCurrentViewport->xC +
                      pCurrentViewport->Zoom * RotCoords.vx / RotCoords.vz;
            if (CentreX >= 1024)
                CentreX = 1023;
            else if (CentreX < -1024)
                CentreX = -1024;

            CentreY = pCurrentViewport->yC +
                      pCurrentViewport->Zoom * RotCoords.vy / RotCoords.vz;
            if (CentreY >= 1024)
                CentreY = 1023;
            else if (CentreY < -1024)
                CentreY = -1024;
        }
        else
        {
            gte_stsxy(&y0x0);
            CentreX = (s16)y0x0;
            CentreY = (s16)(y0x0 >> 16);
        }

        if (CentreX == -1024 || CentreX == 1023 ||
            CentreY == -1024 || CentreY == 1023)
            continue;

        u32 *OTPos;
        DVECTOR *pBufA;
        DVECTOR *pBufB;
        int RadA;
        int x;
        int y;
        int Rad;

        OTPos = (u32 *)((RotCoords.vz & 0x3ffc) +
                        (int)pDoubleBuffer->ViewportOT);
        pBufA = (DVECTOR *)getScratchAddr(0);
        pBufB = (DVECTOR *)getScratchAddr(1) + pGlow->mNumFringes;
        Rad = pSection[n - 1].Radius;
        int MSin = Sine(Ang) * 400 / 256;
        int MCos = Cosine(Ang);
        RadA = (Rad * 400) / (unsigned int)RotCoords.vz;
        x = CentreX + RadA * MSin / 4096;
        y = CentreY - RadA * MCos / 4096;
        if (x > 612)
            x = 612;
        if (x < -100)
            x = -100;
        if (y > 340)
            y = 340;
        if (y < -100)
            y = -100;
        pBufA[0].vx = x;
        pBufA[0].vy = y;

        SFringeQuad *pFringe = pGlow->mpFringes + n - 1;
        for (int f = 0; f < pGlow->mNumFringes; ++f)
        {
            Rad += pFringe->Width;
            RadA = (Rad * 400) / RotCoords.vz;
            x = CentreX + RadA * MSin / 4096;
            y = CentreY - RadA * MCos / 4096;
            if (x > 612)
                x = 612;
            if (x < -100)
                x = -100;
            if (y > 340)
                y = 340;
            if (y < -100)
                y = -100;
            pBufA[f + 1].vx = x;
            pBufA[f + 1].vy = y;
            pFringe += n;
        }

        Ang += Step;
        for (int i = 0; i < n; ++i)
        {
            int DoDisplay = (pGlow->mMask >> i) & 1;
            if (i == n - 1)
                Ang = pGlow->mAngle;

            RadA = (pSection->Radius * 400) /
                   (unsigned int)RotCoords.vz;
            MSin = Sine(Ang) * 400 / 256;
            MCos = Cosine(Ang);
            x = CentreX + RadA * MSin / 4096;
            y = CentreY - RadA * MCos / 4096;
            if (x > 612)
                x = 612;
            if (x < -100)
                x = -100;
            if (y > 340)
                y = 340;
            if (y < -100)
                y = -100;
            pBufB[0].vx = x;
            pBufB[0].vy = y;

            u32 b = pGlow->mSkipTriangles;
            if (!b)
            {
                if (DoDisplay)
                {
                    POLY_G3 *aGou = (POLY_G3 *)pPoly;
                    unsigned char *newppoly = (unsigned char *)(aGou + 1);
                    if (PolyBufferEnd < newppoly)
                        return;
                    pPoly = (u32 *)newppoly;
                    aGou->tag = 0x06000000;
                    *(u32 *)&aGou->r0 = pGlow->mCentreCodeBGR;
                    *(u32 *)&aGou->r1 = pSection->PadBGR;
                    *(u32 *)&aGou->r2 = pSection->PadBGR;
                    if (RGBFade != 0)
                    {
                        aGou->r0 = aGou->r0 * RGBFade >> 8;
                        aGou->g0 = aGou->g0 * RGBFade >> 8;
                        aGou->b0 = aGou->b0 * RGBFade >> 8;
                        aGou->r1 = aGou->r1 * RGBFade >> 8;
                        aGou->g1 = aGou->g1 * RGBFade >> 8;
                        aGou->b1 = aGou->b1 * RGBFade >> 8;
                        aGou->r2 = aGou->r2 * RGBFade >> 8;
                        aGou->g2 = aGou->g2 * RGBFade >> 8;
                        aGou->b2 = aGou->b2 * RGBFade >> 8;
                    }
                    setXY0(aGou, CentreX, CentreY);
                    aGou->x1 = pBufA[0].vx;
                    aGou->y1 = pBufA[0].vy;
                    aGou->x2 = pBufB[0].vx;
                    aGou->y2 = pBufB[0].vy;
                    addPrim(OTPos, aGou);
                }
            }

            Rad = pSection->Radius;
            pFringe = pGlow->mpFringes + i;
            for (int f = 0; f < pGlow->mNumFringes; ++f)
            {
                Rad += pFringe->Width;
                RadA = (Rad * 400) / RotCoords.vz;
                x = CentreX + RadA * MSin / 4096;
                y = CentreY - RadA * MCos / 4096;
                if (x > 612)
                    x = 612;
                if (x < -100)
                    x = -100;
                if (y > 340)
                    y = 340;
                if (y < -100)
                    y = -100;
                pBufB[f + 1].vx = x;
                pBufB[f + 1].vy = y;

                if (DoDisplay)
                {
                    POLY_G4 *aGou = (POLY_G4 *)pPoly;
                    unsigned char *newppoly = (unsigned char *)(aGou + 1);
                    if (PolyBufferEnd < newppoly)
                        return;
                    pPoly = (u32 *)newppoly;
                    aGou->tag = 0x08000000;
                    u32 CodeBGR = pFringe->CodeBGR;
                    *(u32 *)&aGou->r0 = CodeBGR;
                    *(u32 *)&aGou->r1 = CodeBGR;
                    if (f != 0)
                    {
                        u32 PadBGR = (pFringe - n)->CodeBGR;
                        *(u32 *)&aGou->r2 = PadBGR;
                        *(u32 *)&aGou->r3 = PadBGR;
                    }
                    else
                    {
                        *(u32 *)&aGou->r2 = pSection->PadBGR;
                        *(u32 *)&aGou->r3 = pSection->PadBGR;
                    }
                    if (RGBFade != 0)
                    {
                        aGou->r0 = aGou->r0 * RGBFade >> 8;
                        aGou->g0 = aGou->g0 * RGBFade >> 8;
                        aGou->b0 = aGou->b0 * RGBFade >> 8;
                        aGou->r1 = aGou->r1 * RGBFade >> 8;
                        aGou->g1 = aGou->g1 * RGBFade >> 8;
                        aGou->b1 = aGou->b1 * RGBFade >> 8;
                        aGou->r2 = aGou->r2 * RGBFade >> 8;
                        aGou->g2 = aGou->g2 * RGBFade >> 8;
                        aGou->b2 = aGou->b2 * RGBFade >> 8;
                        aGou->r3 = aGou->r3 * RGBFade >> 8;
                        aGou->g3 = aGou->g3 * RGBFade >> 8;
                        aGou->b3 = aGou->b3 * RGBFade >> 8;
                    }
                    aGou->x2 = pBufA[f].vx;
                    aGou->y2 = pBufA[f].vy;
                    aGou->x3 = pBufB[f].vx;
                    aGou->y3 = pBufB[f].vy;
                    setXY0(aGou, pBufA[f + 1].vx, pBufA[f + 1].vy);
                    aGou->x1 = pBufB[f + 1].vx;
                    aGou->y1 = pBufB[f + 1].vy;
                    addPrim(OTPos, aGou);
                }
                pFringe += n;
            }

            DVECTOR *tempBuf = pBufA;
            pBufA = pBufB;
            pBufB = tempBuf;
            Ang += Step;
            ++pSection;
        }

        addPrim(OTPos, ptpage);
    }

    SClipLineBuf *clip_buffer = (SClipLineBuf *)getScratchAddr(0);
    for (pGPolyLine = GPolyLineList;
         pGPolyLine != 0;
         pGPolyLine = (CGPolyLine *)pGPolyLine->mNext)
    {
        u32 LastBGR;
        SLineSeg *pSeg;

        LastBGR = *(u32 *)&pGPolyLine->mStartR;
        clip_buffer->Input[0].vx = pGPolyLine->mStart.vx;
        clip_buffer->Input[0].vy = pGPolyLine->mStart.vy;
        clip_buffer->Input[0].vz = pGPolyLine->mStart.vz;
        pSeg = pGPolyLine->mSegs;

        for (int i = 0; i < pGPolyLine->mNumSegs; ++pSeg, ++i)
        {
            u32 NewBGR = *(u32 *)&pSeg->r;
            clip_buffer->Input[1].vx = pSeg->End.vx;
            clip_buffer->Input[1].vy = pSeg->End.vy;
            clip_buffer->Input[1].vz = pSeg->End.vz;
            int sort = M3dAsm_ClipLine(clip_buffer);
            if (sort >= 0)
            {
                DR_TPAGE *ptpage = (DR_TPAGE *)pPoly;
                LINE_G2 *aline = (LINE_G2 *)(ptpage + 1);
                unsigned char *newppoly = (unsigned char *)(aline + 1);
                if (PolyBufferEnd < newppoly)
                    return;
                pPoly = (u32 *)newppoly;

                aline->tag = 0x04000000;
                *(u32 *)&aline->r0 = LastBGR;
                *(u32 *)&aline->r1 = NewBGR;

                u32 *otpos;
                if (pGPolyLine->mForcedPriority != 0)
                    otpos = pDoubleBuffer->ViewportOT +
                            pGPolyLine->mForcedPriority;
                else
                    otpos = pDoubleBuffer->ViewportOT + sort;
                *(u32 *)&aline->x0 = clip_buffer->Output[0];
                *(u32 *)&aline->x1 = clip_buffer->Output[1];
                addPrim(otpos, aline);

                if (LastBGR & 0x02000000)
                {
                    u32 a = StoredTPagePrim.tag;
                    u32 b = StoredTPagePrim.code[0];
                    ptpage->tag = a;
                    ptpage->code[0] = b;
                    addPrim(otpos, ptpage);
                }
            }
            LastBGR = NewBGR;
            clip_buffer->Input[0].vx = clip_buffer->Input[1].vx;
            clip_buffer->Input[0].vy = clip_buffer->Input[1].vy;
            clip_buffer->Input[0].vz = clip_buffer->Input[1].vz;
        }
    }

    for (pPolyLine = PolyLineList;
         pPolyLine != 0;
         pPolyLine = (CPolyLine *)pPolyLine->mNext)
    {
        clip_buffer->Input[0].vx = pPolyLine->mStart.vx;
        clip_buffer->Input[0].vy = pPolyLine->mStart.vy;
        clip_buffer->Input[0].vz = pPolyLine->mStart.vz;
        SLineSeg *pSeg = pPolyLine->mSegs;

        for (int i = 0; i < pPolyLine->mNumSegs; ++pSeg, ++i)
        {
            u32 CodeBGR = *(u32 *)&pSeg->r;
            clip_buffer->Input[1].vx = pSeg->End.vx;
            clip_buffer->Input[1].vy = pSeg->End.vy;
            clip_buffer->Input[1].vz = pSeg->End.vz;
            int sort = M3dAsm_ClipLine(clip_buffer);
            if (sort >= 0)
            {
                DR_TPAGE *ptpage = (DR_TPAGE *)pPoly;
                LINE_F2 *aline = (LINE_F2 *)(ptpage + 1);
                unsigned char *newppoly = (unsigned char *)(aline + 1);
                if (PolyBufferEnd < newppoly)
                    return;
                pPoly = (u32 *)newppoly;

                aline->tag = 0x03000000;
                *(u32 *)&aline->r0 = CodeBGR;

                u32 *otpos;
                if (pPolyLine->mForcedPriority != 0)
                    otpos = pDoubleBuffer->ViewportOT + pPolyLine->mForcedPriority;
                else
                    otpos = pDoubleBuffer->ViewportOT + sort;
                *(u32 *)&aline->x0 = clip_buffer->Output[0];
                *(u32 *)&aline->x1 = clip_buffer->Output[1];
                addPrim(otpos, aline);

                if (CodeBGR & 0x02000000)
                {
                    u32 a = StoredTPagePrim.tag;
                    u32 b = StoredTPagePrim.code[0];
                    ptpage->tag = a;
                    ptpage->code[0] = b;
                    addPrim(otpos, ptpage);
                }
            }
            clip_buffer->Input[0].vx = clip_buffer->Input[1].vx;
            clip_buffer->Input[0].vy = clip_buffer->Input[1].vy;
            clip_buffer->Input[0].vz = clip_buffer->Input[1].vz;
        }
    }

    for (pFlatBit = FlatBitList;
         pFlatBit != 0;
         pFlatBit = (CFlatBit *)pFlatBit->mNext)
    {
        long zed;
        unsigned int y0x0;
        POLY_FT4 *apoly;
        unsigned char *newppoly;
        unsigned int Scale;
        Texture *stored;
        unsigned int foo1;
        unsigned int foo2;
        unsigned int foo3;
        unsigned short foo4;
        short Angle;
        unsigned int *ptr;
        unsigned char Flipped;
        int OffX;
        int OffY;
        unsigned int Width;
        unsigned int Height;
        unsigned int x_post_scale;
        unsigned int y_post_scale;

        Temp.vx = (pFlatBit->mPos.vx >> 12) - camera_pos.vx;
        Temp.vy = (pFlatBit->mPos.vy >> 12) - camera_pos.vy;
        Temp.vz = (pFlatBit->mPos.vz >> 12) - camera_pos.vz;
        gte_ldlv0(&Temp);
        gte_rtps();

        apoly = (POLY_FT4 *)pPoly;
        newppoly = (unsigned char *)(apoly + 1);
        if ((unsigned char *)PolyBufferEnd < newppoly)
            return;

        pPoly = (u32 *)newppoly;
        apoly->tag = 0x09000000;
        *(u32 *)&apoly->r0 = pFlatBit->mCodeBGR;
        gte_stlvnl2(&zed);
        if (zed < GTEZedLimit || zed > pCurrentViewport->Yon)
        {
            pPoly = (u32 *)apoly;
            continue;
        }

        Scale = (unsigned int)pFlatBit->mScale;
        Scale = (Scale << 7) / (unsigned int)zed;
        if (Scale > 0x200 && !(pFlatBit->mBitFlags & 8))
        {
            pPoly = (u32 *)apoly;
            continue;
        }

        gte_stsxy(&y0x0);
        stored = pFlatBit->mpPSXFrame->pTexture;
        foo1 = *(u32 *)&stored->u0;
        foo2 = *(u32 *)&stored->u1;
        foo3 = *(u32 *)&stored->u2;
        foo4 = *(u16 *)&stored->u3;
        if (pFlatBit->mCLUT != 0)
            foo1 = (foo1 & 0xffff) | ((u32)pFlatBit->mCLUT << 16);
        *(u32 *)&apoly->u0 = foo1;
        *(u32 *)&apoly->u1 = (foo2 & 0xff9fffff) |
                             ((u32)pFlatBit->mSemiTransparencyRate << 16);
        *(u32 *)&apoly->u2 = foo3;
        *(u16 *)&apoly->u3 = foo4;

        Angle = pFlatBit->mAngle;
        if (Scale > 0xff || Angle != 0)
        {
            apoly->u1--;
            apoly->v2--;
            apoly->u3--;
            apoly->v3--;
        }

        ptr = (unsigned int *)&apoly->x0;
        Width = pFlatBit->mpPSXFrame->Width;
        Height = pFlatBit->mpPSXFrame->Height;
        Flipped = pFlatBit->mBitFlags & 1;
        OffX = (signed char)pFlatBit->mpPSXFrame->OffX;
        OffY = (signed char)pFlatBit->mpPSXFrame->OffY;
        x_post_scale = Scale * (pFlatBit->mPostScale & 0xffff);
        y_post_scale = Scale * (pFlatBit->mPostScale >> 16);
        if (Angle == 0)
        {
            OffX = ((int)(Scale * OffX) << 9) >> 16;
            int Y1 = (Scale * OffY >> 7) << 16;
            y0x0 += Y1 + OffX;
            Width = Scale * Width >> 7;
            Height = (Scale * Height >> 7) << 16;
            if (Width < 5)
                Width = 2;
            if (Height < 0x50000)
                Height = 0x10000;

            if (!Flipped)
            {
                ptr[0] = y0x0;
                ptr[2] = y0x0 + Width;
                ptr[4] = y0x0 + Height;
                ptr[6] = y0x0 + Width + Height;
            }
            else
            {
                ptr[2] = y0x0;
                ptr[0] = y0x0 + Width;
                ptr[6] = y0x0 + Height;
                ptr[4] = y0x0 + Width + Height;
            }
        }
        else
        {
            int sin = Sine(Angle);
            int cos = Cosine(Angle);
            int xsin = OffX * sin;
            int ysin = OffY * sin;
            int xcos = OffX * cos;
            int ycos = OffY * cos;
            int xwsin = xsin + Width * sin;
            int yhsin = ysin + Height * sin;
            int xwcos = xcos + Width * cos;
            int yhcos = ycos + Height * cos;
            unsigned int x_scale = x_post_scale >> 12;
            unsigned int y_scale = y_post_scale >> 12;

            if (!Flipped)
            {
                ptr[0] = y0x0 + (x_scale * (xcos - ysin) >> 19)
                                 + (y_scale * (xsin + ycos) >> 19) * 0x10000;
                ptr[2] = y0x0 + (x_scale * (xwcos - ysin) >> 19)
                                 + (y_scale * (xwsin + ycos) >> 19) * 0x10000;
                ptr[4] = y0x0 + (x_scale * (xcos - yhsin) >> 19)
                                 + (y_scale * (xsin + yhcos) >> 19) * 0x10000;
                ptr[6] = y0x0 + (x_scale * (xwcos - yhsin) >> 19)
                                 + (y_scale * (xwsin + yhcos) >> 19) * 0x10000;
            }
            else
            {
                ptr[2] = y0x0 + (x_scale * (xcos - ysin) >> 19)
                                 + (y_scale * (xsin + ycos) >> 19) * 0x10000;
                ptr[0] = y0x0 + (x_scale * (xwcos - ysin) >> 19)
                                 + (y_scale * (xwsin + ycos) >> 19) * 0x10000;
                ptr[6] = y0x0 + (x_scale * (xcos - yhsin) >> 19)
                                 + (y_scale * (xsin + yhcos) >> 19) * 0x10000;
                ptr[4] = y0x0 + (x_scale * (xwcos - yhsin) >> 19)
                                 + (y_scale * (xwsin + yhcos) >> 19) * 0x10000;
            }
        }

        if (pFlatBit->mForcedPriority != 0)
        {
            addPrim(pDoubleBuffer->ViewportOT + pFlatBit->mForcedPriority,
                    apoly);
        }
        else
        {
            addPrim((u32 *)(((zed - pFlatBit->mFrigDeltaZ) & 0x3ffc) +
                            (int)pDoubleBuffer->ViewportOT),
                    apoly);
        }
    }

    for (pGlass = GlassList;
         pGlass != 0;
         pGlass = (CGlassBit *)pGlass->mNext)
    {
        SVECTOR STemp;
        long zed;
        POLY_F3 *aGou;
        DR_TPAGE *ptpage;
        unsigned char *newppoly;
        u32 *OTPos;

        STemp.vx = (pGlass->mPosA.vx >> 12) - (s16)camera_pos.vx;
        STemp.vy = (pGlass->mPosA.vy >> 12) - (s16)camera_pos.vy;
        STemp.vz = (pGlass->mPosA.vz >> 12) - (s16)camera_pos.vz;
        gte_ldv0(&STemp);

        STemp.vx = (pGlass->mPosB.vx >> 12) - (s16)camera_pos.vx;
        STemp.vy = (pGlass->mPosB.vy >> 12) - (s16)camera_pos.vy;
        STemp.vz = (pGlass->mPosB.vz >> 12) - (s16)camera_pos.vz;
        gte_ldv1(&STemp);

        STemp.vx = (pGlass->mPosC.vx >> 12) - (s16)camera_pos.vx;
        STemp.vy = (pGlass->mPosC.vy >> 12) - (s16)camera_pos.vy;
        STemp.vz = (pGlass->mPosC.vz >> 12) - (s16)camera_pos.vz;
        gte_ldv2(&STemp);
        gte_rtpt();

        aGou = (POLY_F3 *)pPoly;
        ptpage = (DR_TPAGE *)(aGou + 1);
        newppoly = (unsigned char *)(ptpage + 1);
        if ((unsigned char *)PolyBufferEnd < newppoly)
            return;
        pPoly = (u32 *)newppoly;

        aGou->tag = 0x04000000;
        aGou->code = 0x22;
        aGou->r0 = pGlass->mR;
        aGou->g0 = pGlass->mG;
        aGou->b0 = pGlass->mB;
        u32 a = StoredTPagePrim.tag;
        u32 b = StoredTPagePrim.code[0];
        ptpage->tag = a;
        ptpage->code[0] = b;
        gte_stsxy3(&aGou->x0, &aGou->x1, &aGou->x2);
        gte_stlvnl2(&zed);

        OTPos = (u32 *)((u8 *)pDoubleBuffer->ViewportOT +
                        (zed & 0x3ffc));
        addPrim(OTPos, aGou);
        addPrim(OTPos, ptpage);
    }

    for (pGLine = GLineList;
         pGLine != 0;
         pGLine = (CGLine *)pGLine->mNext)
    {
        unsigned int y0x01;
        unsigned int y0x02;
        long zed;
        long zed2;

        Temp = (pGLine->mStart >> 12) - camera_pos;
        gte_ldlv0(&Temp);
        gte_rtps();
        gte_stlvnl2(&zed);

        if (zed >= pCurrentViewport->Hither && zed <= pCurrentViewport->Yon)
        {
            gte_stsxy(&y0x01);
            Temp = (pGLine->mEnd >> 12) - camera_pos;
            gte_ldlv0(&Temp);
            gte_rtps();
            gte_stlvnl2(&zed2);

            if (zed2 >= pCurrentViewport->Hither &&
                zed2 <= pCurrentViewport->Yon)
            {
                gte_stsxy(&y0x02);
                LINE_G2 *aline = (LINE_G2 *)pPoly;
                unsigned char *newppoly = (unsigned char *)(aline + 1);
                if (PolyBufferEnd < newppoly)
                    return;
                pPoly = (u32 *)newppoly;

                aline->tag = 0x04000000;
                *(u32 *)&aline->r0 = pGLine->mCodeBGR0;
                *(u32 *)&aline->r1 = pGLine->mPadBGR1;
                *(u32 *)&aline->x0 = y0x01;
                *(u32 *)&aline->x1 = y0x02;
                if (zed2 < zed)
                    zed = zed2;
                addPrim((u32 *)((zed & 0x3ffc) +
                                (int)pDoubleBuffer->ViewportOT),
                        aline);
            }
        }
    }

    for (pQuad = QuadBitList;
         pQuad != 0;
         pQuad = (CQuadBit *)pQuad->mNext)
    {
        long ZedB;
        long ZedC;
        long ZedD;
        long MaxZed;
        Texture *stored;
        POLY_FT4 *apoly;
        unsigned char *newppoly;
        unsigned int foo1;
        unsigned int foo2;
        unsigned int foo3;
        unsigned short foo4;

        if (pQuad->mpTexture == 0)
            continue;

        Temp = (pQuad->mPos >> 12) - camera_pos;
        gte_ldlv0(&Temp);
        gte_rtps();

        stored = pQuad->mpTexture;
        apoly = (POLY_FT4 *)pPoly;
        newppoly = (unsigned char *)(apoly + 1);
        if ((unsigned char *)PolyBufferEnd < newppoly)
            return;
        pPoly = (u32 *)newppoly;
        gte_stlvnl2(&MaxZed);
        if (MaxZed < pCurrentViewport->Hither || MaxZed > pCurrentViewport->Yon)
        {
            pPoly = (u32 *)apoly;
            continue;
        }
        gte_stsxy(&apoly->x0);

        Temp = (pQuad->mPosB >> 12) - camera_pos;
        gte_ldlv0(&Temp);
        gte_rtps();
        gte_stlvnl2(&ZedB);
        if (ZedB < pCurrentViewport->Hither || ZedB > pCurrentViewport->Yon)
        {
            pPoly = (u32 *)apoly;
            continue;
        }
        gte_stsxy(&apoly->x1);

        Temp = (pQuad->mPosC >> 12) - camera_pos;
        gte_ldlv0(&Temp);
        gte_rtps();

        apoly->tag = 0x09000000;
        *(u32 *)&apoly->r0 = pQuad->mCodeBGR;
        if (ZedB > MaxZed)
            MaxZed = ZedB;

        gte_stlvnl2(&ZedC);
        if (ZedC < pCurrentViewport->Hither || ZedC > pCurrentViewport->Yon)
        {
            pPoly = (u32 *)apoly;
            continue;
        }
        gte_stsxy(&apoly->x2);

        Temp = (pQuad->mPosD >> 12) - camera_pos;
        gte_ldlv0(&Temp);
        gte_rtps();

        foo1 = *(u32 *)&stored->u0;
        foo2 = *(u32 *)&stored->u1;
        foo3 = *(u32 *)&stored->u2;
        foo4 = *(u16 *)&stored->u3;
        *(u32 *)&apoly->u0 = foo1;
        *(u32 *)&apoly->u1 = foo2;
        *(u32 *)&apoly->u2 = foo3;
        *(u16 *)&apoly->u3 = foo4;
        if (pQuad->mFlags & 4)
        {
            apoly->tpage = (apoly->tpage & 0xff9f) | 0x40;
        }
        if (ZedC > MaxZed)
            MaxZed = ZedC;

        gte_stlvnl2(&ZedD);
        if (ZedD < pCurrentViewport->Hither || ZedD > pCurrentViewport->Yon)
        {
            pPoly = (u32 *)apoly;
            continue;
        }
        gte_stsxy(&apoly->x3);

        if (MaxZed < ZedD)
            MaxZed = ZedD;
        addPrim((u32 *)(((MaxZed - pQuad->mFrigDeltaZ) & 0x3ffc) +
                        (int)pDoubleBuffer->ViewportOT),
                apoly);
    }

    for (p_chunk = ChunkBitList;
         p_chunk != 0;
         p_chunk = (CChunkBit *)p_chunk->mNext)
    {
        DR_TWIN *p_overflow2;
        long opz;
        long ZedA;
        long ZedB;
        long ZedC;
        long ZedD;
        long MaxZed = -1;
        unsigned int y0x0A;
        unsigned int y0x0B;
        unsigned int y0x0C;
        unsigned int y0x0D;
        POLY_FT3 *aft3poly;
        DR_TWIN *pWinOff;
        DR_TWIN *pWinOn;
        unsigned char *newppoly;
        u32 *pOTPos;

        p_overflow2 = (DR_TWIN *)pPoly;
        newppoly = (unsigned char *)(p_overflow2 + 2) +
                   4 * sizeof(POLY_FT3);
        if ((unsigned char *)PolyBufferEnd < newppoly)
            return;

        Temp = p_chunk->mWorldPosA - camera_pos;
        gte_ldlv0(&Temp);
        gte_rtps();
        gte_stlvnl2(&ZedA);
        gte_stsxy(&y0x0A);

        Temp = p_chunk->mWorldPosB - camera_pos;
        gte_ldlv0(&Temp);
        gte_rtps();
        if (ZedA < pCurrentViewport->Hither ||
            ZedA > pCurrentViewport->Yon)
            continue;
        if (MaxZed < ZedA)
            MaxZed = ZedA;
        gte_stlvnl2(&ZedB);
        gte_stsxy(&y0x0B);

        Temp = p_chunk->mWorldPosC - camera_pos;
        gte_ldlv0(&Temp);
        gte_rtps();
        if (ZedB < pCurrentViewport->Hither ||
            ZedB > pCurrentViewport->Yon)
            continue;
        if (MaxZed < ZedB)
            MaxZed = ZedB;
        gte_stlvnl2(&ZedC);
        gte_stsxy(&y0x0C);

        Temp = p_chunk->mWorldPosD - camera_pos;
        gte_ldlv0(&Temp);
        gte_rtps();
        if (ZedC < pCurrentViewport->Hither ||
            ZedC > pCurrentViewport->Yon)
            continue;
        if (MaxZed < ZedC)
            MaxZed = ZedC;
        gte_stlvnl2(&ZedD);
        if (ZedD < pCurrentViewport->Hither || ZedD > pCurrentViewport->Yon)
            continue;
        if (MaxZed < ZedD)
            MaxZed = ZedD;
        gte_stsxy(&y0x0D);

        pOTPos = (u32 *)((u8 *)pDoubleBuffer->ViewportOT +
                         (MaxZed & 0x3ffc));

        if (p_chunk->mTexWin != 0)
        {
            pWinOff = (DR_TWIN *)pPoly;
            pWinOn = pWinOff + 1;
            pPoly = (u32 *)(pWinOn + 1);

            RECT win;
            win.x = win.y = win.w = win.h = 0;
            setlen(pWinOff, 2);
            pWinOff->code[0] =
                0xe2000000 |
                (((win.y & 0xff) >> 3) << 15) |
                (((win.x & 0xff) >> 3) << 10) |
                (((~(win.h - 1) & 0xff) >> 3) << 5) |
                ((~(win.w - 1) & 0xff) >> 3);
            pWinOff->code[1] = 0;

            setlen(pWinOn, 2);
            pWinOn->code[0] =
                0xe2000000 |
                (((win.y & 0xff) >> 3) << 15) |
                (((win.x & 0xff) >> 3) << 10) |
                (((~(win.h - 1) & 0xff) >> 3) << 5) |
                ((~(win.w - 1) & 0xff) >> 3);
            pWinOn->code[1] = 0;
            pWinOn->code[0] = p_chunk->mTexWin;

            if (p_chunk->mTexWin != 0)
            {
                addPrim(pOTPos, pWinOff);
            }
        }

        aft3poly = (POLY_FT3 *)pPoly;
        gte_ldsxy3(y0x0A, y0x0B, y0x0C);
        gte_nclip();
        gte_stopz(&opz);
        if (opz <= 0)
        {
            aft3poly->tag = 0x07000000;
            *(u32 *)&aft3poly->r0 = p_chunk->mB0G0R0 | 0x24000000;
            *(u32 *)&aft3poly->u0 = p_chunk->mClutV0U0;
            *(u32 *)&aft3poly->u1 = p_chunk->mTPageV1U1;
            *(u16 *)&aft3poly->u2 = p_chunk->mV2U2;
            *(u32 *)&aft3poly->x0 = y0x0A;
            *(u32 *)&aft3poly->x1 = y0x0B;
            *(u32 *)&aft3poly->x2 = y0x0C;
            addPrim(pOTPos, aft3poly);
            newppoly = (unsigned char *)(aft3poly + 1);
            aft3poly = (POLY_FT3 *)newppoly;
        }

        gte_ldsxy3(y0x0A, y0x0D, y0x0B);
        gte_nclip();
        gte_stopz(&opz);
        if (opz <= 0)
        {
            aft3poly->tag = 0x07000000;
            *(u32 *)&aft3poly->r0 = p_chunk->mB1G1R1 | 0x24000000;
            *(u32 *)&aft3poly->u0 = p_chunk->mClutV0U0;
            *(u32 *)&aft3poly->u1 = p_chunk->mTPageV1U1;
            *(u16 *)&aft3poly->u2 = p_chunk->mV2U2;
            *(u32 *)&aft3poly->x0 = y0x0A;
            *(u32 *)&aft3poly->x1 = y0x0D;
            *(u32 *)&aft3poly->x2 = y0x0B;
            addPrim(pOTPos, aft3poly);
            newppoly = (unsigned char *)(aft3poly + 1);
            aft3poly = (POLY_FT3 *)newppoly;
        }

        gte_ldsxy3(y0x0B, y0x0D, y0x0C);
        gte_nclip();
        gte_stopz(&opz);
        if (opz <= 0)
        {
            aft3poly->tag = 0x07000000;
            *(u32 *)&aft3poly->r0 = p_chunk->mB2G2R2 | 0x24000000;
            *(u32 *)&aft3poly->u0 = p_chunk->mClutV0U0;
            *(u32 *)&aft3poly->u1 = p_chunk->mTPageV1U1;
            *(u16 *)&aft3poly->u2 = p_chunk->mV2U2;
            *(u32 *)&aft3poly->x0 = y0x0B;
            *(u32 *)&aft3poly->x1 = y0x0D;
            *(u32 *)&aft3poly->x2 = y0x0C;
            addPrim(pOTPos, aft3poly);
            newppoly = (unsigned char *)(aft3poly + 1);
            aft3poly = (POLY_FT3 *)newppoly;
        }

        gte_ldsxy3(y0x0C, y0x0D, y0x0A);
        gte_nclip();
        gte_stopz(&opz);
        if (opz <= 0)
        {
            aft3poly->tag = 0x07000000;
            *(u32 *)&aft3poly->r0 = p_chunk->mB3G3R3 | 0x24000000;
            *(u32 *)&aft3poly->u0 = p_chunk->mClutV0U0;
            *(u32 *)&aft3poly->u1 = p_chunk->mTPageV1U1;
            *(u16 *)&aft3poly->u2 = p_chunk->mV2U2;
            *(u32 *)&aft3poly->x0 = y0x0C;
            *(u32 *)&aft3poly->x1 = y0x0D;
            *(u32 *)&aft3poly->x2 = y0x0A;
            addPrim(pOTPos, aft3poly);
            newppoly = (unsigned char *)(aft3poly + 1);
            aft3poly = (POLY_FT3 *)newppoly;
        }
        pPoly = (u32 *)aft3poly;

        if (p_chunk->mTexWin != 0)
        {
            addPrim(pOTPos, pWinOn);
        }
    }

    for (pPixel = PixelList;
         pPixel != 0;
         pPixel = (CPixel *)pPixel->mNext)
    {
        Temp = (pPixel->mPos >> 12) - camera_pos;
        gte_ldlv0(&Temp);
        gte_rtps();
        long zed;

        if ((pPixel->mWidthHeight & 0xf) == 1)
        {
            TILE_1 *atile = (TILE_1 *)pPoly;
            unsigned char *newppoly = (unsigned char *)(atile + 1);
            if (PolyBufferEnd < newppoly)
                return;
            pPoly = (u32 *)newppoly;
            atile->tag = pPixel->tag;
            *(u32 *)&atile->r0 = *(u32 *)&pPixel->r0;

            gte_stlvnl2(&zed);
            if (zed < pCurrentViewport->Hither ||
                zed > pCurrentViewport->Yon)
            {
                pPoly = (u32 *)atile;
                continue;
            }
            gte_stsxy(&atile->x0);
            addPrim((u32 *)(((zed - pPixel->mFrigDeltaZ) & 0x3ffc) +
                            (int)pDoubleBuffer->ViewportOT),
                    atile);
        }
        else
        {
            TILE *atile = (TILE *)pPoly;
            unsigned char *newppoly = (unsigned char *)(atile + 1);
            if (PolyBufferEnd < newppoly)
                return;
            pPoly = (u32 *)newppoly;
            atile->tag = pPixel->tag;
            *(u32 *)&atile->r0 = *(u32 *)&pPixel->r0;
            *(u32 *)&atile->w = pPixel->mWidthHeight;

            gte_stlvnl2(&zed);
            if (zed < pCurrentViewport->Hither ||
                zed > pCurrentViewport->Yon)
            {
                pPoly = (u32 *)atile;
                continue;
            }
            gte_stsxy(&atile->x0);
            addPrim((u32 *)(((zed - pPixel->mFrigDeltaZ) & 0x3ffc) +
                            (int)pDoubleBuffer->ViewportOT),
                    atile);
        }
    }

    int Loop1x = 0;
    int Loop1y = 0;
    int Loop2x = 0;
    int Loop2y = 0;
    int StoredA1x = 0;
    int StoredA1y = 0;
    int StoredA2x = 0;
    int StoredA2y = 0;
    int StoredAx = 0;
    int StoredAy = 0;
    int Broken = 0;
    CLinked2EndedBit *pLinked2EndedBit;
    for (pLinked2EndedBit = Linked2EndedBitList;
         pLinked2EndedBit != 0;
         pLinked2EndedBit =
             (CLinked2EndedBit *)pLinked2EndedBit->mNext)
    {
        Texture *stored;
        long zed;
        unsigned int Width;
        int dA;
        int Ax;
        int Ay;
        int NeedToTransformA;
        POLY_FT4 *apoly;
        unsigned char *newppoly;
        unsigned int foo1;
        unsigned int foo2;
        unsigned int foo3;
        int By0x0;
        int Bx;
        int By;
        int dB;
        int A1x;
        int A1y;
        int A2x;
        int A2y;
        int B1x;
        int B1y;
        int B2x;
        int B2y;
        int dy;
        int dx;
        int d;

        NeedToTransformA =
            ((pLinked2EndedBit->mBitFlags & 0x10) != 0) || Broken;

        if (NeedToTransformA)
        {
            Temp = (pLinked2EndedBit->mEndPosA >> 12) - camera_pos;
            int Ay0x0;
            gte_ldlv0(&Temp);
            gte_rtps();
            stored = pLinked2EndedBit->mpPSXFrame->pTexture;
            Width = pLinked2EndedBit->mpPSXFrame->Width;
            gte_stlvnl2(&zed);
            if (zed < pCurrentViewport->Hither)
            {
                Broken = 1;
                continue;
            }
            if (zed > pCurrentViewport->Yon)
            {
                Broken = 1;
                continue;
            }
            gte_stsxy(&Ay0x0);
            Ax = (s16)Ay0x0;
            Ay = (s16)(Ay0x0 >> 16);
            dA = ((unsigned int)((int)pLinked2EndedBit->mScale * Width) /
                  (unsigned int)zed) >> 1;
            if (dA < 2)
                dA = 2;
        }
        else
        {
            Ax = StoredAx;
            Ay = StoredAy;
            stored = pLinked2EndedBit->mpPSXFrame->pTexture;
            Width = pLinked2EndedBit->mpPSXFrame->Width;
        }

        Temp = (pLinked2EndedBit->mEndPosB >> 12) - camera_pos;
        gte_ldlv0(&Temp);
        gte_rtps();

        apoly = (POLY_FT4 *)pPoly;
        newppoly = (unsigned char *)(apoly + 1);
        if ((unsigned char *)PolyBufferEnd < newppoly)
            return;
        pPoly = (u32 *)newppoly;
        apoly->tag = 0x09000000;
        *(u32 *)&apoly->r0 = pLinked2EndedBit->mCodeBGR;
        foo1 = *(u32 *)&stored->u0;
        foo2 = *(u32 *)&stored->u1;
        foo3 = *(u32 *)&stored->u2;
        unsigned short foo4 = *(u16 *)&stored->u3;
        *(u32 *)&apoly->u0 = foo1;
        *(u32 *)&apoly->u1 = foo2;
        *(u32 *)&apoly->u2 = foo3;
        *(u16 *)&apoly->u3 = foo4;
        apoly->u1--;
        apoly->v2--;
        apoly->u3--;
        apoly->v3--;

        gte_stlvnl2(&zed);
        if (zed < pCurrentViewport->Hither || zed > pCurrentViewport->Yon)
        {
            pPoly = (u32 *)apoly;
            Broken = 1;
            continue;
        }
        gte_stsxy(&By0x0);
        Bx = (s16)By0x0;
        By = (s16)(By0x0 >> 16);
        StoredAx = Bx;
        StoredAy = By;
        dB = ((unsigned int)((int)pLinked2EndedBit->mScale * Width) /
              (unsigned int)zed) >> 1;
        if (dB < 2)
            dB = 2;

        dx = Bx - Ax;
        dy = By - Ay;
        d = M3dMaths_SquareRoot0(dx * dx + dy * dy);
        if (d == 0)
        {
            pPoly = (u32 *)apoly;
            Broken = 1;
            continue;
        }

        if (NeedToTransformA)
        {
            int dydA = dy * dA / d;
            int dxdA = dx * dA / d;
            A1x = Ax + dydA;
            A1y = Ay - dxdA;
            A2x = Ax - dydA;
            A2y = Ay + dxdA;
        }
        else
        {
            A1x = StoredA1x;
            A1y = StoredA1y;
            A2x = StoredA2x;
            A2y = StoredA2y;
        }

        if (pLinked2EndedBit->mBitFlags & 0x10)
        {
            Loop1x = A1x;
            Loop1y = A1y;
            Loop2x = A2x;
            Loop2y = A2y;
        }

        if (pLinked2EndedBit->mBitFlags & 0x20)
        {
            B1x = Loop1x;
            B1y = Loop1y;
            B2x = Loop2x;
            B2y = Loop2y;
        }
        else
        {
            int dydB = dy * dB / d;
            int dxdB = dx * dB / d;
            B1x = Bx + dydB;
            B1y = By - dxdB;
            B2x = Bx - dydB;
            B2y = By + dxdB;
        }
        StoredA1x = B1x;
        StoredA1y = B1y;
        StoredA2x = B2x;
        StoredA2y = B2y;

        if (pLinked2EndedBit->mBitFlags & 2)
        {
            if (!(pLinked2EndedBit->mBitFlags & 1))
            {
                apoly->x0 = B2x;
                apoly->x1 = B1x;
                apoly->y0 = B2y;
                apoly->y1 = B1y;
                apoly->x2 = A2x;
                apoly->x3 = A1x;
                apoly->y2 = A2y;
                apoly->y3 = A1y;
            }
            else
            {
                apoly->x0 = B1x;
                apoly->x1 = B2x;
                apoly->y0 = B1y;
                apoly->y1 = B2y;
                apoly->x2 = A1x;
                apoly->x3 = A2x;
                apoly->y2 = A1y;
                apoly->y3 = A2y;
            }
        }
        else if (pLinked2EndedBit->mBitFlags & 1)
        {
            apoly->x0 = A2x;
            apoly->x1 = B2x;
            apoly->y0 = A2y;
            apoly->y1 = B2y;
            apoly->x2 = A1x;
            apoly->x3 = B1x;
            apoly->y2 = A1y;
            apoly->y3 = B1y;
        }
        else
        {
            apoly->x0 = A1x;
            apoly->x1 = B1x;
            apoly->y0 = A1y;
            apoly->y1 = B1y;
            apoly->x2 = A2x;
            apoly->x3 = B2x;
            apoly->y2 = A2y;
            apoly->y3 = B2y;
        }

        addPrim((u32 *)(((zed - pLinked2EndedBit->mFrigDeltaZ) & 0x3ffc) +
                        (int)pDoubleBuffer->ViewportOT),
                apoly);
        Broken = 0;
    }
}

/**
 * CGouraudRibbon::~CGouraudRibbon()
 *
 * Address:   0x8003D268
 * Mangled:   _._14CGouraudRibbon
 * Demangled: CGouraudRibbon::~CGouraudRibbon()
 *
 */
CGouraudRibbon::~CGouraudRibbon()
{
    _vf = (void **)__vt_14CGouraudRibbon;
    int size = Mem_GetBlockSize(mpPoints);
    int usage = TotalBitUsage;
    SRibbonPoint *ptr2 = mpPoints;
    usage = usage - 4;
    usage = usage - size;
    TotalBitUsage = usage;
    Mem_Delete(ptr2);
}

/**
 * CalcScreenNormal__FP11SCalcBufferRiT1i
 *
 * Address:   0x8003D2E0
 * Mangled:   CalcScreenNormal__FP11SCalcBufferRiT1i
 * Demangled: CalcScreenNormal(SCalcBuffer *, int &, int &, int)
 *            Purpose: Projects a two-point segment into camera space and derives its normalized 2D normal.
 *
 * @param pBuf Pair of projected endpoints used to derive the screen-space normal.
 * @param UnitX Receives the normalized horizontal component.
 * @param UnitY Receives the normalized vertical component.
 * @param ClosenessLimit Minimum approximate segment length required for normalization.
 */
static void CalcScreenNormal(SCalcBuffer *pBuf, int &UnitX, int &UnitY, int ClosenessLimit)
{
    if (pBuf->Bad == 0 && pBuf[1].Bad == 0)
    {
        int xa = pBuf[0].Screen.X;
        int ya = pBuf[0].Screen.Y;
        int xb = pBuf[1].Screen.X;
        int yb = pBuf[1].Screen.Y;
        int dx;
        int dy;
        dx = xb - xa;
        dy = yb - ya;
        int scaledDx = dx * 0x140;
        if (scaledDx < 0)
        {
            scaledDx = scaledDx + 0x1FF;
        }
        dx = scaledDx >> 9;
        UnitX = dy;
        UnitY = -dx;
        if (dx < 0)
        {
            dx = -dx;
        }
        if (dy < 0)
        {
            dy = -dy;
        }
        int l;
        if (dy < dx)
        {
            l = dx + dy / 2;
        }
        else
        {
            l = dy + dx / 2;
        }
        if (ClosenessLimit <= l)
        {
            UnitX = (UnitX << 6) / l;
            UnitY = (UnitY << 6) / l;
            UnitX = (UnitX << 9) / 0x140;
            return;
        }
    }
    UnitY = 0;
    UnitX = 0;
    return;
}

/**
 * Display__14CGouraudRibbon
 *
 * Address:   0x8003D3F8
 * Mangled:   Display__14CGouraudRibbon
 * Demangled: CGouraudRibbon::Display(void)
 *            Purpose: Render Gouraud ribbon points into GTE primitive buffer.
 *
 * @param self Ribbon state and the projected point history to render.
 */
void CGouraudRibbon_Display(CGouraudRibbon *self)
{
    SRibbonPoint *pPoint;
    SCalcBuffer *pBuf;
    int LastNormalX;
    int LastNormalY;
    SCalcBuffer *pLastBuf;
    SRibbonPoint *pLastPoint;
    int x;
    int y;
    int dx;
    int dy;
    int LastTop;
    int LastBottom;

    pBuf = (SCalcBuffer *)0x1f800000;
    {
        int i = 0;
        pPoint = self->mpPoints;
        if (0 < self->mNumPoints)
        {
            do
            {
                pBuf->Zed = Transform(pPoint->Pos, pBuf->Pos);
                if (pBuf->Zed < pCurrentViewport->Hither ||
                    pCurrentViewport->Yon < pBuf->Zed)
                {
                    pBuf->Bad = 1;
                }
                else
                {
                    pBuf->Bad = 0;
                    pBuf->ScrWidth = (pPoint->Width * 400) / pBuf->Zed;
                }
                pBuf++;
                pPoint++;
                i++;
            } while (i < self->mNumPoints);
        }
    }

    pBuf = (SCalcBuffer *)0x1f800000;
    CalcScreenNormal(pBuf, LastNormalX, LastNormalY, 2);
    pBuf->NormalX = LastNormalX;
    pBuf->NormalY = LastNormalY;

    {
        int i = 1;
        pBuf = (SCalcBuffer *)0x1f80001c;
        while (i < self->mNumPoints - 1)
        {
            int TempX;
            int TempY;
            CalcScreenNormal(pBuf, TempX, TempY, 2);
            pBuf->NormalX = (TempX + LastNormalX) / 2;
            pBuf->NormalY = (TempY + LastNormalY) / 2;
            LastNormalX = TempX;
            LastNormalY = TempY;
            pBuf++;
            i++;
        }
    }
    pBuf->NormalX = LastNormalX;
    pBuf->NormalY = LastNormalY;

    pBuf = (SCalcBuffer *)0x1f800000;
    pLastBuf = pBuf;
    x = (short)pBuf->Pos;
    y = pBuf->Pos >> 16;
    dx = (pBuf->NormalX * pBuf->ScrWidth) >> 6;
    dy = (pBuf->NormalY * pBuf->ScrWidth) >> 6;
    LastTop = ((y + dy) << 16) | ((x + dx) & 0xffff);
    LastBottom = ((y - dy) << 16) | ((x - dx) & 0xffff);

    pLastPoint = self->mpPoints;
    pPoint = pLastPoint + 1;
    pBuf++;
    {
        int i = 1;
        while (i < self->mNumPoints)
        {
            u32 *pOTPos;
            if ((pBuf->NormalX | pBuf->NormalY) &&
                (pLastBuf->NormalX | pLastBuf->NormalY))
            {
                pOTPos = (u32 *)((int)pDoubleBuffer->ViewportOT +
                                 ((pBuf->Zed - self->mFrigDeltaZ) &
                                  0x3ffc));
                dx = (pBuf->NormalX * pBuf->ScrWidth) >> 6;
                dy = (pBuf->NormalY * pBuf->ScrWidth) >> 6;
                x = (short)pBuf->Pos;
                y = pBuf->Pos >> 16;
                int NewTop = ((y + dy) << 16) | ((x + dx) & 0xffff);
                int NewBottom = ((y - dy) << 16) | ((x - dx) & 0xffff);
                POLY_G4 *pTopPoly = (POLY_G4 *)pPoly;
                POLY_G4 *pBottomPoly = pTopPoly + 1;
                DR_TPAGE *ptpage = (DR_TPAGE *)(pBottomPoly + 1);
                unsigned char *newppoly = (unsigned char *)(ptpage + 1);
                unsigned int a;
                unsigned int b;

                if (PolyBufferEnd < newppoly)
                    goto SEEYA;

                a = StoredTPagePrim.tag;
                b = StoredTPagePrim.code[0];
                ptpage->tag = a;
                ptpage->code[0] = b;
                pPoly = (u32 *)newppoly;

                if (2 < self->mTrail)
                {
                    {
                        unsigned char *newppoly =
                            (unsigned char *)pPoly + sizeof(POLY_G4);
                        if (PolyBufferEnd < newppoly)
                            goto SEEYA;
                        pPoly = (u32 *)newppoly;
                    }
                    ((POLY_G4 *)newppoly)->tag = 0x08000000;
                    *(u32 *)&((POLY_G4 *)newppoly)->r0 = 0x3a000000;
                    *(u32 *)&((POLY_G4 *)newppoly)->r1 = 0;
                    *(u32 *)&((POLY_G4 *)newppoly)->r3 =
                        *(u32 *)&pLastPoint->r;
                    *(u32 *)&((POLY_G4 *)newppoly)->r2 =
                        *(u32 *)&pPoint->r;
                    *(u32 *)&((POLY_G4 *)newppoly)->x1 =
                        pLastPoint->Last2Scr;
                    *(u32 *)&((POLY_G4 *)newppoly)->x0 =
                        pPoint->Last2Scr;
                    *(u32 *)&((POLY_G4 *)newppoly)->x3 = pLastBuf->Pos;
                    *(u32 *)&((POLY_G4 *)newppoly)->x2 = pBuf->Pos;
                    addPrim(pOTPos, (POLY_G4 *)newppoly);
                }

                pTopPoly->tag = pBottomPoly->tag = 0x08000000;
                *(u32 *)&pTopPoly->r0 = *(u32 *)&pBottomPoly->r0 =
                    0x3a000000;
                *(u32 *)&pTopPoly->r1 = 0;
                *(u32 *)&pTopPoly->r3 = *(u32 *)&pLastPoint->r;
                *(u32 *)&pTopPoly->r2 = *(u32 *)&pPoint->r;
                *(u32 *)&pTopPoly->x0 = NewTop;
                *(u32 *)&pTopPoly->x1 = LastTop;
                *(u32 *)&pTopPoly->x2 = pBuf->Pos;
                *(u32 *)&pTopPoly->x3 = pLastBuf->Pos;
                addPrim(pOTPos, pTopPoly);

                *(u32 *)&pBottomPoly->r1 = 0;
                *(u32 *)&pBottomPoly->r3 = *(u32 *)&pLastPoint->r;
                *(u32 *)&pBottomPoly->r2 = *(u32 *)&pPoint->r;
                *(u32 *)&pBottomPoly->x0 = NewBottom;
                *(u32 *)&pBottomPoly->x1 = LastBottom;
                *(u32 *)&pBottomPoly->x2 = pBuf->Pos;
                *(u32 *)&pBottomPoly->x3 = pLastBuf->Pos;
                addPrim(pOTPos, pBottomPoly);
                addPrim(pOTPos, ptpage);
                LastTop = NewTop;
                LastBottom = NewBottom;
            }

            pLastBuf++;
            pBuf++;
            pLastPoint++;
            pPoint++;
            i++;
        }
    }

SEEYA:
    if (self->mTrail != 0)
    {
        pBuf = (SCalcBuffer *)0x1f800000;
        pPoint = self->mpPoints;
        int i = 0;
        if (0 < self->mNumPoints)
        {
            do
            {
                if (self->mTrail == 1)
                {
                    pPoint->Last2Scr = pPoint->Last1Scr = pBuf->Pos;
                }
                else
                {
                    pPoint->Last2Scr = pPoint->Last1Scr;
                    pPoint->Last1Scr = pBuf->Pos;
                }
                pPoint++;
                pBuf++;
                i++;
            } while (i < self->mNumPoints);
        }
        self->mTrail++;
    }
}

/**
 * __15CTexturedRibbonii
 *
 * Address:   0x8003D9E0
 * Mangled:   __15CTexturedRibbonii
 * Demangled: CTexturedRibbon::(int, int)
 *
 * @param NumPoints  Number of stored ribbon points.
 * @param LeaveTrail  Nonzero to preserve previous projected positions.
 */
CTexturedRibbon::CTexturedRibbon(int NumPoints, int LeaveTrail)
    : CSpecialDisplay()
{
    int allocSize = NumPoints * sizeof(SRibbonPoint);
    _vf = (void **)__vt_15CTexturedRibbon;
    mNumPoints = NumPoints;
    SRibbonPoint *buf = (SRibbonPoint *)Mem_New(allocSize, 0, 1);
    mpPoints = buf;
    int size = Mem_GetBlockSize(buf);
    int usage = TotalBitUsage;
    int count = mNumPoints;
    usage = usage + 4;
    usage = usage + size;
    TotalBitUsage = usage;
    int i = 0;
    if (count > 0)
    {
        int offset = 0;
        do
        {
            int base = (int)mpPoints;
            SRibbonPoint *pt = (SRibbonPoint *)(offset + base);
            pt->WidthB = 0;
            pt->Width = 0;
            i++;
            offset += sizeof(SRibbonPoint);
        } while (i < mNumPoints);
    }
    mTrail = (u8)LeaveTrail;
    mDv = 8;
}

/**
 * CTexturedRibbon::~CTexturedRibbon()
 *
 * Address:   0x8003DAA8
 * Mangled:   _._15CTexturedRibbon
 * Demangled: CTexturedRibbon::~CTexturedRibbon()
 *
 */
CTexturedRibbon::~CTexturedRibbon()
{
    _vf = (void **)__vt_15CTexturedRibbon;
    int size = Mem_GetBlockSize(mpPoints);
    int usage = TotalBitUsage;
    SRibbonPoint *ptr2 = mpPoints;
    usage = usage - 4;
    usage = usage - size;
    TotalBitUsage = usage;
    Mem_Delete(ptr2);
}

/**
 * SetTexture__15CTexturedRibbonUi
 *
 * Address:   0x8003DB20
 * Mangled:   SetTexture__15CTexturedRibbonUi
 * Demangled: CTexturedRibbon::SetTexture(unsigned int)
 *            Calls Spool_FindTextureEntry, stores result at 0x4c
 *
 * @param Checksum  Asset hash for content lookup.
 */
void CTexturedRibbon::SetTexture(u32 Checksum)
{
    Texture *texEntry = Spool_FindTextureEntry(Checksum);
    mpTex = texEntry;
}

/**
 * Display__15CTexturedRibbon
 *
 * Address:   0x8003DB4C
 * Mangled:   Display__15CTexturedRibbon
 * Demangled: CTexturedRibbon::Display(void)
 *            Purpose: Render textured ribbon trails into GPU primitive buffers.
 *
 */
void CTexturedRibbon::Display()
{
    SRibbonPoint *pPoint;
    SCalcBuffer *pBuf;
    int LastNormalX;
    int LastNormalY;
    SCalcBuffer *pLastBuf;
    SRibbonPoint *pLastPoint;
    int x;
    int y;
    int dx;
    int dy;
    int dxB;
    int dyB;
    int LastTop;
    int LastBottom;
    int LastTopB;
    int LastBottomB;
    int voff;

    if (mDontDisplay == 0)
    {
        pBuf = (SCalcBuffer *)0x1f800000;
        {
            int i = 0;
            pPoint = mpPoints;
            if (0 < mNumPoints)
            {
                do
                {
                    pBuf->Zed = Transform(pPoint->Pos, pBuf->Pos);
                    if (pBuf->Zed < pCurrentViewport->Hither ||
                        pCurrentViewport->Yon < pBuf->Zed)
                    {
                        pBuf->Bad = 1;
                    }
                    else
                    {
                        pBuf->Bad = 0;
                        pBuf->ScrWidth = (pPoint->Width * 400) / pBuf->Zed;
                        pBuf->ScrWidthB =
                            (pPoint->WidthB * 400) / pBuf->Zed;
                    }
                    pBuf++;
                    pPoint++;
                    i++;
                } while (i < mNumPoints);
            }
        }

        LastNormalX = 0;
        pBuf = (SCalcBuffer *)0x1f800000;
        {
            int i = 0;
            LastNormalY = 0;
            if (0 < mNumPoints - 1)
            {
                do
                {
                    CalcScreenNormal(pBuf, pBuf->NormalX, pBuf->NormalY, 4);
                    if (pBuf->NormalX == 0 && pBuf->NormalY == 0)
                    {
                        pBuf->NormalX = LastNormalX;
                        pBuf->NormalY = LastNormalY;
                    }
                    LastNormalX = pBuf->NormalX;
                    LastNormalY = pBuf->NormalY;
                    pBuf++;
                    i++;
                } while (i < mNumPoints - 1);
            }
        }
        {
            int i;

        pBuf->NormalX = LastNormalX;
        pBuf->NormalY = LastNormalY;

        pBuf = (SCalcBuffer *)0x1f800000;
        pLastBuf = pBuf;
        pLastPoint = mpPoints;
        pPoint = pLastPoint + 1;
        x = (s16)pBuf->Pos;
        y = pBuf->Pos >> 16;
        dx = (pBuf->NormalX * pBuf->ScrWidth) >> 6;
        dy = (pBuf->NormalY * pBuf->ScrWidth) >> 6;
        dxB = (pBuf->NormalX * pBuf->ScrWidthB) >> 6;
        dyB = (pBuf->NormalY * pBuf->ScrWidthB) >> 6;
        LastTop = ((y + dy) << 16) | ((x + dx) & 0xffff);
        LastBottom = ((y - dy) << 16) | ((x - dx) & 0xffff);
        LastTopB = ((y + dyB) << 16) | ((x + dxB) & 0xffff);
        LastBottomB = ((y - dyB) << 16) | ((x - dxB) & 0xffff);

            voff = mV;
            pBuf++;
            for (i = 1; i < mNumPoints;)
            {
            unsigned int *pOTPos;
            if ((pBuf->NormalX | pBuf->NormalY) != 0)
            {
                if ((pLastBuf->NormalX | pLastBuf->NormalY) != 0)
                {
                int MinZ = pLastBuf->Zed;
                if (pBuf->Zed < MinZ)
                    MinZ = pBuf->Zed;

                x = (s16)pBuf->Pos;
                y = pBuf->Pos >> 16;
                dx = (pBuf->NormalX * pBuf->ScrWidth) >> 6;
                dy = (pBuf->NormalY * pBuf->ScrWidth) >> 6;
                dxB = (pBuf->NormalX * pBuf->ScrWidthB) >> 6;
                dyB = (pBuf->NormalY * pBuf->ScrWidthB) >> 6;

                int NewTop = ((y + dy) << 16) | ((x + dx) & 0xffff);
                int NewBottom = ((y - dy) << 16) | ((x - dx) & 0xffff);
                pOTPos =
                    (unsigned int *)((unsigned char *)pDoubleBuffer->ViewportOT +
                                     (MinZ & 0x3ffc));
                int NewTopB = ((y + dyB) << 16) | ((x + dxB) & 0xffff);
                int NewBottomB = ((y - dyB) << 16) | ((x - dxB) & 0xffff);

                if (mWibble != 0 && mNumPoints / 2 <= i)
                {
                    NewTop = ((y + dy +
                               Sine(TTime * 200 + 0x82) / 0x800) << 16) |
                             ((x + dx) & 0xffff);

                    y += (Sine(TTime * 250) * 3) / 0x1000;
                    pBuf->Pos = (pBuf->Pos & 0xffff) | (y << 16);

                    NewBottom = ((y - dy +
                                  Sine(TTime * 150 + 100) / 0x800) << 16) |
                                ((x - dx) & 0xffff);
                }

                POLY_GT4 *pTopPoly = (POLY_GT4 *)pPoly;
                POLY_GT4 *pBottomPoly = pTopPoly + 1;
                POLY_GT4 *pTopPolyB = pBottomPoly + 1;
                POLY_GT4 *pBottomPolyB = pTopPolyB + 1;
                DR_TWIN *pWinOn = (DR_TWIN *)(pBottomPolyB + 1);
                DR_TWIN *pWinOff = pWinOn + 1;
                DR_TPAGE *ptpage = (DR_TPAGE *)(pWinOff + 1);
                unsigned char *newppoly = (unsigned char *)(ptpage + 1);
                if (PolyBufferEnd < newppoly)
                    return;

                pPoly = (u32 *)newppoly;

                RECT win;
                win.x = win.y = win.w = win.h = 0;
                setlen(pWinOff, 2);
                pWinOff->code[0] =
                    0xe2000000 |
                    (((win.y & 0xff) >> 3) << 15) |
                    (((win.x & 0xff) >> 3) << 10) |
                    (((~(win.h - 1) & 0xff) >> 3) << 5) |
                    ((~(win.w - 1) & 0xff) >> 3);
                pWinOff->code[1] = 0;

                win.x = mpTex->u0;
                win.y = mpTex->v0;
                win.h = win.w = 64;
                setlen(pWinOn, 2);
                pWinOn->code[0] =
                    0xe2000000 |
                    (((win.y & 0xff) >> 3) << 15) |
                    (((win.x & 0xff) >> 3) << 10) |
                    (((~(win.h - 1) & 0xff) >> 3) << 5) |
                    ((~(win.w - 1) & 0xff) >> 3);
                pWinOn->code[1] = 0;
                addPrim(pOTPos, pWinOff);

                pBottomPolyB->tag = 0x0c000000;
                pTopPolyB->tag = 0x0c000000;
                pBottomPoly->tag = 0x0c000000;
                pTopPoly->tag = 0x0c000000;

                pTopPoly->clut = pBottomPoly->clut =
                    pTopPolyB->clut = pBottomPolyB->clut =
                        mpTex->clut;
                pTopPoly->tpage = pBottomPoly->tpage =
                    pTopPolyB->tpage = pBottomPolyB->tpage =
                        mpTex->tpage;

                int u;
                int v;
                unsigned int a;
                unsigned int b;
                u = mpTex->u0 + (mU & 0x3f);
                v = mpTex->v0 + (voff & 0x3f);

                /* Vertices 0/2 form the current edge; 1/3 form the previous edge. */
                pTopPolyB->u0 = u;
                pTopPolyB->v0 = v;
                pTopPolyB->u2 = u + 16;
                pTopPolyB->v2 = v;
                pTopPolyB->u1 = u;
                pTopPolyB->v1 = v + mDv;
                pTopPolyB->u3 = u + 16;
                pTopPolyB->v3 = v + mDv;

                pTopPoly->u0 = u + 16;
                pTopPoly->v0 = v;
                pTopPoly->u2 = u + 32;
                pTopPoly->v2 = v;
                pTopPoly->u1 = u + 16;
                pTopPoly->v1 = v + mDv;
                pTopPoly->u3 = u + 32;
                pTopPoly->v3 = v + mDv;

                pBottomPoly->u0 = u + 48;
                pBottomPoly->v0 = v;
                pBottomPoly->u2 = u + 32;
                pBottomPoly->v2 = v;
                pBottomPoly->u1 = u + 48;
                pBottomPoly->v1 = v + mDv;
                pBottomPoly->u3 = u + 32;
                pBottomPoly->v3 = v + mDv;

                pBottomPolyB->u0 = u + 64;
                pBottomPolyB->v0 = v;
                pBottomPolyB->u2 = u + 48;
                pBottomPolyB->v2 = v;
                pBottomPolyB->u1 = u + 64;
                pBottomPolyB->v1 = v + mDv;
                pBottomPolyB->u3 = u + 48;
                pBottomPolyB->v3 = v + mDv;
                voff -= mDv;

                *(u32 *)&pTopPoly->r0 = *(u32 *)&pPoint->rB;
                *(u32 *)&pTopPoly->r1 = *(u32 *)&pLastPoint->rB;
                *(u32 *)&pTopPoly->r2 = *(u32 *)&pPoint->r;
                *(u32 *)&pTopPoly->r3 = *(u32 *)&pLastPoint->r;
                *(u32 *)&pTopPoly->x0 = NewTop;
                *(u32 *)&pTopPoly->x1 = LastTop;
                *(u32 *)&pTopPoly->x2 = pBuf->Pos;
                *(u32 *)&pTopPoly->x3 = pLastBuf->Pos;

                *(u32 *)&pTopPolyB->r0 = 0;
                *(u32 *)&pTopPolyB->r1 = 0;
                *(u32 *)&pTopPolyB->r2 = *(u32 *)&pPoint->rB;
                *(u32 *)&pTopPolyB->r3 = *(u32 *)&pLastPoint->rB;
                *(u32 *)&pTopPolyB->x0 = NewTopB;
                *(u32 *)&pTopPolyB->x1 = LastTopB;
                *(u32 *)&pTopPolyB->x2 = NewTop;
                *(u32 *)&pTopPolyB->x3 = LastTop;

                *(u32 *)&pBottomPoly->r0 = *(u32 *)&pPoint->rB;
                *(u32 *)&pBottomPoly->r1 = *(u32 *)&pLastPoint->rB;
                *(u32 *)&pBottomPoly->r2 = *(u32 *)&pPoint->r;
                *(u32 *)&pBottomPoly->r3 = *(u32 *)&pLastPoint->r;
                *(u32 *)&pBottomPoly->x0 = NewBottom;
                *(u32 *)&pBottomPoly->x1 = LastBottom;
                *(u32 *)&pBottomPoly->x2 = pBuf->Pos;
                *(u32 *)&pBottomPoly->x3 = pLastBuf->Pos;

                *(u32 *)&pBottomPolyB->r0 = 0;
                *(u32 *)&pBottomPolyB->r1 = 0;
                *(u32 *)&pBottomPolyB->r2 = *(u32 *)&pPoint->rB;
                *(u32 *)&pBottomPolyB->r3 = *(u32 *)&pLastPoint->rB;
                *(u32 *)&pBottomPolyB->x0 = NewBottomB;
                *(u32 *)&pBottomPolyB->x1 = LastBottomB;
                *(u32 *)&pBottomPolyB->x2 = NewBottom;
                *(u32 *)&pBottomPolyB->x3 = LastBottom;

                pBottomPolyB->code = 0x3e;
                pTopPolyB->code = 0x3e;
                pBottomPoly->code = 0x3e;
                pTopPoly->code = 0x3e;

                addPrim(pOTPos, pTopPoly);
                addPrim(pOTPos, pTopPolyB);
                addPrim(pOTPos, pBottomPoly);
                addPrim(pOTPos, pBottomPolyB);

                LastTop = NewTop;
                LastBottom = NewBottom;
                LastTopB = NewTopB;
                LastBottomB = NewBottomB;

                addPrim(pOTPos, pWinOn);
                a = StoredTPagePrim.tag;
                b = StoredTPagePrim.code[0];
                ptpage->tag = a;
                ptpage->code[0] = b;
                addPrim(pOTPos, ptpage);
                }
            }

            pLastBuf++;
            pBuf++;
            pLastPoint++;
            pPoint++;
            i++;
            }
}
}
}

/**
 * Empty startup hook emitted for the BIT translation unit.
 *
 * `TotalBitUsage` is zero-initialized in BSS, so the original hook has no
 * dynamic initialization work to perform.
 *
 * Address: 0x8003E620
 * Mangled: _GLOBAL_.I.TotalBitUsage
 */
extern "C" void Bit_GlobalInit_TotalBitUsage(void)
    asm("_GLOBAL_.I.TotalBitUsage");
void Bit_GlobalInit_TotalBitUsage(void)
{
}

/**
 * `.ctors` slot for the hook above.
 *
 * GCC emits this table entry itself for a compiler-generated initializer, but
 * this owner's hook is hand-written, so the entry has to be declared. The
 * recovered application object order reproduces the target's 16-entry
 * constructor sequence.
 */
static void (*Bit_GlobalCtorSlot)(void)
    __attribute__((section(".ctors"))) = Bit_GlobalInit_TotalBitUsage;

/** Destroy an oscillating glow and restore its derived vtable first. */
CWobblyGlow::~CWobblyGlow()
{
    _vf = (void **)&__vt_11CWobblyGlow;
}

/** Default movement hook for a plain radial glow. */
void CGlow::Move()
{
}

/** Default display hook for a special-display bit. */
void CSpecialDisplay::Display()
{
}

/** Default movement hook for a transient bit. */
void CBit::Move()
{
}
