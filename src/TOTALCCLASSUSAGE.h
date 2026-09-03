/*
 * MAIN physically owns the CClass allocation counter in initialized small
 * data. INIT retains its target-proven absolute-address import view.
 */
#if defined(TOTALCCLASSUSAGE_OWNER_DECLARATION) && \
    defined(TOTALCCLASSUSAGE_ABSOLUTE_DECLARATION)
#error "Select only one TotalCClassUsage declaration view"
#elif defined(TOTALCCLASSUSAGE_OWNER_DECLARATION)
#define TOTALCCLASSUSAGE_DECL_ATTR
#elif defined(TOTALCCLASSUSAGE_ABSOLUTE_DECLARATION)
#define TOTALCCLASSUSAGE_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the TotalCClassUsage owner or absolute declaration view"
#endif

/** Bytes currently consumed by CClass allocations. */
extern int TotalCClassUsage
    TOTALCCLASSUSAGE_DECL_ATTR;

#undef TOTALCCLASSUSAGE_DECL_ATTR
#undef TOTALCCLASSUSAGE_ABSOLUTE_DECLARATION
#undef TOTALCCLASSUSAGE_OWNER_DECLARATION
