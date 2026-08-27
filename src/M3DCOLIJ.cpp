// M3DCOLIJ.cpp -- Reconstructed translation unit
// Original: M3DCOLIJ.CPP
// Functions: 12
// Address range: 0x8008FFC0 - 0x800927E4
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#define M3DCOLIJ_OWNER_DECLARATIONS
#include "M3DCOLIJ.h"
#undef M3DCOLIJ_OWNER_DECLARATIONS
#define M3DCOLIJ_OWNER_LAYOUT
#include "M3DCOLIJ.h"
#undef M3DCOLIJ_OWNER_LAYOUT
#include "TRIG.h"
#define M3DMATHS_TYPED_API
#include "M3DMATHS.h"
#undef M3DMATHS_TYPED_API
#include "MIPS.h"
#include <INLINE_C.H>
#include "gte_extensions.h"

SHit NearestHit;

typedef char M3dColij_NearestHit_size_must_be_36[
    (sizeof(SHit) == 36) ? 1 : -1];

/** Collision-query state shared by the line and face cache paths. */
u16 Inquiry __attribute__((section(".sdata"))) = 0xffff;
int CameraCollisionCheck __attribute__((section(".sdata"))) = 0;
int BaddyCollisionCheck __attribute__((section(".sdata"))) = 0;
int BulletCollisionCheck __attribute__((section(".sdata"))) = 0;
int TriggerCollisionCheck __attribute__((section(".sdata"))) = 0;
int LineOfSightCheck __attribute__((section(".sdata"))) = 0;
int ItemCacheIndex __attribute__((section(".sdata"))) = 0;
int ItemCacheStart __attribute__((section(".sdata"))) = -1;
int FaceCacheIndex __attribute__((section(".sdata"))) = 0;
int FaceCacheStart __attribute__((section(".sdata"))) = -1;
int BB_Hit __attribute__((section(".sdata"))) = 0;
int BB_Miss __attribute__((section(".sdata"))) = 0;
int BB_PolyHit __attribute__((section(".sdata"))) = 0;
int BB_PolyMiss __attribute__((section(".sdata"))) = 0;
int DebugCol __attribute__((section(".sdata"))) = 0;
u32 M3dColij_ZeroMask __attribute__((section(".sdata"))) = 0;
u32 M3dColij_OneMask __attribute__((section(".sdata"))) = 0;
u32 M3dColij_ZeroOneMask __attribute__((section(".sdata"))) = 0;
CBody *pBodyHit __attribute__((section(".sdata"))) = 0;
int CollidedTriggerModel __attribute__((section(".sdata"))) = 0;

/** Reusable line-query record that follows the nearest-hit owner in `.data`. */
SLineInfo GColInfo __attribute__((section(".data"))) = {0};

// === Free function declarations ===
void NextInquiry(void);
void M3dColij_LineToItem(Item *pItem, SLineInfo *pInfo);
#define M3DCOLIJ_DECLARE_FLUSH_CACHE_SERVICE
#include "M3DCOLIJ.h"
void M3dColij_LineToThisItemCached(Item *pItem, SLineInfo *pInfo, SColCache *pColCache);
int FitFacesAtCacheEnd(int faces);
void RemoveFacesFromCache(void);
void M3dColij_LineToItemZoned(Item **ppItem, SLineInfo *pInfo);
int M3dColij_GetLineInfo(SLineInfo *pInfo);
void M3dColij_LineToThisItem(Item *pItem, SLineInfo *pInfo);
#define M3DCOLIJ_DECLARE_LINE_TO_SPHERE_SERVICE
#include "M3DCOLIJ.h"

// === Source-owned collision transform and caches ===
MATRIX Tfm __attribute__((section(".data"))) = {0};
SColCache ItemCache[20] __attribute__((section(".data"))) = {{0}};
SFaceCache FaceCache[500] __attribute__((section(".data"))) = {{0}};

// === Function implementations (address order) ===

/**
 * NextInquiry__Fv
 *
 * Address:   0x8008FFC0
 * Mangled:   NextInquiry__Fv
 * Demangled: NextInquiry(void)
 *            Increments 16-bit inquiry counter; on wrap (0), resets to 1 and clears
 *            inquiry flags (halfword at +2) in both EnviroList and EnvironmentalObjectList chains.
 *
 */
void NextInquiry(void)
{
    Inquiry = Inquiry + 1;
    if (Inquiry == 0)
    {
        Item *pItem = EnviroList;
        Inquiry = 1;
        while (pItem)
        {
            pItem->Inquiry = 0;
            pItem = pItem->NextItem;
        }
        pItem = (Item *)EnvironmentalObjectList;
        while (pItem)
        {
            pItem->Inquiry = 0;
            pItem = pItem->NextItem;
        }
    }
}

/**
 * Builds the collision-segment transform, bounds, length, and inquiry stamp.
 *
 * @param pInfo Collision query initialized in place.
 */
