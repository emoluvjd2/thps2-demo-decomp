// Shared persisted game-state and preference records.
#ifndef SAVEGAME_INC_H
#define SAVEGAME_INC_H

/** Mutable gameplay state embedded in a save-game record. */
struct SGameState
{
    char TrigFileName[9];
    char RestartName[50];
    unsigned char pad1;
    unsigned char pad2;
    unsigned char pad3;
    unsigned char WeaponsGot;
    unsigned char CurrentWeapon;
    int Ammo[8];
    int Health;
    int SmartBombs;
    int Lives;
};

/** Front-end preferences persisted with the gameplay state. */
struct SPreferences
{
    int Difficulty;
    int SFXLevel;
    int MidiLevel;
    int XALevel;
    unsigned char DigitalSelectWeapon;
    unsigned char DigitalCrouch;
    unsigned char DigitalJump;
    unsigned char DigitalSmartBomb;
    unsigned char AnalogueSelectWeapon;
    unsigned char AnalogueCrouch;
    unsigned char AnalogueJump;
    unsigned char AnalogueSmartBomb;
};

/** Complete save-game record passed between the front end and gameplay. */
struct SSaveGame
{
    unsigned int Checksum;
    SGameState State;
    SPreferences Preferences;
};

typedef char SGameState_TrigFileName_offset_must_be_0[
    ((unsigned long)&(((SGameState *)0)->TrigFileName) == 0) ? 1 : -1];
typedef char SGameState_RestartName_offset_must_be_9[
    ((unsigned long)&(((SGameState *)0)->RestartName) == 9) ? 1 : -1];
typedef char SGameState_pad1_offset_must_be_59[
    ((unsigned long)&(((SGameState *)0)->pad1) == 59) ? 1 : -1];
typedef char SGameState_pad2_offset_must_be_60[
    ((unsigned long)&(((SGameState *)0)->pad2) == 60) ? 1 : -1];
typedef char SGameState_pad3_offset_must_be_61[
    ((unsigned long)&(((SGameState *)0)->pad3) == 61) ? 1 : -1];
typedef char SGameState_WeaponsGot_offset_must_be_62[
    ((unsigned long)&(((SGameState *)0)->WeaponsGot) == 62) ? 1 : -1];
typedef char SGameState_CurrentWeapon_offset_must_be_63[
    ((unsigned long)&(((SGameState *)0)->CurrentWeapon) == 63) ? 1 : -1];
typedef char SGameState_Ammo_offset_must_be_64[
    ((unsigned long)&(((SGameState *)0)->Ammo) == 64) ? 1 : -1];
typedef char SGameState_Health_offset_must_be_96[
    ((unsigned long)&(((SGameState *)0)->Health) == 96) ? 1 : -1];
typedef char SGameState_SmartBombs_offset_must_be_100[
    ((unsigned long)&(((SGameState *)0)->SmartBombs) == 100) ? 1 : -1];
typedef char SGameState_Lives_offset_must_be_104[
    ((unsigned long)&(((SGameState *)0)->Lives) == 104) ? 1 : -1];
typedef char SGameState_size_must_be_108[
    (sizeof(SGameState) == 108) ? 1 : -1];

typedef char SPreferences_Difficulty_offset_must_be_0[
    ((unsigned long)&(((SPreferences *)0)->Difficulty) == 0) ? 1 : -1];
typedef char SPreferences_SFXLevel_offset_must_be_4[
    ((unsigned long)&(((SPreferences *)0)->SFXLevel) == 4) ? 1 : -1];
typedef char SPreferences_MidiLevel_offset_must_be_8[
    ((unsigned long)&(((SPreferences *)0)->MidiLevel) == 8) ? 1 : -1];
typedef char SPreferences_XALevel_offset_must_be_12[
    ((unsigned long)&(((SPreferences *)0)->XALevel) == 12) ? 1 : -1];
typedef char SPreferences_DigitalSelectWeapon_offset_must_be_16[
    ((unsigned long)&(((SPreferences *)0)->DigitalSelectWeapon) == 16) ? 1 : -1];
typedef char SPreferences_DigitalCrouch_offset_must_be_17[
    ((unsigned long)&(((SPreferences *)0)->DigitalCrouch) == 17) ? 1 : -1];
typedef char SPreferences_DigitalJump_offset_must_be_18[
    ((unsigned long)&(((SPreferences *)0)->DigitalJump) == 18) ? 1 : -1];
typedef char SPreferences_DigitalSmartBomb_offset_must_be_19[
    ((unsigned long)&(((SPreferences *)0)->DigitalSmartBomb) == 19) ? 1 : -1];
typedef char SPreferences_AnalogueSelectWeapon_offset_must_be_20[
    ((unsigned long)&(((SPreferences *)0)->AnalogueSelectWeapon) == 20) ? 1 : -1];
typedef char SPreferences_AnalogueCrouch_offset_must_be_21[
    ((unsigned long)&(((SPreferences *)0)->AnalogueCrouch) == 21) ? 1 : -1];
typedef char SPreferences_AnalogueJump_offset_must_be_22[
    ((unsigned long)&(((SPreferences *)0)->AnalogueJump) == 22) ? 1 : -1];
typedef char SPreferences_AnalogueSmartBomb_offset_must_be_23[
    ((unsigned long)&(((SPreferences *)0)->AnalogueSmartBomb) == 23) ? 1 : -1];
typedef char SPreferences_size_must_be_24[
    (sizeof(SPreferences) == 24) ? 1 : -1];

typedef char SSaveGame_Checksum_offset_must_be_0[
    ((unsigned long)&(((SSaveGame *)0)->Checksum) == 0) ? 1 : -1];
typedef char SSaveGame_State_offset_must_be_4[
    ((unsigned long)&(((SSaveGame *)0)->State) == 4) ? 1 : -1];
typedef char SSaveGame_Preferences_offset_must_be_112[
    ((unsigned long)&(((SSaveGame *)0)->Preferences) == 112) ? 1 : -1];
typedef char SSaveGame_size_must_be_136[
    (sizeof(SSaveGame) == 136) ? 1 : -1];

#endif // SAVEGAME_INC_H
