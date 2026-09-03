// Shared declaration for captured screen sprites.
#ifndef SPRITE_INC_H
#define SPRITE_INC_H

struct SlicedImage;

/** Screen rectangle stored as a sliced image. */
struct Sprite {
    struct CClass CClass;
    int m_w;
    int m_h;
    int m_packed;
    int m_bpp;
    struct SlicedImage *m_image;

    /** Allocate through the original CClass arena. */
    static void *operator new(unsigned int size) throw()
    {
        return ::CClass::operator new(size);
    }

    /** Load an image-backed sprite. */
    Sprite(char *file, int pack, int slicew, int sliceh);

    /** Capture a rectangular screen area into a sprite. */
    Sprite(int x, int y, int w, int h, int pack = 0,
           int slicew = 0, int sliceh = 0);

    /** Destroy the image owned by this sprite. */
    ~Sprite();
    /** Release sprite storage through the original CClass allocator. */
    static void operator delete(void *storage)
    {
        ::CClass::operator delete(storage);
    }

    /** Draw this sprite with optional RGB modulation. */
    void draw(int x, int y, int UsePanelOT, int r = 0, int g = 0, int b = 0);
};

#endif // SPRITE_INC_H

#if defined(SPRITE_DECLARE_SLICED_IMAGE_DESTRUCTOR_ABI)
struct SlicedImage;
/** Addressable deleting-destructor entry used by the sliced-image table. */
void Shell_SlicedImageDestructorSlot(SlicedImage *image, int inCharge)
    asm("_._11SlicedImage");
#endif

#if defined(SPRITE_DECLARE_SPRITE_DESTRUCTOR_ABI)
struct Sprite;
/** Addressable deleting-destructor entry used by the sprite table. */
void Shell_SpriteDestructorSlot(Sprite *sprite, int inCharge)
    asm("_._6Sprite");
#endif

#if defined(SPRITE_DECLARE_ERASE_SPRITE_DESTRUCTOR_ABI)
struct EraseSprite;
/** Addressable deleting-destructor entry used by the erase-sprite table. */
void Shell_EraseSpriteDestructorSlot(EraseSprite *sprite, int inCharge)
    asm("_._11EraseSprite");
#endif

#if defined(SPRITE_DECLARE_VERTICAL_MENU_DESTRUCTOR_ABI)
struct VerticalMenu;
/** Addressable deleting-destructor entry used by the vertical-menu table. */
void Shell_VerticalMenuDestructorSlot(VerticalMenu *menu, int inCharge)
    asm("_._12VerticalMenu");
#endif

#if defined(SPRITE_DECLARE_VERTICAL_DIALOG_DESTRUCTOR_ABI)
struct VerticalDialog;
/** Addressable deleting-destructor entry used by the vertical-dialog table. */
void Shell_VerticalDialogDestructorSlot(VerticalDialog *dialog, int inCharge)
    asm("_._14VerticalDialog");
#endif

#if defined(SPRITE_DECLARE_SCALAR_VTABLE_ADDRESSES)

/** Scalar address view of the EraseSprite dispatch table. */
extern int __vt_11EraseSprite;
/** Scalar address view of the VerticalDialog dispatch table. */
extern int __vt_14VerticalDialog;
/** Scalar address view of the Sprite dispatch table. */
extern int __vt_6Sprite;
/** Scalar address view of the SlicedImage dispatch table. */
extern int __vt_11SlicedImage;
/** Scalar address view of the VerticalMenu dispatch table. */
extern int __vt_12VerticalMenu;
/** Scalar address view of ShellUtils' private CClass dispatch table. */
extern int DAT_800da7e0;

#undef SPRITE_DECLARE_SCALAR_VTABLE_ADDRESSES
#endif

#if defined(SPRITE_DECLARE_SLICED_IMAGE_UNPACK_ABI)

namespace SlicedImageMethodABI
{
/** Release every packed VRAM slice owned by an image. */
extern "C" void unpack__11SlicedImage(SlicedImage *image);
}

#endif // SPRITE_DECLARE_SLICED_IMAGE_UNPACK_ABI
