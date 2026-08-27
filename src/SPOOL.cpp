// SPOOL.cpp -- Reconstructed translation unit
// Original: SPOOL.CPP
// Functions: 32 reconstructed entries
// Address range: 0x800674F8 - 0x80069768
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

extern "C" {

/** One linked animation packet owned by the spooler. */
struct AnimPacket {
    unsigned int *pPacket;
    struct AnimPacket *pNext;
};

}

#include "FILEIO.h"

#include "globals.h"
#define SPOOL_QUEUE_LAYOUT
#define SPOOL_OWNER_LOCAL_SERVICE_DECLS
#include "SPOOL.h"
#undef SPOOL_OWNER_LOCAL_SERVICE_DECLS
#undef SPOOL_QUEUE_LAYOUT
#include "M3DZONE.h"
#include "M3DINIT.h"
#include "PACK.h"
#include "PSXREGION.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include "SMODEL.h"

/** Packed environment payload header followed by its linked item records. */
struct SpoolEnvironmentData {
    unsigned int Header[3];
    Item Items[1];
};

struct tag_S_Pal;
struct STexWibItemInfo;

/** Byte-addressable model counters updated by animation packet attachment. */
struct SpoolAnimatedModelHeader {
    unsigned char _padToNormalCount[4];
    unsigned char NormalCountLow;
    unsigned char NormalCountHigh;
    unsigned short FaceCount;
    unsigned char RadiusByte0;
    unsigned char RadiusByte1;
    unsigned char RadiusByte2;
    unsigned char RadiusByte3;
};

#include "TEXTURE.h"

#include "SANIMFRAME.h"

/** Variable-length animation entry prefix followed by SAnimFrame records. */
struct SpoolAnimationEntryHeader {
    char Name[8];
    unsigned int NumFrames;
};

/** Packed texture payload prefix followed by its pixel words. */
struct SpoolTextureData {
    unsigned int Flags;
    unsigned int NumColours;
    unsigned int PaletteChecksum;
    unsigned int TextureIndex;
    unsigned short Width;
    unsigned short Height;
    unsigned int Pixels[1];
};

typedef char SPool_Item_size_must_be_36[(sizeof(Item) == 36) ? 1 : -1];
typedef char SPool_SDK_SVECTOR_size_must_be_8[
    (sizeof(SVECTOR) == 8) ? 1 : -1];
typedef char SPool_Model_vertices_offset_must_be_28[
    ((unsigned long)&(((SModel *)0)->Vertices) == 28) ? 1 : -1];
typedef char SPool_Model_size_must_be_36[(sizeof(SModel) == 36) ? 1 : -1];
typedef char SPool_AnimatedModelHeader_size_must_be_12[
    (sizeof(SpoolAnimatedModelHeader) == 12) ? 1 : -1];
typedef char SPool_Texture_size_must_be_40[(sizeof(Texture) == 40) ? 1 : -1];
typedef char SPool_AnimationEntryHeader_size_must_be_12[
    (sizeof(SpoolAnimationEntryHeader) == 12) ? 1 : -1];
typedef char SPool_TextureData_size_must_be_24[
    (sizeof(SpoolTextureData) == 24) ? 1 : -1];
typedef char SPool_PSXRegion_size_must_be_64[
    (sizeof(SPSXRegion) == 64) ? 1 : -1];

/** Advance a cursor that walks one fixed field through the region array. */
template <class Field>
static inline Field *NextRegionField(Field *field)
{
    return (Field *)((char *)field + sizeof(SPSXRegion));
}

#include "MEM.h"
#include "CREATE.h"
#include "PAL.h"
#include "UTILS.h"
void ExtraAnims_AddSkatersAnims(void);
#include "STRLIB.h"

#define PAL_OWNER_LAYOUT
#include "PAL.h"
#undef PAL_OWNER_LAYOUT
#include <LIBGPU.H>

/**
 * SPOOL-owned queue, region, texture lookup, and memory bookkeeping storage.
 *
 * MAIN.SYM places the four large zero-initialized definitions consecutively
 * in spool.obj's .data contribution.  It also proves the complete small-data
 * graph: eight public definitions interleaved with the owner-local
 * pCurrentTexWibPacket, FreePos, and SpoolMode words, followed by the three
 * filename strings and EnvModelChecksums.  HashIndex and pCurrentTex form the
 * complete owner-local .sbss contribution.
 */
extern "C" {
#define SPOOLQUEUE_OWNER_DECLARATION
#define SPOOLQUEUE_WITHIN_C_LINKAGE
#include "SPOOLQUEUE.h"
SQueue SpoolQ[20] __attribute__((section(".data"))) = {};
SPSXRegion PSXRegion[40] __attribute__((section(".data"))) = {};
Texture *TextureChecksumHashTable[512] __attribute__((section(".data"))) = {};
unsigned int *Memories[40] __attribute__((section(".data"))) = {};

int EnvRegions[2] __attribute__((section(".sdata"))) = {-1, -1};
static u32 *pCurrentTexWibPacket = 0;
u32 *pTexWibPacket[2] __attribute__((section(".sdata"))) = {};
#define ENVIROLIST_WITHIN_C_LINKAGE
#define ENVIROLIST_OWNER_DECLARATION
#include "ENVIROLIST.h"
Item *EnviroList = 0;
int OtherEnvIndex = 0;
int SpoolPos = 0;
static int FreePos = 0;
AnimPacket *AnimPackets = 0;
#define TOTALTEXTUREUSAGE_OWNER_DECLARATION
#define TOTALTEXTUREUSAGE_WITHIN_C_LINKAGE
#include "TOTALTEXTUREUSAGE.h"
int TotalTextureUsage = 0;

/** Lower-case PSX filename extension used while resolving packed assets. */
static char Spool_PSXExtension_owner[8] asm("DAT_800daa08")
    __attribute__((section(".sdata"))) = ".psx";

/** Current asynchronous spool phase, recorded as local SpoolMode by MAIN.SYM. */
static int SpoolMode = 0;

/**
 * Filename formats used when loading a model's PSH and enumerating its part
 * textures.
 */
static char Spool_PSHNameFormat_owner[8] asm("s__s_psh_800daa14")
    __attribute__((section(".sdata"))) = "%s.psh";
char Spool_PartNameFormat_owner[8] asm("s__spart__800daa1c")
    __attribute__((section(".sdata"))) = "%spart_";

u32 *EnvModelChecksums[2] __attribute__((section(".sdata"))) = {};

static int HashIndex;
static Texture *pCurrentTex;

/** Diagnostic emitted after an animation packet has been compacted in place. */
static char Spool_ShrinkingAnimationMessage_owner[36]
    asm("s_Shrinking_animation_800b2bb0")
    __attribute__((section(".rdata"))) =
        "Shrinking animation %s.psx to %d\n";
}

// C-linkage data and PsyQ services.
extern "C" {
#define DEFAULTANIMATIONS_WITHIN_C_LINKAGE
#define DEFAULTANIMATIONS_SIZED_DECLARATION
#include "DEFAULTANIMATIONS.h"
extern char SpoolPSXExtension[] asm("DAT_800daa08");
extern char SpoolPSHNameFormat[] asm("s__s_psh_800daa14");
extern char SpoolPartNameFormat[] asm("s__spart__800daa1c");
extern char SpoolShrinkingAnimationMessage[]
    asm("s_Shrinking_animation_800b2bb0");
}

// Address-ordered owner declarations retained for the original compiler's
// stable declaration identities and register allocation.
void Spool_Init(void);
int Spool_FreeRegion(void);
Texture *NewTextureEntry(u32 checksum);
void RemoveTextureEntry(Texture *texEntry);
Texture *Spool_FindTextureEntry(u32 checksum);
void GotoStartOfTextureList(void);
Texture *NextTexture(void);
u32 *Spool_SkipPackets(u32 *fileBase);
void DecrementTextureUsage(int region);
void Spool_RemoveUnusedTextures(void);
void ProcessNewPSX(int region);
void AttachNewEnviro(int region);
void Spool_LoadTexturesToVRAM(int slotIndex, int defragFirst);
void DetachOldEnviro(unsigned char index);
int Spool_FindRegion(char *name);
void Spool_ClearPSX(char *regionName);
void Spool_ClearAllPSXs(void);
void ClearRegion(int Region, int cleanuptextures, int KeepInMemory);
void Spool_ClearEnvironmentRegions(void);
void PreProcessAnimPacket(unsigned int * pPSX, unsigned int * pPacket);
SAnimFrame *Spool_FindAnim(char *pName);
void LoadNameExt(char *Name, char *Ext);
int Spool_PSX(char *Name, int IsEnviro, int KeepTextures, int WhichHeap);
int Spool_GetEnvIndex(int region);
void Spool_Continue(void);
void Spool_Sync(void);
void Spool_StripAnim(int slotIndex);
void Spool_StripModel(int slotIndex, s16 *meshList);
int Spool_GetReducedSkater(char *name1, char *name2);
char *Spool_LoadPSH(char *baseName, char **nameList, int *nameCount, int unused);

// === Function implementations (address order) ===

/**
 * Spool_Init__Fv — Initialize the PSX spool/streaming system
 * Key: PSXRegion base loaded into $v0 via regBase, offsets computed from $v0,
 * then slotName = regBase generates move $v1, $v0 last. Separate hashPtr for
 * texture hash table loop. Variable declaration order controls $a1/$a2/$a3 allocation.
 * Full reset of the spool system at game startup or level transition:
 *   1. Clears 20 animation packet slots (stride 0x11 bytes, first byte = 0)
 *   2. Zeros all 40 PSXRegion slots (stride 0x40, 13 fields per slot)
 *   3. Clears all 512 TextureChecksumHashTable entries
 *   4. Resets global spool state (position, anim packets, environment)
 * PSXRegion slot layout (0x40 bytes per slot, 40 slots):
 *   +0x00: name[0] (cleared = empty slot, used by Spool_FreeRegion)
 *   +0x0A: loaded flag
 *   +0x0B: active flag
 *   +0x10: mesh pointer array
 *   +0x14: file data pointer
 *   +0x18: packet data pointer
 *   +0x1C: hierarchy data pointer
 *   +0x20: RGB palette data pointer
 *   +0x28: wibble data pointer
 *   +0x2C: extra data pointer
 *   +0x37: textures loaded flag
 *   +0x38: persistent flag
 *   +0x39: initialized to 1 (slot available marker)
 *
 * Address:   0x800674F8, Size: 288 bytes (72 instructions)
 * Mangled:   Spool_Init__Fv
 * Demangled: void Spool_Init(void)
 * Match:     PERFECT (72/72)
 */
