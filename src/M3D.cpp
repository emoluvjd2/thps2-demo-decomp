// M3D.cpp -- Reconstructed translation unit
// Original: M3D.CPP
// Functions: 19 reconstructed
// Address range: 0x80094AB8 - 0x80097CD0
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#include <STDDEF.H>
#include <LIBGTE.H>
#include "SMODEL.h"
#include <INLINE_C.H>
#include "gte_extensions.h"
#include <LIBGPU.H>
#include "globals.h"
#include "UTILS.h"
#include "MAIN.h"
#include "DB.h"
#include "BADDY.h"
#define M3D_OWNER_DECLARATIONS
#define M3D_VERTEX_WIBBLE_LAYOUTS
#include "M3D.h"
#undef M3D_VERTEX_WIBBLE_LAYOUTS
#undef M3D_OWNER_DECLARATIONS

struct CBody;

#include "SCAMERA.h"
#include "TRIG.h"

#define VERTICES_OWNER_DECLARATION
#include "VERTICES.h"

/** Pulsing-colour keyframe entry. */
struct SRGBI {
    u8 r;
    u8 g;
    u8 b;
    u8 Interval;
};

/** Four texture-wibble vertices attached to one face. */
struct STexWibFaceInfo {
    STexWibVertInfo p0;
    STexWibVertInfo p1;
    STexWibVertInfo p2;
    STexWibVertInfo p3;
};

#define ENVREGIONS_SCALAR_ABSOLUTE_DECLARATION
#include "ENVREGIONS.h"
#define ENVIROLIST_ABSOLUTE_DECLARATION
#include "ENVIROLIST.h"
#include "GLEVEL.h"

#include "M3DELIPS.h"
#include "PSXREGION.h"

/** Draw-environment prefix consumed by the M3D frame setup. */
struct SDrawEnvLocal {
    RECT clip;
    u8 padding_0008[0x54];
};

/** Active render-buffer fields consumed by M3D primitive submission. */
struct SDoubleBufferLocal {
    SDrawEnvLocal Draw;
    u8 Disp[0x14];
    int Visible;
    int RenderNow;
    int Rendering;
    int FlipNow;
    int LastVblanks;
    u32 *PanelOT;
    u32 *OrderingTable;
};

#define SVIEWPORT_OWNER_DECLARATIONS
#include "SVIEWPORT.h"
#undef SVIEWPORT_OWNER_DECLARATIONS

#include "SLIGHT.h"

/** CItem prefix consumed by the item-lighting and reflection paths. */
struct M3dCItemRenderLayout {
    Item ItemBase;
    CSVector mScale;
    u32 mTRN;
    Item *mPreviousItem;
    u32 mStoredRGB;
    u8 mPredatorEffectType;
    u8 mPredatorEffectFade;
    u16 mType;
    SLight *mpLight;
};

/** Owner-local CSuper fields consumed while rendering articulated models. */
struct CSuper {
    u8 opaque_0000[0x88];
    u32 mEllipsoidRGB;
    u16 mEllipsoidScale;
    s16 mEllipsoidOTPushback;
    CVector mShadowPos;
    CSVector mShadowNormal;
    u8 opaque_00A2[0x66];
    u32 mExtraFlags;
    u8 opaque_010C[0x2c];
    u8 mShadowLights;
    u8 opaque_0139[3];
    SVECTOR *mpShadowSource;
    CVECTOR *mpShadowRGB;
    MATRIX mTransform;
    SMatrix *mpPoseBuffer;
    u8 opaque_0168[0x0c];
    u32 mSubObjectDisplayMask;
    u32 mUseAnimOrder;
    u8 mAnimOrder[19];
};

struct tag_S_Pal;

#include "TEXTURE.h"

#include "SANIMFRAME.h"

/** Derived object slice used by M3d_BuildTransform's matrix fast path. */
struct M3dSuperTransformLayout {
    Item ItemBase;
    u8 padding_0024[0xE4];
    u32 mExtraFlags;
    u8 padding_010C[0x38];
    MATRIX mTransform;
    u8 padding_0164[0xD2C];
    MATRIX mRotationMatrix;
};

typedef char M3d_SMatrix_size_must_be_24[(sizeof(SMatrix) == 24) ? 1 : -1];
typedef char M3d_Light_size_must_be_52[(sizeof(SLight) == 52) ? 1 : -1];
typedef char M3d_CItem_render_prefix_size_must_be_64[
    (sizeof(M3dCItemRenderLayout) == 64) ? 1 : -1];
typedef char M3d_Texture_size_must_be_40[(sizeof(Texture) == 40) ? 1 : -1];
typedef char M3d_Super_transform_slice_size_must_be_3760[
    (sizeof(M3dSuperTransformLayout) == 3760) ? 1 : -1];
typedef char M3d_Super_render_size_must_be_400[
    (sizeof(CSuper) == 400) ? 1 : -1];
typedef char M3d_PSXRegion_size_must_be_64[
    (sizeof(SPSXRegion) == 64) ? 1 : -1];

/** Return the target-proven derived slice accepted by M3d_BuildTransform. */
static inline M3dSuperTransformLayout *M3d_GetTransformLayout(
    CSuper *pSuper) {
    return (M3dSuperTransformLayout *)pSuper;
}

/** View the common Item prefix as M3D's recovered CItem rendering slice. */
static inline M3dCItemRenderLayout *M3d_GetRenderItem(Item *pItem) {
    return (M3dCItemRenderLayout *)pItem;
}

/** View an Item known to use the articulated CSuper rendering path. */
static inline CSuper *M3d_GetSuper(Item *pItem) {
    return (CSuper *)pItem;
}

// === External declarations ===
#include "PDOUBLEBUFFER.h"
/**
 * MAIN.SYM places these definitions in the final .sdata output, but target
 * M3D accesses both through absolute relocations. These extern-only .data
 * annotations are a GCC per-TU addressing contract; they neither define nor
 * move either symbol.
 */
#include "CAMERASELECTION.h"

#include "M3DDEFAULTLIGHT.h"

SLight M3d_DefaultLight = {
    {
        {0, 0x1000, 0},
        {0, -0x1000, 0},
        {0, 0, 0x1000},
    },
    0,
    {
        {0x1000, 0x0900, 0x0800},
        {0x0800, 0x0d00, 0x0800},
        {0x0200, 0x1000, 0x0800},
    },
    0,
    {0x0200, 0x0200, 0x0200},
};

/** Shared transformed-vertex workspace initialized by M3dInit. */
SVertex Vertices[800] = {};

#define WIBBLETABLES_OWNER_DECLARATION
#include "WIBBLETABLES.h"

/** Per-depth texture-wibble lookup rows initialized at runtime. */
s16 WibbleTables[1024] = {};

/** Frustum planes rebuilt for the active viewport before rendering. */
SVECTOR FrustumNormals[10] = {};

/** GTE light matrix assembled from the current frustum normals. */
MATRIX FrustumMatrix1 = {};

/** GTE color matrix assembled from the current frustum normals. */
MATRIX FrustumMatrix2 = {};

/** Renderer state and work-buffer pointers owned by M3D. */
int M3d_WireframeMode = 0;
int M3d_WireframeLast = 0;
int WaterClipValue = 0xffff;
SVECTOR FarColor = {};
s16 M3d_OTPushback = 0;
s16 M3d_OTPushback2 = 0;
int M3d_Widescreen = 0;
unsigned int FrameNo = 0;
unsigned int XblanksThen = 0;
#define M3DINITSTATE_OWNER_DECLARATION
#define M3DINITSTATE_DECLARE_DEFAULT_LOD_DISTANCE
#include "M3DINITSTATE.h"
s16 M3d_DefaultLODDistance = 0x1770;
int ForceWireFrameColor = 0;
int GModelVideoDisplayed = 0;
SProjectedVertex *ProjectedVertices = 0;
#define M3DINITSTATE_OWNER_DECLARATION
#define M3DINITSTATE_DECLARE_ROTATED_VERTICES
#include "M3DINITSTATE.h"
SRotatedVertex *RotatedVertices = 0;
int DpqMax = 0;
int DpqMin = 0;
int DpqShift = 0;
int DpqInterpTime = 0;
int DpqInterpDeltaMin = 0;
int DpqInterpDeltaMax = 0;
unsigned int M3d_FadeColour = 0;
unsigned int *pColourTable = 0;
SViewport *pCurrentViewport = 0;
SCamera *pCurrentCamera = 0;
SVECTOR WaterNormal = {};
unsigned int Xres = 0;
unsigned int Yres = 0;
int PixelAspectX = 0;
int PixelAspectY = 0;
SProjectedVertex *FreeStitchVertex = 0;
unsigned int *CurrentOrderingTable = 0;
unsigned int RefMapClut = 0;
unsigned int RefMapTpage = 0;
unsigned int RefMapU0V0 = 0;
unsigned int XblanksNow = 0;
int SubObjectMasked = 0;
int TestTheWater = 0;
int Fast = 0;
int Dewibble = 0;
int BasePhase = 0;

// Shared animation-frame lookup owned by BIT.
#define QUICKANIMLOOKUP_CONSUMER_DECLARATION
#include "QUICKANIMLOOKUP.h"

#include "M3DASM.h"
#define M3DMATHS_TYPED_API
#include "M3DMATHS.h"
#undef M3DMATHS_TYPED_API
void M3dMaths_MulRotMatrix(MATRIX *src, MATRIX *dst);
#include "SPOOL.h"

// Character-pose helpers retain their natural C++ spelling so GCC emits the
// original MAIN.SYM names from their recovered signatures.
SMatrix *Decomp_GetAnimTransform(CSuper *pSuper);
static inline void setCopControlWord_m3d(int reg, int value) {
    switch (reg) {
        case 0:

            break;
        case 0x800:

            break;
        case 0x1000:

            break;
        case 0x1800:

            break;
        case 0x2000:

            break;
        case 0xc000:

            break;
        case 0xc800:

            break;
        case 0xd000:

            break;
        default:
            break;
    }
}

static inline void setCopReg_m3d(int reg, int value) {
    switch (reg) {
        case 0:

            break;
        case 4:

            break;
        case 0x3000:

            break;
        case 0x4000:

            break;
        case 0x4800:

            break;
        case 0x5000:

            break;
        case 0x5800:

            break;
        case 0xf000:

            break;
        default:
            break;
    }
}

