// MEM.cpp -- Game heap allocator.

#define MEM_OWNER_DECLARATIONS
#include "MEM.h"
#undef MEM_OWNER_DECLARATIONS
#include <STDIO.H>
#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBGPU.H>

// === MEM-owned initialized small data ===
int Mem_NumNew = 0;
int Mem_NumDelete = 0;

// MAIN.SYM records this as Bang's local `bg`, but keeping its storage here is
// required to preserve MEM's original small-data order.
static RECT BangRect
    __attribute__((section(".sdata"))) = {100, 0, 50, 256};

unsigned int HeapDefs[2][2] __attribute__((section(".sdata"))) = {{0}};
int LowMemory = 0;

static unsigned int CriticalBigHeapUsage = 0;
static unsigned int UniqueIndentifier = 0;
namespace MemAddressViewOwners
{
extern "C"
{
static char DAT_800da4a4[]
    __attribute__((section(".sdata"), aligned(4))) = "Heap sizes: ";
static char DAT_800da4b4[]
    __attribute__((section(".sdata"), aligned(4))) = "\n";
}
}

int ReturnAddress = 12345;
int Used[2] __attribute__((section(".sdata"))) = {0};
SBlockHeader *FirstFreeBlock[2] __attribute__((section(".sdata"))) = {0};

/** Per-heap usage format stored in MEM's target read-only contribution. */
struct MemHeapUsageFormat {
    char text[sizeof("Heap %d: %ld bytes, ")];
};

namespace MemAddressViewOwners
{
extern "C"
{
static const MemHeapUsageFormat s_Heap__d___ld_bytes__800b28bc
    __attribute__((section(".rdata"), aligned(4))) =
        {"Heap %d: %ld bytes, "};
}
}

// Scalar views retain the allocator's original per-field relocations while
// typed pointer arithmetic below expresses the interleaved heap layout.
#define MEM_HEAP_SCALARS_DECLARATION
#include "MEM.h"
// Keep these address-only views opaque so cc1plus does not fold the owner
// definitions into literals or choose small-data addressing.
extern char DAT_800da4a4[];
extern char DAT_800da4b4[];

static void Bang(unsigned char r, unsigned char g, unsigned char b);

// === Function implementations (address order) ===

/**
 * Clears the screen to a solid color, then stops after a fatal allocator
 * error.
 *
 * Address:   0x8004E4FC, Size: 44 bytes (11 instructions)
 * Mangled:   Bang__FUcUcUc
 * Demangled: void Bang(unsigned char, unsigned char, unsigned char)
 *
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 */
static void Bang(unsigned char r, unsigned char g, unsigned char b)
{
    ClearImage(&BangRect, r, g, b);
    for (;;)
        ;
}

/**
 * Inserts a block into a heap's address-ordered free list and coalesces
 * physically adjacent blocks.
 *
 * @param pNewFreeBlock  Block to return to the free list.
 * @param Heap  Heap whose free list owns the block.
 */
void AddToFreeList(SBlockHeader *pNewFreeBlock, int Heap)
{
    SBlockHeader *pAfter = FirstFreeBlock[Heap];
    SBlockHeader *pBefore = 0;

    while (reinterpret_cast<unsigned int>(pAfter) <
           reinterpret_cast<unsigned int>(pNewFreeBlock)) {
        if (pAfter == 0) break;
        pBefore = pAfter;
        pAfter = pAfter->Next;
    }

    if (pBefore != 0) {
        if (reinterpret_cast<char *>(pBefore) +
                (pBefore->Size + sizeof(SBlockHeader)) ==
            reinterpret_cast<char *>(pNewFreeBlock)) {
            if (reinterpret_cast<char *>(pNewFreeBlock) +
                    (pNewFreeBlock->Size + sizeof(SBlockHeader)) ==
                reinterpret_cast<char *>(pAfter)) {
                pBefore->Next = pAfter->Next;
                pBefore->Size += pNewFreeBlock->Size + pAfter->Size +
                                 2 * sizeof(SBlockHeader);
            } else {
                pBefore->Size +=
                    pNewFreeBlock->Size + sizeof(SBlockHeader);
            }
            return;
        }
        pBefore->Next = pNewFreeBlock;
    } else {
        FirstFreeBlock[Heap] = pNewFreeBlock;
    }

    if (reinterpret_cast<char *>(pNewFreeBlock) +
            (pNewFreeBlock->Size + sizeof(SBlockHeader)) ==
        reinterpret_cast<char *>(pAfter)) {
        pNewFreeBlock->Next = pAfter->Next;
        pNewFreeBlock->Size += pAfter->Size + sizeof(SBlockHeader);
    } else {
        pNewFreeBlock->Next = pAfter;
    }
}

