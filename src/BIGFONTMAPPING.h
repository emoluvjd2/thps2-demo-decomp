#ifndef BIGFONTMAPPING_INC_H
#define BIGFONTMAPPING_INC_H

/** Character-to-frame lookup table used by the large message font. */
#ifdef BIGFONTMAPPING_OWNER_DECLARATION
extern unsigned char BigFont_Mapping[256];
#else
extern unsigned char BigFont_Mapping[];
#endif

#undef BIGFONTMAPPING_OWNER_DECLARATION

#endif // BIGFONTMAPPING_INC_H
