#ifndef HIRESMODE_INC_H
#define HIRESMODE_INC_H

/*
 * SHELLUTILS physically owns the high-resolution shell flag in initialized
 * small data. FRONTEND2 retains its target-proven absolute-address view.
 */
#if defined(HIRESMODE_OWNER_DECLARATION) && \
    defined(HIRESMODE_ABSOLUTE_DECLARATION)
#error "Select only one HiresMode declaration view"
#elif defined(HIRESMODE_OWNER_DECLARATION)
#define HIRESMODE_DECL_ATTR
#elif defined(HIRESMODE_ABSOLUTE_DECLARATION)
#define HIRESMODE_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the HiresMode owner or absolute declaration view"
#endif

/** Nonzero while the shell renders through its high-resolution path. */
extern int HiresMode HIRESMODE_DECL_ATTR;

#undef HIRESMODE_DECL_ATTR
#undef HIRESMODE_ABSOLUTE_DECLARATION
#undef HIRESMODE_OWNER_DECLARATION

#endif // HIRESMODE_INC_H