void M3dColij_InitLineInfo(SLineInfo *pInfo)
{
    short *pShort;
    SVECTOR L;
    SVECTOR AbsL;
    Vector *p0;
    Vector *p1;

    pInfo->pItem = 0;
    pInfo->pFace = 0;
    pInfo->Distance = 0x7fffffff;
    pInfo->tNear = 0x7fffffff;
    pInfo->Model = -1;
    pInfo->RecordTriggerZoneHits = 0;
    pInfo->DropDown = 0;

    L.vx = pInfo->EndCoords.vx - pInfo->StartCoords.vx >> 12;
    L.vy = pInfo->EndCoords.vy - pInfo->StartCoords.vy >> 12;
    L.vz = pInfo->EndCoords.vz - pInfo->StartCoords.vz >> 12;

    int lzc;
    int Shift1;
    int Shift2;
    int s1;
    int s2;
    int s1Norm;
    int s2Norm;
    int r1Norm;
    int r2Norm;
    short Cos;
    short Sin;

    s1 = L.vx * L.vx + L.vz * L.vz;

    if (s1)
    {
        gte_ldlzc(s1);
        gte_nop();
        gte_nop();
        gte_stlzc(&lzc);
        Shift1 = lzc - 1 >> 1;

        s1Norm = s1 << (lzc - 1 & 0x1e);
        r1Norm = M3dMaths_SquareRoot0(s1Norm);

        Sin = (L.vx << 12 + Shift1) / r1Norm;
        Cos = (L.vz << 12 + Shift1) / r1Norm;

        pShort = &Tfm.m[0][0];
        *pShort++ = Cos;
        *pShort++ = 0;
        *pShort++ = -Sin;
        *pShort++ = 0;
        *pShort++ = 0x1000;
        *pShort++ = 0;
        *pShort++ = Sin;
        *pShort++ = 0;
        *pShort = Cos;

        s2 = s1 + L.vy * L.vy;

        gte_ldlzc(s2);
        gte_nop();
        gte_nop();
        gte_stlzc(&lzc);
        Shift2 = lzc - 1 >> 1;

        s2Norm = s2 << (lzc - 1 & 0x1e);
        r2Norm = M3dMaths_SquareRoot0(s2Norm);
        pInfo->Length = r2Norm >> Shift2;

        Sin = (L.vy << 12 + Shift2) / r2Norm;
        Cos = (r1Norm << 12) / (r2Norm << Shift1 - Shift2);

        pShort = &pInfo->WorldCst.m[0][0];
        *pShort++ = 0x1000;
        *pShort++ = 0;
        *pShort++ = 0;
        *pShort++ = 0;
        *pShort++ = Cos;
        *pShort++ = -Sin;
        *pShort++ = 0;
        *pShort++ = Sin;
        *pShort = Cos;

        MulMatrix(&pInfo->WorldCst, &Tfm);
    }
    else
    {
        if (L.vy < 0)
        {
            Sin = -0x1000;
            pInfo->Length = -L.vy;
        }
        else
        {
            Sin = 0x1000;
            pInfo->Length = L.vy;
            pInfo->DropDown = 1;
        }

        pShort = &pInfo->WorldCst.m[0][0];
        *pShort++ = 0x1000;
        *pShort++ = 0;
        *pShort++ = 0;
        *pShort++ = 0;
        *pShort++ = 0;
        *pShort++ = -Sin;
        *pShort++ = 0;
        *pShort++ = Sin;
        *pShort = 0;
    }

    gte_SetRotMatrix(&pInfo->WorldCst);

    p0 = &pInfo->StartCoords;
    p1 = &pInfo->EndCoords;

    if (p0->vx < p1->vx)
        pInfo->MinCoords.vx = p0->vx, pInfo->MaxCoords.vx = p1->vx;
    else
        pInfo->MinCoords.vx = p1->vx, pInfo->MaxCoords.vx = p0->vx;

    if (p0->vy < p1->vy)
        pInfo->MinCoords.vy = p0->vy, pInfo->MaxCoords.vy = p1->vy;
    else
        pInfo->MinCoords.vy = p1->vy, pInfo->MaxCoords.vy = p0->vy;

    if (p0->vz < p1->vz)
        pInfo->MinCoords.vz = p0->vz, pInfo->MaxCoords.vz = p1->vz;
    else
        pInfo->MinCoords.vz = p1->vz, pInfo->MaxCoords.vz = p0->vz;

    NextInquiry();
    pInfo->Inquiry = Inquiry;
}

/**
 * Drives a prepared line collision query across an item chain.
 *
 * The line's world-constant matrix is loaded into COP2 and the item list is
 * preprocessed once for the current inquiry. Items with a stale inquiry stamp
 * are then tested individually.
 *
 * @param pItem First item in the linked list.
 * @param pInfo Prepared line query and collision result.
 */
void M3dColij_LineToItem(Item *pItem, SLineInfo *pInfo)
{
    if (pItem && pInfo->Length)
    {
        gte_SetRotMatrix(&pInfo->WorldCst);
        M3dAsm_LineColijPreprocessItems(
            pItem, 0, pInfo, pInfo->Inquiry);

        do
        {
            if (pItem->Inquiry != pInfo->Inquiry)
            {
                pItem->Inquiry = pInfo->Inquiry;
                M3dColij_LineToThisItem(pItem, pInfo);
            }
            pItem = pItem->NextItem;
        } while (pItem);
    }
}

