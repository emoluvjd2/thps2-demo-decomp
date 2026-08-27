#ifndef DOUBLEBUFFER_INC_H
#define DOUBLEBUFFER_INC_H

struct SDoubleBuffer;

/**
 * The two display buffers physically owned and initialized by DB.cpp.
 *
 * The shared declaration intentionally leaves the array bound incomplete so
 * consumers that need only its address do not inherit the PSYQ GPU layouts.
 * DB's complete definition retains the target-proven two-element bound.
 */
#ifdef DOUBLEBUFFER_DECLARE_KNOWN_BOUND
#define DOUBLEBUFFER_DECL_BOUND 2
#else
#define DOUBLEBUFFER_DECL_BOUND
#endif

extern "C" SDoubleBuffer DoubleBuffer[DOUBLEBUFFER_DECL_BOUND];

#undef DOUBLEBUFFER_DECL_BOUND

#endif // DOUBLEBUFFER_INC_H
