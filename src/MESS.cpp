// MESS.cpp -- Reconstructed translation unit
// Original: MESS.CPP
// Functions: 30 reconstructed
// Address range: 0x8002D6A8 - 0x8002F4E0
//
// Hand-maintained clean-room reconstruction compiled with cc1plus
// (GCC 2.91.66-psx), which emits the original C++ symbol spellings.

// MESS.cpp is the sole consumer of these legacy font and runtime records.
// Keep the recovered layouts with their owner instead of a generated TU header.
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned long ulong;

#include "UTILS.h"
#include "MAIN.h"
#include "DB.h"

/** Runtime glyph placement and texture metadata. */
struct Character {
    int w;
    int h;
    int x;
    int y;
    int baseline;
    unsigned char u0;
    unsigned char v0;
    unsigned short clut;
    unsigned char u1;
    unsigned char v1;
    unsigned short tp;
    unsigned char u2;
    unsigned char v2;
    unsigned char u3;
    unsigned char v3;
    int real_w;
};

/** Loaded font descriptor used by the message renderer. */
struct FontInfo {
    char mFont_name[16];
    int mNum_chars;
    Character *mChar_table;
    unsigned char mFont_shadow_offset;
    unsigned short mFont_scale;
    int mChar_sep;
    int mSpace_width;
};

#define MESS_DECLARE_OWNER_STATE_VIEW
#define MESS_OWNER_DECLARATIONS
#include "MESS.h"
#undef MESS_OWNER_DECLARATIONS
#include "BIGFONTMAPPING.h"

struct Texture;

#include "SANIMFRAME.h"
#include "FONT.h"

/** Intrusive message-list entry allocated and updated by MESS.cpp. */
struct Message {
    char *pText;
    unsigned short T;
    unsigned short T_total;
    short xOffset;
    short yOffset;
    void *recenter;
    SMessageProg *pProg;
    Message *pNext;
    Message *pPrevious;
};

/** One glyph-metadata record at the front of a font file. */
struct MessFontFileGlyph {
    int width;
    int height;
    int baseline;
    int realWidth;
};

/** Owner-local display-buffer slice through the panel ordering table. */
struct MessDoubleBuffer {
    unsigned char beforePanelOT[0x84];
    unsigned int *PanelOT;
};

enum {
    MessFontHeaderSize = sizeof(int),
    MessFontFileGlyphSize = sizeof(MessFontFileGlyph),
    MessCharacterHeightOffset = 4,
    MessCharacterBaselineOffset = 16,
    MessPaletteColorCount = 16,
    MessPaletteEntrySize = sizeof(unsigned short)
};

// Precise source view of the DB-owned active display-buffer pointer.
#include "PDOUBLEBUFFER.h"

// Natural C++ spellings for imported game services used by this owner.
#include "FILEIO.h"
#include "PAL.h"

typedef char mess_program_size_must_be_32[(sizeof(SMessageProg) == 32) ? 1 : -1];
typedef char mess_message_size_must_be_28[(sizeof(Message) == 28) ? 1 : -1];
typedef char mess_font_file_glyph_size_must_be_16[
    (sizeof(MessFontFileGlyph) == 16) ? 1 : -1];
typedef char mess_double_buffer_size_must_be_136[
    (sizeof(MessDoubleBuffer) == 136) ? 1 : -1];
#include "MEM.h"
#include "PACK.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#define PANEL_PRIMITIVE_API
#include "PANEL.h"
#undef PANEL_PRIMITIVE_API
#include <STDIO.H>
#include <STRINGS.H>

typedef char mess_poly_gt4_size_must_be_52[
    (sizeof(POLY_GT4) == 52) ? 1 : -1];

// === Free function declarations ===
int Mess_TextWidth2(const char *pMessage);
int Mess_TextHeight2(const char *pMessage);
int Mess_TextHeightAboveBaseline2(const char *pMessage);
int Mess_TextHeightBelowBaseline2(const char *pMessage);
static Message *CreateMessage(void);
void Mess_DeleteMessage(Message *pMessage);
int GetCharIndex(char ch);
void Mess_SetNewFont(unsigned char which);

// === Source-owned message programs and font table ===
#define MESSAGE_PROGRAM(duration, start_scale, end_scale, color_cycle, next_offset) \
    {                                                                            \
        duration, 1, 0, start_scale, end_scale,                                  \
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                                     \
        0, 0, 0x100, 0x5a, 0x100, 0x5a, 0, color_cycle, next_offset             \
    }

/** Standard zoom-in, hold, and zoom-out message animation. */
SMessageProg Messprog_Zoom[5] __attribute__((section(".data"))) = {
    MESSAGE_PROGRAM(5, 0, 0x180, 0, 1),
    MESSAGE_PROGRAM(1, 0x180, 0x180, 0, 1),
    MESSAGE_PROGRAM(4, 0x180, 0x100, 0, 1),
    MESSAGE_PROGRAM(10, 0x100, 0x100, 0, 1),
    MESSAGE_PROGRAM(15, 0x100, 0, 0, 0),
};

/** Goal/tape notification variant with color cycling enabled. */
SMessageProg Messprog_Goal[5] __attribute__((section(".data"))) = {
    MESSAGE_PROGRAM(5, 0, 0x180, 1, 1),
    MESSAGE_PROGRAM(1, 0x180, 0x180, 1, 1),
    MESSAGE_PROGRAM(4, 0x180, 0x100, 1, 1),
    MESSAGE_PROGRAM(30, 0x100, 0x100, 1, 1),
    MESSAGE_PROGRAM(15, 0x100, 0, 1, 0),
};

/** Runtime font descriptors populated by Mess_LoadFont. */
FontInfo Font_table[10]
    __attribute__((section(".data"))) = {{0}};

#undef MESSAGE_PROGRAM

// Recovered MESS small-data prefix in target order. MAIN.SYM records the first
// four definitions as owner-local rather than XDEFs.
static u8 TextJustification = 0;
static u32 FlagsBGR = 0x2c808080;
static u16 Scale = 0;
static u16 Sort = 0;
int MessFade = 0x40;
static char MessMemoryError[] __attribute__((section(".sdata"))) =
    "\nmemory error";

