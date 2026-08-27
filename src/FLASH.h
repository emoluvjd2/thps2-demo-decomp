// Public screen-flash and fade interface.
#ifndef FLASH_INC_H
#define FLASH_INC_H

/** Clears all active screen-flash and fade state. */
void Flash_Reset(void);

/** Reports whether the active fade has finished. */
int Flash_FadeFinished(void);

/** Starts a colored screen flash when its importance is high enough. */
void Flash_Screen(
    unsigned char StartR, unsigned char StartG, unsigned char StartB,
    int Frames, unsigned char Importance, int Sort);

/** Starts a full-screen fade over the requested number of frames. */
void Flash_Fade(int Frames, int Sort);

/** Advances the active flash or fade by one frame. */
void Flash_Update(void);

/** Draws the active flash or fade into the panel ordering table. */
void Flash_Display(void);

#endif // FLASH_INC_H
