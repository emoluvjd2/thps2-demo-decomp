#ifndef FRONTPRE_H
#define FRONTPRE_H

#include "globals.h"

enum {
    FrontPreFileNameLength = 32,
    FrontPreInfoCount = 2,
};

/** Maps one front-end screen identifier to its PRE archive filename. */
struct _FrontPreInfo {
    int whichScreen;
    char *pFileName;
};

/** Loads and fades in the PRE archive needed by a front-end screen. */
struct FrontPreLoader {
    struct CClass CClass;
    char mFileName[FrontPreFileNameLength];
    int mSpoolID;
    int mTimeout;
    int mFade;
    int mActive;

    FrontPreLoader();
    ~FrontPreLoader();
    int BeginLoad(int whichScreen);
    int DoneLoading();
};

typedef char FrontPreInfo_size_must_be_8[
    (sizeof(_FrontPreInfo) == 8) ? 1 : -1];
typedef char FrontPreLoader_size_must_be_52[
    (sizeof(FrontPreLoader) == 52) ? 1 : -1];

/**
 * Trivial storage for owners whose recovered lifecycle is still explicit.
 *
 * Keeping the storage trivial prevents a surrounding natural destructor from
 * reordering this object's destructor call.  Access always goes through the
 * typed view below, so FrontPreLoader remains the sole layout authority.
 */
struct FrontPreLoaderStorage {
    int mWords[sizeof(FrontPreLoader) / sizeof(int)];
};

/** Return the FrontPreLoader object living in explicit-lifetime storage. */
static inline FrontPreLoader *FrontPreLoaderFromStorage(
    FrontPreLoaderStorage *storage)
{
    return reinterpret_cast<FrontPreLoader *>(storage);
}

typedef char FrontPreLoaderStorage_size_must_be_52[
    (sizeof(FrontPreLoaderStorage) == sizeof(FrontPreLoader)) ? 1 : -1];

#endif // FRONTPRE_H

#ifdef FRONTPRE_DECLARE_PRE_INFO_OWNER_VIEW
/**
 * Four-byte owner declaration used to form the table's GP-relative address.
 * Typed access remains in FRONTPRE.cpp because the recovered table has two
 * eight-byte records while this compiler view controls the common's extent.
 */
extern int preInfo;

#undef FRONTPRE_DECLARE_PRE_INFO_OWNER_VIEW
#endif

#ifdef FRONTPRE_DECLARE_FILENAME_FORMAT_ABSOLUTE_VIEW
/**
 * Absolute-address view of the owner's four-byte PRE filename format string.
 * Its incomplete extent keeps BeginLoad from selecting a GP-relative access.
 */
extern char FrontPre_preManagerAddr[];

#undef FRONTPRE_DECLARE_FILENAME_FORMAT_ABSOLUTE_VIEW
#endif
