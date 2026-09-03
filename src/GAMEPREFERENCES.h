#ifndef GAMEPREFERENCES_INC_H
#define GAMEPREFERENCES_INC_H

#include "SOPTIONS.h"

/** Runtime wrapper for the player's persistent option settings. */
struct GamePreferences {
    struct SOptions Options;
};

/** Persistent option values mirrored into the active career save. */
extern GamePreferences GPreferences;

#endif // GAMEPREFERENCES_INC_H
