// GAMEFMV.cpp -- Reconstructed translation unit
// Original: GAMEFMV.CPP
// Functions: 13
// Address range: 0x8007EDD0 - 0x8007FC6C
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#define GAMEFMV_OWNER_DECLARATIONS
#include "GAMEFMV.h"
#undef GAMEFMV_OWNER_DECLARATIONS
#include "MAIN.h"

typedef char GameFMV_StHEADER_size_must_be_32[
    (sizeof(StHEADER) == 32) ? 1 : -1];
typedef char GameFMV_movie_details_size_must_be_32[
    (sizeof(SMovieDetails) == 32) ? 1 : -1];
typedef char GameFMV_cd_file_size_must_be_24[
    (sizeof(CdlFILE) == 24) ? 1 : -1];

extern "C" {
void DecDCTReset(int mode);
void DecDCToutCallback(void (*callback)(void));
void DecDCTin(unsigned long *data, int mode);
void DecDCTout(unsigned long *data, int wordCount);

extern int StCdIntrFlag __attribute__((section(".data")));
/** Distinct write view retaining the target's read/write declaration identity. */
extern int GameFMV_StCdIntrFlagWrite asm("StCdIntrFlag")
    __attribute__((section(".data")));

extern unsigned int *GameFMV_VlcBuffers[2] asm("vlcBuffer");
extern unsigned long *GameFMV_RingBuffer asm("pRingBuffer");
}

/**
 * Typed views of the generated scalar declarations used by async playback.
 * Replacing these two views with the otherwise equivalent pointer aliases
 * keeps every instruction exact but changes the original PSYQ `.extern`
 * record order. Keeping the casts here preserves a byte-identical sidecar and
 * removes the repeated integer/pointer conversions from the implementation.
 */
#define GameFMV_AsyncRingBuffer \
    (*reinterpret_cast<unsigned long **>(&pRingBuffer))
#define GameFMV_AsyncVlcBuffers \
    (reinterpret_cast<unsigned int **>(&vlcBuffer))

/**
 * Reads the packed byte through an aliasing view that preserves the target's
 * load schedule when the remaining movie fields are copied as one group. A
 * direct field read and an inline-function wrapper each changed 18 instructions
 * and four relocations in `GameFMV_SetStartTrack`.
 */
#define GameFMV_Read24BitFlag(movie_) \
    (*reinterpret_cast<const u8 *>(&(movie_).is24bit))

#include "FRONT.h"

#include "MEM.h"
#define DB_BUFFER_LAYOUT
#include "DB.h"
#undef DB_BUFFER_LAYOUT
#define DOUBLEBUFFER_DECLARE_KNOWN_BOUND
#include "DOUBLEBUFFER.h"
#undef DOUBLEBUFFER_DECLARE_KNOWN_BOUND
#include "PAD.h"
#include "VLC_BIT.h"
#include "globals.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBETC.H>
#include <LIBGPU.H>

/** Owner-local fixed-point vector with the original empty default constructor. */
struct GameFMVSoundVector : CVector {
    GameFMVSoundVector() {}
};

typedef char GameFMV_sound_vector_size_must_be_12[
    (sizeof(GameFMVSoundVector) == 12) ? 1 : -1];

/** Playback-mode flag owned by GAMEFMV. */
int GameFMV_IsMovie = 0;

/** Eight positional movie-audio sources constructed at program startup. */
GameFMVSoundVector GameFMV_SoundSource[8];

/**
 * Movie geometry, decoder sizing, and source-disc filenames in track order.
 */
#define GAMEFMV_MOVIE(name_, width_, height_, end_, vlc_, ring_, rgb_, music_) \
    {name_, width_, height_, end_, vlc_, ring_, rgb_, {0, 0, 0}, 1, music_}

