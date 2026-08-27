// Career progression, persistent preferences, and controller remapping.

#define CAREER_LAYOUT_DECLARATIONS
#include "CAREER.h"
#undef CAREER_LAYOUT_DECLARATIONS
#define CAREER_OWNER_LAYOUT
#include "CAREER.h"
#undef CAREER_OWNER_LAYOUT
#include "STATS.h"
#include "PAD.h"
#include "UTILS.h"
#include <STDIO.H>
typedef char Career_SCharacterProgress_size_must_be_32[
    (sizeof(SCharacterProgress) == 0x20) ? 1 : -1];
typedef char Career_SHighScore_size_must_be_12[
    (sizeof(SHighScore) == 0x0c) ? 1 : -1];
typedef char Career_SOptions_size_must_be_416[
    (sizeof(SOptions) == 0x1a0) ? 1 : -1];
typedef char Career_SCareer_size_must_be_2420[
    (sizeof(SCareer) == 0x974) ? 1 : -1];
typedef char Career_GamePreferences_size_must_be_416[
    (sizeof(GamePreferences) == 0x1a0) ? 1 : -1];

/** Fixed counts and volume conversions used by career preferences. */
enum CareerConstant
{
    CareerSkaterCount = 14,
    CareerMedalSlotCount = 9,
    CareerHighScoreLevelCount = 16,
    CareerScoresPerLevel = 8,
    CareerRandomSkaterCount = 13,
    CareerPadCount = 2,
    CareerControllerFunctionCount = 4,
    CareerGoalLevelCount = 8,
    CareerOrdinaryGoalCount = 5,
    CareerHorseWordUsedLength = 11,
    CareerRandomMedalNone = 0,
    CareerRandomMedalGold = 1,
    CareerRandomMedalSilver = 2,
    CareerRandomMedalBronze = 3,
    CareerControllerTriangle = 0,
    CareerControllerSquare = 1,
    CareerControllerCircle = 2,
    CareerControllerX = 3,
    CareerVersion = 0x60299,
    CareerPadBindingStride = sizeof(SControl) / sizeof(SButton *),
    CareerSfxLevelPerVolume = 0x555,
    CareerMusicLevelPerVolume = 0x15,
    CareerDefaultSfxVolume = 12,
    CareerDefaultMusicVolume = 6,
    CareerDefaultSfxLevel =
        CareerDefaultSfxVolume * CareerSfxLevelPerVolume,
    CareerDefaultMusicLevel =
        CareerDefaultMusicVolume * CareerMusicLevelPerVolume
};

// Canonical declaration for the MAIN-owned career save.
#include "MAIN.h"
#include "GSKATER_STATE.h"

/**
 * Initials used to seed each level's default high-score table.
 *
 * The final two entries deliberately reuse earlier initials, matching the
 * original pointer table rather than duplicating the strings.
 */
char *DefaultHighScoreInitials[25] __attribute__((section(".data"))) = {
    "RJM", "RAD", "JKU", "JRK", "DRS", "MIK", "SIL", "SMP",
    "JOW", "DMT", "NIC", "NAH", "CJR", "AJC", "KEN", "JJJ",
    "CCT", "TVO", "CAB", "DWP", "GAJ", "JAF", "DWP", "TVO",
    0
};

/** Persistent option values mirrored into each career save. */
GamePreferences GPreferences __attribute__((section(".data"))) = {};

// === Function implementations (address order) ===

/**
 * Populate per-skater career stats and medals for a demo save.
 */
