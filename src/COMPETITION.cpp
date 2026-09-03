// COMPETITION.cpp -- Reconstructed translation unit
// Original: COMPETITION.CPP
// Functions: 25 target-matched functions
// Address range: 0x800750CC - 0x800781E8
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#include "globals.h"
#include "CMENU.h"
#include "RFRONT.h"
#include "STATS.h"

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

struct SMedalMessage {
    char *Line1;
    char *Line2;
    char *Line3;
};

/** Stores the professional and consistency biases for one competition judge. */
struct CJudge {
    struct CClass CClass;
    unsigned char mProPlayer;
    unsigned char mProBias;
    unsigned char mConPlayer;
    unsigned char mConBias;
};

/** Owns the competition entrants, judge results, medal state, and marquee. */
struct CCompetition {
    struct CClass CClass;
    unsigned char mCompetitorList[10];
    unsigned int mJudgeTimer;
    unsigned int mLadderTimer;
    void *mWinLoseCheer;
    unsigned char mLastLadderEntry;
    void *mJudgeFullyDisplayed;
    void *mLadderFullyDisplayed;
    struct CJudge mJudges[5];
    short mJudgesScore[5];
    unsigned char mHighScore;
    unsigned char mLowScore;
    unsigned char mPlacing[10];
    unsigned char mOrdering[10];
    unsigned short mScore[10][3];
    unsigned short mTotalScore[10];
    unsigned char mCurrentHeat;
    int medal_mask;
    struct CPowerUp *mMedal1;
    struct CPowerUp *mMedal2;
    struct CPowerUp *mMedal3;
    char *mMarqueeText;
    char mMarqueeTextBuf[75];
    unsigned int mMarqueeTimer;
    int our_skater_num;
    void *mFading;
};

typedef char CJudge_size_must_be_0x08[(sizeof(CJudge) == 0x08) ? 1 : -1];
typedef char CCompetition_size_must_be_0x130[
    (sizeof(CCompetition) == 0x130) ? 1 : -1];

#include "GLEVEL.h"
#define FRONTFLOWSTATE_DECLARE_ROLL_CREDITS
#include "FRONTFLOWSTATE.h"
#include "LOCALTXT.h"
#define POLYBUFFERSIZE_ABSOLUTE_DECLARATION
#include "POLYBUFFERSIZE.h"

#include "GAMEFMV.h"

/** Strided view beginning at each level's competition flag. */
struct CompetitionLevelFlag {
    void *isCompetition;
    unsigned char _padToNext[0x74 - sizeof(void *)];
};

#include "SCHARACTERPROGRESS.h"

struct CompetitionCareerOptions {
    unsigned char _padToMedalMessagesUsed[0x148];
    int MedalMessagesUsed[14];
};

/** COMPETITION-owned career prefix through the medal-message flags. */
struct CompetitionCareerView {
    int Version;
    int MaxTapes;
    SCharacterProgress CharacterProgress[14];
    unsigned char _padToOptions[0x7D4 - 8 - 14 * 0x20];
    CompetitionCareerOptions Options;
};

typedef char CompetitionCareerView_size_must_be_0x954[
    (sizeof(CompetitionCareerView) == 0x954) ? 1 : -1];

struct CPowerUp;
typedef void (*CompetitionPowerUpDestructor)(CPowerUp *, int);
typedef void (*CompetitionPowerUpDisplay)(CPowerUp *);

/** Vtable entries invoked by the competition owner. */
struct CompetitionPowerUpVtable {
    void *Reserved0;
    void *Reserved1;
    CompetitionPowerUpDestructor Destructor;
    void *Reserved3;
    CompetitionPowerUpDisplay Display;
};

/** Competition-visible slice of a medal power-up. */
struct CPowerUp {
    unsigned char _padToPositionY[0x08];
    int mPositionY;
    unsigned char _padToVtable[0x48 - 0x0C];
    CompetitionPowerUpVtable *_vf;
    unsigned char _padToOrgPositionY[0x120 - 0x4C];
    int mOrgPositionY;
    unsigned char _padToMaxWobbleAmplitude[0x12A - 0x124];
    unsigned short mMaxWobbleAmplitude;
    unsigned short mWobbleAmplitude;
    unsigned char _padToTimer[0x134 - 0x12E];
    unsigned int mTimer;
};

/** Panel fields read by competition judge logic. */
struct CompetitionPanelData {
    struct CBruce *mpBruce;
    unsigned int mTotalScore;
    unsigned short mPlayerNum;
    unsigned short mNumTricksLanded;
    unsigned short mNumBails;
};

/** Player slice exposing only the panel and debug-proven skater type. */
struct CBruce {
    unsigned char _padToPanelData[0x190];
    CompetitionPanelData mPanelData;
    unsigned char _padToSkaterType[
        0xDA8 - 0x190 - sizeof(CompetitionPanelData)];
    int mSkaterType;
    unsigned char _opaqueTail[5228 - 0xDAC];
};

typedef char CompetitionLevelFlag_size_must_be_0x74[
    (sizeof(CompetitionLevelFlag) == 0x74) ? 1 : -1];
typedef char Competition_SCharacterProgress_size_must_be_0x20[
    (sizeof(SCharacterProgress) == 0x20) ? 1 : -1];
typedef char Competition_CBruce_size_must_be_5228[
    (sizeof(CBruce) == 5228) ? 1 : -1];

#include "MAIN.h"
#define DB_BUFFER_LAYOUT
#include "DB.h"
#undef DB_BUFFER_LAYOUT
#define DOUBLEBUFFER_DECLARE_KNOWN_BOUND
#include "DOUBLEBUFFER.h"
#undef DOUBLEBUFFER_DECLARE_KNOWN_BOUND
#include "GSKATER_STATE.h"
#include "YESNOMENU.h"

#include "MESS.h"
#include "FRONT.h"
#include "CROWD.h"
#include "PAD.h"

#include "UTILS.h"
#include "SFX.h"
#include "CAREER.h"
#include "FLASH.h"
#include "MEM.h"
#include <LIBCD.H>
#include <STDIO.H>

// === Forward type declarations ===

// Competition globals — non-$gp (accessed via lui/addiu absolute addressing)
// Base struct at 0x800C596C with various fields
#define COMPETITION_DECLARE_OWNER_STATE_VIEW
#include "COMPETITION.h"
#define COMPETITION_DECLARE_ADDRESS_VIEWS
#include "COMPETITION.h"


// === External function declarations ===
CPowerUp *Medal_Create(EMedalType type, int x, int y, int z, int size,
    int yVelocity);

// === Extern "C" forward declarations ===
/*
 * Keep the original call-site declaration opaque to this TU's optimizer.
 * The owner-local C-linkage identity below binds the same symbol without
 * exposing the three-instruction body early enough for GCC to inline it.
 */
/** Opaque external call view of the owner-local medal query. */
extern "C" int GotMedal__12CCompetition(CCompetition *competition);
namespace CompetitionOwnerFunctions
{
extern "C"
{
static int GotMedal__12CCompetition(CCompetition *);
}
}

extern "C" {
static void __DOT__12CCompetition(CCompetition *, int);
}
static void JudgeDestructorOwner(CJudge *, int)
    asm("_._6CJudge");
extern "C" {
static void __DOT__6CClass_800781E4(CClass *, int);
}

/*
 * Original COMPETITION.CPP small-data contribution. These owners precede the
 * functions which use them so ASPSX retains the original GP-relative forms.
 * Namespace-scoped C-linkage owners retain the target's natural identifiers
 * without publishing XDEFs. The incomplete declarations above preserve the
 * target compiler view at use sites.
 */
u16 HOLYCOW_PLACE = 0x03CF;
u16 FIRST_PLACE = 0x0398;
u16 SECOND_PLACE = 0x0366;
u16 THIRD_PLACE = 0x0334;
u16 LAST_PLACE = 0x0208;

namespace CompetitionAddressViewOwners
{
extern "C"
{
static char s__s__s___HEAT__d_800db92c[]
    __attribute__((section(".sdata"), aligned(4))) = "%s %s - HEAT %d";
static char s_You_lose__800db93c[]
    __attribute__((section(".sdata"), aligned(4))) = "You lose!";
static char s__s__1st_place_800db948[]
    __attribute__((section(".sdata"), aligned(4))) = "%s: 1st place";
static char s__s__2nd_place_800db958[]
    __attribute__((section(".sdata"), aligned(4))) = "%s: 2nd place";
static char s__s__3rd_place_800db968[]
    __attribute__((section(".sdata"), aligned(4))) = "%s: 3rd place";
static char s__dth_800db978[]
    __attribute__((section(".sdata"), aligned(4))) = "%dth";
static char s_Heat_800db980[]
    __attribute__((section(".sdata"), aligned(4))) = "        Heat";
static char s__s_800db990[]
    __attribute__((section(".sdata"), aligned(4))) = "%s";
static char s__d__d_800db994[]
    __attribute__((section(".sdata"), aligned(4))) = "%d.%d";
static char s___800db99c[]
    __attribute__((section(".sdata"), aligned(4))) = "-";
static char s_HEAT__d_OVER__800db9a0[]
    __attribute__((section(".sdata"), aligned(4))) = "HEAT %d OVER!";
static char s_SCOREBOARD_800db9b0[]
    __attribute__((section(".sdata"), aligned(4))) = "JUDGES SCORES";
static char s_JUDGE__d_800db9c0[]
    __attribute__((section(".sdata"), aligned(4))) = "JUDGE %d";
}
}

