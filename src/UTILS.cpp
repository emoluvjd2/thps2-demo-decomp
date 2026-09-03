// Core timing, fixed-point maths, collision-query, and visibility utilities.

#define UTILS_OWNER_DECLARATIONS
#include "UTILS.h"
#undef UTILS_OWNER_DECLARATIONS
#include "MAIN.h"
#include "BADDY.h"
#include "MECH.h"
#include "FRONT.h"
#include "M3DZONE.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include <INLINE_C.H>
#include "PSXREGION.h"
#define M3DCOLIJ_LINE_INFO_LAYOUT
#include "M3DCOLIJ.h"
#undef M3DCOLIJ_LINE_INFO_LAYOUT

struct UtilsBodyVTable {
    void *reserved[2];
    void (*Destroy)(CBody *body, int reason);
    void (*Die)(CBody *body);
};

/** Complete CBody storage with the CItem fields used by this owner named. */
struct CBody {
    unsigned short mFlags;
    unsigned short mInquiry;
    CVector mPos;
    unsigned char _itemFieldsToNext[12];
    CBody *mNextItem;
    unsigned char _itemFieldsToVTable[40];
    UtilsBodyVTable *mVTable;
    unsigned char _remainingBodyFields[188];

    /** Return whether this body has been marked dead. */
    int IsDead() const;
};

typedef char utils_body_size_must_be_264[
    (sizeof(CBody) == 264) ? 1 : -1
];

#include "CFRICTION.h"

/** Base particle fields used to find non-rendered light objects. */
struct CBit {
    CBit *mPrevious;
    CBit *mNext;
    short mAge;
    unsigned short mLifetime;
    CVector mTweenPos;
    CVector mPos;
    CVector mVel;
    CVector mAcc;
    CFriction mFric;
    unsigned char mDead;
    unsigned short mFrigDeltaZ;
    unsigned char mProtected;
    unsigned char mType;
    void **_vf;

    static void operator delete(void *p);
};

#include "CNONRENDEREDBIT.h"

#include "CLIGHT.h"

#include "SCAMERA.h"

typedef char utils_bit_size_must_be_72[
    (sizeof(CBit) == 72) ? 1 : -1
];
typedef char utils_light_size_must_be_100[
    (sizeof(CLight) == 100) ? 1 : -1
];
typedef char utils_matrix_size_must_be_32[
    (sizeof(MATRIX) == 32) ? 1 : -1
];
typedef char utils_sdk_vector_size_must_be_16[
    (sizeof(VECTOR) == 16) ? 1 : -1
];
typedef char utils_sdk_svector_size_must_be_8[
    (sizeof(SVECTOR) == 8) ? 1 : -1
];
#include "OB.h"
#include "POWERUP.h"
#include "DB.h"
#include <LIBGPU.H>

struct CBruce;

// === External globals (absolute addressing) ===
/** Packed fixed-point sine/cosine entry used by the original table macros. */
struct SSinCos
{
    short Sin;
    short Cos;
};

union UtilsTrigTableBase
{
    int Word;
    SSinCos Entry;
};
/** Linker-visible Psy-Q trigonometry table viewed through its first entry. */
#define RCOSSIN_DECL_TYPE UtilsTrigTableBase
#define RCOSSIN_SCALAR_VIEW
#include "RCOSSIN.h"

/**
 * Read a sine entry through the table's target-proven single-word base view.
 * This preserves cc1plus's original address scheduling in the stereo mixer.
 */
static inline SSinCos *Utils_TrigEntries(void)
{
    return &rcossin_tbl.Entry;
}

static inline short Utils_Sine(int heading)
{
    return Utils_TrigEntries()[heading & 0xFFF].Sin;
}

static inline short Utils_Cosine(int heading)
{
    return Utils_TrigEntries()[heading & 0xFFF].Cos;
}
#include "CAMERASELECTION.h"
#define ENVIROLIST_ABSOLUTE_DECLARATION
#include "ENVIROLIST.h"
#define BODYLISTS_DECLARE_SUSPENDED_LIST
#include "BODYLISTS.h"
#define BODYLISTS_DECLARE_ENVIRONMENTAL_OBJECT_LIST
#include "BODYLISTS.h"
#define BITSTATE_DECLARE_NON_RENDERED_BIT_LIST
#include "BITSTATE.h"
#include "MECHLIST.h"
/**
 * Layout-compatible owner for the global explosion-centre vectors.
 *
 * The original CVector default constructor is empty but non-trivial, while
 * the generated TU header currently carries only its fields.
 */
