// FLASH.cpp -- Reconstructed translation unit
// Original: FLASH.CPP
// Functions: 6
// Address range: 0x8002F4EC - 0x8002F6A0

#include "FLASH.h"
#define DB_BUFFER_LAYOUT
#include "DB.h"
#undef DB_BUFFER_LAYOUT
#include "MAIN.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBGPU.H>

/**
 * Render-buffer imports whose target accesses use absolute addressing.
 *
 * DB.h supplies the truthful SDoubleBuffer layout. The explicit sections are
 * addressing-mode evidence retained from the target object.
 */
#include "PDOUBLEBUFFER.h"

/** Fixed-point color scale and target display dimensions. */
enum FlashConstants {
    FlashFractionBits = 16,
    FlashMaximumImportance = 255,
    FlashFullIntensity = FlashMaximumImportance << FlashFractionBits,
    FlashScreenWidth = 512,
    FlashScreenHeight = 240
};

// === Owner-local globals ===
// MAIN.SYM records these as FLASH-local definitions rather than XDEFs.
static unsigned char CurrentImportance = 0;
static int FlashSort = 0;
static int FlashCountdown = 0;
static int Fading = 0;
static int FadeCountdown = 0;

/**
 * Zero-initialized flash colors and per-frame deltas in target `.sbss` order.
 *
 * MAIN.SYM records all six as owner-local definitions.  Keeping them in an
 * explicit small-data section preserves the original GP-relative accesses
 * without turning the names into link-visible common symbols.
 */
static unsigned int CurrentR __attribute__((section(".sbss")));
static unsigned int CurrentG __attribute__((section(".sbss")));
static unsigned int CurrentB __attribute__((section(".sbss")));
static unsigned int dR __attribute__((section(".sbss")));
static unsigned int dG __attribute__((section(".sbss")));
static unsigned int dB __attribute__((section(".sbss")));

// === Function implementations (address order) ===

/**
 * Clears all active screen-flash and fade state.
 */
void Flash_Reset(void)
{
    FlashCountdown = 0;
    FadeCountdown = 0;
    Fading = 0;
    CurrentImportance = 0;
}

/**
 * Reports whether the active fade has finished.
 *
 * @return Nonzero once the fade countdown reaches zero.
 */
int Flash_FadeFinished(void)
{
    return FadeCountdown == 0;
}

/**
 * Starts a colored screen flash when its importance is high enough.
 *
 * @param StartR  Initial red intensity.
 * @param StartG  Initial green intensity.
 * @param StartB  Initial blue intensity.
 * @param Frames  Number of frames over which the flash decays.
 * @param Importance  Priority used to arbitrate overlapping flashes.
 * @param Sort  Panel ordering-table slot used to draw the flash.
 */
void Flash_Screen(
    unsigned char StartR, unsigned char StartG, unsigned char StartB,
    int Frames, unsigned char Importance, int Sort)
{
    if (Importance >= CurrentImportance && Frames != 0)
    {
        dR = (static_cast<unsigned int>(StartR) << FlashFractionBits)
            / Frames;
        dG = (static_cast<unsigned int>(StartG) << FlashFractionBits)
            / Frames;
        dB = (static_cast<unsigned int>(StartB) << FlashFractionBits)
            / Frames;
        CurrentB = StartB << FlashFractionBits;
        CurrentR = StartR << FlashFractionBits;
        CurrentG = StartG << FlashFractionBits;
        FlashCountdown = Frames;
        CurrentImportance = Importance;
        FlashSort = Sort;
    }
}

/**
 * Starts a full-intensity screen fade.
 *
 * @param Frames  Number of frames over which the fade grows.
 * @param Sort  Panel ordering-table slot used to draw the fade.
 */
void Flash_Fade(int Frames, int Sort)
{
    int step = FlashFullIntensity / Frames;
    Fading = 1;
    FadeCountdown = Frames;
    CurrentImportance = FlashMaximumImportance;
    CurrentB = 0;
    CurrentG = 0;
    CurrentR = 0;
    FlashSort = Sort;
    dR = step;
}

/**
 * Advances the active flash or fade by one frame.
 */
void Flash_Update(void)
{
    if (Fading != 0)
    {
        if (FadeCountdown != 0)
        {
            FadeCountdown--;
            if (FadeCountdown == 0)
            {
                CurrentB = FlashFullIntensity;
                CurrentG = FlashFullIntensity;
                CurrentR = FlashFullIntensity;
            }
            else
            {
                CurrentR += dR;
                CurrentG += dR;
                CurrentB += dR;
            }
        }

        return;
    }

    if (FlashCountdown != 0)
    {
        FlashCountdown--;
        if (FlashCountdown == 0)
        {
            CurrentImportance = 0;
        }
        else
        {
            CurrentR -= dR;
            CurrentG -= dG;
            CurrentB -= dB;
        }
    }
}

/**
 * Draws the current full-screen flash or fade into the panel ordering table.
 *
 * The primitive buffer stores a semi-transparent flat quad immediately
 * followed by the draw-mode command that selects the blend mode.
 */
void Flash_Display(void)
{
    unsigned int *OTPos;
    POLY_F4 *aPoly;
    DR_TPAGE *ptpage;
    unsigned char *newppoly;

    if (FlashCountdown != 0 || Fading != 0)
    {
        aPoly = reinterpret_cast<POLY_F4 *>(pPoly);
        OTPos = &pDoubleBuffer->PanelOT[FlashSort];
        newppoly = reinterpret_cast<unsigned char *>(aPoly + 1)
            + sizeof(DR_TPAGE);
        ptpage = reinterpret_cast<DR_TPAGE *>(aPoly + 1);

        if (newppoly <= PolyBufferEnd)
        {
            pPoly = reinterpret_cast<unsigned int *>(newppoly);

            setPolyF4(aPoly);
            setSemiTrans(aPoly, 1);
            setRGB0(aPoly,
                    CurrentR >> FlashFractionBits,
                    CurrentG >> FlashFractionBits,
                    CurrentB >> FlashFractionBits);
            aPoly->x0 = 0;   aPoly->x1 = FlashScreenWidth;
            aPoly->y0 = 0;   aPoly->y1 = 0;
            aPoly->x2 = 0;   aPoly->x3 = FlashScreenWidth;
            aPoly->y2 = FlashScreenHeight;
            aPoly->y3 = FlashScreenHeight;
            addPrim(OTPos, aPoly);

            ptpage->tag = 0x01000000;
            if (Fading != 0)
                ptpage->code[0] = 0xE1000640;
            else
                ptpage->code[0] = 0xE1000620;
            addPrim(OTPos, ptpage);
        }
    }
}