static inline int getCopReg_m3d(int reg) {
    int value = 0;

    switch (reg) {
        case 0x4800:

            break;
        case 0x5000:

            break;
        case 0x5800:

            break;
        default:
            break;
    }

    return value;
}

static inline void copFunction_m3d(int op) {
    switch (op) {
        case 0x486012:

            break;
        default:
            break;
    }
}

#define setCopControlWord(cop, reg, value) setCopControlWord_m3d((reg), (int)(value))
#define setCopReg(cop, reg, value) setCopReg_m3d((reg), (int)(value))
#define getCopReg(cop, reg) getCopReg_m3d((reg))
#define copFunction(cop, op) copFunction_m3d((op))

// === Free function declarations ===
void M3d_Line(int x0, int y0, int x1, int y1, int Color, int z);
int Wireframe_ProcessPolys(unsigned int * pFaceX, int Num, int color);
void RenderSuperItem(Item * pItem);
void RenderModel(SModel *pModel);
void RenderModelFast(SModel *pModel, VECTOR *pPosnRelCam);
void RenderBackgroundModel(SModel *pModel);
void RenderModelInSuper(SModel *pModel);
void RenderModelInSuperFast(SModel *pModel);
void RenderModelNonRotated(SModel *pModel, VECTOR *pPosnRelCam);
void RenderBackgroundModelNonRotated(SModel *pModel);
int uWibble(STexWibVertInfo * pTexWibVertInfo);
int vWibble(STexWibVertInfo * pTexWibVertInfo);
void M3d_PreprocessWibblyTextures(int Region);
void M3d_PreprocessPulsingColours(int Region);

// === Function implementations (address order) ===

/**
 * M3d_Line__Fiiiiii — Debug line drawing with Cohen-Sutherland clipping
 * Clips a 2D line to the viewport [0,512) x [0,240), then emits a GPU LINE_F2
 * primitive into the ordering table. No stack frame (leaf function, no callee-saves).
 *
 * Address:   0x80094AB8, Size: 476 bytes (119 instructions)
 * Mangled:   M3d_Line__Fiiiiii
 * Demangled: M3d_Line(int, int, int, int, int, int)
 *
 * @param x0  X component for vertex/component 0.
 * @param y0  Y component for vertex/component 0.
 * @param x1  X component for vertex/component 1.
 * @param y1  Y component for vertex/component 1.
 * @param Color  GPU command colour for the line.
 * @param z  Ordering-table depth.
 */
void M3d_Line(int x0, int y0, int x1, int y1, int Color, int z) {
    int t;

    if (x0 == x1 && y0 == y1) return;

    if (x1 < x0) {
        t = x0;
        x0 = x1;
        x1 = t;
        t = y0;
        y0 = y1;
        y1 = t;
    }

    if (x1 < 0 || x0 >= 0x200) return;

    if (x0 < 0) {
        y0 = ((-x0) * (y1 - y0)) / (x1 - x0) + y0;
        x0 = 0;
    }

    if (x1 >= 0x200) {
        y1 = ((0x1FF - x0) * (y1 - y0)) / (x1 - x0) + y0;
        x1 = 0x1FF;
    }

    if (y1 < y0) {
        t = x0;
        x0 = x1;
        x1 = t;
        t = y0;
        y0 = y1;
        y1 = t;
    }

    if (y1 < 0 || y0 >= 0xF0) return;

    if (y0 < 0) {
        x0 = ((-y0) * (x1 - x0)) / (y1 - y0) + x0;
        y0 = 0;
    }

    if (y1 >= 0xF0) {
        x1 = ((0xEF - y0) * (x1 - x0)) / (y1 - y0) + x0;
        y1 = 0xEF;
    }

    LINE_F2 *aline = (LINE_F2 *)pPoly;
    u8 *newppoly = (u8 *)(aline + 1);
    pPoly = (u32 *)newppoly;
    aline->tag = 0x03000000;
    *(u32 *)&aline->r0 = 0x40000000 + Color;

    aline->x0 = x0;
    aline->y0 = y0;
    aline->x1 = x1;
    aline->y1 = y1;

    addPrim(reinterpret_cast<SDoubleBufferLocal *>(pDoubleBuffer)->OrderingTable + ((z >> 2) & 0xFFF), aline);
}

/**
 * Draw model faces as clipped screen-space wireframe primitives.
 *
 * Address:   0x80094C94
 * Mangled:   Wireframe_ProcessPolys__FPUiii
 * Demangled: Wireframe_ProcessPolys(unsigned int *, int, int)
 *
 * @param pFaceX  FaceX to operate on.
 * @param Num Number of packed faces to process.
 * @param color Base RGB value used for each wireframe edge.
 * @return No defined value; the target integer function falls through.
 */
int Wireframe_ProcessPolys(unsigned int * pFaceX, int Num, int color) {
    if (ForceWireFrameColor) {
        color = ForceWireFrameColor;
    }

    SProjectedVertex *pVertices = ProjectedVertices;
    int FaceR;
    int FaceG;
    int FaceB;

    for (int i = 0; i < Num;
        i++, pFaceX += (*(u16 *)((u8 *)pFaceX + 2) & 0xFFFC) >> 2) {
        u8 *pVertexIndex = (u8 *)(pFaceX + 1);

        int x0 = pVertices[pVertexIndex[0]].X;
        int y0 = pVertices[pVertexIndex[0]].Y;
        int z0 = pVertices[pVertexIndex[0]].z;
        int x1 = pVertices[pVertexIndex[1]].X;
        int y1 = pVertices[pVertexIndex[1]].Y;
        int z1 = pVertices[pVertexIndex[0]].z;
        int x2 = pVertices[pVertexIndex[2]].X;
        int y2 = pVertices[pVertexIndex[2]].Y;
        int z2 = pVertices[pVertexIndex[0]].z;

        if ((x1 - x0) * (y2 - y0) < (x2 - x0) * (y1 - y0)) {
            continue;
        }

        int zmax = z0;
        if (z1 > zmax) zmax = z1;
        if (z2 > zmax) zmax = z2;

        if (!(*(u16 *)pFaceX & 0x10) &&
            ((*pFaceX & 0x40) || (*pFaceX & 0x80))) {
            int x3 = pVertices[pVertexIndex[3]].X;
            int y3 = pVertices[pVertexIndex[3]].Y;
            int z3 = pVertices[pVertexIndex[0]].z;
            if (z3 != zmax) zmax = z3;

            int color2 = color;
            if (color2 == 0x800080) {
                if (*(u16 *)pFaceX & 0x2000) color2 = 0x008000;
                if (*(u16 *)pFaceX & 0x4000) color2 = 0x000080;
            }

            M3d_Line(x0, y0, x1, y1, color2, zmax);
            M3d_Line(x1, y1, x3, y3, color2, zmax);
            M3d_Line(x2, y2, x3, y3, color2, zmax);
            M3d_Line(x2, y2, x0, y0, color2, zmax);

            if (M3d_WireframeMode == 2 &&
                !((pFaceX[3] >> 16) & 1)) {
                FaceR = 0;
                FaceG = 0;
                FaceB = 0;

                if (*(u16 *)pFaceX & 0x2000) {
                    int push = *(s16 *)0x1F8002B4 / 32;
                    if (push < -255) push = -255;
                    if (push > 255) push = 255;
                    if (push < 0) FaceG -= push;
                    else FaceR += push;
                }
                if (*(u16 *)pFaceX & 0x4000) {
                    int push = *(s16 *)0x1F8002B4 / 32;
                    if (push < -255) push = -255;
                    if (push > 255) push = 255;
                    if (push < 0) FaceG -= push;
                    else FaceR += push;
                }

                POLY_F4 *p_poly = (POLY_F4 *)pPoly;
                pPoly = (u32 *)(p_poly + 1);
                setPolyF4(p_poly);
                setXY4(p_poly, x0, y0, x1, y1, x2, y2, x3, y3);
                setRGB0(p_poly, FaceR, FaceG, FaceB);
                addPrim(reinterpret_cast<SDoubleBufferLocal *>(pDoubleBuffer)->OrderingTable +
                            ((zmax >> 2) & 0xFFF),
                        p_poly);
            }
        } else if ((*pFaceX & 0x40) || (*pFaceX & 0x80)) {
            int color2 = color;
            if (color2 == 0x800080) {
                if (*(u16 *)pFaceX & 0x2000) color2 = 0x008000;
                if (*(u16 *)pFaceX & 0x4000) color2 = 0x000080;
            }

            M3d_Line(x0, y0, x1, y1, color2, zmax);
            M3d_Line(x1, y1, x2, y2, color2, zmax);
            M3d_Line(x2, y2, x0, y0, color2, zmax);

            if (M3d_WireframeMode == 2 &&
                !((pFaceX[3] >> 16) & 1)) {
                FaceR = 0;
                FaceG = 0;
                FaceB = 0;

                if (*(u16 *)pFaceX & 0x2000) {
                    int push = *(s16 *)0x1F8002B4 / 32;
                    if (push < -255) push = -255;
                    if (push > 255) push = 255;
                    if (push < 0) FaceG -= push;
                    else FaceR += push;
                }
                if (*(u16 *)pFaceX & 0x4000) {
                    int push = *(s16 *)0x1F8002B4 / 32;
                    if (push < -255) push = -255;
                    if (push > 255) push = 255;
                    if (push < 0) FaceG -= push;
                    else FaceR += push;
                }

                POLY_F3 *p_poly = (POLY_F3 *)pPoly;
                pPoly = (u32 *)(p_poly + 1);
                setPolyF3(p_poly);
                setXY3(p_poly, x0, y0, x1, y1, x2, y2);
                setRGB0(p_poly, FaceR, FaceG, FaceB);
                addPrim(reinterpret_cast<SDoubleBufferLocal *>(pDoubleBuffer)->OrderingTable +
                            ((zmax >> 2) & 0xFFF),
                        p_poly);
            }
        }
    }
}


