// Immediate-mode debug font queue and renderer.

#define FONT_OWNER_DECLARATIONS
#include "FONT.h"
#undef FONT_OWNER_DECLARATIONS
#include "MAIN.h"
#include "DB.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include <INLINE_C.H>
#include "gte_extensions.h"
#include <LIBGPU.H>

struct tag_S_Pal;

#include "TEXTURE.h"

#include "SANIMFRAME.h"

#include "SVIEWPORT.h"

/** Double-buffer prefix through the two ordering-table pointers. */
struct SDoubleBuffer {
    u8 opaque_before_ordering_tables[132];
    u32 *PanelOT;
    u32 *OrderingTable;
};

#include "SCAMERA.h"

typedef char Font_Texture_size_must_be_40[
    (sizeof(Texture) == 40) ? 1 : -1];
typedef char Font_StringCommand_size_must_be_32[
    (sizeof(SString) == 32) ? 1 : -1];
/** Font command entries constructed by FONT.CPP at startup. */
SString StringBuffer[100] __attribute__((section(".data")));

/**
 * Complete FONT small-data contribution in target order.
 *
 * Defining the original globals before their users also gives ASPSX the
 * section information required to select the original GP-relative forms.
 */
static SAnimFrame *FontAnim __attribute__((section(".sdata"))) = 0;
SAnimFrame *BigFontAnim __attribute__((section(".sdata"))) = 0;
static unsigned int CodeVal __attribute__((section(".sdata"))) = 0x2c000000;
static char FontAnimName1[12] __attribute__((section(".sdata"))) = "Fontsmll";
static char FontAnimName2[8] __attribute__((section(".sdata"))) = "Font";
SString *BufferPos __attribute__((section(".sdata"))) = 0;
char *pNumberStrings __attribute__((section(".sdata"))) = 0;

// === External function declarations ===
#include "SPOOL.h"
#include "CVECTOROPS.h"

/** Copy the packed color and UV words into one Psy-Q font primitive. */
static inline void Font_SetPolyAppearance(POLY_FT4 *apoly,
                                          const Texture *stored,
                                          unsigned int ColourInfo)
{
    *reinterpret_cast<unsigned int *>(&apoly->r0) = ColourInfo;
    *reinterpret_cast<unsigned int *>(&apoly->u0) =
        *reinterpret_cast<const unsigned int *>(&stored->u0);
    *reinterpret_cast<unsigned int *>(&apoly->u1) =
        *reinterpret_cast<const unsigned int *>(&stored->u1);
    *reinterpret_cast<unsigned int *>(&apoly->u2) =
        *reinterpret_cast<const unsigned int *>(&stored->u2);
    *reinterpret_cast<unsigned short *>(&apoly->u3) =
        *reinterpret_cast<const unsigned short *>(&stored->u3);
}

// === Owner-local globals ===

/** FONT-owned small-glyph lookup table, defined after its text users. */
extern unsigned char Mapping[128];

/** FONT-owned transient decimal-formatting storage. */
extern char NumberStrings[512];

// === Function implementations (address order) ===

/** Append one signed decimal number to the font number-string buffer. */
void WriteNumber(int Num) {
    int PrintZeros = 0;
    if (Num < 0) {
        Num = -Num;
        *pNumberStrings++ = '-';
    }
    int PowerTen = 1000000000;
    do {
        if (Num >= PowerTen) {
            int digit = Num / PowerTen;
            PrintZeros = 1;
            *pNumberStrings++ = static_cast<char>(digit + '0');
            Num = Num - digit * PowerTen;
        } else if ((PrintZeros != 0) || (PowerTen == 1)) {
            *pNumberStrings++ = '0';
        }
        PowerTen = PowerTen / 10;
    } while (PowerTen > 0);
    *pNumberStrings++ = 0;
}


/** Resolve the small and large font animation tables. */
void Font_Init(void) {
    // This incomplete redeclaration still denotes the owner-local array, while
    // retaining the target's absolute address formation at this call site.
    extern char FontAnimName2[];
    FontAnim = Spool_FindAnim(FontAnimName1);
    BigFontAnim = Spool_FindAnim(FontAnimName2);
}


/** Reset the per-frame font command and number-string cursors. */
void Font_Reset(void) {
    BufferPos = StringBuffer;
    StringBuffer[0].End = 1;
    pNumberStrings = NumberStrings;
}


