// Shared declaration for textured shatter chunks.
#ifndef CCHUNKBIT_INC_H
#define CCHUNKBIT_INC_H

/** Four-corner textured chunk in model-local and world space. */
struct CChunkBit : CBit {
    SVECTOR mPosA;
    SVECTOR mPosB;
    SVECTOR mPosC;
    SVECTOR mPosD;
    struct CVector mWorldPosA;
    struct CVector mWorldPosB;
    struct CVector mWorldPosC;
    struct CVector mWorldPosD;
    struct CSVector mAngles;
    unsigned int mClutV0U0;
    unsigned int mTPageV1U1;
    unsigned short mV2U2;
    unsigned short mV3U3;
    unsigned int mB0G0R0;
    unsigned int mB1G1R1;
    unsigned int mB2G2R2;
    unsigned int mB3G3R3;
    unsigned int mTexWin;

    /** Build a textured chunk from three triangle corners. */
    CChunkBit(const struct CSVector &a, const struct CSVector &b,
              const struct CSVector &c);
    /** Detach and destroy a textured chunk. */
    ~CChunkBit();
    /** Set the packed colours for all four chunk vertices. */
    void SetRGB(unsigned char r, unsigned char g, unsigned char b);
    /** Set the chunk texture page, CLUT, and UV coordinates. */
    void SetUVs(unsigned short clut, unsigned short tpage,
                unsigned char u0, unsigned char v0,
                unsigned char u1, unsigned char v1,
                unsigned char u2, unsigned char v2);
    /** Transform the four model-local corners into world space. */
    void CalculateWorldCoords();
    /** Release chunk storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
};

#endif // CCHUNKBIT_INC_H
