// VLC_BIT.cpp -- semantic C++ reference for the movie player's VLC decoder.
// Original: SCEE's hand-written version 1.4 bit decoder, linked locally as
// vlc_bit.obj. This is an independent clean-C++ recovery; the canonical link
// object uses assembly reconstructed from this target's own instruction words.
// The historical external source is provenance evidence only and is not copied.
// Address range: 0x8007E87C - 0x8007EDD0

#include "VLC_BIT.h"

typedef unsigned char VlcByte;
typedef unsigned short VlcHalfword;
typedef unsigned int VlcWord;

/** A positive MPEG run/level pair stored in the compressed lookup table. */
struct VlcRunLevelPair {
    VlcByte run;
    VlcByte level;
};

/** State retained when decoding has to continue in another output buffer. */
struct VlcContinuationState {
    VlcWord window;
    VlcWord nextWindow;
    VlcWord currentCode;
    const VlcWord *input;
    VlcHalfword *output;
    VlcWord pendingWord;
    VlcWord bitsConsumed;
    VlcWord quantScale;
    VlcHalfword *outputEnd;
    int lastCr;
    int lastCb;
    int lastY;
    VlcWord resumeToken;
};

/**
 * Data contributed by vlc_bit.obj.
 *
 * The original Build3 loop expands 109 pairs. Its final pair deliberately
 * aliases the low halfword of maxOutputHalfwords, so the initialized bytes are
 * shared without duplicating storage.
 */
struct VlcOwnedDataFields {
    VlcRunLevelPair runLevelPairs[108];
    VlcWord maxOutputHalfwords;
    VlcContinuationState continuation;
};

/** Views the final Build3 pair through the overlapping maximum-output word. */
union VlcOwnedData {
    VlcOwnedDataFields fields;
    VlcRunLevelPair buildPairs[109];
};

typedef char VlcPairSizeCheck[(sizeof(VlcRunLevelPair) == 2) ? 1 : -1];

/*
 * The pair order is the order expected by the decoder's scratchpad offsets.
 * DecDCTvlcBuild3 expands each entry into positive and negative MDEC words.
 */
static VlcOwnedData s_vlcData = {
    {
        {
            /* Prefix 0001. */
            { 7, 1}, { 6, 1}, { 1, 2}, { 5, 1},
            /* Prefix 00001. */
            { 2, 2}, { 9, 1}, { 0, 4}, { 8, 1},
            /* Prefix 0000001. */
            {16, 1}, { 5, 2}, { 0, 7}, { 2, 3},
            { 1, 4}, {15, 1}, {14, 1}, { 4, 2},
            /* Prefix 00000001. */
            { 0,11}, { 8, 2}, { 4, 3}, { 0,10},
            { 2, 4}, { 7, 2}, {21, 1}, {20, 1},
            { 0, 9}, {19, 1}, {18, 1}, { 1, 5},
            { 3, 3}, { 0, 8}, { 6, 2}, {17, 1},
            /* Prefix 000000001. */
            {10, 2}, { 9, 2}, { 5, 3}, { 3, 4},
            { 2, 5}, { 1, 7}, { 1, 6}, { 0,15},
            { 0,14}, { 0,13}, { 0,12}, {26, 1},
            {25, 1}, {24, 1}, {23, 1}, {22, 1},
            /* Prefix 0000000001. */
            { 0,31}, { 0,30}, { 0,29}, { 0,28},
            { 0,27}, { 0,26}, { 0,25}, { 0,24},
            { 0,23}, { 0,22}, { 0,21}, { 0,20},
            { 0,19}, { 0,18}, { 0,17}, { 0,16},
            /* Prefix 00000000001. */
            { 0,40}, { 0,39}, { 0,38}, { 0,37},
            { 0,36}, { 0,35}, { 0,34}, { 0,33},
            { 0,32}, { 1,14}, { 1,13}, { 1,12},
            { 1,11}, { 1,10}, { 1, 9}, { 1, 8},
            /* Prefix 000000000001. */
            { 1,18}, { 1,17}, { 1,16}, { 1,15},
            { 6, 3}, {16, 2}, {15, 2}, {14, 2},
            {13, 2}, {12, 2}, {11, 2}, {31, 1},
            {30, 1}, {29, 1}, {28, 1}, {27, 1},
            /* Nine-bit forms beginning with 00100. */
            {13, 1}, { 0, 6}, {12, 1}, {11, 1},
            { 3, 2}, { 1, 3}, { 0, 5}, {10, 1},
            /* Six-bit forms beginning with 001; final entry is padding. */
            { 0, 3}, { 4, 1}, { 3, 1}, { 0, 0}
        },
        0x00ffffff,
        {
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0
        }
    }
};
/** Builds the signed run/level lookup table in scratchpad RAM. */
extern "C" void DecDCTvlcBuild3(void) {
    volatile VlcHalfword *output = (VlcHalfword *)0x1f800000;
    VlcRunLevelPair *input = s_vlcData.buildPairs;
    int pairsRemaining = 109;
    do {
        VlcWord runBits = (VlcWord)input->run << 10;
        VlcWord level = input->level;
        output[0] = (VlcHalfword)(runBits | level);
        output[1] = (VlcHalfword)(runBits | ((-level) & 0x03ff));
        input++;
        output += 2;
    } while (--pairsRemaining != 0);
}

