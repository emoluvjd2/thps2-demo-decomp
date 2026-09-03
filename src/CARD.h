#ifndef CARD_H
#define CARD_H

/** Kind of file stored in a THPS memory-card slot. */
enum EFileType
{
    THPS_SAVE_FILE = 0,
    THPS_VIDEO_FILE = 1,
    THPS_VIDEO_HEADER = 2,
    OTHER_FILE = 3,
};

/** Fixed 512-byte header at the start of every THPS memory-card file. */
struct SCardHead
{
    char Magic[2];
    char Type;
    char BlockEntry;
    char Title[64];
    char reserve[28];
    char Clut[32];
    char Icon[3][128];
};

/* Psy-Q owns the directory record returned by the memory-card API. */
#include <KERNEL.H>

typedef char Card_DIRENTRY_size_must_be_40[
    (sizeof(DIRENTRY) == 40) ? 1 : -1];

/** CARD-owned file classifications and directory records. */
extern EFileType FileType[15];
extern DIRENTRY SLOT_DIR[15];

void Card_SetSelectedFile(char *pPName);
int Card_FileExists(EFileType FileType);
int Card_Write(EFileType FileType, int NumBlocks);
int Card_WriteReplayToDisk(EFileType FileType, char *save_name);
int Card_Load(EFileType FileType, unsigned long *pBuf, unsigned long *pBuf2);
int Card_LoadReplayFromCD(
    unsigned long *pBuf, unsigned long *pBuf2, char *file_name);
void Card_Init(long val);
int Card_ReadFileHeader(char *FName, SCardHead *pHeader);
int Card_GetDirAndFreeBlocks(int cnct, int slot);
int Card_CheckStatus(int cnct, int slot);
int Card_FormatCard(int cnct, int slot);

#endif // CARD_H

#ifdef CARD_DECLARE_READ_PATH_ADDRESS_VIEW
/** Absolute address view of CARD's GP-sized named-save path format. */
extern char DAT_800d8ba4[];
#undef CARD_DECLARE_READ_PATH_ADDRESS_VIEW
#endif

#ifdef CARD_DECLARE_DIRECTORY_FILE_TYPE_VIEW
/** Independent address view used while scanning memory-card directory rows. */
extern EFileType CardDirectoryFileType[] asm("FileType");
#undef CARD_DECLARE_DIRECTORY_FILE_TYPE_VIEW
#endif
