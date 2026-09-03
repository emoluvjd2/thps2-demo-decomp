// RFRONT.cpp -- Reconstructed translation unit
// Original: RFRONT.CPP
// Functions: 19 recovered functions
// Address range: 0x8002131C - 0x80023B14
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#include "FRONT.h"
#define RFRONT_OWNER_DECLARATIONS
#include "RFRONT.h"
#undef RFRONT_OWNER_DECLARATIONS
#include "MAIN.h"
#include "VBLANKUPDATEBAR.h"
#include "SOFTRESETSTATE.h"

/** States returned to the main front-end state machine. */
enum EPausedStatus
{
    STATUS_NORMAL = 0,
    STATUS_PAUSEDMENU = 1,
    STATUS_SKIPTORESTART = 2,
    STATUS_REALLYQUIT = 3,
    STATUS_PADUNPLUGGED = 4,
    STATUS_SAVEGAME = 5,
    STATUS_QUITSAVE = 6,
    STATUS_MEDALSAVE = 7,
    STATUS_ENDLEVELFADESCREEN = 8,
    STATUS_DOSAVEFADESCREEN = 9,
    STATUS_NOSAVEFADESCREEN = 10,
    STATUS_GOTTAPE = 11,
    STATUS_TAPESAVE = 12,
    STATUS_STATS = 13,
    STATUS_ENDINGSCREEN = 14,
    STATUS_VIDEOSCREEN = 15,
    STATUS_VIDEOSCREEN2 = 16,
    STATUS_REALLYMAINMENU = 17,
    STATUS_MEMCARDMENUS = 18,
    STATUS_ENDHEAT = 19,
    STATUS_ENDHEATREPLAY = 20,
    STATUS_ENDHEATJUDGES = 21,
    STATUS_ENDHEATLADDER = 22,
    STATUS_ENDHEATWINLOSE = 23,
    STATUS_ENDHEATMENU = 24,
    STATUS_ENDHEATVIDEOMENU = 25,
    STATUS_BEGINHEAT = 26,
    STATUS_MPC_BEGIN = 27,
    STATUS_MPC_BEGINHEAT = 28,
    STATUS_MPC_JUDGES = 29,
    STATUS_MPC_LADDER = 30,
    STATUS_MPC_LADDERFINAL = 31,
    STATUS_MPC_SWITCH = 32,
    STATUS_MPC_ENDHEAT = 33,
    STATUS_MPC_WINNER = 34,
    STATUS_ENDGAME = 35,
    STATUS_ENDGAMEALLGOLD = 36,
    STATUS_ENDGAMEPLAYERMOVIE = 37,
    STATUS_ENDGAMEBAILMOVIE = 38,
    STATUS_ENDGAMESAVE = 39,
    STATUS_ENDGAMECREDITS = 40,
    STATUS_STARTHORSE = 41,
    STATUS_CHANGEHORSEWORD = 42,
    STATUS_LOADSUCCESS = 43,
    STATUS_LOADFAILED = 44,
    STATUS_HIGHSCORE = 45,
    STATUS_SETSOUNDLEVELS = 46,
    STATUS_DUMMY = 47,
};

/** RFRONT's save/load state machine. */
enum EMemCardState
{
    SVSTATUS_CHECKINGCARDPAUSE = 0,
    SVSTATUS_CHECKINGCARD = 1,
    SVSTATUS_ENTERNAME = 2,
    SVSTATUS_OKTOOVERWRITE = 3,
    SVSTATUS_SAVING = 4,
    SVSTATUS_SAVING2 = 5,
    SVSTATUS_SAVEFAILED = 6,
    SVSTATUS_SAVECOMPLETED = 7,
    SVSTATUS_CARDFULL = 8,
    SVSTATUS_INSERTCARD = 9,
    SVSTATUS_UNFORMATTEDCARD = 10,
    SVSTATUS_FORMATTING = 11,
    SVSTATUS_FORMATFAILED = 12,
    SVSTATUS_FORMATSUCCEEDED = 13,
    SVSTATUS_DELETEQUERY = 14,
    LVSTATUS_CHECKINGCARDPAUSE = 15,
    LVSTATUS_CHECKINGCARD = 16,
    LVSTATUS_INSERTCARD = 17,
    LVSTATUS_UNFORMATTEDCARD = 18,
    LVSTATUS_NOFILES = 19,
    LVSTATUS_SELECTFILE = 20,
    LVSTATUS_LOADING = 21,
    LVSTATUS_LOADFAILED = 22,
    LVSTATUS_LOADCOMPLETED = 23,
    LVSTATUS_FULLCARDCHECK = 24,
    LVSTATUS_FULLCARDWARNING = 25,
    LVSTATUS_HIRESREFRESH1 = 26,
    LVSTATUS_HIRESREFRESH2 = 27,
    LVSTATUS_HIRESREFRESH3 = 28,
    LVSTATUS_BUILDMENU = 29,
    LCSTATUS_CHECKINGCARDPAUSE = 30,
    LCSTATUS_CHECKINGCARD = 31,
    LCSTATUS_INSERTCARD = 32,
    LCSTATUS_NOFILES = 33,
    LCSTATUS_SELECTFILE = 34,
    LCSTATUS_LOADING = 35,
    LCSTATUS_LOADFAILED = 36,
    LCSTATUS_LOADCOMPLETED = 37,
};

struct VerticalMenu;

extern "C" {
#define SAVEACTION_WITHIN_C_LINKAGE
#include "SAVEACTION.h"
#define CARDSTATUS_WITHIN_C_LINKAGE
#include "CARDSTATUS.h"
}

#include "CARD.h"
#include "HIRES.h"
#include "PAD.h"

struct Sprite;

/** GP-sized leading view of the save-name buffer used by default naming. */
struct SaveGameNameLayout
{
    char b0, b1, b2, b3, b4, b5, b6, b7;
};

/** Slice spanning RFRONT's active and stored career names. */
struct RFrontNameStorageView
{
    char saveGameName[11];
    char ownerState[0x95];
    char storedCareerName[11];
};

/** Slice covering the load/save actions and stored career name. */
struct RFrontStoredCareerStateView
{
    ELoadAction loadAction;
    ESaveAction saveAction;
    char storedCareerName[11];
};

inline unsigned char *RFront_NameBytes(char *first)
{
    return reinterpret_cast<unsigned char *>(first);
}

inline RFrontNameStorageView *RFront_NameStorage(char *first)
{
    return reinterpret_cast<RFrontNameStorageView *>(first);
}

/** Sprite dispatch prefix through the deleting-destructor slot. */
struct RFrontSpriteVTable
{
    void *reserved[2];
    void (*destroy)(Sprite *sprite, int flags);
};

#include "globals.h"
#include "CMENU.h"
#include "SPRITE.h"

/** Return caller-owned storage for an ordinary placement construction. */
inline void *operator new(unsigned int, void *storage)
{
    return storage;
}

/** One cached text rectangle and the sprite used to restore its background. */
struct RFrontCachedBackground
{
    Sprite *pSprite;
    const char *pCachedText;
    short x;
    short y;
    short w;
    short h;
    int captureStatus;
};

typedef char RFrontSprite_size_must_be_24[
    (sizeof(Sprite) == 24) ? 1 : -1];
typedef char RFrontCachedBackground_size_must_be_20[
    (sizeof(RFrontCachedBackground) == 20) ? 1 : -1];

#include "VERTICALMENUITEM.h"

/** VerticalMenu dispatch table prefix through its draw callback. */
struct RFrontVerticalMenuVTable
{
    void *reserved[3];
    void (*draw)(VerticalMenu *menu);
};

/** Target-proven 820-byte file-selection menu. */
struct VerticalMenu
{
    RFrontVerticalMenuVTable *vtable;
    int itemSpacing;
    int fontScale;
    int fontScaleBig;
    int numItems;
    int oldSelected;
    int selected;
    VerticalMenuItem items[12];
    int tick;
    Sprite *triangle1;
    Sprite *triangle2;
    Sprite *eraseTriangle1;
    Sprite *eraseTriangle2;
    int dontDeleteSprites;

    /** Draw the erase sprites covering the current menu state. */
    void erase();
    /** Return the selected item index. */
    int getSelected();
    /** Replace one menu item's displayed text. */
    void setString(int item, char *text);
    /** Move to the previous enabled item. */
    void up();
    /** Move to the next enabled item. */
    void down();
};

typedef char RFrontVerticalMenu_size_must_be_820[
    (sizeof(VerticalMenu) == 820) ? 1 : -1];

#include "LOCALTXT.h"

typedef char RFront_LocalText_size_must_be_652[
    (sizeof(LocalText) == 0x28c) ? 1 : -1];

#define VIDEO_HEAD_LAYOUT
#define VIDEO_LAYOUT_ONLY
#include "VIDEO.h"
#undef VIDEO_LAYOUT_ONLY
#undef VIDEO_HEAD_LAYOUT

/** Card block prefix before an on-disk replay header. */
struct RFrontVideoLoadBlock
{
    char reserved_000[0x200];
    SVideoHead header;
};

/**
 * RFRONT-owned view of the active replay descriptor.
 *
 * Only the block pointer, frame count, and highlight table are needed while
 * loading a replay. Reserved ranges retain the complete array stride.
 */