/**
 * MESS-owned saved color state.
 *
 * Keep the section-qualified external access view so GCC retains the target's
 * GP-relative instruction form.  The distinct owner storage declaration emits
 * the local LNK identity recorded by MAIN.SYM without publishing a false XDEF.
 */
#define MESS_DECLARE_SAVED_COLOR_STATE_VIEW
#include "MESS.h"
namespace MessSavedColorOwner
{
extern "C"
{
static u32 LastFlagsBGR;
}
}

// === Function implementations (address order) ===

/**
 * Mess_SetTextJustify__FUc
 *
 * Address:   0x8002D6A8
 * Mangled:   Mess_SetTextJustify__FUc
 * Demangled: Mess_SetTextJustify(unsigned char)
 *
 * @param j  Text alignment: centered (0), left (1), or right (2).
 */
void Mess_SetTextJustify(unsigned char j)
{
    TextJustification = j;
}

/**
 * Mess_SetScale__Fi
 *
 * Address:   0x8002D6B4
 * Mangled:   Mess_SetScale__Fi
 * Demangled: Mess_SetScale(int)
 *            If NewFont is set, calls Mess_SetFontScale; else stores to Scale
 *
 * @param scale  8.8 fixed-point text scale.
 */
void Mess_SetScale(int scale)
{
    if (NewFont)
    {
        Mess_SetFontScale(scale);
    }
    else
    {
        Scale = scale;
    }
}

/**
 * Mess_GetScale__Fv
 *
 * Address:   0x8002D6E8
 * Mangled:   Mess_GetScale__Fv
 * Demangled: Mess_GetScale(void)
 *            If NewFont is set, calls Mess_GetFontScale; else returns Scale
 *
 * @return The active 8.8 fixed-point text scale.
 */
int Mess_GetScale(void)
{
    if (NewFont)
    {
        return Mess_GetFontScale();
    }
    else
    {
        return (u16)Scale;
    }
}

/**
 * Mess_SetSort__Fi
 *
 * Address:   0x8002D71C
 * Mangled:   Mess_SetSort__Fi
 * Demangled: Mess_SetSort(int)
 *
 * @param sort  Panel ordering-table bucket used for emitted primitives.
 */
void Mess_SetSort(int sort)
{
    Sort = sort;
}

/**
 * Mess_SetRGB__FUcUcUci
 *
 * Address:   0x8002D728
 * Mangled:   Mess_SetRGB__FUcUcUci
 * Demangled: Mess_SetRGB(unsigned char, unsigned char, unsigned char, int)
 *            Packs R,G,B into FlagsBGR with GPU command code
 *
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 * @param semitrans  Whether to select the semitransparent GPU command.
 */
void Mess_SetRGB(unsigned char r, unsigned char g, unsigned char b, int semitrans)
{
    u32 rgb = ((u32)(b & 0xff) << 16) |
              ((u32)(g & 0xff) << 8) |
              (u32)(r & 0xff);
    FlagsBGR = rgb;
    FlagsBGR = rgb | (semitrans ? 0x2E000000 : 0x2C000000);
}

/**
 * Mess_DrawPanelText__FiiPCciUii
 *
 * Address:   0x8002D764
 * Mangled:   Mess_DrawPanelText__FiiPCciUii
 * Demangled: Mess_DrawPanelText(int, int, const char *, int, unsigned int, int)
 *            Wrapper that calls Mess_DrawText with drawPanel=true
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param pMessage  Message to operate on.
 * @param Angle  Legacy solid-text selector forwarded to Mess_DrawText.
 * @param FireRGB  Reserved fire-effect colour argument.
 * @param FireSquash  Reserved fire-effect squash argument.
 */
int Mess_DrawPanelText(int x, int y, const char *pMessage, int Angle, unsigned int FireRGB, int FireSquash)
{
    return Mess_DrawText(x, y, pMessage, Angle, FireRGB, FireSquash, 1);
}

/**
 * Mess_TextWidth2__FPCc
 *
 * Address:   0x8002D798
 * Mangled:   Mess_TextWidth2__FPCc
 * Demangled: Mess_TextWidth2(const char *)
 *
 * @param pMessage  Message to operate on.
 * @return width + count * 3
 */
int Mess_TextWidth2(const char *pMessage)
{
    const char *pScan = pMessage;
    int Length = 0;
    int NumLetters = 0;
    if (*(u8 *)pScan)
    {
        do
        {
            int frame = BigFont_Mapping[*(u8 *)pScan];
            if (frame == 0xFE)
                frame = -1;
            if (frame == 0xFD)
                frame = -1;
            if (frame == 0xFC)
                frame = -1;
            if (frame == 0xFB)
                frame = -1;
            if (frame == 0xF9)
                frame = -1;
            if (frame == 0xF8)
                frame = -1;
            if (frame == 0xFA)
                frame = -1;
            if (frame == 0xFF)
                frame = -1;
            if (frame >= 0)
            {
                NumLetters++;
                Length += BigFontAnim[frame].Width +
                          BigFontAnim[frame].OffX;
            }
            else
            {
                Length += 10;
            }
            pScan++;
        } while (*(u8 *)pScan);
    }
    Length = Length * (u16)Scale >> 8;
    return Length + NumLetters * 3;
}

/**
 * Mess_TextHeight2__FPCc
 *
 * Address:   0x8002D8B8
 * Mangled:   Mess_TextHeight2__FPCc
 * Demangled: Mess_TextHeight2(const char *)
 *
 * @param pMessage  Message to operate on.
 * @return maxHeight * (u16)Scale >> 8
 */
int Mess_TextHeight2(const char *pMessage)
{
    const char *pScan = pMessage;
    int Height = 0;
    if (*(u8 *)pScan)
    {
        do
        {
            int frame = BigFont_Mapping[(u8)*pScan];
            if (frame == 0xFE)
                frame = -1;
            if (frame == 0xFD)
                frame = -1;
            if (frame == 0xFC)
                frame = -1;
            if (frame == 0xFB)
                frame = -1;
            if (frame == 0xF9)
                frame = -1;
            if (frame == 0xF8)
                frame = -1;
            if (frame == 0xFA)
                frame = -1;
            if (frame == 0xFF)
                frame = -1;
            if (frame >= 0)
            {
                int temp = BigFontAnim[frame].Height;
                if (Height < temp)
                {
                    Height = temp;
                }
            }
            pScan++;
        } while (*(u8 *)pScan);
    }
    return Height * (u16)Scale >> 8;
}

