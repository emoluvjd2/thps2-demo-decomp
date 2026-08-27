#ifndef GENERATELEVEL_INC_H
#define GENERATELEVEL_INC_H

/**
 * Pointer-sized level-generation flag, physically owned by LEVELEDITOR.cpp.
 *
 * LEVELEDITOR retains the target's GP-relative small-BSS view while consumers
 * form the absolute address recorded in their target objects.
 */
#ifdef GENERATELEVEL_OWNER_DECLARATIONS
#define GENERATELEVEL_DECL_ATTR
#else
#define GENERATELEVEL_DECL_ATTR __attribute__((section(".data")))
#endif

extern "C" void *GenerateLevel GENERATELEVEL_DECL_ATTR;

#undef GENERATELEVEL_DECL_ATTR

#endif // GENERATELEVEL_INC_H
