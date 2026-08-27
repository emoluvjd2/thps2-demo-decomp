#ifndef CFRICTION_INC_H
#define CFRICTION_INC_H

/** Three component-specific binary friction shifts. */
struct CFriction {
    unsigned char vx;
    unsigned char vy;
    unsigned char vz;

#ifdef __cplusplus
    /** Leave all three shifts uninitialized. */
    CFriction() {}

    /** Construct three component-specific friction shifts. */
    CFriction(unsigned char x, unsigned char y, unsigned char z)
        : vx(x), vy(y), vz(z) {}

    /** Assign all three component-specific friction shifts. */
    void Set(unsigned char x, unsigned char y, unsigned char z)
    {
        vx = x;
        vy = y;
        vz = z;
    }

#endif
};

#endif // CFRICTION_INC_H
