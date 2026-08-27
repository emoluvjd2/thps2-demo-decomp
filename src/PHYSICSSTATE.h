#ifndef PHYSICSSTATE_INC_H
#define PHYSICSSTATE_INC_H

/* PHYSICS physically owns these runtime values in small data. */

/** Optional stat value applied in place of the active skater statistics. */
extern "C" int Cheat_StatsOverride
    __attribute__((section(".data")));

/** Terrain type recorded for the most recently resolved collision face. */
extern "C" int Face_Terrain
    __attribute__((section(".data")));

#endif // PHYSICSSTATE_INC_H
