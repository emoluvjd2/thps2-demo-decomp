// VECTOR.cpp -- Reconstructed translation unit
// Original: VECTOR.CPP
// Functions: 28 reconstructed entries
// Address range: 0x8004EF00 - 0x8004F784
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.
//
// CVector::Length uses the PS1 GTE SQR command to square its fixed-point
// components. Scalar arithmetic, including all vector division, remains C++.


// === Type definitions with actual fields ===

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;

#include "CFRICTION.h"

/** Compact three-component vector used for angles and directions. */
struct CSVector {
    s16 vx, vy, vz;

    void Mask();
    void KillSmall();
    CSVector &operator+=(const CSVector &a);
    CSVector &operator/=(const int &a);
    CSVector &operator%=(const CFriction &f);
};

/** Three-component fixed-point vector. */
struct CVector {
    int vx, vy, vz;

    CVector() {}
    CVector(int x, int y, int z) : vx(x), vy(y), vz(z) {}

    int Length() const;
    void Normalize();
    void KillSmall();
    CVector &operator+=(const CVector &a);
    CVector &operator-=(const CVector &a);
    CVector &operator*=(const int &a);
    CVector &operator/=(const int &a);
    CVector &operator>>=(const int &a);
    CVector &operator<<=(const int &a);
    CVector &operator%=(const CFriction &f);
    int operator==(const CVector &v);
    int operator!=(const CVector &v);
    CVector operator-() const;
};

// === Free function declarations ===
CVector operator+(const CVector &a, const CVector &b);
CVector operator-(const CVector &a, const CVector &b);
CVector operator*(const CVector &a, const int &scalar);
CVector operator*(const int &scalar, const CVector &a);
CVector operator/(const CVector &a, const int &scalar);
CVector operator>>(const CVector &a, const int &scalar);
CVector operator<<(const CVector &a, const int &scalar);
CSVector operator-(const CSVector &a, const CSVector &b);
CSVector operator*(const int &scalar, const CSVector &a);
CSVector operator>>(const CSVector &a, const int &scalar);

// === External helper declarations ===
#include "M3DMATHS.h"

// === Function implementations (address order) ===

/**
 * Computes the vector magnitude using the GTE SQR command.
 *
 * @return Magnitude of the vector after converting components from 12-bit
 *         fixed point for the GTE square operation.
 */
int CVector::Length() const {
    CVector u;
    int xSquared;
    int ySquared;
    int zSquared;

    // The target interleaves construction of u with the GTE transfers.  Keep
    // that sequence together, but expose its inputs, outputs, and memory use
    // to the compiler instead of binding C++ locals to fixed registers.
    asm(
        "lw $2, 0(%3)\n\t"
        "nop\n\t"
        "sra $2, $2, 12\n\t"
        "sw $2, %4\n\t"
        "addiu $2, $sp, 16\n\t"
        "lw %0, 4(%3)\n\t"
        "lw %1, 8(%3)\n\t"
        "sra %0, %0, 12\n\t"
        "sra %1, %1, 12\n\t"
        "sw %0, %5\n\t"
        "sw %1, %6\n\t"
        "lwc2 $9, 0($2)\n\t"
        "lwc2 $10, 4($2)\n\t"
        "lwc2 $11, 8($2)\n\t"
        "nop\n\t"
        "nop\n\t"
        ".word 0x4AA00428\n\t"
        "swc2 $25, 0($2)\n\t"
        "swc2 $26, 4($2)\n\t"
        "swc2 $27, 8($2)\n\t"
        "lw %2, %4\n\t"
        "lw %0, %5\n\t"
        "lw %1, %6\n\t"
        : "=r"(ySquared), "=r"(zSquared), "=r"(xSquared)
        : "r"(this), "m"(u.vx), "m"(u.vy), "m"(u.vz)
        : "$2", "memory"
    );
    xSquared += ySquared;
    return M3dMaths_SquareRoot0(xSquared + zSquared);
}


