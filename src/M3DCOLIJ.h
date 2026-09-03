// Shared M3DCOLIJ API and guarded collision-owner layouts.
#ifndef M3DCOLIJ_INC_H
#define M3DCOLIJ_INC_H

struct SLineInfo;

#ifdef M3DCOLIJ_OWNER_DECLARATIONS
#define M3DCOLIJ_COLLISION_STATE_DECL_ATTR
#else
#define M3DCOLIJ_COLLISION_STATE_DECL_ATTR __attribute__((section(".data")))
#endif

/** Enables baddy collision checks for the active line-query pass. */
extern "C" int BaddyCollisionCheck M3DCOLIJ_COLLISION_STATE_DECL_ATTR; // 0x800DC52C

/** Enables camera collision checks for the active line-query pass. */
extern "C" int CameraCollisionCheck M3DCOLIJ_COLLISION_STATE_DECL_ATTR; // 0x800DC528

/** Enables bullet collision checks for the active line-query pass. */
extern "C" int BulletCollisionCheck M3DCOLIJ_COLLISION_STATE_DECL_ATTR; // 0x800DC530

/** Enables trigger collision checks for the active line-query pass. */
extern "C" int TriggerCollisionCheck M3DCOLIJ_COLLISION_STATE_DECL_ATTR; // 0x800DC534

/** Selects the line-of-sight collision-query mode. */
extern "C" int LineOfSightCheck M3DCOLIJ_COLLISION_STATE_DECL_ATTR; // 0x800DC538

#undef M3DCOLIJ_COLLISION_STATE_DECL_ATTR

/** Build the transform, bounds, length, and inquiry stamp for a line query. */
void M3dColij_InitLineInfo(SLineInfo *pInfo);

#ifndef M3DCOLIJ_OWNER_DECLARATIONS
struct CBody;
struct CVector;

/** Reset collision-cache cursors and clear every cached item. */
void M3dColij_FlushCache(void);

/** Test a swept line against the spheres in a body list. */
CBody *M3dColij_LineToSphere(
    const CVector &OldPos, const CVector &NewPos, CVector &ColijPos,
    CBody *pBody, CBody *pIgnore, int ScaleFactor);
#endif

#ifdef M3DCOLIJ_OWNER_DECLARATIONS
#define M3DCOLIJ_OWNER_DECLARATIONS_ACTIVE

#ifdef __cplusplus
extern "C" {
#endif

// Primitive types
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned char byte;
typedef int undefined;
typedef char undefined1;
typedef short undefined2;
typedef int undefined4;
typedef unsigned long ulong;
typedef unsigned char undefined3[3];

#include <STDDEF.H>
#include <LIBGTE.H>
#include "globals.h"

// TSPack — VRAM pack allocator entry

// Palette entry (20 bytes)

// MeshHeader (28 bytes)
typedef struct MeshHeader {
    u16 meshFlags;
    u16 vertexCount;
    u16 normalCount;
    u16 faceCount;
    u32 radius;
    s16 bboxXMax;
    s16 bboxXMin;
    s16 bboxYMax;
    s16 bboxYMin;
    s16 bboxZMax;
    s16 bboxZMin;
    s16 lodDepth;
    u16 lodNextMesh;
} MeshHeader;

// SAnimFrame — Animation frame entry (8 bytes)
// Forward declarations for large/opaque types
struct CBody;
struct CSuper;
struct EditorOutModel;
struct EditorInModel;
struct LevelGenerator;
struct CBruce;
struct CBaddy;
struct SModel;
struct SHook;
struct CPowerUp;

// Additional shared forward declarations
struct CBit;
struct CGlow;
struct CSpecialDisplay;
struct CFlatBit;
struct Font;
struct TextEffect;
struct FontManager;
struct RailPoint;
struct SCardHead;
struct SCamera;
struct STexWibVertInfo;
struct SViewport;
struct SColCache;
struct SLineInfo;
struct SHit;
struct STexWibItemInfo;
struct SBlockHeader;
struct ControllerData;

#endif // M3DCOLIJ_OWNER_DECLARATIONS

#endif // M3DCOLIJ_INC_H

/*
 * M3DCOLIJ.cpp suppresses its public service declarations during the owner
 * include so each declaration can retain its target-proven encounter point.
 * These per-service selectors are intentionally re-entrant.
 */
#if defined(M3DCOLIJ_DECLARE_FLUSH_CACHE_SERVICE) && \
    !defined(M3DCOLIJ_FLUSH_CACHE_SERVICE_DECLARED)
#define M3DCOLIJ_FLUSH_CACHE_SERVICE_DECLARED
/** Reset collision-cache cursors and clear every cached item. */
void M3dColij_FlushCache(void);
#endif
#undef M3DCOLIJ_DECLARE_FLUSH_CACHE_SERVICE

#if defined(M3DCOLIJ_DECLARE_LINE_TO_SPHERE_SERVICE) && \
    !defined(M3DCOLIJ_LINE_TO_SPHERE_SERVICE_DECLARED)
#define M3DCOLIJ_LINE_TO_SPHERE_SERVICE_DECLARED
/** Test a swept line against the spheres in a body list. */
CBody *M3dColij_LineToSphere(
    const CVector &OldPos, const CVector &NewPos, CVector &ColijPos,
    CBody *pBody, CBody *pIgnore, int ScaleFactor);
#endif
#undef M3DCOLIJ_DECLARE_LINE_TO_SPHERE_SERVICE

#if (defined(M3DCOLIJ_OWNER_DECLARATIONS_ACTIVE) || \
     defined(M3DCOLIJ_LAYOUT_PREREQUISITES)) && \
    !defined(M3DCOLIJ_COL_CACHE_LAYOUT)