void Career_Test(void)
{
    int i;
    int j;

    i = 0;
    do
    {
        GCareer.CharacterProgress[i].CareerEarnings = Rnd(400) * 1000;
        GCareer.CharacterProgress[i].Cash =
            (GCareer.CharacterProgress[i].CareerEarnings * Rnd(100)) / 100;
        j = 0;
        do
        {
            switch (Rnd(4))
            {
            case CareerRandomMedalNone:
                GCareer.CharacterProgress[i].Medals[j] = MEDAL_NONE;
                break;
            case CareerRandomMedalGold:
                GCareer.CharacterProgress[i].Medals[j] = MEDAL_GOLD;
                break;
            case CareerRandomMedalSilver:
                GCareer.CharacterProgress[i].Medals[j] = MEDAL_SILVER;
                break;
            case CareerRandomMedalBronze:
                GCareer.CharacterProgress[i].Medals[j] = MEDAL_BRONZE;
                break;
            }
            ++j;
        } while (j < CareerMedalSlotCount);
        ++i;
    } while (i < CareerSkaterCount);
}

/**
 * Initialize the career blob, seed high scores, and reset career flags.
 */
void Career_Init(void)
{
    // Retained by the original demo build even though this path does not read it.
    static char first_game = 1;
    char *p;
    char **pHighScore;
    int HighScoreCount;
    int Level;
    int Score;

    {
        int i;

        p = reinterpret_cast<char *>(&GCareer);
        for (i = 0; i < sizeof(GCareer); ++i)
        {
            *p = 0;
            ++p;
        }
    }

    GCareer.Version = CareerVersion;
    pHighScore = DefaultHighScoreInitials;
    HighScoreCount = 0;
    while (*pHighScore != 0)
    {
        ++pHighScore;
        ++HighScoreCount;
    }

    Level = 0;
    do
    {
        Score = 0;
        do
        {
            pHighScore = DefaultHighScoreInitials + Rnd(HighScoreCount);
            GCareer.HighScore[Level][Score].Name[0] = (*pHighScore)[0];
            GCareer.HighScore[Level][Score].Name[1] = (*pHighScore)[1];
            GCareer.HighScore[Level][Score].Name[2] = (*pHighScore)[2];
            GCareer.HighScore[Level][Score].Score =
                CAREER_LEVEL_HIGH_SCORE(Levels, Level) +
                (CareerScoresPerLevel - 1 - Score) * 5000;
            GCareer.HighScore[Level][Score].Skater =
                Rnd(CareerRandomSkaterCount);
            ++Score;
        } while (Score < CareerScoresPerLevel);
        ++Level;
    } while (Level < CareerHighScoreLevelCount);

    Career_Test();
    {
        int i = 0;

        GHasCareerLoaded = 0;
        GLoadedCareerIsSaved = 0;
        do
        {
            GCareer.LastTrucks[i] = 1;
            ++i;
        } while (i < CareerPadCount);
    }
    Options_SetPreferences();
}

/**
 * Copy current preferences into the career save and refresh derived values.
 */
void Options_SetPreferences(void)
{
    int padNum = 0;
    int i;

    GCareer.Options.SFXVolume = GPreferences.Options.SFXVolume;
    GCareer.Options.MusicVolume = GPreferences.Options.MusicVolume;
    GCareer.Options.TrickTipsEnabled = GPreferences.Options.TrickTipsEnabled;
    GCareer.Options.ScoreDisplayEnabled = GPreferences.Options.ScoreDisplayEnabled;
    do
    {
        GCareer.Options.AutoKickEnabled[padNum] =
            GPreferences.Options.AutoKickEnabled[padNum];
        GCareer.Options.VibrationEnabled[padNum] =
            GPreferences.Options.VibrationEnabled[padNum];
        GCareer.Options.ControllerFunction[padNum][0] =
            GPreferences.Options.ControllerFunction[padNum][0];
        GCareer.Options.ControllerFunction[padNum][1] =
            GPreferences.Options.ControllerFunction[padNum][1];
        GCareer.Options.ControllerFunction[padNum][2] =
            GPreferences.Options.ControllerFunction[padNum][2];
        GCareer.Options.ControllerFunction[padNum][3] =
            GPreferences.Options.ControllerFunction[padNum][3];
        ++padNum;
    } while (padNum < CareerPadCount);
    SFXLEVEL = GCareer.Options.SFXVolume * CareerSfxLevelPerVolume;
    XALEVEL = GCareer.Options.MusicVolume * CareerMusicLevelPerVolume;
    sprintf(GCareer.Options.HorseWord, GPreferences.Options.HorseWord);
    GCareer.Options.HorseLen = GPreferences.Options.HorseLen;
    for (i = GPreferences.Options.HorseLen;
         i < CareerHorseWordUsedLength; ++i)
    {
        GCareer.Options.HorseWord[i] = '\0';
    }
    sprintf(GCareer.Options.LastHighScoreInitials,
            GPreferences.Options.LastHighScoreInitials);
    Options_RemapControllers();
}