/**
 * Scales the X and Y components by the vector length, then applies the same
 * scale once more to the normalized Y value for Z, matching the prototype.
 */
void CVector::Normalize() {
    int length = Length();
    int normalizedY = vy / length;
    int normalizedX = vx / length;
    int normalizedZ = normalizedY / length;
    vx = normalizedX;
    vy = normalizedY;
    vz = normalizedZ;
}


/**
 * Clears components whose magnitude is at most one half-unit in 12-bit fixed point.
 */
void CVector::KillSmall() {
    if ((unsigned)(vx + 0x800) < 0x1001u) vx = 0;
    if ((unsigned)(vy + 0x800) < 0x1001u) vy = 0;
    if ((unsigned)(vz + 0x800) < 0x1001u) vz = 0;
}


/**
 * Adds another vector component by component.
 *
 * @param a Vector to add.
 * @return This vector after addition.
 */
CVector &CVector::operator+=(const CVector &a) {
    vx += a.vx;
    vy += a.vy;
    vz += a.vz;
    return *this;
}


/**
 * Subtracts another vector component by component.
 *
 * @param a Vector to subtract.
 * @return This vector after subtraction.
 */
CVector &CVector::operator-=(const CVector &a) {
    vx -= a.vx;
    vy -= a.vy;
    vz -= a.vz;
    return *this;
}


/**
 * Multiplies every component by a scalar.
 *
 * @param a Scalar multiplier.
 * @return This vector after multiplication.
 */
CVector &CVector::operator*=(const int &a) {
    vx *= a;
    vy *= a;
    vz *= a;
    return *this;
}


/**
 * Divides each fixed-point component by a shared scalar.
 *
 * @param a Divisor applied to all components.
 * @return This vector after division.
 */
CVector &CVector::operator/=(const int &a) {
    vx /= a;
    vy /= a;
    vz /= a;
    return *this;
}


/**
 * Shifts every component right by the same amount.
 *
 * @param a Number of bits to shift.
 * @return This vector after shifting.
 */
CVector &CVector::operator>>=(const int &a) {
    vx >>= a;
    vy >>= a;
    vz >>= a;
    return *this;
}


/**
 * Shifts every component left by the same amount.
 *
 * @param a Number of bits to shift.
 * @return This vector after shifting.
 */
CVector &CVector::operator<<=(const int &a) {
    vx <<= a;
    vy <<= a;
    vz <<= a;
    return *this;
}


/**
 * Applies a component-specific binary friction step.
 *
 * @param f Per-axis friction shifts.
 * @return This vector after friction is applied.
 */
CVector &CVector::operator%=(const CFriction &f) {
    vx -= vx >> f.vx;
    vy -= vy >> f.vy;
    vz -= vz >> f.vz;
    return *this;
}


/** Returns whether every component equals the corresponding component of v. */
int CVector::operator==(const CVector &v) {
    return (vx == v.vx && vy == v.vy && vz == v.vz);
}


/** Returns whether any component differs from the corresponding component of v. */
int CVector::operator!=(const CVector &v) {
    return (vx != v.vx || vy != v.vy || vz != v.vz);
}


/** Returns the component-wise sum of two vectors. */
CVector operator+(const CVector &a, const CVector &b) {
    CVector ans;
    ans.vx = a.vx + b.vx;
    ans.vy = a.vy + b.vy;
    ans.vz = a.vz + b.vz;
    return ans;
}


/** Returns the component-wise difference of two vectors. */
CVector operator-(const CVector &a, const CVector &b) {
    CVector ans;
    ans.vx = a.vx - b.vx;
    ans.vy = a.vy - b.vy;
    ans.vz = a.vz - b.vz;
    return ans;
}


