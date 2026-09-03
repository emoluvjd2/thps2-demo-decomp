// REDBOOK.cpp -- Reconstructed translation unit
// Original: REDBOOK.CPP
// Functions: 16 reconstructed entries
// Address range: 0x80032890 - 0x800330EC
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#include "REDBOOK.h"
#include "FILEIO.h"
#define SPOOL_QUEUE_LAYOUT
#include "SPOOL.h"
#undef SPOOL_QUEUE_LAYOUT
#include <STDDEF.H>
#include <LIBCD.H>
#include <STDIO.H>

/** Status bytes followed by the disc position returned by CD control calls. */
struct Redbook_CdResponse {
    unsigned char status[5];
    CdlLOC position;
};

typedef char Redbook_CdResponse_size_must_be_9[
    sizeof(Redbook_CdResponse) == 9 ? 1 : -1];

#define SPOOLQUEUE_CONSUMER_DECLARATION
#include "SPOOLQUEUE.h"

/** Locate the CdlLOC payload embedded in a CD command response. */
static inline CdlLOC *Redbook_ResponsePosition(unsigned char *result)
{
    Redbook_CdResponse *response =
        reinterpret_cast<Redbook_CdResponse *>(result);
    return &response->position;
}

// === External declarations (C linkage) ===
// $gp-relative globals (≤8 bytes, accessible via $gp+offset with -G8)
int XAGroupSectorLengths[1][8] __attribute__((section(".data"))) = {{
    2251, 2250, -1, -1, -1, -1, -1, -1,
}};

/*
 * Original REDBOOK.CPP small-data contribution, in target address order.  These
 * were previously undefined `extern` declarations, which `prepare_for_aspsx`
 * turns into `.comm` records that PSYLINK allocates in `.sbss` and exports --
 * fabricating both bytes and XDEFs the original never had.  Defining them here
 * with their recovered initializers is codegen-neutral for every referencing
 * TU, which still sees only the header's `extern`.
 */
// Not `const`: GCC 2.x discards an unreferenced const array outright, and this
// word is the first of the owner's small-data contribution.
int XAGroupStartingSector[1] __attribute__((section(".sdata"))) = {0};
int Redbook_XACurrentPriority __attribute__((section(".sdata"))) = -1;
int Redbook_XAStartSector __attribute__((section(".sdata"))) = 0;
int Redbook_XACompleteTimer __attribute__((section(".sdata"))) = 0;
bool Redbook_XACheckSectorOnVSync __attribute__((section(".sdata"))) = false;
bool Redbook_XAModeSet __attribute__((section(".sdata"))) = false;
int Redbook_XAPaused __attribute__((section(".sdata"))) = 0;
bool Redbook_XAValid __attribute__((section(".sdata"))) = false;
int Redbook_XACurrentGroup __attribute__((section(".sdata"))) = 0;
int Redbook_XACurrentChannel __attribute__((section(".sdata"))) = 0;
int Redbook_XATryAgain __attribute__((section(".sdata"))) = 0;
int Redbook_XASeeking __attribute__((section(".sdata"))) = 0;
int Redbook_XAFadeScale __attribute__((section(".sdata"))) = 255;
int Redbook_XAFading __attribute__((section(".sdata"))) = 0;

/** XA disc path and the CD error diagnostic in original small-data order. */
static char XA_FileName[16]
    __attribute__((section(".sdata"))) = "\\COMPILED.XA;1";
static char XA_ErrorMsg[16]
    __attribute__((section(".sdata"))) = "CdlDiskError!\n";

/* Small-data words that follow the two strings in the original contribution. */
int Redbook_XAEndSector __attribute__((section(".sdata"))) = 0;
int Redbook_XACurrentSector __attribute__((section(".sdata"))) = 0;
u8 CdControlComeback[8] __attribute__((section(".sdata"))) = {0};

static void Redbook_InitialRand(int a);
static int Redbook_Rnd(int n);
bool Redbook_XAPlay(int group, int channel);

static int RandSeed;
static int RandA;
static int RandB;

// === Function implementations (address order) ===


/**
 * Seed the private XA track-selection generator.
 *
 * Address: 0x80032890
 *
 * @param a  Initial generator state.
 */
static void Redbook_InitialRand(int a) {
    RandSeed = a;
    RandA = 0x12B9B0A1;
    RandB = 0x0AA2FB3F;
}


/**
 * Advance the private generator and scale its low word to a range.
 *
 * Address: 0x800328B4
 *
 * @param n  Exclusive upper bound.
 * @return A pseudo-random value scaled by n.
 */
