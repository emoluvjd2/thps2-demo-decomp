// VIDEO.cpp -- Reconstructed translation unit
// Original: VIDEO.CPP
// Functions: 32 reconstructed entries
// Address range: 0x80060EF4 - 0x800632B4
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#define VIDEO_OWNER_DECLARATIONS
#include "VIDEO.h"
#undef VIDEO_OWNER_DECLARATIONS
#include "MAIN.h"
#include "BADDY.h"
#include "MECH.h"
#include "UTILS.h"

/** Replay-local types and cross-owner state used by VIDEO.CPP. */
typedef unsigned long ulong;

#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include "globals.h"

struct CBody;

#define VIDEO_HIGHLIGHT_LAYOUT
#define VIDEO_LAYOUT_ONLY
#include "VIDEO.h"
#undef VIDEO_LAYOUT_ONLY
#undef VIDEO_HIGHLIGHT_LAYOUT

enum EPhysicsState {
    PHYSICS_ON_GROUND = 0,
    PHYSICS_ON_RAIL = 4,
};

enum EPausedStatus {
    STATUS_ENDHEATREPLAY = 20,
    STATUS_ENDHEATJUDGES = 21,
    STATUS_ENDHEATLADDER = 22,
};

enum ESoundCmdType {
    SFX_EMPTY = 0,
    SFX_PLAY = 1,
    SFX_STOP = 2,
};

struct SSoundCmd {
    ESoundCmdType SoundCmdType;
    int SoundNum;
};

struct SVideoInfo {
    /** Preserve the original empty construction of the CVector members. */
    SVideoInfo() {}

    EVideoMode VideoMode;
    ulong *pBlockStart;
    ulong *pStreamStart;
    ulong *pStreamEnd;
    ulong *pStream;
    ulong Bits;
    int BitsLeft;
    ulong NumFrames;
    ulong FramesLeft;
    int CurCameraIndex;
    CVector CamPos;
    int CamMode;
    int CamXblanks;
    int TabuCam;
    int NoLOS;
    int OldCameraIndex;
    int TrailingCameraType;
    int ReplayHighlight;
    SHighlight Highlights[5];
    EPhysicsState OldPhysicsState;
    int OldAnim;
    int OldFrame;
    SVECTOR Euler;
    SVECTOR OldEuler;
    CVector OldPos;
    CVector OldPos2;
    int OldFlipAngle;
    int FlipAngleDelta;
    int OldXblanks;
    int Yblanks;
    int PlaybackFrozen;
};

#define VIDEO_HEAD_LAYOUT
#define VIDEO_LAYOUT_ONLY
#include "VIDEO.h"
#undef VIDEO_LAYOUT_ONLY
#undef VIDEO_HEAD_LAYOUT

/** Cross-owner globals addressed by VIDEO.CPP. */
#define BITSTATE_DECLARE_BIT_COUNT
#include "BITSTATE.h"
#define DEMOTIMER_ABSOLUTE_DECLARATION
#include "DEMOTIMER.h"
#define BODYLISTS_DECLARE_ENVIRONMENTAL_OBJECT_LIST
#include "BODYLISTS.h"
#include "GLEVEL.h"
#include "MECHLIST.h"
#include "MUSICSTOPPED.h"
#include "FRONTSTATUS.h"
#include "TRICKPLAYER.h"
#define BITSTATE_DECLARE_USE_CACHE_FOR_BITS
#include "BITSTATE.h"
#include "MIPS.h"
#include "M3D.h"
/**
 * Owner-local gameplay layouts used by VIDEO.CPP.
 *
 * These slices recover only the fields VIDEO reads or writes. Their offsets
 * come from MAIN_STRUCTS.h and are kept local until shared owners agree.
 */
#include "BIT.h"

/** Narrow natural interface to the transient-bit allocator owned by BIT. */
struct CBit
{
    /** Allocate zero-initialized storage from the transient-bit heap. */
    static void *operator new(unsigned int size);
};

#include "SPOOL.h"
#include "MEM.h"
#include <LIBETC.H>
#define M3DCOLIJ_LINE_INFO_LAYOUT
#include "M3DCOLIJ.h"
#undef M3DCOLIJ_LINE_INFO_LAYOUT

struct SControl;
struct SGapTrick;
struct CSuper;

/** Replay-local CItem prefix, including its list link and vtable slot. */
struct VideoCItemOwner {
    unsigned short mFlags;
    unsigned short mInquiry;
    CVector mPos;
    CSVector mAngles;
    unsigned short mModel;
    char mDummyFrame;
    unsigned char mTintIndex;
    unsigned char mDummyAnim;
    unsigned char mRegion;
    VideoCItemOwner *mNextItem;
    unsigned int mRGB;
    u8 _pad0024[36];
    void (**_vf)();
};

/** Replay-local target layout slice of CBody. */
struct CBody {
    VideoCItemOwner CItem;
    u8 _pad004C[8];
    CVector mVel;
    u8 _pad0060[48];
    CVector mShadowPos;
    u8 _pad009C[40];
    CVector mOldPos;
    u8 _pad00D0[56];
};

typedef char Video_CBody_size_must_be_264[
    (sizeof(CBody) == 264) ? 1 : -1];

/** Invoke the power-up body's collect slot through its recovered vtable. */
static inline void Video_CollectBody(CBody *pBody) {
    if (pBody == 0)
        return;
    typedef void (*CollectMethod)(CBody *);
    reinterpret_cast<CollectMethod>(pBody->CItem._vf[3])(pBody);
}

/** Return a signed magnitude without introducing a runtime library call. */
static inline int Video_Abs(int value) {
    return (value ^ (value >> 31)) - (value >> 31);
}

/** Replay-local CSuper prefix through the animation-speed field. */
struct VideoCSuperOwner {
    struct CBody CBody;
    unsigned int mExtraFlags;
    u8 _pad010C[12];
    short mFrame;
    unsigned short mAnim;
    u8 _pad011C[16];
    int mAnimSpeed;
    u8 _pad0130[96];
};

#define TRICKOB_LAYOUT
#define TRICKOB_LAYOUT_ONLY
#include "TRICKOB.h"
#undef TRICKOB_LAYOUT_ONLY
#undef TRICKOB_LAYOUT

/**
 * Owner-local CBruce slice covering replay and shared trick state.
 */
struct CBruce {
    VideoCSuperOwner CSuper;
    u8 _pad0190[2292];
    short *mpTrick;
    short mTrickPoints;
    u8 _pad0A8A[2];
    char *mpTrickName;
    int mDoingAScript;
    short *mpOffTrick;
    short *mpJumpTrick;
    u8 _pad0A9C[4];
    short mBailNumber;
    short mRailBailNumber;
    u8 _pad0AA4[4];
    short *mpReturn;
    u8 _pad0AAC[676];
    int mDoingATrick;
    int mCanStartNextTrick;
    int mTrickAwarded;
    int mDoingWhatTrick;
    u8 _pad0D60[8];
    int mNoSpin;
    int mNoSpinScore;
    int mFlipNum;
    int mFlipAngle;
    u8 _pad0D78[44];
    int mNoTrick;
    int mSkaterType;
    int mIndex;
    int mPlayTime;
    SControl *mPad;
    u8 _pad0DB8[8];
    Item *mLastSItem;
    unsigned int *mLastPSX;
    u8 _pad0DC8[4];
    int mAutoTurn;
    u8 _pad0DD0[64];
    int mManual;
    u8 _pad0E14[80];
    int mRailSlack;
    u8 _pad0E68[8];
    MATRIX mOldRotationMatrix;
    MATRIX mRotationMatrix;
    u8 _pad0EB0[8];
    int mTapTurn;
    u8 _pad0EBC[200];
    int mMagicPoints;
    int mMagicTime;
    u8 _pad0F8C[80];
    int mGaps;
    SGapTrick *mpGrindGapTrick;
    u8 _pad0FE4[56];
    int mTrickTookOff;
    int mTallyAngles;
    int mNum180Spins;
    u8 _pad1028[4];
    int mPerfectLanding;
    int mSloppyLanding;
    int mLastShadowCheckHeight;
    u8 _pad1038[52];
    EPhysicsState mPhysicsState;
    u8 _pad1070[48];
    CVector mFront;
    u8 _pad10AC[136];
    u8 mAnalogue;
    char mAX;
    char mAY;
    /** Opaque trailing input/history state through CBruce's proven end. */
    u8 _pad1138[0x334];

