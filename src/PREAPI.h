#ifndef PREAPI_H
#define PREAPI_H

/** Allocate and publish the shared PRE archive manager. */
void PRE_Init(void);

/** Destroy the shared PRE archive manager and clear its handle. */
void PRE_Cleanup(void);

#endif // PREAPI_H

#ifdef PREAPI_DECLARE_OWNER_ADDRESS_VIEWS
/** Absolute-address views of PRE's owner-local small-data strings. */
extern char PRE_fmt_s[];
extern char PRE_null_arr[];
#undef PREAPI_DECLARE_OWNER_ADDRESS_VIEWS
#endif