static SMovieDetails movieDetails[30] __attribute__((section(".data"))) = {
    GAMEFMV_MOVIE("\\ATVILOGO.STR;1", 320, 240, 208, 0x25800, 48, 1, 128),
    GAMEFMV_MOVIE("\\LOGO.STR;1", 320, 144, 237, 0x16800, 48, 1, 128),
    GAMEFMV_MOVIE("\\ANTHRAX.STR;1", 96, 64, 3746, 0x4000, 16, 0, 102),
    GAMEFMV_MOVIE("\\MCSERCH.STR;1", 96, 64, 3746, 0x4000, 16, 0, 102),
    GAMEFMV_MOVIE("\\NAUGHTY.STR;1", 96, 64, 3746, 0x4000, 16, 0, 102),
    GAMEFMV_MOVIE("\\NWA.STR;1", 96, 64, 3746, 0x4000, 16, 0, 102),
    GAMEFMV_MOVIE("\\SOCIALD.STR;1", 96, 64, 3746, 0x4000, 16, 0, 102),
    GAMEFMV_MOVIE("\\SURFERS.STR;1", 96, 64, 3746, 0x4000, 16, 0, 102),
    GAMEFMV_MOVIE("\\PENNY.STR;1", 96, 64, 3746, 0x4000, 16, 0, 102),
    GAMEFMV_MOVIE("\\PISTOLS.STR;1", 96, 64, 3746, 0x4000, 16, 0, 102),
    GAMEFMV_MOVIE("\\BAILS.STR;1", 320, 240, 790, 0x25800, 48, 1, 128),
    GAMEFMV_MOVIE("\\HAWK.STR;1", 320, 240, 464, 0x25800, 48, 1, 128),
    GAMEFMV_MOVIE("\\BURNQUIS.STR;1", 320, 240, 466, 0x25800, 48, 1, 128),
    GAMEFMV_MOVIE("\\ROWLEY.STR;1", 320, 240, 454, 0x25800, 48, 1, 128),
    GAMEFMV_MOVIE("\\LASEK.STR;1", 320, 240, 488, 0x25800, 48, 1, 128),
    GAMEFMV_MOVIE("\\MUSKA.STR;1", 320, 240, 452, 0x25800, 48, 1, 128),
    GAMEFMV_MOVIE("\\CAMPBELL.STR;1", 320, 240, 495, 0x25800, 48, 1, 128),
    GAMEFMV_MOVIE("\\REYNOLDS.STR;1", 320, 240, 462, 0x25800, 48, 1, 128),
    GAMEFMV_MOVIE("\\GLIFBERG.STR;1", 320, 240, 477, 0x25800, 48, 1, 128),
    GAMEFMV_MOVIE("\\THOMAS.STR;1", 320, 240, 477, 0x25800, 48, 1, 128),
    GAMEFMV_MOVIE("\\STEAMER.STR;1", 320, 240, 437, 0x25800, 48, 1, 128),
    GAMEFMV_MOVIE("\\AARON.STR;1", 320, 240, 619, 0x25800, 48, 1, 128),
    GAMEFMV_MOVIE("\\SABBATH.STR;1", 96, 64, 3746, 0x4000, 16, 0, 128),
    GAMEFMV_MOVIE("\\ZEBRA.STR;1", 96, 64, 3746, 0x4000, 16, 0, 102),
    GAMEFMV_MOVIE("\\INTRO.STR;1", 320, 240, 991, 0x25800, 48, 1, 128),
    GAMEFMV_MOVIE("\\411.STR;1", 320, 240, 451, 0x25800, 48, 1, 128),
    GAMEFMV_MOVIE("\\MAR.STR;1", 176, 48, 449, 0x8400, 48, 0, 128),
    GAMEFMV_MOVIE("\\STATIC.STR;1", 176, 48, 103, 0x8400, 48, 0, 128),
    {},
    {},
};

#undef GAMEFMV_MOVIE

/** Stable public-to-table track mapping used during disc lookup. */
static u8 indices[30] __attribute__((section(".data"))) = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
};

/** Decoder output slices allocated for the active movie. */
extern "C" unsigned short *decodeSliceBuffer[11]
    __attribute__((section(".data"))) = {};

/**
 * Complete owner-local decoder state recorded as class-static globals by
 * MAIN.SYM.
 *
 * The public-looking declarations in the generated TU header are access views
 * used by the recovered functions.  These private definitions retain the
 * original assembly labels so GCC emits local common records and the
 * assembler resolves every same-TU reference without creating false XDEFs.
 */
