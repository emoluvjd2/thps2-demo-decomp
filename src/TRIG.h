#ifndef TRIG_INC_H
#define TRIG_INC_H

struct CBody;
struct CBruce;
struct CVector;
struct SCommandPoint;

#ifdef TRIG_OWNER_DECLARATIONS
#define TRIG_SHARED_STATE_DECL_ATTR
#else
#define TRIG_SHARED_STATE_DECL_ATTR __attribute__((section(".data")))
#endif

/** Number of cheat restart names collected from the active trigger stream. */
extern "C" int NumCheatRestarts TRIG_SHARED_STATE_DECL_ATTR; // 0x800DA268

/** Number of horse restart names collected from the active trigger stream. */
extern "C" int NumHorseRestarts TRIG_SHARED_STATE_DECL_ATTR; // 0x800DA26C

/** Active trigger node used when executing a gameplay restart. */
extern "C" int RestartNode TRIG_SHARED_STATE_DECL_ATTR; // 0x800DA274

/** Region containing the active level's object models. */
extern "C" unsigned char CurrentObjFileRegion
    TRIG_SHARED_STATE_DECL_ATTR; // 0x800DA2C8

/** Region containing the shared item-pickup models. */
extern "C" unsigned char CurrentItemsFileRegion
    TRIG_SHARED_STATE_DECL_ATTR; // 0x800DA2C9

/** Region containing the shared medal-pickup models. */
extern "C" unsigned char CurrentMedalsFileRegion
    TRIG_SHARED_STATE_DECL_ATTR; // 0x800DA2CA

#undef TRIG_SHARED_STATE_DECL_ATTR

/** Score and display metadata for one trigger-defined gap trick. */
struct SGapTrick {
    short Type;
    short Score;
    char *Name;
};

typedef char SGapTrick_Type_offset_must_be_0[
    ((unsigned long)&(((SGapTrick *)0)->Type) == 0) ? 1 : -1];
typedef char SGapTrick_Score_offset_must_be_2[
    ((unsigned long)&(((SGapTrick *)0)->Score) == 2) ? 1 : -1];
typedef char SGapTrick_Name_offset_must_be_4[
    ((unsigned long)&(((SGapTrick *)0)->Name) == 4) ? 1 : -1];
typedef char SGapTrick_size_must_be_8[
    (sizeof(SGapTrick) == 8) ? 1 : -1];

/** Clear the trigger-file menu entries. */
void Trig_ClearTrigMenu(void);

/** Delete every active command point. */
void Trig_DeleteCommandPoints(void);

/** Clear command-point collision state for one player. */
void Trig_ResetCPCollisionFlags(int player);

/** Clear one player's executed bit after leaving each command point. */
void Trig_ResetCPExecutedFlags(int player);

/** Find and trigger a command point by checksum. */
SCommandPoint *Trig_TriggerCommandPoint(
    unsigned int checksum, bool shouldAssert);

/** Select the named restart node. */
void Trig_SetRestart(char *name);

/** Align one skater with the selected restart node. */
void FudgeAngles(CBruce *skater);

/** Move mechanics to the selected restart and execute its commands. */
void Trig_ExecuteRestart(void);

/** Delete the currently loaded trigger file buffer. */
void Trig_DeleteTrigFile(void);

/** Load one compiled trigger stream from disc. */
void Trig_LoadTRG(char *filename);

/** Read one encoded flag from a trigger flag stream. */
unsigned char GetFlag(unsigned char flag, unsigned char *flags);

/** Skip the encoded flags at the head of a trigger payload. */
char *SkipFlags(unsigned char *flags);

/**
 * Read a trigger node's position and return the unsigned word payload that
 * immediately follows it.
 */
unsigned short *Trig_GetPosition(CVector &position, int node);

/** Delivers a pulse directly to one trigger node. */
void Trig_SendPulseToNode(int node);

/** Create the object described by one trigger node. */
CBody *Trig_CreateObject(int node);

/** Delivers a pulse to each trigger node referenced by a link list. */
void Trig_SendPulse(unsigned short *pLinkInfo);

/** Deliver one signal to matching bodies in a list. */
void SendSignalToNode(CBody *body, int node);

/** Deliver one signal through every node in a trigger link list. */
void SendSignalToLinks(unsigned short *pLinkInfo);

/** Suspend matching bodies from one owner list. */
void SendSuspend(CBody **list, int node);

/** Restore suspended bodies associated with one trigger node. */
void SendUnSuspend(int node);

/** Suspend, restore, or activate every node in one link list. */
void SendSuspendOrActivate(unsigned short *pLinkInfo, int signalType);

/** Returns the trigger-link list associated with a node. */
unsigned short *Trig_GetLinksPointer(int node);

/** Skip one zero-terminated string in a trigger command stream. */
unsigned short *SkipString(char *text);

/** Allocate the temporary dual-buffer command stream. */
void Trig_AllocateDualBuffer(unsigned int size);

/** Release the temporary dual-buffer command stream. */
void Trig_RemoveDualBuffer(void);

/** Perform the initial parse pass for the loaded trigger stream. */
void Trig_InitialParseTRGFile(void);

/** Parse the active trigger stream into runtime commands. */
void Trig_ParseTRGFile(void);

#endif // TRIG_INC_H
