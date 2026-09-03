// FILEIO.cpp -- Reconstructed translation unit
// Original: FILEIO.CPP
// Functions: 11 reconstructed entries
// Address range: 0x8004DC70 - 0x8004E4D4
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#include "DEBUG.h"
#include "GAMEFMV.h"
#include "MEM.h"
#include "PAD.h"
#include "PRE.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBCD.H>
#include <LIBETC.H>
#include <LIBGPU.H>

// === SDK declarations and owner-local address views ===
/** First-byte view that preserves the original GP-relative buffer accesses. */
extern char FileSubDirName;

// Called external functions
#include <STDIO.H>
#include <STRINGS.H>

// Source-owned WAD directory and absolute-addressed views
/** In-memory copy of the WAD directory, populated during FileIO_Init. */
char Directory[22528]
    __attribute__((section(".data"))) = {0};
#include "PREMGR.h"
#define REDBOOKSTATE_DECLARE_VSYNC_SECTOR_CHECK
#define REDBOOKSTATE_DECLARE_XA_MODE_SET
#include "REDBOOKSTATE.h"

// These two eight-byte objects need incomplete absolute-addressed use views:
// exposing their owner sizes before the functions would make GCC select GP
// addressing under `-G8`.
extern RECT FileIO_CdModeRect[];
extern int FileIO_OpenDbgStr[];

// MEM diagnostic globals.  MAIN.SYM names the containing arrays HeapDefs and
// Used; these scalar aliases preserve the original interleaved heap stride
// while exposing the individual fields used by this diagnostic hot path.
#define MEM_HEAP_SCALARS_ABSOLUTE_DECLARATION
#include "MEM.h"

/** Original host-path template retained by the debug file loader. */
static char FilePathPattern[28] __attribute__((section(".rdata"))) =
    "c:\\psx\\sk2demo\\????????.???";

/** Heap diagnostic format used while starting an asynchronous load. */
static char FileIO_LoadDbgStr[36]
    __attribute__((section(".rdata"))) =
    " Heaps: Small(0) %d, Large(1) %d";

/** Complete initialized FILEIO small-data contribution in target order. */
char *Where = 0;
int CurrentPos = 0;
int Loading = 0;
static char *FilePath = FilePathPattern;
static int FileOpen = 0;
static int SectorsPerFrame = 4;
static int ReadSyncFailures = 0;
int SpoolerBlocksFileIO = 0;
int LoadFromPRE = 0;
char *ThePREFile = 0;
int ThePREFileSize = 0;

static char FileIO_InitErrStr[16]
    __attribute__((section(".sdata"))) = "Retry CdInit\n";
static char FileIO_WadName[12]
    __attribute__((section(".sdata"))) = "\\CD.HED;1";
static char FileIO_WadName2[12]
    __attribute__((section(".sdata"))) = "\\CD.WAD;1";
static char FileIO_DebugFmt[12]
    __attribute__((section(".sdata"))) = "fileio.cpp";
namespace FileIOAddressViewOwners
{
extern "C"
{
static RECT FileIO_CdModeRect[1]
    __attribute__((section(".sdata"))) = {
        {485, 18, 10, 10},
    };
static char FileIO_OpenDbgStr[8]
    __attribute__((section(".sdata"))) = "(%d) ";
}
}

int FileIO_Size = 0;
char *Opening = 0;
int LastLoading = 0;

/**
 * Zero-initialized FILEIO request state in its complete target `.sbss` order.
 *
 * MAIN.SYM records every entry as owner-local.  FileSubDirName keeps its
 * storage view separate from the scalar declaration above because GCC's
 * `-G8` decision depends on the declared object size.
 */
namespace FileIOAddressViewOwners
{
extern "C"
{
static char FileSubDirName[16]
    __attribute__((section(".sbss")));
}
}
static int FirstChar
    __attribute__((section(".sbss")));
static unsigned int FileID
    __attribute__((section(".sbss")));
static int FileSize
    __attribute__((section(".sbss")));
static int Remainder
    __attribute__((section(".sbss")));
static int NewLoad
    __attribute__((section(".sbss")));
static int WadStart
    __attribute__((section(".sbss")));
