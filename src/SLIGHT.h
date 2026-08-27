// Recovered fixed-function lighting descriptor shared by data-owner TUs.
#ifndef SLIGHT_INC_H
#define SLIGHT_INC_H

/**
 * Direction, colour, and ambient terms consumed by the PSX GTE lighting path.
 */
struct SLight {
    short LightMatrix[3][3];
    unsigned short Pad1;
    short ColorMatrix[3][3];
    unsigned short Pad2;
    int BackColor[3];
};

typedef char SLight_LightMatrix_offset_must_be_0[
    ((unsigned long)&(((SLight *)0)->LightMatrix) == 0) ? 1 : -1];
typedef char SLight_Pad1_offset_must_be_18[
    ((unsigned long)&(((SLight *)0)->Pad1) == 18) ? 1 : -1];
typedef char SLight_ColorMatrix_offset_must_be_20[
    ((unsigned long)&(((SLight *)0)->ColorMatrix) == 20) ? 1 : -1];
typedef char SLight_Pad2_offset_must_be_38[
    ((unsigned long)&(((SLight *)0)->Pad2) == 38) ? 1 : -1];
typedef char SLight_BackColor_offset_must_be_40[
    ((unsigned long)&(((SLight *)0)->BackColor) == 40) ? 1 : -1];
typedef char SLight_size_must_be_52[(sizeof(SLight) == 52) ? 1 : -1];

#endif // SLIGHT_INC_H