/**
 * Queue a string for the next font-display pass.
 *
 * @param X Screen X coordinate.
 * @param Y Screen Y coordinate.
 * @param pString Null-terminated text; storage must remain valid this frame.
 * @param r Red component.
 * @param g Green component.
 * @param b Blue component.
 * @param BigText Nonzero to draw the enlarged font variant.
 */
void Font_String(unsigned short X, unsigned short Y, char *pString,
                 unsigned char r, unsigned char g, unsigned char b,
                 unsigned char BigText) {
    BufferPos->End = 0;
    BufferPos->X = X;
    BufferPos->Y = Y;
    BufferPos->Is3d = 0;
    BufferPos->BigText = BigText;
    BufferPos->pString = pString;
    BufferPos->CodeBGR = CodeVal |
                         (static_cast<unsigned int>(b) << 16) |
                         (static_cast<unsigned int>(g) << 8) | r;
    BufferPos++;
    BufferPos->End = 1;
}


/**
 * Format and queue a signed number for the next font-display pass.
 *
 * @param X Screen X coordinate.
 * @param Y Screen Y coordinate.
 * @param Num Number to format.
 * @param r Red component.
 * @param g Green component.
 * @param b Blue component.
 * @param BigText Nonzero to draw the enlarged font variant.
 */
void Font_Number(unsigned short X, unsigned short Y, int Num,
                 unsigned char r, unsigned char g, unsigned char b,
                 unsigned char BigText) {
    BufferPos->End = 0;
    BufferPos->X = X;
    BufferPos->Y = Y;
    BufferPos->Is3d = 0;
    BufferPos->BigText = BigText;
    BufferPos->pString = pNumberStrings;
    BufferPos->CodeBGR = CodeVal |
                         (static_cast<unsigned int>(b) << 16) |
                         (static_cast<unsigned int>(g) << 8) | r;
    WriteNumber(Num);
    BufferPos++;
    BufferPos->End = 1;
}


/** Render every queued 2D or projected 3D font string. */
void Font_Display(void) {
    SString *pStringInfo;
    char *CurrentChar;
    POLY_FT4 *apoly;
    int LetterFrame;
    int Width;
    int Height;
    int LetterX;
    int LetterY;
    CVector camera_pos;
    unsigned int ColourInfo;

    // Load the active camera's view rotation without its translation.
    gte_SetRotMatrix(&MikeCamera[Camera_CurrentMikeCamera].View);
    gte_ZeroTransVector();
    pStringInfo = StringBuffer;
    camera_pos.vx =
        MikeCamera[Camera_CurrentMikeCamera].Position.vx << 12;
    camera_pos.vy =
        MikeCamera[Camera_CurrentMikeCamera].Position.vy << 12;
    camera_pos.vz =
        MikeCamera[Camera_CurrentMikeCamera].Position.vz << 12;

    // Check first entry: if End != 0, nothing to display
    if (pStringInfo->End != 0) return;

    do {
        long zed;

        CurrentChar = pStringInfo->pString;
        if (pStringInfo->Is3d) {
            CVector Temp;
            unsigned int y0x0;

            // 3D text: transform position to screen coordinates
            Temp = (pStringInfo->Pos - camera_pos) >> 12;

            // Load vector into GTE and run RTPS
            gte_ldlv0(&Temp);
            gte_rtps();

            ColourInfo = pStringInfo->CodeBGR;

            // The original path sorts using the projected MAC3 depth.
            gte_stlvnl2(&zed);

            // Depth clip check
            if (zed < static_cast<int>(pCurrentViewport->Hither)) goto SKIP;
            if (static_cast<int>(pCurrentViewport->Yon) < zed) goto SKIP;

            // Read the projected screen coordinate only after depth clipping.
            gte_stsxy(&y0x0);

            LetterX = y0x0 & 0xffff;
            LetterY = y0x0 >> 16;
        } else {
            // 2D text
            LetterX = pStringInfo->X;
            LetterY = pStringInfo->Y;
            ColourInfo = pStringInfo->CodeBGR;
        }

        // Check if string is empty
        if (*CurrentChar == 0) goto SKIP;

        do {
            char letter;

            letter = *CurrentChar++;
            LetterFrame = Mapping[letter & 0x7f];
            if (letter == '_') {
                LetterFrame = Mapping['-' & 0x7f];
            }

            if (LetterFrame != 0xff) {
                SAnimFrame *pFrame;
                Texture *stored;
                unsigned char *newppoly;

                pFrame = &FontAnim[LetterFrame];
                stored = pFrame->pTexture;
                Width = pFrame->Width;
                if (Width < 4) {
                    LetterX++;
                }

                // The renderer owns pPoly as a word cursor. A distinct typed
                // global alias delays its store and changes ten instructions.
                apoly = reinterpret_cast<POLY_FT4 *>(pPoly);
                newppoly = reinterpret_cast<unsigned char *>(apoly + 1);
                if (newppoly > PolyBufferEnd) return;

                pPoly = reinterpret_cast<unsigned int *>(newppoly);
                apoly->tag = 0x09000000;
                Font_SetPolyAppearance(apoly, stored, ColourInfo);
                Width = apoly->u1 - apoly->u0;
                Height = apoly->v2 - apoly->v0;

                if (pStringInfo->BigText) {
                    Width = (Width * 55) >> 5;
                    Height = (Height * 55) >> 5;
                    apoly->u1--;
                    apoly->u3--;
                    apoly->v2--;
                    apoly->v3--;
                }

                if (LetterFrame == 0x1e) {
                    if (letter == '_') {
                        setXYWH(apoly, LetterX, LetterY + 5, Width, Height);
                    } else {
                        setXYWH(apoly, LetterX, LetterY + 2, Width, Height);
                    }
                } else {
                    setXYWH(apoly, LetterX, LetterY, Width, Height);
                }

                if (pStringInfo->Is3d) {
                    addPrim(
                        &pDoubleBuffer->OrderingTable[
                            (static_cast<unsigned int>(zed) >> 2) & 0xfff],
                        apoly);
                } else {
                    addPrim(pDoubleBuffer->PanelOT, apoly);
                }

                LetterX += Width + 3;
                if (Width < 4) {
                    LetterX++;
                }
            } else {
                LetterX += 5;
            }
        } while (*CurrentChar != 0);

    SKIP:
        pStringInfo++;
    } while (pStringInfo->End == 0);
}


