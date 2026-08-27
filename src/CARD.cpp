// Memory-card save, replay, directory, and event handling.

#include "CARD.h"
#include "FRONT.h"

/** Stages of the asynchronous card-status state machine. */
enum ECardStatusMode
{
    CARD_STATUS_BEGIN_INFO = 0,
    CARD_STATUS_WAIT_INFO = 1,
    CARD_STATUS_BEGIN_LOAD = 2,
    CARD_STATUS_WAIT_LOAD = 3,
    CARD_STATUS_FINISH = 4,
};

/** Sizes and indices defined by the on-card file format. */
enum ECardFileConstants
{
    CARD_FILENAME_CHECKSUM_INDEX = 22,
    CARD_DIRECTORY_CHECKSUM_INDEX = 17,
    CARD_FILENAME_COMPARE_OFFSET = 5,
    CARD_FILENAME_COMPARE_LENGTH = 17,
    CARD_TITLE_NAME_OFFSET = 28,
    CARD_HEADER_SIZE = 640,
    CARD_SECTOR_SIZE = 512,
    CARD_REPLAY_HEADER_SIZE = 128,
    CARD_CAREER_DATA_SIZE = 2432,
    CARD_REPLAY_DATA_SIZE = -512,
    CARD_COMPLETE_FILE_SIZE = 3072,
    CARD_DIRECTORY_ENTRIES = 15,
    CARD_WRITE_ATTEMPTS = 16,
    CARD_HEADER_READ_ATTEMPTS = 8,
    CARD_EVENT_TIMEOUT = 120,
    CARD_BLOCK_SIZE = 8192,
    CARD_CHANNEL_STRIDE = 16,
    CARD_HEADER_TYPE = 17,
    CARD_TITLE_SIZE = 64,
    CARD_RESERVED_SIZE = 28,
    CARD_CLUT_SIZE = 32,
    CARD_ICON_SIZE = 128,
    CARD_CREATE_MODE = 0x200,
    CARD_BAD_HEADER_MAGIC = 0x6666,
};

/** CARD-owned slice of the career data serialized after a save header. */
struct SCareer
{
    int Version;
    char reserved_004[1998];
    unsigned short TimeStamp;
    char reserved_7d4[416];
};

#define VIDEO_HEAD_LAYOUT
#include "VIDEO.h"
#undef VIDEO_HEAD_LAYOUT

/** Fixed 640-byte prefix shared by career and replay card files. */
struct SCardFilePrefix
{
    SCardHead Card;
    SVideoHead Video;
    char reserved_240[64];
};

/** CARD-owned view of the replay buffer descriptor. */
struct SVideoInfo
{
    EVideoMode VideoMode;
    unsigned long *pBlockStart;
    char reserved_008[184];
};

typedef char SCareer_size_must_be_2420[
    (sizeof(SCareer) == 2420) ? 1 : -1];
typedef char SCardFilePrefix_size_must_be_640[
    (sizeof(SCardFilePrefix) == CARD_HEADER_SIZE) ? 1 : -1];
typedef char SCardHead_size_must_be_512[
    (sizeof(SCardHead) == CARD_SECTOR_SIZE) ? 1 : -1];
typedef char SVideoInfo_size_must_be_192[
    (sizeof(SVideoInfo) == 192) ? 1 : -1];

/** Obtain typed portions of a raw memory-card file buffer. */
static inline SCardFilePrefix *Card_FilePrefix(unsigned long *buffer)
{
    return reinterpret_cast<SCardFilePrefix *>(buffer);
}

static inline unsigned long *Card_FilePayload(unsigned long *buffer)
{
    return reinterpret_cast<unsigned long *>(Card_FilePrefix(buffer) + 1);
}

/** Preserve the low 16 bits used by the card checksum accumulator. */
static inline unsigned short Card_ChecksumBits(short Checksum)
{
    return Checksum;
}

#include "MAIN.h"

extern "C"
{
    extern int GHasCareerLoaded __attribute__((section(".data")));
#define SAVEACTION_ABSOLUTE_DECLARATION
#define SAVEACTION_WITHIN_C_LINKAGE
#include "SAVEACTION.h"
#define VIDEOINFO_DECLARE_GVINFO1
#define VIDEOINFO_DECLARE_GVINFO2
#include "VIDEOINFO.h"

    int TestEvent(int event);
    int format(char *device);
    long open(char *filename, unsigned long mode);
    long close(long file);
    long read(long file, void *buffer, long size);
    void PCinit(void);
    int PCcreat(char *filename, int mode);
    int PClseek(int file, int offset, int mode);
    int PCwrite(int file, unsigned long *buffer, int size);
    int PCclose(int file);
    void InitCARD(void);
    void StartCARD(void);
    void _bu_init(void);
    void EnterCriticalSection(void);
    void ExitCriticalSection(void);
    int OpenEvent(unsigned long descriptor, int spec, int mode, int function);
    int EnableEvent(int event);
    int _card_info(int channel);
    int _card_load(int channel);
    int _card_clear(int channel);
    DIRENTRY *firstfile(char *pattern, DIRENTRY *entry);
    DIRENTRY *nextfile(DIRENTRY *entry);
    int write(int file, void *buffer, int size);
    int erase(char *filename);
}

#include "FILEIO.h"

#include "MEM.h"
#include "UTILS.h"
#include <STDDEF.H>
#include <LIBETC.H>
#include <STDIO.H>
#include <STRINGS.H>