    /** Cross-owner member methods invoked while replaying a skater. */
    void BoardOn();
    void BoardOff();
    void BloodSplat(int bodyPart, int maxBits);
    void DoShadowCheck();
    void GlowUpdate();
    void HandleWater();
    void FRUVectorsFromRotMatrix();
};

/** Recover a complete skater from the CBody list head used by VIDEO.CPP. */
static inline CBruce *Video_GetBruce(CBody *pBody) {
    return reinterpret_cast<CBruce *>(pBody);
}

/** Follow the CItem list link to the next skater body. */
static inline CBruce *Video_GetNextBruce(CBody *pBody) {
    return reinterpret_cast<CBruce *>(pBody->CItem.mNextItem);
}

/** External replay collaborators with natural C++ source spelling. */
#include "OB.h"
#include "CAMPT.h"
void RestartCompFrig(void);
#include "FRONT.h"
#include "TRIG.h"

/** Canonical BIT2-owned grind-spark allocation and construction API. */
#include "BIT2.h"
#include "POWERUP.h"

/** Replay collaborators whose owners expose C-linkage storage. */
#define TRICKOBLIST_ABSOLUTE_DECLARATION
#include "TRICKOBLIST.h"

/** Fixed-point vector operators provided by VECTOR.cpp. */
#include "CVECTOROPS.h"

typedef char Video_CBruce_size_must_be_5228[
    (sizeof(CBruce) == 5228) ? 1 : -1];
typedef char Video_CItem_size_must_be_76[
    (sizeof(VideoCItemOwner) == 76) ? 1 : -1];
typedef char Video_CSuper_size_must_be_400[
    (sizeof(VideoCSuperOwner) == 400) ? 1 : -1];
typedef char Video_CTrickOb_size_must_be_24[
    (sizeof(CTrickOb) == 24) ? 1 : -1];
#include "SFX.h"
#include "M3DZONE.h"
#include "PAD.h"
#include "PLATFORM.h"

enum {
    VideoWordBits = 32,
    VideoStreamWords = 32,
    VideoHighlightCount = 5,
    VideoSoundQueueCapacity = 30,
    VideoMaxHighlightFrame = 3599,
    VideoNoEndFrame = 0x7FFFFFFF,
    VideoAngleMask = 0xfff,
    VideoHalfTurn = 0x800,
    VideoFullTurn = 0x1000,
    VideoAngleWrapBias = 0x4000,
};

#define VIDEOINFO_DECLARE_GVINFO1
#define VIDEOINFO_DECLARE_GVINFO2
#include "VIDEOINFO.h"

#define REPLAYSOUNDLOCKOUT_OWNER_DECLARATION
#include "REPLAYSOUNDLOCKOUT.h"
int ReplaySoundLockout __attribute__((section(".sdata"))) = 0;

/** Primary replay stream state owned by VIDEO.CPP. */
SVideoInfo GVInfo1 __attribute__((section(".data")));

/** Secondary replay stream state used by two-player playback. */
SVideoInfo GVInfo2 __attribute__((section(".data")));

/** Position-delta widths selected by the replay prefix code. */
static int PosBits[7] __attribute__((section(".data"))) = {3, 4, 5, 7, 8, 16, 20};

/** Euler-angle widths selected by the replay prefix code. */
static int EulerBits[7] __attribute__((section(".data"))) = {2, 4, 5, 6, 7, 9, 12};

/** Board-flip widths selected by the replay prefix code. */
static int FlipAngleBits[7] __attribute__((section(".data"))) = {0, 6, 7, 8, 12, -1, -1};

/** Variable-length prefix values used by replay compression. */
static int PrefixCode[7] __attribute__((section(".data"))) = {0, 1, 3, 7, 15, 31, 63};

/** Bit lengths for each replay prefix value. */
static int PrefixBits[7] __attribute__((section(".data"))) = {1, 2, 3, 4, 5, 6, 6};

/** Ring buffer of sound commands embedded in a replay stream. */
SSoundCmd SoundCmd[VideoSoundQueueCapacity]
    __attribute__((section(".data"))) = {};

/** Replay and highlight bookkeeping, in the original VIDEO.CPP small-data order. */
int GVideoReplay __attribute__((section(".sdata"))) = 0;
int HLStartCalled __attribute__((section(".sdata"))) = 0;
static int StepCount __attribute__((section(".sdata"))) = 0;
int FirstEntry __attribute__((section(".sdata"))) = 0;
int LastEntry __attribute__((section(".sdata"))) = 0;
int Splat1 __attribute__((section(".sdata"))) = 0;
int Splat2 __attribute__((section(".sdata"))) = 0;
int TrickOb1 __attribute__((section(".sdata"))) = 0;
int TrickOb2 __attribute__((section(".sdata"))) = 0;
int HighlightStartFrame __attribute__((section(".sdata"))) = 0;
int HighlightEndFrame __attribute__((section(".sdata"))) = 0;
int HighlightStartY __attribute__((section(".sdata"))) = 0;
int HighlightEndY __attribute__((section(".sdata"))) = 0;

// === Free function declarations ===
static long BitsRequired(long n);
static void PutNBits(SVideoInfo *VInfo, long v, int n);
static void FlushBits(SVideoInfo *VInfo);
static long GetNBitsUnsigned(SVideoInfo *VInfo, int n);
static long GetNBitsSigned(SVideoInfo *VInfo, int n);
static long GetBit(SVideoInfo *VInfo);
void InitSoundCmdQueue(void);
void QueueSoundCmd(ESoundCmdType SoundCmdType, int SoundNum);
int HasSoundCmd(void);
void DequeueSoundCmd(ESoundCmdType &SoundCmdType, int &SoundNum);
void InitBloodSplat(void);
void InitTrickObs(void);
void Matrix2Euler(MATRIX *m, SVECTOR *a);
static void PutDataChunk(CBruce *pSkater);
static void GetDataChunk(CBruce *pSkater, int NChunks);

// === Function implementations (address order) ===

/**
 * BitsRequired__Fl
 *
 * Address:   0x80060EF4
 * Mangled:   BitsRequired__Fl
 * Demangled: BitsRequired(long)
 *
 * @param n  Count.
 * @return bits
 */
static long BitsRequired(long n) {
    long Bits = 2;
    if (n < 0) {
        long Limit = -2;
        if (n < Limit) {
            do {
                Limit <<= 1;
                Bits++;
            } while (n < Limit);
        }
    } else {
        long Limit = 2;
        if (n >= Limit) {
            do {
                Limit <<= 1;
                Bits++;
            } while (n >= Limit);
        }
    }
    return Bits;
}


/**
 * PutNBits__FP10SVideoInfoli
 *
 * Address:   0x80060F48
 * Mangled:   PutNBits__FP10SVideoInfoli
 * Demangled: PutNBits(SVideoInfo *, long, int)
 *            Append bits to the replay bitstream, flushing full words as needed.
 *
 * @param VInfo  Video-info structure.
 * @param v  Value whose low `n` bits are appended to the stream.
 * @param n  Count.
 */
static void PutNBits(SVideoInfo *VInfo, long v, int n) {
    if (n == 0) {
        return;
    }

    int bitsLeft = VInfo->BitsLeft;
    if (bitsLeft < n) {
        // The stream cursor's target allocation is load-bearing under GCC 2.91.
        int WordBits = VideoWordBits;
        n -= bitsLeft;
        int shift = WordBits - bitsLeft;
        ulong shifted = static_cast<ulong>(v) << shift;
        v >>= bitsLeft;
        int bits = VInfo->Bits;
        register ulong *pWord asm("$4") = VInfo->pStream;
        bits &= (1 << shift) - 1;
        bits |= shifted;
        VInfo->Bits = bits;
        *pWord = bits;
        pWord++;
        VInfo->pStream = pWord;
        VInfo->Bits = 0;
        VInfo->BitsLeft = WordBits;
        bitsLeft = WordBits;
    }

    int remaining = bitsLeft - n;
    register int WordBits2 asm("$6") = VideoWordBits;
    int shift2 = WordBits2 - bitsLeft;
    int mask2 = (1 << shift2) - 1;
    // Preserve the two scheduling dependencies used by the target compiler.
    // Removing either barrier reorders the remaining-bit calculation.
    asm volatile("" : "+r"(mask2));
    int bits2 = VInfo->Bits;
    register int shifted2 asm("$4") = static_cast<ulong>(v) << shift2;
    VInfo->BitsLeft = remaining;
    bits2 &= mask2;
    bits2 |= shifted2;
    VInfo->Bits = bits2;
    if (remaining == 0) {
        ulong *pWord = VInfo->pStream;
        *pWord = bits2;
        pWord++;
        VInfo->pStream = pWord;
        asm volatile("" : "+r"(remaining));
        VInfo->Bits = 0;
        VInfo->BitsLeft = WordBits2;
    }
}


