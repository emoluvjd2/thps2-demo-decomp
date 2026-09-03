#ifndef CVECTOROPS_H
#define CVECTOROPS_H

struct CVector;
struct CSVector;

/** Return the component-wise sum of two fixed-point vectors. */
CVector operator+(const CVector &left, const CVector &right);

/** Return the component-wise difference of two fixed-point vectors. */
CVector operator-(const CVector &left, const CVector &right);

/** Scale a fixed-point vector by an integer. */
CVector operator*(const CVector &vector, const int &scale);

/** Scale a fixed-point vector by an integer with the scalar first. */
CVector operator*(const int &scale, const CVector &vector);

#ifndef CVECTOROPS_OMIT_DIVIDE
/** Divide every fixed-point vector component by an integer. */
CVector operator/(const CVector &vector, const int &divisor);
#endif

/** Shift every fixed-point vector component arithmetically right. */
CVector operator>>(const CVector &vector, const int &shift);

/** Shift every fixed-point vector component left. */
CVector operator<<(const CVector &vector, const int &shift);

/** Return the component-wise difference of two packed short vectors. */
CSVector operator-(const CSVector &left, const CSVector &right);

/** Scale a packed short vector by an integer. */
CSVector operator*(const int &scale, const CSVector &vector);

/** Shift every packed short-vector component arithmetically right. */
CSVector operator>>(const CSVector &vector, const int &shift);

#endif // CVECTOROPS_H
