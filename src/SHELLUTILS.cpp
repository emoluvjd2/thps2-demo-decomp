// SHELLUTILS.cpp -- Reconstructed translation unit
// Original: SHELLUTILS.CPP
// Functions: 31 recovered functions
// Address range: 0x8005E214 - 0x80060EF4
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#include "globals.h"
#include "DB.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBGPU.H>

#include "PAL256.h"

/** Render-buffer fields addressed by shell primitive submission. */
struct SDoubleBuffer {
    u32 Draw[23];
    u32 Disp[5];
    int Visible;
    int RenderNow;
    int Rendering;
    int FlipNow;
    int LastVblanks;
    u32 *PanelOT;
    u32 *OrderingTable;
    u32 *ViewportOT;
    int Top;
    RECT Window;
    u32 Clip[3];
    u8 *Polys;
};

/** Read a file through the game's asynchronous file interface. */
#include "FILEIO.h"

/** Allocate and release packed VRAM rectangles and 8-bit palette slots. */
void Free256Slot(u16 clut);
u16 GetFree256Slot(void);

/** Shell text services used by the menu classes below. */
#include "MESS.h"
#include "PAD.h"

/** Typed source view of the active render buffer pointer. */
#include "PDOUBLEBUFFER.h"

/** Pixel buffer split into independently packable VRAM rectangles. */
struct SlicedImage {
    CClass CClass;
    int width;
    int height;
    int slice_width;
    int slice_height;
    char *slicedBuffer;
    RECT *sliceTab;
    int num_slices_x;
    int num_slices_y;
    int bpp;
    u16 clut;
    int is_packed;
    int m_removed_from_memory;

    /** Release the image's VRAM slices, palette slot, and backing pixels. */
    ~SlicedImage();
    /** Release image storage through the original CClass allocator. */
    static void operator delete(void *p) { ::CClass::operator delete(p); }

    /** Pack every image slice into VRAM and remember its packed rectangle. */
    int pack();

    /** Replace black pixels with the texture transparency sentinel. */
    void removeBlack();

    /** Upload and draw this image one VRAM slice at a time. */
    void rotateDraw(int x, int y);

    /** Draw every packed slice as a textured quad. */
    void draw(int x, int y, int usePanelOT, int r, int g, int b,
              int transparent, int drawStraight);
};

#include "SPRITE.h"

/** Captured screen rectangle used to erase transient shell graphics. */
struct EraseSprite {
    CClass CClass;
    RECT mR;
    unsigned long *mData;

    /** Release the screen-image buffer owned by this erase sprite. */
    ~EraseSprite();
    /** Release erase-sprite storage through the original CClass allocator. */
    static void operator delete(void *p) { ::CClass::operator delete(p); }
};

/** Circular buffer of controller states used by shell input. */
struct PadBuffer {
    /** Shared suppression flag for the second controller input stream. */
    static int mIgnoreTwo;

    CClass CClass;
    int entries;
    int down;
    int buffer[64];
    int first;
    int last;
    int current;
    int mWhich_pad;
};

#include "VERTICALMENUITEM.h"

/** Animated vertical text menu with selection and erase sprites. */
struct VerticalMenu {
    CClass CClass;
    int item_spacing;
    int font_scale;
    int font_scale_big;
    int num_items;
    int old_selected;
    int selected;
    VerticalMenuItem list[12];
    int tick;
    Sprite *mTri1;
    Sprite *mTri2;
    Sprite *mEraseTri1;
    Sprite *mEraseTri2;
    int mDontDeleteSprites;

    /** Build the item metrics and erase sprites used by a vertical shell menu. */
    VerticalMenu(int top, int bottom, int spacing, char **items, int num,
                 int *xlist, int *ylist, int selection);

    /** Release the menu's triangle and per-item erase sprites. */
    ~VerticalMenu();
    /** Release menu storage through the original CClass allocator. */
    static void operator delete(void *p) { ::CClass::operator delete(p); }

    /** Draw the erase sprites covering the current menu state. */
    void erase();

    /** Draw menu entries and advance the selection animation. */
    void draw();

    /** Replace an item's text and refresh its active selection state. */
    void setString(int i, char *string);

    /** Select an enabled item and restart the cursor animation. */
    void select(int num);

    /** Return the selected item index. */
    int getSelected();

    /** Move to the previous enabled item. */
    void up();

    /** Move to the next enabled item. */
    void down();
};

/** Non-interactive vertical dialog rendered with menu item geometry. */
struct VerticalDialog : public VerticalMenu {
    /** Destroy the inherited vertical-menu state. */
    ~VerticalDialog();
    /** Draw every dialog item and its erase sprite. */
    void draw();
};

typedef char shell_sliced_image_size_must_be_52[
    (sizeof(SlicedImage) == 52) ? 1 : -1];
typedef char shell_double_buffer_size_must_be_172[
    (sizeof(SDoubleBuffer) == 172) ? 1 : -1];
typedef char shell_sprite_size_must_be_24[
    (sizeof(Sprite) == 24) ? 1 : -1];
typedef char shell_erase_sprite_size_must_be_16[
    (sizeof(EraseSprite) == 16) ? 1 : -1];
typedef char shell_pad_buffer_size_must_be_284[
    (sizeof(PadBuffer) == 284) ? 1 : -1];
typedef char shell_vertical_menu_size_must_be_820[
    (sizeof(VerticalMenu) == 820) ? 1 : -1];
typedef char shell_vertical_dialog_size_must_be_820[
    (sizeof(VerticalDialog) == 820) ? 1 : -1];

#include "MEM.h"
#include "PACK.h"

// These scalar ABI views preserve the target's absolute vtable addressing.
extern int Shell_EraseSpriteVtable asm("__vt_11EraseSprite");
extern int Shell_VerticalDialogVtable asm("__vt_14VerticalDialog");
extern int Shell_SpriteVtable asm("__vt_6Sprite");
extern int Shell_SlicedImageVtable asm("__vt_11SlicedImage");
extern int Shell_VerticalMenuVtable asm("__vt_12VerticalMenu");
extern "C" int Shell_CClassBaseVtable asm("DAT_800da7e0");

#include <STDIO.H>
#include <STRINGS.H>

// === External globals (absolute) ===
/** Diagnostic emitted when a bitmap is not in the expected 8-bit format. */
static char ShellInvalidBitmapDepthMessage[]
    __attribute__((section(".rdata"), aligned(4))) =
        "\nNot 8 bits per pixel, is %d bits per pixel";
/** Byte offsets in the little-endian BMP header consumed by Load8BitBMP2. */
enum ShellBitmapOffset {
    ShellBitmapWidthOffset = 18,
    ShellBitmapHeightOffset = 22,
    ShellBitmapDepthOffset = 28,
    ShellBitmapColorCountOffset = 46,
    ShellBitmapPaletteOffset = 54,
    ShellBitmapPaletteGreenOffset = 56
};

/** POLY_F4 byte offsets retained where field spelling changes instruction order. */
enum ShellPolyF4Offset {
    ShellPolyF4LengthOffset = 3,
    ShellPolyF4RedOffset = 4,
    ShellPolyF4GreenOffset = 5,
    ShellPolyF4BlueOffset = 6,
    ShellPolyF4CodeOffset = 7,
    ShellPolyF4X0Offset = 8,
    ShellPolyF4Y0Offset = 10,
    ShellPolyF4X1Offset = 12,
    ShellPolyF4Y1Offset = 14,
    ShellPolyF4X2Offset = 16,
    ShellPolyF4Y2Offset = 18,
    ShellPolyF4X3Offset = 20,
    ShellPolyF4Y3Offset = 22
};

// === Free function declarations ===
int Load8BitBMP2(char *pData, char **data, int *ww, int *hh, unsigned short *clut);
int Load8BitBMP(char *file, char **data, int *ww, int *hh, unsigned short *clut);
void DrawColoredBox(int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b);
void SetClippingBox(int x, int y, int w, int h);
void ResetOT(void);
void DrawIt(RECT *r, int x, int y, int bpp, int clut);

