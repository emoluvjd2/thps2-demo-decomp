#ifndef TOTALTEXTUREUSAGE_INC_H
#define TOTALTEXTUREUSAGE_INC_H

/*
 * SPOOL physically owns the texture-allocation counter in small data. Every
 * cross-owner consumer addresses that word absolutely.
 */
#if defined(TOTALTEXTUREUSAGE_OWNER_DECLARATION) && \
    defined(TOTALTEXTUREUSAGE_ABSOLUTE_DECLARATION)
#error "Select only one TotalTextureUsage declaration view"
#elif defined(TOTALTEXTUREUSAGE_OWNER_DECLARATION)
#define TOTALTEXTUREUSAGE_DECL_ATTR
#elif defined(TOTALTEXTUREUSAGE_ABSOLUTE_DECLARATION)
#define TOTALTEXTUREUSAGE_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the TotalTextureUsage owner or absolute declaration view"
#endif

/** Bytes currently consumed by loaded texture records and their allocations. */
extern int TotalTextureUsage
    TOTALTEXTUREUSAGE_DECL_ATTR;

#undef TOTALTEXTUREUSAGE_DECL_ATTR
#undef TOTALTEXTUREUSAGE_ABSOLUTE_DECLARATION
#undef TOTALTEXTUREUSAGE_OWNER_DECLARATION

#endif // TOTALTEXTUREUSAGE_INC_H
