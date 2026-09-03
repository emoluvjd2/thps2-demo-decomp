// PACK.cpp -- Reconstructed translation unit
// Original: PACK.CPP
// Functions: 14 reconstructed entries
// Address range: 0x8004A288 - 0x8004B54C
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#define PACK_OWNER_DECLARATIONS
#define PACK_TSPACK_LAYOUT
#include "PACK.h"
#undef PACK_TSPACK_LAYOUT
#undef PACK_OWNER_DECLARATIONS
#include "MEM.h"

// === Owner-local gp-relative globals ===
TSPack *Pack_pFree = 0;
TSPack *Pack_pFull = 0;
int TotSlots = 0;
int Pack_NoBottom = 0;
int Pack_NoRight = 0;
int Pack_Width = 0;
EPackInitType LastPackInitType = PACK512x240;
int TotalPackUsage = 0;
int ZapCalls = 0;

// === Owner-local function declarations ===
int Pack_Merge(TSPack *pMerge);

// === Function implementations (address order) ===

/**
 * Allocate one pack-list entry and account for its heap block.
 *
 * Address: 0x8004A288
 * Mangled: Pack_GetSlot__Fv
 *
 * @return A new, uninitialized pack-list entry.
 */
TSPack *Pack_GetSlot(void)
{
    TSPack *ptr = static_cast<TSPack *>(Mem_New(sizeof(TSPack), 0, 1));
    TotalPackUsage += sizeof(TSPack) + 8;
    TotSlots += 1;
    return ptr;
}

/**
 * Release one pack-list entry and update the allocator statistics.
 *
 * Address: 0x8004A2C4
 * Mangled: Pack_KillSlot__FP6TSPack
 *
 * @param pToFree Entry to release.
 */
void Pack_KillSlot(TSPack *pToFree)
{
    Mem_Delete(pToFree);
    TotalPackUsage -= sizeof(TSPack) + 8;
    TotSlots -= 1;
}

/**
 *
 * Address: 0x8004A2F8
 * Mangled: Pack_New__FPP6TSPack
 *
 * @param ppHead Address of the list head that receives the new entry.
 * @return The newly prepended entry.
 */
TSPack *Pack_New(TSPack **ppHead)
{
    TSPack *pNew = Pack_GetSlot();
    pNew->pNext = *ppHead;
    *ppHead = pNew;
    return pNew;
}

/**
 * Add an initialized rectangle to the free-pack list.
 *
 * Address: 0x8004A32C
 * Mangled: Pack_NewFree__Fiiii
 *
 * @param X  X coordinate.
 * @param Y  Y coordinate.
 * @param W  Rectangle width.
 * @param H  Rectangle height.
 * @return The new free-list entry.
 */
TSPack *Pack_NewFree(int X, int Y, int W, int H)
{
    TSPack *pNew = Pack_New(&Pack_pFree);
    pNew->X = X;
    pNew->Y = Y;
    pNew->W = W;
    pNew->H = H;
    return pNew;
}

/**
 * Add an initialized rectangle to the occupied-pack list.
 *
 * Address: 0x8004A38C
 * Mangled: Pack_NewFull__Fiiii
 *
 * @param X  X coordinate.
 * @param Y  Y coordinate.
 * @param W  Rectangle width.
 * @param H  Rectangle height.
 * @return The new occupied-list entry.
 */
TSPack *Pack_NewFull(int X, int Y, int W, int H)
{
    TSPack *pNew = Pack_New(&Pack_pFull);
    pNew->X = X;
    pNew->Y = Y;
    pNew->W = W;
    pNew->H = H;
    return pNew;
}

/**
 * Unlink and release an entry from a pack list.
 *
 * Address: 0x8004A3EC
 * Mangled: Pack_Remove__FPP6TSPackP6TSPack
 *
 * @param pHead Address of the list head.
 * @param pRemove Entry to unlink and release.
 */
void Pack_Remove(TSPack **pHead, TSPack *pRemove)
{
    if (*pHead == pRemove)
    {
        *pHead = pRemove->pNext;
    }
    else
    {
        TSPack *pSkip = *pHead;
        while (pSkip->pNext != pRemove)
        {
            pSkip = pSkip->pNext;
        }
        pSkip->pNext = pRemove->pNext;
    }
    Pack_KillSlot(pRemove);
}