/**
 * Initializes both memory heaps.
 *
 * Address:   0x8004E650
 * Mangled:   Mem_Init__Fv
 * Demangled: Mem_Init(void)
 */
void Mem_Init(void)
{
    int i;

    printf(DAT_800da4a4);

    for (i = 0; i < 2; i++) {
        FirstFreeBlock[i] = 0;
        Used[i] = 0;

        SBlockHeader *pAllFreeMem =
            reinterpret_cast<SBlockHeader *>(HeapDefs[i][0]);
        pAllFreeMem->Size =
            HeapDefs[i][1] - HeapDefs[i][0] - sizeof(SBlockHeader);
        AddToFreeList(pAllFreeMem, i);

        printf(MemAddressViewOwners::s_Heap__d___ld_bytes__800b28bc.text,
               i, HeapDefs[i][1] - HeapDefs[i][0]);
    }

    printf(DAT_800da4b4);
    CriticalBigHeapUsage =
        (98 * (HeapDefs[1][1] - HeapDefs[1][0])) / 100;
}

/**
 * Allocates a block from the first suitable hole in the selected heap.
 * Heap zero is redirected through Mem_NewTop so its request is served from
 * the highest suitable block in the big heap. Allocated payloads are rounded
 * to four bytes and initialized with the allocator's debug fill pattern.
 *
 * Address:   0x8004E76C
 * Mangled:   Mem_NewX__FUiii
 * Demangled: Mem_NewX(unsigned int, int, int)
 *
 * @param size  Buffer size in bytes.
 * @param Heap  Heap from which to allocate.
 * @param AssertOnFailure  Whether allocation failure should stop in Bang.
 * @return Pointer to the allocated payload, or null on failure.
 */
void *Mem_NewX(unsigned int size, int Heap, int AssertOnFailure)
{
    if (Heap == 0)
    {
        return Mem_NewTop(size);
    }

    UniqueIndentifier = (UniqueIndentifier + 1) & 0x7fffffff;
    size = (size + 3) & ~3U;

    SBlockHeader *pBlock = FirstFreeBlock[Heap];
    SBlockHeader *pLast = 0;
    while (pBlock->Size < size && pBlock->Next)
    {
        pLast = pBlock;
        pBlock = pBlock->Next;
    }

    if (pBlock->Size < size)
    {
        if (AssertOnFailure)
        {
            if (Heap == 1)
            {
                Bang(128, 0, 0);
            }
            else if (Heap == 0)
            {
                Bang(0, 0, 128);
            }
        }
        return 0;
    }

    if (pBlock->Size > size + sizeof(SBlockHeader))
    {
        SBlockHeader *pLeftOver = reinterpret_cast<SBlockHeader *>(
            reinterpret_cast<char *>(pBlock + 1) + size);
        pLeftOver->Size = pBlock->Size - size - sizeof(SBlockHeader);

        if (pLast)
        {
            pLast->Next = pBlock->Next;
        }
        else
        {
            FirstFreeBlock[Heap] = pBlock->Next;
        }

        pBlock->ParentHeap = Heap;
        pBlock->Size = size;
        Used[Heap] += sizeof(SBlockHeader) + pBlock->Size;
        AddToFreeList(pLeftOver, Heap);

        {
            unsigned int *pMem =
                reinterpret_cast<unsigned int *>(pBlock + 1);
            for (int ii = 0; ii < size / sizeof(unsigned int); ++ii)
            {
                *pMem++ = 0x33333333;
            }
        }

        if (Heap == 1)
        {
            if (static_cast<unsigned int>(Used[Heap]) >=
                CriticalBigHeapUsage)
            {
                LowMemory = 1;
            }
            else
            {
                LowMemory = 0;
            }
        }

        return pBlock + 1;
    }
    else
    {
        if (pLast)
        {
            pLast->Next = pBlock->Next;
        }
        else
        {
            FirstFreeBlock[Heap] = pBlock->Next;
        }

        pBlock->ParentHeap = Heap;
        Used[Heap] += sizeof(SBlockHeader) + pBlock->Size;

        {
            unsigned int *pMem =
                reinterpret_cast<unsigned int *>(pBlock + 1);
            for (int ii = 0; ii < size / sizeof(unsigned int); ++ii)
            {
                *pMem++ = 0x33333333;
            }
        }

        if (Heap == 1)
        {
            if (static_cast<unsigned int>(Used[Heap]) >=
                CriticalBigHeapUsage)
            {
                LowMemory = 1;
            }
            else
            {
                LowMemory = 0;
            }
        }

        return pBlock + 1;
    }
}