// === Extern "C" forward declarations ===
extern "C" {
SlicedImage *Shell_SlicedImageConstructor(
    SlicedImage *, char *, int, int, int, int, int, u16)
    asm("__11SlicedImagePciiiiiUs");
void Shell_SlicedImageUnpack(SlicedImage *)
    asm("unpack__11SlicedImage");
Sprite *Shell_SpriteBitmapConstructor(
    Sprite *, char *, int, int, int) asm("__6SpritePciii");
Sprite *Shell_SpriteRectConstructor(
    Sprite *, int, int, int, int, int = 0, int = 0, int = 0)
    asm("__6Spriteiiiiiii");
}
static void Shell_PadBufferDestructor(PadBuffer *, int)
    asm("_._9PadBuffer");
/** ABI-slot aliases for manual vtables; the natural destructors define them. */
extern "C" {
void Shell_SlicedImageDestructorSlot(SlicedImage *, int)
    asm("_._11SlicedImage");
void Shell_SpriteDestructorSlot(Sprite *, int)
    asm("_._6Sprite");
void Shell_EraseSpriteDestructorSlot(EraseSprite *, int)
    asm("_._11EraseSprite");
void Shell_VerticalMenuDestructorSlot(VerticalMenu *, int)
    asm("_._12VerticalMenu");
void Shell_VerticalDialogDestructorSlot(VerticalDialog *, int)
    asm("_._14VerticalDialog");
void Shell_VerticalMenuDrawVtable(int)
    asm("draw__12VerticalMenu");
void Shell_VerticalDialogDrawVtable(int)
    asm("draw__14VerticalDialog");
}
static void Shell_CClassDestructor(CClass *, int)
    asm("__DOT__6CClass_80060EC4");

extern "C" {

/** Current and midpoint shell ordering-table entries. */
int OT_entry __attribute__((section(".sdata"))) = 0x1000;
static int OT_half __attribute__((section(".sdata"))) = 0;

/** Ordering-table bounds diagnostic retained with its original padding. */
static char Shell_OTEntryDiagnostic_owner[16]
    asm("s_OT_entry_is__d_800da768")
    __attribute__((section(".sdata"))) = "\nOT_entry is %d";

/** Shared PadBuffer input-suppression flag. */
int PadBuffer::mIgnoreTwo
    __attribute__((section(".sdata"))) = 0;

/** Shell class dispatch tables in their original small-data order. */
int Shell_VerticalDialogVtable_owner[4] asm("__vt_14VerticalDialog")
    __attribute__((section(".sdata"), aligned(8))) = {
        0, 0,
        (int)&Shell_VerticalDialogDestructorSlot,
        (int)&Shell_VerticalDialogDrawVtable,
};
int Shell_VerticalMenuVtable_owner[4] asm("__vt_12VerticalMenu")
    __attribute__((section(".sdata"))) = {
        0, 0,
        (int)&Shell_VerticalMenuDestructorSlot,
        (int)&Shell_VerticalMenuDrawVtable,
};
static int Shell_PadBufferVtable_owner[4]
    __attribute__((section(".sdata"))) = {
        0, 0,
        (int)&Shell_PadBufferDestructor,
        0,
};
int Shell_EraseSpriteVtable_owner[4] asm("__vt_11EraseSprite")
    __attribute__((section(".sdata"))) = {
        0, 0,
        (int)&Shell_EraseSpriteDestructorSlot,
        0,
};
int Shell_SpriteVtable_owner[4] asm("__vt_6Sprite")
    __attribute__((section(".sdata"))) = {
        0, 0,
        (int)&Shell_SpriteDestructorSlot,
        0,
};
int Shell_SlicedImageVtable_owner[4] asm("__vt_11SlicedImage")
    __attribute__((section(".sdata"))) = {
        0, 0,
        (int)&Shell_SlicedImageDestructorSlot,
        0,
};
static int Shell_CClassVtable_owner[3] asm("DAT_800da7e0")
    __attribute__((section(".sdata"))) = {
        0, 0,
        (int)&Shell_CClassDestructor,
};

/** Nonzero while the shell is rendering through its high-resolution path. */
int HiresMode __attribute__((section(".sdata"))) = 0;

}

// === Function implementations (address order) ===

/**
 * Load8BitBMP2__FPcPPcPiT2PUs
 *
 * Address:   0x8005E214
 * Mangled:   Load8BitBMP2__FPcPPcPiT2PUs
 * Demangled: Load8BitBMP2(char *, char **, int *, int *, unsigned short *)
 *
 * @param pData  Data to operate on.
 * @param data  Data buffer.
 * @param ww  Width.
 * @param hh  Height.
 * @param clut  Palette CLUT (color lookup table) address.
 * @return 1 after decoding and uploading the palette, or 0 for a non-8-bit BMP.
 */
int Load8BitBMP2(char *pData, char **data, int *ww, int *hh, unsigned short *clut)
{
    int clutx;
    int cluty;
    int w;
    int h;
    int bpp;
    int colors;
    int transparent_color;
    unsigned short colorTab[256];
    char *pColor;
    RECT r;
    int pad;
    char *in;
    char *out;

    *data = 0;
    w = *(unsigned short *)(pData + ShellBitmapWidthOffset);
    bpp = *(unsigned short *)(pData + ShellBitmapDepthOffset);
    h = *(unsigned short *)(pData + ShellBitmapHeightOffset);
    if (bpp != 8)
    {
        printf(ShellInvalidBitmapDepthMessage, bpp);
        return 0;
    }

    colors = *(unsigned short *)(pData + ShellBitmapColorCountOffset);
    pColor = pData + ShellBitmapPaletteOffset;
    if (colors == 0)
    {
        colors = 0x100;
    }

    int i = 0;
    transparent_color = 0;
    if (colors != 0)
    {
        unsigned short opaqueBlack = 0x8000;
        unsigned short *pal = colorTab;
        char *src = pData + ShellBitmapPaletteGreenOffset;
        do
        {
            unsigned short r = *(unsigned char *)pColor;
            unsigned short g = *(unsigned char *)(src - 1);
            unsigned short b = *(unsigned char *)src;
            if (r >= 0xf8 && g == 0 && b >= 0xf8)
            {
                transparent_color = i;
                *pal = 0;
            }
            else
            {
                r >>= 3;
                g >>= 3;
                b >>= 3;
                if (r == 0 && g == 0 && b == 0)
                {
                    *pal = opaqueBlack;
                }
                else
                {
                    *pal = b + ((r << 10) + (g << 5));
                }
            }
            src += 4;
            pColor += 4;
            i++;
            pal++;
        } while (i < colors);
    }

    clutx = Pal256X;
    cluty = Pal256Y + GetFree256Slot();
    r.w = 0x100;
    r.h = 1;
    r.x = clutx;
    r.y = cluty;
    LoadImage(&r, reinterpret_cast<unsigned long *>(colorTab));
    DrawSync(0);

    int widthForDivision = w;
    if (w < 0)
    {
        widthForDivision = w + 3;
    }
    pad = widthForDivision >> 2;
    pad = w - (pad << 2);
    if (pad > 0)
    {
        pad = 4 - pad;
    }
    w += pad;

    *data = (char *)Mem_New(w * h, 1, 1);
    in = pData + (colors * 4 + ShellBitmapPaletteOffset);
    out = *data + (h - 1) * w;
    int j = 0;
    if (h != 0)
    {
        do
        {
            int rowWidth = w - pad;
            for (int i = 0; i < rowWidth; i++)
            {
                *out++ = *in++;
            }

            for (int i = 0; i < pad; i++)
            {
                *out++ = transparent_color;
                in++;
            }
            j++;
            out -= w * 2;
        } while (j < h);
    }

    Mem_Delete(pData);
    *ww = w;
    *hh = h;
    *clut = GetClut(clutx, cluty);
    return 1;
}

