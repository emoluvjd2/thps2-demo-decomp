// Shared declaration for flat-colour polyline effects.
#ifndef CPOLYLINE_INC_H
#define CPOLYLINE_INC_H

/** Flat-colour line segments carried by the transient-bit system. */
struct CPolyLine : CBit {
    unsigned char mForcedPriority;
    int mNumSegs;
    struct SLineSeg *mSegs;
    struct CVector mStart;

    /** Allocate and initialize a line-segment array. */
    CPolyLine(int numSegments);
    /** Release the line-segment array and detach this effect. */
    ~CPolyLine();
    /** Enable semitransparency on every packed segment command. */
    void SetSemiTransparent();
};

#endif // CPOLYLINE_INC_H
