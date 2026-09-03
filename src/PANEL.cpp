// PANEL.cpp -- Reconstructed translation unit
// Original: PANEL.CPP
// Functions: 41 reconstructed entries
// Address range: 0x80064400 - 0x800674B0
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

// PANEL-owned generated declarations and layouts. This was the only live
// consumer of the former PANEL.h reconstruction boundary.
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned char byte;
typedef int undefined;
typedef char undefined1;
typedef short undefined2;
typedef int undefined4;
typedef unsigned long ulong;
typedef unsigned char undefined3[3];

#include "globals.h"

typedef struct MeshHeader {
    u16 meshFlags;
    u16 vertexCount;
    u16 normalCount;
    u16 faceCount;
    u32 radius;
    s16 bboxXMax;
    s16 bboxXMin;
    s16 bboxYMax;
    s16 bboxYMin;
    s16 bboxZMax;
    s16 bboxZMin;
    s16 lodDepth;
    u16 lodNextMesh;
} MeshHeader;

struct CBody;
struct CSuper;
struct EditorOutModel;
struct EditorInModel;
struct LevelGenerator;
/** Pointer-only skater surface used by PANEL's score presentation. */
struct CBruce {
    /** Count completed half-turns for the active trick. */
    int Get180s();
    /** Add one trick occurrence to the skater's history. */
    int History_Add(char *name, int spinAndStance);
};
struct CBaddy;
struct SModel;
struct SHook;
struct CPowerUp;
struct SCareer;
struct SVideoInfo;
struct SDoubleBuffer;

enum EPausedStatus {
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

#include "CTRICK.h"

#include "MAIN.h"
#include "DB.h"
#include "FRONT.h"
#include "STATS.h"

#define CHEATPANELON_ABSOLUTE_DECLARATION
#include "CHEATPANELON.h"
#include "MECHLIST.h"

#include "CAREER.h"
#include "FONTTOOLS.h"
#include "MESS.h"
#include "TEXTEFFECTS.h"

struct FontCharacter;
struct Item;
#include "SPRITE.h"

#include "TEXTURE.h"

struct SVideoInfo {
    int VideoMode;
    unsigned char reservedStream[0x1c - 0x04];
    unsigned long NumFrames;
    unsigned long FramesLeft;
    unsigned char reservedMid[0x4c - 0x24];
    int ReplayHighlight;
    unsigned char reservedTail[192 - 0x50];
};

#include "SANIMFRAME.h"

/** Ordering-table prefix of the active renderer buffer. */
struct PanelDoubleBuffer {
    unsigned char reserved[132];
    unsigned int *PanelOT;
};

/** Polymorphic base used to preserve the leading PSY-Q class vtable slot. */
struct PanelPolymorphicBase {
    virtual ~PanelPolymorphicBase();
};

/** Assignment-only view preserving Font's original polymorphic copy shape. */
struct PanelAssignableFont : PanelPolymorphicBase {
    int JustX;
    int JustY;
    int SpaceX;
    int SpaceY;
    int R;
    int G;
    int B;
    void *Aliased;
    void *Shadow;
    int ShadowDx;
    int ShadowDy;
    int CharMap;
    int FixedWidth;
    int Scale;
    FontCharacter *CharTab;
    int NumChars;
    int Clut;
    TextEffect *CurrentTextEffect;
    unsigned char OldR;
    unsigned char OldG;
    unsigned char OldB;

};

/** Switch-stance sprite; only the CClass vtable slot matters to PANEL. */
struct Sprite2 {
    CClass CClass;
    unsigned char reserved[60 - sizeof(CClass)];

    static void *operator new(unsigned int size) throw() {
        return ::CClass::operator new(size);
    }
    /** Build a sprite from a bitmap file. */
    Sprite2(char *name, int pack, int sliceWidth, int sliceHeight);
};

/** Callable prefix of the target Sprite2 dispatch table. */
struct PanelSpriteDispatchTable {
    void *reserved0;
    void *reserved1;
    void (*Destroy)(Sprite2 *sprite, int flags);
    void *reservedDrawImage;
    void *reservedSetData;
    void (*Draw)(Sprite2 *sprite, int x, int y, int drawFirst);
};

/** Callable prefix of the target TextEffect dispatch table. */
struct PanelTextEffectDispatchTable {
    void *baseOffset;
    void *typeInfo;
    void (*Destroy)(TextEffect *effect, int flags);
    void (*Bind)(TextEffect *effect, Font *font);
    void (*Unbind)(TextEffect *effect, Font *font);
};

static inline PanelSpriteDispatchTable *PanelSpriteDispatch(Sprite2 *sprite)
{
    return reinterpret_cast<PanelSpriteDispatchTable *>(
        sprite->CClass.vtable);
}

static inline PanelTextEffectDispatchTable *PanelTextEffectDispatch(
    TextEffect *effect)
{
    return reinterpret_cast<PanelTextEffectDispatchTable *>(
        effect->CClass.vtable);
}

struct CPanelData {
    /** Shared panel sprite used to mark a switch stance. */
    static Sprite2 *mpSwitchSprite;

    /** Rebuild the three-line message for the current trick chain. */
    void TrickMessageRefresh();
    /** Return the spin multiplier displayed at the start of one trick run. */
    int TQ_SpinDisplay(int index);
    /** Accumulate the spin multiplier for the run containing one trick. */
    int TQ_SpinMultiplier(int index);
    /** Calculate the depreciated score for the active trick queue. */
    int TotalTrickScore();
    /** Calculate the current trick-chain score and combo multiplier. */
    int TotalScore();
    /** Draw and advance the active trick-message effect. */
    void DisplayTrick(CBruce *player, unsigned short x, unsigned short y);
    /** Start the active trick-message effect. */
    void TrickMessage();
    /** Queue a trick and update its spin, stance, and lock state. */
    void Trick(char *name, int points, int lock, int depreciate);
    /** Apply the current bail penalty and clear the trick queue. */
    void Bail();
    /** Apply the current landed combo to the panel totals. */
    void Land();
    /** Start the out-of-bounds message effect. */
    void OutOfBounds();