// === CARD-owned small data ===
extern "C" {
int Card_Init_Called = 0;
int CardStatus = 0;
int CardLoadFlag = 0;
}

/** Memory-card path used when reading one named save header. */
static char CardReadFmtStorage[8] asm("DAT_800d8ba4")
    __attribute__((section(".sdata"))) = "bu00:%s";

/**
 * Non-small-data view used by Card_ReadFileHeader.
 *
 * Accessing the defining identifier makes GCC use `$gp`; the target instead
 * materializes this CARD-owned address independently.
 */
extern "C" char CardReadFmtStr[] asm("DAT_800d8ba4");

/** Memory-card wildcard used to enumerate one connector and slot. */
static char CardDirFmtStr[12] asm("DAT_800d8bac")
    __attribute__((section(".sdata"))) = "bu%1d%1d:*";

/** Memory-card device name used by the format operation. */
static char CardFormatStr[12] asm("DAT_800d8bb8")
    __attribute__((section(".sdata"))) = "bu%1d%1d:";

extern "C" {
int CardStatusTemp = 0;
int CardStatusMode = 0;
int TimeStamp = 0;
int ev0 = 0;
int ev1 = 0;
int ev2 = 0;
int ev3 = 0;
int ev10 = 0;
int ev11 = 0;
int ev12 = 0;
int ev13 = 0;
int EVENT_TIME_OUT = 0;
long FILE_DSCRPTR = 0;
}

// === CARD-owned read-only data ===
/** Development-host diagnostic printed before writing the first header. */
static char CardWriteHeaderFmt[36] asm("DAT_800b03b0")
    __attribute__((section(".rdata"))) =
        "\nWriting the header buffer, size %d";

/** Development-host diagnostic printed before writing the first payload. */
static char CardWriteDataFmt[36] asm("DAT_800b03d4")
    __attribute__((section(".rdata"))) =
        "\nWriting the data buffer, size %d";

/** Development-host diagnostic printed before writing the second header. */
static char CardWrite2PHeaderFmt[48] asm("DAT_800b03f8")
    __attribute__((section(".rdata"))) =
        "\nWriting the 2 player header buffer, size %d";

/** Development-host diagnostic printed before writing the second payload. */
static char CardWrite2PDataFmt[44] asm("DAT_800b0428")
    __attribute__((section(".rdata"))) =
        "\nWriting the 2 player data buffer, size %d";

/** Replay checksum diagnostic for the first player payload. */
static char CardReplayChecksum1PMessage[52]
    asm("s_Checksum_error_for_1P_in_Card_Lo_800b0454")
    __attribute__((section(".rdata"))) =
        "\nChecksum error for 1P in Card_LoadReplayFromCD()";

/** Replay checksum diagnostic for the second player payload. */
static char CardReplayChecksum2PMessage[52]
    asm("s_Checksum_error_for_2P_in_Card_Lo_800b0488")
    __attribute__((section(".rdata"))) =
        "\nChecksum error for 2P in Card_LoadReplayFromCD()";

/** Shift-JIS title stored in career-save headers. */
static char CARD_GTITLE[30]
    __attribute__((section(".rdata"))) =
        "\x82\x73\x82\x67\x82\x6f\x82\x72\x81\x40\x82\x62\x82\x60"
        "\x82\x71\x82\x64\x82\x64\x82\x71\x81\x40\x81\x5b\x81\x40";

/** Shift-JIS title stored in replay-save headers. */
static char CARD_VTITLE[30]
    __attribute__((section(".rdata"))) =
        "\x82\x73\x82\x67\x82\x6f\x82\x72\x81\x40\x82\x71\x82\x64"
        "\x82\x6f\x82\x6b\x82\x60\x82\x78\x81\x40\x81\x5b\x81\x40";

/** 16-color CLUT for the replay memory-card icon. */
static char Tape01IconPalette[32]
    __attribute__((section(".rdata"))) = {
        0xFA, 0x5E, 0x74, 0x35, 0x7B, 0x6F, 0x30, 0x4E,
        0x72, 0x56, 0x16, 0x73, 0xF5, 0x6E, 0xE6, 0x1C,
        0x6A, 0x2D, 0x07, 0x25, 0xEE, 0x41, 0xAC, 0x39,
        0x41, 0x08, 0x84, 0x10, 0xFF, 0x7F, 0xF6, 0x62,
};

/** 16x16 four-bit replay memory-card icon pixels. */
static char Tape01IconTexture[128]
    __attribute__((section(".rdata"))) = {
        0x55, 0x55, 0x55, 0x55, 0x55, 0xB4, 0x6B, 0x55,
        0x55, 0x55, 0x55, 0x55, 0xB4, 0xD8, 0x9C, 0x56,
        0x55, 0x55, 0x55, 0x36, 0x8A, 0xAD, 0xB2, 0x59,
        0x55, 0x55, 0x45, 0xAA, 0x79, 0xF8, 0xEE, 0x38,
        0x55, 0x45, 0xBB, 0xD8, 0x2B, 0xAF, 0xEF, 0xCB,
        0x65, 0x8B, 0xC9, 0xFD, 0x42, 0x8A, 0x8D, 0xCC,
        0x84, 0xD9, 0x3C, 0x44, 0x11, 0xAF, 0xDD, 0xCD,
        0xA9, 0xDC, 0xE2, 0x3E, 0x01, 0x7B, 0xD9, 0x37,
        0xBB, 0x8C, 0xEE, 0x42, 0x87, 0x97, 0x7D, 0x56,
        0x76, 0xCD, 0xE2, 0x4E, 0x7D, 0xC7, 0x67, 0x55,
        0x35, 0xDD, 0x29, 0xDF, 0xDD, 0xBC, 0x56, 0x55,
        0x55, 0x79, 0xCC, 0xDC, 0xCD, 0x53, 0x55, 0x55,
        0x55, 0x74, 0xC7, 0xCC, 0x3C, 0x55, 0x55, 0x55,
        0x55, 0xA5, 0x79, 0xCC, 0x53, 0x55, 0x55, 0x55,
        0x55, 0x65, 0xD8, 0x3C, 0x55, 0x55, 0x55, 0x55,
        0x55, 0x55, 0xD4, 0x53, 0x55, 0x55, 0x55, 0x55,
};

