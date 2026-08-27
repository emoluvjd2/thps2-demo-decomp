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

/** Directory record returned by the Psy-Q memory-card API. */
struct DIRENTRY
{
    char name[20];
    long attr;
    long size;
    DIRENTRY *next;
    long head;
    char system[4];
};

typedef char Card_DIRENTRY_size_must_be_40[
    (sizeof(DIRENTRY) == 40) ? 1 : -1];

extern "C" {
/** CARD-owned file classifications and directory records. */
extern EFileType FileType[15];
extern DIRENTRY SLOT_DIR[15];
}

void Card_SetSelectedFile(char *pPName);
void SetSJISChecksum(char *SJISName, char *FName);
short CalcChecksum(unsigned long *pDataBuf, unsigned long DataSize);
void Card_SetHeader(SCardHead *Head, int NumBlocks, char *pClut, char *pIcon);
int SyncToGoodState(void);
int Card_FileExists(EFileType FileType);
int Card_Write(EFileType FileType, int NumBlocks);
int Card_WriteReplayToDisk(EFileType FileType, char *save_name);
int Card_Load(EFileType FileType, unsigned long *pBuf, unsigned long *pBuf2);
int Card_LoadReplayFromCD(
    unsigned long *pBuf, unsigned long *pBuf2, char *file_name);
void Card_Init(long val);
int _card_event(void);
void _clear_event(void);
int _card_event_x(void);
void _clear_event_x(void);
int Card_ReadFileHeader(char *FName, SCardHead *pHeader);
int Card_GetDirAndFreeBlocks(int cnct, int slot);
int Card_CheckStatus(int cnct, int slot);
int Card_FormatCard(int cnct, int slot);
void Card_GetNextTimeStamp(void);

#endif // CARD_H
