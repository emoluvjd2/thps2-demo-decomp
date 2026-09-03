#ifndef SPOOLPOS_INC_H
#define SPOOLPOS_INC_H

/*
 * SPOOL physically owns the active queue position in initialized small data.
 * REDBOOK retains its target-proven absolute-address import view.
 */
#if defined(SPOOLPOS_OWNER_DECLARATION) && \
    defined(SPOOLPOS_ABSOLUTE_DECLARATION)
#error "Select only one SpoolPos declaration view"
#elif defined(SPOOLPOS_OWNER_DECLARATION)
#define SPOOLPOS_DECL_ATTR
#elif defined(SPOOLPOS_ABSOLUTE_DECLARATION)
#define SPOOLPOS_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the SpoolPos owner or absolute declaration view"
#endif

#ifdef SPOOLPOS_WITHIN_C_LINKAGE
#define SPOOLPOS_DECL_LINKAGE extern
#else
#define SPOOLPOS_DECL_LINKAGE extern "C"
#endif

/** Index of the spool request currently being serviced. */
SPOOLPOS_DECL_LINKAGE int SpoolPos SPOOLPOS_DECL_ATTR;

#undef SPOOLPOS_DECL_LINKAGE
#undef SPOOLPOS_DECL_ATTR
#undef SPOOLPOS_ABSOLUTE_DECLARATION
#undef SPOOLPOS_OWNER_DECLARATION
#undef SPOOLPOS_WITHIN_C_LINKAGE

#endif // SPOOLPOS_INC_H
