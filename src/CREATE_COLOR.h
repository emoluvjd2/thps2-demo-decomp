// Public color-conversion and palette-transform declarations.
#ifndef CREATE_COLOR_INC_H
#define CREATE_COLOR_INC_H

struct tag_S_Pal;

/** Hue, saturation, value, and preserved palette flags for one color. */
struct HSV_VALUE {
    int h;
    int s;
    int v;
    int flags;
};

typedef char create_color_HSV_VALUE_size_must_be_16[
    (sizeof(HSV_VALUE) == 16) ? 1 : -1
];

/** Saturation response curve used by the character color editor. */
extern "C" int saturationCurve[33];

/** Returns the least of three eight-bit color components. */
unsigned char MIN3(unsigned char r, unsigned char g, unsigned char b);

/** Returns the greatest of three eight-bit color components. */
unsigned char MAX3(unsigned char r, unsigned char g, unsigned char b);

/** Converts an RGB color to Q20 fixed-point HSV components. */
void Create_RGB2HSV(
    unsigned char r, unsigned char g, unsigned char b,
    int *pH, int *pS, int *pV);

/** Converts Q20 fixed-point HSV components to an RGB color. */
void Create_HSV2RGB(
    int h, int s, int v,
    unsigned char *pR, unsigned char *pG, unsigned char *pB);

/** Remembers a hardware palette as HSV values. */
void Create_RememberPalette(tag_S_Pal *pPal, HSV_VALUE *pDestPalette);

/** Applies HSV transforms and uploads the resulting hardware palette. */
void Create_UpdatePalette(
    tag_S_Pal *pPal, HSV_VALUE *pSourcePalette,
    unsigned short hXForm, unsigned short sXForm,
    unsigned short vXForm);

#endif // CREATE_COLOR_INC_H