struct UtilsExplosionCentreVector : CVector
{
    UtilsExplosionCentreVector() {}
};

UtilsExplosionCentreVector ExplosionCentre[3];

// === Owner-local globals (gp-relative, <=8 bytes for -G8) ===
int ExplosionStackIndex __attribute__((section(".sdata"))) = 0;
volatile unsigned int GameFade __attribute__((section(".sdata"))) = 0;
volatile unsigned int Vblanks __attribute__((section(".sdata"))) = 0;
volatile unsigned int Xblanks __attribute__((section(".sdata"))) = 0;
unsigned int LastVblanks __attribute__((section(".sdata"))) = 0;
int DoDisplayDraw __attribute__((section(".sdata"))) = 0;
int DoVblankProcessing __attribute__((section(".sdata"))) = 1;
int VblankUpdateBar __attribute__((section(".sdata"))) = 0;
int ExplosionType[3] __attribute__((section(".sdata"))) = {};
int ExplosionRadius[3] __attribute__((section(".sdata"))) = {};
int ExplosionDamage[3] __attribute__((section(".sdata"))) = {};
int ExplosionPriority[3] __attribute__((section(".sdata"))) = {};

// === External function declarations ===
#include "M3DMATHS.h"
/**
 * Compiler-live no-result view used only by the vblank callback.
 *
 * The canonical SDK declaration is the integer-returning DrawSync above.
 * GCC 2.91 reorders the callback's pinned packet-width value when that unused
 * return is visible, changing two target instructions.  A narrow C-linkage
 * declaration preserves the historical call schedule without an ASM alias,
 * cast, or new pin.
 */
namespace UtilsDrawSyncAbi
{
extern "C" void DrawSync(int mode);
}
#include "SFX.h"

static int RandSeed;
static int RandA;
static int RandB;

// === Function implementations (address order) ===

/** Draw the transient fade primitive and service the reverb-depth timer. */
void Utils_VblankProcessing(void)
{
    if (GameFade != 0)
    {
        POLY_F4 aPoly;
        DR_TPAGE ptpage;

        GameFade--;
        ptpage.tag = 0x01000000;
        ptpage.code[0] = 0xE1000640;
        setlen(&aPoly, 5);
        aPoly.code = 0x2A;
        if ((GameFade & 1) != 0)
        {
            aPoly.r0 = 0;
            aPoly.g0 = 0;
            aPoly.b0 = 8;
        }
        else
        {
            aPoly.r0 = 8;
            aPoly.g0 = 8;
            aPoly.b0 = 0;
        }

        // Ordinary width storage changes six instructions.
        register int screenWidth asm("$2") = 512;
        aPoly.x0 = 0;
        aPoly.x1 = screenWidth;
        aPoly.y0 = 0;
        aPoly.y1 = 0;
        aPoly.x2 = 0;
        aPoly.x3 = screenWidth;
        aPoly.y2 = 240;
        aPoly.y3 = 240;

        UtilsDrawSyncAbi::DrawSync(0);
        DrawPrim(const_cast<DR_TPAGE *>(&ptpage));
        DrawPrim(&aPoly);
        UtilsDrawSyncAbi::DrawSync(0);
    }

    if (SFX_ReverbDepthSetTimer != 0)
    {
        SFX_ReverbDepthSetTimer--;
        if (SFX_ReverbDepthSetTimer == 0)
        {
            SFX_SetReverbDepth(' ', ' ');
        }
    }
}

/** Advance frame timing and run the per-vblank game callbacks. */
void MyVSync(void)
{
    // cc1plus otherwise selects v0 and changes three target instructions.
    register unsigned int vblanks asm("$3") = Vblanks;
    Vblanks = vblanks + 1;
    Db_Vblank();
    SFX_ProcessingPerFrame();
    if (!GamePaused && !GameFrozen)
    {
        Xblanks++;
    }
    if (DoVblankProcessing)
    {
        Utils_VblankProcessing();
    }
}

