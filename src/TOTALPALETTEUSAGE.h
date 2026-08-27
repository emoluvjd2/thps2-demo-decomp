/*
 * PAL physically owns the palette-allocation counter as a four-byte common.
 * Cross-owner consumers address that word absolutely.
 */
#if defined(TOTALPALETTEUSAGE_OWNER_DECLARATION) && \
    defined(TOTALPALETTEUSAGE_ABSOLUTE_DECLARATION)
#error "Select only one TotalPaletteUsage declaration view"
#elif defined(TOTALPALETTEUSAGE_OWNER_DECLARATION)
#define TOTALPALETTEUSAGE_DECL_ATTR
#elif defined(TOTALPALETTEUSAGE_ABSOLUTE_DECLARATION)
#define TOTALPALETTEUSAGE_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the TotalPaletteUsage owner or absolute declaration view"
#endif

#ifdef TOTALPALETTEUSAGE_WITHIN_C_LINKAGE
#define TOTALPALETTEUSAGE_DECL_LINKAGE extern
#else
#define TOTALPALETTEUSAGE_DECL_LINKAGE extern "C"
#endif

/** Bytes currently consumed by palette records and their allocations. */
TOTALPALETTEUSAGE_DECL_LINKAGE int TotalPaletteUsage
    TOTALPALETTEUSAGE_DECL_ATTR;

#undef TOTALPALETTEUSAGE_DECL_LINKAGE
#undef TOTALPALETTEUSAGE_DECL_ATTR
#undef TOTALPALETTEUSAGE_ABSOLUTE_DECLARATION
#undef TOTALPALETTEUSAGE_OWNER_DECLARATION
#undef TOTALPALETTEUSAGE_WITHIN_C_LINKAGE
