#ifndef DEMONAME_INC_H
#define DEMONAME_INC_H

/* FRONT owns this replay filename; MAIN advances its single-letter suffix. */
#ifdef DEMONAME_OWNER_DECLARATION
#define DEMONAME_DECL_TYPE char
#define DEMONAME_DECL_BOUND 30
#else
#define DEMONAME_DECL_TYPE unsigned char
#define DEMONAME_DECL_BOUND
#endif

/** Filename buffer used while cycling through attract-mode replays. */
extern DEMONAME_DECL_TYPE DemoName[DEMONAME_DECL_BOUND];

#undef DEMONAME_DECL_BOUND
#undef DEMONAME_DECL_TYPE
#undef DEMONAME_OWNER_DECLARATION

#endif // DEMONAME_INC_H
