// SCREENTOOLS.cpp -- Reconstructed translation unit
// Original: SCREENTOOLS.CPP
// Functions: 22 reconstructed entries
// Address range: 0x8008A4D4 - 0x8008AC80
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.


#include "SCREENTOOLS.h"

// Owner-local screen-element layouts used by SCREENTOOLS.cpp.
// Field names and offsets from original/MAIN_STRUCTS.h (permitted evidence).
#include "FONTTOOLS.h"
#include "globals.h"

struct Slice;

// Image / SlicedImage2 / Sprite2 hierarchy (MAIN_STRUCTS.h): SCREENTOOLS only
// touches the justification bytes and the leading CClass vtable pointer, but
// the full layouts are reproduced so sizeof(Sprite2) is the allocation size.
#include "IMAGEBASE.h"

struct SlicedImage2 : Image {
    Slice *SliceTab;
    short W;
    short H;
    short SliceW;
    short SliceH;
    short SliceCountX;
    short SliceCountY;
    /** True after owned pixel buffers have been allocated for the slices. */
    bool InMemory;
    /** True after the slice rectangles have been allocated in VRAM. */
    bool Packed;
    int BppShift;
    unsigned short Clut;

    int screenWidth();
};

struct Sprite2 : SlicedImage2 {
    /** Allocate sprite storage through the shared CClass arena. */
    static void *operator new(unsigned int size) throw()
    {
        return ::CClass::operator new(size);
    }
    /** Build a sprite from a bitmap file. */
    Sprite2(char *name, int pack, int sliceWidth, int sliceHeight);
};

struct ScreenElement {
    void **vtable;
    int mHomeX;
    int mHomeY;
    int mDrawFirst;
    int mHide;
    int mDelay;
    void *mAtHome;
    int mScreenX;
    int mScreenY;

    /** Return the current screen-space X coordinate. */
    int GetScreenX();
    /** Return the current screen-space Y coordinate. */
    int GetScreenY();
    /** Advance this element toward its home position. */
    void update();
    /** Move the screen-space X coordinate directly to its home position. */
    void SnapToHomeX();
    /** Return this element's drawable width. */
    int width();
    /** Notify this element that its displayed content changed. */
    void refresh();
    ~ScreenElement();
    /** Releases screen-element storage through the original CClass allocator. */
    static void operator delete(void *p) { ::CClass::operator delete(p); }
};

struct ScreenElementList : ScreenElement {
    ScreenElement *mpItem[16];
    int mCount;

    /** Add one element to the first unused list slot. */
    void addItem(ScreenElement *element);
    /** Update every active element in the list. */
    void update();
    /** Draw every active element at the supplied parent offset. */
    void draw(int x, int y);
    ~ScreenElementList();
    /** Releases screen-list storage through the original CClass allocator. */
    static void operator delete(void *p) { ::CClass::operator delete(p); }
};

struct TextScreenElement : ScreenElement {
    char *mpString;
    Font *mpFont;

    /** Replace the owned display string with a copy of the supplied text. */
    void setString(char *pString);
    /** Draw the owned string at the supplied parent offset. */
    void draw(int x, int y);
    /** Return the rendered width of the owned string. */
    int width();
    ~TextScreenElement();
    /** Releases text-element storage through the original CClass allocator. */
    static void operator delete(void *p) { ::CClass::operator delete(p); }
};

struct SpriteScreenElement : ScreenElement {
    Sprite2 *mSprite;

    /** Draw the owned sprite at the supplied parent offset. */
    void draw(int x, int y);
    /** Return the on-screen width of the owned sprite. */
    int width();
    ~SpriteScreenElement();
    /** Releases sprite-element storage through the original CClass allocator. */
    static void operator delete(void *p) { ::CClass::operator delete(p); }
};