/**
 * Initialize the standard 512-pixel-wide VRAM packing regions.
 *
 * Address: 0x8004A45C
 * Mangled: Pack_Init__Fv
 */
void Pack_Init(void)
{
    TotSlots = 0;
    Pack_pFree = 0;
    Pack_pFull = 0;
    Pack_Width = 0x200;
    Pack_NewFree(0x200, 0x100, 0x200, 0x100);
    Pack_NewFree(0x200, 0, 0x200, 0x100);
    LastPackInitType = PACK512x240;
}

/**
 * Initialize the 640-pixel-wide VRAM packing regions.
 *
 * Address: 0x8004A4B0
 * Mangled: Pack_Init640x480__Fv
 *
 */
void Pack_Init640x480(void)
{
    TotSlots = 0;
    Pack_Width = 0x280;
    Pack_pFree = 0;
    Pack_pFull = 0;
    Pack_NewFree(0x280, 0x100, 0x180, 0x100);
    Pack_NewFree(0x280, 0, 0x180, 0x100);
    LastPackInitType = PACK640x480;
}

/**
 *
 * Address: 0x8004A508
 *          VRAM texture packer: searches free blocks for best fit, allocates, splits remainders.
 *          Type=type, WW=width, H=height, X=outX, Y=outY,
 *          BPP=bpp, Aligned=aligned, checksum=asset-hash
 * Mangled: Pack_Pack__FiiiPiT3iiUi
 *
 * @param Type  Allocation type stored in the resulting pack record.
 * @param WW  Requested texture width in pixels.
 * @param H  Requested texture height.
 * @param X  Receives the allocated VRAM X coordinate.
 * @param Y  Receives the allocated VRAM Y coordinate.
 * @param BPP  Texture bit depth used to convert pixels to VRAM words.
 * @param Aligned  Alignment mode; positive values enable recursive page packing.
 * @param checksum  Asset hash for content lookup.
 * @return The allocated pack record, or null when no suitable region exists.
 */
