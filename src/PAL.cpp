// PAL.cpp -- Reconstructed translation unit
// Original: PAL.CPP
// Functions: 13 reconstructed entries
// Address range: 0x8004B6EC - 0x8004BF04
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

// Owner-local declarations attributed to PAL.cpp by MAIN.SYM.
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned char byte;
typedef int undefined;
typedef char undefined1;
typedef short undefined2;
typedef int undefined4;
typedef unsigned long ulong;
typedef unsigned char undefined3[3];

#define PAL256_OWNER_DECLARATIONS
#include "PAL256.h"
#undef PAL256_OWNER_DECLARATIONS
#define TOTALPALETTEUSAGE_OWNER_DECLARATION
#include "TOTALPALETTEUSAGE.h"
#define TRANSPARENTPALFOREDITOR_OWNER_DECLARATION
#include "TRANSPARENTPALFOREDITOR.h"

#define PAL_OWNER_DECLARATIONS
#define PAL_OWNER_LAYOUT
#include "PAL.h"
#undef PAL_OWNER_LAYOUT
#undef PAL_OWNER_DECLARATIONS
#include "MEM.h"
#include "PACK.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBGPU.H>

/** Target-era external compiler view of PAL's list head. */
extern tag_S_Pal *pPaletteList;

static u8 Free16Slots[Palette16SlotCount] __attribute__((section(".bss")));
static u8 Free256Slots[Palette256SlotCount] __attribute__((section(".bss")));
u8 HasTrans16[Palette16SlotCount] __attribute__((section(".data"))) = {};
u8 HasTrans256[Palette256SlotCount] __attribute__((section(".data"))) = {};

// === Function implementations (address order) ===

/**
 * Pal_Init__Fv — Initialize the PS1 palette VRAM system
 * Sets up VRAM regions for 256-color and 16-color palettes, initializes
 * availability tracking arrays, and clears the palette linked list.
 * VRAM layout:
 *   256-color palettes: 256 pixels wide × 75 rows = 75 palette slots
 *   16-color palettes:  16 pixels wide × 256 rows = 256 palette slots
 *   Each palette row is one CLUT (Color Look-Up Table) entry in VRAM
 * Slot 0 of each type is reserved as the default palette:
 *   Index 0 = black (0,0,0) = transparent
 *   All other indices = gray (128,128,128) = placeholder
 * After initialization, the availability arrays track which VRAM rows
 * are free for loading new palette data.
 * External globals
 *
 * Address:   0x8004B6EC, Size: 424 bytes (107 instructions)
 * Mangled:   Pal_Init__Fv
 * Demangled: void Pal_Init(void)
 * Match:     PERFECT (107/107)
 */

void Pal_Init(void) {
    int i;
    RECT rec;

    // Clear palette linked list
    pPaletteList = 0;

    // Allocate VRAM rectangle for 256-color palettes (256 wide × 75 tall)
    Pack_Pack(
        -1, Palette256ColourCount, Palette256SlotCount,
        &Pal256X, &Pal256Y, Palette16ColourCount, 0, 0);

    // Allocate VRAM rectangle for 16-color palettes (16 wide × 256 tall)
    Pack_Pack(
        -1, Palette16ColourCount, Palette16SlotCount,
        &Pal16X, &Pal16Y, Palette16ColourCount, 0, 0);

    // Mark all 256 16-color palette slots as available
    i = 0;
    do {
        Free16Slots[i] = 1;
        i = i + 1;
    } while (i < Palette16SlotCount);

    // Reserve slot 0 of 256-color palettes, mark rest as available
    Free256Slots[0] = 0;
    i = 1;
    do {
        Free256Slots[i] = 1;
        i = i + 1;
    } while (i < Palette256SlotCount);

    // Clear default 256-color palette (slot 0) in VRAM
    Free256Slots[0] = 0;
    rec.x = (short)Pal256X;
    rec.w = Palette256ColourCount;
    rec.y = (short)Pal256Y;
    rec.h = 1;
    ClearImage(&rec, 0, 0, 0);
    DrawSync(0);

    // Remaining pixels (indices 1-255) = gray placeholder
    rec.x = rec.x + 1;
    rec.w = rec.w - 1;
    ClearImage(&rec, 128, 128, 128);
    DrawSync(0);

    // Clear default 16-color palette (slot 0) in VRAM
    Free16Slots[0] = 0;
    rec.x = (short)Pal16X;
    rec.y = (short)Pal16Y;
    rec.w = Palette16ColourCount;
    rec.h = 1;
    ClearImage(&rec, 0, 0, 0);
    DrawSync(0);

    // Remaining pixels (indices 1-15) = gray placeholder
    rec.x = rec.x + 1;
    rec.w = rec.w - 1;
    ClearImage(&rec, 128, 128, 128);
    DrawSync(0);
}

