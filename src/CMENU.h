#ifndef THPS2_CMENU_H
#define THPS2_CMENU_H

#include "globals.h"

/** Display and animation state for one front-end menu entry. */
struct SEntry {
    char *pString;
    short CurrentScale;
    short TargetScale;
    short HiScale;
    short LowScale;
    unsigned char Clearance;
    unsigned char Visible;
    unsigned char Enabled;
    unsigned char HiR;
    unsigned char HiG;
    unsigned char HiB;
    unsigned char LowR;
    unsigned char LowG;
    unsigned char LowB;
    unsigned char FadeUpCounter;
};

/** Front-end text menu and its ordinary service interface. */
struct CMenu {
    struct CClass CClass;
    unsigned char mJustification;
    unsigned char mRep;
    unsigned char mLine;
    unsigned char mHasCursor;
    unsigned char mCursorLine;
    unsigned char mNumLines;
    int mX;
    int mY;
    int mLineSep;
    struct SEntry mEntry[15];

    /** Allocate menu storage through the shared CClass arena. */
    static void *operator new(unsigned int size) throw()
    {
        return ::CClass::operator new(size);
    }
    CMenu(int x, int y, unsigned char justification,
          int highScale, int lowScale, int lineSeparation);
    ~CMenu();
    /** Center the visible entries vertically. */
    void CentreY();
    /** Select one menu line. */
    void SetLine(unsigned char line);
    /** Find an entry by its displayed text. */
    int FindEntry(const char *text);
    /** Append a visible, enabled menu entry. */
    void AddEntry(const char *text);
    /** Make the named entry visible. */
    void EntryOn(const char *text);
    /** Hide the named entry. */
    void EntryOff(const char *text);
    /** Report whether the named entry is visible. */
    int IsEntryOn(const char *text);
    /** Compare text with the selected entry. */
    int ChoiceIs(const char *text);
    /** Draw the visible entries. */
    void Display();
    /** Advance menu input and selection animation. */
    void Update();
};

typedef char CMenu_SEntry_size_must_be_24[
    (sizeof(SEntry) == 0x18) ? 1 : -1];
typedef char CMenu_size_must_be_384[
    (sizeof(CMenu) == 0x180) ? 1 : -1];

#endif // THPS2_CMENU_H