/** Returns a vector with every component multiplied by scalar. */
CVector operator*(const CVector &a, const int &scalar) {
    CVector ans;
    ans.vx = a.vx * scalar;
    ans.vy = a.vy * scalar;
    ans.vz = a.vz * scalar;
    return ans;
}


/** Returns a vector with every component multiplied by scalar. */
CVector operator*(const int &scalar, const CVector &a) {
    CVector ans;
    ans.vx = a.vx * scalar;
    ans.vy = a.vy * scalar;
    ans.vz = a.vz * scalar;
    return ans;
}


/** Returns a vector with every component divided by scalar. */
CVector operator/(const CVector &a, const int &scalar) {
    CVector ans;
    ans.vx = a.vx / scalar;
    ans.vy = a.vy / scalar;
    ans.vz = a.vz / scalar;
    return ans;
}


/** Returns a vector with every component shifted right by scalar bits. */
CVector operator>>(const CVector &a, const int &scalar) {
    CVector ans;
    ans.vx = a.vx >> scalar;
    ans.vy = a.vy >> scalar;
    ans.vz = a.vz >> scalar;
    return ans;
}


/** Returns a vector with every component shifted left by scalar bits. */
CVector operator<<(const CVector &a, const int &scalar) {
    CVector ans;
    ans.vx = a.vx << scalar;
    ans.vy = a.vy << scalar;
    ans.vz = a.vz << scalar;
    return ans;
}


/**
 * Negates every component.
 *
 * @return A vector with every component negated.
 */
CVector CVector::operator-() const {
    return CVector(-vx, -vy, -vz);
}


/**
 * Restricts each angle component to its low 12 bits.
 */
void CSVector::Mask() {
    unsigned short x = vx;
    unsigned short y = vy;
    unsigned short z = vz;
    vx = x & 0xFFF;
    vy = y & 0xFFF;
    vz = z & 0xFFF;
}


/**
 * Clears components in the inclusive range [-1, 1].
 */
void CSVector::KillSmall() {
    if ((u16)(vx + 1) < 3u) vx = 0;
    if ((u16)(vy + 1) < 3u) vy = 0;
    if ((u16)(vz + 1) < 3u) vz = 0;
}


/**
 * Adds another compact vector component by component.
 *
 * @param a Vector to add.
 * @return This vector after addition.
 */
CSVector &CSVector::operator+=(const CSVector &a) {
    vx += a.vx;
    vy += a.vy;
    vz += a.vz;
    return *this;
}


/**
 * Divides each compact-vector component by a shared scalar.
 *
 * @param a Divisor applied to all components.
 * @return This vector after division.
 */
CSVector &CSVector::operator/=(const int &a) {
    vx /= a;
    vy /= a;
    vz /= a;
    return *this;
}


/**
 * Applies a component-specific binary friction step.
 *
 * @param f Per-axis friction shifts.
 * @return This vector after friction is applied.
 */
CSVector &CSVector::operator%=(const CFriction &f) {
    vx -= vx >> f.vx;
    vy -= vy >> f.vy;
    vz -= vz >> f.vz;
    return *this;
}


/** Returns the component-wise difference of two compact vectors. */
CSVector operator-(const CSVector &a, const CSVector &b) {
    CSVector ans;
    ans.vx = a.vx - b.vx;
    ans.vy = a.vy - b.vy;
    ans.vz = a.vz - b.vz;
    return ans;
}


/** Returns a compact vector with every component multiplied by scalar. */
CSVector operator*(const int &scalar, const CSVector &a) {
    CSVector ans;
    ans.vx = a.vx * scalar;
    ans.vy = a.vy * scalar;
    ans.vz = a.vz * scalar;
    return ans;
}


/** Returns a compact vector with every component shifted right by scalar bits. */
CSVector operator>>(const CSVector &a, const int &scalar) {
    CSVector ans;
    ans.vx = a.vx >> scalar;
    ans.vy = a.vy >> scalar;
    ans.vz = a.vz >> scalar;
    return ans;
}
