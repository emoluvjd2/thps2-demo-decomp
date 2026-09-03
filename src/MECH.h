// Shared public interface for the MECH owner.
#ifndef MECH_INC_H
#define MECH_INC_H

struct CBruce;
struct CBruceCtorSlice;

/** Construct a skater in caller-provided gameplay storage. */
namespace CBruceConstructorABI
{
extern "C" CBruceCtorSlice *__6CBruce(CBruceCtorSlice *bruce);
}

/** One checksum/count entry in the skater's recent-event history. */
struct SHistory {
    int Checksum;
    unsigned char Count;
};

typedef char SHistory_Checksum_offset_must_be_0[
    ((unsigned long)&(((SHistory *)0)->Checksum) == 0) ? 1 : -1];
typedef char SHistory_Count_offset_must_be_4[
    ((unsigned long)&(((SHistory *)0)->Count) == 4) ? 1 : -1];
typedef char SHistory_size_must_be_8[
    (sizeof(SHistory) == 8) ? 1 : -1];

/** Five one-byte bonuses selected by the active skater board. */
struct BoardStats {
    char ollie;
    char speed;
    char air;
    char balance;
    char turning;
};

typedef char BoardStats_ollie_offset_must_be_0[
    ((unsigned long)&(((BoardStats *)0)->ollie) == 0) ? 1 : -1];
typedef char BoardStats_speed_offset_must_be_1[
    ((unsigned long)&(((BoardStats *)0)->speed) == 1) ? 1 : -1];
typedef char BoardStats_air_offset_must_be_2[
    ((unsigned long)&(((BoardStats *)0)->air) == 2) ? 1 : -1];
typedef char BoardStats_balance_offset_must_be_3[
    ((unsigned long)&(((BoardStats *)0)->balance) == 3) ? 1 : -1];
typedef char BoardStats_turning_offset_must_be_4[
    ((unsigned long)&(((BoardStats *)0)->turning) == 4) ? 1 : -1];
typedef char BoardStats_size_must_be_5[
    (sizeof(BoardStats) == 5) ? 1 : -1];

#ifdef MECH_OWNER_DECLARATIONS
#define MECH_SHARED_STATE_DECL_ATTR
#else
#define MECH_SHARED_STATE_DECL_ATTR __attribute__((section(".data")))
#endif

/** Active skater used by script and collision systems. */
extern "C" CBruce *pCurrentPlayer MECH_SHARED_STATE_DECL_ATTR; // 0x800D9B08

#undef MECH_SHARED_STATE_DECL_ATTR

/** Play the context-sensitive grunt selected from a skater's current state. */
void SFX_Grunt(CBruce *pSkater);

#endif // MECH_INC_H

#ifdef MECH_DECLARE_PHYSICS_OWNER_ABI
#ifndef MECH_PHYSICS_OWNER_RECEIVER
#error MECH_DECLARE_PHYSICS_OWNER_ABI requires a receiver type
#endif
namespace PhysicsOwnerAbi
{
/** Transition the owner-local skater view to a new physics state. */
extern "C" void StateTransition__6CBruce13EPhysicsStatei(
    MECH_PHYSICS_OWNER_RECEIVER *bruce, int state, int line);
/** Run side-collision handling through the target CBruce entry. */
extern "C" void CheckSideCollisions__6CBruce(
    MECH_PHYSICS_OWNER_RECEIVER *bruce);
}
#undef MECH_PHYSICS_OWNER_RECEIVER
#undef MECH_DECLARE_PHYSICS_OWNER_ABI
#endif