/**
 * Busy-wait for a number of vertical blanks.
 *
 * @param Time Number of vblanks to wait.
 */
void Pause(int Time)
{
    unsigned int Until = Vblanks + Time;
    if (Vblanks < Until)
    {
        do
        {
        } while (Vblanks < Until);
    }
}


/**
 * Initialize the evolving linear-congruential random-number state.
 *
 * @param a Initial seed.
 */
void Utils_InitialRand(int a)
{
    RandSeed = a;
    RandA = 0x12B9B0A1;
    RandB = 0x0AA2FB3F;
}

/**
 * Return a pseudo-random value scaled by an exclusive upper bound.
 *
 * @param n Exclusive upper bound.
 * @return Scaled pseudo-random value.
 */
int Rnd(int n)
{
    RandSeed = RandSeed * RandA + RandB;
    RandA = (RandA ^ RandSeed) + (RandSeed >> 4);
    RandB -= 0x10101010;
    RandB += RandSeed >> 3;
    return ((RandSeed & 0xffff) * n) >> 16;
}

/**
 * Rotate a fixed-point vector around the Y axis.
 *
 * @param ans Receives the rotated vector.
 * @param in Source vector.
 * @param a Fixed-angle rotation.
 */
void Utils_RotateY(CVector &ans, const CVector &in, int a)
{
    a &= 0xFFF;
    // One shared table base is load-bearing; the typed helper form changes
    // three instructions, while direct array spelling changes thirty-three.
    SSinCos *table = Utils_TrigEntries();
    ans.vx = ((in.vx >> 3) * table[a].Cos +
              (in.vz >> 3) * table[a].Sin) >> 9;
    ans.vy = in.vy;
    ans.vz = ((in.vz >> 3) * table[a].Cos -
              (in.vx >> 3) * table[a].Sin) >> 9;
}

/**
 * Normalize a fixed-point vector using the GTE square operation.
 *
 * @param Unit Receives the normalized vector, or zero for a zero-length input.
 * @param Vec Source vector.
 */
void Utils_CalcUnit(CVector &Unit, const CVector &Vec)
{
    // The original allocation keeps the source in s1. Removing this pin
    // changes four instructions, while removing its lifetime barrier changes
    // 23 of the routine's 60 instructions.
    register const CVector *source asm("$17") = &Vec;
    CVector *destination = &Unit;
    asm("" : "+r"(source));
    CVector u;
    int l;

    // Keeping x in v0 preserves the GTE input-store order; ordinary storage
    // changes twenty instructions.
    register int x asm("$2") = source->vx;
    int y = source->vy;
    x >>= 12;
    y >>= 12;
    u.vx = x;
    int z = source->vz;
    CVector *squared = &u;
    squared->vy = y;
    z >>= 12;
    squared->vz = z;
    gte_ldlvl(squared);
    gte_sqr0();
    gte_stlvnl(squared);
    l = M3dMaths_SquareRoot0(u.vx + u.vy + u.vz);
    if (l != 0)
    {
        destination->vx = source->vx / l;
        destination->vy = source->vy / l;
        destination->vz = source->vz / l;
    }
    else
    {
        destination->vx = 0;
        destination->vy = 0;
        destination->vz = 0;
    }
}

/**
 * Convert a magnitude and fixed-angle direction to a Cartesian vector.
 *
 * @param v Receives the Cartesian vector.
 * @param r Vector magnitude.
 * @param a Direction angles.
 */
void Utils_GetVecFromMagDir(CVector &v, int r, const CSVector &a)
{
    v.vx = -(((r * Utils_Cosine(static_cast<unsigned short>(a.vx))) >> 12) *
             Utils_Sine(static_cast<unsigned short>(a.vy)));
    v.vy = r * Utils_Sine(static_cast<unsigned short>(a.vx));
    v.vz = -(((r * Utils_Cosine(static_cast<unsigned short>(a.vx))) >> 12) *
             Utils_Cosine(static_cast<unsigned short>(a.vy)));
}

/**
 * Measure three-dimensional distance between fixed-point positions.
 *
 * @param a First position.
 * @param b Second position.
 * @return Distance in whole world units.
 */
