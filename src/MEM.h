#ifndef MEM_INC_H
#define MEM_INC_H

#ifdef MEM_OWNER_DECLARATIONS
/** Header stored immediately before each allocator payload. */
struct SBlockHeader
{
    signed int ParentHeap : 4;
    unsigned int Size : 28;
    SBlockHeader *Next;
};

typedef char SBlockHeader_size_must_be_8[
    (sizeof(SBlockHeader) == 8) ? 1 : -1];

extern "C"
{
    extern int Mem_NumNew;
    extern int Mem_NumDelete;
    extern unsigned int HeapDefs[2][2];
    extern int LowMemory;
    extern int ReturnAddress;
    extern int Used[2];
    extern SBlockHeader *FirstFreeBlock[2];
}

void AddToFreeList(SBlockHeader *pNewFreeBlock, int Heap);
void Mem_Init(void);
void *Mem_NewX(unsigned int size, int Heap, int AssertOnFailure);
void *Mem_NewTop(unsigned int size);
void Mem_DeleteX(void *p);
void Mem_ShrinkX(void *p, unsigned int newsize);
int Mem_GetBlockSizeX(void *p);
int Mem_GetFree(char heap);
void Mem_Copy(void *dest, void *source, int bytes);
void *Mem_New(unsigned int size, int Heap, int AssertOnFailure);
void Mem_Delete(void *p);
void Mem_Shrink(void *p, unsigned int newsize);
int Mem_GetBlockSize(void *p);
#else
/** Initializes both game-memory heaps. */
void Mem_Init(void);

/** Returns the number of unallocated bytes in one heap. */
int Mem_GetFree(char heap);

/** Copies a byte range from source to destination. */
void Mem_Copy(void *dest, void *source, int bytes);

/** Allocates one game-memory block. */
void *Mem_New(unsigned int size, int heap, int assertOnFailure);

/** Releases one game-memory block. */
void Mem_Delete(void *p);

/** Shrinks one allocated game-memory block. */
void Mem_Shrink(void *p, unsigned int newsize);

/** Returns the usable size of one allocated game-memory block. */
int Mem_GetBlockSize(void *p);
#endif

#endif // MEM_INC_H
