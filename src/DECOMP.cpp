// DECOMP.cpp -- Animation stream decompression and pose reconstruction.
// Original: DECOMP.CPP
// Functions: 2
// Address range: 0x80023B38 - 0x80023ED0

#include "MEM.h"
#include "M3DUTILS.h"
#include "globals.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include <INLINE_C.H>
#include "gte_extensions.h"
#include "PSXREGION.h"

/** Euler angles and pivot packed after each decompressed matrix array. */
struct SEulerPivot {
    short vx;
    short vy;
    short vz;
    short px;
    short py;
    short pz;
};

/** DECOMP's proven CItem prefix through the region selector. */
struct DecompCItemOwner {
    u8 pad_to_mRegion[27];
    u8 mRegion;
    u8 pad_after_mRegion[48];
};

/** DECOMP's opaque CBody slice containing the CItem prefix. */
struct DecompCBodyOwner {
    struct DecompCItemOwner CItem;
    u8 pad_after_CItem[188];
};

/** Animation state consumed by DECOMP, with target-proven field offsets. */
struct CSuper {
    struct DecompCBodyOwner CBody;
    u32 mExtraFlags;
    u16 *mpCalculationOrder;
    struct SMatrix *mpDecompressedFrame;
    u16 mRoot;
    u8 mDecompressedAnim;
    signed char mDecompressedFrame;
    s16 mFrame;
    u16 mAnim;
    u8 opaque_after_mAnim[116];
};

typedef char DecompCItemOwner_size_must_be_0x4c[
    (sizeof(DecompCItemOwner) == 0x4c) ? 1 : -1];
typedef char DecompCBodyOwner_size_must_be_0x108[
    (sizeof(DecompCBodyOwner) == 0x108) ? 1 : -1];
typedef char DecompCSuper_size_must_be_0x190[
    (sizeof(CSuper) == 0x190) ? 1 : -1];

/** Animation data owned by M3DUTILS and shared with the decompressor. */
#define TWEENBUFFER_CONSUMER_DECLARATION
#include "TWEENBUFFER.h"

/** Build a Y-X-Z rotation matrix from the packed Euler angles. */
#define M3DMATHS_TYPED_API
#include "M3DMATHS.h"
#undef M3DMATHS_TYPED_API

enum {
    DECOMP_PACKET_UNCOMPRESSED = 0x2A,
    DECOMP_PACKET_COMPRESSED = 0x2C,
    DECOMP_NO_ROOT_MODEL = 0xFFFF
};

/** Apply the shared rotation helper to DECOMP's packed matrix slice. */
static inline void Decomp_SetRotation(SEulerPivot *pEulerPivot, SMatrix *pMat)
{
    M3dMaths_RotMatrixYXZ(reinterpret_cast<SVECTOR *>(pEulerPivot),
        reinterpret_cast<MATRIX *>(pMat));
}

/** Read one packed 32-bit word for a GTE register transfer. */
static inline int Decomp_PackedWord(const void *pMat, int index = 0)
{
    return reinterpret_cast<const int *>(pMat)[index];
}

/** Locate an eight-byte animation table entry in a packet. */
static inline unsigned char *Decomp_AnimEntry(
    unsigned int *pAnimFile, int AnimNumber)
{
    return reinterpret_cast<unsigned char *>(
        (AnimNumber << 3) + reinterpret_cast<unsigned int>(pAnimFile));
}

/** Read the interpolation flag stored in an animation table entry. */
static inline unsigned short Decomp_AnimTweenFlag(
    unsigned int *pAnimFile, int AnimNumber)
{
    return *reinterpret_cast<unsigned short *>(
        Decomp_AnimEntry(pAnimFile, AnimNumber) + 10);
}

/** Read the frame-data offset stored in an animation table entry. */
static inline unsigned int Decomp_AnimFrameOffset(
    unsigned int *pAnimFile, int AnimNumber)
{
    return *reinterpret_cast<unsigned int *>(
        Decomp_AnimEntry(pAnimFile, AnimNumber) + 4);
}

