#ifndef THPS2_M3DASM_FLAT_H
#define THPS2_M3DASM_FLAT_H

struct Item;

/** Hand-written M3D assembly entry points with flat linker names. */
extern "C" {
void M3dAsm_BoundingSpherePreprocessing(Item *items, int cameraMask);
void M3dAsm_OutcodeVerticesInWater(int count, int clipValue);
void CompactOTagR(unsigned int *orderingTable, int size);
}

#endif // THPS2_M3DASM_FLAT_H
