#ifndef PHYSICSSTATE_INC_H
#define PHYSICSSTATE_INC_H

/* PHYSICS physically owns these runtime values in small data. */

struct CVector;

/** Optional stat value applied in place of the active skater statistics. */
extern int Cheat_StatsOverride
    __attribute__((section(".data")));

/** Terrain type recorded for the most recently resolved collision face. */
extern int Face_Terrain
    __attribute__((section(".data")));

/** Downward acceleration applied to gravity-driven gameplay objects. */
extern CVector WorldGravity;

#endif // PHYSICSSTATE_INC_H
