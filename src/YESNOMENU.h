#ifndef YESNOMENU_INC_H
#define YESNOMENU_INC_H

struct CMenu;

/* FRONT physically owns the shared confirmation menu in small data. */
#ifdef YESNOMENU_WITHIN_C_LINKAGE
#define YESNOMENU_DECL_LINKAGE extern
#else
#define YESNOMENU_DECL_LINKAGE extern "C"
#endif

#ifdef YESNOMENU_OWNER_DECLARATION
#define YESNOMENU_DECL_ATTR
#else
#define YESNOMENU_DECL_ATTR __attribute__((section(".data")))
#endif

/** Confirmation menu shared by front-end flows. */
YESNOMENU_DECL_LINKAGE CMenu *pYesNoMenu YESNOMENU_DECL_ATTR;

#undef YESNOMENU_DECL_ATTR
#undef YESNOMENU_DECL_LINKAGE
#undef YESNOMENU_OWNER_DECLARATION
#undef YESNOMENU_WITHIN_C_LINKAGE

#endif // YESNOMENU_INC_H
