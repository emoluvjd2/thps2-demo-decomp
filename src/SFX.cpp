// SFX.cpp -- Reconstructed translation unit
// Original: SFX.CPP
// Functions: 33
// Address range: 0x8004BF64 - 0x8004D4B0
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#define SFX_OWNER_DECLARATIONS
#include "SFX.h"
#undef SFX_OWNER_DECLARATIONS
#include "MEM.h"
#include "REDBOOK.h"
#include "UTILS.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include <INLINE_C.H>

// === Other owner APIs ===
#include "VIDEO.h"
#include "FILEIO.h"
// === Source-owned sound tables ===
/** Global and level-local sound definitions populated from VAB files. */
u32 SFXArray[192] __attribute__((section(".data"))) = {0};
u16 SFXAliasArray[64] __attribute__((section(".data"))) = {0};
u32 SFXLevelSpecificArray[192]
    __attribute__((section(".data"))) = {0};
u16 SFXLevelSpecificAliasArray[64]
    __attribute__((section(".data"))) = {0};
SFXVoiceSpatialData SFXFalloffArray[SFX_VOICE_COUNT]
    __attribute__((section(".data"))) = {0};

/**
 * Source-owned sound-RAM management workspace.
 *
 * The original initialized-data image stores all 136 bytes as zero.
 */
char SoundRamMemoryManagementTable[136]
    __attribute__((section(".data"))) = {0};

/**
 * Complete SFX small-data state in target order.
 *
 * The two VAB identifiers and sound-RAM usage counter are owner-local in
 * MAIN.SYM; the remaining entries are public cross-owner interfaces.
 * Explicit storage prevents zero-valued entries from degrading into unordered
 * common symbols.
 */
int TotalVabUsage = 0;
void *pSFXVABHeader = 0;
void *pLevelSpecificSFXVABHeader = 0;
static int SFXVABId = -1;
static int LevelSpecificSFXVABId = -1;
int SFXPaused = 0;
static int soundRAMOccupied = 0;
int nko_flag = 0;
int keyon_queue = 0;
int reverb_queue = 0x00ffffff;
int keyoff_queue = 0;
int voiceon_flag = 0;
int exp_flag = 0;
int loop_flag = 0;
int voicefree_flag = 0x00ffffff;
char reverb_changed = 0;
int SFX_ReverbDepthSetTimer = 0;

/** Shared scratch scalar used by baddy sound playback. */
int SFX_v = 0;

/**
 * Owner-private per-voice state and sound-RAM allocation records.
 *
 * These arrays are zero-initialized in the original SFX BSS contribution.
 */
static u16 voicePitch[SFX_VOICE_COUNT]
    __attribute__((section(".bss")));
static u16 voiceVolume[SFX_VOICE_COUNT]
    __attribute__((section(".bss")));
static soundRAMentry soundRAMSlots[4]
    __attribute__((section(".bss")));
static u16 pitchPriorToPause[SFX_VOICE_COUNT]
    __attribute__((section(".bss")));

// === $gp-relative globals ===
// nko_flag, keyon_queue, reverb_queue, keyoff_queue, voiceon_flag, exp_flag,
// loop_flag, voicefree_flag, and reverb_changed are declared in SFX.h and
// owned by the complete small-data block above.
static unsigned int playSFX(unsigned int sfx, unsigned char pitch, short vl, short vr, int pitch_offset, unsigned short volMod);
static int allocateVoice(void);
static unsigned int translateLevelSpecificAliasToIndex(unsigned short alias);
static unsigned int translateGlobalAliasToIndex(unsigned short alias);
static unsigned int sfxMalloc(unsigned int size);

// === Function implementations (address order) ===

/**
 * SFX_InitAtStart__Fv
 *
 * Address:   0x8004BF64
 * Mangled:   SFX_InitAtStart__Fv
 * Demangled: SFX_InitAtStart(void)
 */
void SFX_InitAtStart(void) {
    SpuCommonAttr attr;

    SpuInit();

    int maxSize = 0x0007EFF0;
    int i = 1;

    soundRAMSlots[0].start = 0x1010;
    soundRAMSlots[0].size = maxSize;
    soundRAMSlots[0].occupied = 0;

    do {
        soundRAMSlots[i].size = 0;
        soundRAMSlots[i].start = 0;
        soundRAMSlots[i].occupied = 0;
        i++;
    } while (i < 4);

    attr.mask = SPU_COMMON_MVOLL | SPU_COMMON_MVOLR |
                SPU_COMMON_CDVOLL | SPU_COMMON_CDVOLR |
                SPU_COMMON_CDREV | SPU_COMMON_CDMIX;
    attr.mvol.left = 0x3FFF;
    attr.mvol.right = 0x3FFF;
    attr.cd.volume.left = 0x7F00;
    attr.cd.volume.right = 0x7F00;
    attr.cd.reverb = 1;
    attr.cd.mix = 1;
    SpuSetCommonAttr(&attr);
}


/**
 * SFX_Init__FPc
 *
 * Address:   0x8004C00C
 * Mangled:   SFX_Init__FPc
 * Demangled: SFX_Init(char *)
 *
 * @param filename  Filename string.
 */
void SFX_Init(char * filename) {
    SFX_SetReverbType(0);
    SFXVABId = SFX_OpenVABFile(filename, &pSFXVABHeader);
    SFX_ParseSFXFile(filename, SFXArray, SFXAliasArray, 64, 0);
    int i = 0;
    do {
        i++;
    } while (i < 64);
    i--;
}


/**
 * SFX_SpoolInLevelSFX__FPCc
 *
 * Address:   0x8004C07C
 * Mangled:   SFX_SpoolInLevelSFX__FPCc
 * Demangled: SFX_SpoolInLevelSFX(const char *)
 *
 * Copies the level name and appends the VAB filename extension before loading
 * the level-specific sound bank and its alias table.
 *
 * @param p_name Level name without an extension.
 */