/** Small-data vtables emitted by the original static-object machinery. */
static int DAT_800db9d0
    __attribute__((section(".sdata"), aligned(16))) = 0;
static int CompetitionVtableReserved = 0;
static void (*CompetitionDestructorSlot)(CCompetition *, int)
    = __DOT__12CCompetition;
static int CompetitionVtableEnd = 0;

static int DAT_800db9e0 = 0;
static int JudgeVtableReserved = 0;
static void (*JudgeDestructorSlot)(CJudge *, int)
    = JudgeDestructorOwner;
static int JudgeVtableEnd = 0;

static int DAT_800d7ca0_plus_0x3d50 = 0;
static int BaseVtableReserved = 0;
static void (*BaseDestructorSlot)(CClass *, int)
    = __DOT__6CClass_800781E4;

// === Function implementations (address order) ===

/**
 * Competition_WonGame__Fv
 *
 * Address:   0x800750CC
 * Mangled:   Competition_WonGame__Fv
 * Demangled: Competition_WonGame(void)
 *            Returns 1 if the player has medals from all three competitions
 *
 * @return Whether the player has three competition medals.
 */
int Competition_WonGame(void)
{
    unsigned int medal_count = 0;
    int i = 0;
    CBruce *skater = GSkater[0];
    u8 *scores = (u8 *)&reinterpret_cast<CompetitionCareerView *>(&GCareer)->CharacterProgress[0].GoalMask[0];
    CompetitionLevelFlag *level =
        (CompetitionLevelFlag *)&Levels[0].isCompetition;
    do
    {
        if (level->isCompetition != 0)
        {
            int heat = skater->mSkaterType;
            // Direct indexing commutes the address operands in GCC 2.91.
            u8 val = *(u8 *)((int)(i + (heat << 5)) + (int)scores);
            if ((val & 0xE0) != 0)
            {
                medal_count++;
            }
        }
        i++;
        level++;
    } while (i < 8);
    return medal_count == 3;
}

/**
 * Competition_WonGameAllGold__Fv
 *
 * Address:   0x80075144
 * Mangled:   Competition_WonGameAllGold__Fv
 * Demangled: Competition_WonGameAllGold(void)
 *            Returns 1 if the player has golds from all three competitions
 *
 * @return Whether the player has three competition gold medals.
 */
int Competition_WonGameAllGold(void)
{
    unsigned int medal_count = 0;
    int i = 0;
    CBruce *skater = GSkater[0];
    u8 *scores = (u8 *)&reinterpret_cast<CompetitionCareerView *>(&GCareer)->CharacterProgress[0].GoalMask[0];
    CompetitionLevelFlag *level =
        (CompetitionLevelFlag *)&Levels[0].isCompetition;
    do
    {
        if (level->isCompetition != 0)
        {
            int heat = skater->mSkaterType;
            // Direct indexing commutes the address operands in GCC 2.91.
            u8 val = *(u8 *)((int)(i + (heat << 5)) + (int)scores);
            if ((val & 0x20) != 0)
            {
                medal_count++;
            }
        }
        i++;
        level++;
    } while (i < 8);
    return medal_count == 3;
}

/**
 * Competition_IsOver__Fv — Returns 1 if heat count >= 3
 *
 * Address:   0x800751BC, Size: 24 bytes (6 instructions)
 * Mangled:   Competition_IsOver__Fv
 * Demangled: Competition_IsOver(void)
 *            Assembly:
 *            lui $v1, 0x800c
 *            lbu $v0, 0x5a2c($v1)   ; load heat count
 *            nop
 *            sltiu $v0, $v0, 3      ; $v0 = (heat < 3)
 *            jr $ra
 *            xori $v0, $v0, 1       ; $v0 = !(heat < 3) = (heat >= 3)
 *
 * @return theCompetition.mCurrentHeat >= 3
 */
int Competition_IsOver(void)
{
    return theCompetition.mCurrentHeat >= 3;
}

/**
 * Initialize the competition entrants, judges, scores, and randomized order.
 *
 * Address:   0x800751D4
 * Mangled:   Competition_Init__Fv
 * Demangled: Competition_Init(void)
 */
void Competition_Init(void)
{
    CCompetition &competition = theCompetition;

    competition.mJudgeTimer = 0;
    competition.mLadderTimer = 0;
    competition.mWinLoseCheer = 0;
    competition.mFading = 0;
    Utils_InitialRand(Vblanks);

    {
        int i = 0;
        do
        {
            competition.mCompetitorList[i] = (unsigned char)i;
            i++;
        } while (i < 10);
    }

    competition.our_skater_num = SelectedSkater;
    if (SelectedSkater == 10)
    {
        competition.our_skater_num = Rnd(10);
        competition.mCompetitorList[competition.our_skater_num] = 10;
    }

    if (GLevel == 8)
    {
        HOLYCOW_PLACE = 0x3CF;
        FIRST_PLACE = 0x3A7;
        SECOND_PLACE = 0x38E;
        THIRD_PLACE = 0x35C;
        LAST_PLACE = 0x302;
    }
    else
    {
        HOLYCOW_PLACE = 0x3CF;
        FIRST_PLACE = 0x398;
        SECOND_PLACE = 0x366;
        THIRD_PLACE = 0x334;
        LAST_PLACE = 0x208;
    }

    {
        unsigned char i = 0;
        do
        {
            competition.mJudges[i].mProPlayer = (unsigned char)Rnd(10);
            competition.mJudges[i].mConPlayer = (unsigned char)Rnd(10);
            competition.mJudges[i].mProBias = (unsigned char)Rnd(30);
            competition.mJudges[i].mConBias = (unsigned char)Rnd(30);
            i++;
        } while (i < 5);
    }

    {
        unsigned char i = 0;
        do
        {
            unsigned char j = 0;
            do
            {
                competition.mScore[i][j] = 0;
                j++;
            } while (j < 3);

            competition.mTotalScore[i] = 0;
            competition.mOrdering[i] = i;
            i++;
        } while (i < 10);
    }

    competition.mCurrentHeat = 0;

    {
        int i = 0;
        do
        {
            competition.mPlacing[i] = 0xFF;
            i++;
        } while (i < 10);
    }

    {
        int i = 0;
        do
        {
            int j = Rnd(10 - i);
            int k = 0;
            int slot = -1;

            if (j >= 0)
            {
                do
                {
                    slot++;
                    if (competition.mPlacing[slot] == 0xFF)
                    {
                        k++;
                    }
                } while (k <= j);
            }

            competition.mPlacing[slot] = (unsigned char)i;
            i++;
        } while (i < 10);
    }

    {
        int i = 0;
        do
        {
            if (competition.mPlacing[i] == 9)
            {
                competition.mPlacing[i] =
                    competition.mPlacing[competition.our_skater_num];
            }
            i++;
        } while (i < 10);
    }
    competition.mPlacing[competition.our_skater_num] = 0xFF;
}

/**
 * Calculates the five judges' scores for the player's completed heat.
 *
 * Address:   0x80075470
 * Mangled:   Competition_JudgeLogic__Fv
 * Demangled: Competition_JudgeLogic(void)
 *
 */
