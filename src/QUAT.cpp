// QUAT.cpp -- Reconstructed translation unit
// Original: QUAT.CPP
// Functions: 10
// Address range: 0x80078214 - 0x800790B0
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.


// === Forward type declarations ===

#include "QUAT.h"
#include "UTILS.h"


// === External globals ===
/** Packed fixed-point sine/cosine table entry. */
struct SSinCos {
    short Sin;
    short Cos;
};

/** Linker-visible base word of the 4096-entry trigonometry table. */
#define RCOSSIN_DECL_TYPE int
#define RCOSSIN_SCALAR_VIEW
#include "RCOSSIN.h"
#define Sine(angle) \
    (reinterpret_cast<SSinCos *>( \
        &rcossin_tbl + ((angle) & 0xFFF))->Sin)
extern int __vt_5CQuat;  // gp+0x3D6C

/** Prepare uninitialized quaternion storage for a named return value. */
inline CQuat::CQuat() {
}

/** Copy a quaternion while restoring the CQuat virtual table. */
inline CQuat::CQuat(const CQuat &_ctor_arg) {
    CClass.vtable = &__vt_5CQuat;
    x = _ctor_arg.x;
    y = _ctor_arg.y;
    z = _ctor_arg.z;
    w = _ctor_arg.w;
}
/** Restore the destructor-phase CQuat virtual table. */
inline CQuat::~CQuat() {
    CClass.vtable = &__vt_5CQuat + 4;
}
static int next[3] = { 1, 2, 0 };
#include "M3DMATHS.h"

/** Normalize a fixed-point quaternion in place. */
inline void CQuat::Unit() {
    int mag = SquareRoot0(x * x + y * y + z * z + w * w);
    if (mag == 0) {
        mag = 1;
    }
    x = (x << 12) / mag;
    y = (y << 12) / mag;
    z = (z << 12) / mag;
    w = (w << 12) / mag;
}

// === Free function declarations ===
static void Quat_DestroyQuat(CQuat *, int)
    asm("__DOT__5CQuat_80079050");
static void Quat_DestroyClass(CClass *, int)
    asm("__DOT__6CClass_80079080");

// GCC 2.x passes these structure-return results through a hidden first pointer.
// The named trigonometry-table base and entry cursors in their bodies are also
// intentional: direct typed indexing changed three instructions per rotation,
// while separate component macros changed twelve instructions and one
// relocation.  The cursors preserve the target's single absolute base load.
// === Function implementations (address order) ===

// ====== 0x80078214 __ml__FRC5CQuatT0 ===========================================
/**
 * Multiply two fixed-point quaternions.
 *
 * @param a Left-hand quaternion.
 * @param b Right-hand quaternion.
 * @return The composed quaternion.
 */
CQuat operator*(const CQuat & a, const CQuat & b) {
    CQuat q = b;
    CQuat &result = q;
    int tempw;
    int tempz;
    int tempy;
    int tempx;

    tempw = b.w * a.w - b.x * a.x - b.y * a.y - b.z * a.z;
    tempx = b.w * a.x + b.x * a.w + b.y * a.z - b.z * a.y;
    tempy = b.w * a.y + b.y * a.w + b.z * a.x - b.x * a.z;
    tempz = b.w * a.z + b.z * a.w + b.x * a.y - b.y * a.x;

    result.x = tempx >> 12;
    result.y = tempy >> 12;
    result.z = tempz >> 12;
    result.w = tempw >> 12;
    return result;
}


/**
 * Builds a fixed-point quaternion from an axis vector whose length is the
 * rotation angle.
 *
 * @param v Axis-and-angle vector.
 * @return Quaternion representing the requested axis rotation.
 */
CQuat QFromV(const CVector &v) return result {
    CVector tempv;
    unsigned int mag = M3dMaths_SquareRoot0(
        v.vx * v.vx + v.vy * v.vy + v.vz * v.vz);

    if (mag == 0) {
        result.CClass.vtable = &__vt_5CQuat;
        result.x = 0;
        result.y = 0;
        result.z = 0;
        result.w = 0x1000;
        goto done;
    }

    // The SDK routine reads and writes only the common vx/vy/vz prefix;
    // CVector intentionally omits VECTOR's unused padding word.
    VectorNormal(reinterpret_cast<VECTOR *>(const_cast<CVector *>(&v)),
                 reinterpret_cast<VECTOR *>(&tempv));
    {
        int d = mag << 1;
        SSinCos *base = reinterpret_cast<SSinCos *>(&rcossin_tbl);
        SSinCos *entry = reinterpret_cast<SSinCos *>(
            reinterpret_cast<char *>(base) + (d & 0x3FFCu));
        int sin2 = entry->Sin;
        int cos2 = entry->Cos;
        result.CClass.vtable = &__vt_5CQuat;
        result.w = cos2;
        result.x = sin2 * tempv.vx >> 12;
        result.y = sin2 * tempv.vy >> 12;
        result.z = sin2 * tempv.vz >> 12;
    }
done:
    ;
}


