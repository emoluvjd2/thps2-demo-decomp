#if defined(VIDEO_LAYOUT_ONLY) && \
    !defined(VIDEO_HIGHLIGHT_LAYOUT) && !defined(VIDEO_HEAD_LAYOUT)
#error VIDEO_LAYOUT_ONLY requires VIDEO_HIGHLIGHT_LAYOUT or VIDEO_HEAD_LAYOUT
#endif

#ifndef VIDEO_LAYOUT_ONLY
#ifndef VIDEO_INC_H
#define VIDEO_INC_H

struct SVideoHead;
struct CBruce;

#ifdef VIDEO_OWNER_DECLARATIONS
#define VIDEO_SHARED_STATE_DECL_ATTR
#else
#define VIDEO_SHARED_STATE_DECL_ATTR __attribute__((section(".data")))
#endif

/** Selects full-replay or highlight playback for the active replay. */
extern "C" int GVideoReplay VIDEO_SHARED_STATE_DECL_ATTR; // 0x800DA7F4

#undef VIDEO_SHARED_STATE_DECL_ATTR

/** Replay stream mode used by the recorder and playback engine. */
enum EVideoMode
{
    VIDEO_OFF = 0,
    VIDEO_RECORD = 1,
    VIDEO_PLAYBACK = 2,
};

/** Record one positional sound command in the active replay stream. */
void Video_SFXPlay(int sound, short volume, int pitchOffset);

/** Record one skater blood-splat event in the active replay stream. */
void Video_BloodSplat(CBruce *skater, int bodyPart, int maxBloodBits);

/** Mark the start of one replay highlight window. */
void Video_HighlightStart(CBruce *skater);

/** Close the active replay highlight window with its score. */
void Video_HighlightEnd(CBruce *skater, int score);

/** Sort and merge the recorded highlight windows. */
void Video_SortAndMerge(void);

/** Copy replay metadata into one serialized video header. */
void Video_SetHeader(SVideoHead *head, int skaterNumber);

/** Initialize replay storage that persists across level restarts. */
void Video_InitOnce(void);

/** Initialize the replay subsystem for one level. */
void Video_Init(void);

/** Synchronize recorded objects when the active replay requires it. */
void Video_SyncObsIfNeeded(void);

/** Release the active replay buffers and transient state. */
void Video_Cleanup(void);

/** Start recording or playing one skater replay stream. */
void Video_Start(CBruce *skater, EVideoMode mode,
                 int startFrame, int endFrame);

/** Stop the active replay stream for one skater. */
void Video_Stop(CBruce *skater);

/** Record one frame of skater state. */
void Video_Record(CBruce *skater);

/** Play back one frame of skater state. */
void Video_Playback(CBruce *skater);

/** Apply one frame of paused replay stepping. */
void Video_HandleStepping(void);

#endif // VIDEO_INC_H
#endif // !VIDEO_LAYOUT_ONLY

#if (defined(VIDEO_HIGHLIGHT_LAYOUT) || defined(VIDEO_HEAD_LAYOUT)) && \
    !defined(VIDEO_HIGHLIGHT_LAYOUT_EMITTED)
#define VIDEO_HIGHLIGHT_LAYOUT_EMITTED

/** One recorded highlight window inside a replay stream. */
struct SHighlight {
    short FromFrame;
    short ToFrame;
    int Score;
};

typedef char SHighlight_size_must_be_8[
    (sizeof(struct SHighlight) == 8) ? 1 : -1];
typedef char SHighlight_FromFrame_offset_must_be_0[
    ((unsigned long)&(((struct SHighlight *)0)->FromFrame) == 0) ? 1 : -1];
typedef char SHighlight_ToFrame_offset_must_be_2[
    ((unsigned long)&(((struct SHighlight *)0)->ToFrame) == 2) ? 1 : -1];
typedef char SHighlight_Score_offset_must_be_4[
    ((unsigned long)&(((struct SHighlight *)0)->Score) == 4) ? 1 : -1];

#endif // highlight layout request

#if defined(VIDEO_HEAD_LAYOUT) && !defined(VIDEO_HEAD_LAYOUT_EMITTED)
#define VIDEO_HEAD_LAYOUT_EMITTED

#ifndef VIDEO_HIGHLIGHT_LAYOUT_EMITTED
#error VIDEO_HEAD_LAYOUT requires the SHighlight layout
#endif

/** Serialized replay header stored after the memory-card sector prefix. */
struct SVideoHead {
    unsigned long NumFrames;
    struct SHighlight Highlights[5];
    short NumSkaters;
    short Skater;
    short Skater2;
    short Level;
    short GGame;
    short Checksum;
    char VideoFlags;
    char Padding[7];
};

typedef char SVideoHead_size_must_be_64[
    (sizeof(struct SVideoHead) == 64) ? 1 : -1];
typedef char SVideoHead_NumFrames_offset_must_be_0[
    ((unsigned long)&(((struct SVideoHead *)0)->NumFrames) == 0) ? 1 : -1];
typedef char SVideoHead_Highlights_offset_must_be_4[
    ((unsigned long)&(((struct SVideoHead *)0)->Highlights) == 4) ? 1 : -1];
typedef char SVideoHead_NumSkaters_offset_must_be_44[
    ((unsigned long)&(((struct SVideoHead *)0)->NumSkaters) == 44) ? 1 : -1];
typedef char SVideoHead_Skater_offset_must_be_46[
    ((unsigned long)&(((struct SVideoHead *)0)->Skater) == 46) ? 1 : -1];
typedef char SVideoHead_Skater2_offset_must_be_48[
    ((unsigned long)&(((struct SVideoHead *)0)->Skater2) == 48) ? 1 : -1];
typedef char SVideoHead_Level_offset_must_be_50[
    ((unsigned long)&(((struct SVideoHead *)0)->Level) == 50) ? 1 : -1];
typedef char SVideoHead_GGame_offset_must_be_52[
    ((unsigned long)&(((struct SVideoHead *)0)->GGame) == 52) ? 1 : -1];
typedef char SVideoHead_Checksum_offset_must_be_54[
    ((unsigned long)&(((struct SVideoHead *)0)->Checksum) == 54) ? 1 : -1];
typedef char SVideoHead_VideoFlags_offset_must_be_56[
    ((unsigned long)&(((struct SVideoHead *)0)->VideoFlags) == 56) ? 1 : -1];
typedef char SVideoHead_Padding_offset_must_be_57[
    ((unsigned long)&(((struct SVideoHead *)0)->Padding) == 57) ? 1 : -1];

#endif // VIDEO_HEAD_LAYOUT