/**
 * FlushBits__FP10SVideoInfo
 *
 * Address:   0x8006100C
 * Mangled:   FlushBits__FP10SVideoInfo
 * Demangled: FlushBits(SVideoInfo *)
 *
 * @param VInfo  Video-info structure.
 */
static void FlushBits(SVideoInfo *VInfo) {
    if (VInfo->BitsLeft != VideoWordBits) {
        ulong *pWord = VInfo->pStream;
        *pWord = VInfo->Bits;
        VInfo->pStream = pWord + 1;
        VInfo->Bits = 0;
        VInfo->BitsLeft = VideoWordBits;
    }
}


/**
 * GetNBitsUnsigned__FP10SVideoInfoi
 *
 * Address:   0x80061044
 * Mangled:   GetNBitsUnsigned__FP10SVideoInfoi
 * Demangled: GetNBitsUnsigned(SVideoInfo *, int)
 *
 * @param VInfo  Video-info structure.
 * @param n  Count.
 * @return result
 */
static long GetNBitsUnsigned(SVideoInfo *VInfo, int n) {
    int bitsLeft = VInfo->BitsLeft;
    unsigned long res;
    if (bitsLeft != 0) goto past_refill;
    {
        bitsLeft = VideoWordBits;
        // A normal local, a literal store, and a direct field refill all make
        // GCC allocate a different refill path; keep the proven target pin.
        register int WordBits asm("$3") = VideoWordBits;
        ulong *pWord = VInfo->pStream;
        int word = *pWord;
        pWord = pWord + 1;
        VInfo->pStream = pWord;
        VInfo->BitsLeft = WordBits;
        VInfo->Bits = word;
    }
past_refill:
    if (bitsLeft < n) goto cross_word;
    {
        VInfo->BitsLeft = bitsLeft - n;
        res = VInfo->Bits & ((1 << n) - 1);
        VInfo->Bits = static_cast<unsigned int>(VInfo->Bits) >> n;
        goto done;
    }
cross_word:
    {
        n = n - bitsLeft;
        ulong *pWord = VInfo->pStream;
        res = VInfo->Bits & ((1 << bitsLeft) - 1);
        VInfo->Bits = *pWord;
        pWord = pWord + 1;
        VInfo->pStream = pWord;
        VInfo->BitsLeft = VideoWordBits - n;
        res = res | ((VInfo->Bits & ((1 << n) - 1)) << bitsLeft);
        VInfo->Bits >>= n;
    }
done:
    return res;
}


/**
 * GetNBitsSigned__FP10SVideoInfoi
 *
 * Address:   0x80061100
 * Mangled:   GetNBitsSigned__FP10SVideoInfoi
 * Demangled: GetNBitsSigned(SVideoInfo *, int)
 *
 * @param VInfo  Video-info structure.
 * @param n  Count.
 * @return result
 */
static long GetNBitsSigned(SVideoInfo *VInfo, int n) {
    long res = GetNBitsUnsigned(VInfo, n);
    if (res & (1 << (n - 1))) {
        res |= -(1 << n);
    }
    return res;
}


/**
 * GetBit__FP10SVideoInfo
 *
 * Address:   0x8006114C
 * Mangled:   GetBit__FP10SVideoInfo
 * Demangled: GetBit(SVideoInfo *)
 *
 * @param VInfo  Video-info structure.
 * @return bitBuf & 1
 */
static long GetBit(SVideoInfo *VInfo) {
    int bitsLeft = VInfo->BitsLeft;
    if (bitsLeft != 0) goto past_refill;
    {
        // See GetNBitsUnsigned: this refill constant is register-sensitive.
        register int WordBits asm("$3") = VideoWordBits;
        bitsLeft = VideoWordBits;
        ulong *pWord = VInfo->pStream;
        int word = *pWord;
        VInfo->pStream = pWord + 1;
        VInfo->BitsLeft = WordBits;
        VInfo->Bits = word;
    }
past_refill:;
    int word = VInfo->Bits;
    VInfo->BitsLeft = bitsLeft - 1;
    VInfo->Bits = static_cast<u32>(word) >> 1;
    return word & 1;
}


/**
 * InitSoundCmdQueue__Fv
 *
 * Address:   0x8006119C
 * Mangled:   InitSoundCmdQueue__Fv
 * Demangled: InitSoundCmdQueue(void)
 *
 */
void InitSoundCmdQueue(void) {
    FirstEntry = 0;
    LastEntry = 0;
}


/**
 * QueueSoundCmd__F13ESoundCmdTypei
 *
 * Address:   0x800611AC
 * Mangled:   QueueSoundCmd__F13ESoundCmdTypei
 * Demangled: QueueSoundCmd(ESoundCmdType, int)
 *
 * @param SoundCmdType  Replay sound operation to queue.
 * @param SoundNum  Sound identifier consumed by the queued operation.
 */
void QueueSoundCmd(ESoundCmdType SoundCmdType, int SoundNum) {
    SoundCmd[LastEntry].SoundCmdType = SoundCmdType;
    SoundCmd[LastEntry].SoundNum = SoundNum;
    LastEntry = (LastEntry + 1) % VideoSoundQueueCapacity;
}


/**
 * HasSoundCmd__Fv
 *
 * Address:   0x8006120C
 * Mangled:   HasSoundCmd__Fv
 * Demangled: HasSoundCmd(void)
 *
 * @return FirstEntry != LastEntry
 */
int HasSoundCmd(void) {
    return FirstEntry != LastEntry;
}


/**
 * DequeueSoundCmd__FR13ESoundCmdTypeRi
 *
 * Address:   0x80061224
 * Mangled:   DequeueSoundCmd__FR13ESoundCmdTypeRi
 * Demangled: DequeueSoundCmd(ESoundCmdType &, int &)
 *
 * @param SoundCmdType  Receives the oldest queued operation.
 * @param SoundNum  Receives the oldest queued sound identifier.
 */
void DequeueSoundCmd(ESoundCmdType &SoundCmdType, int &SoundNum) {
    SoundCmdType = SoundCmd[FirstEntry].SoundCmdType;
    SoundNum = SoundCmd[FirstEntry].SoundNum;
    FirstEntry = (FirstEntry + 1) % VideoSoundQueueCapacity;
}


/**
 * Video_SFXPlay__Fisi
 *
 * Address:   0x800612A0
 * Mangled:   Video_SFXPlay__Fisi
 * Demangled: Video_SFXPlay(int, short, int)
 *
 * @param Sound  Sound identifier to capture while recording.
 * @param Vol  Runtime volume; replay capture records only the identifier.
 * @param PitchOffset  Runtime pitch offset; replay capture records only the
 *                     identifier.
 */
void Video_SFXPlay(int Sound, short Vol, int PitchOffset) {
    if (GVInfo1.VideoMode == VIDEO_RECORD) {
        if (GamePaused == 0) {
            QueueSoundCmd(SFX_PLAY, Sound);
        }
    }
}


/**
 * InitBloodSplat__Fv
 *
 * Address:   0x800612E8
 * Mangled:   InitBloodSplat__Fv
 * Demangled: InitBloodSplat(void)
 *
 */
void InitBloodSplat(void) {
    Splat2 = -1;
    Splat1 = -1;
}


/**
 * Video_BloodSplat__FP6CBruceii
 *
 * Address:   0x800612FC
 * Mangled:   Video_BloodSplat__FP6CBruceii
 * Demangled: Video_BloodSplat(CBruce *, int, int)
 *            Record which player's body part should emit a blood splat during video capture.
 *
 * @param pSkater  Active skater being processed.
 * @param BodyPart Body-part index to record for the active player.
 * @param MaxBloodBits Bit width accepted for compatibility with the replay event interface.
 */
void Video_BloodSplat(CBruce *pSkater, int BodyPart, int MaxBloodBits) {
    if (GVInfo1.VideoMode == VIDEO_RECORD) {
        if (&pSkater->CSuper.CBody == MechList) {
            Splat1 = BodyPart;
            return;
        }
        Splat2 = BodyPart;
    }
}


/**
 * InitTrickObs__Fv
 *
 * Address:   0x80061340
 * Mangled:   InitTrickObs__Fv
 * Demangled: InitTrickObs(void)
 *
 */
void InitTrickObs(void) {
    TrickOb2 = -1;
    TrickOb1 = -1;
}


