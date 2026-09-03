#ifndef POLYBUFFERSIZE_INC_H
#define POLYBUFFERSIZE_INC_H

/*
 * DB physically owns the polygon-buffer byte count as a small-BSS common.
 * Cross-owner consumers retain their target-proven absolute-address import
 * view of that word.
 */
#if defined(POLYBUFFERSIZE_OWNER_DECLARATION) && \
    defined(POLYBUFFERSIZE_ABSOLUTE_DECLARATION)
#error "Select only one POLYBUFFERSIZE declaration view"
#elif defined(POLYBUFFERSIZE_OWNER_DECLARATION)
#define POLYBUFFERSIZE_DECL_ATTR
#elif defined(POLYBUFFERSIZE_ABSOLUTE_DECLARATION)
#define POLYBUFFERSIZE_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the POLYBUFFERSIZE owner or absolute declaration view"
#endif

/** Number of bytes reserved for each display polygon buffer. */
extern int POLYBUFFERSIZE POLYBUFFERSIZE_DECL_ATTR;

#undef POLYBUFFERSIZE_DECL_ATTR
#undef POLYBUFFERSIZE_ABSOLUTE_DECLARATION
#undef POLYBUFFERSIZE_OWNER_DECLARATION

#endif // POLYBUFFERSIZE_INC_H