static CdlLOC FileStart
    __attribute__((section(".sbss")));
static unsigned long *Dest
    __attribute__((section(".sbss")));
static int NumSectors
    __attribute__((section(".sbss")));
static int Start
    __attribute__((section(".sbss")));

// Public owner interface. Keep this encounter point aligned with the
// original forward-declaration block.
#define FILEIO_OWNER_DECLARATIONS
#include "FILEIO.h"
#undef FILEIO_OWNER_DECLARATIONS

// === Function implementations (address order) ===

/**
 *
 * Address: 0x8004DC70
 * Mangled: FileIO_OpenLoad__FPc
 *
 * @param pName  Asset / object name string.
 * @return Newly allocated buffer containing the complete file.
 */
char *FileIO_OpenLoad(char *pName) {
    int size = FileIO_Open(pName);
    char *pMemory = reinterpret_cast<char *>(
        Mem_New((unsigned int)size, 1, 1));
    FileIO_Load(pMemory);
    FileIO_Sync();
    unsigned int mask = 0x00FFFFFC;
    int fileSize = FileIO_Size;
    Mem_Shrink(
        pMemory,
        (unsigned int)((fileSize + 3) & mask));
    return pMemory;
}


/**
 * Case-insensitive WAD directory search. Returns 1 if found, 0 if end-of-table.
 *
 * Address: 0x8004DCD4
 * Mangled: CD_There__FPc
 *
 * @param filename  Filename string.
 * @return 1 if the file is present, 0 at the directory sentinel.
 */
int CD_There(char *filename) {
    register unsigned char *name asm("$7") = (unsigned char *)filename;
    asm volatile("" : "+r"(name));
    unsigned char *table = (unsigned char *)Directory;
    unsigned char c1, c2;
    int aligned;
    int align_mask;
    int sentinel;

    if (*name == 0) goto done;

outer:
    align_mask = -4;
    sentinel = 0xFF;

compare:
    c1 = *name++;
    c2 = *table++;
    if ((unsigned int)(c1 - 0x41) < 0x1a) {
        c1 = (c1 + 0x20) & 0xFF;
    }
    if ((unsigned int)(c2 - 0x41) < 0x1a) {
        c2 = (c2 + 0x20) & 0xFF;
    }

    if (c2 != c1) {
        name = (unsigned char *)filename;
    skip_str:
        aligned = *table++;
        if (aligned != 0) goto skip_str;
        goto align_entry;
    }

    if (*name != 0) goto outer;

    // Filename exhausted - check if table string also ended
    if (*table == 0) goto next_entry;

    // Table has more chars - skip rest
    name = (unsigned char *)filename;
    table++;
skip_rest:
    aligned = *table++;
    if (aligned != 0) goto skip_rest;

align_entry:
    aligned = ((int)table + 3) & align_mask;
    table = (unsigned char *)(aligned + 8);
    if (*table == sentinel) return 0;

next_entry:
    if (*name != 0) goto compare;

done:
    return 1;
}


/**
 *
 *
 * Address: 0x8004DDC0
 * Mangled: FileIO_SetSectorsPerFrame__Fi
 *
 * @param n  Count.
 */
void FileIO_SetSectorsPerFrame(int sectors) {
    SectorsPerFrame = sectors;
}


/**
 * Initialize the CD subsystem, load the WAD directory, and locate the main WAD.
 * Failed initialization and file searches are retried after short VSync waits.
 *
 * Address: 0x8004DDCC
 * Mangled: FileIO_Init__Fv
 */
void FileIO_Init(void) {
    CdlFILE fp;
    int nsector;
    int i;

    Loading = 0;
    CdSetDebug(0);

    while (CdInit() == 0) {
        printf(FileIO_InitErrStr);
        i = 0;
        do {
            VSync(0);
            i++;
        } while (i < 100);
    }

    while (CdSearchFile(&fp, FileIO_WadName) == 0) {
        while (CdInit() == 0) {
            i = 0;
            do {
                VSync(0);
                i++;
            } while (i < 100);
        }
        i = 0;
        do {
            VSync(0);
            i++;
        } while (i < 100);
    }

    nsector = (unsigned int)(fp.size + 0x7FF) >> 11;
    CdControl(2, reinterpret_cast<u_char *>(&fp.pos), 0);
    CdRead(nsector, reinterpret_cast<u_long *>(Directory), 0x80);

    while (CdReadSync(1, 0) > 0) {
        VSync(0);
    }

    while (CdSearchFile(&fp, FileIO_WadName2) == 0) {
        i = 0;
        do {
            VSync(0);
            i++;
        } while (i < 100);
    }

    WadStart = CdPosToInt(&fp.pos);
    ReadSyncFailures = 0;
}


