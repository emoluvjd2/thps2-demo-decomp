#ifndef HIRES_H
#define HIRES_H

enum {
    HiresSkaterRegionCount = 14,
};

struct SViewport;

extern "C" {
extern SViewport HiResViewport;
extern int SkaterRegion[HiresSkaterRegionCount];
}

/** Initializes high-resolution rendering state. */
void HiRes_Init(bool clear_screen);

/** Clears the current high-resolution ordering table. */
void HiRes_ClearOTag(void);

/** Draws the current high-resolution ordering table and advances buffers. */
void HiRes_DrawOTag(void);

/** Runs the high-resolution title-screen front end. */
void HiRes_TitleScreen(void);

/** Selects the requested level and skater, then launches gameplay. */
void kickIt(int Selected, int guy);

#endif // HIRES_H