struct SVideoInfo
{
    int VideoMode;
    unsigned long *pBlockStart;
    char reserved_008[0x14];
    unsigned long NumFrames;
    char reserved_020[0x30];
    struct SHighlight Highlights[5];
    char reserved_078[0x48];
};

#include "SCHARACTERPROGRESS.h"

#define CAREER_HIGH_SCORE_LAYOUT
#define CAREER_HIGH_SCORE_LAYOUT_ONLY
#include "CAREER.h"
#undef CAREER_HIGH_SCORE_LAYOUT_ONLY
#undef CAREER_HIGH_SCORE_LAYOUT

#include "SOPTIONS.h"

/** Complete career payload copied from a loaded save-game block. */
struct SCareer
{
    int Version;
    int MaxTapes;
    struct SCharacterProgress CharacterProgress[14];
    struct SHighScore HighScore[16][8];
    char LastSkater[2];
    char LastDeck[2];
    char LastWheel[2];
    char LastTrucks[2];
    char LastLevel;
    char reserved_7d1;
    unsigned short TimeStamp;
    struct SOptions Options;
};

/** Complete temporary card buffer used while loading a career save. */
struct RFrontCareerLoadBlock
{
    char reserved_000[0x20];
    char sjisName[0x260];
    SCareer career;
    char reserved_bf4[0x0c];
};

typedef char SCardHeadRFront_size_must_be_512[
    (sizeof(SCardHead) == 0x200) ? 1 : -1];
typedef char SVideoInfoRFront_size_must_be_192[
    (sizeof(SVideoInfo) == 0xc0) ? 1 : -1];
typedef char SCareerRFront_size_must_be_2420[
    (sizeof(SCareer) == 0x974) ? 1 : -1];
typedef char RFrontCareerLoadBlock_size_must_be_3072[
    (sizeof(RFrontCareerLoadBlock) == 0xc00) ? 1 : -1];

/** Locate typed records inside raw memory-card transfer buffers. */
inline SVideoHead *RFront_GetVideoHeader(unsigned long *block)
{
    return &reinterpret_cast<RFrontVideoLoadBlock *>(block)->header;
}

inline RFrontCareerLoadBlock *RFront_GetCareerLoadBlock(unsigned long *block)
{
    return reinterpret_cast<RFrontCareerLoadBlock *>(block);
}

/** Readable, typed bindings for collaborator ABIs not yet owned by one API. */
#define VIDEOINFO_DECLARE_GVINFO1
#include "VIDEOINFO.h"

/** Addressing views for RFRONT state owned by this or collaborating TUs. */
#define RFRONT_DECLARE_SAVE_NAME_ADDRESS_VIEWS
#include "RFRONT.h"
#define FRONTTIMERS_LEFT_RIGHT_TIMER_ABSOLUTE_DECLARATION
#include "FRONTTIMERS.h"
#include "YESNOMENU.h"

#include "MESS.h"
int InCompetition(void);
int Competition_IsOver(void);
#define FRONT_MEMCARD_API
#include "FRONT.h"
#undef FRONT_MEMCARD_API
/**
 * Copy a save-name buffer through the target's void call-site declaration.
 * The SDK header's returned pointer keeps `$v0` live and changes the following
 * delay-slot allocation, while this declaration reproduces the original call.
 */
#define RFRONT_DECLARE_VOID_STRCPY_ABI
#include "RFRONT.h"
void ResetOT(void);
extern "C" VerticalMenu *__12VerticalMenuiiiPPciPiT6i(
    VerticalMenu *menu, int top, int bottom, int spacing, char **items,
    int count, int *first, int *last, int flags);
#include "MEM.h"

#include "SFX.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <STDIO.H>

/** Repeated display and audio constants used by the memory-card front end. */
enum RFrontDisplayConstants
{
    RFRONT_FULL_SCALE = 256,
    RFRONT_HIGH_RES_SCALE = 200,
    RFRONT_CARD_FULL_SCALE = 192,
    RFRONT_STANDARD_WIDTH = 512,
    RFRONT_NAME_SPACING = 30,
    RFRONT_NAME_HALF_WIDTH = 150,
    RFRONT_SFX_VOLUME = 8192,
};

extern "C" {
/** Whether a career has been loaded into the front end. */
int GHasCareerLoaded = 0;

/** Whether the loaded career has already been saved. */
int GLoadedCareerIsSaved = 0;

/** First eight characters of the ten-character, blank-padded save name. */
SaveGameNameLayout SaveGameName
    __attribute__((section(".sdata"))) =
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
}

/** Final two characters and terminator of the RFRONT-owned save name. */
static char SaveGameNameTail[3]
    __attribute__((section(".sdata"))) = {' ', ' ', '\0'};

extern "C" {

/** Cursor position and pulse amount used by the name-entry screen. */
int CursorPos = 0;
int SizeUp = 0;

/** Most recently used career and replay names. */
char LastCareerName[11] __attribute__((section(".sdata"))) = "          ";
char LastVideoName[11] __attribute__((section(".sdata"))) = "          ";

/** Active memory-card file menu and its visible-window state. */
VerticalMenu *pVertMenu = 0;
int NumVertMenuItems = 0;
int VertMenuTopIndex = 0;
int VertMenuTopChanged = 0;

/** Index of the skater selected by the front-end. */
int SelectedSkater = 0;

/** Cached-background and memory-card display state. */
int FreeSprite = 0;
EMemCardState MCState = SVSTATUS_CHECKINGCARDPAUSE;
EMemCardState OldMCState = SVSTATUS_CHECKINGCARDPAUSE;
int CheckingCardPause = 0;

/** Horizontal resolution used by the memory-card screens. */
static int MCResolution = RFRONT_STANDARD_WIDTH;
}

/**
 * Memory-card retry state retained privately by RFRONT.
 *
 * MAIN.SYM classifies both definitions as rfront.obj statics and records the
 * compiler's target-local suffixes in the linked image.
 */
static int Delay asm("Delay.311");
static int Retries asm("Retries.312");

/**
 * Owner-local memory-card prompt state.
 *
 * GCC emits these local commons in declaration order.  Keep the declarations
 * in MAIN.SYM address order so their four-byte slots follow the private
 * `Delay.311` and `Retries.312` state exactly.
 */
static Sprite *Tri1;
static Sprite *Tri2;
static int SaveNeedBlocks;
static int SaveFreeBlocks;
static int GReturnToState;

extern "C" {
/** Replay restart state, cleared until a replay header is loaded. */
SVideoRestart GVideoRestart
    __attribute__((section(".data"))) = {0, 0, 0, 0, 0};

/** ASCII memory-card names and the menu views built over them. */
char ASCIIName[15][11] __attribute__((section(".data"))) = {{0}};
char *VertMenuItem[15] __attribute__((section(".data"))) = {0};
int VertMenuItemIndex[15] __attribute__((section(".data"))) = {0};

/** Ten cached background captures used by the memory-card front end. */
RFrontCachedBackground BGCache[10]
    __attribute__((section(".data"))) = {{0}};

/** Career name copied to and from the active save-name buffer. */
char StoredCareerName[11] __attribute__((section(".sbss")));
}

/** RFRONT-owned immutable memory-card status strings in target order. */
static char RFrontNoSpaceText[]
    __attribute__((section(".rdata"), aligned(4))) =
        "no space do you want to delete blah blah";
static char RFrontLoadingVideoText[]
    __attribute__((section(".rdata"), aligned(4))) = "loading video file";
static char RFrontLoadingSavedGameText[]
    __attribute__((section(".rdata"), aligned(4))) = "loading saved game";
static char RFrontSavingReplayFormat[]
    __attribute__((section(".rdata"), aligned(4))) = "Saving Replay %s";

// === Function implementations (address order) ===

/**
 * Select the muted message color used for warning text.
 */
static void RGBRed(void) {
    Mess_SetRGB(0x60, 0x60, 0x60, 0);
}


/**
 * Select the muted message color used for ordinary status text.
 */
static void RGBGreen(void) {
    Mess_SetRGB(0x60, 0x60, 0x60, 0);
}


/**
 * Read the two pads and apply the front-end's vertical auto-repeat timing.
 *
 * @return -1 for up, 1 for down, or 0 when no repeated step is due.
 */
int GetUpDown(void)
{
    int RetVal = 0;

    if (Pad[0].Up.Pressed != '\0' || Pad[1].Up.Pressed != '\0' ||
        Pad[0].Down.Pressed != '\0' || Pad[1].Down.Pressed != '\0')
    {
        if (LeftRightAutoRepeatTimer == 0 ||
            (LeftRightAutoRepeatTimer > 15 &&
             (LeftRightAutoRepeatTimer & 3U) == 0))
        {
            if ((Pad[0].Down.Pressed != '\0') || (Pad[1].Down.Pressed != '\0'))
            {
                RetVal = 1;
            }
            if ((Pad[0].Up.Pressed != '\0') || (Pad[1].Up.Pressed != '\0'))
            {
                RetVal = -1;
            }
        }
        LeftRightAutoRepeatTimer++;
    }
    else
    {
        LeftRightAutoRepeatTimer = 0;
    }

    return RetVal;
}


/**
 * Cache the background behind a centered text string before drawing it.
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param pText  Text to operate on.
 */
