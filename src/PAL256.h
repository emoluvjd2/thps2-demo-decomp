#ifndef PAL256_INC_H
#define PAL256_INC_H

/*
 * PAL owns the shared 256-colour palette coordinates in small BSS. Consumers
 * retain the target's absolute address formation through their local .data
 * declaration view.
 */
#ifdef PAL256_OWNER_DECLARATIONS
#define PAL256_DECL_ATTR
#else
#define PAL256_DECL_ATTR __attribute__((section(".data")))
#endif

/** X coordinate of the shared 256-colour palette allocation area. */
extern int Pal256X PAL256_DECL_ATTR;

/** Y coordinate of the shared 256-colour palette allocation area. */
extern int Pal256Y PAL256_DECL_ATTR;

#undef PAL256_DECL_ATTR

#endif // PAL256_INC_H
