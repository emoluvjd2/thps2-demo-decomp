#ifndef PAUSEDSPRITE_INC_H
#define PAUSEDSPRITE_INC_H

struct Sprite;

/* MAIN owns this small-data pointer; FRONT addresses it absolutely. */
/** Sprite displayed behind the pause-menu text. */
extern Sprite *pPausedSprite __attribute__((section(".data")));

#endif // PAUSEDSPRITE_INC_H