TSPack *Pack_Pack(int Type, int WW, int H, int *X, int *Y, int BPP, int Aligned, unsigned int checksum)
{
    TSPack *pBest; // $s2
    TSPack *pPack;  // $s1
    TSPack *pNew; // $s7
    int OX, OY, OW, OH;
    int Over;
    int W;        // $s3

    W = WW;

    // --- Aligned allocation attempt ---
    if (Aligned > 0)
    {
        if (W == 0x100 && H == W)
        {
            Aligned = 0;
        }
        if (Aligned > 0)
        {
            int NewX;
            int NewY;
            int OldX;
            int OldY;
            int OldW;
            int OldH;

            pPack = Pack_Pack(Type, WW, H, X, Y, BPP, -1, checksum);
            if (pPack)
            {
                W = WW;
                goto aligned_done;
            }

            W = WW;

            if ((WW == 0x100) || (H == 0x100))
            {
                pPack = Pack_Pack(
                    Type, 0x100, 0x100, &NewX, &NewY, BPP, 0, checksum);
                if (!pPack)
                    return 0;

                OldW = 0x80;
                OldH = 0x100;
                if (BPP != 0x10)
                    W = (W + 1) / 2;
                if (BPP == 4)
                {
                    W = (W + 1) / 2;
                    OldW = 0x40;
                }

                OldX = pPack->X;
                OldY = pPack->Y;
            }
            else
            {
                pPack = Pack_Pack(
                    Type, WW << 1, H << 1, &NewX, &NewY, BPP, 0, checksum);
                if (!pPack)
                    return 0;

                if (BPP != 0x10)
                    W = (W + 1) / 2;
                if (BPP == 4)
                    W = (W + 1) / 2;

                OldW = W << 1;
                OldH = H * 2;
                OldX = pPack->X;
                OldY = pPack->Y;
                NewX = (NewX + W - 1) & -W;
                NewY = (NewY + H - 1) & -H;
            }

            pPack->W = W;
            pPack->H = H;
            pPack->X = NewX;
            pPack->Y = NewY;
            *X = NewX;
            *Y = NewY;

            if (OldX < NewX)
            {
                TSPack *slot = Pack_NewFree(OldX, OldY, NewX - OldX, OldH);
                Pack_Merge(slot);
                OldW -= NewX - OldX;
            }
            if (W < OldW)
            {
                TSPack *slot = Pack_NewFree(NewX + W, OldY, OldW - W, OldH);
                Pack_Merge(slot);
            }
            if (OldY < NewY)
            {
                TSPack *slot = Pack_NewFree(NewX, OldY, W, NewY - OldY);
                Pack_Merge(slot);
                OldH -= NewY - OldY;
            }
            if (H < OldH)
            {
                TSPack *slot = Pack_NewFree(NewX, NewY + H, W, OldH - H);
                Pack_Merge(slot);
            }
        aligned_done:
            pPack->bpp = BPP;
            pPack->aligned = Aligned;
            pPack->Type = Type;
            pPack->no_bottom = Pack_NoBottom;
            return pPack;
        }
    }

    // --- Adjust W for bpp ---
    if (BPP != 0x10)
    {
        W = (WW + 1) / 2;
    }
    if (BPP == 4)
    {
        W = (W + 1) / 2;
    }

    // === PASS 1: Forward scan of free list ===
    pPack = Pack_pFree;
    pBest = 0;
    for (; pPack; pPack = pPack->pNext)
    {
        int RightEdge;

        if (BPP == 0x10)
            RightEdge = (pPack->X & -0x40) + 0x100;
        else if (BPP == 8)
            RightEdge = (pPack->X & -0x40) + 0x80;
        else
            RightEdge = (pPack->X & -0x40) + 0x40;

        Over = pPack->X + W > RightEdge;
        if (!Over && Aligned == -1 &&
            ((pPack->X & (W - 1)) || (pPack->Y & (H - 1))))
        {
            Over = 1;
        }

        if ((!Pack_NoBottom || ((pPack->Y + H) & 0xFF)) &&
            (!Pack_NoRight || pPack->X + W != RightEdge) &&
            (Type < 0 || !Over))
        {
            if (pPack->W == W && pPack->H == H)
            {
                pBest = pPack;
                break;
            }

            if ((pBest == 0 ||
                 (pBest->W != W && pBest->H != H) ||
                 pPack->W == W || pPack->H == H) &&
                W <= pPack->W && H <= pPack->H &&
                (pBest == 0 ||
                 pPack->W * pPack->H < pBest->W * pBest->H))
            {
                pBest = pPack;
            }
        }
    }

    {
        if (pBest)
            goto use_best_forward;

        // === PASS 2: Reverse/right-aligned scan ===
        for (pPack = Pack_pFree; pPack; pPack = pPack->pNext)
        {
            int RightEdge;

            if (BPP == 0x10)
                RightEdge = ((pPack->X + pPack->W - W) & -0x40) + 0x100;
            else if (BPP == 8)
                RightEdge = ((pPack->X + pPack->W - W) & -0x40) + 0x80;
            else
                RightEdge = ((pPack->X + pPack->W - W) & -0x40) + 0x40;

            Over = pPack->X + pPack->W > RightEdge;
            if (!Over && Aligned == -1 &&
                (((pPack->X - W) & (W - 1)) ||
                 (pPack->Y & (H - 1))))
            {
                Over = 1;
            }

            if ((!Pack_NoBottom || ((pPack->Y + H) & 0xFF)) &&
                (!Pack_NoRight || pPack->X + pPack->W != RightEdge) &&
                (Type < 0 || !Over))
            {
                if (pPack->W == W && pPack->H == H)
                {
                    pBest = pPack;
                }
                else if ((pBest == 0 ||
                          (pBest->W != W && pBest->H != H) ||
                          pPack->W == W || pPack->H == H) &&
                         W <= pPack->W && H <= pPack->H &&
                         (pBest == 0 ||
                          pPack->W * pPack->H < pBest->W * pBest->H))
                {
                    pBest = pPack;
                }
            }
        }

        if (pBest)
            goto use_best_reverse;

        // === PASS 3: Relaxed (trim to alignment) ===
        if (Aligned == -1)
            return 0;

        int AlignedX;
        int AlignedW;

        for (pPack = Pack_pFree; pPack; pPack = pPack->pNext)
        {
            if ((!Pack_NoBottom || ((pPack->Y + H) & 0xFF)) &&
                (!Pack_NoRight || WW != 0x100) &&
                (pBest == 0 ||
                 (pBest->W != W && pBest->H != H) ||
                 pPack->W == W || pPack->H == H))
            {
                AlignedX = (pPack->X + 0x40) & -0x40;
                AlignedW = pPack->W - (AlignedX - pPack->X);
                if (W <= AlignedW && H <= pPack->H &&
                    (pBest == 0 ||
                     pPack->W * pPack->H < pBest->W * pBest->H))
                {
                    pBest = pPack;
                }
            }
        }

        if (!pBest)
            return 0;

        // Trim left to tpage boundary
        AlignedX = (pBest->X + 0x40) & -0x40;
        AlignedW = pBest->W - (AlignedX - pBest->X);
        if (AlignedW > 0)
        {
            Pack_NewFree(AlignedX, pBest->Y, AlignedW, pBest->H);
            pBest->W -= AlignedW;
        }

        // Recursive call with relaxed params
        pPack = Pack_Pack(Type, WW, H, X, Y, BPP, 0, checksum);
        pPack->bpp = BPP;
        pPack->aligned = Aligned;
        pPack->Type = Type;
        pPack->no_bottom = Pack_NoBottom;
        return pPack;
    }

    // --- Reverse allocation: found the best slot in pass 2 ---
use_best_reverse:
{
    OX = pBest->X;
    OY = pBest->Y;
    OW = pBest->W;
    OH = pBest->H;
    Pack_Remove(&Pack_pFree, pBest);

    pNew = Pack_NewFull(OX + OW - W, OY, W, H);

    if (OW != W)
    {
        if (OH != H)
        {
            TSPack *ns = Pack_NewFree(OX, OY, OW - W, OH);
            Pack_Merge(ns);
            ns = Pack_NewFree(OX + OW - W, OY + H, W, OH - H);
            Pack_Merge(ns);
        }
        else
        {
            TSPack *ns = Pack_NewFree(OX, OY, OW - W, OH);
            Pack_Merge(ns);
        }
    }
    else
    {
        if (OH != H)
        {
            TSPack *ns = Pack_NewFree(OX, OY + H, OW, OH - H);
            Pack_Merge(ns);
        }
    }

    *X = OX + OW - W;
    goto allocation_done;
}

    // --- Forward allocation: found the best slot in pass 1 ---
use_best_forward:
{
    OX = pBest->X;
    OY = pBest->Y;
    OW = pBest->W;
    OH = pBest->H;
    Pack_Remove(&Pack_pFree, pBest);

    pNew = Pack_NewFull(OX, OY, W, H);

    if (OW != W)
    {
        if (OH != H)
        {
            TSPack *ns = Pack_NewFree(OX + W, OY, OW - W, OH);
            Pack_Merge(ns);
            ns = Pack_NewFree(OX, OY + H, W, OH - H);
            Pack_Merge(ns);
        }
        else
        {
            TSPack *ns = Pack_NewFree(OX + W, OY, OW - W, OH);
            Pack_Merge(ns);
        }
    }
    else
    {
        if (OH != H)
        {
            TSPack *ns = Pack_NewFree(OX, OY + H, OW, OH - H);
            Pack_Merge(ns);
        }
    }

    *X = OX;
allocation_done:
    *Y = OY;
    pNew->bpp = BPP;
    pNew->aligned = Aligned;
    pNew->Type = Type;
    pNew->no_bottom = Pack_NoBottom;
    return pNew;
}

}

