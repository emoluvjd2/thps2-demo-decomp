#ifndef LEVELEDITOR_STATE_INC_H
#define LEVELEDITOR_STATE_INC_H

#ifdef LEVELEDITOR_OWNER_DECLARATIONS
#define LEVELEDITOR_STATE_DECL_ATTR
#else
#define LEVELEDITOR_STATE_DECL_ATTR __attribute__((section(".data")))
#endif

/** Active theme index used while rebuilding a generated level. */
extern int GenerateThemeNumber LEVELEDITOR_STATE_DECL_ATTR; // 0x800DBF48

#undef LEVELEDITOR_STATE_DECL_ATTR

#endif // LEVELEDITOR_STATE_INC_H
