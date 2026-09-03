/*
 * TRIG physically owns the command-point allocation counter in initialized
 * small data. Cross-owner consumers address that word absolutely.
 */
#if defined(TOTALCOMMANDPOINTMEMORY_OWNER_DECLARATION) && \
    defined(TOTALCOMMANDPOINTMEMORY_ABSOLUTE_DECLARATION)
#error "Select only one TotalCommandPointMemory declaration view"
#elif defined(TOTALCOMMANDPOINTMEMORY_OWNER_DECLARATION)
#define TOTALCOMMANDPOINTMEMORY_DECL_ATTR
#elif defined(TOTALCOMMANDPOINTMEMORY_ABSOLUTE_DECLARATION)
#define TOTALCOMMANDPOINTMEMORY_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the TotalCommandPointMemory owner or absolute declaration view"
#endif

/** Bytes currently consumed by allocated trigger command points. */
extern int TotalCommandPointMemory
    TOTALCOMMANDPOINTMEMORY_DECL_ATTR;

#undef TOTALCOMMANDPOINTMEMORY_DECL_ATTR
#undef TOTALCOMMANDPOINTMEMORY_ABSOLUTE_DECLARATION
#undef TOTALCOMMANDPOINTMEMORY_OWNER_DECLARATION
