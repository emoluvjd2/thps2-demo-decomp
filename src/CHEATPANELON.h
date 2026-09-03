#ifndef CHEATPANELON_INC_H
#define CHEATPANELON_INC_H

/*
 * MAIN physically owns the panel-visibility cheat in initialized small data.
 * PANEL retains its target-proven absolute-address import view.
 */
#if defined(CHEATPANELON_OWNER_DECLARATION) && \
    defined(CHEATPANELON_ABSOLUTE_DECLARATION)
#error "Select only one Cheat_PanelOn declaration view"
#elif defined(CHEATPANELON_OWNER_DECLARATION)
#define CHEATPANELON_DECL_ATTR
#elif defined(CHEATPANELON_ABSOLUTE_DECLARATION)
#define CHEATPANELON_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the Cheat_PanelOn owner or absolute declaration view"
#endif

/** Whether the in-game score panel is enabled. */
extern int Cheat_PanelOn CHEATPANELON_DECL_ATTR;

#undef CHEATPANELON_DECL_ATTR
#undef CHEATPANELON_ABSOLUTE_DECLARATION
#undef CHEATPANELON_OWNER_DECLARATION

#endif // CHEATPANELON_INC_H