/**
 * Load8BitBMP__FPcPPcPiT2PUs
 *
 * Address:   0x8005E49C
 * Mangled:   Load8BitBMP__FPcPPcPiT2PUs
 * Demangled: Load8BitBMP(char *, char **, int *, int *, unsigned short *)
 *            Loads a BMP file, handles open/allocation failures, then parses its 8-bit data.
 *
 * @param file  BMP filename to open and load.
 * @param data  Data buffer.
 * @param ww  Width.
 * @param hh  Height.
 * @param clut  Palette CLUT (color lookup table) address.
 * @return 1 on success, or 0 when the file or allocation is unavailable.
 */
int Load8BitBMP(char *file, char **data, int *ww, int *hh, unsigned short *clut)
{
    int size = FileIO_Open(file);
    if (size <= 0)
    {
        printf("\nFile %s not found", file);
        return 0;
    }

    char *pData = (char *)Mem_New(size, 1, 1);
    if (pData != 0)
    {
        FileIO_Load(pData);
        FileIO_Sync();
        Load8BitBMP2(pData, data, ww, hh, clut);
        return 1;
    }

    printf("\nNot enough memory to load %s", file);
    return 0;
}

/**
 * DrawColoredBox__FiiiiUcUcUc
 *
 * Address:   0x8005E55C
 * Mangled:   DrawColoredBox__FiiiiUcUcUc
 * Demangled: DrawColoredBox(int, int, int, int, unsigned char, unsigned char, unsigned char)
 *            Emit a flat-colored quad into the shell ordering table.
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param w  Width.
 * @param h  Height.
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 */
void DrawColoredBox(int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b)
{
    int lowMask = 0x00ffffff;
    int polyCode2 = 0x28;
    POLY_F4 *Poly = (POLY_F4 *)pPoly;
    int polyAddress = (int)Poly;
    /** Existing register assignments preserve the target's primitive-store order. */
    register unsigned char red asm("$9") = r;
    register unsigned char green asm("$10") = g;
    register unsigned char blue asm("$11") = b;
    register int next asm("$2") = polyAddress + sizeof(POLY_F4);
    pPoly = reinterpret_cast<unsigned int *>(next);
    register int polyCode asm("$2") = 5;
    *(short *)(polyAddress + ShellPolyF4Y0Offset) = y;
    *(short *)(polyAddress + ShellPolyF4Y1Offset) = y;
    y += h;
    *(u8 *)(polyAddress + ShellPolyF4LengthOffset) = polyCode;
    *(u8 *)(polyAddress + ShellPolyF4CodeOffset) = polyCode2;
    *(u8 *)(polyAddress + ShellPolyF4RedOffset) = red;
    *(u8 *)(polyAddress + ShellPolyF4GreenOffset) = green;
    *(u8 *)(polyAddress + ShellPolyF4BlueOffset) = blue;
    SDoubleBuffer *db = pDoubleBuffer;
    int x2 = x;
    x2 = x2 + w;
    *(short *)(polyAddress + ShellPolyF4X1Offset) = x2;
    *(short *)(polyAddress + ShellPolyF4X3Offset) = x2;
    register int entry asm("$6") = OT_entry;
    int tag = *(int *)polyAddress;
    register int highMask asm("$9") = 0xff000000;
    *(short *)(polyAddress + ShellPolyF4X0Offset) = x;
    *(short *)(polyAddress + ShellPolyF4X2Offset) = x;
    *(short *)(polyAddress + ShellPolyF4Y2Offset) = y;
    *(short *)(polyAddress + ShellPolyF4Y3Offset) = y;
    register int ot asm("$4") = (int)db->OrderingTable;
    int offset = entry << 2;
    ot = offset + ot;
    int otVal = *(int *)ot;
    tag = (tag & highMask) | (otVal & lowMask);
    *(int *)polyAddress = tag;

    ot = (int)db->OrderingTable;
    entry = entry - 1;
    OT_entry = entry;
    int otPtr2 = offset + ot;
    int otVal2 = *(int *)otPtr2;
    polyAddress = polyAddress & lowMask;
    otVal2 = otVal2 & highMask;
    otVal2 = otVal2 | polyAddress;
    *(int *)otPtr2 = otVal2;
}

/**
 * SetClippingBox__Fiiii
 *
 * Address:   0x8005E624
 * Mangled:   SetClippingBox__Fiiii
 * Demangled: SetClippingBox(int, int, int, int)
 *            Emit a draw-area primitive for the shell viewport and link it into ShellOTSize.
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param w  Width.
 * @param h  Height.
 */
void SetClippingBox(int x, int y, int w, int h)
{
    RECT r;
    DR_AREA *pClip = (DR_AREA *)pPoly;
    int clipAddress = (int)pClip;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    SetDrawArea(pClip, &r);

    int lowMask = 0x00FFFFFF;
    int highMask = 0xFF000000;
    SDoubleBuffer *db = pDoubleBuffer;
    /** These reads must remain ordered before the ordering-table pointer load. */
    register int entry asm("$6") = *(volatile int *)&OT_entry;
    int tag = *(volatile int *)clipAddress;
    register int ot asm("$4") = (int)db->OrderingTable;
    int otPtr = (entry << 2) + ot;
    int otVal = *(int *)otPtr;
    tag = (tag & highMask) | (otVal & lowMask);
    *(int *)clipAddress = tag;

    ot = (int)db->OrderingTable;
    int next = clipAddress + sizeof(DR_AREA);
    int offset = entry << 2;
    ot = offset + ot;
    register int otVal2 asm("$3") = *(int *)ot;
    clipAddress = clipAddress & lowMask;
    otVal2 = (otVal2 & highMask) | clipAddress;
    OT_entry = entry - 1;
    pPoly = reinterpret_cast<unsigned int *>(next);
    *(int *)ot = otVal2;
}

/**
 * ResetOT__Fv
 *
 * Address:   0x8005E6D0
 * Mangled:   ResetOT__Fv
 * Demangled: ResetOT(void)
 *            Reset the shell ordering-table cursor and restore its default size.
 */
void ResetOT(void)
{
    OT_half = 0;
    OT_entry = 0xFFF;
}

/**
 * SlicedImage construction ABI entry
 *
 * Address:   0x8005E6E4
 * Mangled:   __11SlicedImagePciiiiiUs
 * Demangled: SlicedImage::(char *, int, int, int, int, int, unsigned short)
 *            Build a SlicedImage by copying source pixels into column slices, tracking full
 *            and partial slice counts for later packing/drawing.
 *
 * @param self  Constructed object.
 * @param pImg  Img to operate on.
 * @param w  Width.
 * @param h  Height.
 * @param slicew  Width of each stored image slice, or 0 for the full width.
 * @param sliceh  Height of each stored image slice, or 0 for the full height.
 * @param arg_bpp  Source image bit depth.
 * @param arg_clut  Palette CLUT identifier.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
SlicedImage *Shell_SlicedImageConstructor(
    SlicedImage *self, char *pImg, int w, int h, int slicew, int sliceh,
    int arg_bpp, u16 arg_clut)
{
    self->CClass.vtable = &Shell_SlicedImageVtable;
    self->is_packed = 0;
    self->bpp = arg_bpp;
    self->clut = arg_clut;

    if (arg_bpp == 8)
    {
        w >>= 1;
    }
    if (slicew == 0)
    {
        slicew = w;
    }
    if (sliceh == 0)
    {
        sliceh = h;
    }

    self->width = w;
    self->height = h;
    self->slice_width = slicew;
    self->slice_height = sliceh;
    self->sliceTab = 0;
    self->num_slices_x = w / slicew;
    self->num_slices_y = (h + sliceh - 1) / sliceh;

    self->slicedBuffer = (char *)Mem_New(w * h * 2, 1, 1);
    u16 *out = (u16 *)self->slicedBuffer;
    u16 *in = (u16 *)pImg;
    int slicesX = self->num_slices_x;
    int copyW = self->slice_width;
    int k = 0;
    while (k < slicesX)
    {
        int rows = self->height;
        int j = 0;
        while (j < rows)
        {
            int i = 0;
            while (i < copyW)
            {
                *out = *in;
                in++;
                i++;
                copyW = self->slice_width;
                out++;
            }
            j++;
            rows = self->height;
            in += self->width - copyW;
        }
        slicesX = self->num_slices_x;
        k++;
        in += copyW - self->width * rows;
    }

    int remainder = self->width - slicesX * copyW;
    if (remainder > 0)
    {
        int j = 0;
        if (self->height > 0)
        {
            do
            {
                int i = 0;
                while (i < remainder)
                {
                    *out = *in;
                    in++;
                    i++;
                    out++;
                }
                j++;
                in += self->width - remainder;
            } while (j < self->height);
        }
        self->num_slices_x = self->num_slices_x + 1;
    }

    Mem_Delete(pImg);
    self->m_removed_from_memory = 0;
    return self;
}

/**
 * SlicedImage::~SlicedImage()
 *
 * Address:   0x8005E8E0
 * Mangled:   _._11SlicedImage
 * Demangled: SlicedImage::~SlicedImage()
 *            Unpack any resident image data, release the pixel buffer, then optionally free self.
 *
 */
