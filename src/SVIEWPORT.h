#ifndef SVIEWPORT_INC_H
#define SVIEWPORT_INC_H

/** Render viewport bounds and projection parameters. */
struct SViewport {
    unsigned short xR;
    unsigned short yB;
    unsigned short xL;
    unsigned short yT;
    unsigned short Hither;
    unsigned short Yon;
    unsigned short TanHalf;
    unsigned short Zoom;
    unsigned short xC;
    unsigned short yC;
};

typedef char SViewport_size_must_be_20[
    (sizeof(struct SViewport) == 20) ? 1 : -1];
typedef char SViewport_xR_offset_must_be_0[
    ((unsigned long)&(((struct SViewport *)0)->xR) == 0) ? 1 : -1];
typedef char SViewport_yB_offset_must_be_2[
    ((unsigned long)&(((struct SViewport *)0)->yB) == 2) ? 1 : -1];
typedef char SViewport_xL_offset_must_be_4[
    ((unsigned long)&(((struct SViewport *)0)->xL) == 4) ? 1 : -1];
typedef char SViewport_yT_offset_must_be_6[
    ((unsigned long)&(((struct SViewport *)0)->yT) == 6) ? 1 : -1];
typedef char SViewport_Hither_offset_must_be_8[
    ((unsigned long)&(((struct SViewport *)0)->Hither) == 8) ? 1 : -1];
typedef char SViewport_Yon_offset_must_be_10[
    ((unsigned long)&(((struct SViewport *)0)->Yon) == 10) ? 1 : -1];
typedef char SViewport_TanHalf_offset_must_be_12[
    ((unsigned long)&(((struct SViewport *)0)->TanHalf) == 12) ? 1 : -1];
typedef char SViewport_Zoom_offset_must_be_14[
    ((unsigned long)&(((struct SViewport *)0)->Zoom) == 14) ? 1 : -1];
typedef char SViewport_xC_offset_must_be_16[
    ((unsigned long)&(((struct SViewport *)0)->xC) == 16) ? 1 : -1];
typedef char SViewport_yC_offset_must_be_18[
    ((unsigned long)&(((struct SViewport *)0)->yC) == 18) ? 1 : -1];

#ifdef SVIEWPORT_OWNER_DECLARATIONS
#define SVIEWPORT_STATE_DECL_ATTR
#else
#define SVIEWPORT_STATE_DECL_ATTR __attribute__((section(".data")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** Viewport selected for the active render pass. */
extern SViewport *pCurrentViewport SVIEWPORT_STATE_DECL_ATTR;

#ifdef __cplusplus
}
#endif

#undef SVIEWPORT_STATE_DECL_ATTR

#endif // SVIEWPORT_INC_H