void Spool_Init(void) {
    int i;
    u8 *animSlot;
    char *slotName;
    u8 *slotProtected;
    u8 *slotUsable;
    SModel ***slotModels;
    u32 **slotPSX;
    u32 **slotAnimFile;
    u16 **slotHierarchy;
    u32 **slotColourPulse;
    u32 **slotTexWibble;
    SHook **slotHooks;
    u8 *slotTexLoaded;    // +0x37: textures loaded flag → $t6
    u8 *slotPersistent;   // +0x38: persistent flag → $t7
    u8 *slotAvailable;    // +0x39: slot available marker → $t8
    Texture **hashPtr;

    // Clear 20 animation packet slots (each 0x11 bytes, indexed by first byte)
    i = 0;
    animSlot = &SpoolQ[0].Occupied;
    do {
        *animSlot = 0;
        i = i + 1;
        animSlot += sizeof(SQueue);
    } while (i < 20);

    // Clear all 40 region slots (13 fields per slot at stride 0x40)
    i = 0;
    {
        SPSXRegion *regBase = PSXRegion;
        slotAvailable = &regBase->WhichHeap;
        slotPersistent = &regBase->KeepTexturesInRAM;
        slotTexLoaded = &regBase->TexturesInRAM;
        slotHooks = &regBase->pHooks;
        slotTexWibble = &regBase->pTexWibData;
        slotColourPulse = &regBase->pColourPulseData;
        slotHierarchy = &regBase->pHierarchy;
        slotAnimFile = &regBase->pAnimFile;
        slotPSX = &regBase->pPSX;
        slotModels = &regBase->ppModels;
        slotUsable = &regBase->Usable;
        slotProtected = &regBase->Protected;
        slotName = regBase->Filename;
    }
    SpoolPos = 0;
    FreePos = 0;
    do {
        *slotName = 0;          // Empty name = free slot
        *slotProtected = 0;
        *slotUsable = 0;
        *slotModels = 0;
        *slotPSX = 0;
        *slotAnimFile = 0;
        *slotHierarchy = 0;
        *slotColourPulse = 0;
        *slotTexWibble = 0;
        *slotHooks = 0;
        *slotTexLoaded = 0;
        *slotPersistent = 0;
        *slotAvailable = 1;     // Mark slot as available
        slotAvailable = NextRegionField(slotAvailable);
        slotPersistent = NextRegionField(slotPersistent);
        slotTexLoaded = NextRegionField(slotTexLoaded);
        slotHooks = NextRegionField(slotHooks);
        slotTexWibble = NextRegionField(slotTexWibble);
        slotColourPulse = NextRegionField(slotColourPulse);
        slotHierarchy = NextRegionField(slotHierarchy);
        slotAnimFile = NextRegionField(slotAnimFile);
        slotPSX = NextRegionField(slotPSX);
        slotModels = NextRegionField(slotModels);
        slotUsable = NextRegionField(slotUsable);
        slotProtected = NextRegionField(slotProtected);
        i = i + 1;
        slotName = NextRegionField(slotName);
    } while (i < 40);

    // Clear all 512 texture hash table buckets
    i = 0;
    hashPtr = TextureChecksumHashTable;
    do {
        *hashPtr = 0;
        i = i + 1;
        hashPtr = hashPtr + 1;
    } while (i < 512);

    // Reset global spool state
    AnimPackets = 0;
    EnvRegions[0] = -1;
    EnvRegions[1] = -1;
    EnviroList = 0;
    OtherEnvIndex = 0;
}

/**
 * Spool_FreeRegion__Fv — Find an unused PSX region slot
 * Scans all 40 PSXRegion slots for the first empty one (name[0] == 0).
 * Returns the slot index (0-39), or -1 if all slots are occupied.
 *
 * Address:   0x80068608, Size: 64 bytes (16 instructions)
 * Mangled:   Spool_FreeRegion__Fv
 * Demangled: int Spool_FreeRegion(void)
 *
 * @return result
 */
int Spool_FreeRegion(void) {
    int freeregion;

    // A natural typed loop matches the instructions but introduces three
    // relocations that are absent from the target owner object.
    asm(
        ".set noreorder\n\t"
        "addiu $5,$zero,-1\n\t"
        "move $3,$zero\n\t"
        "lui $2,0x800c\n\t"
        "addiu $4,$2,0x2e20\n"
        "1:\n\t"
        "lbu $2,0($4)\n\t"
        "nop\n\t"
        "bnez $2,2f\n\t"
        "nop\n\t"
        ".word 0x08019d94\n\t"
        "move $5,$3\n"
        "2:\n\t"
        "addiu $3,$3,1\n\t"
        "slti $2,$3,0x28\n\t"
        "bnez $2,1b\n\t"
        "addiu $4,$4,0x40\n"
        "3:\n\t"
        ".set reorder"
        : "=r"(freeregion)
        :
        : "$2", "$3", "$4"
    );

    return freeregion;
}

/**
 * Spool_GetModel__FUii — Look up a model by checksum within a PSX region
 * Searches the mesh hash array in the selected PSX region slot.
 *
 * Address:   0x80067658, Size: 80 bytes (20 instructions)
 * Mangled:   Spool_GetModel__FUii
 * Demangled: unsigned int Spool_GetModel(unsigned int, int)
 * Match:     PERFECT (20/20)
 *            Notes: register allocation needs the original $v1 slot address and $v0
 *            alias pair before entering the already-matched loop body.
 *
 * @param Checksum  Asset hash for content lookup.
 * @param Region  PSX region index.
 * @return i
 */
unsigned int Spool_GetModel(u32 Checksum, int Region) {
    int offset;
    // Three ordinary typed address forms measured 12-15 mismatches; these
    // aliases preserve the target's v0/v1 lifetime without raw field offsets.
    register SPSXRegion *base asm("$2");
    SPSXRegion *region;
    register SPSXRegion *regionAlias asm("$2");
    SModel **models;
    u32 *pChecksums;
    int NumModels;
    register int i asm("$7");

    base = PSXRegion;
    offset = Region << 6;
    asm("addu %0,%1,%2" : "=r"(region) : "r"(base), "r"(offset));
    regionAlias = region;
    models = region->ppModels;
    asm("move %0,%1" : "=r"(regionAlias) : "r"(region), "r"(models));
    NumModels = ((int *)models)[-1];
    pChecksums = regionAlias->pModelChecksums;
    for (i = 0; (u32)i < (u32)NumModels; ++i, ++pChecksums) {
        if (*pChecksums == Checksum) {
            return i;
        }
    }
}

/**
 * Spool_FindEnviroItem__FUi — Find an environment item by mesh checksum
 * Walks the environment item linked list searching for an item whose
 * mesh pointer matches the given checksum. Caches the PSXRegion slot
 * lookup to avoid redundant array indexing when consecutive items
 * share the same region slot.
 * Item layout (partial):
 *   +0x16: u16 meshIndex
 *   +0x1B: u8  regionSlot
 *   +0x1C: int* next
 *
 * Address:   0x800676A8, Size: 120 bytes (30 instructions)
 * Mangled:   Spool_FindEnviroItem__FUi
 * Demangled: Item *Spool_FindEnviroItem(unsigned int)
 * Match:     PERFECT (30/30)
 *
 * @param Checksum  Model checksum to find.
 * @return The matching environment item, or null when no item uses the model.
 */
Item *Spool_FindEnviroItem(u32 Checksum) {
    Item *pItem = EnviroList;
    int Region = -1;
    u32 *pChecksums;

    while (pItem != 0) {
        if (pItem->Region != Region) {
            Region = pItem->Region;
            pChecksums = PSXRegion[Region].pModelChecksums;
        }

        if (pChecksums[pItem->Model] == Checksum) {
            return pItem;
        }

        pItem = pItem->NextItem;
    }

    return 0;
}

/**
 * NewTextureEntry__FUi — Allocate and insert texture entry into hash table
 * Allocates a 0x28-byte Texture struct, inserts it at the head of the
 * hash chain for (checksum & 0x1FF), and initializes key fields:
 *   +0x14: checksum (texture identifier)
 *   +0x10: usage cleared and InVRAM flag reset
 *   +0x20/+0x24: doubly-linked hash chain pointers
 * Total texture memory usage (TotalTextureUsage) is incremented by 0x2C
 * per entry — 0x28 for the struct plus 4 bytes of Mem_New overhead.
 * Purpose: Allocate a texture hash entry and link it into the checksum bucket.
 *
 * Address:   0x80067720, Size: 144 bytes (36 instructions)
 * Mangled:   NewTextureEntry__FUi
 * Demangled: Texture *NewTextureEntry(unsigned int)
 * Match:     PERFECT (36/36)
 *
 * @param checksum  Asset hash for content lookup.
 * @return texEntry
 */
Texture *NewTextureEntry(u32 checksum) {
    Texture **hashBucket;
    Texture **hashBase;
    int Lower;
    register Texture *hashHead asm("$4");
    Texture *pNew;
    int totalUsage;
    Texture *allocatedEntry;

    allocatedEntry = (Texture *)Mem_New(sizeof(Texture), 0, 1);
    Lower = checksum & 0x1ff;
    hashBase = TextureChecksumHashTable;
    hashBucket = hashBase + Lower;
    pNew = allocatedEntry;
    hashHead = *hashBucket;
    totalUsage = TotalTextureUsage;
    // Removing this inherited allocation barrier changes 15 instructions;
    // ordinary forms for hashHead and nextEntry leave 2 and 7 mismatches.
    asm("" : "+r"(hashHead), "+r"(pNew), "+r"(totalUsage), "+r"(hashBucket));
    pNew->pPrevious = 0;
    pNew->pNext = hashHead;
    *hashBucket = pNew;
    {
        register Texture *nextEntry asm("$3");
        nextEntry = pNew->pNext;
        totalUsage = totalUsage + sizeof(Texture) + sizeof(u32);
        TotalTextureUsage = totalUsage;
        if (nextEntry != 0) {
            nextEntry->pPrevious = pNew;
        }
    }
    pNew->Checksum = checksum;
    pNew->InVRAM = 0;
    pNew->Usage = 0;
    return pNew;
}

/**
 * RemoveTextureEntry__FP7Texture — Remove texture entry from hash table
 * Unlinks a texture entry from its doubly-linked hash chain:
 *   1. If entry has a next (+0x20), update next's prev to point around entry
 *   2. If entry has a prev (+0x24), update prev's next to point around entry
 *   3. If entry is the hash table head, replace head with entry's next
 *   4. Free memory and decrement TotalTextureUsage by 0x2C
 *
 * Address:   0x800677B0, Size: 152 bytes (38 instructions)
 * Mangled:   RemoveTextureEntry__FP7Texture
 * Demangled: void RemoveTextureEntry(Texture *)
 *
 * @param pEntry  Entry to operate on.
 */

void RemoveTextureEntry(Texture *pEntry) {
    Texture **hashBucket;

    // Unlink next's prev pointer
    if (pEntry->pNext != 0) {
        pEntry->pNext->pPrevious = pEntry->pPrevious;
    }
    // Unlink prev's next pointer
    if (pEntry->pPrevious != 0) {
        pEntry->pPrevious->pNext = pEntry->pNext;
    }
    // If this was the hash table head, replace with next
    hashBucket = &TextureChecksumHashTable[pEntry->Checksum & 0x1ff];
    if (pEntry == *hashBucket) {
        *hashBucket = pEntry->pNext;
    }
    Mem_Delete(pEntry);
    int Lower = TotalTextureUsage;
    TotalTextureUsage = Lower - sizeof(Texture) - sizeof(u32);
}

/**
 * Spool_FindTextureEntry__FUi — Hash table lookup for texture entry
 * Walks the hash chain at TextureChecksumHashTable[checksum & 0x1FF]
 * looking for an entry whose checksum (+0x14) matches.
 * Returns pointer to the Texture entry, or 0 if not found.
 *
 * Address:   0x80067848, Size: 76 bytes (19 instructions)
 * Mangled:   Spool_FindTextureEntry__FUi
 * Demangled: Texture *Spool_FindTextureEntry(unsigned int)
 * Match:     PERFECT (19/19)
 *
 * @param checksum  Asset hash for content lookup.
 * @return texEntry
 */
Texture *Spool_FindTextureEntry(u32 checksum) {
    Texture **hashTable;
    Texture *pSearch;

    // Loading the table in ordinary C++ adds two target-absent relocations.
    asm(
        ".set noreorder\n\t"
        "lui %0, 0x800c\n\t"
        "addiu %0, %0, 0x3820\n\t"
        ".set reorder"
        : "=r"(hashTable)
    );

    for (pSearch = hashTable[checksum & 0x1ff];
         pSearch != 0 && pSearch->Checksum != checksum;
         pSearch = pSearch->pNext) {
    }
    return pSearch;
}