void Competition_JudgeLogic(void)
{
    CCompetition &competition = theCompetition;

    if (competition.mJudgeTimer == 0)
    {
        CBruce *pSkater = (CBruce *)GSkater[0];
        unsigned int temp;
        unsigned int score;
        competition.mJudgeFullyDisplayed = 0;
        score = pSkater->mPanelData.mTotalScore;

        {
            int i = 0;
            do
            {
                if (score > Levels[GLevel].GoldScore * 2)
                {
                    temp = ((score - Levels[GLevel].GoldScore * 2) << 12) /
                        (Levels[GLevel].GoldScore * 3);
                    temp = ((1000 - HOLYCOW_PLACE) * temp) >> 12;
                    temp += HOLYCOW_PLACE;
                }
                else if (score > Levels[GLevel].GoldScore)
                {
                    temp = ((score - Levels[GLevel].GoldScore) << 12) /
                        Levels[GLevel].GoldScore;
                    temp = ((HOLYCOW_PLACE - FIRST_PLACE) * temp) >> 12;
                    temp += FIRST_PLACE;
                }
                else if (score > Levels[GLevel].SilverScore)
                {
                    temp = ((score - Levels[GLevel].SilverScore) << 12) /
                        (Levels[GLevel].GoldScore - Levels[GLevel].SilverScore);
                    temp = ((FIRST_PLACE - SECOND_PLACE) * temp) >> 12;
                    temp += SECOND_PLACE;
                }
                else if (score > Levels[GLevel].BronzeScore)
                {
                    temp = ((score - Levels[GLevel].BronzeScore) << 12) /
                        (Levels[GLevel].SilverScore - Levels[GLevel].BronzeScore);
                    temp = ((SECOND_PLACE - THIRD_PLACE) * temp) >> 12;
                    temp += THIRD_PLACE;
                }
                else
                {
                    temp = (score << 12) / Levels[GLevel].BronzeScore;
                    temp = ((THIRD_PLACE - 300) * temp) >> 12;
                    temp += 300;
                }

                competition.mJudgesScore[i] = (short)temp;
                temp = (unsigned int)pSkater->mPanelData.mNumBails *
                    (Rnd(15) + 10);
                if (temp > 300)
                    temp = 300;

                if (temp >= (unsigned int)competition.mJudgesScore[i])
                    competition.mJudgesScore[i] = 0;
                else
                    competition.mJudgesScore[i] =
                        (short)(competition.mJudgesScore[i] - temp);

                if (competition.mJudgesScore[i] > 8)
                    competition.mJudgesScore[i] -= (short)Rnd(8);

                if (competition.mJudges[i].mProPlayer ==
                    competition.our_skater_num)
                {
                    competition.mJudgesScore[i] +=
                        competition.mJudges[i].mProBias;
                }

                if (competition.mJudges[i].mConPlayer ==
                        competition.our_skater_num &&
                    competition.mJudgesScore[i] >
                        competition.mJudges[i].mConBias)
                {
                    competition.mJudgesScore[i] -=
                        competition.mJudges[i].mConBias;
                }

                if (competition.mJudgesScore[i] < 0)
                    competition.mJudgesScore[i] = 0;
                if (competition.mJudgesScore[i] > 999)
                    competition.mJudgesScore[i] = 999;
                if (score == 0)
                    competition.mJudgesScore[i] = 0;

                i++;
            } while (i < 5);
        }

        competition.mLowScore = 0;
        {
            int i = 0;
            do
            {
                if (competition.mJudgesScore[i] <
                    competition.mJudgesScore[competition.mLowScore])
                {
                    competition.mLowScore = (unsigned char)i;
                }
                i++;
            } while (i < 5);
        }

        if (competition.mLowScore == 0)
            competition.mHighScore = 1;
        else
            competition.mHighScore = 0;
        {
            int i = 0;
            do
            {
                if (i != competition.mLowScore &&
                    competition.mJudgesScore[competition.mHighScore] <
                        competition.mJudgesScore[i])
                {
                    competition.mHighScore = (unsigned char)i;
                }
                i++;
            } while (i < 5);
        }

        {
            int average_score = 0;
            int i = 0;
            do
            {
                if (i != competition.mLowScore &&
                    i != competition.mHighScore)
                {
                    average_score += competition.mJudgesScore[i];
                }
                i++;
            } while (i < 5);

            average_score = average_score / 3;
            competition.mScore[competition.our_skater_num]
                              [competition.mCurrentHeat] =
                (unsigned short)average_score;
        }
    }

    competition.mJudgeTimer++;
}

/**
 * Draws the five judge scores and reveals the averaged heat score once the
 * judge-display timer has completed.
 *
 * Address:   0x800758F0
 * Mangled:   Competition_JudgeDisplay__Fv
 * Demangled: Competition_JudgeDisplay(void)
 */
void Competition_JudgeDisplay(void)
{
    char text_buf[15];
    CCompetition &competition = theCompetition;

    Mess_DeleteAll();
    Mess_SetScale(0x100);
    Mess_SetTextJustify(0);
    Mess_SetRGB(0xb6, 0x84, 0x24, 0);
    sprintf(text_buf, s_HEAT__d_OVER__800db9a0,
        competition.mCurrentHeat + 1);
    Mess_DrawText(0x100, 0x28, text_buf, 0, 0, 0x100, 0);
    Mess_DrawText(
        0x100, 0x46, s_SCOREBOARD_800db9b0, 0, 0, 0x100, 0);
    Mess_SetRGB(0x7a, 0x7a, 0x7a, 0);
    Mess_SetTextJustify(1);

    int dimness = 0;
    if (competition.mJudgeTimer > 0x4B)
        dimness = (competition.mJudgeTimer - 0x4B) * 4;
    if (dimness > 100)
        dimness = 100;
    int i = 0;
    do {
        if (competition.mJudgeTimer == (unsigned int)(i * 15 + 1))
            SFX_PlayX(0x34, 0x2000, 0);
        if ((unsigned int)(i * 15) < competition.mJudgeTimer) {
            short sval = competition.mJudgesScore[i];
            short a = sval / 10;
            short b = sval % 10;
            sprintf(text_buf, s_JUDGE__d__d__d_800b393c,
                i + 1, a, b);
        } else {
            sprintf(text_buf, s_JUDGE__d_800db9c0, i + 1);
        }
        if (i == competition.mLowScore || i == competition.mHighScore)
        {
            unsigned char dimmed = (unsigned char)(0x7A - dimness);
            Mess_SetRGB(dimmed, dimmed, dimmed, 0);
        }
        else
        {
            Mess_SetRGB(0x7A, 0x7A, 0x7A, 0);
        }
        Mess_DrawText(
            0x96, i * 0x14 + 0x5A, text_buf, 0, 0, 0x100, 0);
        i++;
    } while (i < 5);

    if (dimness >= 100) {
        if (competition.mJudgeFullyDisplayed == 0)
            SFX_PlayX(0x33, 0x2000, 0);
        Mess_SetTextJustify(0);
        Mess_SetRGB(0xb6, 0x84, 0x24, 0);
        int average_score = 0;
        {
            int i = 0;
            do {
                if (i != competition.mLowScore &&
                    i != competition.mHighScore)
                    average_score += competition.mJudgesScore[i];
                i++;
            } while (i < 5);
        }
        average_score = average_score / 3;
        Mess_SetTextJustify(0);
        sprintf(text_buf, s_HEAT__d_SCORE___800b3950,
            competition.mCurrentHeat + 1);
        Mess_DrawText(0xc8, 0xc3, text_buf, 0, 0, 0x100, 0);
        sprintf(text_buf, s__d__d_800db994,
            average_score / 10, average_score % 10);
        Mess_SetScale(0x180);
        Mess_DrawText(0x14f, 0xc3, text_buf, 0, 0, 0x100, 0);
        competition.mJudgeFullyDisplayed = (void *)1;
    } else {
        competition.mJudgeFullyDisplayed = 0;
    }
    Mess_SetScale(0x100);
    Front_PrintCentredPrompt((const char *)GameText.pContinue);
}

/**
 * Computes each opponent's score for the current heat, including judge bias,
 * and advances the ladder-display timer.
 *
 * Address:   0x80075CD4
 * Mangled:   Competition_LadderLogic__Fv
 * Demangled: Competition_LadderLogic(void)
 */
void Competition_LadderLogic(void)
{
    CCompetition &competition = theCompetition;

    if (competition.mLadderTimer == 0)
    {
        competition.mLadderFullyDisplayed = 0;
        unsigned char i = (unsigned char)-1;
        while (++i < 10)
        {
            if (i != competition.our_skater_num)
            {
                unsigned short base;
                if (competition.mPlacing[i] == 0)
                {
                    base = FIRST_PLACE;
                }
                else if (competition.mPlacing[i] == 1)
                {
                    base = SECOND_PLACE;
                }
                else if (competition.mPlacing[i] == 2)
                {
                    base = THIRD_PLACE;
                }
                else
                {
                    base = (unsigned short)((int)LAST_PLACE +
                        Rnd((int)THIRD_PLACE - (int)LAST_PLACE));
                }
                base = (unsigned short)(base - Rnd(0x12));
                unsigned char j = 0;
                do
                {
                    if (i == competition.mJudges[j].mProPlayer)
                        base = (unsigned short)(
                            base + competition.mJudges[j].mProBias);
                    if (i == competition.mJudges[j].mConPlayer)
                        base = (unsigned short)(
                            base - competition.mJudges[j].mConBias);
                    j++;
                } while (j < 5);
                competition.mScore[i][competition.mCurrentHeat] = base;
            }
        }
        competition.mLastLadderEntry = 0;
    }
    competition.mLadderTimer++;
}

/**
 * Draws the three-heat competition standings, revealing and sorting entrants
 * as the ladder timer advances.
 *
 * Address:   0x80075E50
 * Mangled:   Competition_LadderDisplay__Fv
 * Demangled: Competition_LadderDisplay(void)
 */