/** 16-color CLUT for the career memory-card icon. */
static char Tony01IconPalette[32]
    __attribute__((section(".rdata"))) = {
        0x83, 0x18, 0x2F, 0x5E, 0x91, 0x66, 0x8B, 0x35,
        0xCD, 0x41, 0x07, 0x25, 0x32, 0x42, 0x99, 0x1E,
        0x36, 0x1A, 0xDC, 0x26, 0xBA, 0x26, 0x56, 0x26,
        0x1D, 0x33, 0xB9, 0x32, 0xFF, 0x7F, 0xDE, 0x7B,
};

/** 16x16 four-bit career memory-card icon pixels. */
static char Tony01IconTexture[128]
    __attribute__((section(".rdata"))) = {
        0x22, 0x12, 0x12, 0x12, 0x22, 0x12, 0x22, 0x31,
        0xEE, 0x4E, 0x4E, 0x4E, 0xEE, 0x4E, 0xEE, 0x0E,
        0xE1, 0x55, 0x3E, 0x5E, 0x6E, 0x5E, 0x3E, 0x05,
        0xE1, 0x30, 0xEE, 0x5E, 0xEE, 0x5E, 0xEE, 0x0F,
        0xE1, 0x30, 0x3E, 0x5E, 0x5E, 0x00, 0x34, 0x0F,
        0xE1, 0x30, 0x5E, 0x5E, 0x0E, 0x45, 0xEE, 0x0F,
        0x61, 0x30, 0x04, 0x03, 0x03, 0x43, 0x54, 0x00,
        0x66, 0x64, 0x46, 0x46, 0x46, 0x66, 0x46, 0x33,
        0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xBD,
        0xDC, 0x9A, 0xA9, 0x99, 0x99, 0xA9, 0x9A, 0xBB,
        0x5D, 0x7B, 0xAA, 0xAA, 0xAA, 0xAA, 0xBA, 0x80,
        0xBC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78,
        0x9C, 0x87, 0xAA, 0x7A, 0x7A, 0x7A, 0x78, 0x89,
        0x9C, 0x08, 0x98, 0x99, 0x99, 0xB9, 0x80, 0x77,
        0x99, 0x8A, 0xA7, 0x77, 0x7A, 0x77, 0xA8, 0x87,
        0x87, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
};

// === Absolute-addressed globals ===
extern "C" {
/** Memory-card filename templates for career and replay saves. */
char CARD_FNAME[25] __attribute__((section(".data"))) =
    "bu00:BASLUS-00860THPSGxx";
char CARD_VNAME[25] __attribute__((section(".data"))) =
    "bu00:BASLUS-00860THPSVxx";
}
extern "C" {
/** Per-directory-entry file classification and raw card directory records. */
EFileType FileType[15] __attribute__((section(".data"))) = {THPS_SAVE_FILE};
DIRENTRY SLOT_DIR[15] __attribute__((section(".data"))) = {{{0}}};
}

/**
 * Independent view of the CARD-owned file-type table.
 *
 * Reusing `FileType` makes GCC retain its first address in a saved register;
 * this view preserves the target's second address materialization.
 */
extern "C" EFileType CardDirectoryFileType[] asm("FileType");

// === Function implementations (address order) ===

/**
 * Copy a directory entry's two-character suffix into both save-file names.
 *
 * @param pPName Memory-card directory name containing the selected suffix.
 */
void Card_SetSelectedFile(char *pPName)
{
    CARD_FNAME[CARD_FILENAME_CHECKSUM_INDEX] =
        pPName[CARD_DIRECTORY_CHECKSUM_INDEX];
    CARD_FNAME[CARD_FILENAME_CHECKSUM_INDEX + 1] =
        pPName[CARD_DIRECTORY_CHECKSUM_INDEX + 1];
    CARD_VNAME[CARD_FILENAME_CHECKSUM_INDEX] =
        pPName[CARD_DIRECTORY_CHECKSUM_INDEX];
    CARD_VNAME[CARD_FILENAME_CHECKSUM_INDEX + 1] =
        pPName[CARD_DIRECTORY_CHECKSUM_INDEX + 1];
}

/**
 * Encode a Shift-JIS title checksum as the file name's two-letter suffix.
 *
 * @param SJISName Null-terminated Shift-JIS title words.
 * @param FName Destination for the two encoded characters.
 */