void SFX_SpoolInLevelSFX(const char * p_name) {
    char filename[20];
    int i = 0;
    char *out = filename;

    goto copy_test;
copy_next:
    i++;
copy_test:
    if (*p_name == '\0') {
        goto copy_done;
    }
    out[i] = *p_name;
    p_name++;
    goto copy_next;
copy_done:
    out[i++] = '.';
    out[i++] = 'V';
    out[i++] = 'A';
    out[i++] = 'B';
    out[i] = '\0';

    // Separating filename construction from the calls is load-bearing:
    // removing this boundary produces 23 real mismatches in this function.
    asm volatile("");
    LevelSpecificSFXVABId = SFX_OpenVABFile(filename,
                                            &pLevelSpecificSFXVABHeader);
    SFX_ParseSFXFile(filename, SFXLevelSpecificArray,
                     SFXLevelSpecificAliasArray, 64,
                     0x40000000);
}


/**
 * SFX_SpoolInMIDI__FPCc
 *
 * Address:   0x8004C138
 * Mangled:   SFX_SpoolInMIDI__FPCc
 * Demangled: SFX_SpoolInMIDI(const char *)
 *            jr $ra; nop — empty void function
 *
 * @param p_name  MIDI asset name; retained for the no-op prototype implementation.
 */
void SFX_SpoolInMIDI(const char * p_name) {
}


/**
 * SFX_SpoolOutMIDI__Fv
 *
 * Address:   0x8004C140
 * Mangled:   SFX_SpoolOutMIDI__Fv
 * Demangled: SFX_SpoolOutMIDI(void)
 *            jr $ra; nop — empty void function
 */
void SFX_SpoolOutMIDI(void) {
}


/**
 * SFX_OpenVABFile__FPcPPv
 *
 * Address:   0x8004C148
 * Mangled:   SFX_OpenVABFile__FPcPPv
 * Demangled: SFX_OpenVABFile(char *, void **)
 *
 * @param filename  Filename string.
 * @param p_soundbuffer  Receives the retained VAB header and tone table.
 * @return 1 after the VAB image has been loaded and transferred.
 */
int SFX_OpenVABFile(char * filename, void ** p_soundbuffer) {
    int vab_size = FileIO_Open(filename);
    unsigned char *p_buffer =
        (unsigned char *)Mem_New(vab_size + 0x40, 1, 1);
    FileIO_Load(p_buffer);
    FileIO_Sync();

    VabHdr *header = (VabHdr *)p_buffer;
    unsigned int waveform_data_size = header->fsize;
    int headsize = header->ps * SFX_VAB_TONES_PER_PROGRAM * sizeof(VagAtr) +
                   SFX_VAB_TONE_TABLE_OFFSET + SFX_VAB_OFFSET_TABLE_SIZE;
    waveform_data_size -= headsize;
    waveform_data_size += 0x3F;
    waveform_data_size &= ~0x3F;

    int transfer_address = sfxMalloc(waveform_data_size);

    SpuSetTransferStartAddr(transfer_address);
    SpuWrite(p_buffer + headsize, waveform_data_size);
    SpuIsTransferCompleted(SPU_TRANSFER_WAIT);

    unsigned short *p_offset =
        (unsigned short *)(p_buffer + SFX_VAB_TONE_TABLE_OFFSET +
                           header->ps * SFX_VAB_TONES_PER_PROGRAM *
                           sizeof(VagAtr));
    unsigned int address = transfer_address;
    int w = 0;
    do {
        int size = *p_offset;
        *p_offset = (unsigned short)(address >> 3);
        p_offset++;
        w++;
        address += size << 3;
    } while (w < SFX_VAB_OFFSET_COUNT);

    header->fsize = transfer_address;
    Mem_Shrink(p_buffer, headsize);

    TotalVabUsage += headsize;
    *p_soundbuffer = p_buffer;

    return 1;
}


/**
 * SFX_PlayX__FUisi
 *
 * Address:   0x8004C258
 * Mangled:   SFX_PlayX__FUisi
 * Demangled: SFX_PlayX(unsigned int, short, int)
 *            Resolve an SFX alias and play it equally on left/right channels.
 *
 * @param sound  Global or level-specific sound alias, including option flags.
 * @param vol  Base volume applied to both stereo channels.
 * @param pitch_offset  Signed pitch adjustment added to the table pitch.
 * @return Bit field of allocated SPU voices, or 0 when playback fails.
 */
unsigned int SFX_PlayX(unsigned int sound, short vol, int pitch_offset) {
    unsigned int no_reverb;
    unsigned short soundIdx;
    unsigned int global_index;
    bool level_specific;
    unsigned int *p_array;
    unsigned int vol_mod;
    unsigned int pitch;
    unsigned int sfx;
    unsigned int packed;
    const unsigned int invalidSound = 0xFFFFFFFFU;

    if (sound == invalidSound) {
        return 0;
    }

    no_reverb = sound >> 14;
    no_reverb &= 1;
    soundIdx = sound & 0x3FFF;

    sound = translateGlobalAliasToIndex((unsigned short)soundIdx);
    level_specific = false;
    global_index = sound;
    if (sound == invalidSound) {
        level_specific = true;
        sound = translateLevelSpecificAliasToIndex((unsigned short)soundIdx);
        if (sound == global_index) {
            return 0;
        }
    }

    sound = sound * 3;
    if (level_specific) {
        p_array = &SFXLevelSpecificArray[sound];
    } else {
        p_array = &SFXArray[sound];
    }

    if ((int)p_array[0] >= 0) {
        Video_SFXPlay(sound, vol, pitch_offset);
    }
    sfx = p_array[0];
    packed = p_array[1];
    vol_mod = packed >> 16;
    vol = (short)(((int)vol * (int)vol_mod * 0x10) >> 16);
    vol = (short)(((int)vol * SFXLEVEL * 4) >> 16);
    pitch = (unsigned short)packed;
    if (no_reverb) {
        sfx |= 0x4000;
    }
    return playSFX(sfx, (unsigned char)pitch, vol, vol, pitch_offset,
                   (unsigned short)vol_mod);
}


