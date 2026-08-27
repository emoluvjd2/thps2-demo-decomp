// Shared trick-history entry layout.
#ifndef CTRICK_INC_H
#define CTRICK_INC_H

/** One trick-history entry queued by the player panel. */
struct CTrick {
    char *name;
    int point_value;
    int countdown;
    int spins;
    int locked;
    int depreciate;
    int count;
    int switched;
    struct Message *message;

    /** Initialize this queued trick and reset its transient scoring state. */
    void Set(struct Message *message, char *name, int points);
};

#endif // CTRICK_INC_H