/**
 * Resets the collision-cache cursors and clears every cached item.
 *
 */
void M3dColij_FlushCache(void)
{
    int i;

    ItemCacheIndex = 0;
    ItemCacheStart = -1;
    FaceCacheIndex = 0;
    FaceCacheStart = -1;

    for (i = 0; i < 20; ++i)
        ItemCache[i].pItem = 0;
}

/**
 * Tests a prepared line segment against the faces cached for one item.
 *
 * Broad-phase face bounds and face masks reject irrelevant polygons first.
 * Surviving faces are tested against their plane and then against each signed
 * edge.  The nearest solid hit updates the query, while trigger faces defer a
 * command-point notification until the cache scan is complete.
 *
 * @param pItem Item whose model owns the cached faces.
 * @param pInfo Prepared line query updated when a nearer face is found.
 * @param pColCache Cache span describing the item's face records.
 */
void M3dColij_LineToThisItemCached(Item *pItem, SLineInfo *pInfo, SColCache *pColCache)
{
    VECTOR PosnRelStart;
    SModel *pModel;
    int XMin;
    int YMin;
    int ZMin;
    int XMax;
    int YMax;
    int ZMax;
    SFaceCache *pFaceCache;
    int Faces;
    int NumNormals;
    SVECTOR *pVerts;
    SVECTOR *Normals;
    u32 *pFace;
    int tNear;
    int CollidedTriggerModel;

    XMin = pInfo->MinCoords.vx;
    YMin = pInfo->MinCoords.vy;
    ZMin = pInfo->MinCoords.vz;
    XMax = pInfo->MaxCoords.vx;
    YMax = pInfo->MaxCoords.vy;
    ZMax = pInfo->MaxCoords.vz;

    XMin = (XMin >> 12 << 12) - 0x4000;
    YMin = (YMin >> 12 << 12) - 0x4000;
    ZMin = (ZMin >> 12 << 12) - 0x4000;
    XMax = (XMax >> 12 << 12) + 0x4000;
    YMax = (YMax >> 12 << 12) + 0x4000;
    ZMax = (ZMax >> 12 << 12) + 0x4000;

    pFaceCache = &FaceCache[pColCache->FirstFace];
    Faces = pColCache->Faces;
    pModel = PSXRegion[pItem->Region].ppModels[pItem->Model];
    Normals = pModel->Vertices + pModel->NumVertices;
    pVerts = pModel->Vertices;
    NumNormals = pModel->NumNormals;
    pFace = (u32 *)(Normals + NumNormals);
    tNear = pInfo->tNear;
    CollidedTriggerModel = -1;

    for (int i = 0; i < Faces;
         ++i,
         ++pFaceCache,
         pFace = (u32 *)((u8 *)pFace + (((u16 *)pFace)[1] & 0xfffc)))
    {
        if (pFaceCache->YMin <= YMax &&
            pFaceCache->YMax >= YMin &&
            pFaceCache->XMin <= XMax &&
            pFaceCache->XMax >= XMin &&
            pFaceCache->ZMin <= ZMax &&
            pFaceCache->ZMax >= ZMin)
        {
            u32 faceHeader = pFace[3];

            if (!(faceHeader & M3dColij_OneMask))
            if (~(faceHeader | M3dColij_ZeroMask) == 0)
            if ((((faceHeader >> 16) & 3) ^ 1) != 0)
            {
                u16 FaceFlags;
                u8 *pVertexIndex;
                int Nx;
                int Ny;
                int Nz;
                int x0;
                int y0;
                int z0;
                int L0x;
                int L0y;
                int L0z;
                int L1x;
                int L1y;
                int L1z;
                int LineLen;
                int s0;
                int s1;
                int x1;
                int y1;
                int z1;
                int x2;
                int y2;
                int z2;
                int x3;
                int y3;
                int z3;
                long long Dx;
                long long Dy;
                long long Dz;
                int V0x;
                int V0y;
                int V0z;
                int V1x;
                int V1y;
                int V1z;
                int V2x;
                int V2y;
                int V2z;
                int V3x;
                int V3y;
                int V3z;
                long long S0;
                long long S1;
                long long S2;
                long long S3;
                int contained;

                FaceFlags = *(u16 *)pFace;
                pVertexIndex = (u8 *)pFace + 4;
                Nx = ((SVECTOR *)((u8 *)Normals +
                    (((u16 *)pFace)[6] & 0xfff8)))->vx;
                Ny = ((SVECTOR *)((u8 *)Normals +
                    (((u16 *)pFace)[6] & 0xfff8)))->vy;
                Nz = ((SVECTOR *)((u8 *)Normals +
                    (((u16 *)pFace)[6] & 0xfff8)))->vz;

                x0 = pVerts[pVertexIndex[0]].vx;
                y0 = pVerts[pVertexIndex[0]].vy;
                z0 = pVerts[pVertexIndex[0]].vz;

                L0x = (pInfo->StartCoords.vx >> 12) -
                      (pItem->Position.vx >> 12);
                L0y = (pInfo->StartCoords.vy >> 12) -
                      (pItem->Position.vy >> 12);
                L0z = (pInfo->StartCoords.vz >> 12) -
                      (pItem->Position.vz >> 12);
                L1x = (pInfo->EndCoords.vx >> 12) -
                      (pItem->Position.vx >> 12);
                L1y = (pInfo->EndCoords.vy >> 12) -
                      (pItem->Position.vy >> 12);
                L1z = (pInfo->EndCoords.vz >> 12) -
                      (pItem->Position.vz >> 12);

                LineLen = pInfo->Length;
                s0 = Nx * (L0x - x0) +
                     Ny * (L0y - y0) +
                     Nz * (L0z - z0);
                s1 = Nx * (L1x - x0) +
                     Ny * (L1y - y0) +
                     Nz * (L1z - z0);

                if (s0 >= 0 && s1 <= 0 && (s0 || s1))
                {
                    x1 = pVerts[pVertexIndex[1]].vx;
                    y1 = pVerts[pVertexIndex[1]].vy;
                    z1 = pVerts[pVertexIndex[1]].vz;
                    x2 = pVerts[pVertexIndex[2]].vx;
                    y2 = pVerts[pVertexIndex[2]].vy;
                    z2 = pVerts[pVertexIndex[2]].vz;

                    if (!(FaceFlags & 0x10))
                    {
                        x3 = pVerts[pVertexIndex[3]].vx;
                        y3 = pVerts[pVertexIndex[3]].vy;
                        z3 = pVerts[pVertexIndex[3]].vz;
                    }

                    Dx = L1x - L0x;
                    Dy = L1y - L0y;
                    Dz = L1z - L0z;

                    V0x = x0 - L0x;
                    V0y = y0 - L0y;
                    V0z = z0 - L0z;
                    V1x = x1 - L0x;
                    V1y = y1 - L0y;
                    V1z = z1 - L0z;
                    V2x = x2 - L0x;
                    V2y = y2 - L0y;
                    V2z = z2 - L0z;

                    V3x = x3 - L0x;
                    V3y = y3 - L0y;
                    V3z = z3 - L0z;

                    contained = 0;

                    if (FaceFlags & 0x10)
                    {
                        S0 =
                            Dx * (V2y * V0z - V2z * V0y) +
                            Dy * (V2z * V0x - V2x * V0z) +
                            Dz * (V2x * V0y - V2y * V0x);
                        S1 =
                            Dx * (V0y * V1z - V0z * V1y) +
                            Dy * (V0z * V1x - V0x * V1z) +
                            Dz * (V0x * V1y - V0y * V1x);
                        S2 =
                            Dx * (V1y * V2z - V1z * V2y) +
                            Dy * (V1z * V2x - V1x * V2z) +
                            Dz * (V1x * V2y - V1y * V2x);

                        if (S0 >= 0 && S1 >= 0 && S2 >= 0)
                            contained = 1;
                    }
                    else
                    {
                        S0 =
                            Dx * (V2y * V0z - V2z * V0y) +
                            Dy * (V2z * V0x - V2x * V0z) +
                            Dz * (V2x * V0y - V2y * V0x);
                        S1 =
                            Dx * (V0y * V1z - V0z * V1y) +
                            Dy * (V0z * V1x - V0x * V1z) +
                            Dz * (V0x * V1y - V0y * V1x);
                        S2 =
                            Dx * (V1y * V3z - V1z * V3y) +
                            Dy * (V1z * V3x - V1x * V3z) +
                            Dz * (V1x * V3y - V1y * V3x);
                        S3 =
                            Dx * (V3y * V2z - V3z * V2y) +
                            Dy * (V3z * V2x - V3x * V2z) +
                            Dz * (V3x * V2y - V3y * V2x);

                        if (S0 >= 0 && S1 >= 0 &&
                            S2 >= 0 && S3 >= 0)
                            contained = 1;
                    }

                    if (contained)
                    {
                        int Qx;
                        int Qy;
                        int Qz;
                        int t;
                        int xd;
                        int yd;
                        int zd;
                        int sxd;
                        int syd;
                        int szd;
                        int xd1;
                        int yd1;
                        int zd1;

                        pInfo->Model = pItem->Model;

                        if (((u16 *)pFace)[7] & 2)
                        {
                            if (pInfo->RecordTriggerZoneHits)
                                CollidedTriggerModel = pItem->Model;
                        }
                        else
                        {
                            t = M3dMaths_MulDiv64(
                                s0, 0x4000, s0 - s1);

                            if (t < tNear)
                            {
                                Qx = pInfo->EndCoords.vx -
                                     pInfo->StartCoords.vx;
                                Qy = pInfo->EndCoords.vy -
                                     pInfo->StartCoords.vy;
                                Qz = pInfo->EndCoords.vz -
                                     pInfo->StartCoords.vz;

                                sxd = 1;
                                if (Qx < 0)
                                    sxd = -1;
                                xd = (Qx ^ (Qx >> 31)) - (Qx >> 31);

                                syd = 1;
                                if (Qy < 0)
                                    syd = -1;
                                yd = (Qy ^ (Qy >> 31)) - (Qy >> 31);

                                szd = 1;
                                if (Qz < 0)
                                    szd = -1;
                                zd = (Qz ^ (Qz >> 31)) - (Qz >> 31);

                                xd1 = sxd *
                                      M3dMaths_MulDiv64(xd, t, 0x4000);
                                yd1 = syd *
                                      M3dMaths_MulDiv64(yd, t, 0x4000);
                                zd1 = szd *
                                      M3dMaths_MulDiv64(zd, t, 0x4000);

                                tNear = t;
                                pInfo->tNear = t;
                                pInfo->pItem = pItem;
                                pInfo->Position.vx =
                                    pInfo->StartCoords.vx + xd1;
                                pInfo->Position.vy =
                                    pInfo->StartCoords.vy + yd1;
                                pInfo->pFace = pFace;
                                pInfo->Position.vz =
                                    pInfo->StartCoords.vz + zd1;
                                pInfo->Model = pItem->Model;
                                pInfo->Distance =
                                    Mips_FixMul(LineLen, t) >> 2;

                                NearestHit.pFace = pFace;
                                *(SVECTOR **)&NearestHit.Normal =
                                    (SVECTOR *)((u8 *)Normals +
                                    (((u16 *)pFace)[6] & 0xfff8));
                            }
                        }
                    }
                }
            }
        }
    }

    if (CollidedTriggerModel != -1)
    {
        int i = Spool_GetEnvIndex(pItem->Region);
        OtherEnvIndex = i ^ 1;
        TriggerCollisionCheck = 0;
        Trig_TriggerCommandPoint(
            EnvModelChecksums[i][CollidedTriggerModel], true);
    }
}

