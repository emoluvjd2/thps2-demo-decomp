#ifndef SCAMERA_INC_H
#define SCAMERA_INC_H

#ifndef _LIBGTE_H_
#error "SCAMERA.h requires <LIBGTE.H> first"
#endif

/**
 * Complete render-camera state shared by the camera and renderer owners.
 *
 * The layout and 164-byte extent are recovered directly from MAIN.SYM.
 */
struct SCamera
{
    unsigned int Style;
    VECTOR Position;
    VECTOR Focus;
    int Distance;
    int Height;
    SVECTOR Angles;
    MATRIX Transform;
    MATRIX Inverse;
    MATRIX View;
    VECTOR Up;
};

typedef char SCamera_Style_offset_must_be_0[
    ((unsigned long)&(((SCamera *)0)->Style) == 0) ? 1 : -1];
typedef char SCamera_Position_offset_must_be_4[
    ((unsigned long)&(((SCamera *)0)->Position) == 4) ? 1 : -1];
typedef char SCamera_Focus_offset_must_be_20[
    ((unsigned long)&(((SCamera *)0)->Focus) == 20) ? 1 : -1];
typedef char SCamera_Distance_offset_must_be_36[
    ((unsigned long)&(((SCamera *)0)->Distance) == 36) ? 1 : -1];
typedef char SCamera_Height_offset_must_be_40[
    ((unsigned long)&(((SCamera *)0)->Height) == 40) ? 1 : -1];
typedef char SCamera_Angles_offset_must_be_44[
    ((unsigned long)&(((SCamera *)0)->Angles) == 44) ? 1 : -1];
typedef char SCamera_Transform_offset_must_be_52[
    ((unsigned long)&(((SCamera *)0)->Transform) == 52) ? 1 : -1];
typedef char SCamera_Inverse_offset_must_be_84[
    ((unsigned long)&(((SCamera *)0)->Inverse) == 84) ? 1 : -1];
typedef char SCamera_View_offset_must_be_116[
    ((unsigned long)&(((SCamera *)0)->View) == 116) ? 1 : -1];
typedef char SCamera_Up_offset_must_be_148[
    ((unsigned long)&(((SCamera *)0)->Up) == 148) ? 1 : -1];
typedef char SCamera_size_must_be_164[
    (sizeof(SCamera) == 164) ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif

/** Two render-camera slots owned by CAMERA.cpp. */
extern SCamera MikeCamera[2] __attribute__((section(".data")));

#ifdef __cplusplus
}
#endif

#endif // SCAMERA_INC_H
