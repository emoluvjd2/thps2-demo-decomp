// Shared utility types, timing globals, and public functions.
#ifndef UTILS_INC_H
#define UTILS_INC_H

struct CVector;
struct CSVector;
struct CBody;

/** Returns the Euclidean distance between fixed-point positions. */
unsigned int Utils_Dist(const CVector &a, const CVector &b);
/** Returns the horizontal-plane distance between fixed-point positions. */
unsigned int Utils_XZDist(const CVector &a, const CVector &b);
/** Returns the fast octahedral approximation of a three-dimensional distance. */
unsigned int Utils_CrapDist(const CVector &a, const CVector &b);
void Utils_VblankProcessing(void);
void MyVSync(void);
void Pause(int Time);
void Utils_InitialRand(int a);
int Rnd(int n);
void Utils_RotateY(CVector &ans, const CVector &in, int a);
void Utils_CalcUnit(CVector &unit, const CVector &vec);
void Utils_GetVecFromMagDir(CVector &v, int magnitude,
                            const CSVector &direction);
int Utils_CalcAim(CSVector &direction, const CVector &from,
                  const CVector &to);
void Utils_TurnTowards(CSVector current, CSVector &angularVelocity,
                       CSVector &angularAcceleration, CSVector ideal,
                       int accelerationFactor);
void Utils_ClearExplosionStack(void);
int Utils_CompareStrings(const char *lhs, const char *rhs,
                         unsigned int maxLength);
void Utils_CopyString(const char *source, char *destination);
int Utils_ArcCos(int cosine);
int Utils_LineOfSight(const CVector &start, const CVector &end);
int Utils_GetGroundHeight(const CVector &position, int above, int below);
int Utils_KillObjectsInBox(const CVector &min, const CVector &max,
                           CBody *list, bool useDie);
int Utils_KillEverythingInBox(const CVector &min, const CVector &max);
void Utils_SetVisibilityInBox(const CVector &min, const CVector &max,
                              bool visible, bool inside);
unsigned int Utils_CalculateSpatialAttenuation(const CVector &position,
                                                int nearDistance,
                                                int farDistance);

#ifdef UTILS_OWNER_DECLARATIONS
#define UTILS_SHARED_STATE_DECL_ATTR
#else
#define UTILS_SHARED_STATE_DECL_ATTR __attribute__((section(".data")))
#endif

/** Remaining intensity of the interrupt-driven full-screen fade. */
extern "C" volatile unsigned int GameFade UTILS_SHARED_STATE_DECL_ATTR; // 0x800DA2D4

/** Interrupt-driven vertical-blank counter shared by frame loops. */
extern "C" volatile unsigned int Vblanks UTILS_SHARED_STATE_DECL_ATTR; // 0x800DA2D8

/** Interrupt-driven extended frame counter shared by gameplay systems. */
extern "C" volatile unsigned int Xblanks UTILS_SHARED_STATE_DECL_ATTR; // 0x800DA2DC

#undef UTILS_SHARED_STATE_DECL_ATTR

#ifdef UTILS_OWNER_DECLARATIONS

struct CVector
{
    int vx;
    int vy;
    int vz;
};

struct CSVector
{
    short vx;
    short vy;
    short vz;
};

// === Variables (gp-relative in this TU) ===
extern "C" int DoVblankProcessing;
extern "C" int ExplosionStackIndex;

// External game state read or updated by the utility owner.
extern "C" int SFX_ReverbDepthSetTimer
    __attribute__((section(".data")));

#endif // UTILS_OWNER_DECLARATIONS

#endif // UTILS_INC_H
