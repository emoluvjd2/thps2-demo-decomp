// PRE archive loading, ownership, and file lookup.

#include "PRE.h"
#include "PREAPI.h"
#include "MEM.h"

/** Virtual prefix used when releasing the archive manager. */
struct PreManagerVTable {
    void (*reserved0)(PREManager *manager);
    void (*reserved1)(PREManager *manager);
    void (*destroy)(PREManager *manager, int flags);
};

/** Invoke PREManager's deleting destructor through its named vtable slot. */
static inline void PreManager_Destroy(PREManager *manager) {
    PreManagerVTable *vtable = (PreManagerVTable *)manager->CClass.vtable;
    vtable->destroy(manager, 3);
}

typedef char pre_manager_size_must_be_324[
    (sizeof(PREManager) == 324) ? 1 : -1];

// String literals (absolute addresses in .rodata)
static char PRE_full_msg[]; // 0x800B290C "PRE: All slots full\n" (approx)
static char PRE_open_err[]; // 0x800B2928 "PRE: could not open %s\n" (approx)
#define PREAPI_DECLARE_OWNER_ADDRESS_VIEWS
#include "PREAPI.h"

#define PREMGR_OWNER_DECLARATION
#include "PREMGR.h"

/** PRE archive manager singleton owned by this translation unit. */
PREManager *PREMgr
    __attribute__((section(".sdata"))) = 0;

// Small-data order in this owner is PREMgr, PRE_null_arr, PRE_fmt_s, then
// both vtables, so the recovered definitions follow the string owners.
namespace PreAddressViewOwners
{
extern "C"
{
/** Empty name and filename-copy format stored beside the singleton. */
static char PRE_null_arr[4]
    __attribute__((section(".sdata"))) = {0, 0, 0, 0};
static char PRE_fmt_s[4]
    __attribute__((section(".sdata"))) = "%s";
}
}

// GCC 2.x emits a private copy of `CClass::~CClass` beside a private base
// vtable in every owner that derives from CClass.  `MAIN.SYM` records 25 such
// `_._6CClass` copies and names no base vtable, so both are file-local here.
// The filesystem-safe spelling keeps the per-address target assembly reachable.
extern "C" {
static void __DOT__6CClass_8005AC28(CClass *self, int flags);
}

// C++ forbids taking a destructor's address, so the public vtable slot uses an
// addressable declaration of the symbol this owner already defines.
struct PREManager;
void Pre_DestructorAbi(PREManager *manager, int inCharge)
    asm("_._10PREManager");

// Consecutive eight-byte-aligned small-data words per vtable rather than
// arrays, so -G8 still yields the target's `addiu $vX, $gp, off` form.
int __vt_10PREManager
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int Pre_VTableIndex __attribute__((section(".sdata"))) = 0;
static void (*Pre_VTableDtor)(PREManager *, int)
    __attribute__((section(".sdata"))) = Pre_DestructorAbi;

static int Pre_BaseVTableWord
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int Pre_BaseVTableIndex __attribute__((section(".sdata"))) = 0;
static void (*Pre_BaseVTableDtor)(CClass *, int)
    __attribute__((section(".sdata"))) = __DOT__6CClass_8005AC28;

// Additional external functions

// === Extern "C" forward declarations ===
struct PREManager;
namespace PreManagerConstructorAbi
{
/** Addressable constructor entry used by PRE_Init. */
extern "C" PREManager *__10PREManager(PREManager *manager);
}
#define PreManager_Construct PreManagerConstructorAbi::__10PREManager
#include "FILEIO.h"
#include <STDIO.H>
#include <STRINGS.H>

// === Function implementations (address order) ===

/** Allocate and publish the global PRE archive manager. */
void PRE_Init(void)
{
    PREManager *manager = (PREManager *)PREManager::operator new(sizeof(PREManager));
    if (manager != 0)
    {
        manager = PreManager_Construct(manager);
    }
    PREMgr = manager;
}

/** Destroy the global PRE archive manager and clear its handle. */
void PRE_Cleanup(void)
{
    PREManager *manager = PREMgr;
    if (manager != 0)
    {
        PreManager_Destroy(manager);
    }
    PREMgr = 0;
}

/** Initialize every archive slot and its corresponding filename. */
PREManager *PreManager_Construct(PREManager *self)
{
    self->CClass.vtable = &__vt_10PREManager;
    int i = 0;
    // A natural loop moves this address setup behind both field cursors. This
    // four-instruction slice preserves the original order and relocations,
    // replacing the former 16-instruction whole-function assembly body.
    char *emptyNamePage;
    asm("lui\t%0,%%hi(PRE_null_arr)" : "=r"(emptyNamePage));
    char *name = self->mPreName[0];
    char **slot = self->mPre;
    int more;
    do
    {
        *slot = 0;
        slot++;
        i++;
        asm volatile(
            "lbu\t$3,%%lo(PRE_null_arr)(%2)\n\t"
            "slt\t%0,%1,16\n\t"
            "sb\t$3,0(%3)"
            : "=r"(more)
            : "r"(i), "r"(emptyNamePage), "r"(name)
            : "$3", "memory");
        name += PREManager::NameLength;
    } while (more);
    return self;
}

