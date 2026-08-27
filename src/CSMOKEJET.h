// Shared declaration of the textured smoke-jet effect.
#ifndef CSMOKEJET_INC_H
#define CSMOKEJET_INC_H

/** One smoke-ribbon width target and the time remaining to reach it. */
struct SWidthWobble {
    int TChange;
    int TargetWidth;
};

/** Ten-point textured ribbon that animates and damages intersecting bodies. */
struct CSmokeJet : CTexturedRibbon {
    SWidthWobble mWidthWobble[SMOKE_POINT_COUNT];
    int mOrgX[SMOKE_POINT_COUNT];
    int mPhase[SMOKE_POINT_COUNT];
    int mFreq[SMOKE_POINT_COUNT];
    int mAmp[SMOKE_POINT_COUNT];
    unsigned char mScrollSpeed;
    unsigned char mDamageBaddies;
    unsigned short mDamage;
    unsigned short mDamageTimer;
    CSVector mAngles;
    int mLength;

    /** Construct and initialize a textured smoke jet. */
    CSmokeJet(
        const CVector &BasePos, const CSVector &Angles, int Length,
        unsigned short Duration, int Damage, int r, int g, int b,
        unsigned char ScrollSpeed, unsigned int Checksum);
    ~CSmokeJet();
    /** Reposition every point along the smoke jet. */
    void SetPos(const CVector &Pos);
    /** Advance the smoke jet and apply collision-driven damage. */
    void Move();
    /** Release smoke-jet storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
};

#endif // CSMOKEJET_INC_H