SlicedImage::~SlicedImage()
{
    CClass.vtable = &Shell_SlicedImageVtable;
    if (is_packed != 0)
    {
        Shell_SlicedImageUnpack(this);
        goto check_type;
    }
    if (sliceTab != 0)
    {
        Pack_Unpack(sliceTab->x, sliceTab->y);
        Mem_Delete(sliceTab);
    }
check_type:
    if (bpp == 8)
    {
        Free256Slot(clut);
    }
    if (m_removed_from_memory == 0)
    {
        Mem_Delete(slicedBuffer);
    }
    CClass.vtable = &Shell_CClassBaseVtable;
}

/**
 * pack__11SlicedImage
 *
 * Address:   0x8005E9A8
 * Mangled:   pack__11SlicedImage
 * Demangled: SlicedImage::pack(void)
 *
 * Pack each full and remainder slice into VRAM, recording the rectangle used
 * for each upload so the image can later be drawn or unpacked.
 *
 * @return Zero if already packed; otherwise the final GPU synchronization
 *         result.
 */
int SlicedImage::pack()
{
    int j, k;
    unsigned short *data_col, *data;
    RECT r;
    int remx;
    int numy;
    int remy;
    int dum1, dum2;

    if (is_packed)
    {
        return 0;
    }

    is_packed = 1;
    sliceTab = (RECT *)Mem_New(
        num_slices_y * num_slices_x * sizeof(RECT), 1, 1);
    data_col = (unsigned short *)slicedBuffer;

    int numx = width / slice_width;
    remx = width - numx * slice_width;
    numy = height / slice_height;
    remy = height - numy * slice_height;

    Pack_NoBottom = 1;

    for (j = 0; j < numx; j++)
    {
        data = data_col;

        for (k = 0; k < numy; k++)
        {
            if (bpp == 8)
            {
                Pack_Pack(0, slice_width * 2, slice_height,
                                        &dum1, &dum2, bpp, 0, 0);
            }
            else if (bpp == 16)
            {
                Pack_Pack(0, slice_width, slice_height,
                                        &dum1, &dum2, bpp, 0, 0);
            }

            r.w = slice_width;
            r.h = slice_height;
            unsigned short packedX = (unsigned short)dum1;
            unsigned short packedY = (unsigned short)dum2;
            r.x = packedX;
            r.y = packedY;
            sliceTab[k * num_slices_x + j].x = packedX;
            sliceTab[k * num_slices_x + j].y = packedY;
            sliceTab[k * num_slices_x + j].w = slice_width;
            sliceTab[k * num_slices_x + j].h = slice_height;
            LoadImage(&r, reinterpret_cast<unsigned long *>(data));
            data += slice_width * slice_height;
        }

        if (remy > 0)
        {
            if (bpp == 8)
            {
                Pack_Pack(0, slice_width * 2, remy,
                                        &dum1, &dum2, bpp, 0, 0);
            }
            else if (bpp == 16)
            {
                Pack_Pack(0, slice_width, remy,
                                        &dum1, &dum2, bpp, 0, 0);
            }

            r.w = slice_width;
            r.h = remy;
            unsigned short packedX = (unsigned short)dum1;
            unsigned short packedY = (unsigned short)dum2;
            r.x = packedX;
            r.y = packedY;
            sliceTab[numy * num_slices_x + j].x = packedX;
            sliceTab[numy * num_slices_x + j].y = packedY;
            sliceTab[numy * num_slices_x + j].w = slice_width;
            sliceTab[numy * num_slices_x + j].h = remy;
            LoadImage(&r, reinterpret_cast<unsigned long *>(data));
        }

        data_col += slice_width * height;
    }

    if (remx > 0)
    {
        data = data_col;

        for (k = 0; k < numy; k++)
        {
            if (bpp == 8)
            {
                Pack_Pack(0, remx * 2, slice_height,
                                        &dum1, &dum2, bpp, 0, 0);
            }
            else if (bpp == 16)
            {
                Pack_Pack(0, remx, slice_height,
                                        &dum1, &dum2, bpp, 0, 0);
            }

            r.w = remx;
            r.h = slice_height;
            unsigned short packedX = (unsigned short)dum1;
            unsigned short packedY = (unsigned short)dum2;
            r.x = packedX;
            r.y = packedY;
            sliceTab[k * num_slices_x + numx].x = packedX;
            sliceTab[k * num_slices_x + numx].y = packedY;
            sliceTab[k * num_slices_x + numx].w = remx;
            sliceTab[k * num_slices_x + numx].h = slice_height;
            LoadImage(&r, reinterpret_cast<unsigned long *>(data));
            data += remx * slice_height;
        }

        if (remy > 0)
        {
            if (bpp == 8)
            {
                Pack_Pack(0, remx * 2, remy,
                                        &dum1, &dum2, bpp, 0, 0);
            }
            else if (bpp == 16)
            {
                Pack_Pack(0, remx, remy,
                                        &dum1, &dum2, bpp, 0, 0);
            }

            r.w = remx;
            r.h = remy;
            unsigned short packedX = (unsigned short)dum1;
            unsigned short packedY = (unsigned short)dum2;
            r.x = packedX;
            r.y = packedY;
            sliceTab[numy * num_slices_x + numx].x = packedX;
            sliceTab[numy * num_slices_x + numx].y = packedY;
            sliceTab[numy * num_slices_x + numx].w = remx;
            sliceTab[numy * num_slices_x + numx].h = remy;
            LoadImage(&r, reinterpret_cast<unsigned long *>(data));
        }
    }

    Pack_NoBottom = 0;
    return DrawSync(0);
}

/**
 * SlicedImage unpack ABI entry
 *
 * Address:   0x8005EFD4
 * Mangled:   unpack__11SlicedImage
 * Demangled: SlicedImage::unpack(void)
 *
 */
void Shell_SlicedImageUnpack(SlicedImage *self)
{
    int j = 0;
    if (self->is_packed == 0)
    {
        RECT *sliceTab = self->sliceTab;
        if (sliceTab == 0)
        {
            return;
        }
        Pack_Unpack(sliceTab->x, sliceTab->y);
    }
    else
    {
        self->is_packed = 0;
        if (self->num_slices_y > 0)
        {
            do
            {
                int cols = self->num_slices_x;
                int i = 0;
                if (cols > 0)
                {
                    do
                    {
                        int sliceIndex = j * cols + i;
                        RECT *slice = (RECT *)((sliceIndex << 3) +
                                              (int)self->sliceTab);
                        i++;
                        Pack_Unpack(slice->x, slice->y);
                        cols = self->num_slices_x;
                    } while (i < cols);
                }
                j++;
            } while (j < self->num_slices_y);
        }
    }
    Mem_Delete(self->sliceTab);
    self->sliceTab = 0;
}

