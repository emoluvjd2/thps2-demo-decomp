#ifndef SOPTIONS_INC_H
#define SOPTIONS_INC_H

/** Persistent game and controller options stored with a career save. */
struct SOptions {
    int SFXVolume;
    int MusicVolume;
    int OtherOptions[63];
    int AutoKickEnabled[2];
    int VibrationEnabled[2];
    int ControllerFunction[2][4];
    char LastHighScoreInitials[4];
    int TrickTipUsed[2];
    int TrickTipMask[2];
    int MedalMessagesUsed[14];
    int unused0;
    int TrickTipsEnabled;
    int ScoreDisplayEnabled;
    int HorseLen;
    char HorseWord[16];
};

typedef char SOptions_size_must_be_416[
    (sizeof(struct SOptions) == 416) ? 1 : -1];
typedef char SOptions_SFXVolume_offset_must_be_0[
    ((unsigned long)&(((struct SOptions *)0)->SFXVolume) == 0) ? 1 : -1];
typedef char SOptions_MusicVolume_offset_must_be_4[
    ((unsigned long)&(((struct SOptions *)0)->MusicVolume) == 4) ? 1 : -1];
typedef char SOptions_OtherOptions_offset_must_be_8[
    ((unsigned long)&(((struct SOptions *)0)->OtherOptions) == 8) ? 1 : -1];
typedef char SOptions_AutoKickEnabled_offset_must_be_260[
    ((unsigned long)&(((struct SOptions *)0)->AutoKickEnabled) == 260) ? 1 : -1];
typedef char SOptions_VibrationEnabled_offset_must_be_268[
    ((unsigned long)&(((struct SOptions *)0)->VibrationEnabled) == 268) ? 1 : -1];
typedef char SOptions_ControllerFunction_offset_must_be_276[
    ((unsigned long)&(((struct SOptions *)0)->ControllerFunction) == 276) ? 1 : -1];
typedef char SOptions_LastHighScoreInitials_offset_must_be_308[
    ((unsigned long)&(((struct SOptions *)0)->LastHighScoreInitials) == 308) ? 1 : -1];
typedef char SOptions_TrickTipUsed_offset_must_be_312[
    ((unsigned long)&(((struct SOptions *)0)->TrickTipUsed) == 312) ? 1 : -1];
typedef char SOptions_TrickTipMask_offset_must_be_320[
    ((unsigned long)&(((struct SOptions *)0)->TrickTipMask) == 320) ? 1 : -1];
typedef char SOptions_MedalMessagesUsed_offset_must_be_328[
    ((unsigned long)&(((struct SOptions *)0)->MedalMessagesUsed) == 328) ? 1 : -1];
typedef char SOptions_unused0_offset_must_be_384[
    ((unsigned long)&(((struct SOptions *)0)->unused0) == 384) ? 1 : -1];
typedef char SOptions_TrickTipsEnabled_offset_must_be_388[
    ((unsigned long)&(((struct SOptions *)0)->TrickTipsEnabled) == 388) ? 1 : -1];
typedef char SOptions_ScoreDisplayEnabled_offset_must_be_392[
    ((unsigned long)&(((struct SOptions *)0)->ScoreDisplayEnabled) == 392) ? 1 : -1];
typedef char SOptions_HorseLen_offset_must_be_396[
    ((unsigned long)&(((struct SOptions *)0)->HorseLen) == 396) ? 1 : -1];
typedef char SOptions_HorseWord_offset_must_be_400[
    ((unsigned long)&(((struct SOptions *)0)->HorseWord) == 400) ? 1 : -1];

#endif // SOPTIONS_INC_H
