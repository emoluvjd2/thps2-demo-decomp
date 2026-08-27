// CAMPT.cpp -- Reconstructed translation unit
// Original: CAMPT.CPP
// Functions: 4 reconstructed entries
// Address range: 0x8004F7D4 - 0x8004F85C
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

extern "C" int CamPtIndex;
#define CAMERA_TUNING_ABSOLUTE_DECLARATIONS
#include "CAMERATUNING.h"

#include "globals.h"
#include "CAMPT.h"

/** Video state fields owned by the camera-point selection pass. */
struct SVideoInfo {
    int VideoMode;
    unsigned long *pBlockStart;
    unsigned long *pStreamStart;
    unsigned long *pStreamEnd;
    unsigned long *pStream;
    unsigned long Bits;
    int BitsLeft;
    unsigned long NumFrames;
    unsigned long FramesLeft;
    int CurCameraIndex;
    CVector CamPos;
    int CamMode;
    int CamXblanks;
    int TabuCam;
    int NoLOS;
    int OldCameraIndex;
    int TrailingCameraType;
    unsigned char _opaqueTail[192 - 76];
};

#include "SVIEWPORT.h"

/** CCamera fields written by camera-point mode changes. */
struct CCamera {
    unsigned char _padToPos[4];
    CVector mPos;
    unsigned char _padToTripod[0x3f4 - 0x10];
    CBruce *mTripod;
    unsigned char _padToCalcTripod[0x3fc - 0x3f8];
    int mCalcTripod;
    unsigned char _padToNewFocus[0x40c - 0x400];
    int new_focus;
    CVector mTripodPos;
    unsigned char _padToFocus[0x42c - 0x41c];
    CBruce *mFocus;
    int mCalcFocus;
    unsigned char _padToCameraMode[0x558 - 0x434];
    int mCameraMode;
    unsigned char _padToExtraYRot[0x5c4 - 0x55c];
    int extra_YRot;

    /** Select the active gameplay camera mode. */
    void SetMode(int mode) {
        mCameraMode = mode;
    }

    /** Follow a body as the camera's focus target. */
    void SetFocus(CBruce *focus) {
        mCalcFocus = 1;
        mFocus = focus;
        new_focus = 1;
    }

    /** Follow a body as the camera's tripod target. */
    void SetTripod(CBruce *tripod) {
        mCalcTripod = 1;
        mTripod = tripod;
    }

    /** Select an explicit tripod position. */
    void SetTripodPosition(const CVector &position) {
        mCalcTripod = 0;
        mTripodPos = position;
        mTripod = 0;
        new_focus = 1;
    }
};

/** CBruce slice read by the camera-point selection pass. */
struct CBruce {
    unsigned char _padToPos[4];
    CVector mPos;
    unsigned char _padToCamera[0xa74 - 0x10];
    CCamera *mpCamera;
    unsigned char _padToUp[0x10b8 - 0xa78];
    CVector mUp;
};

typedef char campt_video_info_size_must_be_192[
    (sizeof(SVideoInfo) == 192) ? 1 : -1];
typedef char campt_camera_slice_size_must_be_0x5c8[
    (sizeof(CCamera) == 0x5c8) ? 1 : -1];
typedef char campt_bruce_slice_size_must_be_0x10c4[
    (sizeof(CBruce) == 0x10c4) ? 1 : -1];

#include "TRIG.h"
#include "UTILS.h"


// === Owner-local globals ===
// $gp-relative camera point count (0x282c($gp)).
extern "C" int CamPtIndex
    __attribute__((section(".sdata"))) = 0;

// === Absolute-addressed tables ===
extern "C" int CamModes[70]
    __attribute__((section(".data"))) = {
        0x532, 0x550, 0x51e, 0x53c,
        0x532, 0x550, 0x51e, 0x800,
    };
extern "C" u16 CamPtNode[70]
    __attribute__((section(".data"))) = {};

// === Absolute-addressed globals ===
#include "GSKATER_STATE.h"
#define DPQGLOBALS_C_LINKAGE
#define DPQGLOBALS_DECLARE_HISTORY
#include "DPQGLOBALS.h"
#define VIDEOINFO_DECLARE_GVINFO1
#define VIDEOINFO_DECLARE_GVINFO2
#include "VIDEOINFO.h"

// === Function implementations (address order) ===

/**
 * CamPt_Init__Fv
 *
 * Address:   0x8004F7D4
 * Mangled:   CamPt_Init__Fv
 * Demangled: CamPt_Init(void)
 *            Assembly: sw $zero, 0x282c($gp); jr $ra; nop
 */
