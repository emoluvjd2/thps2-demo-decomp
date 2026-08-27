// DUST.cpp -- Reconstructed translation unit
// Original: DUST.CPP
// Functions: 11 reconstructed entries
// Address range: 0x8006B6C0 - 0x8006BFBC
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.


// === Forward type declarations ===
// Empty struct bodies allow by-value/by-reference parameters in C++

#define BIT_DUST_MANAGER_LAYOUT
#define BIT_DUST_CONSTRUCTOR_DECLARATIONS
#include "BIT.h"
#undef BIT_DUST_CONSTRUCTOR_DECLARATIONS
#undef BIT_DUST_MANAGER_LAYOUT
#include "M3DUTILS.h"
#include "PSXREGION.h"
#include "UTILS.h"

/** Return caller-provided storage for an in-place construction. */
inline void *operator new(unsigned int, void *storage)
{
    return storage;
}

/** Owner-local named skater view used by Dust::Update. */
struct CBruce : DustBruceLayout {};

/** Absolute-address view used by the particle animation call site. */
extern "C" char Dust_AnimationNameAbsolute[] asm("DAT_800dab10");

extern "C" {

/** Particle animation selected for dust puffs. */
char Dust_AnimationName[8] asm("DAT_800dab10")
    __attribute__((section(".sdata"))) = "smoke";

}

// GCC 2.x emits a private copy of `CClass::~CClass` beside a private base
// vtable in every owner that derives from CClass.  `MAIN.SYM` records 25 such
// `_._6CClass` copies, so the copy is file-local here.  It sits immediately
// after the animation name, which is why that array is eight bytes and not the
// sixteen its declaration used to claim.
static void Dust_ClassDestructor(CClass *, int)
    asm("__DOT__6CClass_8006BF8C");

// C++ forbids taking a destructor's address, so the public vtable slot uses
// an addressable declaration of the symbol this owner already defines.
extern "C" void Dust_DestructorAbi(Dust *, int) asm("_._4Dust");

// Consecutive eight-byte-aligned small-data words per vtable rather than
// arrays, so -G8 still yields the target's `addiu $vX, $gp, off`.
// MAIN.SYM names Dust's public vtable, while the private base table is
// owner-local.
extern "C" int __vt_4Dust
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int Dust_VTableIndex = 0;
static int Dust_VTableDtor = (int)&Dust_DestructorAbi;

static int Dust_BaseVTableAnchor asm("__vt_4Dust_base")
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int Dust_BaseVTableIndex = 0;
static int Dust_BaseVTableDtor = (int)&Dust_ClassDestructor;


// === Function implementations (address order) ===

/**
 * Initialize an empty dust-particle manager.
 *
 * Address:   0x8006B6C0
 * Mangled:   __4Dust
 * Demangled: Dust::(void)
 *
 */
Dust::Dust() {
    Dust *self = this;
    self->CClass.vtable = &__vt_4Dust;
    int i = 0;
    CFlatBit **ptr = self->mDustStack;
    do {
        *ptr = 0;
        i++;
        ptr++;
    } while (i < 12);
    self->mDustCount = 0;
    self->mCloudsLeftToGenerate = 0;
}


/**
 * Dust::~Dust()
 *
 * Address:   0x8006B6F4
 * Mangled:   _._4Dust
 * Demangled: Dust::~Dust()
 *            Destroy any active dust particles, reset the Dust object, and optionally free it.
 *
 */
Dust::~Dust() {
    int i = 0;
    Dust *self = this;
    CFlatBit **ptr;
    self->CClass.vtable = &__vt_4Dust;
    int count = self->mDustCount;
    if (count > 0) {
        ptr = self->mDustStack;
        do {
            CFlatBit *object = *ptr;
            if (object != 0) {
                object->CFT4Bit.CBit._vf->destroy(
                    &object->CFT4Bit.CBit, 3);
            }
            *ptr++ = 0;
            i++;
        } while (i < self->mDustCount);
    }
    self->mDustCount = 0;
    self->CClass.vtable = &Dust_BaseVTableAnchor;
}


