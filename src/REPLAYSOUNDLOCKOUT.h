// Shared replay-audio lockout physically owned by VIDEO.cpp.
#ifndef REPLAYSOUNDLOCKOUT_INC_H
#define REPLAYSOUNDLOCKOUT_INC_H

#ifdef REPLAYSOUNDLOCKOUT_OWNER_DECLARATION
#define REPLAYSOUNDLOCKOUT_DECL_ATTR
#elif defined(REPLAYSOUNDLOCKOUT_ABSOLUTE_DECLARATION)
#define REPLAYSOUNDLOCKOUT_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the ReplaySoundLockout owner or absolute declaration view"
#endif

/** Prevent replay sounds from being emitted recursively during playback. */
extern int ReplaySoundLockout
    REPLAYSOUNDLOCKOUT_DECL_ATTR;

#undef REPLAYSOUNDLOCKOUT_DECL_ATTR
#undef REPLAYSOUNDLOCKOUT_ABSOLUTE_DECLARATION
#undef REPLAYSOUNDLOCKOUT_OWNER_DECLARATION

#endif // REPLAYSOUNDLOCKOUT_INC_H
