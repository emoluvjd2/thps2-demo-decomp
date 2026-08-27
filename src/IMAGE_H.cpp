// IMAGE_H.cpp -- Reconstructed translation unit
// Original: IMAGE.H
// Functions: 4 reconstructed
// Address range: 0x8005E158 - 0x8005E214
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.
#define IMAGE_OWNER_LAYOUT
#include "IMAGE.h"
#undef IMAGE_OWNER_LAYOUT

// === Typed ABI declarations ===
extern "C" int __vt_12RotateSprite[6];
extern "C" void Sprite2_Destroy(Sprite2 *, int) asm("_._7Sprite2");
extern "C" void RotateSprite_Destroy(RotateSprite *, int)
    asm("_._12RotateSprite");
extern "C" void Image_Draw(Image *, int, int) asm("draw__5Imageii");
static void Image_Destroy(Image *, int) asm("__DOT__5Image");

// GCC 2.x emits a private copy of `CClass::~CClass` beside a private base
// vtable in every owner that derives from CClass.  `MAIN.SYM` records 25 such
// `_._6CClass` copies and names no vtable, so both are file-local here.
static void Image_ClassDestructor(CClass *, int)
    asm("__DOT__6CClass_8005E1E4");

// The public vtable carries Image::draw as a fourth word; the private base
// vtable follows it eight-byte aligned.  Consecutive small-data scalars
// rather than arrays keep -G8 emitting the target's `addiu $vX, $gp, off`.
static int Image_VTable __attribute__((section(".sdata"), aligned(8))) = 0;
static int Image_VTableIndex __attribute__((section(".sdata"))) = 0;
static void (*Image_VTableDtor)(Image *, int)
    __attribute__((section(".sdata"))) = &Image_Destroy;
static void (*Image_VTableDraw)(Image *, int, int)
    __attribute__((section(".sdata"))) = &Image_Draw;

static int Image_BaseVTable
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int Image_BaseVTableIndex __attribute__((section(".sdata"))) = 0;
static void (*Image_BaseVTableDtor)(CClass *, int)
    __attribute__((section(".sdata"))) = &Image_ClassDestructor;

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
void RotateSprite_Destroy(RotateSprite *_this, int __in_chrg) {
    _this->Sprite2.SlicedImage2.Image.CClass.vtable =
        __vt_12RotateSprite;
    Sprite2_Destroy(&_this->Sprite2, 0);
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
void Image_Draw(Image *, int x, int y) {
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
static void Image_Destroy(Image *_this, int __in_chrg) {
    _this->CClass.vtable = &Image_BaseVTable;
    if (__in_chrg & 1) {
        ::CClass::operator delete(_this);
    }
}

// GCC emitted this private `CClass::~CClass` copy last in the owner's text,
// so it stays at the end of the translation unit.
static void Image_ClassDestructor(CClass *_this, int __in_chrg) {
    _this->vtable = &Image_BaseVTable;
    if (__in_chrg & 1) {
        ::CClass::operator delete(_this);
    }
}