/**
 * Builds a quaternion representing an X-axis rotation.
 *
 * @param angle Fixed-point rotation angle.
 * @return Quaternion representing the X-axis rotation.
 */
CQuat QFromXRot(short angle) return result {
    int d = angle << 1;
    SSinCos *base = reinterpret_cast<SSinCos *>(&rcossin_tbl);
    SSinCos *entry = reinterpret_cast<SSinCos *>(
        reinterpret_cast<char *>(base) + (d & 0x3FFCu));
    int cos2 = entry->Cos;
    int sin2 = entry->Sin;
    int a = sin2;
    result.CClass.vtable = &__vt_5CQuat;
    result.y = 0;
    result.z = 0;
    result.x = a;
    result.w = cos2;
}


/**
 * Builds a quaternion representing a Y-axis rotation.
 *
 * @param angle Fixed-point rotation angle.
 * @return Quaternion representing the Y-axis rotation.
 */
CQuat QFromYRot(short angle) return result {
    int d = angle << 1;
    SSinCos *base = reinterpret_cast<SSinCos *>(&rcossin_tbl);
    SSinCos *entry = reinterpret_cast<SSinCos *>(
        reinterpret_cast<char *>(base) + (d & 0x3FFCu));
    int cos2 = entry->Cos;
    int sin2 = entry->Sin;
    int b = sin2;
    result.CClass.vtable = &__vt_5CQuat;
    result.x = 0;
    result.z = 0;
    result.y = b;
    result.w = cos2;
}


/**
 * Builds a quaternion representing a Z-axis rotation.
 *
 * @param angle Fixed-point rotation angle.
 * @return Quaternion representing the Z-axis rotation.
 */
CQuat QFromZRot(short angle) return result {
    int d = angle << 1;
    SSinCos *base = reinterpret_cast<SSinCos *>(&rcossin_tbl);
    SSinCos *entry = reinterpret_cast<SSinCos *>(
        reinterpret_cast<char *>(base) + (d & 0x3FFCu));
    int cos2 = entry->Cos;
    int sin2 = entry->Sin;
    int c = sin2;
    result.CClass.vtable = &__vt_5CQuat;
    result.x = 0;
    result.y = 0;
    result.z = c;
    result.w = cos2;
}


/**
 * Convert a quaternion into a fixed-point rotation matrix.
 *
 * @param q  Source fixed-point unit quaternion.
 * @param m  Destination rotation matrix.
 */
void QToM(const CQuat & q, MATRIX & m) {
    int wx, wy, wz, xx, xy, xz, yy, yz, zz;
    wx = (q.w * q.x) >> 11;
    wy = (q.w * q.y) >> 11;
    wz = (q.w * q.z) >> 11;
    xx = (q.x * q.x) >> 11;
    xy = (q.x * q.y) >> 11;
    xz = (q.x * q.z) >> 11;
    yy = (q.y * q.y) >> 11;
    yz = (q.y * q.z) >> 11;
    zz = (q.z * q.z) >> 11;
    m.t[0] = 0;
    m.t[1] = 0;
    m.t[2] = 0;
    m.m[0][0] = 0x1000 - (yy + zz);
    m.m[0][1] = xy + wz;
    m.m[0][2] = xz - wy;
    m.m[1][0] = xy - wz;
    m.m[1][1] = 0x1000 - (xx + zz);
    m.m[1][2] = yz + wx;
    m.m[2][0] = xz + wy;
    m.m[2][1] = yz - wx;
    m.m[2][2] = 0x1000 - (xx + yy);
    return;
}


/**
 * Converts a fixed-point rotation matrix into a quaternion.
 *
 * @param m  Source rotation matrix.
 * @param q  Quaternion to write.
 */