/**
 * Allocate and insert a palette record at the head of the owned list.
 * Initializes its checksum, usage, and residency fields and accounts for the
 * allocator's four-byte per-allocation overhead.
 *
 * Address:   0x8004B898, Size: 104 bytes (26 instructions)
 * Mangled:   NewPaletteEntry__FUi
 * Demangled: tag_S_Pal *NewPaletteEntry(unsigned int)
 * Match:     PERFECT (26/26)
 *
 * @param checksum  Asset hash for content lookup.
 * @return Newly allocated palette record.
 */

tag_S_Pal *NewPaletteEntry(u32 checksum) {
    tag_S_Pal *pNew;
    int totalUsage;
    tag_S_Pal *listHead;

    pNew = static_cast<tag_S_Pal *>(
        Mem_New(sizeof(tag_S_Pal), 0, 1));
    totalUsage = TotalPaletteUsage;
    listHead = pPaletteList;
    pNew->pPrevious = 0;
    pPaletteList = pNew;
    TotalPaletteUsage =
        totalUsage + sizeof(tag_S_Pal) + PaletteAllocationOverhead;
    pNew->pNext = listHead;
    if (listHead != 0) {
        listHead->pPrevious = pNew;
    }
    pNew->Checksum = checksum;
    pNew->InVRAM = 0;
    pNew->Usage = 0;
    return pNew;
}

/**
 * RemovePaletteEntry__FP9tag_S_Pal — Remove palette entry from linked list
 * Doubly-linked list unlink: updates next's prev and prev's next to skip
 * around this entry. If entry is the list head, replaces head with next.
 * Frees memory and updates TotalPaletteUsage for the record and allocator
 * overhead.
 *
 * Address:   0x8004B900, Size: 116 bytes (29 instructions)
 * Mangled:   RemovePaletteEntry__FP9tag_S_Pal
 * Demangled: void RemovePaletteEntry(tag_S_Pal *)
 * Match:     PERFECT (29/29)
 *
 * @param pEntry  Entry to operate on.
 */

void RemovePaletteEntry(tag_S_Pal *pEntry) {
    // Unlink: update next's prev pointer
    if (pEntry->pNext != 0) {
        pEntry->pNext->pPrevious = pEntry->pPrevious;
    }
    // Unlink: update prev's next pointer
    if (pEntry->pPrevious != 0) {
        pEntry->pPrevious->pNext = pEntry->pNext;
    }
    // If we were the head, replace with our next
    if (pEntry == pPaletteList) {
        pPaletteList = pEntry->pNext;
    }
    Mem_Delete(pEntry);
    TotalPaletteUsage =
        TotalPaletteUsage - sizeof(tag_S_Pal) - PaletteAllocationOverhead;
}

/**
 * Search the palette list for a matching checksum.
 * Returns pointer to the matching tag_S_Pal entry, or 0 if not found.
 *
 * Address:   0x8004B974, Size: 56 bytes (14 instructions)
 * Mangled:   Pal_FindPaletteEntry__FUi
 * Demangled: tag_S_Pal *Pal_FindPaletteEntry(unsigned int)
 * Match:     PERFECT (14/14)
 *
 * @param checksum  Asset hash for content lookup.
 * @return Matching palette record, or null when absent.
 */
tag_S_Pal *Pal_FindPaletteEntry(u32 checksum) {
    tag_S_Pal *pSearch;

    for (pSearch = pPaletteList;
         pSearch != 0 && pSearch->Checksum != checksum;
         pSearch = pSearch->pNext) {
    }
    return pSearch;
}

