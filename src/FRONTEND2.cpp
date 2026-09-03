// FRONTEND2.cpp -- Reconstructed translation unit
// Original: FRONTEND2.CPP
// Functions: 3 reconstructed entries
// Address range: 0x80087600 - 0x80087A08
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#include "globals.h"
#include "PAD.h"
#include "RFRONT.h"
#include "MAIN.h"

struct CBody;
struct CCamera;

struct EditorCamera_alt_definition_0003 {
    struct CClass CClass;
    int Type;
    int Rot;
    int TargetRot;
    int Change;
    struct CBody *CamTri;
    struct CCamera *Camera;
    struct CVector Tripod;
    struct CBody *Target;
};

struct EditorCamera_alt_definition_0004 {
    struct CClass CClass;
    int Type;
    int Rot;
    int TargetRot;
    int Change;
    struct CBody *CamTri;
    struct CCamera *Camera;
    struct CVector Tripod;
    struct CBody *Target;
};

#define FRONTEND2STATE_DECLARE_GAME_MODE
#include "FRONTEND2STATE.h"
#define FRONTEND2STATE_DECLARE_MUSIC_LOOP_CURRENT_VOL
#define FRONTEND2STATE_DECLARE_MUSIC_LOOP_DESIRED_VOL
#include "FRONTEND2STATE.h"
#define FRONTEND2STATE_DECLARE_MUSIC_LOOP_VOICE
#include "FRONTEND2STATE.h"
#define FRONTEND2STATE_DECLARE_SELECTED_LEVEL
#include "FRONTEND2STATE.h"
#define FRONTEND2STATE_DECLARE_TWO_PLAYER
#include "FRONTEND2STATE.h"
#define FRONTEND2STATE_DECLARE_LAST_SCREEN
#define FRONTEND2STATE_DECLARE_WHICH_SCREEN
#include "FRONTEND2STATE.h"
#define DEMOTIMER_ABSOLUTE_DECLARATION
#include "DEMOTIMER.h"
#define HIRESMODE_ABSOLUTE_DECLARATION
#include "HIRESMODE.h"

#include "SFX.h"
#include "CAREER.h"
#include "HIRES.h"
#include "INIT.h"
#include "LEVELEDITOR.h"
#include "FRONT.h"
#include "SPOOLER.h"

/** Static generated-level lifetime interface used by the front end. */
struct LevelGenerator
{
    /** Releases the shared generated-level output. */
    static void UnloadOutPSX(bool kill_buffers);
};

#include "PREAPI.h"
#include "DB.h"
#include "FRONTSCREENS.h"
#include "CREATE.h"
#include "GENERATELEVEL.h"

/** Default VAB loaded whenever control returns to the front end. */
static char SFXDataPath[] __attribute__((section(".sdata"))) = "shell.vab";

// === Free function declarations ===
#include "FRONTEND2.h"
void LaunchTheDamnGame(int selected_level, int mode);

// === Function implementations (address order) ===

/**
 * FrontEnd2_Main__Fv
 *
 * Address:   0x80087600
 * Mangled:   FrontEnd2_Main__Fv
 * Demangled: FrontEnd2_Main(void)
 *
 * Runs the shell/frontend screen loop and launches gameplay or submodes
 * when the selected screen requests them.
 */
