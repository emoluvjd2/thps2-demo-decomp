/*
 * FRONT physically owns the soft-reset enable word in initialized small data.
 * Cross-owner consumers address that word absolutely.
 */
#if defined(SOFTRESETSTATE_OWNER_DECLARATION) && \
    defined(SOFTRESETSTATE_ABSOLUTE_DECLARATION)
#error "Select only one GSoftResetEnabled declaration view"
#elif defined(SOFTRESETSTATE_OWNER_DECLARATION)
#define SOFTRESETSTATE_DECL_ATTR
#elif defined(SOFTRESETSTATE_ABSOLUTE_DECLARATION)
#define SOFTRESETSTATE_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the GSoftResetEnabled owner or absolute declaration view"
#endif

#ifdef SOFTRESETSTATE_WITHIN_C_LINKAGE
#define SOFTRESETSTATE_DECL_LINKAGE extern
#else
#define SOFTRESETSTATE_DECL_LINKAGE extern "C"
#endif

/** Whether the front end accepts the start/select soft-reset chord. */
SOFTRESETSTATE_DECL_LINKAGE int GSoftResetEnabled SOFTRESETSTATE_DECL_ATTR;

#undef SOFTRESETSTATE_DECL_LINKAGE
#undef SOFTRESETSTATE_DECL_ATTR
#undef SOFTRESETSTATE_ABSOLUTE_DECLARATION
#undef SOFTRESETSTATE_OWNER_DECLARATION
#undef SOFTRESETSTATE_WITHIN_C_LINKAGE