/**
 *
 * Address: 0x8004AF54
 *          Frees a packed slot at the given VRAM origin and merges it back into the free list.
 * Mangled: Pack_Unpack__Fss
 *
 * @param X  X coordinate.
 * @param Y  Y coordinate.
 */
void Pack_Unpack(short X, short Y)
{
    TSPack *pToFree = Pack_pFull;
    while (pToFree)
    {
        if (pToFree->X == X)
        {
            if (pToFree->Y == Y)
            {
                int w = pToFree->W;
                int h = pToFree->H;
                Pack_Remove(&Pack_pFull, pToFree);
                TSPack *newSlot = Pack_NewFree(X, Y, w, h);
                Pack_Merge(newSlot);
                return;
            }
        }
        pToFree = pToFree->pNext;
    }
}

/**
 * Merges two free slots when they are exactly adjacent and share size constraints.
 *
 * Address: 0x8004B008
 * Mangled: Pack_MergeAdjacent__FP6TSPackT0
 *
 * @param p_first  First free rectangle to test.
 * @param p_second  Second free rectangle to test.
 * @return The combined free rectangle. The return value is unspecified when
 *         the rectangles cannot be merged; callers only pass adjacent pairs.
 */
TSPack *Pack_MergeAdjacent(TSPack *p_first, TSPack *p_second)
{
    int x;
    int y;
    int w;
    int h;

    if (p_first->Y == p_second->Y)
    {
        if (p_first->X < p_second->X)
        {
            if (p_first->X + p_first->W == p_second->X)
            {
                if (p_first->H == p_second->H)
                {
                    x = p_first->X;
                    y = p_first->Y;
                    w = p_first->W + p_second->W;
                    h = p_first->H;
                    Pack_Remove(&Pack_pFree, p_first);
                    Pack_Remove(&Pack_pFree, p_second);
                    return Pack_NewFree(x, y, w, h);
                }
            }
        }
        else if (p_second->X < p_first->X)
        {
            if (p_second->X + p_second->W == p_first->X)
            {
                if (p_first->H == p_second->H)
                {
                    x = p_second->X;
                    y = p_second->Y;
                    w = p_first->W + p_second->W;
                    h = p_first->H;
                    Pack_Remove(&Pack_pFree, p_first);
                    Pack_Remove(&Pack_pFree, p_second);
                    return Pack_NewFree(x, y, w, h);
                }
            }
        }
    }
    else if (p_first->X == p_second->X)
    {
        if (p_first->Y >= p_second->Y)
        {
            if (p_second->Y < p_first->Y)
            {
                if (p_second->Y + p_second->H == p_first->Y)
                {
                    if (p_first->W == p_second->W)
                    {
                        x = p_second->X;
                        y = p_second->Y;
                        w = p_first->W;
                        h = p_first->H + p_second->H;
                        Pack_Remove(&Pack_pFree, p_first);
                        Pack_Remove(&Pack_pFree, p_second);
                        return Pack_NewFree(x, y, w, h);
                    }
                }
            }
        }
        else
        {
            if (p_first->Y + p_first->H == p_second->Y)
            {
                if (p_first->W == p_second->W)
                {
                    x = p_first->X;
                    y = p_first->Y;
                    w = p_first->W;
                    h = p_first->H + p_second->H;

                    Pack_Remove(&Pack_pFree, p_first);
                    Pack_Remove(&Pack_pFree, p_second);
                    return Pack_NewFree(x, y, w, h);
                }
            }
        }
    }
    // The original ABI leaves v0 unchanged on the no-merge path.
    return;
}

