// Declarations shared by the BIT particle/effect owner.
#ifndef BIT_INC_H
#define BIT_INC_H

struct SAnimFrame;
struct SDoubleBuffer;
struct SViewport;
struct Texture;
struct CBit;
struct CChunkBit;
struct CFT4Bit;
struct CFlatBit;
struct CGLine;
struct CGPolyLine;
struct CGenPoly;
struct CGlassBit;
struct CGlow;
struct CLinked2EndedBit;
struct CNonRenderedBit;
struct CPixel;
struct CPolyLine;
struct CQuadBit;
struct CSVector;
struct CSpecialDisplay;
struct CVector;
struct CWobblyGlow;

/** Target-proven storage sizes for opaque BIT-derived effects. */
enum {
    BIT_SPARK_STORAGE_SIZE = 92,
    BIT_GLASS_STORAGE_SIZE = 120,
};

/** Allocate zero-initialized storage through CBit's owning allocator. */
extern "C" void *Bit_AllocateStorage(unsigned int size)
    asm("__nw__4CBitUi");

/** Delete every active transient bit effect. */
void Bit_DeleteAll(void);

/** Construct a glass fragment in caller-provided CBit storage. */
extern "C" CGlassBit *CGlassBit_Construct(
    CGlassBit *glass, const CVector &Pos, const CVector &Vel, int GroundY,
    unsigned char r, unsigned char g, unsigned char b,
    int dx, int dy, int dz)
    asm("__9CGlassBitRC7CVectorT1iUcUcUciii");

/** Construct a wobbling glow in caller-provided CBit storage. */
extern "C" CWobblyGlow *CWobblyGlow_Construct(
    CWobblyGlow *glow, const CVector &position,
    int innerRadius, int fringeRadius, int amplitude,
    unsigned char r0, unsigned char g0, unsigned char b0,
    unsigned char r1, unsigned char g1, unsigned char b1)
    asm("__11CWobblyGlowRC7CVectoriiiUcUcUcUcUcUc");

#ifdef BIT_OWNER_DECLARATIONS
#define BITSTATE_OWNER_DECLARATIONS
#endif
#define BITSTATE_DECLARE_USE_CACHE_FOR_BITS
#include "BITSTATE.h"

/** Set the shared spark emitter's base trajectory angles. */
void Bit_SetSparkTrajectory(const CSVector &trajectory);

/** Set the random angular cone applied around the spark trajectory. */
void Bit_SetSparkTrajectoryCone(const CSVector &cone);

/** Convert a magnitude into a randomized velocity using the spark settings. */
void Bit_CalculateSparkVelocity(CVector &velocity, int magnitude);

/** Set the size assigned to subsequently constructed sparks. */
void Bit_SetSparkSize(unsigned int size);

/** Set the initial colour assigned to subsequently constructed sparks. */
void Bit_SetSparkRGB(
    unsigned char r, unsigned char g, unsigned char b);

/** Set the per-frame colour fade assigned to new sparks. */
void Bit_SetSparkFadeRGB(
    unsigned char r, unsigned char g, unsigned char b);

#ifdef BIT_OWNER_DECLARATIONS
#include "globals.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#endif

#ifdef BIT_OWNER_DECLARATIONS