void Competition_LadderDisplay(void)
{
    CCompetition &competition = theCompetition;
    unsigned char b;
    unsigned char a;
    unsigned char i;
    char text_buf[45];

    Mess_DeleteAll();
    Mess_SetScale(0x100);
    Mess_SetTextJustify(0);
    Mess_SetRGB(0xB6, 0x84, 0x24, 0);
    sprintf(text_buf, s_COMPETITION_STANDINGS_800b38fc);
    Mess_DrawText(
        0x100, 0x28, text_buf, 0, 0, 0x100, 0);

    Mess_SetTextJustify(1);
    Mess_SetScale(0x78);
    sprintf(text_buf, s_Heat_800db980);
    Mess_DrawText(
        0x104, 0x32, text_buf, 0, 0, 0x100, 0);
    sprintf(text_buf, s_1_2_3_Total_800b3914);
    Mess_DrawText(
        0x104, 0x3C, text_buf, 0, 0, 0x100, 0);

    Mess_SetScale(0xA0);
    i = 0;
    do
    {
        sprintf(text_buf, s__s_800db990,
            Stats[competition.mCompetitorList[competition.mOrdering[i]]].Name);
        if (competition.mOrdering[i] == competition.our_skater_num)
            Mess_SetRGB(0xB6, 0x84, 0x24, 0);
        else
            Mess_SetRGB(0x7A, 0x7A, 0x7A, 0);
        Mess_DrawText(
            0x5A, i * 14 + 0x46, text_buf, 0, 0, 0x100, 0);
        i++;
    } while (i < 10);

    a = 0;
    do
    {
        b = 0;
        do
        {
            unsigned int low_score;
            unsigned int score2;
            unsigned int score1;
            unsigned int score0;
            int dimness;

            if ((unsigned int)(b * 20) < competition.mLadderTimer)
            {
                if (competition.mLadderTimer == (unsigned int)(b * 20 + 1))
                    SFX_PlayX(0x34, 0x2000, 0);

                if (competition.mCurrentHeat == 0)
                {
                    competition.mTotalScore[competition.mOrdering[b]] =
                        competition.mScore[competition.mOrdering[b]][0];
                }
                else
                {
                    unsigned int score2;
                    unsigned int score1;
                    unsigned int score0;

                    score0 = competition.mScore[competition.mOrdering[b]][0];
                    score1 = competition.mScore[competition.mOrdering[b]][1];
                    score2 = competition.mScore[competition.mOrdering[b]][2];
                    if (score0 < score1 && score0 < score2)
                    {
                        competition.mTotalScore[competition.mOrdering[b]] =
                            (unsigned short)(score1 + score2);
                    }
                    else
                    {
                        if (score1 < score0 && score1 < score2)
                        {
                            competition.mTotalScore[competition.mOrdering[b]] =
                                (unsigned short)(score0 + score2);
                        }
                        else
                        {
                            competition.mTotalScore[competition.mOrdering[b]] =
                                (unsigned short)(score0 + score1);
                        }
                    }
                }
            }

            if (b != 0 &&
                (unsigned int)(b * 20) < competition.mLadderTimer)
            {
                int temp_b = b;
                do
                {
                    if (competition.mTotalScore[
                            competition.mOrdering[temp_b]] <=
                        competition.mTotalScore[
                            competition.mOrdering[temp_b - 1]])
                    {
                        break;
                    }
                    unsigned char previous =
                        competition.mOrdering[temp_b - 1];
                    competition.mOrdering[temp_b - 1] =
                        competition.mOrdering[temp_b];
                    competition.mOrdering[temp_b] = previous;
                    temp_b--;
                } while (temp_b > 0);
            }

            dimness = 0;
            if (competition.mCurrentHeat == 2)
            {
                if (competition.mLadderTimer < (unsigned int)((b + 1) * 20))
                {
                    if ((unsigned int)(b * 20) <= competition.mLadderTimer)
                        dimness = (competition.mLadderTimer % 20) * 4;
                }
                else if ((unsigned int)(b * 20) <= competition.mLadderTimer)
                {
                    dimness = 100;
                }
            }

            low_score = 0;
            score0 = competition.mScore[competition.mOrdering[b]][0];
            score1 = competition.mScore[competition.mOrdering[b]][1];
            score2 = competition.mScore[competition.mOrdering[b]][2];
            if (score1 < score0 || score2 < score0)
            {
                low_score = 2;
                if (score1 <= score0 && score1 <= score2)
                    low_score = 1;
            }

            if (a == low_score)
            {
                unsigned char shade = (unsigned char)(0x7A - dimness);
                Mess_SetRGB(shade, shade, shade, 0);
            }
            else if (competition.mOrdering[b] == competition.our_skater_num)
            {
                Mess_SetRGB(0xB6, 0x84, 0x24, 0);
            }
            else
            {
                Mess_SetRGB(0x7A, 0x7A, 0x7A, 0);
            }

            if (a < competition.mCurrentHeat)
            {
                sprintf(text_buf, s__d__d_800db994,
                    competition.mScore[competition.mOrdering[b]][a] / 10,
                    competition.mScore[competition.mOrdering[b]][a] % 10);
                Mess_DrawText(
                    a * 60 + 0xFA, b * 14 + 0x46,
                    text_buf, 0, 0, 0x100, 0);
            }

            if (a == competition.mCurrentHeat)
            {
                if ((unsigned int)(b * 20) < competition.mLadderTimer)
                {
                    sprintf(text_buf, s__d__d_800db994,
                        competition.mScore[competition.mOrdering[b]][a] / 10,
                        competition.mScore[competition.mOrdering[b]][a] % 10);
                }
                else
                {
                    if (competition.mOrdering[b] == competition.our_skater_num)
                        Mess_SetRGB(0xB6, 0x84, 0x24, 0);
                    else
                        Mess_SetRGB(0x7A, 0x7A, 0x7A, 0);
                    sprintf(text_buf, s___800db99c);
                }
                Mess_DrawText(
                    a * 60 + 0xFA, b * 14 + 0x46,
                    text_buf, 0, 0, 0x100, 0);

                if (competition.mOrdering[b] == competition.our_skater_num)
                    Mess_SetRGB(0xB6, 0x84, 0x24, 0);
                else
                    Mess_SetRGB(0x7A, 0x7A, 0x7A, 0);

                if ((unsigned int)(b * 20) < competition.mLadderTimer ||
                    competition.mCurrentHeat != 0)
                {
                    sprintf(text_buf, s__d__d_800db994,
                        competition.mTotalScore[competition.mOrdering[b]] / 10,
                        competition.mTotalScore[competition.mOrdering[b]] % 10);
                }
                else
                {
                    sprintf(text_buf, s___800db99c);
                }
                Mess_DrawText(
                    0x1C2, b * 14 + 0x46,
                    text_buf, 0, 0, 0x100, 0);
            }

            if (competition.mCurrentHeat < a)
            {
                sprintf(text_buf, s___800db99c);
                Mess_DrawText(
                    a * 60 + 0xFA, b * 14 + 0x46,
                    text_buf, 0, 0, 0x100, 0);
            }

            b++;
        } while (b < 10);
        a++;
    } while (a < 3);

    if (competition.mLadderTimer > 200)
        competition.mLadderFullyDisplayed = (void *)1;
    else
        competition.mLadderFullyDisplayed = 0;
    Mess_SetScale(0x100);
    if (competition.mLadderFullyDisplayed != 0)
        Front_PrintCentredPrompt((const char *)GameText.pContinue);
}

/**
 * Competition_WinLoseLogic__FR13EPausedStatusRi
 *
 * Address:   0x800766A8
 * Mangled:   Competition_WinLoseLogic__FR13EPausedStatusRi
 * Demangled: Competition_WinLoseLogic(EPausedStatus &, int &)
 *
 * @param Status  Receives the next paused-game state.
 * @param Activated  Nonzero once the player confirms the screen.
 */
#define COMP_RELEASE(powerUp) do { \
    if ((powerUp) != 0) \
        (powerUp)->_vf->Destructor((powerUp), 3); \
} while (0)

void Competition_WinLoseLogic(EPausedStatus &Status, int &Activated)
{
    CCompetition &competition = theCompetition;

    if (competition.mFading != 0)
    {
        Flash_Update();
        if (Flash_FadeFinished() != 0)
        {
            competition.mFading = 0;
            int entry = pYesNoMenu->FindEntry(
                (const char *)GameText.pYes);
            pYesNoMenu->SetLine((unsigned char)entry);

            COMP_RELEASE(competition.mMedal1);
            COMP_RELEASE(competition.mMedal2);
            COMP_RELEASE(competition.mMedal3);
            competition.mMedal1 = 0;
            competition.mMedal2 = 0;
            competition.mMedal3 = 0;

            reinterpret_cast<CompetitionCareerView *>(&GCareer)->Options.MedalMessagesUsed[
                GSkater[0]->mSkaterType] |= 1 << GLevel;

            GameFMV_StopFMV();
            Pad_ClearAll();
            Pad_ClearTriggers(&Pad[0]);
            Pad_ClearTriggers(&Pad[1]);
            Pad_ActuatorOff(0, 0);
            Pad_ActuatorOff(0, 1);
            Mess_DeleteAll();
            CdControlB(8, 0, 0);
            Pause(10);
            Mem_Delete(DoubleBuffer[0].Polys);
            GameFMV_PlayMovie(-11);
            Flash_Reset();

            void *polys = Mem_New(POLYBUFFERSIZE << 1, 1, 1);
            DoubleBuffer[0].Polys = (unsigned char *)polys;
            DoubleBuffer[1].Polys =
                DoubleBuffer[0].Polys + POLYBUFFERSIZE;
            Status = STATUS_ENDGAME;
        }
    }
    else
    {
        Crowd_Stop();
        if (Activated != 0)
        {
            if (GotMedal__12CCompetition(&competition) != 0)
            {
                int ready = 1;
                if (competition.mMedal1 != 0)
                {
                    unsigned int medalStillAnimating =
                        competition.mMedal1->mTimer < 0x28;
                    ready = !medalStillAnimating;
                }

                if (ready == 0)
                    return;

                if (Competition_WonGame() != 0)
                {
                    competition.mFading = (void *)1;
                    Flash_Fade(0x20, 0);
                    return;
                }

                Status = STATUS_MEDALSAVE;
                int entry = pYesNoMenu->FindEntry(
                    (const char *)GameText.pYes);
                pYesNoMenu->SetLine((unsigned char)entry);
            }
            else
            {
                Status = STATUS_ENDINGSCREEN;
            }

            COMP_RELEASE(competition.mMedal1);
            COMP_RELEASE(competition.mMedal2);
            COMP_RELEASE(competition.mMedal3);
            competition.mMedal1 = 0;
            competition.mMedal2 = 0;
            competition.mMedal3 = 0;

            reinterpret_cast<CompetitionCareerView *>(&GCareer)->Options.MedalMessagesUsed[
                GSkater[0]->mSkaterType] |= 1 << GLevel;
        }
    }
}

