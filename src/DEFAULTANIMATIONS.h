// Shared default-animation table physically owned by PHYSICS.cpp.
#ifndef DEFAULTANIMATIONS_INC_H
#define DEFAULTANIMATIONS_INC_H

#ifdef DEFAULTANIMATIONS_WITHIN_C_LINKAGE
#define DEFAULTANIMATIONS_DECL_LINKAGE extern
#else
#define DEFAULTANIMATIONS_DECL_LINKAGE extern "C"
#endif

#ifdef DEFAULTANIMATIONS_SIZED_DECLARATION
/** Complete default-animation table and writable extra-animation workspace. */
DEFAULTANIMATIONS_DECL_LINKAGE short DefaultAnimations[362];
#else
/** Default-animation list imported without requiring its owner-only bound. */
DEFAULTANIMATIONS_DECL_LINKAGE short DefaultAnimations[];
#endif

#undef DEFAULTANIMATIONS_DECL_LINKAGE
#undef DEFAULTANIMATIONS_SIZED_DECLARATION
#undef DEFAULTANIMATIONS_WITHIN_C_LINKAGE

#endif // DEFAULTANIMATIONS_INC_H
