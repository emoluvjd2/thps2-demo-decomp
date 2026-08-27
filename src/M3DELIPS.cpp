#include "M3DELIPS.h"
#include "globals.h"
#include "MAIN.h"
#include "DB.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include "SMODEL.h"
#include <INLINE_C.H>
#include "gte_extensions.h"

/** Compact GTE vector recorded for the ellipsoid working set. */
struct Vec {
    short vx;
    short vy;
    short vz;
};

#include "SCAMERA.h"

#define M3D_VERTEX_WIBBLE_LAYOUTS
#include "M3D.h"
#undef M3D_VERTEX_WIBBLE_LAYOUTS

/** Fixed header of a loaded PSX model. */
struct M3dElipsBounds {
    short XMax;
    short XMin;
    short YMax;
    short YMin;
    short ZMax;
    short ZMin;
};

struct tag_S_Pal;

#include "TEXTURE.h"

typedef char M3dElips_SDK_SVECTOR_size_must_be_8[
    (sizeof(SVECTOR) == 8) ? 1 : -1];
typedef char M3dElips_Bounds_size_must_be_12[
    (sizeof(M3dElipsBounds) == 12) ? 1 : -1];
typedef char M3dElips_Texture_size_must_be_40[(sizeof(Texture) == 40) ? 1 : -1];
typedef char M3dElips_Vec_size_must_be_6[(sizeof(Vec) == 6) ? 1 : -1];

#include <LIBGPU.H>

extern "C" int M3dElips_MinDotProd;
#include "CURRENTCAMERA.h"
#include "PROJECTEDVERTICES.h"
#define BITSTATE_DECLARE_SHADOW_TEXTURE
#include "BITSTATE.h"

// Maths helpers retain their proven pointer signatures from MAIN.SYM.
#include "M3DMATHS.h"
void M3dMaths_ZeroMat(unsigned short *matrix);
void M3dMaths_TransformVec0(unsigned short *matrix);
int M3dMaths_LastVecMagnitude(void);
void M3dMaths_CrossWithLastVec12(Vec *vector);
void M3dMaths_NormaliseLastVec(void);
void M3dMaths_ScaleLastVec12(int scale);
void M3dMaths_AddScaledLastVec12(Vec *vector, int scale);

typedef short M3dElipsMatrix[3][3];
typedef char M3dElips_Matrix_size_must_be_18[
    (sizeof(M3dElipsMatrix) == 18) ? 1 : -1];

// The original maths ABI exposes matrix storage as unsigned 16-bit words.
// Keep that representation conversion named at the boundary instead of
// scattering raw casts throughout the projection calculations.
#define M3dElips_MatrixWords(matrix) ((unsigned short *)(matrix))
#define M3dElips_VectorComponents(vector) ((Vec *)(vector))
#define M3dElips_OrderingTableEntry(table, index)                         \
    ((unsigned int *)((index) * sizeof(unsigned int) + (unsigned int)(table)))

// GPU primitives are consumed as packed words while the shared allocation
// cursor is word-addressed. Name those representation boundaries explicitly.
#define M3dElips_QuadFromWords(words) ((POLY_FT4 *)(words))
#define M3dElips_WordsFromQuad(quad) ((unsigned int *)(quad))
#define M3dElips_ByteAddress(words) ((unsigned char *)(words))
#define M3dElips_PackedWord(firstByte) (*(unsigned int *)&(firstByte))

/**
 * Projects an ellipsoid along the supplied light vector onto a plane, builds
 * its camera-space bounding quad, and inserts the textured shadow primitive
 * into the ordering table.
 *
 * @param pModel Model whose bounding box defines the shadow ellipsoid.
 * @param pTransform Model-to-world transform.
 * @param pNormal Plane normal.
 * @param pPoint Point on the receiving plane.
 * @param pLightVec Direction along which the ellipsoid is projected.
 * @param rgb Primitive colour.
 */
