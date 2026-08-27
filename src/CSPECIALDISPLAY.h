// Shared declaration of the special-display transient-bit base.
#ifndef CSPECIALDISPLAY_INC_H
#define CSPECIALDISPLAY_INC_H

/** A transient bit whose default display hook emits no geometry. */
struct CSpecialDisplay : CBit {
    /** Construct and attach a special-display bit. */
    CSpecialDisplay();
    /** Detach and destroy a special-display bit. */
    ~CSpecialDisplay();
    /** Default display hook. */
    void Display();
    /** Release special-display storage through the CBit allocator. */
    static void operator delete(void *p) { CBit::operator delete(p); }
};

#endif // CSPECIALDISPLAY_INC_H