/**
 * Video_HighlightStart__FP6CBruce
 *
 * Address:   0x80061354
 * Mangled:   Video_HighlightStart__FP6CBruce
 * Demangled: Video_HighlightStart(CBruce *)
 *
 * @param pSkater  Active skater being processed.
 */
void Video_HighlightStart(CBruce *pSkater) {
    SVideoInfo *VInfo;
    if (&pSkater->CSuper.CBody == MechList) {
        VInfo = &GVInfo1;
    } else {
        VInfo = &GVInfo2;
    }
    if (HLStartCalled == 0) {
        HLStartCalled = 1;
        HighlightStartFrame = VInfo->NumFrames;
        HighlightStartY = pSkater->CSuper.CBody.CItem.mPos.vy;
    }
}


/**
 * Video_HighlightEnd__FP6CBrucei
 *
 * Address:   0x800613A8
 * Mangled:   Video_HighlightEnd__FP6CBrucei
 * Demangled: Video_HighlightEnd(CBruce *, int)
 *
 * @param pSkater  Active skater being processed.
 * @param Score  Score value.
 */
void Video_HighlightEnd(CBruce *pSkater, int Score) {
    SVideoInfo *VInfo;
    int WasABail;
    int i;
    int j;

    if (&pSkater->CSuper.CBody == MechList) {
        VInfo = &GVInfo1;
    } else {
        VInfo = &GVInfo2;
    }

    HLStartCalled = 0;
    HighlightEndFrame = VInfo->NumFrames;
    HighlightEndY = pSkater->CSuper.CBody.CItem.mPos.vy;

    WasABail = static_cast<unsigned int>(Score) >> 31;
    if (Score < 0) {
        Score = -Score;
    }

    if (Score == 0) {
        Score = (Video_Abs(HighlightEndY - HighlightStartY) << 1) >> 12;
    } else {
        Score += Video_Abs(HighlightEndY - HighlightStartY) >> 13;
    }

    i = 0;
    do {
        if (VInfo->Highlights[i].Score < Score) {
            j = VideoHighlightCount - 1;
            if (i < VideoHighlightCount - 1) {
                do {
                    VInfo->Highlights[j] = VInfo->Highlights[j - 1];
                    j--;
                } while (i < j);
            }

            HighlightStartFrame -= 15;
            if (HighlightStartFrame < 0) {
                HighlightStartFrame = 0;
            }

            if (WasABail != 0) {
                HighlightEndFrame += 30;
            } else {
                HighlightEndFrame += 8;
            }
            if (HighlightEndFrame > VideoMaxHighlightFrame) {
                HighlightEndFrame = VideoMaxHighlightFrame;
            }

            VInfo->Highlights[i].FromFrame = HighlightStartFrame;
            VInfo->Highlights[i].ToFrame = HighlightEndFrame;
            VInfo->Highlights[i].Score = Score;
            return;
        }
        i++;
    } while (i < VideoHighlightCount);

}


/**
 * Video_SortAndMerge__Fv
 *
 * Address:   0x80061534
 * Mangled:   Video_SortAndMerge__Fv
 * Demangled: Video_SortAndMerge(void)
 *            Sort recorded highlights by start frame, merge overlapping
 *            ranges, and compact the surviving entries back into GVInfo1.
 *
 */
void Video_SortAndMerge(void) {
    SHighlight TempHL[VideoHighlightCount];
    SHighlight NullHL;

    NullHL.Score = 0;
    NullHL.ToFrame = 0;
    NullHL.FromFrame = 0;

    // Phase 1: Extract entries ordered by earliest start frame
    {
        int i = 0;
        do {
            int BestFrom;
            int BestIndex;
            int j;

            TempHL[i] = NullHL;
            BestFrom = VideoNoEndFrame;
            BestIndex = -1;
            j = 0;
            do {
                if (static_cast<unsigned int>(
                        static_cast<int>(GVInfo1.Highlights[j].ToFrame)) >=
                    GVInfo1.NumFrames) {
                    GVInfo1.Highlights[j].ToFrame =
                        static_cast<unsigned short>(GVInfo1.NumFrames) - 1;
                }
                if (GVInfo1.Highlights[j].Score != 0 &&
                    static_cast<int>(GVInfo1.Highlights[j].FromFrame) < BestFrom) {
                    BestFrom = GVInfo1.Highlights[j].FromFrame;
                    BestIndex = j;
                }

                j++;
            } while (j < VideoHighlightCount);

            if (BestIndex != -1) {
                TempHL[i] = GVInfo1.Highlights[BestIndex];
                GVInfo1.Highlights[BestIndex].Score = 0;
            }

            i++;
        } while (i < VideoHighlightCount);
    }

    // Phase 2: Merge overlapping/adjacent entries (backward)
    {
        int i;
        for (i = VideoHighlightCount - 1; i > 0; i--) {
            if (static_cast<int>(TempHL[i].FromFrame) <=
                    static_cast<int>(TempHL[i - 1].ToFrame) + 15 &&
                static_cast<int>(TempHL[i].FromFrame) >
                    static_cast<int>(TempHL[i - 1].FromFrame)) {
                if (static_cast<int>(TempHL[i - 1].ToFrame) <
                    static_cast<int>(TempHL[i].ToFrame)) {
                    TempHL[i - 1].ToFrame = TempHL[i].ToFrame;
                }
                TempHL[i - 1].Score += TempHL[i].Score;
                TempHL[i] = NullHL;
            }
        }
    }

    // Phase 3: Write back compacted entries
    {
        int i = 0;
        SHighlight *Highlights = GVInfo1.Highlights;
        // Clear every destination while independently advancing the compacted
        // output; separate cursors keep both traversals explicit.
        SHighlight *pSource = TempHL;
        int *pScore = &TempHL[0].Score;
        SHighlight *pClear = Highlights;
        SHighlight *pCompact = Highlights;

        do {
            *pClear = NullHL;
            if (*pScore != 0) {
                *pCompact = *pSource;
                pCompact++;
            }
            pSource++;
            pScore += 2;
            i++;
            pClear++;
        } while (i < VideoHighlightCount);
    }
}


/**
 * Video_SetHeader__FP10SVideoHeadi
 *
 * Address:   0x80061764
 * Mangled:   Video_SetHeader__FP10SVideoHeadi
 * Demangled: Video_SetHeader(SVideoHead *, int)
 *            Copy replay metadata and the selected skater identities into a video header.
 *
 * @param Head  Header to populate.
 * @param SkaterNum  Primary skater index for the recording.
 */
void Video_SetHeader(SVideoHead *Head, int SkaterNum) {
    SVideoInfo VInfo = SkaterNum != 0 ? GVInfo2 : GVInfo1;

    int i = 0;
    Head->NumFrames = VInfo.NumFrames;

    do {
        Head->Highlights[i] = VInfo.Highlights[i];
        i++;
    } while (i < VideoHighlightCount);

    unsigned short numPlayers = static_cast<unsigned short>(GNumberOfPlayers);
    CBruce *pSkater = Video_GetBruce(MechList);
    Head->NumSkaters = numPlayers;
    CBruce *pSkater2 = Video_GetNextBruce(MechList);

    switch (SkaterNum) {
    case 0:
        Head->Skater = pSkater->mSkaterType;
        if (pSkater2 != 0) {
            Head->Skater2 = pSkater2->mSkaterType;
        }
        break;
    case 1:
        Head->Skater = pSkater2->mSkaterType;
        if (pSkater2 != 0) {
            Head->Skater2 = pSkater->mSkaterType;
        }
        break;
    }

    unsigned short level = static_cast<unsigned short>(GLevel);
    unsigned short game = static_cast<unsigned short>(GGame);
    Head->Checksum = 0;
    Head->Level = level;
    Head->GGame = game;
}


/**
 * Video_InitOnce__Fv
 *
 * Address:   0x80061870
 * Mangled:   Video_InitOnce__Fv
 * Demangled: Video_InitOnce(void)
 *            Allocate replay buffers for active video slots.
 */
void Video_InitOnce(void) {
    GVInfo1.pBlockStart = GVInfo1.pStreamStart =
        static_cast<ulong *>(
            Mem_New(VideoStreamWords * sizeof(ulong), 1, 1));
    if (GNumberOfPlayers == 2) {
        GVInfo2.pBlockStart = GVInfo2.pStreamStart =
            static_cast<ulong *>(
                Mem_New(VideoStreamWords * sizeof(ulong), 1, 1));
    } else {
        GVInfo2.pStreamStart = 0;
        GVInfo2.pBlockStart = 0;
    }
}