// === Variables (gp-relative in this TU) ===
#define BITSTATE_DECLARE_BIT_COUNT
#include "BITSTATE.h"
extern "C" CChunkBit *ChunkBitList;                 // 0x800D9AD0
extern "C" CFlatBit *FlatBitList;                   // 0x800D9ABC
#define BITSTATE_DECLARE_G_LINE_LIST
#include "BITSTATE.h"
#define BITSTATE_DECLARE_G_POLY_LINE_LIST
#include "BITSTATE.h"
extern "C" CGenPoly *GenPolyList;                   // 0x800D9ACC
extern "C" CGlassBit *GlassList;                    // 0x800D9AD8
extern "C" CGlow *GlowList;                         // 0x800D9AD4
extern "C" CLinked2EndedBit *Linked2EndedBitList;   // 0x800D9AC0
#define BITSTATE_DECLARE_NON_RENDERED_BIT_LIST
#include "BITSTATE.h"
extern "C" CPixel *PixelList;                       // 0x800D9AC4
#define BITSTATE_DECLARE_POLY_LINE_LIST
#include "BITSTATE.h"
extern "C" CQuadBit *QuadBitList;                   // 0x800D9AC8
extern "C" CVECTOR SparkFadeRGB;         // 0x800D996C
extern "C" CVECTOR SparkRGB;             // 0x800D9968
#define BITSTATE_DECLARE_SPARK_SEMI_TRANS
#include "BITSTATE.h"
extern "C" unsigned int SparkSize;       // 0x800D9960
extern "C" CSVector SparkTrajectory;     // 0x800D9950
extern "C" CSVector SparkTrajectoryCone; // 0x800D9958
extern "C" CSpecialDisplay *SpecialDisplayList;     // 0x800D9ADC
extern "C" DR_TPAGE StoredTPagePrim;     // 0x800D9AEC
#define BITSTATE_DECLARE_TOTAL_BIT_USAGE
#include "BITSTATE.h"
#define BITSTATE_DECLARE_SHADOW_TEXTURE
#include "BITSTATE.h"

// === Variables (absolute in this TU) ===
extern "C" SAnimFrame *ButtonsAnim __attribute__((section(".data"))); // 0x800D8CA8
#include "CAMERASELECTION.h"
#include "PDOUBLEBUFFER.h"

#endif // BIT_OWNER_DECLARATIONS

#ifdef BIT_OWNER_DECLARATIONS
#undef BITSTATE_OWNER_DECLARATIONS
#endif

#endif // BIT_INC_H

// BLOOD and DUST are particle managers whose target-proven layouts are needed
// only by their owners and by MECH's late destructor block.  Keep these slices
// opt-in and outside BIT_INC_H so a consumer that already used the opaque BIT
// interface can request destructor-complete manager types at that later point.
#if defined(BIT_BLOOD_MANAGER_LAYOUT) && \
    !defined(BIT_BLOOD_MANAGER_LAYOUT_DEFINED)
#define BIT_BLOOD_MANAGER_LAYOUT_DEFINED

#include "globals.h"

struct BloodBit;
struct CBruce;

/** Virtual prefix used when retiring a blood particle. */
struct BloodBitVtable {
    void (*reserved0)(void *object);
    void (*reserved1)(void *object);
    void (*destroy)(BloodBit *object, int flags);
};

/** Minimal named CBit layout needed for virtual destruction. */
struct BloodBit {
    unsigned char state[68];
    BloodBitVtable *vtable;
};

/** Blood particle stack and lifetime state. */
struct Blood {
    enum { Capacity = 20, InitialAge = 128 };

    struct CClass CClass;
    /** Initialize an empty blood-particle manager. */
    Blood();
    ~Blood();
    /** Destroy all live blood particles and reset the manager state. */
    void Reset();
    /** Update blood particles for the active skater. */
    void Update(CBruce *bruce);
    /** Release manager storage through the original CClass allocator. */
    static void operator delete(void *p) {
        ::CClass::operator delete(p);
    }

    struct BloodBit *mBloodStack[Capacity];
    int mBloodCount;
    int mAge;
};

typedef char blood_owner_size_must_be_92[
    (sizeof(Blood) == 92) ? 1 : -1];
typedef char blood_bit_size_must_be_72[
    (sizeof(BloodBit) == 72) ? 1 : -1];

#endif // BIT_BLOOD_MANAGER_LAYOUT

#if defined(BIT_DUST_MANAGER_LAYOUT) && \
    !defined(BIT_DUST_MANAGER_LAYOUT_DEFINED)
