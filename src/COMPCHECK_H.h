#ifndef COMPCHECK_H_INC_H
#define COMPCHECK_H_INC_H

#include "globals.h"

/** Write one hexadecimal digit. */
void PutHexDigit(char *p, unsigned int n);

/** Write the low byte as two hexadecimal digits. */
void PutHexByte(char *p, unsigned int n);

/** Write the low halfword as four hexadecimal digits. */
void PutHex2Byte(char *p, unsigned int n);

/** Write a word as eight hexadecimal digits. */
void PutHex4Byte(char *p, unsigned int n);

/** Mix the recorded score state into its 32-bit check value. */
u32 CheckSum_Calculate(u32 Score, u8 Random, int gs);

/** Encode the calculated check value into the shared eight-digit string. */
void CheckSum_Encode(u32 Score, u8 Random, int gs);

#endif // COMPCHECK_H_INC_H

#ifdef COMPCHECK_DECLARE_CHECKSUM_STRING_VIEW
/** Eight-character address view of MAIN's nine-byte encoded-check buffer. */
extern char Main_ChecksumCodeString[8] asm("Code_String");
#undef COMPCHECK_DECLARE_CHECKSUM_STRING_VIEW
#endif
