#ifndef PAUSEDSONGPOS_INC_H
#define PAUSEDSONGPOS_INC_H

struct SONG_POS;

/** Disc position captured while XA playback is paused. */
extern "C" SONG_POS Paused_SongPos;

#endif // PAUSEDSONGPOS_INC_H

#ifdef PAUSEDSONGPOS_DECLARE_SCALAR_ADDRESS_VIEW
/** Four-byte address view required by FRONT's update-state compiler identity. */
namespace FrontPausedSongPositionAddressView
{
extern "C" int Paused_SongPos;
}
#undef PAUSEDSONGPOS_DECLARE_SCALAR_ADDRESS_VIEW
#endif