/**
 * Checks whether a face span fits at the circular cache tail.
 *
 * FitFacesAtCacheEnd is the sole retained assembly body in this owner. Clean
 * C++ reproduces every instruction but not the target's first cache-tail
 * branch relocation, so the exact pre-existing body remains canonical here.
 *
 * @param faces Number of face records to append.
 * @return Nonzero when the span can be added without overlapping live data.
 */
asm(
    ".set noreorder\n"
    ".globl FitFacesAtCacheEnd__Fi\n"
    "FitFacesAtCacheEnd__Fi:\n"
    "lw $3,0x489C($28)\n"
    "nop\n"
    "slti $2,$3,0x14\n"
    "bne $2,$0,1f\n"
    "nop\n"
    "sw $0,0x489C($28)\n"
    "addu $3,$0,$0\n"
    "1:\n"
    "lw $2,0x48A0($28)\n"
    "nop\n"
    "beq $3,$2,4f\n"
    "addu $2,$0,$0\n"
    "lw $3,0x48A4($28)\n"
    "lw $5,0x48A8($28)\n"
    "nop\n"
    "slt $2,$5,$3\n"
    "beq $2,$0,2f\n"
    "addu $2,$3,$4\n"
    "slti $2,$2,0x1F5\n"
    "bne $2,$0,3f\n"
    "slt $2,$5,$3\n"
    "sw $0,0x48A4($28)\n"
    "addu $3,$0,$0\n"
    "2:\n"
    "slt $2,$5,$3\n"
    "3:\n"
    "bne $2,$0,4f\n"
    "addiu $2,$0,1\n"
    "addu $3,$3,$4\n"
    "slt $3,$5,$3\n"
    "bne $3,$0,4f\n"
    "addu $2,$0,$0\n"
    "addiu $2,$0,1\n"
    "4:\n"
    "jr $31\n"
    "nop\n"
    ".set reorder\n");