/**
 * Reset the pending cloud count and select a dust-generation mode.
 *
 * Address:   0x8006B7A4
 * Mangled:   Reset__4Dustii
 * Demangled: Dust::Reset(int, int)
 *
 * @param terrainType  Terrain type code.
 * @param dustType  Land, bail, or brake dust mode.
 */
void Dust::Reset(int terrainType, int dustType) {
    Dust *self = this;
    unsigned char tint = 64;
    short transDecay = 1;
    short scaleDecay = 8;
    int randomOffset = 16;
    self->mDustType = dustType;
    self->mCloudsLeftToGenerate = 0;
    short scale = 0;
    self->get_terrain_parameters(
        terrainType, &scale, &tint, &transDecay, &scaleDecay,
        &self->mCloudsLeftToGenerate, &randomOffset);
}


/**
 * Advance live particles and emit the next terrain-dependent dust cloud.
 *
 * Address:   0x8006B814
 * Mangled:   Update__4DustP6CBruce
 * Demangled: Dust::Update(CBruce *)
 *
 * @param pBruce  Active skater (CBruce) being processed.
 */
void Dust::Update(CBruce *pBruce) {
    Dust *dust = this;
    int i = 0;
    if (dust->mDustCount > 0) {
        do {
            CFlatBit *pDust = dust->mDustStack[i];
            short scale = pDust->CFT4Bit.mScale;
            unsigned short unsignedScale =
                *(unsigned short *)&pDust->CFT4Bit.mScale;
            if (scale < 512) {
                *(unsigned short *)&pDust->CFT4Bit.mScale =
                    unsignedScale + *(unsigned short *)&
                        pDust->CFT4Bit.mScaleDecay;
            }
            dust->mDustStack[i]->CFT4Bit.Fade(0);
            int shift = (dust->mDustType != 1);
            dust->mDustStack[i]->CFT4Bit.CBit.mPos.vx =
                dust->mDustStack[i]->CFT4Bit.CBit.mPos.vx +
                (pBruce->mVelocity.vx >> shift);
            dust->mDustStack[i]->CFT4Bit.CBit.mPos.vz =
                dust->mDustStack[i]->CFT4Bit.CBit.mPos.vz +
                (pBruce->mVelocity.vz >> shift);
            int r = Rnd(3);
            dust->mDustStack[i]->CFT4Bit.CBit.mPos.vy =
                dust->mDustStack[i]->CFT4Bit.CBit.mPos.vy - (r << 12);
            i++;
        } while (i < dust->mDustCount);
    }
    // Set up locals for new particle spawn
    short scale = 0;
    unsigned char tint = 64;
    short transDecay = 1;
    short scaleDecay = 8;
    int randomOffset = 16;
    int dummy;
    if (dust->mCloudsLeftToGenerate <= 0) {
        // Timer expired: check if all particles are faded
        int done = 1;
        int i = 0;
        if (dust->mDustCount > 0) {
            do {
                if (dust->mDustStack[i]->CFT4Bit.mColour.mCodeBGR != 0) {
                    done = 0;
                }
            } while (++i < dust->mDustCount);
        }
        if (done == 0) return;
        // Destroy all particles
        if (dust->mDustCount > 0) {
            int i = 0;
            CFlatBit **slot = dust->mDustStack;
            do {
                CFlatBit *object = *slot;
                if (object != 0) {
                    object->CFT4Bit.CBit._vf->destroy(
                        &object->CFT4Bit.CBit, 3);
                }
                *slot = 0;
                slot++;
            } while (++i < dust->mDustCount);
        }
        dust->mDustCount = 0;
    } else {
        // Spawn new particle
        dust->mCloudsLeftToGenerate--;
        dust->get_terrain_parameters(
            pBruce->mTerrain, &scale, &tint, &transDecay,
            &scaleDecay, &dummy, &randomOffset);
        CFlatBit *pDust = static_cast<CFlatBit *>(
            Bit_AllocateStorage(sizeof(CFlatBit)));
        if (pDust != 0) {
            pDust = ::new (pDust) CFlatBit();
        }
        // Copy bruce position to new bit position (batched CVector assignment)
        pDust->CFT4Bit.CBit.mPos = pBruce->mPosition;
        CVector *pResult = &pDust->CFT4Bit.CBit.mPos;
        if (dust->mDustType == 1) {
            // Bail mode
            CVector tempVector;
            tempVector.vx = 0;
            tempVector.vy = 0;
            tempVector.vz = 0;
            dust->apply_random_offset(tempVector, 32, 0);
            // Look up hook from PSXRegion table
            int playerIdx = pBruce->mRegion;
            unsigned short numParts = PSXRegion[playerIdx].NumParts;
            SHook hook;
            hook.Offset.vx = (short)tempVector.vx;
            hook.Offset.vz = (short)tempVector.vz;
            int r = Rnd(numParts);
            hook.Part = (unsigned short)r;
            M3dUtils_GetDynamicHookPosition(
                reinterpret_cast<VECTOR *>(pResult),
                reinterpret_cast<CSuper *>(pBruce),
                &hook);
            // Double the particle size for bail
            int doubledScale = scale;
            int doubledScaleDecay = scaleDecay;
            scale = doubledScale + doubledScale;
            scaleDecay = doubledScaleDecay + doubledScaleDecay;
        } else {
            // Normal mode
            dust->apply_random_offset(*pResult, randomOffset, 12);
        }
        pDust->CFT4Bit.SetAnim(Dust_AnimationNameAbsolute);
        pDust->CFT4Bit.SetSemiTransparent();
        pDust->CFT4Bit.ChooseRandomFrame();
        unsigned short finalScale = (unsigned short)scale;
        pDust->CFT4Bit.mAnimSpeed = 85;
        pDust->CFT4Bit.mScale = finalScale;
        unsigned int dustTint = (unsigned int)tint;
        pDust->CFT4Bit.SetTint(dustTint, dustTint, dustTint);
        unsigned short finalTransDecay = (unsigned short)transDecay;
        unsigned short finalScaleDecay = (unsigned short)scaleDecay;
        pDust->CFT4Bit.mTransDecay = finalTransDecay;
        pDust->CFT4Bit.mScaleDecay = finalScaleDecay;
        dust->insert_dust(pDust);
    }
}


