// CREDITS.cpp -- Reconstructed translation unit
// Original: CREDITS.CPP
// Functions: 1 compiler-generated static initializer
// Address range: 0x80089824 - 0x80089824

#include "SLIGHT.h"

/** Per-credit-stream counters initialized to their first entries. */
int SCROLL_COUNT[2] = {1, 1};

/** Filename of the credits music stream. */
static char CreditsMusicFilename[] = "music.txt";

/** Filename of the credits text stream. */
static char CreditsTextFilename[] = "credits.txt";

/** Filename of the credits font. */
static char CreditsFontFilename[] = "credits.fnt";

/**
 * Directional light used by the skater rendered behind the credits.
 */
SLight Skater_CreditLight = {
    {
        {0, 4096, 0},
        {0, -4096, 0},
        {4096, 0, 0},
    },
    0,
    {
        {1020, 704, 3520},
        {644, 704, 3520},
        {416, 704, 3520},
    },
    0,
    {1024, 1024, 1024},
};

/** Fixed-point depth used by the credit-skater projection. */
static int Z_OVER_S = 945;

/** Fixed-point vertical projection factor. */
static int Z_OVER_SY = 945;

/** Horizontal center of the projected credit skater. */
static int SKATER_MID_POS = 0;

/** Vertical credit-skater position derived during static initialization. */
static int SKATER_Y_POS = Z_OVER_SY * 330;
