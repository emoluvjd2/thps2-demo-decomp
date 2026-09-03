#if !defined(PAL_SHARED_STATE_DECLARATIONS_EMITTED)
#define PAL_SHARED_STATE_DECLARATIONS_EMITTED

struct tag_S_Pal;

#ifdef PAL_OWNER_DECLARATIONS
#define PAL_COORDINATE_DECL_ATTR
#else
#define PAL_COORDINATE_DECL_ATTR __attribute__((section(".data")))
#endif

/** X coordinate of the shared 16-colour palette allocation area. */
extern int Pal16X PAL_COORDINATE_DECL_ATTR; // 0x800DA37C

/** Y coordinate of the shared 16-colour palette allocation area. */
extern int Pal16Y PAL_COORDINATE_DECL_ATTR; // 0x800DA380

#undef PAL_COORDINATE_DECL_ATTR
#endif // PAL_SHARED_STATE_DECLARATIONS_EMITTED

#if defined(PAL_ENTRY_LAYOUT_ONLY) && !defined(PAL_ENTRY_LAYOUT)
#error PAL_ENTRY_LAYOUT_ONLY requires PAL_ENTRY_LAYOUT
#endif

#if !defined(PAL_OWNER_LAYOUT) && !defined(PAL_ENTRY_LAYOUT_ONLY) && \
    !defined(PAL_INC_H)
#define PAL_INC_H

/** Initialize the palette allocator and its VRAM slot tables. */
void Pal_Init(void);

/** Find a palette allocation record by its asset checksum. */
tag_S_Pal *Pal_FindPaletteEntry(unsigned int checksum);

/** Releases palette entries whose usage count reached zero. */
void Pal_RemoveUnusedPalettes(void);

/** Reserve and return the first available 16-colour palette slot. */
int GetFree16Slot(void);

/** Release a 16-colour palette slot identified by its CLUT value. */
void Free16Slot(unsigned short clut);

/** Load one palette into VRAM and return its allocation record. */
tag_S_Pal *Pal_LoadPalette(
    unsigned int checksum, unsigned int *pData,
    unsigned char flags, int Remap);

/** Copy one palette's VRAM contents into caller-owned memory. */
void Pal_CopyToMemory(tag_S_Pal *palette, void *buffer);

/** Upload caller-owned palette data into an existing VRAM allocation. */
void Pal_StoreFromMemory(tag_S_Pal *palette, void *buffer);

#endif // ordinary PAL declarations

#if defined(PAL_OWNER_LAYOUT) && !defined(PAL_ENTRY_LAYOUT)
#define PAL_ENTRY_LAYOUT
#define PAL_OWNER_IMPLIED_ENTRY_LAYOUT
#endif

#if defined(PAL_ENTRY_LAYOUT) && !defined(PAL_ENTRY_LAYOUT_EMITTED)
#define PAL_ENTRY_LAYOUT_EMITTED

/** One linked palette allocation record. */
struct tag_S_Pal {
    unsigned short Clut;
    unsigned char slot;
    unsigned char flags;
    unsigned short Usage;
    unsigned char InVRAM;
    unsigned char RemapIndex;
    unsigned int Checksum;
    struct tag_S_Pal *pNext;
    struct tag_S_Pal *pPrevious;
};

typedef char tag_S_Pal_size_must_be_20[
    (sizeof(tag_S_Pal) == 20) ? 1 : -1];
typedef char tag_S_Pal_Clut_offset_must_be_0[
    ((unsigned long)&(((tag_S_Pal *)0)->Clut) == 0) ? 1 : -1];
typedef char tag_S_Pal_slot_offset_must_be_2[
    ((unsigned long)&(((tag_S_Pal *)0)->slot) == 2) ? 1 : -1];
typedef char tag_S_Pal_flags_offset_must_be_3[
    ((unsigned long)&(((tag_S_Pal *)0)->flags) == 3) ? 1 : -1];
typedef char tag_S_Pal_Usage_offset_must_be_4[
    ((unsigned long)&(((tag_S_Pal *)0)->Usage) == 4) ? 1 : -1];
typedef char tag_S_Pal_InVRAM_offset_must_be_6[
    ((unsigned long)&(((tag_S_Pal *)0)->InVRAM) == 6) ? 1 : -1];
typedef char tag_S_Pal_RemapIndex_offset_must_be_7[
    ((unsigned long)&(((tag_S_Pal *)0)->RemapIndex) == 7) ? 1 : -1];
typedef char tag_S_Pal_Checksum_offset_must_be_8[
    ((unsigned long)&(((tag_S_Pal *)0)->Checksum) == 8) ? 1 : -1];
typedef char tag_S_Pal_pNext_offset_must_be_12[
    ((unsigned long)&(((tag_S_Pal *)0)->pNext) == 12) ? 1 : -1];
typedef char tag_S_Pal_pPrevious_offset_must_be_16[
    ((unsigned long)&(((tag_S_Pal *)0)->pPrevious) == 16) ? 1 : -1];

#endif // PAL_ENTRY_LAYOUT && !PAL_ENTRY_LAYOUT_EMITTED

#ifdef PAL_OWNER_IMPLIED_ENTRY_LAYOUT
#undef PAL_ENTRY_LAYOUT
#undef PAL_OWNER_IMPLIED_ENTRY_LAYOUT
#endif

#if defined(PAL_OWNER_LAYOUT) && !defined(PAL_ENTRY_LAYOUT_ONLY) && \
    !defined(PAL_OWNER_LAYOUT_EMITTED)
#define PAL_OWNER_LAYOUT_EMITTED

/** Palette capacities, record flags, and allocator accounting constants. */
enum PaletteConstants {
    Palette16SlotCount = 256,
    Palette256SlotCount = 75,
    Palette16ColourCount = 16,
    Palette256ColourCount = 256,
    PaletteAllocationOverhead = 4,
    PaletteFlag16Colour = 1,
    PaletteFlag256Colour = 2
};

/**
 * Address a palette colour while preserving the target's index-first add.
 * Ordinary pointer indexing commutes that `addu` under GCC 2.91.
 */
#define PAL_COLOUR_AT_INDEX(data, index) \
    ((unsigned short *)((int)(index) * sizeof(unsigned short) + (int)(data)))

#endif // PAL_OWNER_LAYOUT && !PAL_OWNER_LAYOUT_EMITTED