/**
 * Remap controller button pointers from the saved function table.
 */
void Options_RemapControllers(void)
{
    int padNum = 0;
    SControl *firstPad = Pad;
    SButton **mappedX = &firstPad->pX;
    SButton **mappedCircle = &firstPad->pCircle;
    SButton **mappedSquare = &firstPad->pSquare;
    SButton **mappedTriangle = &firstPad->pTriangle;
    int *controllerFunctions = &GCareer.Options.ControllerFunction[0][0];
    SControl *pad = firstPad;

    do
    {
        int i = 0;
        SButton *button = &pad->Triangle;
        int *mapping = controllerFunctions;
        do
        {
            switch (*mapping)
            {
            case CareerControllerTriangle:
                *mappedTriangle = button;
                break;
            case CareerControllerSquare:
                *mappedSquare = button;
                break;
            case CareerControllerCircle:
                *mappedCircle = button;
                break;
            case CareerControllerX:
                *mappedX = button;
                break;
            }
            ++button;
            ++i;
            ++mapping;
        } while (i < CareerControllerFunctionCount);
        mappedX += CareerPadBindingStride;
        mappedCircle += CareerPadBindingStride;
        mappedSquare += CareerPadBindingStride;
        mappedTriangle += CareerPadBindingStride;
        controllerFunctions += CareerControllerFunctionCount;
        ++padNum;
        ++pad;
    } while (padNum < CareerPadCount);
}

/**
 * Restore default audio, gameplay, controller, and text preferences.
 */
void Options_Init(void)
{
    int padNum = 0;

    GPreferences.Options.SFXVolume = CareerDefaultSfxVolume;
    SFXLEVEL = CareerDefaultSfxLevel;
    GPreferences.Options.MusicVolume = CareerDefaultMusicVolume;
    XALEVEL = CareerDefaultMusicLevel;
    GPreferences.Options.TrickTipsEnabled = 1;
    GPreferences.Options.ScoreDisplayEnabled = 1;

    do
    {
        GPreferences.Options.AutoKickEnabled[padNum] = 1;
        GPreferences.Options.VibrationEnabled[padNum] = 1;
        GPreferences.Options.ControllerFunction[padNum][0] =
            CareerControllerTriangle;
        GPreferences.Options.ControllerFunction[padNum][1] =
            CareerControllerSquare;
        GPreferences.Options.ControllerFunction[padNum][2] =
            CareerControllerCircle;
        GPreferences.Options.ControllerFunction[padNum][3] =
            CareerControllerX;
        padNum += 1;
    } while (padNum < CareerPadCount);
    {
        sprintf(GPreferences.Options.HorseWord, "HORSE");
        GPreferences.Options.HorseLen = 5;
        sprintf(GPreferences.Options.LastHighScoreInitials, "AAA");
    }
}

/**
 * Test one goal bit for the current skater and level.
 *
 * @param Goal Goal bit to test.
 * @return Nonzero when the goal has already been awarded.
 */
int Career_Got(int Goal)
{
    return (GCareer.CharacterProgress[
                reinterpret_cast<CBruceCareerView *>(GSkater[0])->mSkaterType]
                .GoalMask[GLevel] >> Goal) & 1;
}

