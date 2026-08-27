// Front-end PRE archive loading and fade presentation.

#define DB_BUFFER_LAYOUT
#include "DB.h"
#undef DB_BUFFER_LAYOUT
#include "FRONTPRE.h"
#include "PRE.h"
#include <STDIO.H>
#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBGPU.H>

extern "C" void FrontPreLoaderDestructorSlot(FrontPreLoader *, int)
    asm("_._14FrontPreLoader");
// GCC 2.x emits a private copy of `CClass::~CClass` last in every owner that
// derives from CClass.  `MAIN.SYM` records 25 such `_._6CClass` copies, all
// file-local, so this owner defines its own instead of importing another
// object's copy at a different address.
static void FrontPreLoaderCClassDestructorSlot(CClass *, int)
    asm("__DOT__6CClass_8006B53C");

extern "C" {

/** Front-end PRE filename format stored immediately before the loader tables. */
static char FrontPreFilenameFormatOwner[4] asm("FrontPre_preManagerAddr")
    __attribute__((section(".sdata"))) = "%s";

/**
 * FrontPreLoader's dispatch table and the local CClass base table.
 *
 * The public table is split into its four-byte target XDEF anchor and
 * owner-local tail words so GCC retains the constructor's original
 * gp-relative address formation under this TU's -G8 threshold.
 */
int __vt_14FrontPreLoader
    __attribute__((section(".sdata"))) = 0;
static int FrontPreLoaderVtableReserved
    __attribute__((section(".sdata"))) = 0;
static void (*FrontPreLoaderVtableDestructor)(FrontPreLoader *, int)
    __attribute__((section(".sdata"))) = FrontPreLoaderDestructorSlot;
static int FrontPreLoaderVtableEnd
    __attribute__((section(".sdata"))) = 0;

static int FrontPreLoaderBaseVtableOwner asm("DAT_800daae0")
    __attribute__((section(".sdata"))) = 0;
static int FrontPreLoaderBaseVtableReserved
    __attribute__((section(".sdata"))) = 0;
static void (*FrontPreLoaderBaseVtableDestructor)(CClass *, int)
    __attribute__((section(".sdata"))) = FrontPreLoaderCClassDestructorSlot;

}

/** Pointer-only interface to the spool manager methods used by this owner. */
struct Spooler {
    static Spooler *SpoolMgr __attribute__((section(".data")));
    void Reset();
    int AddFile(char *filename, int ignoreExtension);
    void Spooool();
    void Update();
    int IsFileLoaded(int slot);
    void *GetFile(int slot);
};

enum {
    FrontPreInitialTimeout = 3,
    FrontPreFadeStep = 8,
    FrontPreMaximumFade = 255,
    FrontPreRedFadeDivisor = 8,
    FrontPreGreenFadeDivisor = 4,
    FrontPreScreenWidth = 512,
    FrontPreScreenHeight = 240,
};

extern "C"
{
    // PSY-Q classifies this external table from its declared scalar size.  Keep
    // a four-byte address anchor so the original gp-relative access survives;
    // callers use the typed table view below.
    extern int preInfo;

    // The target loads this local sdata string through its absolute address,
    // so this unbounded alias is the caller-side view used by BeginLoad.
    extern char FrontPreFilenameFormatAbsolute[]
        asm("FrontPre_preManagerAddr");

}

// DB owns this small-data pointer, but FRONTPRE addresses it absolutely.
#include "PDOUBLEBUFFER.h"

extern "C"
{
#define PREMGR_WITHIN_C_LINKAGE
#include "PREMGR.h"

}

/** Return the typed view of the front-end PRE selection table. */
static inline _FrontPreInfo *GetFrontPreInfoTable()
{
    return reinterpret_cast<_FrontPreInfo *>(&preInfo);
}

/**
 * Initialize an inactive loader with its short front-end timeout.
 */
FrontPreLoader::FrontPreLoader()
{
    CClass.vtable = &__vt_14FrontPreLoader;
    mActive = 0;
    mTimeout = FrontPreInitialTimeout;
    mFade = 0;
}

/**
 * Restore the composed CClass dispatch table before destruction.
 */
FrontPreLoader::~FrontPreLoader()
{
    CClass.vtable = &FrontPreLoaderBaseVtableOwner;
}

/**
 * Queue the PRE archive associated with a front-end screen.
 *
 * @param whichScreen Screen identifier stored in the PRE selection table.
 * @return 1 when a request is already active or was queued; 0 when the
 *         selected table record has no filename.
 */
int FrontPreLoader::BeginLoad(int whichScreen)
{
    if (mActive != 0)
    {
        return 1;
    }

    _FrontPreInfo *pInfo = GetFrontPreInfoTable();
    while (*pInfo->pFileName != '\0')
    {
        if (whichScreen == pInfo->whichScreen)
            break;
        pInfo++;
    }

    if (pInfo->pFileName == 0)
    {
        return 0;
    }

    {
        mActive = 1;
        sprintf(mFileName, FrontPreFilenameFormatAbsolute, pInfo->pFileName);

        Spooler::SpoolMgr->Reset();
        mSpoolID = Spooler::SpoolMgr->AddFile(mFileName, 0);
        Spooler::SpoolMgr->Spooool();
    }

    return 1;
}

/**
 * Advances the front-end PRE spooler. While a load is active, draws the
 * full-screen fade primitive and installs the PRE data once spooling finishes.
 *
 * @return 1 when the inactive timeout expires or the PRE is installed;
 *         otherwise 0.
 */
int FrontPreLoader::DoneLoading()
{
    POLY_F4 *Poly;
    DR_MODE *dr;

    Spooler::SpoolMgr->Update();

    if (mActive == 0)
    {
        if (mTimeout > 0)
            mTimeout--;
        return mTimeout == 0;
    }

    Poly = reinterpret_cast<POLY_F4 *>(pPoly);
    pPoly = reinterpret_cast<unsigned int *>(Poly + 1);
    setPolyF4(Poly);

    mFade += FrontPreFadeStep;
    if (mFade > FrontPreMaximumFade)
        mFade = FrontPreMaximumFade;

    setRGB0(Poly, mFade / FrontPreRedFadeDivisor,
            mFade / FrontPreGreenFadeDivisor, mFade);
    setXYWH(Poly, 0, 0, FrontPreScreenWidth, FrontPreScreenHeight);

    setSemiTrans(Poly, 1);
    addPrim(pDoubleBuffer->PanelOT, Poly);

    int tpage = GetTPage(2, 2, 0, 0);
    dr = reinterpret_cast<DR_MODE *>(pPoly);
    pPoly = reinterpret_cast<unsigned int *>(dr + 1);
    SetDrawMode(dr, 0, 1, tpage, 0);

    addPrim(pDoubleBuffer->PanelOT, dr);

    if (Spooler::SpoolMgr->IsFileLoaded(mSpoolID))
    {
        PREMgr->spoolPREData(
            reinterpret_cast<char *>(Spooler::SpoolMgr->GetFile(mSpoolID)),
            mFileName);
        return 1;
    }
    return 0;
}

/**
 * Restore the owner-local CClass table and optionally release the object.
 *
 * GCC emitted this private base-destructor copy last in the owner's text, so
 * its definition remains at the end of the translation unit.
 */
static void FrontPreLoaderCClassDestructorSlot(CClass *_this, int __in_chrg)
{
    _this->vtable = &FrontPreLoaderBaseVtableOwner;
    if (__in_chrg & 1)
    {
        CClass::operator delete(_this);
    }
}
