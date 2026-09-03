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

/** Remembers a hardware palette as HSV values. */
void Create_RememberPalette(tag_S_Pal *pPal, HSV_VALUE *pDestPalette);

/** Applies HSV transforms and uploads the resulting hardware palette. */
void Create_UpdatePalette(
    tag_S_Pal *pPal, HSV_VALUE *pSourcePalette,
    unsigned short hXForm, unsigned short sXForm,
    unsigned short vXForm);

#endif // CREATE_COLOR_INC_H
