// FONTTOOLS.cpp -- Reconstructed translation unit
// Original: FONTTOOLS.CPP
// Functions: 21
// Address range: 0x8005ACB8 - 0x8005C228
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#define FONTTOOLS_SLICED_IMAGE_CONSTRUCTION
#define FONTTOOLS_OWNER_DECLARATIONS

#include "FONTTOOLS.h"
#define IMAGE_OWNER_LAYOUT
#include "IMAGE.h"
#undef IMAGE_OWNER_LAYOUT
#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBGPU.H>

/** Dispatch table entries used by FONTTOOLS for a text effect. */
struct FontToolsTextEffectVTable {
    int baseOffset;
    void *typeInfo;
    void (*destroy)(TextEffect *effect, int inCharge);
    void *reserved3;
    void *reserved4;
    void *reserved5;
    void *reserved6;
    void (*modifyPosition)(TextEffect *effect, int *x, int *y);
    void (*modifyColour)(TextEffect *effect, u8 *r, u8 *g, u8 *b);
    void *reserved9;
    void (*afterDraw)(TextEffect *effect, int preX, int preY,
                      int postX, int postY);
};

#include "TEXTEFFECT.h"

/** Dispatch prefix used by the image operations in this owner. */
struct FontToolsImageVTable {
    int baseOffset;
    void *typeInfo;
    void (*destroy)(SlicedImage2 *image, int inCharge);
    void *reserved3;
    void *reserved4;
    void (*draw)(SlicedImage2 *image, int x, int y, int depth);
};

/** Recover the typed image dispatch table from the shared CClass prefix. */
inline FontToolsImageVTable *FontTools_GetImageVTable(SlicedImage2 *image)
{
    return reinterpret_cast<FontToolsImageVTable *>(
        image->Image.CClass.vtable);
}

/**
 * Address one glyph while preserving the target's scaled-index-first add.
 * GCC reverses the operands for ordinary pointer indexing in these loops.
 */
inline FontCharacter *FontTools_GetCharacter(FontCharacter *table, int index)
{
    return reinterpret_cast<FontCharacter *>(
        index * static_cast<int>(sizeof(FontCharacter)) +
        reinterpret_cast<int>(table));
}

/** Preserve FONTTOOLS' scaled-index-first add for a font-table slot. */
inline Font **FontTools_GetFontSlot(Font **table, int index)
{
    return reinterpret_cast<Font **>(
        index * static_cast<int>(sizeof(Font *)) +
        reinterpret_cast<int>(table));
}

// These packed-stream accesses must remain unaligned native loads.  Function
// wrappers are not inlined by the original compiler, so keep the cast at this
// owner-local boundary while giving call sites a data-oriented spelling.
#define FontTools_ReadInt(data) (*reinterpret_cast<int *>(data))
#define FontTools_ReadU16(data) (*reinterpret_cast<u16 *>(data))

/** Read the low byte of a word as stored by the little-endian target. */
inline u8 FontTools_LowByte(const int &word)
{
    return *reinterpret_cast<const u8 *>(&word);
}

/** Read the low halfword of a word as stored by the little-endian target. */
inline u16 FontTools_LowHalf(const int &word)
{
    return *reinterpret_cast<const u16 *>(&word);
}

// C++ APIs retain their compiler-generated Psy-Q names.
#include "FILEIO.h"
#include "PAL.h"

// Owner-ABI entry points retain the linker names recorded in the target
// object. The destructor alias stays at the ABI boundary.
extern "C" void FontTools_DestroyFont(Font *font, int inCharge)
    asm("_._4Font");
extern "C" void FontTools_DrawAbi(
    Font *font, int x, int y, char *str, int drawFirst)
    asm("draw__4FontiiPci");

typedef char FontTools_TextEffect_size_must_be_8[
    (sizeof(TextEffect) == 8) ? 1 : -1];

#undef FONTTOOLS_OWNER_DECLARATIONS
#include "MEM.h"
#include <STRINGS.H>

/**
 * Names and loaded-font pointers owned by FontManager.
 *
 * The original object places both zero-initialized tables in one contiguous
 * 160-byte `.data` contribution rather than common/BSS storage.
 */