/**
 * Allocates from the highest suitable free block in the big heap.
 * The last fitting block is selected to keep this allocation near the top;
 * when it is split, the allocated block is carved from that block's end.
 *
 * Address:   0x8004EA08
 * Mangled:   Mem_NewTop__FUi
 * Demangled: Mem_NewTop(unsigned int)
 *
 * @param size  Buffer size in bytes.
 * @return Pointer to the allocated payload, or null on failure.
 */
void *Mem_NewTop(unsigned int size)
{
    int Heap = 1;
    int AssertOnFailure = 1;
    UniqueIndentifier = (UniqueIndentifier + 1) & 0x7fffffff;
    size = (size + 3) & ~3U;

    SBlockHeader *pBlock = FirstFreeBlock[Heap];
    SBlockHeader *pLast = 0;
    SBlockHeader *pChoice = pBlock;
    SBlockHeader *pChoiceLast = 0;

    while (pBlock)
    {
        if (pBlock->Size >= size)
        {
            pChoice = pBlock;
            pChoiceLast = pLast;
        }
        pLast = pBlock;
        pBlock = pBlock->Next;
    }

    pBlock = pChoice;
    pLast = pChoiceLast;

    if (pBlock->Size < size)
    {
        if (AssertOnFailure)
        {
            if (Heap == 1)
            {
                Bang(128, 0, 0);
            }
            else if (Heap == 0)
            {
                Bang(0, 0, 128);
            }
        }
        return 0;
    }

    if (pBlock->Size > size + sizeof(SBlockHeader))
    {
        int old_size = pBlock->Size;
        SBlockHeader *pLeftOver = pBlock;
        pLeftOver->Size = old_size - size - sizeof(SBlockHeader);

        pBlock = reinterpret_cast<SBlockHeader *>(
            reinterpret_cast<char *>(pLeftOver + 1) + old_size - size -
            sizeof(SBlockHeader));
        pBlock->Size = size;
        pBlock->ParentHeap = Heap;

        Used[Heap] += sizeof(SBlockHeader) + pBlock->Size;

        if (Heap == 1)
        {
            if (static_cast<unsigned int>(Used[Heap]) >=
                CriticalBigHeapUsage)
            {
                LowMemory = 1;
            }
            else
            {
                LowMemory = 0;
            }
        }

        {
            unsigned int *pMem =
                reinterpret_cast<unsigned int *>(pBlock + 1);
            for (int ii = 0; ii < size / sizeof(unsigned int); ++ii)
            {
                *pMem++ = 0x33333333;
            }
        }

        return pBlock + 1;
    }
    else
    {
        if (pLast)
        {
            pLast->Next = pBlock->Next;
        }
        else
        {
            FirstFreeBlock[Heap] = pBlock->Next;
        }

        pBlock->ParentHeap = Heap;
        Used[Heap] += sizeof(SBlockHeader) + pBlock->Size;

        if (Heap == 1)
        {
            if (static_cast<unsigned int>(Used[Heap]) >=
                CriticalBigHeapUsage)
            {
                LowMemory = 1;
            }
            else
            {
                LowMemory = 0;
            }
        }

        {
            unsigned int *pMem =
                reinterpret_cast<unsigned int *>(pBlock + 1);
            for (int ii = 0; ii < size / sizeof(unsigned int); ++ii)
            {
                *pMem++ = 0x33333333;
            }
        }

        return pBlock + 1;
    }
}

