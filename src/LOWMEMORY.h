#ifndef LOWMEMORY_INC_H
#define LOWMEMORY_INC_H

/*
 * MEM physically owns the allocator pressure flag in initialized small data.
 * SHATTER retains its target-proven absolute-address import view.
 */
#if defined(LOWMEMORY_OWNER_DECLARATION) && \
    defined(LOWMEMORY_ABSOLUTE_DECLARATION)
#error "Select only one LowMemory declaration view"
#elif defined(LOWMEMORY_OWNER_DECLARATION)
#define LOWMEMORY_DECL_ATTR
#elif defined(LOWMEMORY_ABSOLUTE_DECLARATION)
#define LOWMEMORY_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the LowMemory owner or absolute declaration view"
#endif

/** Nonzero while allocator pressure requires reduced-memory effects. */
extern int LowMemory LOWMEMORY_DECL_ATTR;

#undef LOWMEMORY_DECL_ATTR
#undef LOWMEMORY_ABSOLUTE_DECLARATION
#undef LOWMEMORY_OWNER_DECLARATION

#endif // LOWMEMORY_INC_H
