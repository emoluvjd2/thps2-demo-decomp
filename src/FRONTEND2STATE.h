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
extern "C" int FrontEnd2_GameMode FRONTEND2STATE_DECL_ATTR;
#endif

#ifdef FRONTEND2STATE_DECLARE_MUSIC_LOOP_VOICE
/** Voice handle for the looping front-end music track. */
extern "C" int FrontEnd2_MusicLoopVoice FRONTEND2STATE_DECL_ATTR;
#endif

#ifdef FRONTEND2STATE_DECLARE_TWO_PLAYER
/** Whether the active front-end path is selecting a second player. */
extern "C" int FrontEnd2_TwoPlayer FRONTEND2STATE_DECL_ATTR;
#endif

#undef FRONTEND2STATE_DECLARE_GAME_MODE
#undef FRONTEND2STATE_DECLARE_MUSIC_LOOP_VOICE
#undef FRONTEND2STATE_DECLARE_TWO_PLAYER
#undef FRONTEND2STATE_DECL_ATTR
#undef FRONTEND2STATE_ABSOLUTE_DECLARATIONS
