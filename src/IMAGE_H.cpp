// IMAGE_H.cpp -- Reconstructed translation unit
// Original: IMAGE.H
// Functions: 4 reconstructed
// Address range: 0x8005E158 - 0x8005E214
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.
#define IMAGE_OWNER_LAYOUT
#define IMAGE_DECLARE_SPRITE2_DESTRUCTOR
#define IMAGE_DECLARE_ROTATE_SPRITE_DESTRUCTOR
#include "IMAGE.h"
#undef IMAGE_OWNER_LAYOUT

// === Typed ABI declarations ===
extern "C"
{
static void __DOT__5Image(Image *, int);

// GCC 2.x emits a private copy of `CClass::~CClass` beside a private base
// vtable in every owner that derives from CClass.  `MAIN.SYM` records 25 such
// `_._6CClass` copies and names no vtable, so both are file-local here.
static void __DOT__6CClass_8005E1E4(CClass *, int);
}

// The public vtable carries Image::draw as a fourth word; the private base
// vtable follows it eight-byte aligned.  Consecutive small-data scalars
// rather than arrays keep -G8 emitting the target's `addiu $vX, $gp, off`.
static int Image_VTable __attribute__((section(".sdata"), aligned(8))) = 0;
static int Image_VTableIndex __attribute__((section(".sdata"))) = 0;
static void (*Image_VTableDtor)(Image *, int)
    __attribute__((section(".sdata"))) = &__DOT__5Image;
static void (*Image_VTableDraw)(Image *, int, int)
    __attribute__((section(".sdata"))) =
        (void (*)(Image *, int, int))&Image::draw;

static int Image_BaseVTable
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int Image_BaseVTableIndex __attribute__((section(".sdata"))) = 0;
static void (*Image_BaseVTableDtor)(CClass *, int)
    __attribute__((section(".sdata"))) = &__DOT__6CClass_8005E1E4;

// === Function implementations (address order) ===

/**
 * RotateSprite::~RotateSprite()
 *
 * Address:   0x8005E158
 * Mangled:   _._12RotateSprite
 * Demangled: RotateSprite::~RotateSprite()
 *
 * @param _this  Sprite being destroyed.
 * @param __in_chrg  GCC 2.x in-charge flag; bit 0 also frees the sprite.
 */
void RotateSprite_DestroyAbi(RotateSprite *_this, int __in_chrg) {
    _this->CClass.vtable =
        __vt_12RotateSprite;
    Sprite2_DestroyAbi(_this, 0);
    if (__in_chrg & 1) {
        ::CClass::operator delete(_this);
    }
}


/**
 * Image::draw(int, int)
 *
 * Address:   0x8005E1AC
 * Mangled:   draw__5Imageii
 * Demangled: Image::draw(int, int)
 *
 * @param x  Horizontal draw position.
 * @param y  Vertical draw position.
 */
void Image::draw(int x, int y) {
}


/**
 * Image::~Image()
 *
 * Address:   0x8005E1B4
 * Mangled:   _._5Image
 * Demangled: Image::~Image()
 *
 * @param _this  Image being destroyed.
 * @param __in_chrg  GCC 2.x in-charge flag; bit 0 also frees the image.
 */
static void __DOT__5Image(Image *_this, int __in_chrg) {
    _this->CClass.vtable = &Image_BaseVTable;
    if (__in_chrg & 1) {
        ::CClass::operator delete(_this);
    }
}

// GCC emitted this private `CClass::~CClass` copy last in the owner's text,
// so it stays at the end of the translation unit.
static void __DOT__6CClass_8005E1E4(CClass *_this, int __in_chrg) {
    _this->vtable = &Image_BaseVTable;
    if (__in_chrg & 1) {
        ::CClass::operator delete(_this);
    }
}
