#ifndef VLC_BIT_INC_H
#define VLC_BIT_INC_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Builds the run/level VLC lookup table in the PlayStation scratchpad.
 */
void DecDCTvlcBuild3(void);

/**
 * Decodes a version 2 or version 3 movie bitstream into MDEC run/level words.
 *
 * Passing a non-null bitstream starts a frame. Passing null resumes a frame
 * whose output was split by the decoder's output-size limit.
 *
 * @param bitstream Compressed frame header and VLC data, or null to continue.
 * @param output Destination buffer used when starting a frame.
 * @return Zero when the frame is complete, or one when another call is needed.
 */
int DecDCTvlc3(const unsigned int *bitstream, unsigned int *output);

#ifdef __cplusplus
}
#endif

#endif
