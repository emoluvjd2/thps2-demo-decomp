#ifndef THPS2_CFLATRIPPLE_H
#define THPS2_CFLATRIPPLE_H

#include "SLINESEG.h"
#include "CPOLYLINE.h"

/** Expanding flat-ripple effect state. */
struct CFlatRipple : CPolyLine
{
    unsigned char mR;
    unsigned char mG;
    unsigned char mB;
    int mMaxRadius;
    int mRadius;
    int mGrowRate;
    int mAngle;

    /** Build a flat ripple and initialize its ring segments. */
    CFlatRipple(
        const CVector &position, int maxRadius,
        unsigned char red, unsigned char green, unsigned char blue,
        int numSegments, int growRate);
    /** Restore the derived dispatch table before base destruction. */
    ~CFlatRipple();
    /** Expand, fade, and rebuild this ripple for one frame. */
    void Move();
};

#endif // THPS2_CFLATRIPPLE_H
