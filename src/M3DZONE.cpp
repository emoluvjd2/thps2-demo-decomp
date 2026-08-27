// Environment collision-zone setup and grid traversal.

#include "M3DZONE.h"
#include "globals.h"
#include "PSXREGION.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#define M3DCOLIJ_LINE_INFO_LAYOUT
#include "M3DCOLIJ.h"
#undef M3DCOLIJ_LINE_INFO_LAYOUT

enum M3dZoneConstants
{
    M3dZoneGridDimension = 20,
    M3dZonePackedItemWords = 9,
    M3dZonePackedItemPositionWords = 3
};

/** Spatial collision grid for one environment. */
struct SZone
{
    unsigned int Flags;
    int xMin;
    int zMin;
    int xMax;
    int zMax;
    int ZoneWidth;
    int ZoneHeight;
    short Width;
    short Height;
    Item **Ptr[M3dZoneGridDimension][M3dZoneGridDimension];
};

typedef char M3dZone_zone_size_must_be_1632[
    (sizeof(SZone) == 1632) ? 1 : -1];

extern "C"
{
    extern SZone Zones[1];

    extern unsigned int M3dColij_OneMask __attribute__((section(".data")));
    extern unsigned int M3dColij_ZeroMask __attribute__((section(".data")));
    #define BODYLISTS_DECLARE_ENVIRONMENTAL_OBJECT_LIST
    #define BODYLISTS_WITHIN_C_LINKAGE
    #include "BODYLISTS.h"
    extern int EnvRegions[2] __attribute__((section(".data")));

    unsigned int SetSp(unsigned int newSp);
}

#include "M3DMATHS.h"
void M3dColij_LineToItem(Item *itemList, SLineInfo *lineInfo);
void M3dColij_LineToItemZoned(Item **itemList, SLineInfo *lineInfo);
int M3dColij_GetLineInfo(SLineInfo *lineInfo);

/** Primary collision zone, populated when an environment is loaded. */
SZone Zones[1] __attribute__((section(".data"))) = {{0}};

/**
 * Clear one zone's active flag and spatial collision grid.
 *
 * @param EnvIndex Environment slot to clear; the prototype owns slot zero.
 */
void M3dZone_FreePSX(int EnvIndex)
{
    if (EnvIndex > 0)
    {
        return;
    }

    Zones[EnvIndex].Flags = 0;
    for (int i = 0; i < M3dZoneGridDimension; i++)
    {
        for (int j = 0; j < M3dZoneGridDimension; j++)
        {
            Zones[EnvIndex].Ptr[i][j] = 0;
        }
    }
}

/** Initialize the zone system by clearing the primary zone. */
void M3dZone_Init(void)
{
    M3dZone_FreePSX(0);
}

/**
 * Run a collision query while the collision scratch data is mapped as the
 * active stack workspace.
 *
 * @param pInfo Line endpoints and collision-result state.
 * @param CheckEnviroObs Nonzero to test the environmental object list first.
 * @return Value produced while restoring the previous stack pointer.
 */
int M3dZone_LineToItem(SLineInfo *pInfo, int CheckEnviroObs)
{
    unsigned int sp = SetSp(0x1F8003F0);
    M3dZone_LineToItemNotSCached(pInfo, CheckEnviroObs);
    return SetSp(sp);
}

/**
 * Ray-cast collision test across zone grid cells.
 * Applies the active collision masks, optionally checks environmental objects,
 * clips the line to each active zone, and walks every intersected grid cell.
 *
 * @param pInfo Line endpoints and collision-result state.
 * @param CheckEnviroObs Nonzero to test the environmental object list first.
 * @return The current collision result produced from pInfo.
 */