char FontManager::FontName[8][16] __attribute__((section(".data"))) = {{0}};
Font *FontManager::FontTab[8] __attribute__((section(".data"))) = {0};
int FontManager::dumb = 0;

// === Function implementations (address order) ===

/**
 * Initialize an empty font.
 *
 * Address:   0x8005ACB8
 * Mangled:   __4Font
 * Demangled: Font::Font(void)
 *            Sets the vtable ($gp+0x2a88) and marks the Clut slot unused.
 *
 */
// GCC 2.x emits a private copy of `CClass::~CClass` beside a private base
// vtable in every owner that derives from CClass.  `MAIN.SYM` records 25 such
// `_._6CClass` copies and names no base vtable, so both are file-local here.
// The filesystem-safe spelling keeps the per-address target assembly reachable.
static void FontTools_ClassDestructor(CClass *base, int __in_chrg)
    asm("__DOT__6CClass_8005C228");

// Two owner-local strings sit between the manager scalar and the vtables in
// the target's small-data contribution.  No surviving code references them.
static char FontTools_DefaultFontName[12] __attribute__((section(".sdata"))) =
    "mainf.fnt";
static char FontTools_Greeting[12] __attribute__((section(".sdata"))) =
    "Hello world";

// Consecutive eight-byte-aligned small-data words per vtable rather than
// arrays, so -G8 still yields the target's `addiu $vX, $gp, off` form.
// GCC 2.x vtable layout for CClass-derived types: word [0] base offset,
// word [1] RTTI slot, word [2] virtual destructor, words [3..] virtual
// methods in declaration order.
extern "C" int __vt_4Font
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int FontTools_VTableIndex = 0;
static void (*FontTools_VTableDtor)(Font *, int) = FontTools_DestroyFont;
static void (*FontTools_VTableDraw)(Font *, int, int, char *, int) =
    FontTools_DrawAbi;

static int __vt_base_4Font
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int FontTools_BaseVTableIndex = 0;
static void (*FontTools_BaseVTableDtor)(CClass *, int) =
    FontTools_ClassDestructor;
Font::Font()
{
    CClass.vtable = &__vt_4Font;
    Clut = -1;
}

/**
 * Construct a font from its packed resource data.
 *
 * Address:   0x8005ACD0
 * Mangled:   __4FontPUc
 * Demangled: Font::Font(unsigned char *)
 *            Initializes font rendering defaults, uploads the embedded 16-color
 *            palette, creates glyph images, and builds the fallback glyph.
 *
 * @param buf   Packed font data.
 */
