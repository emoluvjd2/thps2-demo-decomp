// IMAGE.cpp -- Reconstructed translation unit
// Original: IMAGE.CPP
// Functions: 23
// Address range: 0x8005C258 - 0x8005DAA4
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

// IMAGE.cpp is the only consumer of this recovered slice record.
// Keep it owner-local instead of publishing the former generated TU header.
struct SDoubleBuffer;

/** One rectangular pixel slice owned by SlicedImage2. */
struct Slice {
    short x;
    short y;
    short w;
    short h;
    unsigned short *data;
};

#include "PAL256.h"
#include "PDOUBLEBUFFER.h"

#define DB_BUFFER_LAYOUT
#include "DB.h"
#undef DB_BUFFER_LAYOUT
#include "MEM.h"
#include "PACK.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <STDIO.H>
#define IMAGE_OWNER_LAYOUT
#include "IMAGE.h"
#undef IMAGE_OWNER_LAYOUT

/** Placement construction for the inherited Sprite2 base. */
inline void *operator new(unsigned int, void *address)
{
    return address;
}

// === Free function declarations ===
#define IMAGE_DECLARE_SPRITE2_DESTRUCTOR
#define IMAGE_DECLARE_ROTATE_SPRITE_DESTRUCTOR
#include "IMAGE.h"
void MakeTransparentClut2(int x, int y);
int Load8BitBMP_2(
    char *pData, char **data, int *ww, int *hh, unsigned short *clut);
int Load8BitBMP_(
    char *file, char **data, int *ww, int *hh, unsigned short *clut);

// === ABI-labelled forward declarations ===
#define IMAGE_DECLARE_DEFAULT_CONSTRUCTOR_ABI
#define IMAGE_DECLARE_SLICED_IMAGE2_DESTRUCTOR
#include "IMAGE.h"
#define Image_Construct ImageDefaultConstructorAbi::__5Image
#define SlicedImage2_Construct ImageDefaultConstructorAbi::__12SlicedImage2
// === External function declarations ===
// These palette return types are the declarations IMAGE was compiled against;
// the target caller consumes both results as halfwords.  Heap declarations
// come from MEM.h, whose void deletion signature is recorded by MAIN.SYM.
void Free256Slot(unsigned short clut);
unsigned short GetFree256Slot(void);
unsigned short Get256SlotNumber(unsigned short clut);
#include "FILEIO.h"
#define RCOSSIN_DECL_TYPE ImageSinCos
#define RCOSSIN_SCALAR_VIEW
#include "RCOSSIN.h"

#define IMAGE_SIN(angle) \
    ((&rcossin_tbl)[(angle) & 0xfff].sin)
#define IMAGE_COS(angle) \
    ((&rcossin_tbl)[(angle) & 0xfff].cos)
#define IMAGE_BMP_HEADER(data) \
    (*(ImageBmpHeader *)(data))
#define IMAGE_BMP_COLOR(data) \
    (*(ImageBmpPaletteEntry *)(data))
// SlicedImage2::draw advances a byte stride independently of its logical
// index.  Replacing this target-proven access shape with SliceTab[index]
// changes operand order, so keep the cast confined to this named accessor.
#define IMAGE_SLICE_AT_OFFSET(table, offset) \
    (*(Slice *)((offset) + (int)(table)))
#define IMAGE_ROTATE_SLICE(index) \
    (this->SliceTab[(index)])

// The prototype emits each invocation as one source-line region.  Keep the
// working input pointer alive across allocation, matching its recovered scope.
#define IMAGE_COPY_SLICE_PIXELS(SLICE, SRC, SLICE_W, SLICE_H, SOURCE_W, NEEDS_ALLOCATION) \
    { \
        unsigned short *out; \
        unsigned short *in = (SRC); \
        if ((NEEDS_ALLOCATION) != 0) { \
            (SLICE)->data = (unsigned short *)Mem_New( \
                (SLICE_W) * (SLICE_H) * sizeof(unsigned short), 1, 1); \
        } \
        out = (SLICE)->data; \
        for (int j = 0; j < (SLICE_H); j++) { \
            int i = 0; \
            if ((SLICE_W) > 0) { \
                do { \
                    *out++ = *in++; \
                    i++; \
                } while (i < (SLICE_W)); \
            } \
            in += (SOURCE_W) - (SLICE_W); \
        } \
    }

// === Vtable declarations ===
#define IMAGE_DECLARE_OWNER_VTABLE_ADDRESSES
#include "IMAGE.h"

// === Function implementations (address order) ===

/** Set the active clipping box and relink its primitive into the panel OT. */
void SetClippingBox2(int x, int y, int w, int h) {
    SDoubleBuffer *buffer = pDoubleBuffer;
    RECT r;
    DR_AREA *pClip = (DR_AREA *)pPoly;

    r.x = x;
    r.y = buffer->Top + y;
    r.w = w;
    r.h = h;
    SetDrawArea(pClip, &r);
    SDoubleBuffer *buffer2 = pDoubleBuffer;
    addPrim(buffer2->PanelOT, pClip);
    pPoly = (unsigned int *)(pClip + 1);
}


/**
 *
 * Address:   0x8005C2FC
 * Mangled:   MakeTransparentClut2__Fii
 * Demangled: MakeTransparentClut2(int, int)
 *            Creates a 256-color CLUT with transparency bit set on non-zero entries
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 */
void MakeTransparentClut2(int x, int y) {
    unsigned short *buf =
        (unsigned short *)Mem_New(256 * sizeof(unsigned short), 1, 1);
    RECT temp;
    temp.x = x;
    temp.y = y;
    temp.w = 256;
    temp.h = 1;
    StoreImage(&temp, (unsigned long *)buf);
    DrawSync(0);
    unsigned short *buf2 = buf;
    int i = 0;
    do {
        unsigned short val = *buf2;
        if (val != 0) {
            *buf2 = val | 0x8000;
        }
        i++;
        buf2++;
    } while (i < 256);
    LoadImage(&temp, reinterpret_cast<unsigned long *>(buf));
    DrawSync(0);
    Mem_Delete(buf);
}