void CamPt_Init(void) {
    CamPtIndex = 0;
}


/**
 * CamPt_Cleanup__Fv
 *
 * Address:   0x8004F7E0
 * Mangled:   CamPt_Cleanup__Fv
 * Demangled: CamPt_Cleanup(void)
 *            Assembly: stores 0x122, 0x96, -0x6c to absolute globals, sets numCamPts=0
 */
void CamPt_Cleanup(void) {
    // Restore the camera-point defaults and clear the live count.
    int cleanupA = 0x122;
    int cleanupB = 0x96;
    camDist = cleanupA;
    int cleanupC = -0x6c;
    CamPtIndex = 0;
    camPitch = cleanupB;
    camYOffset = cleanupC;
}


/**
 * CamPt_New__Fi
 *
 * Address:   0x8004F810
 * Mangled:   CamPt_New__Fi
 * Demangled: CamPt_New(int)
 *            Add a new camera point and advance the live node count.
 *
 * @param Node  Rail / graph node identifier.
 */
void CamPt_New(int Node) {
    CVector Dummy;
    Trig_GetPosition(Dummy, Node);
    // Ordinary locals select $v1/$a0 and change eight of the 19 instructions.
    // Retain the target's MAIN.SYM-visible pointer/index registers at this ABI
    // boundary until an exact clean expression shape is found.
    register u16 *nodeBase asm("$2") = CamPtNode;
    register int nodeIndex asm("$6") = CamPtIndex;
    nodeBase[nodeIndex] = (u16)Node;
    CamPtIndex = nodeIndex + 1;
}


/**
 * CamPt_SetClosestCamera__FP6CBruce
 *
 * Address:   0x8004F85C
 * Mangled:   CamPt_SetClosestCamera__FP6CBruce
 * Demangled: CamPt_SetClosestCamera(CBruce *)
 *
 * @param pSkater  Active skater being processed.
 */
