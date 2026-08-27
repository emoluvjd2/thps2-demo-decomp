// SPOOLER.cpp -- Reconstructed translation unit
// Original: SPOOLER.CPP
// Functions: 15 recovered functions
// Address range: 0x8005A090 - 0x8005A6E8
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

struct SpoolItem {
    void *isAvailable;
    char name[16];
    int type;
    int region;
    unsigned char *pData;
};

#include "FILEIO.h"

#include "globals.h"

/** Manages the fixed 64-entry asynchronous file queue. */
struct Spooler {
    struct CClass CClass;
    struct SpoolItem mItem[64];
    int mNumItems;
    int mCurrentItem;
    int mStatus;

    static Spooler *SpoolMgr;

    Spooler();
    ~Spooler();
    int AddFile(char *, int);
    void BeginSpoolingCurrentFile();
    static void Cleanup();
    void *GetFile(int id);
    static void Init();
    int IsFileLoaded(int id);
    int IsSetDone();
    void KillFile(int slot);
    static void OrderHelper(int *array, int num, int first);
    void Reset();
    void Spooool();
    void Update();
};

typedef char SpoolItem_size_must_be_32[
    (sizeof(SpoolItem) == 32) ? 1 : -1];
typedef char Spooler_size_must_be_2064[
    (sizeof(Spooler) == 2064) ? 1 : -1];

/** Prefix of the GCC 2.x dispatch table used for deleting destruction. */
struct SpoolerDispatchTable {
    void *mReserved0;
    void *mReserved1;
    void (*Destroy)(Spooler *instance, int flags);
};

/** Return the typed dispatch prefix stored in the composed CClass base. */
static inline SpoolerDispatchTable *GetSpoolerDispatch(Spooler *instance)
{
    return (SpoolerDispatchTable *)instance->CClass.vtable;
}

/** Advance a cursor that walks one fixed field through the item array. */
template <class Field>
static inline Field *NextSpoolItemField(Field *field)
{
    return (Field *)((char *)field + sizeof(SpoolItem));
}

// Typed interfaces to the memory and PSX-spool owners.
#include "SPOOL.h"

/** Addressable typed view of the constructor symbol used by Init. */
Spooler *Spooler_DefaultConstructor(Spooler *instance)
    asm("__7Spooler");

#include "MEM.h"
#include <STDIO.H>
#include <STRINGS.H>

/** Globally active spooler instance. */
Spooler *Spooler::SpoolMgr = 0;

/** Filename and platform suffixes used by the spooler in original order. */
static char Spooler_NameFormatOwner[4] asm("DAT_800da698")
    __attribute__((section(".sdata"))) = "%s";
static char Spooler_LowerPlatformOwner[4] asm("DAT_800da69c")
    __attribute__((section(".sdata"))) = "psx";
static char Spooler_UpperPlatformOwner[4] asm("DAT_800da6a0")
    __attribute__((section(".sdata"))) = "PSX";

// Keep the call sites opaque to the literals' owner definitions above.  The
// original compiler addressed these three strings absolutely even though the
// object stores them in small data; seeing the definitions here makes GCC 2.x
// incorrectly choose gp-relative addressing.
extern "C" char Spooler_NameFormat[] asm("DAT_800da698");
extern "C" char Spooler_LowerPlatform[] asm("DAT_800da69c");
extern "C" char Spooler_UpperPlatform[] asm("DAT_800da6a0");

// Vtables
// GCC 2.x emits a private copy of `CClass::~CClass` beside a private base
// vtable in every owner that derives from CClass.  `MAIN.SYM` records 25 such
// `_._6CClass` copies and names no base vtable, so both are file-local here.
// The filesystem-safe spelling keeps the per-address target assembly reachable.
static void Spooler_ClassDestructor(CClass *instance, int flags)
    asm("__DOT__6CClass_8005A6B8");

// C++ forbids taking a destructor's address, so the public vtable slot uses an
// addressable declaration of the symbol this owner already defines.
void Spooler_DestructorAbi(Spooler *instance, int flags)
    asm("_._7Spooler");

typedef void (*SpoolerDestructorSlot)(Spooler *, int);
typedef void (*CClassDestructorSlot)(CClass *, int);

// Consecutive eight-byte-aligned small-data words per vtable rather than
// arrays, so -G8 still yields the target's `addiu $vX, $gp, off` form.
int __vt_7Spooler
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int Spooler_VTableIndex = 0;
static SpoolerDestructorSlot Spooler_VTableDtor = Spooler_DestructorAbi;

static int __vt_7Spooler_base
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int Spooler_BaseVTableIndex = 0;
static CClassDestructorSlot Spooler_BaseVTableDtor = Spooler_ClassDestructor;

// === Function implementations (address order) ===

/** Allocate and construct the globally active spooler.
 *
 * Address:   0x8005A090
 * Mangled:   Init__7Spooler
 * Demangled: Spooler::Init(void)
 */