/**
 *
 * Address:   0x8005C3B8
 * Mangled:   Load8BitBMP_2__FPcPPcPiT2PUs
 * Demangled: Load8BitBMP_2(char *, char **, int *, int *, unsigned short *)
 *            Decode an 8-bit BMP palette and bottom-up pixel rows into PSX image data.
 *
 * @param pData  Complete in-memory BMP file.
 * @param data  Receives the allocated, top-down pixel buffer.
 * @param ww  Receives the four-byte-aligned image width.
 * @param hh  Receives the image height.
 * @param clut  Receives the uploaded palette's CLUT identifier.
 * @return 1 after the palette and pixel data have been decoded.
 */
int Load8BitBMP_2(
    char *pData, char **data, int *ww, int *hh, unsigned short *clut) {
    unsigned short colorTab[256];
    int clutx;
    int cluty;
    int w;
    int h;
    int colors;
    int transparent_color;
    char *pColor;
    RECT r;
    int pad;
    char *in;
    char *out;

    *data = 0;
    w = IMAGE_BMP_HEADER(pData).width;
    colors = IMAGE_BMP_HEADER(pData).color_count;
    h = IMAGE_BMP_HEADER(pData).height;
    if (colors == 0) {
        colors = 256;
    }
    pColor = pData + sizeof(ImageBmpHeader);
    int i = 0;
    transparent_color = 0;
    if (colors != 0) {
        // Keeping the opaque-black sentinel named hoists it once outside the
        // palette loop; an inline literal produces three real mismatches.
        unsigned short opaqueBlack = 0x8000;
        // The recovered advancing palette pointer is also load-bearing;
        // indexing colorTab by i produces two real mismatches.
        unsigned short *pal = colorTab;
        do {
            unsigned short r = IMAGE_BMP_COLOR(pColor).r;
            unsigned short g = IMAGE_BMP_COLOR(pColor).g;
            unsigned short b = IMAGE_BMP_COLOR(pColor).b;
            if (r >= 0xf8) {
                if (g == 0) {
                    if (b >= 0xf8) {
                        transparent_color = i;
                        *pal = 0;
                        goto next;
                    }
                }
            }
            r = r >> 3;
            g = g >> 3;
            b = b >> 3;
            if (r == 0) {
                if (g == 0) {
                    if (b == 0) {
                        *pal = opaqueBlack;
                        goto next;
                    }
                }
            }
            *pal = b + ((r << 10) + (g << 5));
        next:
            pColor += 4;
            i++;
            pal++;
        } while (i < colors);
    }
    clutx = Pal256X;
    unsigned short slot = GetFree256Slot();
    cluty = Pal256Y + slot;
    r.w = 0x100;
    r.h = 1;
    r.x = clutx;
    r.y = cluty;
    LoadImage(&r, reinterpret_cast<unsigned long *>(colorTab));
    DrawSync(0);

    // Calculate padding: round width up to multiple of 4
    // The temporary keeps signed rounding in v0 before `pad` takes over in
    // s0; folding it into pad causes four real and one reloc mismatch.
    int t = w;
    if (w < 0) {
        t = w + 3;
    }
    pad = t >> 2;
    pad = w - (pad << 2);
    if (pad > 0) {
        pad = 4 - pad;
    }
    w = w + pad;

    // Allocate pixel buffer and copy rows (BMP is bottom-up)
    int rowBytes = w * h;
    *data = (char *)Mem_New(rowBytes, 1, 1);
    in = pData + (colors * 4 + sizeof(ImageBmpHeader));
    out = *data + (h - 1) * w;
    int j = 0;
    if (h != 0) {
        do {
            for (int i = 0; i < w - pad; i++) {
                *out++ = *in++;
            }
            for (int i = 0; i < pad; i++) {
                *out++ = transparent_color;
                in++;
            }
            j++;
            out -= w * 2;
        } while (j < h);
    }
    *ww = w;
    *hh = h;
    *clut = GetClut(clutx, cluty);
    return 1;
}


/**
 *
 * Address:   0x8005C610
 * Mangled:   Load8BitBMP___FPcPPcPiT2PUs
 * Demangled: Load8BitBMP_(char *, char **, int *, int *, unsigned short *)
 *            Loads a BMP file from disk, then calls Load8BitBMP_2 to parse it
 *
 * @param file  BMP filename to open.
 * @param data  Receives the allocated decoded pixel buffer.
 * @param ww  Receives the four-byte-aligned image width.
 * @param hh  Receives the image height.
 * @param clut  Receives the uploaded palette's CLUT identifier.
 * @return 1 on success, or 0 if the file cannot be opened or buffered.
 */
int Load8BitBMP_(
    char *file, char **data, int *ww, int *hh, unsigned short *clut) {
    int size = FileIO_Open(file);
    if (size <= 0) {
        printf("\nFile %s not found", file);
        return 0;
    }
    char *pData = (char *)Mem_New(size, 1, 1);
    if (pData != 0) {
        FileIO_Load(pData);
        FileIO_Sync();
        Load8BitBMP_2(pData, data, ww, hh, clut);
        Mem_Delete(pData);
        return 1;
    }
    printf("\nNot enough memory to load %s", file);
    return 0;
}