/**
 * Mess_TextHeightAboveBaseline2__FPCc
 *
 * Address:   0x8002D9C0
 * Mangled:   Mess_TextHeightAboveBaseline2__FPCc
 * Demangled: Mess_TextHeightAboveBaseline2(const char *)
 *            Calls Mess_TextHeight2 and returns its result
 *
 * @param pMessage  Message to operate on.
 * @return The legacy font height above the baseline.
 */
int Mess_TextHeightAboveBaseline2(const char *pMessage)
{
    return Mess_TextHeight2(pMessage);
}

/**
 * Mess_TextHeightBelowBaseline2__FPCc
 *
 * Address:   0x8002D9E0
 * Mangled:   Mess_TextHeightBelowBaseline2__FPCc
 * Demangled: Mess_TextHeightBelowBaseline2(const char *)
 * @param pMessage  Message to operate on.
 * @return 0
 */
int Mess_TextHeightBelowBaseline2(const char *pMessage)
{
    return 0;
}

/**
 * CreateMessage__Fv
 *
 * Address:   0x8002D9E8
 * Mangled:   CreateMessage__Fv
 * Demangled: CreateMessage(void)
 *            Allocates a new Message (28 bytes), inserts at head of pMessages linked list
 *
 * @return msg
 */
static Message *CreateMessage(void)
{
    Message *pNew = (Message *)Mem_New(sizeof(Message), 0, 1);
    Message *old = pMessages;
    pNew->pPrevious = 0;
    pMessages = pNew;
    pNew->pNext = old;
    if (old)
    {
        old->pPrevious = pNew;
    }
    return pNew;
}

/**
 * Mess_DeleteMessage__FP7Message
 *
 * Address:   0x8002DA28
 * Mangled:   Mess_DeleteMessage__FP7Message
 * Demangled: Mess_DeleteMessage(Message *)
 *            Unlinks message from doubly-linked list and frees it
 *
 * @param pMessage  Message to operate on.
 */
void Mess_DeleteMessage(Message *pMessage)
{
    Message *next = pMessage->pNext;
    if (next)
    {
        next->pPrevious = pMessage->pPrevious;
    }
    Message *previous = pMessage->pPrevious;
    if (previous)
    {
        previous->pNext = pMessage->pNext;
    }
    if (pMessage == pMessages)
    {
        pMessages = pMessage->pNext;
    }
    Mem_Delete(pMessage);
}

/**
 * Mess_DeleteAll__Fv
 *
 * Address:   0x8002DA94
 * Mangled:   Mess_DeleteAll__Fv
 * Demangled: Mess_DeleteAll(void)
 *            Frees all messages in linked list and clears pMessages
 *
 */
void Mess_DeleteAll(void)
{
    Message *pMessage = pMessages;
    if (pMessage)
    {
        Message *pNext = pMessage->pNext;
    loop:
        Mem_Delete(pMessage);
        if (pNext)
        {
            pMessage = pNext;
            pNext = pNext->pNext;
            goto loop;
        }
    }
    pMessages = 0;
}

/**
 * Mess_Message__FPCcP12SMessageProgbss
 *
 * Address:   0x8002DADC
 * Mangled:   Mess_Message__FPCcP12SMessageProgbss
 * Demangled: Mess_Message(const char *, SMessageProg *, bool, short, short)
 *            Allocate, populate, and return a new message entry.
 *
 * @param pText  Text to operate on.
 * @param pProg  Prog to operate on.
 * @param recenter  Whether the message participates in vertical recentering.
 * @param xOffset  Horizontal offset from the animation-program position.
 * @param yOffset  Vertical offset from the animation-program position.
 */
Message *Mess_Message(const char *pText, SMessageProg *pProg, bool recenter, short xOffset, short yOffset)
{
    Message *pNew = CreateMessage();
    pNew->pText = (char *)pText;
    pNew->pProg = pProg;
    pNew->T = 0;
    pNew->T_total = 0;
    pNew->recenter = (void *)recenter;
    pNew->xOffset = xOffset;
    pNew->yOffset = yOffset;
    return pNew;
}

/**
 * Mess_Update__Fv
 *
 * Address:   0x8002DB54
 * Mangled:   Mess_Update__Fv
 * Demangled: Mess_Update(void)
 *            Walk active messages, advance timers, and delete completed entries.
 *
 */
void Mess_Update(void)
{
    Message *pMess = pMessages;
    if (pMess)
    {
        Message *pNext = pMess->pNext;
    body:
        {
            SMessageProg *pProg = pMess->pProg;
            int timer = pMess->T;
            int maxTime = pProg->Duration;
            int nextProg;
            SMessageProg *newProg;
            if (timer < (unsigned)maxTime)
            {
                goto timer_inc;
            }
            nextProg = pProg->NextOff;
            if (!nextProg)
            {
                Mess_DeleteMessage(pMess);
                pMess = pNext;
                goto check;
            }
            if (nextProg == 0x7f)
                goto done_msg;
            newProg = pProg + nextProg;
            nextProg = pMess->T_total;
            pMess->pProg = newProg;
            pMess->T = 0;
            goto inc_f6;
        timer_inc:
            nextProg = pMess->T_total;
            pMess->T = timer + 1;
        inc_f6:
            nextProg++;
            pMess->T_total = nextProg;
        }
    done_msg:;
        pMess = pNext;
    check:
        if (pNext)
        {
            pNext = pNext->pNext;
            goto body;
        }
    }
}

/**
 * Mess_Display__Fv
 *
 * Address:   0x8002DBFC
 * Mangled:   Mess_Display__Fv
 * Demangled: Mess_Display(void)
 *            Counts recentered messages, then animates and renders each visible message.
 *
 */
