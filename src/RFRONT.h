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

#ifdef RFRONT_OWNER_DECLARATIONS
#define CAREERLOADSTATE_OWNER_DECLARATION
#else
#define CAREERLOADSTATE_ABSOLUTE_DECLARATION
#endif
#define CAREERLOADSTATE_DECLARE_HAS_CAREER_LOADED
#define CAREERLOADSTATE_DECLARE_LOADED_CAREER_IS_SAVED
#include "CAREERLOADSTATE.h"

/** Pulse amount used by the name-entry cursor. */
extern "C" int SizeUp RFRONT_SHARED_STATE_DECL_ATTR; // 0x800D7E18

#undef RFRONT_SHARED_STATE_DECL_ATTR

#endif // RFRONT_INC_H

#ifdef RFRONT_DECLARE_SAVE_NAME_ADDRESS_VIEWS
/** Scalar compiler identities for RFRONT's three eleven-byte name buffers. */
namespace RFrontSaveNameViews {
extern "C" {
extern char SaveGameName;
extern char LastCareerName;
extern char LastVideoName;
}
}
#undef RFRONT_DECLARE_SAVE_NAME_ADDRESS_VIEWS
#endif

#ifdef RFRONT_DECLARE_VOID_STRCPY_ABI
/**
 * RFRONT's typed, void-return compiler view of the Psy-Q strcpy import.
 *
 * The general SDK header declares an unprototyped pointer return, which keeps
 * the return register live at these target-proven call sites.
 */
extern "C" void strcpy(char *destination, const char *source);
#undef RFRONT_DECLARE_VOID_STRCPY_ABI
#endif