typedef char screentools_image_size_must_be_28[(sizeof(Image) == 28) ? 1 : -1];
typedef char screentools_sliced_image2_size_must_be_60[(sizeof(SlicedImage2) == 60) ? 1 : -1];
typedef char screentools_sprite2_size_must_be_60[(sizeof(Sprite2) == 60) ? 1 : -1];
typedef char screentools_font_size_must_be_80[(sizeof(Font) == 80) ? 1 : -1];
typedef char screen_element_size_must_be_36[(sizeof(ScreenElement) == 36) ? 1 : -1];
typedef char screen_element_list_size_must_be_104[(sizeof(ScreenElementList) == 104) ? 1 : -1];
typedef char text_screen_element_size_must_be_44[(sizeof(TextScreenElement) == 44) ? 1 : -1];
typedef char sprite_screen_element_size_must_be_40[(sizeof(SpriteScreenElement) == 40) ? 1 : -1];

#include "MEM.h"
#include "PUREVIRTUAL.h"
#include <STRINGS.H>

// === This TU's exported functions (typed views of the mangled symbols) ===
#define SCREENTOOLS_DECLARE_SCREEN_ELEMENT_CONSTRUCTOR
#define SCREENTOOLS_DECLARE_SCREEN_ELEMENT_LIST_CONSTRUCTOR
#define SCREENTOOLS_DECLARE_TEXT_SCREEN_ELEMENT_CONSTRUCTOR
#define SCREENTOOLS_DECLARE_SPRITE_SCREEN_ELEMENT_CONSTRUCTOR
#include "SCREENTOOLS.h"
#define SCREENTOOLS_DECLARE_SCREEN_ELEMENT_DESTRUCTOR
#include "SCREENTOOLS.h"
#define SCREENTOOLS_DECLARE_SCREEN_ELEMENT_LIST_DESTRUCTOR
#include "SCREENTOOLS.h"
#define SCREENTOOLS_DECLARE_TEXT_SCREEN_ELEMENT_DESTRUCTOR
#include "SCREENTOOLS.h"
#define SCREENTOOLS_DECLARE_SPRITE_SCREEN_ELEMENT_DESTRUCTOR
#include "SCREENTOOLS.h"

/**
 * Screen-element dispatch tables in their original read-only section order.
 *
 * Each table has the two GCC 2.x header words and five virtual slots.
 * Eight-byte alignment contributes one zero padding word between tables.
 */
int __vt_17ScreenElementList[7]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&ScreenElementList_DestroyAbi,
        (int)(void (*)(ScreenElementList *))&ScreenElementList::update,
        (int)(void (*)(ScreenElementList *, int, int))&ScreenElementList::draw,
        (int)(int (*)(ScreenElement *))&ScreenElement::width,
        (int)(void (*)(ScreenElement *))&ScreenElement::refresh,
};

int __vt_19SpriteScreenElement[7]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&SpriteScreenElement_destructor_slot,
        (int)(void (*)(ScreenElement *))&ScreenElement::update,
        (int)(void (*)(SpriteScreenElement *, int, int))&SpriteScreenElement::draw,
        (int)(int (*)(SpriteScreenElement *))&SpriteScreenElement::width,
        (int)(void (*)(ScreenElement *))&ScreenElement::refresh,
};

int __vt_17TextScreenElement[7]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&TextScreenElement_DestroyAbi,
        (int)(void (*)(ScreenElement *))&ScreenElement::update,
        (int)(void (*)(TextScreenElement *, int, int))&TextScreenElement::draw,
        (int)(int (*)(TextScreenElement *))&TextScreenElement::width,
        (int)(void (*)(ScreenElement *))&ScreenElement::refresh,
};

int __vt_13ScreenElement[7]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&ScreenElement_DestroyAbi,
        (int)(void (*)(ScreenElement *))&ScreenElement::update,
        (int)&__pure_virtual,
        (int)(int (*)(ScreenElement *))&ScreenElement::width,
        (int)(void (*)(ScreenElement *))&ScreenElement::refresh,
};