/** Release every loaded archive owned by this manager. */
PREManager::~PREManager()
{
    CClass.vtable = &__vt_10PREManager;

    {
        int i = 0;
        char **slot = mPre;

        do
        {
            char *data = *slot;
            if (data != 0)
            {
                Mem_Delete(data);
            }
            i++;
            slot++;
        } while (i < PREManager::SlotCount);
    }

    CClass.vtable = &Pre_BaseVTableWord;
}

/** Start loading an archive, then wait for the transfer to complete. */
void PREManager::loadPRE(char *pre_name)
{
    spoolPRE(pre_name);
    FileIO_Sync();
}

/** Load an archive into the first free manager slot. */
void PREManager::spoolPRE(char *pre_name)
{
    int slot = 0;
    char **slots;
    char **p;

    slots = mPre;
    p = slots;

    // Find empty slot
loop:
    if (*p == 0)
        goto check;
    slot++;
    if (slot < PREManager::SlotCount)
    {
        p++;
        goto loop;
    }
check:

    if (slot == PREManager::SlotCount)
    {
        printf(PRE_full_msg);
        return;
    }

    // Open file and allocate buffer
    int fileSize = FileIO_Open(pre_name);
    char *buffer = (char *)Mem_New(fileSize, 0, 1);
    char **selectedSlot = slots + slot;
    *selectedSlot = buffer;
    if (buffer != 0)
    {
        FileIO_Load(buffer);
    }
    else
    {
        printf(PRE_open_err, pre_name);
    }

    // Store filename in names array
    char *nameSlot = mPreName[slot];
    sprintf(nameSlot, PRE_fmt_s, pre_name);
}

/** Register an already-loaded archive in the first free manager slot. */
void PREManager::spoolPREData(char *pDataBuffer, char *pre_name)
{
    int slot = 0;
    char **slots;
    char **p;

    slots = mPre;
    p = slots;

    // Find empty slot
loop2:
    if (*p == 0)
        goto check2;
    slot++;
    if (slot < PREManager::SlotCount)
    {
        p++;
        goto loop2;
    }
check2:

    if (slot == PREManager::SlotCount)
    {
        printf(PRE_full_msg);
        return;
    }

    // Store the data pointer directly
    char **selectedSlot = slots + slot;
    *selectedSlot = pDataBuffer;

    // Store name in names array
    char *nameSlot = mPreName[slot];
    sprintf(nameSlot, PRE_fmt_s, pre_name);
}

/** Free the named archive and clear its slot. */
void PREManager::unloadPRE(char *pre_name)
{
    int slot = 0;
    // GCC otherwise schedules this absolute-address setup after the two field
    // cursors, unlike the original compiler output.
    register char *null_base asm("$19") = PRE_null_arr;
    char **slots = mPre;
    char *names = mPreName[0];

    do
    {
        int result = strcmp(names, pre_name);
        if (result != 0)
        {
            slot++;
            goto unload_continue;
        }

        slot++;

        *names = PRE_null_arr[0];
        {
            char *data = *slots;
            if (data != 0)
            {
                Mem_Delete(data);
            }
        }
        *slots = 0;
        goto unload_done;

    unload_continue:
        slots++;
        names += PREManager::NameLength;
    } while (slot < PREManager::SlotCount);

unload_done:;
}

/** Find a file in any loaded archive and return its data and size. */
char *PREManager::getFile(char *filename, int *size)
{
    char *result = 0;
    int i = 0;
    char **slots = mPre;

    while (i < PREManager::SlotCount)
    {
        char *ptr = *slots;
        if (ptr != 0)
        {
            result = parsePRE(ptr, filename, size);
        }
        i++;
        slots++;
    }
    return result;
}

/**
 * Find a file inside one archive buffer.
 *
 * Each entry contains a padded filename, its byte length, and padded data.
 */
char *PREManager::parsePRE(char *pre, char *filename, int *size)
{
    char *ptr = pre;
    int num_files = *(int *)ptr;
    ptr += 4;
    int i = 0;

    if (num_files > 0)
    {
        int mask = -4;

        do
        {
            int offs = strlen(ptr);
            offs = (offs + 4) & mask;

            char *name = ptr;
            ptr += offs;

            int length = *(int *)ptr;
            ptr += 4;

            if (strcmp(name, filename) == 0)
            {
                *size = length;
                return ptr;
            }

            offs = (length + 3) & mask;
            ptr += offs;
            i++;
        } while (i < num_files);
    }

    return 0;
}

/** Return whether the named archive is currently loaded. */
int PREManager::inPRE(char *pre_name)
{
    int slot = 0;
    char **slots = mPre;
    char *names = mPreName[0];

    do
    {
        if (*slots == 0)
            goto inpre_not_found;

        if (strcmp(pre_name, names) == 0)
        {
            return 1;
        }

        if (*slots == 0)
            goto inpre_not_found;
        slots++;
        slot++;
        names += PREManager::NameLength;
    } while (slot < PREManager::SlotCount);

inpre_not_found:
    return 0;
}

/** Diagnostics emitted when no PRE slot or allocation is available. */
static char PRE_full_msg[28]
    __attribute__((section(".rdata"))) = "\nOut of PREManager slots";
static char PRE_open_err[28]
    __attribute__((section(".rdata"))) = "\nMemory error loading %s";

// GCC emitted this private `CClass::~CClass` copy last in the owner's
// text, so it stays at the end of the translation unit.
static void __DOT__6CClass_8005AC28(CClass *self, int flags)
{
    self->vtable = &Pre_BaseVTableWord;
    if (flags & 1)
    {
        CClass::operator delete(self);
    }
}
