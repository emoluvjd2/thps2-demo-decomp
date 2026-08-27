// globals.h — Game core types shared across most translation units.
//
// Reconstructed from the original GLOBALS.H header (referenced by 25
// functions in MAIN.SYM's debug symbol table). Houses the universally-
// used base types: CClass (vtable holder), the distinct game-space vector
// classes, and the plain scene-value records shared by level/gameplay code.
//
// Phase F1.3 of the source-tree split. See docs/f1_split_plan.md.

#if !defined(GLOBALS_SCENE_VALUES_ONLY)
#ifndef SRC_GLOBALS_H
#define SRC_GLOBALS_H
#define SRC_GLOBALS_CORE_EMISSION
#endif
#else
#define SRC_GLOBALS_SCENE_EMISSION
#endif

#ifdef SRC_GLOBALS_CORE_EMISSION

#ifndef SRC_PSYQ_TYPES_DEFINED
#define SRC_PSYQ_TYPES_DEFINED
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef signed char    s8;
typedef signed short   s16;
typedef signed int     s32;
#endif

// CClass — base class, single vtable pointer.
struct CClass {
#ifdef __cplusplus
    /** Allocate zeroed object storage through the shared CClass arena. */
    static void *operator new(unsigned int size);
    /** Release object storage through the shared CClass arena. */
    static void operator delete(void *storage);
#endif
    int *vtable;
};

struct CFriction;

// 3D vectors in game-space (note `vx/vy/vz` field naming, distinct from
// PSY-Q SDK's SVECTOR — same letters but different layouts).
struct CVector  {
    int vx, vy, vz;
#ifdef __cplusplus
    /** Return this vector's fixed-point magnitude. */
    int Length() const;
    /** Clear components inside the fixed-point dead zone. */
    void KillSmall();
    /** Compare all three fixed-point components. */
    int operator==(const CVector &vector);
    /** Report whether any fixed-point component differs. */
    int operator!=(const CVector &vector);
    CVector &operator+=(const CVector &vector);
    CVector &operator-=(const CVector &vector);
    CVector &operator*=(const int &scale);
    CVector &operator/=(const int &divisor);
    CVector &operator>>=(const int &shift);
    CVector &operator<<=(const int &shift);
    CVector &operator%=(const CFriction &friction);
#endif
};
struct CSVector {
    short vx, vy, vz;
#ifdef __cplusplus
    /** Wrap every component into the signed twelve-bit angle range. */
    void Mask();
    /** Clear components inside the short-vector dead zone. */
    void KillSmall();
    CSVector &operator+=(const CSVector &vector);
    CSVector &operator/=(const int &divisor);
    CSVector &operator%=(const CFriction &friction);
#endif
};
#include "CFRICTION.h"

#endif // SRC_GLOBALS_CORE_EMISSION

#if (defined(SRC_GLOBALS_CORE_EMISSION) || \
     defined(SRC_GLOBALS_SCENE_EMISSION)) && \
    !defined(SRC_GLOBALS_SCENE_VALUES_DEFINED)
#define SRC_GLOBALS_SCENE_VALUES_DEFINED

/** Plain 32-bit scene vector, distinct from the CVector class identity. */
struct Vector {
    int vx;
    int vy;
    int vz;

#ifdef __cplusplus
    /** Assign all three fixed-point components. */
    void Set(int x, int y, int z)
    {
        vx = x;
        vy = y;
        vz = z;
    }
#endif
};

/** Plain 16-bit scene vector, distinct from CSVector and SDK SVECTOR. */
struct SVector {
    short vx;
    short vy;
    short vz;
};

/** Environment item record shared by scene, collision, and spool code. */
struct Item {
    unsigned short Flags;
    unsigned short Inquiry;
    Vector Position;
    SVector Angles;
    unsigned short Model;
    unsigned char Frame;
    unsigned char TintIndex;
    unsigned char Anim;
    unsigned char Region;
    Item *NextItem;
    unsigned int RGB;
};

/** Compact per-part hook offset stored in region data. */
struct SHook {
    SVector Offset;
    unsigned short Part;
};

typedef char Vector_size_must_be_12[(sizeof(Vector) == 12) ? 1 : -1];
typedef char Vector_vx_offset_must_be_0[((unsigned long)&(((Vector *)0)->vx) == 0) ? 1 : -1];
typedef char Vector_vy_offset_must_be_4[((unsigned long)&(((Vector *)0)->vy) == 4) ? 1 : -1];
typedef char Vector_vz_offset_must_be_8[((unsigned long)&(((Vector *)0)->vz) == 8) ? 1 : -1];
typedef char SVector_size_must_be_6[(sizeof(SVector) == 6) ? 1 : -1];
typedef char SVector_vx_offset_must_be_0[((unsigned long)&(((SVector *)0)->vx) == 0) ? 1 : -1];
typedef char SVector_vy_offset_must_be_2[((unsigned long)&(((SVector *)0)->vy) == 2) ? 1 : -1];
typedef char SVector_vz_offset_must_be_4[((unsigned long)&(((SVector *)0)->vz) == 4) ? 1 : -1];
typedef char Item_size_must_be_36[(sizeof(Item) == 36) ? 1 : -1];
typedef char Item_Flags_offset_must_be_0[((unsigned long)&(((Item *)0)->Flags) == 0) ? 1 : -1];
typedef char Item_Inquiry_offset_must_be_2[((unsigned long)&(((Item *)0)->Inquiry) == 2) ? 1 : -1];
typedef char Item_Position_offset_must_be_4[((unsigned long)&(((Item *)0)->Position) == 4) ? 1 : -1];
typedef char Item_Angles_offset_must_be_16[((unsigned long)&(((Item *)0)->Angles) == 16) ? 1 : -1];
typedef char Item_Model_offset_must_be_22[((unsigned long)&(((Item *)0)->Model) == 22) ? 1 : -1];
typedef char Item_Frame_offset_must_be_24[((unsigned long)&(((Item *)0)->Frame) == 24) ? 1 : -1];
typedef char Item_TintIndex_offset_must_be_25[((unsigned long)&(((Item *)0)->TintIndex) == 25) ? 1 : -1];
typedef char Item_Anim_offset_must_be_26[((unsigned long)&(((Item *)0)->Anim) == 26) ? 1 : -1];
typedef char Item_Region_offset_must_be_27[((unsigned long)&(((Item *)0)->Region) == 27) ? 1 : -1];
typedef char Item_NextItem_offset_must_be_28[((unsigned long)&(((Item *)0)->NextItem) == 28) ? 1 : -1];
typedef char Item_RGB_offset_must_be_32[((unsigned long)&(((Item *)0)->RGB) == 32) ? 1 : -1];
typedef char SHook_size_must_be_8[(sizeof(SHook) == 8) ? 1 : -1];
typedef char SHook_Offset_offset_must_be_0[((unsigned long)&(((SHook *)0)->Offset) == 0) ? 1 : -1];
typedef char SHook_Part_offset_must_be_6[((unsigned long)&(((SHook *)0)->Part) == 6) ? 1 : -1];

#endif // SRC_GLOBALS_SCENE_VALUES_DEFINED

#ifdef SRC_GLOBALS_CORE_EMISSION

/** Packed per-part animation transform with a 16-bit translation. */
struct SMatrix {
    short m[3][3];
    short t[3];
};

typedef char SMatrix_size_must_be_24[
    (sizeof(SMatrix) == 24) ? 1 : -1];

#endif // SRC_GLOBALS_CORE_EMISSION

#undef SRC_GLOBALS_CORE_EMISSION
#undef SRC_GLOBALS_SCENE_EMISSION
