#ifndef PREAPI_H
#define PREAPI_H

/** Allocate and publish the shared PRE archive manager. */
void PRE_Init(void);

/** Destroy the shared PRE archive manager and clear its handle. */
void PRE_Cleanup(void);

#endif // PREAPI_H
