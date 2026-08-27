#ifndef THPS2_M3DINIT_H
#define THPS2_M3DINIT_H

struct STexWibItemInfo;

#ifdef M3DINIT_OWNER_DECLARATIONS
#define M3DINIT_SHARED_STATE_DECL_ATTR
#else
#define M3DINIT_SHARED_STATE_DECL_ATTR __attribute__((section(".data")))
#endif

/** Model part index enlarged for the character-select head preview. */
extern "C" int GHeadPart M3DINIT_SHARED_STATE_DECL_ATTR; // 0x800DC574

/** Vertical offset applied to the enlarged character-select head. */
extern "C" int GHeadUp M3DINIT_SHARED_STATE_DECL_ATTR; // 0x800DC584

#undef M3DINIT_SHARED_STATE_DECL_ATTR

/** Initialize renderer scratch storage and default projection state. */
void M3dInit_InitAtStart(void);

/** Set the display resolution and derived fixed-point pixel aspect. */
void M3dInit_SetResolution(unsigned int X, unsigned int Y);

/** Set the renderer's fog interval and normalized depth shift. */
void M3dInit_SetFoggingParams(int Dummy, int Min, unsigned int Range);

/** Apply runtime fixups to the models and items in a loaded PSX region. */
void M3dInit_ParsePSX(int Region);

/** Mark texture-wibble records whose amplitude data is entirely zero. */
void M3dInit_FlagZeroWibbles(STexWibItemInfo *pTexWibItemInfo);

#endif // THPS2_M3DINIT_H