void Mess_Display(void)
{
    Message *pMess = pMessages;
    int i = 0;
    Message *first = pMess;
    if (pMess)
    {
        do
        {
            if (pMess->recenter)
            {
                i++;
            }
            pMess = pMess->pNext;
        } while (pMess);
    }

    int y_offset = (i - 1) << 4;
    pMess = first;
    if (pMess)
    {
        do
        {
            SMessageProg *pProg = pMess->pProg;
            if (pProg->FlashDuration)
            {
                int flash = pMess->T / pProg->FlashDuration;
                flash &= 1;
                if (flash == 0)
                    goto next;
            }
            {
                TextJustification = pProg->Justify;
                unsigned short Dur = pProg->Duration;
                int scaleEnd = pProg->EndScale;
                int scaleStart = pProg->StartScale;
                int timer = pMess->T;
                int scaleVal = scaleStart + (scaleEnd - scaleStart) * timer / Dur;
                Scale = scaleVal;
                Mess_SetScale(scaleVal & 0xFFFF);

                int t = pMess->T;
                // Clean expression variants either swapped the red/green
                // temporaries or changed load order, so retain this one
                // target-proven assignment.
                register int rDiff asm("$5") =
                    (pProg->EndR - pProg->StartR) * t / Dur;
                int r = (pProg->StartR + rDiff) & 0xFF;

                int g1 = pProg->EndG;
                int g0 = pProg->StartG;
                int gDiff = (g1 - g0) * t / Dur;

                int b0 = pProg->StartB;
                int b1 = pProg->EndB;
                int bDiff = (b1 - b0) * t / Dur;

                int g = (g0 + gDiff) & 0xFF;
                int b = (b0 + bDiff) & 0xFF;
                int bgr = (b << 16) | (g << 8) | r;
                FlagsBGR = bgr;

                int semi = pProg->Semitrans;
                FlagsBGR = bgr | (semi ? 0x2E000000 : 0x2C000000);

                if (pProg->ColorCycle)
                {
                    Mess_SetRGB(((Xblanks % 20) * 12) & 0xFC, 100, 100, 0);
                }

                int y0 = pProg->StartY;
                int y1 = pProg->EndY;
                int t2 = pMess->T;
                int yInterp = (y1 - y0) * t2 / Dur;

                int x0 = pProg->StartX;
                int x1 = pProg->EndX;
                int xInterp = (x1 - x0) * t2 / Dur;

                int yOff = pMess->yOffset;
                int xOff = pMess->recenter != 0;
                int TextY = y0 + yInterp + yOff;
                int xm = pMess->xOffset;
                int TextX = x0 + xInterp + xm;

                if (xOff)
                {
                    TextY += y_offset;
                    y_offset -= 0x20;
                }

                int w0 = pProg->StartAngle;
                int w1 = pProg->EndAngle;
                int wInterp = (w1 - w0) * t2 / Dur;
                int width = w0 + wInterp;

                Mess_DrawPanelText(TextX, TextY, pMess->pText, width, 0, 0x100);
            }
        next:
            pMess = pMess->pNext;
        } while (pMess);
    }
}

/**
 * Mess_TextWidth__FPCc
 *
 * Address:   0x8002DEDC
 * Mangled:   Mess_TextWidth__FPCc
 * Demangled: Mess_TextWidth(const char *)
 *            Measures rendered text width with either the legacy big font or the loaded font table.
 *
 * @param pMessage  Null-terminated text to measure.
 * @return The scaled width of all drawable characters and spaces.
 */
int Mess_TextWidth(const char *pMessage)
{
    if (!NewFont)
    {
        return Mess_TextWidth2(pMessage);
    }
    const char *pScan = pMessage;
    int Length = 0;
    if (*(u8 *)pScan)
    {
        do
        {
            int c = GetCharIndex(*(u8 *)pScan);
            if ((unsigned)(c - 0xFB) < 3)
                goto skip;
            if (c == 0xF9)
                goto skip;
            if (c == 0xF8)
                goto skip;
            if (c == 0xFE)
                goto skip;
            if (c == 0xFA)
                goto skip;
            if (c == 0xFF)
            {
                Length += ((Font_table[Current_font].mSpace_width + Font_table[Current_font].mChar_sep) *
                           Font_table[Current_font].mFont_scale) >> 8;
            }
            else
            {
                Length += ((Font_table[Current_font].mChar_table[c].real_w +
                            Font_table[Current_font].mChar_sep) *
                           Font_table[Current_font].mFont_scale) >> 8;
            }
        skip:
            pScan++;
        } while (*(u8 *)pScan);
    }
    return Length;
}

/**
 * Mess_TextHeightBelowBaseline__FPCc
 *
 * Address:   0x8002E02C
 * Mangled:   Mess_TextHeightBelowBaseline__FPCc
 * Demangled: Mess_TextHeightBelowBaseline(const char *)
 *
 * @param pMessage  Message to operate on.
 * @return The scaled maximum glyph extent below the baseline.
 */
int Mess_TextHeightBelowBaseline(const char *pMessage)
{
    if (!NewFont)
    {
        return Mess_TextHeightBelowBaseline2(pMessage);
    }
    const char *pScan = pMessage;
    int Height = 0;
    if (*(u8 *)pScan)
    {
        // This interior-base form is retained because three typed indexing
        // variants changed the target's temporary-allocation.
        int charDataBase = (int)Font_table + 0x14;
        do
        {
            int c = GetCharIndex(*(u8 *)pScan);
            int temph = 0;
            if ((unsigned)(c - 0xFB) < 3)
                goto check_max;
            if (c == 0xF9)
                goto check_max;
            if (c == 0xF8)
                goto check_max;
            if (c == 0xFE)
                goto check_max;
            if (c == 0xFA)
                goto check_max;
            if (c == 0xFF)
                goto check_max;
            {
                int cf = Current_font;
                int off = cf * 36;
                int charData = *(int *)(off + charDataBase);
                int entry = c * 36 + charData;
                temph = *(int *)(entry + MessCharacterHeightOffset) -
                    *(int *)(entry + MessCharacterBaselineOffset);
            }
        check_max:
            if (Height < temph)
            {
                Height = temph;
            }
            pScan++;
        } while (*(u8 *)pScan);
    }
    int base = (int)Font_table;
    int cf = Current_font;
    int off = cf * 36;
    int scale = *(u16 *)(base + off + 0x1A);
    return (Height * scale) >> 8;
}