/**
 * Removes the oldest item and its face span from the circular cache.
 */
void RemoveFacesFromCache(void)
{
    ItemCache[ItemCacheStart].pItem = 0;
    ItemCacheStart++;
    if (ItemCacheStart >= 20)
        ItemCacheStart = 0;

    if (ItemCacheStart == ItemCacheIndex)
    {
        ItemCacheStart = -1;
        ItemCacheIndex = 0;
        FaceCacheStart = -1;
        FaceCacheIndex = 0;
        return;
    }

    FaceCacheStart = ItemCache[ItemCacheStart].FirstFace;
}

/**
 * Tests a null-terminated item list using the circular collision-face cache.
 *
 * Address:   0x800919A0
 * Mangled:   M3dColij_LineToItemZoned__FPP4ItemP9SLineInfo
 * Demangled: M3dColij_LineToItemZoned(Item **, SLineInfo *)
 *
 * @param ppItem Null-terminated array of environment item pointers.
 * @param pInfo Prepared line query updated with the nearest collision.
 */
void M3dColij_LineToItemZoned(Item **ppItem, SLineInfo *pInfo)
{
    if (!ppItem)
        return;

    if (pInfo->Length == 0)
        return;

    gte_SetRotMatrix(&pInfo->WorldCst);
    M3dAsm_LineColijPreprocessItemsZoned(
        ppItem, 0, pInfo, pInfo->Inquiry);

    for (; *ppItem; ++ppItem)
    {
        if ((*ppItem)->Inquiry != pInfo->Inquiry)
        {
            Item *pItem;
            int i;

            (*ppItem)->Inquiry = pInfo->Inquiry;
            pItem = *ppItem;
            for (i = 0; i < 20; ++i)
            {
                if (ItemCache[i].pItem == pItem)
                    break;
            }

            if (i != 20)
            {
                M3dColij_LineToThisItemCached(
                    pItem, pInfo, &ItemCache[i]);
            }
            else
            {
                SModel *pModel;
                int Faces;
                int ItemX;
                int ItemY;
                int ItemZ;
                int NumFaces;
                int NumNormals;
                SVECTOR *Normals;
                SVECTOR *pVertices;
                u32 *pFaceX;
                SFaceCache *pFaceCache;

                pModel = PSXRegion[pItem->Region].ppModels[pItem->Model];
                Faces = pModel->NumFaces;

                while (!FitFacesAtCacheEnd(Faces))
                    RemoveFacesFromCache();

                ItemCache[ItemCacheIndex].pItem = pItem;
                ItemCache[ItemCacheIndex].pModel = pModel;
                ItemCache[ItemCacheIndex].Faces = Faces;
                ItemCache[ItemCacheIndex].FirstFace = FaceCacheIndex;

                if (ItemCacheStart == -1)
                    ItemCacheStart = ItemCacheIndex;
                if (FaceCacheStart == -1)
                    FaceCacheStart = FaceCacheIndex;

                {
                    int i;

                    pFaceCache = &FaceCache[FaceCacheIndex];
                    FaceCacheIndex += Faces;

                    ItemX = pItem->Position.vx;
                    ItemY = pItem->Position.vy;
                    ItemZ = pItem->Position.vz;

                    NumFaces = pModel->NumFaces;
                    pVertices = pModel->Vertices;
                    Normals = pModel->Vertices + pModel->NumVertices;
                    NumNormals = pModel->NumNormals;
                    pFaceX = (u32 *)(Normals + NumNormals);

                    for (i = 0; i < NumFaces; ++i)
                    {
                        u8 *pVertexIndex = (u8 *)pFaceX + 4;
                        pFaceCache->Flags = *(u16 *)pFaceX;

                        int z0 = pVertices[pVertexIndex[0]].vz;
                        int xmin = pVertices[pVertexIndex[0]].vx;
                        int ymin = pVertices[pVertexIndex[0]].vy;
                        int zmin = z0;
                        int xmax = xmin;
                        int ymax = ymin;
                        int zmax = zmin;

                        int x1 = pVertices[pVertexIndex[1]].vx;
                        int y1 = pVertices[pVertexIndex[1]].vy;
                        int z1 = pVertices[pVertexIndex[1]].vz;

                        int x2 = pVertices[pVertexIndex[2]].vx;
                        int y2 = pVertices[pVertexIndex[2]].vy;
                        int z2 = pVertices[pVertexIndex[2]].vz;

                        if (x1 > xmax) xmax = x1;
                        if (x2 > xmax) xmax = x2;
                        if (y1 > ymax) ymax = y1;
                        if (y2 > ymax) ymax = y2;
                        if (z1 > zmax) zmax = z1;
                        if (z2 > zmax) zmax = z2;

                        if (x1 < xmin) xmin = x1;
                        if (x2 < xmin) xmin = x2;
                        if (y1 < ymin) ymin = y1;
                        if (y2 < ymin) ymin = y2;
                        if (z1 < zmin) zmin = z1;
                        if (z2 < zmin) zmin = z2;

                        int x3 = pVertices[pVertexIndex[3]].vx;
                        int y3 = pVertices[pVertexIndex[3]].vy;
                        int z3 = pVertices[pVertexIndex[3]].vz;

                        if (!(*(u16 *)pFaceX & 0x10))
                        {
                            if (x3 > xmax) xmax = x3;
                            if (y3 > ymax) ymax = y3;
                            if (z3 > zmax) zmax = z3;
                            if (x3 < xmin) xmin = x3;
                            if (y3 < ymin) ymin = y3;
                            if (z3 < zmin) zmin = z3;
                        }

                        xmin <<= 12;
                        ymin <<= 12;
                        zmin <<= 12;
                        xmax <<= 12;
                        ymax <<= 12;
                        zmax <<= 12;

                        xmin += ItemX;
                        ymin += ItemY;
                        zmin += ItemZ;
                        xmax += ItemX;
                        ymax += ItemY;
                        zmax += ItemZ;

                        pFaceCache->XMin = xmin;
                        pFaceCache->YMin = ymin;
                        pFaceCache->ZMin = zmin;
                        pFaceCache->XMax = xmax;
                        pFaceCache->YMax = ymax;
                        pFaceCache->ZMax = zmax;

                        ++pFaceCache;
                        pFaceX = (u32 *)((u8 *)pFaceX +
                                         (((u16 *)pFaceX)[1] & 0xfffc));
                    }
                }

                M3dColij_LineToThisItemCached(
                    pItem, pInfo, &ItemCache[ItemCacheIndex]);

                ++ItemCacheIndex;
                if (ItemCacheIndex == 20)
                    ItemCacheIndex = 0;
            }
        }
    }
}

