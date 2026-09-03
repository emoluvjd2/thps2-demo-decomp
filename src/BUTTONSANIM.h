#ifndef BUTTONSANIM_INC_H
#define BUTTONSANIM_INC_H

struct SAnimFrame;

/* FRONT owns this small-data pointer; BIT addresses it absolutely. */
/** Animation frames used for front-end confirm and back button prompts. */
extern SAnimFrame *ButtonsAnim __attribute__((section(".data")));

#endif // BUTTONSANIM_INC_H