void Front_DirtyRectText(int x, int y, const char * pText) {
    int w;
    int h;
    const char **pCachedText1;
    int left;
    int i;
    RFrontCachedBackground *entry;
    Sprite *pSpriteObj;

    i = 0;
    if (0 < FreeSprite) {
        int textScale = RFRONT_FULL_SCALE;
        pCachedText1 = &BGCache[0].pCachedText;
        do {
            i = i + 1;
            if (*pCachedText1 == pText) {
                Mess_DrawText(
                    x, y, pText, 0, 0, textScale, 0);
                return;
            }
            pCachedText1 = pCachedText1 + 5;
        } while (i < FreeSprite);
    }

    RFrontCachedBackground *base = BGCache;
    BGCache[FreeSprite].pCachedText = pText;
    w = Mess_TextWidth(pText);
    h = Mess_TextHeight(pText);
    left = x - (w >> 1);
    entry = &base[FreeSprite];
    entry->x = static_cast<short>(left);
    entry->y = static_cast<short>(y) - static_cast<short>(h);
    w += 4;
    h += 4;
    pSpriteObj = static_cast<Sprite *>(
        ::CClass::operator new(sizeof(Sprite)));
    if (pSpriteObj != 0) {
        pSpriteObj = ::new (pSpriteObj) Sprite(
            left, base[FreeSprite].y, w, h, 1, 32, 128);
    }
    entry->pSprite = pSpriteObj;
    BGCache[FreeSprite].captureStatus = 2;
    FreeSprite = FreeSprite + 1;
}


/**
 * Restore every captured menu-text background before the next redraw.
 */
void Front_RestoreBackground(void) {
    int i = 0;
    int limit = FreeSprite;
    if (limit <= 0) goto done;
    {
        int neg1 = -1;
        Sprite **base = &BGCache[0].pSprite;
        short *y = &BGCache[0].y;
        short *x = &BGCache[0].x;
        int offset = 0;
        int *status = &BGCache[0].captureStatus;
        do {
            // Keep the index first: GCC otherwise commutes this addu.
            int val = *reinterpret_cast<int *>(
                offset + reinterpret_cast<int>(status));
            register int captured asm("$3") = 2;
            if (val == captured) {
                (*base)->draw(*x, *y, 0, neg1, neg1, neg1);
                limit = FreeSprite;
            }
            base = reinterpret_cast<Sprite **>(
                reinterpret_cast<char *>(base) + sizeof(RFrontCachedBackground));
            y = reinterpret_cast<short *>(
                reinterpret_cast<char *>(y) + sizeof(RFrontCachedBackground));
            x = reinterpret_cast<short *>(
                reinterpret_cast<char *>(x) + sizeof(RFrontCachedBackground));
            i++;
            offset += sizeof(RFrontCachedBackground);
        } while (i < limit);
    }
done:
    ;
}


/**
 * Destroy all cached background sprites and reset the cache.
 */
void Front_FreeBackground(void) {
    int i = 0;
    if (FreeSprite <= 0) goto done;
    {
        RFrontCachedBackground *tmp = BGCache;
        int *clear = &tmp->captureStatus;
        Sprite **base = &tmp->pSprite;
        do {
            Sprite *sprite = *base;
            if (sprite != 0) {
                reinterpret_cast<RFrontSpriteVTable *>(
                    sprite->CClass.vtable)->destroy(sprite, 3);
            }
            *clear = 0;
            // Separate interior-field cursors reproduce the target's s1/s0
            // walk; whole-record cursors changed 27 of 36 instructions.
            clear = reinterpret_cast<int *>(
                reinterpret_cast<char *>(clear) + sizeof(RFrontCachedBackground));
            base = reinterpret_cast<Sprite **>(
                reinterpret_cast<char *>(base) + sizeof(RFrontCachedBackground));
            {
                int limit = FreeSprite;
                i++;
                if (i >= limit) break;
            }
        } while (1);
    }
done:
    FreeSprite = 0;
}


/**
 * Initialize the memory-card UI state machine.
 *
 * @param Resolution Horizontal display resolution used for menu centering.
 * @param StartState Initial memory-card state.
 * @param ReturnToState Front-end state restored after the operation finishes.
 */
void Front_InitMemCardMenus(int Resolution, EMemCardState StartState, EPausedStatus ReturnToState) {
    GReturnToState = ReturnToState;
    FreeSprite = 0;
    MCResolution = Resolution;
    MCState = StartState;
    CheckingCardPause = 0;
}


/**
 * Select what kind of data the memory-card loader should read.
 *
 * @param LoadAction Requested replay, replay-header, or career load.
 */
void Front_SetLoadAction(ELoadAction LoadAction) {
    GLoadAction = LoadAction;
}


/**
 * Select whether the memory-card writer should save a replay or career.
 *
 * @param SaveAction Requested save operation.
 */
void Front_SetSaveAction(ESaveAction SaveAction) {
    GSaveAction = SaveAction;
}


/**
 * Convert the ten-character memory-card title from Shift-JIS to ASCII.
 *
 * @param pSJIS  SJIS to operate on.
 * @param pASCII  ASCII to operate on.
 */
void Front_ExtractASCIINameFromSJIS(char * pSJIS, char * pASCII) {
    int i = 0;
    int sjis82 = 0x82;
    int sjis81 = 0x81;
    int space = 0x20;
    loop:
    {
        int c = *reinterpret_cast<unsigned char *>(pSJIS);
        if (c == 0) {
            *pASCII = 0;
            return;
        }
        if (c == sjis82) {
            int c2 = reinterpret_cast<unsigned char *>(pSJIS)[1];
            pSJIS += 2;
            *pASCII++ = c2 - 0x1f;
        } else if (c == sjis81) {
            *pASCII++ = space;
            pSJIS += 2;
        }
        i++;
        if (i < 10) goto loop;
    }
}


/**
 * Convert the edited ASCII name buffer into the Shift-JIS card-title format.
 *
 * @param SJISName Destination card-title buffer.
 */
void Front_SetSJISName(char * SJISName) {
    register int i asm("$5") = 9;
    int buf = reinterpret_cast<int>(&RFrontSaveNameViews::SaveGameName);
    char *p;
    unsigned char c;

    do {
        p = reinterpret_cast<char *>(i + buf);
        c = static_cast<unsigned char>(*p);
        if (c != 0x20) {
            if (c != 0) goto fill_phase;
        }
        i--;
        *p = 0;
    } while (i > 0);

fill_phase:
    if (i >= 0) {
        register int buf2 asm("$7") =
            reinterpret_cast<int>(&RFrontSaveNameViews::SaveGameName);
        int sp = 0x20;
        p = reinterpret_cast<char *>(i + buf2);
        do {
            c = static_cast<unsigned char>(*p);
            if (c == 0) {
                *p = static_cast<unsigned char>(sp);
            }
            i--;
            p = reinterpret_cast<char *>(i + buf2);
        } while (i >= 0);
    }

    {
        register int j asm("$5") = 0;
        register int bp asm("$11") =
            reinterpret_cast<int>(&RFrontSaveNameViews::SaveGameName);
        register int K82 asm("$10") = 0x82;
        register int K20 asm("$9") = 0x20;
        register int K81 asm("$8") = 0x81;
        int K40 = 0x40;
        register unsigned char *q asm("$6");
        // These two boundaries retain the target's constant-load order and
        // keep the initial j + buffer address addition explicit. A single
        // combined boundary commutes five instructions; none changes memory.
        asm("" : "+r"(j), "+r"(bp), "+r"(K82),
                 "+r"(K20), "+r"(K81));
        q = reinterpret_cast<unsigned char *>(j + bp);
loop3:
        {
            unsigned int v0 = *q;
            unsigned int v1 = v0 & 0xff;
            if (v1 == 0) {
                *SJISName = 0;
                return;
            }
            v0 = v0 - 'A';
            if (static_cast<unsigned int>(v0 & 0xff) < 26) {
                *SJISName = static_cast<unsigned char>(K82);
                v0 = *q;
                SJISName++;
                v0 = v0 + 0x1f;
                *SJISName = static_cast<unsigned char>(v0);
                goto bump;
            }
            if (v1 != static_cast<unsigned int>(K20)) goto no_bump;
            *SJISName = static_cast<unsigned char>(K81);
            SJISName++;
            *SJISName = static_cast<unsigned char>(K40);
bump:
            SJISName++;
no_bump:
            ;
        }
        j++;
        if (j < 10) {
            q = reinterpret_cast<unsigned char *>(j + bp);
            goto loop3;
        }
    }

    return;
}


/**
 * Initialize name entry and restore the last name for this save category.
 */
void InitEnterName(void) {
    CursorPos = 0;
    SizeUp = 0;
    Pad[0].Up.Triggered = 0;
    Pad[0].Down.Triggered = 0;
    Pad[1].Up.Triggered = 0;
    Pad[1].Down.Triggered = 0;
    if (GSaveAction == SAVE_VIDEO) {
        strcpy(&RFrontSaveNameViews::SaveGameName,
               &RFrontSaveNameViews::LastVideoName);
    } else {
        strcpy(&RFrontSaveNameViews::SaveGameName,
               &RFrontSaveNameViews::LastCareerName);
    }
}


/**
 * Draw the low-resolution prompt for the active memory-card state.
 */