/**
 * Finishes any unplayed competition heats, ranks the entrants, awards the
 * player's medal, and draws either the medal presentation or final standings.
 *
 * Address:   0x80076A10
 * Mangled:   Competition_WinLoseDisplay__Fv
 * Demangled: Competition_WinLoseDisplay(void)
 */
void Competition_WinLoseDisplay(void)
{
    CCompetition &competition = theCompetition;
    int our_placing;
    char buf[40];

    Mess_DeleteAll();

    while (competition.mCurrentHeat < 3)
    {
        {
            unsigned char i = 0;
            do
            {
                if ((int)i != competition.our_skater_num)
                {
                    // This inherited barrier preserves the target CFG/register
                    // allocation; three ordinary loop/declaration forms did not.
                    asm volatile("");
                    unsigned short base;
                    if (competition.mPlacing[i] == 0)
                    {
                        base = FIRST_PLACE;
                    }
                    else if (competition.mPlacing[i] == 1)
                    {
                        base = SECOND_PLACE;
                    }
                    else if (competition.mPlacing[i] == 2)
                    {
                        base = THIRD_PLACE;
                    }
                    else
                    {
                        base = (unsigned short)((int)LAST_PLACE +
                            Rnd((int)THIRD_PLACE - (int)LAST_PLACE));
                    }

                    base = (unsigned short)(base - Rnd(0x12));
                    unsigned char j = 0;
                    do
                    {
                        if (i == competition.mJudges[j].mProPlayer)
                            base = (unsigned short)(
                                base + competition.mJudges[j].mProBias);
                        if (i == competition.mJudges[j].mConPlayer)
                            base = (unsigned short)(
                                base - competition.mJudges[j].mConBias);
                        j++;
                    } while (j < 5);

                    competition.mScore[i][competition.mCurrentHeat] = base;
                }
            } while (++i < 10);
        }

        {
            int b = 0;
            unsigned short *totalScore = competition.mTotalScore;
            unsigned short *score0Base = &competition.mScore[0][0];
            unsigned short *score1Base = &competition.mScore[0][1];
            unsigned short *score2Base = &competition.mScore[0][2];
            unsigned short *score2Cursor = score2Base;
            unsigned short *score1Cursor = score1Base;
            unsigned short *score0Cursor = score0Base;

            competition.mCurrentHeat++;
            do
            {
                unsigned int score2;
                unsigned int score1;
                unsigned int score0;

                score0 = *score0Cursor;
                score1 = *score1Cursor;
                score2 = *score2Cursor;

                if (score0 < score1 && score0 < score2)
                {
                    *totalScore = (unsigned short)(score1 + score2);
                }
                else if (score1 < score0 && score1 < score2)
                {
                    *totalScore = (unsigned short)(score0 + score2);
                }
                else
                {
                    *totalScore = (unsigned short)(score0 + score1);
                }
                totalScore++;
                score2Cursor += 3;
                score1Cursor += 3;
                b++;
                score0Cursor += 3;
            } while (b < 10);
        }
    }

    {
        int i = 0;
        do
        {
            competition.mPlacing[i] = (unsigned char)i;
            i++;
        } while (i < 10);
    }

    {
        int i = 1;
        do
        {
            int entry = i;
            do
            {
                if (competition.mTotalScore[competition.mPlacing[entry]] <=
                    competition.mTotalScore[competition.mPlacing[entry - 1]])
                {
                    break;
                }
                unsigned char previous = competition.mPlacing[entry - 1];
                competition.mPlacing[entry - 1] = competition.mPlacing[entry];
                competition.mPlacing[entry] = previous;
                entry--;
            } while (entry > 0);
            i++;
        } while (i < 10);
    }

    Mess_SetScale(0x100);
    Mess_SetRGB(0x7A, 0x7A, 0x7A, 0);

    {
        int i = 0;
        do
        {
            if (competition.mPlacing[i] == competition.our_skater_num)
                our_placing = i;
            i++;
        } while (i < 10);
    }

    switch (our_placing)
    {
    case 0:
    {
        Mess_DrawText(
            0x100, 0x28, s_You_win_the_Gold__800b38c0,
            0, 0, 0x100, 0);
        if (competition.mMedal1 == 0)
        {
            Career_GiveMedal(GLevel, MEDAL_GOLD);
            competition.mMedal1 =
                Medal_Create(MEDAL_GOLD, 0, -5, 0x8C, 0x800, 10);
        }
        if (competition.mWinLoseCheer == 0)
        {
            SFX_PlayX(0x2D, 0x2000, 0);
            SFX_PlayX(0x30, 0x2000, 0);
            competition.mWinLoseCheer = (void *)1;
        }
        break;
    }
    case 1:
    {
        Mess_DrawText(
            0x100, 0x28, s_You_win_the_Silver__800b38d4,
            0, 0, 0x100, 0);
        if (competition.mMedal1 == 0)
        {
            Career_GiveMedal(GLevel, MEDAL_SILVER);
            competition.mMedal1 =
                Medal_Create(MEDAL_SILVER, 0, -5, 0x8C, 0x800, 10);
        }
        if (competition.mWinLoseCheer == 0)
        {
            SFX_PlayX(0x30, 0x2000, 0);
            competition.mWinLoseCheer = (void *)1;
        }
        break;
    }
    case 2:
    {
        Mess_DrawText(
            0x100, 0x28, s_You_win_the_Bronze__800b38e8,
            0, 0, 0x100, 0);
        if (competition.mMedal1 == 0)
        {
            Career_GiveMedal(GLevel, MEDAL_BRONZE);
            competition.mMedal1 =
                Medal_Create(MEDAL_BRONZE, 0, -5, 0x8C, 0x800, 10);
        }
        if (competition.mWinLoseCheer == 0)
        {
            SFX_PlayX(0x2D, 0x2000, 0);
            competition.mWinLoseCheer = (void *)1;
        }
        break;
    }
    default:
    {
        if (competition.mWinLoseCheer == 0)
        {
            SFX_PlayX(0x2F, 0x2000, 0);
            competition.mWinLoseCheer = (void *)1;
        }

        Mess_DrawText(
            0x100, 0x28, s_You_lose__800db93c,
            0, 0, 0x100, 0);

        int first;
        int second;
        int third;
        int i = 0;
        do
        {
            if (competition.mPlacing[0] == i)
                first = i;
            if (competition.mOrdering[1] == i)
                second = i;
            if (competition.mOrdering[2] == i)
                third = i;
            i++;
        } while (i < 10);

        Mess_SetRGB(0xB6, 0x84, 0x24, 0);
        Mess_SetScale(0xA0);

        sprintf(buf, s__s__1st_place_800db948,
            Stats[competition.mCompetitorList[first]].Name);
        Mess_DrawText(
            0x100, 0x3C, buf, 0, 0, 0x100, 0);

        sprintf(buf, s__s__2nd_place_800db958,
            Stats[competition.mCompetitorList[second]].Name);
        Mess_DrawText(
            0x100, 0x46, buf, 0, 0, 0x100, 0);

        sprintf(buf, s__s__3rd_place_800db968,
            Stats[competition.mCompetitorList[third]].Name);
        Mess_DrawText(
            0x100, 0x50, buf, 0, 0, 0x100, 0);

        Mess_SetRGB(0x7A, 0x7A, 0x7A, 0);
        sprintf(buf, s__dth_800db978, our_placing + 1);
        Mess_SetScale(0x180);
        Mess_DrawText(
            0x100, 0x78,
            Stats[competition.mCompetitorList[competition.our_skater_num]].Name,
            0, 0, 0x100, 0);
        Mess_DrawText(
            0x100, 0x96, buf, 0, 0, 0x100, 0);
        break;
    }
    }

    if (competition.mMedal1 != 0)
    {
        competition.mMedal1->_vf->Display(competition.mMedal1);

        competition.medal_mask = 0;
        if ((reinterpret_cast<CompetitionCareerView *>(&GCareer)->CharacterProgress[
                GSkater[0]->mSkaterType].GoalMask[GLevel] & 0xE0) != 0)
        {
            competition.medal_mask = 1 << GLevel;
        }

        Mess_SetScale(0x100);
        Mess_SetRGB(0x7A, 0x7A, 0x7A, 0);

        unsigned int used = reinterpret_cast<CompetitionCareerView *>(&GCareer)->Options.MedalMessagesUsed[
            GSkater[0]->mSkaterType];

        if (used == 0x148)
        {
            CPowerUp *medal = competition.mMedal1;
            medal->mWobbleAmplitude = 2;
            medal->mMaxWobbleAmplitude = 2;
            competition.mMedal1->mPositionY =
                competition.mMedal1->mOrgPositionY - 0x5000;

            if (MedalMessage[2].Line1 != 0)
                Mess_DrawText(
                    0x100, 0x9B, MedalMessage[2].Line1,
                    0, 0, 0x100, 0);
            if (MedalMessage[2].Line2 != 0)
                Mess_DrawText(
                    0x100, 0xAF, MedalMessage[2].Line2,
                    0, 0, 0x100, 0);
            if (MedalMessage[2].Line3 != 0)
                Mess_DrawText(
                    0x100, 0xC3, MedalMessage[2].Line3,
                    0, 0, 0x100, 0);
        }
        else
        {
            int newMessage =
                (used & competition.medal_mask) ^ competition.medal_mask;
            switch (newMessage)
            {
        case 8:
        {
            if (MedalMessage[0].Line1 != 0)
                Mess_DrawText(
                    0x100, 0x9B, MedalMessage[0].Line1,
                    0, 0, 0x100, 0);
            if (MedalMessage[0].Line2 != 0)
                Mess_DrawText(
                    0x100, 0xAF, MedalMessage[0].Line2,
                    0, 0, 0x100, 0);
            if (MedalMessage[0].Line3 != 0)
                Mess_DrawText(
                    0x100, 0xC3, MedalMessage[0].Line3,
                    0, 0, 0x100, 0);
            break;
        }
        case 0x40:
        {
            if (MedalMessage[1].Line1 != 0)
                Mess_DrawText(
                    0x100, 0x9B, MedalMessage[1].Line1,
                    0, 0, 0x100, 0);
            if (MedalMessage[1].Line2 != 0)
                Mess_DrawText(
                    0x100, 0xAF, MedalMessage[1].Line2,
                    0, 0, 0x100, 0);
            if (MedalMessage[1].Line3 != 0)
                Mess_DrawText(
                    0x100, 0xC3, MedalMessage[1].Line3,
                    0, 0, 0x100, 0);
            break;
        }
        case 0x100:
        {
            CPowerUp *medal = competition.mMedal1;
            medal->mWobbleAmplitude = 2;
            medal->mMaxWobbleAmplitude = 2;
            competition.mMedal1->mPositionY =
                competition.mMedal1->mOrgPositionY - 0x5000;

            if (MedalMessage[2].Line1 != 0)
                Mess_DrawText(
                    0x100, 0x9B, MedalMessage[2].Line1,
                    0, 0, 0x100, 0);
            if (MedalMessage[2].Line2 != 0)
                Mess_DrawText(
                    0x100, 0xAF, MedalMessage[2].Line2,
                    0, 0, 0x100, 0);
            if (MedalMessage[2].Line3 != 0)
                Mess_DrawText(
                    0x100, 0xC3, MedalMessage[2].Line3,
                    0, 0, 0x100, 0);
            break;
        }
        default:
            break;
            }
        }
        int ready = 1;
        if (competition.mMedal1 != 0)
        {
            unsigned int medalStillAnimating =
                competition.mMedal1->mTimer < 0x28;
            ready = !medalStillAnimating;
        }
        if (ready == 0)
            return;
    }

    if (competition.mFading == 0)
        Front_PrintCentredPrompt((const char *)GameText.pContinue);
}