/**
 * Video_Init__Fv
 *
 * Address:   0x800618EC
 * Mangled:   Video_Init__Fv
 * Demangled: Video_Init(void)
 */
void Video_Init(void) {
}


/**
 * Video_SyncObs__Fi
 *
 * Address:   0x800618F4
 * Mangled:   Video_SyncObs__Fi
 * Demangled: Video_SyncObs(int)
 *
 * @param NumFrames  Number of recorded frames to simulate without replay audio.
 */
void Video_SyncObs(int NumFrames) {
    CBruce *pSkater = Video_GetBruce(MechList);
    ReplaySoundLockout = 1;
    CBruce *pSkater2 = Video_GetNextBruce(MechList);
    int i;
    for (i = 0; i < NumFrames; i++) {
        Ob_ClearInputFlags(BaddyList);
        Ob_ClearInputFlags(EnvironmentalObjectList);
        GetDataChunk(pSkater, -1);
        if (pSkater2 != 0) {
            GetDataChunk(pSkater2, -1);
        }
        Ob_QuickAI(&EnvironmentalObjectList);
        Ob_AI(&BaddyList);
    }
    ReplaySoundLockout = 0;
}


/**
 * Video_SyncObsIfNeeded__Fv
 *
 * Address:   0x800619B4
 * Mangled:   Video_SyncObsIfNeeded__Fv
 * Demangled: Video_SyncObsIfNeeded(void)
 */
void Video_SyncObsIfNeeded(void) {
    if (GVideoReplay == 1) {
        if (GVInfo1.Highlights[0].FromFrame != 1) {
            Video_SyncObs(GVInfo1.Highlights[0].FromFrame - 1);
        }
    } else {
        int t1 = GVInfo1.Highlights[GVideoReplay - 1].FromFrame;
        int t2 = GVInfo1.Highlights[GVideoReplay - 2].ToFrame;
        Video_SyncObs(t1 - t2 - 2);
    }
}


/**
 * Video_Cleanup__Fv
 *
 * Address:   0x80061A30
 * Mangled:   Video_Cleanup__Fv
 * Demangled: Video_Cleanup(void)
 */
void Video_Cleanup(void) {
    GVInfo1.VideoMode = VIDEO_OFF;
    GVInfo2.VideoMode = VIDEO_OFF;
}


/**
 * Video_Start__FP6CBruce10EVideoModeii
 *
 * Address:   0x80061A44
 * Mangled:   Video_Start__FP6CBruce10EVideoModeii
 * Demangled: Video_Start(CBruce *, EVideoMode, int, int)
 *            Initialize the selected skater's replay stream for recording or playback.
 *
 * @param pSkater  Active skater being processed.
 * @param mode  Replay mode to enter.
 * @param startFrame  First playback frame.
 * @param endFrame  Last playback frame, or `0x7fffffff` for the stored frame count.
 */
void Video_Start(CBruce *pSkater, EVideoMode VMode, int StartFrame, int EndFrame) {
    SVideoInfo *VInfo;
    if (&pSkater->CSuper.CBody == MechList) {
        VInfo = &GVInfo1;
    } else {
        VInfo = &GVInfo2;
    }

    HLStartCalled = 0;
    InitSoundCmdQueue();
    InitBloodSplat();
    InitTrickObs();

    VInfo->VideoMode = VMode;

    if (VMode == VIDEO_RECORD) {
        // Recording mode: init bit writer
        {
            int i = 0;
            int *pScore = &VInfo->Highlights[0].Score;
            VInfo->NumFrames = 0;
            VInfo->BitsLeft = 32;
            do {
                *pScore = 0;
                i++;
                pScore += 2;
            } while (i < VideoHighlightCount);
        }

        // Clear write buffer
        {
            ulong *pWord = VInfo->pBlockStart;
            int i = 0;
            VInfo->ReplayHighlight = 0;
            do {
                *pWord = 0;
                i++;
                pWord++;
            } while (i < VideoStreamWords);
        }
    } else {
        VInfo->BitsLeft = 0;
    }

    VInfo->Bits = 0;
    VInfo->CurCameraIndex = -1;
    VInfo->OldCameraIndex = -2;
    VInfo->CamXblanks = 0;
    VInfo->NoLOS = 0;
    VInfo->TabuCam = -1;
    VInfo->pStream = VInfo->pStreamStart;
    VInfo->FramesLeft = VInfo->NumFrames;

    if (EndFrame != VideoNoEndFrame) {
        VInfo->FramesLeft = EndFrame - StartFrame + 1;
    }

    int xblanks = Xblanks;
    VInfo->OldPos.vz = 0;
    VInfo->OldPos.vy = 0;
    VInfo->OldPos.vx = 0;
    VInfo->OldPos2.vz = 0;
    VInfo->OldPos2.vy = 0;
    VInfo->OldPos2.vx = 0;
    VInfo->Euler.vz = 0;
    VInfo->Euler.vy = 0;
    VInfo->Euler.vx = 0;
    VInfo->OldEuler.vz = 0;
    VInfo->OldEuler.vy = 0;
    VInfo->OldEuler.vx = 0;
    VInfo->OldPhysicsState = PHYSICS_ON_GROUND;
    VInfo->OldAnim = 0;
    VInfo->OldFrame = 0;
    VInfo->OldFlipAngle = 0;
    VInfo->PlaybackFrozen = 0;
    VInfo->OldXblanks = xblanks;
    VInfo->Yblanks = xblanks;
    pSkater->CSuper.mAnim = 0;
    pSkater->CSuper.mFrame = 0;
    pSkater->mFlipAngle = 0;
    pSkater->BoardOn();
}


/**
 * Video_Stop__FP6CBruce
 *
 * Address:   0x80061BDC
 * Mangled:   Video_Stop__FP6CBruce
 * Demangled: Video_Stop(CBruce *)
 *
 * @param pSkater  Active skater being processed.
 */
void Video_Stop(CBruce *pSkater) {
    SVideoInfo *VInfo;
    if (&pSkater->CSuper.CBody == MechList) {
        VInfo = &GVInfo1;
    } else {
        VInfo = &GVInfo2;
    }
    if (VInfo->VideoMode == VIDEO_RECORD) {
        FlushBits(VInfo);
    }
    VInfo->VideoMode = VIDEO_OFF;
    GVideoReplay = 0;
}


/**
 * Matrix2Euler__FP6MATRIXP7SVECTOR
 *
 * Address:   0x80061C38
 * Mangled:   Matrix2Euler__FP6MATRIXP7SVECTOR
 * Demangled: Matrix2Euler(MATRIX *, SVECTOR *)
 *
 * @param m  Rotation matrix to convert.
 * @param a  Receives the fixed-point Euler angles.
 */
void Matrix2Euler(MATRIX *m, SVECTOR *a) {
    MATRIX mm = *m;

    // Negate middle column: m[0][1], m[1][1], m[2][1]
    mm.m[0][1] = -static_cast<unsigned short>(mm.m[0][1]);
    mm.m[1][1] = -static_cast<unsigned short>(mm.m[1][1]);
    mm.m[2][1] = -static_cast<unsigned short>(mm.m[2][1]);

    // ratan2(m[0][2], m[2][2]) for Y angle
    int y = ratan2(mm.m[0][2], mm.m[2][2]) & VideoAngleMask;
    RotMatrixY(-y, &mm);

    // ratan2(-m[1][2], m[2][2]) for X angle
    int x = ratan2(-mm.m[1][2], mm.m[2][2]) & VideoAngleMask;
    RotMatrixY(-x, &mm);

    // ratan2(m[1][0], m[1][1]) for Z angle
    int z = ratan2(mm.m[1][0], mm.m[1][1]);

    a->vy = y;
    z &= VideoAngleMask;
    a->vx = x;
    a->vz = z;
}


/**
 * PutDataChunk__FP6CBruce
 *
 * Address:   0x80061D34
 * Mangled:   PutDataChunk__FP6CBruce
 * Demangled: PutDataChunk(CBruce *)
 *
 * @param pSkater  Active skater being processed.
 */
