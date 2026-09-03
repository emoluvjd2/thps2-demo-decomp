/*
 * FRONTEND2 allocates these front-end selections as four-byte commons. This
 * header is intentionally re-entrant so their original declaration order can
 * be preserved while LEVELSELECT2 retains its absolute-address view.
 */
#ifdef FRONTEND2STATE_ABSOLUTE_DECLARATIONS
#define FRONTEND2STATE_DECL_ATTR __attribute__((section(".data")))
#else
#define FRONTEND2STATE_DECL_ATTR
#endif

#ifdef FRONTEND2STATE_DECLARE_GAME_MODE
/** Game mode selected by the front-end flow. */
extern int FrontEnd2_GameMode FRONTEND2STATE_DECL_ATTR;
#endif

#ifdef FRONTEND2STATE_DECLARE_MUSIC_LOOP_CURRENT_VOL
/** Current volume of the looping front-end music track. */
extern int FrontEnd2_MusicLoopCurrentVol FRONTEND2STATE_DECL_ATTR;
#endif

#ifdef FRONTEND2STATE_DECLARE_MUSIC_LOOP_DESIRED_VOL
/** Requested volume of the looping front-end music track. */
extern int FrontEnd2_MusicLoopDesiredVol FRONTEND2STATE_DECL_ATTR;
#endif

#ifdef FRONTEND2STATE_DECLARE_MUSIC_LOOP_VOICE
/** Voice handle for the looping front-end music track. */
extern int FrontEnd2_MusicLoopVoice FRONTEND2STATE_DECL_ATTR;
#endif

#ifdef FRONTEND2STATE_DECLARE_SELECTED_LEVEL
/** Level selected by the active front-end flow. */
extern int FrontEnd2_SelectedLevel FRONTEND2STATE_DECL_ATTR;
#endif

#ifdef FRONTEND2STATE_DECLARE_TWO_PLAYER
/** Whether the active front-end path is selecting a second player. */
extern int FrontEnd2_TwoPlayer FRONTEND2STATE_DECL_ATTR;
#endif

#ifdef FRONTEND2STATE_DECLARE_LAST_SCREEN
/** Front-end screen to return to after the active selection flow. */
extern int LastScreen FRONTEND2STATE_DECL_ATTR;
#endif

#ifdef FRONTEND2STATE_DECLARE_WHICH_SCREEN
/** Front-end screen currently selected by the shell loop. */
extern int WhichScreen FRONTEND2STATE_DECL_ATTR;
#endif

#undef FRONTEND2STATE_DECLARE_GAME_MODE
#undef FRONTEND2STATE_DECLARE_MUSIC_LOOP_CURRENT_VOL
#undef FRONTEND2STATE_DECLARE_MUSIC_LOOP_DESIRED_VOL
#undef FRONTEND2STATE_DECLARE_MUSIC_LOOP_VOICE
#undef FRONTEND2STATE_DECLARE_SELECTED_LEVEL
#undef FRONTEND2STATE_DECLARE_TWO_PLAYER
#undef FRONTEND2STATE_DECLARE_LAST_SCREEN
#undef FRONTEND2STATE_DECLARE_WHICH_SCREEN
#undef FRONTEND2STATE_DECL_ATTR
#undef FRONTEND2STATE_ABSOLUTE_DECLARATIONS