/**
 * Competition_JudgeFullyDisplayed__Fv — Returns judge fully displayed flag
 *
 * Address:   0x8007751C, Size: 16 bytes (4 instructions)
 * Mangled:   Competition_JudgeFullyDisplayed__Fv
 * Demangled: Competition_JudgeFullyDisplayed(void)
 *
 * @return whether the judge display has completed
 */
int Competition_JudgeFullyDisplayed(void)
{
    return (int)theCompetition.mJudgeFullyDisplayed;
}

/**
 * Competition_LadderFullyDisplayed__Fv — Returns ladder fully displayed flag
 *
 * Address:   0x8007752C, Size: 16 bytes (4 instructions)
 * Mangled:   Competition_LadderFullyDisplayed__Fv
 * Demangled: Competition_LadderFullyDisplayed(void)
 *
 * @return whether the ladder display has completed
 */
int Competition_LadderFullyDisplayed(void)
{
    return (int)theCompetition.mLadderFullyDisplayed;
}

/**
 * Competition_FullyDisplayJudge__Fv — Sets judge timer to 1000
 *
 * Address:   0x8007753C, Size: 16 bytes (4 instructions)
 * Mangled:   Competition_FullyDisplayJudge__Fv
 * Demangled: Competition_FullyDisplayJudge(void)
 *
 */
void Competition_FullyDisplayJudge(void)
{
    theCompetition.mJudgeTimer = 1000;
}

/**
 * Competition_FullyDisplayLadder__Fv — Sets ladder timer to 1000
 *
 * Address:   0x8007754C, Size: 16 bytes (4 instructions)
 * Mangled:   Competition_FullyDisplayLadder__Fv
 * Demangled: Competition_FullyDisplayLadder(void)
 *
 */
void Competition_FullyDisplayLadder(void)
{
    theCompetition.mLadderTimer = 1000;
}

/**
 * Competition_NewHeat__Fv — Advance to next heat, clear scores, return if more heats remain
 *
 * Address:   0x8007755C, Size: 52 bytes (13 instructions)
 * Mangled:   Competition_NewHeat__Fv
 * Demangled: Competition_NewHeat(void)
 *            Uses base struct at 0x800C596C accessed via lui+addiu
 *
 * @return Nonzero when another heat remains to be played.
 */
int Competition_NewHeat(void)
{
    CCompetition &competition = theCompetition;
    competition.mLadderTimer = 0;
    competition.mJudgeTimer = 0;
    competition.mWinLoseCheer = 0;
    competition.mJudgeFullyDisplayed = 0;
    competition.mLadderFullyDisplayed = 0;
    competition.mCurrentHeat++;
    return competition.mCurrentHeat < 3;
}

/**
 * Competition_MarqueeDisplay__Fv
 *
 * Address:   0x80077590
 * Mangled:   Competition_MarqueeDisplay__Fv
 * Demangled: Competition_MarqueeDisplay(void)
 *
 */
void Competition_MarqueeDisplay(void)
{
    CCompetition &competition = theCompetition;
    if (competition.mMarqueeText != 0)
    {
        Mess_SetRGB(0x7A, 0x7A, 0x7A, 0);
        Mess_SetScale(0x100);
        Mess_SetTextJustify(1);
        if (competition.mMarqueeTimer >= 0x301)
        {
            competition.mMarqueeTimer = 0;
        }
        int x = 0x200 - competition.mMarqueeTimer;
        Mess_DrawText(
            x, 0xC8, competition.mMarqueeText,
            0, 0, 0x100, 0);
        competition.mMarqueeTimer += 5;
    }
}

/**
 * Reset the competition entrants and compose the current heat marquee.
 *
 * Address:   0x80077634
 * Mangled:   Competition_ResetMarquee__Fv
 * Demangled: Competition_ResetMarquee(void)
 *
 */
