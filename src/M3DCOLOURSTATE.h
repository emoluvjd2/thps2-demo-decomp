/*
 * M3D physically owns these renderer-colour words in small data. This header
 * is intentionally re-entrant so consumers retain their original declaration
 * points, linkage contexts, and absolute-address import views.
 */
#define M3DCOLOURSTATE_DECL_ATTR __attribute__((section(".data")))

#ifdef M3DCOLOURSTATE_DECLARE_FADE_COLOUR
/** Renderer fade colour copied into the scratchpad frame state. */
extern unsigned int M3d_FadeColour
    M3DCOLOURSTATE_DECL_ATTR;
#endif

#ifdef M3DCOLOURSTATE_DECLARE_COLOUR_TABLE
/** Colour table selected for the region currently being rendered. */
extern unsigned int *pColourTable
    M3DCOLOURSTATE_DECL_ATTR;
#endif

#undef M3DCOLOURSTATE_DECLARE_FADE_COLOUR
#undef M3DCOLOURSTATE_DECLARE_COLOUR_TABLE
#undef M3DCOLOURSTATE_DECL_ATTR
