#ifndef TEXTEFFECT_INC_H
#define TEXTEFFECT_INC_H

/** Base state and non-pure operations shared by all font text effects. */
struct TextEffect {
    CClass CClass;
    int mEnabled;

    /** Attach an enabled effect to a font. */
    void bind(Font *pFont);

    /** Detach an enabled effect from a font. */
    void unbind(Font *pFont);

    /** Apply the effect's screen-position adjustment. */
    void getScreenPos(int *pX, int *pY);

    /** Apply the effect's color adjustment. */
    void getColor(unsigned char *pR, unsigned char *pG,
                  unsigned char *pB);

    /** Run the effect after drawing one character. */
    void PostEffect(int oldX, int oldY, int screenX, int screenY);

    /** Disable the effect. */
    void disable();
};

#endif // TEXTEFFECT_INC_H
