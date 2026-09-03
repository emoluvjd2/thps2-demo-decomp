#if defined(STATS_BOARD_LAYOUT_ONLY) && !defined(STATS_BOARD_LAYOUT)
#error STATS_BOARD_LAYOUT_ONLY requires STATS_BOARD_LAYOUT
#endif

#if !defined(STATS_BOARD_LAYOUT_ONLY) && \
    !defined(THPS2_PSX_PROTO_STATS_H)
#define THPS2_PSX_PROTO_STATS_H
#define STATS_ORDINARY_LAYOUT_ACTIVE

/** Fixed table and embedded-array lengths recovered from MAIN.SYM layouts. */
enum {
    HawkBioLineCount = 11,
    MuskaBioLineCount = 17,
    UnavailableBioLineCount = 5,
    SkaterStatsCount = 14,
    SkaterBoardCount = 8,
    SkaterAbilityCount = 12,
    LevelStatsCount = 10,
};

/** Pointer-sized boolean representation recorded for the level flags. */
typedef void *SLevelFlag;

#ifndef STATS_BOARD_LAYOUT
#define STATS_BOARD_LAYOUT
#define STATS_BOARD_LAYOUT_IMPLIED_BY_ORDINARY
#endif

#endif // !STATS_BOARD_LAYOUT_ONLY && !THPS2_PSX_PROTO_STATS_H

#if defined(STATS_BOARD_LAYOUT) && !defined(STATS_BOARD_LAYOUT_EMITTED)
#define STATS_BOARD_LAYOUT_EMITTED

/**
 * One selectable deck and its gameplay-stat bonuses.
 *
 * The layout is recovered from `original/MAIN_STRUCTS.h`; the original
 * compiler gives this type a size of 32 bytes.
 */
struct SBoard
{
    char *BoardTexture;
    char *BoardName;
    char TapesToUnlock;
    int ollie;
    int speed;
    int air;
    int balance;
    int turning;
};

typedef char SBoard_size_must_be_32[
    (sizeof(struct SBoard) == 32) ? 1 : -1];
typedef char SBoard_BoardTexture_offset_must_be_0[
    ((unsigned long)&(((struct SBoard *)0)->BoardTexture) == 0) ? 1 : -1];
typedef char SBoard_BoardName_offset_must_be_4[
    ((unsigned long)&(((struct SBoard *)0)->BoardName) == 4) ? 1 : -1];
typedef char SBoard_TapesToUnlock_offset_must_be_8[
    ((unsigned long)&(((struct SBoard *)0)->TapesToUnlock) == 8) ? 1 : -1];
typedef char SBoard_ollie_offset_must_be_12[
    ((unsigned long)&(((struct SBoard *)0)->ollie) == 12) ? 1 : -1];
typedef char SBoard_speed_offset_must_be_16[
    ((unsigned long)&(((struct SBoard *)0)->speed) == 16) ? 1 : -1];
typedef char SBoard_air_offset_must_be_20[
    ((unsigned long)&(((struct SBoard *)0)->air) == 20) ? 1 : -1];
typedef char SBoard_balance_offset_must_be_24[
    ((unsigned long)&(((struct SBoard *)0)->balance) == 24) ? 1 : -1];
typedef char SBoard_turning_offset_must_be_28[
    ((unsigned long)&(((struct SBoard *)0)->turning) == 28) ? 1 : -1];

#endif // STATS_BOARD_LAYOUT && !STATS_BOARD_LAYOUT_EMITTED

#ifdef STATS_BOARD_LAYOUT_IMPLIED_BY_ORDINARY
#undef STATS_BOARD_LAYOUT_IMPLIED_BY_ORDINARY
#undef STATS_BOARD_LAYOUT
#endif

#ifdef STATS_ORDINARY_LAYOUT_ACTIVE

/**
 * Front-end identity, base abilities, and available decks for one skater.
 *
 * The original array uses fourteen 344-byte records.
 */
struct SStats
{
    char *PSXName;
    char *PSXName2;
    char *FrontName;
    int ComboID;
    unsigned char Board;
    unsigned char Wheel1;
    unsigned char Wheel2;
    unsigned char Head;
    char Neck;
    int Toughness;
    int Speed;
    int Air;
    int Balance;
    int Street;
    char *Name;
    unsigned int BoardChecksum;
    unsigned int WheelChecksum;
    unsigned int TruckChecksum;
    SBoard Boards[SkaterBoardCount];
    unsigned char Ability[SkaterAbilityCount];
    char *PortraitName;
    char **BioText;
    int Available;
    int FrontEndOrientation;
};

/**
 * Static presentation and gameplay settings for one level.
 *
 * The layout is recovered from `original/MAIN_STRUCTS.h`; each record is
 * 116 bytes.
 */
struct SLevel
{
    char *pLevelName;
    char *pCityName;
    char *pVab;
    SLevelFlag isCompetition;
    SLevelFlag isLocked;
    char *pTRGName;
    unsigned int GoldScore;
    unsigned int SilverScore;
    unsigned int BronzeScore;
    char *pImageName;
    char STR;
    int map_loc[2];
    char *pGoalsImage;
    int TapesToUnlock;
    int MedalsToUnlock;
    int HiScore;
    int ProScore;
    int Crowd;
    int ExtraPolyBuffer[2];
    int TrickTipMask[2];
    int FMV;
    int IntroStartPosX;
    int IntroStartPosY;
    int IntroStartPosZ;
    short IntroStartAngX;
    short IntroStartAngY;
    short IntroStartAngZ;
};

typedef char SStats_size_must_be_344[(sizeof(SStats) == 344) ? 1 : -1];
typedef char SLevel_size_must_be_116[(sizeof(SLevel) == 116) ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif

extern char *HawkBio[HawkBioLineCount];
extern char *MuskaBio[MuskaBioLineCount];
extern char *MullenBio[UnavailableBioLineCount];
extern char *BurnquistBio[UnavailableBioLineCount];
extern char *RowleyBio[UnavailableBioLineCount];
extern char *LasekBio[UnavailableBioLineCount];
extern char *CampbellBio[UnavailableBioLineCount];
extern char *ReynoldsBio[UnavailableBioLineCount];
extern char *GlifbergBio[UnavailableBioLineCount];
extern char *ThomasBio[UnavailableBioLineCount];
extern char *SteamerBio[UnavailableBioLineCount];
extern char *CaballeroBio[UnavailableBioLineCount];
extern char *KostonBio[UnavailableBioLineCount];
extern char *CustomBio[UnavailableBioLineCount];

#define STATSTABLE_WITHIN_C_LINKAGE
#include "STATSTABLE.h"
#ifdef STATS_LEVELS_READ_ONLY
#define STATS_LEVELS_DECL_CONST const
#else
#define STATS_LEVELS_DECL_CONST
#endif
extern STATS_LEVELS_DECL_CONST SLevel Levels[LevelStatsCount];
#undef STATS_LEVELS_DECL_CONST

#ifdef __cplusplus
}
#endif

#undef STATS_ORDINARY_LAYOUT_ACTIVE
#endif // STATS_ORDINARY_LAYOUT_ACTIVE
