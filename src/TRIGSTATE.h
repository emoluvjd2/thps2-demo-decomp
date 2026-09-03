/*
 * TRIG physically owns the trigger-file pointer, node-offset table, and node
 * count in small data.  This header is intentionally re-entrant so each
 * translation unit retains its original declaration order and import view.
 */
#if defined(TRIGSTATE_OWNER_DECLARATION) && \
    defined(TRIGSTATE_ABSOLUTE_DECLARATION)
#error "Select only one TRIG state declaration view"
#elif defined(TRIGSTATE_OWNER_DECLARATION)
#define TRIGSTATE_DECL_ATTR
#elif defined(TRIGSTATE_ABSOLUTE_DECLARATION)
#define TRIGSTATE_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the TRIG state owner or absolute declaration view"
#endif

#if !defined(TRIGSTATE_DECLARE_TRIG_FILE) && \
    !defined(TRIGSTATE_DECLARE_OFFSET_LIST) && \
    !defined(TRIGSTATE_DECLARE_NUM_NODES) && \
    !defined(TRIGSTATE_DECLARE_FILE_SIZE)
#error "Select at least one TRIG state declaration"
#endif

#ifdef TRIGSTATE_DECLARE_NUM_NODES
/** Number of nodes in the active trigger file. */
extern int NumNodes TRIGSTATE_DECL_ATTR;
#endif

#ifdef TRIGSTATE_DECLARE_OFFSET_LIST
/** Indexed table of node-record pointers in the active trigger file. */
extern short **OffsetList TRIGSTATE_DECL_ATTR;
#endif

#ifdef TRIGSTATE_DECLARE_TRIG_FILE
/** Storage containing the active trigger-file image. */
extern unsigned short *TrigFile TRIGSTATE_DECL_ATTR;
#endif

#ifdef TRIGSTATE_DECLARE_FILE_SIZE
/** Compacted byte size of the active trigger-file image. */
extern int TrigFileSize TRIGSTATE_DECL_ATTR;
#endif

#undef TRIGSTATE_DECL_ATTR
#undef TRIGSTATE_ABSOLUTE_DECLARATION
#undef TRIGSTATE_OWNER_DECLARATION
#undef TRIGSTATE_DECLARE_NUM_NODES
#undef TRIGSTATE_DECLARE_OFFSET_LIST
#undef TRIGSTATE_DECLARE_TRIG_FILE
#undef TRIGSTATE_DECLARE_FILE_SIZE
