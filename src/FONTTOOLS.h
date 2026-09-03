// Hand-maintained per-TU interface for FONTTOOLS.cpp.
// gen_tu_headers.py is retained only for explicit bootstrap regeneration.
#ifndef FONTTOOLS_INC_H
#define FONTTOOLS_INC_H

#include "globals.h"

struct Font;
struct SlicedImage2;
struct TextEffect;

/** Registry for the eight resident fonts. */
struct FontManager {
    static Font *LoadFont(char *filename);
    static Font *LoadFont(unsigned char *pData, char *fontname);
    static Font *GetFont(char *fontname);
    static void UnloadFont(char *fontname);

private:
    static char FontName[8][16];
    static Font *FontTab[8];
    static int dumb;
};

/** Metrics and image storage for one font glyph. */
struct FontCharacter {
    SlicedImage2 *Image;
    int W;
    int H;
    int Baseline;
};

/** Inert storage view for owners that explicitly manage Font lifetime. */
struct FontStorage {
    CClass CClass;
    int JustX;
    int JustY;
    int SpaceX;
    int SpaceY;
    int R;
    int G;
    int B;
    void *Aliased;
    void *Shadow;
    int ShadowDx;
    int ShadowDy;
    int CharMap;
    int FixedWidth;
    int Scale;
    FontCharacter *CharTab;
    int NumChars;
    int Clut;
    TextEffect *CurrentTextEffect;
    unsigned char OldR;
    unsigned char OldG;
    unsigned char OldB;
};

/** Stateful renderer for one loaded bitmap font. */
struct Font : FontStorage {
    Font();
    Font(unsigned char *buf);
    Font(const Font &source);
    ~Font();

    /** Allocate font storage through the shared CClass arena. */
    static void *operator new(unsigned int size)
    {
        return ::CClass::operator new(size);
    }

    void draw(int x, int y, char *str, int drawFirst);
    int width(char *str);
    int height(char *str);
    int heightAboveBaseline(char *str);
    int heightBelowBaseline(char *str);
    int getCharIndex(char asc);
    int isEscapeChar(char asc);
    void handleEscapeChar(char asc);
    int fixedWidth(char *pString);
    int fixedCharWidth(char asc);
    void setRGB(int r, int g, int b);
    void SetTextEffect(TextEffect *pEffect);
    void unload();
};

/** Target dispatch table shared by virtual Font consumers. */
struct FontVTable {
    int baseOffset;
    void *typeInfo;
    void (*destroy)(Font *font, int inCharge);
    void (*draw)(Font *font, int x, int y, char *str, int drawFirst);
};

/** Recover a Font's typed target dispatch table. */
inline FontVTable *FontDispatch(Font *font)
{
    return reinterpret_cast<FontVTable *>(font->CClass.vtable);
}

/** Recover the active Font interface from explicitly managed storage. */
inline Font *FontFromStorage(FontStorage *storage)
{
    return static_cast<Font *>(storage);
}

/** Const view of explicitly managed Font storage. */
inline const Font *FontFromStorage(const FontStorage *storage)
{
    return static_cast<const Font *>(storage);
}

typedef char FontCharacter_size_must_be_0x10[
    (sizeof(FontCharacter) == 0x10) ? 1 : -1];
typedef char FontStorage_size_must_be_0x50[
    (sizeof(FontStorage) == 0x50) ? 1 : -1];
typedef char Font_size_must_be_0x50[
    (sizeof(Font) == 0x50) ? 1 : -1];

#ifdef FONTTOOLS_OWNER_DECLARATIONS

#ifdef __cplusplus
extern "C" {
#endif

// Primitive types
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned char byte;
typedef int undefined;
typedef char undefined1;
typedef short undefined2;
typedef int undefined4;
typedef unsigned long ulong;
typedef unsigned char undefined3[3];

// Additional shared forward declarations
struct CBit;
struct CGlow;
struct CSpecialDisplay;
struct CFlatBit;
struct TextEffect;
struct RailPoint;
struct SCardHead;
struct SCamera;
struct STexWibVertInfo;
struct SViewport;
struct SColCache;
struct SLineInfo;
struct STexWibItemInfo;
struct SBlockHeader;
struct ControllerData;

#ifdef __cplusplus
}
#endif

#endif // FONTTOOLS_OWNER_DECLARATIONS

#endif // FONTTOOLS_INC_H

#ifdef FONTTOOLS_DECLARE_STORAGE_CONSTRUCTOR_ABI
struct Font;
struct FontStorage;

namespace CreateCollaboratorConstructorAbi
{
/** Construct a Font in explicitly managed storage through its target entry. */
extern "C" Font *__4Font(FontStorage *storage);
}

#undef FONTTOOLS_DECLARE_STORAGE_CONSTRUCTOR_ABI
#endif