// GCC 2.x emits a private copy of `CClass::~CClass` last in every owner that
// derives from CClass.  `MAIN.SYM` records 25 such `_._6CClass` copies and names
// no base vtable, so both are file-local here.  ScreenElement's *public* vtable
// lives in `.rdata` above; only this private base table is small-data.
extern "C" {
static void __DOT__6CClass_8008AC50(CClass *_this, int flags);
}

static int __vt_base_13ScreenElement
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int ScreenTools_BaseVTableIndex = 0;
static int ScreenTools_BaseVTableDtor = (int)&__DOT__6CClass_8008AC50;

// === Function implementations (address order) ===

/**
 * ScreenElement::ScreenElement(int, int, int, int)
 *
 * Address:   0x8008A4D4
 * Mangled:   __13ScreenElementiiii
 *            Constructor: sets vtable, stores home/screen positions, clears delay/flags
 *
 * @param self  `this` pointer (explicit — the class is kept vtable-free).
 * @param homeX  Home X coordinate (resting position).
 * @param homeY  Home Y coordinate (resting position).
 * @param screenX  Screen-space X coordinate, or -1 to start at home.
 * @param screenY  Screen-space Y coordinate, or -1 to start at home.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
ScreenElement *__13ScreenElementiiii(ScreenElement *self, int homeX, int homeY,
                                     int screenX, int screenY) {
    self->vtable = (void **)__vt_13ScreenElement;
    self->mHomeX = homeX;
    self->mHomeY = homeY;
    if (screenX == -1) {
        self->mScreenX = homeX;
    } else {
        self->mScreenX = screenX;
    }
    if (screenY == -1) {
        self->mScreenY = self->mHomeY;
    } else {
        self->mScreenY = screenY;
    }
    self->mDrawFirst = 0;
    self->mHide = 0;
    self->mDelay = 0;
    return self;
}


/**
 * ScreenElement::~ScreenElement()
 *
 * Address:   0x8008A530
 * Mangled:   _._13ScreenElement
 *
 */
ScreenElement::~ScreenElement() {
    vtable = (void **)&__vt_base_13ScreenElement;
}


/**
 * ScreenElement::width(void)
 *
 * Address:   0x8008A560
 * Mangled:   width__13ScreenElement
 *            ASM: jr $ra; addiu $v0, $zero, -1
 *
 * @return -1 (pure-virtual-style placeholder width).
 */
int ScreenElement::width() {
    return -1;
}


/**
 * ScreenElement::update(void)
 *
 * Address:   0x8008A568
 * Mangled:   update__13ScreenElement
 *            Interpolates screenX/Y toward homeX/Y, sets arrived flag when done
 */
void ScreenElement::update() {
    int v = mDelay;
    if (v > 0) {
        mDelay = v - 1;
    } else {
        // Interpolate X
        int homeX = mHomeX;
        int screenX = mScreenX;
        int distX = homeX - screenX;
        if ((unsigned int)(distX + 1) < 3) {
            mScreenX = homeX;
        } else {
            int add = distX >> 2;
            if (add == 0) {
                add = distX >> 1;
            }
            mScreenX = screenX + add;
        }

        // Interpolate Y
        int homeY = mHomeY;
        int screenY = mScreenY;
        int distY = homeY - screenY;
        if ((unsigned int)(distY + 1) < 3) {
            mScreenY = homeY;
        } else {
            int add = distY >> 2;
            if (add == 0) {
                add = distY >> 1;
            }
            mScreenY = screenY + add;
        }

        // Check if arrived
        if (mScreenY != mHomeY) {
            goto not_at_home;
        }
        if (mScreenX != mHomeX) {
            goto not_at_home;
        }
        mAtHome = 1;
        return;
not_at_home:
        mAtHome = 0;
    }
}


/**
 * ScreenElement::refresh(void)
 *
 * Address:   0x8008A630
 * Mangled:   refresh__13ScreenElement
 *            ASM: jr $ra; nop (no .s file — 2 instr empty function)
 */
void ScreenElement::refresh() {
}


