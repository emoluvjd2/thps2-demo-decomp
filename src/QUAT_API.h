#ifndef QUAT_API_INC_H
#define QUAT_API_INC_H

#include <LIBGTE.H>

struct CQuat;
struct CVector;

/** Compose two fixed-point quaternion rotations. */
CQuat operator*(const CQuat &left, const CQuat &right);

/** Build a unit quaternion from one direction vector. */
CQuat QFromV(const CVector &vector);

/** Build a quaternion for one rotation around the X axis. */
CQuat QFromXRot(short angle);

/** Build a quaternion for one rotation around the Y axis. */
CQuat QFromYRot(short angle);

/** Build a quaternion for one rotation around the Z axis. */
CQuat QFromZRot(short angle);

/** Convert one fixed-point quaternion to a rotation matrix. */
void QToM(const CQuat &quaternion, MATRIX &matrix);

/** Convert one rotation matrix to a fixed-point quaternion. */
void MToQ(const MATRIX &matrix, CQuat &quaternion);

/** Spherically interpolate between two fixed-point quaternions. */
void Quat_Slerp(
    CQuat begin, CQuat end, int time, CQuat &result);

#endif // QUAT_API_INC_H