static void PutDataChunk(CBruce *pSkater) {
    int i;
    int bx;
    int by;
    int bz;
    int PredX;
    int PredY;
    int PredZ;
    int DeltaX;
    int DeltaY;
    int DeltaZ;
    // The original source retained aggregate scratch vectors alongside the
    // scalar prediction and delta components consumed below.
    CVector PredPos;
    CVector DeltaPos;
    SVideoInfo *VInfo;

    if (&pSkater->CSuper.CBody == MechList)
        VInfo = &GVInfo1;
    else
        VInfo = &GVInfo2;

    if (VInfo->pStream + 6 >= VInfo->pStreamStart + VideoStreamWords) {
        Video_Stop(pSkater);
        return;
    }

    VInfo->NumFrames++;

    PredX = (VInfo->OldPos.vx << 1) - VInfo->OldPos2.vx;
    PredY = (VInfo->OldPos.vy << 1) - VInfo->OldPos2.vy;
    PredZ = (VInfo->OldPos.vz << 1) - VInfo->OldPos2.vz;

    DeltaX = (pSkater->CSuper.CBody.CItem.mPos.vx >> 12) - PredX;
    DeltaY = (pSkater->CSuper.CBody.CItem.mPos.vy >> 12) - PredY;
    DeltaZ = (pSkater->CSuper.CBody.CItem.mPos.vz >> 12) - PredZ;

    VInfo->OldPos2 = VInfo->OldPos;
    VInfo->OldPos = pSkater->CSuper.CBody.CItem.mPos >> 12;

    bx = BitsRequired(DeltaX);
    by = BitsRequired(DeltaY);
    bz = BitsRequired(DeltaZ);
    if (bx < by)
        bx = by;
    if (bx < bz)
        bx = bz;

    for (i = 0; i < 6 && PosBits[i] < bx; i++) {
    }

    PutNBits(VInfo, PrefixCode[i], PrefixBits[i]);
    PutNBits(VInfo, DeltaX, PosBits[i]);
    PutNBits(VInfo, DeltaY, PosBits[i]);
    PutNBits(VInfo, DeltaZ, PosBits[i]);

    Matrix2Euler(&pSkater->mRotationMatrix, &VInfo->Euler);

    VInfo->Euler.vx &= VideoAngleMask;
    DeltaX = (VInfo->Euler.vx - VInfo->OldEuler.vx + VideoAngleWrapBias) &
        VideoAngleMask;
    VInfo->Euler.vy &= VideoAngleMask;
    DeltaY = (VInfo->Euler.vy - VInfo->OldEuler.vy + VideoAngleWrapBias) &
        VideoAngleMask;
    VInfo->Euler.vz &= VideoAngleMask;
    DeltaZ = (VInfo->Euler.vz - VInfo->OldEuler.vz + VideoAngleWrapBias) &
        VideoAngleMask;

    if (DeltaX >= VideoHalfTurn)
        DeltaX -= VideoFullTurn;
    if (DeltaY >= VideoHalfTurn)
        DeltaY -= VideoFullTurn;
    if (DeltaZ >= VideoHalfTurn)
        DeltaZ -= VideoFullTurn;

    VInfo->OldEuler = VInfo->Euler;

    bx = BitsRequired(DeltaX);
    by = BitsRequired(DeltaY);
    bz = BitsRequired(DeltaZ);
    if (bx < by)
        bx = by;
    if (bx < bz)
        bx = bz;

    for (i = 0; i < 6 && EulerBits[i] < bx; i++) {
    }

    PutNBits(VInfo, PrefixCode[i], PrefixBits[i]);
    PutNBits(VInfo, DeltaX, EulerBits[i]);
    PutNBits(VInfo, DeltaY, EulerBits[i]);
    PutNBits(VInfo, DeltaZ, EulerBits[i]);

    if (pSkater->mPhysicsState == VInfo->OldPhysicsState)
        PutNBits(VInfo, 0, 1);
    else
        PutNBits(VInfo, (pSkater->mPhysicsState << 1) | 1, 5);
    VInfo->OldPhysicsState = pSkater->mPhysicsState;

    if (pSkater->CSuper.mAnim == VInfo->OldAnim)
        PutNBits(VInfo, 0, 1);
    else
        PutNBits(VInfo, (pSkater->CSuper.mAnim << 1) | 1, 9);
    VInfo->OldAnim = pSkater->CSuper.mAnim;

    DeltaX = pSkater->CSuper.mFrame - VInfo->OldFrame;
    if (static_cast<unsigned int>(DeltaX) < 4)
        PutNBits(VInfo, DeltaX << 1, 3);
    else
        PutNBits(VInfo, (pSkater->CSuper.mFrame << 1) | 1, 9);
    VInfo->OldFrame = pSkater->CSuper.mFrame;

    PutNBits(VInfo, (pSkater->CSuper.mExtraFlags >> 1) & 1, 1);

    if (pSkater->mFlipNum == 0) {
        PutNBits(VInfo, 0, 1);
        VInfo->OldFlipAngle = 0;
        VInfo->FlipAngleDelta = 0;
    } else {
        PutNBits(VInfo, 1, 1);
        int Delta = pSkater->mFlipAngle -
                    (VInfo->OldFlipAngle + VInfo->FlipAngleDelta);
        int Bits = 0;
        if (Delta != 0)
            Bits = BitsRequired(Delta);

        for (i = 0; i < 6 && FlipAngleBits[i] < Bits; i++) {
        }

        PutNBits(VInfo, PrefixCode[i], PrefixBits[i]);
        PutNBits(VInfo, Delta, FlipAngleBits[i]);
        VInfo->OldFlipAngle = pSkater->mFlipAngle;
        VInfo->FlipAngleDelta = Delta;
    }

    DeltaX = Xblanks - VInfo->OldXblanks;
    if (DeltaX >= 6) {
        VInfo->OldXblanks = Xblanks + 5;
        VInfo->OldXblanks -= DeltaX;
        DeltaX = 5;
    } else {
        VInfo->OldXblanks = Xblanks;
    }
    PutNBits(VInfo, DeltaX - 2, 2);

    // Sound command encoding
    if (HasSoundCmd()) {
        PutNBits(VInfo, 1, 1);
        ESoundCmdType SoundCmdType;
        int SoundNum;
        DequeueSoundCmd(SoundCmdType, SoundNum);
        if (SoundCmdType == SFX_PLAY) {
            PutNBits(VInfo, 1, 1);
            PutNBits(VInfo, SoundNum, 8);
        } else if (SoundCmdType == SFX_STOP) {
            PutNBits(VInfo, 0, 1);
            PutNBits(VInfo, SoundNum, 8);
        }
    } else {
        PutNBits(VInfo, 0, 1);
    }

    if (Splat1 != -1 || Splat2 != -1 ||
        TrickOb1 != -1 || TrickOb2 != -1) {
        PutNBits(VInfo, 1, 1);

        if (&pSkater->CSuper.CBody == MechList) {
            if (Splat1 != -1) {
                PutNBits(VInfo, 1, 1);
                PutNBits(VInfo, Splat1, 2);
                Splat1 = -1;
            } else {
                PutNBits(VInfo, 0, 1);
            }
        } else {
            if (Splat2 != -1) {
                PutNBits(VInfo, 1, 1);
                PutNBits(VInfo, Splat2, 2);
                Splat2 = -1;
            } else {
                PutNBits(VInfo, 0, 1);
            }
        }

        if (&pSkater->CSuper.CBody == MechList) {
            if (TrickOb1 != -1) {
                PutNBits(VInfo, 1, 1);
                PutNBits(VInfo, TrickOb1, 8);
                TrickOb1 = -1;
            } else {
                PutNBits(VInfo, 0, 1);
            }
        } else {
            if (TrickOb2 != -1) {
                PutNBits(VInfo, 1, 1);
                PutNBits(VInfo, TrickOb2, 8);
                TrickOb2 = -1;
            } else {
                PutNBits(VInfo, 0, 1);
            }
        }
    } else {
        PutNBits(VInfo, 0, 1);
    }
}


/**
 * Video_Record__FP6CBruce
 *
 * Address:   0x800624D8
 * Mangled:   Video_Record__FP6CBruce
 * Demangled: Video_Record(CBruce *)
 *
 * @param pSkater  Active skater being processed.
 */
void Video_Record(CBruce *pSkater) {
    if (GState == 1) {
        Video_Stop(pSkater);
    } else {
        PutDataChunk(pSkater);
    }
}


/**
 * GetDataChunk__FP6CBrucei
 *
 * Address:   0x80062518
 * Mangled:   GetDataChunk__FP6CBrucei
 * Demangled: GetDataChunk(CBruce *, int)
 *
 * @param pSkater  Active skater being processed.
 * @param NChunks  Number of replay chunks being applied in this update.
 */