void MToQ(const MATRIX & m, CQuat & q) {
    int tr = m.m[0][0] + m.m[1][1] + m.m[2][2];
    int s;

    if (tr > 0) {
        s = SquareRoot0((tr + 0x1000) << 12);
        q.w = s >> 1;

        if (s != 0) {
            s = 0x800000 / s;
        }

        q.x = ((m.m[1][2] - m.m[2][1]) * s) >> 12;
        q.y = ((m.m[2][0] - m.m[0][2]) * s) >> 12;
        q.z = ((m.m[0][1] - m.m[1][0]) * s) >> 12;
    } else {
        int i = (m.m[0][0] < m.m[1][1]);
        if (m.m[i][i] < m.m[2][2]) {
            i = 2;
        }

        int j = next[i];
        int k = next[j];
        s = SquareRoot0((m.m[i][i] - (m.m[j][j] + m.m[k][k]) + 0x1000) << 12);

        switch (i) {
            case 0:
                q.x = s >> 1;
                break;
            case 1:
                q.y = s >> 1;
                break;
            case 2:
                q.z = s >> 1;
                break;
        }

        if (s != 0) {
            s = 0x800000 / s;
        }

        q.w = ((m.m[j][k] - m.m[k][j]) * s) >> 12;

        switch (j) {
            case 0:
                q.x = ((m.m[i][j] + m.m[j][i]) * s) >> 12;
                break;
            case 1:
                q.y = ((m.m[i][j] + m.m[j][i]) * s) >> 12;
                break;
            case 2:
                q.z = ((m.m[i][j] + m.m[j][i]) * s) >> 12;
                break;
        }

        switch (k) {
            case 0:
                q.x = ((m.m[i][k] + m.m[k][i]) * s) >> 12;
                break;
            case 1:
                q.y = ((m.m[i][k] + m.m[k][i]) * s) >> 12;
                break;
            case 2:
                q.z = ((m.m[i][k] + m.m[k][i]) * s) >> 12;
                break;
        }
    }
}


/**
 * Interpolates between two quaternions and writes a normalized result.
 *
 * @param p  Starting quaternion.
 * @param q  Ending quaternion.
 * @param t  Fixed-point interpolation amount.
 * @param qt Interpolated quaternion.
 */
void Quat_Slerp(CQuat p, CQuat q, int t, CQuat & qt) {
    int omega, cosom, sinom, sclp, sclq;

    p.Unit();
    q.Unit();

    cosom = (p.x * q.x + p.y * q.y + p.z * q.z + p.w * q.w) >> 12;
    if (cosom < 0) {
        cosom = -cosom;
        p.x = -p.x;
        p.y = -p.y;
        p.z = -p.z;
        p.w = -p.w;
    }

    if (cosom > 0x1000) {
        cosom = 0x1000;
    }

    if (cosom + 0x1000 >= 0x81) {
        if (0x1000 - cosom >= 0x81) {
            omega = Utils_ArcCos(cosom);
            sinom = Sine(omega);
            sclp = (Sine(((0x1000 - t) * omega) >> 12) << 12) / sinom;
            sclq = (Sine((t * omega) >> 12) << 12) / sinom;
        } else {
            sclp = 0x1000 - t;
            sclq = t;
        }

        qt.x = (sclp * p.x + sclq * q.x) >> 12;
        qt.y = (sclp * p.y + sclq * q.y) >> 12;
        qt.z = (sclp * p.z + sclq * q.z) >> 12;
        qt.w = (sclp * p.w + sclq * q.w) >> 12;
    } else {
        qt.x = -p.y;
        qt.y = -p.x;
        qt.z = -p.w;
        qt.w = p.z;
        sclp = Sine(((u32)((0x1000 - t) * 0xc91)) >> 11);
        sclq = Sine(((u32)(t * 0xc91)) >> 11);
        qt.x = (sclp * p.x + sclq * qt.x) >> 12;
        qt.y = (sclp * p.y + sclq * qt.y) >> 12;
        qt.z = (sclp * p.z + sclq * qt.z) >> 12;
        qt.w = (sclp * p.w + sclq * qt.w) >> 12;
    }

    qt.Unit();
}


/**
 * Destroy a quaternion and optionally release its storage.
 *
 * @param _this Quaternion being destroyed.
 * @param __in_chrg GCC 2.x in-charge flag; bit 0 releases the object.
 */
static void Quat_DestroyQuat(CQuat *_this, int __in_chrg) {
    _this->CClass.vtable = &__vt_5CQuat + 4;
    if (__in_chrg & 1) {
        CClass::operator delete(_this);
    }
}


/**
 * Destroy the owner-local CClass base and optionally release its storage.
 *
 * @param _this Base object being destroyed.
 * @param __in_chrg GCC 2.x in-charge flag; bit 0 releases the object.
 */
static void Quat_DestroyClass(CClass *_this, int __in_chrg) {
    _this->vtable = &__vt_5CQuat + 4;
    if (__in_chrg & 1) {
        CClass::operator delete(_this);
    }
}