/**
 * Mess_TextHeightAboveBaseline__FPCc
 *
 * Address:   0x8002E168
 * Mangled:   Mess_TextHeightAboveBaseline__FPCc
 * Demangled: Mess_TextHeightAboveBaseline(const char *)
 *
 * @param pMessage  Message to operate on.
 * @return The scaled maximum glyph baseline height.
 */
int Mess_TextHeightAboveBaseline(const char *pMessage)
{
    if (!NewFont)
    {
        return Mess_TextHeightAboveBaseline2(pMessage);
    }
    const char *pScan = pMessage;
    int Height = 0;
    if (*(u8 *)pScan)
    {
        do
        {
            int c = GetCharIndex(*(u8 *)pScan);
            int temph;
            if ((unsigned)(c - 0xFB) < 3)
                goto set_h_zero;
            if (c == 0xF9)
                goto set_h_zero;
            if (c == 0xF8)
                goto set_h_zero;
            if (c == 0xFE)
                goto set_h_zero;
            if (c == 0xFA)
                goto set_h_zero;
            if (c == 0xFF)
            {
            set_h_zero:
                temph = 0;
            }
            else
            {
                int cf = Current_font;
                Character *charData = Font_table[cf].mChar_table;
                temph = charData[c].baseline;
            }
            if (Height < temph)
            {
                Height = temph;
            }
            pScan++;
        } while (*(u8 *)pScan);
    }
    int cf = Current_font;
    int scale = Font_table[cf].mFont_scale;
    return (Height * scale) >> 8;
}

/**
 * Mess_TextHeight__FPCc
 *
 * Address:   0x8002E2A0
 * Mangled:   Mess_TextHeight__FPCc
 * Demangled: Mess_TextHeight(const char *)
 *            If NewFont, returns HeightAboveBaseline + HeightBelowBaseline; else returns TextHeight2
 *
 * @param pMessage  Message to operate on.
 * @return The total scaled text height.
 */
int Mess_TextHeight(const char *pMessage)
{
    if (!NewFont)
    {
        return Mess_TextHeight2(pMessage);
    }
    return Mess_TextHeightAboveBaseline(pMessage) +
           Mess_TextHeightBelowBaseline(pMessage);
}

/**
 * GetCharIndex__Fc
 *
 * Address:   0x8002E2EC
 * Mangled:   GetCharIndex__Fc
 * Demangled: GetCharIndex(char)
 *
 * @param ch  Character to map into the current font's glyph table.
 * @return A glyph index, formatting control code, or fallback glyph index.
 */
int GetCharIndex(char ch)
{
    int numChars = Font_table[Current_font].mNum_chars;
    unsigned int characterIndex;
    ch &= 0xFF;

    if (numChars < 15)
    {
        characterIndex = ch - 0x30;
        if ((u8)characterIndex < 10)
            return characterIndex;
        if (ch == 0x3A)
            return 10;
        return 0;
    }

    if (ch == 0x20)
        return 0xFF;
    if (ch == 0x5B)
        return 0xFE;
    if (ch == 0x5D)
        return 0xFD;
    if (ch == 0x7B)
        return 0xFE;
    if (ch == 0x7D)
        return 0xFC;
    if (ch == 0x3C)
        return 0xFE;
    if (ch == 0x3E)
        return 0xFB;
    if (ch == 0x7C)
        return 0xFE;
    if (ch == 0x60)
        return 0xF9;
    if (ch == 0x7E)
        return 0xFE;
    if (ch == 0x26)
        return 0xF8;
    if (ch == 0x5E)
        return 0xFA;

    characterIndex = ch - 0x41;
    if ((u8)characterIndex < 0x1A)
        return characterIndex;
    characterIndex = ch - 0x61;
    if ((u8)characterIndex < 0x1A)
        return characterIndex;
    if ((u8)(ch - 0x30) < 10)
        return ch - 0x16;
    switch (ch)
    {
    case '!':
        return 0x25;
    case ':':
        return 0x26;
    case '.':
        return 0x27;
    case ',':
        return 0x28;
    case '-':
    case '_':
        return 0x29;
    case '%':
        return 0x2A;
    case '+':
        return 0x2B;
    case '\'':
    {
        if (Font_table[Current_font].mNum_chars >= 0x2D)
            return 0x2C;
        return 0x24;
    }
    case '#':
        return 0x2D;
    case '"':
        return 0x2E;
    case '&':
        return 0x2F;
    case '@':
        return 0x30;
    case '?':
    default:
        return 0x24;
    }
}

/**
 * Mess_DrawText__FiiPCciUiib
 *
 * Address:   0x8002E4E4
 * Mangled:   Mess_DrawText__FiiPCciUiib
 * Demangled: Mess_DrawText(int, int, const char *, int, unsigned int, int, bool)
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param pMessage  Message to operate on.
 * @param DrawSolid  Whether glyphs use the solid rather than semitransparent path.
 * @param FireRGB  Reserved fire-effect colour argument.
 * @param FireSquash  Reserved fire-effect squash argument.
 * @param IsPanel  Whether to submit primitives through Panel_addPrim.
 * @return The rendered text width.
 */
