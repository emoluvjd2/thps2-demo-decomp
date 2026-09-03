/*
 * Re-entrant destructor ABI selector.  Owner translation units retain their
 * target-proven receiver views while sharing the single CBody ABI label.
 */
#if defined(CBODY_DECLARE_DESTRUCTOR)
#if !defined(CBODY_DESTRUCTOR_NAME) || !defined(CBODY_DESTRUCTOR_RECEIVER)
#error CBODY_DECLARE_DESTRUCTOR requires a name and receiver type
#endif
void CBODY_DESTRUCTOR_NAME(CBODY_DESTRUCTOR_RECEIVER *body, int inCharge)
    asm("_._5CBody");
#undef CBODY_DESTRUCTOR_RECEIVER
#undef CBODY_DESTRUCTOR_NAME
#undef CBODY_DECLARE_DESTRUCTOR
#endif

#ifndef CBODY_ABI_ONLY
#ifndef THPS2_CBODY_H
#define THPS2_CBODY_H

/**
 * Pointer-only natural interface for ordinary services owned by OB.cpp.
 * Concrete CBody storage remains in each translation unit's proven layout.
 */
struct CVector;

struct CBody {
    /** Initialize a body with the target's default item and physics state. */
    CBody();
    /** Mark this body dead. */
    void Die();
    /** Run the base body's AI behavior. */
    void AI();
    /** Run this body's per-frame behavior. */
    void EveryFrame();
    /** Interleave expensive AI samples across frames. */
    void InterleaveAI();
    /** Configure this body's lighting state. */
    void DoLightingSetup();
    /** Apply a hit and report whether it was accepted. */
    int Hit(int damage, const CVector &knock, int whatHit);
    /** Release body-owned resources. */
    void DeleteStuff();
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
#endif // !CBODY_ABI_ONLY
