#ifndef TEXTURE_INC_H
#define TEXTURE_INC_H

/** Linked runtime texture descriptor managed by the spooler. */
struct Texture {
    unsigned char u0;
    unsigned char v0;
    unsigned short clut;
    unsigned char u1;
    unsigned char v1;
    unsigned short tpage;
    unsigned char u2;
    unsigned char v2;
    unsigned char u3;
    unsigned char v3;
    unsigned int TexWin;
    unsigned short Usage;
    unsigned char InVRAM : 4;
    unsigned char IsTiling : 4;
    unsigned int Checksum;
    struct tag_S_Pal *pPalette;
    unsigned short x;
    unsigned short y;
    struct Texture *pNext;
    struct Texture *pPrevious;
};

#endif // TEXTURE_INC_H
