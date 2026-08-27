/*
 * SPOOL owns the active environment checksum tables and alternate-slot index
 * in small data. This header is intentionally re-entrant: M3DCOLIJ first sees
 * OtherEnvIndex through its generated owner declarations, then reapplies the
 * target's absolute-address declaration view beside EnvModelChecksums.
 */
#ifdef SPOOLENV_ABSOLUTE_DECLARATIONS
#define SPOOLENV_DECL_ATTR __attribute__((section(".data")))
#else
#define SPOOLENV_DECL_ATTR
#endif

#ifdef SPOOLENV_DECLARE_ENV_MODEL_CHECKSUMS
/** Per-environment model-checksum tables physically owned by SPOOL.cpp. */
extern "C" unsigned int *EnvModelChecksums[2] SPOOLENV_DECL_ATTR;
#endif
#undef SPOOLENV_DECLARE_ENV_MODEL_CHECKSUMS

#ifdef SPOOLENV_DECLARE_OTHER_ENV_INDEX
/** Index of the inactive environment slot physically owned by SPOOL.cpp. */
extern "C" int OtherEnvIndex SPOOLENV_DECL_ATTR;
#endif
#undef SPOOLENV_DECLARE_OTHER_ENV_INDEX

#undef SPOOLENV_DECL_ATTR