/**
 * ScreenElement::SnapToHomeX(void)
 *
 * Address:   0x8008A638
 * Mangled:   SnapToHomeX__13ScreenElement
 *            ASM: lw $v0, 4($a0); jr $ra; sw $v0, 0x1c($a0)
 */
void ScreenElement::SnapToHomeX() {
    mScreenX = mHomeX;
}


/**
 * ScreenElement::GetScreenX(void)
 *
 * Address:   0x8008A644
 * Mangled:   GetScreenX__13ScreenElement
 *            ASM: lw $v0, 0x1c($a0); jr $ra; nop
 *
 * @return Current screen-space X coordinate.
 */
int ScreenElement::GetScreenX() {
    return mScreenX;
}


/**
 * ScreenElement::GetScreenY(void)
 *
 * Address:   0x8008A650
 * Mangled:   GetScreenY__13ScreenElement
 *            ASM: lw $v0, 0x20($a0); jr $ra; nop
 *
 * @return Current screen-space Y coordinate.
 */
int ScreenElement::GetScreenY() {
    return mScreenY;
}


/**
 * ScreenElementList::ScreenElementList(int, int, int, int)
 *
 * Address:   0x8008A65C
 * Mangled:   __17ScreenElementListiiii
 *            Constructor: calls parent ScreenElement ctor, clears 16 item slots, sets count=0
 *
 * @param self  `this` pointer (explicit — the class is kept vtable-free).
 * @param homeX  Home X coordinate (resting position).
 * @param homeY  Home Y coordinate (resting position).
 * @param screenX  Screen-space X coordinate.
 * @param screenY  Screen-space Y coordinate.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
ScreenElementList *__17ScreenElementListiiii(
    ScreenElementList *self, int homeX, int homeY, int screenX, int screenY) {
    __13ScreenElementiiii(self, homeX, homeY, screenX, screenY);
    int i = 0;
    ScreenElement **ptr = self->mpItem;
    self->vtable = (void **)__vt_17ScreenElementList;
    do {
        *ptr = 0;
        i++;
        ptr++;
    } while (i < 16);
    self->mCount = 0;
    return self;
}


/**
 * ScreenElementList::~ScreenElementList()
 *
 * Address:   0x8008A6B8
 * Mangled:   _._17ScreenElementList
 *            Virtually destroys every remaining item (flags=3: dtor + free).
 *
 */
ScreenElementList::~ScreenElementList() {
    typedef void (*DtorFunc)(ScreenElement *, int);
    vtable = (void **)__vt_17ScreenElementList;
    int i = 0;
    ScreenElement **ptr = mpItem;
    do {
        ScreenElement *item = *ptr;
        if (item != 0) {
            int *itemVTable = (int *)item->vtable;
            DtorFunc fn = (DtorFunc)itemVTable[2];
            fn(item, 3);
        }
        i++;
        ptr++;
    } while (i < 16);
}


/**
 * ScreenElementList::addItem(ScreenElement *)
 *
 * Address:   0x8008A760
 * Mangled:   addItem__17ScreenElementListP13ScreenElement
 *            Finds first empty slot in items array, stores item, increments count
 *
 * @param pScreenElement  ScreenElement to add to the list.
 */
void ScreenElementList::addItem(ScreenElement *pScreenElement) {
    int i = 0;
    ScreenElement **ptr = mpItem;
    do {
        if (*ptr == 0) {
            *ptr = pScreenElement;
            mCount = mCount + 1;
            return;
        }
        i++;
        ptr++;
    } while (i < 16);
}


/**
 * ScreenElementList::update(void)
 *
 * Address:   0x8008A7A4
 * Mangled:   update__17ScreenElementList
 *            Calls each item's virtual update, then updates itself as a ScreenElement
 */