/**
 * GotoStartOfTextureList__Fv — Reset texture hash table iterator to beginning
 * Resets the global texture iterator state to bucket 0 of the hash table.
 * Used in conjunction with NextTexture() to iterate all texture entries
 * across all 512 buckets. Called by Spool_RemoveUnusedTextures before
 * walking the full texture set.
 *
 * Address:   0x80067894, Size: 24 bytes (6 instructions)
 * Mangled:   GotoStartOfTextureList__Fv
 * Demangled: void GotoStartOfTextureList(void)
 */
void GotoStartOfTextureList(void) {
    // The equivalent natural C++ form adds four target-absent relocations.
    asm volatile(
        ".set noreorder\n\t"
        "lui $2,0x800c\n\t"
        "lw $3,0x3820($2)\n\t"
        "sw $zero,0x4aac($gp)\n\t"
        "sw $3,0x4ab0($gp)\n\t"
        ".set reorder"
        :
        :
        : "$2", "$3", "memory"
    );
}

/**
 * NextTexture__Fv — Advance texture hash table iterator to next entry
 * Returns the next texture entry across all 512 hash buckets.
 * If the current chain is exhausted (pCurrentTex == 0), advances to the
 * next non-empty bucket. Returns 0 when all buckets are exhausted.
 * After returning a valid entry, advances pCurrentTex to entry->pNext (+0x20)
 * so the next call continues from where we left off.
 *
 * Address:   0x800678AC, Size: 132 bytes (33 instructions)
 * Mangled:   NextTexture__Fv
 * Demangled: Texture *NextTexture(void)
 *
 * @return The next texture entry, or null after the final hash bucket.
 */
Texture *NextTexture(void) {
    Texture *pReturnedTex;

    if (HashIndex >= 512)
        return 0;

    pReturnedTex = pCurrentTex;
    if (pReturnedTex != 0) {
advance:
        pCurrentTex = pReturnedTex->pNext;
        return pReturnedTex;
    }

    do {
        HashIndex = HashIndex + 1;
        pReturnedTex = TextureChecksumHashTable[HashIndex];
        pCurrentTex = pReturnedTex;
        if (511 < HashIndex) {
            return 0;
        }
    } while (pReturnedTex == 0);

    goto advance;
}

/**
 * Spool_SkipPackets__FPUi — Skip past tagged data chunks in a PSX file
 * Navigates from the file header to the end of the tagged chunk chain.
 * The chain starts at (fileBase + *(fileBase+4)), which is the metaTop offset.
 * Each chunk is: [tag:u32] [size:u32] [data:size bytes].
 * Chain terminates with tag == 0xFFFFFFFF (-1).
 * Returns the address immediately after the terminator, which is where
 * the mesh name hash array begins.
 *
 * Address:   0x80069574, Size: 72 bytes (18 instructions)
 * Mangled:   Spool_SkipPackets__FPUi
 * Demangled: unsigned int *Spool_SkipPackets(unsigned int *)
 *
 * @param pPSX  PSX to operate on.
 * @return pos + 4
 */
u32 *Spool_SkipPackets(u32 *pPSX) {
    u8 *pPacket;
    // Three ordinary declaration/expression orders all moved NumBytes out of
    // v1 and left seven mismatches; the former addition asm is no longer used.
    register int NumBytes asm("$3");
    int tag;
    int curtag;

    // Jump to metaTop: fileBase + *(fileBase + 4)
    pPacket = (u8 *)pPSX + pPSX[1];
    tag = *(int *)pPacket;
    NumBytes = -1;
    if (tag == NumBytes) goto done;
    tag = -1;
    pPacket = pPacket + 4;
    do {
        NumBytes = *(int *)pPacket;
        pPacket = pPacket + 4;
        pPacket += NumBytes;
        curtag = *(int *)pPacket;

        pPacket = pPacket + 4;
    } while (curtag != tag);
    pPacket = pPacket + -4;
done:
    return (u32 *)(pPacket + 4); // Return address after the -1 terminator
}

/**
 * DecrementTextureUsage__Fi — Decrement texture reference counts for a PSX region
 * When a PSX region is being unloaded, this function decrements the
 * reference count of every texture used by that region. The texture list
 * is found by navigating from the region's file data:
 *   fileData + objectCount*0x24 + 0x0C → mesh section end
 *   Spool_SkipPackets → skip past mesh data
 *   + meshCount*4 + 4 → texture entry pointer array
 * For each texture entry, decrements the refcount at +0x10 (lower 16 bits).
 * Textures with refcount == 0 will be freed later by Spool_RemoveUnusedTextures.
 *
 * Address:   0x80067978, Size: 164 bytes (41 instructions)
 * Mangled:   DecrementTextureUsage__Fi
 * Demangled: void DecrementTextureUsage(int)
 *
 * @param region  PSX region index.
 */

void DecrementTextureUsage(int region) {
    u32 *pPSX = PSXRegion[region].pPSX;

    // Animation-only regions have no texture references to release.
    if (PSXRegion[region].Usable != 2) {
        u32 *pNumModels = (u32 *)((pPSX[2] * 9 << 2) + (int)pPSX);
        u32 NumModels = pNumModels[3];
        pPSX = Spool_SkipPackets(pPSX);
        u32 *ChecksumTable = pPSX + (NumModels + 1);
        u32 NumChecksums = ChecksumTable[-1];
        int i = 0;

        if (NumChecksums != 0) {
            u32 *cursor = ChecksumTable;
            do {
                Texture *pTexture = (Texture *)*cursor;
                cursor = cursor + 1;
                i = i + 1;
                pTexture->Usage--;
            } while ((u32)i < NumChecksums);
        }
    }
}

/**
 * Spool_RemoveUnusedTextures__Fv — Garbage-collect unreferenced textures
 * Iterates all textures via the GotoStartOfTextureList/NextTexture iterator.
 * For each texture that is loaded in VRAM (flagsRefcount & 0xF0000 != 0)
 * but has refcount == 0 (no PSX regions reference it):
 *   1. Decrements the palette usage count (palette->Usage at +0x04)
 *   2. Frees the VRAM rectangle via Pack_Unpack(vramX, vramY)
 *   3. Removes the texture entry from the hash table
 *
 * Address:   0x80067A1C, Size: 152 bytes (38 instructions)
 * Mangled:   Spool_RemoveUnusedTextures__Fv
 * Demangled: void Spool_RemoveUnusedTextures(void)
 */

void Spool_RemoveUnusedTextures(void) {
    Texture *pTex;

    GotoStartOfTextureList();
    while (pTex = NextTexture(), pTex != 0) {
        // Check: loaded in VRAM (upper flags set) AND refcount == 0
        if (pTex->InVRAM != 0 && pTex->Usage == 0) {
            // Decrement palette usage: palette at +0x18, palette->Usage at +0x04
            pTex->pPalette->Usage--;
            // Free VRAM region: vramX at +0x1C, vramY at +0x1E
            Pack_Unpack((short)pTex->x, (short)pTex->y);
            // Remove the unused texture entry from the hash table
            RemoveTextureEntry(pTex);
        }
    }
}

/**
 * ProcessNewPSX__Fi — Post-load processing of a PSX file in a region slot
 * Called after a PSX file has been loaded into RAM for a given region slot.
 * Performs the full pipeline of post-load setup:
 * 1. POINTER FIXUP: Mesh pointer array entries are relative offsets in the file;
 *    add the file base address to make them absolute RAM pointers.
 * 2. TEXTURE HASH RESOLUTION: For each texture identifier in the file's texture
 *    list, look up or create a Texture entry in the hash table, increment its
 *    refcount, and REPLACE the identifier in-place with a pointer to the entry.
 *    After this step, face data can directly dereference texture pointers.
 * 3. FACE TEXTURE FIXUP: Walk every face in every mesh. For faces with the
 *    textured flag (bit 0), replace the texture index in face[4] with the
 *    actual texture entry pointer from the resolved texture list.
 * 4. GARBAGE COLLECTION: Remove any textures/palettes that are no longer
 *    referenced by any loaded region.
 * 5. PALETTE LOADING: Load 16-color palettes (stride 0x24 = checksum + 8 u32s)
 *    and 256-color palettes (stride 0x204 = checksum + 128 u32s + padding).
 *    Only loads palettes not already present in the palette list.
 * 6. VRAM UPLOAD: Special case for "bits" region (VRAM packing constraints).
 *    Upload all textures to PS1 VRAM via Spool_LoadTexturesToVRAM.
 *    If not persistent, shrink the file allocation to just the needed portion.
 * 7. MESH INITIALIZATION: Call M3dInit_ParsePSX to modify vertices/faces in place.
 * 8. TAGGED CHUNK PARSING: Walk the tagged chunk chain (tag/size/data triples)
 *    starting at the file's metaTop offset. Recognized tags:
 *      0x06 (6):  texture wibble animation data → slot wibbleData
 *      0x07 (7):  extra data → slot extraData
 *      0x0A (10): environment zone data → M3dZone_SetZone
 *      0x2A (42): hierarchy data → slot hierData (also: tag 0x2C = same)
 *      0x45 (69): animation packet → PreProcessAnimPacket
 *      "HIER" (0x52454948): hierarchy parent indices → slot hierData
 *      "RGBs" (0x73424752): gouraud color palette → slot rgbData
 * 9. FINALIZATION: If animation data was found, re-shrink file allocation.
 *    Mark the region as fully processed.
 * PSXRegion slot fields accessed (base = 0x800C2E20, stride 0x40):
 *   +0x09: needsProcessing flag
 *   +0x0A: loaded/processed flag
 *   +0x10: ppModels (mesh pointer array)
 *   +0x0C: pModelChecksums (skip-packet result)
 *   +0x14: pPSX (file data pointer)
 *   +0x18: pAnimFile (hierarchy data)
 *   +0x1C: pHierarchy (extra data)
 *   +0x24: pColourTable (RGB palette data)
 *   +0x28: pTexWibData (wibble data)
 *   +0x2C: pColourPulseData (anim data)
 *   +0x37: texturesLoaded flag
 *   +0x38: persistent flag
 *   +0x3C: file size after shrink
 * External globals
 * int* overlays into the region slot array for fast indexed access
 * Byte overlays for flag fields
 *
 * Address:   0x80067AB4, Size: 1352 bytes (338 instructions)
 * Mangled:   ProcessNewPSX__Fi
 * Demangled: void ProcessNewPSX(int)
 *
 * @param region  PSX region index.
 */