void Front_MemCardDisplay(void) {
    int i;
    int XMid = MCResolution >> 1;

    Mess_SetScale(RFRONT_FULL_SCALE);
    Mess_SetTextJustify(0);

    switch (MCState)
    {
    case SVSTATUS_CHECKINGCARDPAUSE:
    case SVSTATUS_CHECKINGCARD:
    case LVSTATUS_CHECKINGCARDPAUSE:
    case LVSTATUS_CHECKINGCARD:
        RGBGreen();
        Mess_DrawText(
            XMid, 90, GameText.pCheckingCard, 0, 0, RFRONT_FULL_SCALE, 0);
        RGBRed();
        Mess_DrawText(
            XMid, 140, GameText.pDoNotRemove1, 0, 0, RFRONT_FULL_SCALE, 0);
        Mess_DrawText(
            XMid, 166, GameText.pDoNotRemove2, 0, 0, RFRONT_FULL_SCALE, 0);
        break;

    case SVSTATUS_INSERTCARD:
    case LVSTATUS_INSERTCARD:
        RGBRed();
        Mess_DrawText(
            XMid, 90, GameText.pPleaseInsert1, 0, 0, RFRONT_FULL_SCALE, 0);
        Mess_DrawText(
            XMid, 116, GameText.pPleaseInsert2, 0, 0, RFRONT_FULL_SCALE, 0);
        Mess_DrawText(
            XMid, 142, GameText.pPleaseInsert3, 0, 0, RFRONT_FULL_SCALE, 0);
        Mess_DrawText(
            XMid, 168, GameText.pPleaseInsert4, 0, 0, RFRONT_FULL_SCALE, 0);
        break;

    case SVSTATUS_UNFORMATTEDCARD:
        Front_PrintGoBackSelect();
        RGBRed();
        Mess_DrawText(
            XMid, 35, GameText.pUnformatted1, 0, 0, RFRONT_FULL_SCALE, 0);
        Mess_DrawText(
            XMid, 61, GameText.pUnformatted2, 0, 0, RFRONT_FULL_SCALE, 0);
        Mess_DrawText(
            XMid, 87, GameText.pUnformatted3, 0, 0, RFRONT_FULL_SCALE, 0);
        Mess_DrawText(
            XMid, 113, GameText.pUnformatted4, 0, 0, RFRONT_FULL_SCALE, 0);
        {
            CMenu *menu = pYesNoMenu;
            menu->mY = 148;
            menu->Display();
        }
        break;

    case SVSTATUS_FORMATTING:
        RGBGreen();
        if ((TTime / 10) & 1U)
        {
            Mess_DrawText(
                XMid, 110, GameText.pFormatting,
                0, 0, RFRONT_FULL_SCALE, 0);
        }
        RGBRed();
        Mess_DrawText(
            XMid, 140, GameText.pDoNotRemove1, 0, 0, RFRONT_FULL_SCALE, 0);
        Mess_DrawText(
            XMid, 166, GameText.pDoNotRemove2, 0, 0, RFRONT_FULL_SCALE, 0);
        break;

    case SVSTATUS_FORMATSUCCEEDED:
        RGBGreen();
        Mess_DrawText(
            XMid, 110, GameText.pFormatComplete,
            0, 0, RFRONT_FULL_SCALE, 0);
        break;

    case SVSTATUS_FORMATFAILED:
        RGBRed();
        Mess_DrawText(
            XMid, 110, GameText.pFormatFailed,
            0, 0, RFRONT_FULL_SCALE, 0);
        break;

    case SVSTATUS_CARDFULL:
    {
        char buf[24];

        RGBRed();
        Mess_SetScale(RFRONT_CARD_FULL_SCALE);
        Mess_DrawText(
            XMid, 67, GameText.pCardFull1, 0, 0, RFRONT_FULL_SCALE, 0);
        Mess_DrawText(
            XMid, 85, GameText.pCardFull2, 0, 0, RFRONT_FULL_SCALE, 0);
        Mess_DrawText(
            XMid, 103, GameText.pCardFull3, 0, 0, RFRONT_FULL_SCALE, 0);
        sprintf(
            buf, GameText.pCardFull4, SaveNeedBlocks,
            SaveNeedBlocks == 1 ? "" : "s");
        Mess_DrawText(
            XMid, 121, buf, 0, 0, RFRONT_FULL_SCALE, 0);
        sprintf(
            buf, GameText.pCardFull5,
            SaveNeedBlocks - SaveFreeBlocks,
            SaveNeedBlocks - SaveFreeBlocks == 1 ? "" : "s");
        Mess_DrawText(
            XMid, 139, buf, 0, 0, RFRONT_FULL_SCALE, 0);
        Mess_DrawText(
            XMid, 157, GameText.pCardFull6, 0, 0, RFRONT_FULL_SCALE, 0);
        Front_PrintCentredPrompt(GameText.pContinue);
        break;
    }

    case SVSTATUS_OKTOOVERWRITE:
        RGBGreen();
        Mess_DrawText(
            XMid, 50, GameText.pAFileByThat, 0, 0, RFRONT_FULL_SCALE, 0);
        Mess_DrawText(
            XMid, 76, GameText.pNameAlreadyExists,
            0, 0, RFRONT_FULL_SCALE, 0);
        Mess_DrawText(
            XMid, 102, GameText.pOKToOverwrite1,
            0, 0, RFRONT_FULL_SCALE, 0);
        Front_PrintGoBackSelect();
        {
            CMenu *menu = pYesNoMenu;
            menu->mY = 0x91;
            menu->Display();
        }
        break;

    case SVSTATUS_SAVING:
        RGBGreen();
        Mess_DrawText(
            XMid, 110, GameText.pSaving, 0, 0, RFRONT_FULL_SCALE, 0);
        RGBRed();
        Mess_DrawText(
            XMid, 140, GameText.pDoNotRemove1, 0, 0, RFRONT_FULL_SCALE, 0);
        Mess_DrawText(
            XMid, 166, GameText.pDoNotRemove2, 0, 0, RFRONT_FULL_SCALE, 0);
        break;

    case SVSTATUS_SAVECOMPLETED:
        RGBGreen();
        Mess_DrawText(
            XMid, 110, GameText.pSaveComplete, 0, 0, RFRONT_FULL_SCALE, 0);
        Front_PrintCentredPrompt(GameText.pContinue);
        break;

    case SVSTATUS_SAVEFAILED:
        RGBRed();
        Mess_DrawText(
            XMid, 110, GameText.pSaveFailed, 0, 0, RFRONT_FULL_SCALE, 0);
        Front_PrintCentredPrompt(GameText.pContinue);
        break;

    case SVSTATUS_ENTERNAME:
    {
        i = 0;
        RGBRed();
        Mess_SetTextJustify(0);
        Mess_SetScale(RFRONT_FULL_SCALE);
        Mess_DrawText(
            XMid, 40, GameText.pEnterName, 0, 0, RFRONT_FULL_SCALE, 0);

        do
        {
            if (SizeUp > 0)
            {
                SizeUp--;
            }

            Mess_SetScale(RFRONT_FULL_SCALE);
            if (i == CursorPos)
            {
                Mess_SetRGB(0xb6, 0x84, 0x24, 0);
            }
            else
            {
                Mess_SetRGB(0x7a, 0x7a, 0x7a, 0);
            }

            Mess_DrawText(
                XMid + (i * RFRONT_NAME_SPACING) - RFRONT_NAME_HALF_WIDTH, 100, "_", 0, 0, RFRONT_FULL_SCALE, 0);
            if (i == CursorPos)
            {
                Mess_SetScale((SizeUp * 6) + RFRONT_FULL_SCALE);
            }

            if ((&RFrontSaveNameViews::SaveGameName)[i] != 0)
            {
                char tmp[3] = "\0\0";
                tmp[0] = (&RFrontSaveNameViews::SaveGameName)[i];
                Mess_DrawText(
                    XMid + (i * RFRONT_NAME_SPACING) - RFRONT_NAME_HALF_WIDTH, 92, tmp, 0, 0, RFRONT_FULL_SCALE, 0);
            }

            i++;
        } while (i < 10);

        Front_PrintGoBackSelect();
        break;
    }

    case SVSTATUS_DELETEQUERY:
        Mess_DrawText(
            XMid, 100, RFrontNoSpaceText,
            0, 0, RFRONT_FULL_SCALE, 0);
        break;

    case LVSTATUS_NOFILES:
        RGBRed();
        if (GLoadAction == LOAD_GAME)
        {
            Mess_DrawText(
                XMid, 110, GameText.pNoCareerFiles,
                0, 0, RFRONT_FULL_SCALE, 0);
        }
        else
        {
            Mess_DrawText(
                XMid, 110, GameText.pNoVideoFiles,
                0, 0, RFRONT_FULL_SCALE, 0);
        }
        if (MCResolution == RFRONT_STANDARD_WIDTH)
        {
            Front_PrintCentredPrompt(GameText.pContinue);
        }
        break;

    case LVSTATUS_SELECTFILE:
        if (MCResolution == RFRONT_STANDARD_WIDTH)
        {
            Front_PrintGoBackSelect();
        }
        break;

    case LVSTATUS_LOADING:
        RGBGreen();
        Mess_DrawText(
            XMid, 110, RFrontLoadingVideoText, 0, 0, RFRONT_FULL_SCALE, 0);
        RGBRed();
        Mess_DrawText(
            XMid, 140, GameText.pDoNotRemove1, 0, 0, RFRONT_FULL_SCALE, 0);
        Mess_DrawText(
            XMid, 166, GameText.pDoNotRemove2, 0, 0, RFRONT_FULL_SCALE, 0);
        break;

    case LVSTATUS_LOADFAILED:
        RGBRed();
        Mess_DrawText(
            XMid, 110, GameText.pLoadFailed, 0, 0, RFRONT_FULL_SCALE, 0);
        if (MCResolution == RFRONT_STANDARD_WIDTH)
        {
            Front_PrintCentredPrompt(GameText.pContinue);
        }
        break;

    case LVSTATUS_LOADCOMPLETED:
        break;

    default:
        break;
    }
}