/**
 * Open an asset from the loaded PRE archive or locate it in the WAD table.
 *
 * Address: 0x8004DF24
 * Mangled: FileIO_Open__FPCc
 *
 * @param filename  Filename string.
 * @return Exact PRE size, or the sector-rounded CD transfer size.
 */
int FileIO_Open(const char *filename) {
    char *header;
    char *scan;
    int len;

    CurrentPos = 0;
    // Debug_String's target ABI predates this function's const-qualified path.
    Debug_String(const_cast<char *>(filename), 0x159, FileIO_DebugFmt);

    // Check PRE file first
    LoadFromPRE = 0;
    if (PREMgr != 0) {
        ThePREFile = PREMgr->getFile((char *)filename, &ThePREFileSize);
        if (ThePREFile != 0) {
            LoadFromPRE = 1;
            FileOpen = 1;
            FileIO_Size = ThePREFileSize;
            return ThePREFileSize;
        }
    }

    // WAD directory search
    Debug_String(const_cast<char *>(filename), 0x174, FileIO_DebugFmt);
    {
        unsigned int first;
        header = Directory;
        Opening = (char *)filename;
        first = *(unsigned char *)filename;
        scan = (char *)filename;
        if (first != 0) {
        next_character:
            do {
                char letter = *scan++;
            char checkletter = *header++;

            if (letter >= 'A' && letter <= 'Z') letter += 'a' - 'A';
            if (checkletter >= 'A' && checkletter <= 'Z') checkletter += 'a' - 'A';

            if (checkletter != letter) {
                scan = (char *)filename;
                while (*header++) {
                }
                header = (char *)(((int)header + 3) & 0xfffffffc) + 8;

                if ((unsigned char)*header == 255) {
                    RECT rect = FileIO_CdModeRect[0];
                    ClearImage(&rect, 0, 0, 255);
                    rect.y += 256;
                    ClearImage(&rect, 0, 0, 255);
                }
            } else {
                if (*scan != 0) {
                    goto next_character;
                }
                if (*header != 0) {
                    scan = (char *)filename;
                    while (*header++) {
                    }
                    header = (char *)(((int)header + 3) & 0xfffffffc) + 8;

                    if ((unsigned char)*header == 255) {
                        RECT rect = FileIO_CdModeRect[0];
                        ClearImage(&rect, 0, 0, 255);
                        rect.y += 256;
                        ClearImage(&rect, 0, 0, 255);
                    }
                }
            }
            } while (*scan);
        }
    }

    header = (char *)(((int)header + 4) & 0xfffffffc);
    Start = *(int *)header;
    header += 4;
    len = *(int *)header;
    FileIO_Size = len;
    NumSectors = (len + 2048 - 1) / 2048;
    {
        // $5 pin: the sector-rounded size must live in $a1 (not reuse len's
        // $v0) so the Pad lbu can schedule ahead of the sll/store sequence.
        register int rounded asm("$5") = NumSectors * 2048;

        FileSize = rounded;
        FileOpen = 1;

        if (Pad[0].LeftTwo.Pressed != 0) {
            printf((const char *)FileIO_OpenDbgStr);
        }
    }

    Debug_String(const_cast<char *>(filename), 0x20F, FileIO_DebugFmt);
    return FileSize;
}


/**
 * Start loading the open file into the requested destination.
 * PRE-backed files are copied immediately; CD-backed files enter the async
 * state machine advanced by FileIO_Continue.
 *
 * Address: 0x8004E19C
 * Mangled: FileIO_Load__FPv
 *
 * @param where  Destination buffer for the open file.
 */