#define BIT_DUST_MANAGER_LAYOUT_DEFINED

#include "globals.h"

struct SAnimFrame;
struct CBruce;

struct CBit;

/** Virtual prefix shared by dust-owned bit subclasses. */
struct DustBitVtable {
    void (*reserved0)(void *object);
    void (*reserved1)(void *object);
    void (*destroy)(CBit *object, int flags);
};

/** Base particle state needed to dispatch particle destruction. */
struct CBit {
    struct CBit *mPrevious;
    struct CBit *mNext;
    short mAge;
    unsigned short mLifetime;
    struct CVector mTweenPos;
    struct CVector mPos;
    struct CVector mVel;
    struct CVector mAcc;
    struct CFriction mFric;
    unsigned char mDead;
    unsigned short mFrigDeltaZ;
    unsigned char mProtected;
    unsigned char mType;
    DustBitVtable *_vf;
};

/** Textured four-point particle state embedded in a flat dust particle. */
struct CFT4Bit {
    struct CBit CBit;
    short mTransDecay;
    short mScaleDecay;
    union {
        unsigned int mCodeBGR;
        struct {
            unsigned char mRed;
            unsigned char mGreen;
            unsigned char mBlue;
            unsigned char mCode;
        } Components;
    } mColour;
    struct SAnimFrame *mpPSXAnim;
    struct SAnimFrame *mpPSXFrame;
    unsigned char mBitFlags;
    unsigned char mNumFrames;
    char mFrame;
    unsigned char mFrameFrac;
    short mAnimSpeed;
    short mScale;

#ifdef __cplusplus
    /** Select an animation by name. */
    void SetAnim(char *name);
    /** Enable semi-transparent primitive rendering. */
    void SetSemiTransparent();
    /** Select a random frame from the current animation. */
    void ChooseRandomFrame();
    /** Assign the particle's RGB tint. */
    void SetTint(unsigned char r, unsigned char g, unsigned char b);
    /** Fade the particle tint and optionally retire the particle. */
    int Fade(int die);
#endif
};

/** Flat textured particle allocated and managed by Dust. */
struct CFlatBit {
    struct CFT4Bit CFT4Bit;
    short mAngle;
    short mAngVel;
    short mAngAcc;
    unsigned short mAngFric;
    unsigned int mPostScale;
    unsigned char mForcedPriority;
    unsigned char mSemiTransparencyRate;
    unsigned short mCLUT;

#if defined(__cplusplus) && defined(BIT_DUST_CONSTRUCTOR_DECLARATIONS)
    /** Construct and attach a flat textured particle. */
    CFlatBit();
#endif
};

/** Number of dust clouds selected by one terrain policy. */
typedef int DustCloudCount;

/** Dust particle stack and terrain-generation state. */
struct Dust {
    struct CClass CClass;
    struct CFlatBit *mDustStack[12];
    int mDustCount;
    int mDustType;
    int mCloudsLeftToGenerate;

    /** Initialize an empty dust-particle manager. */
    Dust();
    ~Dust();
    /** Release manager storage through the original CClass allocator. */
    static void operator delete(void *p) {
        ::CClass::operator delete(p);
    }

    /** Reset the pending cloud count and select a dust-generation mode. */
    void Reset(int terrainType, int dustType);
    /** Advance live particles and emit the next terrain-dependent cloud. */
    void Update(CBruce *bruce);
    /** Insert a particle, replacing the dimmest live particle when full. */
    void insert_dust(CFlatBit *dust);
    void get_terrain_parameters(int terrainType, short *scale,
        unsigned char *tint, short *transDecay, short *scaleDecay,
        DustCloudCount *numClouds, int *randomOffset);
    /** Select particle parameters for ordinary landing dust. */
    void get_land_terrain_parameters(int terrainType, short *scale,
        unsigned char *tint, short *transDecay, short *scaleDecay,
        DustCloudCount *numClouds, int *randomOffset);
    /** Select particle parameters for bail dust. */
    void get_bail_terrain_parameters(int terrainType, short *scale,
        unsigned char *tint, short *transDecay, short *scaleDecay,
        DustCloudCount *numClouds, int *randomOffset);
    /** Select particle parameters for braking dust. */
    void get_brake_terrain_parameters(int terrainType, short *scale,
        unsigned char *tint, short *transDecay, short *scaleDecay,
        DustCloudCount *numClouds, int *randomOffset);
    /** Apply independent random horizontal offsets to a particle position. */
    void apply_random_offset(CVector &position, int range, int shift);
};