/** Mutable status text stored in RFRONT's target small-data tail. */
static char RFrontLoadingReplayText[]
    __attribute__((section(".sdata"), aligned(4))) = "loading replay";
static char RFrontVideoLoadedText[]
    __attribute__((section(".sdata"), aligned(4))) = "Video loaded";

/** Rolling suffixes used when a blank replay or career name is accepted. */
static int Video
    __attribute__((section(".sdata"), aligned(4))) = 0;
static int Career
    __attribute__((section(".sdata"), aligned(4))) = 0;

/** On-disc replay filename template; byte four is replaced by the slot letter. */
static char RFrontDemoFileName[]
    __attribute__((section(".sdata"), aligned(4))) = "demoX.rec";


/**
 * Draw the high-resolution memory-card state and its file-selection menu.
 */
void Front_MemCardHiResDisplay(void) {
    int i;
    int XMid;

    Mess_SetScale(RFRONT_HIGH_RES_SCALE);
    XMid = MCResolution >> 1;

    if ((MCState == LVSTATUS_LOADING) &&
        (static_cast<unsigned int>(GLoadAction - 1) < 2U))
    {
        char *load;

        for (i = 0; i < 2; i++)
        {
            HiRes_ClearOTag();
            ResetOT();
            if (pVertMenu != 0)
            {
                pVertMenu->erase();
            }
            HiRes_DrawOTag();
        }

        if (GLoadAction == LOAD_GAME)
        {
            load = RFrontLoadingSavedGameText;
        }
        else
        {
            load = RFrontLoadingReplayText;
        }

        DrawSync(0);
        for (i = 0; i < 4; i++)
        {
            HiRes_ClearOTag();
            ResetOT();
            RGBGreen();
            Front_DirtyRectText(XMid, 240, load);
            RGBRed();
            HiRes_DrawOTag();
        }
        OldMCState = LVSTATUS_HIRESREFRESH1;
        return;
    }

    if (OldMCState != LVSTATUS_HIRESREFRESH3)
    {
        Front_RestoreBackground();
    }

    if (OldMCState != MCState)
    {
        if (OldMCState == LVSTATUS_CHECKINGCARDPAUSE)
        {
            // The checking pause can transition directly to the live state.
        }
        else if (OldMCState == LVSTATUS_HIRESREFRESH1)
        {
            OldMCState = LVSTATUS_HIRESREFRESH2;
            return;
        }
        else if (OldMCState == LVSTATUS_HIRESREFRESH2)
        {
            OldMCState = LVSTATUS_HIRESREFRESH3;
            return;
        }
        else
        {
            if (OldMCState != LVSTATUS_HIRESREFRESH3)
            {
                OldMCState = LVSTATUS_HIRESREFRESH1;
                return;
            }
            Front_FreeBackground();
        }
        OldMCState = MCState;
    }

    Mess_SetScale(RFRONT_HIGH_RES_SCALE);
    Mess_SetTextJustify(0);

    switch (MCState)
    {
    case LVSTATUS_CHECKINGCARDPAUSE:
    case LVSTATUS_CHECKINGCARD:
        RGBGreen();
        Front_DirtyRectText(XMid, 180, GameText.pCheckingCard);
        RGBRed();
        Front_DirtyRectText(XMid, 280, GameText.pDoNotRemove1);
        Front_DirtyRectText(XMid, 332, GameText.pDoNotRemove2);
        break;

    case LVSTATUS_INSERTCARD:
        RGBRed();
        Front_DirtyRectText(XMid, 180, GameText.pPleaseInsert1);
        Front_DirtyRectText(XMid, 232, GameText.pPleaseInsert2);
        Front_DirtyRectText(XMid, 284, GameText.pPleaseInsert3);
        Front_DirtyRectText(XMid, 336, GameText.pPleaseInsert4);
        break;

    case LVSTATUS_UNFORMATTEDCARD:
        RGBRed();
        Front_DirtyRectText(XMid, 174, GameText.pHiResUnformatted1);
        Front_DirtyRectText(XMid, 226, GameText.pHiResUnformatted2);
        Front_DirtyRectText(XMid, 278, GameText.pHiResUnformatted3);
        break;

    case LVSTATUS_NOFILES:
        RGBRed();
        if (GLoadAction == LOAD_GAME)
        {
            Front_DirtyRectText(
                XMid, 220, GameText.pNoCareerFiles);
        }
        else
        {
            Front_DirtyRectText(
                XMid, 220, GameText.pNoVideoFiles);
        }
        if (MCResolution == RFRONT_STANDARD_WIDTH)
        {
            Front_PrintCentredPrompt(GameText.pContinue);
        }
        break;

    case LVSTATUS_SELECTFILE:
    {
        // Preserve the target local-common identity used by the native linker.
        static char msg[32] asm("msg.28");

        if (pVertMenu == 0)
        {
            VerticalMenu *m = static_cast<VerticalMenu *>(
                ::CClass::operator new(sizeof(VerticalMenu)));
            if (m != 0)
            {
                m = __12VerticalMenuiiiPPciPiT6i(
                    m, 100, 398, 0xf, VertMenuItem,
                    (NumVertMenuItems < 6) ? NumVertMenuItems : 5,
                    0, 0, 0);
            }
            pVertMenu = m;
            sprintf(msg, GameText.pHiResScroll, GameText.pHiResUpOrDown);
            Front_DirtyRectText(XMid, 380, msg);
        }

        for (i = 0; i < 5; i++)
        {
            if ((i != pVertMenu->getSelected()) ||
                (VertMenuTopChanged != 0))
            {
                pVertMenu->setString(
                    i, VertMenuItem[VertMenuTopIndex + i]);
            }
        }

        if (NumVertMenuItems > 5)
        {
            if (((TTime / 20) & 3) != 0)
            {
                int up;
                int top;
                int lastTop;
                int canScrollBothWays;

                RGBGreen();
                top = VertMenuTopIndex;
                up = top > 0;
                lastTop = NumVertMenuItems - 5;
                canScrollBothWays = top < lastTop;
                if (up && canScrollBothWays)
                {
                    sprintf(
                        msg, GameText.pHiResScroll,
                        GameText.pHiResUpOrDown);
                }
                else
                {
                    sprintf(
                        msg, GameText.pHiResScroll,
                        up ? GameText.pHiResUp
                           : GameText.pHiResDown);
                }
                Front_DirtyRectText(XMid, 380, msg);
            }
        }

        pVertMenu->vtable->draw(pVertMenu);
        if (MCResolution == RFRONT_STANDARD_WIDTH)
        {
            Front_PrintGoBackSelect();
        }
        break;
    }

    case LVSTATUS_LOADING:
        RGBGreen();
        Front_DirtyRectText(XMid, 220, RFrontLoadingVideoText);
        RGBRed();
        Front_DirtyRectText(XMid, 280, GameText.pDoNotRemove1);
        Front_DirtyRectText(XMid, 332, GameText.pDoNotRemove2);
        break;

    case LVSTATUS_LOADFAILED:
        RGBRed();
        Front_DirtyRectText(XMid, 220, GameText.pLoadFailed);
        if (MCResolution == RFRONT_STANDARD_WIDTH)
        {
            Front_PrintCentredPrompt(GameText.pContinue);
        }
        break;

    case LVSTATUS_LOADCOMPLETED:
        RGBRed();
        Front_DirtyRectText(XMid, 220, RFrontVideoLoadedText);
        if (MCResolution == RFRONT_STANDARD_WIDTH)
        {
            Front_PrintCentredPrompt(GameText.pContinue);
        }
        break;

    case LVSTATUS_FULLCARDWARNING:
        RGBRed();
        Front_DirtyRectText(XMid, 134, GameText.pCardFull1);
        Front_DirtyRectText(XMid, 170, GameText.pCardFull2);
        Front_DirtyRectText(XMid, 206, GameText.pCardFullWarning1);
        Front_DirtyRectText(XMid, 242, GameText.pCardFullWarning2);
        Front_DirtyRectText(XMid, 278, GameText.pCardFullWarning3);
        Front_DirtyRectText(XMid, 314, GameText.pCardFullWarning4);
        Front_DirtyRectText(XMid, 350, GameText.pCardFullWarning5);
        if (MCResolution == RFRONT_STANDARD_WIDTH)
        {
            Front_PrintCentredPrompt(GameText.pContinue);
        }
        break;

    case LVSTATUS_BUILDMENU:
        break;

    default:
        break;
    }

    return;
}


/**
 * Test whether the active name consists only of spaces or terminators.
 *
 * @return 1 when the name is blank, otherwise 0.
 */