static int Redbook_Rnd(int n) {
    RandSeed = RandSeed * RandA +
        RandB;
    RandA = (RandA ^ RandSeed) +
        (RandSeed >> 4);
    int addend = RandB + 0xEFEFEFF0;
    RandB = addend + (RandSeed >> 3);
    return ((RandSeed & 0xFFFF) * n) >> 16;
}


/**
 * Select the CD-ROM mode used for XA playback.
 *
 * Address: 0x80032910
 */
void Redbook_SetXAMode(void) {
    u8 mode;
    mode = 0xC8;
    CdControl(0x0E, &mode, CdControlComeback);
    if (CdControlComeback[0]) {
        Redbook_XAModeSet = 1;
    }
}


/**
 * Retained compatibility hook for callers that gate XA playback.
 *
 * Address: 0x80032954
 *
 * @param allow  Requested playback permission; unused by this build.
 */
void Redbook_XAAllow(bool allow) {
}


/**
 * Update the XA attenuation and apply it to the CD mixer.
 *
 * Address: 0x8003295C
 */
void Redbook_XAUpdateVolume(void) {
    int vl;
    int vr;
    CdlATV atv;

    if (Redbook_XAFading) {
        Redbook_XAFadeScale -= 3;
        if (Redbook_XAFadeScale < 0) {
            Redbook_XAFadeScale = 0;
        }
    }

    if (Redbook_XAFading) {
        Redbook_XAFadeScale = 0;
    }

    vl = vr = XALEVEL * Redbook_XAFadeScale / 255;
    if (vl > 255) {
        vl = 255;
    }
    if (vr > 255) {
        vr = 255;
    }

    atv.val2 = atv.val3 = 0;
    vl = vl * 60 / 100;
    vr = vr * 60 / 100;
    atv.val0 = vl;
    atv.val1 = vr;
    CdMix(&atv);
}


/**
 * Locate the XA archive and reset playback state.
 *
 * Address: 0x80032A5C
 */
void Redbook_XAInit(void) {
    CdlFILE p_xa_file;

    Redbook_InitialRand(0x75DE7);
    CdlFILE *found = CdSearchFile(&p_xa_file, XA_FileName);
    if (found) {
        Redbook_XAStartSector = CdPosToInt(&p_xa_file.pos);
        Redbook_XAValid = 1;
    } else {
        Redbook_XAValid = 0;
    }
    Redbook_XACompleteTimer = 30;
    Redbook_XAPaused = 0;
    Redbook_XACurrentGroup = 0;
    Redbook_XACurrentChannel = 0;
    Redbook_XATryAgain = 0;
    Redbook_XASeeking = 0;
    Redbook_XACheckSectorOnVSync = 0;
    Redbook_XAModeSet = 0;
    Redbook_XACurrentSector = Redbook_XAStartSector;
    Redbook_XAEndSector = Redbook_XAStartSector;
}


/**
 * Pick a random valid XA song-table entry, then start that group/track.
 *
 * Address: 0x80032AE4
 */
bool Redbook_XAPlayRandom(void) {
    int selectedGroup;
    int selectedChannel;
    int track;
    do {
        track = Redbook_Rnd(8);
        /*
         * GCC 2.91 otherwise coalesces the quotient with group/channel and
         * changes most of this small function.  Keep the measured register
         * lifetimes, while expressing the table lookup with its real type.
         */
        register int quotient asm("$5") = track / 8;
        asm volatile("" : "+r"(quotient));
        register int group asm("$6") = quotient;
        selectedGroup = group;
        register int channel asm("$5") = track - (group << 3);
        selectedChannel = channel;
    } while (XAGroupSectorLengths[selectedGroup][selectedChannel] == -1);
    return Redbook_XAPlay(selectedGroup, selectedChannel);
}


/**
 * Check whether the CD head has reached the requested XA sector.
 *
 * Address: 0x80032B60
 *
 * @return 1 after read-ahead begins, otherwise 0 while seeking continues.
 */
int Redbook_XASeek(void) {
    CdlLOC pos;
    CdIntToPos(Redbook_XACurrentSector, &pos);
    CdControl(0x11, 0, CdControlComeback);
    int diff = CdPosToInt(Redbook_ResponsePosition(CdControlComeback)) -
        Redbook_XACurrentSector;
    if (diff < 0) {
        diff = -diff;
    }
    if (diff >= 17) {
        return 0;
    }
    CdControl(0x1B, reinterpret_cast<u_char *>(&pos), CdControlComeback);
    if (!CdControlComeback[0]) {
        return 0;
    }
    CdControlF(0x0C, 0);
    Redbook_XACheckSectorOnVSync = 1;
    Redbook_XASeeking = 0;
    return 1;
}