/**
 * Returns an internal allocation to its owning heap and updates low-memory
 * tracking.
 *
 * Address:   0x8004EC68
 * Mangled:   Mem_DeleteX__FPv
 * Demangled: Mem_DeleteX(void *)
 *
 * @param p  Payload pointer immediately after the internal block header.
 */
void Mem_DeleteX(void *p)
{
    SBlockHeader *pBlock = static_cast<SBlockHeader *>(p) - 1;
    int Heap = pBlock->ParentHeap;

    if (static_cast<unsigned int>(Heap) < 2)
    {
        int *pUsed = &MemUsed + Heap;
        int used = *pUsed;
        used -= sizeof(SBlockHeader);
        used -= pBlock->Size;
        *pUsed = used;

        AddToFreeList(pBlock, Heap);

        if (Heap == 1)
        {
            if (static_cast<unsigned int>(MemTopUsed) >=
                CriticalBigHeapUsage)
            {
                LowMemory = Heap;
            }
            else
            {
                LowMemory = 0;
            }
        }
    }
}

/**
 * Mem_ShrinkX__FPvUi — Shrink an internal allocation and free the tail block.
 *
 * Address:   0x8004ECFC
 * Mangled:   Mem_ShrinkX__FPvUi
 * Demangled: Mem_ShrinkX(void *, unsigned int)
 *
 * @param p  Internal allocation pointer immediately after the block header.
 * @param newsize  New payload size in bytes.
 */
void Mem_ShrinkX(void *p, unsigned int newsize)
{
    SBlockHeader *pBlock = static_cast<SBlockHeader *>(p) - 1;
    int Heap = pBlock->ParentHeap;

    if (newsize < pBlock->Size - sizeof(SBlockHeader))
    {
        SBlockHeader *pNewFreeBlock = reinterpret_cast<SBlockHeader *>(
            static_cast<char *>(p) + newsize);
        unsigned int newBlockOffset = newsize + sizeof(SBlockHeader);
        pNewFreeBlock->Size = pBlock->Size - newBlockOffset;

        int *pUsed = &MemUsed + Heap;
        *pUsed -= pBlock->Size - newsize;
        pBlock->Size = newsize;

        AddToFreeList(pNewFreeBlock, Heap);

        if (Heap == 1)
        {
            if (static_cast<unsigned int>(MemTopUsed) >=
                CriticalBigHeapUsage)
            {
                LowMemory = Heap;
            }
            else
            {
                LowMemory = 0;
            }
        }
    }
}

/**
 * Reads the payload size stored in an allocation's internal block header.
 *
 * Address:   0x8004EDD0, Size: 12 bytes (3 instructions)
 * Mangled:   Mem_GetBlockSizeX__FPv
 * Demangled: int Mem_GetBlockSizeX(void *)
 *
 * @param p  Payload pointer immediately after the internal block header.
 * @return Payload size in bytes.
 */
int Mem_GetBlockSizeX(void *p)
{
    return (static_cast<SBlockHeader *>(p) - 1)->Size;
}

/**
 * Returns the selected heap's total span minus its allocated byte count.
 *
 * Address:   0x8004EDDC, Size: 60 bytes (15 instructions)
 * Mangled:   Mem_GetFree__Fc
 * Demangled: int Mem_GetFree(char)
 *
 * @param heap  Heap index.
 * @return Remaining bytes in the heap.
 */
