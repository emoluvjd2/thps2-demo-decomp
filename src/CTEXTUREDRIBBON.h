// Shared declaration of the textured-ribbon transient-bit type.
#ifndef CTEXTUREDRIBBON_INC_H
#define CTEXTUREDRIBBON_INC_H

struct Texture;
struct SRibbonPoint;

/** A special-display bit rendered as a chain of textured ribbon points. */
struct CTexturedRibbon : CSpecialDisplay {
    unsigned char mTrail;
    Texture *mpTex;
    unsigned char mDontDisplay;
    int mU;
    int mV;
    int mDv;
    int mWibble;
    int mNumPoints;
    SRibbonPoint *mpPoints;

    /** Construct a textured ribbon with the requested point count. */
    CTexturedRibbon(int NumPoints, int LeaveTrail);
    ~CTexturedRibbon();
    /** Bind the texture identified by the asset checksum. */
    void SetTexture(unsigned int Checksum);
    /** Render the ribbon into the current viewport ordering table. */
    void Display();
    /** Release textured-ribbon storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
};

#endif // CTEXTUREDRIBBON_INC_H
