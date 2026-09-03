#ifndef SPOOL_INC_H
#define SPOOL_INC_H

struct Item;

/** Finds the loaded environment item with the requested mesh checksum. */
Item *Spool_FindEnviroItem(unsigned int Checksum);

/** Finds a model index by checksum within a loaded PSX region. */
unsigned int Spool_GetModel(unsigned int checksum, int region);

#ifndef SPOOL_OWNER_LOCAL_SERVICE_DECLS

struct Texture;
struct SAnimFrame;

/** Finds the loaded texture entry with the requested checksum. */
Texture *Spool_FindTextureEntry(unsigned int checksum);

/** Finds the loaded animation-frame sequence with the requested name. */
SAnimFrame *Spool_FindAnim(char *name);

/** Initializes the PSX spool and streaming state. */
void Spool_Init(void);

/** Finds an unused PSX region slot. */
int Spool_FreeRegion(void);

/** Advances past the tagged packet prefix in a PSX payload. */
unsigned int *Spool_SkipPackets(unsigned int *fileBase);

/** Decrements texture references owned by one PSX region. */
void DecrementTextureUsage(int region);

/** Releases texture entries whose usage count reached zero. */
void Spool_RemoveUnusedTextures(void);

/** Processes the newly loaded PSX payload in one region slot. */
void ProcessNewPSX(int region);

/** Finds a loaded PSX region by name. */
int Spool_FindRegion(char *name);

/** Clears a loaded PSX region by name. */
void Spool_ClearPSX(char *regionName);

/** Clears one PSX region with the requested cleanup policy. */
void ClearRegion(int region, int cleanupTextures, int keepInMemory);

/** Clears both environment region slots. */
void Spool_ClearEnvironmentRegions(void);

/** Strips unused animation data from one PSX region. */
void Spool_StripAnim(int region);

/** Strips model data using a terminated mesh-index list. */
void Spool_StripModel(int region, short *meshIndices);

/** Clears every non-persistent PSX region. */
void Spool_ClearAllPSXs(void);

/** Queues a PSX file and returns its assigned region slot. */
int Spool_PSX(char *Name, int IsEnviro, int KeepTextures, int WhichHeap);

/** Advances the asynchronous PSX spool state machine. */
void Spool_Continue(void);

/** Blocks until all queued PSX work completes. */
void Spool_Sync(void);

/** Loads the reduced model and animation pair for one skater. */
int Spool_GetReducedSkater(char *modelFile, char *animFile);

/** Loads and parses a PSH hierarchy-name table. */
char *Spool_LoadPSH(
    char *pPSHName, char **pHierarchyArray, int *pPartCount,
    int maxPartNames);

#endif // SPOOL_OWNER_LOCAL_SERVICE_DECLS

#endif // SPOOL_INC_H

#if defined(SPOOL_DECLARE_OWNER_ADDRESS_VIEWS)
/** Absolute-address views of SPOOL's owner-local filename strings. */
namespace SpoolAddressViews
{
extern "C"
{
extern char DAT_800daa08[];
extern char s__s_psh_800daa14[];
#include "SPOOLPARTPREFIX.h"
extern char s_Shrinking_animation_800b2bb0[];
}
}
#undef SPOOL_DECLARE_OWNER_ADDRESS_VIEWS
#endif

#if defined(SPOOL_QUEUE_LAYOUT) && !defined(SPOOL_QUEUE_LAYOUT_EMITTED)
#define SPOOL_QUEUE_LAYOUT_EMITTED

/** One pending spool request in the packed global queue. */
struct SQueue {
    char Name[13];
    unsigned char Occupied;
    unsigned char FileType;
    unsigned char Region;
    unsigned char KeepTextures;
};

typedef char SQueue_size_must_be_17[
    (sizeof(SQueue) == 17) ? 1 : -1];
typedef char SQueue_Name_offset_must_be_0[
    ((unsigned long)&(((SQueue *)0)->Name) == 0) ? 1 : -1];
typedef char SQueue_Occupied_offset_must_be_13[
    ((unsigned long)&(((SQueue *)0)->Occupied) == 13) ? 1 : -1];
typedef char SQueue_FileType_offset_must_be_14[
    ((unsigned long)&(((SQueue *)0)->FileType) == 14) ? 1 : -1];
typedef char SQueue_Region_offset_must_be_15[
    ((unsigned long)&(((SQueue *)0)->Region) == 15) ? 1 : -1];
typedef char SQueue_KeepTextures_offset_must_be_16[
    ((unsigned long)&(((SQueue *)0)->KeepTextures) == 16) ? 1 : -1];

#endif // SPOOL_QUEUE_LAYOUT && !SPOOL_QUEUE_LAYOUT_EMITTED