void SetSJISChecksum(char *SJISName, char *FName)
{
    unsigned short *p = reinterpret_cast<unsigned short *>(SJISName);
    unsigned short word = *p;
    // Natural accumulators allocate away from the target's incoming $a0 home.
    register int val asm("$4") = 0;
    if (word != 0)
    {
        do
        {
            p++;
            int highByte = word >> 8;
            word = *p;
            int val2 = (val << 2) +
                (static_cast<unsigned int>(val) >> 10);
            val2 = val2 ^ highByte;
            val = val2 & 0xFFFF;
        } while (word != 0);
    }
    val = val & 0x3FF;
    int val2 = val & 0x1F;
    int upperValue = static_cast<unsigned int>(val) >> 5;
    if (static_cast<unsigned int>(upperValue) < 26)
    {
        *FName = static_cast<char>(upperValue + 'A');
    }
    else
    {
        *FName = static_cast<char>(upperValue + 22);
    }
    if (static_cast<unsigned int>(val2) < 26)
    {
        FName[1] = static_cast<char>(val2 + 'A');
    }
    else
    {
        FName[1] = static_cast<char>(val2 + 22);
    }
}

/**
 * Calculate the signed 16-bit checksum used by career and replay payloads.
 *
 * @param pDataBuf Bytes to checksum.
 * @param DataSize Number of bytes to consume.
 * @return The accumulated checksum.
 */
short CalcChecksum(unsigned long *pDataBuf, unsigned long DataSize)
{
    int i = 0;
    short Checksum = 0;
    if (DataSize == 0)
        goto done;
    do
    {
        int byte = reinterpret_cast<unsigned char *>(pDataBuf)[i];
        i++;
        byte = byte + 1;
        Checksum = Card_ChecksumBits(Checksum) + byte;
    } while (i < DataSize);
done:
    return Checksum;
}

/**
 * Fill a PlayStation memory-card header, title, palette, and three icons.
 *
 * @param Head Header to initialize.
 * @param NumBlocks Number of card blocks occupied by the file.
 * @param pClut Source icon palette.
 * @param pIcon Source icon pixels.
 */
void Card_SetHeader(SCardHead *Head, int NumBlocks, char *pClut, char *pIcon)
{
    int i = 0;

    Head->Magic[0] = 'S';
    Head->Magic[1] = 'C';
    Head->Type = CARD_HEADER_TYPE;
    Head->BlockEntry = NumBlocks;

    for (i = 0; i < CARD_TITLE_SIZE; i++)
    {
        Head->Title[i] = 0;
    }

    if (GSaveAction == SAVE_GAME)
    {
        strcpy(Head->Title, CARD_GTITLE);
    }
    else
    {
        strcpy(Head->Title, CARD_VTITLE);
    }

    i = 0;
    for (; i < CARD_RESERVED_SIZE; i++)
    {
        Head->reserve[i] = 0;
    }

    i = 0;
    for (; i < CARD_CLUT_SIZE; i++)
    {
        Head->Clut[i] = pClut[i];
    }

    i = 0;
    for (; i < CARD_ICON_SIZE; i++)
    {
        Head->Icon[0][i] = pIcon[i];
        Head->Icon[1][i] = pIcon[i];
        Head->Icon[2][i] = pIcon[i];
    }
}

/**
 * Poll the memory-card state machine until it is ready or times out.
 *
 * @return Zero when ready, or one after the retry budget is exhausted.
 */
int SyncToGoodState(void)
{
    int timeout = 200;
    int readyFlag = 1;
    CardStatusMode = CARD_STATUS_BEGIN_INFO;
    CardLoadFlag = 0;
    goto check;
loop:
    if (timeout >= 0)
        goto check;
    CardStatusTemp = 0;
    CardStatus = 0;
    return 1;
check:
    VSync(0);
    Card_CheckStatus(0, 0);
    if (CardLoadFlag != readyFlag)
    {
        timeout--;
        goto loop;
    }
    int newStatus = 1;
    CardStatusTemp = newStatus;
    CardStatus = newStatus;
    return 0;
}

/**
 * Test whether the selected career or replay file exists on the card.
 *
 * @param FileType Save-file category to test.
 * @return One when the file can be opened, otherwise zero.
 */
int Card_FileExists(EFileType FileType)
{
    SCardHead Headdy;
    char *FName;
    if (FileType != THPS_SAVE_FILE)
        goto use2;
    FName = CARD_FNAME;
    goto init;
use2:
    FName = CARD_VNAME;
init:
    Card_SetHeader(
        &Headdy,
        1,
        Headdy.Magic,
        Headdy.Magic);
    Front_SetSJISName(Headdy.Title + CARD_TITLE_NAME_OFFSET);
    SetSJISChecksum(
        Headdy.Title, FName + CARD_FILENAME_CHECKSUM_INDEX);
    if (SyncToGoodState() == 1)
        return 0;
    {
        int fd = open(FName, 1);
        if (fd == -1)
            return 0;
        close(fd);
    }
    return 1;
}

/**
 * Build and write a career or replay save to the memory card.
 *
 * @param FileType Type of payload being saved.
 * @param NumBlocks Number of memory-card blocks to allocate.
 * @return Zero on success, otherwise one.
 */
