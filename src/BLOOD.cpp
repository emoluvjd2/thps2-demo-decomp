// BLOOD.cpp -- Reconstructed translation unit
// Original: BLOOD.CPP
// Functions: 4 reconstructed entries
// Address range: 0x8006B56C - 0x8006B680
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.


// === Forward type declarations ===
// Empty struct bodies allow by-value/by-reference parameters in C++

#define __SKIP_BLOOD_EXTERNS__

#define BIT_BLOOD_MANAGER_LAYOUT
#include "BIT.h"
#undef BIT_BLOOD_MANAGER_LAYOUT


struct CBruce;

// GCC 2.x emits a private copy of `CClass::~CClass` beside a private base
// vtable in every owner that derives from CClass.  `MAIN.SYM` records 25 such
// `_._6CClass` copies and names no base vtable, so both are file-local here.
// The filesystem-safe spelling keeps the per-address target assembly reachable.
static void Blood_ClassDestructor(CClass *self, int flags)
    asm("__DOT__6CClass_8006B690");

// C++ forbids taking a destructor's address, so the public vtable slot uses an
// addressable declaration of the symbol this owner already defines.
extern "C" void Blood_DestructorAbi(Blood *self, int flags) asm("_._5Blood");

// Consecutive eight-byte-aligned small-data words per vtable rather than
// arrays, so -G8 still yields the target's `addiu $vX, $gp, off` form.
extern "C" int __vt_5Blood
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int Blood_VTableIndex = 0;
static int Blood_VTableDtor = (int)&Blood_DestructorAbi;

static int Blood_BaseVTable
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int Blood_BaseVTableIndex = 0;
static int Blood_BaseVTableDtor = (int)&Blood_ClassDestructor;

// === Function implementations (address order) ===

/**
 * Initialize an empty blood-particle manager.
 *
 * Address:   0x8006B56C
 * Mangled:   __5Blood
 * Demangled: Blood::Blood(void)
 *
 */
Blood::Blood() {
    Blood *self = this;
    self->CClass.vtable = &__vt_5Blood;
    int i = 0;
    BloodBit **ptr = self->mBloodStack;
    do {
        *ptr = 0;
        i++;
        ptr++;
    } while (i < Blood::Capacity);
    self->mBloodCount = 0;
    self->mAge = Blood::InitialAge;
}


/**
 * Blood::~Blood()
 *
 * Address:   0x8006B5A4
 * Mangled:   _._5Blood
 * Demangled: Blood::~Blood()
 *            Destructor: sets Blood vtable, calls Reset, sets base CClass vtable, conditionally deletes
 *
 */
Blood::~Blood() {
    CClass.vtable = &__vt_5Blood;
    Reset();
    CClass.vtable = &Blood_BaseVTable;
}


/**
 * Destroy all live blood particles and reset the manager state.
 *
 * Address:   0x8006B5F4
 * Mangled:   Reset__5Blood
 * Demangled: Blood::Reset(void)
 *            Clears active blood bits and resets Blood's counters.
 *
 */
void Blood::Reset() {
    int i = 0;
    if (0 < mBloodCount) {
        BloodBit **p = mBloodStack;
        do {
            BloodBit *bit = *p;
            if (bit != 0) {
                bit->vtable->destroy(bit, 3);
            }
            *p++ = 0;
            i++;
        } while (i < mBloodCount);
    }
    mBloodCount = 0;
    mAge = 0;
}


/**
 * Preserve the blood-update working set used by the full game implementation.
 *
 * Address:   0x8006B680
 * Mangled:   Update__5BloodP6CBruce
 * Demangled: Blood::Update(CBruce *)
 *            Reserves the original update scratch frame, but performs no per-frame blood
 *            simulation work in this build.
 *
 * @param pBruce  Active skater (CBruce) being processed.
 */
void Blood::Update(CBruce *pBruce) {
    CSVector traj;
    CSVector cone;
    CVector SparkPos;
    CVector tempVector;
    SHook hook;
    char reservedScratch[16];
    (void)traj;
    (void)cone;
    (void)SparkPos;
    (void)tempVector;
    (void)hook;
    (void)reservedScratch;
    return;
}

/**
 * Destroy the owner-local CClass base and optionally release its storage.
 *
 * GCC emitted this private copy last in the owner's text, so it remains at
 * the end of the translation unit.
 */
static void Blood_ClassDestructor(CClass *self, int flags)
{
    self->vtable = &Blood_BaseVTable;
    if (flags & 1)
    {
        CClass::operator delete(self);
    }
}