/** Read the model count stored immediately before the region's pointer table. */
static inline unsigned int Decomp_ModelCount(SPSXRegion *pRegion)
{
    return reinterpret_cast<unsigned int *>(pRegion->ppModels)[-1];
}

/** Locate the Euler-frame storage packed after an object's matrix array. */
static inline SEulerPivot *Decomp_EulerFrames(
    SMatrix *matrices, unsigned int NumModels)
{
    return reinterpret_cast<SEulerPivot *>(matrices + NumModels);
}

// === Function implementations (address order) ===

/**
 * DecompressStream(unsigned char *, short *, int, int)
 * Animation stream decompression codec.
 * The first byte encodes the interpolation interval in its high nibble and
 * the compression mode in its low nibble.
 * Mode 0: 16-bit linear interpolation. Modes 1-13: bit-packed delta
 * interpolation (bit width = mode+1). Mode 14 repeats one 16-bit value;
 * mode 15 fills the stream with zeroes.
 * Returns updated source pointer (bytes consumed).
 *
 * Address: 0x80023B38
 * Mangled: DecompressStream__FPUcPsii
 *
 * @param pSource  Start of the compressed component stream.
 * @param pDest  First destination component to populate.
 * @param StepSize  Stride, in 16-bit elements, between destination values.
 * @param StreamLength  Number of values to decode.
 * @return The first unread byte in the compressed stream.
 */
