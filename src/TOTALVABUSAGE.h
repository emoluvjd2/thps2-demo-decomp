#ifndef TOTALVABUSAGE_INC_H
#define TOTALVABUSAGE_INC_H

/*
 * SFX physically owns the sound-RAM allocation counter in initialized small
 * data. INIT retains its target-proven absolute-address import view.
 */
#if defined(TOTALVABUSAGE_OWNER_DECLARATION) && \
    defined(TOTALVABUSAGE_ABSOLUTE_DECLARATION)
#error "Select only one TotalVabUsage declaration view"
#elif defined(TOTALVABUSAGE_OWNER_DECLARATION)
#define TOTALVABUSAGE_DECL_ATTR
#elif defined(TOTALVABUSAGE_ABSOLUTE_DECLARATION)
#define TOTALVABUSAGE_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the TotalVabUsage owner or absolute declaration view"
#endif

/** Bytes of SPU sound RAM occupied by loaded VAB data. */
extern int TotalVabUsage TOTALVABUSAGE_DECL_ATTR;

#undef TOTALVABUSAGE_DECL_ATTR
#undef TOTALVABUSAGE_ABSOLUTE_DECLARATION
#undef TOTALVABUSAGE_OWNER_DECLARATION

#endif // TOTALVABUSAGE_INC_H
