// CROWD.cpp -- Reconstructed translation unit
// Original: CROWD.CPP
// Functions: 9
// Address range: 0x80020AAC - 0x80021224
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

#include "MAIN.h"

/** Runtime state for the single ambient crowd voice. */
struct SCrowd {
    int Voice;
    int VolL;
    int VolR;
    int TargetL;
    int TargetR;
    int Pitch;
};

#define CROWD_OWNER_DECLARATIONS
#include "CROWD.h"
#undef CROWD_OWNER_DECLARATIONS
#include "GLEVEL.h"

#include "SFX.h"
#include "STATS.h"
#include "UTILS.h"

SCrowd Crowd[1] __attribute__((section(".data"))) = {};

/** Debug trace used when starting the crowd voice. */
static char CrowdPitchFormat[16]
    __attribute__((section(".sdata"))) = "Pitch = %d \n";

#include <STDIO.H>

// === Owner-local function declarations ===
void Crowd_Boo(int volume);


// === Function implementations (address order) ===

/** Initializes the stateless crowd-audio subsystem. */
void Crowd_Init(void)
{
}

/** Starts and clears the crowd ambience voice for a supported game level. */
void Crowd_Start(void)
{
    int i;

    if ((Levels[GLevel].Crowd != 0) && (GGame == 1))
    {
        Crowd_Bored = 0;
        for (i = 0; i < 1; i++)
        {
            Crowd[i].Voice = SFX_PlayX(0x2c, 0, 0);
            printf(CrowdPitchFormat,
                   (int)SFX_GetVoicePitch(Crowd[i].Voice));
            Crowd[i].VolL = Crowd[i].TargetL = Crowd[i].TargetR = Crowd[i].VolR = 0;
        }
    }
}

/** Stops every active crowd ambience voice. */
void Crowd_Stop(void)
{
    int i;

    if ((Levels[GLevel].Crowd != 0) && (i = 0, GGame == 1))
    {
        do
        {
            if (Crowd[i].Voice != 0)
            {
                SFX_Stop(Crowd[i].Voice);
            }
            Crowd[i].Voice = 0;
            i = i + 1;
        } while (i < 1);
    }
}

/**
 * Moves a scalar toward a target without overshooting it.
 *
 * @param Value Value updated in place.
 * @param Target Desired value.
 * @param Step Maximum change for this update.
 * @return Nonzero when the input had not yet reached the target.
 */
int Crowd_MoveTowards(int &Value, int Target, int Step)
{
    if (Value == Target)
        return 0;
    if (Value > Target - Step && Value < Target + Step)
    {
        Value = Target;
    }
    else if (Value < Target)
    {
        Value = Value + Step;
    }
    else
    {
        Value = Value - Step;
    }
    return 1;
}

/** Updates crowd voice panning and triggers boredom-driven boos. */
void Crowd_Update(void)
{
    int i;

    if ((Levels[GLevel].Crowd != 0) && (GGame == 1))
    {
        for (i = 0; i < 1; i++)
        {
            if (Crowd[i].Voice != 0)
            {
                if (Crowd_MoveTowards(Crowd[i].VolL, Crowd[i].TargetL, 100) ||
                    Crowd_MoveTowards(Crowd[i].VolR, Crowd[i].TargetR, 100))
                {
                    SFX_ModifyVol(Crowd[i].Voice, (Crowd[i].VolL * 2) / 3,
                                  (Crowd[i].VolR * 2) / 3);
                }
                else if (Rnd(30) == 0)
                {
                    Crowd[i].TargetL = Rnd(0x2000);
                    Crowd[i].TargetR = Rnd(0x2000);
                }
            }
        }
        if ((Crowd_Bored != 0) && (Rnd(50) == 0))
        {
            Crowd_Boo(Crowd_Bored << 0xd);
            Crowd_Bored--;
        }
    }
}

/**
 * Plays a positive crowd reaction, with competition-specific layering.
 *
 * @param Score Score awarded by the current trick.
 */
