// MISCTOOLS_H.cpp -- Reconstructed translation unit
// Original: MISCTOOLS.H
// Functions: 2 reconstructed
// Address range: 0x8005AC58 - 0x8005ACB8
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.



#include "globals.h"

/** Slider state with the target-proven MISCTOOLS.H layout. */
struct Slider {
    CClass CClass;
    int X;
    int V;
    int X0;
    int X1;
    int V0;
    int V1;
    int A0;
    int DerivA;
    int T;
    int TMax;
};

typedef char MiscTools_SliderSizeCheck[(sizeof(Slider) == 44) ? 1 : -1];

typedef void (*MiscTools_SliderDestructor)(Slider *, int);
typedef void (*MiscTools_ClassDestructor)(CClass *, int);

static void MiscTools_DestroySlider(Slider *, int)
    asm("__DOT__6Slider");

// GCC 2.x emits a private copy of `CClass::~CClass` beside a private base
// vtable in every owner that derives from CClass.  `MAIN.SYM` records 25 such
// `_._6CClass` copies and names no base vtable, so both are file-local here.
// The filesystem-safe spelling keeps the per-address target assembly reachable.
static void MiscTools_DestroyClass(CClass *, int)
    asm("__DOT__6CClass_8005AC88");

// Both vtables are three consecutive eight-byte-aligned small-data words rather
// than arrays, so -G8 still yields the target's `addiu $vX, $gp, off` form.
// The public vtable is unnamed in MAIN.SYM; the base vtable is the one both
// destructors install, at $gp + 0x2a5c.
static int MiscTools_SliderVTable
    __attribute__((aligned(8))) = 0;
static int MiscTools_SliderVTableIndex = 0;
static MiscTools_SliderDestructor MiscTools_SliderVTableDtor =
    MiscTools_DestroySlider;

static int MiscTools_DestructorVTable
    __attribute__((aligned(8))) = 0;
static int MiscTools_DestructorVTableIndex = 0;
static MiscTools_ClassDestructor MiscTools_DestructorVTableDtor =
    MiscTools_DestroyClass;

// === Function implementations (address order) ===

/**
 * Destroys a Slider and optionally releases its storage.
 *
 * Address:   0x8005AC58
 * Mangled:   _._6Slider
 * Demangled: Slider::~Slider()
 *
 * @param _this Slider being destroyed.
 * @param __in_chrg GCC 2.x in-charge flag; bit 0 releases the object.
 */
static void MiscTools_DestroySlider(Slider *_this, int __in_chrg) {
    _this->CClass.vtable = &MiscTools_DestructorVTable;
    if (__in_chrg & 1) {
        CClass::operator delete(_this);
    }
}

/**
 * Destroys the embedded CClass base and optionally releases its storage.
 *
 * GCC emitted this private copy last in the owner's text.
 *
 * @param _this Base object being destroyed.
 * @param __in_chrg GCC 2.x in-charge flag; bit 0 releases the object.
 */
static void MiscTools_DestroyClass(CClass *_this, int __in_chrg) {
    _this->vtable = &MiscTools_DestructorVTable;
    if (__in_chrg & 1) {
        CClass::operator delete(_this);
    }
}