void M3dElips_RenderShadow(
    SModel *pModel, MATRIX *pTransform, CSVector *pNormal,
    CVector *pPoint, SVECTOR *pLightVec, CVECTOR rgb) {
    short a;
    short b;
    short c;
    short d;
    short Mag_d_m;
    int d12;
    int k;
    int n_dot_l;
    M3dElipsMatrix R;
    M3dElipsMatrix RT;
    M3dElipsMatrix d_Ainv;
    M3dElipsMatrix M;
    Vec t;
    Vec v;
    Vec d_m;
    Vec Scaled_q;
    Vec e;
    Vec f;
    Vec DotProds;
    Vec Centre;
    Vec e_Proj;
    Vec f_Proj;
    Vec Vertex;
    CVector d_m_Squared;
    POLY_FT4 *pQuad;
    unsigned int OTPos;
    int t1;
    int t2;
    int t3;

    a = ((((M3dElipsBounds *)&pModel->Box)->XMax -
          ((M3dElipsBounds *)&pModel->Box)->XMin) >> 1) * 5 >> 2;
    b = ((((M3dElipsBounds *)&pModel->Box)->YMax -
          ((M3dElipsBounds *)&pModel->Box)->YMin) >> 1) * 11 >> 3;
    c = ((((M3dElipsBounds *)&pModel->Box)->ZMax -
          ((M3dElipsBounds *)&pModel->Box)->ZMin) >> 1) * 5 >> 2;

    Centre.vx = (((M3dElipsBounds *)&pModel->Box)->XMax +
                 ((M3dElipsBounds *)&pModel->Box)->XMin) >> 1;
    Centre.vy = (((M3dElipsBounds *)&pModel->Box)->YMax +
                 ((M3dElipsBounds *)&pModel->Box)->YMin) >> 1;
    Centre.vz = (((M3dElipsBounds *)&pModel->Box)->ZMax +
                 ((M3dElipsBounds *)&pModel->Box)->ZMin) >> 1;

    gte_ldsv(&Centre);
    M3dMaths_TransformLastVec(M3dElips_MatrixWords(pTransform));
    gte_stsv(&t);

    t.vx += pTransform->t[0];
    t.vy += pTransform->t[1];
    t.vz += pTransform->t[2];
    d = (a + b + c) / 3;

    M3dMaths_CopyMat(
        M3dElips_MatrixWords(pTransform), M3dElips_MatrixWords(R));
    M3dMaths_TransposeMat(
        M3dElips_MatrixWords(R), M3dElips_MatrixWords(RT));
    M3dMaths_ZeroMat(M3dElips_MatrixWords(d_Ainv));

    d12 = d << 12;
    d_Ainv[0][0] = d12 / a;
    d_Ainv[1][1] = d12 / b;
    d_Ainv[2][2] = d12 / c;

    gte_ldsv(pLightVec);
    M3dMaths_TransformLastVec(M3dElips_MatrixWords(RT));
    M3dMaths_NormaliseLastVec();
    M3dMaths_TransformLastVec(M3dElips_MatrixWords(d_Ainv));
    gte_stsv(&d_m);
    Mag_d_m = M3dMaths_LastVecMagnitude();
    gte_stlvnl(&d_m_Squared);

    v.vz = 0;
    v.vy = 0;
    v.vx = 0;
    if ((d_m_Squared.vx <= d_m_Squared.vy) &&
        (d_m_Squared.vx <= d_m_Squared.vz)) {
        v.vx = 4096;
    } else if ((d_m_Squared.vy <= d_m_Squared.vx) &&
               (d_m_Squared.vy <= d_m_Squared.vz)) {
        v.vy = 4096;
    } else {
        v.vz = 4096;
    }

    gte_ldsv(&d_m);
    M3dMaths_CrossWithLastVec12(&v);
    gte_stsv(&Scaled_q);
    M3dMaths_NormaliseLastVec();

    k = (((((a * b) / d) * c) * Mag_d_m) / d) >> 12;

    M3dMaths_TransformLastVec(M3dElips_MatrixWords(d_Ainv));
    M3dMaths_TransformLastVec(M3dElips_MatrixWords(R));
    M3dMaths_CrossWithLastVec12(M3dElips_VectorComponents(pLightVec));
    M3dMaths_ScaleLastVec12(k);
    gte_stsv(&e);

    gte_ldsv(&d_m);
    M3dMaths_CrossWithLastVec12(&Scaled_q);
    M3dMaths_NormaliseLastVec();
    M3dMaths_TransformLastVec(M3dElips_MatrixWords(d_Ainv));
    M3dMaths_TransformLastVec(M3dElips_MatrixWords(R));
    M3dMaths_CrossWithLastVec12(M3dElips_VectorComponents(pLightVec));
    M3dMaths_ScaleLastVec12(k);
    gte_stsv(&f);

    gte_ldv0(pNormal);
    M3dMaths_TransformVec0(M3dElips_MatrixWords(pLightVec));
    gte_stlvnl0(&n_dot_l);
    if (n_dot_l < M3dElips_MinDotProd) {
        return;
    }

    M[0][0] = (pPoint->vx >> 12) - t.vx;
    M[0][1] = (pPoint->vy >> 12) - t.vy;
    M[0][2] = (pPoint->vz >> 12) - t.vz;
    M[1][0] = e.vx;
    M[1][1] = e.vy;
    M[1][2] = e.vz;
    M[2][0] = f.vx;
    M[2][1] = f.vy;
    M[2][2] = f.vz;

    M3dMaths_TransformVec0(M3dElips_MatrixWords(M));
    gte_stsv(&DotProds);
    if (DotProds.vx > 0) {
        return;
    }

    t.vx -= pCurrentCamera->Position.vx;
    t.vy -= pCurrentCamera->Position.vy;
    t.vz -= pCurrentCamera->Position.vz;

    gte_ldsv(pLightVec);
    M3dMaths_AddScaledLastVec12(&t, (DotProds.vx << 12) / n_dot_l);
    gte_stsv(&Centre);

    gte_ldsv(pLightVec);
    M3dMaths_AddScaledLastVec12(&e, -(DotProds.vy << 12) / n_dot_l);
    gte_stsv(&e_Proj);

    gte_ldsv(pLightVec);
    M3dMaths_AddScaledLastVec12(&f, -(DotProds.vz << 12) / n_dot_l);
    gte_stsv(&f_Proj);

    gte_SetRotMatrix(&pCurrentCamera->View);
    gte_ZeroTransVector();

    t1 = Centre.vx - e_Proj.vx - f_Proj.vx;
    t2 = Centre.vy - e_Proj.vy - f_Proj.vy;
    t3 = Centre.vz - e_Proj.vz - f_Proj.vz;
    gte_ldvxy0((t2 << 16) + (t1 & 0xffff));
    gte_ldvz0(t3);

    t1 = Centre.vx + e_Proj.vx - f_Proj.vx;
    t2 = Centre.vy + e_Proj.vy - f_Proj.vy;
    t3 = Centre.vz + e_Proj.vz - f_Proj.vz;
    gte_ldvxy1((t2 << 16) + (t1 & 0xffff));
    gte_ldvz1(t3);

    t1 = Centre.vx - e_Proj.vx + f_Proj.vx;
    t2 = Centre.vy - e_Proj.vy + f_Proj.vy;
    t3 = Centre.vz - e_Proj.vz + f_Proj.vz;
    gte_ldvxy2((t2 << 16) + (t1 & 0xffff));
    gte_ldvz2(t3);
    gte_rtpt();

    t1 = Centre.vx + e_Proj.vx + f_Proj.vx;
    t2 = Centre.vy + e_Proj.vy + f_Proj.vy;
    t3 = Centre.vz + e_Proj.vz + f_Proj.vz;
    gte_stsxy0(&ProjectedVertices[0]);
    gte_stsxy1(&ProjectedVertices[1]);
    gte_stsxy2(&ProjectedVertices[2]);

    gte_ldvxy0((t2 << 16) + (t1 & 0xffff));
    gte_ldvz0(t3);
    gte_rtps();
    gte_stsxy2(&ProjectedVertices[3]);
    gte_avsz4();
    gte_stotz(&OTPos);

    if (OTPos > 4095) {
        OTPos = 4095;
    }

    pQuad = M3dElips_QuadFromWords(pPoly);
    pPoly += sizeof(*pQuad) / sizeof(*pPoly);
    if (M3dElips_ByteAddress(pPoly) > PolyBufferEnd) {
        pPoly = M3dElips_WordsFromQuad(pQuad);
        return;
    }

    M3dElips_PackedWord(pQuad->r0) =
        (M3dElips_PackedWord(rgb.r) & 0x00ffffff) | 0x2e000000;
    setlen(pQuad, 9);

    {
        unsigned int t0 = M3dElips_PackedWord(ProjectedVertices[0].X);
        unsigned int t1 = M3dElips_PackedWord(ProjectedVertices[1].X);
        unsigned int t2 = M3dElips_PackedWord(ProjectedVertices[2].X);
        unsigned int t3 = M3dElips_PackedWord(ProjectedVertices[3].X);
        M3dElips_PackedWord(pQuad->x0) = t0;
        M3dElips_PackedWord(pQuad->x1) = t1;
        M3dElips_PackedWord(pQuad->x2) = t2;
        M3dElips_PackedWord(pQuad->x3) = t3;
    }

    setUV4(pQuad,
           pShadowTexture->u0, pShadowTexture->v0,
           pShadowTexture->u1 - 1, pShadowTexture->v1,
           pShadowTexture->u2, pShadowTexture->v2 - 1,
           pShadowTexture->u3 - 1, pShadowTexture->v3 - 1);
    pQuad->clut = pShadowTexture->clut;

    pQuad->tpage = (pShadowTexture->tpage & 0xff9f) | 0x40;
    addPrim(M3dElips_OrderingTableEntry(CurrentOrderingTable, OTPos), pQuad);
}