void Competition_ResetMarquee(void)
{
    CCompetition &competition = theCompetition;

    if (competition.mCurrentHeat == 0)
    {
        competition.mJudgeTimer = 0;
        competition.mLadderTimer = 0;
        competition.mWinLoseCheer = 0;
        competition.mFading = 0;
        Utils_InitialRand(Vblanks);

        {
            int i = 0;
            do
            {
                competition.mCompetitorList[i] = (unsigned char)i;
                i++;
            } while (i < 10);
        }

        competition.our_skater_num = SelectedSkater;
        if (SelectedSkater == 10)
        {
            competition.our_skater_num = Rnd(10);
            competition.mCompetitorList[competition.our_skater_num] = 10;
        }

        if (GLevel == 8)
        {
            HOLYCOW_PLACE = 0x3CF;
            FIRST_PLACE = 0x3A7;
            SECOND_PLACE = 0x38E;
            THIRD_PLACE = 0x35C;
            LAST_PLACE = 0x302;
        }
        else
        {
            HOLYCOW_PLACE = 0x3CF;
            FIRST_PLACE = 0x398;
            SECOND_PLACE = 0x366;
            THIRD_PLACE = 0x334;
            LAST_PLACE = 0x208;
        }

        {
            unsigned char i = 0;
            do
            {
                competition.mJudges[i].mProPlayer = (unsigned char)Rnd(10);
                competition.mJudges[i].mConPlayer = (unsigned char)Rnd(10);
                competition.mJudges[i].mProBias = (unsigned char)Rnd(30);
                competition.mJudges[i].mConBias = (unsigned char)Rnd(30);
                i++;
            } while (i < 5);
        }

        {
            unsigned char i = 0;
            do
            {
                unsigned char j = 0;
                do
                {
                    competition.mScore[i][j] = 0;
                    j++;
                } while (j < 3);

                competition.mTotalScore[i] = 0;
                competition.mOrdering[i] = i;
                i++;
            } while (i < 10);
        }

        competition.mCurrentHeat = 0;

        {
            int i = 0;
            do
            {
                competition.mPlacing[i] = 0xFF;
                i++;
            } while (i < 10);
        }

        {
            int i = 0;
            do
            {
                int j = Rnd(10 - i);
                int k = 0;
                int slot = -1;

                if (j >= 0)
                {
                    do
                    {
                        slot++;
                        if (competition.mPlacing[slot] == 0xFF)
                        {
                            k++;
                        }
                    } while (k <= j);
                }

                competition.mPlacing[slot] = (unsigned char)i;
                i++;
            } while (i < 10);
        }

        {
            int i = 0;
            do
            {
                if (competition.mPlacing[i] == 9)
                {
                    competition.mPlacing[i] =
                        competition.mPlacing[competition.our_skater_num];
                }
                i++;
            } while (i < 10);
        }
        competition.mPlacing[competition.our_skater_num] = 0xFF;
    }

    competition.mMarqueeTimer = 0;
    competition.mMarqueeText = 0;
    sprintf(competition.mMarqueeTextBuf,
            s__s__s___HEAT__d_800db92c,
            Levels[GLevel].pLevelName,
            Levels[GLevel].pCityName,
            competition.mCurrentHeat + 1);
    competition.mMarqueeText = competition.mMarqueeTextBuf;
}

/**
 * Advance the all-gold ending fade or dismiss the ending medal screen.
 *
 * Address:   0x80077940
 * Mangled:   Competition_EndGameLogic__FR13EPausedStatusRi
 * Demangled: Competition_EndGameLogic(EPausedStatus &, int &)
 *
 * @param Status     Receives the next paused-game state.
 * @param Activated  Nonzero once the player confirms the screen.
 */
void Competition_EndGameLogic(EPausedStatus &Status, int &Activated)
{
    CCompetition &competition = theCompetition;

    if (competition.mFading != 0)
    {
        Flash_Update();
        if (Flash_FadeFinished() != 0)
        {
            CPowerUp *firstMedal = competition.mMedal1;
            competition.mFading = 0;

            COMP_RELEASE(firstMedal);
            COMP_RELEASE(competition.mMedal2);
            COMP_RELEASE(competition.mMedal3);
            competition.mMedal1 = 0;
            competition.mMedal2 = 0;
            competition.mMedal3 = 0;

            reinterpret_cast<CompetitionCareerView *>(&GCareer)->Options.MedalMessagesUsed[
                GSkater[0]->mSkaterType] |= 1 << GLevel;

            GameFMV_StopFMV();
            Pad_ClearAll();
            Pad_ClearTriggers(&Pad[0]);
            Pad_ClearTriggers(&Pad[1]);
            Pad_ActuatorOff(0, 0);
            Pad_ActuatorOff(0, 1);
            Mess_DeleteAll();
            CdControlB(8, 0, 0);
            Pause(10);
            Mem_Delete(DoubleBuffer[0].Polys);
            GameFMV_PlayMovie(-(GSkater[0]->mSkaterType + 12));
            Flash_Reset();

            void *polys = Mem_New(POLYBUFFERSIZE << 1, 1, 1);
            DoubleBuffer[0].Polys = (unsigned char *)polys;
            DoubleBuffer[1].Polys =
                DoubleBuffer[0].Polys + POLYBUFFERSIZE;
            Status = STATUS_MEDALSAVE;
        }
    }
    else if (Activated != 0)
    {
        int ready = 1;
        if (competition.mMedal1 != 0)
        {
            unsigned int medalStillAnimating = competition.mMedal1->mTimer < 0x28;
            ready = !medalStillAnimating;
        }

        if (ready != 0)
        {
            GRollCredits = 1;
            int entry = pYesNoMenu->FindEntry(
                (const char *)GameText.pYes);
            pYesNoMenu->SetLine((unsigned char)entry);

            if (Competition_WonGameAllGold() != 0)
            {
                competition.mFading = (void *)1;
                Flash_Fade(0x20, 0);
                return;
            }

            Status = STATUS_MEDALSAVE;
            COMP_RELEASE(competition.mMedal1);
            COMP_RELEASE(competition.mMedal2);
            COMP_RELEASE(competition.mMedal3);
            competition.mMedal1 = 0;
            competition.mMedal2 = 0;
            competition.mMedal3 = 0;

            reinterpret_cast<CompetitionCareerView *>(&GCareer)->Options.MedalMessagesUsed[
                GSkater[0]->mSkaterType] |= 1 << GLevel;
        }
    }
}

/**
 * Draws the end-of-competition message and creates and animates the player's
 * three earned medals.
 *
 * Address:   0x80077C70
 * Mangled:   Competition_EndGameDisplay__Fv
 * Demangled: Competition_EndGameDisplay(void)
 */
void Competition_EndGameDisplay(void)
{
    CCompetition &competition = theCompetition;

    Mess_DeleteAll();
    Mess_SetScale(0x100);
    Mess_SetRGB(0x7A, 0x7A, 0x7A, 0);
    Mess_SetTextJustify(0);

    if (Competition_WonGameAllGold() != 0)
    {
        Mess_DrawText(
            0x100, 0x28, s_You_scored_all_golds__800b37e8,
            0, 0, 0x100, 0);
        Mess_DrawText(
            0x100, 0x3C, s_Your_are_the_top_pro_skater_800b3800,
            0, 0, 0x100, 0);
        Mess_DrawText(
            0x100, 0x50, s_Congratulations__800b3734,
            0, 0, 0x100, 0);
        Mess_DrawText(
            0x100, 0xB4, s_Watch_this_then_get_off_your_800b3820,
            0, 0, 0x100, 0);
        Mess_DrawText(
            0x100, 0xC8, s_couch_and_go_skate__800b3840,
            0, 0, 0x100, 0);

        if (competition.mMedal1 == 0)
        {
            competition.mMedal1 =
                Medal_Create(MEDAL_GOLD, -30, 5, 140, 0x600, 3);
        }
        if (competition.mMedal2 == 0)
        {
            competition.mMedal2 =
                Medal_Create(MEDAL_GOLD, 0, 5, 140, 0x600, 3);
        }
        if (competition.mMedal3 == 0)
        {
            competition.mMedal3 =
                Medal_Create(MEDAL_GOLD, 30, 5, 140, 0x600, 3);
        }
    }
    else
    {
        Mess_DrawText(
            0x100, 0x28, s_Your_third_medal_Good_going__800b3854,
            0, 0, 0x100, 0);
        Mess_DrawText(
            0x100, 0x3C, s_You_just_might_become_800b3874,
            0, 0, 0x100, 0);
        Mess_DrawText(
            0x100, 0x50, s_an_average_skater_800b388c,
            0, 0, 0x100, 0);
        Mess_DrawText(
            0x100, 0xB4, s_pro_skaters_get_all_golds_800b38a0,
            0, 0, 0x100, 0);

        EMedalType m[3];
        int i = 0;
        const char *goalMasks = reinterpret_cast<CompetitionCareerView *>(&GCareer)->CharacterProgress[0].GoalMask;
        EMedalType *medal = m;
        do
        {
            int goalOffset = i;
            goalOffset += GSkater[0]->mSkaterType *
                sizeof(SCharacterProgress);
            // Pointer indexing changes target operand order in GCC 2.91.
            char goalMask = *(const char *)(goalOffset + (int)goalMasks);
            if ((goalMask & 0x20) != 0)
            {
                *medal++ = MEDAL_GOLD;
            }
            else if ((goalMask & 0x40) != 0)
            {
                *medal++ = MEDAL_SILVER;
            }
            else if ((goalMask & 0x80) != 0)
            {
                *medal++ = MEDAL_BRONZE;
            }
            i++;
        } while (i < 8);

        if (competition.mMedal1 == 0)
        {
            competition.mMedal1 =
                Medal_Create(m[0], -30, 5, 140, 0x500, 5);
        }
        if (competition.mMedal2 == 0)
        {
            competition.mMedal2 =
                Medal_Create(m[1], 0, 5, 140, 0x500, 5);
        }
        if (competition.mMedal3 == 0)
        {
            competition.mMedal3 =
                Medal_Create(m[2], 30, 5, 140, 0x500, 5);
        }
    }

    competition.mMedal1->_vf->Display(competition.mMedal1);
    competition.mMedal2->_vf->Display(competition.mMedal2);
    competition.mMedal3->_vf->Display(competition.mMedal3);

    int ready = 1;
    if (competition.mMedal1 != 0)
    {
        unsigned int medalStillAnimating = competition.mMedal1->mTimer < 0x28;
        ready = !medalStillAnimating;
    }
    if (ready != 0 && competition.mFading == 0)
    {
        Front_PrintCentredPrompt((const char *)GameText.pContinue);
    }
}

