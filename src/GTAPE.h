#ifndef GTAPE_INC_H
#define GTAPE_INC_H

struct CTape;

#ifdef GTAPE_WITHIN_C_LINKAGE
#define GTAPE_DECL_LINKAGE extern
#else
#define GTAPE_DECL_LINKAGE extern "C"
#endif

/** Tape-object slots populated by the front end for the active level. */
GTAPE_DECL_LINKAGE CTape *GTape[5]
    __attribute__((section(".data")));

#undef GTAPE_DECL_LINKAGE
#undef GTAPE_WITHIN_C_LINKAGE

#endif // GTAPE_INC_H