unsigned int Utils_Dist(const CVector &a, const CVector &b)
{
    CVector u;
    u.vx = (a.vx - b.vx) >> 12;
    u.vy = (a.vy - b.vy) >> 12;
    u.vz = (a.vz - b.vz) >> 12;
    return M3dMaths_SquareRoot0(
        u.vx * u.vx + u.vy * u.vy + u.vz * u.vz);
}

/**
 * Measure horizontal-plane distance between fixed-point positions.
 *
 * @param a First position.
 * @param b Second position.
 * @return XZ-plane distance in whole world units.
 */
unsigned int Utils_XZDist(const CVector &a, const CVector &b)
{
    int x = (a.vx - b.vx) >> 12;
    int z = (a.vz - b.vz) >> 12;
    return M3dMaths_SquareRoot0(x * x + z * z);
}

/**
 * Compute a fast octahedral approximation of three-dimensional distance.
 *
 * @param a First position.
 * @param b Second position.
 * @return Approximate distance in whole world units.
 */
unsigned int Utils_CrapDist(const CVector &a, const CVector &b)
{
    CVector u;
    u.vx = a.vx - b.vx;
    u.vy = a.vy - b.vy;
    u.vz = a.vz - b.vz;
    if (u.vx < 0)
        u.vx = -u.vx;
    if (u.vy < 0)
        u.vy = -u.vy;
    if (u.vz < 0)
        u.vz = -u.vz;
    if (u.vx < u.vy)
    {
        if (u.vy < u.vz)
        {
            return ((u.vx >> 2) + (u.vy >> 1) + u.vz) >> 12;
        }
        else if (u.vz < u.vx)
        {
            return ((u.vz >> 2) + (u.vx >> 1) + u.vy) >> 12;
        }
        else
        {
            return ((u.vx >> 2) + (u.vz >> 1) + u.vy) >> 12;
        }
    }
    else
    {
        if (u.vx < u.vz)
        {
            return ((u.vy >> 2) + (u.vx >> 1) + u.vz) >> 12;
        }
        else if (u.vz < u.vy)
        {
            return ((u.vz >> 2) + (u.vy >> 1) + u.vx) >> 12;
        }
        else
        {
            return ((u.vy >> 2) + (u.vz >> 1) + u.vx) >> 12;
        }
    }
}

/**
 * Calculate the fixed-angle direction from one position to another.
 *
 * @param Dir Receives pitch and heading.
 * @param From Origin position.
 * @param To Target position.
 * @return Horizontal distance to the target.
 */
int Utils_CalcAim(CSVector &Dir, const CVector &From, const CVector &To)
{
    int dx = (To.vx - From.vx) >> 12;
    int dy = (To.vy - From.vy) >> 12;
    int dz = (To.vz - From.vz) >> 12;

    if (dz != 0)
    {
        if (dz > 0)
        {
            Dir.vy = 0x800 - catan((dx << 12) / (-dz));
        }
        else
        {
            Dir.vy = catan((dx << 12) / dz);
        }
    }
    else
    {
        Dir.vy = (dx <= 0) ? 0x400 : -0x400;
    }

    unsigned int d = M3dMaths_SquareRoot0(dx * dx + dz * dz);
    if (d != 0)
    {
        if (dy > 0)
        {
            Dir.vx = catan(static_cast<unsigned int>(dy << 12) / d);
        }
        else
        {
            Dir.vx = -catan(static_cast<unsigned int>((-dy) << 12) / d);
        }
    }
    else
    {
        Dir.vx = (dy <= 0) ? -0x400 : 0x400;
    }

    Dir.vz = 0;
    Dir.vx = Dir.vx & 0xfff;
    Dir.vy = Dir.vy & 0xfff;
    return d;
}

/**
 * Accelerate toward an ideal orientation over the shortest wrapped arc.
 *
 * @param Current Current orientation.
 * @param AngVel Angular velocity, cleared when already aligned.
 * @param AngAcc Receives the angular acceleration.
 * @param Ideal Desired orientation.
 * @param accfactor Acceleration scale.
 */