static int GameFMV_CurrentDecodeBufferStorage asm("currentDecodeBuffer");
static int GameFMV_CurrentVlcBufferStorage asm("currentVlcBuffer");
static int GameFMV_SendVlcStorage asm("sendVLC");
static int GameFMV_AlreadyHandledPauseStorage asm("alreadyHandledPause");
static unsigned long *GameFMV_RingBufferStorage asm("pRingBuffer");

// === Owner-local callback declarations ===
static void handleLidOpen(void);
static void strInit(void);
static void strCallback(void);
static unsigned long *strNext(void);
static int strNextVlc(int DecodeData);
static void strKickCD(void);

// === Function implementations (address order) ===

/**
 * Resolves every configured movie filename to its starting disc sector.
 *
 * Address: 0x8007EDD0
 */
void GameFMV_Init(void) {
    int i = 0;
    CdlFILE fmv_file;

    do {
        if (movieDetails[indices[i]].startsector) {
            if (CdSearchFile(&fmv_file, movieDetails[indices[i]].name)) {
                movieDetails[indices[i]].startsector = CdPosToInt(&fmv_file.pos);
            } else {
                movieDetails[indices[i]].startsector = 0;
            }
        }
        i++;
    } while (i < 30);
}


/**
 * Selects and plays one full-screen movie, then releases its decoder buffers.
 *
 * @param track Encoded movie request; negative values select `~track`, while
 *              nonnegative values advance the cycling movie list.
 *
 * Address: 0x8007EEA0
 */