void FrontEnd2_Main(void) {
    WhichScreen = 0;
    FrontEnd2_GameMode = 2;
    FrontEnd2_SelectedLevel = 0;
    FrontEnd2_TwoPlayer = 0;
    GenerateLevel = 0;

    SFX_Init(SFXDataPath);
    FrontEnd2_MusicLoopCurrentVol = 0;
    FrontEnd2_MusicLoopDesiredVol = 0x4000;
    FrontEnd2_MusicLoopVoice = SFX_PlayX(0x9d, (short)FrontEnd2_MusicLoopCurrentVol, 0);
    PRE_Init();
    Spooler::Init();
    Create_InitSystem();

    for (;;) {
        int choice;

        switch (WhichScreen) {
        case 0:
            LastScreen = 0;
            choice = MainMenu_Main();
            switch (choice) {
            case 0x20:
                FrontEnd2_GameMode = 2;
                FrontEnd2_TwoPlayer = 0;
                WhichScreen = 1;
                break;
            case 0x21:
                FrontEnd2_GameMode = 1;
                FrontEnd2_TwoPlayer = 0;
                WhichScreen = 1;
                break;
            case 0x22:
                FrontEnd2_TwoPlayer = 0;
                WhichScreen = 4;
                break;
            case 0x23:
                WhichScreen = 3;
                break;
            case 0x26:
                WhichScreen = 2;
                break;
            case 0x27:
                FrontEnd2_TwoPlayer = 1;
                WhichScreen = 9;
                break;
            case 0x28:
                FrontEnd2_GameMode = 3;
                FrontEnd2_TwoPlayer = 0;
                WhichScreen = 1;
                break;
            default:
                break;
            }
            break;

        case 1:
            LastScreen = 1;
            choice = PlayerSelect_Main(FrontEnd2_GameMode);
            if (choice != -1) {
                WhichScreen = 2;
            } else {
                if (FrontEnd2_TwoPlayer != 0) {
                    WhichScreen = 9;
                } else {
                    WhichScreen = 0;
                }
            }
            break;

        case 2:
            choice = LevelSelect2_Main();
            if (choice != -1) {
                WhichScreen = 5;
                FrontEnd2_SelectedLevel = choice;
            } else {
                WhichScreen = LastScreen;
            }
            break;

        case 3:
            Create_Main();
            WhichScreen = 0;
            break;

        case 4:
            FrontEnd2_MusicLoopDesiredVol = 0;
            LevelEditor_Main();
            FrontEnd2_MusicLoopDesiredVol = 0x4000;
            if (FrontEnd2_MusicLoopVoice != 0) {
                SFX_Stop(FrontEnd2_MusicLoopVoice);
            }
            FrontEnd2_MusicLoopVoice = SFX_PlayX(0x9d, (short)FrontEnd2_MusicLoopCurrentVol, 0);
            if (GenerateLevel != 0) {
                FrontEnd2_GameMode = 3;
                FrontEnd2_TwoPlayer = 0;
                FrontEnd2_SelectedLevel = 0;
                WhichScreen = 5;
            } else {
                WhichScreen = 0;
            }
            break;

        case 5:
            FrontEnd2_MusicLoopDesiredVol = 0;
            LaunchTheDamnGame(FrontEnd2_SelectedLevel, FrontEnd2_GameMode);
            if (GenerateLevel != 0) {
                WhichScreen = 4;
            } else {
                WhichScreen = 0;
            }
            break;

        case 9:
            LastScreen = 9;
            choice = MultiSelect_Main(&FrontEnd2_GameMode);
            if (choice == -1) {
                WhichScreen = 0;
            } else {
                if (FrontEnd2_GameMode == 10) {
                    WhichScreen = 6;
                } else {
                    WhichScreen = 1;
                }
            }
            break;

        default:
            break;
        }
    }
}


/**
 * LaunchTheDamnGame__Fii
 *
 * Address:   0x800878CC
 * Mangled:   LaunchTheDamnGame__Fii
 * Demangled: LaunchTheDamnGame(int, int)
 *
 * @param selected_level  Level to load and launch.
 * @param mode  Game mode assigned to the new session.
 */
void LaunchTheDamnGame(int selected_level, int mode) {
    PRE_Cleanup();
    Spooler::Cleanup();

    char *pad = reinterpret_cast<char *>(Pad);
    pad[1] = 0;
    pad[0x16d] = 0;
    pad[0] = 0;
    pad[0x16c] = 0;

    Front_ClearGVideoRestartData();
    ClearSoftReset();
    Career_Init();

    DemoTimer = 1000;
    GGame = mode;
    int players = 2;
    if ((unsigned int)(mode - 1) < 3)
        players = 1;
    GNumberOfPlayers = players;
    HiresMode = 0;

    LoadingScreen_SetCurrentLevel(selected_level, 1, 0);
    kickIt(selected_level, SelectedSkater);

    if (GenerateLevel != 0)
        LevelGenerator::UnloadOutPSX(false);

    Init_Restart(PACK512x240, 16);
    Db_Init();
    PRE_Init();
    Spooler::Init();
    SFX_Init(SFXDataPath);

    unsigned int voice = FrontEnd2_MusicLoopVoice;
    FrontEnd2_MusicLoopCurrentVol = 0;
    FrontEnd2_MusicLoopDesiredVol = 0x4000;
    if (voice != 0)
        SFX_Stop(voice);

    FrontEnd2_MusicLoopVoice = SFX_PlayX(0x9d, (short)FrontEnd2_MusicLoopCurrentVol, 0);
}


/**
 * FrontEnd2_UpdateMusicLoop__Fv
 *
 * Address:   0x80087A08
 * Mangled:   FrontEnd2_UpdateMusicLoop__Fv
 * Demangled: FrontEnd2_UpdateMusicLoop(void)
 *
 * Gradually moves the music-loop volume toward its requested level, stopping
 * the voice after a completed fade-out.
 */
void FrontEnd2_UpdateMusicLoop(void) {
    int voice = FrontEnd2_MusicLoopVoice;
    if (voice == 0) return;
    int cur = FrontEnd2_MusicLoopCurrentVol;
    int des = FrontEnd2_MusicLoopDesiredVol;
    if (cur == des) {
        if (des != 0) return;
        SFX_Stop(voice);
        FrontEnd2_MusicLoopVoice = 0;
        return;
    }
    if (cur < des) {
        int new_cur = cur + 0x88;
        FrontEnd2_MusicLoopCurrentVol = new_cur;
        if (des < new_cur) {
            FrontEnd2_MusicLoopCurrentVol = des;
        }
    } else if (des < cur) {
        int new_cur = cur - 0x88;
        FrontEnd2_MusicLoopCurrentVol = new_cur;
        if (new_cur < des) {
            FrontEnd2_MusicLoopCurrentVol = des;
        }
    } else {
        return;
    }
    int vol = FrontEnd2_MusicLoopCurrentVol;
    SFX_ModifyVol(voice, vol, vol);
}