/**
 * Insert a particle, replacing the dimmest live particle when full.
 *
 * Address:   0x8006BB60
 * Mangled:   insert_dust__4DustP8CFlatBit
 * Demangled: Dust::insert_dust(CFlatBit *)
 *
 * @param pDust  Dust to operate on.
 */
void Dust::insert_dust(CFlatBit *pDust) {
    Dust *dust = this;
    // Clean locals reschedule the full replacement path (40 real and two
    // relocation mismatches), so retain the target's count/index registers.
    register int count asm("$3") = dust->mDustCount;
    if (count < 12) {
        register int idx asm("$2");
        idx = count * 4;
        DUST_STACK_SLOT_AT_BYTE_OFFSET(dust, idx) = pDust;
        dust->mDustCount = dust->mDustCount + 1;
        return;
    }

    int unusedSlot = -1;
    int dimmestColor = 65535;
    int i = 0;
    CFlatBit **base = dust->mDustStack;
    if (count > 0) {
        int countLoop = count;
        CFlatBit **slot = base;
        do {
            struct CFlatBit *oldDust = *slot;
            int newColor = (int)oldDust->CFT4Bit.mColour.Components.mRed
                         + (int)oldDust->CFT4Bit.mColour.Components.mGreen
                         + (int)oldDust->CFT4Bit.mColour.Components.mBlue;
            if (newColor < dimmestColor) {
                dimmestColor = newColor;
                unusedSlot = i;
            }
            i++;
            slot++;
        } while (i < countLoop);
    }

    int offset = unusedSlot * 4;
    CFlatBit **target =
        (CFlatBit **)((char *)base + offset);
    struct CFlatBit *oldDust = *target;
    if (oldDust != 0) {
        oldDust->CFT4Bit.CBit._vf->destroy(&oldDust->CFT4Bit.CBit, 3);
    }
    *target = pDust;
}