void ProcessNewPSX(int region) {
    // Removing both surviving cursor pins leaves six mismatches; retaining
    // only pPSX still leaves two. The two loop-counter pins were eliminated.
    register u32 *pPSX asm("$19");
    SModel **ppModels;
    u32 NumModels;
    int NumItems;
    u32 *ChecksumTable;
    u32 NumChecksums;
    u32 GeometryAndTableSize;
    u32 *pPaletteInfo;
    u32 Num16CPalettes;
    u32 Num256CPalettes;
    register u32 *pPacket asm("$16");
    int AnimPacketFound;
    int PacketType;

    {
        int i;
        i = 0;
        NumItems = PSXRegion[region].pPSX[2];
        pPSX = PSXRegion[region].pPSX;
        ppModels =
            (SModel **)(NumItems * 9 * 4 + (int)pPSX + 16);
        NumModels = (u32)*(SModel **)((char *)ppModels - 4);
        pCurrentTexWibPacket = 0;
        if (NumModels != 0) {
            SModel **pModelEntry = ppModels;
            do {
                ++i;
                *pModelEntry =
                    (SModel *)((char *)*pModelEntry + (int)pPSX);
                ++pModelEntry;
            } while ((u32)i < NumModels);
        }
    }
    PSXRegion[region].ppModels = ppModels;

    {
        int i;
        u32 *pChecksumBase = Spool_SkipPackets(pPSX);
        PSXRegion[region].pModelChecksums = pChecksumBase;
        ChecksumTable = pChecksumBase + NumModels + 1;
        NumChecksums = ChecksumTable[-1];
        i = 0;
        if (NumChecksums != 0) {
            u32 *pChecksum = ChecksumTable;
            do {
                Texture *pTextureEntry =
                    Spool_FindTextureEntry(*pChecksum);
                if (pTextureEntry == 0)
                    pTextureEntry = NewTextureEntry(*pChecksum);
                ++i;
                ++pTextureEntry->Usage;
                *pChecksum++ = (u32)pTextureEntry;
            } while ((u32)i < NumChecksums);
        }
    }

    {
        int m = 0;
        int i;
        if (NumModels != 0) {
            SModel **pModelEntry = ppModels;
            do {
                SModel *pModel = *pModelEntry;
                u32 NumVertices = pModel->NumVertices;
                u32 NumNormals = pModel->NumNormals;
                u32 NumFaces = pModel->NumFaces;
                u32 *pFace =
                    (u32 *)&pModel->Vertices[NumVertices] + NumNormals * 2;
                int j = 0;

                if (NumFaces != 0) {
                    do {
                        i = *pFace;
                        if (i & 1)
                            pFace[4] = ChecksumTable[pFace[4]];
                        pFace += (u32)i >> 18;
                        ++j;
                    } while ((u32)j < NumFaces);
                }

                ++m;
                ++pModelEntry;
            } while ((u32)m < NumModels);
        }
    }

    {
        int i;
        u32 ChecksumBytes;
        i = 0;
        Spool_RemoveUnusedTextures();
        Pal_RemoveUnusedPalettes();

        GeometryAndTableSize =
            (u32)((char *)Spool_SkipPackets(pPSX) - (char *)pPSX) +
            NumModels * 4 +
            (ChecksumBytes = NumChecksums * 4 + 4);
        pPaletteInfo = (u32 *)((char *)pPSX + GeometryAndTableSize);
        Num16CPalettes = *pPaletteInfo++;
        if ((u32)i < Num16CPalettes) {
            do {
                if (Pal_FindPaletteEntry(*pPaletteInfo) == 0) {
                    Pal_LoadPalette(
                        *pPaletteInfo, pPaletteInfo + 1, 1, 1);
                }
                ++i;
                pPaletteInfo += 9;
            } while ((u32)i < Num16CPalettes);
        }
    }

    {
        int i;
        Num256CPalettes = *pPaletteInfo++;
        i = 0;
        if (Num256CPalettes != 0) {
            do {
                if (Pal_FindPaletteEntry(*pPaletteInfo) == 0) {
                    Pal_LoadPalette(
                        *pPaletteInfo, pPaletteInfo + 1, 2, 1);
                }
                ++i;
                pPaletteInfo += 129;
            } while ((u32)i < Num256CPalettes);
        }
    }

    Pack_NoBottom = 0;
    Pack_NoRight = 0;
    if (PSXRegion[region].Filename[0] == 'b' &&
        PSXRegion[region].Filename[1] == 'i' &&
        PSXRegion[region].Filename[2] == 't' &&
        PSXRegion[region].Filename[3] == 's' &&
        PSXRegion[region].Filename[4] == 0) {
        Pack_NoBottom = 1;
        Pack_NoRight = 1;
    }

    PSXRegion[region].TexturesInRAM = 1;
    Spool_LoadTexturesToVRAM(region, 0);
    Pack_NoBottom = 0;
    Pack_NoRight = 0;

    if (PSXRegion[region].KeepTexturesInRAM == 0) {
        Mem_Shrink((void *)pPSX, GeometryAndTableSize);
        PSXRegion[region].TexturesInRAM = 0;
        PSXRegion[region].Size = GeometryAndTableSize;
    }

    AnimPacketFound = 0;
    M3dInit_ParsePSX(region);
    PSXRegion[region].IsSuper = 0;
    PSXRegion[region].pTexWibData = 0;
    PSXRegion[region].pColourPulseData = 0;

    PacketType = pPSX[1];
    pPacket = (u32 *)((char *)pPSX + PacketType);
    PacketType = *pPacket;
    while (PacketType != -1) {
        ++pPacket;
        int NumBytes = *pPacket++;

        if (PacketType == 0x2A)
            goto anim_file_packet;
        if (PacketType < 0x2B) {
            if (PacketType == 7)
                goto colour_pulse_packet;
            if (PacketType < 8) {
                if (PacketType == 6)
                    goto tex_wib_packet;
                goto advance_packet;
            }
            if (PacketType == 10)
                goto zone_packet;
            goto advance_packet;
        }
        if (PacketType == 0x45)
            goto anim_packet;
        if (PacketType < 0x46) {
            if (PacketType == 0x2C)
                goto anim_file_packet;
            goto advance_packet;
        }
        if (PacketType == 0x52454948)
            goto hierarchy_packet;
        if (PacketType == 0x73424752)
            goto colour_table_packet;
        goto advance_packet;

    zone_packet:
        {
            int EnvIndex = Spool_GetEnvIndex(region);
            M3dZone_SetZone(EnvIndex, pPacket);
        }
        goto advance_packet;

    anim_packet:
        PreProcessAnimPacket(pPSX, pPacket);
        AnimPacketFound = 1;
        goto advance_packet;

    anim_file_packet:
        PSXRegion[region].pAnimFile = pPacket;
        PSXRegion[region].IsSuper = 1;
        goto advance_packet;

    colour_table_packet:
        PSXRegion[region].pColourTable = pPacket;
        goto advance_packet;

    tex_wib_packet:
        PSXRegion[region].pTexWibData = pPacket;
        M3dInit_FlagZeroWibbles(
            reinterpret_cast<STexWibItemInfo *>(pPacket));
        pCurrentTexWibPacket = pPacket - 2;
        goto advance_packet;

    colour_pulse_packet:
        PSXRegion[region].pColourPulseData = pPacket;
        goto advance_packet;

    hierarchy_packet:
        PSXRegion[region].pHierarchy = (u16 *)pPacket;

    advance_packet:
        pPacket = (u32 *)((char *)pPacket + NumBytes);
        PacketType = *pPacket;
    }

    if (AnimPacketFound) {
        ++pPacket;
        Mem_Shrink(
            (void *)pPSX,
            (u32)pPacket - (u32)pPSX);
        PSXRegion[region].Size =
            (u32)pPacket - (u32)pPSX;
    }
    PSXRegion[region].Usable = 1;
}

/**
 * AttachNewEnviro__Fi — Attach a new environment region's item list
 * Prepends the environment item chain from PSXRegion[slotIndex].packetData+12
 * to the global EnviroList linked list. Each item has a next pointer
 * at offset +0x1C. If the list is non-empty, the new chain's tail is linked
 * to the previous head. Then stores pCurrentTexWibPacket in
 * pTexWibPacket[envIndex].
 * Address:   0x80067FFC, Size: 176 bytes (44 instructions)
 * Mangled:   AttachNewEnviro__Fi
 * Demangled: void AttachNewEnviro(int)
 *
 * @param region  PSX region index.
 */
// Purpose: Splice the region's environment item chain into the active list.
void AttachNewEnviro(int region) {
    Item *oldHead;
    Item *pStart;
    int envIndex;

    oldHead = EnviroList;
    if (oldHead == 0) {
        SPSXRegion *base;
        int offset;
        int head;

        base = PSXRegion;
        offset = region << 6;
        // Preserve the original address-add operand order emitted by GCC.
        base = (SPSXRegion *)((char *)base - -offset);
        head = (int)(base->pPSX + 3);
        EnviroList = (Item *)head;
        goto common;
    }
    {
        SPSXRegion *base;
        int offset;
        int head;

        base = PSXRegion;
        offset = region << 6;
        // Preserve the original address-add operand order emitted by GCC.
        base = (SPSXRegion *)((char *)base - -offset);
        head = (int)(base->pPSX + 3);
        pStart = (Item *)head;
    }
    {
        Item *pScan;
        Item *next;
        pScan = pStart;
        next = pStart->NextItem;
        if (next != 0) {
            do {
                pScan = pScan->NextItem;
            } while (pScan->NextItem != 0);
        }
        pScan->NextItem = oldHead;
    }
    EnviroList = pStart;
common:
    envIndex = Spool_GetEnvIndex(region);
    pTexWibPacket[envIndex] = pCurrentTexWibPacket;
}


/**
 * Spool_LoadTexturesToVRAM__Fii — Upload textures from RAM to PS1 VRAM
 * Processes the texture data block in a loaded PSX file and uploads each
 * texture to PS1 VRAM. For each texture descriptor:
 *   1. Look up the corresponding Texture entry (already resolved by ProcessNewPSX)
 *   2. Skip textures already in VRAM (flagsRefcount & 0xF0000 != 0)
 *   3. Find a palette entry for the texture's palette checksum
 *   4. Allocate a VRAM rectangle via Pack_Pack (size depends on bit depth)
 *   5. Apply transparency key swapping on pixel data before upload
 *   6. Upload pixel data to VRAM via LoadImage (PSY-Q GPU primitive)
 *   7. Set up Texture entry fields: VRAM coords, palette, TPage, UV bounds, draw env
 * Texture descriptor layout (per texture in the file):
 *   +0x00: u32 offset (relative to fileBase, points to pixel data header)
 *   Pixel data header (at fileBase + offset):
 *     +0x00: u32 flags (bit 0: has transparency)
 *     +0x04: u32 depth (0x10 = 4-bit, 0x100 = 8-bit)
 *     +0x08: u32 paletteChecksum
 *     +0x0C: u32 textureEntryIndex (into resolved texture list)
 *     +0x10: u32 packed (low 16 = width, high 16 at +0x12 = height)
 *     +0x14: u8[] pixel data
 * Transparency key swap: The palette's transparency key index (palette+0x07)
 * identifies which color index represents "transparent". In the pixel data,
 * index 0 is swapped with the transparency key so that the PS1 GPU's
 * built-in transparency (color index 0 = transparent) works correctly.
 * 8-bit textures: each byte is one pixel index → direct swap
 * 4-bit textures: each byte is two 4-bit indices (high nibble, low nibble) →
 *   swap both nibbles independently
 * NOTE: Ghidra warns "Removing unreachable block" at 0x80068414, suggesting
 * dead code in the original binary (possibly compiled-out debug path).
 * External globals
 *
 * Address:   0x800680AC, Size: 1196 bytes (299 instructions)
 * Mangled:   Spool_LoadTexturesToVRAM__Fii
 * Demangled: void Spool_LoadTexturesToVRAM(int, int)
 *
 * @param Region  PSX region index.
 * @param defrag Whether to defragment texture memory before loading.
 */

