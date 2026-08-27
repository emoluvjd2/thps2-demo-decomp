// Public platform interface shared by trigger and skater systems.
#ifndef PLATFORM_INC_H
#define PLATFORM_INC_H

struct CBruce;
struct CPlatform;
struct CVector;

enum {
    PLATFORM_OBJECT_SIZE = 592
};

/** Check one skater against every active bouncing platform. */
void Platform_HandleBounce(CBruce *pSkater);

#endif // PLATFORM_INC_H
