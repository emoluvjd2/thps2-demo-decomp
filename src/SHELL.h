#ifndef SHELL_INC_H
#define SHELL_INC_H

extern "C" {

// FRONT and MAIN address these imports through absolute HI/LO relocations.
// The extern-only attributes preserve that contract; SHELL.cpp still owns
// both definitions in its target-proven small-data contribution.
/** Rolling state used by the shell's controller-input cheat detector. */
extern unsigned int CheatCheck
    __attribute__((section(".data"))); // 0x800DBA30

/** Nonzero while gameplay is executing under the front-end shell. */
extern unsigned char InShell
    __attribute__((section(".data"))); // 0x800DBA34

}

/** Process controller triggers used by the shell's cheat detector. */
int Shell_DoCheatProcessing(void);

/** Run the shell pause-screen hook. */
void Shell_PauseScreen(void);

#endif // SHELL_INC_H
