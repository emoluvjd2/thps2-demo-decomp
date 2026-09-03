/*
 * GAMEFMV physically owns these playback-state words in zero-initialized
 * small data. Cross-owner consumers retain their target-proven absolute
 * addressing views. This header is intentionally re-entrant so each owner
 * declaration stays at its original symbol-order position.
 */
#if defined(GAMEFMVSTATE_OWNER_DECLARATION) && \
    defined(GAMEFMVSTATE_ABSOLUTE_DECLARATION)
#error "Select only one GAMEFMV state declaration view"
#elif defined(GAMEFMVSTATE_OWNER_DECLARATION)
#define GAMEFMVSTATE_DECL_ATTR
#elif defined(GAMEFMVSTATE_ABSOLUTE_DECLARATION)
#define GAMEFMVSTATE_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the GAMEFMV state owner or absolute declaration view"
#endif

#if !defined(GAMEFMVSTATE_DECLARE_ACTIVE) && \
    !defined(GAMEFMVSTATE_DECLARE_CURRENT_FRAME) && \
    !defined(GAMEFMVSTATE_DECLARE_END_X) && \
    !defined(GAMEFMVSTATE_DECLARE_START_X) && \
    !defined(GAMEFMVSTATE_DECLARE_START_Y)
#error "Select at least one GAMEFMV state symbol"
#endif

#ifdef GAMEFMVSTATE_DECLARE_ACTIVE
/** Whether asynchronous movie playback currently owns the decoder. */
extern int GameFMV_Active GAMEFMVSTATE_DECL_ATTR;
#endif

#ifdef GAMEFMVSTATE_DECLARE_CURRENT_FRAME
/** Frame number most recently consumed from the active movie stream. */
#ifdef GAMEFMVSTATE_OWNER_DECLARATION
extern int GameFMV_CurrentFrame GAMEFMVSTATE_DECL_ATTR;
#else
extern int GameFMV_CurrentFrame[] GAMEFMVSTATE_DECL_ATTR;
#endif
#endif

#ifdef GAMEFMVSTATE_DECLARE_END_X
/** Exclusive horizontal end of the active movie decode region. */
extern u16 GameFMV_EndX GAMEFMVSTATE_DECL_ATTR;
#endif

#ifdef GAMEFMVSTATE_DECLARE_START_X
/** Horizontal origin of the active movie decode region. */
extern s16 GameFMV_StartX GAMEFMVSTATE_DECL_ATTR;
#endif

#ifdef GAMEFMVSTATE_DECLARE_START_Y
/** Vertical origin of the active movie decode region. */
extern s16 GameFMV_StartY GAMEFMVSTATE_DECL_ATTR;
#endif

#undef GAMEFMVSTATE_DECL_ATTR
#undef GAMEFMVSTATE_DECLARE_ACTIVE
#undef GAMEFMVSTATE_DECLARE_CURRENT_FRAME
#undef GAMEFMVSTATE_DECLARE_END_X
#undef GAMEFMVSTATE_DECLARE_START_X
#undef GAMEFMVSTATE_DECLARE_START_Y
#undef GAMEFMVSTATE_ABSOLUTE_DECLARATION
#undef GAMEFMVSTATE_OWNER_DECLARATION
