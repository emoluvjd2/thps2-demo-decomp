#ifndef THPS2_CSUPER_H
#define THPS2_CSUPER_H

/**
 * Pointer-only natural interface for ordinary animation services owned by
 * OB.cpp. Concrete CSuper storage remains in each owner's proven layout.
 */
struct CSuper {
    /** Start a bounded animation sequence. */
    void RunAnim(int anim, int from, int to, int to2);
    /** Start a looping animation in the requested playback direction. */
    void CycleAnim(int animation, signed char direction);
    /** Assign one model hierarchy slot to an animation hierarchy slot. */
    void SetAnimOrder(unsigned char modelId, unsigned char animOrder);
    /** Build the animation-order table by matching hierarchy names. */
    void CalculateAnimOrder(char **animHierarchy, int animPartCount,
                            char **modelHierarchy, int modelPartCount);
};

#endif // THPS2_CSUPER_H
