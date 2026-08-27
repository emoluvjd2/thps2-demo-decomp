/*
 * M3D physically owns the 1024-entry depth-scaled sine table. M3DINIT retains
 * its target-proven incomplete-array view while initializing that storage.
 */
#if defined(WIBBLETABLES_OWNER_DECLARATION) && \
    defined(WIBBLETABLES_CONSUMER_DECLARATION)
#error "Select only one WibbleTables declaration view"
#elif defined(WIBBLETABLES_OWNER_DECLARATION)
#define WIBBLETABLES_DECL_EXTENT 1024
#elif defined(WIBBLETABLES_CONSUMER_DECLARATION)
#define WIBBLETABLES_DECL_EXTENT
#else
#error "Select the WibbleTables owner or consumer declaration view"
#endif

/** Per-depth texture-wibble lookup rows initialized at runtime. */
extern s16 WibbleTables[WIBBLETABLES_DECL_EXTENT];

#undef WIBBLETABLES_DECL_EXTENT
#undef WIBBLETABLES_CONSUMER_DECLARATION
#undef WIBBLETABLES_OWNER_DECLARATION
