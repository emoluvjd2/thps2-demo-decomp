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
