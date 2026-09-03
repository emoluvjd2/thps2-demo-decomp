// Shared default-animation table physically owned by PHYSICS.cpp.
#ifndef DEFAULTANIMATIONS_INC_H
#define DEFAULTANIMATIONS_INC_H

#ifdef DEFAULTANIMATIONS_SIZED_DECLARATION
/** Complete default-animation table and writable extra-animation workspace. */
extern short DefaultAnimations[362];
#else
/** Default-animation list imported without requiring its owner-only bound. */
extern short DefaultAnimations[];
#endif

#undef DEFAULTANIMATIONS_SIZED_DECLARATION

#endif // DEFAULTANIMATIONS_INC_H
