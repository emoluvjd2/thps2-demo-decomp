#ifndef INIT_INC_H
#define INIT_INC_H

#include "PACK.h"

#ifdef INIT_OWNER_DECLARATIONS
#define INIT_SHARED_STATE_DECL_ATTR
#else
#define INIT_SHARED_STATE_DECL_ATTR __attribute__((section(".data")))
#endif

/** Active trigger filename selected for the current level. */
extern "C" char *TrigFileName INIT_SHARED_STATE_DECL_ATTR; // 0x800DAA9C

#undef INIT_SHARED_STATE_DECL_ATTR

/** Delete every active gameplay-object list and reset list-owning systems. */
void Init_KillAll(void);

/** Release level-owned resources for the requested cleanup mode. */
void Init_Cleanup(int type);

/** Rebuild the two allocator ranges and clear subsystem usage counters. */
int Init_ResetMemory(int Heap0Size);

/** Tear down the current game and initialize the requested pack mode. */
void Init_Restart(EPackInitType Mode, int SmallHeapSize);

/** Initialize the display, frontend, video, and camera for gameplay. */
void Init_ForGame(void);

/** Run the source-owned initialization debug hook. */
void Init_DebugStuff(void);

/** Initialize platform callbacks and global startup systems. */
void Init_AtStart(void);

#endif // INIT_INC_H