/**
 * SFX_PlayLR__FUissi
 *
 * Address:   0x8004C3A8
 * Mangled:   SFX_PlayLR__FUissi
 * Demangled: SFX_PlayLR(unsigned int, short, short, int)
 *            Resolve an alias sound, scale stereo volumes, and dispatch to playSFX.
 *
 * @param sound  Global or level-specific sound alias, including option flags.
 * @param vl  Base left-channel volume.
 * @param vr  Base right-channel volume.
 * @param pitch_offset  Signed pitch adjustment added to the table pitch.
 * @return Bit field of allocated SPU voices, or 0 when playback fails.
 */
unsigned int SFX_PlayLR(unsigned int sound, short vl, short vr, int pitch_offset) {
    unsigned int no_reverb;
    unsigned short soundIdx;
    unsigned int global_index;
    bool level_specific;
    unsigned int *p_array;
    unsigned int vol_mod;
    unsigned int pitch;
    unsigned int sfx;
    unsigned int packed;
    const unsigned int invalidSound = 0xFFFFFFFFU;

    if (sound == invalidSound) {
        return 0;
    }

    no_reverb = sound >> 14;
    no_reverb &= 1;
    soundIdx = sound & 0x3FFF;

    sound = translateGlobalAliasToIndex((unsigned short)soundIdx);
    level_specific = false;
    global_index = sound;
    if (sound == invalidSound) {
        level_specific = true;
        sound = translateLevelSpecificAliasToIndex((unsigned short)soundIdx);
        if (sound == global_index) {
            return 0;
        }
    }

    sound = sound * 3;
    if (level_specific) {
        p_array = &SFXLevelSpecificArray[sound];
    } else {
        p_array = &SFXArray[sound];
    }

    packed = p_array[1];
    pitch = (unsigned short)packed;
    vol_mod = packed >> 16;
    vl = (short)(((int)vl * (int)vol_mod * 0x10) >> 16);
    vl = (short)(((int)vl * SFXLEVEL * 4) >> 16);
    vr = (short)(((int)vr * (int)vol_mod * 0x10) >> 16);
    vr = (short)(((int)vr * SFXLEVEL * 4) >> 16);

    sfx = p_array[0];
    if (no_reverb) {
        sfx |= 0x4000;
    }

    return playSFX(sfx, (unsigned char)pitch, vl, vr, pitch_offset,
                   (unsigned short)vol_mod);
}


/**
 * SFX_PlayPos__FUiRC7CVectori
 *
 * Address:   0x8004C50C
 * Mangled:   SFX_PlayPos__FUiRC7CVectori
 * Demangled: SFX_PlayPos(unsigned int, const CVector &, int)
 *            Resolve an alias sound from position, apply spatial falloff, and dispatch to playSFX.
 *
 * @param sound  SFX alias; bit 14 requests playback without reverb.
 * @param pos  Position vector.
 * @param pitch_offset  Pitch adjustment passed to the voice allocator.
 * @return Voice bitfield, or zero when the alias cannot be resolved.
 */
unsigned int SFX_PlayPos(unsigned int sound, const CVector & pos, int pitch_offset) {
    unsigned int no_reverb;
    unsigned int soundIdx;
    unsigned int global_index;
    bool level_specific;
    unsigned int *p_array;
    unsigned int falloff;
    unsigned int vol;
    unsigned int vol_mod;
    int vl_normalised;
    int vl;
    int vr_normalised;
    int vr;
    unsigned int pitch;
    unsigned int rv;
    unsigned int sfx;
    unsigned int packed;
    const unsigned int invalidSound = 0xFFFFFFFFU;

    if (sound == invalidSound) {
        return 0;
    }

    no_reverb = sound >> 14;
    no_reverb &= 1;
    soundIdx = sound & 0x3FFF;

    sound = translateGlobalAliasToIndex((unsigned short)soundIdx);
    level_specific = false;
    global_index = sound;
    if (sound == invalidSound) {
        level_specific = true;
        sound = translateLevelSpecificAliasToIndex((unsigned short)soundIdx);
        if (sound == global_index) {
            return 0;
        }
    }
    sound = sound * 3;
    if (level_specific) {
        p_array = &SFXLevelSpecificArray[sound];
    } else {
        p_array = &SFXArray[sound];
    }

    falloff = p_array[2];
    vol = Utils_CalculateSpatialAttenuation(pos, falloff >> 3, falloff);
    packed = p_array[1];
    vol_mod = packed >> 16;
    vl_normalised = (vol & 0xFFF) << 2;
    vl_normalised = (vl_normalised * vol_mod) >> 12;
    vr_normalised = (vol >> 14) & 0x3FFC;
    vr_normalised = (vr_normalised * vol_mod) >> 12;
    vl = (vl_normalised * SFXLEVEL) >> 14;
    vr = (vr_normalised * SFXLEVEL) >> 14;
    if ((int)vol < 0) {
        vl = -vl;
    }

    pitch = (unsigned short)packed;
    sfx = p_array[0];
    if (no_reverb) {
        sfx |= 0x4000;
    }

    rv = playSFX(sfx, (unsigned char)pitch, (short)vl, (short)vr,
                 pitch_offset, (unsigned short)vol_mod);

    {
        int i;
        for (i = 0; i < SFX_VOICE_COUNT; i++) {
            if (rv & (1 << i)) {
                SFXFalloffArray[i].packed = falloff | (vol_mod << 16);
                return rv;
            }
        }
    }

    return rv;
}