/**
 * M3dColij_GetLineInfo__FP9SLineInfo
 *
 * Address:   0x80091E58
 * Mangled:   M3dColij_GetLineInfo__FP9SLineInfo
 * Demangled: M3dColij_GetLineInfo(SLineInfo *)
 *
 * @param pInfo Completed line query whose normal is transformed in place.
 * @return One if the query hit an item; otherwise zero.
 */
int M3dColij_GetLineInfo(SLineInfo *pInfo)
{
    if (pInfo->pItem)
    {
        M3dMaths_RotMatrixYXZ(
            M3dColij_GteAngles(&pInfo->pItem->Angles), &Tfm);
        gte_SetRotMatrix(&Tfm);
        // The line path stores a model-normal pointer in Normal's first word.
        gte_ldv0(*(SVECTOR **)&NearestHit.Normal);
        gte_rtv0();
        gte_stsv(&pInfo->Normal);
        return 1;
    }

    return 0;
}

/**
 * Tests one environment item against a prepared collision segment.
 *
 * @param pItem Environment item and model instance to test.
 * @param pInfo Prepared line query updated when the model has a nearer hit.
 */
void M3dColij_LineToThisItem(Item *pItem, SLineInfo *pInfo)
{
    VECTOR PosnRelStart;
    u16 And;
    SModel *pModel;
    SProjectedVertex *pVertices;

    gte_SetColorMatrix(&pInfo->WorldCst);

    pModel = PSXRegion[pItem->Region].ppModels[pItem->Model];

    if (pModel->NumVertices <= 128)
        pVertices = (SProjectedVertex *)0x1f800000;
    else
        pVertices = ProjectedVertices;

    if (pItem->Angles.vx == 0 &&
        pItem->Angles.vy == 0 &&
        pItem->Angles.vz == 0 &&
        !(pItem->Flags & 0x200))
    {
        PosnRelStart.vx =
            pItem->Position.vx - pInfo->StartCoords.vx >> 12;
        PosnRelStart.vy =
            pItem->Position.vy - pInfo->StartCoords.vy >> 12;
        PosnRelStart.vz =
            pItem->Position.vz - pInfo->StartCoords.vz >> 12;

        gte_ZeroTransVector();
    }
    else
    {
        if (pInfo->DropDown)
        {
            Tfm.t[0] =
                pItem->Position.vx - pInfo->StartCoords.vx >> 12;
            Tfm.t[1] =
                pInfo->StartCoords.vz - pItem->Position.vz >> 12;
            Tfm.t[2] =
                pItem->Position.vy - pInfo->StartCoords.vy >> 12;
        }
        else
        {
            Tfm.t[0] =
                pItem->Position.vx - pInfo->StartCoords.vx >> 12;
            Tfm.t[1] =
                pItem->Position.vy - pInfo->StartCoords.vy >> 12;
            Tfm.t[2] =
                pItem->Position.vz - pInfo->StartCoords.vz >> 12;

            gte_ldlv0(&Tfm.t[0]);
            gte_lcv0();
            gte_stlvnl(&Tfm.t[0]);
        }

        gte_SetTransMatrix(&Tfm);

        PosnRelStart.vx = 0;
        PosnRelStart.vy = 0;
        PosnRelStart.vz = 0;

        if (pItem->Angles.vx || pItem->Angles.vy || pItem->Angles.vz)
        {
            M3dMaths_RotMatrixYXZ(
                M3dColij_GteAngles(&pItem->Angles), &Tfm);
            if (pItem->Flags & 0x200)
                M3dMaths_ScaleMatrix(pItem, &Tfm);
            M3dMaths_MulLcmMatrix(&Tfm, &Tfm);
            gte_SetColorMatrix(&Tfm);
        }
        else
        {
            M3dMaths_SetIdentityRotation(&Tfm);
            M3dMaths_ScaleMatrix(pItem, &Tfm);
            M3dMaths_MulLcmMatrix(&Tfm, &Tfm);
            gte_SetColorMatrix(&Tfm);
        }
    }

    And = M3dAsm_LineColijProcessVertices(
        pModel, pVertices, pInfo->Length, &PosnRelStart);

    if (And & 0x060f)
        return;

    CollidedTriggerModel = -1;
    M3dAsm_LineColijProcessFaces(
        pModel, pVertices, pInfo, pItem);

    if (CollidedTriggerModel != -1)
    {
        int i = Spool_GetEnvIndex(pItem->Region);
        OtherEnvIndex = i ^ 1;
        TriggerCollisionCheck = 0;
        Trig_TriggerCommandPoint(
            EnvModelChecksums[i][CollidedTriggerModel], true);
    }
}