/**
 *
 * Address:   0x8005C6DC
 *            Image constructor — sets vtable, color=128, scale=0x1000, clears fields
 * Mangled:   __5Image
 * Demangled: Image::Image(void)
 *
 * @param image Image being initialized.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
Image *Image_Construct(Image *image) {
    Image *result = image;
    // GCC otherwise materializes the vtable address before preserving `this`,
    // producing two real mismatches.  Natural constructor spelling was also
    // measured, but changes the implicit embedded-constructor graph.  A
    // pointer-typed vtable temporary changes eight instructions, so the one
    // shared scalar below retains the target's value-reuse lifetime.
    asm volatile("" : "+r"(result));
    int value = (int)&DAT_800da744;
    result->CClass.vtable = (int *)value;
    value = 0x80;
    result->R = value;
    result->G = value;
    result->B = value;
    value = 0x1000;
    result->Transparent = 0;
    result->Shaded = 0;
    result->TransType = 0;
    result->JustX = 0;
    result->JustY = 0;
    result->Scale = value;
    return result;
}


/**
 *
 * Address:   0x8005C718
 *            SlicedImage2 default constructor — calls Image(), sets vtable, clears fields
 * Mangled:   __12SlicedImage2
 * Demangled: SlicedImage2::SlicedImage2(void)
 *
 * @param image Sliced image being initialized.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
SlicedImage2 *SlicedImage2_Construct(SlicedImage2 *image) {
    Image_Construct(image);
    image->CClass.vtable = &__vt_12SlicedImage2[0];
    image->InMemory = 0;
    image->Packed = 0;
    image->SliceTab = 0;
    return image;
}


/**
 *
 * Address:   0x8005C758
 * Mangled:   __12SlicedImage2PviiiiUcUs
 * Demangled: SlicedImage2::SlicedImage2(void *, int, int, int, int, unsigned char, unsigned short)
 *            Construct and initialize a SlicedImage2 with tile dimensions, data, bpp, and CLUT.
 *
 * @param image Sliced image being initialized.
 * @param pData  Data to operate on.
 * @param width  Width in pixels.
 * @param height  Height in pixels.
 * @param slice_width Maximum width of one VRAM slice.
 * @param slice_height Maximum height of one VRAM slice.
 * @param bpp Source bit depth used to select the horizontal packing shift.
 * @param clut  Palette CLUT (color lookup table) address.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
extern "C" SlicedImage2 *__12SlicedImage2PviiiiUcUs(
    SlicedImage2 *image, void *pData, int width, int height,
    int slice_width, int slice_height, unsigned char bpp,
    unsigned short clut) {
    Image_Construct(image);
    image->CClass.vtable = &__vt_12SlicedImage2[0];
    image->InMemory = 0;
    image->Packed = 0;
    image->SliceTab = 0;
    image->newSlicedImage2(
        pData, width, height, slice_width, slice_height, bpp, clut);
    return image;
}


/**
 *
 * Address:   0x8005C810
 * Mangled:   _._12SlicedImage2
 * Demangled: SlicedImage2::~SlicedImage2()
 *
 * @param image Sliced image being destroyed.
 * @param __in_chrg GCC 2.x in-charge flag; bit 0 also frees the object.
 */
void SlicedImage2_Destroy(
    SlicedImage2 *image, int __in_chrg) {
    image->CClass.vtable = __vt_12SlicedImage2;
    if (image->Packed != 0) {
        image->unpack();
    }
    if (image->InMemory != 0) {
        image->removeFromMemory();
    }
    Slice *data = image->SliceTab;
    if (data != 0) {
        Mem_Delete(data);
    }
    image->CClass.vtable = &DAT_800da754;
    if (__in_chrg & 1) {
        ::CClass::operator delete(image);
    }
}


/**
 *
 * Address:   0x8005C8A0
 * Mangled:   newSlicedImage2__12SlicedImage2PviiiiUcUs
 * Demangled: SlicedImage2::newSlicedImage2(void *, int, int, int, int, unsigned char, unsigned short)
 *            Sets tile dimensions, computes shift bits, adjusts widths, then calls setData
 *
 * @param image Sliced image being reinitialized.
 * @param pData  Data to operate on.
 * @param width  Width in pixels.
 * @param height  Height in pixels.
 * @param slice_width Maximum width of one VRAM slice.
 * @param slice_height Maximum height of one VRAM slice.
 * @param bpp Source bit depth used to select the horizontal packing shift.
 * @param clut  Palette CLUT (color lookup table) address.
 */
void SlicedImage2::newSlicedImage2(
    void *pData, int width, int height,
    int slice_width, int slice_height, unsigned char bpp,
    unsigned short clut) {
    SlicedImage2 *image = this;
    image->W = width;
    image->H = height;
    image->SliceW = slice_width;
    image->SliceH = slice_height;
    image->Clut = clut;
    if (bpp == 4) goto shift2;
    image->BppShift = ((bpp ^ 8) < 1u);
    goto shiftDone;
shift2:
    image->BppShift = 2;
shiftDone:
    if (image->SliceW == 0) {
        image->SliceW = 255;
    }
    if (image->SliceH == 0) {
        image->SliceH = 255;
    }
    image->W >>= image->BppShift;
    image->SliceW >>= image->BppShift;
    image->setData(pData);
}


/**
 *
 * Address:   0x8005C948
 * Mangled:   setData__12SlicedImage2Pv
 * Demangled: SlicedImage2::setData(void *)
 *            Slices an image into tiles, allocating tile descriptors and copying pixel data
 *
 * @param data  Data buffer.
 */
