// Shared declaration of one oscillating radial glow.
#ifndef CWOBBLYGLOW_INC_H
#define CWOBBLYGLOW_INC_H

struct CVector;

/** Glow whose radial sections oscillate independently over time. */
struct CWobblyGlow : CGlow {
    int mInc[8];
    int mT[8];
    int mAmplitude;
    int mInnerRadius;

    /** Construct and seed an oscillating glow. */
    CWobblyGlow(
        const CVector &Pos, int InnerRadius, int FringeRadius, int Amp,
        unsigned char r0, unsigned char g0, unsigned char b0,
        unsigned char r1, unsigned char g1, unsigned char b1);
    /** Destroy the oscillating glow and its radial base. */
    ~CWobblyGlow();
    /** Advance every section's wobble phase and radius. */
    void Move();
};

#endif // CWOBBLYGLOW_INC_H
