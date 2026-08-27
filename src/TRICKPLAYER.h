#ifndef TRICKPLAYER_INC_H
#define TRICKPLAYER_INC_H

/* TRICKOB physically allocates the active trick-player word as a common. */
#ifdef TRICKPLAYER_WITHIN_C_LINKAGE
#define TRICKPLAYER_DECL_LINKAGE extern
#else
#define TRICKPLAYER_DECL_LINKAGE extern "C"
#endif

#ifdef TRICKPLAYER_OWNER_DECLARATION
#define TRICKPLAYER_DECL_ATTR
#else
#define TRICKPLAYER_DECL_ATTR __attribute__((section(".data")))
#endif

/** Player index associated with the active trick object. */
TRICKPLAYER_DECL_LINKAGE int TrickPlayer TRICKPLAYER_DECL_ATTR;

#undef TRICKPLAYER_DECL_ATTR
#undef TRICKPLAYER_DECL_LINKAGE
#undef TRICKPLAYER_OWNER_DECLARATION
#undef TRICKPLAYER_WITHIN_C_LINKAGE

#endif // TRICKPLAYER_INC_H
