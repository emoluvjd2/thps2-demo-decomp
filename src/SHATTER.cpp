// SHATTER.cpp -- Reconstructed translation unit
// Original: SHATTER.CPP
// Functions: 12 reconstructed entries
// Address range: 0x80030F94 - 0x80032888
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#define SHATTER_OWNER_DECLARATIONS
#include "SHATTER.h"
#undef SHATTER_OWNER_DECLARATIONS
#include "SFX.h"
#include "UTILS.h"
#include "BIT.h"
#include "M3DZONE.h"
#define M3DCOLIJ_LINE_INFO_LAYOUT
#include "M3DCOLIJ.h"
#undef M3DCOLIJ_LINE_INFO_LAYOUT
#include "SMODEL.h"
#include "PSXREGION.h"
#include <INLINE_C.H>

/** Return caller-owned storage for an ordinary placement construction. */
inline void *operator new(unsigned int, void *storage)
{
    return storage;
}

// === Forward type declarations ===

// Owner-local layout slices recovered from MAIN_STRUCTS.h. SHATTER only needs
// these fields; keeping the slices local avoids importing unrelated gameplay
// classes from the monolithic generated layout header.
struct tag_S_Pal;

#include "TEXTURE.h"

/** Variable-length face packet prefix consumed by the shatter traversal. */
struct ShatterFace {
    unsigned short Flags;
    unsigned short PacketSize;
    unsigned char VertexNums[4];
    unsigned int ColourInfo;
    unsigned short NormalOffset;
    unsigned short ShatterFlags;
    union {
        Texture Embedded;
        Texture *Reference;
    } Material;
};

typedef char Shatter_face_prefix_must_be_16[
    ((int)&((ShatterFace *)0)->Material == 16) ? 1 : -1];

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
    struct ShatterObjectVtable *_vf;

    void SetPos(const CVector &pos);
    /** Mark this bit for removal. */
    void Die();
    /** Allocate zero-initialized storage from the transient-bit heap. */
    static void *operator new(unsigned int size);
    static void operator delete(void *p);
};

#include "CCHUNKBIT.h"

/** Common deleting-destructor prefix used by bit-owned effect objects. */
struct ShatterObjectVtable {
    void (*Reserved0)(void *object);
    void (*Reserved1)(void *object);
    void (*destroy)(void *object, int flags);
};

#include "SCAMERA.h"

#include "CNONRENDEREDBIT.h"

#include "CRIBBON.h"

#include "CSMOKETRAIL.h"

typedef char Shatter_smoke_trail_size_must_be_100[
    (sizeof(CSmokeTrail) == 100) ? 1 : -1];
typedef char Shatter_bit_size_must_be_72[
    (sizeof(CBit) == 72) ? 1 : -1];
typedef char Shatter_chunk_bit_size_must_be_192[
    (sizeof(CChunkBit) == 192) ? 1 : -1];

// CShatterBit — Phase F1.6: retired from `extern "C"` mangled-name shims.
struct CShatterBit : public CChunkBit {
    struct CSVector mAngVel;
    struct CSmokeTrail *mpSmokeTrail;