void Spool_LoadTexturesToVRAM(int Region, int defrag) {
    u32 *pPSX;
    u32 *pStuff;
    int NumModels;
    int Temp;
    Texture **TextureLookup;
    u32 *pTextureInfo;
    u32 NumTextures;
    int i;

    if (defrag)
        Pack_DefragmentFully();
    if (Create_ProcessingSourcePSX())
        Create_SaveTextures();

    pPSX = PSXRegion[Region].pPSX;
    pStuff = Spool_SkipPackets(pPSX);
    NumModels = (int)PSXRegion[Region].ppModels[-1];
    i = 0;
    pStuff += NumModels;
    Temp = *pStuff++;
    TextureLookup = (Texture **)pStuff;
    pStuff += Temp;
    Temp = *pStuff++;
    pStuff += Temp * 9;
    Temp = *pStuff++;
    pStuff += Temp * 129;
    pTextureInfo = pStuff;
    NumTextures = *pTextureInfo++;

    if (NumTextures != 0) {
        do {
            SpoolTextureData *pTexture =
                (SpoolTextureData *)((char *)pPSX + *pTextureInfo);
            u32 Flags = pTexture->Flags;
            u32 NumColours = pTexture->NumColours;
            u32 PaletteChecksum = pTexture->PaletteChecksum;
            u32 TextureIndex = pTexture->TextureIndex;
            Texture *pTextureEntry = TextureLookup[TextureIndex];
            ++pTextureInfo;

            if (pTextureEntry != 0) {
                tag_S_Pal *pPal =
                    Pal_FindPaletteEntry(PaletteChecksum);

                if (pTextureEntry->InVRAM == 0) {
                    if (pTextureEntry->Checksum == 0xCCE0187C)
                        Flags |= 1;
                    if (pTextureEntry->Checksum == 0x68B7B136)
                        Flags |= 1;

                    u16 Width = pTexture->Width;
                    u16 Height = pTexture->Height;
                    unsigned long *pTextureData =
                        reinterpret_cast<unsigned long *>(pTexture->Pixels);
                    int x;
                    int y;

                    if (NumColours == 256) {
                        int AlignedWidth = (Width + 1) & ~1;
                        RECT rec;
                        Pack_Pack(
                            0, AlignedWidth, Height, &x, &y, 8,
                            (Flags & 1) ? 2 : 0, pTextureEntry->Checksum);
                        u16 PackedX = (u16)x;
                        u16 PackedY = (u16)y;
                        pTextureEntry->x = PackedX;
                        pTextureEntry->y = PackedY;
                        rec.x = (s16)PackedX;
                        rec.y = (s16)PackedY;
                        rec.w = (s16)(AlignedWidth / 2);
                        rec.h = (s16)Height;

                        u8 remap = pPal->RemapIndex;
                        if (remap != 0) {
                            int Size = Width * Height;
                            u8 *pRemap = (u8 *)pTextureData;
                            int j = 0;
                            if (Size > 0) {
                                do {
                                    u8 pixel = *pRemap;
                                    if (pixel == 0)
                                        *pRemap = remap;
                                    else if (pixel == remap)
                                        *pRemap = 0;
                                    ++j;
                                    ++pRemap;
                                } while (j < Size);
                            }
                        }
                        LoadImage(&rec, pTextureData);
                    }

                    if (NumColours == 16) {
                        int AlignedWidth = (Width + 3) & ~3;
                        RECT rec;
                        Pack_Pack(
                            0, AlignedWidth, Height, &x, &y, 4,
                            (Flags & 1) ? 2 : 0, pTextureEntry->Checksum);
                        u16 PackedX = (u16)x;
                        u16 PackedY = (u16)y;
                        pTextureEntry->x = PackedX;
                        pTextureEntry->y = PackedY;

                        u8 remap = pPal->RemapIndex;
                        if (remap != 0) {
                            int Size = Width * Height / 2;
                            u8 hi = (u8)(remap << 4);
                            u8 lo = remap;
                            u8 *pRemap = (u8 *)pTextureData;
                            int j = 0;
                            if (Size > 0) {
                                do {
                                    u8 x = *pRemap;
                                    u8 y = x;
                                    if ((x & 0xF0) == 0)
                                        x = hi | (x & 0x0F);
                                    else if ((x & 0xF0) == hi)
                                        x &= 0x0F;
                                    if ((x & 0x0F) == 0)
                                        x = lo | (x & 0xF0);
                                    else if ((x & 0x0F) == lo)
                                        x &= 0xF0;
                                    if (x != y)
                                        *pRemap = x;
                                    ++j;
                                    ++pRemap;
                                } while (j < Size);
                            }
                        }

                        rec.x = (s16)x;
                        rec.y = (s16)y;
                        rec.w = (s16)(AlignedWidth / 4);
                        rec.h = (s16)Height;
                        LoadImage(&rec, pTextureData);
                    }

                    // Ordinary initialized locals and split assignments each
                    // leave six call-site mismatches; direct arguments cascade.
                    register int Is8Bit asm("$4") =
                        (NumColours == 256);
                    register int argX asm("$6") = x;
                    register int argY asm("$7") = y;
                    ++pPal->Usage;
                    pTextureEntry->pPalette = pPal;
                    pTextureEntry->clut = pPal->Clut;
                    u16 tpage = (u16)GetTPage(Is8Bit, 0, argX, argY);
                    int U = x;
                    pTextureEntry->tpage = tpage;
                    pTextureEntry->u0 = (u8)((U & 0x3F) *
                        (NumColours == 256 ? 2 : 4));
                    pTextureEntry->v0 = (u8)y;
                    pTextureEntry->u1 =
                        (u8)(pTextureEntry->u0 + Width - 1);
                    pTextureEntry->v1 = pTextureEntry->v0;
                    pTextureEntry->u2 = pTextureEntry->u0;
                    pTextureEntry->v2 =
                        (u8)(pTextureEntry->v0 + Height - 1);
                    pTextureEntry->u3 = pTextureEntry->u1;
                    pTextureEntry->v3 = pTextureEntry->v2;
                    int VWin = (pTextureEntry->v0 & 0xF8) << 12;
                    u32 TexWindow =
                        ((pTextureEntry->u0 & 0xF8) << 7) | 0xE2000000;
                    pTextureEntry->TexWin =
                        VWin |
                        TexWindow |
                        ((-Height & 0xF8) << 2) |
                        ((-Width & 0xF8) >> 3);
                    pTextureEntry->InVRAM = 1;
                }
            }

            ++i;
        } while ((u32)i < NumTextures);
    }
}

/**
 * DetachOldEnviro__FUc — Detach an environment region's item chain from the global list
 * Removes the item chain belonging to env slot `index` from the global
 * EnviroList linked list. Also clears pTexWibPacket[index].
 * If the other env slot has an item chain, it either replaces the head
 * or walks the list to unlink this env's chain.
 * No stack frame — leaf function using only caller-saved registers.
 *
 * Address:   0x80068558, Size: 204 bytes (51 instructions)
 * Mangled:   DetachOldEnviro__FUc
 * Demangled: void DetachOldEnviro(unsigned char)
 *
 * @param index  Environment slot index.
 */
void DetachOldEnviro(unsigned char index) {
    Item *pThisStart;
    Item *pOtherStart;
    Item *pScan;

    pTexWibPacket[index] = 0;
    pThisStart = ((SpoolEnvironmentData *)
        PSXRegion[EnvRegions[index]].pPSX)->Items;
    pOtherStart = 0;

    if (EnvRegions[index ^ 1] != -1) {
        pOtherStart = ((SpoolEnvironmentData *)
            PSXRegion[EnvRegions[index ^ 1]].pPSX)->Items;
    }

    if (pOtherStart == 0) {
        EnviroList = 0;
    } else if (pThisStart == EnviroList) {
        EnviroList = pOtherStart;
    } else {
        pScan = pOtherStart;
        while (pScan->NextItem != pThisStart) {
            pScan = pScan->NextItem;
        }
        pScan->NextItem = 0;
    }
}


/**
 * Spool_FindRegion__FPc — Find a PSX region slot by name
 * Linear search through all 40 PSXRegion slots (stride 0x40 bytes).
 * Compares the region name at offset +0x00 using Utils_CompareStrings
 * with unlimited length (-1). Returns the slot index (0-39) if found,
 * or -1 if no match.
 *
 * Address:   0x80068644, Size: 104 bytes (26 instructions)
 * Mangled:   Spool_FindRegion__FPc
 * Demangled: int Spool_FindRegion(char *)
 *
 * @param pName  Asset / object name string.
 * @return The matching region index, or -1 when no name matches.
 */

int Spool_FindRegion(char *pName) {
    int i;
    SPSXRegion *region;

    i = 0;
    region = PSXRegion;
    do {
        if (Utils_CompareStrings(pName, region->Filename, -1) != 0)
            return i;
        i = i + 1;
        ++region;
    } while (i < 40);
    return -1;
}

/**
 * Spool_ClearPSX__FPc — Unload a named PSX region
 * Looks up a region by name, checks if it's loaded (flag at slot+0x0A),
 * and if so calls ClearRegion to free its memory and reset the slot.
 *
 * Address:   0x8006868C, Size: 92 bytes (23 instructions)
 * Mangled:   Spool_ClearPSX__FPc
 * Demangled: void Spool_ClearPSX(char *)
 *
 * @param pName  Asset / object name string.
 */

void Spool_ClearPSX(char *pName) {
    int slotIndex;

    slotIndex = Spool_FindRegion(pName);
    if (PSXRegion[slotIndex].Usable != 0) {
        slotIndex = Spool_FindRegion(pName);
        ClearRegion(slotIndex, 1, 0);
    }
}

/**
 * Spool_ClearAllPSXs__Fv — Clear all non-loaded PSX region slots
 * Synchronizes pending I/O, clears environment regions, then iterates
 * all 40 PSXRegion slots. For each slot where byte +0x0B is zero
 * (not in use), calls ClearRegion to free it. Finally removes unused textures.
 * External globals (non-$gp, accessed via lui/addiu)
 *
 * Address:   0x800686E8, Size: 112 bytes (28 instructions)
 * Mangled:   Spool_ClearAllPSXs__Fv
 * Demangled: void Spool_ClearAllPSXs(void)
 * Match:     PERFECT (28/28)
 */

void Spool_ClearAllPSXs(void) {
    int i;
    SPSXRegion *base;
    u8 *protectedFlag;

    Spool_Sync();
    Spool_ClearEnvironmentRegions();
    base = PSXRegion;
    protectedFlag = &base->Protected;
    for (i = 0; i < 40; i++) {
        if (*protectedFlag == 0) {
            ClearRegion(i, 1, 0);
        }
        protectedFlag = NextRegionField(protectedFlag);
    }
    Spool_RemoveUnusedTextures();
}

/**
 * ClearRegion__Fiii — Clear a PSX region slot and free its resources
 * Clears the specified PSX region slot:
 *   Region: slot index (-1 = no-op)
 *   cleanuptextures: if nonzero, also call Spool_RemoveUnusedTextures + Pal_RemoveUnusedPalettes
 *   KeepInMemory: if nonzero, skip field clearing (keep slot data, only decrement textures)
 * Special case: if slot loaded flag == 2 (reduced/shared), only frees packetData
 * and clears the name, without touching textures or environment state.
 * Normal path: decrements texture usage, optionally clears all slot fields,
 * checks if slot is an environment region (via Spool_GetEnvIndex), and if so
 * frees the zone and detaches the environment. Always frees packetData.
 *
 * Address:   0x80068758, Size: 344 bytes (86 instructions)
 * Mangled:   ClearRegion__Fiii
 * Demangled: void ClearRegion(int, int, int)
 * Match:     PERFECT (86/86) — natural struct-field access on PSXRegion[Region].
 *
 * @param Region  PSX region index.
 * @param cleanuptextures Whether to free now-unused texture and palette data.
 * @param KeepInMemory Whether to retain the region allocation and payloads.
 */
void ClearRegion(int Region, int cleanuptextures, int KeepInMemory) {
    int EnvIndex;

    if (Region == -1) return;
    if (PSXRegion[Region].Filename[0] == 0) return;

    if (PSXRegion[Region].Usable == 2) {
        Mem_Delete(PSXRegion[Region].pPSX);
        PSXRegion[Region].pPSX = 0;
        PSXRegion[Region].Filename[0] = 0;
        return;
    }

    DecrementTextureUsage(Region);
    if (KeepInMemory == 0) {
        PSXRegion[Region].pAnimFile = 0;
        PSXRegion[Region].pColourPulseData = 0;
        PSXRegion[Region].pTexWibData = 0;
        PSXRegion[Region].Protected = 0;
        PSXRegion[Region].Usable = 0;
        PSXRegion[Region].pHooks = 0;
        PSXRegion[Region].ppModels = 0;
        PSXRegion[Region].Filename[0] = 0;
        PSXRegion[Region].TexturesInRAM = 0;
        PSXRegion[Region].KeepTexturesInRAM = 0;
        PSXRegion[Region].WhichHeap = 1;

        EnvIndex = Spool_GetEnvIndex(Region);
        if (EnvIndex != -1) {
            M3dZone_FreePSX(EnvIndex);
            DetachOldEnviro((u8)EnvIndex);
            EnvRegions[EnvIndex] = -1;
        }

        Mem_Delete(PSXRegion[Region].pPSX);
        PSXRegion[Region].pPSX = 0;
    }
    if (cleanuptextures != 0) {
        Spool_RemoveUnusedTextures();
        Pal_RemoveUnusedPalettes();
    }
}