Font::Font(unsigned char *buf)
{
    int clutx;
    int cluty;
    unsigned short *colors;
    RECT r;
    unsigned char *db;
    unsigned char *tb;
    int image_w;
    int crap_size;
    unsigned char *crap;

    CClass.vtable = &__vt_4Font;
    JustX = 0;
    JustY = 0;
    R = 0x80;
    G = 0x80;
    B = 0x80;
    SpaceX = 4;
    SpaceY = 4;
    Aliased = 0;
    Shadow = reinterpret_cast<void *>(1);
    ShadowDx = 4;
    ShadowDy = 4;
    CharMap = 0;
    FixedWidth = 0;
    Scale = 0x1000;
    CurrentTextEffect = 0;

    NumChars = FontTools_ReadInt(buf);
    CharTab = static_cast<FontCharacter *>(
        Mem_New((NumChars + 1) * sizeof(FontCharacter), 1, 1));

    clutx = Pal16X;
    cluty = Pal16Y + GetFree16Slot();
    colors = static_cast<unsigned short *>(
        Mem_New(16 * sizeof(unsigned short), 1, 1));
    {
        int i = 0;
        unsigned short *colorEntry = colors;
        do
        {
            *colorEntry = FontTools_ReadU16(
                buf + NumChars * sizeof(FontCharacter) + 4 + i * 2);
            i++;
            colorEntry++;
        } while (i < 16);
    }

    r.x = clutx;
    r.y = cluty;
    r.w = 16;
    r.h = 1;
    LoadImage(&r, reinterpret_cast<unsigned long *>(colors));
    Mem_Delete(colors);
    Clut = GetClut(clutx, cluty);

    tb = buf + 4;
    int imageDataOffset = NumChars * sizeof(FontCharacter) + 4;
    db = &buf[imageDataOffset] + 32;
    unsigned char *firstCharacter = tb;
    {
        int i = 0;
        FontCharacter *characterTable = CharTab;
        unsigned char *heightField;
        unsigned char *baselineField;
        if (NumChars > 0)
        {
            baselineField = buf + 12;
            heightField = buf + 8;
            do
            {
                characterTable[i].W = FontTools_ReadInt(heightField + 8);
                CharTab[i].H = FontTools_ReadInt(heightField);
                CharTab[i].Baseline = FontTools_ReadInt(baselineField);

                int image_w = FontTools_ReadInt(tb);
                CharTab[i].Image = new SlicedImage2(
                    db, image_w * 4, CharTab[i].H, 0, 0, 4, Clut);

                CharTab[i].Image->pack();
                CharTab[i].Image->removeFromMemory();
                CharTab[i].Image->Image.JustX = 0;
                CharTab[i].Image->Image.JustY = 0;
                CharTab[i].Image->Image.Shaded = reinterpret_cast<void *>(1);

                characterTable = CharTab;
                int pixelCount = image_w * characterTable[i].H;
                int oddPixelPadding = pixelCount & 1;
                oddPixelPadding <<= 1;
                pixelCount <<= 1;
                pixelCount += oddPixelPadding;
                heightField += sizeof(FontCharacter);
                baselineField += sizeof(FontCharacter);
                tb += sizeof(FontCharacter);
                db += pixelCount;
                i++;
            } while (i < NumChars);
        }
    }

    CharTab[NumChars].W = FontTools_ReadInt(firstCharacter + 12);
    CharTab[NumChars].H = FontTools_ReadInt(firstCharacter + 4);
    CharTab[NumChars].Baseline = FontTools_ReadInt(firstCharacter + 8);

    image_w = FontTools_ReadInt(buf + 4);
    crap_size = image_w * (CharTab[NumChars].H << 1);
    crap = static_cast<unsigned char *>(Mem_New(crap_size, 1, 1));
    {
        int i = 0;
        if (crap_size > 0)
        {
            /*
             * GCC otherwise sinks the byte store into the loop delay slot.
             * The target reuses $v0 for the address and loop condition; clean
             * do/while, while, for, and scoped-do forms were all measured.
             */
            register int moreBytes asm("$2");
            do
            {
                crap[i] = 0x12;
                i++;
                moreBytes = (i < crap_size);
            } while (moreBytes);
        }
    }

    CharTab[NumChars].Image = new SlicedImage2(
        crap, image_w * 4, CharTab[NumChars].H,
        0, 0, 4, Clut);
    Mem_Delete(crap);
    CharTab[NumChars].Image->pack();
    CharTab[NumChars].Image->removeFromMemory();
    CharTab[NumChars].Image->Image.JustX = 0;
    CharTab[NumChars].Image->Image.JustY = 0;
    CharTab[NumChars].Image->Image.Shaded = reinterpret_cast<void *>(1);
}

/**
 * Destroy a font through the GCC 2.x in-charge ABI.
 *
 * Address:   0x8005B150
 * Mangled:   _._4Font
 * Demangled: Font::~Font()
 *
 * A natural destructor does not emit the target's hidden delete path, so this
 * typed owner-ABI boundary retains the compiler's in-charge parameter.
 *
 * @param self       Font being destroyed.
 * @param __in_chrg  Bit 0 requests `operator delete` on the object.
 */
void FontTools_DestroyFont(Font *font, int __in_chrg)
{
    font->CClass.vtable = &__vt_base_4Font;
    if (__in_chrg & 1)
    {
        ::CClass::operator delete(font);
    }
}

/**
 *
 * Address:   0x8005B180
 * Mangled:   draw__4FontiiPci
 * Demangled: Font::draw(int x, int y, char *str, int depth)
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param str  Input string.
 * @param drawFirst  Draw-order flag forwarded to each glyph sprite.
 */
