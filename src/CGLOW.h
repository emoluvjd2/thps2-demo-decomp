// Shared declaration of one radial glow effect.
#ifndef CGLOW_INC_H
#define CGLOW_INC_H

struct CVector;
struct SSection;
struct SFringeQuad;

/** Radial glow geometry and colour state. */
struct CGlow : CBit {
    SSection *mpSections;
    SFringeQuad *mpFringes;
    unsigned int mNumSections;
    unsigned int mNumFringes;
    unsigned int mCentreCodeBGR;
    short mStepAngle;
    unsigned char mSkipTriangles;
    short mAngle;
    unsigned int mMask;

    /** Construct a glow with caller-selected section and fringe counts. */
    CGlow(unsigned int NumPoints, unsigned int NumFringes);
    /** Construct a fixed glow from its position, radii, and colours. */
    CGlow(const CVector &Pos, int InnerRadius, int FringeRadius,
          unsigned char r0, unsigned char g0, unsigned char b0,
          unsigned char r1, unsigned char g1, unsigned char b1);
    /** Destroy the allocated geometry and unlink the glow. */
    ~CGlow();
    /** Update a plain glow; the base implementation is empty. */
    void Move();
    /** Set every radial-section radius. */
    void SetRadius(int Radius);
    /** Set every segment width in one fringe ring. */
    void SetFringeWidth(unsigned int Fringe, unsigned int Width);
    /** Set the packed centre colour. */
    void SetCentreRGB(unsigned char r, unsigned char g, unsigned char b);
    /** Set every radial-section colour. */
    void SetRGB(unsigned char r, unsigned char g, unsigned char b);
    /** Set every segment colour in one fringe ring. */
    void SetFringeRGB(
        unsigned int Fringe, unsigned char r,
        unsigned char g, unsigned char b);
    /** Set one radial spoke and its corresponding fringe colours. */
    void SetRadialRGB(
        int Spoke, unsigned char r, unsigned char g, unsigned char b);
    /** Release glow storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
};

#endif // CGLOW_INC_H
