// CREATE_COLOR.cpp -- Reconstructed translation unit
// Original: CREATE_COLOR.CPP
// Functions: 6
// Address range: 0x8006CAAC - 0x8006CE98

#include "CREATE_COLOR.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBGPU.H>

#define PAL_ENTRY_LAYOUT
#include "PAL.h"
#undef PAL_ENTRY_LAYOUT

/** Palette sizes and Q20 fixed-point limits used by this owner. */
enum CreateColorConstants {
    FullPaletteColourCount = 256,
    SmallPaletteColourCount = 16,
    Palette16ColourFlag = 1,
    NeutralSaturationScale = 256,
    NeutralValueScale = 128,
    FixedFractionBits = 20,
    FixedOne = 1 << FixedFractionBits,
    FixedMaximum = FixedOne - 1,
    HueCycle = 360 << FixedFractionBits,
    HueMaximum = HueCycle - 1,
    HSVValueWordCount = sizeof(HSV_VALUE) / sizeof(int),
    SaturationWordOffsetFromValue = -1
};

/**
 * Saturation response curve used by the character color editor.
 *
 * The 33 source-owned samples form the complete CREATE_COLOR data
 * contribution recovered from the original executable.
 */
int saturationCurve[33] = {
      8,  12,  14,  18,  24,  36,  46,  60,
     76,  94, 114, 140, 168, 192, 214, 230,
    240, 246, 250, 254, 256, 256, 254, 252,
    246, 230, 210, 192, 180, 168, 154, 130,
      0,
};

// === Function implementations (address order) ===

/**
 * Returns the least of three eight-bit color components.
 *
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 * @return The smallest component.
 */
unsigned char MIN3(unsigned char r, unsigned char g, unsigned char b) {
    char min = r;
    if (g < min) min = g;
    if (b < min) min = b;
    return min;
}


/**
 * Returns the greatest of three eight-bit color components.
 *
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 * @return The largest component.
 */
unsigned char MAX3(unsigned char r, unsigned char g, unsigned char b) {
    unsigned char max = r;
    if (max < g) max = g;
    if (max < b) max = b;
    return max;
}


/**
 * Converts an RGB color to Q20 fixed-point HSV components.
 *
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 * @param pH  Receives hue in degrees, represented as Q20 fixed point.
 * @param pS  Receives saturation in the Q20 range [0, 1].
 * @param pV  Receives value in the Q20 range [0, 255].
 */
void Create_RGB2HSV(
    unsigned char r, unsigned char g, unsigned char b,
    int *pH, int *pS, int *pV) {
    int min;
    int max;
    int delta;
    int diff;
    int rotOffset;

    min = MIN3(r, g, b);
    max = MAX3(r, g, b);

    *pV = (max << FixedFractionBits) / FullPaletteColourCount;
    delta = max - min;
    if (max != 0) {
        *pS = (delta << FixedFractionBits) / max;
    }
    if (max == 0) {
        *pS = 0;
        *pH = 0;
        return;
    }

    if (r == max) {
        diff = g - b;
        rotOffset = 0;
    } else if (g == max) {
        diff = b - r;
        rotOffset = 2;
    } else {
        diff = r - g;
        rotOffset = 4;
    }

    *pH = (
        rotOffset * FixedOne + ((diff << FixedFractionBits) / delta)) * 60;
    if (*pH < 0) {
        *pH += HueCycle;
    }
}


/**
 * Converts Q20 fixed-point HSV components to an RGB color.
 *
 * @param h  Hue in degrees, represented as Q20 fixed point.
 * @param s  Saturation in the Q20 range [0, 1].
 * @param v  Value in the Q20 range [0, 255].
 * @param pR  Receives the red component (0-255).
 * @param pG  Receives the green component (0-255).
 * @param pB  Receives the blue component (0-255).
 */
void Create_HSV2RGB(
    int h, int s, int v,
    unsigned char *pR, unsigned char *pG, unsigned char *pB) {
    int i;
    int f;
    int p;
    int q;
    int t;

    if (s == 0) {
        v >>= 12;
        *pB = v;
        *pG = v;
        *pR = v;
        return;
    }

    h /= 60;
    f = h & FixedMaximum;
    p = (v >> 10) * ((FixedOne - s) >> 10);
    q = (v >> 10) * ((FixedOne - ((s >> 10) * (f >> 10))) >> 10);
    t = (v >> 10) * (
        (FixedOne - ((s >> 10) * ((FixedOne - f) >> 10))) >> 10);
    v >>= 12;
    p >>= 12;
    q >>= 12;
    t >>= 12;
    i = static_cast<unsigned int>(h) >> FixedFractionBits;

    switch (i) {
    case 0:
        *pR = v;
        *pG = t;
        *pB = p;
        break;
    case 1:
        *pR = q;
        *pG = v;
        *pB = p;
        break;
    case 2:
        *pR = p;
        *pG = v;
        *pB = t;
        break;
    case 3:
        *pR = p;
        *pG = q;
        *pB = v;
        break;
    case 4:
        *pR = t;
        *pG = p;
        *pB = v;
        break;
    case 5:
        *pR = v;
        *pG = p;
        *pB = q;
        break;
    }
}