void Utils_TurnTowards(CSVector Current, CSVector &AngVel, CSVector &AngAcc,
                       CSVector Ideal, int accfactor)
{
    CSVector angdiff;
    angdiff.vx = Ideal.vx - Current.vx;
    angdiff.vy = Ideal.vy - Current.vy;

    if (angdiff.vx < -0x800)
    {
        angdiff.vx += 0x1000;
    }
    if (angdiff.vx > 0x800)
    {
        angdiff.vx -= 0x1000;
    }
    if (angdiff.vy < -0x800)
    {
        angdiff.vy += 0x1000;
    }
    if (angdiff.vy > 0x800)
    {
        angdiff.vy -= 0x1000;
    }

    if (angdiff.vx == 0 && angdiff.vy == 0)
    {
        AngVel.vx = 0;
        AngVel.vy = 0;
    }
    else
    {
        AngAcc.vx = (accfactor * angdiff.vx) >> 8;
        AngAcc.vy = (accfactor * angdiff.vy) >> 8;
    }
}

/** Clear all queued explosion records. */
void Utils_ClearExplosionStack(void)
{
    ExplosionStackIndex = 0;
}

/**
 * Compare two strings case-insensitively with null-pointer handling.
 *
 * @param pA First string.
 * @param pB Second string.
 * @param MaxLen Maximum characters to compare.
 * @return Nonzero when the bounded strings are equal.
 */
int Utils_CompareStrings(const char *pA, const char *pB, unsigned int MaxLen)
{
    unsigned int lettera, letterb;
    if (pA == 0)
    {
        if (pB != 0)
            goto return_zero;
        return 1;
    }
    if (pB != 0)
        goto start;
return_zero:
    return 0;

    do
    {
        if (MaxLen == 0)
            goto done;
    start:
        lettera = static_cast<unsigned char>(*pA);
        pA++;
        letterb = static_cast<unsigned char>(*pB);
        pB++;
        if (static_cast<unsigned int>(lettera - 0x41) < 0x1A)
            lettera = (lettera + 0x20) & 0xFF;
        if (static_cast<unsigned int>(letterb - 0x41) < 0x1A)
            letterb = (letterb + 0x20) & 0xFF;
        if (lettera != letterb)
            goto done;
        MaxLen--;
    } while (lettera != 0);
done:
    return (lettera ^ letterb) < 1;
}

/**
 * Copy a null-terminated string, including its terminator.
 *
 * @param pSource Source string.
 * @param pDest Destination buffer.
 */
void Utils_CopyString(const char *pSource, char *pDest)
{
    do
    {
        *pDest = *pSource;
        pSource++;
        pDest++;
    } while (*(pSource - 1) != 0);
}

/**
 * Approximate inverse cosine through a binary search of the trig table.
 *
 * @param a Fixed-point cosine.
 * @return Fixed-angle result.
 */
int Utils_ArcCos(int a)
{
    int flag = 0;
    SSinCos *base;
    int t;
    register int d asm("$5");
    int c;

    if (a < 0)
    {
        a = -a;
        flag = 1;
    }

    d = 0x100;
    base = Utils_TrigEntries();
    // The a1 step register and this table-base lifetime are load-bearing:
    // ordinary allocation changes nine instructions and removing the barrier seven.
    asm volatile("" : "+r"(base));
    t = 0x200;

    for (;;)
    {
        c = base[t].Cos;
        if (a < c)
        {
            t += d;
        }
        else if (c == a)
        {
            break;
        }
        else
        {
            t -= d;
        }
        d >>= 1;
        if (d == 0)
            break;
    }

    if (flag)
    {
        return 0x800 - t;
    }
    return t;
}

/**
 * Test whether collision geometry leaves an unobstructed line of sight.
 *
 * @param pos0 Ray start.
 * @param pos1 Ray end.
 * @return Nonzero when no item blocks the ray.
 */

