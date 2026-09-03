// Shared front-end service interface.
#ifndef FRONT_INC_H
#define FRONT_INC_H

struct SLevel;
struct SPreferences;
struct SSaveGame;

#ifdef FRONT_OWNER_DECLARATIONS
#define FRONT_SHARED_STATE_DECL_ATTR
#else
#define FRONT_SHARED_STATE_DECL_ATTR __attribute__((section(".data")))
#endif

/** Whether gameplay is currently paused by the front end. */
extern "C" int GamePaused FRONT_SHARED_STATE_DECL_ATTR; // 0x800D8C94

#undef FRONT_SHARED_STATE_DECL_ATTR

/** Memory-card payload selected by the front end. */
enum ELoadAction
{
    LOAD_VIDEO = 0,
    LOAD_VIDEO_HEADER = 1,
    LOAD_GAME = 2,
};

/** Memory-card payload currently selected by the front end. */
extern "C" ELoadAction GLoadAction;

/** Whether the active replay is being streamed to disk. */
extern "C" int VideoToDisk;

/** Memory-card operation selected by the front end. */
enum ESaveAction
{
    SAVE_VIDEO = 0,
    SAVE_GAME = 1,
};

/** Draw the demo-build label over the current front-end screen. */
void Front_PrintDemo(void);

/** Replace the current front-end background and return its load status. */
int Front_DisplayScreen(char *name);

/** Clear both display buffers and their ordering tables. */
void Front_ClearScreen(void);

/** Wait for the front-end display buffers to reach a synchronized state. */
void FlickerSync(void);

/** Apply one saved preference record to the active front end. */
void Front_SetPreferences(const SPreferences &preferences);

/** Initialize the front-end menu and display state. */
void Front_Init(void);

/** Draw the standard go-back and select prompts. */
void Front_PrintGoBackSelect(void);

/** Draw one horizontally centered front-end prompt. */
void Front_PrintCentredPrompt(const char *text);

/** Draw the active front-end state. */
void Front_Display(void);

/** Enter the competition end-heat sequence. */
void Front_SwitchOnEndHeat(void);

/** Enter the normal end-level menu. */
void Front_SwitchOnEndLevelMenu(void);

/** Enter the start-level menu. */
void Front_SwitchOnStartLevelMenu(void);

/** Draw the current HORSE word state. */
void Front_PrintHorseLetters(void);

/** Enter the saved-video playback screen. */
void Front_SwitchOnVideoScreen(void);

/** Advance the active front-end state by one frame. */
void Front_Update(void);

/** Snapshot the current gameplay and preference state. */
void Front_SaveGameState(void);

/** Restore gameplay and preference state from one saved game. */
void Front_LoadGame(SSaveGame *game);

/** Decode the shared front-end button actions for one frame. */
void Front_GetButtons(int &activated, int &goBack, int &anyButton,
                      int &start);

/** Select which payload the memory-card loader should read. */
void Front_SetLoadAction(ELoadAction action);

/** Select which payload the memory-card writer should save. */
void Front_SetSaveAction(ESaveAction action);

/** Replace the active save-name buffer with one SJIS title. */
void Front_SetSJISName(char *name);

/** Clear the saved-video restart state. */
void Front_ClearGVideoRestartData(void);

/** Clear the front end's pending soft-reset state. */
void ClearSoftReset(void);

/** Select the level presented by the loading screen. */
void LoadingScreen_SetCurrentLevel(int level, int mode, int unused);

#endif // FRONT_INC_H

// Memory-card owners opt in after defining their shared state enums.
#if defined(FRONT_MEMCARD_API) && !defined(FRONT_MEMCARD_API_EMITTED)
#define FRONT_MEMCARD_API_EMITTED

/** Advance the memory-card state machine and return its front-end status. */
EPausedStatus Front_MemCardUpdate(void);

/** Initialize the memory-card menus for one save or load operation. */
void Front_InitMemCardMenus(int resolution, EMemCardState startState,
                            EPausedStatus returnToState);

/** Draw the active low-resolution memory-card menu. */
void Front_MemCardDisplay(void);

#endif // FRONT_MEMCARD_API && !FRONT_MEMCARD_API_EMITTED

#ifdef FRONT_DECLARE_DEFAULT_GAME_STATE_VIEW
/** Read-only owner view of FRONT's target-proven default save payload. */
extern SSaveGame DefaultGameState;

#undef FRONT_DECLARE_DEFAULT_GAME_STATE_VIEW
#endif

#ifdef FRONT_DECLARE_CMENU_VTABLE_ADDRESS_VIEWS
#ifndef FRONT_CMENU_VTABLE_ABI_TYPE
#error FRONT_DECLARE_CMENU_VTABLE_ADDRESS_VIEWS requires a vtable type
#endif

/** Typed owner view of FRONT's CMenu virtual table. */
namespace FrontCMenuVTableOwner
{
extern "C" FRONT_CMENU_VTABLE_ABI_TYPE __vt_5CMenu;
}

/** Integer owner view preserving FRONT's constructor/destructor arithmetic. */
namespace FrontCMenuVTableAddressView
{
extern "C" int __vt_5CMenu;
}

#undef FRONT_CMENU_VTABLE_ABI_TYPE
#undef FRONT_DECLARE_CMENU_VTABLE_ADDRESS_VIEWS
#endif