static void GetDataChunk(CBruce *pSkater, int NChunks) {
    int i;
    int DeltaX;
    int DeltaY;
    int DeltaZ;
    SVideoInfo *VInfo;

    if (&pSkater->CSuper.CBody == MechList)
        VInfo = &GVInfo1;
    else
        VInfo = &GVInfo2;

    {
    pSkater->CSuper.CBody.mOldPos = pSkater->CSuper.CBody.CItem.mPos;

    for (i = 0; i < 6; i++) {
        if (!GetBit(VInfo))
            break;
    }

    pSkater->CSuper.CBody.CItem.mPos.vx =
        (VInfo->OldPos.vx << 1) - VInfo->OldPos2.vx +
        GetNBitsSigned(VInfo, PosBits[i]);
    pSkater->CSuper.CBody.CItem.mPos.vy =
        (VInfo->OldPos.vy << 1) - VInfo->OldPos2.vy +
        GetNBitsSigned(VInfo, PosBits[i]);
    pSkater->CSuper.CBody.CItem.mPos.vz =
        (VInfo->OldPos.vz << 1) - VInfo->OldPos2.vz +
        GetNBitsSigned(VInfo, PosBits[i]);

    VInfo->OldPos2 = VInfo->OldPos;
    VInfo->OldPos = pSkater->CSuper.CBody.CItem.mPos;
    pSkater->CSuper.CBody.CItem.mPos <<= 12;
    pSkater->CSuper.CBody.mVel = pSkater->CSuper.CBody.CItem.mPos -
                                      pSkater->CSuper.CBody.mOldPos;

    for (i = 0; i < 6; i++) {
        if (!GetBit(VInfo))
            break;
    }

    DeltaX = GetNBitsSigned(VInfo, EulerBits[i]) & VideoAngleMask;
    DeltaY = GetNBitsSigned(VInfo, EulerBits[i]) & VideoAngleMask;
    DeltaZ = GetNBitsSigned(VInfo, EulerBits[i]) & VideoAngleMask;
    VInfo->Euler.vx =
        (VInfo->OldEuler.vx + DeltaX + VideoAngleWrapBias) & VideoAngleMask;
    VInfo->Euler.vy =
        (VInfo->OldEuler.vy + DeltaY + VideoAngleWrapBias) & VideoAngleMask;
    VInfo->Euler.vz =
        (VInfo->OldEuler.vz + DeltaZ + VideoAngleWrapBias) & VideoAngleMask;
    VInfo->OldEuler = VInfo->Euler;

    if (NChunks == 1) {
        RotMatrixYXZ_gte(&VInfo->Euler, &pSkater->mRotationMatrix);
        pSkater->mRotationMatrix.m[0][1] =
            -pSkater->mRotationMatrix.m[0][1];
        pSkater->mRotationMatrix.m[1][1] =
            -pSkater->mRotationMatrix.m[1][1];
        pSkater->mRotationMatrix.m[2][1] =
            -pSkater->mRotationMatrix.m[2][1];
        pSkater->FRUVectorsFromRotMatrix();
        M3d_BuildTransform(reinterpret_cast<CSuper *>(&pSkater->CSuper));
    }

    if (GetBit(VInfo) == 1)
        pSkater->mPhysicsState =
            static_cast<EPhysicsState>(GetNBitsUnsigned(VInfo, 4));

    if (GetBit(VInfo) == 1)
        pSkater->CSuper.mAnim =
            GetNBitsUnsigned(VInfo, 8);
    VInfo->OldAnim = pSkater->CSuper.mAnim;

    if (!GetBit(VInfo))
        pSkater->CSuper.mFrame =
            VInfo->OldFrame +
            GetNBitsUnsigned(VInfo, 2);
    else
        pSkater->CSuper.mFrame =
            GetNBitsUnsigned(VInfo, 8);
    VInfo->OldFrame = pSkater->CSuper.mFrame;

    if (GetBit(VInfo))
        pSkater->CSuper.mExtraFlags |= 2;
    else
        pSkater->CSuper.mExtraFlags &= ~2;

    // Decode the compressed flip state.
    if (!GetBit(VInfo)) {
        VInfo->OldFlipAngle = 0;
        VInfo->FlipAngleDelta = 0;
        pSkater->mFlipAngle = 0;
        pSkater->mFlipNum = 0;
    } else {
        pSkater->mFlipNum = 1000;
        for (i = 0; i < 6; i++) {
            if (!GetBit(VInfo))
                break;
        }

        if (FlipAngleBits[i]) {
            int Delta = GetNBitsSigned(VInfo, FlipAngleBits[i]);
            pSkater->mFlipAngle =
                VInfo->OldFlipAngle + VInfo->FlipAngleDelta + Delta;
            VInfo->OldFlipAngle = pSkater->mFlipAngle;
            VInfo->FlipAngleDelta = Delta;
        }
    }

    VInfo->Yblanks +=
        2 + GetNBitsUnsigned(VInfo, 2);
    if (NChunks == 1) {
        int Delta = VInfo->Yblanks - Xblanks;
        for (i = 0; i < Delta; i++)
            VSync(0);
    }

    // Read sound command
    if (GetBit(VInfo)) {
        if (GetBit(VInfo)) {
            int SoundNum =
                GetNBitsUnsigned(VInfo, 8);
            if (!ReplaySoundLockout)
                SFX_PlayX(SoundNum, 0x2000, 0);
        } else {
            int SoundNum =
                GetNBitsUnsigned(VInfo, 8);
            if (!ReplaySoundLockout)
                SFX_Stop(SoundNum);
        }
    }

    // Replay blood-splat and trick-observation events.
    if (GetBit(VInfo)) {
        if (GetBit(VInfo))
            pSkater->BloodSplat(
                GetNBitsUnsigned(VInfo, 2),
                50);

        // Read trick observation
        if (GetBit(VInfo)) {
            int TrickObIndex =
                GetNBitsUnsigned(VInfo, 8);
            CTrickOb *pTrickOb = TrickObList;
            for (int i = 0; i < TrickObIndex; i++)
                pTrickOb = pTrickOb->mpNext;

            pTrickOb->mpItem =
                Spool_FindEnviroItem(pTrickOb->mChecksum);
            pTrickOb->mpItem->Flags |= 0x400;
            pTrickOb->mpItem->RGB = 0x202020;
            pTrickOb->mState = 1;
            if (&pSkater->CSuper.CBody == MechList)
                pTrickOb->mPlayer = 1;
            else
                pTrickOb->mPlayer = 2;
            TrickPlayer = pTrickOb->mPlayer;
            Trig_SendPulse(Trig_GetLinksPointer(pTrickOb->mNode));
        }
    }

    // Replay trigger and power-up crossings along a non-zero previous path.
    if (VInfo->OldPos2.vx || VInfo->OldPos2.vy || VInfo->OldPos2.vz) {
        SLineInfo LInfo;
        int Player;
        CVector nearest;
        CVector Start;
        CVector End;

        LInfo.StartCoords.vx = VInfo->OldPos2.vx << 12;
        LInfo.StartCoords.vy = VInfo->OldPos2.vy << 12;
        LInfo.StartCoords.vz = VInfo->OldPos2.vz << 12;
        LInfo.EndCoords.vx = VInfo->OldPos.vx << 12;
        LInfo.EndCoords.vy = VInfo->OldPos.vy << 12;
        LInfo.EndCoords.vz = VInfo->OldPos.vz << 12;
        M3dColij_InitLineInfo(&LInfo);

        Player = pSkater->mIndex + 1;
        LInfo.RecordTriggerZoneHits = 1;
        Trig_ResetCPCollisionFlags(Player);
        pCurrentPlayer = pSkater;
        M3dZone_LineToItem(&LInfo, 1);
        pCurrentPlayer = 0;
        Trig_ResetCPExecutedFlags(Player);

        Start.vx = VInfo->OldPos2.vx << 12;
        Start.vy = VInfo->OldPos2.vy << 12;
        Start.vz = VInfo->OldPos2.vz << 12;
        End.vx = VInfo->OldPos.vx << 12;
        End.vy = VInfo->OldPos.vy << 12;
        End.vz = VInfo->OldPos.vz << 12;

        Video_CollectBody(
            M3dColij_LineToSphere(
                Start, End, nearest, PowerUpList, 0, 4096));
    }

    switch (pSkater->CSuper.mAnim) {
    case 0x2c:
    case 0x2e:
    case 0x37:
        pSkater->BoardOff();
        break;
    case 0x36:
        if (pSkater->CSuper.mFrame < 0x70)
            pSkater->BoardOff();
        else
            pSkater->BoardOn();
        break;
    default:
        pSkater->BoardOn();
        break;
    }
    }

    if (NChunks != 1)
        VInfo->Yblanks = Xblanks;
}