/**
 * Builds the camera-facing quad that bounds a transformed ellipsoid.  The
 * axes are derived in inverse ellipsoid space, transformed back into model
 * space, and projected through the GTE before the textured primitive is
 * inserted into the ordering table.
 *
 * @param pModel Model whose bounding box defines the ellipsoid.
 * @param pTransform Model-to-world transform.
 * @param Rgb Packed primitive colour and command value.
 * @param Scale 12-bit fixed-point ellipsoid scale.
 * @param OTPushback Ordering-table depth adjustment.
 * @param pTex Texture applied to the generated quad.
 */
void M3dElips_RenderEllipsoid(
    SModel *pModel, MATRIX *pTransform, unsigned int Rgb, int Scale,
    int OTPushback, Texture *pTex) {
    short a;
    short b;
    short c;
    short d;
    short Mag_d_m;
    int d12;
    int k;
    M3dElipsMatrix R;
    M3dElipsMatrix RT;
    M3dElipsMatrix d_Ainv;
    Vec l;
    Vec t;
    Vec v;
    Vec d_m;
    Vec Scaled_q;
    Vec e;
    Vec f;
    Vec Vertex;
    Vec Centre;
    CVector d_m_Squared;
    POLY_FT4 *pQuad;
    unsigned int OTPos;

    a = ((((M3dElipsBounds *)&pModel->Box)->XMax -
          ((M3dElipsBounds *)&pModel->Box)->XMin) >> 1) * Scale >> 12;
    b = ((((M3dElipsBounds *)&pModel->Box)->YMax -
          ((M3dElipsBounds *)&pModel->Box)->YMin) >> 1) * Scale >> 12;
    c = ((((M3dElipsBounds *)&pModel->Box)->ZMax -
          ((M3dElipsBounds *)&pModel->Box)->ZMin) >> 1) * Scale >> 12;

    Centre.vx = (((M3dElipsBounds *)&pModel->Box)->XMax +
                 ((M3dElipsBounds *)&pModel->Box)->XMin) >> 1;
    Centre.vy = (((M3dElipsBounds *)&pModel->Box)->YMax +
                 ((M3dElipsBounds *)&pModel->Box)->YMin) >> 1;
    Centre.vz = (((M3dElipsBounds *)&pModel->Box)->ZMax +
                 ((M3dElipsBounds *)&pModel->Box)->ZMin) >> 1;

    gte_ldsv(&Centre);
    M3dMaths_TransformLastVec(M3dElips_MatrixWords(pTransform));
    gte_stsv(&t);

    t.vx += pTransform->t[0];
    t.vy += pTransform->t[1];
    t.vz += pTransform->t[2];
    t.vx -= pCurrentCamera->Position.vx;
    t.vy -= pCurrentCamera->Position.vy;
    t.vz -= pCurrentCamera->Position.vz;

    gte_ldsv(&t);
    M3dMaths_NormaliseLastVec();
    gte_stsv(&l);

    d = (a + b + c) / 3;

    M3dMaths_CopyMat(
        M3dElips_MatrixWords(pTransform), M3dElips_MatrixWords(R));
    M3dMaths_TransposeMat(
        M3dElips_MatrixWords(R), M3dElips_MatrixWords(RT));
    M3dMaths_ZeroMat(M3dElips_MatrixWords(d_Ainv));

    d12 = d << 12;
    d_Ainv[0][0] = d12 / a;
    d_Ainv[1][1] = d12 / b;
    d_Ainv[2][2] = d12 / c;

    gte_ldsv(&l);
    M3dMaths_TransformLastVec(M3dElips_MatrixWords(RT));
    M3dMaths_NormaliseLastVec();
    M3dMaths_TransformLastVec(M3dElips_MatrixWords(d_Ainv));
    gte_stsv(&d_m);
    Mag_d_m = M3dMaths_LastVecMagnitude();
    gte_stlvnl(&d_m_Squared);

    v.vz = 0;
    v.vy = 0;
    v.vx = 0;
    if ((d_m_Squared.vx <= d_m_Squared.vy) &&
        (d_m_Squared.vx <= d_m_Squared.vz)) {
        v.vx = 4096;
    } else if ((d_m_Squared.vy <= d_m_Squared.vx) &&
               (d_m_Squared.vy <= d_m_Squared.vz)) {
        v.vy = 4096;
    } else {
        v.vz = 4096;
    }

    gte_ldsv(&d_m);
    M3dMaths_CrossWithLastVec12(&v);
    gte_stsv(&Scaled_q);
    M3dMaths_NormaliseLastVec();

    k = (((((a * b) / d) * c) * Mag_d_m) / d) >> 12;

    M3dMaths_TransformLastVec(M3dElips_MatrixWords(d_Ainv));
    M3dMaths_TransformLastVec(M3dElips_MatrixWords(R));
    M3dMaths_CrossWithLastVec12(&l);
    M3dMaths_ScaleLastVec12(k);
    gte_stsv(&e);

    gte_ldsv(&d_m);
    M3dMaths_CrossWithLastVec12(&Scaled_q);
    M3dMaths_NormaliseLastVec();
    M3dMaths_TransformLastVec(M3dElips_MatrixWords(d_Ainv));
    M3dMaths_TransformLastVec(M3dElips_MatrixWords(R));
    M3dMaths_CrossWithLastVec12(&l);
    M3dMaths_ScaleLastVec12(k);
    gte_stsv(&f);

    gte_SetRotMatrix(&pCurrentCamera->View);
    gte_ZeroTransVector();

    Vertex.vx = t.vx - e.vx - f.vx;
    Vertex.vy = t.vy - e.vy - f.vy;
    Vertex.vz = t.vz - e.vz - f.vz;
    gte_ldv0(&Vertex);

    Vertex.vx = t.vx + e.vx - f.vx;
    Vertex.vy = t.vy + e.vy - f.vy;
    Vertex.vz = t.vz + e.vz - f.vz;
    gte_ldv1(&Vertex);

    Vertex.vx = t.vx - e.vx + f.vx;
    Vertex.vy = t.vy - e.vy + f.vy;
    Vertex.vz = t.vz - e.vz + f.vz;
    gte_ldv2(&Vertex);
    gte_rtpt();

    Vertex.vx = t.vx + e.vx + f.vx;
    Vertex.vy = t.vy + e.vy + f.vy;
    Vertex.vz = t.vz + e.vz + f.vz;
    gte_stsxy0(&ProjectedVertices[0]);
    gte_stsxy1(&ProjectedVertices[1]);
    gte_stsxy2(&ProjectedVertices[2]);

    gte_ldv0(&Vertex);
    gte_rtps();
    gte_stsxy2(&ProjectedVertices[3]);
    gte_avsz4();
    gte_stotz(&OTPos);

    OTPos += OTPushback;
    if (OTPos > 4095) {
        OTPos = 4095;
    }

    pQuad = M3dElips_QuadFromWords(pPoly);
    pPoly += sizeof(*pQuad) / sizeof(*pPoly);
    if (M3dElips_ByteAddress(pPoly) > PolyBufferEnd) {
        pPoly = M3dElips_WordsFromQuad(pQuad);
    } else {
        M3dElips_PackedWord(pQuad->r0) = Rgb;
        setPolyFT4(pQuad);
        setSemiTrans(pQuad, 1);

        setXY4(pQuad,
               ProjectedVertices[0].X, ProjectedVertices[0].Y,
               ProjectedVertices[1].X, ProjectedVertices[1].Y,
               ProjectedVertices[2].X, ProjectedVertices[2].Y,
               ProjectedVertices[3].X, ProjectedVertices[3].Y);

        setUV4(pQuad,
               pTex->u0, pTex->v0,
               pTex->u1 - 1, pTex->v1,
               pTex->u2, pTex->v2 - 1,
               pTex->u3 - 1, pTex->v3 - 1);
        pQuad->clut = pTex->clut;
        pQuad->tpage = (pTex->tpage & 0xff9f) | 0x20;

        addPrim(
            M3dElips_OrderingTableEntry(CurrentOrderingTable, OTPos), pQuad);
    }
}