/**
 * Pal_RemoveUnusedPalettes__Fv — Garbage-collect unreferenced palette entries
 * Walks the entire palette linked list. For each entry with usageCount == 0:
 *   - If typeFlags bit 0 (16-color): marks Free16Slots[slotIndex] = 1
 *   - If typeFlags bit 1 (256-color): marks Free256Slots[slotIndex] = 1
 *   - Removes the palette entry from the list
 * Uses a save-next-before-remove pattern since RemovePaletteEntry frees memory.
 *
 * Address:   0x8004B9AC, Size: 188 bytes (47 instructions)
 * Mangled:   Pal_RemoveUnusedPalettes__Fv
 * Demangled: void Pal_RemoveUnusedPalettes(void)
 */

void Pal_RemoveUnusedPalettes(void) {
    tag_S_Pal *pPal;
    tag_S_Pal *pNext;

    pPal = pPaletteList;
    while (pPal != 0) {
        pNext = pPal->pNext;
        if (pPal->Usage == 0) {
            // Free 16-color VRAM slot
            if ((pPal->flags & PaletteFlag16Colour) != 0) {
                Free16Slots[pPal->slot] = 1;
            }
            // Free 256-color VRAM slot
            if ((pPal->flags & PaletteFlag256Colour) != 0) {
                Free256Slots[pPal->slot] = 1;
            }
            RemovePaletteEntry(pPal);
        }
        pPal = pNext;
    }
}

/**
 * GetFree16Slot__Fv — Find a free 16-color palette VRAM slot
 * Scans Free16Slots[0..255] for the first available slot (value != 0).
 * Marks the found slot as used (0) and returns its index.
 * If no slot is free, returns 256 (past end) — caller must handle.
 *
 * Address:   0x8004BA68, Size: 80 bytes (20 instructions)
 * Mangled:   GetFree16Slot__Fv
 * Demangled: int GetFree16Slot(void)
 * Match:     PERFECT (20/20)
 *
 * @return i
 */
int GetFree16Slot(void) {
    int i;

    i = 0;
    if (Free16Slots[i] == 0) {
        do {
            i++;
        } while (i < Palette16SlotCount && Free16Slots[i] == 0);
    }
    Free16Slots[i] = 0;
    return i;
}


/**
 * Free a 16-color palette slot by its CLUT value.
 * Iterates all 256 16-color palette slots, calling GetClut to compute each
 * slot's CLUT value. When the matching slot is found, marks it as available
 * (Free16Slots[i] = 1).
 *
 * Address:   0x8004BAB8, Size: 100 bytes (25 instructions)
 * Mangled:   Free16Slot__FUs
 * Demangled: void Free16Slot(unsigned short)
 * Match:     PERFECT (25/25)
 *
 * @param clut  Palette CLUT (color lookup table) address.
 */
void Free16Slot(unsigned short clut) {
    int i;

    for (i = 0; i < Palette16SlotCount; i++) {
        if (GetClut(Pal16X, Pal16Y + i) == clut) {
            break;
        }
    }
    Free16Slots[i] = 1;
}


/**
 * GetFree256Slot__Fv — Find a free 256-color palette VRAM slot
 * Scans Free256Slots[0..74] for the first available slot (value != 0).
 * Marks the found slot as used (0) and returns its index.
 *
 * Address:   0x8004BB1C, Size: 80 bytes (20 instructions)
 * Mangled:   GetFree256Slot__Fv
 * Demangled: int GetFree256Slot(void)
 * Match:     PERFECT (20/20)
 *
 * @return i
 */
int GetFree256Slot(void) {
    int i;

    i = 0;
    if (Free256Slots[i] == 0) {
        do {
            i++;
        } while (i < Palette256SlotCount && Free256Slots[i] == 0);
    }
    Free256Slots[i] = 0;
    return i;
}


/**
 * Free a 256-color palette slot by its CLUT value.
 * Iterates all 75 256-color palette slots, calling GetClut to compute each
 * slot's CLUT value. When the matching slot is found, marks it as available
 * (Free256Slots[i] = 1).
 *
 * Address:   0x8004BB6C, Size: 100 bytes (25 instructions)
 * Mangled:   Free256Slot__FUs
 * Demangled: void Free256Slot(unsigned short)
 * Match:     PERFECT (25/25)
 *
 * @param clut  Palette CLUT (color lookup table) address.
 */
void Free256Slot(unsigned short clut) {
    int i;

    for (i = 0; i < Palette256SlotCount; i++) {
        if (GetClut(Pal256X, Pal256Y + i) == clut) {
            break;
        }
    }
    Free256Slots[i] = 1;
}


