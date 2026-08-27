// Shared message-animation layout and callable renderer interface.
#ifndef MESS_INC_H
#define MESS_INC_H

struct Message;

/** One 32-byte message animation program shared with message callers. */
struct SMessageProg {
    unsigned short Duration;
    unsigned char Semitrans;
    unsigned char Justify;
    unsigned short StartScale;
    unsigned short EndScale;
    unsigned char StartR;
    unsigned char StartG;
    unsigned char StartB;
    unsigned char EndR;
    unsigned char EndG;
    unsigned char EndB;
    unsigned short StartAngle;
    unsigned short EndAngle;
    short StartX;
    short StartY;
    short EndX;
    short EndY;
    unsigned char FlashDuration;
    unsigned short ColorCycle;
    signed char NextOff;
};

#ifdef MESS_OWNER_DECLARATIONS
#define MESS_PROGRAM_DECL_ATTR
#else
#define MESS_PROGRAM_DECL_ATTR __attribute__((section(".data")))
#endif

/** Standard zoom-in, hold, and zoom-out message animation. */
extern "C" SMessageProg Messprog_Zoom[5] MESS_PROGRAM_DECL_ATTR;

/** Goal/tape notification variant with color cycling enabled. */
extern "C" SMessageProg Messprog_Goal[5] MESS_PROGRAM_DECL_ATTR;

#undef MESS_PROGRAM_DECL_ATTR

/** Select the justification used by subsequent message draws. */
void Mess_SetTextJustify(unsigned char justify);

/** Set the 8.8 fixed-point scale used by subsequent message draws. */
void Mess_SetScale(int scale);

/** Return the active 8.8 fixed-point message scale. */
int Mess_GetScale(void);

/** Select the ordering-table position used by subsequent message draws. */
void Mess_SetSort(int sort);

/** Set the RGB color and semitransparency used by subsequent message draws. */
void Mess_SetRGB(unsigned char red, unsigned char green, unsigned char blue,
                 int semitrans);

/** Draw one string with the active panel font. */
int Mess_DrawPanelText(int x, int y, const char *message, int angle,
                       unsigned int fireRGB, int fireSquash);

/** Allocate and initialize one active message. */
Message *Mess_Message(const char *text, SMessageProg *program,
                      bool recenter, short xOffset, short yOffset);

/** Delete every active message. */
void Mess_DeleteAll(void);

/** Advance every active message animation by one frame. */
void Mess_Update(void);

/** Draw every active message. */
void Mess_Display(void);

/** Return the rendered width of a message string. */
int Mess_TextWidth(const char *message);

/** Return the rendered height of a message string. */
int Mess_TextHeight(const char *message);

/** Return the rendered height above the active font baseline. */
int Mess_TextHeightAboveBaseline(const char *message);

/** Return the rendered height below the active font baseline. */
int Mess_TextHeightBelowBaseline(const char *message);

/** Draw one message string with the active font and render state. */
int Mess_DrawText(int x, int y, const char *message, int drawSolid,
                  unsigned int fireRGB, int fireSquash, bool isPanel);

/** Load a font from an already-resident file buffer. */
int Mess_LoadFont(char *filename, char *fileBuffer, int charSeparation,
                  int shadowOffset, int width);

/** Load a font from the packed-file system. */
int Mess_LoadFont(char *filename, int charSeparation,
                  int shadowOffset, int width);

/** Release the currently loaded font. */
void Mess_UnloadFont(void);

/** Select the active font by table index. */
void Mess_SetCurrentFont(int font);

/** Select the active font by filename. */
void Mess_SetCurrentFont(char *fontName);

/** Set the active font's 8.8 fixed-point scale. */
void Mess_SetFontScale(unsigned int scale);

/** Return the active font's 8.8 fixed-point scale. */
int Mess_GetFontScale(void);

typedef char message_program_size_must_be_32[
    (sizeof(SMessageProg) == 32) ? 1 : -1];

#endif // MESS_INC_H
