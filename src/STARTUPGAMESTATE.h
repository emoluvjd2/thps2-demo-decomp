// Shared startup save-game state owned by the front end.
#ifndef STARTUPGAMESTATE_INC_H
#define STARTUPGAMESTATE_INC_H

#include "SAVEGAME.h"

#ifdef STARTUPGAMESTATE_WITHIN_C_LINKAGE
#define STARTUPGAMESTATE_DECL_LINKAGE extern
#else
#define STARTUPGAMESTATE_DECL_LINKAGE extern "C"
#endif

/**
 * Save-game state transferred between front-end and gameplay startup.
 *
 * FRONT.cpp fixes the owning definition in `.data`.  Keeping the complete
 * 136-byte type here preserves the consumers' absolute-address imports and
 * their original external-symbol size metadata.
 */
STARTUPGAMESTATE_DECL_LINKAGE SSaveGame StartupGameState;

#undef STARTUPGAMESTATE_DECL_LINKAGE
#undef STARTUPGAMESTATE_WITHIN_C_LINKAGE

#endif // STARTUPGAMESTATE_INC_H
