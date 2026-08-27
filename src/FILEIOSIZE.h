#ifndef FILEIOSIZE_INC_H
#define FILEIOSIZE_INC_H

/**
 * Byte length of the most recently opened asset, physically owned by
 * FILEIO.cpp. Consumers keep an absolute-address view of the small-data word
 * so their target code uses ordinary `lui`/`lw` addressing.
 */
extern "C" int FileIO_Size __attribute__((section(".data")));

#endif // FILEIOSIZE_INC_H