void Font::draw(int x, int y, char *str, int drawFirst)
{
    int strWidth;
    if (FixedWidth == 1)
    {
        strWidth = this->fixedWidth(str);
    }
    else
    {
        strWidth = width(str);
    }

    int xstart;
    {
        switch (JustX)
        {
        case 0:
            xstart = x;
            break;
        case 1:
            xstart = x - (strWidth >> 1);
            break;
        case 2:
            xstart = x - strWidth;
            break;
        default:
            break;
        }
    }

    int ystart;
    {
        switch (JustY)
        {
        case 0:
            ystart = y + heightAboveBaseline(str);
            break;
        case 1:
            ystart = y - (height(str) >> 1) + heightAboveBaseline(str);
            break;
        case 3:
            ystart = y;
            break;
        case 2:
            ystart = y - height(str) + heightAboveBaseline(str);
            break;
        default:
            break;
        }
    }

    int xt = xstart << 12;
    int yt = ystart << 12;
    char *ltr = str;
    if (*ltr == 0)
        return;

    do
    {
        int i = getCharIndex(*ltr);
        if (i == -1)
        {
            if (isEscapeChar(*ltr))
            {
                handleEscapeChar(*ltr);
                goto next;
            }

            xt += CharTab[0].W * Scale;
        }
        else
        {
            int ScreenX;
            int ScreenY;
            int PostEffectX;
            int PostEffectY;
            int PreEffectX;
            int PreEffectY;

            CharTab[i].Image->Image.Scale = Scale;
            CharTab[i].Image->Image.Transparent = 0;
            CharTab[i].Image->Image.TransType = 1;
            CharTab[i].Image->Image.R = static_cast<u8>(R);
            CharTab[i].Image->Image.G = static_cast<u8>(G);
            CharTab[i].Image->Image.B = static_cast<u8>(B);

            int aboveScaled = CharTab[i].Baseline * Scale;
            ScreenX = xt;
            ScreenY = yt - aboveScaled;

            TextEffect *colourEffect = CurrentTextEffect;
            if (colourEffect != 0)
            {
                PreEffectX = ScreenX;
                PreEffectY = ScreenY;
                SlicedImage2 *sprite = CharTab[i].Image;
                FontToolsTextEffectVTable *colourVTable =
                    reinterpret_cast<FontToolsTextEffectVTable *>(
                        colourEffect->CClass.vtable);
                void (*modifyColour)(TextEffect *, u8 *, u8 *, u8 *) =
                    colourVTable->modifyColour;
                modifyColour(
                    colourEffect, &sprite->Image.R, &sprite->Image.G,
                    &sprite->Image.B);
                TextEffect *positionEffect = CurrentTextEffect;
                FontToolsTextEffectVTable *positionVTable =
                    reinterpret_cast<FontToolsTextEffectVTable *>(
                        positionEffect->CClass.vtable);
                void (*modifyPosition)(TextEffect *, int *, int *) =
                    positionVTable->modifyPosition;
                modifyPosition(positionEffect, &ScreenX, &ScreenY);
                PostEffectX = ScreenX;
                PostEffectY = ScreenY;
            }

            if (FixedWidth != 0)
            {
                ScreenX += (fixedCharWidth(*ltr) << 12) - CharTab[i].W * Scale;
                xt += fixedCharWidth(*ltr) << 12;
            }
            else
            {
                xt += (CharTab[i].W + SpaceX) * Scale;
            }

            {
                SlicedImage2 *sprite = CharTab[i].Image;
                FontToolsImageVTable *vtbl = FontTools_GetImageVTable(sprite);
                void (*drawFn)(SlicedImage2 *, int, int, int) = vtbl->draw;
                drawFn(sprite, ScreenX >> 12, ScreenY >> 12, drawFirst);
            }

            if (Aliased != 0)
            {
                CharTab[i].Image->Image.Transparent =
                    reinterpret_cast<void *>(1);
                CharTab[i].Image->Image.TransType = 2;
                SlicedImage2 *sprite = CharTab[i].Image;
                FontToolsImageVTable *vtbl = FontTools_GetImageVTable(sprite);
                void (*drawFn)(SlicedImage2 *, int, int, int) = vtbl->draw;
                drawFn(sprite, ScreenX >> 12, ScreenY >> 12, drawFirst);
            }

            if (Shadow != 0)
            {
                CharTab[i].Image->Image.Transparent =
                    reinterpret_cast<void *>(1);
                CharTab[i].Image->Image.TransType = 2;
                CharTab[i].Image->Image.R = 0xFF;
                CharTab[i].Image->Image.G = 0xFF;
                CharTab[i].Image->Image.B = 0xFF;

                SlicedImage2 *sprite = CharTab[i].Image;
                FontToolsImageVTable *vtbl = FontTools_GetImageVTable(sprite);
                void (*drawFn)(SlicedImage2 *, int, int, int) = vtbl->draw;
                ScreenX += ShadowDx * Scale;
                ScreenY += ShadowDy * Scale;
                drawFn(sprite, ScreenX >> 12, ScreenY >> 12, drawFirst);
            }

            TextEffect *effect = CurrentTextEffect;
            if (effect != 0)
            {
                reinterpret_cast<FontToolsTextEffectVTable *>(
                    effect->CClass.vtable)->afterDraw(
                        effect, PreEffectX, PreEffectY, PostEffectX,
                        PostEffectY);
            }
        }
    next:
        ltr++;
    } while (*ltr != 0);
}

