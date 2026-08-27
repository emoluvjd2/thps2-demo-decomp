#ifndef SANIMFRAME_INC_H
#define SANIMFRAME_INC_H

struct Texture;

/** One frame in a PSX sprite animation. */
struct SAnimFrame {
    signed char OffX;
    signed char OffY;
    unsigned char Width;
    unsigned char Height;
    struct Texture *pTexture;
};

typedef char SAnimFrame_size_must_be_8[
    (sizeof(struct SAnimFrame) == 8) ? 1 : -1];
typedef char SAnimFrame_OffX_offset_must_be_0[
    ((unsigned long)&(((struct SAnimFrame *)0)->OffX) == 0) ? 1 : -1];
typedef char SAnimFrame_OffY_offset_must_be_1[
    ((unsigned long)&(((struct SAnimFrame *)0)->OffY) == 1) ? 1 : -1];
typedef char SAnimFrame_Width_offset_must_be_2[
    ((unsigned long)&(((struct SAnimFrame *)0)->Width) == 2) ? 1 : -1];
typedef char SAnimFrame_Height_offset_must_be_3[
    ((unsigned long)&(((struct SAnimFrame *)0)->Height) == 3) ? 1 : -1];
typedef char SAnimFrame_pTexture_offset_must_be_4[
    ((unsigned long)&(((struct SAnimFrame *)0)->pTexture) == 4) ? 1 : -1];

#endif // SANIMFRAME_INC_H
