/*
 * HIRES physically allocates the current-level word as a four-byte small-BSS
 * common. This header is intentionally re-entrant so every consumer retains
 * its original declaration point and absolute-address import view.
 */
#if defined(GLEVEL_OWNER_DECLARATION) || \
    defined(GLEVEL_WITHIN_C_LINKAGE)
#define GLEVEL_DECL_LINKAGE extern
#else
#define GLEVEL_DECL_LINKAGE extern "C"
#endif

#ifdef GLEVEL_OWNER_DECLARATION
#define GLEVEL_DECL_ATTR
#else
#define GLEVEL_DECL_ATTR __attribute__((section(".data")))
#endif

/** Current gameplay-level index, physically allocated by HIRES.cpp. */
GLEVEL_DECL_LINKAGE int GLevel GLEVEL_DECL_ATTR;

#undef GLEVEL_DECL_ATTR
#undef GLEVEL_DECL_LINKAGE
#undef GLEVEL_OWNER_DECLARATION
#undef GLEVEL_WITHIN_C_LINKAGE
