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

struct SlicedImage2 {
    Image Image;
    Slice *SliceTab;
    short W;
    short H;
    short SliceW;
    short SliceH;
    short SliceCountX;
    short SliceCountY;
    void *InMemory;
    void *Packed;
    int BppShift;
    unsigned short Clut;

    int screenWidth();
};

struct Sprite2 {
    SlicedImage2 SlicedImage2;

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
    /** Move the screen-space X coordinate directly to its home position. */
    void SnapToHomeX();
    ~ScreenElement();
    /** Releases screen-element storage through the original CClass allocator. */
    static void operator delete(void *p) { ::CClass::operator delete(p); }
};

struct ScreenElementList : ScreenElement {
    ScreenElement *mpItem[16];
    int mCount;

    /** Add one element to the first unused list slot. */
    void addItem(ScreenElement *element);
    ~ScreenElementList();
    /** Releases screen-list storage through the original CClass allocator. */
    static void operator delete(void *p) { ::CClass::operator delete(p); }
};

struct TextScreenElement : ScreenElement {
    char *mpString;
    Font *mpFont;

    /** Replace the owned display string with a copy of the supplied text. */
    void setString(char *pString);
    ~TextScreenElement();
    /** Releases text-element storage through the original CClass allocator. */
    static void operator delete(void *p) { ::CClass::operator delete(p); }
};

struct SpriteScreenElement : ScreenElement {
    Sprite2 *mSprite;

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
#include <STRINGS.H>

// === This TU's exported functions (typed views of the mangled symbols) ===
ScreenElement *ScreenElement_ctor(ScreenElement *self, int homeX, int homeY,
                                  int screenX, int screenY)
    asm("__13ScreenElementiiii");
int ScreenElement_width(ScreenElement *self) asm("width__13ScreenElement");
void ScreenElement_update(ScreenElement *self) asm("update__13ScreenElement");
ScreenElementList *ScreenElementList_ctor(ScreenElementList *self, int homeX, int homeY,
                                          int screenX, int screenY)
    asm("__17ScreenElementListiiii");
void ScreenElementList_update(ScreenElementList *self) asm("update__17ScreenElementList");
void ScreenElementList_draw(ScreenElementList *self, int x, int y)
    asm("draw__17ScreenElementListii");
TextScreenElement *TextScreenElement_ctor(TextScreenElement *self, char *pString,
                                          Font *pFont, int homeX, int homeY,
                                          int screenX, int screenY)
    asm("__17TextScreenElementPcP4Fontiiii");
void TextScreenElement_draw(TextScreenElement *self, int x, int y)
    asm("draw__17TextScreenElementii");
int TextScreenElement_width(TextScreenElement *self) asm("width__17TextScreenElement");
SpriteScreenElement *SpriteScreenElement_ctor(SpriteScreenElement *self, char *pFileName,
                                              int homeX, int homeY, int screenX,
                                              int screenY, int justY, int justX)
    asm("__19SpriteScreenElementPciiiiii");
void SpriteScreenElement_draw(SpriteScreenElement *self, int x, int y)
    asm("draw__19SpriteScreenElementii");
int SpriteScreenElement_width(SpriteScreenElement *self)
    asm("width__19SpriteScreenElement");
extern "C" void __pure_virtual(void);
void ScreenElement_destructor_slot(ScreenElement *, int)
    asm("_._13ScreenElement");
void ScreenElementList_destructor_slot(ScreenElementList *, int)
    asm("_._17ScreenElementList");
void TextScreenElement_destructor_slot(TextScreenElement *, int)
    asm("_._17TextScreenElement");
void SpriteScreenElement_destructor_slot(SpriteScreenElement *, int)
    asm("_._19SpriteScreenElement");

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
        (int)&ScreenElementList_destructor_slot,
        (int)&ScreenElementList_update,
        (int)&ScreenElementList_draw,
        (int)&ScreenElement_width,
        (int)&ScreenElement_Refresh,
};

int __vt_19SpriteScreenElement[7]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&SpriteScreenElement_destructor_slot,
        (int)&ScreenElement_update,
        (int)&SpriteScreenElement_draw,
        (int)&SpriteScreenElement_width,
        (int)&ScreenElement_Refresh,
};

int __vt_17TextScreenElement[7]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&TextScreenElement_destructor_slot,
        (int)&ScreenElement_update,
        (int)&TextScreenElement_draw,
        (int)&TextScreenElement_width,
        (int)&ScreenElement_Refresh,
};

int __vt_13ScreenElement[7]
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        (int)&ScreenElement_destructor_slot,
        (int)&ScreenElement_update,
        (int)&__pure_virtual,
        (int)&ScreenElement_width,
        (int)&ScreenElement_Refresh,
};

// GCC 2.x emits a private copy of `CClass::~CClass` last in every owner that
// derives from CClass.  `MAIN.SYM` records 25 such `_._6CClass` copies and names
// no base vtable, so both are file-local here.  ScreenElement's *public* vtable
// lives in `.rdata` above; only this private base table is small-data.
static void ScreenTools_ClassDestructor(CClass *_this, int flags)
    asm("__DOT__6CClass_8008AC50");

