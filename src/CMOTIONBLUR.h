// Shared declaration for the motion-blur particle.
#ifndef CMOTIONBLUR_INC_H
#define CMOTIONBLUR_INC_H

/** Flat textured particle used for motion-blur effects. */
struct CMotionBlur : CFlatBit {
    /** Destroy a motion-blur particle. */
    ~CMotionBlur();
    /** Advance the blur and apply its shrink/fade lifecycle. */
    void Move();
    /** Releases motion-blur storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
};

#endif // CMOTIONBLUR_INC_H
