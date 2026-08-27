#ifndef PACK_H
#define PACK_H

struct TSPack;

/** Initialize the standard 512-by-240 VRAM pack layout. */
void Pack_Init(void);

/** VRAM layout selected by the pack allocator initialization routines. */
enum EPackInitType {
    PACK512x240 = 0,
    PACK640x480 = 1,
};

/** PACK-owned allocator lists, limits, and usage counters. */
extern "C" EPackInitType LastPackInitType;       // 0x800DA364

/*
 * PACK accesses these owner definitions through $gp.  Their collaborating
 * translation units materialize the same addresses directly.
 */
#ifdef PACK_OWNER_DECLARATIONS
#define PACK_STATE_DECL_ATTR
#else
#define PACK_STATE_DECL_ATTR __attribute__((section(".data")))
#endif

extern "C" int Pack_NoBottom PACK_STATE_DECL_ATTR;       // 0x800DA358
extern "C" int Pack_NoRight PACK_STATE_DECL_ATTR;       // 0x800DA35C
extern "C" int Pack_Width;       // 0x800DA360
extern "C" TSPack *Pack_pFree;       // 0x800DA34C
extern "C" TSPack *Pack_pFull;       // 0x800DA350
extern "C" int TotSlots;       // 0x800DA354
extern "C" int TotalPackUsage PACK_STATE_DECL_ATTR;       // 0x800DA368
extern "C" int ZapCalls;       // 0x800DA36C

#undef PACK_STATE_DECL_ATTR

/** Allocate one pack-list entry. */
TSPack *Pack_GetSlot(void);

/** Release one pack-list entry. */
void Pack_KillSlot(TSPack *pToFree);

TSPack *Pack_New(TSPack **ppHead);
TSPack *Pack_NewFree(int X, int Y, int W, int H);
TSPack *Pack_NewFull(int X, int Y, int W, int H);
void Pack_Remove(TSPack **pHead, TSPack *pRemove);
void Pack_Init640x480(void);
TSPack *Pack_Pack(int Type, int WW, int H, int *X, int *Y, int BPP,
                  int Aligned, unsigned int checksum);
void Pack_Unpack(short X, short Y);
TSPack *Pack_MergeAdjacent(TSPack *p_first, TSPack *p_second);
int Pack_Merge(TSPack *p_merge);
void Pack_Zap(int x, int y, int w, int h);
void Pack_DefragmentFully(void);

#endif // PACK_H

#if defined(PACK_TSPACK_LAYOUT) && !defined(PACK_TSPACK_LAYOUT_EMITTED)
#define PACK_TSPACK_LAYOUT_EMITTED

/** One rectangle in the VRAM pack allocator's free or occupied list. */
struct TSPack {
    unsigned short X;
    unsigned short Y;
    unsigned short W;
    unsigned short H;
    struct TSPack *pNext;
    char Type;
    unsigned char no_bottom;
    unsigned char bpp;
    unsigned char aligned;
};

typedef char TSPack_size_must_be_16[
    (sizeof(TSPack) == 16) ? 1 : -1];
typedef char TSPack_X_offset_must_be_0[
    ((unsigned long)&(((TSPack *)0)->X) == 0) ? 1 : -1];
typedef char TSPack_Y_offset_must_be_2[
    ((unsigned long)&(((TSPack *)0)->Y) == 2) ? 1 : -1];
typedef char TSPack_W_offset_must_be_4[
    ((unsigned long)&(((TSPack *)0)->W) == 4) ? 1 : -1];
typedef char TSPack_H_offset_must_be_6[
    ((unsigned long)&(((TSPack *)0)->H) == 6) ? 1 : -1];
typedef char TSPack_pNext_offset_must_be_8[
    ((unsigned long)&(((TSPack *)0)->pNext) == 8) ? 1 : -1];
typedef char TSPack_Type_offset_must_be_12[
    ((unsigned long)&(((TSPack *)0)->Type) == 12) ? 1 : -1];
typedef char TSPack_no_bottom_offset_must_be_13[
    ((unsigned long)&(((TSPack *)0)->no_bottom) == 13) ? 1 : -1];
typedef char TSPack_bpp_offset_must_be_14[
    ((unsigned long)&(((TSPack *)0)->bpp) == 14) ? 1 : -1];
typedef char TSPack_aligned_offset_must_be_15[
    ((unsigned long)&(((TSPack *)0)->aligned) == 15) ? 1 : -1];

#endif // PACK_TSPACK_LAYOUT && !PACK_TSPACK_LAYOUT_EMITTED
