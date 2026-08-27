#if !defined(IMAGE_OWNER_LAYOUT) && !defined(IMAGE_INC_H)
#define IMAGE_INC_H

/** Set the active clipping box and relink it into the panel ordering table. */
void SetClippingBox2(int x, int y, int width, int height);

#endif // !IMAGE_OWNER_LAYOUT && !IMAGE_INC_H

#if defined(IMAGE_OWNER_LAYOUT) && !defined(IMAGE_OWNER_LAYOUT_EMITTED)
#define IMAGE_OWNER_LAYOUT_EMITTED

// Owner-local image hierarchy used by IMAGE.cpp.

#include "globals.h"

struct Slice;

/** One sine/cosine pair in the 4096-entry fixed-point lookup table. */
struct ImageSinCos {
    short sin;
    short cos;
};

/** BMP header fields consumed by the 8-bit image loader. */
struct ImageBmpHeader {
    unsigned char opaque_before_width[0x12];
    unsigned short width;
    unsigned char opaque_before_height[0x16 - 0x14];
    unsigned short height;
    unsigned char opaque_before_color_count[0x2e - 0x18];
    unsigned short color_count;
    unsigned char opaque_before_palette[0x36 - 0x30];
};

/** Four-byte palette record consumed by the BMP decoder. */
struct ImageBmpPaletteEntry {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char reserved;
};

#include "IMAGEBASE.h"

/** Image split into independently allocated pixel and VRAM slices. */
struct SlicedImage2 {
    Image Image;
    Slice *SliceTab;
    short W;
    short H;
    short SliceW;
    short SliceH;
    short SliceCountX;
    short SliceCountY;
    void *InMemory;
    void *Packed;
    int BppShift;
    unsigned short Clut;

    /** Initialize slice dimensions, packing mode, and source data. */
    void newSlicedImage2(void *data, int width, int height,
                         int sliceWidth, int sliceHeight,
                         unsigned char bpp, unsigned short clut);
    /** Replace the copied pixel data for every image slice. */
    void setData(void *data);
    /** Draw every image slice at the requested screen position. */
    void draw(int x, int y, int drawFirst);
    /** Allocate VRAM rectangles for every slice. */
    void pack();
    /** Upload every slice to its existing VRAM rectangle. */
    void repack();
    /** Release every packed VRAM rectangle. */
    void unpack();
    /** Release every copied slice buffer while retaining the slice table. */
    void removeFromMemory();
    /** Return the scaled on-screen image width. */
    int screenWidth();
    /** Return the scaled on-screen image height. */
    int screenHeight();

#ifdef FONTTOOLS_SLICED_IMAGE_CONSTRUCTION
    /** Allocate image objects through the original CClass arena. */
    static void *operator new(unsigned int size)
    {
        return ::CClass::operator new(size);
    }

    /** Build and slice packed image data using the original image constructor. */
    SlicedImage2(void *data, int tileW, int tileH, int maxW, int maxH,
                 unsigned char bpp, unsigned short clut);
#endif
};

/** BMP-backed sliced image with no additional target storage. */
struct Sprite2 {
    SlicedImage2 SlicedImage2;

    /** Build a sprite from a bitmap file. */
    Sprite2(char *filename, int pack, int sliceWidth, int sliceHeight);
    /** Build a sprite from an in-memory bitmap. */
    Sprite2(unsigned char *buffer, int pack, int sliceWidth, int sliceHeight);
    /** Replace the sprite bitmap and refresh its palette slot. */
    void setData(void *data);

#ifndef FONTTOOLS_SLICED_IMAGE_CONSTRUCTION
    // RotateSprite embeds this base and invokes the recovered ABI constructor
    // explicitly, so the implicit construction step must remain empty.
    Sprite2() {}
#endif
};

/** Rotatable sprite extension with the target-proven 64-byte layout. */
struct RotateSprite {
    Sprite2 Sprite2;
    unsigned char AspectType;

    RotateSprite(char *filename, int pack, int slice_w, int slice_h);
    void draw(int x, int y, int rot);
};

typedef char image_Image_size_must_be_28[(sizeof(Image) == 28) ? 1 : -1];
typedef char image_SlicedImage2_size_must_be_60[(sizeof(SlicedImage2) == 60) ? 1 : -1];
typedef char image_Sprite2_size_must_be_60[(sizeof(Sprite2) == 60) ? 1 : -1];
typedef char image_RotateSprite_size_must_be_64[
    (sizeof(RotateSprite) == 64) ? 1 : -1];
typedef char image_BmpHeader_size_must_be_0x36[
    (sizeof(ImageBmpHeader) == 0x36) ? 1 : -1];
typedef char image_BmpPaletteEntry_size_must_be_4[
    (sizeof(ImageBmpPaletteEntry) == 4) ? 1 : -1];
typedef char image_SinCos_size_must_be_4[
    (sizeof(ImageSinCos) == 4) ? 1 : -1];

#endif // IMAGE_OWNER_LAYOUT && !IMAGE_OWNER_LAYOUT_EMITTED
