// Public display-buffer and frame-submission interface.

/*
 * The complete buffer layout is needed by a bounded set of rendering owners,
 * but ordinary DB service consumers should not inherit GPU types or data
 * declarations.  Keep the block re-entrant so a TU may request the layout
 * after first including the API-only interface.
 */
#if defined(DB_BUFFER_LAYOUT) && !defined(DB_BUFFER_LAYOUT_EMITTED)
#define DB_BUFFER_LAYOUT_EMITTED

#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBGPU.H>

/**
 * Owns one display environment, its ordering tables, and polygon buffer.
 */
struct SDoubleBuffer {
    DRAWENV Draw;
    DISPENV Disp;
    int Visible;
    int RenderNow;
    int Rendering;
    int FlipNow;
    int LastVblanks;
    unsigned int *PanelOT;
    unsigned int *OrderingTable;
    unsigned int *ViewportOT;
    int Top;
    RECT Window;
    DR_AREA Clip;
    unsigned char *Polys;
};

typedef char SDoubleBuffer_size_must_be_0xAC[
    (sizeof(SDoubleBuffer) == 0xAC) ? 1 : -1];

#endif // DB_BUFFER_LAYOUT

#ifndef DB_INC_H
#define DB_INC_H

#ifdef DB_OWNER_DECLARATIONS
#define DB_SHARED_STATE_DECL_ATTR
#else
#define DB_SHARED_STATE_DECL_ATTR __attribute__((section(".data")))
#endif

/** Packed clear colour installed in both display-buffer draw environments. */
extern "C" unsigned int Db_SkyColor DB_SHARED_STATE_DECL_ATTR; // 0x800DA324

/** Current write cursor in the active polygon command buffer. */
extern "C" unsigned int *pPoly DB_SHARED_STATE_DECL_ATTR; // 0x800DA344

#undef DB_SHARED_STATE_DECL_ATTR

int Db_OTSize(void);
void Db_UpdateSky(void);
void Db_CallBack(void);
void Db_Vblank(void);
void Db_WaitRender(void);
void Db_Render(void);
void Db_InitAtStart(void);
void Db_Reset(void);
void Db_Init(void);
void Db_FlipClear(void);
void Db_DisplayDraw(void);

#endif // DB_INC_H