int M3dZone_LineToItemNotSCached(SLineInfo *pInfo, int CheckEnviroObs)
{
    int x0;
    int x1;
    int z0;
    int z1;
    int xL;
    int xH;
    int zL;
    int zH;
    int X0;
    int X1;
    int Z0;
    int Z1;
    int r0;
    int s0;
    int X;
    int Z;
    int dX;
    int dZ;
    int w;
    int h;
    int l;
    int d;
    int qh;
    int qw;

    M3dColij_OneMask = 0;
    M3dColij_ZeroMask = ~0u;

    if (LineOfSightCheck)
    {
        M3dColij_OneMask = 0x00400000;
    }
    if (BulletCollisionCheck)
    {
        M3dColij_OneMask ^= 0x00400000;
    }
    if (!BaddyCollisionCheck)
    {
        M3dColij_OneMask ^= 0x00200000;
    }
    if (CameraCollisionCheck)
    {
        M3dColij_ZeroMask = 0xFFEFFFFF;
    }
    if (TriggerCollisionCheck)
    {
        M3dColij_ZeroMask ^= 0x00020000;
    }
    if (CheckEnviroObs)
    {
        M3dColij_LineToItem(
            reinterpret_cast<Item *>(EnvironmentalObjectList), pInfo);
    }

    {
        int EnvIndex;

        for (EnvIndex = 0; EnvIndex <= 0; EnvIndex++)
        {
            if (!Zones[EnvIndex].Flags)
            {
                continue;
            }

            x0 = pInfo->StartCoords.vx;
            x1 = pInfo->EndCoords.vx;
            z0 = pInfo->StartCoords.vz;
            z1 = pInfo->EndCoords.vz;

            xL = Zones[EnvIndex].xMin;
            xH = Zones[EnvIndex].xMax;
            zL = Zones[EnvIndex].zMin;
            zH = Zones[EnvIndex].zMax;
            l = Zones[EnvIndex].ZoneWidth;

            if (((x0 >= xL) || (x1 >= xL)) &&
                ((x0 <= xH) || (x1 <= xH)) &&
                ((z0 >= zL) || (z1 >= zL)) &&
                ((z0 <= zH) || (z1 <= zH)))
            {
                if ((x0 == x1) && (z0 == z1))
                {
                    X = (x0 - xL) / l;
                    Z = (z0 - zL) / l;
                    if (X == Zones[EnvIndex].Width)
                    {
                        X--;
                    }
                    if (Z == Zones[EnvIndex].Height)
                    {
                        Z--;
                    }
                    M3dColij_LineToItemZoned(Zones[EnvIndex].Ptr[X][Z], pInfo);
                }
                else
                {
                    if (x0 < xL)
                    {
                        z0 = (z0 < z1)
                                 ? z0 + M3dMaths_MulDiv64(xL - x0, z1 - z0, x1 - x0)
                                 : z1 + M3dMaths_MulDiv64(x1 - xL, z0 - z1, x1 - x0);
                        x0 = xL;
                    }
                    if (x1 < xL)
                    {
                        z1 = (z1 < z0)
                                 ? z1 + M3dMaths_MulDiv64(xL - x1, z0 - z1, x0 - x1)
                                 : z0 + M3dMaths_MulDiv64(x0 - xL, z1 - z0, x0 - x1);
                        x1 = xL;
                    }
                    if (x0 > xH)
                    {
                        z0 = (z1 < z0)
                                 ? z1 + M3dMaths_MulDiv64(xH - x1, z0 - z1, x0 - x1)
                                 : z0 + M3dMaths_MulDiv64(x0 - xH, z1 - z0, x0 - x1);
                        x0 = xH;
                    }
                    if (x1 > xH)
                    {
                        z1 = (z0 < z1)
                                 ? z0 + M3dMaths_MulDiv64(xH - x0, z1 - z0, x1 - x0)
                                 : z1 + M3dMaths_MulDiv64(x1 - xH, z0 - z1, x1 - x0);
                        x1 = xH;
                    }
                    if (z0 < zL)
                    {
                        x0 = (x0 < x1)
                                 ? x0 + M3dMaths_MulDiv64(zL - z0, x1 - x0, z1 - z0)
                                 : x1 + M3dMaths_MulDiv64(z1 - zL, x0 - x1, z1 - z0);
                        z0 = zL;
                    }
                    if (z1 < zL)
                    {
                        x1 = (x1 < x0)
                                 ? x1 + M3dMaths_MulDiv64(zL - z1, x0 - x1, z0 - z1)
                                 : x0 + M3dMaths_MulDiv64(z0 - zL, x1 - x0, z0 - z1);
                        z1 = zL;
                    }
                    if (z0 > zH)
                    {
                        x0 = (x1 < x0)
                                 ? x1 + M3dMaths_MulDiv64(zH - z1, x0 - x1, z0 - z1)
                                 : x0 + M3dMaths_MulDiv64(z0 - zH, x1 - x0, z0 - z1);
                        z0 = zH;
                    }
                    if (z1 > zH)
                    {
                        x1 = (x0 < x1)
                                 ? x0 + M3dMaths_MulDiv64(zH - z0, x1 - x0, z1 - z0)
                                 : x1 + M3dMaths_MulDiv64(z1 - zH, x0 - x1, z1 - z0);
                        z1 = zH;
                    }

                    w = x1 - x0;
                    if (w < 0)
                    {
                        dX = -1;
                        w = -w;
                    }
                    else
                    {
                        dX = 1;
                    }
                    h = z1 - z0;
                    if (h < 0)
                    {
                        dZ = -1;
                        h = -h;
                    }
                    else
                    {
                        dZ = 1;
                    }

                    X0 = (x0 - xL) / l;
                    X1 = (x1 - xL) / l;
                    r0 = (x0 - xL) % l;
                    Z0 = (z0 - zL) / l;
                    Z1 = (z1 - zL) / l;
                    s0 = (z0 - zL) % l;

                    if (X0 == Zones[EnvIndex].Width)
                    {
                        X0--;
                        r0 += l;
                    }
                    if (Z0 == Zones[EnvIndex].Height)
                    {
                        Z0--;
                        s0 += l;
                    }
                    if (X1 == Zones[EnvIndex].Width)
                    {
                        X1--;
                    }
                    if (Z1 == Zones[EnvIndex].Height)
                    {
                        Z1--;
                    }

                    qw = M3dMaths_MulDiv64(w, s0, l);
                    qh = M3dMaths_MulDiv64(h, r0, l);

                    X = X0;
                    Z = Z0;
                    d = ((x0 < x1) ? qh - h : -qh) +
                        ((z0 < z1) ? w - qw : qw);
                    while (((X != X1) || (Z != Z1)) &&
                           (X >= 0) && (X < Zones[EnvIndex].Width) &&
                           (Z >= 0) && (Z < Zones[EnvIndex].Height))
                    {
                        M3dColij_LineToItemZoned(
                            Zones[EnvIndex].Ptr[X][Z], pInfo);
                        if (d >= 0)
                        {
                            d -= h;
                            X += dX;
                        }
                        else
                        {
                            d += w;
                            Z += dZ;
                        }
                    }

                    if ((X < 0) || (X >= Zones[EnvIndex].Width) ||
                        (Z < 0) || (Z >= Zones[EnvIndex].Height))
                    {
                        goto next_zone;
                    }
                    M3dColij_LineToItemZoned(Zones[EnvIndex].Ptr[X][Z], pInfo);
                }
            }

        next_zone:;
        }
    }

    return M3dColij_GetLineInfo(pInfo);
}