/**
 * draw__11SlicedImageiiiiiiii
 *
 * Address:   0x8005F0BC
 * Mangled:   draw__11SlicedImageiiiiiiii
 * Demangled: SlicedImage::draw(int, int, int, int, int, int, int, int)
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * Draw all packed VRAM slices at the requested position. Each slice becomes a
 * textured quad linked into either the panel ordering table or the current
 * shell ordering-table entry.
 *
 * @param UsePanelOT  Nonzero to draw through the panel ordering table.
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 * @param Trans  Nonzero to enable semi-transparency.
 * @param DrawStraight  Nonzero to submit each primitive immediately.
 */
void SlicedImage::draw(int x, int y, int UsePanelOT, int r, int g, int b,
                       int Trans, int DrawStraight)
{
    int xt;
    int yt = y;
    int index = 0;
    unsigned int *pOT;
    POLY_FT4 *apoly;
    int bpp8;
    int j = 0;
    int i;

    if ((unsigned int)OT_entry >= 0x2000)
    {
        printf(Shell_OTEntryDiagnostic_owner, OT_entry);
    }

    if (UsePanelOT != 0)
    {
        pOT = pDoubleBuffer->PanelOT;
    }
    else
    {
        pOT = pDoubleBuffer->OrderingTable + OT_entry;
    }

    bpp8 = bpp == 8;
    for (j = 0; j < num_slices_y; j++)
    {
        xt = x;
        for (i = 0; i < num_slices_x; i++)
        {
            apoly = (POLY_FT4 *)pPoly;
            pPoly = reinterpret_cast<unsigned int *>(apoly + 1);

            setPolyFT4(apoly);
            if (r == -1)
            {
                setRGB0(apoly, 0, 0, 0);
            }
            else
            {
                setRGB0(apoly, r, g, b);
            }

            setXY4(apoly,
                   xt, yt,
                   xt + (sliceTab[index].w << bpp8), yt,
                   xt, yt + (unsigned short)sliceTab[index].h,
                   xt + (sliceTab[index].w << bpp8),
                   yt + (unsigned short)sliceTab[index].h);
            setUV0(apoly,
                   ((unsigned short)sliceTab[index].x & 0x3f) << bpp8,
                   (unsigned char)sliceTab[index].y);
            apoly->u1 =
                (((unsigned short)sliceTab[index].x & 0x3f) << bpp8) +
                (sliceTab[index].w << bpp8);
            apoly->v1 = (unsigned char)sliceTab[index].y;
            apoly->u2 = ((unsigned short)sliceTab[index].x & 0x3f)
                << bpp8;
            {
                unsigned char sliceY = (unsigned char)sliceTab[index].y;
                unsigned char sliceH = (unsigned char)sliceTab[index].h;
                apoly->v2 = sliceH + sliceY;
            }
            apoly->u3 =
                (((unsigned short)sliceTab[index].x & 0x3f) << bpp8) +
                (sliceTab[index].w << bpp8);
            {
                unsigned char sliceY = (unsigned char)sliceTab[index].y;
                unsigned char sliceH = (unsigned char)sliceTab[index].h;
                apoly->v3 = sliceH + sliceY;
            }

            setSemiTrans(apoly, Trans != 0);
            setShadeTex(apoly, r == -1);
            apoly->tpage = getTPage(bpp == 8 ? 1 : 2, 0,
                                    sliceTab[index].x,
                                    sliceTab[index].y);
            apoly->clut = clut;

            if (DrawStraight == 0)
            {
                addPrim(pOT, apoly);
            }
            else
            {
                DrawPrim(apoly);
            }

            if (pOT != pDoubleBuffer->PanelOT)
            {
                OT_entry--;
            }

            xt += sliceTab[index].w << bpp8;
            index++;
        }
        yt += sliceTab[j * num_slices_x].h;
    }
}

/**
 * DrawIt__FP4RECTiiii
 *
 * Address:   0x8005F534
 * Mangled:   DrawIt__FP4RECTiiii
 * Demangled: DrawIt(RECT *, int, int, int, int)
 *            Draw a VRAM rectangle as a textured quad at the requested screen
 *            position, accounting for 16-bit and 8-bit texture widths.
 *
 * @param r  Source rectangle in VRAM.
 * @param x  Destination X coordinate.
 * @param y  Destination Y coordinate.
 * @param bpp  Source texture bit depth.
 * @param clut  Encoded CLUT address used by indexed textures.
 */
void DrawIt(RECT *r, int x, int y, int bpp, int clut)
{
    POLY_FT4 poly;
    POLY_FT4 *apoly = &poly;

    setPolyFT4(apoly);
    setRGB0(apoly, 0, 0, 0);
    setShadeTex(apoly, 1);

    if (bpp == 0x10)
    {
        setXYWH(apoly, x, y, (unsigned short)r->w,
                (unsigned short)r->h);
        setUV4(apoly,
               (unsigned char)(r->x & 0x3f), (unsigned char)r->y,
               (unsigned char)(r->x & 0x3f) + (unsigned char)r->w,
               (unsigned char)r->y,
               (unsigned char)(r->x & 0x3f),
               (unsigned char)r->h + (unsigned char)r->y,
               (unsigned char)(r->x & 0x3f) + (unsigned char)r->w,
               (unsigned char)r->h + (unsigned char)r->y);
        apoly->tpage = getTPage(2, 0, r->x, r->y);
    }
    else
    {
        setXYWH(apoly, x, y, ((unsigned short)r->w) << 1,
                (unsigned short)r->h);
        setUV4(apoly,
               ((unsigned char)(r->x & 0x3f)) << 1,
               (unsigned char)r->y,
               (((unsigned char)(r->x & 0x3f)) << 1) +
                   (((unsigned char)r->w) << 1),
               (unsigned char)r->y,
               ((unsigned char)(r->x & 0x3f)) << 1,
               (unsigned char)r->h + (unsigned char)r->y,
               (((unsigned char)(r->x & 0x3f)) << 1) +
                   (((unsigned char)r->w) << 1),
               (unsigned char)r->h + (unsigned char)r->y);
        apoly->tpage = getTPage(1, 0, r->x, r->y);
        apoly->clut = clut;
    }

    DrawPrim(apoly);
    DrawSync(0);
}

/**
 * rotateDraw__11SlicedImageii
 *
 * Address:   0x8005F6DC
 * Mangled:   rotateDraw__11SlicedImageii
 * Demangled: SlicedImage::rotateDraw(int, int)
 *
 * Upload and draw every full and remainder slice while preserving the image's
 * packed VRAM rectangle for subsequent uploads.
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 */
void SlicedImage::rotateDraw(int x, int y)
{
    int xt;
    int yt;
    RECT r;
    unsigned short *data_col;
    unsigned short *data;
    int remx;
    int j;
    int k;

    if (sliceTab == 0)
    {
        int dum1;
        int dum2;
        sliceTab = (RECT *)Mem_New(sizeof(RECT), 1, 1);
        Pack_Pack(0, slice_width, slice_height,
                                &dum1, &dum2, 0x10, 0, 0);
        sliceTab->w = slice_width;
        sliceTab->h = slice_height;
        sliceTab->x = dum1;
        sliceTab->y = dum2;
    }

    xt = x;
    r.h = sliceTab->h;
    r.w = sliceTab->w;
    r.x = sliceTab->x;
    r.y = sliceTab->y;
    data_col = (unsigned short *)slicedBuffer;
    int num_slices_x = width / slice_width;
    remx = width - num_slices_x * slice_width;
    int num_slices_y = height / slice_height;
    int remy = height - num_slices_y * slice_height;
    Pack_NoBottom = 1;

    for (j = 0; j < num_slices_x; j++)
    {
        data = data_col;
        yt = y;
        for (k = 0; k < num_slices_y; k++)
        {
            r.w = slice_width;
            r.h = slice_height;
            LoadImage(&r, reinterpret_cast<unsigned long *>(data));
            DrawSync(0);
            DrawIt(&r, xt, yt, bpp, clut);
            yt += slice_height;
            data += slice_width * slice_height;
        }

        if (remy > 0)
        {
            r.w = slice_width;
            r.h = remy;
            LoadImage(&r, reinterpret_cast<unsigned long *>(data));
            DrawSync(0);
            DrawIt(&r, xt, yt, bpp, clut);
        }

        data_col += slice_width * height;
        if (bpp == 0x10)
        {
            xt += slice_width;
        }
        else
        {
            xt += slice_width * 2;
        }
    }

    if (remx > 0)
    {
        data = data_col;
        yt = y;
        for (k = 0; k < num_slices_y; k++)
        {
            r.w = remx;
            r.h = slice_height;
            LoadImage(&r, reinterpret_cast<unsigned long *>(data));
            DrawSync(0);
            DrawIt(&r, xt, yt, bpp, clut);
            yt += slice_height;
            data += remx * slice_height;
        }

        if (remy > 0)
        {
            r.w = remx;
            r.h = remy;
            LoadImage(&r, reinterpret_cast<unsigned long *>(data));
            DrawSync(0);
            DrawIt(&r, xt, yt, bpp, clut);
        }
    }

    Pack_NoBottom = 0;
}