/**
 * Configure and begin an XA group/channel selection.
 *
 * Address: 0x80032C00
 *
 * @param group  XA sector-table group.
 * @param channel  Channel within the group.
 * @return true once playback has started seeking; false when it must retry.
 */
bool Redbook_XAPlay(int group, int channel) {
    CdlFILTER filter;
    CdlLOC pos;

    if (!Redbook_XAValid) {
        return false;
    }
    Redbook_XAFadeScale = 255;
    /*
     * These scheduling anchors keep the target's early table loads ahead of
     * the state stores.  Natural indexed variants were measured and change
     * the legacy compiler's allocation/order, so the residual byte-offset
     * form uses sizeof-derived offsets rather than numeric layout constants.
     */
    register unsigned int w asm("$4");
    asm volatile("" : "=r"(w));
    register int groupByteOffset asm("$2") = group * sizeof(int);
    register const char *groupStartsBase asm("$6") =
        reinterpret_cast<const char *>(XAGroupStartingSector);
    int grpStart = *reinterpret_cast<const int *>(
        groupStartsBase + groupByteOffset);
    int startSec = Redbook_XAStartSector;
    asm volatile("" : : "r"(w));
    SONG_POS *songPos = &Paused_SongPos;
    asm volatile("" : "+r"(songPos));
    Redbook_XAPaused = 0;
    Redbook_XAFading = 0;
    Redbook_XACurrentGroup = group;
    Redbook_XACurrentChannel = channel;
    Redbook_XACurrentSector = startSec + grpStart * 8;
    Redbook_XARemember(songPos);

    {
        register char *sectorLengthsBase asm("$6") =
            reinterpret_cast<char *>(XAGroupSectorLengths);
        int channelByteOffset = channel * sizeof(int);
        int groupByteOffset = group * sizeof(XAGroupSectorLengths[0]);
        Redbook_XAEndSector = Redbook_XACurrentSector +
            *reinterpret_cast<int *>(sectorLengthsBase +
                (channelByteOffset + groupByteOffset)) * 8 - 131;
    }

    if (Loading != 0) {
        goto try_again;
    }

    if (SpoolQ[SpoolPos].Occupied != 0) {
        goto try_again;
    }

    if (SpoolerBlocksFileIO != 0) {
        goto try_again;
    }
    if (GameFMV_Active != 0) {
        goto try_again;
    }

    if (!Redbook_XAModeSet) {
        Redbook_SetXAMode();
        if (!Redbook_XAModeSet) {
            goto try_again;
        }
    }

    {
        /*
         * The sole residue is target #75/#76: the original materializes 1
         * before the `CdControlComeback` address, while GCC schedules those
         * independent instructions in the opposite order.  The complete
         * experiment ledger proves the earlier $a2 table-base bindings are
         * load-bearing; unwinding them fixes this pair but damages the exact
         * prologue.  Keep this plain SDK call shape.
         */
        filter.file = 1;
        filter.chan = channel;
        CdControl(0x0d, reinterpret_cast<u8 *>(&filter), CdControlComeback);
        if (!CdControlComeback[0]) {
            goto try_again;
        }
        CdIntToPos(Redbook_XACurrentSector, &pos);
        CdControlF(0x16, reinterpret_cast<u_char *>(&pos));
        if (!CdControlComeback[0]) {
            goto try_again;
        }
        Redbook_XAUpdateVolume();
        Redbook_XACompleteTimer = 30;
        Redbook_XATryAgain = 0;
        Redbook_XASeeking = 1;
        return true;
    }

try_again:
    Redbook_XATryAgain = 1;
    return false;
}


/**
 * Stop XA playback and reset its mode/priority state.
 *
 * Address: 0x80032DB8
 */
void Redbook_XAStop(void) {
    Redbook_XACheckSectorOnVSync = 0;
    Redbook_XACompleteTimer = 30;
    Redbook_XACurrentPriority = -1;
    Redbook_XAModeSet = 0;
    CdControlF(0x0B, 0);
}

/**
 * Advance to the next valid XA track and play it.
 *
 * Address: 0x80032DF4
 */
