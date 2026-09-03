#ifndef SFXV_INC_H
#define SFXV_INC_H

/*
 * SFX physically owns the baddy-playback scratch volume in initialized small
 * data. BADDY retains its target-proven absolute-address import view.
 */
#if defined(SFXV_OWNER_DECLARATION) && defined(SFXV_ABSOLUTE_DECLARATION)
#error "Select only one SFX_v declaration view"
#elif defined(SFXV_OWNER_DECLARATION)
#define SFXV_DECL_ATTR
#elif defined(SFXV_ABSOLUTE_DECLARATION)
#define SFXV_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the SFX_v owner or absolute declaration view"
#endif

/** Scratch volume used while projecting baddy sound effects. */
extern int SFX_v SFXV_DECL_ATTR;

#undef SFXV_DECL_ATTR
#undef SFXV_ABSOLUTE_DECLARATION
#undef SFXV_OWNER_DECLARATION

#endif // SFXV_INC_H
