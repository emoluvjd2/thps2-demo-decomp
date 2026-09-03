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

/** Release level-owned resources for the requested cleanup mode. */
void Init_Cleanup(int type);

/** Tear down the current game and initialize the requested pack mode. */
void Init_Restart(EPackInitType Mode, int SmallHeapSize);

/** Initialize the display, frontend, video, and camera for gameplay. */
void Init_ForGame(void);

/** Initialize platform callbacks and global startup systems. */
void Init_AtStart(void);

#endif // INIT_INC_H

#ifdef INIT_DECLARE_TOP_OF_CODE_ABSOLUTE_VIEW
/** Linker-provided start address used to place the two runtime heaps. */
extern int TopOfCode __attribute__((section(".data")));
#undef INIT_DECLARE_TOP_OF_CODE_ABSOLUTE_VIEW
#endif

#ifdef INIT_DECLARE_STARTUP_RECT_ADDRESS_VIEW
/** Opaque address view of INIT's GP-sized startup clear rectangle. */
extern RECT DAT_800daaa4[];
#undef INIT_DECLARE_STARTUP_RECT_ADDRESS_VIEW
#endif