int NameWasBlank(void) {
    int i = 0;
    register char *buf asm("$5") = &RFrontSaveNameViews::SaveGameName;
    asm("" : "+r"(buf), "+r"(i));
    int space = ' ';
    char *p;
    // The target keeps the buffer and cursor in $a1/$v0. Three ordinary
    // pointer-loop forms changed five of this function's 19 instructions.
    p = reinterpret_cast<char *>(
        i + reinterpret_cast<int>(buf));
loop:
    {
        int c = *reinterpret_cast<unsigned char *>(p);
        if (c == 0)
            return 1;
        if (c != space)
            return 0;
        i++;
        if (i < 10) {
            p = reinterpret_cast<char *>(
                i + reinterpret_cast<int>(buf));
            goto loop;
        }
    }
    return 1;
}


/**
 * Build the next default VIDEO or CAREER name and advance its suffix.
 */
void FillInDefaultName(void) {
    if (GSaveAction == SAVE_VIDEO) {
        int next;

        SaveGameName.b0 = 'V';
        SaveGameName.b1 = 'I';
        SaveGameName.b2 = 'D';
        SaveGameName.b3 = 'E';
        SaveGameName.b4 = 'O';
        SaveGameName.b5 = ' ';
        SaveGameName.b6 = static_cast<unsigned char>(Video) + 'A';
        next = Video + 1;
        Video = next % 26;
        return;
    }

    {
        int next;

        SaveGameName.b0 = 'C';
        SaveGameName.b1 = 'A';
        SaveGameName.b2 = 'R';
        SaveGameName.b3 = 'E';
        SaveGameName.b4 = 'E';
        SaveGameName.b5 = 'R';
        SaveGameName.b6 = ' ';
        SaveGameName.b7 = static_cast<unsigned char>(Career) + 'A';
        next = Career + 1;
        Career = next % 26;
    }
}


/**
 * Choose the front-end state entered after a successful save.
 *
 * @return The state appropriate to the current save and competition context.
 */
static EPausedStatus SaveReturnState(void) {
    if (GSaveAction != SAVE_VIDEO) goto paused;
    if (InCompetition() == 0) goto ret_f;
    if (Competition_IsOver() == 0) return STATUS_ENDHEATVIDEOMENU;
ret_f:
    return STATUS_VIDEOSCREEN;
paused:
    return static_cast<EPausedStatus>(GReturnToState);
}


/**
 * Advances the save/load memory-card state machine and returns the front-end
 * state that should run on the next frame.
 */
