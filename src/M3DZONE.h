#ifndef M3DZONE_INC_H
#define M3DZONE_INC_H

struct SLineInfo;

/** Clears one environment collision zone and its spatial grid. */
void M3dZone_FreePSX(int EnvIndex);

/** Initializes the environment collision-zone system. */
void M3dZone_Init(void);

/** Runs a collision query with the collision scratch workspace enabled. */
int M3dZone_LineToItem(SLineInfo *pInfo, int CheckEnviroObs);

/** Runs a collision query without changing the scratchpad stack pointer. */
int M3dZone_LineToItemNotSCached(SLineInfo *pInfo, int CheckEnviroObs);

/** Installs a packed spatial collision grid for one environment. */
void M3dZone_SetZone(int EnvIndex, unsigned int *pPack);

#endif // M3DZONE_INC_H