/**
 * SFX_GetVoicePitch__FUi
 * Assembly (16 instr):
 *   lw $v0,0x2700($gp)        -- SFXPaused
 *   addiu $sp,$sp,-0x58
 *   beqz $v0, .notPaused
 *   sw $ra,0x50($sp)
 *   j .end
 *   addiu $v0,$zero,0x3e8     -- return 1000
 * .notPaused:
 *   beqz $a0, .end
 *   nop
 *   sw $a0,0x10($sp)
 *   jal SpuGetVoiceAttr
 *   addiu $a0,$sp,0x10
 *   lhu $v0,0x24($sp)
 * .end:
 *   lw $ra,0x50($sp)
 *   nop
 *   jr $ra
 *   addiu $sp,$sp,0x58
 *
 * Address:   0x8004C6C4
 * Mangled:   SFX_GetVoicePitch__FUi
 * Demangled: SFX_GetVoicePitch(unsigned int)
 *
 * @param voice_bf  Bit field selecting the voice whose attributes are queried.
 * @return Current SPU pitch, or 1000 while sound effects are paused.
 */
unsigned short SFX_GetVoicePitch(unsigned int voice_bf) {
    SpuVoiceAttr attr;

    if (SFXPaused) {
        return 1000;
    }
    if (voice_bf == 0) {
        // fall through, return whatever is in $v0
    } else {
        attr.voice = voice_bf;
        SpuGetVoiceAttr(&attr);
        return attr.pitch;
    }
}


/**
 * SFX_SetVoicePitch__FUiUs
 *
 * Address:   0x8004C704
 * Mangled:   SFX_SetVoicePitch__FUiUs
 * Demangled: SFX_SetVoicePitch(unsigned int, unsigned short)
 *
 * @param voice_bf Bit field selecting the voices to update.
 * @param pitch New SPU pitch value for every selected voice.
 */
void SFX_SetVoicePitch(unsigned int voice_bf, unsigned short pitch) {
    struct SpuVoiceAttr attr;

    if (SFXPaused) return;
    if (voice_bf == 0) return;

    int v;
    for (v = 0; v < SFX_VOICE_COUNT; v++) {
        if (voice_bf & (1 << v)) {
            attr.mask = SPU_VOICE_PITCH;
            attr.pitch = pitch;
            SpuNSetVoiceAttr(v, &attr);
        }
    }
}


/**
 * SFX_ModifyPos__FUiRC7CVectori
 *
 * Address:   0x8004C790
 * Mangled:   SFX_ModifyPos__FUiRC7CVectori
 * Demangled: SFX_ModifyPos(unsigned int, const CVector &, int)
 *
 * Recomputes the selected voices' stereo volume and optionally adjusts pitch
 * for the listener's change in distance.
 *
 * @param voice_id Bit field selecting the voices to update.
 * @param pos World-space sound position.
 * @param delta_dist Change in listener distance used for the pitch adjustment.
 */
void SFX_ModifyPos(unsigned int voice_id, const CVector & pos, int delta_dist) {
    struct SpuVoiceAttr attr;

    if (voice_id == 0) return;

    int v = 0;
    u16 *spatialData = voicePitch;
    SFXVoiceSpatialData *extraData = SFXFalloffArray;

    do {
        if (voice_id & (1 << v)) {
            u16 rawSpatial = extraData->fields.falloff;
            unsigned int vol = Utils_CalculateSpatialAttenuation(pos, rawSpatial >> 3, rawSpatial);

            unsigned int vol_mod = extraData->fields.volumeModifier;
            int vl_normalised = vol & 0xFFF;
            vol_mod &= 0x3FFF;
            vl_normalised = (int)((unsigned int)(vl_normalised * vol_mod) >> 12);
            if (vl_normalised < -0x3FFF) vl_normalised = -0x3FFF;
            if (vl_normalised >= 0x4000) vl_normalised = 0x3FFF;

            int vr_normalised = (vol >> 16) & 0xFFF;
            vr_normalised = (int)((unsigned int)(vr_normalised * vol_mod) >> 12);

            int scaledLeft = (vl_normalised * SFXLEVEL) >> 14;
            attr.volume.left = (short)scaledLeft;
            if (vr_normalised < -0x3FFF) vr_normalised = -0x3FFF;
            if (vr_normalised >= 0x4000) vr_normalised = 0x3FFF;

            attr.volume.right =
                (short)((vr_normalised * SFXLEVEL) >> 14);
            if ((int)vol < 0) {
                attr.volume.left = -(short)scaledLeft;
            }

            attr.voice = v;
            attr.mask = SPU_VOICE_VOLL | SPU_VOICE_VOLR;

            if (delta_dist != 0) {
                u16 pitchData = *spatialData;
                int adjusted = pitchData * delta_dist;
                attr.mask = SPU_VOICE_VOLL | SPU_VOICE_VOLR |
                            SPU_VOICE_PITCH;
                if (adjusted < 0) adjusted += 0x1FFF;
                int pitchAdjustment = adjusted >> 13;
                int new_pitch = pitchData - pitchAdjustment;
                if ((int)pitchData < new_pitch) {
                    int delta2 = new_pitch - pitchData;
                    int limit = pitchData + (pitchData >> 1);
                    if (limit < delta2) {
                        new_pitch = limit;
                    }
                }
                attr.pitch = (u16)new_pitch;
            }

            SpuNSetVoiceAttr(v, &attr);
        }
        spatialData++;
        v++;
        extraData++;
    } while (v < SFX_VOICE_COUNT);
}


/**
 * SFX_ModifyVol__FUiii
 *
 * Address:   0x8004C938
 * Mangled:   SFX_ModifyVol__FUiii
 * Demangled: SFX_ModifyVol(unsigned int, int, int)
 *
 * @param voice_id  Bit field selecting the voices to update.
 * @param vl  Requested left-channel volume.
 * @param vr  Requested right-channel volume.
 */