/**
 * SlicedImage::removeBlack()
 *
 * Address:   0x8005FA38
 * Mangled:   removeBlack__11SlicedImage
 * Demangled: SlicedImage::removeBlack(void)
 *            Replaces all black (0x0000) pixels with 0x8000 (transparent) in 16bpp images
 *
 */
void SlicedImage::removeBlack()
{
    if (bpp != 0x10)
        return;
    int temp = width * height;
    u16 *buf = (u16 *)slicedBuffer;
    if (temp <= 0)
        return;
    int i = 0;
    do
    {
        if (*buf == 0)
        {
            *buf = 0x8000;
        }
        i++;
        buf++;
    } while (i < temp);
}

/**
 * Sprite bitmap construction ABI entry
 *
 * Address:   0x8005FA98
 * Mangled:   __6SpritePciii
 * Demangled: Sprite::(char *, int, int, int)
 *            Load an 8bpp BMP into a SlicedImage-backed Sprite.
 *
 * @param self  Constructed object.
 * @param file  BMP filename to load.
 * @param pack  Whether to pack the image into VRAM immediately.
 * @param slicew  Slice width, or 0 to keep it unsliced.
 * @param sliceh  Slice height, or 0 to keep it unsliced.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
Sprite *Shell_SpriteBitmapConstructor(
    Sprite *self, char *file, int pack, int slicew, int sliceh)
{
    char *buf;
    int w;
    int h;
    u16 clut;

    self->CClass.vtable = &Shell_SpriteVtable;
    Load8BitBMP(file, &buf, &w, &h, &clut);
    self->m_bpp = 8;
    self->m_w = w;
    self->m_h = h;
    if (slicew * 2 >= w)
    {
        slicew = 0;
    }
    if (sliceh >= h)
    {
        sliceh = 0;
    }

    SlicedImage *image =
        (SlicedImage *)::CClass::operator new(sizeof(SlicedImage));
    if (image != 0)
    {
        image = Shell_SlicedImageConstructor(
            image, buf, w, h, slicew, sliceh, self->m_bpp, clut);
    }
    self->m_image = image;
    buf = 0;
    self->m_packed = pack;
    if (pack != 0)
    {
        image->pack();
    }
    return self;
}

/**
 * Sprite screen-rectangle construction ABI entry
 *
 * Address:   0x8005FB8C
 * Mangled:   __6Spriteiiiiiii
 * Demangled: Sprite::(int, int, int, int, int, int, int)
 *            Capture a rectangular screen area into a 16bpp SlicedImage-backed Sprite.
 *
 * @param self  Constructed object.
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param w  Width.
 * @param h  Height.
 * @param pack  Whether to pack the captured image into VRAM immediately.
 * @param slicew  Slice width, or 0 to keep it unsliced.
 * @param sliceh  Slice height, or 0 to keep it unsliced.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
Sprite *Shell_SpriteRectConstructor(
    Sprite *self, int x, int y, int w, int h,
    int pack, int slicew, int sliceh)
{
    RECT r;
    int bpp = 0x10;

    self->CClass.vtable = &Shell_SpriteVtable;
    self->m_bpp = bpp;
    self->m_w = w;
    self->m_h = h;
    r.x = (short)x;
    r.y = (short)y;
    r.w = (short)w;
    r.h = (short)h;
    if (slicew >= w)
    {
        slicew = 0;
    }
    if (sliceh >= h)
    {
        sliceh = 0;
    }

    unsigned long *buf = (unsigned long *)Mem_New(w * h * 2, 1, 1);
    StoreImage(&r, buf);
    DrawSync(0);

    SlicedImage *image =
        (SlicedImage *)::CClass::operator new(sizeof(SlicedImage));
    if (image != 0)
    {
        image = Shell_SlicedImageConstructor(
            image, (char *)buf, w, h, slicew, sliceh, bpp, 0);
    }
    self->m_image = image;
    image->removeBlack();
    self->m_packed = pack;
    if (pack != 0)
    {
        self->m_image->pack();
    }
    return self;
}

/**
 * Sprite::~Sprite()
 *
 * Address:   0x8005FCBC
 * Mangled:   _._6Sprite
 * Demangled: Sprite::~Sprite()
 *            Destroy the owned image, restore the base vtable, then optionally free self.
 *
 */
Sprite::~Sprite()
{
    CClass.vtable = &Shell_SpriteVtable;
    SlicedImage *image = m_image;
    if (image != 0)
    {
        int *vtable = image->CClass.vtable;
        typedef void (*VtFunc)(SlicedImage *, int);
        VtFunc func = (VtFunc)vtable[2];
        func(image, 3);
    }
    CClass.vtable = &Shell_CClassBaseVtable;
}

/**
 * Sprite::draw(int, int, int, int, int, int)
 *
 * Address:   0x8005FD28
 * Mangled:   draw__6Spriteiiiiii
 * Demangled: Sprite::draw(int, int, int, int, int, int)
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param UsePanelOT  Nonzero to submit through the panel ordering table.
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 */
void Sprite::draw(int x, int y, int UsePanelOT, int r, int g, int b)
{
    if (m_packed)
    {
        m_image->draw(x, y, UsePanelOT, r, g, b, 0, 0);
    }
    else
    {
        m_image->rotateDraw(x, y);
    }
}

/**
 * EraseSprite::~EraseSprite()
 *
 * Address:   0x8005FD8C
 * Mangled:   _._11EraseSprite
 * Demangled: EraseSprite::~EraseSprite()
 *            Release the erase backing buffer and optionally delete the sprite object.
 *
 */
EraseSprite::~EraseSprite()
{
    CClass.vtable = &Shell_EraseSpriteVtable;
    Mem_Delete(mData);
    CClass.vtable = &Shell_CClassBaseVtable;
}

/**
 * __12VerticalMenuiiiPPciPiT6i
 *
 * Address: 0x8005FDE0
 * Mangled: __12VerticalMenuiiiPPciPiT6i
 *
 * Build the item metrics and erase sprites used by a vertical shell menu.
 *
 * @param top  Top of the vertical layout range.
 * @param bottom  Bottom of the vertical layout range.
 * @param spacing  Extra space inserted between entries.
 * @param items  Null-terminated text for each entry.
 * @param num  Number of entries.
 * @param xlist  Optional per-entry x coordinates.
 * @param ylist  Optional per-entry y coordinates.
 * @param selection  Initially selected entry.
 * @return `this` pointer (GCC 2.x constructor convention).
 */
