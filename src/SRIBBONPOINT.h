// Shared ribbon-point sample layout.
#ifndef SRIBBONPOINT_INC_H
#define SRIBBONPOINT_INC_H

#ifndef SRC_GLOBALS_H
#error "SRIBBONPOINT.h requires the ordinary globals.h core first"
#endif

/** One point in a rendered ribbon, including its three screen histories. */
struct SRibbonPoint {
    struct CVector Pos;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned short Width;
    unsigned short WidthB;
    unsigned char rB;
    unsigned char gB;
    unsigned char bB;
    int Last1Scr;
    int Last2Scr;
    int Last3Scr;
};

#endif // SRIBBONPOINT_INC_H
