// Shared declaration for history-backed ribbon effects.
#ifndef CRIBBON_INC_H
#define CRIBBON_INC_H

struct CRibbonBit;

/** History-backed chain of animated ribbon segments. */
struct CRibbon : CNonRenderedBit {
    int mNumBits;
    int mPointsPerBit;
    int mNumPoints;
    int mCurrent;
    CVector *mPoints;
    CRibbonBit **mBits;

    /** Construct a ribbon from position history and animated segments. */
    CRibbon(const CVector &pos, int numbits, int pointsperbit,
            char *pMiddleAnimName, char *pEndAnimName, int scale,
            int semitrans);
    /** Destroy the ribbon's animated segments and position history. */
    ~CRibbon();
    /** Apply one tint to every animated ribbon segment. */
    void SetTint(u8 r, u8 g, u8 b);
    /** Apply one scale to every animated ribbon segment. */
    void SetScale(int Scale);
    /** Advance the ribbon history to the supplied position. */
    void SetPos(CVector &pos);
    /** Release ribbon storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
};

#endif // CRIBBON_INC_H
