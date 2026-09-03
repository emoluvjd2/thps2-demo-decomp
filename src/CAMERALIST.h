#ifndef CAMERALIST_INC_H
#define CAMERALIST_INC_H

struct CBody;

/**
 * Head of the active camera list, physically owned by CAMERA.cpp.
 *
 * CAMERA retains the target's GP-relative small-BSS view while consumers
 * form the absolute address recorded in their target objects.
 */
#ifdef CAMERALIST_OWNER_DECLARATIONS
#define CAMERALIST_DECL_ATTR
#else
#define CAMERALIST_DECL_ATTR __attribute__((section(".data")))
#endif

extern CBody *CameraList CAMERALIST_DECL_ATTR;

#undef CAMERALIST_DECL_ATTR

#endif // CAMERALIST_INC_H
