#ifndef ZEROVECTOR_INC_H
#define ZEROVECTOR_INC_H

struct CVector;

/**
 * Shared zero vector physically owned by OB.cpp.
 * Full-vector consumers form the target-recorded absolute address.
 */
extern "C" CVector ZeroVector __attribute__((section(".data")));

#endif // ZEROVECTOR_INC_H
