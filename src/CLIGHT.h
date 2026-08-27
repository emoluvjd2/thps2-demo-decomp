// Shared declaration of one dynamic non-rendered light.
#ifndef CLIGHT_INC_H
#define CLIGHT_INC_H

struct CWobblyGlow;

/** Dynamic non-rendered light owning an optional wobbling glow. */
struct CLight : CNonRenderedBit {
    int mInnerRadius;
    int mFringeRadius;
    unsigned char mr0;
    unsigned char mg0;
    unsigned char mb0;
    unsigned char mr1;
    unsigned char mg1;
    unsigned char mb1;
    unsigned char mOn;
    int mAmp;
    int mClipDist;
    CWobblyGlow *mpGlow;

    /** Construct one dynamic light from its node and glow parameters. */
    CLight(const CVector &Pos, int Node, int ClipDist,
           int InnerRadius, int FringeRadius, int Amp,
           unsigned char r0, unsigned char g0, unsigned char b0,
           unsigned char r1, unsigned char g1, unsigned char b1);
    /** Destroy the owned glow and the non-rendered-light base. */
    ~CLight();
    /** Release light storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
    /** Update visibility and the owned glow. */
    void Move();
};

#endif // CLIGHT_INC_H