VerticalMenu::VerticalMenu(int top, int bottom, int spacing, char **items,
                           int num, int *xlist, int *ylist, int selection)
{
    int tot_h = 0;

    CClass.vtable = &Shell_VerticalMenuVtable;
    item_spacing = spacing;
    num_items = num;
    font_scale = Mess_GetFontScale();
    font_scale_big = (font_scale * 320) / 256;

    for (int i = 0; i < num_items; i++)
    {
        strcpy(list[i].string, items[i]);
        list[i].w = Mess_TextWidth(items[i]);
        list[i].h = Mess_TextHeight(items[i]);
        list[i].x = (xlist != 0) ? xlist[i] : 320;
        list[i].baseline = Mess_TextHeightAboveBaseline(list[i].string);
        list[i].grayed = 0;
        tot_h += item_spacing + list[i].h;
    }

    int y = (top + bottom - tot_h) / 2;
    for (int i = 0; i < num_items; i++)
    {
        int item_y = y + (list[i].h >> 1);
        list[i].y = item_y;
        y += item_spacing + list[i].h;
        if (ylist != 0)
        {
            list[i].y = ylist[i];
        }
        else
        {
            list[i].y = item_y;
        }

        if (xlist[i] != 0 && xlist[i] != 320)
        {
            list[i].erase_sprite_x = list[i].x - 230;
        }
        else
        {
            list[i].erase_sprite_x = 45;
        }

        list[i].erase_sprite =
            new Sprite(
                list[i].erase_sprite_x,
                list[i].y -
                    (((list[i].h >> 1) * font_scale_big) /
                     font_scale) - 1,
                550,
                ((list[i].h * font_scale_big) /
                 font_scale) + 4,
                1, 32, 128);

        DrawSync(0);
        if (list[i].grayed != 0)
        {
            Mess_SetRGB(0x40, 0x40, 0x40, 0);
        }
        else
        {
            Mess_SetRGB(0x7a, 0x7a, 0x7a, 0);
        }
        Mess_DrawText(
            list[i].x,
            list[i].y - (list[i].h >> 1) + list[i].baseline,
            list[i].string, 0, 0, 0x100, 0);
    }

    mTri1 = 0;
    mTri2 = 0;
    mEraseTri1 = 0;
    mEraseTri2 = 0;
    selected = selection;
    select(selection);
}

/**
 * VerticalMenu::setString(int, char *)
 *
 * Address:   0x800601A4
 * Mangled:   setString__12VerticalMenuiPc
 * Demangled: VerticalMenu::setString(int, char *)
 *            Purpose: Copy menu text and refresh selection state while the cursor is idle.
 *
 * @param i  Menu-entry index to update.
 * @param string  Replacement label text.
 */
void VerticalMenu::setString(int i, char *string)
{
    strcpy(list[i].string, string);
    if (tick < 0x3d && i == selected)
    {
        select(i);
    }
}

/**
 * VerticalMenu::~VerticalMenu()
 *
 * Address:   0x80060214
 * Mangled:   _._12VerticalMenu
 * Demangled: VerticalMenu::~VerticalMenu()
 *            Purpose: Release optional triangle sprites and per-item erase sprites, then optionally delete self.
 *
 */
VerticalMenu::~VerticalMenu()
{
    /** GCC 2.x needs the target's existing dispatch registers for these slots. */
    typedef int (*VtFunc)(Sprite *, int);
    CClass.vtable = &Shell_VerticalMenuVtable;
    if (mDontDeleteSprites == 0)
    {
        Sprite *sprite = mTri1;
        if (sprite != 0)
        {
            register int *vtable asm("$2") = sprite->CClass.vtable;
            register VtFunc func asm("$3") = (VtFunc)vtable[2];
            func(sprite, 3);
        }
        sprite = mTri2;
        if (sprite != 0)
        {
            register int *vtable asm("$2") = sprite->CClass.vtable;
            register VtFunc func asm("$3") = (VtFunc)vtable[2];
            func(sprite, 3);
        }
    }

    Sprite *sprite = mEraseTri1;
    if (sprite != 0)
    {
        register int *vtable asm("$2") = sprite->CClass.vtable;
        register VtFunc func asm("$3") = (VtFunc)vtable[2];
        func(sprite, 3);
    }
    sprite = mEraseTri2;
    if (sprite != 0)
    {
        register int *vtable asm("$2") = sprite->CClass.vtable;
        register VtFunc func asm("$3") = (VtFunc)vtable[2];
        func(sprite, 3);
    }

    int numItems = num_items;
    if (numItems > 0)
    {
        int i = 0;
        do
        {
            sprite = list[i].erase_sprite;
            if (sprite != 0)
            {
                register int *vtable asm("$2") = sprite->CClass.vtable;
                register VtFunc func asm("$3") = (VtFunc)vtable[2];
                func(sprite, 3);
                numItems = num_items;
            }
            i = i + 1;
        } while (i < numItems);
    }

    CClass.vtable = &Shell_CClassBaseVtable;
}

/**
 * draw__12VerticalMenu
 *
 * Address:   0x80060368
 * Mangled:   draw__12VerticalMenu
 * Demangled: VerticalMenu::draw(void)
 * Draw all menu entries, rebuild the transition erase sprites when needed,
 * and advance the selection animation timer.
 */
void VerticalMenu::draw()
{
    int tri_off;
    int y;
    int i;
    Mess_SetTextJustify(0);
    Mess_SetFontScale(font_scale);

    if (tick == 63 || tick == 64)
    {
        tri_off = (((list[old_selected].w >> 1) * font_scale_big) /
                   font_scale) + 30;
        if (mEraseTri1 != 0)
        {
            mEraseTri1->draw(list[old_selected].x - tri_off - 22,
                             list[old_selected].y - 34,
                             0, -1, -1, -1);
        }
        if (mEraseTri2 != 0)
        {
            mEraseTri2->draw(list[old_selected].x + tri_off - 22,
                             list[old_selected].y - 34,
                             0, -1, -1, -1);
        }
        if (tick == 63)
        {
            list[selected].w = Mess_TextWidth(list[selected].string);
        }
    }

    for (i = 0; i < num_items; i++)
    {
        list[i].erase_sprite->draw(
            list[i].erase_sprite_x,
            list[i].y - (((list[i].h >> 1) * font_scale_big) /
                         font_scale) - 1,
            0, -1, -1, -1);

        if (i == selected)
        {
            Mess_SetRGB(0xb6, 0x84, 0x24, 0);
            Mess_SetFontScale(font_scale_big);
            y = list[i].y -
                ((((list[i].h >> 1) - list[i].baseline) * font_scale_big) /
                 font_scale);
        }
        else
        {
            if (list[i].grayed != 0)
            {
                Mess_SetRGB(0x40, 0x40, 0x40, 0);
            }
            else
            {
                Mess_SetRGB(0x7a, 0x7a, 0x7a, 0);
            }
            y = list[i].y - (list[i].h >> 1) + list[i].baseline;
        }

        Mess_DrawText(list[i].x, y, list[i].string,
                                   0, 0, 0x100, 0);
        Mess_SetFontScale(font_scale);
    }

    tri_off = (((list[selected].w >> 1) * font_scale_big) /
               font_scale) + 30;

    if (tick == 61)
    {
        if (mEraseTri1 != 0)
        {
            int *vtable = mEraseTri1->CClass.vtable;
            typedef void (*Destructor)(Sprite *, int);
            ((Destructor)vtable[2])(mEraseTri1, 3);
        }

        Sprite *tri = (Sprite *)::CClass::operator new(sizeof(Sprite));
        if (tri != 0)
        {
            tri = Shell_SpriteRectConstructor(
                tri,
                list[selected].x -
                    (((list[selected].w >> 1) * font_scale_big) / font_scale) -
                    52,
                list[selected].y - 34, 44, 68, 1, 0, 0);
        }
        mEraseTri1 = tri;

        if (mEraseTri2 != 0)
        {
            int *vtable = mEraseTri2->CClass.vtable;
            typedef void (*Destructor)(Sprite *, int);
            ((Destructor)vtable[2])(mEraseTri2, 3);
        }

        Sprite *tri2 = (Sprite *)::CClass::operator new(sizeof(Sprite));
        if (tri2 != 0)
        {
            tri2 = Shell_SpriteRectConstructor(
                tri2,
                list[selected].x +
                    (((list[selected].w >> 1) * font_scale_big) / font_scale) +
                    8,
                list[selected].y - 34, 44, 68, 1, 0, 0);
        }
        mEraseTri2 = tri2;
    }

    if (tick >= 16 && tick <= 60)
    {
        if (mTri1 != 0)
        {
            mTri1->draw(list[selected].x - tri_off - 22,
                        list[selected].y - 34,
                        0, -1, -1, -1);
        }
        if (mTri2 != 0)
        {
            mTri2->draw(list[selected].x + tri_off - 22,
                        list[selected].y - 34,
                        0, -1, -1, -1);
        }
    }
    else if (tick == 14 || tick == 15)
    {
        mEraseTri1->draw(list[selected].x - tri_off - 22,
                         list[selected].y - 34,
                         0, -1, -1, -1);
        mEraseTri2->draw(list[selected].x + tri_off - 22,
                         list[selected].y - 34,
                         0, -1, -1, -1);
    }

    tick--;
    if (tick <= 0)
    {
        tick = 60;
    }
}

