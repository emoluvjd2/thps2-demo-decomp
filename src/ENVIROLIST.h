// Shared environment-item list physically owned by SPOOL.cpp.
#ifndef ENVIROLIST_INC_H
#define ENVIROLIST_INC_H

struct Item;

#ifdef ENVIROLIST_WITHIN_C_LINKAGE
#define ENVIROLIST_DECL_LINKAGE extern
#else
#define ENVIROLIST_DECL_LINKAGE extern "C"
#endif

#ifdef ENVIROLIST_OWNER_DECLARATION
#define ENVIROLIST_DECL_ATTR
#elif defined(ENVIROLIST_ABSOLUTE_DECLARATION)
#define ENVIROLIST_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the EnviroList owner or absolute declaration view"
#endif

/** Head of the loaded environment-item chain. */
ENVIROLIST_DECL_LINKAGE Item *EnviroList ENVIROLIST_DECL_ATTR;

#undef ENVIROLIST_DECL_ATTR
#undef ENVIROLIST_DECL_LINKAGE
#undef ENVIROLIST_ABSOLUTE_DECLARATION
#undef ENVIROLIST_OWNER_DECLARATION
#undef ENVIROLIST_WITHIN_C_LINKAGE

#endif // ENVIROLIST_INC_H