/**
 *
 * Address:   0x8005B704
 * Mangled:   width__4FontPc
 * Demangled: Font::width(char *)
 *            Returns total width of string, scaled by font scale >> 12
 *
 * @param str  Input string.
 * @return Scaled pixel width of the whole string.
 */
int Font::width(char *str)
{
    int w = 0;
    if (*str != 0)
    {
        do
        {
            int i = getCharIndex(*str);
            if (i != -1)
                goto found;
            {
                int esc = isEscapeChar(*str);
                if (esc != 0)
                    goto next;
                w += CharTab[0].W;
                goto next;
            }
        found:
        {
            // Scaled-index-first sum (not &charTable[idx]) keeps the
            // target's `addu` operand order.
            w += SpaceX + FontTools_GetCharacter(CharTab, i)->W;
        }
        next:
            str++;
        } while (*str != 0);
    }
    return (w * Scale) >> 12;
}

/**
 * Return a string's total height.
 *
 * Address:   0x8005B7D4
 * Mangled:   height__4FontPc
 * Demangled: Font::height(char *)
 *            Returns heightAboveBaseline + heightBelowBaseline
 *
 * @param str  Input string.
 * @return above + below
 */
int Font::height(char *str)
{
    return heightAboveBaseline(str) + heightBelowBaseline(str);
}

/**
 *
 * Address:   0x8005B820
 * Mangled:   heightAboveBaseline__4FontPc
 * Demangled: Font::heightAboveBaseline(char *)
 *            Returns max heightAboveBaseline across all chars in string, scaled by font scale >> 12
 *
 * @param str  Input string.
 * @return Scaled maximum ascent of the string.
 */
int Font::heightAboveBaseline(char *str)
{
    int max_h = 0;
    if (*str != 0)
    {
        do
        {
            int i = getCharIndex(*str);
            if (i != -1)
            {
                // Scaled-index-first sum keeps the target's `addu` order.
                FontCharacter *entry = FontTools_GetCharacter(CharTab, i);
                if (max_h < entry->Baseline)
                {
                    max_h = entry->Baseline;
                }
            }
            str++;
        } while (*str != 0);
    }
    return (max_h * Scale) >> 12;
}

/**
 *
 * Address:   0x8005B8D4
 * Mangled:   heightBelowBaseline__4FontPc
 * Demangled: Font::heightBelowBaseline(char *)
 *            Returns max heightBelowBaseline across all chars in string, scaled by font scale >> 12
 *
 * @param str  Input string.
 * @return Scaled maximum descent of the string.
 */
int Font::heightBelowBaseline(char *str)
{
    int max_h = 0;
    if (*str != 0)
    {
        do
        {
            int i = getCharIndex(*str);
            if (i != -1)
            {
                // Scaled-index-first sum keeps the target's `addu` order.
                FontCharacter *entry = FontTools_GetCharacter(CharTab, i);
                if (max_h < entry->H - entry->Baseline)
                {
                    max_h = entry->H - entry->Baseline;
                }
            }
            str++;
        } while (*str != 0);
    }
    return (max_h * Scale) >> 12;
}

/**
 *
 * Address:   0x8005B98C
 * Mangled:   getCharIndex__4Fontc
 * Demangled: Font::getCharIndex(char)
 *            Maps character to font index based on mode (0=standard, 1=numbers, 2=lowercase-first)
 *
 * @param asc  Character to map.
 * @return Glyph index, or -1 for space/escape characters.
 */