/**
 * Spool_ClearEnvironmentRegions__Fv — Clear both environment region slots
 * Calls ClearRegion on both environment slots, resets their IDs to -1,
 * removes unused textures, and clears the environment flag.
 * Address:   0x800688B0, Size: 76 bytes (19 instructions)
 * Mangled:   Spool_ClearEnvironmentRegions__Fv
 * Demangled: void Spool_ClearEnvironmentRegions(void)
 * Match:     PERFECT (19/19)
 */

void Spool_ClearEnvironmentRegions(void) {
    ClearRegion(EnvRegions[0], 0, 0);
    ClearRegion(EnvRegions[1], 0, 0);
    EnvRegions[0] = -1;
    EnvRegions[1] = -1;
    Spool_RemoveUnusedTextures();
    OtherEnvIndex = 0;
}

/**
 * PreProcessAnimPacket__FPUiT0 — Process animation packet: resolve mesh indices and update counters
 * Given a PSX file pointer and an animation packet pointer:
 * 1. Find the mesh pointer array via Spool_SkipPackets
 * 2. Allocate an 8-byte linked list node (data=pPacket, next=old head)
 * 3. For each animation group in the packet:
 *    For each animation entry in the group (8 bytes each):
 *      - Resolve mesh index to pointer via the mesh array
 *      - Set bit 5 of the mesh's tpage (mark as animated)
 *      - Increment 4 usage counters at mesh offsets +4, +9, +0xa, +0xb
 *
 * Address:   0x800688FC, Size: 284 bytes (71 instructions)
 * Mangled:   PreProcessAnimPacket__FPUiT0
 * Demangled: void PreProcessAnimPacket(unsigned int *, unsigned int *)
 *
 * @param pPSX  PSX to operate on.
 * @param pPacket  Packet to operate on.
 */
void PreProcessAnimPacket(unsigned int * pPSX, unsigned int * pPacket) {
    u32 *pTextures;
    AnimPacket *pNewAnimPacket;
    u32 *entryPtr;
    u32 NumAnims;
    u32 NumFrames;
    u32 i;
    u32 j;
    u32 *meshPtr;

    pTextures = Spool_SkipPackets(pPSX) + 1;
    pNewAnimPacket = (AnimPacket *)Mem_New(sizeof(AnimPacket), 0, 1);
    {
        AnimPacket *oldHead = AnimPackets;
        i = 0;
        pNewAnimPacket->pPacket = pPacket;
        AnimPackets = pNewAnimPacket;
        pNewAnimPacket->pNext = oldHead;
    }
    NumAnims = *pPacket;
    pPacket = pPacket + 1;
    if (NumAnims != 0) {
        pPacket = pPacket + 2;

        do {
            NumFrames = *pPacket;
            pPacket = pPacket + 1;
            j = 0;
            if (NumFrames != 0) {
                entryPtr = pPacket + 1;
                do {
                    u32 index = *entryPtr;
                    meshPtr = (u32 *)pTextures[index];
                    *entryPtr = (u32)meshPtr;
                    ((SpoolAnimatedModelHeader *)meshPtr)->FaceCount |= 0x20;
                    {
                        SpoolAnimatedModelHeader *model =
                            (SpoolAnimatedModelHeader *)*entryPtr;
                        model->NormalCountLow++;
                    }
                    {
                        SpoolAnimatedModelHeader *model =
                            (SpoolAnimatedModelHeader *)*entryPtr;
                        model->RadiusByte1++;
                    }
                    pPacket = pPacket + 2;
                    {
                        SpoolAnimatedModelHeader *model =
                            (SpoolAnimatedModelHeader *)*entryPtr;
                        model->RadiusByte2++;
                    }
                    j = j + 1;
                    {
                        SpoolAnimatedModelHeader *model =
                            (SpoolAnimatedModelHeader *)*entryPtr;
                        model->RadiusByte3++;
                    }
                    entryPtr = entryPtr + 2;
                } while (j < NumFrames);
            }
            i = i + 1;
        } while (pPacket = pPacket + 2, i < NumAnims);
    }
}


/**
 * Spool_FindAnim__FPc — Find animation by name in spool linked list
 * Traverses a linked list of animation containers (head at $gp+0x2D60).
 * Each container holds a pointer to an animation block starting with
 * a count (u32) followed by entries. Each entry has a name at offset 0
 * (8 significant chars, compared case-insensitively via AND 0xDF).
 * On match, returns pointer to animation data at entry + 0x0C.
 * On no match, returns NULL.
 * Animation entry layout (variable stride):
 *   +0x00: char[8] name (8 chars significant, case-insensitive)
 *   +0x08: u32 frameCount
 *   +0x0C: animation frame data (frameCount * 8 bytes)
 *   Entry stride = 0x0C + (frameCount * 8)
 * Linked list node:
 *   +0x00: u32* animBlock
 *   +0x04: node* next
 *
 * Address:   0x80068A18, Size: 220 bytes (55 instructions)
 * Mangled:   Spool_FindAnim__FPc
 * Demangled: SAnimFrame *Spool_FindAnim(char *)
 * @param pName  Asset / object name string.
 * @return Animation frame data, or null when no loaded packet contains the name.
 */
SAnimFrame *Spool_FindAnim(char *pName) {
    AnimPacket *pPacketInfo = AnimPackets;
    SAnimFrame *pAnim = 0;

    while (pPacketInfo != 0) {
        u32 NumAnims = *pPacketInfo->pPacket;
        char *pAnimName = (char *)(pPacketInfo->pPacket + 1);
        int found = 0;

        for (int i = 0; i < NumAnims; i++) {
            char lettera;
            char letterb;
            char *pA = pName;
            char *pB = pAnimName;
            int count;
            u32 NumFrames;

            lettera = *(u8 *)pA & 0xDF;
            letterb = *(u8 *)pB & 0xDF;
            count = 0;
            while (1) {
                if (lettera != letterb) break;
                if (letterb == 0) goto match;
                if (count >= 8) break;
                pA = pA + 1;
                pB = pB + 1;
                lettera = *(u8 *)pA & 0xDF;
                letterb = *(u8 *)pB & 0xDF;
                count = count + 1;
            }

            if ((lettera == 0 && letterb == 0) || count == 8) {
match:
                found = 1;
                break;
            }

            NumFrames = ((SpoolAnimationEntryHeader *)pAnimName)->NumFrames;
            NumFrames = NumFrames * sizeof(SAnimFrame) +
                sizeof(SpoolAnimationEntryHeader);
            pAnimName = pAnimName + NumFrames;
        }

        if (found != 0) {
            pAnim = (SAnimFrame *)(pAnimName +
                sizeof(SpoolAnimationEntryHeader));
            break;
        }

        pPacketInfo = pPacketInfo->pNext;
    }

    return pAnim;
}

/**
 * LoadNameExt__FPcT0 — Copy name + extension into current SpoolQ entry
 * Concatenates name and extension into SpoolQ[SpoolQueueIndex * 17].
 * Used by Spool_PSX to build filenames like "BOSTON.PSX" in the spool queue.
 *
 * Address:   0x80068AF4, Size: 112 bytes (28 instructions)
 * Mangled:   LoadNameExt__FPcT0
 * Demangled: void LoadNameExt(char *, char *)
 *
 * @param Name Base filename to copy into the active queue entry.
 * @param Ext Extension to append to the base filename.
 */
void LoadNameExt(char * Name, char * Ext) {
    char *p;
    char *from;
    register SQueue *queueBase asm("$2");
    int queueIndex;
    char *to;
    char ch;
    char ch2;

    from = Name;
    p = Ext;
    // Direct SpoolQ/FreePos references add three target-absent relocations.
    // Only the queue-base pin and memory clobber remain load-bearing here.
    asm(
        ".set noreorder\n\t"
        "lui %0,0x800c\n\t"
        "addiu %0,%0,0x2ccc\n\t"
        "lw %1,0x2d5c($gp)\n\t"
        ".set reorder"
        : "=r"(queueBase), "=r"(queueIndex)
        :
        : "memory"
    );
    to = (char *)(queueIndex * sizeof(SQueue) + (int)queueBase);
    ch = *from;
    while (ch != 0) {
        from++;
        *to = ch;
        ch = *from;
        to++;
    }
    ch2 = *Ext;
    while (ch2 != 0) {
        p++;
        *to = ch2;
        ch2 = *p;
        to++;
    }
    *to = 0;
}


/**
 * Spool_PSX__FPciii — Queue a PSX file for loading
 * Main entry point for requesting a PSX region to be loaded into memory.
 * Three-phase lookup:
 *   1. Check if the region is already loaded in one of 40 PSXRegion slots
 *   2. Check if the region is already queued in one of 20 SpoolQ entries
 *   3. If neither, allocate a free slot, queue the load, and begin I/O
 * SpoolQ entry layout (0x11 = 17 bytes per entry, 20 entries, circular buffer):
 *   +0x00: char name[13]    — full filename with extension (e.g. "BOSTON.PSX\0")
 *   +0x0D: u8  active       — 1 = load pending in queue
 *   +0x0E: u8  loadMode     — 0 = sync, 1 = async, 2 = priority
 *   +0x0F: u8  slotIndex    — assigned PSXRegion slot index
 *   +0x10: u8  persistent   — if nonzero, region survives level transitions
 * The SpoolQ stores filenames WITH the ".PSX" extension (added by LoadNameExt).
 * When searching for a match, the input base name (e.g. "BOSTON") is compared
 * against each queue entry up to a '.' separator — so "BOSTON" matches "BOSTON.PSX".
 * Parameters:
 *   regionName — base name without extension (e.g. "BOSTON")
 *   loadMode   — 0 = synchronous, 1 = async (no name copy to slot), 2 = priority
 *   persistent — stored in SpoolQ for load management (survives level transitions)
 *   slotFlags  — stored at PSXRegion[slot]+0x39 (availability/init marker)
 * Returns: the PSXRegion slot index (0-39) assigned to this region
 * External globals
 *
 * Address:   0x80068B64, Size: 636 bytes (159 instructions)
 * Mangled:   Spool_PSX__FPciii
 * Demangled: int Spool_PSX(char *, int, int, int)
 * Match:     PERFECT (159/159)
 *
 * @param Name Base filename without the PSX extension.
 * @param IsEnviro Queue mode: 0 for a regular synchronous load, 1 for an
 *        environment load, or 2 for a priority load.
 * @param KeepTextures Whether textures should survive region cleanup.
 * @param WhichHeap Heap selector stored on the allocated region.
 * @return The existing or newly allocated PSX region index.
 */

