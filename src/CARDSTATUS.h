#ifndef CARDSTATUS_INC_H
#define CARDSTATUS_INC_H

/* CARD physically owns the memory-card status word in small data. */
#ifdef CARDSTATUS_WITHIN_C_LINKAGE
#define CARDSTATUS_DECL_LINKAGE extern
#else
#define CARDSTATUS_DECL_LINKAGE extern "C"
#endif

/** Current memory-card operation and error status. */
CARDSTATUS_DECL_LINKAGE int CardStatus
    __attribute__((section(".data")));

#undef CARDSTATUS_DECL_LINKAGE
#undef CARDSTATUS_WITHIN_C_LINKAGE

#endif // CARDSTATUS_INC_H
