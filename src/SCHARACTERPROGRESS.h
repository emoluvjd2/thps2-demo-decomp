// Canonical per-skater career-progress layout.
#ifndef SCHARACTERPROGRESS_INC_H
#define SCHARACTERPROGRESS_INC_H

/** Persistent goal, medal, and earnings state for one skater. */
struct SCharacterProgress
{
    char NumTapes;
    char NumBronze;
    char NumSilver;
    char NumGold;
    char GoalMask[8];
    char Medals[9];
    int CareerEarnings;
    int Cash;
};

typedef char SCharacterProgress_size_must_be_32[
    (sizeof(struct SCharacterProgress) == 0x20) ? 1 : -1];
typedef char SCharacterProgress_NumTapes_offset_must_be_0[
    ((unsigned long)&(((struct SCharacterProgress *)0)->NumTapes) == 0)
        ? 1 : -1];
typedef char SCharacterProgress_NumBronze_offset_must_be_1[
    ((unsigned long)&(((struct SCharacterProgress *)0)->NumBronze) == 1)
        ? 1 : -1];
typedef char SCharacterProgress_NumSilver_offset_must_be_2[
    ((unsigned long)&(((struct SCharacterProgress *)0)->NumSilver) == 2)
        ? 1 : -1];
typedef char SCharacterProgress_NumGold_offset_must_be_3[
    ((unsigned long)&(((struct SCharacterProgress *)0)->NumGold) == 3)
        ? 1 : -1];
typedef char SCharacterProgress_GoalMask_offset_must_be_4[
    ((unsigned long)&(((struct SCharacterProgress *)0)->GoalMask) == 4)
        ? 1 : -1];
typedef char SCharacterProgress_Medals_offset_must_be_12[
    ((unsigned long)&(((struct SCharacterProgress *)0)->Medals) == 12)
        ? 1 : -1];
typedef char SCharacterProgress_CareerEarnings_offset_must_be_24[
    ((unsigned long)&(((struct SCharacterProgress *)0)->CareerEarnings) == 24)
        ? 1 : -1];
typedef char SCharacterProgress_Cash_offset_must_be_28[
    ((unsigned long)&(((struct SCharacterProgress *)0)->Cash) == 28)
        ? 1 : -1];

#endif // SCHARACTERPROGRESS_INC_H