/**
 * Dispatch terrain parameters to the active dust mode.
 *
 * Address:   0x8006BC48
 * Mangled:   get_terrain_parameters__4DustiPsPUcT2T2PiPi
 * Demangled: Dust::get_terrain_parameters(int, short *, unsigned char *, short *, short *, int *, int *)
 *            Dispatches by the manager's named dust-mode field.
 *
 * @param terrainType  Terrain type code.
 * @param pScale  Scale to operate on.
 * @param pTint  Tint to operate on.
 * @param pTransDecay  TransDecay to operate on.
 * @param pScaleDecay  ScaleDecay to operate on.
 * @param pNumClouds  NumClouds to operate on.
 * @param pRandomOffset  RandomOffset to operate on.
 */
void Dust::get_terrain_parameters(
    int terrainType, short *pScale, unsigned char *pTint,
    short *pTransDecay, short *pScaleDecay, DustCloudCount *pNumClouds,
    int *pRandomOffset) {
    int mode = mDustType;
    switch (mode) {
    case 0:
        get_land_terrain_parameters(
            terrainType, pScale, pTint, pTransDecay, pScaleDecay,
            pNumClouds, pRandomOffset);
        break;
    case 1:
        get_bail_terrain_parameters(
            terrainType, pScale, pTint, pTransDecay, pScaleDecay,
            pNumClouds, pRandomOffset);
        break;
    case 2:
        get_brake_terrain_parameters(
            terrainType, pScale, pTint, pTransDecay, pScaleDecay,
            pNumClouds, pRandomOffset);
        break;
    }
}


/**
 * Select particle parameters for ordinary landing dust.
 *
 * Address:   0x8006BCF8
 * Mangled:   get_land_terrain_parameters__4DustiPsPUcT2T2PiPi
 * Demangled: Dust::get_land_terrain_parameters(int, short*, unsigned char*, short*, short*, int*, int*)
 *            Leaf function: switch on terrain type, sets 6 output parameters
 *
 * @param terrainType  Terrain type code.
 * @param pScale  Scale to operate on.
 * @param pTint  Tint to operate on.
 * @param pTransDecay  TransDecay to operate on.
 * @param pScaleDecay  ScaleDecay to operate on.
 * @param pNumClouds  NumClouds to operate on.
 * @param pRandomOffset  RandomOffset to operate on.
 */
void Dust::get_land_terrain_parameters(
    int terrainType, short *pScale, unsigned char *pTint,
    short *pTransDecay, short *pScaleDecay, DustCloudCount *pNumClouds,
    int *pRandomOffset)
{
    if (terrainType == 3) goto land_case3;
    if (terrainType != 6) goto land_default;
    // case 6
    *pScale = 1024;
    *pTint = 64;
    *pTransDecay = 8;
    *pScaleDecay = 64;
    *pNumClouds = 12;
    *pRandomOffset = 32;
    return;
land_case3:
    *pScale = 256;
    *pTint = 64;
    *pTransDecay = 1;
    *pScaleDecay = 8;
    *pNumClouds = 8;
    *pRandomOffset = 16;
    return;
land_default:
    *pScale = 0;
    *pTint = 0;
    *pTransDecay = 0;
    *pScaleDecay = 0;
    *pNumClouds = 0;
    *pRandomOffset = 0;
}


/**
 * Select particle parameters for bail dust.
 *
 * Address:   0x8006BD94
 * Mangled:   get_bail_terrain_parameters__4DustiPsPUcT2T2PiPi
 * Demangled: Dust::get_bail_terrain_parameters(int, short *, unsigned char *, short *, short *, int *, int *)
 *            Switch on terrain type, sets 6 output parameters for bail dust
 *
 * @param terrainType  Terrain type code.
 * @param pScale  Scale to operate on.
 * @param pTint  Tint to operate on.
 * @param pTransDecay  TransDecay to operate on.
 * @param pScaleDecay  ScaleDecay to operate on.
 * @param pNumClouds  NumClouds to operate on.
 * @param pRandomOffset  RandomOffset to operate on.
 */