void ScreenElementList::update() {
    int count = mCount;
    if (count > 0) {
        int i = 0;
        ScreenElement **ptr = mpItem;
        do {
            ScreenElement *item = *ptr;
            int *itemVTable = (int *)item->vtable;
            ((void (*)(ScreenElement *))itemVTable[3])(item);
            ptr++;
            count = mCount;
            i++;
        } while (i < count);
    }
    ScreenElement::update();
}


/**
 * ScreenElementList::draw(int, int)
 *
 * Address:   0x8008A824
 * Mangled:   draw__17ScreenElementListii
 *            Virtually draws every item at the list's screen position plus offset
 *
 * @param x  X offset added to the list's screen position.
 * @param y  Y offset added to the list's screen position.
 */
void ScreenElementList::draw(int x, int y) {
    if (mHide != 0) return;
    int count = mCount;
    if (count <= 0) return;
    int i = 0;
    ScreenElement **ptr = mpItem;
    do {
        ScreenElement *item = *ptr;
        ptr++;
        int sx = mScreenX + x;
        int sy = mScreenY + y;
        int *itemVTable = (int *)item->vtable;
        ((void (*)(ScreenElement *, int, int))itemVTable[4])(item, sx, sy);
        count = mCount;
        i++;
    } while (i < count);
}


/**
 * TextScreenElement::TextScreenElement(char *, Font *, int, int, int, int)
 *
 * Address:   0x8008A8CC
 * Mangled:   __17TextScreenElementPcP4Fontiiii
 *            Constructor: calls parent ScreenElement ctor, allocates+copies string, stores font
 *
 * @param self  `this` pointer (explicit — the class is kept vtable-free).
 * @param pString  Text to display (copied into a fresh allocation).
 * @param pFont  Font used for rendering.
 * @param homeX  Home X coordinate (resting position).
 * @param homeY  Home Y coordinate (resting position).
 * @param screenX  Screen-space X coordinate.
 * @param screenY  Screen-space Y coordinate.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
TextScreenElement *__17TextScreenElementPcP4Fontiiii(
    TextScreenElement *self, char *pString, Font *pFont, int homeX, int homeY,
    int screenX, int screenY) {
    __13ScreenElementiiii(self, homeX, homeY, screenX, screenY);
    self->vtable = (void **)__vt_17TextScreenElement;
    int len = strlen(pString);
    char *buf = (char *)Mem_New(len + 1, 0, 1);
    self->mpString = buf;
    strcpy(buf, pString);
    self->mpFont = pFont;
    return self;
}


/**
 * TextScreenElement::~TextScreenElement()
 *
 * Address:   0x8008A958
 * Mangled:   _._17TextScreenElement
 *
 */
TextScreenElement::~TextScreenElement() {
    vtable = (void **)__vt_17TextScreenElement;
    Mem_Delete(mpString);
}


/**
 * TextScreenElement::setString(char *)
 *
 * Address:   0x8008A9B8
 * Mangled:   setString__17TextScreenElementPc
 *            Frees old string, allocates new, copies
 *
 * @param pString  Replacement text (copied into a fresh allocation).
 */
void TextScreenElement::setString(char *pString) {
    Mem_Delete(mpString);
    int len = strlen(pString);
    char *buf = (char *)Mem_New(len + 1, 0, 1);
    mpString = buf;
    strcpy(buf, pString);
}


/**
 * TextScreenElement::draw(int, int)
 *
 * Address:   0x8008AA14
 * Mangled:   draw__17TextScreenElementii
 *            Draws the string via the font's virtual draw (slot 3), honoring delay/hide.
 *
 * @param x  X offset added to the element's screen position.
 * @param y  Y offset added to the element's screen position.
 */
void TextScreenElement::draw(int x, int y) {
    register int ty asm("$9") = y;
    if (mDelay > 0) return;
    if (mHide != 0) return;
    Font *font = mpFont;
    int scale = mDrawFirst;
    int sx = mScreenX + x;
    int sy = mScreenY + ty;
    char *str = mpString;
    FontDispatch(font)->draw(font, sx, sy, str, scale);
}


