#ifndef SUSPENDDISTANCE_INC_H
#define SUSPENDDISTANCE_INC_H

/* OB owns this small-data word; TRIG addresses it absolutely. */
/** Distance beyond which environmental objects may be suspended. */
extern unsigned int SuspendDistance
    __attribute__((section(".data")));

#endif // SUSPENDDISTANCE_INC_H