EPausedStatus Front_MemCardUpdate(void) {
    int Activated;
    int GoBack;
    int AnyButton;
    int Start;

    TTime++;
    if ((TTime & 1) != 0)
    {
        Card_CheckStatus(0, 0);
    }

    Front_GetButtons(Activated, GoBack, AnyButton, Start);

    switch (MCState)
    {
    case SVSTATUS_CHECKINGCARDPAUSE:
        if (CheckingCardPause != 0)
        {
            CheckingCardPause--;
            if (CheckingCardPause == 0)
            {
                MCState = SVSTATUS_CHECKINGCARD;
            }
        }
        else
        {
            CheckingCardPause = 10;
        }
        break;

    case SVSTATUS_CHECKINGCARD:
        if (VideoToDisk != 0)
        {
            MCState = SVSTATUS_ENTERNAME;
            break;
        }

        if (CardStatus == -1)
        {
            goto save_card_missing;
        }

        if (CardStatus < 0)
        {
            if (CardStatus == -2)
            {
                goto save_card_unformatted;
            }
            break;
        }

        if (CardStatus != 1)
        {
            break;
        }

        SaveFreeBlocks = Card_GetDirAndFreeBlocks(0, 0);
        if (GSaveAction == SAVE_GAME)
        {
            SaveNeedBlocks = 1;
            if (SaveFreeBlocks <= 0)
            {
                if (GHasCareerLoaded == 0)
                {
                    MCState = SVSTATUS_CARDFULL;
                    break;
                }
            }
            if (GHasCareerLoaded != 0)
            {
                for (int i = 0; i < 10; ++i)
                {
                    RFrontNameStorageView *names =
                        RFront_NameStorage(
                            &RFrontSaveNameViews::SaveGameName);
                    names->saveGameName[i] = names->storedCareerName[i];
                }
                MCState = SVSTATUS_SAVING;
                Delay = 4;
                Retries = 4;
            }
            else
            {
                MCState = SVSTATUS_ENTERNAME;
                InitEnterName();
            }
            break;
        }

        SaveNeedBlocks = 3;
        if (GNumberOfPlayers == 2)
        {
            SaveNeedBlocks = 6;
        }

        if (SaveFreeBlocks < SaveNeedBlocks)
        {
            MCState = SVSTATUS_CARDFULL;
        }
        else
        {
            MCState = SVSTATUS_ENTERNAME;
            InitEnterName();
        }
        break;

    save_card_missing:
        MCState = SVSTATUS_INSERTCARD;
        break;

    save_card_unformatted:
        MCState = SVSTATUS_UNFORMATTEDCARD;
        pYesNoMenu->SetLine(
            static_cast<unsigned char>(
                pYesNoMenu->FindEntry(GameText.pNo)));
        break;

    case SVSTATUS_CARDFULL:
        switch (CardStatus)
        {
        case -1:
            MCState = SVSTATUS_INSERTCARD;
            break;

        case -2:
        case 2:
            MCState = SVSTATUS_CHECKINGCARDPAUSE;
            break;

        default:
            break;
        }
        if (AnyButton != 0)
        {
            SFX_PlayX(2, RFRONT_SFX_VOLUME, 0);
            return static_cast<EPausedStatus>(SaveReturnState());
        }
        break;

    case SVSTATUS_ENTERNAME:
    {
        int UpDown = GetUpDown();

        if (UpDown == -1)
        {
            SizeUp = 16;
            SFX_PlayX(0x11, RFRONT_SFX_VOLUME, 0);
            if ((RFront_NameBytes(&RFrontSaveNameViews::SaveGameName)[CursorPos] == ' ') ||
                (RFront_NameBytes(&RFrontSaveNameViews::SaveGameName)[CursorPos] == '\0'))
            {
                RFront_NameBytes(&RFrontSaveNameViews::SaveGameName)[CursorPos] = 'A';
            }
            else
            {
                ++RFront_NameBytes(&RFrontSaveNameViews::SaveGameName)[CursorPos];
                if (RFront_NameBytes(&RFrontSaveNameViews::SaveGameName)[CursorPos] > 'Z')
                {
                    RFront_NameBytes(&RFrontSaveNameViews::SaveGameName)[CursorPos] = ' ';
                }
            }
        }

        if (UpDown == 1)
        {
            SizeUp = 16;
            SFX_PlayX(0x11, RFRONT_SFX_VOLUME, 0);
            if ((RFront_NameBytes(&RFrontSaveNameViews::SaveGameName)[CursorPos] == ' ') ||
                (RFront_NameBytes(&RFrontSaveNameViews::SaveGameName)[CursorPos] == '\0'))
            {
                RFront_NameBytes(&RFrontSaveNameViews::SaveGameName)[CursorPos] = 'Z';
            }
            else
            {
                --RFront_NameBytes(&RFrontSaveNameViews::SaveGameName)[CursorPos];
                if (RFront_NameBytes(&RFrontSaveNameViews::SaveGameName)[CursorPos] < 'A')
                {
                    RFront_NameBytes(&RFrontSaveNameViews::SaveGameName)[CursorPos] = ' ';
                }
            }
        }

        if ((Pad[0].Left.Triggered != 0) ||
            (Pad[1].Left.Triggered != 0))
        {
            SizeUp = 16;
            SFX_PlayX(5, RFRONT_SFX_VOLUME, 0);
            Pad[0].Left.Triggered = 0;
            Pad[1].Left.Triggered = 0;
            CursorPos--;
            if (CursorPos < 0)
            {
                CursorPos = 9;
            }
        }

        if ((Pad[0].Right.Triggered != 0) ||
            (Pad[1].Right.Triggered != 0))
        {
            SizeUp = 16;
            SFX_PlayX(5, RFRONT_SFX_VOLUME, 0);
            Pad[0].Right.Triggered = 0;
            Pad[1].Right.Triggered = 0;
            CursorPos++;
            if (CursorPos > 9)
            {
                CursorPos = 0;
            }
        }

        strcpy(
            GSaveAction == SAVE_VIDEO
                ? &RFrontSaveNameViews::LastVideoName
                : &RFrontSaveNameViews::LastCareerName,
            &RFrontSaveNameViews::SaveGameName);

        if (GoBack == 0)
        {
            if ((CardStatus == -1) && (VideoToDisk == 0))
            {
                MCState = SVSTATUS_INSERTCARD;
            }

            if ((Activated != 0) && (VideoToDisk == 0))
            {
                SFX_PlayX(5, RFRONT_SFX_VOLUME, 0);
                EFileType FileType = GSaveAction == SAVE_VIDEO
                    ? THPS_VIDEO_FILE : THPS_SAVE_FILE;

                if (NameWasBlank())
                {
                    do
                    {
                        FillInDefaultName();
                    } while (Card_FileExists(FileType));
                }

                if (Card_FileExists(FileType) &&
                    ((GSaveAction != SAVE_GAME) ||
                     (GHasCareerLoaded == 0)))
                {
                    MCState = SVSTATUS_OKTOOVERWRITE;
                    int entry = pYesNoMenu->FindEntry(GameText.pNo);
                    pYesNoMenu->SetLine(
                        static_cast<unsigned char>(entry));
                    pYesNoMenu->CentreY();
                }
                else
                {
                    MCState = SVSTATUS_SAVING;
                    Delay = 4;
                    Retries = 4;
                }
            }

            if ((Activated != 0) && (VideoToDisk != 0))
            {
                SFX_PlayX(5, RFRONT_SFX_VOLUME, 0);
                MCState = SVSTATUS_SAVING2;
            }
        }
        else
        {
            SFX_PlayX(2, RFRONT_SFX_VOLUME, 0);
            return static_cast<EPausedStatus>(SaveReturnState());
        }
        break;
    }

    case SVSTATUS_OKTOOVERWRITE:
        pYesNoMenu->Update();
        if (Activated != 0)
        {
            if (pYesNoMenu->ChoiceIs(GameText.pYes))
            {
                SFX_PlayX(5, RFRONT_SFX_VOLUME, 0);
                MCState = SVSTATUS_SAVING;
                Delay = 4;
                Retries = 4;
            }
            else
            {
                SFX_PlayX(2, RFRONT_SFX_VOLUME, 0);
                MCState = SVSTATUS_ENTERNAME;
                InitEnterName();
            }
        }
        if (GoBack != 0)
        {
            SFX_PlayX(2, RFRONT_SFX_VOLUME, 0);
            MCState = SVSTATUS_ENTERNAME;
            InitEnterName();
        }
        break;

    case SVSTATUS_INSERTCARD:
        if (CardStatus == -2)
        {
            MCState = SVSTATUS_CHECKINGCARDPAUSE;
        }
        else if (CardStatus > -3)
        {
            if (CardStatus < 3)
            {
                if (CardStatus > 0)
                {
                    MCState = SVSTATUS_CHECKINGCARDPAUSE;
                }
            }
        }
        if (AnyButton != 0)
        {
            SFX_PlayX(2, RFRONT_SFX_VOLUME, 0);
            return static_cast<EPausedStatus>(SaveReturnState());
        }
        break;

    case LVSTATUS_UNFORMATTEDCARD:
        if (CardStatus == -1)
        {
            MCState = LVSTATUS_INSERTCARD;
        }
        if (AnyButton != 0)
        {
            unsigned int sound = 0x91;
            if (GoBack != 0)
            {
                sound = 0x93;
            }
            SFX_PlayX(sound, RFRONT_SFX_VOLUME, 0);
            return static_cast<EPausedStatus>(SaveReturnState());
        }
        break;

    case SVSTATUS_UNFORMATTEDCARD:
        pYesNoMenu->Update();
        if (CardStatus != -1)
        {
            if (CardStatus >= -1)
            {
                if (CardStatus < 3)
                {
                    if (CardStatus > 0)
                    {
                        MCState = SVSTATUS_CHECKINGCARDPAUSE;
                    }
                }
            }
        }
        else
        {
            MCState = SVSTATUS_INSERTCARD;
        }

        if (Activated != 0)
        {
            if (pYesNoMenu->ChoiceIs(GameText.pYes))
            {
                SFX_PlayX(5, RFRONT_SFX_VOLUME, 0);
                MCState = SVSTATUS_FORMATTING;
                Delay = 4;
                Retries = 4;
            }
            else
            {
                SFX_PlayX(5, RFRONT_SFX_VOLUME, 0);
                return static_cast<EPausedStatus>(SaveReturnState());
            }
        }

        if (GoBack != 0)
        {
            SFX_PlayX(2, RFRONT_SFX_VOLUME, 0);
            MCState = SVSTATUS_CHECKINGCARDPAUSE;
        }
        break;

    case SVSTATUS_FORMATTING:
        GSoftResetEnabled = 0;
        if ((CardStatus == 2) || (CardStatus == -1))
        {
            MCState = SVSTATUS_FORMATFAILED;
            Delay = 75;
            break;
        }

        Delay--;
        if (Delay >= 0)
        {
            break;
        }
        if (CardStatus == 0)
        {
            break;
        }

        if (Card_FormatCard(0, 0) == 1)
        {
            MCState = SVSTATUS_FORMATSUCCEEDED;
            Delay = 75;
        }
        else
        {
            Retries--;
            if (Retries < 0)
            {
                MCState = SVSTATUS_FORMATFAILED;
                Delay = 75;
            }
            else
            {
                Delay = (4 - Retries) * 60;
            }
        }
        break;

    case SVSTATUS_SAVEFAILED:
    case SVSTATUS_SAVECOMPLETED:
        if (Activated != 0)
        {
            SFX_PlayX(5, RFRONT_SFX_VOLUME, 0);
            return static_cast<EPausedStatus>(SaveReturnState());
        }
        break;

    case SVSTATUS_FORMATFAILED:
    case SVSTATUS_FORMATSUCCEEDED:
        Delay--;
        if (Delay < 0)
        {
            if (CardStatus == -1)
            {
                MCState = SVSTATUS_INSERTCARD;
            }
            else
            {
                MCState = SVSTATUS_CHECKINGCARDPAUSE;
            }
        }

        if (AnyButton != 0)
        {
            unsigned int sound = 5;
            if (GoBack != 0)
            {
                sound = 2;
            }
            SFX_PlayX(sound, RFRONT_SFX_VOLUME, 0);
            MCState = SVSTATUS_CHECKINGCARDPAUSE;
        }
        break;

    case SVSTATUS_SAVING:
    {
        int WriteError;

        GSoftResetEnabled = 0;
        if (CardStatus == -1)
        {
            MCState = SVSTATUS_SAVEFAILED;
            Delay = 75;
        }

        Delay--;
        if (Delay >= 0)
        {
            break;
        }
        if (CardStatus == 0)
        {
            break;
        }

        switch (GSaveAction)
        {
        case SAVE_VIDEO:
            SaveNeedBlocks = 3;
            if (GNumberOfPlayers == 2)
            {
                SaveNeedBlocks = 6;
            }
            WriteError = Card_Write(
                THPS_VIDEO_FILE, SaveNeedBlocks);
            break;

        case SAVE_GAME:
            SaveNeedBlocks = 1;
            WriteError = Card_Write(
                THPS_SAVE_FILE, SaveNeedBlocks);
            break;

        default:
            break;
        }

        if (WriteError == 0)
        {
            MCState = SVSTATUS_SAVECOMPLETED;
            if (GSaveAction == SAVE_GAME)
            {
                int i = 0;
                GHasCareerLoaded = GSaveAction;
                GLoadedCareerIsSaved = GSaveAction;
                do
                {
                    reinterpret_cast<RFrontStoredCareerStateView *>(
                        &GLoadAction)
                        ->storedCareerName[i] =
                            (&RFrontSaveNameViews::SaveGameName)[i];
                    ++i;
                } while (i < 10);
            }
        }
        else
        {
            MCState = SVSTATUS_SAVEFAILED;
            Delay = 75;
        }
        break;
    }

    case SVSTATUS_SAVING2:
    {
        GSoftResetEnabled = 0;
        unsigned char first = static_cast<unsigned char>(
            RFrontSaveNameViews::SaveGameName);
        if (static_cast<unsigned int>(first - 'A') >= 26U)
        {
            RFrontSaveNameViews::SaveGameName = 'A';
            first = 'A';
        }

        if (first >= 'K')
        {
            MCState = SVSTATUS_ENTERNAME;
            Activated = 0;
            GoBack = 0;
        }
        else
        {
            char save_name[16];
            __builtin_memcpy(
                save_name, RFrontDemoFileName, sizeof(RFrontDemoFileName));
            save_name[4] = first;
            printf(RFrontSavingReplayFormat, save_name);
            Card_WriteReplayToDisk(
                THPS_VIDEO_FILE, save_name);
            MCState = SVSTATUS_SAVECOMPLETED;
        }
        break;
    }

    case SVSTATUS_DELETEQUERY:
        if (AnyButton != 0)
        {
            SFX_PlayX(5, RFRONT_SFX_VOLUME, 0);
        }
        break;

    case LVSTATUS_FULLCARDCHECK:
        if (CardStatus == 1)
        {
            if (Card_GetDirAndFreeBlocks(0, 0) != 0)
            {
                goto return_to_front_state;
            }
            MCState = LVSTATUS_FULLCARDWARNING;
        }
        else if (CardStatus != 0)
        {
            if (CardStatus != 2)
            {
                goto return_to_front_state;
            }
        }
        break;

    return_to_front_state:
        return static_cast<EPausedStatus>(GReturnToState);

    case LVSTATUS_FULLCARDWARNING:
        if (AnyButton != 0)
        {
            unsigned int sound = 0x91;
            if (GoBack != 0)
            {
                sound = 0x93;
            }
            SFX_PlayX(sound, RFRONT_SFX_VOLUME, 0);
            if (GoBack == 0)
            {
                return static_cast<EPausedStatus>(SaveReturnState());
            }
            return STATUS_LOADFAILED;
        }
        break;

    case LVSTATUS_CHECKINGCARDPAUSE:
        if (CheckingCardPause != 0)
        {
            CheckingCardPause--;
            if (CheckingCardPause == 0)
            {
                MCState = LVSTATUS_CHECKINGCARD;
            }
        }
        else
        {
            CheckingCardPause = 10;
        }
        break;

    case LVSTATUS_CHECKINGCARD:
        if (CardStatus == -1)
        {
            goto load_card_missing;
        }
        if (CardStatus < 0)
        {
            if (CardStatus == -2)
            {
                goto load_card_unformatted;
            }
            break;
        }
        if (CardStatus != 1)
        {
            break;
        }

        {
            SaveFreeBlocks = Card_GetDirAndFreeBlocks(0, 0);
            const EFileType WantedFileType =
                (GLoadAction == LOAD_GAME)
                    ? THPS_SAVE_FILE
                    : THPS_VIDEO_FILE;
            int Vids = 0;

            for (int i = 0; i < 15; ++i)
            {
                Vids += (FileType[i] == WantedFileType);
            }

            if (Vids != 0)
            {
                int m = 0;

                for (int i = 0; i < 15; ++i)
                {
                    // fileTypeBase must be written twice inside the loop:
                    // a single-assignment base is hoisted out of the loop and
                    // a single-assignment address becomes a walking pointer,
                    // both of which the original build does not do here.
                    EFileType *fileTypeBase = FileType;
                    EFileType *fileTypeAddress =
                        reinterpret_cast<EFileType *>(
                            i * sizeof(EFileType));
                    fileTypeAddress = reinterpret_cast<EFileType *>(
                        reinterpret_cast<int>(fileTypeAddress) +
                        reinterpret_cast<int>(fileTypeBase));
                    fileTypeBase = fileTypeAddress;
                    if (*fileTypeAddress == WantedFileType)
                    {
                        SCardHead CardHead;
                        char *p;

                        Card_ReadFileHeader(
                            SLOT_DIR[i].name, &CardHead);
                        p = ASCIIName[m];
                        int j;
                        for (j = 0; j < 10; ++j)
                        {
                            if (static_cast<unsigned char>(
                                    CardHead.Title[j * 2 + 28]) == 0)
                            {
                                *p++ = '\0';
                                break;
                            }
                            if (static_cast<unsigned char>(
                                    CardHead.Title[j * 2 + 28]) == 0x82)
                            {
                                *p = static_cast<unsigned char>(
                                         CardHead.Title[j * 2 + 29]) -
                                     0x1f;
                            }
                            else
                            {
                                *p = ' ';
                            }
                            ++p;
                        }

                        VertMenuItem[m] = ASCIIName[m];
                        *p = '\0';
                        VertMenuItemIndex[m] = i;
                        ++m;
                    }
                }

                NumVertMenuItems = m;
                VertMenuTopIndex = 0;
                MCState = LVSTATUS_SELECTFILE;
            }
            else
            {
                MCState = LVSTATUS_NOFILES;
            }
        }
        break;

    load_card_missing:
        MCState = LVSTATUS_INSERTCARD;
        break;

    load_card_unformatted:
        MCState = LVSTATUS_UNFORMATTEDCARD;
        break;

    case LVSTATUS_INSERTCARD:
        if (CardStatus == -2)
        {
            MCState = LVSTATUS_CHECKINGCARDPAUSE;
        }
        else if (CardStatus > -3)
        {
            if (CardStatus < 3)
            {
                if (CardStatus > 0)
                {
                    MCState = LVSTATUS_CHECKINGCARDPAUSE;
                }
            }
        }
        if (AnyButton != 0)
        {
            unsigned int sound = 0x91;
            if (GoBack != 0)
            {
                sound = 0x93;
            }
            SFX_PlayX(sound, RFRONT_SFX_VOLUME, 0);
            return static_cast<EPausedStatus>(SaveReturnState());
        }
        break;

    case LVSTATUS_NOFILES:
        if (AnyButton != 0)
        {
            unsigned int sound = 0x91;
            if (GoBack != 0)
            {
                sound = 0x93;
            }
            SFX_PlayX(sound, RFRONT_SFX_VOLUME, 0);
            return static_cast<EPausedStatus>(SaveReturnState());
        }
        break;

    case LVSTATUS_SELECTFILE:
        VertMenuTopChanged = 0;

        if ((Pad[0].Up.Triggered != 0) ||
            (Pad[1].Up.Triggered != 0))
        {
            Pad[0].Up.Triggered = 0;
            Pad[1].Up.Triggered = 0;
            if (pVertMenu->getSelected() > 0)
            {
                pVertMenu->up();
            }
            else
            {
                if (VertMenuTopIndex > 0)
                {
                    VertMenuTopIndex--;
                    VertMenuTopChanged = 1;
                }
            }
        }

        if ((Pad[0].Down.Triggered != 0) ||
            (Pad[1].Down.Triggered != 0))
        {
            Pad[0].Down.Triggered = 0;
            Pad[1].Down.Triggered = 0;
            if (VertMenuTopIndex < NumVertMenuItems - 5)
            {
                VertMenuTopIndex++;
                VertMenuTopChanged = 1;
            }
            else if (pVertMenu->getSelected() < 4)
            {
                pVertMenu->down();
            }
        }

        if ((GoBack != 0) || (CardStatus == -1))
        {
            SFX_PlayX(0x93, RFRONT_SFX_VOLUME, 0);
            goto return_to_front_state;
        }

        if (Activated != 0)
        {
            Card_SetSelectedFile(
                SLOT_DIR[
                    VertMenuItemIndex[
                        VertMenuTopIndex +
                        pVertMenu->getSelected()]]
                    .name);
            SFX_PlayX(0x91, RFRONT_SFX_VOLUME, 0);
            MCState = LVSTATUS_LOADING;
        }
        break;

    case LVSTATUS_LOADING:
        GSoftResetEnabled = 0;
        switch (GLoadAction)
        {
        case LOAD_VIDEO:
        {
            VblankUpdateBar = 1;
            int loadError = Card_Load(
                THPS_VIDEO_FILE, GVInfo1.pBlockStart, 0);
            VblankUpdateBar = 0;
            if (loadError != 0)
            {
                MCState = LVSTATUS_LOADFAILED;
                break;
            }

            SVideoHead *VH = RFront_GetVideoHeader(GVInfo1.pBlockStart);
            GVInfo1.NumFrames = VH->NumFrames;
            for (int i = 0; i < 5; ++i)
            {
                GVInfo1.Highlights[i] = VH->Highlights[i];
            }
            MCState = LVSTATUS_LOADCOMPLETED;
            break;
        }

        case LOAD_VIDEO_HEADER:
        {
            if (Card_Load(
                    THPS_VIDEO_HEADER, GVInfo1.pBlockStart, 0))
            {
                MCState = LVSTATUS_LOADFAILED;
                break;
            }

            SVideoHead *VH = RFront_GetVideoHeader(GVInfo1.pBlockStart);
            GVideoRestart.NumSkaters = VH->NumSkaters;
            GVideoRestart.Skater = VH->Skater;
            GVideoRestart.Skater2 = VH->Skater2;
            GVideoRestart.Level = VH->Level;
            GVideoRestart.GGame = VH->GGame;
            GVInfo1.NumFrames = VH->NumFrames;
            for (int i = 0; i < 5; ++i)
            {
                GVInfo1.Highlights[i] = VH->Highlights[i];
            }
            MCState = LVSTATUS_LOADCOMPLETED;
            break;
        }

        case LOAD_GAME:
        {
            unsigned long *pTmp = static_cast<unsigned long *>(
                Mem_New(sizeof(RFrontCareerLoadBlock), 1, 1));
            EMemCardState nextState;
            if (Card_Load(
                    THPS_SAVE_FILE, pTmp, 0) != 0)
            {
                nextState = LVSTATUS_LOADFAILED;
            }
            else
            {
                GCareer = RFront_GetCareerLoadBlock(pTmp)->career;
                GHasCareerLoaded = 1;
                GLoadedCareerIsSaved = 0;
                Front_ExtractASCIINameFromSJIS(
                    RFront_GetCareerLoadBlock(pTmp)->sjisName,
                    (RFront_NameStorage(
                        &RFrontSaveNameViews::SaveGameName))->storedCareerName);
                nextState = LVSTATUS_LOADCOMPLETED;
            }
            MCState = nextState;
            Mem_Delete(pTmp);
            break;
        }

        default:
            break;
        }
        break;

    case LVSTATUS_LOADFAILED:
        if (AnyButton != 0)
        {
            unsigned int sound = 0x91;
            if (GoBack != 0)
            {
                sound = 0x93;
            }
            SFX_PlayX(sound, RFRONT_SFX_VOLUME, 0);
            return STATUS_LOADFAILED;
        }
        break;

    case LVSTATUS_LOADCOMPLETED:
        return STATUS_LOADSUCCESS;

    case LVSTATUS_HIRESREFRESH1:
    case LVSTATUS_HIRESREFRESH2:
    case LVSTATUS_HIRESREFRESH3:
    case LVSTATUS_BUILDMENU:
        break;

    default:
        break;
    }

    return STATUS_MEMCARDMENUS;
}


/**
 * Clear the replay restart descriptor to its inactive sentinel state.
 */
void Front_ClearGVideoRestartData(void) {
    GVideoRestart.NumSkaters = 0;
    GVideoRestart.Skater = -1;
    GVideoRestart.Skater2 = -1;
    GVideoRestart.Level = -1;
    GVideoRestart.GGame = -1;
}
