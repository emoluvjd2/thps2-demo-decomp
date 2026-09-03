#ifndef CROWD_INC_H
#define CROWD_INC_H

#ifdef CROWD_OWNER_DECLARATIONS
/**
 * Pending crowd-reaction intensity consumed by the CROWD owner.
 *
 * The owner selector retains the original GP-relative external declaration
 * without exposing unused state through the public crowd API.
 */
extern int Crowd_Bored;
#endif

/** Initialize the crowd-audio subsystem. */
void Crowd_Init(void);

/** Start the ambient crowd voice for the current level. */
void Crowd_Start(void);

/** Stop every active crowd voice. */
void Crowd_Stop(void);

/** Update the active crowd voice and boredom reactions. */
void Crowd_Update(void);

/** Play a positive crowd reaction for one score. */
void Crowd_Cheer(int score);

/** Play a crowd pain reaction for one impact speed. */
void Crowd_Oww(int speed);

/** Play a crowd surprise reaction for one score. */
void Crowd_Ahh(int score);

#endif // CROWD_INC_H