void SFX_ModifyVol(unsigned int voice_id, int vl, int vr) {
    SpuVoiceAttr attr;

    if (voice_id == 0) return;

    if (vl >= 0x4000) vl = 0x3FFF;
    if (vr >= 0x4000) vr = 0x3FFF;

    vl = (vl * SFXLEVEL) >> 14;
    vr = (vr * SFXLEVEL) >> 14;

    int v = 0;

    do {
        if (voice_id & (1 << v)) {
            vl = (vl * voiceVolume[v]) >> 12;
            vr = (vr * voiceVolume[v]) >> 12;
            attr.voice = v;
            attr.mask = SPU_VOICE_VOLL | SPU_VOICE_VOLR;
            if (vl < -0x3FFF) vl = -0x3FFF;
            if (vl >= 0x4000) vl = 0x3FFF;
            if (vr < -0x3FFF) vr = -0x3FFF;
            if (vr >= 0x4000) vr = 0x3FFF;
            attr.volume.left = (short)vl;
            attr.volume.right = (short)vr;
            SpuNSetVoiceAttr(v, &attr);
        }
        v++;
    } while (v < SFX_VOICE_COUNT);
}


/**
 * SFX_ResetMIDIPausedPriorToPause__Fv
 *
 * Address:   0x8004CA74
 * Mangled:   SFX_ResetMIDIPausedPriorToPause__Fv
 * Demangled: SFX_ResetMIDIPausedPriorToPause(void)
 *            jr $ra; nop — empty void function
 */
void SFX_ResetMIDIPausedPriorToPause(void) {
}


/**
 * SFX_LevelStart__Fv
 *
 * Address:   0x8004CA7C
 * Mangled:   SFX_LevelStart__Fv
 * Demangled: SFX_LevelStart(void)
 *            sw $zero,0x2700($gp); jr $ra; nop
 */
void SFX_LevelStart(void) {
    SFXPaused = 0;
}


/**
 * SFX_Pause__Fv
 *
 * Address:   0x8004CA88
 * Mangled:   SFX_Pause__Fv
 * Demangled: SFX_Pause(void)
 *            Pause active SFX voices by saving their pitch and forcing pitch to zero.
 */
void SFX_Pause(void) {
    SpuVoiceAttr attr;

    if (SFXPaused) return;

    int v = 0;

    SFXPaused = 1;
    attr.mask = SPU_VOICE_PITCH;
    attr.pitch = 0;

    do {
        SpuGetVoicePitch(v, &pitchPriorToPause[v]);
        SpuNSetVoiceAttr(v, &attr);
        v++;
    } while (v < SFX_VOICE_COUNT);

    Redbook_XAPause(1);
}


/**
 * SFX_Unpause__Fv
 *
 * Address:   0x8004CB04
 * Mangled:   SFX_Unpause__Fv
 * Demangled: SFX_Unpause(void)
 *            Restore saved voice pitches and resume XA after an SFX pause.
 */
void SFX_Unpause(void) {
    SpuVoiceAttr attr;

    if (!SFXPaused) return;

    int v = 0;

    SFXPaused = 0;
    attr.mask = SPU_VOICE_PITCH;

    do {
        attr.pitch = pitchPriorToPause[v];
        SpuNSetVoiceAttr(v, &attr);
        v++;
    } while (v < SFX_VOICE_COUNT);

    Redbook_XAPause(0);
}


/**
 * SFX_StopAll__Fv
 * Assembly (17 instr):
 *   addiu $sp,-0x20; sw $s0,0x10($sp); move $s0,$zero
 *   sw $s1,0x14($sp); addiu $s1,$zero,1; sw $ra,0x18($sp)
 *   .loop: jal SFX_Stop; sllv $a0,$s1,$s0
 *   addiu $s0,1; slti $v0,$s0,0x18; bnez $v0,.loop; nop
 *   epilogue
 *
 * Address:   0x8004CB78
 * Mangled:   SFX_StopAll__Fv
 * Demangled: SFX_StopAll(void)
 */
void SFX_StopAll(void) {
    int i;
    for (i = 0; i < SFX_VOICE_COUNT; i++) {
        SFX_Stop(1 << i);
    }
}


/**
 * SFX_Stop__FUi
 * Assembly (12 instr, no stack frame):
 *   nor $a2,$zero,$a0           -- mask = ~voice_id
 *   lw $v0,0x2708($gp)         -- nko_flag
 *   lw $v1,0x2720($gp)         -- loop_flag
 *   lw $a1,0x2718($gp)         -- voiceon_flag
 *   and $v0,$v0,$a2; sw $v0    -- nko_flag &= ~voice_id
 *   lw $v0,0x270C($gp)         -- keyon_queue
 *   and $v1,$v1,$a2; sw $v1    -- loop_flag &= ~voice_id
 *   lw $v1,0x2714($gp)         -- keyoff_queue
 *   and $a1,$a1,$a2; sw $a1    -- voiceon_flag &= ~voice_id
 *   and $v0,$v0,$a2            -- keyon_queue &= ~voice_id
 *   or $v1,$v1,$a0             -- keyoff_queue |= voice_id
 *   sw $v0; sw $v1
 *   jr $ra; nop
 *
 * Address:   0x8004CBBC
 * Mangled:   SFX_Stop__FUi
 * Demangled: SFX_Stop(unsigned int)
 *
 * @param voice_id  Bit field selecting the voices to stop.
 */
void SFX_Stop(unsigned int voice_id) {
    nko_flag &= ~voice_id;
    loop_flag &= ~voice_id;
    voiceon_flag &= ~voice_id;
    keyon_queue &= ~voice_id;
    keyoff_queue |= voice_id;
}