void CamPt_SetClosestCamera(CBruce *pSkater) {
    unsigned long ClosestDist = 0x7fffffff;
    unsigned long ClosestDist2;
    int Index = -1;
    int Index2 = -1;
    CVector CamPos;
    CVector ClosestPos;
    CVector ClosestPos2;
    CVector AdjustedPlayerPos;
    CVector Dummy;
    AdjustedPlayerPos = pSkater->mPos;
    AdjustedPlayerPos.vx += pSkater->mUp.vx * 110;
    AdjustedPlayerPos.vy += pSkater->mUp.vy * 110;
    AdjustedPlayerPos.vz += pSkater->mUp.vz * 110;

    SVideoInfo *VInfo;
    if (pSkater == GSkater[0]) {
        VInfo = &GVInfo1;
    } else {
        VInfo = &GVInfo2;
    }

    if (Utils_LineOfSight(AdjustedPlayerPos, pSkater->mpCamera->mPos)) {
        VInfo->NoLOS = 0;
    } else {
        ++VInfo->NoLOS;
        if (VInfo->NoLOS >= 11) {
            VInfo->TabuCam = VInfo->CurCameraIndex;
        }
    }

    int i = 0;
    if (CamPtIndex > 0) {
        short *pNode = (short *)CamPtNode;
        do {
            if (i != VInfo->TabuCam) {
                Trig_GetPosition(CamPos, *pNode);
                unsigned long Dist = Utils_CrapDist(pSkater->mPos, CamPos);
                if (Dist < ClosestDist) {
                    Index2 = Index;
                    ClosestDist2 = ClosestDist;
                    ClosestDist = Dist;
                    Index = i;
                    ClosestPos2 = ClosestPos;
                    ClosestPos = CamPos;
                }
            }
            ++pNode;
        } while (++i < CamPtIndex);
    }

    if ((Index != VInfo->CurCameraIndex) && (Index != -1) &&
        !Utils_LineOfSight(AdjustedPlayerPos, ClosestPos)) {
        if (Index2 != VInfo->CurCameraIndex) {
            if (Index2 != -1) {
                if (!Utils_LineOfSight(AdjustedPlayerPos, ClosestPos2)) {
                    Index = -1;
                } else {
                    Index = Index2;
                    ClosestPos = ClosestPos2;
                    ClosestDist = ClosestDist2;
                }
            } else {
                Index = -1;
            }
        } else {
            Index = -1;
        }
    }

    if (ClosestDist >= 9001) {
        Index = -1;
    }

    if ((Index != -1) && (Index != VInfo->CurCameraIndex) &&
        ((Index != VInfo->OldCameraIndex) ||
         (Xblanks >= (unsigned int)(VInfo->CamXblanks + 60)))) {
        // MAIN.SYM records pCam as a signed-word view: camera type and
        // parameter are signed fields in the returned node payload.
        short *pCam = reinterpret_cast<short *>(
            Trig_GetPosition(Dummy, (short)CamPtNode[Index]));
        int Type = pCam[0];
        int Param = pCam[1];

        CamModes[0] &= ~0xc00;
        if (Type == 1) {
            CamModes[0] |= 0x400;
        } else {
            CamModes[0] |= 0x800;
        }

        CamModes[Index] = CamModes[0] =
            (CamModes[0] & ~0xff) + (Rnd(5) + 5) * 10;

        if (ClosestDist > 4000) {
            int Val = 10000 - (int)ClosestDist;
            if (Val < 0) {
                Val = 0;
            }
            Val = Val / 200 + 10;
            CamModes[0] = (CamModes[0] & ~0xff) + Val;
        }

        unsigned int CurrentXblanks = Xblanks;
        int OldCameraIndex = VInfo->CurCameraIndex;
        VInfo->CurCameraIndex = Index;
        VInfo->CamXblanks = CurrentXblanks;
        VInfo->OldCameraIndex = OldCameraIndex;
        VInfo->CamPos = ClosestPos;

        int Mode = CamModes[Index];
        if (Mode & 0x800) {
            pSkater->mpCamera->SetMode(1);
            pSkater->mpCamera->SetFocus(pSkater);
            pSkater->mpCamera->SetTripod(pSkater);
            VInfo->TrailingCameraType = Param;
        } else if (Mode & 0x400) {
            pSkater->mpCamera->SetMode(2);
            pSkater->mpCamera->SetFocus(pSkater);
            pSkater->mpCamera->SetTripodPosition(ClosestPos);
        }
    }
    // Read current camera index and mode
    {
        Index = VInfo->CurCameraIndex;
        int Mode = CamModes[Index];

        if (Mode & 0x800) {
            pSkater->mpCamera->extra_YRot = 0;

            switch (VInfo->TrailingCameraType) {
                case 0:
                    camDist = 0xF5;
                    camPitch = 0xD7;
                    pCurrentViewport->TanHalf = 0x96A;
                    pSkater->mpCamera->extra_YRot = 0;
                    camYOffset = -0x46;
                    break;
                case 1:
                    camDist = 0xAF;
                    camPitch = 0x32;
                    pCurrentViewport->TanHalf = 0xB72;
                    pSkater->mpCamera->extra_YRot = 0;
                    camYOffset = -0x11;
                    break;
                case 2:
                    camDist = 0x122;
                    camPitch = 0x96;
                    pCurrentViewport->TanHalf = 0xD52;
                    pSkater->mpCamera->extra_YRot = 0;
                    camYOffset = -0x6C;
                    break;
                case 4:
                    camDist = 0xF5;
                    camPitch = 0xD7;
                    pCurrentViewport->TanHalf = 0x96A;
                    camYOffset = -0x46;
                    pSkater->mpCamera->extra_YRot = 0x800;
                    break;
                case 5:
                    camDist = 0xC3;
                    camPitch = 0x32;
                    pCurrentViewport->TanHalf = 0xB72;
                    camYOffset = -0x11;
                    pSkater->mpCamera->extra_YRot = 0x800;
                    break;
                case 6:
                    camDist = 0x122;
                    camPitch = 0x96;
                    pCurrentViewport->TanHalf = 0xD52;
                    camYOffset = -0x6C;
                    pSkater->mpCamera->extra_YRot = 0x800;
                    break;
                default:
                    break;
            }
        } else if (Mode & 0x400) {
            if (Mode & 0x200) {
                pCurrentViewport->TanHalf =
                    (0x100 - (Mode & 0xff)) * 10 + 300;
            } else if (Mode & 0x100) {
                int FogDist = Utils_Dist(pSkater->mPos, VInfo->CamPos);
                int Dist;
                if (FogDist > 2400) {
                    Dist = 2400;
                } else {
                    Dist = FogDist;
                }
                if (Dist < 450) {
                    Dist = 450;
                }

                int DpqRange = LastDpqMax - LastDpqMin;
                DpqMin = FogDist + LastDpqMin;
                DpqMax = DpqMin + DpqRange;
                pCurrentViewport->TanHalf =
                    (unsigned char)CamModes[Index] * 0x8552 / Dist;
            }
        }
    }
}