/**
 * Video_Playback__FP6CBruce
 *
 * Address:   0x80062C88
 * Mangled:   Video_Playback__FP6CBruce
 * Demangled: Video_Playback(CBruce *)
 *
 * @param pSkater  Active skater being processed.
 */
void Video_Playback(CBruce *pSkater) {
    SVideoInfo *VInfo;
    int IsAGrind;
    CSVector Angles;
    CSVector cone;
    CVector SparkPos;

    CamPt_SetClosestCamera(pSkater);

    if (&pSkater->CSuper.CBody == MechList) {
        VInfo = &GVInfo1;
    } else {
        VInfo = &GVInfo2;
    }

    if (Status == STATUS_ENDHEATREPLAY || Status == STATUS_ENDHEATLADDER ||
        Status == STATUS_ENDHEATJUDGES) {
        if (VInfo->FramesLeft != 0) goto has_frames;
        RestartCompFrig();
        Video_Start(pSkater, VIDEO_PLAYBACK, 1, VideoNoEndFrame);
    }

    if (VInfo->FramesLeft == 0) goto end_playback;

has_frames:
    GetDataChunk(pSkater, 1);
    VInfo->FramesLeft = VInfo->FramesLeft - 1;

    {
        // Preserve the original branchless absolute-value macro expansion;
        // its Mips_DotProd2 argument is evaluated three times.
        IsAGrind =
            (Mips_DotProd2(-pSkater->mFront.vz,
                 pSkater->CSuper.CBody.mVel.vx, pSkater->mFront.vx,
                 pSkater->CSuper.CBody.mVel.vz) ^
             (Mips_DotProd2(-pSkater->mFront.vz,
                  pSkater->CSuper.CBody.mVel.vx, pSkater->mFront.vx,
                  pSkater->CSuper.CBody.mVel.vz) >> 31)) -
            (Mips_DotProd2(-pSkater->mFront.vz,
                 pSkater->CSuper.CBody.mVel.vx, pSkater->mFront.vx,
                 pSkater->CSuper.CBody.mVel.vz) >> 31);

        int inRange = (0x9C3F < IsAGrind) ^ 1;
        if (pSkater->mPhysicsState != PHYSICS_ON_RAIL) goto skip_sparks;
        if (inRange != 0) goto create_sparks;
        if (pSkater->CSuper.mAnim != 0x3a) goto skip_sparks;

    create_sparks:
        if (BitCount >= 200) goto skip_sparks;
        {
            int _x = VInfo->Euler.vx;
            int _y = VInfo->Euler.vy;
            int _z = VInfo->Euler.vz;
            Angles.vx = _x;
            Angles.vy = _y;
            Angles.vz = _z;
            Bit_SetSparkTrajectory(Angles);
            cone.vx = 0x1c2;
            cone.vy = 0x384;
            cone.vz = 0;
            Bit_SetSparkTrajectoryCone(cone);
            UseCacheForBits = 0;
            SparkPos.vz = 0;
            SparkPos.vx = 0;
            SparkPos.vy = 0x19000;

            int i = 0;
            do {
                CGrindSpark *pSpark =
                    static_cast<CGrindSpark *>(
                        CBit::operator new(BIT2_GRIND_SPARK_STORAGE_SIZE));
                if (pSpark != 0) {
                    Bit2_ConstructGrindSpark(
                        pSpark, &pSkater->CSuper.CBody,
                        SparkPos, -0x28, 0x1000, 5);
                }
                i++;
            } while (i < 9);

            UseCacheForBits = 1;
        }

    skip_sparks:
        if (pSkater->mPhysicsState != PHYSICS_ON_GROUND) goto do_shadow;
        if (Video_Abs(pSkater->CSuper.CBody.CItem.mPos.vy -
                pSkater->mLastShadowCheckHeight) < 65) {
            goto no_shadow;
        }
    }

do_shadow:
    pSkater->mLastShadowCheckHeight = pSkater->CSuper.CBody.CItem.mPos.vy;
    pSkater->DoShadowCheck();
    goto glow;

no_shadow:
    pSkater->CSuper.CBody.mShadowPos.vx =
        pSkater->CSuper.CBody.CItem.mPos.vx;
    pSkater->CSuper.CBody.mShadowPos.vz =
        pSkater->CSuper.CBody.CItem.mPos.vz;

glow:
    pSkater->GlowUpdate();
    Platform_HandleBounce(pSkater);
    pSkater->HandleWater();
    goto done;

end_playback:
    {
        int ReplayHighlight = VInfo->ReplayHighlight;
        if (static_cast<unsigned int>(ReplayHighlight - 1) <
            VideoHighlightCount - 1) {
            if (VInfo->Highlights[ReplayHighlight].Score != 0) {
                int EndFrame = VInfo->Highlights[ReplayHighlight].ToFrame;
                int StartFrame = VInfo->Highlights[ReplayHighlight].FromFrame;
                if (EndFrame != VideoNoEndFrame) {
                    VInfo->FramesLeft = EndFrame - StartFrame + 1;
                }
                GVideoReplay = GVideoReplay + 1;
                VInfo->ReplayHighlight = ReplayHighlight + 1;
                Video_SyncObsIfNeeded();
                goto done;
            }
        }
    }

    Video_Stop(pSkater);
    if (GNumberOfPlayers == 1) goto switch_screen;
    if (&pSkater->CSuper.CBody == MechList) goto switch_screen;
    goto done;

switch_screen:
    Front_SwitchOnVideoScreen();

done:;
}


/**
 * Video_HandleStepping__Fv
 *
 * Address:   0x80062FDC
 * Mangled:   Video_HandleStepping__Fv
 * Demangled: Video_HandleStepping(void)
 *
 */
void Video_HandleStepping(void) {
    static const int RepeatDelay = 12;

    if (GVInfo1.VideoMode == VIDEO_PLAYBACK && DemoTimer != 0) goto stepping;

    if (DemoTimer != 0) return;

    {
        int p;
        for (p = 0; p < 2; p++) {
            if (Pad[p].Triangle.Triggered || Pad[p].Square.Triggered ||
                Pad[p].LeftOne.Triggered || Pad[p].LeftTwo.Triggered ||
                Pad[p].RightOne.Triggered || Pad[p].RightTwo.Triggered ||
                Pad[p].Left.Triggered || Pad[p].Right.Triggered ||
                Pad[p].Up.Triggered || Pad[p].Down.Triggered ||
                Pad[p].Circle.Triggered || Pad[p].Select.Triggered) {
                Pad[p].X.Triggered = 1;
            }
        }
    }
    return;

    stepping:;
    {
        if (Status == STATUS_ENDHEATREPLAY) return;
        if (Status == STATUS_ENDHEATLADDER) return;
        if (Status == STATUS_ENDHEATJUDGES) return;

        if (Pad[0].Square.Triggered != 0) {
            goto toggle;
        }
        if (GNumberOfPlayers != GVInfo1.VideoMode) goto check_frozen;
        if (Pad[1].Square.Triggered == 0) goto check_frozen;

    toggle:
        Pad[0].Square.Triggered = 0;
        Pad[1].Square.Triggered = 0;
        GVInfo1.PlaybackFrozen ^= 1;
        GameFrozen = GVInfo1.PlaybackFrozen;
        MusicStopped = GVInfo1.PlaybackFrozen;

    check_frozen:;
        if (GVInfo1.PlaybackFrozen == 0) return;

        GameFrozen = 1;

        if (Pad[0].Circle.Pressed != 0) goto do_step;
        if (GNumberOfPlayers != 2) goto check_other;
        if (Pad[1].Circle.Pressed != 0) goto do_step;
        goto check_other;

    do_step:
        if (StepCount != 0) {
            StepCount--;
            return;
        }
        StepCount = RepeatDelay / 2;
        GameFrozen = 0;
        return;

    check_other:
        if (Pad[0].Triangle.Pressed != 0) goto do_step_alt;
        if (GNumberOfPlayers != 2) goto no_step;
        if (Pad[1].Triangle.Pressed != 0) goto do_step_alt;
        goto no_step;

    do_step_alt:
        if (StepCount != 0) {
            StepCount--;
            return;
        }
        StepCount = 1;
        GameFrozen = 0;
        return;

    no_step:
        StepCount = 0;
        return;
    }
}


// ====== 0x800632B4 _GLOBAL_.I.ReplaySoundLockout ===============================
// _GLOBAL_.I.ReplaySoundLockout [SKIP] — compiler-generated static init
