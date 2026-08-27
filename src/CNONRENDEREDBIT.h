// Shared declaration of the non-rendered transient-bit base.
#ifndef CNONRENDEREDBIT_INC_H
#define CNONRENDEREDBIT_INC_H

/** A transient bit effect with no render payload. */
struct CNonRenderedBit : CBit {
    /** Construct and attach a non-rendered transient bit. */
    CNonRenderedBit();
    /** Detach and destroy a non-rendered transient bit. */
    ~CNonRenderedBit();
    /** Release non-rendered storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
};

#endif // CNONRENDEREDBIT_INC_H
