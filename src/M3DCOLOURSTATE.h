/*
 * M3D physically owns these renderer-colour words in small data. This header
 * is intentionally re-entrant so consumers retain their original declaration
 * points, linkage contexts, and absolute-address import views.
 */
#if defined(M3DCOLOURSTATE_PLAIN_LINKAGE) || \
    defined(M3DCOLOURSTATE_WITHIN_C_LINKAGE)
#define M3DCOLOURSTATE_DECL_LINKAGE extern
#else
#define M3DCOLOURSTATE_DECL_LINKAGE extern "C"
#endif

#define M3DCOLOURSTATE_DECL_ATTR __attribute__((section(".data")))

#ifdef M3DCOLOURSTATE_DECLARE_FADE_COLOUR
/** Renderer fade colour copied into the scratchpad frame state. */
M3DCOLOURSTATE_DECL_LINKAGE unsigned int M3d_FadeColour
    M3DCOLOURSTATE_DECL_ATTR;
#endif

#ifdef M3DCOLOURSTATE_DECLARE_COLOUR_TABLE
/** Colour table selected for the region currently being rendered. */
M3DCOLOURSTATE_DECL_LINKAGE unsigned int *pColourTable
    M3DCOLOURSTATE_DECL_ATTR;
#endif

#undef M3DCOLOURSTATE_DECLARE_FADE_COLOUR
#undef M3DCOLOURSTATE_DECLARE_COLOUR_TABLE
#undef M3DCOLOURSTATE_DECL_ATTR
#undef M3DCOLOURSTATE_DECL_LINKAGE
#undef M3DCOLOURSTATE_PLAIN_LINKAGE
#undef M3DCOLOURSTATE_WITHIN_C_LINKAGE