/**
 * M3d_RenderSetup__FP7SCameraP9SViewportPUi
 *
 * Address:   0x80095220
 * Mangled:   M3d_RenderSetup__FP7SCameraP9SViewportPUi
 * Demangled: M3d_RenderSetup(SCamera *, SViewport *, unsigned int *)
 *
 * @param pCamera  Camera to operate on.
 * @param pVP  VP to operate on.
 * @param ot Ordering table receiving this frame's primitives.
 */
void M3d_RenderSetup(SCamera * pCamera, SViewport * pVP, unsigned int * ot) {
    XblanksThen = XblanksNow;
    XblanksNow = Xblanks;
    if (XblanksThen != XblanksNow) {
        FrameNo++;
    }

    pCurrentCamera = pCamera;
    pCurrentViewport = pVP;
    pCurrentViewport->Yon = DpqMax;
    CurrentOrderingTable = ot;

    RECT Window;
    Window.x = 0;
    Window.y = reinterpret_cast<SDoubleBufferLocal *>(pDoubleBuffer)->Draw.clip.y;
    Window.w = Xres;
    Window.h = Yres;
    DR_AREA *pClipArea = (DR_AREA *)pPoly;
    SetDrawArea(pClipArea, &Window);
    pPoly = (u32 *)(pClipArea + 1);
    addPrim(CurrentOrderingTable, pClipArea);

    u32 opp;
    u32 adj;
    u32 hyp;

    pVP->Zoom = (((pVP->xR - pVP->xL) >> 1) << 12) /
                pVP->TanHalf * 4096 / PixelAspectY;
    pVP->xC = (pVP->xL + pVP->xR) >> 1;
    pVP->yC = (pVP->yT + pVP->yB) >> 1;

    FrustumNormals[0].vx = 0;
    FrustumNormals[0].vy = 0;
    FrustumNormals[0].vz = 0xF000;
    FrustumNormals[0].pad = pVP->Yon;
    FrustumNormals[1].vx = 0;
    FrustumNormals[1].vy = 0;
    FrustumNormals[1].vz = 0x1000;
    FrustumNormals[1].pad = -pVP->Hither;

    opp = (pVP->xR - pVP->xL) << 11;
    adj = pVP->Zoom * PixelAspectX;
    hyp = M3dMaths_SquareRoot0((opp >> 12) * (opp >> 12) +
                                    (adj >> 12) * (adj >> 12));
    FrustumNormals[2].vx = 0;
    FrustumNormals[2].vy = adj / hyp;
    FrustumNormals[2].vz = opp / hyp;
    FrustumNormals[2].pad = 0;
    FrustumNormals[3].vx = 0;
    FrustumNormals[3].vy = -FrustumNormals[2].vy;
    FrustumNormals[3].vz = FrustumNormals[2].vz;
    FrustumNormals[3].pad = 0;

    opp = (pVP->xR - pVP->xL) << 11;
    adj = pVP->Zoom * PixelAspectY;
    hyp = M3dMaths_SquareRoot0((opp >> 12) * (opp >> 12) +
                                    (adj >> 12) * (adj >> 12));
    FrustumNormals[4].vx = adj / hyp;
    FrustumNormals[4].vy = 0;
    FrustumNormals[4].vz = opp / hyp;
    FrustumNormals[4].pad = 0;
    FrustumNormals[5].vx = -FrustumNormals[4].vx;
    FrustumNormals[5].vy = 0;
    FrustumNormals[5].vz = FrustumNormals[4].vz;
    FrustumNormals[5].pad = 0;

    opp = (pVP->yB - pVP->yT) >> 1;
    adj = pVP->Zoom;
    hyp = M3dMaths_SquareRoot0(opp * opp + adj * adj);
    FrustumNormals[6].vx = 0;
    FrustumNormals[6].vy = (adj << 12) / hyp;
    FrustumNormals[6].vz = (opp << 12) / hyp;
    FrustumNormals[6].pad = 0;
    FrustumNormals[7].vx = 0;
    FrustumNormals[7].vy = -FrustumNormals[6].vy;
    FrustumNormals[7].vz = FrustumNormals[6].vz;
    FrustumNormals[7].pad = 0;

    opp = (pVP->xR - pVP->xL) >> 1;
    adj = pVP->Zoom;
    hyp = M3dMaths_SquareRoot0(opp * opp + adj * adj);
    FrustumNormals[8].vx = (adj << 12) / hyp;
    FrustumNormals[8].vy = 0;
    FrustumNormals[8].vz = (opp << 12) / hyp;
    FrustumNormals[8].pad = 0;
    FrustumNormals[9].vx = -FrustumNormals[8].vx;
    FrustumNormals[9].vy = 0;
    FrustumNormals[9].vz = FrustumNormals[8].vz;
    FrustumNormals[9].pad = 0;

    gte_SetRotMatrix(&pCamera->Transform);
    for (int i = 0; i < 3; i++) {
        gte_ldv0(&FrustumNormals[i]);
        gte_rtv0();
        gte_stsv(&FrustumMatrix1.m[i]);

        gte_ldv0(&FrustumNormals[i + 3]);
        gte_rtv0();
        gte_stsv(&FrustumMatrix2.m[i]);
    }

    SetGeomScreen(pVP->Zoom);
    SetGeomOffset(pVP->xC, pVP->yC);
    M3dMaths_TransposeMat((u16 *)&pCamera->Transform, (u16 *)&pCamera->Inverse);
    for (int i = 0; i < 3; i++) {
        pCamera->View.m[0][i] = pCamera->Inverse.m[0][i] * PixelAspectY >> 12;
        pCamera->View.m[1][i] = pCamera->Inverse.m[1][i] * PixelAspectX >> 12;
        pCamera->View.m[2][i] = pCamera->Inverse.m[2][i];
    }

    Texture *pTex = Spool_FindTextureEntry(0xcce0187c);
    RefMapClut = (u32)pTex->clut << 16;
    RefMapTpage = (u32)pTex->tpage << 16;
    RefMapU0V0 = pTex->u0 | (pTex->v0 << 8);
    int *envRegions = (int *)&EnvRegions;
    M3d_PreprocessPulsingColours(envRegions[0]);
    M3d_PreprocessPulsingColours(envRegions[1]);
    M3d_PreprocessPulsingColours(CurrentObjFileRegion);
    M3d_PreprocessPulsingColours(CurrentItemsFileRegion);
    M3d_PreprocessWibblyTextures(CurrentObjFileRegion);
    M3d_PreprocessWibblyTextures(CurrentItemsFileRegion);
    WaterNormal.vx = -pCurrentCamera->Transform.m[1][0];
    WaterNormal.vy = -pCurrentCamera->Transform.m[1][1];
    WaterNormal.vz = -pCurrentCamera->Transform.m[1][2];
    WaterNormal.pad = (CurrentWaterLevel >> 12) - pCurrentCamera->Position.vy;
}


/**
 * M3d_RenderCleanup__Fv — Restore draw area and draw widescreen borders
 * Prologue saves: $s0, $ra. Stack: 0x20 (sp+0x10..0x17 = RECT)
 *
 * Address:   0x800957FC, Size: 428 bytes (107 instructions)
 * Mangled:   M3d_RenderCleanup__Fv
 * Demangled: M3d_RenderCleanup(void)
 *
 */
void M3d_RenderCleanup(void) {
    DR_AREA *pClipArea = (DR_AREA *)pPoly;
    RECT Window;

    Window.x = pCurrentViewport->xL;
    Window.y = pCurrentViewport->yT + reinterpret_cast<SDoubleBufferLocal *>(pDoubleBuffer)->Draw.clip.y +
               (u16)M3d_Widescreen;
    Window.w = pCurrentViewport->xR - pCurrentViewport->xL;
    int widescreen = M3d_Widescreen;
    Window.h = pCurrentViewport->yB - pCurrentViewport->yT -
               (u16)(widescreen << 1);
    SetDrawArea(pClipArea, &Window);

    addPrim(&CurrentOrderingTable[0xfff], pClipArea);
    pPoly = (u32 *)(pClipArea + 1);
    if (M3d_Widescreen != 0) {
        for (int i = 0; i < 2; i++) {
            POLY_F4 *p_polyF4 = (POLY_F4 *)pPoly;
            pPoly = (u32 *)(p_polyF4 + 1);
            *(u32 *)&p_polyF4->r0 = 0;
            setlen(p_polyF4, 5);
            setcode(p_polyF4, 0x28);

            if (i != 0) {
                p_polyF4->x0 = 0;
                p_polyF4->y0 = 0;
                p_polyF4->x1 = 0x200;
                p_polyF4->y1 = 0;
                p_polyF4->x2 = 0;
                p_polyF4->x3 = 0x200;
                p_polyF4->y2 = (u16)M3d_Widescreen;
                p_polyF4->y3 = p_polyF4->y2;
            } else {
                p_polyF4->x0 = 0;
                p_polyF4->y0 = 0xf0;
                p_polyF4->x1 = 0x200;
                p_polyF4->y1 = 0xf0;
                p_polyF4->x2 = 0;
                p_polyF4->x3 = 0x200;
                p_polyF4->y2 = 0xf0 - (u16)M3d_Widescreen;
                p_polyF4->y3 = p_polyF4->y2;
            }

            addPrim(&CurrentOrderingTable[0xfff], p_polyF4);
        }
    }
}


/**
 * M3d_Render__FPv — Render an Item list through the model or super-item path
 *
 * Address:   0x800959A8
 * Mangled:   M3d_Render__FPv
 * Demangled: M3d_Render(void *)
 *
 * @param p  Head of the Item list to render.
 */