void SlicedImage2::setData(void *data) {
    unsigned short sourceWidth;
    // The original holds the slice-grid width and the slice height in
    // zero-extended halfword locals and sign-extends them at each use, which is
    // why the target loads them once with `lhu` and pays an sll/sra per site
    // instead of re-reading with `lh`.
    unsigned short sliceCountX;
    unsigned short sliceH;
    int remX;
    int remY;
    unsigned short *line_start;
    int j;
    unsigned short *in;
    int i;
    int inMemory;

    remX = this->W % this->SliceW;
    sourceWidth = this->W;
    if (remX == 0) {
        remX = this->SliceW;
    }
    remY = this->H % this->SliceH;
    if (remY == 0) {
        remY = this->SliceH;
    }

    inMemory = (int)this->InMemory;
    if (!inMemory) {
        int sliceCountX = (this->W + this->SliceW - 1) / this->SliceW;
        int sliceCountY = (this->H + this->SliceH - 1) / this->SliceH;
        this->SliceCountX = sliceCountX;
        this->SliceCountY = sliceCountY;
        int gridW = (short)sliceCountX;
        int sliceTableStride = (short)sliceCountY * sizeof(Slice);
        this->SliceTab = (Slice *)Mem_New(
            gridW * sliceTableStride, 1, 1);
        sourceWidth = this->W;
        inMemory = (int)this->InMemory;
    }

    line_start = (unsigned short *)data;
    j = 0;
    if (this->SliceCountY - 1 > 0) {
        do {
            unsigned short *in = line_start;
            int i = 0;
            sliceCountX = this->SliceCountX;
            sliceH = this->SliceH;
            if ((short)sliceCountX - 1 > 0) {
                unsigned short cx = sliceCountX;
                do {
                    unsigned short *src = in;
                    int sliceh = (short)sliceH;
                    int sourceW = (short)sourceWidth;
                    Slice *slice =
                        &this->SliceTab[j * (short)cx + i];
                    int slicew = this->SliceW;
                    inMemory ^= 1;
                    IMAGE_COPY_SLICE_PIXELS(slice, src, slicew, sliceh, sourceW, inMemory);
                    i++;
                    slice->w = slicew;
                    slice->h = sliceh;
                    in += this->SliceW;
                    sliceCountX = this->SliceCountX;
                    sourceWidth = this->W;
                    sliceH = this->SliceH;
                    inMemory = (int)this->InMemory;
                    cx = sliceCountX;
                } while (i < (short)sliceCountX - 1);
            }

            {
                unsigned short *src = in;
                int sliceh = (short)sliceH;
                int sourceW = (short)sourceWidth;
                Slice *slice =
                    &this->SliceTab[j * (short)sliceCountX + i];
                int slicew = remX;
                IMAGE_COPY_SLICE_PIXELS(
                    slice, src, slicew, sliceh, sourceW, inMemory ^= 1);
                slice->w = slicew;
                slice->h = sliceh;
            }

            j++;
            line_start += this->W * this->SliceH;
            sourceWidth = this->W;
            inMemory = (int)this->InMemory;
        } while (j < this->SliceCountY - 1);
    }

    sliceCountX = this->SliceCountX;
    in = line_start;
    for (i = 0; i < (short)sliceCountX - 1; i++) {
        unsigned short cx = sliceCountX;
        int sliceh = remY;
        unsigned short *src = in;
        int sourceW = (short)sourceWidth;
        Slice *slice =
            &this->SliceTab[j * (short)cx + i];
        int slicew = this->SliceW;
        IMAGE_COPY_SLICE_PIXELS(
            slice, src, slicew, sliceh, sourceW, inMemory ^= 1);
        slice->w = slicew;
        slice->h = sliceh;
        in += this->SliceW;
        sliceCountX = this->SliceCountX;
        sourceWidth = this->W;
        inMemory = (int)this->InMemory;
        cx = sliceCountX;
    }

    {
        int slicew = remX;
        int sliceh = remY;
        unsigned short *src = in;
        int sourceW = (short)sourceWidth;
        Slice *slice =
            &this->SliceTab[j * (short)sliceCountX + i];
        inMemory ^= 1;
        IMAGE_COPY_SLICE_PIXELS(slice, src, slicew, sliceh, sourceW, inMemory);
        slice->w = slicew;
        slice->h = sliceh;
    }

    this->InMemory = (void *)1;
    if (this->Packed != 0) {
        this->repack();
    }
}


/**
 *
 * Address:   0x8005CE7C
 *            Loop over rows*cols entries, freeing each tile's data
 * Mangled:   removeFromMemory__12SlicedImage2
 * Demangled: SlicedImage2::removeFromMemory(void)
 *
 * @param image Sliced image whose copied tile pixels are released.
 */
void SlicedImage2::removeFromMemory() {
    if (SliceCountX * SliceCountY > 0) {
        int i = 0;
        do {
            Mem_Delete(SliceTab[i].data);
            i++;
        } while (i < SliceCountX * SliceCountY);
    }
    InMemory = 0;
}


/**
 *
 * Address:   0x8005CF08
 * Mangled:   pack__12SlicedImage2
 * Demangled: SlicedImage2::pack(void)
 *            Purpose: Pack each tile's pixel data into VRAM and refresh packed coordinates.
 *
 * @param image Sliced image whose tiles are allocated in VRAM.
 */
void SlicedImage2::pack() {
    SlicedImage2 *image = this;
    int totalTiles = image->SliceCountX * image->SliceCountY;
    int temp1 = Pack_NoBottom;
    int temp2 = Pack_NoRight;
    Pack_NoBottom = 1;
    Pack_NoRight = 1;
    if (totalTiles > 0) {
        int i = 0;
        do {
            int tileW = image->SliceTab[i].w;
            int tileH = image->SliceTab[i].h;
            int shift = image->BppShift;
            int packW = 16 >> shift;
            int shiftedW = tileW << shift;
            int x, y;
            Pack_Pack(
                0, shiftedW, tileH, &x, &y, packW, 0, 0);
            image->SliceTab[i].x = (unsigned short)x;
            image->SliceTab[i].y = (unsigned short)y;
            i++;
        } while (i < image->SliceCountX * image->SliceCountY);
    }
    Pack_NoBottom = temp1;
    Pack_NoRight = temp2;
    image->Packed = (void *)1;
    repack();
}