void GameFMV_PlayMovie(int track) {
    DISPENV disp;
    short old_screen_w;
    CdlATV atv;

    if (GameFMV_Active) {
        return;
    }

    GameFMV_IsMovie = 1;
    GameFMV_SetStartTrack(track);

    GameFMV_StartX = (0x140 - GameFMV_Width) / 2;
    GameFMV_EndX = GameFMV_StartX
        + (GameFMV_Is24Bit ? (GameFMV_Width * 3) / 2 : GameFMV_Width);
    GameFMV_StartY = (0xf0 - GameFMV_Height) / 2;
    GameFMV_RewindSwitch = 0;

    if (movieDetails[GameFMV_CurrentTrack].startsector == 0) {
        return;
    }

    decodeSlice.x = GameFMV_StartX;
    decodeSlice.y = GameFMV_StartY;
    decodeSlice.h = GameFMV_Height;
    decodeSlice.w = GameFMV_Is24Bit ? 0x18 : 0x10;

    {
        currentDecodeBuffer = 0;
        currentVlcBuffer = 0;
        sendVLC = 0;
        old_screen_w = DoubleBuffer[0].Disp.screen.w;
        int i = 0;
        unsigned int **vlc_buffer = GameFMV_VlcBuffers;
        do {
            *vlc_buffer = static_cast<unsigned int *>(
                Mem_New(GameFMV_VLCBufferSize, 1, 0));
            if (*vlc_buffer == 0) {
                goto InsufficientMemory;
            }
            i++;
            vlc_buffer++;
        } while (i < 2);
    }

    GameFMV_RingBuffer = static_cast<unsigned long *>(Mem_New(
        GameFMV_RingBufferSectors << 11, 1, 0));
    if (GameFMV_RingBuffer == 0) {
        goto FreeVlcBuffers;
    }

    {
        GameFMV_SliceBuffers = 2;
        int i = 0;
        unsigned short **slice_buffer = decodeSliceBuffer;
        do {
            unsigned int buffer_size = GameFMV_Height;
            if (GameFMV_Is24Bit) {
                buffer_size = (buffer_size * 3) << 4;
            } else {
                buffer_size <<= 5;
            }
            *slice_buffer = static_cast<unsigned short *>(
                Mem_New(buffer_size, 1, 0));
            if (*slice_buffer == 0) {
                goto InsufficientMemory;
            }
            slice_buffer++;
        } while (static_cast<int>(GameFMV_SliceBuffers) > ++i);
    }

    DecDCTvlcBuild3();
    GameFMV_Active = 1;
    Front_ClearScreen();

    SetDefDispEnv(&disp, 0, 0x100, 0x140, 0xf0);
    VSync(0);
    PutDispEnv(&disp);

    atv.val2 = 0;
    atv.val3 = 0;
    if (track == 3) {
        atv.val0 = 0xc0;
        atv.val1 = atv.val0;
    } else {
        int fmv_level;
        if (MIDILEVEL < 0x40) {
            fmv_level = MIDILEVEL * 3;
        } else {
            fmv_level = ((MIDILEVEL - 0x40) * 0x3f) / 0x40 + 0xc0;
        }
        atv.val0 = fmv_level;
        atv.val1 = atv.val0;
    }
    CdMix(&atv);

    strInit();
    Pad_Update();
    Pad[0].Start.Triggered = 0;
    Pad[0].X.Triggered = 0;

    while (strNextVlc(1) == 0) {
    }

    {
        unsigned char result[8];
        int count;

        for (;;) {
            DecDCTin(
                reinterpret_cast<unsigned long *>(
                    GameFMV_VlcBuffers[currentVlcBuffer]),
                GameFMV_Is24Bit ? 3 : 2);
            DecDCTout(
                reinterpret_cast<unsigned long *>(decodeSliceBuffer[currentDecodeBuffer]),
                (decodeSlice.w * decodeSlice.h) / 2);

            CdControl(1, 0, result);
            if (result[0] & 0x10) {
                handleLidOpen();
                break;
            }

            count = 0x800000;
            while (strNextVlc(1) == 0) {
                if (count == 0) {
                    goto StopMovie;
                }
                count--;
            }
            if (count == 0) {
                goto StopMovie;
            }

            count = 0x800000;
            while (decodeSlice.x != GameFMV_StartX) {
                if (count == 0) {
                    goto StopMovie;
                }
                count--;
            }
            if (count == 0) {
                goto StopMovie;
            }

            SetDefDispEnv(
                &disp,
                0,
                decodeSlice.y < 0x100 ? 0x100 : 0,
                GameFMV_Is24Bit ? 0x1e0 : 0x140,
                0xf0);
            if (GameFMV_Is24Bit) {
                disp.isrgb24 = 1;
                disp.disp.w = (short)((disp.disp.w * 2) / 3);
            }
            VSync(0);
            PutDispEnv(&disp);

            if (GameFMV_RewindSwitch) {
                break;
            }
            Pad_Update();
            if (Pad[0].Start.Triggered || Pad[0].X.Triggered) {
                break;
            }
        }
    }

StopMovie:
    Pad_ClearTriggers(&Pad[0]);
    GameFMV_Active = 0;
    Front_ClearScreen();

    disp.isrgb24 = 0;
    disp.disp.w = 0x200;
    disp.screen.w = old_screen_w;
    VSync(0);
    PutDispEnv(&disp);

    atv.val0 = 0;
    atv.val1 = 0;
    atv.val2 = 0;
    atv.val3 = 0;
    CdMix(&atv);
    DecDCToutCallback(0);
    StUnSetRing();
    CdControlB(9, 0, 0);

InsufficientMemory:
    if (GameFMV_RingBuffer) {
        Mem_Delete(GameFMV_RingBuffer);
        GameFMV_RingBuffer = 0;
    }

FreeVlcBuffers:
    {
        int i = 0;
        unsigned int **vlc_buffer = GameFMV_VlcBuffers;
        do {
            if (*vlc_buffer) {
                Mem_Delete(*vlc_buffer);
                *vlc_buffer = 0;
            }
            i++;
            vlc_buffer++;
        } while (i < 2);
    }

    {
        int i = 0;
        unsigned int count = GameFMV_SliceBuffers;
        if (count) {
            unsigned short **slice_buffer = decodeSliceBuffer;
            do {
                if (*slice_buffer) {
                    Mem_Delete(*slice_buffer);
                    count = GameFMV_SliceBuffers;
                    *slice_buffer = 0;
                }
                i++;
                slice_buffer++;
            } while (i < static_cast<int>(count));
        }
    }
}


/**
 * Selects a movie and copies its geometry and decoder metadata into active state.
 *
 * @param track Movie index, or a negative encoded request for the cycle list.
 *
 * Address: 0x8007F3EC
 */
