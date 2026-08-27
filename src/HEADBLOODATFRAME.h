/*
 * MECH physically owns the fourteen-entry head-blood animation table in
 * initialized data. PHYSICS retains its target-proven incomplete-array view,
 * which forms the table's absolute address without inventing another extent.
 */
#if defined(HEADBLOODATFRAME_OWNER_DECLARATION) && \
    defined(HEADBLOODATFRAME_CONSUMER_DECLARATION)
#error "Select only one HeadBloodAtFrame declaration view"
#elif defined(HEADBLOODATFRAME_OWNER_DECLARATION)
#define HEADBLOODATFRAME_DECL_EXTENT 14
#elif defined(HEADBLOODATFRAME_CONSUMER_DECLARATION)
#define HEADBLOODATFRAME_DECL_EXTENT
#else
#error "Select the HeadBloodAtFrame owner or consumer declaration view"
#endif

#if defined(HEADBLOODATFRAME_OWNER_DECLARATION) || \
    defined(HEADBLOODATFRAME_WITHIN_C_LINKAGE)
#define HEADBLOODATFRAME_DECL_LINKAGE extern
#else
#define HEADBLOODATFRAME_DECL_LINKAGE extern "C"
#endif

/** Animation frame that emits head blood for each skater model. */
HEADBLOODATFRAME_DECL_LINKAGE s16
    HeadBloodAtFrame[HEADBLOODATFRAME_DECL_EXTENT];

#undef HEADBLOODATFRAME_DECL_LINKAGE
#undef HEADBLOODATFRAME_DECL_EXTENT
#undef HEADBLOODATFRAME_CONSUMER_DECLARATION
#undef HEADBLOODATFRAME_OWNER_DECLARATION
#undef HEADBLOODATFRAME_WITHIN_C_LINKAGE