void M3d_Render(void * p) {
    Item *pItem = (Item *)p;
    VECTOR Position;
    VECTOR PosnRelCam;
    MATRIX Transform;
    MATRIX WorldTransform;
    MATRIX TempMatrix;
    SModel *pModel;
    unsigned int GlobalFaceFlags;
    unsigned short yon;

    if (pItem == 0) {
        return;
    }

    gte_SetLightMatrix(&FrustumMatrix1);
    gte_SetColorMatrix(&FrustumMatrix2);
    gte_ldbkdir(
        (short)FrustumNormals[0].pad >> 1,
        (short)FrustumNormals[1].pad >> 1,
        0);

    if ((CBody *)pItem != BaddyList) {
        M3dAsm_BoundingSpherePreprocessing(
            pItem, Camera_CurrentMikeCamera != 0 ? 0x40 : 1);
    } else {
        M3dAsm_BoundingSpherePreprocessing(pItem, 1);
    }

    if (pItem == EnviroList) {
        M3d_PreprocessWibblyTextures(((int *)&EnvRegions)[0]);
        M3d_PreprocessWibblyTextures(((int *)&EnvRegions)[1]);
    }

    *(u16 *)0x1f8002d4 = 0;
    *(u16 *)0x1f800284 = (u16)DpqMin;
    *(u16 *)0x1f800294 = (u16)DpqShift;
    *(u16 *)0x1f8002b4 = (u16)M3d_OTPushback;
    *(u16 *)0x1f8002c4 = (u16)M3d_OTPushback2;
    *(u32 *)0x1f800364 = RefMapClut;
    *(u32 *)0x1f800374 = RefMapTpage;
    *(u32 *)0x1f800354 = M3d_FadeColour;

    M3d_WireframeLast = M3d_WireframeMode;
    while (pItem != 0) {
        M3d_WireframeMode = M3d_WireframeLast;

        if ((pItem->Flags & 0x8000) == 0 &&
            PSXRegion[pItem->Region].Usable != 0) {
            GlobalFaceFlags = 0xffff0000;
            if ((pItem->Flags & 0x80) != 0) {
                GlobalFaceFlags = 0xfff70004;
                if ((pItem->Flags & 0x400) != 0) {
                    GlobalFaceFlags = 0xffff000c;
                }
            } else if ((pItem->Flags & 0x400) != 0) {
                GlobalFaceFlags = 0xfffb0008;
                *(u32 *)0x1f8003e4 =
                    ((pItem->RGB & 0xff) << 2) |
                    ((pItem->RGB & 0xff00) << 5) |
                    ((pItem->RGB & 0xff0000) << 8);
            }

            if ((pItem->Flags & 0x4000) != 0) {
                GlobalFaceFlags |= 2;
                GlobalFaceFlags &= 0xfffeffff;
                GlobalFaceFlags &= 0xffdfffff;
            } else {
                GlobalFaceFlags &= 0xfffffffd;
                GlobalFaceFlags |= 0x10000;
                GlobalFaceFlags |= 0x200000;
            }

            if ((pItem->Flags & 0x800) != 0) {
                unsigned int globalFaceBits =
                    GlobalFaceFlags & 0xfffffe7f;
                unsigned int itemFaceFlags =
                    (M3d_GetRenderItem(pItem)->mTRN & 0x180) |
                    0x40;
                GlobalFaceFlags = globalFaceBits | itemFaceFlags;
            }

            *(u32 *)0x1f8003f4 = GlobalFaceFlags;
            TestTheWater = (pItem->Flags >> 3) & 1;

            if ((pItem->Flags & 2) != 0) {
                WaterNormal.pad =
                    (short)(CurrentWaterLevel >> 12) -
                    (short)pCurrentCamera->Position.vy;
                RenderSuperItem(pItem);

                if (TestTheWater != 0) {
                    int tmp = pItem->Flags;
                    int r;
                    int g;
                    int b;

                    pItem->Flags = (tmp | 0x400) & 0xff7f;
                    if (GLevel == 0) {
                        b = 10;
                        g = 13;
                        r = 8;
                    } else {
                        b = 0x22;
                        g = 0x20;
                        r = 0x20;
                    }

                    GlobalFaceFlags &= 0xfffbffff;
                    GlobalFaceFlags |= 8;
                    pItem->RGB = (b << 16) | (g << 8) | r;
                    WaterClipValue = 0;
                    *(u32 *)0x1f8003e4 =
                        ((pItem->RGB & 0xff) << 2) |
                        ((pItem->RGB & 0xff00) << 5) |
                        ((pItem->RGB & 0xff0000) << 8);
                    *(u32 *)0x1f8003f4 = GlobalFaceFlags;
                    RenderSuperItem(pItem);
                    WaterClipValue = 0xffff;
                    pItem->Flags = (u16)tmp;
                }
            } else {
                pModel =
                    PSXRegion[pItem->Region].ppModels[pItem->Model];
                if ((pModel->Flags & 0x20) == 0) {
                    if ((pModel->Flags & 0x200) != 0) {
                        GModelVideoDisplayed = 1;
                    }
                    if ((pModel->Flags & 0x400) != 0) {
                        M3d_WireframeMode = 1;
                    }

                    gte_SetRotMatrix(&pCurrentCamera->View);
                    gte_ZeroTransVector();

                    PosnRelCam.vx =
                        (pItem->Position.vx >> 12) -
                        pCurrentCamera->Position.vx;
                    PosnRelCam.vy =
                        (pItem->Position.vy >> 12) -
                        pCurrentCamera->Position.vy;
                    PosnRelCam.vz =
                        (pItem->Position.vz >> 12) -
                        pCurrentCamera->Position.vz;
                    gte_ldlv0(&PosnRelCam);
                    gte_rtv0();
                    gte_stlvnl(&Position);

                    if (pModel->zMax < 0) {
                        while (-(((short)(
                                      (pModel->zMax *
                                       pCurrentViewport->Zoom) / 0xbf) *
                                  DpqMax) >> 10) <
                                   Position.vz &&
                               pModel->NextLOD != 0xffff) {
                            pModel =
                                PSXRegion[pItem->Region]
                                    .ppModels[pModel->NextLOD];
                        }

                        if (pModel->zMax < 0) {
                            if (-((short)(
                                      (pModel->zMax *
                                       pCurrentViewport->Zoom) /
                                      0xbf) *
                                  DpqMax >>
                                  10) < Position.vz &&
                                pModel->NextLOD == 0xffff) {
                                goto next_item;
                            }
                        } else {
                            int tmp = pModel->zMax;
                            tmp *= pCurrentViewport->Zoom;
                            tmp /= 0xbf;
                            if (tmp < Position.vz &&
                                pModel->NextLOD == 0xffff) {
                                goto next_item;
                            }
                        }
                    } else {
                        while ((pModel->zMax *
                                pCurrentViewport->Zoom) / 0xbf <
                                   Position.vz &&
                               pModel->NextLOD != 0xffff) {
                            pModel =
                                PSXRegion[pItem->Region]
                                    .ppModels[pModel->NextLOD];
                        }
                        if ((pModel->zMax *
                             pCurrentViewport->Zoom) / 0xbf <
                                Position.vz &&
                            pModel->NextLOD == 0xffff) {
                            goto next_item;
                        }
                    }

                    if (pModel->NumVertices < 0x65 &&
                        (pModel->Flags & 0x100) == 0 &&
                        (pItem->Flags & 0x480) == 0 &&
                        Position.vz - (pModel->Radius >> 12) >
                            pCurrentViewport->Yon / 10 &&
                        (pItem->Angles.vx |
                         pItem->Angles.vy |
                         pItem->Angles.vz) == 0) {
                        Fast = 1;
                    } else {
                        Fast = 0;
                    }

                    if (pItem->Angles.vx != 0 ||
                        pItem->Angles.vy != 0 ||
                        pItem->Angles.vz != 0) {
                        M3dMaths_RotMatrixYXZ(
                            (SVECTOR *)&pItem->Angles, &WorldTransform);
                        if ((pItem->Flags & 0x200) != 0) {
                            M3dMaths_ScaleMatrix(pItem, &WorldTransform);
                        }
                        M3dMaths_MulRotMatrix(&WorldTransform, &Transform);
                        gte_SetRotMatrix(&Transform);
                    } else {
                        M3dMaths_SetIdentityRotation(&WorldTransform);
                        if ((pItem->Flags & 0x200) != 0) {
                            M3dMaths_ScaleMatrix(pItem, &WorldTransform);
                            M3dMaths_MulRotMatrix(
                                &WorldTransform, &Transform);
                            gte_SetRotMatrix(&Transform);
                        }
                    }

                    pColourTable =
                        PSXRegion[pItem->Region].pColourTable;

                    if ((pModel->Flags & 1) != 0 ||
                        (pItem->Flags & 0x4000) != 0) {
                        M3dAsm_GetReflectionMapping(
                            &WorldTransform,
                            (long *)&Position,
                            (u32 *)pModel);
                    } else if ((pModel->Flags & 4) != 0 ||
                               (pItem->Flags & 0x80) != 0) {
                        if (pItem->Angles.vx != 0 ||
                            pItem->Angles.vy != 0 ||
                            pItem->Angles.vz != 0) {
                            gte_SetRotMatrix(
                                (MATRIX *)
                                    M3d_GetRenderItem(pItem)
                                        ->mpLight->LightMatrix);
                            gte_ldclmv(&WorldTransform.m[0][0]);
                            gte_rtir();
                            gte_stclmv(&TempMatrix.m[0][0]);
                            gte_ldclmv(&WorldTransform.m[0][1]);
                            gte_rtir();
                            gte_stclmv(&TempMatrix.m[0][1]);
                            gte_ldclmv(&WorldTransform.m[0][2]);
                            gte_rtir();
                            gte_stclmv(&TempMatrix.m[0][2]);
                            gte_SetLightMatrix(&TempMatrix);
                        } else {
                            gte_SetLightMatrix(
                                (MATRIX *)
                                    M3d_GetRenderItem(pItem)
                                        ->mpLight->LightMatrix);
                        }
                        gte_SetColorMatrix(
                            (MATRIX *)
                                M3d_GetRenderItem(pItem)
                                    ->mpLight->ColorMatrix);
                        M3dAsm_SetAmbient(
                            M3d_GetRenderItem(pItem)
                                ->mpLight->BackColor);
                        M3dAsm_GetDynamicLighting(
                            pModel->Vertices +
                                pModel->NumVertices,
                            pModel->NumNormals);
                    }

                    yon = pCurrentViewport->Yon;
                    if ((pItem->Flags & 0x2000) != 0) {
                        *(u16 *)0x1f800284 = 0x7fff;
                        *(u16 *)0x1f8002d4 = 0;
                        pCurrentViewport->Yon = 0x7fff;
                    }

                    if (Fast != 0) {
                        if (pItem->Frame != 0 ||
                            pItem->Anim != 0) {
                            if (pItem->Frame != 0) {
                                *(u16 *)0x1f8002b4 =
                                    (signed char)pItem->Frame
                                    << 5;
                            }
                            if (pItem->Anim != 0) {
                                *(u16 *)0x1f8002c4 =
                                    (signed char)pItem->Anim
                                    << 5;
                            }
                            RenderModelFast(
                                pModel, &PosnRelCam);
                            *(u16 *)0x1f8002b4 =
                                (u16)M3d_OTPushback;
                            *(u16 *)0x1f8002c4 =
                                (u16)M3d_OTPushback2;
                        } else {
                            RenderModelFast(
                                pModel, &PosnRelCam);
                        }
                    } else if (pItem->Angles.vx == 0 &&
                               pItem->Angles.vy == 0 &&
                               pItem->Angles.vz == 0 &&
                               (pItem->Flags & 0x200) == 0) {
                        if (pItem->Frame != 0 ||
                            pItem->Anim != 0) {
                            if (pItem->Frame != 0) {
                                *(u16 *)0x1f8002b4 =
                                    (signed char)pItem->Frame
                                    << 5;
                            }
                            if (pItem->Anim != 0) {
                                *(u16 *)0x1f8002c4 =
                                    (signed char)pItem->Anim
                                    << 5;
                            }
                            RenderModelNonRotated(
                                pModel, &PosnRelCam);
                            *(u16 *)0x1f8002b4 =
                                (u16)M3d_OTPushback;
                            *(u16 *)0x1f8002c4 =
                                (u16)M3d_OTPushback2;
                        } else {
                            RenderModelNonRotated(
                                pModel, &PosnRelCam);
                        }
                    } else {
                        gte_SetTransVector(&Position);
                        if (pItem->Frame != 0 ||
                            pItem->Anim != 0) {
                            if (pItem->Frame != 0) {
                                *(u16 *)0x1f8002b4 =
                                    (signed char)pItem->Frame
                                    << 5;
                            }
                            if (pItem->Anim != 0) {
                                *(u16 *)0x1f8002c4 =
                                    (signed char)pItem->Anim
                                    << 5;
                            }
                            RenderModel(pModel);
                            *(u16 *)0x1f8002b4 =
                                (u16)M3d_OTPushback;
                            *(u16 *)0x1f8002c4 =
                                (u16)M3d_OTPushback2;
                        } else {
                            RenderModel(pModel);
                        }
                    }

                    if ((pItem->Flags & 0x2000) != 0) {
                        *(u16 *)0x1f8002d4 = 0;
                        *(u16 *)0x1f800284 = (u16)DpqMin;
                        pCurrentViewport->Yon = yon;
                    }
                }
            }
        }

next_item:
        pItem = pItem->NextItem;
    }

    M3d_WireframeMode = M3d_WireframeLast;
}


