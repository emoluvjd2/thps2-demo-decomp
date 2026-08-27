#ifndef OTENTRY_INC_H
#define OTENTRY_INC_H

/* SHELLUTILS physically owns the ordering-table cursor in small data. */
#ifdef OTENTRY_WITHIN_C_LINKAGE
#define OTENTRY_DECL_LINKAGE extern
#else
#define OTENTRY_DECL_LINKAGE extern "C"
#endif

/** Ordering-table slot currently selected by the shell renderer. */
OTENTRY_DECL_LINKAGE int OT_entry
    __attribute__((section(".data")));

#undef OTENTRY_DECL_LINKAGE
#undef OTENTRY_WITHIN_C_LINKAGE

#endif // OTENTRY_INC_H