/**
 * SFX_SetReverbType__FUc
 *
 * Address:   0x8004CC04
 * Mangled:   SFX_SetReverbType__FUc
 * Demangled: SFX_SetReverbType(unsigned char)
 *
 * @param reverb_type  SPU reverb mode index.
 */
void SFX_SetReverbType(unsigned char reverb_type) {
    SpuReverbAttr attr;
    attr.mask = SPU_REV_MODE;
    attr.mode = reverb_type;
    int ret = SpuSetReverbModeParam(&attr);
    if (ret == -1) return;

    if (reverb_type == 0) {
        SFX_ReverbDepthSetTimer = 0;
        SpuSetReverbVoice(SPU_OFF, SPU_ALLCH);
    } else {
        if (SpuSetReverb(SPU_ON)) {
            SFX_ReverbDepthSetTimer = 0xF0;
            SpuSetReverbVoice(SPU_ON, SPU_ALLCH);
        } else {
            SpuSetReverbVoice(SPU_OFF, SPU_ALLCH);
        }
    }
}


/**
 * SFX_ReverbOff__Fv
 * Assembly (8 instr):
 *   addiu $sp,-0x18; sw $ra,0x10($sp)
 *   jal SFX_SetReverbType; move $a0,$zero
 *   lw $ra,0x10($sp); nop; jr $ra; addiu $sp,0x18
 *
 * Address:   0x8004CC84
 * Mangled:   SFX_ReverbOff__Fv
 * Demangled: SFX_ReverbOff(void)
 */
void SFX_ReverbOff(void) {
    SFX_SetReverbType(0);
}


/**
 * SFX_SetReverbDepth__FUcUc
 * Assembly (14 instr):
 *   addiu $sp,-0x30
 *   addiu $v0,$zero,6
 *   sll $a0,$a0,8; sll $a1,$a1,8
 *   sh $a0,0x18($sp); addiu $a0,$sp,0x10
 *   sw $ra,0x28($sp); sw $v0,0x10($sp)
 *   jal SpuSetReverbModeParam; sh $a1,0x1a($sp)
 *   epilogue
 *
 * Address:   0x8004CCA4
 * Mangled:   SFX_SetReverbDepth__FUcUc
 * Demangled: SFX_SetReverbDepth(unsigned char, unsigned char)
 *
 * @param ld  Left-channel reverb depth.
 * @param rd  Right-channel reverb depth.
 */
void SFX_SetReverbDepth(unsigned char ld, unsigned char rd) {
    SpuReverbAttr attr;
    attr.mask = SPU_REV_DEPTHL | SPU_REV_DEPTHR;
    attr.depth.left = (short)(ld << 8);
    attr.depth.right = (short)(rd << 8);
    SpuSetReverbModeParam(&attr);
}


/**
 * SFX_MIDIFadeOut__Fi
 *
 * Address:   0x8004CCDC
 * Mangled:   SFX_MIDIFadeOut__Fi
 * Demangled: SFX_MIDIFadeOut(int)
 *            jr $ra; nop — empty void function
 *
 * @param ticks  Requested fade duration in ticks.
 */
void SFX_MIDIFadeOut(int ticks) {
}


/**
 * SFX_MIDIStop__Fv
 *
 * Address:   0x8004CCE4
 * Mangled:   SFX_MIDIStop__Fv
 * Demangled: SFX_MIDIStop(void)
 *            jr $ra; nop — empty void function
 */
void SFX_MIDIStop(void) {
}


/**
 * SFX_MIDIFadeIn__Fi
 *
 * Address:   0x8004CCEC
 * Mangled:   SFX_MIDIFadeIn__Fi
 * Demangled: SFX_MIDIFadeIn(int)
 *            jr $ra; nop — empty void function
 *
 * @param ticks  Requested fade duration in ticks.
 */
void SFX_MIDIFadeIn(int ticks) {
}


/**
 * SFX_MIDIChannelMute__FUs
 *
 * Address:   0x8004CCF4
 * Mangled:   SFX_MIDIChannelMute__FUs
 * Demangled: SFX_MIDIChannelMute(unsigned short)
 *            jr $ra; nop — empty void function
 *
 * @param mute  Channel-mute mask retained for the no-op prototype implementation.
 */
void SFX_MIDIChannelMute(unsigned short mute) {
}


/**
 * playSFX__FUiUcssiUs
 *
 * Address:   0x8004CCFC
 * Mangled:   playSFX__FUiUcssiUs
 * Demangled: playSFX(unsigned int, unsigned char, short, short, int, unsigned short)
 *
 * @param sfx  Packed VAB/program/tone selection and playback flags.
 * @param pitch  Base MIDI-style pitch value.
 * @param vl  Left-channel volume.
 * @param vr  Right-channel volume.
 * @param pitch_offset  Signed adjustment applied to the base pitch.
 * @param volMod  Per-sound volume modifier.
 * Allocates SPU voices for the selected VAB tones and queues them to start.
 */