/** Preserve the target's byte-scaled slot address without exposing it in code. */
#define DUST_STACK_SLOT_AT_BYTE_OFFSET(owner, byteOffset) \
    (*(CFlatBit **)((char *)(owner) + (byteOffset) + sizeof(CClass)))

/** CBruce fields read while carrying dust particles with the skater. */
struct DustBruceLayout {
    unsigned char pad_to_position[4];
    CVector mPosition;
    unsigned char pad_to_region[11];
    unsigned char mRegion;
    unsigned char pad_to_velocity[56];
    CVector mVelocity;
    unsigned char pad_to_terrain[4100];
    int mTerrain;
};

typedef char dust_CBit_size_must_be_72[
    (sizeof(CBit) == 72) ? 1 : -1];
typedef char dust_CFT4Bit_size_must_be_96[
    (sizeof(CFT4Bit) == 96) ? 1 : -1];
typedef char dust_CFlatBit_size_must_be_112[
    (sizeof(CFlatBit) == 112) ? 1 : -1];
typedef char dust_owner_size_must_be_64[
    (sizeof(Dust) == 64) ? 1 : -1];
typedef char dust_bruce_region_must_be_27[
    ((int)&((DustBruceLayout *)0)->mRegion == 27) ? 1 : -1];
typedef char dust_bruce_velocity_must_be_84[
    ((int)&((DustBruceLayout *)0)->mVelocity == 84) ? 1 : -1];
typedef char dust_bruce_terrain_must_be_4196[
    ((int)&((DustBruceLayout *)0)->mTerrain == 4196) ? 1 : -1];

#endif // BIT_DUST_MANAGER_LAYOUT

// Glow element bodies are needed only by the BIT owner.  Keep this phase
// re-entrant so ordinary pointer-only BIT consumers retain opaque types.
#if defined(BIT_GLOW_ELEMENT_LAYOUT) && \
    !defined(BIT_GLOW_ELEMENT_LAYOUT_DEFINED)
#define BIT_GLOW_ELEMENT_LAYOUT_DEFINED

/** One radial band in a glow effect. */
struct SSection {
    unsigned int Radius;
    unsigned int PadBGR;
};

/** One outer fringe band in a glow effect. */
struct SFringeQuad {
    unsigned int Width;
    unsigned int CodeBGR;
};

typedef char SSection_Radius_offset_must_be_0[
    ((unsigned long)&(((SSection *)0)->Radius) == 0) ? 1 : -1];
typedef char SSection_PadBGR_offset_must_be_4[
    ((unsigned long)&(((SSection *)0)->PadBGR) == 4) ? 1 : -1];
typedef char SSection_size_must_be_8[
    (sizeof(SSection) == 8) ? 1 : -1];
typedef char SFringeQuad_Width_offset_must_be_0[
    ((unsigned long)&(((SFringeQuad *)0)->Width) == 0) ? 1 : -1];
typedef char SFringeQuad_CodeBGR_offset_must_be_4[
    ((unsigned long)&(((SFringeQuad *)0)->CodeBGR) == 4) ? 1 : -1];
typedef char SFringeQuad_size_must_be_8[
    (sizeof(SFringeQuad) == 8) ? 1 : -1];

#endif // BIT_GLOW_ELEMENT_LAYOUT