void Spooler::Init()
{
    Spooler *instance = (Spooler *)::CClass::operator new(sizeof(Spooler));
    if (instance != 0)
    {
        instance = Spooler_DefaultConstructor(instance);
    }
    SpoolMgr = instance;
}

/**
 * Destroy the globally active spooler and clear its singleton pointer.
 *
 * The deleting-destructor flag value 3 requests complete object destruction
 * and deallocation through the owner's GCC 2.x dispatch table.
 *
 * Address:   0x8005A0C0, Size: 60 bytes (15 instructions)
 * Mangled:   Cleanup__7Spooler
 * Demangled: Spooler::Cleanup(void)
 *
 */
void Spooler::Cleanup()
{
    if (SpoolMgr != 0)
    {
        GetSpoolerDispatch(SpoolMgr)->Destroy(SpoolMgr, 3);
        SpoolMgr = 0;
    }
}

/**
 * Fill an array by alternating outward from a selected first index.
 *
 * Address:   0x8005A0FC
 * Mangled:   OrderHelper__7SpoolerPiii
 * Demangled: Spooler::OrderHelper(int *, int, int)
 *
 * @param array Destination array containing exactly `num` entries.
 * @param num Number of circular indices to emit.
 * @param first Index placed first in the output order.
 */
void Spooler::OrderHelper(int *array, int num, int first)
{
    int bottom = first;
    int top = first + 1;
    int n = 0;
    while (1)
    {
        if (top >= num)
            top = 0;
        *array = bottom;
        n++;
        array++;
        if (n == num)
            break;
        *array = top;
        n++;
        array++;
        if (n == num)
            break;
        bottom--;
        if (bottom < 0)
            bottom = num - 1;
        top++;
    }
}

/** Initialize the manager and every fixed queue entry.
 *
 * Address:   0x8005A154
 * Mangled:   __7Spooler
 * Demangled: Spooler::Spooler(void)
 */
Spooler::Spooler()
{
    int i = 0;
    int unloadedRegion = -1;
    int *region = &mItem[0].region;
    unsigned char **data = &mItem[0].pData;
    void **available = &mItem[0].isAvailable;

    CClass.vtable = &__vt_7Spooler;
    mNumItems = 0;
    mCurrentItem = 0;
    mStatus = 0;
    SpoolerBlocksFileIO = 0;

    do
    {
        *available = 0;
        *data = 0;
        *region = unloadedRegion;
        region = NextSpoolItemField(region);
        data = NextSpoolItemField(data);
        i++;
        available = NextSpoolItemField(available);
    } while (i < 64);
}

/** Reset queued resources, then restore the composed CClass dispatch table.
 *
 * Address:   0x8005A1B0
 * Mangled:   _._7Spooler
 * Demangled: Spooler::~Spooler() [deleting]
 */
Spooler::~Spooler()
{
    CClass.vtable = &__vt_7Spooler;
    this->Reset();
    CClass.vtable = &__vt_7Spooler_base;

}

/**
 * Finish an active request, release every queue entry, and reset the manager.
 *
 * Address:   0x8005A200
 * Mangled:   Reset__7Spooler
 * Demangled: Spooler::Reset(void)
 */
void Spooler::Reset()
{
    if (mStatus == 1)
    {
        int type = mItem[mCurrentItem].type;
        SpoolerBlocksFileIO = 0;
        if (type == 1)
        {
            Spool_Sync();
        }
        else
        {
            FileIO_Sync();
        }
        int current = mCurrentItem;
        SpoolerBlocksFileIO = 1;
        mItem[current].isAvailable = (void *)1;
    }

    int i = 0;
    do
    {
        this->KillFile(i);
        i = i + 1;
    } while (i < 64);

    mNumItems = 0;
    mCurrentItem = 0;
    mStatus = 0;
    SpoolerBlocksFileIO = 0;
}

/**
 * Queue a filename for later loading.
 *
 * Lower- or upper-case PSX extensions select the region spooler and are
 * stripped from its stored region name. Other files use ordinary FileIO.
 *
 * Address:   0x8005A2C4
 * Mangled:   AddFile__7SpoolerPci
 * Demangled: Spooler::AddFile(char *, int)
 *
 * @param filename  File path to queue.
 * @param ignoreExt  Nonzero to force normal FileIO loading.
 * @return Queued slot index.
 */
int Spooler::AddFile(char *filename, int ignoreExt)
{
    int n;
    sprintf(mItem[mNumItems].name, Spooler_NameFormat, filename);

    n = 0;
    if (*filename != '.')
    {
        do
        {
            n++;
        } while (filename[n] != '.');
    }

    if (ignoreExt == 0)
    {
        char *extension = filename + n + 1;
        if (strcmp(extension, Spooler_LowerPlatform) == 0 ||
            strcmp(extension, Spooler_UpperPlatform) == 0)
        {
            mItem[mNumItems].type = 1;
            mItem[mNumItems].name[n] = 0;
            goto done;
        }
    }

    mItem[mNumItems].type = 0;

done:
    return mNumItems++;
}

