// TEXTWIN.cpp -- Reconstructed translation unit
// Original: TEXTWIN.CPP
// Functions: 1 reconstructed entry
// Address range: 0x8006AA00 - 0x8006AAB0
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#include "globals.h"

/** One linked text window and its line-rendering storage. */
struct CTextWindow {
    CClass CClass;
    char mPoly[24];
    int mX;
    int mY;
    int mWidth;
    int mHeight;
    void *mpLineData;
    unsigned char *mpLineFlags;
    unsigned char mTextR[4];
    unsigned char mTextG[4];
    unsigned char mTextB[4];
    char *mpTitle;
    CTextWindow *mpNext;
    CTextWindow *mpPrevious;
    int mNumLines;

    /** Unlinks the window and releases its line buffers. */
    ~CTextWindow();
    /** Releases window storage through the owning CClass allocator. */
    static void operator delete(void *p) { ::CClass::operator delete(p); }
};

static CTextWindow *WinList = 0;

#include "MEM.h"

// === $gp-relative globals ===
// GCC 2.x emits a private copy of `CClass::~CClass` beside a private base
// vtable in every owner that derives from CClass.  `MAIN.SYM` records 25 such
// `_._6CClass` copies and names no base vtable, so both are file-local here.
// The filesystem-safe spelling keeps the per-address target assembly reachable.
extern "C" {
static void __DOT__6CClass_8006AAB0(CClass *self, int flags);
}

// C++ forbids taking a destructor's address, so the public vtable slot uses an
// addressable declaration of the symbol this owner already defines.
void TextWin_DestructorAbi(CTextWindow *, int)
    asm("_._11CTextWindow");

// Consecutive eight-byte-aligned small-data words per vtable rather than
// arrays, so -G8 still yields the target's `addiu $vX, $gp, off` form.
int __vt_11CTextWindow
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int TextWin_VTableIndex = 0;
static int TextWin_VTableDtor = (int)&TextWin_DestructorAbi;

static int __vt_11CTextWindow_base
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int TextWin_BaseVTableIndex = 0;
static int TextWin_BaseVTableDtor = (int)&__DOT__6CClass_8006AAB0;
// === Function implementations (address order) ===

/**
 * CTextWindow::~CTextWindow()
 *
 * Address:   0x8006AA00
 * Mangled:   _._11CTextWindow
 * Demangled: CTextWindow::~CTextWindow()
 *
 */
CTextWindow::~CTextWindow() {
    CClass.vtable = &__vt_11CTextWindow;
    if (mpNext != 0) {
        mpNext->mpPrevious = mpPrevious;
    }
    if (mpPrevious != 0) {
        mpPrevious->mpNext = mpNext;
    }
    if (WinList == this) {
        WinList = mpNext;
    }
    Mem_Delete(mpLineData);
    Mem_Delete(mpLineFlags);
    CClass.vtable = &__vt_11CTextWindow_base;
}

/**
 * Destroy the owner-local CClass base and optionally release its storage.
 *
 * GCC emitted this private copy last in the owner's text, so it remains at
 * the end of the translation unit.
 */
static void __DOT__6CClass_8006AAB0(CClass *self, int flags)
{
    self->vtable = &__vt_11CTextWindow_base;
    if (flags & 1)
    {
        CClass::operator delete(self);
    }
}