int Mess_DrawText(int x, int y, const char *pMessage, int DrawSolid, unsigned int FireRGB, int FireSquash, bool IsPanel)
{
    char *pScan;
    int Off;
    int length;
    int currentScale;
    int useCurrentScale;
    int ScreenX;
    unsigned int shadow_off;
    int n;
    int char_count;

    length = Mess_TextWidth(pMessage);
    currentScale = Mess_GetScale();
    useCurrentScale = 1;

    switch (TextJustification)
    {
    case 0:
        Off = -length / 2;
        break;
    case 2:
        Off = -length;
        break;
    case 1:
        Off = 0;
        break;
    }

    shadow_off = ((unsigned int)(Font_table[Current_font].mFont_shadow_offset *
                                 Font_table[Current_font].mFont_scale) >> 8) *
                 0x10001;

    ScreenX = x + Off;
    pScan = (char *)pMessage;
    ScreenX += length;
    char_count = strlen(pScan);
    pScan += char_count;

    while (char_count)
    {
        POLY_GT4 *apoly;
        POLY_FT4 *shadow;
        POLY_FT4 *anothershadow;
        u8 *newppoly;
        unsigned int u0;
        unsigned int u1;
        unsigned short u2;
        unsigned short u3;
        unsigned char LargestRGB;

        --pScan;
        --char_count;
        n = GetCharIndex(*pScan);

        if (n == 0xFA)
        {
            useCurrentScale = !useCurrentScale;
            Mess_SetScale(useCurrentScale ? currentScale : 0x100);
            continue;
        }
        if (n == 0xFE)
        {
            FlagsBGR = LastFlagsBGR;
            continue;
        }
        if (n == 0xFD)
        {
            LastFlagsBGR = FlagsBGR;
            Mess_SetRGB(0, 0x80, 0xFF, 0);
            continue;
        }
        if (n == 0xFC)
        {
            LastFlagsBGR = FlagsBGR;
            Mess_SetRGB(0xFF, 0x80, 0, 0);
            continue;
        }
        if (n == 0xFB)
        {
            LastFlagsBGR = FlagsBGR;
            Mess_SetRGB(0x40, 0x40, 0x40, 0);
            continue;
        }
        if (n == 0xF9)
        {
            LastFlagsBGR = FlagsBGR;
            Mess_SetRGB(0x80, 0x40, 0x40, 0);
            continue;
        }
        if (n == 0xF8)
        {
            LastFlagsBGR = FlagsBGR;
            Mess_SetRGB(0x40, 0x80, 0x40, 0);
            continue;
        }
        if (n == 0xFF)
        {
            ScreenX -= ((Font_table[Current_font].mSpace_width +
                         Font_table[Current_font].mChar_sep) *
                        Font_table[Current_font].mFont_scale) >> 8;
            continue;
        }

        ScreenX -= ((Font_table[Current_font].mChar_table[n].real_w +
                     Font_table[Current_font].mChar_sep) *
                    Font_table[Current_font].mFont_scale) >> 8;

        apoly = (POLY_GT4 *)pPoly;
        shadow = (POLY_FT4 *)((u8 *)apoly + sizeof(POLY_GT4));
        anothershadow = shadow + 1;
        newppoly = (u8 *)anothershadow + sizeof(POLY_FT4);
        if (PolyBufferEnd < newppoly)
            return length;
        pPoly = (u32 *)newppoly;

        *(u32 *)&apoly->r0 = (FlagsBGR & 0x00FFFFFF) + 0x3E000000;
        *(u32 *)&apoly->r1 = FlagsBGR;
        *(u32 *)&apoly->r2 = FlagsBGR;
        *(u32 *)&apoly->r3 = FlagsBGR;
        int textureFont = Current_font;
        apoly->tag = 0x0C000000;

        u0 = *(u32 *)&Font_table[textureFont].mChar_table[n].u0;
        u1 = *(u32 *)&Font_table[textureFont].mChar_table[n].u1;
        u2 = *(u16 *)&Font_table[textureFont].mChar_table[n].u2;
        u3 = *(u16 *)&Font_table[textureFont].mChar_table[n].u3;
        if (Font_table[textureFont].mFont_scale > 0x100)
        {
            u1--;
            u2 -= 0x100;
            u3 -= 0x101;
        }
        *(u32 *)&apoly->u0 = u0;
        *(u32 *)&apoly->u1 = u1;
        *(u16 *)&apoly->u2 = u2;
        *(u16 *)&apoly->u3 = u3;

        int geomFont = Current_font;
        apoly->x0 = ScreenX;
        apoly->y0 = y - ((Font_table[geomFont].mChar_table[n].baseline *
                          Font_table[geomFont].mFont_scale) >> 8);
        apoly->x1 = ScreenX + (((Font_table[geomFont].mChar_table[n].w << 2) *
                               Font_table[geomFont].mFont_scale) >> 8);
        apoly->y1 = apoly->y0;
        apoly->x2 = ScreenX;
        apoly->y2 = apoly->y0 + ((Font_table[geomFont].mChar_table[n].h *
                                  Font_table[geomFont].mFont_scale) >> 8);
        apoly->x3 = apoly->x1;
        apoly->y3 = apoly->y2;

        apoly->code &= 0xFD;
        if (!DrawSolid)
        {
            apoly->code |= 2;
            apoly->tpage = (apoly->tpage & 0xFF9F) | 0x20;
        }

        // Read FlagsBGR into a temporary before the tag store: the original's
        // schedule issues the gp-relative load ahead of the `lui`/`sw` pair,
        // and cc1plus will not sink a static load past a store through a
        // struct pointer on its own.
        u32 shadowFlags = FlagsBGR;
        shadow->tag = 0x09000000;
        *(u32 *)&shadow->r0 = shadowFlags;
        *(u32 *)&shadow->u0 = u0;
        *(u32 *)&shadow->u1 = u1;
        *(u16 *)&shadow->u2 = u2;
        *(u16 *)&shadow->u3 = u3;
        setXY4(shadow,
               apoly->x0, apoly->y0,
               apoly->x1, apoly->y1,
               apoly->x2, apoly->y2,
               apoly->x3, apoly->y3);
        shadow->code |= 2;
        setRGB0(shadow, 0x80, 0x80, 0x80);
        shadow->tpage = (shadow->tpage & 0xFF9F) | 0x40;

        // Copy the flat-textured shadow primitive in the original four-word
        // batches, folding each shadow_off adjustment into the batch that just
        // loaded the word (MESS.CPP:1011/1016/1017/1020).  The compound-assign
        // form lets cc1plus add in place instead of allocating a fresh pseudo.
        {
            unsigned int w0, w1, w2, w3;
            u32 *shadowWords = (u32 *)shadow;
            u32 *otherShadowWords = (u32 *)anothershadow;

            w0 = shadowWords[0];
            w1 = shadowWords[1];
            w2 = shadowWords[2];
            w3 = shadowWords[3];
            otherShadowWords[0] = w0;
            otherShadowWords[1] = w1;
            otherShadowWords[2] = w2;
            otherShadowWords[3] = w3;
            w2 += shadow_off;
            shadowWords[2] = w2;

            w0 = shadowWords[4];
            w1 = shadowWords[5];
            w2 = shadowWords[6];
            w3 = shadowWords[7];
            otherShadowWords[4] = w0;
            otherShadowWords[5] = w1;
            otherShadowWords[6] = w2;
            otherShadowWords[7] = w3;
            w0 += shadow_off;
            shadowWords[4] = w0;
            w2 += shadow_off;
            shadowWords[6] = w2;

            w0 = shadowWords[8];
            w1 = shadowWords[9];
            otherShadowWords[8] = w0;
            otherShadowWords[9] = w1;
            w0 += shadow_off;
            shadowWords[8] = w0;
        }
        LargestRGB = 0x80;
        setRGB0(anothershadow, LargestRGB, LargestRGB, LargestRGB);
        anothershadow->code |= 2;
        anothershadow->tpage = (anothershadow->tpage & 0xFF9F) | 0x40;

        if (IsPanel)
        {
            Panel_addPrim(apoly);
            Panel_addPrim(anothershadow);
            Panel_addPrim(shadow);
        }
        else
        {
            addPrim(reinterpret_cast<MessDoubleBuffer *>(pDoubleBuffer)->PanelOT + Sort, apoly);
            addPrim(reinterpret_cast<MessDoubleBuffer *>(pDoubleBuffer)->PanelOT + Sort, anothershadow);
            addPrim(reinterpret_cast<MessDoubleBuffer *>(pDoubleBuffer)->PanelOT + Sort, shadow);
        }
    }

    (void)FireRGB;
    (void)FireSquash;
    return length;
}

