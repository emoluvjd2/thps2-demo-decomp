#ifndef THPS2_M3D_H
#define THPS2_M3D_H

struct SCamera;
struct SViewport;
struct CSuper;

#ifdef M3D_OWNER_DECLARATIONS
#define M3D_RENDER_STATE_DECL_ATTR
#else
#define M3D_RENDER_STATE_DECL_ATTR __attribute__((section(".data")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** Optional colour override used while rendering wireframe geometry. */
extern int ForceWireFrameColor M3D_RENDER_STATE_DECL_ATTR;

/** Ordering-table base selected for the active render pass. */
extern unsigned int *CurrentOrderingTable M3D_RENDER_STATE_DECL_ATTR;

/** Primary ordering-table depth bias used by model rendering. */
extern short M3d_OTPushback M3D_RENDER_STATE_DECL_ATTR; // 0x800DC5A8

/** Secondary ordering-table depth bias used by model rendering. */
extern short M3d_OTPushback2 M3D_RENDER_STATE_DECL_ATTR; // 0x800DC5AA

/** Records whether the current render pass displayed a model. */
extern int GModelVideoDisplayed M3D_RENDER_STATE_DECL_ATTR; // 0x800DC5C0

#ifdef __cplusplus
}
#endif

#undef M3D_RENDER_STATE_DECL_ATTR

/** Configure the renderer for a camera, viewport, and ordering table. */
void M3d_RenderSetup(
    SCamera *camera, SViewport *viewport, unsigned int *orderingTable);

/** Restore shared renderer state after an item-rendering pass. */
void M3d_RenderCleanup(void);

/** Render a linked list of scene items. */
void M3d_Render(void *items);

/** Render a linked list of background scene items. */
void M3d_RenderBackground(void *items);

/** Build the current articulated-model transform for an object. */
void M3d_BuildTransform(CSuper *object);

#endif // THPS2_M3D_H

// The projected/rotated workspace and texture-wibble records are shared by
// three renderer owners.  Keep their complete layouts opt-in and outside the
// service guard so a consumer that already used the opaque M3D interface can
// request the canonical records at its proven declaration point.
#if defined(M3D_VERTEX_WIBBLE_LAYOUTS) && \
    !defined(M3D_VERTEX_WIBBLE_LAYOUTS_EMITTED)
#define M3D_VERTEX_WIBBLE_LAYOUTS_EMITTED

/** Projected screen coordinate and clipping state. */
struct SProjectedVertex {
    short X;
    short Y;
    short z;
    unsigned short Outcode;
};

/** Rotated screen coordinate with its packed special field. */
struct SRotatedVertex {
    short x;
    short y;
    unsigned int Special;
};

/** Per-vertex texture-wibble amplitudes and phases. */
struct STexWibVertInfo {
    unsigned char u;
    unsigned char v;
    unsigned char uAmpPhase;
    unsigned char vAmpPhase;
};

/** Header preceding each inline group of texture-wibble vertex records. */
struct STexWibItemInfo {
    unsigned int ItemOffset;
    short uVel;
    short vVel;
    int Frequency;
    unsigned short NumFaces;
    unsigned char ZeroUAmplitudes;
    unsigned char ZeroVAmplitudes;
};

typedef char SProjectedVertex_X_offset_must_be_0[
    ((unsigned long)&(((SProjectedVertex *)0)->X) == 0) ? 1 : -1];
typedef char SProjectedVertex_Y_offset_must_be_2[
    ((unsigned long)&(((SProjectedVertex *)0)->Y) == 2) ? 1 : -1];
typedef char SProjectedVertex_z_offset_must_be_4[
    ((unsigned long)&(((SProjectedVertex *)0)->z) == 4) ? 1 : -1];
typedef char SProjectedVertex_Outcode_offset_must_be_6[
    ((unsigned long)&(((SProjectedVertex *)0)->Outcode) == 6) ? 1 : -1];
typedef char SProjectedVertex_size_must_be_8[
    (sizeof(SProjectedVertex) == 8) ? 1 : -1];

typedef char SRotatedVertex_x_offset_must_be_0[
    ((unsigned long)&(((SRotatedVertex *)0)->x) == 0) ? 1 : -1];
typedef char SRotatedVertex_y_offset_must_be_2[
    ((unsigned long)&(((SRotatedVertex *)0)->y) == 2) ? 1 : -1];
typedef char SRotatedVertex_Special_offset_must_be_4[
    ((unsigned long)&(((SRotatedVertex *)0)->Special) == 4) ? 1 : -1];
typedef char SRotatedVertex_size_must_be_8[
    (sizeof(SRotatedVertex) == 8) ? 1 : -1];

typedef char STexWibVertInfo_u_offset_must_be_0[
    ((unsigned long)&(((STexWibVertInfo *)0)->u) == 0) ? 1 : -1];
typedef char STexWibVertInfo_v_offset_must_be_1[
    ((unsigned long)&(((STexWibVertInfo *)0)->v) == 1) ? 1 : -1];
typedef char STexWibVertInfo_uAmpPhase_offset_must_be_2[
    ((unsigned long)&(((STexWibVertInfo *)0)->uAmpPhase) == 2) ? 1 : -1];
typedef char STexWibVertInfo_vAmpPhase_offset_must_be_3[
    ((unsigned long)&(((STexWibVertInfo *)0)->vAmpPhase) == 3) ? 1 : -1];
typedef char STexWibVertInfo_size_must_be_4[
    (sizeof(STexWibVertInfo) == 4) ? 1 : -1];

typedef char STexWibItemInfo_ItemOffset_offset_must_be_0[
    ((unsigned long)&(((STexWibItemInfo *)0)->ItemOffset) == 0) ? 1 : -1];
typedef char STexWibItemInfo_uVel_offset_must_be_4[
    ((unsigned long)&(((STexWibItemInfo *)0)->uVel) == 4) ? 1 : -1];
typedef char STexWibItemInfo_vVel_offset_must_be_6[
    ((unsigned long)&(((STexWibItemInfo *)0)->vVel) == 6) ? 1 : -1];
typedef char STexWibItemInfo_Frequency_offset_must_be_8[
    ((unsigned long)&(((STexWibItemInfo *)0)->Frequency) == 8) ? 1 : -1];
typedef char STexWibItemInfo_NumFaces_offset_must_be_12[
    ((unsigned long)&(((STexWibItemInfo *)0)->NumFaces) == 12) ? 1 : -1];
typedef char STexWibItemInfo_ZeroUAmplitudes_offset_must_be_14[
    ((unsigned long)&(((STexWibItemInfo *)0)->ZeroUAmplitudes) == 14) ? 1 : -1];
typedef char STexWibItemInfo_ZeroVAmplitudes_offset_must_be_15[
    ((unsigned long)&(((STexWibItemInfo *)0)->ZeroVAmplitudes) == 15) ? 1 : -1];
typedef char STexWibItemInfo_size_must_be_16[
    (sizeof(STexWibItemInfo) == 16) ? 1 : -1];

#endif // M3D_VERTEX_WIBBLE_LAYOUTS && !M3D_VERTEX_WIBBLE_LAYOUTS_EMITTED