/**
 * Decodes version 2 and version 3 data with one shared AC loop.
 *
 * This expresses the target's shared internal AC coroutine as an ordinary
 * block-state loop, keeping all decoder ownership inside the public routine.
 *
 * @param bitstream Compressed frame header/data, or null to resume.
 * @param destination Output buffer for a new decode; ignored when resuming.
 * @return Zero when decoding is complete, or one when continuation is needed.
 */
extern "C" int DecDCTvlc3(
    const VlcWord *bitstream,
    VlcWord *destination
) {
    VlcWord window;
    VlcWord nextWindow;
    VlcWord pendingWord;
    const VlcWord *input;
    VlcHalfword *output;
    VlcHalfword *outputEnd;
    VlcHalfword *chunkEnd;
    VlcWord bitsConsumed;
    VlcWord quantScale;
    int version2;
    int lastCr;
    int lastCb;
    int lastY;
    int block = 0;

    /* A non-null stream starts a frame; null restores the private coroutine. */
    if (bitstream != 0) {
        VlcWord header = bitstream[0];
        VlcWord format = bitstream[1];
        *destination++ = header;
        output = (VlcHalfword *)destination;
        outputEnd = output + ((header & 0xffff) * 2);
        chunkEnd = ((VlcHalfword *)(destination - 1)) +
                   s_vlcData.fields.maxOutputHalfwords;
        window = (bitstream[2] << 16) | (bitstream[2] >> 16);
        nextWindow = (bitstream[3] << 16) | (bitstream[3] >> 16);
        pendingWord = bitstream[4];
        input = bitstream + 5;
        bitsConsumed = 0;
        quantScale = (format & 0x3f) << 10;
        version2 = (format >> 16) != 3;
        lastCr = 0;
        lastCb = 0;
        lastY = 0;
    } else {
        VlcContinuationState &state = s_vlcData.fields.continuation;
        window = state.window;
        nextWindow = state.nextWindow;
        input = state.input;
        output = state.output;
        pendingWord = state.pendingWord;
        bitsConsumed = state.bitsConsumed;
        quantScale = state.quantScale;
        outputEnd = state.outputEnd;
        chunkEnd = output + s_vlcData.fields.maxOutputHalfwords;
        lastCr = state.lastCr;
        lastCb = state.lastCb;
        lastY = state.lastY;
        version2 = state.resumeToken != 0;

        VlcWord marker = window >> 22;
        if ((!version2 && marker == 0x03ff) ||
            (version2 && marker == 0x01ff)) {
            goto complete;
        }
    }

    for (;;) {
        VlcWord codeValue;
        VlcWord codeLength;
        int endOfBlock = 0;
        if (!version2) {
            /* Version 3 carries differential DC values in Cr, Cb, then Y order. */
            VlcWord size;
            VlcWord prefixLength;
            int delta;
            int *predictor;
            int chroma = block < 2;
            if (block == 0) predictor = &lastCr;
            else if (block < 2) predictor = &lastCb;
            else predictor = &lastY;

            if (chroma) {
                VlcWord prefix = window >> 30;
                if (prefix != 3) {
                    size = prefix;
                    prefixLength = 2;
                } else {
                    VlcWord scan = ~window;
                    VlcWord leadingOnes = 0;
                    while (scan < 0x80000000U && leadingOnes != 32) {
                        scan <<= 1;
                        leadingOnes++;
                    }
                    size = leadingOnes + 1;
                    prefixLength = leadingOnes + 1;
                }
            } else {
                VlcWord prefix = window >> 30;
                if (prefix == 0) {
                    size = 1;
                    prefixLength = 2;
                } else if (prefix == 1) {
                    size = 2;
                    prefixLength = 2;
                } else {
                    prefix = window >> 29;
                    /* Only the four three-bit prefixes 4..7 remain here. */
                    if (prefix < 5) {
                        size = 0;
                        prefixLength = 3;
                    } else if (prefix < 6) {
                        size = 3;
                        prefixLength = 3;
                    } else {
                        VlcWord scan = ~window;
                        VlcWord leadingOnes = 0;
                        while (scan < 0x80000000U && leadingOnes != 32) {
                            scan <<= 1;
                            leadingOnes++;
                        }
                        size = leadingOnes + 2;
                        prefixLength = leadingOnes + 1;
                    }
                }
            }

            if (size == 0) {
                delta = 0;
            } else {
                VlcWord amplitude =
                    (window << prefixLength) >> (32 - size);
                if ((amplitude & (1 << (size - 1))) == 0)
                    delta = (int)amplitude - (int)((1 << size) - 1);
                else
                    delta = (int)amplitude;
            }
            *predictor += delta;
            codeValue = quantScale | (((VlcWord)*predictor * 4) & 0x03ff);
            codeLength = prefixLength + size;
        } else {
            codeValue = quantScale | (window >> 22);
            codeLength = 10;
        }

        /* Emit that DC value, then every AC value through the block marker. */
        for (;;) {
            *output++ = (VlcHalfword)codeValue;
            window = (window << codeLength) |
                     (nextWindow >> (32 - codeLength));
            nextWindow <<= codeLength;
            bitsConsumed += codeLength;
            if (bitsConsumed >= 32) {
                bitsConsumed -= 32;
                VlcWord next = (pendingWord << 16) | (pendingWord >> 16);
                if (bitsConsumed != 0)
                    window |= next >> (32 - bitsConsumed);
                nextWindow = next << bitsConsumed;
                pendingWord = *input++;
            }
            if (endOfBlock)
                break;

            VlcWord value = window;
            endOfBlock = 0;
            /* Signed tests consume the next high-order bit on 32-bit MIPS. */
            if ((int)value < 0) {
                if ((int)(value << 1) >= 0) {
                    codeValue = 0xfe00;
                    codeLength = 2;
                    endOfBlock = 1;
                } else {
                    codeValue = (int)(value << 2) < 0 ? 0x03ff : 1;
                    codeLength = 3;
                }
            } else if ((int)(value << 1) < 0) {
                if ((int)(value << 2) < 0) {
                    codeValue = (int)(value << 3) < 0 ? 0x07ff : 0x0401;
                    codeLength = 4;
                } else {
                    if ((int)(value << 3) >= 0)
                        codeValue = (int)(value << 4) >= 0 ? 2 : 0x03fe;
                    else
                        codeValue = (int)(value << 4) >= 0 ? 0x0801 : 0x0bff;
                    codeLength = 5;
                }
            } else if ((int)(value << 2) < 0) {
                volatile VlcByte *scratch = (VlcByte *)0x1f800000;
                if (((value << 3) >> 30) == 0) {
                    codeValue = *(volatile VlcHalfword *)(
                        scratch + 0x180 + ((value >> 22) & 0x1e));
                    codeLength = 9;
                } else {
                    codeValue = *(volatile VlcHalfword *)(
                        scratch + 0x19c + ((value >> 25) & 0x0e));
                    codeLength = 6;
                }
            } else {
                VlcWord scan = value;
                VlcWord leadingZeroes = 0;
                while (scan < 0x80000000U && leadingZeroes != 32) {
                    scan <<= 1;
                    leadingZeroes++;
                }
                if (leadingZeroes == 5) {
                    codeValue = (value >> 10) & 0xffff;
                    codeLength = 22;
                } else {
                    volatile VlcByte *scratch =
                        (VlcByte *)0x1f800000;
                    VlcWord offset;
                    if (leadingZeroes < 5) {
                        offset = (leadingZeroes - 3) * 0x10;
                        offset += (scan >> 27) & 0x0e;
                        codeLength = leadingZeroes + 4;
                    } else if (leadingZeroes <= 6) {
                        offset = 0x20 + ((scan >> 26) & 0x1e);
                        codeLength = 11;
                    } else {
                        offset = (leadingZeroes - 6) * 0x40;
                        offset += (scan >> 25) & 0x3e;
                        codeLength = leadingZeroes + 6;
                    }
                    codeValue = *(volatile VlcHalfword *)(scratch + offset);
                }
            }
        }

        if (!version2) {
            block++;
            if (block < 6)
                continue;
            block = 0;
            if (output >= chunkEnd)
                break;
            if ((window >> 22) == 0x03ff)
                goto complete;
        } else {
            if ((window >> 22) == 0x01ff)
                goto complete;
            if (output >= chunkEnd)
                break;
        }
    }

    /* Preserve the exact private layout consumed by a null-input resume call. */
    {
        VlcContinuationState &state = s_vlcData.fields.continuation;
        state.window = window;
        state.nextWindow = nextWindow;
        state.currentCode = window >> 22;
        if (version2)
            state.currentCode |= quantScale;
        state.input = input;
        state.output = output;
        state.pendingWord = pendingWord;
        state.bitsConsumed = bitsConsumed;
        state.quantScale = quantScale;
        state.outputEnd = outputEnd;
        state.lastCr = lastCr;
        state.lastCb = lastCb;
        state.lastY = lastY;
        state.resumeToken = (VlcWord)version2;
    }
    return 1;

complete:
    while (output < outputEnd)
        *output++ = 0xfe00;
    return 0;
}
