/*
 * Clean-room compatibility source for the two public PAD communication
 * wrappers that precede the SDK entry helpers in the target PADENTRY member.
 *
 * This experimental source is compiled only by
 * tools/build_padentry_compat.py.  It is not an application translation unit
 * and must remain at the LIBPAD archive encounter point after a compatible
 * PADMAIN owner is recovered.
 */

extern void _padStartCom(void);
extern void _padStopCom(void);

/**
 * Starts controller communication through the selected low-level PAD owner.
 */
void PadStartCom(void)
{
    _padStartCom();
}

/**
 * Stops controller communication through the selected low-level PAD owner.
 */
void PadStopCom(void)
{
    _padStopCom();
}
