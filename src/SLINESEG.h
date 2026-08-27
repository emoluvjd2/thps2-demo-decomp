#ifndef SLINESEG_INC_H
#define SLINESEG_INC_H

#ifndef SRC_GLOBALS_H
#error "SLINESEG.h requires the ordinary globals.h core first"
#endif

/** One coloured endpoint in a rendered polyline. */
struct SLineSeg {
    struct CVector End;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char code;
};

#endif // SLINESEG_INC_H
