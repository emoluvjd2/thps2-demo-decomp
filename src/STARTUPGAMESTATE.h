// Shared startup save-game state owned by the front end.
#ifndef STARTUPGAMESTATE_INC_H
#define STARTUPGAMESTATE_INC_H

#include "SAVEGAME.h"

/**
 * Save-game state transferred between front-end and gameplay startup.
 *
 * FRONT.cpp fixes the owning definition in `.data`.  Keeping the complete
 * 136-byte type here preserves the consumers' absolute-address imports and
 * their original external-symbol size metadata.
 */
extern SSaveGame StartupGameState;

#endif // STARTUPGAMESTATE_INC_H
