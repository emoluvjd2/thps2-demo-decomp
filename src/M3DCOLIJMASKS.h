/*
 * M3DCOLIJ physically owns the collision-face masks in initialized small
 * data. M3DZONE retains its target-proven absolute-address import view.
 * This header is intentionally re-entrant so both declarations stay at their
 * original owner-local encounter points.
 */
#if defined(M3DCOLIJMASKS_OWNER_DECLARATION) && \
    defined(M3DCOLIJMASKS_ABSOLUTE_DECLARATION)
#error "Select only one M3DCOLIJ mask declaration view"
#elif defined(M3DCOLIJMASKS_OWNER_DECLARATION)
#define M3DCOLIJMASKS_DECL_ATTR
#elif defined(M3DCOLIJMASKS_ABSOLUTE_DECLARATION)
#define M3DCOLIJMASKS_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the M3DCOLIJ mask owner or absolute declaration view"
#endif

#ifdef M3DCOLIJMASKS_WITHIN_C_LINKAGE
#define M3DCOLIJMASKS_DECL_LINKAGE extern
#else
#define M3DCOLIJMASKS_DECL_LINKAGE extern "C"
#endif

/** Collision-face bits accepted by the active environment zone. */
M3DCOLIJMASKS_DECL_LINKAGE unsigned int M3dColij_OneMask
    M3DCOLIJMASKS_DECL_ATTR;

/** Collision-face bits rejected by the active environment zone. */
M3DCOLIJMASKS_DECL_LINKAGE unsigned int M3dColij_ZeroMask
    M3DCOLIJMASKS_DECL_ATTR;

#undef M3DCOLIJMASKS_DECL_LINKAGE
#undef M3DCOLIJMASKS_DECL_ATTR
#undef M3DCOLIJMASKS_ABSOLUTE_DECLARATION
#undef M3DCOLIJMASKS_OWNER_DECLARATION
#undef M3DCOLIJMASKS_WITHIN_C_LINKAGE
