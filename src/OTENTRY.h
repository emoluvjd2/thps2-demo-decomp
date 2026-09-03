#ifndef OTENTRY_INC_H
#define OTENTRY_INC_H

/* SHELLUTILS physically owns the ordering-table cursor in small data. */
/** Ordering-table slot currently selected by the shell renderer. */
extern int OT_entry
    __attribute__((section(".data")));

#endif // OTENTRY_INC_H