int Card_Write(EFileType FileType, int NumBlocks)
{
    int fd;
    int i;
    int DataSize;
    int DataSize2;
    int HeaderSize;
    int HeaderSize2;
    unsigned long *pDataBuf;
    unsigned long *pDataBuf2;
    unsigned long *pHeaderBuf;
    unsigned long *pHeaderBuf2;
    char *pClut;
    char *pIcon;
    char *FName;

    if (SyncToGoodState() == 1)
        return 1;

    switch (FileType)
    {
    case THPS_SAVE_FILE:
        Card_GetNextTimeStamp();
        GCareer.TimeStamp = static_cast<unsigned short>(TimeStamp);
        FName = CARD_FNAME;
        pHeaderBuf = GVInfo1.pBlockStart;
        HeaderSize = CARD_HEADER_SIZE;
        pDataBuf = reinterpret_cast<unsigned long *>(&GCareer);
        DataSize = CARD_CAREER_DATA_SIZE;
        pHeaderBuf2 = 0;
        HeaderSize2 = 0;
        pDataBuf2 = 0;
        DataSize2 = 0;
        pClut = Tony01IconPalette;
        pIcon = Tony01IconTexture;
        break;
    case THPS_VIDEO_FILE:
        FName = CARD_VNAME;
        pHeaderBuf = GVInfo1.pBlockStart;
        HeaderSize = CARD_HEADER_SIZE;
        pDataBuf = Card_FilePayload(pHeaderBuf);
        DataSize = CARD_REPLAY_DATA_SIZE;
        pClut = Tape01IconPalette;
        pIcon = Tape01IconTexture;
        pHeaderBuf2 = GVInfo2.pBlockStart;
        HeaderSize2 = CARD_HEADER_SIZE;
        pDataBuf2 = Card_FilePayload(pHeaderBuf2);
        DataSize2 = CARD_REPLAY_DATA_SIZE;
        break;
    case THPS_VIDEO_HEADER:
        break;
    case OTHER_FILE:
        break;
    default:
        break;
    }

    Video_SetHeader(&Card_FilePrefix(pHeaderBuf)->Video, 0);
    if (pHeaderBuf2)
    {
        Video_SetHeader(&Card_FilePrefix(pHeaderBuf2)->Video, 1);
    }
    {
        // Both barriers preserve GCC's target argument-evaluation order.
        // Direct and typed-local calls swap the $a1/$a2 setup instructions.
        SCardHead *pHead = &Card_FilePrefix(pHeaderBuf)->Card;
        asm volatile("" : "+r"(pHead));
        char *pClutArg = pClut;
        asm volatile("" : "+r"(pClutArg));
        Card_SetHeader(pHead, NumBlocks, pClutArg, pIcon);
    }
    Front_SetSJISName(
        Card_FilePrefix(pHeaderBuf)->Card.Title + CARD_TITLE_NAME_OFFSET);
    SetSJISChecksum(
        Card_FilePrefix(pHeaderBuf)->Card.Title,
        FName + CARD_FILENAME_CHECKSUM_INDEX);

    switch (FileType)
    {
    case THPS_SAVE_FILE:
        Card_FilePrefix(pHeaderBuf)->Video.Checksum = CalcChecksum(pDataBuf, DataSize);
        break;
    case THPS_VIDEO_FILE:
        Card_FilePrefix(pHeaderBuf)->Video.Checksum = CalcChecksum(pDataBuf, DataSize);
        if (pHeaderBuf2 != 0)
        {
            Card_FilePrefix(pHeaderBuf2)->Video.Checksum = CalcChecksum(pDataBuf2, DataSize2);
        }
        break;
    case THPS_VIDEO_HEADER:
        break;
    case OTHER_FILE:
        break;
    default:
        break;
    }

    i = 0;
    do
    {
        if (CardStatus != 2 || CardStatusTemp != 2)
        {
            if (FileType == THPS_VIDEO_FILE)
            {
                erase(FName);
            }
            fd = open(FName, (NumBlocks << 16) | CARD_CREATE_MODE);
            if (fd != -1)
            {
                close(fd);
            }
            fd = open(FName, 2);
            if (fd != -1)
            {
                _clear_event();
                if (write(fd, pHeaderBuf, HeaderSize) != HeaderSize)
                {
                    return 1;
                }
                if (write(fd, pDataBuf, DataSize) != DataSize)
                {
                    return 1;
                }
                if (pHeaderBuf2 != 0)
                {
                    if (write(fd, pHeaderBuf2, HeaderSize2) != HeaderSize2)
                    {
                        return 1;
                    }
                    if (write(fd, pDataBuf2, DataSize2) != DataSize2)
                    {
                        return 1;
                    }
                }
                close(fd);
                return 0;
            }
        }
        Pause(1);
        Pause(1);
        Card_CheckStatus(0, 0);
        i++;
    } while (i < CARD_WRITE_ATTEMPTS);
    return 1;
}

/**
 * Writes the replay headers and video buffers to a development-host file.
 *
 * @param FileType Replay file type to write.
 * @param save_name Destination file name on the development host.
 * @return Zero after the file has been written and closed.
 */
