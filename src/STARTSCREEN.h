/*
 * FRONT physically owns the start-screen selection in initialized small data.
 * Cross-owner consumers address that word absolutely.
 */
#if defined(STARTSCREEN_OWNER_DECLARATION) && \
    defined(STARTSCREEN_ABSOLUTE_DECLARATION)
#error "Select only one StartScreen declaration view"
#elif defined(STARTSCREEN_OWNER_DECLARATION)
#define STARTSCREEN_DECL_ATTR
#elif defined(STARTSCREEN_ABSOLUTE_DECLARATION)
#define STARTSCREEN_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the StartScreen owner or absolute declaration view"
#endif

#ifdef STARTSCREEN_WITHIN_C_LINKAGE
#define STARTSCREEN_DECL_LINKAGE extern
#else
#define STARTSCREEN_DECL_LINKAGE extern "C"
#endif

/** Front-end screen selected when gameplay returns to the shell. */
STARTSCREEN_DECL_LINKAGE int StartScreen STARTSCREEN_DECL_ATTR;

#undef STARTSCREEN_DECL_LINKAGE
#undef STARTSCREEN_DECL_ATTR
#undef STARTSCREEN_ABSOLUTE_DECLARATION
#undef STARTSCREEN_OWNER_DECLARATION
#undef STARTSCREEN_WITHIN_C_LINKAGE
