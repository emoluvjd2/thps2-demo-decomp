// Public declarations and owner-local layouts for SFX.cpp.
#ifndef SFX_INC_H
#define SFX_INC_H

struct CVector;

/** Initializes the SPU and the source-owned sound-RAM allocator. */
void SFX_InitAtStart(void);
/** Loads the global VAB and its SFX metadata. */
void SFX_Init(char *filename);
void SFX_SpoolInLevelSFX(const char *name);
void SFX_SpoolInMIDI(const char *name);
void SFX_SpoolOutMIDI(void);
int SFX_OpenVABFile(char *filename, void **soundBuffer);
/** Play a sound effect at equal left/right volume. */
unsigned int SFX_PlayX(
    unsigned int sound, short volume, int pitchOffset);
unsigned int SFX_PlayLR(unsigned int sound, short left, short right,
                        int pitchOffset);
unsigned int SFX_PlayPos(unsigned int sound, const CVector &position,
                         int pitchOffset);
unsigned short SFX_GetVoicePitch(unsigned int voiceBits);
void SFX_SetVoicePitch(unsigned int voiceBits, unsigned short pitch);
void SFX_ModifyPos(unsigned int voiceBits, const CVector &position,
                   int deltaDistance);
void SFX_ModifyVol(unsigned int voiceBits, int left, int right);
void SFX_ResetMIDIPausedPriorToPause(void);
void SFX_LevelStart(void);
void SFX_Pause(void);
void SFX_Unpause(void);
void SFX_StopAll(void);
void SFX_Stop(unsigned int voiceBits);
void SFX_SetReverbType(unsigned char reverbType);
void SFX_ReverbOff(void);
void SFX_SetReverbDepth(unsigned char left, unsigned char right);
void SFX_MIDIFadeOut(int ticks);
void SFX_MIDIStop(void);
void SFX_MIDIFadeIn(int ticks);
void SFX_MIDIChannelMute(unsigned short mute);
void SFX_ProcessingPerFrame(void);
void SFX_ParseSFXFile(char *name, unsigned int *entries,
                      unsigned short *aliases, int maxEntries, int mask);

#ifdef SFX_OWNER_DECLARATIONS

// Primitive aliases retained for the owner's target-era type vocabulary.
#ifdef __cplusplus
extern "C"
{
#endif

    typedef unsigned char u8;
    typedef unsigned short u16;
    typedef unsigned int u32;
    typedef signed char s8;
    typedef signed short s16;
    typedef signed int s32;
    typedef unsigned char uchar;
    typedef unsigned short ushort;
    typedef unsigned int uint;
    typedef unsigned char byte;
    typedef int undefined;
    typedef char undefined1;
    typedef short undefined2;
    typedef int undefined4;
    typedef unsigned long ulong;
    typedef unsigned char undefined3[3];

    // Game vector types
    struct CVector
    {
        int vx, vy, vz;
    };
    struct CSVector
    {
        short vx, vy, vz;
    };
#include "CFRICTION.h"

/* PsyQ owns the exact VabHdr, ProgAtr, and VagAtr layouts used below. */
#include <LIBSND.H>

enum {
    SFX_VOICE_COUNT = 24,
    SFX_VAB_PROGRAM_COUNT = 128,
    SFX_VAB_TONES_PER_PROGRAM = 16,
    SFX_VAB_OFFSET_COUNT = 256,
    SFX_VAB_TONE_TABLE_OFFSET = sizeof(VabHdr) +
                                sizeof(ProgAtr) * SFX_VAB_PROGRAM_COUNT,
    SFX_VAB_OFFSET_TABLE_SIZE = sizeof(unsigned short) *
                                SFX_VAB_OFFSET_COUNT
};

/** Typed view of the fixed VAB header and the variable tone table prefix. */
struct SFXVabImage {
    VabHdr header;
    ProgAtr programs[SFX_VAB_PROGRAM_COUNT];
    VagAtr tones[1];
};

typedef char sfx_VabHdr_size_must_be_32[(sizeof(VabHdr) == 32) ? 1 : -1];
typedef char sfx_ProgAtr_size_must_be_16[(sizeof(ProgAtr) == 16) ? 1 : -1];
typedef char sfx_VagAtr_size_must_be_32[(sizeof(VagAtr) == 32) ? 1 : -1];

struct soundRAMentry {
    unsigned int start;
    unsigned int size;
    void * occupied;
};

/** Per-voice spatial falloff and volume modifier, also written as one word. */
union SFXVoiceSpatialData {
    unsigned int packed;
    struct {
        unsigned short falloff;
        unsigned short volumeModifier;
    } fields;
};

typedef char sfx_spatial_data_size_must_be_4[
    (sizeof(SFXVoiceSpatialData) == 4) ? 1 : -1];

#ifdef __cplusplus
}
#endif

// Canonical PsyQ SPU types and API declarations.
#include <LIBSPU.H>

// === Variables (gp-relative in this TU) ===
extern "C" int SFXPaused;                  // 0x800DA3A0
extern "C" int SFX_ReverbDepthSetTimer;    // 0x800DA3CC
extern "C" int TotalVabUsage;              // 0x800DA38C
extern "C" int exp_flag;                   // 0x800DA3BC
extern "C" int keyoff_queue;               // 0x800DA3B4
extern "C" int keyon_queue;                // 0x800DA3AC
extern "C" int loop_flag;                  // 0x800DA3C0
extern "C" int nko_flag;                   // 0x800DA3A8
extern "C" void *pLevelSpecificSFXVABHeader; // 0x800DA394
extern "C" void *pSFXVABHeader;              // 0x800DA390
extern "C" char reverb_changed;            // 0x800DA3C8
extern "C" int reverb_queue;               // 0x800DA3B0
extern "C" int voicefree_flag;             // 0x800DA3C4
extern "C" int voiceon_flag;               // 0x800DA3B8

// === Variables (absolute in this TU) ===
extern "C" unsigned int SFXArray[];                       // 0x800B9290
extern "C" unsigned short SFXAliasArray[];                 // 0x800B9590
extern "C" unsigned int SFXLevelSpecificArray[];           // 0x800B9610
extern "C" unsigned short SFXLevelSpecificAliasArray[];    // 0x800B9910
extern "C" SFXVoiceSpatialData SFXFalloffArray[];          // 0x800B9990
#define SOUNDLEVELS_DECLARE_SFXLEVEL
#include "SOUNDLEVELS.h"

#endif // SFX_OWNER_DECLARATIONS

#endif // SFX_INC_H