/**
 * TextScreenElement::width(void)
 *
 * Address:   0x8008AA84
 * Mangled:   width__17TextScreenElement
 *            Calls Font::width with this->font and this->string
 *
 * @return Rendered width of the string in the element's font.
 */
int TextScreenElement::width() {
    return mpFont->width(mpString);
}


/**
 * SpriteScreenElement::SpriteScreenElement(char *, int, int, int, int, int, int)
 *
 * Address:   0x8008AAB0
 * Mangled:   __19SpriteScreenElementPciiiiii
 *            Constructor: calls parent ScreenElement ctor, allocates the Sprite2,
 *            stores it, then sets its justification bytes.
 *
 * @param self  `this` pointer (explicit — the class is kept vtable-free).
 * @param pFileName  Sprite image file name.
 * @param homeX  Home X coordinate (resting position).
 * @param homeY  Home Y coordinate (resting position).
 * @param screenX  Screen-space X coordinate.
 * @param screenY  Screen-space Y coordinate.
 * @param justY  Vertical justification byte for the sprite image.
 * @param justX  Horizontal justification byte for the sprite image.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
SpriteScreenElement *__19SpriteScreenElementPciiiiii(
    SpriteScreenElement *self, char *pFileName, int homeX, int homeY,
    int screenX, int screenY, int justY, int justX) {
    __13ScreenElementiiii(self, homeX, homeY, screenX, screenY);
    self->vtable = (void **)__vt_19SpriteScreenElement;
    Sprite2 *sprite = new Sprite2(pFileName, 1, 64, 64);
    self->mSprite = sprite;
    self->mSprite->JustY = (char)justY;
    self->mSprite->JustX = (char)justX;
    return self;
}


/**
 * SpriteScreenElement::~SpriteScreenElement()
 *
 * Address:   0x8008AB48
 * Mangled:   _._19SpriteScreenElement
 *            Virtually destroys the owned sprite (flags=3: dtor + free).
 *
 */
SpriteScreenElement::~SpriteScreenElement() {
    typedef void (*DtorFunc)(Sprite2 *, int);
    vtable = (void **)__vt_19SpriteScreenElement;
    Sprite2 *sprite = mSprite;
    if (sprite != 0) {
        int *spriteVTable = sprite->CClass.vtable;
        DtorFunc fn = (DtorFunc)spriteVTable[2];
        fn(sprite, 3);
    }
}


/**
 * SpriteScreenElement::draw(int, int)
 *
 * Address:   0x8008ABC0
 * Mangled:   draw__19SpriteScreenElementii
 *            Draws the sprite via its virtual draw (slot 5), honoring delay/hide.
 *
 * @param x  X offset added to the element's screen position.
 * @param y  Y offset added to the element's screen position.
 */
void SpriteScreenElement::draw(int x, int y) {
    if (mDelay > 0) return;
    if (mHide != 0) return;
    Sprite2 *sprite = mSprite;
    int sx = mScreenX + x;
    int sy = mScreenY + y;
    int scale = mDrawFirst;
    int *spriteVTable = sprite->CClass.vtable;
    ((void (*)(Sprite2 *, int, int, int))spriteVTable[5])(sprite, sx, sy, scale);
}


/**
 * SpriteScreenElement::width(void)
 *
 * Address:   0x8008AC2C
 * Mangled:   width__19SpriteScreenElement
 *            Returns the sprite's screen width via its SlicedImage2 base.
 *
 * @return On-screen width of the owned sprite.
 */
// Shared SpriteScreenElement member function
int SpriteScreenElement::width() {
    return mSprite->SlicedImage2::screenWidth();
}

// GCC emitted this private `CClass::~CClass` copy last in the owner's text,
// so it stays at the end of the translation unit.
static void __DOT__6CClass_8008AC50(CClass *_this, int flags) {
    _this->vtable = (int *)&__vt_base_13ScreenElement;
    if (flags & 1) {
        CClass::operator delete(_this);
    }
}
