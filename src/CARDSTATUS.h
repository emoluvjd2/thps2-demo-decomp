#ifndef CARDSTATUS_INC_H
#define CARDSTATUS_INC_H

/** Current memory-card operation and error status. */
extern int CardStatus
    __attribute__((section(".data")));

#undef CARDSTATUS_WITHIN_C_LINKAGE

#endif // CARDSTATUS_INC_H