int Utils_LineOfSight(const CVector &pos0, const CVector &pos1)
{
    SLineInfo line_info;

    int _x = pos0.vx;
    int _y = pos0.vy;
    int _z = pos0.vz;
    line_info.StartCoords.vx = _x;
    line_info.StartCoords.vy = _y;
    line_info.StartCoords.vz = _z;
    {
        int _x = pos1.vx;
        int _y = pos1.vy;
        int _z = pos1.vz;
        line_info.EndCoords.vx = _x;
        line_info.EndCoords.vy = _y;
        line_info.EndCoords.vz = _z;
    }

    LineOfSightCheck = 1;

    M3dColij_InitLineInfo(&line_info);
    M3dZone_LineToItem(&line_info, 1);

    LineOfSightCheck = 0;

    return line_info.pItem == 0;
}

/**
 * Cast a vertical collision ray and return the floor height beneath a point.
 *
 * @param pos Reference position.
 * @param above Distance to search above the point.
 * @param below Distance to search below the point.
 * @return Fixed-point floor height, or -1 when no upward-facing floor is hit.
 */
int Utils_GetGroundHeight(const CVector &pos, int above, int below)
{
    SLineInfo line_info;

    line_info.StartCoords.vx = pos.vx;
    line_info.StartCoords.vy = pos.vy - (above << 12);
    line_info.StartCoords.vz = pos.vz;
    line_info.EndCoords.vx = pos.vx;
    line_info.EndCoords.vy = pos.vy + (below << 12);
    line_info.EndCoords.vz = pos.vz;

    M3dColij_InitLineInfo(&line_info);
    line_info.RecordTriggerZoneHits = 0;
    M3dZone_LineToItem(&line_info, 1);

    if (line_info.pItem != 0)
    {
        if (line_info.Normal.vy < -0xBFF)
        {
            return line_info.Position.vy;
        }
    }
    return -1;
}

/**
 * Destroy or kill every live body inside an axis-aligned box.
 *
 * @param min Inclusive lower bounds.
 * @param max Inclusive upper bounds.
 * @param p_list First body in the intrusive list.
 * @param use_die Invoke Die when true; otherwise invoke Destroy with reason 3.
 * @return Number of affected bodies.
 */
int Utils_KillObjectsInBox(const CVector &min, const CVector &max, CBody *p_list, bool use_die)
{
    int killed = 0;
    while (p_list)
    {
        CBody *p_next = p_list->mNextItem;
        if (!p_list->IsDead())
        {
            CVector pos = p_list->mPos;
            if (pos.vx >= min.vx && pos.vx <= max.vx &&
                pos.vz >= min.vz && pos.vz <= max.vz &&
                pos.vy >= min.vy && pos.vy <= max.vy)
            {
                if (use_die)
                {
                    p_list->mVTable->Die(p_list);
                }
                else
                {
                    if (p_list != 0)
                    {
                        p_list->mVTable->Destroy(p_list, 3);
                    }
                }
                killed++;
            }
        }
        p_list = p_next;
    }
    return killed;
}

/**
 * Remove every baddy, environment object, power-up, and suspended body in a box.
 *
 * @param min Inclusive lower bounds.
 * @param max Inclusive upper bounds.
 * @return Number of affected bodies.
 */
int Utils_KillEverythingInBox(const CVector &min, const CVector &max)
{
    int killed;
    killed = Utils_KillObjectsInBox(min, max,
                                    BaddyList, true) +
             Utils_KillObjectsInBox(
                 min, max,
                 EnvironmentalObjectList, true);
    killed += Utils_KillObjectsInBox(
        min, max, PowerUpList, true);
    killed += Utils_KillObjectsInBox(
        min, max, SuspendedList, false);
    return killed;
}

/**
 * Change object and light visibility inside or outside an axis-aligned box.
 *
 * @param min Lower bounds of the selection box.
 * @param max Upper bounds of the selection box.
 * @param visible Whether selected objects should be shown.
 * @param in Select objects inside the box when true, or outside it when false.
 */