/**
 * Repeatedly merges a free slot with any adjacent neighbor in the free list.
 *
 * Address: 0x8004B210
 * Mangled: Pack_Merge__FP6TSPack
 *
 * @param p_merge Free rectangle to merge repeatedly.
 * @return Nonzero when at least one merge occurred.
 */
int Pack_Merge(TSPack *p_merge)
{
    int merged = 0;
    int worth_continuing;
    // Share the selected axis values across the common comparison path.
    int mergeEdge;
    int mergeCoord;
    int freeCoord;

    do
    {
        TSPack *p_free = Pack_pFree;
        worth_continuing = 0;

        while (p_free)
        {
            // Check horizontal adjacency: same Y, same H
            if (p_merge->Y == p_free->Y && p_merge->H == p_free->H)
            {
                mergeCoord = p_merge->X;
                if (p_free->X + p_free->W == mergeCoord)
                    goto do_merge;
                mergeEdge = mergeCoord + p_merge->W;
                freeCoord = p_free->X;
                goto check_tail;
            }
            if (p_merge->X == p_free->X && p_merge->W == p_free->W)
            {
                if ((p_free->Y & 0x100) != (p_merge->Y & 0x100))
                    goto next_p;
                mergeCoord = p_merge->Y;
                if (p_free->Y + p_free->H == mergeCoord)
                    goto do_merge;
                mergeEdge = mergeCoord + p_merge->H;
                freeCoord = p_free->Y;
                goto check_tail;
            }
            goto next_p;

        check_tail:
            if (mergeEdge != freeCoord)
                goto next_p;

        do_merge:
            worth_continuing = 1;
            p_merge = Pack_MergeAdjacent(p_free, p_merge);
            merged = 1;
            goto end_inner;

        next_p:
            p_free = p_free->pNext;
        }

    end_inner:;
    } while (worth_continuing);

    return merged;
}

/**
 * Recursively expose a VRAM rectangle, splitting it around occupied packs and
 * returning each uncovered piece to the free-pack list.
 *
 * Address: 0x8004B31C
 * Mangled: Pack_Zap__Fiiii
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param w  Width.
 * @param h  Height.
 */
