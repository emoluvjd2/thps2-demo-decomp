#ifndef VBLANKUPDATEBAR_INC_H
#define VBLANKUPDATEBAR_INC_H

/* UTILS owns this small-data word; MAIN and RFRONT address it absolutely. */
/** Requests that the vblank callback draw the card-loading progress bar. */
extern int VblankUpdateBar __attribute__((section(".data")));

#endif // VBLANKUPDATEBAR_INC_H