unsigned char *DecompressStream(
    unsigned char *pSource, short *pDest, int StepSize, int StreamLength)
{
    int Interval;
    unsigned char Type;
    int Segs;
    int RemainderLength;
    int Val;                                 /* decoded value */

    Type = *pSource++;
    Interval = (Type >> 4) + 1;
    Type &= 0xF;

    if (Interval >= 2) {
        Segs = (StreamLength - 1) / Interval;
        int consumed = Segs * Interval + 1;
        RemainderLength = StreamLength - consumed;
    } else {
        Segs = StreamLength - 1;
        RemainderLength = 0;
    }

    switch (Type) {
    case 0: {
        /* 16-bit linear interpolation, 2 bytes per segment endpoint */
        short v = *pSource++;
        v = v + (*pSource++ << 8);
        int i = 0;
        *pDest = v;
        pDest = pDest + StepSize;
        if (Segs > 0) {
            int lim = Interval - 1;
            do {
                short NewVal = *pSource++;
                NewVal += *pSource++ << 8;
                short Inc = (NewVal - v) / Interval;
                int j = 0;
                if (lim > 0) {
                    do {
                        v = v + Inc;
                        *pDest = v;
                        j++;
                        pDest = pDest + StepSize;
                    } while (j < lim);
                }
                v = NewVal;
                *pDest = v;
                i++;
                pDest = pDest + StepSize;
            } while (i < Segs);
        }
        if (RemainderLength != 0) {
            short NewVal = *pSource++;
            int Off;
            NewVal = NewVal + (*pSource++ << 8);
            Off = NewVal - v;
            short Inc = Off / RemainderLength;
            register int rlim asm("$7") = RemainderLength - 1;
            int j = 0;                            /* $a0 */
            if (rlim > 0) {
                do {
                    v = v + Inc;
                    *pDest = v;
                    j++;
                    pDest = pDest + StepSize;
                } while (j < rlim);
            }
            *pDest = NewVal;
        }
        break;
    }

    case 1: case 2: case 3: case 4: case 5: case 6: case 7:
    case 8: case 9: case 10: case 11: case 12: case 13: {
        /* bit-packed delta interpolation, bit width = Type + 1 */
        int Bits = Type + 1;
        int Off;
        Val = *pSource++;
        int b1 = *pSource++;
        Val = Val + (b1 << 8);
        int i = 0;
        Off = 0;
        *pDest = Val;
        pDest = pDest + StepSize;
        if (Segs > 0) {
            int shiftAmt;
            shiftAmt = 32 - Bits;
            int lim = Interval - 1;
            do {
                register unsigned int b0 asm("$7") = pSource[0];
                register unsigned int b1 asm("$4") = pSource[1];
                unsigned int b2 = pSource[2];
                register int k asm("$7");
                { register unsigned int sh0 asm("$2") = b0 << 8; k = sh0 | b1; }
                { register unsigned int sh1 asm("$3") = static_cast<unsigned int>(k) << 8; k = sh1 | b2; }
                k = static_cast<int>(static_cast<unsigned int>(k) << (Off + 8)) >> shiftAmt;
                short Inc = k / Interval;
                { int d = Off + Bits;
                  pSource = pSource + (d >> 3);
                  Off = d & 7; }
                int j = 0;
                short NewVal;
                { register int et asm("$2") = Val + k; NewVal = et; }
                if (lim > 0) {
                    do {
                        Val = Val + Inc;
                        *pDest = Val;
                        j++;
                        pDest = pDest + StepSize;
                    } while (j < lim);
                }
                Val = NewVal;
                *pDest = Val;
                i++;
                pDest = pDest + StepSize;
            } while (i < Segs);
        }
        if (RemainderLength != 0) {
            register unsigned int b0 asm("$7") = pSource[0];
            register unsigned int b1 asm("$4") = pSource[1];
            unsigned int b2 = pSource[2];
            int k;
            { register unsigned int sh0 asm("$2") = b0 << 8; k = sh0 | b1; }
            { register unsigned int sh1 asm("$3") = static_cast<unsigned int>(k) << 8; k = sh1 | b2; }
            k = static_cast<int>(static_cast<unsigned int>(k) << (Off + 8));
            k >>= 32 - Bits;
            short Inc = k / RemainderLength;
            { int d = Off + Bits;
              pSource = pSource + (d >> 3);
              Off = d & 7; }
            int j = 0;
            short NewVal = Val + k;
            register int rlim asm("$5");
            { register int r asm("$25") = RemainderLength; asm("" : "+r"(r) : "r"(NewVal)); rlim = r - 1; }
            if (rlim > 0) {
                do {
                    Val = Val + Inc;
                    *pDest = Val;
                    j++;
                    pDest = pDest + StepSize;
                } while (j < rlim);
            }
            *pDest = NewVal;
        }
        if (Off != 0) {
            pSource = pSource + 1;
        }
        break;
    }

    case 14: {
        /* repeat single 16-bit value */
        Val = *pSource++;
        int b1 = *pSource++;
        Val = Val + (b1 << 8);
        int i = 0;
        if (StreamLength > 0) {
            do {
                *pDest = Val;
                i++;
                pDest = pDest + StepSize;
            } while (i < StreamLength);
        }
        break;
    }

    case 15: {
        /* fill zeros */
        int i = 0;
        if (StreamLength > 0) {
            do {
                *pDest = 0;
                i++;
                pDest = pDest + StepSize;
            } while (i < StreamLength);
        }
        break;
    }
    }

    return pSource;
}


/**
 * Decomp_GetAnimTransform(CSuper *)
 * Animation transform extraction — decompresses and builds bone transforms.
 * Type 0x2A: simple uncompressed animation data lookup.
 * Type 0x2C: compressed streams, decompressed per-bone with GTE rotation.
 * Returns the matrix array for the current animation frame.
 *
 * Address: 0x80023ED0
 * Mangled: Decomp_GetAnimTransform__FP6CSuper
 *
 * @param pSuper  Object whose current animation pose is required.
 * @return The object's decompressed matrices, an uncompressed frame, or the tween buffer.
 */
