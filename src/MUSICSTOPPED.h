#ifndef MUSICSTOPPED_INC_H
#define MUSICSTOPPED_INC_H

/* MAIN physically owns the playback-pause word in small data. */
#ifdef MUSICSTOPPED_WITHIN_C_LINKAGE
#define MUSICSTOPPED_DECL_LINKAGE extern
#else
#define MUSICSTOPPED_DECL_LINKAGE extern "C"
#endif

/** Whether replay or movie playback has paused the music stream. */
MUSICSTOPPED_DECL_LINKAGE int MusicStopped
    __attribute__((section(".data")));

#undef MUSICSTOPPED_DECL_LINKAGE
#undef MUSICSTOPPED_WITHIN_C_LINKAGE

#endif // MUSICSTOPPED_INC_H
