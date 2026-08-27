#ifndef DEMOTIMER_INC_H
#define DEMOTIMER_INC_H

/*
 * FRONT physically owns the demo countdown word in small data. Consumers
 * address it absolutely, so each inclusion must select the proven view.
 */
#if defined(DEMOTIMER_OWNER_DECLARATION) && \
    defined(DEMOTIMER_ABSOLUTE_DECLARATION)
#error "Select only one DemoTimer declaration view"
#elif defined(DEMOTIMER_OWNER_DECLARATION)
#define DEMOTIMER_DECL_ATTR
#elif defined(DEMOTIMER_ABSOLUTE_DECLARATION)
#define DEMOTIMER_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the DemoTimer owner or absolute declaration view"
#endif

#if defined(DEMOTIMER_OWNER_DECLARATION) || \
    defined(DEMOTIMER_WITHIN_C_LINKAGE)
#define DEMOTIMER_DECL_LINKAGE extern
#else
#define DEMOTIMER_DECL_LINKAGE extern "C"
#endif

/** Countdown used while demo playback is active. */
DEMOTIMER_DECL_LINKAGE int DemoTimer DEMOTIMER_DECL_ATTR;

#undef DEMOTIMER_DECL_LINKAGE
#undef DEMOTIMER_DECL_ATTR
#undef DEMOTIMER_ABSOLUTE_DECLARATION
#undef DEMOTIMER_OWNER_DECLARATION
#undef DEMOTIMER_WITHIN_C_LINKAGE

#endif // DEMOTIMER_INC_H
