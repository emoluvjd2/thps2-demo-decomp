#ifndef RFRONT_INC_H
#define RFRONT_INC_H

#ifdef RFRONT_OWNER_DECLARATIONS
#define RFRONT_SHARED_STATE_DECL_ATTR
#else
#define RFRONT_SHARED_STATE_DECL_ATTR __attribute__((section(".data")))
#endif

/** Replay state restored from a saved video header. */
struct SVideoRestart
{
    int NumSkaters;
    int Skater;
    int Skater2;
    int Level;
    int GGame;
};

typedef char SVideoRestart_size_must_be_20[
    (sizeof(SVideoRestart) == 20) ? 1 : -1];
typedef char SVideoRestart_NumSkaters_offset_must_be_0[
    ((unsigned long)&(((SVideoRestart *)0)->NumSkaters) == 0) ? 1 : -1];
typedef char SVideoRestart_Skater_offset_must_be_4[
    ((unsigned long)&(((SVideoRestart *)0)->Skater) == 4) ? 1 : -1];
typedef char SVideoRestart_Skater2_offset_must_be_8[
    ((unsigned long)&(((SVideoRestart *)0)->Skater2) == 8) ? 1 : -1];
typedef char SVideoRestart_Level_offset_must_be_12[
    ((unsigned long)&(((SVideoRestart *)0)->Level) == 12) ? 1 : -1];
typedef char SVideoRestart_GGame_offset_must_be_16[
    ((unsigned long)&(((SVideoRestart *)0)->GGame) == 16) ? 1 : -1];

/** Replay restart descriptor shared by gameplay and the front end. */
extern "C" SVideoRestart GVideoRestart RFRONT_SHARED_STATE_DECL_ATTR; // 0x800B5138

/** Index of the skater selected by the front end. */
extern "C" int SelectedSkater RFRONT_SHARED_STATE_DECL_ATTR; // 0x800D7E44

/** Whether the loaded career has already been saved. */
extern "C" int GLoadedCareerIsSaved RFRONT_SHARED_STATE_DECL_ATTR; // 0x800D7E04

/** Pulse amount used by the name-entry cursor. */
extern "C" int SizeUp RFRONT_SHARED_STATE_DECL_ATTR; // 0x800D7E18

#undef RFRONT_SHARED_STATE_DECL_ATTR

#endif // RFRONT_INC_H