/**
 * Mark the queue active and begin its first pending request.
 *
 * Address:   0x8005A3E8, Size: 48 bytes (12 instructions)
 * Mangled:   Spooool__7Spooler
 * Demangled: Spooler::Spooool(void)
 */
void Spooler::Spooool()
{
    mStatus = 1;
    SpoolerBlocksFileIO = 1;
    this->BeginSpoolingCurrentFile();
}

/**
 * Report whether every queued item has been processed.
 *
 * Address:   0x8005A418, Size: 24 bytes (6 instructions)
 * Mangled:   IsSetDone__7Spooler
 * Demangled: Spooler::IsSetDone(void)
 * @return Nonzero when the current index has reached the item count.
 */
int Spooler::IsSetDone()
{
    return mCurrentItem == mNumItems;
}

/** Advance the current asynchronous request and start the next one when ready.
 *
 * Address:   0x8005A430
 * Mangled:   Update__7Spooler
 * Demangled: Spooler::Update(void)
 */
void Spooler::Update()
{
    if (mStatus == 1)
    {
        int type = mItem[mCurrentItem].type;
        SpoolerBlocksFileIO = 0;
        if (type == 1)
        {
            FileIO_Continue();
            Spool_Continue();
        }
        else
        {
            FileIO_Continue();
        }
        SpoolerBlocksFileIO = 1;
        if (Loading == 0)
        {
            mItem[mCurrentItem].isAvailable = (void *)1;
            mCurrentItem++;
            if (mCurrentItem == mNumItems)
            {
                mStatus = 0;
                SpoolerBlocksFileIO = 0;
            }
            else
            {
                this->BeginSpoolingCurrentFile();
            }
        }
    }
}

/**
 * Start the current queue entry with either PSX spooling or FileIO loading,
 * stores the resulting region/data handle, and restores FileIO blocking.
 *
 * Address:   0x8005A504
 * Mangled:   BeginSpoolingCurrentFile__7Spooler
 * Demangled: Spooler::BeginSpoolingCurrentFile(void)
 */
void Spooler::BeginSpoolingCurrentFile()
{
    int current = mCurrentItem;
    SpoolerBlocksFileIO = 0;

    if (mItem[current].type == 1)
    {
        mItem[mCurrentItem].region = Spool_PSX(
            mItem[current].name, 2, 1, 1);
    }
    else
    {
        mItem[mCurrentItem].pData = (unsigned char *)Mem_New(
            FileIO_Open(mItem[current].name), 1, 1);
        FileIO_Load(mItem[mCurrentItem].pData);
    }

    SpoolerBlocksFileIO = 1;
}

/**
 * Report whether the requested queue entry is available.
 *
 * Address:   0x8005A5D0, Size: 20 bytes (5 instructions)
 * Mangled:   IsFileLoaded__7Spooleri
 * Demangled: Spooler::IsFileLoaded(int)
 * @param id Queue entry index returned by AddFile.
 * @return Nonzero once the entry is ready to consume.
 */
int Spooler::IsFileLoaded(int id)
{
    return mItem[id].isAvailable != 0;
}

/**
 * Transfer ownership of a loaded file buffer to the caller.
 *
 * Address:   0x8005A5E4
 * Mangled:   GetFile__7Spooleri
 * Demangled: Spooler::GetFile(int)
 * @param id Queue entry index returned by AddFile.
 * @return Loaded data pointer; the queue entry no longer owns it.
 */
void *Spooler::GetFile(int id)
{
    unsigned char *pData = mItem[id].pData;
    mItem[id].pData = 0;
    mItem[id].isAvailable = 0;
    return pData;
}

/**
 * Release a queued or loaded spool entry. PSX entries are processed and cleared
 * through the spool system; normal FileIO entries free their data buffer.
 *
 * Address:   0x8005A604
 * Mangled:   KillFile__7Spooleri
 * Demangled: Spooler::KillFile(int)
 */
void Spooler::KillFile(int slot)
{
    if (mItem[slot].isAvailable != 0)
    {
        if (mItem[slot].type == 1)
        {
            ProcessNewPSX(mItem[slot].region);
            Spool_ClearPSX(mItem[slot].name);
            Spool_Sync();
            mItem[slot].region = -1;
        }
        else
        {
            Mem_Delete(mItem[slot].pData);
            mItem[slot].pData = 0;
            mItem[slot].isAvailable = 0;
        }
    }
}

/**
 * Restore the private base dispatch table and optionally free the object.
 *
 * GCC emitted this private `CClass::~CClass` copy last in the owner's text,
 * so it stays at the end of the translation unit.
 */
static void Spooler_ClassDestructor(CClass *instance, int flags)
{
    instance->vtable = &__vt_7Spooler_base;
    if (flags & 1)
    {
        ::CClass::operator delete(instance);
    }
}
