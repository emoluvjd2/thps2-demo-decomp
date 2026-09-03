#ifndef TRANSPARENTPALFOREDITOR_INC_H
#define TRANSPARENTPALFOREDITOR_INC_H

/*
 * PAL owns this editor palette-mode word as target BSS.  LEVELGEN retains its
 * target-proven absolute-address import view.
 */
#if defined(TRANSPARENTPALFOREDITOR_OWNER_DECLARATION) && \
    defined(TRANSPARENTPALFOREDITOR_ABSOLUTE_DECLARATION)
#error "Select only one TransparentPalForEditor declaration view"
#elif defined(TRANSPARENTPALFOREDITOR_OWNER_DECLARATION)
#define TRANSPARENTPALFOREDITOR_DECL_ATTR
#elif defined(TRANSPARENTPALFOREDITOR_ABSOLUTE_DECLARATION)
#define TRANSPARENTPALFOREDITOR_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the TransparentPalForEditor owner or absolute declaration view"
#endif

/** Whether palette uploads should retain transparent colours for the editor. */
extern int TransparentPalForEditor TRANSPARENTPALFOREDITOR_DECL_ATTR;

#undef TRANSPARENTPALFOREDITOR_DECL_ATTR
#undef TRANSPARENTPALFOREDITOR_ABSOLUTE_DECLARATION
#undef TRANSPARENTPALFOREDITOR_OWNER_DECLARATION

#endif // TRANSPARENTPALFOREDITOR_INC_H