/**
 *
 * Address:   0x8005D030
 * Mangled:   repack__12SlicedImage2
 * Demangled: SlicedImage2::repack(void)
 *            Uploads each tile's pixel data to VRAM using LoadImage
 *
 * @param image Sliced image whose tile pixels are uploaded to VRAM.
 */
void SlicedImage2::repack() {
    int j = 0;
    if (SliceCountY > 0) {
        do {
            int i = 0;
            short colLimit = SliceCountX;
            // The original reuses this scalar first as a zero-extended column
            // count and then as the slice-table address.  Splitting those
            // roles into typed locals regresses this function by 35 real and
            // two relocation mismatches.
            int tilePtr = (unsigned short)SliceCountX;
            if (colLimit > 0) {
                do {
                    int index = j * (short)tilePtr + i;
                    tilePtr = (int)SliceTab;
                    Slice *slice =
                        (Slice *)(index * sizeof(Slice) + tilePtr);
                    RECT r;
                    r.w = slice->w;
                    r.h = slice->h;
                    r.x = slice->x;
                    unsigned short tileY = slice->y;
                    i++;
                    r.y = tileY;
                    LoadImage(
                        &r, reinterpret_cast<unsigned long *>(slice->data));
                    DrawSync(0);
                    tilePtr = (unsigned short)SliceCountX;
                } while (i < SliceCountX);
            }
            j++;
        } while (j < SliceCountY);
    }
}


/**
 *
 * Address:   0x8005D114
 *            Loop over rows*cols entries, unpacking each tile
 * Mangled:   unpack__12SlicedImage2
 * Demangled: SlicedImage2::unpack(void)
 *
 * @param image Sliced image whose packed VRAM rectangles are released.
 */
void SlicedImage2::unpack() {
    if (SliceCountX * SliceCountY > 0) {
        int i = 0;
        do {
            Pack_Unpack(
                SliceTab[i].x, SliceTab[i].y);
            i++;
        } while (i < SliceCountX * SliceCountY);
    }
    Packed = 0;
}


/**
 *
 * Address:   0x8005D1A4
 * Mangled:   draw__12SlicedImage2iii
 * Demangled: SlicedImage2::draw(int, int, int)
 *            Draws all tiles as POLY_FT4 textured quads into the OT
 *
 * @param image Sliced image being drawn.
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param drawFirst Nonzero to draw at the far end of the main ordering table;
 *                  zero to use the panel ordering table.
 */
void SlicedImage2::draw(int x, int y, int drawFirst) {
    SlicedImage2 *image = this;
    POLY_FT4 *apoly;
    unsigned int *pOT;
    int just_x;
    int just_y;
    int dx;
    int dy;
    int index;
    int yt;

    if (image->Packed == 0) {
        return;
    }

    if (drawFirst != 0) {
        pOT = pDoubleBuffer->OrderingTable + 0xfff;
    } else {
        pOT = pDoubleBuffer->PanelOT;
    }

    just_x = image->JustX ? image->W << image->BppShift : 0;
    if (image->JustX == 1) {
        just_x >>= 1;
    }

    just_y = image->JustY ? image->H : 0;
    if (image->JustY == 1) {
        just_y >>= 1;
    }

    just_x = (just_x * image->Scale) >> 12;
    just_y = (just_y * image->Scale) >> 12;
    dx = ((image->SliceW << image->BppShift) * image->Scale) >> 12;
    dy = (image->SliceH * image->Scale) >> 12;
    index = 0;
    yt = y - just_y;

    if ((unsigned short)(image->SliceCountY - 1) >= 15) {
        printf("weird SliceCountY\n");
        if ((unsigned short)(image->SliceCountY - 1) >= 15) {
            printf("weird SliceCountY\n");
        }
    }
    if ((unsigned int)image->BppShift >= 3) {
        printf("weird BppShift %d\n", image->BppShift);
    }

    for (int j = 0; j < image->SliceCountY; j++) {
        int xt = x - just_x;
        int i = 0;
        if (image->SliceCountX > 0) {
            // The independent byte offset is load-bearing: direct typed
            // indexing changes two operand choices, while mixing the two
            // forms destabilizes the surrounding schedule substantially.
            int sliceOffset = index * sizeof(Slice);
            do {
                if (image->SliceW <
                    IMAGE_SLICE_AT_OFFSET(image->SliceTab, sliceOffset).w) {
                    printf("weird slice width\n");
                }
                if (IMAGE_SLICE_AT_OFFSET(image->SliceTab, sliceOffset).h >
                    image->SliceH) {
                    printf("weird slice height\n");
                }
                if ((unsigned short)(
                        IMAGE_SLICE_AT_OFFSET(
                            image->SliceTab, sliceOffset).x - 0x200) >=
                    0x200) {
                    printf("weird slice x loc\n");
                }
                if ((unsigned short)IMAGE_SLICE_AT_OFFSET(
                        image->SliceTab, sliceOffset).y >= 0x200) {
                    printf("weird slice y loc\n");
                }

                apoly = (POLY_FT4 *)pPoly;
                pPoly = (unsigned int *)(apoly + 1);
                setPolyFT4(apoly);
                setXY4(
                    apoly,
                    xt,
                    yt,
                    xt + (((IMAGE_SLICE_AT_OFFSET(
                                image->SliceTab, sliceOffset).w
                            << image->BppShift) * image->Scale) >> 12),
                    yt,
                    xt,
                    yt + ((IMAGE_SLICE_AT_OFFSET(
                               image->SliceTab, sliceOffset).h *
                           image->Scale) >> 12),
                    xt + (((IMAGE_SLICE_AT_OFFSET(
                                image->SliceTab, sliceOffset).w
                            << image->BppShift) * image->Scale) >> 12),
                    yt + ((IMAGE_SLICE_AT_OFFSET(
                               image->SliceTab, sliceOffset).h *
                           image->Scale) >> 12));
                setUV0(
                    apoly,
                    (IMAGE_SLICE_AT_OFFSET(
                         image->SliceTab, sliceOffset).x & 0x3f)
                        << image->BppShift,
                    (unsigned char)IMAGE_SLICE_AT_OFFSET(
                        image->SliceTab, sliceOffset).y);
                apoly->u1 =
                    (IMAGE_SLICE_AT_OFFSET(
                         image->SliceTab, sliceOffset).w +
                     (IMAGE_SLICE_AT_OFFSET(
                          image->SliceTab, sliceOffset).x & 0x3f))
                    << image->BppShift;
                apoly->v1 = (unsigned char)IMAGE_SLICE_AT_OFFSET(
                    image->SliceTab, sliceOffset).y;
                apoly->u2 =
                    (IMAGE_SLICE_AT_OFFSET(
                         image->SliceTab, sliceOffset).x & 0x3f)
                    << image->BppShift;
                {
                    Slice *slice =
                        &IMAGE_SLICE_AT_OFFSET(image->SliceTab, sliceOffset);
                    unsigned char sliceY = slice->y;
                    unsigned char sliceH = slice->h;
                    apoly->v2 = sliceH + sliceY;
                }
                apoly->u3 =
                    (IMAGE_SLICE_AT_OFFSET(
                         image->SliceTab, sliceOffset).w +
                     (IMAGE_SLICE_AT_OFFSET(
                          image->SliceTab, sliceOffset).x & 0x3f))
                    << image->BppShift;
                {
                    Slice *slice =
                        &IMAGE_SLICE_AT_OFFSET(image->SliceTab, sliceOffset);
                    unsigned char sliceY = slice->y;
                    unsigned char sliceH = slice->h;
                    apoly->v3 = sliceH + sliceY;
                }
                setRGB0(apoly, image->R, image->G, image->B);
                setSemiTrans(apoly, image->Transparent != 0);
                setShadeTex(apoly, image->Shaded == 0);

                xt += dx;
                apoly->tpage =
                    (((2 - image->BppShift) & 3) << 7) |
                    ((image->TransType & 3) << 5) |
                    (((short)((unsigned short)IMAGE_SLICE_AT_OFFSET(
                        image->SliceTab, sliceOffset).y & 0x100)) >> 4) |
                    (((unsigned short)IMAGE_SLICE_AT_OFFSET(
                        image->SliceTab, sliceOffset).x & 0x3ff) >> 6) |
                    (((unsigned short)IMAGE_SLICE_AT_OFFSET(
                        image->SliceTab, sliceOffset).y & 0x200) << 2);
                apoly->clut = image->Clut;
                addPrim(pOT, apoly);

                index++;
                i++;
                sliceOffset += sizeof(Slice);
            } while (i < image->SliceCountX);
        }
        yt += dy;
    }
}