#define M3DCOLIJ_COL_CACHE_LAYOUT
#define M3DCOLIJ_COL_CACHE_LAYOUT_WAS_IMPLIED
#endif

#if defined(M3DCOLIJ_COL_CACHE_LAYOUT) && \
    !defined(M3DCOLIJ_COL_CACHE_LAYOUT_EMITTED)
#define M3DCOLIJ_COL_CACHE_LAYOUT_EMITTED

/** One cached item's model and contiguous face span. */
struct SColCache {
    struct Item *pItem;
    struct SModel *pModel;
    int Faces;
    int FirstFace;
};

#endif // M3DCOLIJ_COL_CACHE_LAYOUT

#ifdef M3DCOLIJ_COL_CACHE_LAYOUT_WAS_IMPLIED
#undef M3DCOLIJ_COL_CACHE_LAYOUT
#undef M3DCOLIJ_COL_CACHE_LAYOUT_WAS_IMPLIED
#endif

#if defined(M3DCOLIJ_OWNER_DECLARATIONS_ACTIVE) && \
    !defined(M3DCOLIJ_OWNER_DECLARATIONS_TAIL_EMITTED)
#define M3DCOLIJ_OWNER_DECLARATIONS_TAIL_EMITTED

struct SFaceCache {
    int Flags;
    int XMin;
    int YMin;
    int ZMin;
    int XMax;
    int YMax;
    int ZMax;
};

#ifdef __cplusplus
}
#endif

// === Variables (gp-relative in this TU) ===
extern "C" int CollidedTriggerModel;       // 0x800DC570
extern "C" int FaceCacheIndex;       // 0x800DC544
extern "C" int FaceCacheStart;       // 0x800DC548
extern "C" u16 Inquiry;       // 0x800DC524
extern "C" int ItemCacheIndex;       // 0x800DC53C
extern "C" int ItemCacheStart;       // 0x800DC540
#define M3DCOLIJMASKS_OWNER_DECLARATION
#include "M3DCOLIJMASKS.h"
extern "C" CBody *pBodyHit;       // 0x800DC56C

// === Variables (absolute in this TU) ===
#define ENVIROLIST_ABSOLUTE_DECLARATION
#include "ENVIROLIST.h"
#define BODYLISTS_DECLARE_ENVIRONMENTAL_OBJECT_LIST
#include "BODYLISTS.h"
extern "C" SHit NearestHit;     // 0x800C84F0
#define SPOOLENV_DECLARE_OTHER_ENV_INDEX
#include "SPOOLENV.h"
#include "PROJECTEDVERTICES.h"
extern "C" MATRIX Tfm;     // 0x800C85B8

#endif // M3DCOLIJ_OWNER_DECLARATIONS_TAIL_EMITTED

#ifdef M3DCOLIJ_OWNER_DECLARATIONS_ACTIVE
#undef M3DCOLIJ_OWNER_DECLARATIONS_ACTIVE
#endif

#if (defined(M3DCOLIJ_LINE_INFO_LAYOUT) || defined(M3DCOLIJ_OWNER_LAYOUT)) && \
    !defined(M3DCOLIJ_LINE_INFO_LAYOUT_EMITTED)
#define M3DCOLIJ_LINE_INFO_LAYOUT_EMITTED

#define GLOBALS_SCENE_VALUES_ONLY
#include "globals.h"
#undef GLOBALS_SCENE_VALUES_ONLY

#ifndef _LIBGTE_H_
#error "M3DCOLIJ line layout requires the Psy-Q LIBGTE declarations first"
#endif

