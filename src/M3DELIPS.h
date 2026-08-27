#ifndef M3DELIPS_INC_H
#define M3DELIPS_INC_H

#include "globals.h"
#include <STDDEF.H>
#include <LIBGTE.H>

struct SModel;
struct Texture;

/** Projects a model's ellipsoid onto a receiving plane as a shadow quad. */
void M3dElips_RenderShadow(
    SModel *pModel, MATRIX *pTransform, CSVector *pNormal,
    CVector *pPoint, SVECTOR *pLightVec, CVECTOR rgb);

/** Renders a camera-facing quad around a transformed model ellipsoid. */
void M3dElips_RenderEllipsoid(
    SModel *pModel, MATRIX *pTransform, unsigned int Rgb, int Scale,
    int OTPushback, Texture *pTex);

#endif // M3DELIPS_INC_H