int Card_WriteReplayToDisk(EFileType FileType, char *save_name)
{
    unsigned short fileid;
    unsigned long *pHeaderBuf;
    unsigned long *pHeaderBuf2;
    unsigned long *pDataBuf;
    unsigned long *pDataBuf2;
    int HeaderSize;
    int DataSize;
    int DataSize2;

    if (FileType == THPS_VIDEO_FILE)
    {
        HeaderSize = CARD_HEADER_SIZE;
        DataSize = CARD_REPLAY_DATA_SIZE;
        DataSize2 = CARD_REPLAY_DATA_SIZE;
        pHeaderBuf = GVInfo1.pBlockStart;
        pHeaderBuf2 = GVInfo2.pBlockStart;
        pDataBuf = Card_FilePayload(pHeaderBuf);
        pDataBuf2 = Card_FilePayload(pHeaderBuf2);
    }

    Video_SetHeader(&Card_FilePrefix(pHeaderBuf)->Video, 0);
    if (pHeaderBuf2 != 0)
    {
        Video_SetHeader(&Card_FilePrefix(pHeaderBuf2)->Video, 1);
    }

    if (FileType == THPS_VIDEO_FILE)
    {
        Card_FilePrefix(pHeaderBuf)->Video.Checksum = CalcChecksum(pDataBuf, DataSize);
        if (pHeaderBuf2 != 0)
        {
            Card_FilePrefix(pHeaderBuf2)->Video.Checksum = CalcChecksum(pDataBuf2, DataSize2);
        }
    }

    PCinit();
    fileid = PCcreat(save_name, 0) & 0xFFFF;
    PClseek(fileid, 0, 0);
    printf(CardWriteHeaderFmt, HeaderSize);
    PCwrite(fileid, pHeaderBuf, HeaderSize);
    printf(CardWriteDataFmt, DataSize);
    PCwrite(fileid, pDataBuf, DataSize);
    if (pHeaderBuf2 != 0)
    {
        printf(CardWrite2PHeaderFmt, CARD_HEADER_SIZE);
        PCwrite(fileid, pHeaderBuf2, CARD_HEADER_SIZE);
        printf(CardWrite2PDataFmt, DataSize2);
        PCwrite(fileid, pDataBuf2, DataSize2);
    }
    PCclose(fileid);
    return 0;
}

/**
 * Loads a card file, validates its checksum, and optionally checks player 2 data.
 *
 * @param FileType Type of card file to load.
 * @param pBuf Destination for the primary save or replay block.
 * @param pBuf2 Optional destination for the second replay block.
 * @return 0 on success, or 1 if synchronization, I/O, or validation fails.
 */
int Card_Load(EFileType FileType, unsigned long *pBuf_, unsigned long *pBuf2_)
{
    // These aliases retain MAIN.SYM's $s3/$s6 parameter homes. GCC swaps the
    // saved-register assignments when the parameters are used directly; the
    // read-size home likewise determines the target loop's saved-register use.
    register unsigned long *pBuf asm("$19") = pBuf_;
    register unsigned long *pBuf2 asm("$22") = pBuf2_;
    int fd;
    char *FName;
    register int ReadSize asm("$18");

    switch (FileType)
    {
    case THPS_SAVE_FILE:
        FName = CARD_FNAME;
        ReadSize = CARD_COMPLETE_FILE_SIZE;
        break;
    case THPS_VIDEO_FILE:
        FName = CARD_VNAME;
        ReadSize = CARD_REPLAY_HEADER_SIZE;
        break;
    case THPS_VIDEO_HEADER:
        FName = CARD_VNAME;
        ReadSize = CARD_HEADER_SIZE;
        break;
    case OTHER_FILE:
        FName = 0;
        ReadSize = 0;
        break;
    }

    if (SyncToGoodState() != 1)
    {
        goto load_file;
    }

load_failed:
    // This inherited empty barrier keeps the failure return as its own target
    // block; direct-return and structured alternatives merge it into load_file.
    asm volatile("");
    return 1;

load_file:
    fd = open(FName, 1);
    if (fd == -1)
    {
        return 1;
    }

    _clear_event();
    if (read(fd, pBuf, ReadSize) != ReadSize)
    {
        goto load_failed;
    }

        {
            int Chk1 = 0;
            int Chk2 = 0;

            switch (FileType)
            {
            case THPS_SAVE_FILE:
                Chk1 = Card_FilePrefix(pBuf)->Video.Checksum;
                Chk2 = CalcChecksum(
                    Card_FilePayload(pBuf), CARD_CAREER_DATA_SIZE);
                break;
            case THPS_VIDEO_FILE:
                Chk1 = Card_FilePrefix(pBuf)->Video.Checksum;
                Chk2 = CalcChecksum(
                    Card_FilePayload(pBuf), CARD_REPLAY_DATA_SIZE);
                break;
            case THPS_VIDEO_HEADER:
            case OTHER_FILE:
            default:
                break;
            }

            if (Chk1 != Chk2)
            {
                return 1;
            }
        }

        if (pBuf2 != 0)
        {
            if (read(fd, pBuf2, ReadSize) != ReadSize)
            {
                goto load_failed;
            }

            int Chk1 = 0;
            int Chk2 = 0;
            if (FileType == THPS_VIDEO_FILE)
            {
                Chk1 = Card_FilePrefix(pBuf2)->Video.Checksum;
                Chk2 = CalcChecksum(
                    Card_FilePayload(pBuf2), CARD_REPLAY_DATA_SIZE);
            }

            if (Chk1 != Chk2)
            {
                return 1;
            }
        }

    close(fd);
    return 0;
}

/**
 * Load replay headers from the CD image and validate both player checksums.
 *
 * @param pBuf Destination for player one's replay header.
 * @param pBuf2 Optional destination for player two's replay header.
 * @param file_name CD file to load.
 * @return Zero after the load and checksum diagnostics complete.
 */
