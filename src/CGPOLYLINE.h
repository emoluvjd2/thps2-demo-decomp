// Shared declaration for Gouraud-colour polyline effects.
#ifndef CGPOLYLINE_INC_H
#define CGPOLYLINE_INC_H

/** Gouraud-colour line segments carried by the transient-bit system. */
struct CGPolyLine : CBit {
    int mNumSegs;
    SLineSeg *mSegs;
    CVector mStart;
    unsigned char mStartR;
    unsigned char mStartG;
    unsigned char mStartB;
    unsigned char mCode;
    unsigned char mForcedPriority;

    /** Allocate and initialize a Gouraud line-segment array. */
    CGPolyLine(int numSegments);
    /** Release the line-segment array and detach this effect. */
    ~CGPolyLine();
    /** Enable semitransparency on all segments and the starting colour. */
    void SetSemiTransparent();
    /** Release polyline storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
};

#endif // CGPOLYLINE_INC_H
