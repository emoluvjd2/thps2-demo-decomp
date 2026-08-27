#ifndef PHYSICS_INC_H
#define PHYSICS_INC_H

#ifdef PHYSICS_OWNER_DECLARATIONS
#define PHYSICS_SHARED_SCORE_DECL_ATTR
#else
#define PHYSICS_SHARED_SCORE_DECL_ATTR __attribute__((section(".data")))
#endif

/** Player-one multiplayer results. */
extern "C" int GWins1 PHYSICS_SHARED_SCORE_DECL_ATTR; // 0x800D7D48
extern "C" int GLosses1 PHYSICS_SHARED_SCORE_DECL_ATTR; // 0x800D7D4C
extern "C" int GStreak1 PHYSICS_SHARED_SCORE_DECL_ATTR; // 0x800D7D50
extern "C" int GHighTags1 PHYSICS_SHARED_SCORE_DECL_ATTR; // 0x800D7D54

/** Player-two multiplayer results. */
extern "C" int GWins2 PHYSICS_SHARED_SCORE_DECL_ATTR; // 0x800D7D58
extern "C" int GLosses2 PHYSICS_SHARED_SCORE_DECL_ATTR; // 0x800D7D5C
extern "C" int GStreak2 PHYSICS_SHARED_SCORE_DECL_ATTR; // 0x800D7D60
extern "C" int GHighTags2 PHYSICS_SHARED_SCORE_DECL_ATTR; // 0x800D7D64

#undef PHYSICS_SHARED_SCORE_DECL_ATTR

#endif // PHYSICS_INC_H