int Font::getCharIndex(char asc)
{
    int myCharMap = CharMap;
    if (myCharMap == 2)
    {
        if (static_cast<u8>(asc - 'a') < 26)
            return asc - 'a' + 48;
        myCharMap = 0;
    }
    if (myCharMap != 0)
        goto mode1;
    // Mode 0: standard character mapping
    {
        if (static_cast<u8>(asc - 'A') < 26)
            return asc - 'A';
        if (static_cast<u8>(asc - 'a') < 26)
            return asc - 'a';
        if ((unsigned)(asc - '0') < 10)
            return asc - '0' + 26;
        if (asc == ' ')
            return -1;
        if (asc == '?')
        {
            if (NumChars >= 37)
                return 36;
        }
        if (asc == '!')
        {
            if (NumChars >= 38)
                return 37;
        }
        if (asc == ':')
        {
            if (NumChars >= 39)
                return 38;
        }
        if (asc == '.')
        {
            if (NumChars >= 40)
                return 39;
        }
        if (asc == ',')
        {
            if (NumChars >= 41)
                return 40;
        }
        if (asc == '-')
        {
            if (NumChars >= 42)
                return 41;
        }
        if (asc == '%')
        {
            if (NumChars >= 43)
                return 42;
        }
        if (asc == '+')
        {
            if (NumChars >= 44)
                return 43;
        }
        if (asc == '\'')
        {
            if (NumChars >= 45)
                return 44;
        }
        if (asc == '#')
        {
            if (NumChars >= 46)
                return 45;
        }
        if (asc == '/')
        {
            if (NumChars >= 47)
                return 46;
        }
        if (asc == '$')
        {
            if (NumChars >= 48)
                return 47;
        }
        goto escape_check;
    }
mode1:
    if (myCharMap != 1)
        goto escape_check;
    {
        if (static_cast<u8>(asc - '0') < 10)
            return asc - '0';
        if (asc == ':')
            return 10;
        if (asc == ' ')
            return -1;
    }
escape_check:
{
    int esc = isEscapeChar(asc);
    if (esc != 0)
        return -1;
    return NumChars;
}
}

/**
 * Decode a text-formatting escape character.
 *
 * Address:   0x8005BC20
 * Mangled:   isEscapeChar__4Fontc
 * Demangled: Font::isEscapeChar(char)
 *            Maps escape characters to replacement codes (0xFB-0xFE), returns 0 if not escape
 *
 * @param asc  Character to test.
 * @return Escape code (0xF8-0xFE), or 0 if not an escape character.
 */
int Font::isEscapeChar(char asc)
{
    if (asc == ']')
        return 0xFE;
    if (asc == '[')
        return 0xFD;
    if (asc == '}')
        return 0xFE;
    if (asc == '{')
        return 0xFC;
    if (asc == '>')
        return 0xFE;
    if (asc == '<')
        return 0xFB;
    if (asc == '`')
        return 0xFE;
    if (asc == '|')
        return 0xF9;
    if (asc == '&')
        return 0xFE;
    if (asc == '~')
        return 0xF8;
    if (asc == '^')
        return 0xFA;
    return 0;
}

/**
 *
 * Address:   0x8005BCC0
 * Mangled:   handleEscapeChar__4Fontc
 * Demangled: Font::handleEscapeChar(char)
 *            Handles escape character sequences for color changes
 *
 * The original reads back only the low byte of each 32-bit color field when
 * saving into OldR/OldG/OldB (`lbu` on a little-endian word).
 *
 * @param asc  Escape character to apply.
 */
