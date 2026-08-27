/*
 * M3D physically owns these renderer-initialization globals in small data.
 * This header is intentionally re-entrant so the owner and M3DINIT retain
 * their original per-symbol declaration points and addressing views.
 */
struct SRotatedVertex;

#if defined(M3DINITSTATE_OWNER_DECLARATION) && \
    defined(M3DINITSTATE_ABSOLUTE_DECLARATION)
#error "Select only one M3DINIT state declaration view"
#elif defined(M3DINITSTATE_OWNER_DECLARATION)
#define M3DINITSTATE_DECL_ATTR
#elif defined(M3DINITSTATE_ABSOLUTE_DECLARATION)
#define M3DINITSTATE_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the M3DINIT state owner or absolute declaration view"
#endif

#if !defined(M3DINITSTATE_DECLARE_DEFAULT_LOD_DISTANCE) && \
    !defined(M3DINITSTATE_DECLARE_ROTATED_VERTICES)
#error "Select at least one M3DINIT state symbol"
#endif

#ifdef M3DINITSTATE_DECLARE_DEFAULT_LOD_DISTANCE
/** Default far depth assigned to models without an explicit LOD distance. */
extern s16 M3d_DefaultLODDistance M3DINITSTATE_DECL_ATTR;
#endif

#ifdef M3DINITSTATE_DECLARE_ROTATED_VERTICES
/** Rotated-vertex view into the shared transformed-vertex workspace. */
extern SRotatedVertex *RotatedVertices M3DINITSTATE_DECL_ATTR;
#endif

#undef M3DINITSTATE_DECLARE_DEFAULT_LOD_DISTANCE
#undef M3DINITSTATE_DECLARE_ROTATED_VERTICES
#undef M3DINITSTATE_DECL_ATTR
#undef M3DINITSTATE_ABSOLUTE_DECLARATION
#undef M3DINITSTATE_OWNER_DECLARATION