/**
 * Tests a fixed-point line segment against a linked list of body spheres.
 *
 * Address:   0x800922B4
 * Mangled:   M3dColij_LineToSphere__FRC7CVectorT0R7CVectorP5CBodyT3i
 * Demangled: M3dColij_LineToSphere(const CVector &, const CVector &, CVector &, CBody *, CBody *, int)
 *
 * @param OldPos Segment start in 20.12 world coordinates.
 * @param NewPos Segment end in 20.12 world coordinates.
 * @param ColijPos Receives the closest collision point.
 * @param pBody First body in the linked collision list.
 * @param pIgnore Body excluded from the query, if any.
 * @param ScaleFactor Fixed-point scale applied to each body's minor radius.
 * @return The closest intersected body, or null when there is no hit.
 */
CBody *M3dColij_LineToSphere(const CVector &OldPos, const CVector &NewPos, CVector &ColijPos, CBody *pBody, CBody *pIgnore, int ScaleFactor)
{
    int Radius;
    int RadiusSquared;
    int Length;
    int DotProd;
    CVector DirectionVec;
    CVector MinCoords;
    CVector MaxCoords;
    CVector SquareVec;
    VECTOR OldToCentre;
    VECTOR NewToCentre;
    SVECTOR DirVecShort;

    NearestHit.Dist = 0x7fffffff;
    pBodyHit = 0;

    DirectionVec = (NewPos - OldPos) >> 12;
    gte_ldlvl(&DirectionVec);
    gte_sqr0();
    gte_stlvnl(&SquareVec);
    Length = M3dMaths_SquareRoot0(
        SquareVec.vx + SquareVec.vy + SquareVec.vz);

    if (Length == 0)
        return 0;

    DirectionVec = (DirectionVec << 12) / Length;
    gte_ldopv1(&DirectionVec);

    int _x = DirectionVec.vx;
    int _y = DirectionVec.vy;
    int _z = DirectionVec.vz;
    DirVecShort.vx = _x;
    DirVecShort.vy = _y;
    DirVecShort.vz = _z;
    gte_ldsvllrow0(&DirVecShort);

    if (OldPos.vx < NewPos.vx)
    {
        MinCoords.vx = OldPos.vx;
        MaxCoords.vx = NewPos.vx;
    }
    else
    {
        MinCoords.vx = NewPos.vx;
        MaxCoords.vx = OldPos.vx;
    }

    if (OldPos.vy < NewPos.vy)
    {
        MinCoords.vy = OldPos.vy;
        MaxCoords.vy = NewPos.vy;
    }
    else
    {
        MinCoords.vy = NewPos.vy;
        MaxCoords.vy = OldPos.vy;
    }

    if (OldPos.vz < NewPos.vz)
    {
        MinCoords.vz = OldPos.vz;
        MaxCoords.vz = NewPos.vz;
    }
    else
    {
        MinCoords.vz = NewPos.vz;
        MaxCoords.vz = OldPos.vz;
    }

    while (pBody)
    {
        if (!(pBody->mCBodyFlags & 0x40) && pBody != pIgnore)
        {
            Radius = pBody->mRMinor * ScaleFactor;

            if (MinCoords.vx <= pBody->CItem.mPos.vx + Radius &&
                pBody->CItem.mPos.vx - Radius <= MaxCoords.vx &&
                MinCoords.vy <= pBody->CItem.mPos.vy + Radius &&
                pBody->CItem.mPos.vy - Radius <= MaxCoords.vy &&
                MinCoords.vz <= pBody->CItem.mPos.vz + Radius &&
                pBody->CItem.mPos.vz - Radius <= MaxCoords.vz)
            {
                OldToCentre.vx =
                    (pBody->CItem.mPos.vx - OldPos.vx) >> 12;
                OldToCentre.vy =
                    (pBody->CItem.mPos.vy - OldPos.vy) >> 12;
                OldToCentre.vz =
                    (pBody->CItem.mPos.vz - OldPos.vz) >> 12;

                gte_ldopv2(&OldToCentre);
                gte_op12();
                gte_sqr0();
                gte_stlvnl(&SquareVec);

                RadiusSquared = (Radius >> 12) * (Radius >> 12);
                if (SquareVec.vx + SquareVec.vy + SquareVec.vz <
                    RadiusSquared)
                {
                    gte_ldlv0(&OldToCentre);
                    gte_llv0();
                    gte_stlvnl0(&DotProd);

                    if (DotProd < NearestHit.Dist)
                    {
                        if (DotProd < 0)
                        {
                            gte_ldlvl(&OldToCentre);
                            gte_sqr0();
                            gte_stlvnl(&SquareVec);

                            if (RadiusSquared <=
                                SquareVec.vx + SquareVec.vy + SquareVec.vz)
                                goto next_body;
                        }
                        else if (Length < DotProd)
                        {
                            NewToCentre.vx =
                                (pBody->CItem.mPos.vx - NewPos.vx) >> 12;
                            NewToCentre.vy =
                                (pBody->CItem.mPos.vy - NewPos.vy) >> 12;
                            NewToCentre.vz =
                                (pBody->CItem.mPos.vz - NewPos.vz) >> 12;

                            gte_ldlvl(&NewToCentre);
                            gte_sqr0();
                            gte_stlvnl(&SquareVec);

                            if (RadiusSquared <=
                                SquareVec.vx + SquareVec.vy + SquareVec.vz)
                                goto next_body;
                        }

                        NearestHit.Dist = DotProd;
                        pBodyHit = pBody;
                    }
                }
            }
        }

next_body:
        pBody = pBody->CItem.mNextItem;
    }

    if (pBodyHit)
    {
        gte_ldlvl(&DirectionVec);
        gte_lddp(NearestHit.Dist);
        gte_gpf12();
        gte_stlvnl(&ColijPos);
        ColijPos <<= 12;
        ColijPos += OldPos;
    }

    return pBodyHit;
}

/**
 * `_GLOBAL_.I.NearestHit` is generated by the owner above. The original
 * default-construction path is empty after static storage is zeroed.
 */