void FileIO_Load(void *where) {
    if (PREMgr != 0) {
        if (LoadFromPRE) {
            Mem_Copy(where, ThePREFile, ThePREFileSize);
            FileOpen = 0;
            return;
        }
    }

    {
        // Debug memory usage
        int Free0 = MemTop - MemBottom - MemUsed;
        int Free1 = (&MemTop)[2] - (&MemBottom)[2] - (&MemUsed)[1];

        if (Pad[0].LeftTwo.Pressed != 0) {
            printf((const char *)FileIO_LoadDbgStr, Free0, Free1);
        }
    }

    GameFMV_StopFMV();

    Dest = (unsigned long *)where;
    {
        int startPos = Start;
        if (startPos < 0) {
            startPos += 0x7FF;
        }
        CdIntToPos((startPos >> 11) + WadStart, &FileStart);
    }

    Loading = 1;
    Redbook_XACheckSectorOnVSync = 0;
    Redbook_XAModeSet = 0;
    CurrentPos = 0;
    FileIO_Continue();
}


/**
 *
 * Address: 0x8004E2A8
 * Mangled: FileIO_Exists__FPc
 *
 * @param filename  Filename string.
 * @return Nonzero when the file exists on the active medium.
 */
int FileIO_Exists(char *filename) {
    return CD_There(filename);
}


/**
 * Advance the asynchronous CD-read state machine by one step.
 * Read failures are retried and signaled by flashing the screen red.
 *
 * Address: 0x8004E2C8
 * Mangled: FileIO_Continue__Fv
 */
void FileIO_Continue(void) {
    int ReadError;
    unsigned char result[100];
    int status;
    RECT rect;

    if (LoadFromPRE) {
        Loading = 0;
        return;
    }

    LastLoading = Loading;
    ReadError = 0;
    if (Loading == 1) goto case1;
    if (Loading < 2) goto do_retry;
    if (Loading == 2) goto case2;
    if (Loading == 3) goto case3;
    goto do_retry;

case1:
    CurrentPos = 0;
    if (CdControl(2, reinterpret_cast<u_char *>(&FileStart), 0) == 0) {
        ReadError = 1;
        goto do_retry;
    }
    Loading = 2;
case2:
    if (CdRead(NumSectors, Dest, 0x180) == 0) {
        ReadError = 1;
        Loading = 1;
        goto do_retry;
    }
    Loading = 3;

case3:
    // case 3 body
    if (CurrentPos == FileSize) {
        Loading = 0;
        FileOpen = 0;
    } else {
        Pad_Update();
        {
            status = CdReadSync(1, result);
            if (status > 0) {
                ReadSyncFailures = 0;
            } else {
                if (status < 0) {
                    ReadSyncFailures++;
                    ReadError = 1;
                    if (ReadSyncFailures < 5) goto do_retry;
                    {
                        int busyOne = 1;
                        asm volatile("" : "+r"(busyOne));
                        ReadSyncFailures = 0;
                        Loading = busyOne;
                    }
                } else {
                    // result == 0: read complete
                    CurrentPos = FileSize;
                }
            }
        }
    }

do_retry:
    if (ReadError) {
        rect = FileIO_CdModeRect[0];
        ClearImage(&rect, 0xFF, 0, 0);
        rect.y += 0x100;
        ClearImage(&rect, 0xFF, 0, 0);
    }
    return;
}


/**
 *
 * Address: 0x8004E478
 * Mangled: FileIO_Sync__Fv
 *
 */
void FileIO_Sync(void) {
    if (LoadFromPRE) {
        Loading = 0;
        LoadFromPRE = 0;
    } else {
        goto loop_check;
    loop_body:
        FileIO_Continue();
    loop_check:
        if (Loading) goto loop_body;
        FileIO_Continue();
    }
}


/**
 *
 *
 * Address: 0x8004E4C8
 * Mangled: FileIO_GetSubDir__Fv
 *
 */
char *FileIO_GetSubDir(void) {
    return &FileSubDirName;
}


/**
 *
 * Address: 0x8004E4D4
 * Mangled: FileIO_SetSubDir__FPc
 *
 * @param subDir  Subdirectory prefix used for subsequent file operations.
 */
void FileIO_SetSubDir(char *subDir) {
    strcpy(&FileSubDirName, subDir);
}