    CBruce *mpBruce;
    unsigned int mTotalScore;
    unsigned short mPlayerNum;
    unsigned short mNumTricksLanded;
    unsigned short mNumBails;
    unsigned short mNumTricksInCombo;
    unsigned int mBestComboScore;
    char mBestComboName[100];
    char mBestComboName2[100];
    char mBestComboName3[100];
    unsigned int mLandedTotal;
    unsigned int mLandedScore;
    unsigned int mLandedMultiplier;
    int mFallingEnabled;
    int mWantToFall;
    int mLastHoldingScore;
    int mLastMult;
    int mBailFlag;
    int mOutOfBounds;
    Font mTimerFont;
    Font mTrickFont;
    Font mScoreFont;
    FallingTextEffect mFallingTextEffect;
    TrickTextEffect mTrickTextEffect;
    TextEffect *mpTextEffect;
    FallingTextEffect mOutOfBoundsText;
    CTrick mTQ[20];
    int mTQStack;
    char mTrickName[100];
    char mTrickName2[100];
    char mTrickName3[100];
    int old_180spins;
    unsigned int prev_xblanks;
    Message *mTrickMessage;
    void *SKATE[5];
    int Goals;
    int mHoldingTankMultiplier;
    int mHoldingTankMultiplierTimer;
    int mHoldingTankScore;
    int mHoldingTankFlashTimer;

};

/** Owner-local CBruce slice through the last panel field read by this TU. */
struct PanelBruceScoreView {
    unsigned char reserved[28];
    void *mNextItem;
    unsigned char reservedAfterNext[264 - 32];
    unsigned int mExtraFlags;
    unsigned char reservedAfterFlags[400 - 268];
    CPanelData mPanelData;
    unsigned char reservedAfterPanel[3496 - 2676];
    int mSkaterType;
    int mIndex;
    int mPlayTime;
    void *mPad;
    int mTags;
    unsigned char reservedAfterTags[3972 - 3516];
    int mMagicPoints;
    int mMagicTime;
    unsigned char reservedAfterMagic[4132 - 3980];
    int mNum180Spins;
    int mComboCounter;
    int mPerfectLanding;
    int mSloppyLanding;
    unsigned char reservedAfterLanding[4204 - 4148];
    int mPhysicsState;
};

/** Owner-local SCareer prefix through SOptions::ScoreDisplayEnabled. */
struct PanelCareerScoreView {
    unsigned char reserved[2396];
    int ScoreDisplayEnabled;
};

/** Convert the opaque public skater type to PANEL's proven owner-local slice. */
static inline PanelBruceScoreView *PanelBruce(CBruce *player)
{
    return reinterpret_cast<PanelBruceScoreView *>(player);
}

static inline PanelBruceScoreView *PanelBruce(CBody *player)
{
    return reinterpret_cast<PanelBruceScoreView *>(player);
}

/** Recover the public CBruce identity from the global CBody list pointer. */
static inline CBruce *PanelBruceObject(CBody *player)
{
    return reinterpret_cast<CBruce *>(player);
}

/** View the score-display option through PANEL's proven SCareer prefix. */
static inline PanelCareerScoreView *PanelCareer(SCareer *career)
{
    return reinterpret_cast<PanelCareerScoreView *>(career);
}

/** Follow CBruce's intrusive object-list link through the proven local slice. */
static inline CBruce *PanelNextBruce(CBruce *player)
{
    return reinterpret_cast<CBruce *>(PanelBruce(player)->mNextItem);
}

/** Recover the enclosing effect type from its leading TextEffect subobject. */
static inline TrickTextEffect *PanelTrickEffect(TextEffect *effect)
{
    return reinterpret_cast<TrickTextEffect *>(effect);
}

/** Recover the enclosing effect type from its leading TextEffect subobject. */
static inline FallingTextEffect *PanelFallingEffect(TextEffect *effect)
{
    return reinterpret_cast<FallingTextEffect *>(effect);
}

typedef char panel_texture_size_must_be_40[(sizeof(Texture) == 40) ? 1 : -1];
typedef char panel_video_info_size_must_be_192[(sizeof(SVideoInfo) == 192) ? 1 : -1];
typedef char panel_font_size_must_be_80[(sizeof(Font) == 80) ? 1 : -1];
typedef char panel_assignable_font_size_must_be_80[
    (sizeof(PanelAssignableFont) == 80) ? 1 : -1];
typedef char panel_text_effect_size_must_be_8[(sizeof(TextEffect) == 8) ? 1 : -1];
typedef char panel_falling_text_effect_size_must_be_288[(sizeof(FallingTextEffect) == 288) ? 1 : -1];
typedef char panel_trick_text_effect_size_must_be_24[(sizeof(TrickTextEffect) == 24) ? 1 : -1];
typedef char panel_trick_size_must_be_36[(sizeof(CTrick) == 36) ? 1 : -1];
typedef char panel_sprite2_size_must_be_60[(sizeof(Sprite2) == 60) ? 1 : -1];
typedef char panel_sprite_dispatch_size_must_be_24[
    (sizeof(PanelSpriteDispatchTable) == 24) ? 1 : -1];
typedef char panel_text_effect_dispatch_prefix_size_must_be_20[
    (sizeof(PanelTextEffectDispatchTable) == 20) ? 1 : -1];
typedef char panel_data_size_must_be_2276[(sizeof(CPanelData) == 2276) ? 1 : -1];
typedef char panel_double_buffer_prefix_size_must_be_136[
    (sizeof(PanelDoubleBuffer) == 136) ? 1 : -1];
typedef char panel_bruce_view_size_must_be_4208[
    (sizeof(PanelBruceScoreView) == 4208) ? 1 : -1];
typedef char panel_career_view_size_must_be_2400[
    (sizeof(PanelCareerScoreView) == 2400) ? 1 : -1];

/** Readable, typed ABI bridges to services owned by other translation units. */
#include "PDOUBLEBUFFER.h"
#include "FRONTSTATUS.h"
#include "OTENTRY.h"

#include "SPOOL.h"

void Horse_DisplayWords(int *letters);
#include "VIDEO.h"

#include "SFX.h"
#include "TRICKTIPS.h"
#include "LOCALTXT.h"
#include "UTILS.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#define PANEL_DECLARE_DATA_CONSTRUCTOR_ABI
#define PANEL_DATA_CONSTRUCTOR_RECEIVER CPanelData
#define PANEL_DATA_CONSTRUCTOR_PLAYER CBruce
#define PANEL_PRIMITIVE_API
#include "PANEL.h"
#undef PANEL_PRIMITIVE_API
#include <STDIO.H>
#include <STRINGS.H>

/** Return caller-owned storage for explicit member construction. */
inline void *operator new(unsigned int, void *storage)
{
    return storage;
}

/** Diagnostic emitted while constructing the player's panel state. */
static char str_cpanel_print[]
    __attribute__((section(".rdata"))) =
        "CPanelData::CPanelData() called\n";

/**
 * Complete PANEL small-data prefix through the S-K-A-T-E label strings.
 *
 * Public globals retain their canonical linker symbols. Private storage uses
 * the target's natural identifiers without adding target-absent XDEFs.  The
 * namespace-scoped C-linkage owners remain lexically distinct from the
 * external-linkage address views required at selected call sites.
 */
static SAnimFrame *PanelAnim = 0;
Sprite2 *CPanelData::mpSwitchSprite = 0;
int LowMemoryIndicatorTimer = 0;

namespace PanelAddressViewOwners
{
extern "C"
{
static char str_panel_anim[8]
    __attribute__((section(".sdata"))) = "Font";
}
}
static char str_panel_sfx[16]
    __attribute__((section(".sdata"))) = "Panel_Init()\n";
static char str_panel_font1[12]
    __attribute__((section(".sdata"))) = "time.fnt";
static char str_panel_font2[12]
    __attribute__((section(".sdata"))) = "score.fnt";
static char str_panel_font3[16]
    __attribute__((section(".sdata"))) = "s2tricks.fnt";
static char str_panel_messfont[12]
    __attribute__((section(".sdata"))) = "ingame1.fnt";
static char str_panel_sprite[16]
    __attribute__((section(".sdata"))) = "s2switch.bmp";
static char str_panel_uninit[16]
    __attribute__((section(".sdata"))) = "Panel_Uninit()\n";

int TAPES_Display = 0;
int TAPES_Pos = 0x214;
int SKATE_Display = 0;
int SKATE_Pos = 0x214;

namespace PanelAddressViewOwners
{
extern "C"
{
static char str_letter_S[4]
    __attribute__((section(".sdata"))) = "S";
static char str_letter_K[4]
    __attribute__((section(".sdata"))) = "K";
static char str_letter_A[4]
    __attribute__((section(".sdata"))) = "A";
static char str_letter_T[4]
    __attribute__((section(".sdata"))) = "T";
static char str_letter_E[4]
    __attribute__((section(".sdata"))) = "E";
}
}

/** Percentage retained as repeated tricks depreciate through a combo. */
int Depreciation_Values[5] __attribute__((section(".data"))) = {
    100, 75, 50, 25, 10,
};

/** Multiplier contribution selected by a trick's spin bucket. */
int Spin_Mult_Values[6] __attribute__((section(".data"))) = {
    0, 1, 2, 4, 8, 16,
};

/** Multiplier applied to a completed chain by its trick-count bucket. */
int Num_Mult_Values[15] __attribute__((section(".data"))) = {
    0, 2, 4, 6, 8, 10, 12, 14,
    16, 18, 20, 22, 24, 26, 28,
};

/** Fixed score-overflow message emitted before the late PANEL formats. */
static char Panel_ScoreOverflow_owner[8] asm("$LC0")
    __attribute__((section(".sdata"))) = "Yowza!";

/*
 * These address-only views intentionally omit the storage section.  GCC 2.x
 * then reproduces the target's absolute address materialization for data that
 * this owner emits in .sdata.  Referencing the definitions directly changes
 * those sites to GP-relative code (measured below in the owning functions).
 */
extern char str_panel_anim[];
extern char Panel_ScoreOverflow_reference[] asm("$LC0");
extern char str_letter_S[];
extern char str_letter_K[];
extern char str_letter_A[];
extern char str_letter_T[];
extern char str_letter_E[];
extern char str_score_fmt[];
extern char str_mult_fmt[];
extern char str_goalcounter_name[];
extern char str_tmr_spin[];
extern char str_tmr_name[];
extern char str_tmr_cont[];
extern char str_tmr_break_spin[];
extern char str_tmr_break[];

/**
 * PANEL score, timer, goal, and trick-chain formats in their original
 * small-data order.
 */
namespace PanelAddressViewOwners
{
extern "C"
{
static char str_score_fmt[4]
    __attribute__((section(".sdata"))) = "%ld";
}
}
static char Panel_HalfMultiplierFormat[8]
    __attribute__((section(".sdata"))) = " x%d.5";
namespace PanelAddressViewOwners
{
extern "C"
{
static char str_mult_fmt[8]
    __attribute__((section(".sdata"))) = " x%d";
}
}
static char str_time_fmt[12]
    __attribute__((section(".sdata"))) = "%ld:%02ld";
namespace PanelAddressViewOwners
{
extern "C"
{
static char str_goalcounter_name[8]
    __attribute__((section(".sdata"))) = "spank";
}
}
static char str_goalcounter_fmt[12]
    __attribute__((section(".sdata"))) = "%d OF 5 %s";

static char str_tmr_perfect_spin[16]
    __attribute__((section(".sdata"))) = "~PERFECT& %d %s";
static char str_tmr_perfect[16]
    __attribute__((section(".sdata"))) = "~PERFECT& %s";
static char str_tmr_sloppy_spin[16]
    __attribute__((section(".sdata"))) = "|SLOPPY` %d %s";
static char str_tmr_sloppy[12]
    __attribute__((section(".sdata"))) = "|SLOPPY` %s";
namespace PanelAddressViewOwners
{
extern "C"
{
static char str_tmr_spin[8]
    __attribute__((section(".sdata"))) = "%d %s";
static char str_tmr_name[4]
    __attribute__((section(".sdata"))) = "%s";
}
}
static char str_tmr_cont_spin[12]
    __attribute__((section(".sdata"))) = "%s + %d %s";
namespace PanelAddressViewOwners
{
extern "C"
{
static char str_tmr_cont[8]
    __attribute__((section(".sdata"))) = "%s + %s";
}
}
static char str_tmr_overflow[12]
    __attribute__((section(".sdata"))) = "%s + ...";
namespace PanelAddressViewOwners
{
extern "C"
{
static char str_tmr_break_spin[8]
    __attribute__((section(".sdata"))) = "+ %d %s";
static char str_tmr_break[8]
    __attribute__((section(".sdata"))) = "+ %s";
}
}

/**
 * Zero-initialized PANEL state that follows the formatting strings.
 *
 * Defining the complete tail before the first function lets ASPSX retain the
 * original GP-relative accesses while preserving the target section order.
 */
int Cheat_BigScore = 0;
int GGameFont = 0;
unsigned int LastHoldingScore = 0;
void *prim_added = 0;

/** Returns the top edge of the balance meter's one-pixel shadow. */
static inline int PanelShadowTop(int y, int length)
{
    int offset = length - 1;
    return y - offset;
}

/** Returns the bottom edge of the balance meter's one-pixel shadow. */
static inline int PanelShadowBottom(int y, int length)
{
    int offset = length + 1;
    return y + offset;
}

// === Function implementations (address order) ===

/**
 * Panel_Line__Fiiiii
 *
 * Address:   0x80064400
 * Mangled:   Panel_Line__Fiiiii
 * Demangled: Panel_Line(int, int, int, int, int)
 *            Creates a LINE_F2 primitive with given endpoints and color, links into OT
 *
 * @param x0  X component for vertex/component 0.
 * @param y0  Y component for vertex/component 0.
 * @param x1  X component for vertex/component 1.
 * @param y1  Y component for vertex/component 1.
 * @param Color  Packed RGB value for the flat-shaded line.
 */
void Panel_Line(int x0, int y0, int x1, int y1, int Color)
{
    LINE_F2 *aline = reinterpret_cast<LINE_F2 *>(pPoly);
    unsigned char *newppoly =
        reinterpret_cast<unsigned char *>(pPoly) + sizeof(LINE_F2);
    pPoly = reinterpret_cast<unsigned int *>(newppoly);
    *reinterpret_cast<unsigned int *>(&aline->r0) =
        static_cast<unsigned int>(Color) + 0x40000000;
    aline->tag = 0x03000000;
    aline->x0 = x0;
    aline->y0 = y0;
    aline->x1 = x1;
    aline->y1 = y1;
    addPrim(reinterpret_cast<PanelDoubleBuffer *>(pDoubleBuffer)->PanelOT, aline);
}

/**
 * Draws a vertical balance meter, its current-position marker, and a dimmed
 * one-pixel shadow.
 *
 * Address:   0x80064480
 * Mangled:   Panel_BalanceUD__Fiiiiiii
 * Demangled: Panel_BalanceUD(int, int, int, int, int, int, int)
 *
 * @param Balance  Current signed balance value.
 * @param Max  Maximum balance magnitude used to scale the marker.
 * @param Length  Half-height of the meter.
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param LineColor  Main meter color.
 * @param MarkerColor  Current-position marker color.
 */
void Panel_BalanceUD(int Balance, int Max, int Length, int x, int y,
                     int LineColor, int MarkerColor)
{
    int ShadowColor;

    Panel_Line(x, y + Balance * Length / Max,
               x + 6, y + Balance * Length / Max, MarkerColor);

    ShadowColor = (LineColor >> 3) & 0x001F1F1F;
    Panel_Line(x, y - Length, x, y + Length, LineColor);
    Panel_Line(x, y - Length, x + 6, y - Length, LineColor);
    Panel_Line(x, y, x + 6, y, LineColor);
    Panel_Line(x, y + Length, x + 6, y + Length, LineColor);

    Panel_Line(x + 1, PanelShadowTop(y, Length),
               x + 1, PanelShadowBottom(y, Length), ShadowColor);
    Panel_Line(x + 1, PanelShadowTop(y, Length),
               x + 7, PanelShadowTop(y, Length), ShadowColor);
    Panel_Line(x + 1, y + 1, x + 7, y + 1, ShadowColor);
    Panel_Line(x + 1, PanelShadowBottom(y, Length),
               x + 7, PanelShadowBottom(y, Length), ShadowColor);
}

/**
 * Adds a panel primitive to the active ordering table and records that the
 * frame submitted at least one primitive.
 *
 * Address:   0x80064614
 * Mangled:   Panel_addPrim__FPv
 * Demangled: Panel_addPrim(void *)
 *            PS1 OT primitive insertion: links prim into ordering table via 24-bit pointer manipulation
 *
 * @param prim  Primitive to prepend to the panel ordering table.
 */
void Panel_addPrim(void *prim)
{
    PanelDoubleBuffer *buffer =
        reinterpret_cast<PanelDoubleBuffer *>(pDoubleBuffer);
    addPrim(buffer->PanelOT, prim);
    prim_added = reinterpret_cast<void *>(1);
}

// ====== 0x80064668 Panel_Init__Fv ==============================================
// Panel_Init__Fv
// Address: 0x80064668
// Demangled: Panel_Init(void)
// Loads animations, fonts, creates a Sprite2 for panel display, inits falling text
void Panel_Init(void)
{
    PanelAnim = Spool_FindAnim(str_panel_anim);
    printf(str_panel_sfx);
    FontManager::LoadFont(str_panel_font1);
    FontManager::LoadFont(str_panel_font2);
    FontManager::LoadFont(str_panel_font3);
    GGameFont = Mess_LoadFont(str_panel_messfont, 2, 2, -1);
    Sprite2 *ptr = new Sprite2(
        str_panel_sprite, 1, 0, 0);
    CPanelData::mpSwitchSprite = ptr;
    FallingTextEffect::InitSprite();
}

// ====== 0x80064710 Panel_Uninit__Fv ============================================
// Panel_Uninit__Fv
// Address: 0x80064710
// Demangled: Panel_Uninit(void)
// Unloads fonts, destroys panel sprite, frees falling text
void Panel_Uninit(void)
{
    printf(str_panel_uninit);
    FontManager::UnloadFont(str_panel_font1);
    FontManager::UnloadFont(str_panel_font2);
    FontManager::UnloadFont(str_panel_font3);
    Mess_SetCurrentFont(GGameFont);
    Mess_UnloadFont();
    Sprite2 *sprite = CPanelData::mpSwitchSprite;
    if (sprite != 0)
    {
        PanelSpriteDispatch(sprite)->Destroy(sprite, 3);
    }
    CPanelData::mpSwitchSprite = 0;
    FallingTextEffect::FreeSprite();
}

/**
 * Panel_Update__Fv
 *
 * Address:   0x800647A0
 * Mangled:   Panel_Update__Fv
 * Demangled: Panel_Update(void)
 *            Reserved update hook; intentionally empty in this build.
 *
 */
void Panel_Update(void)
{
}

/**
 * Allocates and submits a textured quad for one animation frame.
 *
 * Address:   0x800647A8
 * Mangled:   Panel_DrawTexturedPoly__FP10SAnimFrame
 * Demangled: Panel_DrawTexturedPoly(SAnimFrame *)
 *
 * @param pFrame  Frame to operate on.
 */
POLY_FT4 *Panel_DrawTexturedPoly(SAnimFrame *pFrame)
{
    POLY_FT4 *apoly = reinterpret_cast<POLY_FT4 *>(pPoly);
    unsigned char *newppoly =
        reinterpret_cast<unsigned char *>(pPoly) + sizeof(POLY_FT4);
    if (newppoly > PolyBufferEnd)
    {
        return 0;
    }

    pPoly = reinterpret_cast<unsigned int *>(newppoly);
    apoly->tag = 0x09000000;
    *reinterpret_cast<unsigned int *>(&apoly->r0) = 0x2C808080;

    Texture *stored = pFrame->pTexture;
    unsigned int foo1 = *reinterpret_cast<unsigned int *>(&stored->u0);
    unsigned int foo2 = *reinterpret_cast<unsigned int *>(&stored->u1);
    unsigned int foo3 = *reinterpret_cast<unsigned int *>(&stored->u2);
    unsigned short lastUV =
        *reinterpret_cast<unsigned short *>(&stored->u3);
    *reinterpret_cast<unsigned int *>(&apoly->u0) = foo1;
    *reinterpret_cast<unsigned int *>(&apoly->u1) = foo2;
    *reinterpret_cast<unsigned int *>(&apoly->u2) = foo3;
    *reinterpret_cast<unsigned short *>(&apoly->u3) = lastUV;

    addPrim(reinterpret_cast<PanelDoubleBuffer *>(pDoubleBuffer)->PanelOT, apoly);
    return apoly;
}

/**
 * Writes horizontally stretched screen coordinates for an animation frame.
 *
 * Address:   0x8006485C
 * Mangled:   Panel_SetStretchedScreenCoords__FiiP8POLY_FT4P10SAnimFrame
 * Demangled: Panel_SetStretchedScreenCoords(int, int, POLY_FT4 *, SAnimFrame *)
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param pPol  Pol to operate on.
 * @param pFrame  Frame to operate on.
 */
void Panel_SetStretchedScreenCoords(int x, int y, POLY_FT4 *pPol,
                                    SAnimFrame *pFrame)
{
    int Width = static_cast<int>(pFrame->Width) * 512 / 320;
    int Height = pFrame->Height;
    int OffX = static_cast<int>(pFrame->OffX) * 512 / 320;
    int OffY = pFrame->OffY;
    pPol->x0 = x + OffX;
    pPol->y0 = y + OffY;
    pPol->x1 = x + OffX + Width;
    pPol->y1 = y + OffY;
    pPol->x2 = x + OffX;
    pPol->y2 = y + OffY + Height;
    pPol->x3 = x + OffX + Width;
    pPol->y3 = y + OffY + Height;
}

/**
 * Allocates and submits a flat-shaded panel quad.
 *
 * Address:   0x800648D8
 * Mangled:   Panel_DrawFlatShadedPoly__FiiiiUcUcUci
 * Demangled: Panel_DrawFlatShadedPoly(int, int, int, int, unsigned char, unsigned char, unsigned char, int)
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param w  Width.
 * @param h  Height.
 * @param r  Red component (0-255).
 * @param g  Green component (0-255).
 * @param b  Blue component (0-255).
 * @param sort  Panel ordering-table bucket.
 * @return The allocated primitive, or null when the primitive buffer is full.
 */
POLY_F4 *Panel_DrawFlatShadedPoly(int x, int y, int w, int h, unsigned char r, unsigned char g, unsigned char b, int sort)
{
    POLY_F4 *apoly = reinterpret_cast<POLY_F4 *>(pPoly);
    unsigned char *newppoly =
        reinterpret_cast<unsigned char *>(pPoly) + sizeof(POLY_F4);

    if (newppoly > PolyBufferEnd)
        return 0;

    pPoly = reinterpret_cast<unsigned int *>(newppoly);
    (setlen(apoly, 5), apoly->code = 0x28);
    setRGB0(apoly, r, g, b);

    apoly->x0 = x;
    apoly->y0 = y;
    apoly->x1 = x + w;
    apoly->y1 = y;
    apoly->x2 = x;
    apoly->y2 = y + h;
    apoly->x3 = x + w;
    apoly->y3 = y + h;

    addPrim(&reinterpret_cast<PanelDoubleBuffer *>(pDoubleBuffer)->PanelOT[sort], apoly);
    return apoly;
}

/**
 * Allocates and submits a vertically shaded panel quad.
 *
 * Address:   0x800649B8
 * Mangled:   DrawGouraudPoly__FiiiiUcUcUcUcUcUci
 * Demangled: DrawGouraudPoly(int, int, int, int, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, int)
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param w  Width.
 * @param h  Height.
 * @param rtop  Red component for the top edge.
 * @param gtop  Green component for the top edge.
 * @param btop  Blue component for the top edge.
 * @param rbottom  Red component for the bottom edge.
 * @param gbottom  Green component for the bottom edge.
 * @param bbottom  Blue component for the bottom edge.
 * @param sort  Panel ordering-table bucket.
 * @return The allocated primitive, or null when the primitive buffer is full.
 */
POLY_G4 *DrawGouraudPoly(int x, int y, int w, int h, unsigned char rtop, unsigned char gtop, unsigned char btop, unsigned char rbottom, unsigned char gbottom, unsigned char bbottom, int sort)
{
    POLY_G4 *apoly = reinterpret_cast<POLY_G4 *>(pPoly);
    unsigned char *newppoly =
        reinterpret_cast<unsigned char *>(pPoly) + sizeof(POLY_G4);

    if (newppoly > PolyBufferEnd)
        return 0;

    pPoly = reinterpret_cast<unsigned int *>(newppoly);
    (setlen(apoly, 8), apoly->code = 0x38);

    apoly->r0 = rbottom;
    apoly->r1 = rtop;
    apoly->g0 = gbottom;
    apoly->g1 = gtop;
    apoly->b0 = bbottom;
    apoly->b1 = btop;
    apoly->r2 = rbottom;
    apoly->r3 = rtop;
    apoly->g2 = gbottom;
    apoly->g3 = gtop;
    apoly->b2 = bbottom;
    apoly->b3 = btop;

    apoly->x0 = x;
    apoly->x1 = x + w;
    apoly->y0 = y;
    apoly->y1 = y;
    apoly->x2 = x;
    apoly->x3 = x + w;
    apoly->y2 = y + h;
    apoly->y3 = y + h;

    addPrim(&reinterpret_cast<PanelDoubleBuffer *>(pDoubleBuffer)->PanelOT[sort], apoly);
    return apoly;
}

/**
 * Updates and draws the collected SKATE letters, then advances the tape
 * counter's slide animation.
 */
void DisplayTapesSkate(void)
{
    PanelBruceScoreView *player = PanelBruce(MechList);
    if (player->mNextItem == 0)
    {
        int skateDisp = SKATE_Display;
        if (skateDisp != 0)
        {
            int skatePos = SKATE_Pos;
            if (skatePos >= 483)
            {
                if (skateDisp >= 11)
                {
                    SKATE_Pos = skatePos - 4;
                }
            }
            if (skateDisp < 10)
            {
                SKATE_Pos = 522 - (skateDisp << 2);
            }
            SKATE_Display = skateDisp - 1;
            unsigned int xb = static_cast<unsigned int>(Xblanks);
            unsigned int rem = xb - (xb / 20u) * 20u;
            int color = static_cast<int>((rem * 12u) & 0xfcu);
            Mess_SetRGB(color, 100, 100, 0);
            Mess_SetTextJustify(1);
            Mess_SetScale(0xff);
            if (player->mPanelData.SKATE[0] != 0)
            {
                Mess_DrawPanelText(SKATE_Pos, 80,
                                   str_letter_S, 0, 0, 0x100);
            }
            if (player->mPanelData.SKATE[1] != 0)
            {
                Mess_DrawPanelText(SKATE_Pos, 105,
                                   str_letter_K, 0, 0, 0x100);
            }
            if (player->mPanelData.SKATE[2] != 0)
            {
                Mess_DrawPanelText(SKATE_Pos, 130,
                                   str_letter_A, 0, 0, 0x100);
            }
            if (player->mPanelData.SKATE[3] != 0)
            {
                Mess_DrawPanelText(SKATE_Pos, 155,
                                   str_letter_T, 0, 0, 0x100);
            }
            if (player->mPanelData.SKATE[4] != 0)
            {
                Mess_DrawPanelText(SKATE_Pos, 180,
                                   str_letter_E, 0, 0, 0x100);
            }
        }
        if (TAPES_Display != 0)
        {
            if (TAPES_Pos >= 482)
            {
                if (TAPES_Display >= 11)
                    TAPES_Pos -= 4;
            }
            if (TAPES_Display < 11)
                TAPES_Pos = 511 - (TAPES_Display << 2);
            --TAPES_Display;
        }
    }
}

/**
 * GraffitiColor__FP6CBruce
 *
 * Address:   0x80064CFC
 * Mangled:   GraffitiColor__FP6CBruce
 * Demangled: GraffitiColor(CBruce *)
 *            Sets graffiti text color based on team/stance when game mode is 8 (graffiti)
 *
 * @param pBruce  Active skater (CBruce) being processed.
 */
void GraffitiColor(CBruce *pBruce)
{
    if (GGame == 8)
    {
        PanelBruceScoreView *player = PanelBruce(pBruce);
        if (player->mIndex == 0)
        {
            Mess_SetRGB(0x50, 0x50, 0x80, 0);
            player->mPanelData.mTrickFont.setRGB(0x50, 0x50, 0x80);
        }
        else
        {
            Mess_SetRGB(0x80, 0x50, 0x50, 0);
            player->mPanelData.mTrickFont.setRGB(0x80, 0x50, 0x50);
        }
    }
}

/**
 * DisplayScore__FP6CBruceUsUs
 *
 * Address:   0x80064D8C
 * Mangled:   DisplayScore__FP6CBruceUsUs
 * Demangled: DisplayScore(CBruce *, unsigned short, unsigned short)
 *            Draws the score and magic-point bar, transfers newly landed
 *            points into the total, and awards competition score goals.
 *
 * @param pBruce  Active skater (CBruce) being processed.
 * @param xpos  X coordinate.
 * @param ypos  Y coordinate.
 */
void DisplayScore(CBruce *pBruce, unsigned short xpos, unsigned short ypos)
{
    PanelBruceScoreView *player = PanelBruce(pBruce);
    unsigned int x = xpos;
    unsigned int y = ypos;
    int DoDisplay = GVInfo1.VideoMode != 2;

    if (Cheat_Fire != 0)
    {
        player->mMagicTime = 750;
        player->mMagicPoints = 750;
    }

    if (GGame == 7)
    {
        if (Cheat_Fire == 0)
        {
            player->mMagicTime = 0;
            player->mMagicPoints = -1;
        }
        Horse_DisplayWords(&GHorseLetters[0]);
        return;
    }

    if (DoDisplay != 0)
    {
        int barWidth = player->mMagicPoints;
        if (barWidth != -1)
        {
            if (player->mMagicTime != 0)
            {
                barWidth = player->mMagicTime * 120 / 750;
                DrawGouraudPoly(x - 1, y + 10, barWidth, 3,
                                (static_cast<unsigned int>(Xblanks) % 40) * 6,
                                (static_cast<unsigned int>(Xblanks) % 40) * 6,
                                50,
                                (static_cast<unsigned int>(Xblanks) % 40) * 6,
                                (static_cast<unsigned int>(Xblanks) % 40) * 6,
                                50, 0);
            }
            else if (barWidth != 0)
            {
                barWidth /= 25;
                DrawGouraudPoly(x - 1, y + 10, barWidth, 3,
                                0, 150, 0, 0, 150, 50, 0);
            }
        }

        int r = -1;
        int g = -1;
        OT_entry = 1;
        int b = -1;
        if (GGame == 8)
        {
            if (player->mIndex == 0)
            {
                r = 0x50;
                g = 0x50;
                b = 0x80;
            }
            else
            {
                r = 0x80;
                g = 0x50;
                b = 0x50;
            }
        }
        pScoreSprite->draw(x, y - 20, 1, r, g, b);

        if (GGame != 7)
        {
            if (player->mMagicTime == 0)
            {
                pBarSprite->draw(x - 4, y, 1, r, g, b);
            }
            else
            {
                pBarSprite->draw(
                    x - 4, y, 1,
                    (static_cast<unsigned int>(Xblanks) % 40) * 6,
                    (static_cast<unsigned int>(Xblanks) % 40) * 6,
                    50);
            }
        }
    }

    if (GGame != 7 && player->mPanelData.mLandedTotal != 0 &&
        player->mPanelData.mHoldingTankFlashTimer == 0)
    {
        unsigned int landed = player->mPanelData.mLandedTotal;
        if (GGame == 8)
        {
            player->mPanelData.mLandedTotal = 0;
        }
        else if (landed >= 10001)
        {
            player->mPanelData.mLandedTotal = landed - 250;
            player->mPanelData.mTotalScore += 250;
        }
        else if (landed >= 2001)
        {
            player->mPanelData.mLandedTotal = landed - 100;
            player->mPanelData.mTotalScore += 100;
        }
        else if (landed >= 26)
        {
            player->mPanelData.mLandedTotal = landed - 25;
            player->mPanelData.mTotalScore += 25;
        }
        else
        {
            player->mPanelData.mLandedTotal = 0;
            player->mPanelData.mTotalScore += landed;
        }
    }

    if (DoDisplay != 0)
    {
        if (GGame == 1 && Levels[GLevel].isCompetition == 0)
        {
            if (player->mPanelData.mTotalScore >= Levels[GLevel].HiScore &&
                Career_Got(0) == 0)
            {
                Panel_Tape(0);
            }
            if (player->mPanelData.mTotalScore >= Levels[GLevel].ProScore &&
                Career_Got(4) == 0)
            {
                Panel_Tape(4);
            }
        }
    }

    if (DoDisplay != 0)
    {
        char score[15];
        if (player->mPanelData.mTotalScore >= 1000000)
        {
            // The original variadic call supplied the score even though this
            // fixed overflow message does not consume it.
            sprintf(score, Panel_ScoreOverflow_reference,
                    player->mPanelData.mTotalScore);
            player->mPanelData.mScoreFont.setRGB(
                (static_cast<unsigned int>(Xblanks) % 20) * 12, 100, 100);
        }
        else
        {
            player->mPanelData.mScoreFont.setRGB(0x80, 0x80, 0x80);
            int Value = player->mPanelData.mTotalScore;
            if (GGame == 8)
            {
                Value = player->mTags;
                GraffitiColor(pBruce);
            }
            sprintf(score, str_score_fmt, Value);
        }
        player->mPanelData.mScoreFont.draw(x + 125, y - 4, score, 0);
    }

}

/**
 * DisplaySwitchness__FP6CBruceUsUs
 *
 * Address:   0x800653B4
 * Mangled:   DisplaySwitchness__FP6CBruceUsUs
 * Demangled: DisplaySwitchness(CBruce *, unsigned short, unsigned short)
 *            Checks if current stance is "switch" and updates sprite display accordingly
 *
 * @param pBruce  Active skater (CBruce) being processed.
 * @param xpos  X coordinate.
 * @param ypos  Y coordinate.
 */
void DisplaySwitchness(CBruce *pBruce, unsigned short xpos, unsigned short ypos)
{
    register int masked_xpos asm("$5") = xpos;
    masked_xpos &= 0xffff;
    register SStats *table asm("$7") = Stats;
    unsigned int Flipped = PanelBruce(pBruce)->mExtraFlags;
    int skaterType = PanelBruce(pBruce)->mSkaterType;
    Flipped = (Flipped >> 1) & 1;
    int isZero = table[skaterType].Ability[11] < 1;
    if (Flipped != isZero)
    {
        register Sprite2 *sprite asm("$2") = CPanelData::mpSwitchSprite;
        PanelSpriteDispatch(sprite)->Draw(sprite, masked_xpos,
                                          static_cast<int>(ypos), 0);
    }
}

/**
 * Draws the score and multiplier for the active trick chain, then refreshes
 * the trick message when the skater's spin count changes.
 *
 * Address: 0x80065438
 * Mangled: DisplayHolding__FP6CBruceUsUs
 * Demangled: DisplayHolding(CBruce *, unsigned short, unsigned short)
 *
 * @param pBruce  Active skater whose held trick is displayed.
 * @param xpos  Horizontal text position.
 * @param ypos  Vertical text position.
 */
/** Formats the multiplier text into its fixed-size panel buffer. */
static inline void Panel_FormatMultiplier(char (&buffer)[15], int multiplier)
{
    sprintf(buffer, str_mult_fmt, multiplier >> 1);
}

void DisplayHolding(CBruce *pBruce, unsigned short xpos, unsigned short ypos)
{
    PanelBruceScoreView *player = PanelBruce(pBruce);
    unsigned int HoldingScore = 0;
    int Mult = 0;
    int bailing;
    int DoDisplay = (GVInfo1.VideoMode != 2);

    if (PanelCareer(&GCareer)->ScoreDisplayEnabled != 0)
    {
        if (player->mPanelData.mTQStack > 0)
        {
            Mult = player->mPanelData.mTQStack << 1;
            HoldingScore = player->mPanelData.TotalTrickScore();
        }
        if (player->mPanelData.mLandedTotal != 0)
        {
            HoldingScore = player->mPanelData.mLandedTotal;
        }

        bailing = 0;
        if (player->mPanelData.mBailFlag != 0)
        {
            bailing = 1;
        }
        else if (player->mPanelData.mpTextEffect ==
                 &player->mPanelData.mFallingTextEffect.TextEffect)
        {
            bailing = 1;
        }

        if (HoldingScore != 0 || bailing != 0)
        {
            char score[15];
            sprintf(score, str_score_fmt,
                    bailing != 0
                        ? static_cast<unsigned int>(LastHoldingScore)
                        : HoldingScore);
            player->mPanelData.mTrickFont.setRGB(0x80, 0x80, 0x80);
            GraffitiColor(pBruce);
            if (Mult >= 3)
                player->mPanelData.mTrickFont.JustX = 2;
            else
                player->mPanelData.mTrickFont.JustX = 1;
            if (DoDisplay != 0)
            {
                player->mPanelData.mFallingTextEffect.bind(
                    &player->mPanelData.mTrickFont);
                player->mPanelData.mTrickFont.Scale = 0x1000;
                player->mPanelData.mTrickFont.draw(static_cast<int>(xpos),
                                                   static_cast<int>(ypos),
                                                   score, 0);
                player->mPanelData.mFallingTextEffect.TextEffect.unbind(
                    &player->mPanelData.mTrickFont);
            }
            if (player->mPanelData.mHoldingTankFlashTimer > 0)
            {
                --player->mPanelData.mHoldingTankFlashTimer;
            }
            if (Mult >= 3)
            {
                char mult[15];
                Panel_FormatMultiplier(mult, Mult);
                player->mPanelData.mTrickFont.setRGB(0x80, 0x80, 0x80);
                GraffitiColor(pBruce);
                player->mPanelData.mTrickFont.JustX = 0;
                if (DoDisplay != 0)
                {
                    player->mPanelData.mFallingTextEffect.bind(
                        &player->mPanelData.mTrickFont);
                    player->mPanelData.mTrickFont.Scale = 0x1000;
                    player->mPanelData.mTrickFont.draw(static_cast<int>(xpos),
                                                       static_cast<int>(ypos),
                                                       mult, 0);
                    player->mPanelData.mFallingTextEffect.TextEffect.unbind(
                        &player->mPanelData.mTrickFont);
                }
            }
            if (HoldingScore != 0)
            {
                LastHoldingScore = static_cast<int>(
                    (HoldingScore * static_cast<unsigned int>(Mult)) >> 1);
            }
        }

        if (player->mPanelData.mTQStack != 0)
        {
            if (player->mNum180Spins > player->mPanelData.old_180spins)
            {
                if (player->mPhysicsState != 2 ||
                    player->mNum180Spins >= 2)
                {
                    player->mPanelData.TrickMessage();
                    player->mPanelData.old_180spins = player->mNum180Spins;
                }
            }
        }
    }
}

/**
 * DisplayTime__FP6CBruceUsUs
 *
 * Address:   0x800656BC
 * Mangled:   DisplayTime__FP6CBruceUsUs
 * Demangled: DisplayTime(CBruce *, unsigned short, unsigned short)
 *
 * @param pBruce  Active skater (CBruce) being processed.
 * @param xpos  X coordinate.
 * @param ypos  Y coordinate.
 */
void DisplayTime(CBruce *pBruce, unsigned short xpos, unsigned short ypos)
{
    char time[8];
    int ticks;
    int mins;
    int secs;
    SVideoInfo *VInfo;
    static int Urgent asm("Urgent.214");
    int sub;
    PanelBruceScoreView *timer = PanelBruce(pBruce);
    if (GGame != 2 && (GGame != 7 || timer->mPlayTime != 0))
    {
        if (PanelBruce(pBruce) == PanelBruce(MechList))
        {
            VInfo = &GVInfo1;
        }
        else
        {
            VInfo = &GVInfo2;
        }
        if (VInfo->VideoMode != 2)
        {
            ticks = timer->mPlayTime - XFrames;
            mins = ticks / 3600;
            secs = (ticks % 3600) / 60;
            if (ticks < 0)
            {
                secs = 0;
                mins = 0;
            }
            {
                sprintf(time, str_time_fmt, mins, secs);
                if (GGame == 7)
                {
                    if (mins != 0)
                        goto dt_draw_ot;
                    if (secs >= 4)
                        goto dt_draw_ot;
                    if (static_cast<unsigned int>(Xblanks) % 30u < 15u)
                        return;
                dt_draw_ot:
                    timer->mPanelData.mTimerFont.draw(static_cast<int>(xpos),
                                                      static_cast<int>(ypos),
                                                      time, 0);
                    return;
                }
                if (mins != 0)
                    goto dt_draw;
                if (secs >= 11)
                    goto dt_draw;
                if (static_cast<unsigned int>(Xblanks) % 60u < 30u)
                    goto dt_skip;
            dt_draw:
                timer->mPanelData.mTimerFont.draw(static_cast<int>(xpos),
                                                  static_cast<int>(ypos),
                                                  time, 0);
            dt_skip:
                if (timer->mIndex == 0)
                {
                    if (static_cast<unsigned int>(ticks - 1) >= 599u)
                    {
                        Urgent = 600;
                    }
                    else if (ticks < Urgent)
                    {
                        SFX_PlayX(0x16, 0x2000, 0);
                        sub = Urgent / 10;
                        if (sub < 10)
                            sub = 10;
                        Urgent -= sub;
                    }
                }
            }
        }
    }
}

/**
 * Selects the message offset for one viewport layout.
 *
 * Address:   0x80065944
 * Mangled:   Panel_GetViewportMessOffset__FiPsT1
 * Demangled: Panel_GetViewportMessOffset(int, short *, short *)
 *
 * @param viewportMode  Combined viewport layout and player index.
 * @param pOffsetX  OffsetX to operate on.
 * @param pOffsetY  OffsetY to operate on.
 */
void Panel_GetViewportMessOffset(int viewportMode, short *pOffsetX, short *pOffsetY)
{
    switch (viewportMode)
    {
    case 0:
    case 1:
        *pOffsetX = 0;
        *pOffsetY = 0;
        break;
    case 4:
        *pOffsetX = 0;
        *pOffsetY = 60;
        break;
    case 6:
        *pOffsetX = 128;
        *pOffsetY = 60;
        break;
    case 2:
        *pOffsetX = 128;
        *pOffsetY = 0;
        break;
    case 5:
        *pOffsetX = 0;
        *pOffsetY = -60;
        break;
    case 7:
        *pOffsetX = -128;
        *pOffsetY = -60;
        break;
    case 3:
        *pOffsetX = -128;
        *pOffsetY = 0;
        break;
    }
}

// ====== 0x800659CC DisplayOutOfBounds__FP6CBruceUsUs ===========================
// DisplayOutOfBounds__FP6CBruceUsUs
// Address: 0x800659CC
// Demangled: DisplayOutOfBounds(CBruce *, unsigned short, unsigned short)
void DisplayOutOfBounds(CBruce *pBruce, unsigned short xpos, unsigned short ypos)
{
    PanelBruceScoreView *player = PanelBruce(pBruce);
    if (GameFrozen == 0 && GamePaused == 0 && player->mPanelData.mOutOfBounds != 0)
    {
        short offsetX;
        short offsetY;
        Panel_GetViewportMessOffset(
            ViewportMode * 2 + player->mPanelData.mPlayerNum,
            &offsetX, &offsetY);
        char *pString;
        switch (player->mPanelData.mOutOfBounds)
        {
        case 1:
            pString = GameText.pOutOfBounds1;
            break;
        case 2:
            pString = GameText.pOutOfBounds2;
            break;
        case 3:
            pString = GameText.pOutOfBounds3;
            break;
        default:
            pString = GameText.pOutOfBounds4;
            break;
        }
        player->mPanelData.mOutOfBoundsText.update();
        player->mPanelData.mOutOfBoundsText.bind(
            &player->mPanelData.mTrickFont);
        player->mPanelData.mTrickFont.Scale = 0x1000;
        player->mPanelData.mTrickFont.draw(
            static_cast<int>(xpos) + static_cast<int>(offsetX),
            static_cast<int>(ypos) + static_cast<int>(offsetY), pString, 0);
        player->mPanelData.mOutOfBoundsText.TextEffect.unbind(
            &player->mPanelData.mTrickFont);
        if (player->mPanelData.mOutOfBoundsText.handleComplete() != 0)
        {
            player->mPanelData.mOutOfBounds = 0;
        }
    }
}

// ====== 0x80065B3C Panel_ReplayInstructions__Fv ================================
// Panel_ReplayInstructions__Fv
// Address: 0x80065B3C
// Demangled: Panel_ReplayInstructions(void)
// Displays replay instruction text when conditions are met
void Panel_ReplayInstructions(void)
{
    SVideoInfo *info = &GVInfo1;
    if (info->VideoMode == 2 && DemoTimer != 0)
    {
        int status = Status;
        if (status == 0x14)
            return;
        if (status == 0x16)
            return;
        if (status == 0x15)
            return;
        int phase = info->ReplayHighlight;
        if (phase != 0)
            goto check_phase;
        if (static_cast<unsigned int>(info->NumFrames - info->FramesLeft) <
            90u)
            goto draw;
    check_phase:
        if (phase != 1)
            return;
    draw:
        Mess_SetRGB(0x80, 0x80, 0x80, 0);
        Mess_SetScale(0xbc);
        Mess_SetTextJustify(0);
        int width = 256;
        int y = 220;
        int width_last = 256;
        Mess_DrawPanelText(width, y, GameText.pReplayInstructions,
                           0, 0, width_last);
    }
}

/**
 * TQ_SpinDisplay__10CPanelDatai
 *
 * Address:   0x80065C1C
 * Mangled:   TQ_SpinDisplay__10CPanelDatai
 * Demangled: CPanelData::TQ_SpinDisplay(int)
 * Finds the first trick in the current spin run and displays a multiplier only
 * when the supplied trick begins that run.
 *
 * @param i  Trick-queue index to examine.
 * @return The run's spin multiplier, or zero when this entry does not begin it.
 */
int CPanelData::TQ_SpinDisplay(int i)
{
    int result;
    if (mTQ[i].locked == 2)
    {
        result = 0;
    }
    else
    {
        int first = i;
        while (first > 0 && mTQ[first - 1].locked != 1)
            --first;

        result = first == i ? TQ_SpinMultiplier(first) : 0;
    }
    return result;
}

/**
 * TQ_SpinMultiplier__10CPanelDatai
 *
 * Address:   0x80065CB4
 * Mangled:   TQ_SpinMultiplier__10CPanelDatai
 * Demangled: CPanelData::TQ_SpinMultiplier(int)
 *
 * Totals spin values from the first trick in a run through its locked endpoint.
 *
 * @param i  Any trick-queue index within the run.
 * @return The accumulated spin multiplier for the run.
 */
int CPanelData::TQ_SpinMultiplier(int i)
{
    int first = i;
    while (first > 0 && mTQ[first - 1].locked != 1)
        --first;

    int spins = 0;
    while (first < mTQStack && mTQ[first].locked != 1)
    {
        spins += mTQ[first].spins;
        ++first;
    }

    if (first < mTQStack)
        spins += mTQ[first].spins;

    return spins;
}

/**
 * TotalTrickScore__10CPanelData
 *
 * Address:   0x80065D80
 * Mangled:   TotalTrickScore__10CPanelData
 * Demangled: CPanelData::TotalTrickScore(void)
 *            Applies repeat depreciation and spin multipliers to every queued
 *            trick, then applies the active perfect/sloppy score modifier.
 *
 * @return The adjusted score for the current trick queue.
 */
int CPanelData::TotalTrickScore()
{
    int score;
    {
        int i = 0;
        score = 0;

        if (mTQStack > 0)
        {
            do
            {
                int count = mTQ[i].count;
                if (mTQ[i].depreciate == 0)
                    count = 1;
                --count;
                if (count > 4)
                    count = 4;

                int trickScore = mTQ[i].point_value;
                int depreciation = Depreciation_Values[count];
                int updatedScore;
                if (TQ_SpinMultiplier(i) < 6)
                {
                    updatedScore = score +
                                   ((trickScore *
                                     (Spin_Mult_Values[TQ_SpinMultiplier(i)] + 2)) / 2) *
                                       depreciation / 100;
                }
                else
                {
                    updatedScore = score +
                                   ((trickScore *
                                     (TQ_SpinMultiplier(i) * 4 +
                                      Spin_Mult_Values[5] - 0x12)) / 2) *
                                       depreciation / 100;
                }
                score = updatedScore;

                ++i;
            } while (i < mTQStack);
        }
    }

    PanelBruceScoreView *bruce = PanelBruce(mpBruce);
    if (bruce->mPerfectLanding != 0)
    {
        score = score * 150 / 100;
    }
    if (bruce->mSloppyLanding != 0)
    {
        score = score * 75 / 100;
    }
    return score;
}

/**
 * Calculates the current trick-chain score including its combo multiplier.
 *
 * Address:   0x80065FB4
 * Mangled:   TotalScore__10CPanelData
 * Demangled: CPanelData::TotalScore(void)
 *
 * @return result
 */
int CPanelData::TotalScore()
{
    int base = TotalTrickScore();
    int count = mTQStack;
    int result;
    if (count < 15)
    {
        result = base * Num_Mult_Values[count] / 2;
    }
    else
    {
        int extra = count - 14;
        result = base * (Num_Mult_Values[14] + extra) / 2;
    }
    return result;
}

/**
 * DisplayTrick__10CPanelDataP6CBruceUsUs
 *
 * Address:   0x80066040
 * Mangled:   DisplayTrick__10CPanelDataP6CBruceUsUs
 * Demangled: CPanelData::DisplayTrick(CBruce *, unsigned short, unsigned short)
 *
 * @param pBruce  Active skater (CBruce) being processed.
 * @param xpos  X coordinate.
 * @param ypos  Y coordinate.
 */
void CPanelData::DisplayTrick(CBruce *pBruce, unsigned short xpos,
                              unsigned short ypos)
{
    if ((GVInfo1.VideoMode ^ 2) == 0)
        return;
    if (PanelCareer(&GCareer)->ScoreDisplayEnabled == 0)
        return;
    mTrickFont.setRGB(0x80, 0x80, 0x80);
    GraffitiColor(pBruce);
    mTrickFont.JustX = 1;
    if (mpTextEffect == &mTrickTextEffect.TextEffect)
    {
        mTrickTextEffect.bind(&mTrickFont);
    }
    else
    {
        if (mpTextEffect != &mFallingTextEffect.TextEffect)
            return;
        mFallingTextEffect.bind(&mTrickFont);
    }
    // Draw line 3 if non-empty
    if (mTrickName3[0] != '\0')
    {
        mTrickFont.draw(xpos, ypos, mTrickName3, 0);
        ypos -= mTrickFont.height(mTrickName3) + 5;
    }
    // Draw line 2 if non-empty
    if (mTrickName2[0] != '\0')
    {
        mTrickFont.draw(xpos, ypos, mTrickName2, 0);
        ypos -= mTrickFont.height(mTrickName2) + 5;
    }
    // Main trick text line
    mFallingTextEffect.drawBloodyFrame(
        xpos, ypos, mTrickFont.width(mTrickName));
    mTrickFont.draw(
        xpos, ypos, PanelBruce(pBruce)->mPanelData.mTrickName, 0);
    // Virtual call: activeEffect->update(font)
    TextEffect *effect = mpTextEffect;
    PanelTextEffectDispatch(effect)->Unbind(effect, &mTrickFont);
    // Check completion
    effect = mpTextEffect;
    if (effect == &mTrickTextEffect.TextEffect)
    {
        int done = PanelTrickEffect(effect)->handleComplete();
        if (done != 0)
        {
            mFallingTextEffect.init();
            mBailFlag = 0;
            mFallingTextEffect.setScale(mTrickFont.Scale);
            mpTextEffect = &mFallingTextEffect.TextEffect;
        }
    }
    else if (effect == &mFallingTextEffect.TextEffect)
    {
        int done = PanelFallingEffect(effect)->handleComplete();
        if (done != 0)
        {
            mpTextEffect = 0;
        }
    }
}

/**
 * Panel_Display__Fv
 *
 * Address:   0x80066268
 * Mangled:   Panel_Display__Fv
 * Demangled: Panel_Display(void)
 *            Draws the HUD for every active skater using the current viewport
 *            layout, with replay, pause, score, timer, and divider handling.
 */
void Panel_Display(void)
{
    if (Cheat_PanelOn != 0)
    {
        CBruce *pBruce = PanelBruceObject(MechList);
        Panel_ReplayInstructions();
        for (; pBruce != 0; pBruce = PanelNextBruce(pBruce))
        {
        PanelBruce(pBruce)->mPanelData.mFallingTextEffect.update();
        PanelBruce(pBruce)->mPanelData.mTrickTextEffect.update();
        DisplayOutOfBounds(pBruce, 256, 90);
        DisplayTapesSkate();
        if (GamePaused != 0)
        {
            SKATE_Display = 30;
            TAPES_Display = 30;
            return;
        }
        {
            int vm = ViewportMode;
            unsigned short playerNum = PanelBruce(pBruce)->mPanelData.mPlayerNum;
            unsigned int vpIdx = vm * 2 + playerNum;
            switch (vpIdx)
            {
            case 0:
            case 1:
                PanelBruce(pBruce)->mPanelData.mTrickFont.SpaceX = 1;
                TrickTips_Display();
                DisplaySwitchness(pBruce, 430, 30);
                DisplayHolding(pBruce, 256, 224);
                PanelBruce(pBruce)->mPanelData.DisplayTrick(pBruce, 256, 205);
                DisplayScore(pBruce, 25, 45);
                DisplayTime(pBruce, 256, 48);
                break;
            case 2:
                PanelBruce(pBruce)->mPanelData.mTrickFont.SpaceX = 0;
                DisplaySwitchness(pBruce, 443, 210);
                DisplayHolding(pBruce, 384, 224);
                PanelBruce(pBruce)->mPanelData.DisplayTrick(pBruce, 384, 205);
                DisplayScore(pBruce, 358, 45);
                break;
            case 4:
                PanelBruce(pBruce)->mPanelData.mTrickFont.SpaceX = 1;
                DisplaySwitchness(pBruce, 443, 210);
                PanelBruce(pBruce)->mPanelData.DisplayTrick(pBruce, 256, 205);
                DisplayHolding(pBruce, 256, 224);
                DisplayScore(pBruce, 25, 145);
                DisplayTime(pBruce, 452, 145);
                break;
            case 6:
                PanelBruce(pBruce)->mPanelData.mTrickFont.SpaceX = 0;
                DisplaySwitchness(pBruce, 55, 150);
                DisplayHolding(pBruce, 128, 234);
                PanelBruce(pBruce)->mPanelData.DisplayTrick(pBruce, 128, 215);
                DisplayScore(pBruce, 112, 150);
                DisplayTime(pBruce, 55, 145);
                Panel_DrawFlatShadedPoly(256, 0, 256, 120, 30, 30, 30, 1);
                break;
            case 3:
                PanelBruce(pBruce)->mPanelData.mTrickFont.SpaceX = 0;
                DisplaySwitchness(pBruce, 25, 210);
                DisplayHolding(pBruce, 128, 224);
                PanelBruce(pBruce)->mPanelData.DisplayTrick(pBruce, 128, 205);
                DisplayScore(pBruce, 25, 45);
                DisplayTime(pBruce, 256, 48);
                break;
            case 5:
                PanelBruce(pBruce)->mPanelData.mTrickFont.SpaceX = 1;
                DisplaySwitchness(pBruce, 443, 95);
                PanelBruce(pBruce)->mPanelData.DisplayTrick(pBruce, 256, 85);
                DisplayHolding(pBruce, 256, 104);
                DisplayScore(pBruce, 25, 35);
                DisplayTime(pBruce, 452, 35);
                break;
            case 7:
                PanelBruce(pBruce)->mPanelData.mTrickFont.SpaceX = 0;
                DisplaySwitchness(pBruce, 457, 35);
                DisplayHolding(pBruce, 384, 119);
                PanelBruce(pBruce)->mPanelData.DisplayTrick(pBruce, 384, 98);
                DisplayScore(pBruce, 276, 35);
                DisplayTime(pBruce, 457, 30);
                Panel_DrawFlatShadedPoly(0, 120, 257, 2, 30, 30, 30, 1);
                Panel_DrawFlatShadedPoly(0, 120, 257, 120, 20, 20, 20, 1);
                Panel_DrawFlatShadedPoly(256, 119, 512, 2, 30, 30, 30, 1);
                break;
            case 8:
            case 9:
                break;
            }
        }
        }
    }
}

/**
 * Panel_PlayerInit__FUsP6CBruce
 *
 * Address:   0x8006665C
 * Mangled:   Panel_PlayerInit__FUsP6CBruce
 * Demangled: Panel_PlayerInit(unsigned short, CBruce *)
 *            Assigns the panel slot from the player's position in MechList.
 *
 * @param PlayerNum  Original player-number argument; slot selection uses list order.
 * @param pPlayer  Player to operate on.
 */
void Panel_PlayerInit(unsigned short PlayerNum, CBruce *pPlayer)
{
    if (PanelBruce(pPlayer) == PanelBruce(MechList))
    {
        PanelBruce(pPlayer)->mPanelData.mPlayerNum = 0;
    }
    else
    {
        PanelBruce(pPlayer)->mPanelData.mPlayerNum = 1;
    }
}

/**
 * Panel_TakeOff__FP6CBruce
 *
 * Address:   0x80066680
 * Mangled:   Panel_TakeOff__FP6CBruce
 * Demangled: Panel_TakeOff(CBruce *)
 *            Clears the displayed spin count and starts highlight capture.
 *
 * @param pPlayer  Player to operate on.
 */
void Panel_TakeOff(CBruce *pPlayer)
{
    PanelBruce(pPlayer)->mPanelData.old_180spins = 0;
    Video_HighlightStart(pPlayer);
}

/**
 * Panel_HoldTrick__FP6CBrucei
 *
 * Address:   0x800666A0
 * Mangled:   Panel_HoldTrick__FP6CBrucei
 * Demangled: Panel_HoldTrick(CBruce *, int)
 *            Adds points to the active trick and refreshes its message.
 *
 * @param pPlayer  Player to operate on.
 * @param Points  Points to add to the most recent trick.
 */
void Panel_HoldTrick(CBruce *pPlayer, int Points)
{
    PanelBruceScoreView *player = PanelBruce(pPlayer);
    if (player->mPanelData.mTQStack > 0)
    {
        player->mPanelData.mTQ[player->mPanelData.mTQStack - 1].point_value +=
            Points;
        player->mPanelData.TrickMessageRefresh();
    }
}

/**
 * Panel_Land__FP6CBruce
 *
 * Address:   0x800666F4
 * Mangled:   Panel_Land__FP6CBruce
 * Demangled: Panel_Land(CBruce *)
 *            Forwards the landing to the player's panel state.
 *
 * @param pPlayer  Player to operate on.
 */
void Panel_Land(CBruce *pPlayer)
{
    PanelBruce(pPlayer)->mPanelData.Land();
}

/**
 * Panel_Bail__FP6CBruce
 *
 * Address:   0x80066714
 * Mangled:   Panel_Bail__FP6CBruce
 * Demangled: Panel_Bail(CBruce *)
 *            Forwards the bail to the player's panel state.
 *
 * @param pPlayer  Player to operate on.
 */
void Panel_Bail(CBruce *pPlayer)
{
    PanelBruce(pPlayer)->mPanelData.Bail();
}

/**
 * Panel_Letter__Fi
 *
 * Address:   0x80066734
 * Mangled:   Panel_Letter__Fi
 * Demangled: Panel_Letter(int)
 *            Sets SKATE letter flag, checks if all 5 collected → calls Panel_Tape(2) and returns 1
 *
 * @param Letter  Zero-based S-K-A-T-E letter index.
 * @return One once all five letters are collected; otherwise zero.
 */
int Panel_Letter(int Letter)
{
    SKATE_Display = 40;
    if (static_cast<unsigned int>(Letter) < 5u)
    {
        PanelBruceScoreView *player = PanelBruce(MechList);
        player->mPanelData.SKATE[Letter] = reinterpret_cast<void *>(1);
    }
    PanelBruceScoreView *p = PanelBruce(MechList);
    if (p->mPanelData.SKATE[0] != reinterpret_cast<void *>(1))
        return 0;
    if (p->mPanelData.SKATE[1] != reinterpret_cast<void *>(1))
        return 0;
    if (p->mPanelData.SKATE[2] != reinterpret_cast<void *>(1))
        return 0;
    if (p->mPanelData.SKATE[3] != reinterpret_cast<void *>(1))
        return 0;
    if (p->mPanelData.SKATE[4] != reinterpret_cast<void *>(1))
        return 0;
    Panel_Tape(2);
    return 1;
}

// ====== 0x800667E4 Panel_Tape__Fi ==============================================
// Panel_Tape__Fi
// Address: 0x800667E4
// Demangled: Panel_Tape(int)
void Panel_Tape(int Tape)
{
    static char buf[32] __attribute__((section(".bss")));

    TAPES_Display = 40;
    Career_GiveTape(GLevel, Tape);
    switch (Tape)
    {
    case 2:
        Mess_Message(GameText.pSKATETape, Messprog_Goal, 1, 0, 0);
        return;
    case 0:
        SFX_PlayX(0x13, 0x2000, 0);
        Mess_Message(GameText.p20000Tape, Messprog_Goal, 1, 0, 0);
        return;
    case 4:
        SFX_PlayX(0x13, 0x2000, 0);
        Mess_Message(GameText.p40000Tape, Messprog_Goal, 1, 0, 0);
        return;
    case 1:
        SFX_PlayX(0x13, 0x2000, 0);
        sprintf(buf, GameText.p5GoalsTape,
                str_goalcounter_name);
        Mess_Message(buf, Messprog_Goal, 1, 0, 0);
        return;
    case 3:
        SFX_PlayX(0x9f, 0x2000, 0);
        Mess_Message(GameText.pHiddenTape, Messprog_Goal, 1, 0, 0);
        return;
    default:
        return;
    }
}

/**
 * Panel_GoalCounter__Fi
 *
 * Address:   0x80066920
 * Mangled:   Panel_GoalCounter__Fi
 * Demangled: Panel_GoalCounter(int)
 *            Increments goal counter; when all 5 goals reached, calls Panel_Tape(1)
 *            Otherwise formats a message and displays it via Mess_Message
 *
 * @param N  Goal identifier retained by the original interface.
 */
void Panel_GoalCounter(int N)
{
    static char Msg[32] __attribute__((section(".bss")));

    if (GGame == 1)
    {
        PanelBruceScoreView *player = PanelBruce(MechList);
        ++player->mPanelData.Goals;
        if (player->mPanelData.Goals == 5)
        {
            player->mPanelData.Goals = 99;
            Panel_Tape(1);
        }
        else
        {
            sprintf(Msg, str_goalcounter_fmt,
                    player->mPanelData.Goals,
                    str_goalcounter_name);
            Mess_Message(Msg, Messprog_Zoom, 1, 0, 0);
        }
    }
}

/**
 * Set__6CTrickP7MessagePci
 *
 * Address:   0x800669C0
 * Mangled:   Set__6CTrickP7MessagePci
 * Demangled: CTrick::Set(Message *, char *, int)
 *            Stores message pointer, string, count, fixed timer=0x12, clears fields
 *
 * @param m  Message associated with the trick.
 * @param n  Trick display name.
 * @param p  Base point value.
 */
void CTrick::Set(Message *m, char *n, int p)
{
    message = m;
    name = n;
    point_value = p;
    countdown = 18;
    spins = 0;
    locked = 0;
}

/**
 * Construct one gameplay panel and its owned fonts and text effects.
 *
 * Address: 0x800669E0
 * Mangled: __10CPanelDataP6CBruce
 * Demangled: CPanelData::CPanelData(CBruce *)
 *
 * @param pBruce Skater whose score and trick state the panel presents.
 */
CPanelData *__10CPanelDataP6CBruce(CPanelData *self,
                                   CBruce *pBruce)
{
    self->mpBruce = pBruce;
    self->mTotalScore = 0;
    self->mFallingEnabled = 0;
    self->mOutOfBounds = 0;
    ::new (&self->mTimerFont) Font();
    ::new (&self->mTrickFont) Font();
    ::new (&self->mScoreFont) Font();
    ::new (&self->mFallingTextEffect) FallingTextEffect();
    TrickTextEffect_ConstructAbi(&self->mTrickTextEffect);
    ::new (&self->mOutOfBoundsText) FallingTextEffect();

    int i = 0;
    self->mTQStack = 0;
    self->prev_xblanks = 0;
    self->mHoldingTankFlashTimer = 0;
    do
    {
        self->SKATE[i++] = 0;
    } while (i < 5);

        printf(str_cpanel_print);
    self->Goals = 0;

    *reinterpret_cast<PanelAssignableFont *>(&self->mTimerFont) =
        *reinterpret_cast<PanelAssignableFont *>(
            FontManager::GetFont(str_panel_font1));
    self->mTimerFont.JustX = 1;
    self->mTimerFont.JustY = 3;
    self->mTimerFont.setRGB(0x80, 0x30, 0x30);
    self->mTimerFont.Shadow = reinterpret_cast<void *>(1);
    self->mTimerFont.SpaceX = 4;
    self->mTimerFont.SpaceY = 0;
    self->mTimerFont.Scale = 0x1000;
    self->mTimerFont.CharMap = 1;
    self->mTimerFont.FixedWidth = 1;

    *reinterpret_cast<PanelAssignableFont *>(&self->mScoreFont) =
        *reinterpret_cast<PanelAssignableFont *>(
            FontManager::GetFont(str_panel_font2));
    self->mScoreFont.JustX = 2;
    self->mScoreFont.JustY = 3;
    self->mScoreFont.setRGB(0x80, 0x80, 0x80);
    self->mScoreFont.Shadow = reinterpret_cast<void *>(1);
    self->mScoreFont.SpaceX = 4;
    self->mScoreFont.SpaceY = 0;
    self->mScoreFont.Scale = 0x1000;
    self->mScoreFont.CharMap = 1;
    self->mScoreFont.ShadowDx = GNumberOfPlayers == 1 ? 2 : 1;
    self->mScoreFont.ShadowDy = GNumberOfPlayers == 1 ? 2 : 1;

    *reinterpret_cast<PanelAssignableFont *>(&self->mTrickFont) =
        *reinterpret_cast<PanelAssignableFont *>(
            FontManager::GetFont(str_panel_font3));
    self->mTrickFont.JustX = 1;
    self->mTrickFont.JustY = 3;
    self->mTrickFont.setRGB(0x80, 0x80, 0x80);
    self->mTrickFont.Shadow = reinterpret_cast<void *>(1);
    self->mTrickFont.SpaceX = 0;
    self->mTrickFont.SpaceY = 0;
    self->mTrickFont.Scale = 0x1000;
    self->mTrickFont.ShadowDx = GNumberOfPlayers == 1 ? 2 : 1;
    self->mTrickFont.ShadowDy = GNumberOfPlayers == 1 ? 2 : 1;
    self->mpTextEffect = 0;
    return self;
}

/**
 * Rebuilds the three-line trick-chain message from the queued tricks.
 *
 * Address: 0x80066CA8
 * Mangled: TrickMessageRefresh__10CPanelData
 * Demangled: CPanelData::TrickMessageRefresh(void)
 *
 */
void CPanelData::TrickMessageRefresh()
{
    char *p = this->mTrickName;
    char tbuf[96];
    int lineWidth;
    int steadyScale;
    int currLine;

    currLine = 0;

    this->mTrickName2[0] = 0;
    this->mTrickName3[0] = 0;

    int viewport = ViewportMode * 2 +
                   reinterpret_cast<PanelBruceScoreView *>(this->mpBruce)
                       ->mPanelData.mPlayerNum;
    if (viewport < 2)
    {
        goto width_480;
    }

    lineWidth = 240;
    if (viewport < 4)
        goto width_done;
    if (viewport >= 8)
        goto width_480;

    lineWidth = 480;
    if (viewport < 6)
        goto width_done;
    lineWidth = 240;
    goto width_done;

width_480:
    lineWidth = 480;
width_done:
    steadyScale = 0xbc0;
    if (GNumberOfPlayers >= 2)
        steadyScale = 0x940;

    if (this->mTQStack == 0)
    {
        this->mTrickName[0] = 0;
        return;
    }

    int last = this->mTQStack - 1;
    if (this->mTQ[last].locked == 0)
    {
        if (this->mTQ[last].spins !=
            reinterpret_cast<PanelBruceScoreView *>(this->mpBruce)
                ->mNum180Spins)
        {
            this->mTQ[last].spins =
                reinterpret_cast<PanelBruceScoreView *>(this->mpBruce)
                    ->mNum180Spins;
        }
    }

    if (reinterpret_cast<PanelBruceScoreView *>(this->mpBruce)
            ->mPerfectLanding != 0)
    {
        if (this->TQ_SpinDisplay(0) != 0)
        {
            sprintf(p, str_tmr_perfect_spin,
                    this->TQ_SpinDisplay(0) * 180,
                    this->mTQ[0].name);
        }
        else
        {
            sprintf(p, str_tmr_perfect, this->mTQ[0].name);
        }
    }
    else if (reinterpret_cast<PanelBruceScoreView *>(this->mpBruce)
                 ->mSloppyLanding != 0)
    {
        if (this->TQ_SpinDisplay(0) != 0)
        {
            sprintf(p, str_tmr_sloppy_spin,
                    this->TQ_SpinDisplay(0) * 180,
                    this->mTQ[0].name);
        }
        else
        {
            sprintf(p, str_tmr_sloppy, this->mTQ[0].name);
        }
    }
    else
    {
        if (this->TQ_SpinDisplay(0) != 0)
        {
            sprintf(p, str_tmr_spin,
                    this->TQ_SpinDisplay(0) * 180,
                    this->mTQ[0].name);
        }
        else
        {
            sprintf(p, str_tmr_name, this->mTQ[0].name);
        }
    }

    {
        int i = 1;
        this->mTrickFont.Scale = steadyScale;
        if (i < this->mTQStack)
        {
            do
            {
                if (this->TQ_SpinDisplay(i) != 0)
                {
                    sprintf(tbuf, str_tmr_cont_spin, p,
                            this->TQ_SpinDisplay(i) * 180,
                            this->mTQ[i].name);
                }
                else
                {
                    sprintf(tbuf, str_tmr_cont, p,
                            this->mTQ[i].name);
                }

                if (lineWidth < this->mTrickFont.width(tbuf))
                {
                    ++currLine;
                    if (currLine == 1)
                        p = this->mTrickName2;
                    else if (currLine == 2)
                        p = this->mTrickName3;

                    if (currLine == 3)
                    {
                        sprintf(p, str_tmr_overflow, p);
                        return;
                    }

                    if (this->TQ_SpinDisplay(i) != 0)
                    {
                        sprintf(p, str_tmr_break_spin,
                                this->TQ_SpinDisplay(i) * 180,
                                this->mTQ[i].name);
                    }
                    else
                    {
                        sprintf(p, str_tmr_break,
                                this->mTQ[i].name);
                    }
                }
                else
                {
                    sprintf(p, tbuf);
                }

                ++i;
            } while (i < this->mTQStack);
        }
    }
}

/**
 * TrickMessage__10CPanelData
 *
 * Address:   0x80067050
 * Mangled:   TrickMessage__10CPanelData
 * Demangled: CPanelData::TrickMessage(void)
 *            Enables falling text and activates the trick effect when needed.
 *
 */
void CPanelData::TrickMessage()
{
    mFallingEnabled = 1;
    mFallingTextEffect.TextEffect.disable();
    if (mTQStack != 0)
    {
        mTrickTextEffect.init();
        mpTextEffect = &mTrickTextEffect.TextEffect;
        TrickMessageRefresh();
    }
}

/**
 * Trick__10CPanelDataPciii
 *
 * Address:   0x800670AC
 * Mangled:   Trick__10CPanelDataPciii
 * Demangled: CPanelData::Trick(char *, int, int, int)
 *            Queues a trick, captures spin and stance history, and refreshes
 *            the on-screen trick message.
 *
 * @param pName  Display name for the queued trick.
 * @param Points  Base point value for the trick.
 * @param Lock  Lock state recorded with the queue entry.
 * @param Dep  Whether repeat-score depreciation applies.
 */
void CPanelData::Trick(char *pName, int Points, int Lock, int Dep)
{
    int Flipped;

    if (Cheat_BigScore != 0)
    {
        Points = Points * 10;
    }
    int count = mTQStack;
    if (count == 0)
    {
        int oldTotal = mTotalScore;
        int landed = mLandedTotal;
        mLandedTotal = 0;
        mLandedMultiplier = 0;
        mHoldingTankFlashTimer = 0;
        mTotalScore = oldTotal + landed;
    }
    mTQ[count].Set(0, pName, Points);
    count = mTQStack;
    if (count <= 0)
    {
        goto check_lock;
    }
    {
        int prevIdx = count - 1;
        int prevState = mTQ[prevIdx].locked;
        if (prevState != 0)
        {
            goto check_lock;
        }
        // Previous trick has state 0 - get 180s
        CBruce *bruce = mpBruce;
        int halfTurns = bruce->Get180s();
        // Store halfTurns in the previous entry's spin slot
        int cnt2 = mTQStack;
        mTQ[cnt2 - 1].spins = halfTurns;
        CBruce *bruce2 = mpBruce;
        PanelBruce(bruce2)->mNum180Spins = 0;
        goto after_lock;
    }
check_lock:
{
    if (Lock == 2)
        goto skip_lock;
    if (Lock == 1)
    {
        CBruce *bruce = mpBruce;
        int halfTurns = bruce->Get180s();
        CBruce *bruce2 = mpBruce;
        PanelBruce(bruce2)->mNum180Spins = halfTurns;
        count = mTQStack;
    }
    {
        CBruce *bruce3 = mpBruce;
        int val = PanelBruce(bruce3)->mNum180Spins;
        mTQ[count].spins = val;
    }
after_lock:
    count = mTQStack;
}
skip_lock:
{
    // Switchness check
    PanelBruceScoreView *bruce = PanelBruce(mpBruce);
    unsigned int flags = bruce->mExtraFlags;
    int skaterType = bruce->mSkaterType;
    Flipped = (flags >> 1) & 1;
    // Switched-stance value in this skater's Stats entry.
    SStats *stats = Stats;
    int isZero = stats[skaterType].Ability[11] < 1;
    if (Flipped != isZero)
    {
        mTQ[count].switched = 1;
    }
    else
    {
        mTQ[count].switched = 0;
    }
    // History_Add
    int cnt3 = mTQStack;
    CBruce *bruce4 = mpBruce;
    char *name = mTQ[cnt3].name;
    int bc = mTQ[cnt3].spins;
    int sc = mTQ[cnt3].switched;
    int histVal = bruce4->History_Add(name, bc * 2 + sc);
    // Store results
    int cnt4 = mTQStack;
    mTQ[cnt4].count = histVal;
    int cnt5 = mTQStack;
    mTQ[cnt5].locked = Lock;
    int cnt6 = mTQStack;
    mTQ[cnt6].depreciate = Dep;
    int cnt7 = mTQStack;
    if (cnt7 < 19)
    {
        mTQStack = cnt7 + 1;
    }
    TrickMessage();
}
}

/**
 * Land__10CPanelData
 *
 * Address:   0x8006733C
 * Mangled:   Land__10CPanelData
 * Demangled: CPanelData::Land(void)
 *            Processes trick landing: accumulate score, update best trick, timer, etc.
 *
 */
void CPanelData::Land()
{
    mFallingEnabled = 0;
    if (mTQStack > 0)
    {
        mNumTricksLanded =
            mNumTricksLanded + static_cast<unsigned short>(mTQStack);
        mLandedScore = TotalScore();
        mLandedTotal += mLandedScore;
        if (mBestComboScore < mLandedScore)
        {
            mBestComboScore = mLandedScore;
            mNumTricksInCombo = static_cast<unsigned short>(mTQStack);
            strcpy(mBestComboName, mTrickName);
            strcpy(mBestComboName2, mTrickName2);
            strcpy(mBestComboName3, mTrickName3);
        }
        mTQStack = 0;
        mHoldingTankFlashTimer = 40;
        if (PanelCareer(&GCareer)->ScoreDisplayEnabled == 0)
        {
            mHoldingTankFlashTimer = 0;
        }

        if (GGame == 2 && mLandedScore != 0)
        {
            mTotalScore = 0;
        }
    }
    Video_HighlightEnd(mpBruce, mLandedTotal);
}

/**
 * Bail__10CPanelData
 *
 * Address:   0x80067430
 * Mangled:   Bail__10CPanelData
 * Demangled: CPanelData::Bail(void)
 *            Increments bail count, computes penalty, clears trick display
 *
 */
void CPanelData::Bail()
{
    ++mNumBails;
    Video_HighlightEnd(mpBruce, -TotalScore());
    if (mFallingEnabled != 0)
    {
        mFallingEnabled = 0;
        if (mTrickTextEffect.handleComplete() == 0)
        {
            mTrickTextEffect.Bail();
            mBailFlag = 1;
        }
    }
    mTQStack = 0;
}

/**
 * OutOfBounds__10CPanelData
 *
 * Address:   0x800674B0
 * Mangled:   OutOfBounds__10CPanelData
 * Demangled: CPanelData::OutOfBounds(void)
 *            Selects an out-of-bounds message and initializes its text effect.
 *
 */
void CPanelData::OutOfBounds()
{
    mOutOfBounds = Rnd(4) + 1;
    mOutOfBoundsText.init();
    mOutOfBoundsText.SetBloodiness(0);
}