/**
 *
 * Address:   0x8005D764
 * Mangled:   __7Sprite2Pciii
 * Demangled: Sprite2::Sprite2(char *, int, int, int)
 *            Constructor: loads BMP from file, sets up SlicedImage2, optionally packs, creates CLUT
 *
 * @param filename  Filename string.
 * @param pack Nonzero to upload the constructed slices to VRAM immediately.
 * @param slice_w Maximum width of one VRAM slice.
 * @param slice_h Maximum height of one VRAM slice.
 */
Sprite2::Sprite2(
    char *filename, int pack, int slice_w, int slice_h) {
    Sprite2 *sprite = this;
    SlicedImage2_Construct(sprite);
    sprite->CClass.vtable = &__vt_7Sprite2[0];
    unsigned short clut;
    char *pixel_data_buf;
    int w;
    int h;
    Load8BitBMP_(filename, &pixel_data_buf, &w, &h, &clut);
    sprite->newSlicedImage2(
        pixel_data_buf, w, h, slice_w, slice_h, 8, clut);
    Mem_Delete(pixel_data_buf);
    if (pack != 0) {
        sprite->pack();
    }
    unsigned short slot = Get256SlotNumber(clut);
    int palY = Pal256Y;
    int palX = Pal256X;
    MakeTransparentClut2(palX, palY + slot);
}


/**
 *
 * Address:   0x8005D850
 * Mangled:   __7Sprite2PUciii
 * Demangled: Sprite2::Sprite2(unsigned char *, int, int, int)
 *            Constructor: same as above but takes raw BMP data instead of filename
 *
 * @param buffer  Buffer to operate on.
 * @param pack Nonzero to upload the constructed slices to VRAM immediately.
 * @param slice_w Maximum width of one VRAM slice.
 * @param slice_h Maximum height of one VRAM slice.
 */
Sprite2::Sprite2(
    unsigned char *buffer, int pack, int slice_w, int slice_h) {
    Sprite2 *sprite = this;
    SlicedImage2_Construct(sprite);
    sprite->CClass.vtable = &__vt_7Sprite2[0];
    unsigned short clut;
    char *pixel_data_buf;
    int w;
    int h;
    Load8BitBMP_2((char *)buffer, &pixel_data_buf, &w, &h, &clut);
    sprite->newSlicedImage2(
        pixel_data_buf, w, h, slice_w, slice_h, 8, clut);
    Mem_Delete(pixel_data_buf);
    if (pack != 0) {
        sprite->pack();
    }
    unsigned short slot = Get256SlotNumber(clut);
    int palY = Pal256Y;
    int palX = Pal256X;
    MakeTransparentClut2(palX, palY + slot);
}


