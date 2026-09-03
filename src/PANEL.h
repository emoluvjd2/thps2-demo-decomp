#ifndef THPS2_PANEL_H
#define THPS2_PANEL_H

struct CBruce;
struct SAnimFrame;

/** Draw the vertical balance meter used by the gameplay display. */
void Panel_BalanceUD(int balance, int max, int length, int x, int y,
                     int lineColor, int markerColor);

/** Add one primitive to the active panel ordering table. */
void Panel_addPrim(void *primitive);

/** Allocate and initialize the gameplay panel. */
void Panel_Init(void);

/** Release resources owned by the gameplay panel. */
void Panel_Uninit(void);

/** Advance the gameplay panel state for one frame. */
void Panel_Update(void);

/** Draw all gameplay panel elements for the current frame. */
void Panel_Display(void);

/** Bind one skater to its panel state. */
void Panel_PlayerInit(unsigned short playerNumber, CBruce *player);

/** Notify the panel that a skater has taken off. */
void Panel_TakeOff(CBruce *player);

/** Add the current held-trick score to the panel. */
void Panel_HoldTrick(CBruce *player, int points);

/** Notify the panel that a skater has landed. */
void Panel_Land(CBruce *player);

/** Notify the panel that a skater has bailed. */
void Panel_Bail(CBruce *player);

/** Award one SKATE letter and report whether the word is complete. */
int Panel_Letter(int letter);

/** Award one tape category through the panel. */
void Panel_Tape(int tape);

/** Advance the current level's panel goal counter. */
void Panel_GoalCounter(int count);

#endif // THPS2_PANEL_H

// Rendering owners opt in after LIBGPU.H has supplied the primitive typedefs.
#if defined(PANEL_PRIMITIVE_API) && !defined(THPS2_PANEL_PRIMITIVE_API_EMITTED)
#define THPS2_PANEL_PRIMITIVE_API_EMITTED

/** Build one textured panel primitive for an animation frame. */
POLY_FT4 *Panel_DrawTexturedPoly(SAnimFrame *frame);

/** Stretch one textured panel primitive to the requested screen position. */
void Panel_SetStretchedScreenCoords(
    int x, int y, POLY_FT4 *poly, SAnimFrame *frame);

/** Build one flat-shaded panel rectangle. */
POLY_F4 *Panel_DrawFlatShadedPoly(
    int x, int y, int width, int height,
    unsigned char red, unsigned char green, unsigned char blue, int sort);

#endif // PANEL_PRIMITIVE_API && !THPS2_PANEL_PRIMITIVE_API_EMITTED

#ifdef PANEL_DECLARE_DATA_CONSTRUCTOR_ABI
#if !defined(PANEL_DATA_CONSTRUCTOR_RECEIVER) || \
    !defined(PANEL_DATA_CONSTRUCTOR_PLAYER)
#error "Select the panel constructor receiver and player view"
#endif

/** Construct one panel-data object through its target ABI entry. */
extern "C" PANEL_DATA_CONSTRUCTOR_RECEIVER *__10CPanelDataP6CBruce(
    PANEL_DATA_CONSTRUCTOR_RECEIVER *panel,
    PANEL_DATA_CONSTRUCTOR_PLAYER *player);

#undef PANEL_DATA_CONSTRUCTOR_PLAYER
#undef PANEL_DATA_CONSTRUCTOR_RECEIVER
#undef PANEL_DECLARE_DATA_CONSTRUCTOR_ABI
#endif
