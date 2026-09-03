#ifndef NOTINDEMOPIECE_INC_H
#define NOTINDEMOPIECE_INC_H

/*
 * EDITORPIECES physically owns the active unavailable-piece replacement in
 * initialized small data. Consumers retain the target's absolute view.
 */
#if defined(NOTINDEMOPIECE_OWNER_DECLARATION) && \
    defined(NOTINDEMOPIECE_ABSOLUTE_DECLARATION)
#error "Select only one NotInDemoPiece declaration view"
#elif defined(NOTINDEMOPIECE_OWNER_DECLARATION)
#define NOTINDEMOPIECE_DECL_ATTR
#elif defined(NOTINDEMOPIECE_ABSOLUTE_DECLARATION)
#define NOTINDEMOPIECE_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the NotInDemoPiece owner or absolute declaration view"
#endif

/** Theme-specific replacement used for pieces unavailable in the demo. */
extern int NotInDemoPiece
    NOTINDEMOPIECE_DECL_ATTR;

#undef NOTINDEMOPIECE_DECL_ATTR
#undef NOTINDEMOPIECE_ABSOLUTE_DECLARATION
#undef NOTINDEMOPIECE_OWNER_DECLARATION

#endif // NOTINDEMOPIECE_INC_H
