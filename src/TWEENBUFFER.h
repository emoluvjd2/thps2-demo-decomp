// Shared animation-tween workspace physically owned by M3DUTILS.cpp.

struct SMatrix;

/*
 * This declaration header is intentionally re-entrant so the owner retains
 * MAIN.SYM's complete thirty-element type while address-only consumers keep
 * the original incomplete array declaration emitted by their object.
 */
#if defined(TWEENBUFFER_OWNER_DECLARATION) && \
    defined(TWEENBUFFER_CONSUMER_DECLARATION)
#error "Select only one TweenBuffer declaration view"
#elif defined(TWEENBUFFER_OWNER_DECLARATION)
#define TWEENBUFFER_DECL_BOUND 30
#elif defined(TWEENBUFFER_CONSUMER_DECLARATION)
#define TWEENBUFFER_DECL_BOUND
#else
#error "Select the TweenBuffer owner or consumer declaration view"
#endif

/** Scratch transforms produced while interpolating model parts. */
extern SMatrix TweenBuffer[TWEENBUFFER_DECL_BOUND];

#undef TWEENBUFFER_DECL_BOUND
#undef TWEENBUFFER_CONSUMER_DECLARATION
#undef TWEENBUFFER_OWNER_DECLARATION
