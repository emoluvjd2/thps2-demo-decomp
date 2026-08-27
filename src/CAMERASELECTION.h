/*
 * CAMERA physically owns the active Mike-camera index in small BSS.  This
 * header is intentionally re-entrant so absolute-address consumers can retain
 * their original declaration points without inventing alternate C++ names.
 */
#ifdef CAMERASELECTION_GP_RELATIVE_DECLARATIONS
#define CAMERASELECTION_DECL_ATTR
#else
#define CAMERASELECTION_DECL_ATTR __attribute__((section(".data")))
#endif

/** Index of the MikeCamera entry used by the active render pass. */
extern "C" unsigned int Camera_CurrentMikeCamera CAMERASELECTION_DECL_ATTR;

#undef CAMERASELECTION_DECL_ATTR
#undef CAMERASELECTION_GP_RELATIVE_DECLARATIONS
