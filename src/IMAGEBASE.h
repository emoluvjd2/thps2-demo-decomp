#ifndef IMAGEBASE_INC_H
#define IMAGEBASE_INC_H

#ifndef SRC_GLOBALS_H
#error "IMAGEBASE.h requires the ordinary globals.h core first"
#endif

/** Common render state shared by IMAGE's textured image classes. */
struct Image {
    CClass CClass;
    void *Transparent;
    void *Shaded;
    int TransType;
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char JustX;
    unsigned char JustY;
    int Scale;

    /** Draw the image at the requested screen position. */
    void draw(int x, int y);
};

#endif // IMAGEBASE_INC_H