void Redbook_XANextTrack(void) {
    Redbook_XAStop();
    int group = Redbook_XACurrentGroup;
    int channel = Redbook_XACurrentChannel;
    do {
        channel = channel + 1;
        if (channel >= 8) {
            group = group + 1;
            channel = 0;
        }
        if (group > 0) {
            group = 0;
        }
    } while (XAGroupSectorLengths[group][channel] == -1);
    Redbook_XAPlay(group, channel);
}


/**
 * Pause or resume XA playback at the remembered disc sector.
 *
 * Address: 0x80032E78
 *
 * @param pauseIt  True to pause; false to resume.
 */
void Redbook_XAPause(bool pauseIt) {
    if (pauseIt == Redbook_XAPaused) {
        return;
    }
    if (pauseIt) {
        CdControlF(0x0B, 0);
        CdControlB(0x09, 0, 0);
        CdControl(0x11, 0, CdControlComeback);
        Redbook_XACurrentSector =
            CdPosToInt(Redbook_ResponsePosition(CdControlComeback));
    } else {
        CdlLOC pos;
        CdIntToPos(Redbook_XACurrentSector, &pos);
        do {
        } while (CdControl(0x02, reinterpret_cast<u_char *>(&pos), 0) == 0);
        CdControl(0x1B, reinterpret_cast<u_char *>(&pos), CdControlComeback);
        CdControlF(0x0C, 0);
    }
    Redbook_XAPaused = pauseIt;
}


/**
 * Advance the XA playback state machine once per update.
 *
 * Address: 0x80032F2C
 */
void Redbook_XAUpdate(void) {
    if (!Redbook_XAValid) return;
    if (Redbook_XAPaused) return;

    if (Redbook_XAFading) {
        Redbook_XAUpdateVolume();
    }

    if (Redbook_XACompleteTimer) {
        Redbook_XACompleteTimer = Redbook_XACompleteTimer - 1;
        return;
    }

    if (Redbook_XATryAgain) {
        Redbook_XAPlay(Redbook_XACurrentGroup, Redbook_XACurrentChannel);
        return;
    }

    if (Redbook_XASeeking) {
        Redbook_XASeek();
        return;
    }

    if (!Redbook_XACheckSectorOnVSync) return;

    int ret = CdSync(1, CdControlComeback);
    if (ret == 5) {
        Redbook_XACheckSectorOnVSync = 0;
        printf(XA_ErrorMsg);
        Redbook_XANextTrack();
        return;
    }
    if (ret != 2) return;

    if (Redbook_XACurrentSector >= Redbook_XAEndSector) {
        Redbook_XANextTrack();
        return;
    }

    int lastCmd = CdLastCom();
    if (lastCmd != 0x11) {
        CdControlF(0x11, 0);
        return;
    }

    ret = CdPosToInt(Redbook_ResponsePosition(CdControlComeback));
    if (ret <= 0) {
        CdControlF(0x11, 0);
        return;
    }

    CdControl(0x01, 0, CdControlComeback);
    if ((CdControlComeback[0] & 0x40) == 0) {
        Redbook_XACurrentSector = ret;
    }
    Redbook_XACompleteTimer = 4;

    CdControlF(0x11, 0);
}


/**
 * Begin reducing the XA playback volume.
 *
 * Address: 0x800330B8
 */
void Redbook_XABeginFade(void) {
    Redbook_XAFadeScale = 255;
    Redbook_XAFading = 1;
}


/**
 * Copy the active XA selection into a song-position record.
 *
 * Address: 0x800330D0
 *
 * @param pSongPos  SongPos to operate on.
 */
void Redbook_XARemember(SONG_POS *pSongPos) {
    // Keep all three source loads ahead of the destination stores.
    int group = Redbook_XACurrentGroup;
    int channel = Redbook_XACurrentChannel;
    int sector = Redbook_XACurrentSector;
    pSongPos->group = group;
    pSongPos->channel = channel;
    pSongPos->sector = sector;
}


/**
 * Restore a remembered XA selection or restart its group/channel.
 *
 * Address: 0x800330EC
 *
 * @param pSongPos  SongPos to operate on.
 */
void Redbook_XARestore(SONG_POS *pSongPos) {
    if (pSongPos->group == Redbook_XACurrentGroup &&
        pSongPos->channel == Redbook_XACurrentChannel) {
        Redbook_XACurrentSector = pSongPos->sector;
    } else {
        Redbook_XAStop();
        Redbook_XAPlay(pSongPos->group, pSongPos->channel);
    }
}