void Pack_Zap(int x, int y, int w, int h)
{
    TSPack *p_loop;

REDO:
    p_loop = Pack_pFull;
    while (p_loop)
    {
        if ((p_loop->X + (p_loop->W - 1) >= x) &&
            (p_loop->X < x + w) &&
            (p_loop->Y + (p_loop->H - 1) >= y) &&
            (p_loop->Y < y + h))
        {
            break;
        }
        p_loop = p_loop->pNext;
    }

    if (!p_loop)
    {
        Pack_Merge(Pack_NewFree(x, y, w, h));
        return;
    }
    else
    {
        // A rectangle already covering the requested region needs no split.
        if ((x >= p_loop->X) &&
            (y >= p_loop->Y) &&
            (x + w <= p_loop->X + p_loop->W) &&
            (y + h <= p_loop->Y + p_loop->H))
        {
            return;
        }

        // Small regions are clipped at the first intersecting pack edge.
        if ((w < 0x20) && (h < 0x20))
        {
            if (x < p_loop->X)
            {
                int half = p_loop->X - x;
                Pack_Zap(x, y, half, h);
                x += half;
                w -= half;
                goto REDO;
            }
            else if (p_loop->X + p_loop->W < x + w)
            {
                int half = p_loop->X + p_loop->W - x;
                Pack_Zap(x, y, half, h);
                x += half;
                w -= half;
                goto REDO;
            }
            else if (y < p_loop->Y)
            {
                int half = p_loop->Y - y;
                Pack_Zap(x, y, w, half);
                y += half;
                h -= half;
                goto REDO;
            }
            else if (p_loop->Y + p_loop->H < y + h)
            {
                int half = p_loop->Y + p_loop->H - y;
                Pack_Zap(x, y, w, half);
                y += half;
                h -= half;
                goto REDO;
            }
        }

        if (w >= h)
        {
            int half = w / 2;
            Pack_Zap(x, y, half, h);
            x += half;
            w -= half;
        }
        else
        {
            int half = h / 2;
            Pack_Zap(x, y, w, half);
            y += half;
            h -= half;
        }
    }
    goto REDO;
}

/**
 * Rebuild and fully merge the free-region list around all occupied packs.
 *
 * Address: 0x8004B54C
 * Mangled: Pack_DefragmentFully__Fv
 */
void Pack_DefragmentFully(void)
{
    TSPack *p_loop;
    int Changes;

    if (Pack_pFree != 0)
    {
        // Kill all free slots
        ZapCalls = 0;
        p_loop = Pack_pFree;
        do
        {
            TSPack *p_kill = p_loop;
            p_loop = p_loop->pNext;
            Pack_KillSlot(p_kill);
        } while (p_loop);

        Pack_pFree = 0;

        // Rebuild the free regions for the active VRAM packing width.
        switch (Pack_Width)
        {
        case 0x140:
            // 320-wide mode
            Pack_Zap(0x140, 0x100, 0x40, 0x100);
            Pack_Zap(Pack_Width + 0x40, 0x100, 0x80, 0x100);
            Pack_Zap(Pack_Width + 0xC0, 0x100, 0x200, 0x100);
            Pack_Zap(Pack_Width, 0, 0x40, 0x100);
            Pack_Zap(Pack_Width + 0x40, 0, 0x80, 0x100);
            Pack_Zap(Pack_Width + 0xC0, 0, 0x200, 0x100);
            break;
        case 0x280:
            // 640-wide mode
            Pack_Zap(0x280, 0x100, 0x80, 0x100);
            Pack_Zap(Pack_Width + 0x80, 0x100, 0x100, 0x100);
            Pack_Zap(Pack_Width, 0, 0x80, 0x100);
            Pack_Zap(Pack_Width + 0x80, 0, 0x100, 0x100);
            break;
        default:
            // Default mode
            Pack_Zap(Pack_Width, 0x100, 0x400 - Pack_Width, 0x100);
            Pack_Zap(Pack_Width, 0, 0x400 - Pack_Width, 0x100);
        }

        // Merge all free blocks repeatedly until stable
        do
        {
            Changes = 0;
            for (TSPack *p_merge = Pack_pFree;
                 p_merge;
                 p_merge = p_merge->pNext)
            {
                Changes |= Pack_Merge(p_merge);
            }
        } while (Changes);
    }
}
