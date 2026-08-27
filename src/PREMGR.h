#ifndef PREMGR_INC_H
#define PREMGR_INC_H

struct PREManager;

/*
 * PRE physically owns the archive-manager pointer in small data. Consumers
 * retain their target absolute-address view through an explicit .data
 * declaration, while the owner sees the natural GP-relative declaration.
 */
#ifdef PREMGR_WITHIN_C_LINKAGE
#define PREMGR_DECL_LINKAGE extern
#else
#define PREMGR_DECL_LINKAGE extern "C"
#endif

#ifdef PREMGR_OWNER_DECLARATION
#define PREMGR_DECL_ATTR
#else
#define PREMGR_DECL_ATTR __attribute__((section(".data")))
#endif

/** Singleton archive manager physically owned by PRE.cpp. */
PREMGR_DECL_LINKAGE PREManager *PREMgr PREMGR_DECL_ATTR;

#undef PREMGR_DECL_ATTR
#undef PREMGR_DECL_LINKAGE
#undef PREMGR_OWNER_DECLARATION
#undef PREMGR_WITHIN_C_LINKAGE

#endif // PREMGR_INC_H
