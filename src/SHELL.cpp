// SHELL.cpp -- Reconstructed translation unit
// Original: SHELL.CPP
// Functions: 3 reconstructed entries
// Address range: 0x80079274 - 0x800793C8
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#include "SHELL.h"

#include "PAD.h"

/** Prototype integer-angle helper used by the cheat-state mixer. */
int arccos(int a);

/** Rolling state used by the shell's controller-input cheat detector. */
unsigned int CheatCheck __attribute__((section(".sdata"))) = 0;

/** Nonzero while gameplay is executing under the front-end shell. */
unsigned char InShell __attribute__((section(".sdata"))) = 0;

// === Function implementations (address order) ===

/**
 * Mixes a value into the rolling cheat-detection state.
 *
 * @param n Value to mix into the state.
 */
void frug(unsigned int n) {
    CheatCheck = (((CheatCheck ^ n) << 1) ^ ((CheatCheck ^ n) >> 31)) * 521;
    CheatCheck += arccos(CheatCheck);
}


/**
 * Mixes a button-specific constant into the cheat state when a controller
 * trigger is active.
 *
 * @return Zero after processing the current controller state.
 */
int Shell_DoCheatProcessing(void) {
    if (Pad[0].Up.Triggered != 0) {
        frug(0x01234567U);
        return 0;
    }
    if (Pad[0].Right.Triggered != 0) {
        frug(0xab432901U);
        return 0;
    }
    if (Pad[0].Down.Triggered != 0) {
        frug(0x776643d1U);
        return 0;
    }
    if (Pad[0].Left.Triggered != 0) {
        frug(0x93fe1682U);
        return 0;
    }
    if (Pad[0].Triangle.Triggered != 0) {
        frug(0x31415926U);
        return 0;
    }
    if (Pad[0].Circle.Triggered != 0) {
        frug(0xdeadbeefU);
        return 0;
    }
    if (Pad[0].X.Triggered != 0) {
        frug(0xb87610dbU);
        return 0;
    }
    if (Pad[0].Square.Triggered != 0) {
        frug(0x03185332U);
        return 0;
    }
    return 0;
}


/**
 * Placeholder for the shell pause-screen hook.
 */
void Shell_PauseScreen(void) {
}
