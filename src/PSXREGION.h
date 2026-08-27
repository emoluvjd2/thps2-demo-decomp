#ifndef THPS2_PSXREGION_H
#define THPS2_PSXREGION_H

struct SModel;
struct SHook;

// SPSXRegion — PSX region slot (64 bytes)
struct SPSXRegion {
    char Filename[9];
    unsigned char IsSuper;
    unsigned char Usable;
    unsigned char Protected;
    unsigned int *pModelChecksums;
    struct SModel **ppModels;
    unsigned int *pPSX;
    unsigned int *pAnimFile;
    unsigned short *pHierarchy;
    struct SHook *pHooks;
    unsigned int *pColourTable;
    unsigned int *pTexWibData;
    unsigned int *pColourPulseData;
    short *mpAnimTranslation;
    unsigned short NumParts;
    unsigned char Fastable;
    unsigned char TexturesInRAM;
    unsigned char KeepTexturesInRAM;
    unsigned char WhichHeap;
    char Pad[2];
    int Size;
};

typedef char SPSXRegion_size_must_be_64[(sizeof(SPSXRegion) == 64) ? 1 : -1];
typedef char SPSXRegion_Filename_offset_must_be_0[((unsigned long)&(((SPSXRegion *)0)->Filename) == 0) ? 1 : -1];
typedef char SPSXRegion_IsSuper_offset_must_be_9[((unsigned long)&(((SPSXRegion *)0)->IsSuper) == 9) ? 1 : -1];
typedef char SPSXRegion_Usable_offset_must_be_10[((unsigned long)&(((SPSXRegion *)0)->Usable) == 10) ? 1 : -1];
typedef char SPSXRegion_Protected_offset_must_be_11[((unsigned long)&(((SPSXRegion *)0)->Protected) == 11) ? 1 : -1];
typedef char SPSXRegion_pModelChecksums_offset_must_be_12[((unsigned long)&(((SPSXRegion *)0)->pModelChecksums) == 12) ? 1 : -1];
typedef char SPSXRegion_ppModels_offset_must_be_16[((unsigned long)&(((SPSXRegion *)0)->ppModels) == 16) ? 1 : -1];
typedef char SPSXRegion_pPSX_offset_must_be_20[((unsigned long)&(((SPSXRegion *)0)->pPSX) == 20) ? 1 : -1];
typedef char SPSXRegion_pAnimFile_offset_must_be_24[((unsigned long)&(((SPSXRegion *)0)->pAnimFile) == 24) ? 1 : -1];
typedef char SPSXRegion_pHierarchy_offset_must_be_28[((unsigned long)&(((SPSXRegion *)0)->pHierarchy) == 28) ? 1 : -1];
typedef char SPSXRegion_pHooks_offset_must_be_32[((unsigned long)&(((SPSXRegion *)0)->pHooks) == 32) ? 1 : -1];
typedef char SPSXRegion_pColourTable_offset_must_be_36[((unsigned long)&(((SPSXRegion *)0)->pColourTable) == 36) ? 1 : -1];
typedef char SPSXRegion_pTexWibData_offset_must_be_40[((unsigned long)&(((SPSXRegion *)0)->pTexWibData) == 40) ? 1 : -1];
typedef char SPSXRegion_pColourPulseData_offset_must_be_44[((unsigned long)&(((SPSXRegion *)0)->pColourPulseData) == 44) ? 1 : -1];
typedef char SPSXRegion_mpAnimTranslation_offset_must_be_48[((unsigned long)&(((SPSXRegion *)0)->mpAnimTranslation) == 48) ? 1 : -1];
typedef char SPSXRegion_NumParts_offset_must_be_52[((unsigned long)&(((SPSXRegion *)0)->NumParts) == 52) ? 1 : -1];
typedef char SPSXRegion_Fastable_offset_must_be_54[((unsigned long)&(((SPSXRegion *)0)->Fastable) == 54) ? 1 : -1];
typedef char SPSXRegion_TexturesInRAM_offset_must_be_55[((unsigned long)&(((SPSXRegion *)0)->TexturesInRAM) == 55) ? 1 : -1];
typedef char SPSXRegion_KeepTexturesInRAM_offset_must_be_56[((unsigned long)&(((SPSXRegion *)0)->KeepTexturesInRAM) == 56) ? 1 : -1];
typedef char SPSXRegion_WhichHeap_offset_must_be_57[((unsigned long)&(((SPSXRegion *)0)->WhichHeap) == 57) ? 1 : -1];
typedef char SPSXRegion_Pad_offset_must_be_58[((unsigned long)&(((SPSXRegion *)0)->Pad) == 58) ? 1 : -1];
typedef char SPSXRegion_Size_offset_must_be_60[((unsigned long)&(((SPSXRegion *)0)->Size) == 60) ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif

/** Forty loadable PSX region slots owned by SPOOL.cpp. */
extern SPSXRegion PSXRegion[];

#ifdef __cplusplus
}
#endif

#endif
