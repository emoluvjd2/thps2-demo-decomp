#ifndef STARTSCREEN_INC_H
#define STARTSCREEN_INC_H

/* FRONT owns this small-data word; MAIN addresses it absolutely. */
/** Front-end screen selected when gameplay returns to the shell. */
extern int StartScreen __attribute__((section(".data")));

#endif // STARTSCREEN_INC_H
