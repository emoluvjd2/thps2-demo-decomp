#ifndef THPS2_CPOWERUP_H
#define THPS2_CPOWERUP_H

struct CBruce;

/**
 * Pointer-only natural interface for ordinary services owned by POWERUP.cpp.
 * Concrete CPowerUp storage remains in each owner's proven layout.
 */
struct CPowerUp {
    /** Set vertical acceleration and damping. */
    void SetGravity(int gravity, int friction);
    /** Associate this power-up with a trigger node. */
    void SetNode(int nodeIndex);
    /** Apply this pickup's effects to a skater. */
    void TakeEffect(CBruce *skater);
};

#endif // THPS2_CPOWERUP_H