int Card_LoadReplayFromCD(unsigned long *pBuf, unsigned long *pBuf2, char *file_name)
{
    unsigned int size;
    unsigned long *pData;
    int Chk1;

    size = static_cast<unsigned int>(FileIO_Open(file_name));
    pData = static_cast<unsigned long *>(Mem_New(size, 1, 1));
    if (pData != 0)
    {
        FileIO_Load(pData);
        FileIO_Sync();
    }
    Mem_Copy(pBuf, pData, CARD_REPLAY_HEADER_SIZE);
    if (pBuf2 != 0)
    {
        Mem_Copy(
            pBuf2,
            pData + CARD_REPLAY_HEADER_SIZE / sizeof(unsigned long),
            CARD_REPLAY_HEADER_SIZE);
    }
    Mem_Delete(pData);
    Chk1 = Card_FilePrefix(pBuf)->Video.Checksum;
    if (Chk1 != CalcChecksum(Card_FilePayload(pBuf), CARD_REPLAY_DATA_SIZE))
    {
        printf(CardReplayChecksum1PMessage);
    }
    if (pBuf2 != 0)
    {
        Chk1 = Card_FilePrefix(pBuf2)->Video.Checksum;
        if (Chk1 != CalcChecksum(
                Card_FilePayload(pBuf2), CARD_REPLAY_DATA_SIZE))
        {
            printf(CardReplayChecksum2PMessage);
        }
    }
    return 0;
}

/**
 * Initializes card events and resets the card status state.
 *
 * @param val Reserved initialization argument.
 */
void Card_Init(long val)
{
    Card_Init_Called = 1;
    InitCARD();
    StartCARD();
    _bu_init();
    EnterCriticalSection();
    ev0 = OpenEvent(0xF4000001, 4, 0x2000, 0);
    ev1 = OpenEvent(0xF4000001, 0x8000, 0x2000, 0);
    ev2 = OpenEvent(0xF4000001, 0x100, 0x2000, 0);
    ev3 = OpenEvent(0xF4000001, 0x2000, 0x2000, 0);
    ev10 = OpenEvent(0xF0000011, 4, 0x2000, 0);
    ev11 = OpenEvent(0xF0000011, 0x8000, 0x2000, 0);
    ev12 = OpenEvent(0xF0000011, 0x100, 0x2000, 0);
    ev13 = OpenEvent(0xF0000011, 0x2000, 0x2000, 0);
    ExitCriticalSection();
    EnableEvent(ev0);
    EnableEvent(ev1);
    EnableEvent(ev2);
    EnableEvent(ev3);
    EnableEvent(ev10);
    EnableEvent(ev11);
    EnableEvent(ev12);
    EnableEvent(ev13);
    CardStatusMode = CARD_STATUS_BEGIN_INFO;
    CardLoadFlag = 0;
}

/**
 * Poll the primary card events and apply the event timeout.
 *
 * @return Event number one through four, or two on timeout.
 */
int _card_event(void)
{
    int ret;
    ret = TestEvent(ev0) == 1;
    if (TestEvent(ev1) == 1)
        ret = 2;
    if (TestEvent(ev2) == 1)
        ret = 3;
    if (TestEvent(ev3) == 1)
        ret = 4;
    if (EVENT_TIME_OUT++ >= CARD_EVENT_TIMEOUT)
        ret = 2;
    return ret;
}

/** Clear all pending primary card events. */
void _clear_event(void)
{
    TestEvent(ev0);
    TestEvent(ev1);
    TestEvent(ev2);
    TestEvent(ev3);
}

/**
 * Wait for one of the secondary card events.
 *
 * @return Event number one through four.
 */
int _card_event_x(void)
{
    do
    {
        if (TestEvent(ev10) == 1)
            return 1;
        if (TestEvent(ev11) == 1)
            return 2;
        if (TestEvent(ev12) == 1)
            return 3;
    } while (TestEvent(ev13) != 1);
    return 4;
}

/** Clear all pending secondary card events. */
void _clear_event_x(void)
{
    TestEvent(ev10);
    TestEvent(ev11);
    TestEvent(ev12);
    TestEvent(ev13);
}

/**
 * Read one complete save-file header, retrying while the card is unavailable.
 *
 * @param FName File name relative to the first memory card.
 * @param pHeader Destination header.
 * @return Zero on success, otherwise one after all retries fail.
 */
int Card_ReadFileHeader(char *FName, SCardHead *pHeader)
{
    char FileName[30];
    int i;
    int fd;
    int len;

    sprintf(FileName, CardReadFmtStr, FName);
    i = 0;
    do
    {
        fd = open(FileName, 1);
        if (fd == -1)
        {
            *reinterpret_cast<short *>(pHeader) = CARD_BAD_HEADER_MAGIC;
            Pause(1);
            Card_CheckStatus(0, 0);
        }
        else
        {
            _clear_event();
            len = read(fd, pHeader, sizeof(SCardHead));
            close(fd);
            if (len == sizeof(SCardHead))
            {
                return 0;
            }
        }
        i++;
    } while (i < CARD_HEADER_READ_ATTEMPTS);
    return 1;
}

/**
 * Map the selected card directory, classify saves, and count free blocks.
 *
 * @param cnct Memory-card connector number.
 * @param slot Memory-card slot number.
 * @return The number of free blocks on the selected card.
 */
