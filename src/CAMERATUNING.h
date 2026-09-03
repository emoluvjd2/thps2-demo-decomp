/*
 * CAMERA physically owns the three base-camera tuning words in small data.
 * This header is intentionally re-entrant so CAMPT and MAIN can retain their
 * absolute-address declaration views at the original compiler encounter sites.
 */
#ifdef CAMERA_TUNING_ABSOLUTE_DECLARATIONS
#define CAMERA_TUNING_DECL_ATTR __attribute__((section(".data")))
#else
#define CAMERA_TUNING_DECL_ATTR
#endif

/** Base camera distance selected by the current display layout. */
extern int camDist CAMERA_TUNING_DECL_ATTR;
/** Base camera pitch selected by the current display layout. */
extern int camPitch CAMERA_TUNING_DECL_ATTR;
/** Base camera vertical offset selected by the current display layout. */
extern int camYOffset CAMERA_TUNING_DECL_ATTR;

#undef CAMERA_TUNING_DECL_ATTR
#undef CAMERA_TUNING_ABSOLUTE_DECLARATIONS