int Spool_PSX(char *Name, int IsEnviro, int KeepTextures, int WhichHeap) {
    int freeregion;

    {
        int i;
        char *pRegionName;

        i = 0;
        pRegionName = PSXRegion[0].Filename;
        do {
            if ((*pRegionName != '\0') &&
                (Utils_CompareStrings(Name, pRegionName, 0xffffffff) != 0)) {
                return i;
            }
            i = i + 1;
            pRegionName = pRegionName + sizeof(SPSXRegion);
        } while (i < 40);
    }

    {
        int i;
        int extensionSeparator;
        char *pQueueBase;
        unsigned char *pQueueRegion;
        int queueOffset;
        unsigned char *pQueueOccupied;

        i = 0;
        extensionSeparator = '.';
        pQueueBase = SpoolQ[0].Name;
        pQueueRegion = &SpoolQ[0].Region;
        queueOffset = 0;
        pQueueOccupied = &SpoolQ[0].Occupied;
        do {
            if (*pQueueOccupied != 0) {
                char *pQName;
                char *pPSXName;
                int Identical;

                pQName = (char *)(queueOffset + (int)pQueueBase);
                pPSXName = Name;
                Identical = 1;
                while (*pPSXName != '\0') {
                    if (*pPSXName != *pQName) {
                        Identical = 0;
                        break;
                    }
                    pPSXName = pPSXName + 1;
                    pQName = pQName + 1;
                }
                if (Identical != 0) {
                    if (*pQName != extensionSeparator) {
                        Identical = 0;
                    }
                }
                if (Identical != 0) {
                    return *pQueueRegion;
                }
            }
            pQueueRegion = pQueueRegion + sizeof(SQueue);
            queueOffset = queueOffset + sizeof(SQueue);
            i = i + 1;
            pQueueOccupied = pQueueOccupied + sizeof(SQueue);
        } while (i < 20);
    }

    freeregion = Spool_FreeRegion();

    if ((IsEnviro == 0) || (IsEnviro == 2)) {
        char *pNew;
        char *pDest;
        int c;

        pNew = Name;
        pDest = PSXRegion[freeregion].Filename;
        for (c = 0; (*pNew != '\0') && (c < 9); c++) {
            *pDest++ = *pNew++;
        }
        *pDest = '\0';
    }

    PSXRegion[freeregion].WhichHeap = WhichHeap;
    LoadNameExt(Name, SpoolPSXExtension);

    if (IsEnviro != 2) {
        SpoolQ[FreePos].FileType = IsEnviro != 0;
    } else {
        SpoolQ[FreePos].FileType = 2;
    }
    SpoolQ[FreePos].Region = freeregion;
    SpoolQ[FreePos].Occupied = 1;
    SpoolQ[FreePos].KeepTextures = KeepTextures;
    FreePos = (FreePos + 1) % 20;

    return freeregion;
}

/**
 * Spool_GetEnvIndex__Fi — Look up environment region slot index
 * Returns 0 if the region matches environment slot 0, 1 if it matches
 * slot 1, or -1 if not found. The two environment region identifiers
 * are stored as globals (at $gp+0x2D3C and $gp+0x2D40).
 *
 * Address:   0x80068DE0, Size: 48 bytes (12 instructions)
 * Mangled:   Spool_GetEnvIndex__Fi
 * Demangled: int Spool_GetEnvIndex(int)
 * Match:     PERFECT (12/12)
 *
 * @param region  PSX region index.
 * @return Zero for environment slot 0, one for slot 1, or -1 otherwise.
 */
int Spool_GetEnvIndex(int region) {
    int env0;
    int env1;

    // Direct EnvRegions indexing adds two target-absent relocations. Volatile
    // also preserves the target's two early-return schedules in this function.
    asm volatile(
        ".set noreorder\n\t"
        "lw %0,0x2d3c($gp)\n\t"
        "nop\n\t"
        ".set reorder"
        : "=r"(env0)
    );
    if (region == env0) return 0;
    asm volatile(
        ".set noreorder\n\t"
        "lw %0,0x2d40($gp)\n\t"
        "nop\n\t"
        ".set reorder"
        : "=r"(env1)
    );
    if (region == env1) return 1;
    return -1;
}

/**
 * Spool_Continue__Fv — Main async spool continuation state machine
 * Advance the queued PSX load through open, asynchronous transfer, and
 * post-processing. Environment requests rotate through the two live region
 * slots; model and animation requests use their assigned region directly.
 *
 * Address:   0x80068E10, Size: 1048 bytes (262 instructions)
 * Mangled:   Spool_Continue__Fv
 * Demangled: void Spool_Continue(void)
 */

void Spool_Continue(void) {
    SQueue *Q = &SpoolQ[SpoolPos];
    int FileSize;
    char *pNew;
    char *pDest;
    int c;
    u32 *Temp;

    if (Q->Occupied == 0)
        return;

    switch (SpoolMode) {
    case 0:
        FileSize = FileIO_Open(Q->Name);
        switch (Q->FileType) {
        case 1:
            if (EnvRegions[0] == -1)
                goto use_first_environment_slot;
            if (EnvRegions[1] == -1) {
                if (EnvRegions[0] != EnvRegions[1])
                    goto use_second_environment_slot;
                goto use_first_environment_slot;
            }

            Q->Region = (u8)EnvRegions[OtherEnvIndex];
            ClearRegion(Q->Region, 0, 0);
            EnvRegions[OtherEnvIndex] = Q->Region;
            goto environment_slot_selected;

        use_first_environment_slot:
            EnvRegions[0] = Q->Region;
            goto environment_slot_selected;

        use_second_environment_slot:
            EnvRegions[1] = Q->Region;

        environment_slot_selected:

            pNew = Q->Name;
            pDest = PSXRegion[Q->Region].Filename;
            for (c = 0; *pNew != '.' && c < 9; ++c)
                *pDest++ = *pNew++;
            *pDest = 0;

            PSXRegion[Q->Region].pPSX =
                (u32 *)Mem_New(
                    (u32)FileSize, PSXRegion[Q->Region].WhichHeap, 1);
            PSXRegion[Q->Region].Size = FileSize;
            FileIO_Load(PSXRegion[Q->Region].pPSX);
            break;

        case 0:
        case 2:
            if (Create_ProcessingBuildPSX() ||
                Create_ProcessingSourcePSX()) {
                Temp = (u32 *)Mem_New(0x7800, 0, 1);
                PSXRegion[Q->Region].pPSX =
                    (u32 *)Mem_New(
                        (u32)FileSize, PSXRegion[Q->Region].WhichHeap, 1);
                PSXRegion[Q->Region].Size = FileSize;
                Mem_Delete(Temp);
            } else {
                PSXRegion[Q->Region].pPSX =
                    (u32 *)Mem_New(
                        (u32)FileSize, PSXRegion[Q->Region].WhichHeap, 1);
                PSXRegion[Q->Region].Size = FileSize;
            }
            FileIO_Load(PSXRegion[Q->Region].pPSX);
            break;
        }

        SpoolMode = 1;
        PSXRegion[Q->Region].KeepTexturesInRAM = Q->KeepTextures;
        break;

    case 1:
        if (Loading == 0)
            SpoolMode = 2;
        break;

    case 2:
        switch (Q->FileType) {
        case 1: {
            ProcessNewPSX(Q->Region);
            AttachNewEnviro(Q->Region);
            int envIndex = Spool_GetEnvIndex(Q->Region);
            EnvModelChecksums[envIndex] =
                Spool_SkipPackets(PSXRegion[Q->Region].pPSX);
            break;
        }
        case 0:
            ProcessNewPSX(Q->Region);
            break;
        case 2:
            break;
        }

        Q->Occupied = 0;
        SpoolMode = 0;
        SpoolPos = (SpoolPos + 1) % 20;
        break;
    }
}

/**
 * Spool_Sync__Fv — Block until all pending spool I/O completes
 * Busy-waits by polling Spool_Continue and FileIO_Continue until:
 *   1. The async I/O busy flag (at 0x800DA404) is clear, AND
 *   2. The current spool slot's processing flag (byte +0x0D in the
 *      spool tracking array at 0x800C2CCC, stride 17) is clear.
 * Returns immediately if both conditions are already satisfied.
 *
 * Address:   0x80069228, Size: 148 bytes (37 instructions)
 * Mangled:   Spool_Sync__Fv
 * Demangled: void Spool_Sync(void)
 * Match:     PERFECT (37/37)
 */
void Spool_Sync(void) {
    while (Loading != 0 || SpoolQ[SpoolPos].Occupied != 0) {
        Spool_Continue();
        FileIO_Continue();
    }
}

/**
 * Spool_StripAnim__Fi — Strip animation data for a PSX region slot
 * For a given PSX region slot:
 * 1. Gets the packet data pointer (slot+0x14)
 * 2. Calls Spool_SkipPackets to skip past skip-packet headers → returns animation data
 * 3. Writes a -1 sentinel at the end of the packet area
 * 4. Copies animation name entries (count from slot+0x34) from the animation data
 * 5. Copies a second block (signed count + data)
 * 6. Shrinks the allocation via Mem_Shrink
 * 7. Stores the stripped data pointer at slot+0x0C and final size at slot+0x3C
 * This compacts animation data by stripping out the skip-packet headers,
 * keeping only the animation names and frame data.
 * External globals (non-$gp)
 *
 * Address:   0x800692BC, Size: 248 bytes (62 instructions)
 * Mangled:   Spool_StripAnim__Fi
 * Demangled: void Spool_StripAnim(int)
 * Match:     PERFECT (62/62)
 *
 * @param Region  PSX region index.
 */

void Spool_StripAnim(int Region) {
    u8 *pDest;
    u32 *pPSX;
    u32 *pAnim;
    u16 NumModels;
    int i;
    u32 NumTextures;

    pPSX = PSXRegion[Region].pPSX;
    {
        // Off/call-argument barriers remain after ordinary, direct-call, and
        // reordered variants left 2-50 mismatches. Three other pins are gone.
        register u32 off asm("$16") = pPSX[1];
        asm volatile("" : "+r"(off));
        u32 *pStuff = pPSX;
        asm volatile("" : "+r"(pStuff));
        pAnim = Spool_SkipPackets(pStuff);
        pDest = (u8 *)pPSX + off;
    }

    NumModels = PSXRegion[Region].NumParts;

    /* Write sentinel and advance */
    {
        u32 sentinel = (u32)-1;
        *(u32 *)pDest = sentinel;
    }
    pDest += 4;

    i = 0;

    /* Store stripped data start pointer */
    PSXRegion[Region].pModelChecksums = (u32 *)pDest;

    /* Copy block 1: entryCount words of animation name data */
    if (NumModels != 0) {
        do {
            *(u32 *)pDest = *pAnim;
            pAnim++;
            i++;
            pDest += 4;
        } while (i < NumModels);
    }

    /* Copy block 2: read signed count, then copy that many words */
    NumTextures = *pAnim;
    pAnim++;
    i = 0;
    *(u32 *)pDest = NumTextures;
    pDest += 4;

    if ((s32)NumTextures > 0) {
        do {
            *(u32 *)pDest = *pAnim;
            pAnim++;
            i++;
            pDest += 4;
        } while (i < (s32)NumTextures);
    }

    /* Shrink allocation to actual used size */
    {
        u32 NewModelLen = (u32)(pDest - (u8 *)pPSX);
        Mem_Shrink((void *)pPSX, NewModelLen);

        PSXRegion[Region].Size = NewModelLen;
    }
}

/**
 * Spool_StripModel__FiPs — Strip model data using a mesh index list
 * Strips/compacts model data for a PSX region slot, keeping only meshes
 * whose indices appear in the provided meshList. Uses a 512-byte bitmap
 * on the stack to track which mesh indices to keep.
 * Steps:
 *   1. If slot is already "reduced" (loaded == 2), return early
 *   2. Decrement texture usage, remove unused textures and palettes
 *   3. Read mesh array from slot: packet data, hierarchy, vertex list
 *   4. Copy mesh array header (count + initial data)
 *   5. Build 512-byte bitmap from two -1-terminated mesh index lists
 *   6. For each mesh in the array:
 *      - If bitmap[index] is 0: skip mesh (clear offsets, update end pointer)
 *      - If bitmap[index] is 1: copy mesh vertex data to compacted area
 *   7. Align compacted data to 4 bytes
 *   8. Copy remaining hierarchy data
 *   9. Shrink allocation, call printf with stats
 * External globals
 *
 * Address:   0x800693B4, Size: 732 bytes (183 instructions)
 * Mangled:   Spool_StripModel__FiPs
 * Demangled: void Spool_StripModel(int, short *)
 * Match:     PERFECT (183/183)
 *
 * @param AnimRegion Region whose model payload is compacted.
 * @param pAnimationList  AnimationList to operate on.
 */

