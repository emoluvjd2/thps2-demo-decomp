#ifndef THPS2_M3DASM_H
#define THPS2_M3DASM_H

#include <STDDEF.H>
#include <LIBGTE.H>
#include "globals.h"

/**
 * Shared C++ interface to the hand-written M3dAsm owner.
 *
 * The natural signatures below emit the linker names recorded by MAIN.SYM;
 * the separately grouped entry points have flat linkage.
 */
struct SClipLineBuf;
struct SLineInfo;
struct SModel;
struct SProjectedVertex;

#include "M3DASM_FLAT.h"

/** Clip a line through the M3D scratch workspace. */
int M3dAsm_ClipLine(SClipLineBuf *buffer);

/** Load the current GTE background colour. */
void M3dAsm_SetAmbient(int *backColor);

/** Light a run of model normals with the current GTE state. */
void M3dAsm_GetDynamicLighting(SVECTOR *normals, int normalCount);

/** Build reflection-map coordinates for a model. */
void M3dAsm_GetReflectionMapping(
    MATRIX *matrix, long *position, unsigned int *model);

/** Prepare one item for a line-collision query. */
void M3dAsm_LineColijPreprocessItems(
    Item *item, int dummy, SLineInfo *info, unsigned short inquiry);

/** Prepare a zoned item list for a line-collision query. */
void M3dAsm_LineColijPreprocessItemsZoned(
    Item **items, int dummy, SLineInfo *info, unsigned short inquiry);

/** Transform and outcode a model's line-collision vertices. */
unsigned short M3dAsm_LineColijProcessVertices(
    SModel *model, SProjectedVertex *vertices,
    int length, VECTOR *relativeStart);

/** Test a model's faces against the prepared collision line. */
void M3dAsm_LineColijProcessFaces(
    SModel *model, SProjectedVertex *vertices,
    SLineInfo *info, Item *item);

/** Load one packed animation transform into the GTE. */
void M3dAsm_NewSetRotTransMatrix(SMatrix *matrix);

/** Process a model's projected face packets. */
void M3dAsm_ProcessPolys(
    unsigned int *faces, SVECTOR *normals, int faceCount);

/** Process the fast-path projected face packets. */
void M3dAsm_ProcessPolysFast(
    unsigned int *faces, SVECTOR *normals, int faceCount);

/** Load an articulated-model transform into the GTE. */
void M3dAsm_SetSuperTransforms(
    MATRIX *superTransform, MATRIX *superLightTransform,
    SMatrix *animTransform);

/** Load a dewibbled articulated-model transform into the GTE. */
void M3dAsm_SetSuperTransformsDewibbled(
    MATRIX *superTransform, MATRIX *superLightTransform,
    SMatrix *animTransform);

/** Load a fixed-point scene translation into the GTE. */
void M3dAsm_SetTransVector(Vector *translation);

/** Transform and outcode background vertices. */
unsigned int M3dAsm_TransformAndOutcodeBackgroundVertices(
    SVECTOR *vertices, int count);

/** Transform and outcode a translated block of vertices. */
unsigned int M3dAsm_TransformAndOutcodeBlockVertices(
    SVECTOR *vertices, int count, VECTOR *translation);

/** Transform and outcode an item's vertices. */
unsigned int M3dAsm_TransformAndOutcodeItemVertices(
    SVECTOR *vertices, int count);

/** Transform and outcode articulated-model vertices. */
unsigned int M3dAsm_TransformAndOutcodeSuperVertices(
    SVECTOR *vertices, int count);

/** Transform and outcode dewibbled articulated-model vertices. */
unsigned int M3dAsm_TransformAndOutcodeSuperVerticesDewibbled(
    SVECTOR *vertices, int count);

/** Transform and outcode articulated-model vertices through the fast path. */
unsigned int M3dAsm_TransformAndOutcodeSuperVerticesFast(
    SVECTOR *vertices, int count);

/** Transform and outcode dewibbled vertices through the fast path. */
unsigned int M3dAsm_TransformAndOutcodeSuperVerticesFastDewibbled(
    SVECTOR *vertices, int count);

/** Transform and outcode translated vertices through the fast path. */
unsigned int M3dAsm_TransformAndOutcodeVerticesFast(
    SVECTOR *vertices, int count, VECTOR *translation);

/** Transform a packed vector through animation and model matrices. */
void M3dAsm_TransformTwice(
    SVector *vector, SMatrix *first, MATRIX *second);

#endif // THPS2_M3DASM_H