/**
 * erase__12VerticalMenu
 *
 * Address:   0x80060970
 * Mangled:   erase__12VerticalMenu
 * Demangled: VerticalMenu::erase(void)
 *            Purpose: Draw erase sprites for currently-selected item and all menu erase items.
 */
void VerticalMenu::erase()
{
    int tri_off = (((list[selected].w >> 1) * font_scale_big) /
                   font_scale) + 30;

    if (tick < 61)
    {
        if (mEraseTri1 != 0)
        {
            mEraseTri1->draw(list[selected].x - tri_off - 22,
                             list[selected].y - 34,
                             0, -1, -1, -1);
        }
        if (mEraseTri2 != 0)
        {
            mEraseTri2->draw(list[selected].x + tri_off - 22,
                             list[selected].y - 34,
                             0, -1, -1, -1);
        }
    }

    for (int i = 0; i < num_items; i++)
    {
        list[i].erase_sprite->draw(
            list[i].erase_sprite_x,
            list[i].y - (((list[i].h >> 1) * font_scale_big) /
                         font_scale) - 1,
            0, -1, -1, -1);
    }
}

/**
 * VerticalMenu::select(int)
 *
 * Address:   0x80060AF4
 * Mangled:   select__12VerticalMenui
 * Demangled: VerticalMenu::select(int)
 *            Sets selected entry if not disabled, updates previous and timer
 *
 * @param num  Menu-entry index to select.
 */
void VerticalMenu::select(int num)
{
    if (list[num].grayed != 0)
        return;
    old_selected = selected;
    selected = num;
    if (tick >= 0x3D)
    {
        tick = 0x3E;
        return;
    }
    tick = 0x40;
}

/**
 * VerticalMenu::getSelected()
 *
 * Address:   0x80060B40
 * Mangled:   getSelected__12VerticalMenu
 * Demangled: VerticalMenu::getSelected(void)
 *            Return the current selected menu entry index.
 *
 * @return Current selected menu-entry index.
 */
int VerticalMenu::getSelected()
{
    return selected;
}

/**
 * VerticalMenu::up()
 *
 * Address:   0x80060B4C
 * Mangled:   up__12VerticalMenu
 * Demangled: VerticalMenu::up(void)
 *            Move selection upward, wrapping past disabled items and clearing pad triggers during transitions.
 *
 */
void VerticalMenu::up()
{
    if (tick >= 0x3d)
    {
        Pad_ClearTriggers(&Pad[0]);
        Pad_ClearTriggers(&Pad[1]);
        return;
    }

    int current = selected;
    int choice = current - 1;
    if (choice < 0)
    {
        choice = num_items - 1;
    }
    if (choice != current)
    {
        char *disabledBase = (char *)&list[0].grayed;
        int saved = current;
        int shifted = choice << 6;
    up_check:
        if (*(int *)(disabledBase + shifted) == 0)
            goto up_done;
        choice = choice - 1;
        if (choice < 0)
        {
            choice = num_items - 1;
        }
        if (choice != saved)
        {
            shifted = choice << 6;
            goto up_check;
        }
    }
up_done:
    select(choice);
}

/**
 * VerticalMenu::down()
 *
 * Address:   0x80060C04
 * Mangled:   down__12VerticalMenu
 * Demangled: VerticalMenu::down(void)
 *            Move selection downward, wrapping past disabled items and clearing pad triggers during transitions.
 *
 */
void VerticalMenu::down()
{
    if (tick >= 0x3d)
    {
        Pad_ClearTriggers(&Pad[0]);
        Pad_ClearTriggers(&Pad[1]);
        return;
    }

    int current = selected;
    /** Retain the target's loop-bound and sentinel register across the scan. */
    register int numEntries asm("$6") = num_items;
    int choice = current + 1;
    if (choice >= numEntries)
        choice = 0;
    if (choice != current)
    {
        char *disabledBase = (char *)&list[0].grayed;
        int total = numEntries;
        register int saved asm("$6") = current;
        int shifted = choice << 6;
    down_check:
        if (*(int *)(disabledBase + shifted) == 0)
            goto down_done;
        choice++;
        if (choice >= total)
            choice = 0;
        if (choice != saved)
        {
            shifted = choice << 6;
            goto down_check;
        }
    }
down_done:
    select(choice);
}

/**
 * VerticalDialog::~VerticalDialog()
 *
 * Address:   0x80060CB8
 * Mangled:   _._14VerticalDialog
 * Demangled: VerticalDialog::~VerticalDialog()
 *            Destroy the contained vertical menu state, then optionally free self.
 *
 */
VerticalDialog::~VerticalDialog()
{
    CClass.vtable = &Shell_VerticalDialogVtable;
}

/**
 * draw__14VerticalDialog
 *
 * Address:   0x80060D08
 * Mangled:   draw__14VerticalDialog
 * Demangled: VerticalDialog::draw(void)
 *            Purpose: Render vertical dialog list text and baseline sprites with current font scale.
 *
 * @param self  Implicit `this` pointer.
 */
void VerticalDialog::draw()
{
    Mess_SetTextJustify(0);
    Mess_SetFontScale(font_scale);

    for (int i = 0; i < num_items; i++)
    {
        list[i].erase_sprite->draw(
            list[i].erase_sprite_x,
            list[i].y - (((list[i].h >> 1) * font_scale_big) /
                         font_scale) - 1,
            0, -1, -1, -1);

        if (list[i].grayed != 0)
        {
            Mess_SetRGB(0x40, 0x40, 0x40, 0);
        }
        else
        {
            Mess_SetRGB(0x7a, 0x7a, 0x7a, 0);
        }

        Mess_DrawText(
            list[i].x,
            list[i].y - (list[i].h >> 1) + list[i].baseline,
            list[i].string, 0, 0, 0x100, 0);
        Mess_SetFontScale(font_scale);
    }
}

/**
 * Destroy the shell's input buffer and optionally release its storage.
 *
 * This owner-instantiated destructor belongs to the same original
 * `ShellUtils.obj` contribution as the shell implementation above.
 *
 * Address: 0x80060E94
 * Mangled: _._9PadBuffer
 */
static void Shell_PadBufferDestructor(PadBuffer *self, int flag)
{
    self->CClass.vtable = &Shell_CClassBaseVtable;
    if (flag & 1)
        ::CClass::operator delete(self);
}

/**
 * Destroy the private `CClass` base instantiated by `ShellUtils.obj`.
 *
 * Address: 0x80060EC4
 * Mangled: _._6CClass
 */
static void Shell_CClassDestructor(CClass *self, int flag)
{
    self->vtable = &Shell_CClassBaseVtable;
    if (flag & 1)
        ::CClass::operator delete(self);
}