/**
 * Copies a CLUT into HSV entries for later palette transforms.
 *
 * @param pPal  Palette structure.
 * @param pDestPalette  Destination HSV entries.
 */
void Create_RememberPalette(tag_S_Pal *pPal, HSV_VALUE *pDestPalette) {
    unsigned short colour;
    // Interior induction pointers reproduce the target's field-wise loop.
    int *pValueField;
    int i;
    unsigned short *pEntry16;
    int *pFlagsField;
    int PalSize;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned short TempPalette[256];
    HSV_VALUE *pEntry32;
    int h;
    int s;
    int v;

    pEntry32 = pDestPalette;
    pEntry16 = TempPalette;
    Pal_CopyToMemory(pPal, pEntry16);
    DrawSync(0);
    PalSize = FullPaletteColourCount;
    if ((pPal->flags & Palette16ColourFlag) != 0) {
        PalSize = SmallPaletteColourCount;
    }
    i = 0;
    if (PalSize != 0) {
        pFlagsField = &pEntry32->flags;
        pValueField = &pEntry32->v;
        do {
            colour = *pEntry16;
            r = (colour & 0x1f) << 3;
            g = (colour >> 2) & 0xf8;
            b = (colour >> 7) & 0xf8;
            if (i < FullPaletteColourCount) {
                h = 0;
                s = 0;
                v = 0;
                Create_RGB2HSV(r, g, b, &h, &s, &v);
                pEntry32->h = h;
                pValueField[SaturationWordOffsetFromValue] = s;
                *pValueField = v;
                *pFlagsField = colour & 0x8000;
            }
            pEntry16++;
            pValueField += HSVValueWordCount;
            pFlagsField += HSVValueWordCount;
            pEntry32++;
            i++;
        } while (i < PalSize);
    }
}


/**
 * Rebuilds a CLUT from remembered HSV values after applying transforms.
 *
 * @param pPal  Palette structure.
 * @param pSourcePalette  Source HSV entries.
 * @param hXForm  Hue offset in whole degrees.
 * @param sXForm  Saturation scale; 256 is neutral.
 * @param vXForm  Value scale; 128 is neutral.
 */
void Create_UpdatePalette(
    tag_S_Pal *pPal, HSV_VALUE *pSourcePalette,
    unsigned short hXForm, unsigned short sXForm,
    unsigned short vXForm) {
    unsigned short TempPalette[256];
    HSV_VALUE *pEntry32 = pSourcePalette;
    unsigned short *pEntry16 = TempPalette;
    int PalSize = FullPaletteColourCount;

    if ((pPal->flags & Palette16ColourFlag) != 0) {
        PalSize = SmallPaletteColourCount;
    }

    for (int i = 0; i < PalSize; i++) {
        HSV_VALUE bgr = *pEntry32;
        int h = bgr.h;
        int s = bgr.s;
        int v = bgr.v;
        pEntry32++;

        if (i < FullPaletteColourCount) {
            h += hXForm << FixedFractionBits;

            s = (s >> 10) * (
                ((sXForm << FixedFractionBits) / NeutralSaturationScale) >> 10);
            v = (v >> 10) * (
                ((vXForm << FixedFractionBits) / NeutralValueScale) >> 10);
        }

        if (h < 0) {
            h += HueCycle;
        }
        if (h > HueMaximum) {
            h -= HueCycle;
            if (h > HueMaximum) {
                h -= HueCycle;
            }
        }

        if (v > FixedMaximum) {
            v = FixedMaximum;
        }
        if (v < 0) {
            v = 0;
        }
        if (s > FixedMaximum) {
            s = FixedMaximum;
        }
        if (s < 0) {
            s = 0;
        }

        unsigned char r;
        unsigned char g;
        unsigned char b;
        Create_HSV2RGB(h, s, v, &r, &g, &b);
        r >>= 3;
        g >>= 3;
        b >>= 3;

        if (r == 0) {
            r = 1;
        }
        if (i >= FullPaletteColourCount) {
            r = 0;
            g = 0;
            b = 0;
        }

        *pEntry16++ =
            (bgr.flags & 0x8000) + (b << 10) + (g << 5) + r;
    }

    Pal_StoreFromMemory(pPal, TempPalette);
    DrawSync(0);
}