int Card_GetDirAndFreeBlocks(int cnct, int slot)
{
    EFileType *fileType;
    long i;
    long size;
    char key[32];
    DIRENTRY *d;

    size = i = 0;
    {
        i = size;
        EFileType emptyFileType = OTHER_FILE;
        EFileType *fileTypeToClear = FileType;
        do
        {
            *fileTypeToClear = emptyFileType;
            i++;
            fileTypeToClear++;
        } while (i < CARD_DIRECTORY_ENTRIES);
    }

    sprintf(key, CardDirFmtStr, cnct, slot);
    d = SLOT_DIR;
    if (firstfile(key, d) == d)
    {
        fileType = CardDirectoryFileType;
        do
        {
            size = size + d->size;
            if (Utils_CompareStrings(
                    d->name,
                    CARD_FNAME + CARD_FILENAME_COMPARE_OFFSET,
                    CARD_FILENAME_COMPARE_LENGTH) != 0)
            {
                *fileType = THPS_SAVE_FILE;
            }
            else
            {
                int comparison = Utils_CompareStrings(
                    d->name,
                    CARD_VNAME + CARD_FILENAME_COMPARE_OFFSET,
                    CARD_FILENAME_COMPARE_LENGTH);
                if (comparison != 0)
                {
                    comparison = THPS_VIDEO_FILE;
                    *fileType = static_cast<EFileType>(comparison);
                }
            }
            fileType = fileType + 1;
            d = d + 1;
        } while (nextfile(d) == d);
    }
    size /= CARD_BLOCK_SIZE;
    return CARD_DIRECTORY_ENTRIES - size;
}

/**
 * Advance the asynchronous memory-card state machine.
 *
 * @param cnct Memory-card connector number.
 * @param slot Memory-card slot number.
 * @return Current public card status.
 */
int Card_CheckStatus(int cnct, int slot)
{
    int ret;
    long chan;

    chan = cnct * CARD_CHANNEL_STRIDE + slot;
    switch (CardStatusMode)
    {
    case CARD_STATUS_BEGIN_INFO:
        _card_info(chan);
        CardStatusMode = CARD_STATUS_WAIT_INFO;
        EVENT_TIME_OUT = 0;
        CardStatus = 0;
        break;
    case CARD_STATUS_WAIT_INFO:
    {
        int event = _card_event();
        if (event != 0)
        {
            switch (event)
            {
            case 1:
                CardStatusTemp = event;
                if (CardLoadFlag == event)
                {
                    CardStatusMode = CARD_STATUS_FINISH;
                }
                else
                {
                    CardStatusMode = CARD_STATUS_BEGIN_LOAD;
                }
                return CardStatus;
            case 3:
                CardStatusTemp = -1;
                CardStatusMode = CARD_STATUS_FINISH;
                CardLoadFlag = 0;
                break;
            case 4:
                GHasCareerLoaded = 0;
                CardStatusTemp = 2;
                _clear_event_x();
                _card_clear(chan);
                _card_event_x();
                CardStatusMode = CARD_STATUS_BEGIN_LOAD;
                CardLoadFlag = 0;
                goto done;
            case 2:
            default:
                CardStatusTemp = -3;
                CardStatusMode = CARD_STATUS_FINISH;
                CardLoadFlag = 0;
                break;
            }
        }
        break;
    }
    case CARD_STATUS_BEGIN_LOAD:
        _clear_event();
        _card_load(chan);
        CardStatusMode = CARD_STATUS_WAIT_LOAD;
        EVENT_TIME_OUT = 0;
        break;
    case CARD_STATUS_WAIT_LOAD:
    {
        int event = _card_event();
        if (event != 0)
        {
            CardStatusMode = CARD_STATUS_FINISH;
            switch (event)
            {
            case 1:
                CardLoadFlag = 1;
                goto done;
            case 3:
                ret = -1;
                break;
            case 4:
                ret = -2;
                break;
            case 2:
            default:
                ret = -3;
                break;
            }
            CardStatusTemp = ret;
            CardLoadFlag = 0;
        }
        break;
    }
    case CARD_STATUS_FINISH:
        CardStatusMode = CARD_STATUS_BEGIN_INFO;
        CardStatus = CardStatusTemp;
    }
done:
    return CardStatus;
}

/**
 * Formats a memory card and translates the card event into a status code.
 *
 * @param cnct Memory-card connector number.
 * @param slot Memory-card slot number.
 * @return One on success, or a negative card error code.
 */
int Card_FormatCard(int cnct, int slot)
{
    char fs[6];
    sprintf(fs, CardFormatStr, cnct, slot);
    _clear_event_x();
    int fret = format(fs);
    int ret = _card_event_x();
    if (ret == 1)
        goto check_fmt;
    if (ret == 3)
    {
        ret = -1;
    }
    else
    {
        ret = -3;
    }
check_fmt:
    if (fret == 0 && ret == 1)
    {
        ret = -3;
    }
    return ret;
}

/**
 * Scan career saves and advance the timestamp beyond the newest entry.
 */
void Card_GetNextTimeStamp(void)
{
    int i;
    unsigned long *pTmp;
    EFileType *pFileType;
    DIRENTRY *pEntry;
    SCareer TmpCareer;

    TimeStamp = 0;
    if (SyncToGoodState() == 0)
    {
        i = 0;
        pEntry = SLOT_DIR;
        pFileType = FileType;
        do
        {
            if (*pFileType == THPS_SAVE_FILE)
            {
                pTmp = static_cast<unsigned long *>(Mem_New(
                    CARD_COMPLETE_FILE_SIZE, 1, 1));
                Card_SetSelectedFile(pEntry->name);
                if (Card_Load(THPS_SAVE_FILE, pTmp, 0) == 0)
                {
                    TmpCareer = *reinterpret_cast<SCareer *>(
                        Card_FilePayload(pTmp));
                    {
                        unsigned short ts = TmpCareer.TimeStamp;
                        if (TimeStamp <= ts)
                        {
                            TimeStamp = ts + 1;
                        }
                    }
                }
                Mem_Delete(pTmp);
            }
            pEntry++;
            i++;
            pFileType++;
        } while (i < CARD_DIRECTORY_ENTRIES);
    }
}