// ====== 0x80096700 M3d_RenderBackground__FPv ===================================
// M3d_RenderBackground__FPv — Render all background items in linked list
// Address: 0x80096700, Size: 472 bytes (118 instructions)
// Demangled: M3d_RenderBackground(void *)
//
// Iterates a linked list of background render items. For each item:
// - Checks flags (skip if bit 0 or bit 15 set)
// - Looks up PSXRegion slot, checks loaded
// - Loads camera matrix into GTE
// - Optionally builds a rotation matrix if item has rotation/translation
// - Dispatches to RenderBackgroundModel or RenderBackgroundModelNonRotated
//
// Uses COP2 (GTE) instructions for matrix loads, stack-local MATRIX buffers.
// Callee-saves: $s0-$s5, $ra. Stack: 0x90. Local MATRIXes at sp+0x30 and sp+0x50.

void M3d_RenderBackground(void *p) {
    // MAIN.SYM records these workspaces in this order; the vectors occupy the
    // otherwise-unused slots before the two transformation matrices.
    VECTOR Position;
    VECTOR PosnRelCam;
    MATRIX Transform;
    MATRIX WorldTransform;
    Item *pItem = (Item *)p;
    SModel *pModel;
    u32 GlobalFaceFlags;

    if (pItem != 0) {
        GlobalFaceFlags = 0xFFFF0000;
        *(unsigned short *)0x1f800284 = 0x7fff;
        *(unsigned short *)0x1f8002d4 = 0;
        do {
            if (((pItem->Flags & 0x8001) == 0) &&
                (PSXRegion[pItem->Region].Usable != 0)) {
                *(unsigned int *)0x1f8003f4 = GlobalFaceFlags;

                gte_SetRotMatrix(&pCurrentCamera->View);
                if ((*(int *)&pItem->Angles != 0) ||
                    (pItem->Angles.vz != 0)) {
                    M3dMaths_RotMatrixYXZ(
                        (SVECTOR *)&pItem->Angles, &WorldTransform);
                    M3dMaths_MulRotMatrix(&WorldTransform, &Transform);
                    gte_SetRotMatrix(&Transform);
                } else {
                    M3dMaths_SetIdentityRotation(&WorldTransform);
                }

                pColourTable = PSXRegion[pItem->Region].pColourTable;
                pModel = PSXRegion[pItem->Region].ppModels[pItem->Model];

                if ((*(int *)&pItem->Angles == 0) &&
                    (pItem->Angles.vz == 0)) {
                    RenderBackgroundModelNonRotated(pModel);
                } else {
                    RenderBackgroundModel(pModel);
                }
            }
            pItem = pItem->NextItem;
        } while (pItem != 0);
        *(unsigned short *)0x1f800284 = *(unsigned short *)&DpqMin;
    }
}


/**
 * Render one articulated CSuper item, including its animation transforms,
 * lighting/reflection state, optional ellipsoid, and projected shadows.
 *
 * Address:   0x800968D8
 * Mangled:   RenderSuperItem__FP4Item
 * Demangled: RenderSuperItem(Item *)
 *
 * @param pItem Character item guaranteed valid by M3d_Render.
 */
