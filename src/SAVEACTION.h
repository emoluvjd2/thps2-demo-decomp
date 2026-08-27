/*
 * Focused declaration for the memory-card operation selected by RFRONT.
 * The header is intentionally re-entrant so the owner can keep its GP-sized
 * view while CARD retains the target's absolute-address declaration.
 * `ESaveAction` is provided by FRONT.h before either selector is used.
 */
#ifdef SAVEACTION_ABSOLUTE_DECLARATION
#define SAVEACTION_DECL_ATTR __attribute__((section(".data")))
#else
#define SAVEACTION_DECL_ATTR
#endif

#if defined(__cplusplus) && !defined(SAVEACTION_WITHIN_C_LINKAGE)
extern "C" {
#endif

extern ESaveAction GSaveAction SAVEACTION_DECL_ATTR;

#if defined(__cplusplus) && !defined(SAVEACTION_WITHIN_C_LINKAGE)
}
#endif

#undef SAVEACTION_DECL_ATTR
#undef SAVEACTION_ABSOLUTE_DECLARATION
#undef SAVEACTION_WITHIN_C_LINKAGE