/** Character-to-frame mapping for the small font. */
unsigned char Mapping[128] __attribute__((section(".data"))) = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x1d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x1b, 0x1e, 0x1a, 0xff,
    0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,
    0x27, 0x28, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1c,
    0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
    0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
    0x17, 0x18, 0x19, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
    0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
    0x17, 0x18, 0x19, 0xff, 0xff, 0xff, 0xff, 0xff,
};

/** Character-to-frame mapping for the large message font. */
#define BIGFONTMAPPING_OWNER_DECLARATION
#include "BIGFONTMAPPING.h"
unsigned char BigFont_Mapping[256] __attribute__((section(".data"))) = {
    0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a,
    0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a,
    0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a,
    0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a,
    0xff, 0x2c, 0x2a, 0x2a, 0x2a, 0x02, 0x2a, 0x30,
    0x2a, 0x2a, 0x2a, 0x01, 0x2a, 0x32, 0x31, 0x2a,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
    0x28, 0x29, 0x2a, 0x2a, 0x2a, 0x35, 0x2a, 0x2a,
    0x2a, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c,
    0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14,
    0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c,
    0x1d, 0x1e, 0x1f, 0x2a, 0x2a, 0x2a, 0x2a, 0x00,
    0x2a, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c,
    0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14,
    0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c,
    0x1d, 0x1e, 0x1f, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a,
    0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a,
    0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a,
    0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a,
    0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a,
    0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a,
    0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a,
    0x14, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a,
    0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a,
    0x2b, 0x2a, 0x2a, 0x2a, 0x03, 0x2a, 0x2a, 0x2a,
    0x2e, 0x2d, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a,
    0x2a, 0x2a, 0x2a, 0x2a, 0x2f, 0x2a, 0x04, 0x2a,
    0x2a, 0x2a, 0x2a, 0x2a, 0x05, 0x2a, 0x2a, 0x2a,
    0x2b, 0x2a, 0x2a, 0x2a, 0x03, 0x2a, 0x2a, 0x2a,
    0x2e, 0x2d, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a,
    0x2a, 0x2a, 0x2a, 0x2a, 0x2f, 0x2a, 0x04, 0x2a,
    0x2a, 0x2a, 0x2a, 0x2a, 0x05, 0x2a, 0x2a, 0x2a,
};

/** Scratch storage for decimal strings queued by Font_Number. */
char NumberStrings[512] __attribute__((section(".data"))) = {};