/** Complete collision line-query state shared across gameplay owners. */
struct SLineInfo {
    Vector StartCoords;
    Vector EndCoords;
    Vector MinCoords;
    Vector MaxCoords;
    int iLo;
    int iHi;
    int jLo;
    int jHi;
    int Distance;
    int Length;
    MATRIX WorldCst;
    Item *pItem;
    Vector Position;
    SVECTOR Normal;
    unsigned int *pFace;
    int Model;
    unsigned char RecordTriggerZoneHits;
    unsigned char DropDown;
    unsigned short Inquiry;
    int tNear;
    unsigned int tNumtrLo;
    int tNumtrHi;
    unsigned int tDenomLo;
    int tDenomHi;
    unsigned int NormalOffset;
};

typedef char SLineInfo_size_must_be_164[(sizeof(SLineInfo) == 164) ? 1 : -1];
typedef char SLineInfo_StartCoords_offset_must_be_0[((unsigned long)&(((SLineInfo *)0)->StartCoords) == 0) ? 1 : -1];
typedef char SLineInfo_EndCoords_offset_must_be_12[((unsigned long)&(((SLineInfo *)0)->EndCoords) == 12) ? 1 : -1];
typedef char SLineInfo_MinCoords_offset_must_be_24[((unsigned long)&(((SLineInfo *)0)->MinCoords) == 24) ? 1 : -1];
typedef char SLineInfo_MaxCoords_offset_must_be_36[((unsigned long)&(((SLineInfo *)0)->MaxCoords) == 36) ? 1 : -1];
typedef char SLineInfo_iLo_offset_must_be_48[((unsigned long)&(((SLineInfo *)0)->iLo) == 48) ? 1 : -1];
typedef char SLineInfo_iHi_offset_must_be_52[((unsigned long)&(((SLineInfo *)0)->iHi) == 52) ? 1 : -1];
typedef char SLineInfo_jLo_offset_must_be_56[((unsigned long)&(((SLineInfo *)0)->jLo) == 56) ? 1 : -1];
typedef char SLineInfo_jHi_offset_must_be_60[((unsigned long)&(((SLineInfo *)0)->jHi) == 60) ? 1 : -1];
typedef char SLineInfo_Distance_offset_must_be_64[((unsigned long)&(((SLineInfo *)0)->Distance) == 64) ? 1 : -1];
typedef char SLineInfo_Length_offset_must_be_68[((unsigned long)&(((SLineInfo *)0)->Length) == 68) ? 1 : -1];
typedef char SLineInfo_WorldCst_offset_must_be_72[((unsigned long)&(((SLineInfo *)0)->WorldCst) == 72) ? 1 : -1];
typedef char SLineInfo_pItem_offset_must_be_104[((unsigned long)&(((SLineInfo *)0)->pItem) == 104) ? 1 : -1];
typedef char SLineInfo_Position_offset_must_be_108[((unsigned long)&(((SLineInfo *)0)->Position) == 108) ? 1 : -1];
typedef char SLineInfo_Normal_offset_must_be_120[((unsigned long)&(((SLineInfo *)0)->Normal) == 120) ? 1 : -1];
typedef char SLineInfo_pFace_offset_must_be_128[((unsigned long)&(((SLineInfo *)0)->pFace) == 128) ? 1 : -1];
typedef char SLineInfo_Model_offset_must_be_132[((unsigned long)&(((SLineInfo *)0)->Model) == 132) ? 1 : -1];
typedef char SLineInfo_RecordTriggerZoneHits_offset_must_be_136[((unsigned long)&(((SLineInfo *)0)->RecordTriggerZoneHits) == 136) ? 1 : -1];
typedef char SLineInfo_DropDown_offset_must_be_137[((unsigned long)&(((SLineInfo *)0)->DropDown) == 137) ? 1 : -1];
typedef char SLineInfo_Inquiry_offset_must_be_138[((unsigned long)&(((SLineInfo *)0)->Inquiry) == 138) ? 1 : -1];
typedef char SLineInfo_tNear_offset_must_be_140[((unsigned long)&(((SLineInfo *)0)->tNear) == 140) ? 1 : -1];
typedef char SLineInfo_tNumtrLo_offset_must_be_144[((unsigned long)&(((SLineInfo *)0)->tNumtrLo) == 144) ? 1 : -1];
typedef char SLineInfo_tNumtrHi_offset_must_be_148[((unsigned long)&(((SLineInfo *)0)->tNumtrHi) == 148) ? 1 : -1];
typedef char SLineInfo_tDenomLo_offset_must_be_152[((unsigned long)&(((SLineInfo *)0)->tDenomLo) == 152) ? 1 : -1];
typedef char SLineInfo_tDenomHi_offset_must_be_156[((unsigned long)&(((SLineInfo *)0)->tDenomHi) == 156) ? 1 : -1];
typedef char SLineInfo_NormalOffset_offset_must_be_160[((unsigned long)&(((SLineInfo *)0)->NormalOffset) == 160) ? 1 : -1];

