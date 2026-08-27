/*
 * FRONT physically owns these front-end flow words in small data. This header
 * is intentionally re-entrant so each word can remain at its target-proven
 * declaration point while consumers retain absolute-address import views.
 */
#if defined(FRONTFLOWSTATE_OWNER_DECLARATION) || \
    defined(FRONTFLOWSTATE_WITHIN_C_LINKAGE)
#define FRONTFLOWSTATE_DECL_LINKAGE extern
#else
#define FRONTFLOWSTATE_DECL_LINKAGE extern "C"
#endif

#ifdef FRONTFLOWSTATE_OWNER_DECLARATION
#define FRONTFLOWSTATE_DECL_ATTR
#else
#define FRONTFLOWSTATE_DECL_ATTR __attribute__((section(".data")))
#endif

#ifdef FRONTFLOWSTATE_DECLARE_BACKOUT
/** Whether the active game should return to the front end. */
FRONTFLOWSTATE_DECL_LINKAGE int BackOut FRONTFLOWSTATE_DECL_ATTR;
#endif

#ifdef FRONTFLOWSTATE_DECLARE_ROLL_CREDITS
/** Whether the front end should enter the credits sequence. */
FRONTFLOWSTATE_DECL_LINKAGE int GRollCredits FRONTFLOWSTATE_DECL_ATTR;
#endif

#undef FRONTFLOWSTATE_DECLARE_BACKOUT
#undef FRONTFLOWSTATE_DECLARE_ROLL_CREDITS
#undef FRONTFLOWSTATE_DECL_ATTR
#undef FRONTFLOWSTATE_DECL_LINKAGE
#undef FRONTFLOWSTATE_OWNER_DECLARATION
#undef FRONTFLOWSTATE_WITHIN_C_LINKAGE