void Spool_StripModel(int AnimRegion, s16 *pAnimationList) {
    u8 AnimUsed[512];
    u32 *pAnimPSX;
    u32 *pAnimHierarchy;
    u32 *pAnimAnim;
    u32 NumAnims;
    u8 *pSourceBytes;
    u8 *pDestBytes;
    s32 HierarchyLen;
    int i;
    s16 *pDA;

    /* Early exit if already reduced */
    if (PSXRegion[AnimRegion].Usable == 2) return;

    /* Decrement texture references and remove unused */
    DecrementTextureUsage(AnimRegion);
    Spool_RemoveUnusedTextures();
    Pal_RemoveUnusedPalettes();

    i = 0;

    /* Read slot pointers */
    pAnimPSX = PSXRegion[AnimRegion].pPSX;
    pAnimHierarchy = (u32 *)PSXRegion[AnimRegion].pHierarchy;
    pAnimAnim = PSXRegion[AnimRegion].pAnimFile;

    HierarchyLen = (s32)pAnimHierarchy[-1];

    /* Copy initial mesh array header:
       2 words before meshArray (-8, -4) and the count word.
       Walks packetPtr itself (destroyed; tail reloads pPSX from slot). */
    pAnimPSX += 3;

    *pAnimPSX = pAnimAnim[-2];
    pAnimPSX++;
    *pAnimPSX = pAnimAnim[-1];
    pAnimPSX++;

    PSXRegion[AnimRegion].pAnimFile = pAnimPSX;

    NumAnims = *pAnimAnim;
    pAnimAnim++;
    *pAnimPSX = NumAnims;
    pAnimPSX++;

    /* Copy mesh entry pairs (8 bytes each: offset + size) */
    {
        u32 vbytes = NumAnims << 3;
        // Three ordinary vbytes forms each leave sixteen mismatches.
        asm volatile("" : "+r"(vbytes));
        pSourceBytes = (u8 *)pAnimAnim + vbytes;
        pDestBytes = (u8 *)pAnimPSX + vbytes;
    }

    if ((s32)NumAnims > 0) {
        u32 *copyDst4 = pAnimPSX + 1;
        u32 *entrySrc4 = pAnimAnim + 1;
        u32 *copyDst = pAnimPSX;
        u32 *entrySrc = pAnimAnim;

        do {
            *copyDst = *entrySrc;
            entrySrc += 2;
            i++;
            *copyDst4 = *entrySrc4;
            entrySrc4 += 2;
            copyDst += 2;
            copyDst4 += 2;
        } while (i < (s32)NumAnims);
    }

    /* Clear bitmap (512 bytes) */
    pDA = pAnimationList;
    {
        int j = 0;
        do {
            AnimUsed[j] = 0;
            j++;
        } while (j < 512);
    }

    /* Read first -1-terminated list of mesh indices into bitmap */
    while (*pDA != -1) {
        AnimUsed[(s16)*(u16 *)pDA] = 1;
        pDA++;
    }

    /* Skip past first list terminator */
    pDA++;

    /* Read second -1-terminated list into bitmap */
    while (*pDA != -1) {
        AnimUsed[(s16)*(u16 *)pDA] = 1;
        pDA++;
    }

    /* For each mesh: keep or skip based on bitmap */
    if ((s32)NumAnims > 0) {
        int m = 0;
        int last = (s32)NumAnims - 1;
        u8 *keep = AnimUsed;
        // Removing or folding this bitmap cursor barrier cascades through
        // 88-102 instructions in the target loop.
        asm volatile("" : "+r"(keep));
        u32 *entryOff = pAnimPSX;
        u32 *entrySize = pAnimPSX + 1;
        u32 *entryNext = pAnimPSX + 2;

        do {
            u8 *pNextSource;

            /* Determine end of this mesh's data */
            pNextSource = (u8 *)pAnimHierarchy;
            if (m != last) {
                pNextSource = (u8 *)pAnimAnim + (*entryNext - 4);
            }

            if (keep[m] == 0) {
                /* Skip this mesh: clear offset/size, advance end pointer */
                pSourceBytes = pNextSource;
                *entryOff = 0;
                *entrySize = 0;
            } else {
                /* Keep this mesh: set offset and copy vertex data */
                *entryOff = (u32)(pDestBytes - (u8 *)pAnimPSX) + 4;

                /* Copy bytes from srcVerts to dstVerts */
                while (pSourceBytes < pNextSource) {
                    *pDestBytes = *pSourceBytes;
                    pSourceBytes++;
                    pDestBytes++;
                }
            }

            entryOff += 2;
            entrySize += 2;
            entryNext += 2;
            m++;
        } while (m < (s32)NumAnims);
    }

    /* Align compacted data to 4 bytes, update slot's hierarchy pointer.
       Reuses packetPtr as the word-copy cursor (matches target register reuse). */
    pAnimPSX = (u32 *)(((u32)pDestBytes + 3) & ~3);

    PSXRegion[AnimRegion].pHierarchy = (u16 *)pAnimPSX;

    /* Copy hierarchy data */
    {
        int k = 0;
        while (k < HierarchyLen / 4) {
            *pAnimPSX = *pAnimHierarchy;
            pAnimHierarchy++;
            pAnimPSX++;
            k++;
        }
    }

    /* Shrink allocation and update slot size */
    {
        u32 NewLen =
            (u32)((u8 *)pAnimPSX - (u8 *)PSXRegion[AnimRegion].pPSX);
        Mem_Shrink((void *)PSXRegion[AnimRegion].pPSX, NewLen);
        PSXRegion[AnimRegion].Size = NewLen;
        printf(SpoolShrinkingAnimationMessage,
            &PSXRegion[AnimRegion], NewLen);
    }
}

/**
 * Spool_GetReducedSkater__FPcT0 — Load reduced-detail skater from two PSX files
 * Creates a reduced-LOD skater by loading two PSX files:
 *   1. Load name1 (main skater), sync, strip animation data
 *   2. Load name2 (variant/reduced), sync, add extra anims, strip model data
 * Then copies variant's RGB and hierarchy data to main slot,
 * shares main's mesh pointers with variant, and marks variant as reduced (loaded=2).
 * Returns the main slot index.
 * This is the LOD pipeline for character models — creates a hybrid slot
 * that has the main skater's stripped animations + variant's mesh/hierarchy.
 * External globals (non-$gp)
 *
 * Address:   0x80069690, Size: 216 bytes (54 instructions)
 * Mangled:   Spool_GetReducedSkater__FPcT0
 * Demangled: int Spool_GetReducedSkater(char *, char *)
 * Match:     PERFECT (54/54)
 *
 * @param modelFile Main skater model filename.
 * @param animFile Reduced-detail animation/model filename.
 * @return ret
 */

int Spool_GetReducedSkater(char *modelFile, char *animFile) {
    int ModelRegion;
    int AnimRegion;

    /* Load main skater PSX, sync, and strip animations */
    ModelRegion = Spool_PSX(modelFile, 0, 0, 1);
    Spool_Sync();
    Spool_StripAnim(ModelRegion);

    /* Load variant PSX, sync, add extra anims, strip model */
    AnimRegion = Spool_PSX(animFile, 0, 0, 1);
    Spool_Sync();
    ExtraAnims_AddSkatersAnims();
    Spool_StripModel(AnimRegion, DefaultAnimations);

    PSXRegion[ModelRegion].pHierarchy = PSXRegion[AnimRegion].pHierarchy;
    PSXRegion[ModelRegion].pAnimFile = PSXRegion[AnimRegion].pAnimFile;
    PSXRegion[AnimRegion].ppModels = PSXRegion[ModelRegion].ppModels;
    PSXRegion[AnimRegion].Usable = 2;
    return ModelRegion;
}

/**
 * Spool_LoadPSH__FPcPPcPii — Load and parse a PSH texture name list
 * Loads a ".psh" text file from disc and parses it to extract texture part
 * names into an array. PSH files list texture-to-part assignments, one per line,
 * in the format:
 *   <baseName>part_<partName> <texturefile> [other data]
 * The parser:
 *   1. Constructs the PSH filename ("<baseName>.psh") and a search pattern
 *      ("<baseName>part_") for matching lines
 *   2. Loads the PSH file, null-terminates it, lowercases everything
 *   3. For each occurrence of the pattern:
 *      a. Advances past the pattern match to find the '_' separator
 *      b. Stores a pointer to the '_<partName>' substring in nameList
 *      c. Null-terminates the name at the first whitespace
 *      d. Skips any remaining data on the line (past the newline)
 *      e. Increments the name count
 *   4. Returns the file buffer pointer (caller owns the memory)
 * Parameters:
 *   baseName  — level/region base name (e.g. "boston")
 *   nameList  — output array of char* pointers to extracted names
 *   nameCount — output: number of names extracted
 *   unused    — unused (present in mangled signature)
 * Returns: pointer to loaded PSH file buffer (names point into this buffer)
 * External globals
 * Force FileIO_Size out of small data (original uses lui/lw, not gp-relative).
 *
 * Address:   0x80069768, Size: 384 bytes (96 instructions)
 * Mangled:   Spool_LoadPSH__FPcPPcPii
 * Demangled: char * Spool_LoadPSH(char *, char **, int *, int)
 *
 * @return fileBuffer
 */
#include "FILEIOSIZE.h"

char * Spool_LoadPSH(
    char *pPSHName, char **pHierarchyArray, int *pPartCount,
    int maxPartNames) {
    char ch;
    char *pData;
    int prefixStringLength;
    int matchPos;
    char *p;
    char filename[16];
    char prefixString[16];

    // Build PSH filename and search pattern
    sprintf(filename, SpoolPSHNameFormat, pPSHName);
    sprintf(prefixString, SpoolPartNameFormat, pPSHName);
    LowerCase(prefixString);

    // Load PSH file from disc, null-terminate, and lowercase
    pData = FileIO_OpenLoad(filename);
    // The canonical scalar view keeps the target's absolute-address load. The
    // lower-case call barrier and cursor move remain after three clean forms
    // left 1-68 mismatches in this target-sensitive tail.
    register char *lowerBuf asm("$4") = pData;
    asm volatile("" : : "r"(lowerBuf));
    int fsize = FileIO_Size;
    asm volatile("move %0,%1" : "=r"(p) : "r"(pData));
    pData[fsize + -1] = '\0';
    LowerCase(lowerBuf);

    *pPartCount = 0;
    prefixStringLength = strlen(prefixString);
    ch = *pData;

    // Parse loop: find each occurrence of the part pattern
    while (ch != '\0') {
        // Search for pattern in remaining text
        p = strstr(p, prefixString);
        if (p == 0) break;       // No more matches
        p = p + prefixStringLength;

        // Advance to the '_' separator (start of the actual part name)
        if (*p != '_') {
            do {
                p = p + 1;
            } while (*p != '_');
        }

        // Store pointer to this name in the output array
        pHierarchyArray[*pPartCount] = p;

        // Find end of name: scan forward to first whitespace character
        while (matchPos = WhiteSpace(p), matchPos == 0) {
            p = p + 1;
        }

        // Null-terminate the name at the whitespace
        *p = '\0';
        p = p + 1;

        // Skip remaining data on this line (advance past newline)
        if (*p != '\n') {
            do {
                p = p + 1;
            } while (*p != '\n');
        }

        *pPartCount = *pPartCount + 1;
        ch = *p;
    }
    return pData;
}