#endif // M3DCOLIJ_LINE_INFO_LAYOUT

#if defined(M3DCOLIJ_OWNER_LAYOUT) && !defined(M3DCOLIJ_OWNER_LAYOUT_EMITTED)
#define M3DCOLIJ_OWNER_LAYOUT_EMITTED

#include "PSXREGION.h"
#include "SMODEL.h"

struct SProjectedVertex;
struct SLight;

/** View compact item angles through the GTE helper's SVECTOR prefix. */
static inline SVECTOR *M3dColij_GteAngles(SVector *angles)
{
    return (SVECTOR *)angles;
}

/** Nearest collision result owned by M3DCOLIJ. */
struct SHit {
    struct Item *pItem;
    unsigned int *pFace;
    struct CVector Normal;
    struct CVector PolyCentre;
    int Dist;

    SHit() {}
};

/** Complete item prefix embedded in a collision body (76 bytes). */
struct M3dColijBodyItem {
    u16 mFlags;
    u16 mInquiry;
    struct CVector mPos;
    struct CSVector mAngles;
    u16 mModel;
    s8 mDummyFrame;
    u8 mTintIndex;
    u8 mDummyAnim;
    u8 mRegion;
    struct CBody *mNextItem;
    u32 mRGB;
    struct CSVector mScale;
    u32 mTRN;
    struct M3dColijBodyItem *mPreviousItem;
    u32 mStoredRGB;
    u8 mPredatorEffectType;
    u8 mPredatorEffectFade;
    u16 mType;
    struct SLight *mpLight;
    u16 mFlashTimer;
    s16 mFlash_dR;
    s16 mFlash_dG;
    s16 mFlash_dB;
    void **_vf;
};

/** Collision-body layout recovered from `MAIN_STRUCTS.h` (264 bytes). */
struct CBody {
    struct M3dColijBodyItem CItem;
    struct CBody **mppOriginalList;
    u16 mInputFlags;
    u16 mCBodyFlags;
    struct CVector mVel;
    struct CVector mAcc;
    struct CFriction mFric;
    struct CSVector mAngVel;
    struct CSVector mAngAcc;
    struct CFriction mAngFric;
    struct CSVector mNormal;
    u8 mEventFlags;
    u32 mEllipsoidRGB;
    u16 mEllipsoidScale;
    s16 mEllipsoidOTPushback;
    struct CVector mShadowPos;
    struct CSVector mShadowNormal;
    void *mpShadow;
    u16 mShadowScale;
    s16 mShadowDist;
    s16 mShadowThreshold;
    int mPushVal;
    u16 mRMinor;
    u16 mNode;
    u16 mCollision;
    s16 mHealth;
    u32 mPlayerDist;
    u8 mAIFrequency;
    u8 mAICount;
    struct CVector mOldPos;
    struct CVector mTweenPos;
    struct CVector mNewPos;
    struct CVector mDPos;
    struct CSVector mOldAngles;
    struct CSVector mNewAngles;
    struct CSVector mDAngles;
};

/** Collision assembly and fixed-point maths collaborators used by this owner. */
#include "M3DASM.h"
void M3dMaths_MulLcmMatrix(MATRIX *left, MATRIX *right);
int Spool_GetEnvIndex(int region);
struct CVector operator-(const CVector &left, const CVector &right);
struct CVector operator>>(const CVector &vector, const int &shift);
struct CVector operator<<(const CVector &vector, const int &shift);
struct CVector operator/(const CVector &vector, const int &divisor);

extern "C" {
/** Unmangled matrix helper. */
MATRIX *MulMatrix(MATRIX *m0, MATRIX *m1);

/** Typed absolute views of collision globals owned by other modules. */
#define SPOOLENV_ABSOLUTE_DECLARATIONS
#define SPOOLENV_DECLARE_ENV_MODEL_CHECKSUMS
#define SPOOLENV_DECLARE_OTHER_ENV_INDEX
#include "SPOOLENV.h"
#undef SPOOLENV_ABSOLUTE_DECLARATIONS
}

typedef char M3dColij_Model_size_must_be_36[(sizeof(SModel) == 36) ? 1 : -1];
typedef char M3dColij_BodyItem_size_must_be_76[
    (sizeof(M3dColijBodyItem) == 76) ? 1 : -1];
typedef char M3dColij_Body_size_must_be_264[
    (sizeof(CBody) == 264) ? 1 : -1];
typedef char M3dColij_Cache_size_must_be_16[(sizeof(SColCache) == 16) ? 1 : -1];
typedef char M3dColij_Hit_size_must_be_36[(sizeof(SHit) == 36) ? 1 : -1];

#endif // M3DCOLIJ_OWNER_LAYOUT