/**
 * Mess_LoadFont__FPcT0iii
 *
 * Address:   0x8002EBC0
 * Mangled:   Mess_LoadFont__FPcT0iii
 * Demangled: Mess_LoadFont(char *, char *, int, int, int)
 *
 * @param filename  Filename string.
 * @param file_buffer  Serialized font metadata, palette, and glyph pixels.
 * @param char_sep  Inter-glyph spacing, or -1 to derive it from glyph zero.
 * @param shadow_offset  Shadow displacement, or -1 to derive it from glyph zero.
 * @param width  Width in pixels.
 * @return The installed font-table index.
 */
int Mess_LoadFont(char *filename, char *file_buffer, int char_sep, int shadow_offset, int width)
{
    int this_font;
    int clutx;
    int cluty;
    unsigned short *clut;
    RECT r;
    char *db;
    int x;
    int y;

    Mess_SetNewFont(1);

    this_font = Num_fonts;
    {
        int the_font;
        for (the_font = 0; the_font < Num_fonts; the_font++)
        {
            if (Font_table[the_font].mChar_table)
            {
                if (strcmp(Font_table[the_font].mFont_name, filename) == 0)
                {
                    Mem_Delete(file_buffer);
                    Current_font = the_font;
                    return 0;
                }
            }
            else
            {
                this_font = the_font;
            }
        }
    }

    Pack_NoBottom = 1;
    Pack_NoRight = 1;
    if (this_font == Num_fonts)
    {
        Num_fonts = this_font + 1;
    }

    {
        int the_font = this_font;
        Font_table[the_font].mNum_chars = *(int *)file_buffer;
        strcpy(Font_table[the_font].mFont_name, filename);

        clutx = Pal16X;
        cluty = Pal16Y + GetFree16Slot();
        clut = (unsigned short *)Mem_New(
            MessPaletteColorCount * MessPaletteEntrySize, 1, 1);
        for (int i = 0; i < MessPaletteColorCount; i++)
        {
            clut[i] = *(unsigned short *)(file_buffer +
                                         Font_table[this_font].mNum_chars * MessFontFileGlyphSize +
                                         MessFontHeaderSize + i * MessPaletteEntrySize);
        }
    }

    r.x = clutx;
    r.y = cluty;
    r.w = 16;
    r.h = 1;
    LoadImage(&r, reinterpret_cast<unsigned long *>(clut));
    Mem_Delete(clut);
    Clut = GetClut(clutx, cluty);

    Font_table[this_font].mChar_table =
        (Character *)Mem_New(Font_table[this_font].mNum_chars * sizeof(Character), 0, 1);
    Font_table[this_font].mFont_scale = 0x100;
    if (!Font_table[this_font].mChar_table)
    {
        printf(MessMemoryError);
    }

    {
        int i;
        MessFontFileGlyph *tb =
            (MessFontFileGlyph *)(file_buffer + sizeof(int));
        db = file_buffer +
             (Font_table[this_font].mNum_chars * MessFontFileGlyphSize +
              MessFontHeaderSize) +
             MessPaletteColorCount * MessPaletteEntrySize;
        for (i = 0; i < Font_table[this_font].mNum_chars; i++)
        {
            int w = tb->width;
            int h = tb->height;
            RECT r;

            Pack_Pack(0, w * 4, h, &x, &y, 4, 0, 0);

            r.x = x;
            r.y = y;
            r.w = w;
            r.h = h;
            LoadImage(&r, reinterpret_cast<unsigned long *>(db));
            DrawSync(0);

            Font_table[this_font].mChar_table[i].x = x;
            Font_table[this_font].mChar_table[i].y = y;
            Font_table[this_font].mChar_table[i].w = w;
            Font_table[this_font].mChar_table[i].h = h;
            Font_table[this_font].mChar_table[i].baseline = tb->baseline;

            if (width == -1 || (width == 0 && i == 0))
            {
                Font_table[this_font].mChar_table[i].real_w = tb->realWidth;
            }
            else if (width == 0)
            {
                Font_table[this_font].mChar_table[i].real_w =
                    Font_table[this_font].mChar_table[0].real_w;
            }
            else
            {
                Font_table[this_font].mChar_table[i].real_w = width;
            }

            Font_table[this_font].mChar_table[i].u0 = (unsigned char)x << 2;
            Font_table[this_font].mChar_table[i].v0 = (unsigned char)y;
            Font_table[this_font].mChar_table[i].u1 =
                Font_table[this_font].mChar_table[i].u0 + w * 4;
            Font_table[this_font].mChar_table[i].v1 =
                Font_table[this_font].mChar_table[i].v0;
            Font_table[this_font].mChar_table[i].u2 =
                Font_table[this_font].mChar_table[i].u0;
            Font_table[this_font].mChar_table[i].v2 =
                Font_table[this_font].mChar_table[i].v0 + h;
            Font_table[this_font].mChar_table[i].u3 =
                Font_table[this_font].mChar_table[i].u1;
            Font_table[this_font].mChar_table[i].v3 =
                Font_table[this_font].mChar_table[i].v2;
            Font_table[this_font].mChar_table[i].tp = GetTPage(0, 0, x, y);
            Font_table[this_font].mChar_table[i].clut = Clut;

            int pixels = w * h;
            int oddBytes = pixels & 1;
            oddBytes <<= 1;
            pixels <<= 1;
            pixels += oddBytes;
            db += pixels;
            tb++;
        }
    }

    Mem_Delete(file_buffer);

    if (shadow_offset == -1)
    {
        Font_table[this_font].mFont_shadow_offset =
            Font_table[this_font].mChar_table[0].w >> 1;
    }
    else
    {
        Font_table[this_font].mFont_shadow_offset = shadow_offset;
    }

    if (char_sep == -1)
    {
        Font_table[this_font].mChar_sep = Font_table[this_font].mChar_table[0].w >> 2;
    }
    else
    {
        Font_table[this_font].mChar_sep = char_sep;
    }

    Font_table[this_font].mSpace_width =
        Font_table[this_font].mChar_table[0].w * 28 / 10;
    Pack_NoBottom = 0;
    Pack_NoRight = 0;
    Current_font = this_font;
    return this_font;
}