void RenderSuperItem(Item *pItem) {
    unsigned int NumParts;
    unsigned int Rgb;
    VECTOR Position;
    SMatrix *pAnimTransform;
    MATRIX Transform;
    MATRIX SuperTransform;
    MATRIX WorldTransform;
    MATRIX SuperLightTransform;
    int i;
    int Type;
    SModel *pModel;
    SModel **ppModels;
    SAnimFrame *pAnimTex;
    unsigned short Scale;
    short OTPushback;
    unsigned int ItemFlags;
    SMatrix *pFirstTransform;

    ItemFlags = pItem->Flags;
    if (PSXRegion[pItem->Region].Fastable == 0 ||
        (ItemFlags & 0x0c80) != 0) {
        Fast = 0;
    } else {
        Fast = 1;
    }

    /* Reflection rendering mirrors the first column only for this pass. */
    if ((M3d_GetSuper(pItem)->mExtraFlags & 2) != 0) {
        *(u16 *)0x1f8002d4 = 1;
        M3d_GetSuper(pItem)->mTransform.m[0][0] =
            -M3d_GetSuper(pItem)->mTransform.m[0][0];
        M3d_GetSuper(pItem)->mTransform.m[1][0] =
            -M3d_GetSuper(pItem)->mTransform.m[1][0];
        M3d_GetSuper(pItem)->mTransform.m[2][0] =
            -M3d_GetSuper(pItem)->mTransform.m[2][0];
    } else {
        *(u16 *)0x1f8002d4 = 0;
    }

    gte_SetRotMatrix(&pCurrentCamera->View);
    M3dMaths_MulRotMatrix(
        &M3d_GetSuper(pItem)->mTransform, &SuperTransform);

    Position.vx =
        (pItem->Position.vx >> 12) - pCurrentCamera->Position.vx;
    Position.vy =
        (pItem->Position.vy >> 12) - pCurrentCamera->Position.vy;
    Position.vz =
        (pItem->Position.vz >> 12) - pCurrentCamera->Position.vz;
    gte_ldlvl(&Position);
    gte_rtir();
    gte_stlvnl(&SuperTransform.t[0]);

    if ((short)SuperTransform.t[2] < 1000) {
        Dewibble = 1;
        Position.vx =
            (pItem->Position.vx >> 8) -
            (pCurrentCamera->Position.vx << 4);
        Position.vy =
            (pItem->Position.vy >> 8) -
            (pCurrentCamera->Position.vy << 4);
        Position.vz =
            (pItem->Position.vz >> 8) -
            (pCurrentCamera->Position.vz << 4);
        gte_ldlvl(&Position);
        gte_rtir();
        gte_stlvnl(&SuperTransform.t[0]);
    } else {
        Dewibble = 0;
    }

    ppModels = PSXRegion[pItem->Region].ppModels;
    NumParts = PSXRegion[pItem->Region].NumParts;
    if ((ItemFlags & 4) != 0) {
        pAnimTransform = M3d_GetSuper(pItem)->mpPoseBuffer;
    } else {
        pAnimTransform = Decomp_GetAnimTransform(M3d_GetSuper(pItem));
    }
    pFirstTransform = pAnimTransform;

    gte_SetRotMatrix(
        (MATRIX *)M3d_GetRenderItem(pItem)->mpLight->LightMatrix);
    gte_ldclmv(&M3d_GetSuper(pItem)->mTransform.m[0][0]);
    gte_rtir();
    gte_stclmv(&SuperLightTransform.m[0][0]);
    gte_ldclmv(&M3d_GetSuper(pItem)->mTransform.m[0][1]);
    gte_rtir();
    gte_stclmv(&SuperLightTransform.m[0][1]);
    gte_ldclmv(&M3d_GetSuper(pItem)->mTransform.m[0][2]);
    gte_rtir();
    gte_stclmv(&SuperLightTransform.m[0][2]);

    Type = M3d_GetSuper(pItem)->mEllipsoidRGB >> 24;
    if ((Type & 0x7f) != 0) {
        int Lookup = (Type - 1) & 0x7f;
        if (Lookup < 0) {
            Lookup = 0;
        }
        pAnimTex = QuickAnimLookup[Lookup];
        Rgb = M3d_GetSuper(pItem)->mEllipsoidRGB & 0x00ffffff;
        Scale = M3d_GetSuper(pItem)->mEllipsoidScale;
        OTPushback = M3d_GetSuper(pItem)->mEllipsoidOTPushback;
        pAnimTex += FrameNo % (unsigned int)pAnimTex[-1].pTexture;
    }

    FreeStitchVertex =
        Fast == 0 ? ProjectedVertices + 399
                  : (SProjectedVertex *)0x1f8003f8;
    pColourTable = PSXRegion[pItem->Region].pColourTable;

    for (i = 0; i < NumParts; i++, pAnimTransform++) {
        pModel = ppModels[i];
        SubObjectMasked =
            (M3d_GetSuper(pItem)->mSubObjectDisplayMask & (1 << i)) != 0;
        if (M3d_GetSuper(pItem)->mUseAnimOrder != 0) {
            pAnimTransform =
                pFirstTransform + M3d_GetSuper(pItem)->mAnimOrder[i];
        }
        if ((pModel->Flags & 0x20) != 0) {
            SubObjectMasked = 1;
        }

        gte_SetRotMatrix(&M3d_GetSuper(pItem)->mTransform);
        gte_ldclmv(&pAnimTransform->m[0][0]);
        gte_rtir();
        gte_stclmv(&WorldTransform.m[0][0]);
        gte_ldclmv(&pAnimTransform->m[0][1]);
        gte_rtir();
        gte_stclmv(&WorldTransform.m[0][1]);
        gte_ldclmv(&pAnimTransform->m[0][2]);
        gte_rtir();
        gte_stclmv(&WorldTransform.m[0][2]);

        if (Dewibble != 0) {
            M3dAsm_SetSuperTransformsDewibbled(
                &SuperTransform, &SuperLightTransform, pAnimTransform);
        } else {
            M3dAsm_SetSuperTransforms(
                &SuperTransform, &SuperLightTransform, pAnimTransform);
        }

        if ((pModel->Flags & 1) != 0 ||
            (pItem->Flags & 0x4000) != 0) {
            M3dAsm_GetReflectionMapping(
                &WorldTransform, (long *)&SuperTransform.t[0],
                (u32 *)pModel);
        } else if ((pModel->Flags & 4) != 0 ||
                   (ItemFlags & 0x80) != 0) {
            gte_SetColorMatrix(
                (MATRIX *)M3d_GetRenderItem(pItem)
                    ->mpLight->ColorMatrix);
            M3dAsm_SetAmbient(
                M3d_GetRenderItem(pItem)->mpLight->BackColor);
            M3dAsm_GetDynamicLighting(
                pModel->Vertices + pModel->NumVertices,
                pModel->NumNormals);
        }

        if ((Type & 0x80) == 0) {
            if (Fast != 0) {
                RenderModelInSuperFast(pModel);
            } else {
                RenderModelInSuper(pModel);
            }
        }

        int DoEllipsoids = Type & 0x7f;
        int DoShadows = M3d_GetSuper(pItem)->mShadowLights != 0;
        if (DoEllipsoids != 0 || DoShadows != 0) {
            gte_ldsv(&pAnimTransform->t[0]);
            M3dMaths_TransformLastVec(
                (unsigned short *)&M3d_GetSuper(pItem)->mTransform);
            gte_stlvnl(&WorldTransform.t[0]);
            WorldTransform.t[0] >>= 4;
            WorldTransform.t[1] >>= 4;
            WorldTransform.t[2] >>= 4;
            WorldTransform.t[0] += pItem->Position.vx >> 12;
            WorldTransform.t[1] += pItem->Position.vy >> 12;
            WorldTransform.t[2] += pItem->Position.vz >> 12;
        }

        if (DoEllipsoids != 0 &&
            (TestTheWater == 0 ||
             WorldTransform.t[1] <
                 (CurrentWaterLevel >> 12) - 40)) {
            M3dElips_RenderEllipsoid(
                pModel, &WorldTransform, Rgb, Scale, OTPushback,
                pAnimTex->pTexture);
        }

        if (DoShadows != 0) {
            for (int s = 0;
                 s < M3d_GetSuper(pItem)->mShadowLights; s++) {
                if (SubObjectMasked == 0) {
                    M3dElips_RenderShadow(
                        pModel, &WorldTransform,
                        &M3d_GetSuper(pItem)->mShadowNormal,
                        &M3d_GetSuper(pItem)->mShadowPos,
                        &M3d_GetSuper(pItem)->mpShadowSource[s],
                        ((CSuper *)pItem)->mpShadowRGB[s]);
                }
            }
        }
    }

    if ((M3d_GetSuper(pItem)->mExtraFlags & 2) != 0) {
        M3d_GetSuper(pItem)->mTransform.m[0][0] =
            -M3d_GetSuper(pItem)->mTransform.m[0][0];
        M3d_GetSuper(pItem)->mTransform.m[1][0] =
            -M3d_GetSuper(pItem)->mTransform.m[1][0];
        M3d_GetSuper(pItem)->mTransform.m[2][0] =
            -M3d_GetSuper(pItem)->mTransform.m[2][0];
    }
}


/**
 * RenderModel__FP6SModel — Render a mesh model (standard path)
 * Core rendering function for non-character (Item) meshes. Pipeline:
 * 1. Transform and outcode vertices via M3dAsm_TransformAndOutcodeItemVertices
 * 2. If water rendering mode ($gp+0x4988): outcode vertices for water clipping
 * 3. Check outcode result — if all vertices are outside view (bits 0-5,7 set), skip
 * 4. Skip normals (normalCount * 8 bytes past vertices)
 * 5. Set far color from $gp+0x4900/4902/4904
 * 6. If wireframe mode ($gp+0x48F4): render as wireframe, check for mode 3
 * 7. Otherwise: process polygons via M3dAsm_ProcessPolys
 * MeshHeader fields used:
 *   +0x02: vertexCount (u16)
 *   +0x04: normalCount (u16)
 *   +0x06: faceCount (u16)
 *   +0x1C: vertex data start (SVECTOR array)
 * External globals ($gp-relative)
 *
 * Address:   0x80097164, Size: 220 bytes (55 instructions)
 * Mangled:   RenderModel__FP6SModel
 * Demangled: void RenderModel(SModel *)
 * @param pModel  Model data being processed.
 */

void RenderModel(SModel *pModel) {
    SVECTOR *pGeom = pModel->Vertices;
    unsigned int Num = pModel->NumVertices;
    unsigned int Outcode;

    Outcode = M3dAsm_TransformAndOutcodeItemVertices(pGeom, Num);

    if (TestTheWater) {
        M3dAsm_OutcodeVerticesInWater(Num, WaterClipValue);
    }

    pGeom += Num;

    if ((Outcode & 0xBF) != 0) return;

    Num = pModel->NumFaces;
    u32 *pFace = (u32 *)(pGeom + pModel->NumNormals);

    SetFarColor(FarColor.vx, FarColor.vy, FarColor.vz);

    if (M3d_WireframeMode) {
        Wireframe_ProcessPolys(pFace, Num, 0x80);
        if (M3d_WireframeMode && M3d_WireframeMode != 3) return;
    }

    M3dAsm_ProcessPolys(pFace, pGeom, Num);
}

/**
 * RenderModelFast__FP6SModelP6VECTOR — Render mesh with pre-computed translation (fast path)
 * Optimized rendering path for meshes with known translations. Uses
 * M3dAsm_TransformAndOutcodeVerticesFast which takes an additional VECTOR
 * for pre-computed world-space translation, avoiding a matrix multiply.
 *
 * Address:   0x80097240, Size: 244 bytes (61 instructions)
 * Mangled:   RenderModelFast__FP6SModelP6VECTOR
 * Demangled: void RenderModelFast(SModel *, VECTOR *)
 *
 * @param pModel  Model data being processed.
 * @param pPosnRelCam  PosnRelCam to operate on.
 */

void RenderModelFast(SModel *pModel, VECTOR *pPosnRelCam) {
    SVECTOR *pGeom = pModel->Vertices;
    unsigned int NumVerts = pModel->NumVertices;
    unsigned int Outcode = M3dAsm_TransformAndOutcodeVerticesFast(
        pGeom, NumVerts, pPosnRelCam);

    SVECTOR *pVertices = pGeom;
    pGeom += NumVerts;

    if ((Outcode & 0x6000E000) != 0) return;

    unsigned int Num = pModel->NumFaces;
    u32 *pFace = (u32 *)(pGeom + pModel->NumNormals);

    SetFarColor(FarColor.vx, FarColor.vy, FarColor.vz);

    if (M3d_WireframeMode) {
        M3dAsm_TransformAndOutcodeBlockVertices(
            pVertices, NumVerts, pPosnRelCam);
        Wireframe_ProcessPolys(pFace, Num, 0x8000);
        if (M3d_WireframeMode && M3d_WireframeMode != 3) return;
    }

    M3dAsm_ProcessPolysFast(pFace, pGeom, Num);
}

/**
 * RenderBackgroundModel__FP6SModel — Background mesh rendering
 * Background rendering: uses TransformAndOutcodeBackgroundVertices, no water check,
 * no SetFarColor. Outcode mask 0xBF. Wireframe with color 0x8000.
 * Calls M3dAsm_ProcessPolys (not Fast).
 *
 * Address:   0x80097334, Size: 164 bytes (41 instructions)
 * Mangled:   RenderBackgroundModel__FP6SModel
 * Demangled: void RenderBackgroundModel(SModel *)
 * @param pModel  Model data being processed.
 */

void RenderBackgroundModel(SModel *pModel) {
    SVECTOR *pGeom = pModel->Vertices;
    unsigned int Num = pModel->NumVertices;

    unsigned int Outcode =
        M3dAsm_TransformAndOutcodeBackgroundVertices(pGeom, Num);

    pGeom += Num;

    if ((Outcode & 0xBF) != 0) return;

    Num = pModel->NumFaces;
    u32 *pFace = (u32 *)(pGeom + pModel->NumNormals);

    if (M3d_WireframeMode) {
        Wireframe_ProcessPolys(pFace, Num, 0x8000);
        if (M3d_WireframeMode && M3d_WireframeMode != 3) return;
    }

    M3dAsm_ProcessPolys(pFace, pGeom, Num);
}


