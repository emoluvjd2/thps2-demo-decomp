/*
 * Shared spool-request queue physically owned by SPOOL.cpp.
 *
 * This declaration header is intentionally re-entrant so the owner and its
 * sole consumer retain their original declaration points and fixed extent.
 */
#ifndef SPOOL_QUEUE_LAYOUT_EMITTED
#error "SPOOLQUEUE.h requires the SPOOL_QUEUE_LAYOUT view from SPOOL.h"
#endif

#if defined(SPOOLQUEUE_OWNER_DECLARATION) && \
    defined(SPOOLQUEUE_CONSUMER_DECLARATION)
#error "Select only one SpoolQ declaration view"
#elif defined(SPOOLQUEUE_OWNER_DECLARATION)
#define SPOOLQUEUE_DECL_EXTENT 20
#elif defined(SPOOLQUEUE_CONSUMER_DECLARATION)
#define SPOOLQUEUE_DECL_EXTENT 20
#else
#error "Select the SpoolQ owner or consumer declaration view"
#endif

#ifdef SPOOLQUEUE_WITHIN_C_LINKAGE
#define SPOOLQUEUE_DECL_LINKAGE extern
#else
#define SPOOLQUEUE_DECL_LINKAGE extern "C"
#endif

/** Twenty-entry packed queue of pending spool requests. */
SPOOLQUEUE_DECL_LINKAGE SQueue SpoolQ[SPOOLQUEUE_DECL_EXTENT];

#undef SPOOLQUEUE_DECL_EXTENT
#undef SPOOLQUEUE_DECL_LINKAGE
#undef SPOOLQUEUE_CONSUMER_DECLARATION
#undef SPOOLQUEUE_OWNER_DECLARATION
#undef SPOOLQUEUE_WITHIN_C_LINKAGE