/**
 * Mess_LoadFont__FPciii
 *
 * Address:   0x8002F1CC
 * Mangled:   Mess_LoadFont__FPciii
 * Demangled: Mess_LoadFont(char *, int, int, int)
 *            Loads a font file into memory, waits for I/O completion, then installs it from the buffer.
 *
 * @param filename  Filename string.
 * @param char_sep  Inter-glyph spacing, or -1 to derive it from glyph zero.
 * @param shadow_offset  Shadow displacement, or -1 to derive it from glyph zero.
 * @param width  Width in pixels.
 * @return The installed font-table index.
 */
int Mess_LoadFont(char *filename, int char_sep, int shadow_offset, int width)
{
    int size = FileIO_Open(filename);
    char *buf = (char *)Mem_New(size, 1, 1);
    FileIO_Load(buf);
    FileIO_Sync();
    return Mess_LoadFont(filename, buf, char_sep, shadow_offset, width);
}

/**
 * Mess_UnloadFont__Fv
 *
 * Address:   0x8002F258
 * Mangled:   Mess_UnloadFont__Fv
 * Demangled: Mess_UnloadFont(void)
 *            Releases the current font's texture slots, CLUT, and character table,
 *            then trims trailing empty font entries.
 *
 */
void Mess_UnloadFont(void)
{
    {
        int i;
        for (i = 0; i < Font_table[Current_font].mNum_chars; i++)
        {
            Pack_Unpack(Font_table[Current_font].mChar_table[i].x,
                        Font_table[Current_font].mChar_table[i].y);
        }
    }

    Free16Slot(Font_table[Current_font].mChar_table->clut);
    Mem_Delete(Font_table[Current_font].mChar_table);

    int remaining = Num_fonts;
    bool currentIsLast = Current_font == remaining - 1;
    Font_table[Current_font].mChar_table = 0;
    if (currentIsLast)
    {
        int previous;
        do
        {
            previous = remaining;
            if (previous < 1)
                return;
            int newCount = previous - 1;
            Num_fonts = newCount;
            remaining = newCount;
        } while (!Font_table[previous - 2].mChar_table);
    }
}

/**
 * Mess_SetCurrentFont__Fi
 *
 * Address:   0x8002F3D4
 * Mangled:   Mess_SetCurrentFont__Fi
 * Demangled: Mess_SetCurrentFont(int)
 *
 * @param Font  Font-table index to select.
 */
void Mess_SetCurrentFont(int Font)
{
    Current_font = Font;
}

/**
 * Mess_SetCurrentFont__FPc
 *
 * Address:   0x8002F3E0
 * Mangled:   Mess_SetCurrentFont__FPc
 * Demangled: Mess_SetCurrentFont(char *)
 *            Searches Font_table by name, sets Current_font to matching index
 *
 * @param font_name  Name of the font to select.
 */
void Mess_SetCurrentFont(char *font_name)
{
    int i;
    for (i = 0; i < Num_fonts; i++)
    {
        if (strcmp(font_name, Font_table[i].mFont_name) == 0)
            break;
    }
    Current_font = i;
}

/**
 * Mess_SetFontScale__FUi
 *
 * Address:   0x8002F450
 * Mangled:   Mess_SetFontScale__FUi
 * Demangled: Mess_SetFontScale(unsigned int)
 * Match:     PERFECT (21/21)
 *            If NewFont is not set, calls Mess_SetScale; else stores scale in Font_table
 *
 * @param scale  8.8 fixed-point font scale.
 */
void Mess_SetFontScale(unsigned int scale)
{
    if (!NewFont)
    {
        Mess_SetScale(scale);
    }
    else
    {
        Font_table[Current_font].mFont_scale = scale;
    }
}

/**
 * Mess_GetFontScale__Fv
 *
 * Address:   0x8002F4A4
 * Mangled:   Mess_GetFontScale__Fv
 * Demangled: Mess_GetFontScale(void)
 *            If NewFont is set, returns font scale from Font_table[Current_font].scale; else returns 0
 *
 * @return The loaded font's 8.8 scale, or zero in legacy-font mode.
 */
int Mess_GetFontScale(void)
{
    if (!NewFont)
    {
        return 0;
    }
    return Font_table[Current_font].mFont_scale;
}

/**
 * Mess_SetNewFont__FUc
 *
 * Address:   0x8002F4E0
 * Mangled:   Mess_SetNewFont__FUc
 * Demangled: Mess_SetNewFont(unsigned char)
 *
 * @param which  Nonzero to use the dynamically loaded font renderer.
 */
void Mess_SetNewFont(unsigned char which)
{
    NewFont = which;
}