/**
 * Award a tape, then recompute the skater's tape count.
 *
 * @param Level  Level whose goal mask is updated.
 * @param Goal  Goal bit to award within that level.
 */
void Career_GiveTape(int Level, int Goal)
{
    int t = 0;
    int j = 0;

    GCareer.CharacterProgress[
        reinterpret_cast<CBruceCareerView *>(GSkater[0])->mSkaterType]
        .GoalMask[Level] |= 1 << Goal;
    do
    {
        int i = 0;
        do
        {
            if (CAREER_LEVEL_IS_COMPETITION(Levels, j) == 0 &&
                ((static_cast<unsigned char>(
                      GCareer.CharacterProgress[
                          reinterpret_cast<CBruceCareerView *>(GSkater[0])
                              ->mSkaterType]
                          .GoalMask[j]) >> i) & 1))
            {
                ++t;
            }
            ++i;
        } while (i < CareerOrdinaryGoalCount);
        ++j;
    } while (j < CareerGoalLevelCount);
    GCareer.CharacterProgress[
        reinterpret_cast<CBruceCareerView *>(GSkater[0])->mSkaterType]
        .NumTapes = t;
    ++reinterpret_cast<CBruceCareerView *>(GSkater[0])->mSessionTapes;
}

/**
 * Award a medal, then recompute the skater's medal totals.
 *
 * @param Level  Level slot to mark with the medal bit.
 * @param medal Medal bit index to award.
 */
void Career_GiveMedal(int Level, EMedalType medal)
{
    int g = 0;
    int s = 0;
    int b = 0;
    int j = 0;

    GCareer.CharacterProgress[
        reinterpret_cast<CBruceCareerView *>(GSkater[0])->mSkaterType]
        .GoalMask[Level] |=
        1 << medal;
    do
    {
        if (!Levels[j].isCompetition)
        {
            if ((GCareer.CharacterProgress[
                     reinterpret_cast<CBruceCareerView *>(GSkater[0])
                         ->mSkaterType]
                     .GoalMask[j] & (1 << MEDAL_GOLD)) != 0)
            {
                ++g;
            }
            if ((GCareer.CharacterProgress[
                     reinterpret_cast<CBruceCareerView *>(GSkater[0])
                         ->mSkaterType]
                     .GoalMask[j] & (1 << MEDAL_SILVER)) != 0)
            {
                ++s;
            }
            if ((GCareer.CharacterProgress[
                     reinterpret_cast<CBruceCareerView *>(GSkater[0])
                         ->mSkaterType]
                     .GoalMask[j] & (1 << MEDAL_BRONZE)) != 0)
            {
                ++b;
            }
        }
        ++j;
    } while (j < CareerGoalLevelCount);
    GCareer.CharacterProgress[
        reinterpret_cast<CBruceCareerView *>(GSkater[0])->mSkaterType]
        .NumGold = g;
    GCareer.CharacterProgress[
        reinterpret_cast<CBruceCareerView *>(GSkater[0])->mSkaterType]
        .NumSilver = s;
    GCareer.CharacterProgress[
        reinterpret_cast<CBruceCareerView *>(GSkater[0])->mSkaterType]
        .NumBronze = b;
}

/**
 * Count the five ordinary goal bits in the current level.
 *
 * @param Level Legacy parameter retained by the ABI; GLevel selects the level.
 * @return Number of ordinary goals already awarded.
 */
int Career_LevelTapes(int Level)
{
    (void)Level;
    int t = 0;
    int i = 0;
    do
    {
        if ((GCareer.CharacterProgress[
                 reinterpret_cast<CBruceCareerView *>(GSkater[0])->mSkaterType]
                 .GoalMask[GLevel] >> i) & 1)
        {
            ++t;
        }
        ++i;
    } while (i < CareerOrdinaryGoalCount);
    return t;
}
