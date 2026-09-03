#ifndef LASTGAMESTATE_INC_H
#define LASTGAMESTATE_INC_H

#include "SAVEGAME.h"

/* FRONT owns this save snapshot; INIT consumes it directly. */
/** Most recent gameplay state captured by the front end. */
extern SSaveGame LastGameState;

#endif // LASTGAMESTATE_INC_H