/**
 * RenderModelInSuper__FP6SModel — Character model rendering (non-fast path)
 * Character rendering with Dewibble check, water support, wireframe, and
 * SubObjectMasked. Uses non-fast Super vertex transforms and ProcessPolys.
 * No SetFarColor. Outcode mask 0xBF. Wireframe color 0x00800080.
 *
 * Address:   0x800973D8, Size: 252 bytes (63 instructions)
 * Mangled:   RenderModelInSuper__FP6SModel
 * Demangled: void RenderModelInSuper(SModel *)
 *
 * @param pModel  Model data being processed.
 */

void RenderModelInSuper(SModel *pModel) {
    SVECTOR *pGeom = pModel->Vertices;
    unsigned int Num = pModel->NumVertices;
    unsigned int Outcode;

    if (Dewibble) {
        Outcode = M3dAsm_TransformAndOutcodeSuperVerticesDewibbled(pGeom, Num);
    } else {
        Outcode = M3dAsm_TransformAndOutcodeSuperVertices(pGeom, Num);
    }

    if (TestTheWater) {
        M3dAsm_OutcodeVerticesInWater(Num, WaterClipValue);
    }

    pGeom += Num;

    if (SubObjectMasked) return;

    if ((Outcode & 0xBF) != 0) return;

    Num = pModel->NumFaces;
    u32 *pFace = (u32 *)(pGeom + pModel->NumNormals);

    if (M3d_WireframeMode) {
        Wireframe_ProcessPolys(pFace, Num, 0x800080);
        if (M3d_WireframeMode && M3d_WireframeMode != 3) return;
    }

    M3dAsm_ProcessPolys(pFace, pGeom, Num);
}


/**
 * RenderModelInSuperFast__FP6SModel — Character rendering with pre-computed translation (fast path)
 * Simplified character mesh rendering: no water check, no wireframe, no far color.
 * Selects between dewibbled/non-dewibbled vertex transform based on Dewibble ($gp+0x4990).
 * Exits early if SubObjectMasked ($gp+0x4984) is set.
 * Outcode mask 0x60006000. Calls ProcessPolysFast.
 *
 * Address:   0x800974D4, Size: 160 bytes (40 instructions)
 * Mangled:   RenderModelInSuperFast__FP6SModel
 * Demangled: void RenderModelInSuperFast(SModel *)
 * @param pModel  Model data being processed.
 */

void RenderModelInSuperFast(SModel *pModel) {
    SVECTOR *pGeom = pModel->Vertices;
    unsigned int Num = pModel->NumVertices;
    unsigned int Outcode;

    if (Dewibble) {
        Outcode = M3dAsm_TransformAndOutcodeSuperVerticesFastDewibbled(
            pGeom, Num);
    } else {
        Outcode = M3dAsm_TransformAndOutcodeSuperVerticesFast(pGeom, Num);
    }

    pGeom += Num;

    if (SubObjectMasked) return;

    if ((Outcode & 0x60006000) != 0) return;

    Num = pModel->NumFaces;
    u32 *pFace = (u32 *)(pGeom + pModel->NumNormals);

    M3dAsm_ProcessPolysFast(pFace, pGeom, Num);
}


/**
 * RenderModelNonRotated__FP6SModelP6VECTOR — Render mesh without rotation (block transform)
 * Like RenderModel but uses TransformAndOutcodeBlockVertices (with pre-computed
 * translation vector) instead of TransformAndOutcodeItemVertices.
 * Wireframe color 0x8080. Outcode mask 0xBF. Calls ProcessPolys (non-fast).
 *
 * Address:   0x80097574, Size: 224 bytes (56 instructions)
 * Mangled:   RenderModelNonRotated__FP6SModelP6VECTOR
 * Demangled: void RenderModelNonRotated(SModel *, VECTOR *)
 *
 * @param pModel  Model data being processed.
 * @param pPosnRelCam  PosnRelCam to operate on.
 */

void RenderModelNonRotated(SModel *pModel, VECTOR *pPosnRelCam) {
    SVECTOR *pGeom = pModel->Vertices;
    unsigned int Num = pModel->NumVertices;
    unsigned int Outcode;

    Outcode = M3dAsm_TransformAndOutcodeBlockVertices(
        pGeom, Num, pPosnRelCam);

    if (TestTheWater) {
        M3dAsm_OutcodeVerticesInWater(Num, WaterClipValue);
    }

    pGeom += Num;

    if ((Outcode & 0xBF) != 0) return;

    Num = pModel->NumFaces;
    u32 *pFace = (u32 *)(pGeom + pModel->NumNormals);

    SetFarColor(FarColor.vx, FarColor.vy, FarColor.vz);

    if (M3d_WireframeMode) {
        Wireframe_ProcessPolys(pFace, Num, 0x8080);
        if (M3d_WireframeMode && M3d_WireframeMode != 3) return;
    }

    M3dAsm_ProcessPolys(pFace, pGeom, Num);
}


/**
 * RenderBackgroundModelNonRotated__FP6SModel — Background rendering without rotation
 * Same as RenderBackgroundModel but wireframe color 0x00202060.
 *
 * Address:   0x80097654, Size: 168 bytes (42 instructions)
 * Mangled:   RenderBackgroundModelNonRotated__FP6SModel
 * Demangled: void RenderBackgroundModelNonRotated(SModel *)
 * @param pModel  Model data being processed.
 */

void RenderBackgroundModelNonRotated(SModel *pModel) {
    SVECTOR *pGeom = pModel->Vertices;
    unsigned int Num = pModel->NumVertices;

    unsigned int Outcode =
        M3dAsm_TransformAndOutcodeBackgroundVertices(pGeom, Num);

    pGeom += Num;

    if ((Outcode & 0xBF) != 0) return;

    Num = pModel->NumFaces;
    u32 *pFace = (u32 *)(pGeom + pModel->NumNormals);

    if (M3d_WireframeMode) {
        Wireframe_ProcessPolys(pFace, Num, 0x202060);
        if (M3d_WireframeMode && M3d_WireframeMode != 3) return;
    }

    M3dAsm_ProcessPolys(pFace, pGeom, Num);
}


/**
 * M3d_BuildTransform__FP6CSuper — Build rotation matrix and translation for character
 * Builds the 3x3 rotation matrix and integer translation vector used to
 * transform a character's skeleton for rendering.
 * Recovered fields used by this owner-local view:
 *   - ItemBase.Flags selects optional scale-matrix processing.
 *   - ItemBase.Position supplies the 20.12 fixed-point translation.
 *   - ItemBase.Angles supplies YXZ Euler angles.
 *   - mExtraFlags selects the precomputed rotation path.
 *   - mTransform receives the final rotation and translation.
 *   - mRotationMatrix is the derived CBruce precomputed matrix at +0xE90.
 * Two rotation paths:
 *   - flags2 bit 0 set: copies pre-computed matrix from +0xE90 to +0x144,
 *     then negates m[0][1], m[1][1], m[2][1] (mirror/flip correction)
 *   - flags2 bit 0 clear: builds fresh matrix from rotation angles via RotMatrixYXZ
 * If flags bit 9 (0x200) is set, applies scale matrix via M3dMaths_ScaleMatrix.
 * Finally, converts 20.12 fixed-point position to integer translation (>>12).
 *
 * Address:   0x800976FC, Size: 188 bytes (47 instructions)
 * Mangled:   M3d_BuildTransform__FP6CSuper
 * Demangled: void M3d_BuildTransform(CSuper *)
 * @param pSuper  Super to operate on.
 */

void M3d_BuildTransform(CSuper *pSuper) {
    if ((M3d_GetTransformLayout(pSuper)->mExtraFlags & 1) != 0) {
        M3dMaths_CopyMat(
            (u16 *)&M3d_GetTransformLayout(pSuper)->mRotationMatrix,
            (u16 *)&M3d_GetTransformLayout(pSuper)->mTransform);
        M3d_GetTransformLayout(pSuper)->mTransform.m[0][1] =
            -M3d_GetTransformLayout(pSuper)->mTransform.m[0][1];
        M3d_GetTransformLayout(pSuper)->mTransform.m[1][1] =
            -M3d_GetTransformLayout(pSuper)->mTransform.m[1][1];
        M3d_GetTransformLayout(pSuper)->mTransform.m[2][1] =
            -M3d_GetTransformLayout(pSuper)->mTransform.m[2][1];
    } else {
        M3dMaths_RotMatrixYXZ(
            (SVECTOR *)&M3d_GetTransformLayout(pSuper)->ItemBase.Angles,
            &M3d_GetTransformLayout(pSuper)->mTransform);
    }

    if ((M3d_GetTransformLayout(pSuper)->ItemBase.Flags & 0x200) != 0) {
        M3dMaths_ScaleMatrix(
            &M3d_GetTransformLayout(pSuper)->ItemBase,
            &M3d_GetTransformLayout(pSuper)->mTransform);
    }

    M3d_GetTransformLayout(pSuper)->mTransform.t[0] =
        M3d_GetTransformLayout(pSuper)->ItemBase.Position.vx >> 12;
    M3d_GetTransformLayout(pSuper)->mTransform.t[1] =
        M3d_GetTransformLayout(pSuper)->ItemBase.Position.vy >> 12;
    M3d_GetTransformLayout(pSuper)->mTransform.t[2] =
        M3d_GetTransformLayout(pSuper)->ItemBase.Position.vz >> 12;
}

/**
 * uWibble__FP15STexWibVertInfo — Texture U coordinate wibble (animation offset)
 * Returns a sine-table-based offset for animating texture U coordinates.
 * Byte 2 of the info struct encodes: upper nibble = amplitude (table row),
 * lower nibble = phase offset. Returns 0 if amplitude is 0.
 *
 * Address:   0x800977B8, Size: 80 bytes (20 instructions)
 * Mangled:   uWibble__FP15STexWibVertInfo
 * Demangled: uWibble(STexWibVertInfo *)
 *
 * @param pTexWibVertInfo  TexWibVertInfo to operate on.
 * @return Signed U-coordinate offset from the selected wibble table.
 */
