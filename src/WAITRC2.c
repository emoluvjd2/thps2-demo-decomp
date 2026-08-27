/*
 * WAITRC2.c -- Recovered legacy LIBPAD root-counter timeout owner.
 *
 * Original: \psx\lib\libpad.lib(WAITRC2.obj)
 * Text:     0x800AC158 - 0x800AC218
 * BSS:      _waitTime  0x800E660C
 *           _startTime 0x800E6610
 *
 * This source is not yet a production native-link input. GCC 2.8.1 reproduces
 * the target control flow, opcode stream, data layout, and relocation topology,
 * but the exact legacy compiler/assembler provenance remains unresolved:
 * chkRC2wait still differs only in temporary-register allocation, and the
 * installed ASPSX versions do not schedule setRC2wait's final store into the
 * return delay slot. Keep this owner out of the compatibility archive until
 * those two code-generation details are reproduced generically.
 */

typedef unsigned short u_short;
typedef unsigned long u_long;

/** Number of root-counter ticks requested by the active timeout. */
u_long _waitTime __attribute__((section(".bss")));

/** Root-counter-2 value captured when the active timeout began. */
u_long _startTime __attribute__((section(".bss")));

#define RC2_COUNT (*(volatile u_short *)0x1F801120)
#define RC2_MODE (*(volatile u_short *)0x1F801124)
#define RC2_TARGET (*(volatile u_short *)0x1F801128)

/**
 * Starts a timeout measured by root counter 2.
 *
 * @param waitTime Number of counter ticks to wait.
 */
void setRC2wait(u_long waitTime)
{
    _waitTime = waitTime;
    _startTime = RC2_COUNT;
}

/**
 * Checks whether the current root-counter-2 timeout has elapsed.
 *
 * Counter wrap uses the programmed RC2 target when one is active, otherwise
 * the ordinary 16-bit counter period. Counter mode bit 0x200 selects whether
 * the elapsed value is measured directly or converted from system-clock ticks.
 *
 * @return Nonzero after the requested duration has elapsed.
 */
int chkRC2wait(void)
{
    u_long now = RC2_COUNT;
    u_long wait;
    u_long elapsed;

    if (now < _startTime)
    {
        if (RC2_TARGET != 0)
        {
            now += RC2_TARGET;
        }
        else
        {
            now += 0x10000;
        }
    }

    if ((RC2_MODE & 0x200) == 0)
    {
        elapsed = (now - _startTime) >> 3;
        wait = _waitTime;
    }
    else
    {
        elapsed = now - _startTime;
        wait = _waitTime;
    }

    return elapsed >= wait;
}