/**
 * Unpacks zone data from pPack into Zones[EnvIndex].
 * Pack format: xmin, zmin, xmax, zmax, gridXCount(u16), gridZCount(u16),
 * then per grid cell: [8 skip bytes] [linkCount] [item indices...] [4 skip bytes]
 * Item indices are resolved in-place to pointers: base + index * 36 + 12.
 *
 * @param EnvIndex Environment slot receiving the packed grid.
 * @param pPack Packed zone bounds, dimensions, and cell item lists.
 */

void M3dZone_SetZone(int EnvIndex, unsigned int *pPack)
{
    Zones[EnvIndex].xMin = *pPack++;
    Zones[EnvIndex].zMin = *pPack++;
    Zones[EnvIndex].xMax = *pPack++;
    Zones[EnvIndex].zMax = *pPack++;
    // GCC 2.91's cast-as-lvalue extension keeps the target's single packet
    // cursor. A typed halfword cursor and a typed union each changed 114 of
    // 119 instructions; an inline reference view changed 113 plus one reloc.
    Zones[EnvIndex].Width = *((unsigned short *)pPack)++;
    Zones[EnvIndex].Height = *((unsigned short *)pPack)++;
    Zones[EnvIndex].Flags = 1;
    Zones[EnvIndex].ZoneWidth =
        (Zones[EnvIndex].xMax - Zones[EnvIndex].xMin) /
        Zones[EnvIndex].Width;

    int Z;
    for (Z = 0; Z < Zones[EnvIndex].Height; Z++)
    {
        int X;
        for (X = 0; X < Zones[EnvIndex].Width; X++)
        {
            int count;
            unsigned int *pPSX;

            pPack += 2;
            count = *pPack++;
            Zones[EnvIndex].Ptr[X][Z] = reinterpret_cast<Item **>(pPack);
            pPSX = PSXRegion[EnvRegions[EnvIndex]].pPSX;
            while (count-- > 0)
            {
                *pPack = reinterpret_cast<unsigned int>(
                    pPSX + (*pPack * M3dZonePackedItemWords +
                            M3dZonePackedItemPositionWords));
                pPack++;
            }
            pPack++;
        }
    }
}