static unsigned int playSFX(unsigned int sfx, unsigned char pitch, short vl, short vr, int pitch_offset, unsigned short volMod) {
    unsigned int looping;
    unsigned short prog;
    unsigned short tones;
    unsigned short voices;
    int voice_bf;

    prog = (sfx >> 16) & 0x7f;
    looping = sfx >> 31;
    tones = sfx & 0xffff;

    if (pitch_offset) {
        pitch = (pitch + pitch_offset) & 0xff;
    }

    if (tones < 2) {
        voices = 1;
    } else {
        voices = 0;
        for (int t = 0; t < 16; t++) {
            if ((tones >> t) & 1) {
                voices++;
            }
        }
    }

    voice_bf = 0;
    for (int i = 0; i < voices; i++) {
        int free = allocateVoice();
        if (free == -1) {
            voice_bf = 0;
            break;
        }
        voice_bf |= 1 << free;
    }

    if (voice_bf > 0) {
        unsigned int lzc;
        SFXVabImage *p_vab_hdr;
        int tone_start;
        SpuVoiceAttr attr;

        gte_ldlzc(voice_bf);
        if ((sfx & 0x40000000) == 0) {
            p_vab_hdr = (SFXVabImage *)pSFXVABHeader;
        } else {
            p_vab_hdr = (SFXVabImage *)pLevelSpecificSFXVABHeader;
        }
        gte_stlzc(&lzc);

        lzc = 32 - lzc;
        tone_start = 0;
        attr.mask = SPU_VOICE_VOLL | SPU_VOICE_VOLR | SPU_VOICE_PITCH |
                    SPU_VOICE_WDSA | SPU_VOICE_ADSR_ADSR1 |
                    SPU_VOICE_ADSR_ADSR2;

        int v = 0;
        if (v < lzc) {
            unsigned short *p_volume = voiceVolume;
            for (; v < lzc; v++, p_volume++) {
                bool no_reverb;
                if (tones == 0) {
                    break;
                } else {
                    no_reverb = false;
                }
                if ((voice_bf >> v) & 1) {
                    int t = tone_start;
                    if (t < 16) {
                        VagAtr *p_offset =
                            &p_vab_hdr->tones[prog * SFX_VAB_TONES_PER_PROGRAM];
                        // This source local is present in MAIN.SYM and affects
                        // the old compiler's register interference graph.
                        int pitch_byte_off = v * 2;
                        // Both compiler boundaries below are match-critical.
                        // The first forces the p_volume reload into this
                        // preheader instead of the store site; the second
                        // flips the caller-saved tie between `tones` ($t0)
                        // and p_pitch ($t1).
                        unsigned short *p_vol = p_volume;
                        // Removing this operand changes 61 instructions and
                        // two relocations in playSFX.
                        asm volatile("" : "+r"(p_vol));
                        unsigned short *p_pitch = voicePitch + v;
                        int voice_bit = 1 << v;
                        // Removing this scheduling boundary produces 14 real
                        // mismatches in playSFX.
                        asm volatile("");
                        for (; t < 16; t++) {
                            if ((tones >> t) & 1) {
                                VagAtr *p_vagatr = p_offset + t;
                                tone_start = t + 1;

                                if (vl < -0x3fff) vl = -0x3fff;
                                if (vl > 0x3fff) vl = 0x3fff;
                                if (vr < -0x3fff) vr = -0x3fff;
                                if (vr > 0x3fff) vr = 0x3fff;

                                int reverb_voice = 1 << v;
                                *p_vol = volMod;
                                attr.voice = reverb_voice;
                                attr.volume.left = vl;
                                attr.volume.right = vr;

                                attr.pitch = SsPitchFromNote(
                                    pitch, 0, p_vagatr->center,
                                    p_vagatr->shift);
                                *p_pitch = attr.pitch;

                                unsigned short *p_vagtbl =
                                    (unsigned short *)&p_vab_hdr->tones[
                                        p_vab_hdr->header.ps *
                                        SFX_VAB_TONES_PER_PROGRAM];
                                attr.addr = p_vagtbl[p_vagatr->vag] << 3;
                                attr.adsr1 = p_vagatr->adsr1;
                                attr.adsr2 = p_vagatr->adsr2;

                                SpuNSetVoiceAttr(v, &attr);

                                if (no_reverb) {
                                    if (reverb_queue & reverb_voice) {
                                        reverb_changed = 1;
                                        reverb_queue &= ~reverb_voice;
                                    }
                                } else {
                                    if ((reverb_queue & reverb_voice) == 0) {
                                        reverb_changed = 1;
                                        reverb_queue |= reverb_voice;
                                    }
                                }
                                if (looping) {
                                    loop_flag |= voice_bit;
                                }

                                int one = 1;
                                tones &= ~(one << t);
                                voicefree_flag &= ~voice_bit;
                                nko_flag |= voice_bit;
                                voiceon_flag |= voice_bit;
                                keyon_queue |= voice_bit;
                                keyoff_queue &= ~voice_bit;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    return voice_bf;
}


/**
 * allocateVoice__Fv
 *
 * Address:   0x8004D0CC
 * Mangled:   allocateVoice__Fv
 * Demangled: allocateVoice(void)
 *
 * @return Index of a free voice, or -1 when every voice is occupied.
 */
static int allocateVoice(void) {
    // Keeping the loop index in $v1 is load-bearing: an ordinary local
    // produces one real mismatch in this function.
    register int i asm("$3") = 0;
    int one = 1;
    asm volatile("" : "+r"(i));
    int vf = voicefree_flag;
    int mask = one << i;
loop_av:
    if (vf & mask) {
        return i;
    }
    i++;
    if (i < SFX_VOICE_COUNT) {
        mask = one << i;
        goto loop_av;
    }
    return -1;
}


/**
 * SFX_ProcessingPerFrame__Fv
 *
 * Address:   0x8004D108
 * Mangled:   SFX_ProcessingPerFrame__Fv
 * Demangled: SFX_ProcessingPerFrame(void)
 */
void SFX_ProcessingPerFrame(void) {
    short env;

    voicefree_flag = 0;

    // Process keyon_queue
    if (keyon_queue) {
        SpuSetKey(SPU_ON, keyon_queue);
        keyon_queue = 0;
    }

    // Process keyoff_queue
    if (keyoff_queue) {
        SpuSetKey(SPU_OFF, keyoff_queue);
        keyoff_queue = 0;
    }

    // Process reverb_changed
    if (reverb_changed) {
        SpuSetReverbVoice(SPU_BIT, reverb_queue);
        reverb_changed = 0;
    }

    int i = 0;
    do {
        int mask = 1 << i;

        if (nko_flag & mask) goto next;
        if (loop_flag & mask) goto next;

        SpuGetVoiceEnvelope(i, &env);

        if (voiceon_flag & mask) {
            if (env != 0) goto next;

            if (exp_flag & mask) {
                voiceon_flag &= ~mask;
                i++;
                continue;
            } else {
                exp_flag |= mask;
                i++;
                continue;
            }
        }

        if (env != 0) goto next;

        voicefree_flag |= mask;

    next:
        i++;
    } while (i < SFX_VOICE_COUNT);

    nko_flag = 0;
}


/**
 * SFX_ParseSFXFile__FPcPUiPUsii
 *
 * Address:   0x8004D248
 * Mangled:   SFX_ParseSFXFile__FPcPUiPUsii
 * Demangled: SFX_ParseSFXFile(char *, unsigned int *, unsigned short *, int, int)
 *            Build the matching .SFX filename, load its compact table, and unpack aliases.
 *
 * @param p_name  Source asset name whose extension is replaced with `.SFX`.
 * @param array  Receives unpacked three-word sound records.
 * @param aliasArray  Receives the alias associated with each sound record.
 * @param maxEntries  Capacity of the alias table.
 * @param mask  Flag bits ORed into every unpacked sound record.
 */
void SFX_ParseSFXFile(char * p_name, unsigned int * array, unsigned short * aliasArray, int maxEntries, int mask) {
    char filename[20];
    unsigned char *p_block;
    int alias_index;
    int msk = mask;
    asm("" : "+r"(msk));
    int i = 0;
    asm("" : "+r"(i) : "r"(msk));
    unsigned int *arr = array;
    unsigned short *als = aliasArray;
    int max = maxEntries;
    asm volatile("");
    char *out = filename;

    goto copy_test;
copy_loop:
    i++;
copy_test:
    {
        unsigned char c = *p_name;
        if (c == 0) goto copy_done;
        out[i] = c;
        p_name++;
        goto copy_loop;
    }

copy_done:
    i -= 3;
    out[i++] = 'S';
    out[i++] = 'F';
    out[i++] = 'X';
    out[i] = '\0';

    // Keeps filename construction separate from the following allocator call.
    asm volatile("");
    p_block = (unsigned char *)Mem_New(0x800, 0, 1);
    FileIO_Open(filename);
    FileIO_Load(p_block);
    FileIO_Sync();

    alias_index = 0;
    unsigned int *p_array;
    unsigned int *p_parse = (unsigned int *)p_block;
    unsigned int val = *p_parse;
    p_parse++;
    if (val != 0xFFFFFFFF) {
        unsigned int fe = 0xFE;
        unsigned int loopMask = msk | 0x80000000;
        unsigned short *p_alias = als;
        p_array = arr;

        do {
            if ((val & 0xFF) == fe) {
                p_array[0] = loopMask;
            } else {
                p_array[0] = msk;
            }

            alias_index++;
            unsigned int tone;
            p_array[0] |= ((val >> 8) & 0xFF) << 16;
            tone = 1 << (val >> 16);
            p_array[0] |= tone;
            p_array++;

            unsigned int vol_and_falloff;
            p_array[0] = val >> 24;
            vol_and_falloff = *p_parse++;
            p_array[0] |= vol_and_falloff & 0xFFFF0000;
            p_array++;
            p_array[0] = vol_and_falloff & 0xFFFF;
            p_array++;

            *p_alias++ = *(unsigned short *)p_parse;
            p_parse += 2;
            val = *p_parse++;
        } while (val != 0xFFFFFFFF);
    }

    if (alias_index < max) {
        als[alias_index] = 0xFFFF;
    }
    Mem_Delete(p_block);
}


/**
 * translateLevelSpecificAliasToIndex(unsigned short) — search level-specific alias array
 *
 * Address: 0x8004D408
 * Mangled: translateLevelSpecificAliasToIndex__FUs
 *
 * @param alias  Level-specific alias to resolve.
 * @return Matching table index, or `UINT_MAX` when not found.
 */
static unsigned int translateLevelSpecificAliasToIndex(unsigned short alias) {
    int i = 0;
    unsigned short e;
    for (; i < 64; i++) {
        e = SFXLevelSpecificAliasArray[i];
        if (e == 0xFFFF) return -1;
        if (alias == e) return i;
    }
    return -1;
}


/**
 * translateGlobalAliasToIndex(unsigned short) — search global alias array
 *
 * Address: 0x8004D45C
 * Mangled: translateGlobalAliasToIndex__FUs
 *
 * @param alias  Global alias to resolve.
 * @return Matching table index, or `UINT_MAX` when not found.
 */
static unsigned int translateGlobalAliasToIndex(unsigned short alias) {
    int i = 0;
    unsigned short e;
    for (; i < 64; i++) {
        e = SFXAliasArray[i];
        if (e == 0xFFFF) return -1;
        if (alias == e) return i;
    }
    return -1;
}


/**
 * sfxMalloc__FUi
 *
 * Address:   0x8004D4B0
 * Mangled:   sfxMalloc__FUi
 * Demangled: sfxMalloc(unsigned int)
 *            Allocate a block from the three-entry SPU sound RAM slot table, splitting free space.
 *
 * @param size  Buffer size in bytes.
 * @return Start address of the allocated SPU RAM block, or zero when full.
 */
static unsigned int sfxMalloc(unsigned int size) {
    int i = 0;
    do {
        if (soundRAMSlots[i].occupied == 0) {
            if (soundRAMSlots[i].start != 0) {
                if (soundRAMSlots[i].size >= size) {
                    soundRAMSlots[i].occupied = (void *)1;
                    if (size < soundRAMSlots[i].size) {
                        soundRAMSlots[i + 1].start = soundRAMSlots[i].start + size;
                        soundRAMSlots[i + 1].size = soundRAMSlots[i].size - size;
                        soundRAMSlots[i].size = size;
                    }
                    return soundRAMSlots[i].start;
                }
            }
        }
        i++;
    } while (i < 3);
    return 0;
}
