#ifndef GAMEPREFERENCES_INC_H
#define GAMEPREFERENCES_INC_H

#include "SOPTIONS.h"

/** Runtime wrapper for the player's persistent option settings. */
struct GamePreferences {
    struct SOptions Options;
};

#endif // GAMEPREFERENCES_INC_H