    CShatterBit(const CSVector &a, const CSVector &b, const CSVector &c,
                const CVector &pos, int Smoke);
    ~CShatterBit();
    /** Releases shatter-bit storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
    void Move();
    void SetPos(const CVector & pos);
};

// === External function declarations ===
#define M3DMATHS_TYPED_API
#include "M3DMATHS.h"
#undef M3DMATHS_TYPED_API
#include "M3DASM.h"

// === Owner-local globals ===
// MAIN.SYM records these as SHATTER-local definitions rather than XDEFs.
static CSVector *pNormal = 0;
static int ShatterSound = 0;

/**
 * Zero-initialized SHATTER state in its target-proven local `.sbss` order.
 *
 * The explicit section preserves the original owner and lets the generic
 * ASPSX preparation step expose these local definitions before their uses.
 */
static CVector ShatterSoundPos __attribute__((section(".sbss")));
static CVector ShatterCentre
    __attribute__((section(".sbss"), aligned(16)));
static int VRAMClut __attribute__((section(".sbss")));
static int VRAMTPage __attribute__((section(".sbss")));
static u8 AverageR __attribute__((section(".sbss")));
static u8 AverageG __attribute__((section(".sbss")));
static u8 AverageB __attribute__((section(".sbss")));

// === $gp-relative globals ===
// The target contributes one local vtable anchor word in this owner's `.sbss`.
#define SHATTER_DECLARE_CSHATTERBIT_VTABLE_ADDRESS
#include "SHATTER.h"

#include "CVECTOROPS.h"

// === Absolute-addressed globals ===
#include "GLEVEL.h"

// === Free function declarations ===
static void Split(const CVector &a, const CVector &b, const CVector &c,
                  int au, int av, int bu, int bv, int cu, int cv,
                  unsigned int TextureWindow, int n);
static void TransformVertex(CVector &a, SVECTOR *pVertices, unsigned char *pVertexNums, int Vertex);
static void CalcRGB(int IsGouraud, unsigned int ColourInfo, int NumVertices, unsigned int * pColourTable);
void Shatter_Glass(int n, const CVector &p0, const CVector &p1, const CVector &p2,
                   const CVector &Normal, u8 r, u8 g, u8 b);

// === Function implementations (address order) ===

/**
 * Purpose: Initialize the shatter bit from three chunk vertices and seed its motion.
 *
 * Address: 0x80030F94
 * Mangled: __11CShatterBitRC8CSVectorN21RC7CVectori
 *
 * @param a First chunk vertex relative to its centre.
 * @param b Second chunk vertex relative to its centre.
 * @param c Third chunk vertex relative to its centre.
 * @param pos World-space chunk centre.
 * @param Smoke Legacy smoke-selection argument retained by the original ABI.
 */
CShatterBit::CShatterBit(const CSVector &a, const CSVector &b,
                         const CSVector &c, const CVector &pos, int Smoke)
    : CChunkBit(a, b, c)
{
    _vf = (ShatterObjectVtable *)&__vt_11CShatterBit;
    short *angBase;
    // These empty constraints retain the target's value/address scheduling.
    // Ordinary int, short, direct-field, and reordered forms were measured;
    // the best clean form still commuted the addiu pairs in both branches.
    if (Rnd(2) != 0) {
        int v = Rnd(160) + 80;
        asm("" : "+r"(v));
        angBase = &mAngVel.vx;
        mAngVel.vx = (short)v;
        angBase[1] = 0;
    } else {
        int v = Rnd(160) + 80;
        asm("" : "+r"(v));
        angBase = &mAngVel.vx;
        mAngVel.vx = 0;
        angBase[1] = (short)v;
    }
    angBase[2] = 0;
    // Without these two scheduling boundaries GCC moves the final angular
    // store into Rnd's delay slot and the lifetime store into SetPos's.
    asm("");
    mLifetime = (unsigned short)(Rnd(30) + 45);
    asm("");
    this->SetPos(pos);
    CalculateWorldCoords();
    if (Rnd(2) != 0) {
        CSmokeTrail *Smoke = static_cast<CSmokeTrail *>(
            CBit::operator new(sizeof(CSmokeTrail)));
        if (Smoke != 0) {
            Smoke = ::new (Smoke) CSmokeTrail(mPos, 4, 128, 128, 128);
        }
        mpSmokeTrail = Smoke;
        Smoke->mProtected = 1;
        mpSmokeTrail->SetScale(1200);
        mpSmokeTrail->SetTint(32, 32, 32);
    }
}


/**
 * Destroy the owned smoke trail, then release inherited chunk state.
 *
 * Address:   0x8003109C
 * Mangled:   _._11CShatterBit
 * Demangled: CShatterBit::~CShatterBit()
 *
 */
CShatterBit::~CShatterBit()
{
    _vf = (ShatterObjectVtable *)&__vt_11CShatterBit;
    CSmokeTrail *child = mpSmokeTrail;
    if (child != 0) {
        child->_vf->destroy(child, 3);
    }
}


/**
 * Purpose: Seed shatter velocity from the shatter centre and bias it with the surface normal.
 *
 * Address:   0x80031110
 * Mangled:   SetPos__11CShatterBitRC7CVector
 * Demangled: CShatterBit::SetPos(const CVector &)
 *
 * @param pos  Position vector.
 */
void CShatterBit::SetPos(const CVector & pos)
{
    CBit::SetPos(pos);

    CVector d = pos - ShatterCentre;
    int Len = d.Length();
    if (Len != 0) {
        int Norm;
        mVel = ((Norm = Rnd(48)) * d) / Len;
    } else {
        Die();
    }

    int wobble = Rnd(45);
    CSVector *pn = pNormal;
    mVel.vx += wobble * pn->vx;
    mVel.vy += wobble * pn->vy;
    mVel.vz += wobble * pn->vz;

    int falloff = Rnd(175) << 12;
    mVel.vy -= falloff;

    if (Rnd(20) == 0) {
        int shift = 1;
        mVel <<= shift;
    }

    if (mVel.vy > 0) {
        mVel.vy = 0;
    }

    if (mpSmokeTrail != 0) {
        mpSmokeTrail->SetPos(mPos);
    }
}


/**
 * Advance the fragment, apply gravity and angular velocity, and age it.
 *
 * Address:   0x8003128C
 * Mangled:   Move__11CShatterBit
 * Demangled: CShatterBit::Move(void)
 *            Adds velocity to position, adds gravity to short-vector velocity,
 *            optionally updates ribbon pos, calculates world coords, decrements life
 *
 */
void CShatterBit::Move()
{
    CVector *pos = &mPos;
    *pos += mVel;
    // Apply the fixed gravity increment before rotating the shard.
    int gravity = mVel.vy;
    gravity += 0x5014;
    mVel.vy = gravity;
    mAngles += mAngVel;
    CSmokeTrail *ribbon = mpSmokeTrail;
    if (ribbon != 0) {
        ribbon->SetPos(*pos);
    }
    CalculateWorldCoords();
    u16 life = this->mLifetime;
    if (life != 0) {
        life = life - 1;
        this->mLifetime = life;
    } else {
        Die();
    }
}


/**
 * Recursively subdivide a triangle and emit one shatter bit per leaf.
 *
 * Address: 0x8003131C
 * Mangled: Split__FRC7CVectorN20iiiiiiUii
 *
 * Recursively split a triangle and spawn a textured shatter chunk for each
 * final fragment.
 */
static void Split(const CVector &a, const CVector &b, const CVector &c,
                  int au, int av, int bu, int bv, int cu, int cv,
                  unsigned int TextureWindow, int n) {
    if (LowMemory != 0) {
        return;
    }

    if (n != 0) {
        CVector p;
        int pu;
        int pv;
        int MaxLen;
        int Side;
        int Len;
        int Gamma;

        p = b - c;
        MaxLen = p.Length();
        Side = 0;

        p = a - c;
        Len = p.Length();
        if (MaxLen < Len) {
            MaxLen = Len;
            Side = 1;
        }

        p = a - b;
        Len = p.Length();
        if (MaxLen < Len)
            Side = 2;

        Gamma = Rnd(9);
        switch (Side) {
        case 0:
            p = b + (((c - b) * Gamma) >> 3);
            pu = bu + (((cu - bu) * Gamma) >> 3);
            pv = bv + (((cv - bv) * Gamma) >> 3);
            Split(a, b, p, au, av, bu, bv, pu, pv, TextureWindow, n - 1);
            Split(a, c, p, au, av, cu, cv, pu, pv, TextureWindow, n - 1);
            break;

        case 1:
            p = a + (((c - a) * Gamma) >> 3);
            pu = au + (((cu - au) * Gamma) >> 3);
            pv = av + (((cv - av) * Gamma) >> 3);
            Split(b, a, p, bu, bv, au, av, pu, pv, TextureWindow, n - 1);
            Split(b, c, p, bu, bv, cu, cv, pu, pv, TextureWindow, n - 1);
            break;

        case 2:
            p = a + (((b - a) * Gamma) >> 3);
            pu = au + (((bu - au) * Gamma) >> 3);
            pv = av + (((bv - av) * Gamma) >> 3);
            Split(c, a, p, cu, cv, au, av, pu, pv, TextureWindow, n - 1);
            Split(c, b, p, cu, cv, bu, bv, pu, pv, TextureWindow, n - 1);
            break;
        }
        return;
    }

    CVector sa = a >> 12;
    CVector sb = b >> 12;
    CVector sc = c >> 12;
    CVector Centre;
    CSVector OffA;
    CSVector OffB;
    CSVector OffC;

    Centre.vx = (sa.vx + sb.vx + sc.vx) / 3;
    Centre.vy = (sa.vy + sb.vy + sc.vy) / 3;
    Centre.vz = (sa.vz + sb.vz + sc.vz) / 3;

    OffA.vx = (s16)(sa.vx - Centre.vx);
    OffA.vy = (s16)(sa.vy - Centre.vy);
    OffA.vz = (s16)(sa.vz - Centre.vz);
    OffB.vx = (s16)(sb.vx - Centre.vx);
    OffB.vy = (s16)(sb.vy - Centre.vy);
    OffB.vz = (s16)(sb.vz - Centre.vz);
    OffC.vx = (s16)(sc.vx - Centre.vx);
    OffC.vy = (s16)(sc.vy - Centre.vy);
    OffC.vz = (s16)(sc.vz - Centre.vz);

    OffA += (Rnd(100) * *pNormal) >> 12;
    OffB += (Rnd(100) * *pNormal) >> 12;
    OffC += (Rnd(100) * *pNormal) >> 12;
    Centre <<= 12;

    UseCacheForBits = 0;
    CShatterBit *pBit = static_cast<CShatterBit *>(
        CBit::operator new(sizeof(CShatterBit)));
    if (pBit != 0) {
        pBit = ::new (pBit) CShatterBit(OffA, OffB, OffC, Centre, 0);
    }
    UseCacheForBits = 1;
    pBit->SetRGB(AverageR, AverageG, AverageB);
    pBit->SetUVs(
        (u16)VRAMClut, (u16)VRAMTPage,
        (u8)au, (u8)av, (u8)bu, (u8)bv, (u8)cu, (u8)cv);
    pBit->mTexWin = TextureWindow;
}


/**
 * Purpose: Transform one indexed shatter vertex and scale the result into world space.
 *
 * Address:   0x80031AD0
 * Mangled:   TransformVertex__FR7CVectorP7SVECTORPUci
 * Demangled: TransformVertex(CVector &, SVECTOR *, unsigned char *, int)
 *
 * @param a Receives the transformed long-vector result.
 * @param pVertices Model vertex table.
 * @param pVertexNums Face-local vertex indices.
 * @param Vertex Index within the face's vertex-index list.
 */
static void TransformVertex(CVector &a, SVECTOR *pVertices, unsigned char *pVertexNums, int Vertex)
{
    SVECTOR *vertex = pVertices + pVertexNums[Vertex];
    gte_ldv0(vertex);
    gte_mvmva(1, 0, 0, 0, 0);
    gte_stlvnl(&a);
    // Promote the transformed coordinate into fixed-point space.
    int shiftValue = 12;
    int shift = shiftValue;
    a <<= shift;
}


/**
 * Recover the average face colour from flat or indexed Gouraud data.
 *
 * Address:   0x80031B2C
 * Mangled:   CalcRGB__FiUiiPUi
 * Demangled: CalcRGB(int, unsigned int, int, unsigned int *)
 *
 * @param IsGouraud Nonzero when ColourInfo contains palette indices.
 * @param ColourInfo Packed colour or four colour-table indices.
 * @param NumVertices Three for triangles, four for quads.
 * @param pColourTable Region colour table used by Gouraud faces.
 */
static void CalcRGB(int IsGouraud, unsigned int ColourInfo, int NumVertices, unsigned int * pColourTable) {
    if (IsGouraud != 0) {
        unsigned int Col0 = pColourTable[ColourInfo & 0xff];
        unsigned int Col1 = pColourTable[(ColourInfo >> 8) & 0xff];
        unsigned int Col2 = pColourTable[(ColourInfo >> 16) & 0xff];

        int TotalRed = (Col0 & 0xff) + (Col1 & 0xff) + (Col2 & 0xff);
        int TotalGreen = ((Col0 >> 8) & 0xff) + ((Col1 >> 8) & 0xff) + ((Col2 >> 8) & 0xff);
        int TotalBlue = ((Col0 >> 16) & 0xff) + ((Col1 >> 16) & 0xff) + ((Col2 >> 16) & 0xff);

        if (NumVertices == 4) {
            unsigned int Col3 = pColourTable[ColourInfo >> 24];
            TotalRed += Col3 & 0xff;
            TotalGreen += (Col3 >> 8) & 0xff;
            TotalBlue += (Col3 >> 16) & 0xff;
            AverageR = TotalRed / 4;
            AverageG = TotalGreen / 4;
            AverageB = TotalBlue / 4;
        } else {
            AverageR = TotalRed / 3;
            AverageG = TotalGreen / 3;
            AverageB = TotalBlue / 3;
        }
    } else {
        AverageR = ColourInfo;
        AverageG = ColourInfo >> 8;
        AverageB = ColourInfo >> 16;
    }
}


/**
 * Consume one model face packet and emit its glass or textured fragments.
 *
 * Address:   0x80031CA4
 * Mangled:   Shatter_Face__FP4ItemPUiiiiii
 * Demangled: Shatter_Face(Item *, unsigned int *, int, int, int, int, int)
 *
 * @param pItem  Item to operate on.
 * @param pFace  Face to operate on.
 * @param NumSplits  Subdivision count passed to chunk splitting.
 * @param MakeFlash  Legacy flash request; unused by the target body.
 * @param UseFlags  Whether face eligibility flags gate the operation.
 * @param ModifyFlags  Whether consumed face flags are cleared.
 * @param TryToShatter  Whether geometry should be emitted after flag handling.
 * @return 0 when flag-gated, 1 for a processed face, or 2 for glass shatter.
 */
int Shatter_Face(Item * pItem, unsigned int * pFace, int NumSplits, int MakeFlash, int UseFlags, int ModifyFlags, int TryToShatter) {
    SModel *pModel;
    SVECTOR *pVertices, *pNormals;
    unsigned char *pVertexNums;
    Texture *pTexture;
    MATRIX Transform;
    CVector a, b, c;
    int retval;
    ShatterFace *face = (ShatterFace *)pFace;

    pModel = PSXRegion[pItem->Region].ppModels[pItem->Model];
    pVertices = pModel->Vertices;
    pNormals = pVertices + pModel->NumVertices;
    pVertexNums = face->VertexNums;

    if (UseFlags) {
        if (!(face->ShatterFlags & 8)) {
            return 0;
        }

        if (ModifyFlags) {
            face->ShatterFlags |= 1;
            face->ShatterFlags &= ~8;
            face->Flags &= ~0x80;
            face->Flags &= ~0x100;

            if (!TryToShatter) {
                if ((face->Flags & 0x40) || !(face->Flags & 1)) {
                    face->Flags &= ~0x40;
                }
            }
        }
    }

    if (!TryToShatter) {
        return 1;
    }

    if (face->Flags & 1) {
        pTexture = &face->Material.Embedded;
        if (!(face->Flags & 2)) {
            pTexture = face->Material.Reference;
        }
        int clut = pTexture->clut;
        int tpage = pTexture->tpage;
        VRAMClut = clut;
        VRAMTPage = tpage;
    }

    pNormal = (CSVector *)((unsigned char *)pNormals +
        (face->NormalOffset & 0xfff8));

    M3dMaths_RotMatrixYXZ(
        reinterpret_cast<SVECTOR *>(&pItem->Angles), &Transform);
    SetRotMatrix(&Transform);
    M3dAsm_SetTransVector(&pItem->Position);

    TransformVertex(a, pVertices, pVertexNums, 0);
    TransformVertex(b, pVertices, pVertexNums, 1);
    TransformVertex(c, pVertices, pVertexNums, 2);

    retval = 1;

    if (face->Flags & 0x10) {
        CalcRGB(face->Flags & 0x800, face->ColourInfo, 3,
                PSXRegion[pItem->Region].pColourTable);
        ShatterCentre.vx = (((a.vx >> 12) + (b.vx >> 12) + (c.vx >> 12)) / 3) << 12;
        ShatterCentre.vy = (((a.vy >> 12) + (b.vy >> 12) + (c.vy >> 12)) / 3) << 12;
        ShatterCentre.vz = (((a.vz >> 12) + (b.vz >> 12) + (c.vz >> 12)) / 3) << 12;

        if ((face->Flags & 0x40) || !(face->Flags & 1)) {
            if (!LowMemory) {
                CVector norm;
                int nx = pNormal->vx;
                int ny = pNormal->vy;
                int nz = pNormal->vz;
                norm.vx = nx;
                norm.vy = ny;
                norm.vz = nz;
                Shatter_Glass(15, a, b, c, norm, AverageR, AverageG, AverageB);
                retval = 2;
            }
            if (ModifyFlags) {
                face->Flags &= ~0x40;
            }
        } else {
            unsigned int TextureWindow = 0;
            if (face->Flags & 0x20) {
                TextureWindow = pTexture->TexWin;
            }
            if (!LowMemory) {
                Split(a, b, c,
                      pTexture->u0, pTexture->v0,
                      pTexture->u1, pTexture->v1,
                      pTexture->u2, pTexture->v2,
                      TextureWindow, NumSplits);
            }
        }
    } else {
        CVector d;
        TransformVertex(d, pVertices, pVertexNums, 3);
        CalcRGB(face->Flags & 0x800, face->ColourInfo, 4,
                PSXRegion[pItem->Region].pColourTable);
        ShatterCentre.vx = (((a.vx >> 12) + (b.vx >> 12) + (c.vx >> 12) + (d.vx >> 12)) >> 2) << 12;
        ShatterCentre.vy = (((a.vy >> 12) + (b.vy >> 12) + (c.vy >> 12) + (d.vy >> 12)) >> 2) << 12;
        ShatterCentre.vz = (((a.vz >> 12) + (b.vz >> 12) + (c.vz >> 12) + (d.vz >> 12)) >> 2) << 12;

        if ((face->Flags & 0x40) || !(face->Flags & 1)) {
            if (!LowMemory) {
                CVector norm;
                int nx = pNormal->vx;
                int ny = pNormal->vy;
                int nz = pNormal->vz;
                norm.vx = nx;
                norm.vy = ny;
                norm.vz = nz;
                Shatter_Glass(30, a, b, c, norm, AverageR, AverageB, AverageB);
                retval = 2;
            }
            if (ModifyFlags) {
                face->Flags &= ~0x40;
            }
        } else {
            unsigned int TextureWindow = 0;
            if (face->Flags & 0x20) {
                TextureWindow = pTexture->TexWin;
            }
            if (!LowMemory) {
                Split(a, b, c,
                      pTexture->u0, pTexture->v0,
                      pTexture->u1, pTexture->v1,
                      pTexture->u2, pTexture->v2,
                      TextureWindow, NumSplits);
                Split(b, c, d,
                      pTexture->u1, pTexture->v1,
                      pTexture->u2, pTexture->v2,
                      pTexture->u3, pTexture->v3,
                      TextureWindow, NumSplits);
            }
        }
    }
    return retval;
}


/**
 * Walk every variable-length face packet in a model and shatter eligible faces.
 *
 * Address:   0x800322BC
 * Mangled:   Shatter_Item__FP4Itemii
 * Demangled: Shatter_Item(Item *, int, int)
 *
 * @param pItem  Item to operate on.
 * @param UseFlags Nonzero to require the face's shatter-eligibility bit.
 * @param ModifyFlags Nonzero to persist the consumed face state.
 * @return 0
 */
int Shatter_Item(Item * pItem, int UseFlags, int ModifyFlags) {
    int num;
    int i;
    int Shattered;
    int comeback;
    SModel *pModel;
    ShatterFace *pFace;
    int NumFaces;

    num = 0;
    i = 0;
    Shattered = 0;
    pModel = PSXRegion[pItem->Region].ppModels[pItem->Model];
    NumFaces = pModel->NumFaces;
    pFace = (ShatterFace *)(pModel->Vertices + pModel->NumVertices + pModel->NumNormals);

    if (NumFaces != 0)
    {
        do
        {
            comeback = Shatter_Face(pItem, (unsigned int *)pFace, Rnd(5) == 0, 0, UseFlags, ModifyFlags, num < 6);
            if (comeback != 0)
            {
                Shattered = 1;
                if (comeback == 1)
                    num++;
            }
            i++;
            pFace = (ShatterFace *)((unsigned char *)pFace +
                                    (pFace->PacketSize & 0xfffc));
        } while (i < NumFaces);
    }
    if ((ModifyFlags != 0) && (Shattered != 0))
        pItem->Flags |= 1;
    return Shattered;
}


/**
 * Distribute glass shards across a triangle and retain the nearest shatter
 * position for the frame-level glass sound.
 *
 * Address: 0x800323F4
 * Mangled: Shatter_Glass__FiRC7CVectorN31UcUcUc
 *
 * @param n Number of glass shards to create.
 * @param p0 First triangle vertex.
 * @param p1 Second triangle vertex.
 * @param p2 Third triangle vertex.
 * @param Normal Triangle normal used to bias shard velocity.
 * @param r Red shard colour component.
 * @param g Green shard colour component.
 * @param b Blue shard colour component.
 */
void Shatter_Glass(int n, const CVector &p0, const CVector &p1, const CVector &p2,
                   const CVector &Normal, u8 r, u8 g, u8 b) {
    CVector middle = (p1 + p2) >> 1;

    SLineInfo ColInfo;
    ColInfo.StartCoords.vx = middle.vx + Normal.vx * 500;
    ColInfo.StartCoords.vy = middle.vy + Normal.vy * 500;
    ColInfo.StartCoords.vz = middle.vz + Normal.vz * 500;
    ColInfo.EndCoords.vx = ColInfo.StartCoords.vx;
    ColInfo.EndCoords.vy = ColInfo.StartCoords.vy + (3000 << 12);
    ColInfo.EndCoords.vz = ColInfo.StartCoords.vz;

    int GroundY;
    M3dColij_InitLineInfo(&ColInfo);
    M3dZone_LineToItem(&ColInfo, 1);
    GroundY = 0x7fffffff;
    if (ColInfo.pItem != 0) {
        GroundY = ColInfo.Position.vy;
    }

    CVector u = (p2 - p0) >> 12;
    CVector v = (p1 - p0) >> 12;

    for (int i = 0; i < n; i++) {
        int alpha = Rnd(0x1000);
        int beta = Rnd(0x1000);
        CVector p = ((alpha * u) + (beta * v)) >> 12;
        p = (p << 12) + p0;

        CVector a = p - middle;
        Utils_CalcUnit(a, a);
        a = (a * Rnd(40)) + (Normal * Rnd(40));

        if (ReplaySoundLockout == 0) {
            CGlassBit *glass = static_cast<CGlassBit *>(
                CBit::operator new(BIT_GLASS_STORAGE_SIZE));
            if (glass != 0) {
                CGlassBit_Construct(glass, p, a, GroundY,
                                    r, g, b, 100, 100, 100);
            }
        }
    }

    if (ShatterSound == 0) {
        ShatterSoundPos = middle;
        ShatterSound = 1;
    } else {
        CVector CamPos;
        CamPos.vx = MikeCamera[Camera_CurrentMikeCamera].Position.vx;
        CamPos.vy = MikeCamera[Camera_CurrentMikeCamera].Position.vy;
        CamPos.vz = MikeCamera[Camera_CurrentMikeCamera].Position.vz;
        int shift = 12;
        CamPos <<= shift;
        unsigned int centreDistance = Utils_CrapDist(CamPos, middle);
        unsigned int previousDistance = Utils_CrapDist(CamPos, ShatterSoundPos);
        if (centreDistance < previousDistance) {
            ShatterSoundPos = middle;
        }
    }
}


/**
 * Play the deferred level-specific glass sound once per frame.
 *
 * Address:   0x80032838
 * Mangled:   Shatter_MaybeMakeGlassShatterSound__Fv
 * Demangled: Shatter_MaybeMakeGlassShatterSound(void)
 *
 */
void Shatter_MaybeMakeGlassShatterSound(void) {
    if (ShatterSound) {
        int level = GLevel;
        int sound = 0x3A;
        if (level != 1 && level != 4) {
            sound = 0x2A;
        }
        SFX_PlayX(sound, 0x2000, 0);
    }
    ShatterSound = 0;
}


/**
 *
 * Address: 0x80032888
 * Mangled: _GLOBAL_.I.__11CShatterBitRC8CSVectorN21RC7CVectori
 *
 */
void Shatter_GlobalInit_CShatterBit(void) asm("_GLOBAL_.I.__11CShatterBitRC8CSVectorN21RC7CVectori");
void Shatter_GlobalInit_CShatterBit(void)
{
}

/**
 * `.ctors` table slot for this owner's hand-written global constructor.
 *
 * GCC emits this entry itself for a compiler-generated constructor, but this owner's
 * hook is written by hand, so the entry has to be declared. The sectioned,
 * initialized definition is retained in the object and reproduces the target
 * application object's table sequence.
 */
static void (*Shatter_GlobalCtorSlot)(void)
    __attribute__((section(".ctors"))) = Shatter_GlobalInit_CShatterBit;