void GameFMV_SetStartTrack(int track) {
    if (GameFMV_Active != 0) {
        return;
    }

    if (track < 0) {
        track = ~track;
    } else {
        track = CycleTracks;
        CycleTracks++;
        while (CycleTracks < 30) {
            if (movieDetails[CycleTracks].width == 0x60) {
                break;
            }
            CycleTracks++;
        }
        if (CycleTracks == 30) {
            CycleTracks = 2;
        }
    }

    GameFMV_Width = movieDetails[track].width;
    GameFMV_Height = movieDetails[track].height;
    GameFMV_EndFrame = movieDetails[track].endframe;
    GameFMV_VLCBufferSize = movieDetails[track].vlcbuffersize;
    GameFMV_RingBufferSectors = movieDetails[track].ringbuffersectors;
    GameFMV_Is24Bit = GameFMV_Read24BitFlag(movieDetails[track]);
    GameFMV_MusicScale = movieDetails[track].musicScale;
    GameFMV_StartSector = movieDetails[track].startsector;
    GameFMV_CurrentTrack = static_cast<u8>(track);
}


/**
 * Starts asynchronous playback for the selected movie.
 *
 * @param Decoding Nonzero to allocate VLC/slice buffers and decode stream data.
 *
 * Address: 0x8007F4E4
 */
void GameFMV_StartFMV(int Decoding) {
    if (GameFMV_Active) goto done;

    if (movieDetails[GameFMV_CurrentTrack].startsector == 0) goto done;

    decodeSlice.x = GameFMV_StartX;
    decodeSlice.y = GameFMV_StartY;
    {
        int w = 0x10;
        if (GameFMV_Is24Bit) {
            w = 0x18;
        }
        decodeSlice.w = static_cast<s16>(w);
    }
    alreadyHandledPause = 0;
    GameFMV_IsMovie = 0;
    currentDecodeBuffer = 0;
    currentVlcBuffer = 0;
    sendVLC = 0;
    decodeSlice.h = static_cast<s16>(GameFMV_Height);

    if (Decoding) {
        int i = 0;
        unsigned int **vlc_buffer = GameFMV_AsyncVlcBuffers;
        do {
            *vlc_buffer = static_cast<unsigned int *>(
                Mem_New(GameFMV_VLCBufferSize, 1, 1));
            i++;
            vlc_buffer++;
        } while (i < 2);
    }

    GameFMV_AsyncRingBuffer = static_cast<unsigned long *>(
        Mem_New(GameFMV_RingBufferSectors << 11, 1, 1));
    {
        unsigned int slice_count = GameFMV_Width;
        slice_count = slice_count >> 4;
        GameFMV_SliceBuffers = static_cast<u8>(slice_count);

        if (Decoding) {
            int slice_mask = slice_count & 0xff;
            if (slice_mask) {
                int i = 0;
                unsigned short **slice_buffer = decodeSliceBuffer;
                do {
                    *slice_buffer = static_cast<unsigned short *>(Mem_New(
                        static_cast<unsigned int>(GameFMV_Height) << 5, 1, 1));
                    i++;
                    slice_buffer++;
                } while (i < static_cast<int>(GameFMV_SliceBuffers));
            }
            DecDCTvlcBuild3();
        }
    }

    GameFMV_Active = 1;
    GameFMV_RewindSwitch = 0;
    strInit();
    strNextVlc(1);

done:
    ;
}


/**
 * Stops asynchronous playback and releases all decoder-owned memory.
 *
 * Address: 0x8007F644
 */