/**
 *
 * Address:   0x8005D93C
 * Mangled:   _._7Sprite2
 * Demangled: Sprite2::~Sprite2()
 *
 * @param sprite Sprite being destroyed.
 * @param __in_chrg GCC 2.x in-charge flag; bit 0 also frees the object.
 */
void Sprite2_DestroyAbi(Sprite2 *sprite, int __in_chrg) {
    sprite->CClass.vtable = __vt_7Sprite2;
    if (sprite->BppShift == 1) {
        Free256Slot(sprite->Clut);
    }
    SlicedImage2_Destroy(sprite, 0);
    if (__in_chrg & 1) {
        ::CClass::operator delete(sprite);
    }
}


/**
 *
 * Address:   0x8005D9AC
 *            Loads bitmap data, optionally frees old palette slot, calls SlicedImage2::setData
 * Mangled:   setData__7Sprite2Pv
 * Demangled: Sprite2::setData(void *)
 *
 * @param sprite Sprite receiving replacement bitmap data.
 * @param data  Data buffer.
 */
void Sprite2::setData(void *data) {
    Sprite2 *sprite = this;
    char *pixel_data_buf;
    unsigned short clut;
    int w;
    int h;
    Load8BitBMP_2((char *)data, &pixel_data_buf, &w, &h, &clut);
    if (sprite->BppShift == 1) {
        Free256Slot(sprite->Clut);
    }
    sprite->Clut = clut;
    sprite->SlicedImage2::setData(pixel_data_buf);
    Mem_Delete(pixel_data_buf);
}


/**
 *
 * Address:   0x8005DA24
 *            Returns (this->tileWidth << this->shiftBits) * this->scale >> 12
 * Mangled:   screenWidth__12SlicedImage2
 * Demangled: SlicedImage2::screenWidth(void)
 *
 * @param image Sliced image being measured.
 * @return Scaled screen width in pixels.
 */
int SlicedImage2::screenWidth() {
    return ((W << BppShift) * Scale) >> 12;
}


/**
 *
 * Address:   0x8005DA44
 *            Returns this->tileHeight * this->scale >> 12
 * Mangled:   screenHeight__12SlicedImage2
 * Demangled: SlicedImage2::screenHeight(void)
 *
 * @param image Sliced image being measured.
 * @return Scaled screen height in pixels.
 */
int SlicedImage2::screenHeight() {
    return (H * Scale) >> 12;
}


/**
 *
 * Address:   0x8005DA60
 *            Calls Sprite2 constructor, sets RotateSprite vtable and rotation flag
 * Mangled:   __12RotateSpritePciii
 * Demangled: RotateSprite::RotateSprite(char *, int, int, int)
 *
 * @param filename  Filename string.
 * @param pack Nonzero to upload the constructed slices to VRAM immediately.
 * @param slice_w Maximum width of one VRAM slice.
 * @param slice_h Maximum height of one VRAM slice.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
RotateSprite::RotateSprite(
    char *filename, int pack, int slice_w, int slice_h) {
    ::new (static_cast<Sprite2 *>(this)) struct Sprite2(
        filename, pack, slice_w, slice_h);
    CClass.vtable =
        &__vt_12RotateSprite[0];
    AspectType = 1;
}


/**
 *
 * Address:   0x8005DAA4
 * Mangled:   draw__12RotateSpriteiii
 * Demangled: RotateSprite::draw(int, int, int)
 *            Draws a rotated sprite using trig lookup table for rotation matrix
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param rot  Twelve-bit rotation-table angle; values wrap every 4096 units.
 */