/**
 * Get256SlotNumber__FUs — Look up a 256-color palette slot by CLUT value
 * Iterates all 75 256-color palette slots, calling GetClut to compute each
 * slot's CLUT value. Returns the slot index when a match is found, or 75
 * if no match exists.
 *
 * Address:   0x8004BBD0, Size: 84 bytes (21 instructions)
 * Mangled:   Get256SlotNumber__FUs
 * Demangled: int Get256SlotNumber(unsigned short)
 * Match:     PERFECT (21/21)
 *
 * @param clut  Palette CLUT (color lookup table) address.
 * @return i
 */
int Get256SlotNumber(unsigned short clut) {
    int i;

    for (i = 0; i < Palette256SlotCount; i++) {
        if (GetClut(Pal256X, Pal256Y + i) == clut) {
            break;
        }
    }
    return i;
}


/**
 * Load palette data into VRAM while detecting and remapping transparency.
 *
 * The `$4` entry cursor, empty scheduling constraint, and explicit 16-bit
 * normalization remain load-bearing under GCC 2.91. Clean-source variants
 * were measured before retaining each one.
 *
 * Address:   0x8004BC24, Size: 608 bytes (152 instructions)
 * Mangled:   Pal_LoadPalette__FUiPUiUci
 * Demangled: tag_S_Pal *Pal_LoadPalette(unsigned int, unsigned int *, unsigned char, int)
 * Match:     PERFECT (152/152)
 *
 * @param checksum  Asset checksum for the new palette record.
 * @param pData  Packed ABGR1555 palette colours.
 * @param flags  Palette-format flags.
 * @param Remap  Whether to move the transparent colour to entry zero.
 * @return Newly allocated and resident palette record.
 */

tag_S_Pal *Pal_LoadPalette(
    unsigned int checksum, unsigned int *pData, unsigned char flags, int Remap) {
    tag_S_Pal *pNew;
    int transparent_encountered;
    register u16 *pEntry asm("$4");
    int i;
    int transparent_index;
    int entries;
    u16 color;
    RECT rec;

    pNew = NewPaletteEntry(checksum);
    transparent_encountered = 0;
    pEntry = reinterpret_cast<u16 *>(pData);
    entries = Palette256ColourCount;
    pNew->flags = flags;
    rec.h = 1;

    if (flags & PaletteFlag16Colour) {
        entries = Palette16ColourCount;
    }

    transparent_index = 0;
    i = 0;
    if (entries != 0) {
        do {
            color = *pEntry;
            if ((color & 0x7FFF) == 0x7C1F) {
                // Transparent key color (magenta)
                if (transparent_encountered == 0) {
                    *pEntry = 0;
                    transparent_index = i;
                    transparent_encountered = 1;
                }
            } else {
                if (transparent_encountered != 0) goto store_color;
                {
                    u16 colorTest;
                    asm("andi %0, %1, 0xFFFF"
                        : "=r"(colorTest) : "r"(color));
                    if (colorTest == 0) goto store_color;
                }
                if (TransparentPalForEditor != 0) {
                    store_color:
                    *pEntry = color | 0x8000;
                }
            }
            i = i + 1;
            pEntry = pEntry + 1;
        } while (i < entries);
    }

    // Swap transparent color to index 0
    if (Remap != 0 && transparent_encountered != 0) {
        u16 tmp;
        tmp = *reinterpret_cast<u16 *>(pData);
        u16 transColor = *PAL_COLOUR_AT_INDEX(pData, transparent_index);
        *PAL_COLOUR_AT_INDEX(pData, transparent_index) = tmp;
        *reinterpret_cast<u16 *>(pData) = transColor;
        pNew->RemapIndex = (u8)transparent_index;
    } else {
        pNew->RemapIndex = 0;
    }

    // If no transparent found, set STP bit on all colors
    if (transparent_encountered == 0) {
        pEntry = reinterpret_cast<u16 *>(pData);
        asm("");
        if (transparent_encountered < entries) {
            i = 0;
            do {
                i = i + 1;
                *pEntry = *pEntry | 0x8000;
                pEntry = pEntry + 1;
            } while (i < entries);
        }
    }

    if (entries == Palette16ColourCount) {
        pNew->slot = (u8)GetFree16Slot();
        rec.x = (short)Pal16X;
        rec.y = (short)(Pal16Y + pNew->slot);
        pNew->Clut = (u16)(((Pal16Y + pNew->slot) << 6) | ((Pal16X >> 4) & 0x3F));
        if (transparent_encountered != 0) {
            HasTrans16[pNew->slot] = 1;
        } else {
            HasTrans16[pNew->slot] = 0;
        }
    } else {
        pNew->slot = (u8)GetFree256Slot();
        rec.x = (short)Pal256X;
        rec.y = (short)(Pal256Y + pNew->slot);
        pNew->Clut = (u16)(((Pal256Y + pNew->slot) << 6) | ((Pal256X >> 4) & 0x3F));
        if (transparent_encountered != 0) {
            HasTrans256[pNew->slot] = 1;
        } else {
            HasTrans256[pNew->slot] = 0;
        }
    }

    rec.w = (short)entries;
    LoadImage(&rec, reinterpret_cast<unsigned long *>(pData));
    pNew->InVRAM = 1;
    return pNew;
}


