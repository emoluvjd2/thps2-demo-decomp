#ifndef CURRENTGAMELEVEL_INC_H
#define CURRENTGAMELEVEL_INC_H

/**
 * Gameplay level currently loaded by MAIN. The owner stores this word in
 * small data; consumers retain their target-recorded absolute address view.
 */
extern "C" int CurrentGameLevel
    __attribute__((section(".data")));

#endif // CURRENTGAMELEVEL_INC_H
