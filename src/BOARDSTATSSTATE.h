#ifndef BOARDSTATSSTATE_INC_H
#define BOARDSTATSSTATE_INC_H

#include "MECH.h"

/* MECH owns the two-entry array; MAIN uses its target-proven scalar view. */
/** Per-player board-stat bonuses selected during skater construction. */
extern BoardStats GBoardStats __attribute__((section(".data")));

#endif // BOARDSTATSSTATE_INC_H
