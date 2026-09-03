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

/*
 * PACK owns these small-data definitions.  Consumers materialize their target
 * addresses directly, while the owner must retain its $gp-relative view.
 */
#ifdef PACK_OWNER_DECLARATIONS
#define PACK_SHARED_STATE_DECL_ATTR
#else
#define PACK_SHARED_STATE_DECL_ATTR __attribute__((section(".data")))
#endif

/** Suppress allocations that touch the bottom edge of the VRAM pack area. */
extern "C" int Pack_NoBottom PACK_SHARED_STATE_DECL_ATTR;

/** Suppress allocations that touch the right edge of the VRAM pack area. */
extern "C" int Pack_NoRight PACK_SHARED_STATE_DECL_ATTR;

/** Total heap bytes currently charged to VRAM pack-list entries. */
extern "C" int TotalPackUsage PACK_SHARED_STATE_DECL_ATTR;

#undef PACK_SHARED_STATE_DECL_ATTR

void Pack_Init640x480(void);
TSPack *Pack_Pack(int Type, int WW, int H, int *X, int *Y, int BPP,
                  int Aligned, unsigned int checksum);
void Pack_Unpack(short X, short Y);
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
