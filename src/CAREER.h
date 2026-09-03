// Canonical career progression and persistent-preference interface.
#if (defined(CAREER_LAYOUT_DECLARATIONS) || defined(CAREER_OWNER_LAYOUT)) && \
    !defined(CAREER_HIGH_SCORE_LAYOUT)
#define CAREER_HIGH_SCORE_LAYOUT
#define CAREER_HIGH_SCORE_LAYOUT_WAS_IMPLIED
#endif

#if defined(CAREER_HIGH_SCORE_LAYOUT) && \
    !defined(CAREER_HIGH_SCORE_LAYOUT_EMITTED)
#define CAREER_HIGH_SCORE_LAYOUT_EMITTED

/** One named entry in a level's high-score table. */
struct SHighScore {
    char Name[4];
    char Skater;
    int Score;
};

typedef char SHighScore_size_must_be_12[
    (sizeof(struct SHighScore) == 12) ? 1 : -1];
typedef char SHighScore_Name_offset_must_be_0[
    ((unsigned long)&(((struct SHighScore *)0)->Name) == 0) ? 1 : -1];
typedef char SHighScore_Skater_offset_must_be_4[
    ((unsigned long)&(((struct SHighScore *)0)->Skater) == 4) ? 1 : -1];
typedef char SHighScore_Score_offset_must_be_8[
    ((unsigned long)&(((struct SHighScore *)0)->Score) == 8) ? 1 : -1];

#endif // CAREER_HIGH_SCORE_LAYOUT

#ifdef CAREER_HIGH_SCORE_LAYOUT_WAS_IMPLIED
#undef CAREER_HIGH_SCORE_LAYOUT
#undef CAREER_HIGH_SCORE_LAYOUT_WAS_IMPLIED
#endif

#ifndef CAREER_HIGH_SCORE_LAYOUT_ONLY
#ifndef CAREER_INC_H
#define CAREER_INC_H

/** Medal values stored in each skater's career progress. */
enum EMedalType
{
    MEDAL_GOLD = 5,
    MEDAL_SILVER = 6,
    MEDAL_BRONZE = 7,
    MEDAL_NONE = 8,
};

/** Initialize career storage, scores, and saved preferences. */
void Career_Init(void);

/** Restore default option values. */
void Options_Init(void);

/** Test whether the current skater has the given goal. */
int Career_Got(int Goal);

/** Award a goal tape in the requested level. */
void Career_GiveTape(int Level, int Goal);

/** Award a competition medal in the requested level. */
void Career_GiveMedal(int Level, EMedalType medal);

/** Count ordinary goal tapes earned in the current level. */
int Career_LevelTapes(int Level);

/*
 * The complete save layout is owner-facing. Define this only in translation
 * units that access SCareer fields directly.
 */
#ifdef CAREER_LAYOUT_DECLARATIONS

#include "SCHARACTERPROGRESS.h"

#include "SOPTIONS.h"

struct SCareer {
    int Version;
    int MaxTapes;
    struct SCharacterProgress CharacterProgress[14];
    struct SHighScore HighScore[16][8];
    char LastSkater[2];
    char LastDeck[2];
    char LastWheel[2];
    char LastTrucks[2];
    char LastLevel;
    unsigned short TimeStamp;
    struct SOptions Options;
};

#include "GAMEPREFERENCES.h"

// === Variables (absolute in this TU) ===
#define CAREERLOADSTATE_ABSOLUTE_DECLARATION
#define CAREERLOADSTATE_DECLARE_HAS_CAREER_LOADED
#include "CAREERLOADSTATE.h"
#include "GLEVEL.h"
#define CAREERLOADSTATE_ABSOLUTE_DECLARATION
#define CAREERLOADSTATE_DECLARE_LOADED_CAREER_IS_SAVED
#include "CAREERLOADSTATE.h"
#define SOUNDLEVELS_DECLARE_SFXLEVEL
#define SOUNDLEVELS_DECLARE_XALEVEL
#include "SOUNDLEVELS.h"

#endif // CAREER_LAYOUT_DECLARATIONS

#endif // CAREER_INC_H

#if defined(CAREER_OWNER_LAYOUT) && !defined(CAREER_OWNER_LAYOUT_EMITTED)
#define CAREER_OWNER_LAYOUT_EMITTED

/** Level fields used when counting career goals and medals. */
struct SLevelCareerView {
    char pad_00[12];
    void *isCompetition;
    char pad_10[48];
    int HiScore;
    char pad_44[48];
};

/** Strided view beginning at each level's high-score field. */
struct SLevelHiScoreView {
    int HiScore;
    char pad_04[112];
};

/** Strided view beginning at each level's competition marker. */
struct SLevelCompetitionView {
    void *isCompetition;
    char pad_04[112];
};

/** Read one high-score field while retaining the target's interior base. */
#define CAREER_LEVEL_HIGH_SCORE(levels, index) \
    (reinterpret_cast<SLevelHiScoreView *>(&(levels)[0].HiScore) \
         [index].HiScore)

/** Read one competition marker while retaining the target's interior base. */
#define CAREER_LEVEL_IS_COMPETITION(levels, index) \
    (reinterpret_cast<SLevelCompetitionView *>( \
         &(levels)[0].isCompetition)[index].isCompetition)

/** Skater fields updated when career goals are awarded. */
struct CBruceCareerView {
    char pad_0000[0xda8];
    int mSkaterType;
    char pad_0dac[0x394];
    int mSessionTapes;
};

#include "SCHARACTERPROGRESS.h"

#include "SOPTIONS.h"

/** Complete persistent career-save layout. */
struct SCareerOwner {
    int Version;
    int MaxTapes;
    struct SCharacterProgress CharacterProgress[14];
    struct SHighScore HighScore[16][8];
    char LastSkater[2];
    char LastDeck[2];
    char LastWheel[2];
    char LastTrucks[2];
    char LastLevel;
    char pad_7D1;
    unsigned short TimeStamp;
    struct SOptions Options;
};

typedef char SLevelCareerView_size_must_be_116[
    (sizeof(SLevelCareerView) == 0x74) ? 1 : -1];
typedef char SLevelHiScoreView_size_must_be_116[
    (sizeof(SLevelHiScoreView) == 0x74) ? 1 : -1];
typedef char SLevelCompetitionView_size_must_be_116[
    (sizeof(SLevelCompetitionView) == 0x74) ? 1 : -1];
typedef char CBruceCareerView_size_must_be_4420[
    (sizeof(CBruceCareerView) == 0x1144) ? 1 : -1];
typedef char CareerOwner_SCharacterProgress_size_must_be_32[
    (sizeof(SCharacterProgress) == 0x20) ? 1 : -1];
typedef char CareerOwner_SOptions_size_must_be_416[
    (sizeof(SOptions) == 0x1a0) ? 1 : -1];
typedef char SCareerOwner_size_must_be_2420[
    (sizeof(SCareerOwner) == 0x974) ? 1 : -1];

#endif // CAREER_OWNER_LAYOUT
#endif // CAREER_HIGH_SCORE_LAYOUT_ONLY