/**
 * Pal_CopyToMemory__FP9tag_S_PalPv — Copy palette data from VRAM to memory
 * Reads a palette's VRAM region into a memory buffer via StoreImage.
 * Builds a RECT on the stack based on the palette type:
 *   16-color (typeFlags bit 0 set):  w=16,  VRAM coords from Pal16 globals
 *   256-color (typeFlags bit 0 clear): w=256, VRAM coords from Pal256 globals
 * Y coordinate = base Y + slotIndex; height is always 1 row.
 * External globals
 *
 * Address:   0x8004BE84, Size: 128 bytes (32 instructions)
 * Mangled:   Pal_CopyToMemory__FP9tag_S_PalPv
 * Demangled: void Pal_CopyToMemory(tag_S_Pal *, void *)
 * Match:     PERFECT (32/32)
 *
 * @param p_pal  Palette record identifying the VRAM slot.
 * @param p_buffer  Destination buffer for palette colours.
 */
void Pal_CopyToMemory(tag_S_Pal *p_pal, void *p_buffer) {
    RECT rc;

    if (p_pal->flags & PaletteFlag16Colour) {
        // 16-color palette
        rc.x = (short)Pal16X;
        rc.w = Palette16ColourCount;
        rc.h = 1;
        rc.y = (short)(Pal16Y + p_pal->slot);
    } else {
        // 256-color palette
        rc.x = (short)Pal256X;
        rc.w = Palette256ColourCount;
        rc.h = 1;
        rc.y = (short)(Pal256Y + p_pal->slot);
    }
    StoreImage(&rc, reinterpret_cast<unsigned long *>(p_buffer));
}

/**
 * Pal_StoreFromMemory__FP9tag_S_PalPv — Upload palette from RAM to VRAM
 * Reverse of Pal_CopyToMemory: uploads palette data from RAM to VRAM.
 * Checks typeFlags bit 0 to determine 16-color vs 256-color palette,
 * then calls the appropriate PSY-Q LoadClut function with VRAM coordinates
 * derived from the palette slot index.
 * For 16-color (bit 0 set): LoadClut2(data, Pal16X, Pal16Y + slotIndex)
 * For 256-color (bit 0 clear): LoadClut(data, Pal256X, Pal256Y + slotIndex)
 * External globals ($gp-relative)
 *
 * Address:   0x8004BF04, Size: 96 bytes (24 instructions)
 * Mangled:   Pal_StoreFromMemory__FP9tag_S_PalPv
 * Demangled: void Pal_StoreFromMemory(tag_S_Pal *, void *)
 * Match:     PERFECT (24/24)
 *
 * @param p_pal  Palette record identifying the VRAM slot.
 * @param p_buffer  Source buffer containing palette colours.
 */

void Pal_StoreFromMemory(tag_S_Pal *p_pal, void *p_buffer) {
    if (p_pal->flags & PaletteFlag16Colour) {
        /* 16-color palette */
        LoadClut2(reinterpret_cast<unsigned long *>(p_buffer),
                  Pal16X, Pal16Y + p_pal->slot);
    } else {
        /* 256-color palette */
        LoadClut(reinterpret_cast<unsigned long *>(p_buffer),
                 Pal256X, Pal256Y + p_pal->slot);
    }
}
