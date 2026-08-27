#ifndef CROWD_INC_H
#define CROWD_INC_H

/** Initialize the crowd-audio subsystem. */
void Crowd_Init(void);

/** Start the ambient crowd voice for the current level. */
void Crowd_Start(void);

/** Stop every active crowd voice. */
void Crowd_Stop(void);

/** Move one crowd-audio scalar toward its target without overshooting. */
int Crowd_MoveTowards(int &value, int target, int step);

/** Update the active crowd voice and boredom reactions. */
void Crowd_Update(void);

/** Play a positive crowd reaction for one score. */
void Crowd_Cheer(int score);

/** Play a crowd pain reaction for one impact speed. */
void Crowd_Oww(int speed);

/** Play a crowd surprise reaction for one score. */
void Crowd_Ahh(int score);

/** Play a negative crowd reaction at the requested volume. */
void Crowd_Boo(int volume);

#endif // CROWD_INC_H
