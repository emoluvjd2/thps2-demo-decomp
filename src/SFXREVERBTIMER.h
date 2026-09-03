#ifndef SFXREVERBTIMER_INC_H
#define SFXREVERBTIMER_INC_H

/*
 * SFX physically owns the reverb-depth timer in initialized small data.
 * UTILS retains its target-proven absolute-address import view.
 */
#if defined(SFXREVERBTIMER_OWNER_DECLARATION) && \
    defined(SFXREVERBTIMER_ABSOLUTE_DECLARATION)
#error "Select only one SFX reverb timer declaration view"
#elif defined(SFXREVERBTIMER_OWNER_DECLARATION)
#define SFXREVERBTIMER_DECL_ATTR
#elif defined(SFXREVERBTIMER_ABSOLUTE_DECLARATION)
#define SFXREVERBTIMER_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the SFX reverb timer owner or absolute declaration view"
#endif

/** Frames remaining before the pending reverb-depth change is applied. */
extern unsigned int SFX_ReverbDepthSetTimer
    SFXREVERBTIMER_DECL_ATTR;

#undef SFXREVERBTIMER_DECL_ATTR
#undef SFXREVERBTIMER_ABSOLUTE_DECLARATION
#undef SFXREVERBTIMER_OWNER_DECLARATION

#endif // SFXREVERBTIMER_INC_H
