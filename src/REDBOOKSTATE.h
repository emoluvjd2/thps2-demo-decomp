/*
 * REDBOOK physically owns these XA control words in small data. This header
 * is intentionally re-entrant so the owner's original per-symbol declaration
 * order can coexist with absolute consumer views.
 */
#ifdef REDBOOKSTATE_GP_RELATIVE_DECLARATIONS
#define REDBOOKSTATE_DECL_ATTR
#else
#define REDBOOKSTATE_DECL_ATTR __attribute__((section(".data")))
#endif

#ifdef REDBOOKSTATE_DECLARE_VSYNC_SECTOR_CHECK
/** Whether the VSync path should validate the current XA sector. */
extern bool Redbook_XACheckSectorOnVSync REDBOOKSTATE_DECL_ATTR;
#endif

#ifdef REDBOOKSTATE_DECLARE_XA_MODE_SET
/** Whether XA playback mode has already been configured. */
extern bool Redbook_XAModeSet REDBOOKSTATE_DECL_ATTR;
#endif

#undef REDBOOKSTATE_DECLARE_VSYNC_SECTOR_CHECK
#undef REDBOOKSTATE_DECLARE_XA_MODE_SET
#undef REDBOOKSTATE_DECL_ATTR
#undef REDBOOKSTATE_GP_RELATIVE_DECLARATIONS