void Font::handleEscapeChar(char asc)
{
    int escCode = isEscapeChar(static_cast<u8>(asc));
    if (escCode == 0xFA)
        goto done;
    if (escCode == 0xFE)
    {
        int r = OldR;
        int g = OldG;
        int b = OldB;
        R = r;
        G = g;
        B = b;
        goto done;
    }
    if (escCode == 0xFD)
    {
        int oldR = FontTools_LowByte(R);
        int oldG = FontTools_LowByte(G);
        int oldB = FontTools_LowByte(B);
        R = 0;
        G = 0x80;
        B = 0xFF;
        OldR = oldR;
        OldG = oldG;
        OldB = oldB;
        goto done;
    }
    if (escCode == 0xFC)
    {
        int oldR = FontTools_LowByte(R);
        int oldG = FontTools_LowByte(G);
        int oldB = FontTools_LowByte(B);
        R = 0xFF;
        G = 0x80;
        B = 0;
        OldR = oldR;
        OldG = oldG;
        OldB = oldB;
        goto done;
    }
    if (escCode == 0xFB)
    {
        int oldR = FontTools_LowByte(R);
        int oldG = FontTools_LowByte(G);
        int oldB = FontTools_LowByte(B);
        R = 0x40;
        G = 0x40;
        B = 0x40;
        OldR = oldR;
        OldG = oldG;
        OldB = oldB;
        goto done;
    }
    if (escCode == 0xF9)
    {
        int oldR = FontTools_LowByte(R);
        int oldG = FontTools_LowByte(G);
        int oldB = FontTools_LowByte(B);
        R = 0x80;
        G = 0x40;
        B = 0x40;
        OldR = oldR;
        OldG = oldG;
        OldB = oldB;
        goto done;
    }
    if (escCode != 0xF8)
        goto done;
    {
        int oldR = FontTools_LowByte(R);
        int oldG = FontTools_LowByte(G);
        int oldB = FontTools_LowByte(B);
        R = 0x40;
        G = 0x80;
        B = 0x40;
        OldR = oldR;
        OldG = oldG;
        OldB = oldB;
    }
done:;
}

/**
 * Measure a string with fixed glyph advances.
 *
 * Address:   0x8005BE14
 * Mangled:   fixedWidth__4FontPc
 * Demangled: Font::fixedWidth(char *)
 *            Sums fixedCharWidth for each character in the string
 *
 * @param pString  Input string.
 * @return width
 */
int Font::fixedWidth(char *pString)
{
    int width = 0;
    if (*pString == 0)
        goto done;
    do
    {
        int w = fixedCharWidth(*pString);
        pString++;
        width += w;
    } while (*pString != 0);
done:
    return width;
}

/**
 *
 * Address:   0x8005BE7C
 * Mangled:   fixedCharWidth__4Fontc
 * Demangled: Font::fixedCharWidth(char)
 *            Measure a single glyph using the fixed-width table.
 *
 * In number mode (CharMap == 1) a ':' is measured from its own glyph entry
 * (CharTab[10]); every other glyph is measured from the first entry.
 *
 * @param asc  Character to measure.
 * @return Scaled fixed advance of the glyph.
 */
int Font::fixedCharWidth(char asc)
{
    int charWidth = 0;
    unsigned char character = asc;
    if (isEscapeChar(character) != 0)
    {
        return 0;
    }
    if (CharMap == 0)
        goto defwidth;
    if (CharMap == 2)
        goto defwidth;
    if (CharMap != 1)
        goto done;
    if (character != ':')
        goto defwidth;
    charWidth = (CharTab[10].W + SpaceX) * Scale;
    goto done;
defwidth:
    charWidth = (CharTab[0].W + SpaceX) * Scale;
done:
    return charWidth >> 12;
}

/**
 * Set the current text color.
 *
 * Address:   0x8005BF24
 * Mangled:   setRGB__4Fontiii
 * Demangled: Font::setRGB(int, int, int)
 *
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 */
void Font::setRGB(int r, int g, int b)
{
    R = r;
    G = g;
    B = b;
}

/**
 * Attach the effect used while drawing text.
 *
 * Address:   0x8005BF34
 * Mangled:   SetTextEffect__4FontP10TextEffect
 * Demangled: Font::SetTextEffect(TextEffect *)
 *
 * @param pEffect  Effect to operate on.
 */
void Font::SetTextEffect(TextEffect *pEffect)
{
    CurrentTextEffect = pEffect;
}

/**
 * unload__4Font — Release the font slot, then destroy and free glyph storage
 *
 * Address:   0x8005BF3C
 * Mangled:   unload__4Font
 * Demangled: Font::unload(void)
 *
 * Free16Slot takes only the low half of the 32-bit Clut word (`lhu`).
 */