void GameFMV_StopFMV(void) {
    CdlATV atv;
    CdlLOC pos;
    RECT r;

    GameFMV_NumSoundSources = 0;
    GameFMV_FrameBufferCopyActive = 0;

    if (GameFMV_Active) {

    atv.val0 = 0;
    atv.val1 = 0;
    atv.val2 = 0;
    atv.val3 = 0;
    CdMix(&atv);

    GameFMV_Active = 0;
    DecDCToutCallback(0);
    StUnSetRing();

    CdIntToPos(GameFMV_StartSector, &pos);
    CdControl(0x16, reinterpret_cast<u_char *>(&pos), 0);

    if (GameFMV_AsyncRingBuffer) {
        Mem_Delete(GameFMV_AsyncRingBuffer);
        GameFMV_AsyncRingBuffer = 0;
    }

    {
        int i = 0;
        int slice_count = GameFMV_SliceBuffers;
        if (slice_count) {
            unsigned short **slice_buffer = decodeSliceBuffer;
            do {
                if (*slice_buffer) {
                    Mem_Delete(*slice_buffer);
                    slice_count = GameFMV_SliceBuffers;
                    *slice_buffer = 0;
                }
                i++;
                slice_buffer++;
            } while (i < slice_count);
        }
    }

    {
        int i = 0;
        unsigned int **vlc_buffer = GameFMV_AsyncVlcBuffers;
        do {
            if (*vlc_buffer) {
                Mem_Delete(*vlc_buffer);
                *vlc_buffer = 0;
            }
            i++;
            vlc_buffer++;
        } while (i < 2);
    }

    if (GameFMV_StartX != -1) {
        r.x = GameFMV_StartX;
        r.y = GameFMV_StartY;
        r.w = static_cast<s16>(GameFMV_Width);
        r.h = static_cast<s16>(GameFMV_Height);
        ClearImage(&r, 1, 1, 1);
    }

    } // end if (GameFMV_Active)
}


/**
 * Advances asynchronous decoding and synchronizes the CD mask with pause state.
 *
 * @param DecodeData Nonzero to VLC-decode the next available stream sector.
 *
 * Address: 0x8007F79C
 */
void GameFMV_SyncFMV(int DecodeData) {
    if (GameFMV_Active) {

    DecDCTvlcBuild3();

    int paused = alreadyHandledPause;
    if (paused) goto pause_check;

    if (GameFMV_RewindSwitch) {
        GameFMV_RewindSwitch = 0;
        strKickCD();
    }

    if (sendVLC) {
        DecDCTin(reinterpret_cast<unsigned long *>(
            GameFMV_AsyncVlcBuffers[currentVlcBuffer]), 2);

        int slice_width = decodeSlice.w;
        int slice_height = decodeSlice.h;

        unsigned short **slice_buffers = decodeSliceBuffer;
        int decode_index = currentDecodeBuffer;
        currentVlcBuffer = 1 - currentVlcBuffer;
        unsigned long *output_buffer = reinterpret_cast<unsigned long *>(
            slice_buffers[decode_index]);

        DecDCTout(output_buffer, (slice_width * slice_height) / 2);
    }

    sendVLC = strNextVlc(DecodeData);
    paused = alreadyHandledPause;

pause_check:
    if (GamePaused || GameFrozen || MusicStopped) {
        if (paused) goto end;
        alreadyHandledPause = 1;
        StSetMask(1, 0, -1);
        CdControlB(9, 0, 0);
    } else {
        if (!paused) goto end;
        alreadyHandledPause = 0;
        StSetMask(0, 0, -1);
        CdRead2(0x1e0);
    }

    } // end if (GameFMV_Active)
end:
    ;
}


/**
 * Waits for the CD lid to close and for the drive to report a ready state.
 *
 * The original control flow shares one `CdControlB` call site. Its two argument
 * registers remain constrained: an ordinary typed merge changed 27 of 28
 * instructions, while a direct pair of loops changed four instructions and one
 * relocation.
 *
 * Address: 0x8007F910
 */
static void handleLidOpen(void) {
    unsigned char result[8];
    int command;
    register u_char *arg1 asm("$5");
    register u_char *arg2 asm("$6");
    do {
        command = 1;
        arg1 = 0;
        arg2 = result;
        CdControlB(command, arg1, arg2);
    } while (result[0] & 0x10);

    command = 0x13;
    arg1 = 0;
    arg2 = 0;
    goto do_control;

wait_ready:
    arg1 = 0;
    arg2 = result;
do_control:
    CdControlB(command, arg1, arg2);
    if (!(result[0] & 2)) {
        command = 1;
        goto wait_ready;
    }
}


/**
 * Initializes the MDEC stream ring and starts reading the selected track.
 *
 * Address: 0x8007F980
 */
static void strInit(void) {
    DecDCTReset(0);
    DecDCToutCallback(strCallback);
    StSetRing(GameFMV_AsyncRingBuffer, GameFMV_RingBufferSectors);
    StSetStream(GameFMV_Is24Bit, 0, -1, 0, 0);
    Redbook_XACheckSectorOnVSync = 0;
    Redbook_XAModeSet = 0;

    strKickCD();
    CdControlF(12, 0);
}


