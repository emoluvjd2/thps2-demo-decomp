#ifndef TRICKOBLIST_INC_H
#define TRICKOBLIST_INC_H

struct CTrickOb;

/*
 * TRICKOB physically owns the active trick-object head in small data. Replay
 * code addresses that pointer absolutely, so each inclusion selects its
 * target-proven declaration view without exposing the object layout.
 */
#if defined(TRICKOBLIST_OWNER_DECLARATION) && \
    defined(TRICKOBLIST_ABSOLUTE_DECLARATION)
#error "Select only one TrickObList declaration view"
#elif defined(TRICKOBLIST_OWNER_DECLARATION)
#define TRICKOBLIST_DECL_ATTR
#elif defined(TRICKOBLIST_ABSOLUTE_DECLARATION)
#define TRICKOBLIST_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the TrickObList owner or absolute declaration view"
#endif

#ifdef TRICKOBLIST_WITHIN_C_LINKAGE
#define TRICKOBLIST_DECL_LINKAGE extern
#else
#define TRICKOBLIST_DECL_LINKAGE extern "C"
#endif

/** Head of the active trick-trigger object list. */
TRICKOBLIST_DECL_LINKAGE CTrickOb *TrickObList TRICKOBLIST_DECL_ATTR;

#undef TRICKOBLIST_DECL_ATTR
#undef TRICKOBLIST_DECL_LINKAGE
#undef TRICKOBLIST_ABSOLUTE_DECLARATION
#undef TRICKOBLIST_OWNER_DECLARATION
#undef TRICKOBLIST_WITHIN_C_LINKAGE

#endif // TRICKOBLIST_INC_H