void RotateSprite::draw(int x, int y, int rot) {
    POLY_FT4 *apoly;
    unsigned int *pOT;
    int dux[2];
    int duy[2];
    int dvx[2];
    int dvy[2];
    int aspect;
    int just_ux;
    int just_uy;
    int just_vx;
    int just_vy;
    int just_u;
    int just_v;
    int xbig1;
    int ybig1;
    int slice_type_y;

    if (this->Packed == 0) {
        return;
    }

    pOT = pDoubleBuffer->PanelOT;

    switch (AspectType) {
    case 0:
        aspect = 0x1000;
        break;
    case 1:
        aspect = 0xa00;
        break;
    }

    {
        int i = 0;
        do {
            dux[i] = IMAGE_COS(rot);
            duy[i] = (-IMAGE_SIN(rot) * aspect) >> 12;
            dvx[i] = IMAGE_SIN(rot);
            dvy[i] = (IMAGE_COS(rot) * aspect) >> 12;
            dux[i] = (dux[i] * this->Scale) >> 12;
            duy[i] = (duy[i] * this->Scale) >> 12;
            dvx[i] = (dvx[i] * this->Scale) >> 12;
            dvy[i] = (dvy[i] * this->Scale) >> 12;
            i++;
        } while (i < 2);
    }

    just_ux = dux[0];
    dux[0] *= this->SliceW <<
        this->BppShift;
    just_uy = duy[0];
    duy[0] *= this->SliceW <<
        this->BppShift;
    dux[1] *=
        this->SliceTab[this->SliceCountX - 1].w
        << this->BppShift;
    duy[1] *=
        this->SliceTab[this->SliceCountX - 1].w
        << this->BppShift;
    just_vx = dvx[0];
    dvx[0] *= this->SliceH;
    just_vy = dvy[0];
    dvy[0] *= this->SliceH;
    dvx[1] *= this->SliceTab[
        this->SliceCountX * (this->SliceCountY - 1)].h;
    dvy[1] *= this->SliceTab[
        this->SliceCountX * (this->SliceCountY - 1)].h;

    just_u = this->JustX
        ? this->W << this->BppShift
        : 0;
    if (this->JustX == 1) {
        just_u >>= 1;
    }

    just_v = this->JustY
        ? this->H
        : 0;
    if (this->JustY == 1) {
        just_v >>= 1;
    }

    {
        int j;
        int i;
        int entry;
        int rowLimit;
        int colLimit;
        unsigned short sliceCount;
        // The loop latch consumes the same 16-bit count as both an unsigned
        // next-iteration sentinel and a signed loop bound.  Keep those two
        // type-correct views in shared storage so neither view is widened.
        union {
            short signedCount;
            unsigned short unsignedCount;
        } nextCountBits;

        just_ux *= just_u;
        just_uy *= just_u;
        just_vx *= just_v;
        just_vy *= just_v;
        slice_type_y = 0;
        j = 0;
        sliceCount = this->SliceCountY;
        xbig1 = (x << 12) - just_ux - just_vx;
        ybig1 = (y << 12) - just_uy - just_vy;

        if (this->SliceCountY > 0) {
            entry = 0;
            do {
                int xbig2;
                int ybig2;
                int slice_type_x;
                if (j == (short)sliceCount - 1) {
                    slice_type_y = 1;
                }

                {
                    int i;
                    xbig2 = xbig1;
                    ybig2 = ybig1;
                    slice_type_x = 0;
                    i = 0;
                    // Share the zero-extended count across both slice loops.
                    sliceCount = this->SliceCountX;
                    if ((short)sliceCount > 0) {
                        do {
                            if (i == (short)sliceCount - 1) {
                                slice_type_x = 1;
                            }

                            apoly = (POLY_FT4 *)pPoly;
                            pPoly = (unsigned int *)(apoly + 1);
                            setPolyFT4(apoly);
                            setXY4(
                                apoly,
                                xbig2 >> 12,
                                ybig2 >> 12,
                                (xbig2 + dux[slice_type_x]) >> 12,
                                (ybig2 + duy[slice_type_x]) >> 12,
                                (xbig2 + dvx[slice_type_y]) >> 12,
                                (ybig2 + dvy[slice_type_y]) >> 12,
                                (xbig2 + dux[slice_type_x] +
                                 dvx[slice_type_y]) >> 12,
                                (ybig2 + duy[slice_type_x] +
                                 dvy[slice_type_y]) >> 12);

                            setUV4(
                                apoly,
                                (IMAGE_ROTATE_SLICE(entry).x & 0x3f)
                                    << this->BppShift,
                                (unsigned char)IMAGE_ROTATE_SLICE(entry).y,
                                ((IMAGE_ROTATE_SLICE(entry).x & 0x3f)
                                     << this->BppShift) +
                                    ((IMAGE_ROTATE_SLICE(entry).w
                                      << this->BppShift) - 1),
                                (unsigned char)IMAGE_ROTATE_SLICE(entry).y,
                                (IMAGE_ROTATE_SLICE(entry).x & 0x3f)
                                    << this->BppShift,
                                IMAGE_ROTATE_SLICE(entry).h - 1 +
                                    IMAGE_ROTATE_SLICE(entry).y,
                                ((IMAGE_ROTATE_SLICE(entry).x & 0x3f)
                                     << this->BppShift) +
                                    ((IMAGE_ROTATE_SLICE(entry).w
                                      << this->BppShift) - 1),
                                IMAGE_ROTATE_SLICE(entry).h - 1 +
                                    IMAGE_ROTATE_SLICE(entry).y);

                            setRGB0(
                                apoly,
                                this->R,
                                this->G,
                                this->B);
                            setSemiTrans(
                                apoly,
                                this->Transparent != 0);
                            setShadeTex(
                                apoly,
                                this->Shaded == 0);

                            apoly->tpage = getTPage(
                                2 - this->BppShift,
                                this->TransType,
                                IMAGE_ROTATE_SLICE(entry).x,
                                IMAGE_ROTATE_SLICE(entry).y);
                            apoly->clut = this->Clut;
                            addPrim(pOT, apoly);

                            xbig2 += dux[0];
                            ybig2 += duy[0];
                            entry++;
                            i++;
                            colLimit = this->SliceCountX;
                            nextCountBits.signedCount = this->SliceCountX;
                            sliceCount = nextCountBits.unsignedCount;
                        } while (i < colLimit);
                    }
                }

                j++;
                xbig1 += dvx[0];
                rowLimit = this->SliceCountY;
                sliceCount = this->SliceCountY;
                ybig1 += dvy[0];
            } while (j < rowLimit);
        }
    }
}

/**
 * Virtual dispatch table for rotated sprites.
 *
 * The original owner emits its diagnostic string pool before the three image
 * vtables. Keeping these definitions after the functions preserves that
 * source-level section order.
 */
int __vt_12RotateSprite[6]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&RotateSprite_DestroyAbi,
        (int)(void (*)(Image *, int, int))&Image::draw,
        (int)(void (*)(Sprite2 *, void *))&Sprite2::setData,
        (int)(void (*)(RotateSprite *, int, int, int))&RotateSprite::draw,
    };

/** Virtual dispatch table for ordinary sprites. */
int __vt_7Sprite2[6]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&Sprite2_DestroyAbi,
        (int)(void (*)(Image *, int, int))&Image::draw,
        (int)(void (*)(Sprite2 *, void *))&Sprite2::setData,
        (int)(void (*)(SlicedImage2 *, int, int, int))&SlicedImage2::draw,
    };

/** Virtual dispatch table for sliced images. */
int __vt_12SlicedImage2[6]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&SlicedImage2_Destroy,
        (int)(void (*)(Image *, int, int))&Image::draw,
        (int)(void (*)(SlicedImage2 *, void *))&SlicedImage2::setData,
        (int)(void (*)(SlicedImage2 *, int, int, int))&SlicedImage2::draw,
    };