SMatrix *Decomp_GetAnimTransform(CSuper *pSuper)
{
    SPSXRegion *pRegion;
    unsigned int *pAnimFile;
    unsigned int NumModels;
    unsigned int NumItems;
    int AnimNumber;

    pRegion = &PSXRegion[pSuper->CBody.CItem.mRegion];

    pAnimFile = pRegion->pAnimFile;
    NumModels = Decomp_ModelCount(pRegion);
    NumItems = pRegion->pPSX[2];
    AnimNumber = pSuper->mAnim;

    switch (pAnimFile[-2]) {
    case DECOMP_PACKET_UNCOMPRESSED:
    {
        if (Decomp_AnimTweenFlag(pAnimFile, AnimNumber) != 0) {
            M3dUtils_InBetween(pSuper);
            return TweenBuffer;
        }
        return reinterpret_cast<SMatrix *>(reinterpret_cast<unsigned char *>(pAnimFile) +
            Decomp_AnimFrameOffset(pAnimFile, AnimNumber) +
            pSuper->mFrame * NumItems * sizeof(SMatrix));
    }

    case DECOMP_PACKET_COMPRESSED:
    {
        int NewBuffer;
        unsigned int *pAnimHeader;
        int NewAnim;

        NewBuffer = 0;

        if (pSuper->mpDecompressedFrame == 0) {
            unsigned int *pHeader;
            int NumAnims;
            int MaxFrames;
            unsigned int CalculationOrderSize;
            unsigned short *pParent;
            unsigned short *pChild;
            int c;

            NumAnims = *pAnimFile;
            pHeader = pAnimFile + 1;
            MaxFrames = 0;
            {
                int i;

                i = 0;
                if (NumAnims > 0) {
                    do {
                        pHeader++;
                        {
                            unsigned int NumFrames;
                            NumFrames = *pHeader;
                            if (MaxFrames < NumFrames) {
                                MaxFrames = NumFrames;
                            }
                        }
                        i++;
                        pHeader++;
                    } while (i < NumAnims);
                }
            }

            CalculationOrderSize = NumModels * sizeof(unsigned short);

            pSuper->mpDecompressedFrame = static_cast<SMatrix *>(Mem_New(
                NumModels * sizeof(SMatrix) + MaxFrames * NumModels * sizeof(SEulerPivot),
                1, 1));

            pSuper->mpCalculationOrder = static_cast<unsigned short *>(Mem_New(
                CalculationOrderSize, 0, 1));
            pSuper->mRoot = DECOMP_NO_ROOT_MODEL;

            {
                int i = 0;
                if (NumModels != 0) {
                    do {
                        if (pRegion->pHierarchy[i] == i) {
                            pSuper->mRoot = i;
                        }
                        i++;
                    } while (i < NumModels);
                }
            }

            pSuper->mpCalculationOrder[0] = pSuper->mRoot;
            pParent = pSuper->mpCalculationOrder;
            pChild = pParent + 1;
            for (c = NumModels - 1; c != 0; pParent++) {
                unsigned short *pHi = pRegion->pHierarchy;
                {
                    int i;

                    i = 0;
                    if (NumModels != 0) {
                        do {
                            if (i != pSuper->mRoot && *pHi == *pParent) {
                                *pChild++ = i;
                                c--;
                            }
                            i++;
                            pHi++;
                        } while (i < NumModels);
                    }
                }
            }

            NewBuffer = 1;
        }

        {
            NewAnim = 0;
            pAnimHeader = pAnimFile + (AnimNumber * 2 + 1);

            if (pSuper->mDecompressedAnim != AnimNumber || NewBuffer != 0) {
                {
                    int NumFrames;
                    int StepSize;
                    unsigned char *pCompressedStream;
                    SEulerPivot *pEulerPivot;
                    {
                        int m;

                        pEulerPivot = Decomp_EulerFrames(
                            pSuper->mpDecompressedFrame, NumModels);
                        m = 0;
                        pCompressedStream = reinterpret_cast<unsigned char *>(pAnimFile) +
                            pAnimHeader[0];
                        NumFrames = pAnimHeader[1];
                        StepSize = NumModels * 6;
                        if (NumModels != 0) {
                            do {
                                pCompressedStream = DecompressStream(pCompressedStream, &pEulerPivot->vx, StepSize, NumFrames);
                                pCompressedStream = DecompressStream(pCompressedStream, &pEulerPivot->vy, StepSize, NumFrames);
                                pCompressedStream = DecompressStream(pCompressedStream, &pEulerPivot->vz, StepSize, NumFrames);
                                pCompressedStream = DecompressStream(pCompressedStream, &pEulerPivot->px, StepSize, NumFrames);
                                pCompressedStream = DecompressStream(pCompressedStream, &pEulerPivot->py, StepSize, NumFrames);
                                pCompressedStream = DecompressStream(pCompressedStream, &pEulerPivot->pz, StepSize, NumFrames);
                                m++;
                                pEulerPivot++;
                            } while (m < NumModels);
                        }
                    }
                }

                NewAnim = 1;
                pSuper->mDecompressedAnim = AnimNumber;
            }
        }

        {
            if (pSuper->mDecompressedFrame != pSuper->mFrame || NewAnim != 0) {
                SEulerPivot *pEulerPivots;
                SMatrix *pMat;
                SEulerPivot *pRootEuler;
                unsigned short *pModel;

                pEulerPivots = Decomp_EulerFrames(
                    pSuper->mpDecompressedFrame, NumModels);
                {
                    int i;

                    pMat = &pSuper->mpDecompressedFrame[pSuper->mRoot];
                    pEulerPivots += NumModels * pSuper->mFrame;
                    pRootEuler = &pEulerPivots[pSuper->mRoot];

                    Decomp_SetRotation(pRootEuler, pMat);
                    pMat->t[0] = pRootEuler->px;
                    pMat->t[1] = pRootEuler->py;
                    pMat->t[2] = pRootEuler->pz;
                    pModel = pSuper->mpCalculationOrder + 1;

                    for (i = 1; i < NumModels; i++) {
                        SMatrix *pParentMat;
                        SEulerPivot *pEulerPivot;

                        pMat = &pSuper->mpDecompressedFrame[*pModel];
                        pEulerPivot = &pEulerPivots[*pModel];
                        pParentMat = &pSuper->mpDecompressedFrame[pRegion->pHierarchy[*pModel++]];

                        Decomp_SetRotation(pEulerPivot, pMat);

                        // --- Parent-rotate the child's local translation (GTE MVMVA) ---
                        //
                        //     t_child(parent space) = (parentRot * t_local) >> 12   (this block)
                        //                            + t_parent                     (added below)
                        //
                        // t_local = pEulerPivot's three translation components.
                        // parentRot = pParentMat's packed 3.12 fixed-point rotation,
                        // loaded into GTE ctrl $0..$4. MVMVA 0x4A486012 computes
                        // (R*V0)>>12, results saturated to s16 in IR1/IR2/IR3.
                        //
                        // Two load paths keyed on (pEulerPivot & 3): the record is only
                        // guaranteed 2-aligned (12-byte records). When 2-mod-4 aligned,
                        // +6/+0xA are word-aligned and each pair is one lw; when 4-aligned,
                        // the three shorts are staged through an aligned stack scratch.
                        {
                            if ((reinterpret_cast<unsigned int>(pEulerPivot) & 3) == 0) {
                                SVECTOR vec;
                                vec.vx = pEulerPivot->px;
                                vec.vy = pEulerPivot->py;
                                vec.vz = pEulerPivot->pz;
                                gte_ldvxy0(Decomp_PackedWord(&vec.vx));
                                gte_ldvz0(Decomp_PackedWord(&vec.vz));
                            } else {
                                gte_ldvxy0(Decomp_PackedWord(&pEulerPivot->px));
                                gte_ldvz0(Decomp_PackedWord(&pEulerPivot->pz));
                            }
                            gte_ldrot0(Decomp_PackedWord(pParentMat, 0));
                            gte_ldrot1(Decomp_PackedWord(pParentMat, 1));
                            gte_ldrot2(Decomp_PackedWord(pParentMat, 2));
                            gte_ldrot3(Decomp_PackedWord(pParentMat, 3));
                            gte_ldrot4(Decomp_PackedWord(pParentMat, 4));
                        }
                        gte_rtv0(); // (RotMatrix * V0) >> 12
                        gte_stsv(&pMat->t[0]);

                        // Add parent translation
                        pMat->t[0] += pParentMat->t[0];
                        pMat->t[1] += pParentMat->t[1];
                        pMat->t[2] += pParentMat->t[2];
                    }
                }

                pSuper->mDecompressedFrame = pSuper->mFrame;
            }
        }
        return pSuper->mpDecompressedFrame;
    }

    }
}