void Utils_SetVisibilityInBox(const CVector &min, const CVector &max, bool visible, bool in)
{
    int minvx = min.vx;
    int minvy = min.vy;
    int minvz = min.vz;
    int maxvx = max.vx;
    int maxvy = max.vy;
    int maxvz = max.vz;
    int SetFlags;
    int ClearFlags;

    if (pCurrentPlayer == 0 || GNumberOfPlayers == 1 || GGame == 7)
    {
        SetFlags = 0x41;
    }
    else
    {
        SetFlags = 1;
        if (pCurrentPlayer == (CBruce *)MechList)
        {
            SetFlags = 0x40;
        }
    }

    ClearFlags = ~SetFlags;
    if (visible)
    {
        SetFlags = 0;
    }
    else
    {
        ClearFlags = -1;
    }

    Item *p_item = EnviroList;
    while (p_item != 0)
    {
        // Keeping the region cursor retains the target's single-base
        // relocation identity; direct indexing is instruction-exact but adds
        // two relocation mismatches.
        SPSXRegion *region = &PSXRegion[p_item->Region];
        if (region->Usable != 0)
        {
            int posvx = p_item->Position.vx;
            int posvy = p_item->Position.vy;
            int posvz = p_item->Position.vz;

            if (in)
            {
                if (posvx >= minvx && posvx <= maxvx &&
                    posvz >= minvz && posvz <= maxvz &&
                    posvy >= minvy && posvy <= maxvy)
                {
                    p_item->Flags = (p_item->Flags & ClearFlags) | SetFlags;
                }
            }
            else
            {
                if ((posvx < minvx || posvx > maxvx) &&
                    (posvz < minvz || posvz > maxvz) &&
                    (posvy < minvy || posvy > maxvy))
                {
                    p_item->Flags = (p_item->Flags & ClearFlags) | SetFlags;
                }
            }
        }
        p_item = p_item->NextItem;
    }

    CBody *pUp = PowerUpList;
    while (pUp != 0)
    {
        if (in)
        {
            if (pUp->mPos.vx >= minvx && pUp->mPos.vx <= maxvx &&
                pUp->mPos.vz >= minvz && pUp->mPos.vz <= maxvz &&
                pUp->mPos.vy >= minvy && pUp->mPos.vy <= maxvy)
            {
                pUp->mFlags = (pUp->mFlags & ClearFlags) | SetFlags;
            }
        }
        else
        {
            if ((pUp->mPos.vx < minvx || pUp->mPos.vx > maxvx) &&
                (pUp->mPos.vz < minvz || pUp->mPos.vz > maxvz) &&
                (pUp->mPos.vy < minvy || pUp->mPos.vy > maxvy))
            {
                pUp->mFlags = (pUp->mFlags & ClearFlags) | SetFlags;
            }
        }
        pUp = pUp->mNextItem;
    }

    CBody *pSuspended = SuspendedList;
    while (pSuspended != 0)
    {
        if (in)
        {
            if (pSuspended->mPos.vx >= minvx && pSuspended->mPos.vx <= maxvx &&
                pSuspended->mPos.vz >= minvz && pSuspended->mPos.vz <= maxvz &&
                pSuspended->mPos.vy >= minvy && pSuspended->mPos.vy <= maxvy)
            {
                pSuspended->mFlags = (pSuspended->mFlags & ClearFlags) | SetFlags;
            }
        }
        else
        {
            if ((pSuspended->mPos.vx < minvx || pSuspended->mPos.vx > maxvx) &&
                (pSuspended->mPos.vz < minvz || pSuspended->mPos.vz > maxvz) &&
                (pSuspended->mPos.vy < minvy || pSuspended->mPos.vy > maxvy))
            {
                pSuspended->mFlags = (pSuspended->mFlags & ClearFlags) | SetFlags;
            }
        }
        pSuspended = pSuspended->mNextItem;
    }

    CBody *pEnvObj = EnvironmentalObjectList;
    while (pEnvObj != 0)
    {
        if (in)
        {
            if (pEnvObj->mPos.vx >= minvx && pEnvObj->mPos.vx <= maxvx &&
                pEnvObj->mPos.vz >= minvz && pEnvObj->mPos.vz <= maxvz &&
                pEnvObj->mPos.vy >= minvy && pEnvObj->mPos.vy <= maxvy)
            {
                pEnvObj->mFlags = (pEnvObj->mFlags & ClearFlags) | SetFlags;
            }
        }
        else
        {
            if ((pEnvObj->mPos.vx < minvx || pEnvObj->mPos.vx > maxvx) &&
                (pEnvObj->mPos.vz < minvz || pEnvObj->mPos.vz > maxvz) &&
                (pEnvObj->mPos.vy < minvy || pEnvObj->mPos.vy > maxvy))
            {
                pEnvObj->mFlags = (pEnvObj->mFlags & ClearFlags) | SetFlags;
            }
        }
        pEnvObj = pEnvObj->mNextItem;
    }

    CBit *pBit = NonRenderedBitList;
    while (pBit != 0)
    {
        if (pBit->mType == 2)
        {
            if (in)
            {
                if (pBit->mPos.vx >= minvx && pBit->mPos.vx <= maxvx &&
                    pBit->mPos.vz >= minvz && pBit->mPos.vz <= maxvz &&
                    pBit->mPos.vy >= minvy && pBit->mPos.vy <= maxvy)
                {
                    if (visible)
                    {
                        static_cast<CLight *>(pBit)->mOn = 1;
                    }
                    else if (GNumberOfPlayers == 1)
                    {
                        static_cast<CLight *>(pBit)->mOn = 0;
                    }
                }
            }
            else
            {
                if ((pBit->mPos.vx < minvx || pBit->mPos.vx > maxvx) &&
                    (pBit->mPos.vz < minvz || pBit->mPos.vz > maxvz) &&
                    (pBit->mPos.vy < minvy || pBit->mPos.vy > maxvy))
                {
                    if (visible)
                    {
                        static_cast<CLight *>(pBit)->mOn = 1;
                    }
                    else if (GNumberOfPlayers == 1)
                    {
                        static_cast<CLight *>(pBit)->mOn = 0;
                    }
                }
            }
        }
        pBit = pBit->mNext;
    }
}