/**
 * Uploads one completed decode slice and schedules the next MDEC output slice.
 *
 * Address: 0x8007F9F8
 */
static void strCallback(void) {
    RECT old_slice;
    unsigned long *p_old_buffer;

    if (GameFMV_Is24Bit) {
        if (StCdIntrFlag) {
            StCdInterrupt();
            // The second view preserves the target object's distinct read and
            // write symbol records for this interrupt-owned SDK flag.
            GameFMV_StCdIntrFlagWrite = 0;
        }
    }

    {
        int decode_index = currentDecodeBuffer;
        int slice_count = GameFMV_SliceBuffers;
        unsigned short **slice_buffers = decodeSliceBuffer;
        old_slice = decodeSlice;
        int next_decode_index = decode_index + 1;
        unsigned short **old_buffer_entry = slice_buffers + decode_index;
        currentDecodeBuffer = next_decode_index;
        p_old_buffer = reinterpret_cast<unsigned long *>(*old_buffer_entry);
        if (next_decode_index >= slice_count) {
            currentDecodeBuffer = 0;
        }
    }

    {
        u16 slice_x = static_cast<u16>(decodeSlice.x);
        u16 word_width = static_cast<u16>(decodeSlice.w);
        int next_x = slice_x + word_width;
        decodeSlice.x = static_cast<s16>(next_x);
        if (static_cast<s16>(next_x) < static_cast<int>(GameFMV_EndX)) {
            int slice_width = static_cast<s16>(word_width);
            int slice_height = decodeSlice.h;
            unsigned long *output_buffer = reinterpret_cast<unsigned long *>(
                decodeSliceBuffer[currentDecodeBuffer]);
            DecDCTout(output_buffer, (slice_width * slice_height) / 2);
        } else {
            decodeSlice.x = GameFMV_StartX;
            if (GameFMV_IsMovie) {
                s16 signed_y = decodeSlice.y;
                u16 unsigned_y = static_cast<u16>(decodeSlice.y);
                if (signed_y < 0x100) {
                    decodeSlice.y = static_cast<s16>(unsigned_y + 0x100);
                } else {
                    decodeSlice.y = static_cast<s16>(unsigned_y - 0x100);
                }
            }
        }
    }

    if (GameFMV_StartX != -1) {
        LoadImage(&old_slice, p_old_buffer);
    }
}


/**
 * Acquires the next stream sector and updates movie frame/rewind state.
 *
 * @return Ring-buffer address for the sector, or null while no sector is ready.
 *
 * Address: 0x8007FB44
 */
static unsigned long *strNext(void) {
    unsigned long *addr;
    StHEADER *sector;
    int err = StGetNext(&addr, reinterpret_cast<u_long **>(&sector));
    if (err != 0) return 0;
    int end_frame = GameFMV_EndFrame;
    int frame = sector->frameCount;
    GameFMV_CurrentFrame = frame;
    if (frame >= end_frame) {
        GameFMV_RewindSwitch = 1;
    }
    return addr;
}


/**
 * Consumes the next stream sector and optionally expands its VLC payload.
 *
 * @param DecodeData Nonzero to decode into the active VLC buffer.
 * @return `DecodeData` when a sector was consumed, otherwise zero.
 *
 * Address: 0x8007FB98
 */
static int strNextVlc(int DecodeData) {
    unsigned long *next = strNext();
    if (next == 0) return 0;
    if (DecodeData) {
        DecDCTvlc3(
            reinterpret_cast<const unsigned int *>(next),
            GameFMV_AsyncVlcBuffers[currentVlcBuffer]
        );
    }
    StFreeRing(next);
    return DecodeData;
}


/**
 * Seeks to the selected movie and starts double-speed streaming reads.
 *
 * Address: 0x8007FC00
 */
static void strKickCD(void) {
    CdlLOC loc;
    unsigned char param = 0x80;
    CdIntToPos(GameFMV_StartSector, &loc);
    do {
        do {} while (CdControl(2, reinterpret_cast<u_char *>(&loc), 0) == 0);
        do {} while (CdControl(0xe, &param, 0) == 0);
    } while (CdRead2(0x1e0) == 0);
}
