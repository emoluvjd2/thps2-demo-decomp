// Public explosion and flat-ripple interfaces.
#ifndef EXP_INC_H
#define EXP_INC_H

struct Item;

/** Shatter an environmental item or its struck face after an explosion. */
void Exp_HitEnvItem(Item *pItem, unsigned int *pFace, int damage);

#endif // EXP_INC_H
