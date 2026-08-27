#ifndef MECHLIST_INC_H
#define MECHLIST_INC_H

struct CBody;

/**
 * Head of the active mechanical-object list, physically owned by MECH.cpp.
 * Consumers retain an absolute-address declaration while the owner emits the
 * target's four-byte small-BSS common.
 */
#ifdef MECHLIST_OWNER_DECLARATIONS
#define MECHLIST_DECL_ATTR
#else
#define MECHLIST_DECL_ATTR __attribute__((section(".data")))
#endif

extern "C" CBody *MechList MECHLIST_DECL_ATTR;

#undef MECHLIST_DECL_ATTR

#endif // MECHLIST_INC_H
