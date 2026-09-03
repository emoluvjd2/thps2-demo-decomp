#if defined(SPOOLER_DECLARE_OWNER_ADDRESS_VIEWS)
/** Absolute-address views of the spooler's owner-local filename strings. */
namespace SpoolerAddressViews
{
extern "C"
{
extern char DAT_800da698[];
extern char DAT_800da69c[];
extern char DAT_800da6a0[];
}
}
#undef SPOOLER_DECLARE_OWNER_ADDRESS_VIEWS
#endif

#ifndef SPOOLER_INC_H
#define SPOOLER_INC_H

#include "globals.h"

/** One entry in the asynchronous file spooler's fixed request table. */
struct SpoolItem {
    void *isAvailable;
    char name[16];
    int type;
    int region;
    unsigned char *pData;
};

#ifdef SPOOLER_ABSOLUTE_MANAGER_DECLARATION
#define SPOOLER_MANAGER_DECL_ATTR __attribute__((section(".data")))
#else
#define SPOOLER_MANAGER_DECL_ATTR
#endif

/** Manages the fixed 64-entry asynchronous file queue. */
struct Spooler {
    struct CClass CClass;
    struct SpoolItem mItem[64];
    int mNumItems;
    int mCurrentItem;
    int mStatus;

    /** Globally active spooler instance. */
    static Spooler *SpoolMgr SPOOLER_MANAGER_DECL_ATTR;

    /** Initialize an empty spooler instance. */
    Spooler();
    /** Release every resource owned by this spooler. */
    ~Spooler();
    /** Add one file request to the queue. */
    int AddFile(char *, int);
    /** Start the request selected by the current queue index. */
    void BeginSpoolingCurrentFile();
    /** Destroy the globally active spooler. */
    static void Cleanup();
    /** Return the loaded data for one queue slot. */
    void *GetFile(int id);
    /** Allocate and construct the globally active spooler. */
    static void Init();
    /** Report whether one queue slot has finished loading. */
    int IsFileLoaded(int id);
    /** Report whether the complete request set is finished. */
    int IsSetDone();
    /** Release the data owned by one queue slot. */
    void KillFile(int slot);
    /** Fill an index array in alternating order around a starting slot. */
    static void OrderHelper(int *array, int num, int first);
    /** Clear every queued request and restore the idle state. */
    void Reset();
    /** Advance one step of the active asynchronous transfer. */
    void Spooool();
    /** Advance the spooler state machine. */
    void Update();
};

#undef SPOOLER_MANAGER_DECL_ATTR

typedef char SpoolItem_size_must_be_32[
    (sizeof(SpoolItem) == 32) ? 1 : -1];
typedef char Spooler_size_must_be_2064[
    (sizeof(Spooler) == 2064) ? 1 : -1];

#endif // SPOOLER_INC_H