/**
 * Destroy the five judges in reverse construction order, then mark the
 * competition object as destructed.
 *
 * Address: 0x80078070
 */
void Competition_GlobalDtor(void) asm("_GLOBAL_.D.MedalMessage");

void Competition_GlobalDtor(void)
{
    CCompetition *competition = &theCompetition;
    CJudge *first = &competition->mJudges[0];
    if (first != 0)
    {
        CJudge *cur = &competition->mJudges[5];
        if (first != cur)
        {
            do
            {
                --cur;
                int *vtable = cur->CClass.vtable;
                void (*destructor)(CJudge *, int) =
                    (void (*)(CJudge *, int))vtable[2];
                destructor(cur, 2);
            } while (first != cur);
        }
    }
    competition->CClass.vtable = &DAT_800d7ca0_plus_0x3d50;
}

/**
 * Install the competition and judge vtables and clear the three owned medal
 * pointers for the process-lifetime competition object.
 *
 * Address: 0x800780E0
 */
void Competition_GlobalInit(void) asm("_GLOBAL_.I.MedalMessage");

void Competition_GlobalInit(void)
{
    CCompetition *competition = &theCompetition;
    competition->CClass.vtable = &DAT_800db9d0;

    CJudge *judge = &competition->mJudges[0];
    int i = 4;
    do
    {
        judge->CClass.vtable = &DAT_800db9e0;
        --i;
        ++judge;
    } while (i != -1);

    competition->mMedal1 = 0;
    competition->mMedal2 = 0;
    competition->mMedal3 = 0;
}

/**
 * ====== 0x80078120 GotMedal__12CCompetition ====================================
 * Owner-local implementation of CCompetition::GotMedal().
 *
 * Address:   0x80078120
 * Mangled:   GotMedal__12CCompetition
 * Demangled: CCompetition::GotMedal(void)
 *            Returns 1 if the first medal pointer is nonzero
 *
 * @param competition  Competition state whose medal presentation is queried.
 * @return Nonzero once the first medal has been created.
 */
namespace CompetitionOwnerFunctions
{
extern "C"
{
static int GotMedal__12CCompetition(CCompetition *competition)
{
    return competition->mMedal1 != 0;
}
}
}

/**
 * Owner-local implementation of the CCompetition deleting destructor.
 *
 * Address:   0x8007812C
 * Mangled:   _._12CCompetition
 * Demangled: CCompetition::~CCompetition()
 *
 * @param _this  Competition object being destroyed.
 * @param flags  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
static void __DOT__12CCompetition(CCompetition *_this, int flags)
{
    CCompetition *competition = _this;
    int fl = flags;
    CJudge *first = &competition->mJudges[0];
    if (first != 0)
    {
        CJudge *cur = &competition->mJudges[5];
        if (first != cur)
        {
            do
            {
                --cur;
                int *vtable = cur->CClass.vtable;
                void (*func)(CJudge *, int) =
                    (void (*)(CJudge *, int))vtable[2];
                func(cur, 2);
            } while (first != cur);
        }
    }
    competition->CClass.vtable = &DAT_800d7ca0_plus_0x3d50;
    {
        int test = fl & 1;
        if (test)
        {
            CClass::operator delete(competition);
        }
    }
}

/**
 * _._6CJudge — CJudge destructor (first copy)
 *
 * Address:   0x800781B4
 * Mangled:   _._6CJudge
 * Demangled: CJudge::~CJudge()
 *
 * @param _this  Judge object being destroyed.
 * @param flags  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
static void JudgeDestructorOwner(CJudge *_this, int flags)
{
    _this->CClass.vtable = &DAT_800d7ca0_plus_0x3d50;
    if (flags & 1)
    {
        CClass::operator delete(_this);
    }
}

/**
 * __DOT__6CJudge — CJudge destructor (second copy, identical)
 *
 * Address: 0x800781E4
 * Mangled: __DOT__6CJudge
 *
 * @param _this  Base judge object being destroyed.
 * @param flags  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
static void __DOT__6CClass_800781E4(CClass *_this, int flags)
{
    _this->vtable = &DAT_800d7ca0_plus_0x3d50;
    if (flags & 1)
    {
        CClass::operator delete(_this);
    }
}

/*
 * Original COMPETITION.CPP initialized-data contribution.
 *
 * The read-only strings are kept as separate semantic objects because the
 * medal-message table below points into this exact owner-local pool. Their
 * target-named namespace owners keep C linkage without publishing XDEFs; the
 * incomplete declarations above preserve the target access forms.
 */
namespace CompetitionAddressViewOwners
{
extern "C"
{
static char s_Congratulations__800b3734[]
    __attribute__((section(".rdata"), aligned(4))) = "Congratulations!";
}
}
static char Competition_NextDowntown[]
    __attribute__((section(".rdata"), aligned(4))) =
        "Next stop, the DOWNTOWN level.";
static char Competition_NextSanFrancisco[]
    __attribute__((section(".rdata"), aligned(4))) =
        "Next stop, SAN FRANCISCO.";
static char Competition_AllMedalsHeading[]
    __attribute__((section(".rdata"), aligned(4))) =
        "Congratulations! You've earned";
static char Competition_AllMedalsBody[]
    __attribute__((section(".rdata"), aligned(4))) =
        "a medal in every competition.";
static char Competition_AllMedalsEnding[]
    __attribute__((section(".rdata"), aligned(4))) =
        "Now sit back and enjoy the show...";
namespace CompetitionAddressViewOwners
{
extern "C"
{
static char s_You_scored_all_golds__800b37e8[]
    __attribute__((section(".rdata"), aligned(4))) =
        "You scored all golds!";
static char s_Your_are_the_top_pro_skater_800b3800[]
    __attribute__((section(".rdata"), aligned(4))) =
        "Your are the top pro skater.";
static char s_Watch_this_then_get_off_your_800b3820[]
    __attribute__((section(".rdata"), aligned(4))) =
        "Watch this, then get off your";
static char s_couch_and_go_skate__800b3840[]
    __attribute__((section(".rdata"), aligned(4))) =
        "couch and go skate!";
static char s_Your_third_medal_Good_going__800b3854[]
    __attribute__((section(".rdata"), aligned(4))) =
        "Your third medal.. Good going!";
static char s_You_just_might_become_800b3874[]
    __attribute__((section(".rdata"), aligned(4))) =
        "You just might become";
static char s_an_average_skater_800b388c[]
    __attribute__((section(".rdata"), aligned(4))) =
        "an average skater.";
static char s_pro_skaters_get_all_golds_800b38a0[]
    __attribute__((section(".rdata"), aligned(4))) =
        "pro skaters get all golds...";
static char s_You_win_the_Gold__800b38c0[]
    __attribute__((section(".rdata"), aligned(4))) =
        "You win the Gold!";
static char s_You_win_the_Silver__800b38d4[]
    __attribute__((section(".rdata"), aligned(4))) =
        "You win the Silver!";
static char s_You_win_the_Bronze__800b38e8[]
    __attribute__((section(".rdata"), aligned(4))) =
        "You win the Bronze!";
static char s_COMPETITION_STANDINGS_800b38fc[]
    __attribute__((section(".rdata"), aligned(4))) =
        "COMPETITION STANDINGS";
static char s_1_2_3_Total_800b3914[]
    __attribute__((section(".rdata"), aligned(4))) =
        "1         2         3           Total";
static char s_JUDGE__d__d__d_800b393c[]
    __attribute__((section(".rdata"), aligned(4))) =
        "JUDGE %d     %d.%d";
static char s_HEAT__d_SCORE___800b3950[]
    __attribute__((section(".rdata"), aligned(4))) =
        "HEAT %d SCORE - ";
}
}

/** Messages displayed for the first, second, and all-medal milestones. */
SMedalMessage MedalMessage[3]
    __attribute__((section(".data"))) = {
        {
            0,
            (char *)s_Congratulations__800b3734,
            (char *)Competition_NextDowntown,
        },
        {
            0,
            (char *)s_Congratulations__800b3734,
            (char *)Competition_NextSanFrancisco,
        },
        {
            (char *)Competition_AllMedalsHeading,
            (char *)Competition_AllMedalsBody,
            (char *)Competition_AllMedalsEnding,
        },
    };

/** Process-lifetime competition state, constructed by the owner initializer. */
CCompetition theCompetition
    __attribute__((section(".data"))) = {};

/**
 * `.ctors` table slot for this owner's hand-written global constructor.
 *
 * GCC emits this entry itself for a compiler-generated constructor, but this
 * owner's hook is written by hand, so explicit section placement retains the
 * slot. The recovered application object order reproduces the target's table
 * sequence.
 */
static void (*Competition_GlobalCtorSlot)(void)
    __attribute__((section(".ctors"))) = Competition_GlobalInit;

/**
 * `.dtors` table slot for this owner's hand-written global destructor.
 *
 * GCC emits this entry itself for a compiler-generated destructor, but this
 * owner's hook is written by hand, so explicit section placement retains the
 * slot. The recovered application object order reproduces the target's table
 * sequence.
 */
static void (*Competition_GlobalDtorSlot)(void)
    __attribute__((section(".dtors"))) = Competition_GlobalDtor;