void Crowd_Cheer(int Score)
{
    if (((Score != 0) && (Levels[GLevel].Crowd != 0)) && (GNumberOfPlayers < 2))
    {
        if (Levels[GLevel].isCompetition == 0)
        {
            if (Score < 0x3a99)
            {
                return;
            }
            int Volume = (Score << 0xb) / 0x186a;
            if (0x4000 < Volume)
            {
                Volume = 0x4000;
            }
            SFX_PlayX(0xde, (short)Volume, 0);
            return;
        }
        if (Crowd_Bored != 0)
        {
            if (Score < 100)
            {
                goto reduce_boredom;
            }
            Crowd_Bored = 0;
            goto update_volume;
        reduce_boredom:
            Crowd_Bored--;
        }
    update_volume:
        int Volume = (Score << 0xe) / 1000;
        if (0x4000 < Volume)
        {
            Volume = 0x4000;
        }
        if (Volume >= 0x801)
        {
            short vshort = (short)Volume;
            SFX_PlayX(0x30, vshort, 0);
            SFX_PlayX(0x2e, vshort, 0);
            if (Score >= 0x1f40)
            {
                SFX_PlayX(0x31, vshort, 0);
            }
            return;
        }
        else
        {
            if (Score < 400)
            {
                return;
            }
            SFX_PlayX(0x2e, (short)(Volume << 2), 0);
            return;
        }
    }
}

/**
 * Plays the crowd's injury reaction and increments its boredom state.
 *
 * @param Speed Bail speed used to scale the reaction volume.
 */
void Crowd_Oww(int Speed)
{
    int Volume;
    if ((Levels[GLevel].Crowd != 0) && (GGame == 1))
    {
        Crowd_Bored = Crowd_Bored + 1;
        Volume = (Speed << 0xe) / 0x50;
        if (0x4000 < Volume)
        {
            Volume = 0x4000;
        }
        if (Volume < 0x800)
        {
            Volume = 0x800;
        }
        SFX_PlayX(0x31, (short)Volume, 0);
    }
}

/**
 * Plays a stronger crowd reaction and updates competition boredom.
 *
 * @param Score Score awarded by the current trick.
 */
void Crowd_Ahh(int Score)
{
    int Volume;

    if ((Levels[GLevel].Crowd != 0) && (GNumberOfPlayers < 2))
    {
        if (Levels[GLevel].isCompetition == 0)
        {
            int Volume;
            if (Score < 0x3a99)
            {
                return;
            }
            Volume = (Score << 0xb) / 0x186a;
            if (0x4000 < Volume)
            {
                Volume = 0x4000;
            }
            SFX_PlayX(0xdf, (short)Volume, 0);
            return;
        }
        if (Score < 100)
        {
            Crowd_Bored = Crowd_Bored + 2;
        }
        else
        {
            Crowd_Bored = 0;
        }
        Volume = (Score << 0xe) / 1000;
        if (0x4000 < Volume)
        {
            Volume = 0x4000;
        }
        if (Volume < 0x1000)
        {
            Volume = 0x1000;
        }
        else
        {
            SFX_PlayX(0x2e, (short)((unsigned int)((Volume + ((unsigned int)Volume >> 0x1f)) * 0x8000) >> 0x10),
                      0);
        }
        SFX_PlayX(0x2f, (short)Volume, 0);
    }
}

/**
 * Emits a randomized stereo boo while crowd ambience is active.
 *
 * @param Volume Base reaction volume before left/right randomization.
 */
void Crowd_Boo(int Volume)
{
    short vl;
    short vr;
    int iVar1;
    int iVar2;

    if ((Levels[GLevel].Crowd != 0) && (GGame == 1))
    {
        iVar1 = Rnd(0x14);
        vl = (short)((Volume * (iVar1 + 0x5a)) / 100);
        iVar2 = Rnd(0x14);
        vr = (short)((Volume * (iVar2 + 0x5a)) / 100);
        SFX_PlayLR(0x32, vl, vr, 0);
    }
}