int uWibble(STexWibVertInfo * pTexWibVertInfo) {
    u32 Amplitude = pTexWibVertInfo->uAmpPhase >> 4;
    int Phase = pTexWibVertInfo->uAmpPhase & 0xF;

    if (Amplitude != 0) {
        int Index = (BasePhase + Phase * 4) & 0x3F;
        s16 *pWibbleTable = WibbleTables + Amplitude * 64;
        return pWibbleTable[Index];
    }
    return 0;
}


/**
 * vWibble__FP15STexWibVertInfo — Texture V coordinate wibble (animation offset)
 * Returns a sine-table-based offset for animating texture V coordinates.
 * Byte 3 of the info struct encodes: upper nibble = amplitude (table row),
 * lower nibble = phase offset. Returns 0 if amplitude is 0.
 *
 * Address:   0x80097808, Size: 80 bytes (20 instructions)
 * Mangled:   vWibble__FP15STexWibVertInfo
 * Demangled: vWibble(STexWibVertInfo *)
 *
 * @param pTexWibVertInfo  TexWibVertInfo to operate on.
 * @return Signed V-coordinate offset from the selected wibble table.
 */
int vWibble(STexWibVertInfo * pTexWibVertInfo) {
    u32 Amplitude = pTexWibVertInfo->vAmpPhase >> 4;
    int Phase = pTexWibVertInfo->vAmpPhase & 0xF;

    if (Amplitude != 0) {
        int Index = (BasePhase + Phase * 4) & 0x3F;
        s16 *pWibbleTable = WibbleTables + Amplitude * 64;
        return pWibbleTable[Index];
    }
    return 0;
}


/**
 * M3d_PreprocessWibblyTextures__Fi
 *
 * Address:   0x80097858
 * Mangled:   M3d_PreprocessWibblyTextures__Fi
 * Demangled: M3d_PreprocessWibblyTextures(int)
 *
 * @param Region  PSX region index.
 */
void M3d_PreprocessWibblyTextures(int Region) {
    u32 *pData;
    u32 *pPSX;
    u32 *pFace;
    STexWibItemInfo *pTexWibItemInfo;
    STexWibFaceInfo *pTexWibFaceInfo;
    Item *pItem;
    SModel *pModel;
    int NumFaces;
    int i;
    s32 uScroll;
    s32 vScroll;
    s32 TileWidth;
    s32 TileHeight;
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;
    s32 u2;
    s32 v2;
    s32 u3;
    s32 v3;
    s32 And;
    s32 Or;
    int Count;

    if (Region == -1) {
        return;
    }

    pData = PSXRegion[Region].pTexWibData;
    if (pData == 0) {
        return;
    }

    pPSX = PSXRegion[Region].pPSX;
    while (*pData != 0) {
        pTexWibItemInfo = (STexWibItemInfo *)pData;
        pItem = (Item *)((u8 *)pPSX + pTexWibItemInfo->ItemOffset);
        pTexWibFaceInfo = (STexWibFaceInfo *)(pTexWibItemInfo + 1);
        NumFaces = pTexWibItemInfo->NumFaces;

        if ((pItem->Flags & 0x8000) != 0) {
            pData = (u32 *)(pTexWibFaceInfo + NumFaces);
            continue;
        }

        pModel = PSXRegion[pItem->Region].ppModels[pItem->Model];
        pFace = (u32 *)(pModel->Vertices + pModel->NumVertices +
                       pModel->NumNormals);

        uScroll = (s32)(XblanksNow * pTexWibItemInfo->uVel) >> 4;
        vScroll = (s32)(XblanksNow * pTexWibItemInfo->vVel) >> 4;
        BasePhase =
            (s32)(XblanksNow * pTexWibItemInfo->Frequency) >> 10;
        int ZeroUAmplitudes = pTexWibItemInfo->ZeroUAmplitudes;
        int ZeroVAmplitudes = pTexWibItemInfo->ZeroVAmplitudes;

        for (i = 0; i < NumFaces;
             i++,
             pFace += (*(u16 *)((u8 *)pFace + 2) & 0xFFFC) >> 2,
             pTexWibFaceInfo++) {
            TileWidth = ((-(pFace[7] & 0x001F) << 3) & 0xFF) << 8;
            TileHeight = ((-(pFace[7] & 0x03E0) >> 2) & 0xFF) << 8;

            u0 = pTexWibFaceInfo->p0.u << 8;
            v0 = pTexWibFaceInfo->p0.v << 8;
            u1 = pTexWibFaceInfo->p1.u << 8;
            v1 = pTexWibFaceInfo->p1.v << 8;
            u2 = pTexWibFaceInfo->p2.u << 8;
            v2 = pTexWibFaceInfo->p2.v << 8;
            u3 = pTexWibFaceInfo->p3.u << 8;
            v3 = pTexWibFaceInfo->p3.v << 8;

            if (uScroll != 0) {
                u0 += uScroll;
                u1 += uScroll;
                u2 += uScroll;
                u3 += uScroll;
            }

            if (vScroll != 0) {
                v0 += vScroll;
                v1 += vScroll;
                v2 += vScroll;
                v3 += vScroll;
            }

            if (ZeroUAmplitudes == 0) {
                u0 += uWibble(&pTexWibFaceInfo->p0);
                u1 += uWibble(&pTexWibFaceInfo->p1);
                u2 += uWibble(&pTexWibFaceInfo->p2);
                u3 += uWibble(&pTexWibFaceInfo->p3);
            }

            if (ZeroVAmplitudes == 0) {
                v0 += vWibble(&pTexWibFaceInfo->p0);
                v1 += vWibble(&pTexWibFaceInfo->p1);
                v2 += vWibble(&pTexWibFaceInfo->p2);
                v3 += vWibble(&pTexWibFaceInfo->p3);
            }

            And = u0 & u1 & u2 & u3;
            Or = u0 | u1 | u2 | u3;
            Count = 0;
            while (((And ^ Or) & 0x10000) != 0 && Count < 16) {
                u0 += TileWidth;
                u1 += TileWidth;
                u2 += TileWidth;
                u3 += TileWidth;
                And = u0 & u1 & u2 & u3;
                Or = u0 | u1 | u2 | u3;
                Count++;
            }

            And = v0 & v1 & v2 & v3;
            Or = v0 | v1 | v2 | v3;
            Count = 0;
            while (((And ^ Or) & 0x10000) != 0 && Count < 16) {
                v0 += TileHeight;
                v1 += TileHeight;
                v2 += TileHeight;
                v3 += TileHeight;
                And = v0 & v1 & v2 & v3;
                Or = v0 | v1 | v2 | v3;
                Count++;
            }

            u0 = (u0 & 0xFF00) >> 8;
            v0 = (v0 & 0xFF00) >> 8;
            u1 = (u1 & 0xFF00) >> 8;
            v1 = (v1 & 0xFF00) >> 8;
            u2 = (u2 & 0xFF00) >> 8;
            v2 = (v2 & 0xFF00) >> 8;
            u3 = (u3 & 0xFF00) >> 8;
            v3 = (v3 & 0xFF00) >> 8;

            *(u16 *)&pFace[4] = u0 | (v0 << 8);
            *(u16 *)&pFace[5] = u1 | (v1 << 8);
            pFace[6] = u2 | (v2 << 8) | (u3 << 16) | (v3 << 24);

        }

        pData = (u32 *)pTexWibFaceInfo;
    }
}


/**
 * Precompute the region's pulsing vertex-colour stream for this frame.
 *
 * Address:   0x80097CD0
 * Mangled:   M3d_PreprocessPulsingColours__Fi
 * Demangled: M3d_PreprocessPulsingColours(int)
 *
 * @param Region  PSX region index.
 */
struct SColourPulseInfo {
    u8 VertexColourIndex;
    u8 ListLen;
    u8 ListPos;
    u8 t;
    SRGBI RGBs[1];
};

void M3d_PreprocessPulsingColours(int Region) {
    u32 *pData;
    u32 *pEnd;
    u32 dt;
    u32 ListLen;
    u32 ListPos;
    u32 OldListPos;
    int t;
    int rgb0[3];
    int drgb[3];
    SRGBI *RGBs;
    SColourPulseInfo *pColourPulseInfo;

    if (Region == -1) {
        return;
    }

    pData = PSXRegion[Region].pColourPulseData;
    if (pData == 0) {
        return;
    }

    pEnd = (u32 *)((u8 *)pData + pData[-1]);
    pColourTable = PSXRegion[Region].pColourTable;
    if ((u32)XblanksNow < (u32)XblanksThen) {
        dt = 1;
    } else {
        dt = (u32)XblanksNow - (u32)XblanksThen;
    }

    while (pData < pEnd) {
        pColourPulseInfo = (SColourPulseInfo *)pData;
        RGBs = pColourPulseInfo->RGBs;
        ListLen = pColourPulseInfo->ListLen;
        ListPos = pColourPulseInfo->ListPos;
        t = pColourPulseInfo->t;

        t += dt;
        while (t >= RGBs[ListPos].Interval) {
            t -= RGBs[ListPos].Interval;
            ListPos++;
            if (ListPos == ListLen) {
                ListPos = 0;
            }
        }

        pColourPulseInfo->ListPos = (u8)ListPos;
        pColourPulseInfo->t = (u8)t;

        rgb0[0] = RGBs[ListPos].r << 4;
        rgb0[1] = RGBs[ListPos].g << 4;
        rgb0[2] = RGBs[ListPos].b << 4;

        OldListPos = ListPos;
        ListPos++;
        if (ListPos == ListLen) {
            ListPos = 0;
        }

        drgb[0] = ((int)RGBs[ListPos].r << 4) - rgb0[0];
        drgb[1] = ((int)RGBs[ListPos].g << 4) - rgb0[1];
        drgb[2] = ((int)RGBs[ListPos].b << 4) - rgb0[2];

        gte_ldrgb_word(0);
        gte_lddp((t << 12) / RGBs[OldListPos].Interval);
        gte_ldlvnl(&rgb0[0]);
        gte_ldlvl(&drgb[0]);
        gte_gpl12();
        gte_strgb(&pColourTable[pColourPulseInfo->VertexColourIndex]);
        pData = (u32 *)(pColourPulseInfo + 1) + ListLen - 1;
    }
}