/**
 * Calculate camera-relative stereo attenuation for a world-space sound.
 *
 * @param pos Sound position.
 * @param near Distance at which both channels remain at full volume.
 * @param far Distance at which the sound becomes inaudible.
 * @return Packed left/right 12-bit attenuation plus the rear-channel flag.
 */
unsigned int Utils_CalculateSpatialAttenuation(const CVector &pos, int near, int far)
{
    CVector cam_pos;
    cam_pos.vx = MikeCamera[Camera_CurrentMikeCamera].Position.vx << 12;
    // This assignment cursor reproduces the inlined CVector assignment's
    // target lifetime; direct fields change 148 of 157 instructions.
    CVector *camera_position = &cam_pos;
    camera_position->vy = MikeCamera[Camera_CurrentMikeCamera].Position.vy << 12;
    camera_position->vz = MikeCamera[Camera_CurrentMikeCamera].Position.vz << 12;

    int d = Utils_CrapDist(pos, *camera_position);
    if (d <= near)
        return 0x0fff0fff;
    if (d >= far)
        return 0;

    int vol = ((far - d) * 0x0fff) / far;

    gte_SetRotMatrix(&MikeCamera[Camera_CurrentMikeCamera].View);

    CVector centre;
    centre.vx = (pos.vx >> 12) - MikeCamera[Camera_CurrentMikeCamera].Position.vx;
    centre.vy = (pos.vy >> 12) - MikeCamera[Camera_CurrentMikeCamera].Position.vy;
    centre.vz = (pos.vz >> 12) - MikeCamera[Camera_CurrentMikeCamera].Position.vz;

    gte_ldlvl(&centre);
    gte_rtir();
    gte_stlvl(&centre);

    if (!(centre.vx | centre.vz))
        return static_cast<unsigned int>(vol) |
               (static_cast<unsigned int>(vol) << 16);

    short heading = static_cast<short>(
        (0x400 - ratan2(centre.vz, centre.vx)) & 0x0fff);
    unsigned int behind = 0;
    if (static_cast<unsigned int>(heading - 0x401) < 0x07ff)
    {
        behind = 0x80000000u;
        vol -= vol >> 4;
    }

    unsigned int left;
    unsigned int right;
    if (heading < 0x800)
    {
        left = vol - ((vol * Utils_Sine(heading)) >> 12);
        right = vol;
    }
    else
    {
        left = vol;
        right = vol + ((vol * Utils_Sine(heading)) >> 12);
    }

    return left | (right << 16) | behind;
}

/**
 * `_GLOBAL_.I.ExplosionCentre` is generated by the ExplosionCentre owner above.
 * Its loop invokes the original empty CVector constructor for all three entries.
 */
