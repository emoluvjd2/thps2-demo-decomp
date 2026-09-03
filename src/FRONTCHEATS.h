#ifndef FRONTCHEATS_INC_H
#define FRONTCHEATS_INC_H

/*
 * MAIN physically owns these front-end cheat toggles in small data.  FRONT
 * retains the target-proven absolute-address import view.
 */
#if defined(FRONTCHEATS_OWNER_DECLARATION) && \
    defined(FRONTCHEATS_ABSOLUTE_DECLARATION)
#error "Select only one front-end cheat declaration view"
#elif defined(FRONTCHEATS_OWNER_DECLARATION)
#define FRONTCHEATS_DECL_ATTR
#elif defined(FRONTCHEATS_ABSOLUTE_DECLARATION)
#define FRONTCHEATS_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the front-end cheat owner or absolute declaration view"
#endif

#ifdef FRONTCHEATS_WITHIN_C_LINKAGE
#define FRONTCHEATS_DECL_LINKAGE extern
#else
#define FRONTCHEATS_DECL_LINKAGE extern "C"
#endif

/** Whether the pause menu permits selecting an arbitrary restart point. */
FRONTCHEATS_DECL_LINKAGE int Cheat_RestartSkip FRONTCHEATS_DECL_ATTR;

/** Whether the front end displays the in-game diagnostic information. */
FRONTCHEATS_DECL_LINKAGE int Cheat_GameInfo FRONTCHEATS_DECL_ATTR;

#undef FRONTCHEATS_DECL_LINKAGE
#undef FRONTCHEATS_DECL_ATTR
#undef FRONTCHEATS_ABSOLUTE_DECLARATION
#undef FRONTCHEATS_OWNER_DECLARATION
#undef FRONTCHEATS_WITHIN_C_LINKAGE

#endif // FRONTCHEATS_INC_H
