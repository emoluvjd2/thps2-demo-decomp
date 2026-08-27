#ifndef THPS2_SMODEL_H
#define THPS2_SMODEL_H

#define GLOBALS_SCENE_VALUES_ONLY
#include "globals.h"
#undef GLOBALS_SCENE_VALUES_ONLY

#ifndef _LIBGTE_H_
#error "SMODEL.h requires the Psy-Q LIBGTE declarations first"
#endif

/** Runtime model record stored in each loaded PSX region. */
struct SModel {
    unsigned short Flags;
    unsigned short NumVertices;
    unsigned short NumNormals;
    unsigned short NumFaces;
    int Radius;
    Vector Box;
    short zMax;
    unsigned short NextLOD;
    SVECTOR Vertices[1];
};

typedef char SModel_size_must_be_36[(sizeof(SModel) == 36) ? 1 : -1];
typedef char SModel_Flags_offset_must_be_0[((unsigned long)&(((SModel *)0)->Flags) == 0) ? 1 : -1];
typedef char SModel_NumVertices_offset_must_be_2[((unsigned long)&(((SModel *)0)->NumVertices) == 2) ? 1 : -1];
typedef char SModel_NumNormals_offset_must_be_4[((unsigned long)&(((SModel *)0)->NumNormals) == 4) ? 1 : -1];
typedef char SModel_NumFaces_offset_must_be_6[((unsigned long)&(((SModel *)0)->NumFaces) == 6) ? 1 : -1];
typedef char SModel_Radius_offset_must_be_8[((unsigned long)&(((SModel *)0)->Radius) == 8) ? 1 : -1];
typedef char SModel_Box_offset_must_be_12[((unsigned long)&(((SModel *)0)->Box) == 12) ? 1 : -1];
typedef char SModel_zMax_offset_must_be_24[((unsigned long)&(((SModel *)0)->zMax) == 24) ? 1 : -1];
typedef char SModel_NextLOD_offset_must_be_26[((unsigned long)&(((SModel *)0)->NextLOD) == 26) ? 1 : -1];
typedef char SModel_Vertices_offset_must_be_28[((unsigned long)&(((SModel *)0)->Vertices) == 28) ? 1 : -1];

#endif
