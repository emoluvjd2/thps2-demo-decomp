#ifndef PREMGR_INC_H
#define PREMGR_INC_H

struct PREManager;

/*
 * PRE physically owns the archive-manager pointer in small data. Consumers
 * retain their target absolute-address view through an explicit .data
 * declaration, while the owner sees the natural GP-relative declaration.
 */
#ifdef PREMGR_OWNER_DECLARATION
#define PREMGR_DECL_ATTR
#else
#define PREMGR_DECL_ATTR __attribute__((section(".data")))
#endif

/** Singleton archive manager physically owned by PRE.cpp. */
extern PREManager *PREMgr PREMGR_DECL_ATTR;

#undef PREMGR_DECL_ATTR
#undef PREMGR_OWNER_DECLARATION

#endif // PREMGR_INC_H