int Mem_GetFree(char heap)
{
    unsigned int i = static_cast<unsigned char>(heap);
    int heapOffset = i * 2;
    unsigned int *pTop = &MemTop + heapOffset;
    unsigned int *pBottom = &MemBottom + heapOffset;
    int *pUsed = &MemUsed + i;
    return *pTop - *pBottom - *pUsed;
}

/**
 * Copies complete words first, then copies the remaining bytes.
 *
 * Address:   0x8004EE18, Size: 76 bytes (19 instructions)
 * Mangled:   Mem_Copy__FPvT0i
 * Demangled: void Mem_Copy(void *, void *, int)
 *
 * @param dest  Destination buffer.
 * @param source  Source buffer.
 * @param bytes  Number of bytes to copy.
 */
void Mem_Copy(void *dest, void *source, int bytes)
{
    int num32 = bytes >> 2;
    int num8 = bytes & 3;
    unsigned int *d1 = static_cast<unsigned int *>(dest);
    unsigned int *s1 = static_cast<unsigned int *>(source);

    while (num32 > 0)
    {
        *d1 = *s1;
        s1++;
        num32--;
        d1++;
    }

    unsigned char *d2 = reinterpret_cast<unsigned char *>(d1);
    unsigned char *s2 = reinterpret_cast<unsigned char *>(s1);

    while (num8 > 0)
    {
        *d2 = *s2;
        s2++;
        num8--;
        d2++;
    }
}

/**
 * Allocates at least two words and reserves one word before the internal
 * allocator's returned payload.
 *
 * Address:   0x8004EE64, Size: 44 bytes (11 instructions)
 * Mangled:   Mem_New__FUiii
 * Demangled: void *Mem_New(unsigned int, int, int)
 *
 * @param size  Buffer size in bytes.
 * @param Heap  Heap from which to allocate.
 * @param AssertOnFailure  Whether allocation failure should stop in Bang.
 * @return User-visible allocation pointer.
 */

void *Mem_New(unsigned int size, int Heap, int AssertOnFailure)
{
    if (size <= sizeof(unsigned int))
    {
        size = 2 * sizeof(unsigned int);
    }
    unsigned int *p =
        static_cast<unsigned int *>(Mem_NewX(size - sizeof(unsigned int),
                                             Heap, AssertOnFailure));
    p--;
    return p;
}

/**
 * Returns a public allocation to its heap after skipping its one-word prefix.
 *
 * Address:   0x8004EE90, Size: 32 bytes (8 instructions)
 * Mangled:   Mem_Delete__FPv
 * Demangled: void Mem_Delete(void *)
 *
 * @param p  User-visible allocation pointer.
 */
void Mem_Delete(void *p)
{
    Mem_DeleteX(static_cast<char *>(p) + sizeof(unsigned int));
}

/**
 * Shrinks a public allocation while preserving its one-word prefix and
 * minimum two-word allocation size.
 *
 * Address:   0x8004EEB0, Size: 48 bytes (12 instructions)
 * Mangled:   Mem_Shrink__FPvUi
 * Demangled: void Mem_Shrink(void *, unsigned int)
 *
 * @param p  User allocation pointer.
 * @param newsize  Requested user-visible allocation size.
 */
void Mem_Shrink(void *p, unsigned int newsize)
{
    if (newsize <= sizeof(unsigned int))
    {
        newsize = 2 * sizeof(unsigned int);
    }
    Mem_ShrinkX(static_cast<char *>(p) + sizeof(unsigned int),
                newsize - sizeof(unsigned int));
}

/**
 * Returns a public allocation's size, including its one-word prefix.
 *
 * Address:   0x8004EEE0, Size: 32 bytes (8 instructions)
 * Mangled:   Mem_GetBlockSize__FPv
 * Demangled: int Mem_GetBlockSize(void *)
 *
 * @param p  User-visible allocation pointer.
 * @return User-visible allocation size in bytes.
 */
int Mem_GetBlockSize(void *p)
{
    return Mem_GetBlockSizeX(static_cast<char *>(p) + sizeof(unsigned int)) +
           sizeof(unsigned int);
}