static int __vt_base_13ScreenElement
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int ScreenTools_BaseVTableIndex = 0;
static int ScreenTools_BaseVTableDtor = (int)&ScreenTools_ClassDestructor;

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
ScreenElement *ScreenElement_ctor(ScreenElement *self, int homeX, int homeY,
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
int ScreenElement_width(ScreenElement *self) {
    return -1;
}


/**
 * ScreenElement::update(void)
 *
 * Address:   0x8008A568
 * Mangled:   update__13ScreenElement
 *            Interpolates screenX/Y toward homeX/Y, sets arrived flag when done
 */
void ScreenElement_update(ScreenElement *self) {
    int v = self->mDelay;
    if (v > 0) {
        self->mDelay = v - 1;
    } else {
        // Interpolate X
        int homeX = self->mHomeX;
        int screenX = self->mScreenX;
        int distX = homeX - screenX;
        if ((unsigned int)(distX + 1) < 3) {
            self->mScreenX = homeX;
        } else {
            int add = distX >> 2;
            if (add == 0) {
                add = distX >> 1;
            }
            self->mScreenX = screenX + add;
        }

        // Interpolate Y
        int homeY = self->mHomeY;
        int screenY = self->mScreenY;
        int distY = homeY - screenY;
        if ((unsigned int)(distY + 1) < 3) {
            self->mScreenY = homeY;
        } else {
            int add = distY >> 2;
            if (add == 0) {
                add = distY >> 1;
            }
            self->mScreenY = screenY + add;
        }

        // Check if arrived
        if (self->mScreenY != self->mHomeY) {
            goto not_at_home;
        }
        if (self->mScreenX != self->mHomeX) {
            goto not_at_home;
        }
        self->mAtHome = 1;
        return;
not_at_home:
        self->mAtHome = 0;
    }
}


/**
 * ScreenElement::refresh(void)
 *
 * Address:   0x8008A630
 * Mangled:   refresh__13ScreenElement
 *            ASM: jr $ra; nop (no .s file — 2 instr empty function)
 */
void ScreenElement_Refresh(ScreenElement *self) {
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
ScreenElementList *ScreenElementList_ctor(ScreenElementList *self, int homeX, int homeY,
                                          int screenX, int screenY) {
    ScreenElement_ctor(self, homeX, homeY, screenX, screenY);
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
void ScreenElementList_update(ScreenElementList *self) {
    int count = self->mCount;
    if (count > 0) {
        int i = 0;
        ScreenElement **ptr = self->mpItem;
        do {
            ScreenElement *item = *ptr;
            int *itemVTable = (int *)item->vtable;
            ((void (*)(ScreenElement *))itemVTable[3])(item);
            ptr++;
            count = self->mCount;
            i++;
        } while (i < count);
    }
    ScreenElement_update(self);
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
void ScreenElementList_draw(ScreenElementList *self, int x, int y) {
    if (self->mHide != 0) return;
    int count = self->mCount;
    if (count <= 0) return;
    int i = 0;
    ScreenElement **ptr = self->mpItem;
    do {
        ScreenElement *item = *ptr;
        ptr++;
        int sx = self->mScreenX + x;
        int sy = self->mScreenY + y;
        int *itemVTable = (int *)item->vtable;
        ((void (*)(ScreenElement *, int, int))itemVTable[4])(item, sx, sy);
        count = self->mCount;
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
TextScreenElement *TextScreenElement_ctor(TextScreenElement *self, char *pString,
                                          Font *pFont, int homeX, int homeY,
                                          int screenX, int screenY) {
    ScreenElement_ctor(self, homeX, homeY, screenX, screenY);
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
void TextScreenElement_draw(TextScreenElement *self, int x, int y) {
    int tx = x;
    register int ty asm("$9") = y;
    if (self->mDelay > 0) return;
    if (self->mHide != 0) return;
    Font *font = self->mpFont;
    int scale = self->mDrawFirst;
    int sx = self->mScreenX + tx;
    int sy = self->mScreenY + ty;
    char *str = self->mpString;
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
int TextScreenElement_width(TextScreenElement *self) {
    return self->mpFont->width(self->mpString);
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
SpriteScreenElement *SpriteScreenElement_ctor(SpriteScreenElement *self, char *pFileName,
                                              int homeX, int homeY, int screenX,
                                              int screenY, int justY, int justX) {
    ScreenElement_ctor(self, homeX, homeY, screenX, screenY);
    self->vtable = (void **)__vt_19SpriteScreenElement;
    Sprite2 *sprite = new Sprite2(pFileName, 1, 64, 64);
    self->mSprite = sprite;
    self->mSprite->SlicedImage2.Image.JustY = (char)justY;
    self->mSprite->SlicedImage2.Image.JustX = (char)justX;
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
        int *spriteVTable = sprite->SlicedImage2.Image.CClass.vtable;
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
void SpriteScreenElement_draw(SpriteScreenElement *self, int x, int y) {
    int tx = x;
    int ty = y;
    if (self->mDelay > 0) return;
    if (self->mHide != 0) return;
    Sprite2 *sprite = self->mSprite;
    int sx = self->mScreenX + tx;
    int sy = self->mScreenY + ty;
    int scale = self->mDrawFirst;
    int *spriteVTable = sprite->SlicedImage2.Image.CClass.vtable;
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
int SpriteScreenElement_width(SpriteScreenElement *self) {
    return self->mSprite->SlicedImage2.screenWidth();
}

// GCC emitted this private `CClass::~CClass` copy last in the owner's text,
// so it stays at the end of the translation unit.
static void ScreenTools_ClassDestructor(CClass *_this, int flags) {
    _this->vtable = (int *)&__vt_base_13ScreenElement;
    if (flags & 1) {
        CClass::operator delete(_this);
    }
}
