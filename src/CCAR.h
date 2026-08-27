#ifndef CCAR_INC_H
#define CCAR_INC_H

/** Traffic car state extending the shared baddy layout. */
struct CCar : CBaddy {
    /** Initialize a traffic car from one trigger record. */
    CCar(short *pInfo, int nodeIndex, int type);

    int mCurrentRoadSegment;
    int mHonkLock;
};

#endif // CCAR_INC_H
