#ifndef PROMPTSCREENELEMENT_INC_H
#define PROMPTSCREENELEMENT_INC_H

#ifndef SCREENTOOLS_AGGREGATE_LAYOUT_EMITTED
#error "PromptScreenElement requires the SCREENTOOLS aggregate layout first"
#endif

/** Prompt row containing an icon and its explanatory text. */
struct PromptScreenElement {
    struct ScreenElementList ScreenElementList;

    /** Allocate prompt storage through the shared CClass arena. */
    static void *operator new(unsigned int size) throw()
    {
        return ::CClass::operator new(size);
    }
    PromptScreenElement(char *pFileName, char *pString, Font *pFont, int homeX, int homeY, int screenX, int screenY);
    ~PromptScreenElement();
    int width();
};

#endif // PROMPTSCREENELEMENT_INC_H