void Font::unload()
{
    if (Clut != -1)
    {
        Free16Slot(FontTools_LowHalf(Clut));
    }
    int i = 0;
    if (NumChars + 1 <= 0)
        goto done;
    do
    {
        if (CharTab[i].Image != 0)
        {
            FontTools_GetImageVTable(CharTab[i].Image)->destroy(
                CharTab[i].Image, 3);
        }
        i++;
    } while (i < NumChars + 1);
done:
    Mem_Delete(CharTab);
}

/**
 * Load and register a font file.
 *
 * Address:   0x8005BFEC
 * Mangled:   LoadFont__11FontManagerPc
 * Demangled: FontManager::LoadFont(char *)
 *            Opens file, allocates memory, loads data, calls LoadFont(data, name), frees data
 *
 * @param filename  Filename string.
 * @return font
 */
Font *FontManager::LoadFont(char *filename)
{
    int handle = FileIO_Open(filename);
    unsigned char *pData = static_cast<unsigned char *>(Mem_New(handle, 1, 1));
    FileIO_Load(pData);
    FileIO_Sync();
    Font *loadedFont = FontManager::LoadFont(pData, filename);
    Mem_Delete(pData);
    return loadedFont;
}

/**
 *
 * Address:   0x8005C058
 * Mangled:   LoadFont__11FontManagerPUcPc
 * Demangled: FontManager::LoadFont(unsigned char *, char *)
 *            Finds free slot in FontTab, creates Font from data, stores name and pointer
 *
 * @param pData  Packed font data.
 * @param fontname  Name to register the font under.
 * @return Loaded font pointer, or 0 on allocation failure.
 */
Font *FontManager::LoadFont(unsigned char *pData, char *fontname)
{
    int slot = 0;
    Font **fontTab = FontManager::FontTab;
    Font **candidate = fontTab;
    do
    {
        if (*candidate == 0)
            goto found;
        slot++;
        candidate++;
    } while (slot < 8);
found:
{
    Font **fontSlot = FontTools_GetFontSlot(fontTab, slot);
    *fontSlot = new Font(pData);
    strcpy(FontManager::FontName[slot], fontname);
    return *fontSlot;
}
}

/**
 * Find a registered font by name.
 *
 * Address:   0x8005C10C
 * Mangled:   GetFont__11FontManagerPc
 * Demangled: FontManager::GetFont(char *)
 *            Searches FontName[0..7] for name match, returns FontTab[slot]
 *
 * @param fontname  Name of a previously loaded font.
 * @return Matching font pointer.
 */
Font *FontManager::GetFont(char *fontname)
{
    int slot = 0;
    char *registeredName = &FontManager::FontName[0][0];
    do
    {
        if (strcmp(registeredName, fontname) == 0)
            goto found;
        slot++;
        registeredName += sizeof(FontManager::FontName[0]);
    } while (slot < 8);
found:
    return FontManager::FontTab[slot];
}

/**
 *
 * Address:   0x8005C180
 * Mangled:   UnloadFont__11FontManagerPc
 * Demangled: FontManager::UnloadFont(char *)
 *            Finds font by name in FontName, calls unload, destroys, clears slot
 *
 * @param fontname  Name of a previously loaded font.
 */
void FontManager::UnloadFont(char *fontname)
{
    int slot = 0;
    char *registeredName = &FontManager::FontName[0][0];
    do
    {
        if (strcmp(registeredName, fontname) == 0)
            goto found;
        slot++;
        if (slot >= 8)
            goto found;
        registeredName += sizeof(FontManager::FontName[0]);
    } while (1);
found:
{
    Font **fontSlot = FontTools_GetFontSlot(FontManager::FontTab, slot);
    (*fontSlot)->unload();
    if (*fontSlot != 0)
    {
        FontDispatch(*fontSlot)->destroy(*fontSlot, 3);
    }
    *fontSlot = 0;
}
}

// GCC emitted this private `CClass::~CClass` copy last in the owner's
// text, so it stays at the end of the translation unit.
static void FontTools_ClassDestructor(CClass *base, int __in_chrg)
{
    base->vtable = &__vt_base_4Font;
    if (__in_chrg & 1)
    {
        ::CClass::operator delete(base);
    }
}