void Dust::get_bail_terrain_parameters(
    int terrainType, short *pScale, unsigned char *pTint,
    short *pTransDecay, short *pScaleDecay, DustCloudCount *pNumClouds,
    int *pRandomOffset) {
    switch (terrainType) {
    case 3:
        *pScale = 128;
        *pTint = 64;
        *pTransDecay = 2;
        *pScaleDecay = 8;
        *pNumClouds = 10;
        *pRandomOffset = 4;
        break;
    case 2:
    case 4:
    case 5:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        *pScale = 64;
        *pTint = 64;
        *pTransDecay = 2;
        *pScaleDecay = 8;
        *pNumClouds = 10;
        *pRandomOffset = 0;
        break;
    default:
        *pScale = 0;
        *pTint = 0;
        *pTransDecay = 0;
        *pScaleDecay = 0;
        *pNumClouds = 0;
        *pRandomOffset = 0;
        break;
    }
}


/**
 * Select particle parameters for braking dust.
 *
 * Address:   0x8006BE4C
 * Mangled:   get_brake_terrain_parameters__4DustiPsPUcT2T2PiPi
 * Demangled: Dust::get_brake_terrain_parameters(int, short *, unsigned char *, short *, short *, int *, int *)
 *            If-else chain on terrain type, sets 6 output parameters for brake dust
 *
 * @param terrainType  Terrain type code.
 * @param pScale  Scale to operate on.
 * @param pTint  Tint to operate on.
 * @param pTransDecay  TransDecay to operate on.
 * @param pScaleDecay  ScaleDecay to operate on.
 * @param pNumClouds  NumClouds to operate on.
 * @param pRandomOffset  RandomOffset to operate on.
 */
void Dust::get_brake_terrain_parameters(
    int terrainType, short *pScale, unsigned char *pTint,
    short *pTransDecay, short *pScaleDecay, DustCloudCount *pNumClouds,
    int *pRandomOffset) {
    if (terrainType != 3) goto not3;
    goto case3;
not3:
    if (terrainType < 4) {
        if (terrainType == 2) goto shared;
        goto def;
    }
    if (terrainType == 8) goto shared;
    if (terrainType != 13) goto def;
shared:
    *pScale = 64;
    *pTint = 64;
    *pTransDecay = 2;
    *pScaleDecay = 8;
    *pNumClouds = 6;
    *pRandomOffset = 6;
    return;
case3:
    *pScale = 64;
    *pTint = 64;
    *pTransDecay = 2;
    *pScaleDecay = 8;
    *pNumClouds = 10;
    *pRandomOffset = 12;
    return;
def:
    *pScale = 0;
    *pTint = 0;
    *pTransDecay = 0;
    *pScaleDecay = 0;
    *pNumClouds = 0;
    *pRandomOffset = 0;
}


/**
 * Apply independent random horizontal offsets to a particle position.
 *
 * Address:   0x8006BF0C
 * Mangled:   apply_random_offset__4DustR7CVectorii
 * Demangled: Dust::apply_random_offset(CVector &, int, int)
 *            Applies random offset to vec.x and vec.z, range scaled by 4, shifted by shift
 *
 * @param pos  Position vector.
 * @param randomRange  Exclusive random range before centering.
 * @param shiftAmount  Fixed-point shift applied to each offset.
 */
// Shared Dust member function
void Dust::apply_random_offset(
    CVector &position, int randomRange, int shiftAmount) {
    CVector *pos = &position;
    int range4 = randomRange << 2;
    int r = Rnd(range4);
    int sh = shiftAmount;
    int neg_half = -(range4 >> 1);
    int offset = neg_half + r;

    int val = pos->vx;
    int off = offset << sh;
    pos->vx = val + off;
    r = Rnd(range4);
    offset = neg_half + r;
    sh = offset << sh;
    pos->vz = pos->vz + sh;
}

/**
 * Destroy the owner-local CClass base and optionally release its storage.
 *
 * GCC emitted this private copy last in the owner's text, so it remains at
 * the end of the translation unit.
 */
static void Dust_ClassDestructor(CClass *self, int flags) {
    self->vtable = &Dust_BaseVTableAnchor;
    if (flags & 1) {
        CClass::operator delete(self);
    }
}
