#ifndef THPS2_CBODY_H
#define THPS2_CBODY_H

/**
 * Pointer-only natural interface for ordinary services owned by OB.cpp.
 * Concrete CBody storage remains in each translation unit's proven layout.
 */
struct CBody {
    /** Initialize a body with the target's default item and physics state. */
    CBody();
    /** Insert this body at the head of a linked body list. */
    void AttachTo(CBody **list);
    /** Remove this body from a linked body list. */
    void DeleteFrom(CBody **list);
    /** Decode and store one packed position triple. */
    short *SquirtPos(short *info);
    /** Decode and store one packed angle triple. */
    short *SquirtAngles(short *info);
    /** Move this body from its live list to the suspended list. */
    void Suspend(CBody **originalList);
    /** Restore this body to its saved live list. */
    void UnSuspend();
    /** Enable shadow rendering for this body. */
    void ShadowOn();
    /** Destroy this body's shadow and clear its shadow flag. */
    void KillShadow();
    /** Rebuild or retire this body's shadow and refresh its opacity. */
    void UpdateShadow();
    /** Return whether this body has been marked dead. */
    int IsDead() const;
};

#endif // THPS2_CBODY_H
