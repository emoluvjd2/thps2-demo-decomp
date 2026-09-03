// MENU.cpp -- Reconstructed translation unit
// Original: MENU.CPP
// Functions: 56 reconstructed entries
// Address range: 0x80087A98 - 0x80089824
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.


#define __SKIP_MENUITEM_EXTERNS__
#define __SKIP_BASICMENU_EXTERNS__

#include "MENU.h"
#include "globals.h"

struct Slice;

#include "IMAGEBASE.h"

/** Sliced image used for the optional menu arrows. */
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
};

typedef char menu_image_size_must_be_28[(sizeof(Image) == 28) ? 1 : -1];
typedef char menu_sliced_image_size_must_be_60[
    (sizeof(SlicedImage2) == 60) ? 1 : -1];

#include "SFX.h"
#include "PAD.h"


class ScrollingMenu2;

inline void *operator new(unsigned int, void *p) {
    return p;
}

/** Polymorphic base view matching the original CClass subobject. */
class MenuAssignableCClass {
public:
    virtual void FontTypeAnchor();
};

/** Polymorphic view used to invoke Font's original vtable-preserving assignment. */
class MenuAssignableFont : public MenuAssignableCClass {
public:
    int state[19];
};

typedef char menu_assignable_font_size_must_be_80[
    (sizeof(MenuAssignableFont) == 80) ? 1 : -1];

static inline void SetMenuItemHighlight(VMenuItem *item, int highlighted) {
    ((void (*)(VMenuItem *, int))item->vtable[5])(item, highlighted);
}

static inline void PassMenuItemPress(VMenuItem *item, int press) {
    ((void (*)(VMenuItem *, int))item->vtable[7])(item, press);
}

static inline void SetMenuItemXY(VMenuItem *item, int x, int y) {
    ((void (*)(VMenuItem *, int, int))item->vtable[3])(item, x, y);
}

// === Named ABI call views ===
#include <STDIO.H>
#include <STRINGS.H>
#define MENU_DECLARE_ITEM_CONSTRUCTOR_ABI
#include "MENU.h"
#define VMenuItem_ctor MenuConstructorAbi::__9VMenuItemi
#include "IMAGE.h"

/** Font name whose menu-specific scale is selected below. */
static char s_ingame1_fnt_800dc1b0[12]
    __attribute__((section(".sdata"), aligned(4))) = "ingame1.fnt";

// MENU.H's private small-data tables belong to the original menu.obj.  Keep
// them after the owner's font-name contribution in the target section order.
#define MENU_DECLARE_OWNER_SMALL_DATA_DESTRUCTOR_ABI
#include "MENU.h"

static int Menu_TextPropsVTable
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int Menu_TextPropsVTableIndex __attribute__((section(".sdata"))) = 0;
static void (*Menu_TextPropsVTableDtor)(TextMenuProperties *, int)
    __attribute__((section(".sdata"))) = Menu_DestroyTextMenuPropertiesAbi;

static int Menu_BaseVTable
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int Menu_BaseVTableIndex __attribute__((section(".sdata"))) = 0;
static void (*Menu_BaseVTableDtor)(CClass *, int)
    __attribute__((section(".sdata"))) = __DOT__6CClass_800897F4;

// === Named ABI owner and address views ===
#define SCREENTOOLS_DECLARE_VERTICAL_MENU2_CONSTRUCTOR
#define SCREENTOOLS_AGGREGATE_LAYOUT_ONLY
#include "SCREENTOOLS.h"
#undef SCREENTOOLS_AGGREGATE_LAYOUT_ONLY
#define VerticalMenu2_ctor \
    VerticalMenu2ConstructorABI::__13VerticalMenu2iiiiiii
#define MENU_DECLARE_CONTAINER_OWNER_ABI_HEAD
#define MENU_DECLARE_TEXT_WRAPPER_CONSTRUCTOR_ABI
#define MENU_TEXT_WRAPPER_CONSTRUCTOR_RECEIVER TextMenuWrapper
#define MENU_DECLARE_CONTAINER_OWNER_ABI_TAIL
#include "MENU.h"

// === MENU.H-emitted method bindings owned by menu.obj ===
#define MENU_DECLARE_ITEM_OWNER_DESTRUCTOR_ABI
#include "MENU.h"

typedef void (*MenuVirtualFunction)(void);

#define MENU_DECLARE_OWNER_VTABLES
#include "MENU.h"

// GCC 2.x destructors cannot be named by a C++ address expression, so the two
// natural destructor owners below retain erased ABI views for their tables.
#define MENU_DECLARE_ERASED_BASE_DESTRUCTOR_ABI
#include "MENU.h"

/**
 * MENU diagnostic text and virtual dispatch tables in original read-only
 * section order.
 */
static char processPress_fmt[24] __attribute__((section(".rdata"), aligned(4))) =
    "Highlighted is %d\n";

MenuVirtualFunction const __vt_15TextMenuWrapper[]
    __attribute__((section(".rdata"), aligned(4))) = {
    0,
    0,
    (MenuVirtualFunction)&Menu_TextMenuWrapper_dtor,
    (MenuVirtualFunction)&TextMenuWrapper::HandleInput,
    (MenuVirtualFunction)&TextMenuWrapper::TestComplete,
    (MenuVirtualFunction)&MenuWrapper::GetHighlightedId,
};

MenuVirtualFunction const __vt_11MenuWrapper[]
    __attribute__((section(".rdata"), aligned(4))) = {
    0,
    0,
    MenuV_DtorMenuWrapper,
    (MenuVirtualFunction)&MenuWrapper::HandleInput,
    (MenuVirtualFunction)&MenuWrapper::TestComplete,
    (MenuVirtualFunction)&MenuWrapper::GetHighlightedId,
};

MenuVirtualFunction const __vt_14ScrollingMenu2[]
    __attribute__((section(".rdata"), aligned(4))) = {
    0,
    0,
    (MenuVirtualFunction)&Menu_ScrollingMenu2_dtor,
    (MenuVirtualFunction)&BasicMenu::setArrows,
    (MenuVirtualFunction)&MenuScrollingMenu2ABI::activate__14ScrollingMenu2,
    (MenuVirtualFunction)&ScrollingMenu2::draw,
    (MenuVirtualFunction)&MenuScrollingMenu2ABI::update__14ScrollingMenu2,
    (MenuVirtualFunction)&ScrollingMenu2::processPress,
    (MenuVirtualFunction)&ScrollingMenu2::addItem,
    0,
};

MenuVirtualFunction const __vt_13VerticalMenu2[]
    __attribute__((section(".rdata"), aligned(4))) = {
    0,
    0,
    (MenuVirtualFunction)&Menu_VerticalMenu2_dtor,
    (MenuVirtualFunction)&BasicMenu::setArrows,
    (MenuVirtualFunction)&VerticalMenu2::activate,
    (MenuVirtualFunction)&VerticalMenu2::draw,
    (MenuVirtualFunction)&VerticalMenu2::update,
    (MenuVirtualFunction)&VerticalMenu2::processPress,
    (MenuVirtualFunction)&VerticalMenu2::addItem,
    0,
};

MenuVirtualFunction const __vt_14SelectMenuItem[]
    __attribute__((section(".rdata"), aligned(4))) = {
    0,
    0,
    (MenuVirtualFunction)&Menu_DestroySelectMenuItemAbi,
    (MenuVirtualFunction)&SelectMenuItem::setXY,
    (MenuVirtualFunction)&SelectMenuItem::draw,
    (MenuVirtualFunction)&MenuItem::setHighlightState,
    (MenuVirtualFunction)&MenuItem::setItemState,
    (MenuVirtualFunction)&SelectMenuItem::processPress,
    (MenuVirtualFunction)&MenuItem::update,
    0,
};

MenuVirtualFunction const __vt_13TitleMenuItem[]
    __attribute__((section(".rdata"), aligned(4))) = {
    0,
    0,
    (MenuVirtualFunction)&Menu_DestroyTitleMenuItemAbi,
    (MenuVirtualFunction)&TextMenuItem::setXY,
    (MenuVirtualFunction)&TitleMenuItem::draw,
    (MenuVirtualFunction)&MenuItem::setHighlightState,
    (MenuVirtualFunction)&MenuItem::setItemState,
    (MenuVirtualFunction)&MenuItem::processPress,
    (MenuVirtualFunction)&MenuItem::update,
    0,
};

MenuVirtualFunction const __vt_12TextMenuItem[]
    __attribute__((section(".rdata"), aligned(4))) = {
    0,
    0,
    (MenuVirtualFunction)&Menu_DestroyTextMenuItemAbi,
    (MenuVirtualFunction)&TextMenuItem::setXY,
    (MenuVirtualFunction)&TextMenuItem::draw,
    (MenuVirtualFunction)&MenuItem::setHighlightState,
    (MenuVirtualFunction)&MenuItem::setItemState,
    (MenuVirtualFunction)&MenuItem::processPress,
    (MenuVirtualFunction)&MenuItem::update,
    0,
};

MenuVirtualFunction const __vt_9BasicMenu[]
    __attribute__((section(".rdata"), aligned(4))) = {
    0,
    0,
    MenuV_DtorBasicMenu,
    (MenuVirtualFunction)&BasicMenu::setArrows,
    (MenuVirtualFunction)&BasicMenu::activate,
    (MenuVirtualFunction)&BasicMenu::draw,
    (MenuVirtualFunction)&BasicMenu::update,
    (MenuVirtualFunction)&BasicMenu::processPress,
    (MenuVirtualFunction)&BasicMenu::addItem,
    0,
};

static MenuVirtualFunction __vt_9VMenuItem[]
    __attribute__((section(".rdata"), aligned(4))) = {
    0,
    0,
    (MenuVirtualFunction)&Menu_DestroyVMenuItemAbi,
    (MenuVirtualFunction)&MenuItem::setXY,
    (MenuVirtualFunction)&MenuItem::draw,
    (MenuVirtualFunction)&MenuItem::setHighlightState,
    (MenuVirtualFunction)&MenuItem::setItemState,
    (MenuVirtualFunction)&MenuItem::processPress,
    (MenuVirtualFunction)&MenuItem::update,
    0,
};

MenuVirtualFunction const __vt_8MenuItem[]
    __attribute__((section(".rdata"), aligned(4))) = {
    0,
    0,
    (MenuVirtualFunction)&MenuItem_dtor,
    (MenuVirtualFunction)&MenuItem::setXY,
    (MenuVirtualFunction)&MenuItem::draw,
    (MenuVirtualFunction)&MenuItem::setHighlightState,
    (MenuVirtualFunction)&MenuItem::setItemState,
    (MenuVirtualFunction)&MenuItem::processPress,
    (MenuVirtualFunction)&MenuItem::update,
};

// === Function implementations (address order) ===

/**
 * Menu_GetPress__Fv
 *
 * Address:   0x80087A98
 * Mangled:   Menu_GetPress__Fv
 * Demangled: Menu_GetPress(void)
 *
 * @return result
 */
int Menu_GetPress(void) {
    SControl *pad = &Pad[0];
    int press = -1;
    if (pad->Left.Triggered != 0) {
        pad->Left.Triggered = 0;
        press = 0;
        goto done;
    }
    if (pad->Right.Triggered != 0) {
        pad->Right.Triggered = 0;
        press = 1;
        goto done;
    }
    if (pad->Up.Triggered != 0) {
        pad->Up.Triggered = 0;
        press = 2;
        goto done;
    }
    if (pad->Down.Triggered != 0) {
        pad->Down.Triggered = 0;
        press = 3;
    }
done:
    return press;
}


/**
 *
 * Address:   0x80087B14
 * Mangled:   __8MenuItemi
 * Demangled: MenuItem::MenuItem(int)
 *
 * @param id  Identifier.
 */
MenuItem::MenuItem(int id) {
    this->vtable = (int *)&__vt_8MenuItem[0];
    this->ID = id;
}


/**
 *
 * Address:   0x80087B2C
 * Mangled:   _._8MenuItem
 * Demangled: MenuItem::~MenuItem()
 */
MenuItem::~MenuItem() {
    this->vtable = &Menu_BaseVTable;
}


/**
 *
 * Address:   0x80087B5C
 * Mangled:   setHighlightState__8MenuItemb
 * Demangled: MenuItem::setHighlightState(bool)
 *
 * @param highlight_state  New highlight state.
 */
void MenuItem::setHighlightState(bool highlight_state) {
    this->Highlighted = highlight_state;
}


/**
 *
 * Address:   0x80087B64
 * Mangled:   setItemState__8MenuItemi
 * Demangled: MenuItem::setItemState(int)
 *
 * @param state  New interaction state for the item.
 */
void MenuItem::setItemState(int state) {
    this->InternalState = state;
}


/**
 *
 * Address:   0x80087B6C
 * Mangled:   processPress__8MenuItemi
 * Demangled: MenuItem::processPress(int)
 *
 * @param press  Button press code.
 */
void MenuItem::processPress(int press) {
    printf(processPress_fmt, this->Highlighted);
}


/**
 *
 * Address:   0x80087B94
 * Mangled:   update__8MenuItem
 * Demangled: MenuItem::update(void)
 */
void MenuItem::update() {
}


/**
 *
 * Address:   0x80087B9C
 * Mangled:   __9VMenuItemi
 * Demangled: VMenuItem::VMenuItem(int)
 *
 */
VMenuItem::VMenuItem(int id) : MenuItem(id) {
    this->vtable = (int *)&__vt_9VMenuItem[0];
}


/**
 *
 * Address:   0x80087BD0
 * Mangled:   GetHighlightedId__11MenuWrapper
 * Demangled: MenuWrapper::GetHighlightedId(void)
 *
 * @return Identifier cached for the highlighted menu item.
 */
int MenuWrapper::GetHighlightedId() {
    return this->HighlightedId;
}


/**
 *
 * Address:   0x80087BDC
 * Mangled:   __18TextMenuProperties
 * Demangled: TextMenuProperties::TextMenuProperties(void)
 */
TextMenuProperties::TextMenuProperties() {
    this->vtable = &Menu_TextPropsVTable;
    ::new (&this->TheFont) Font();
    this->BigScale = 0x1324;
    this->NormalScale = 0x1000;
    this->SelectedR = 0xB6;
    this->SelectedG = 0x84;
    this->SelectedB = 0x24;
    this->NormalR = 0x7A;
    this->NormalG = 0x7A;
    this->NormalB = 0x7A;
    this->GrayR = 0x40;
    this->GrayG = 0x40;
    this->GrayB = 0x40;
}


/**
 *
 * Address:   0x80087C58
 * Mangled:   __12TextMenuItemiPCcR18TextMenuProperties
 * Demangled: TextMenuItem::TextMenuItem(int, const char *, TextMenuProperties &)
 *            Note: kept as a named ABI owner because natural C++ base construction generates cleanup paths
 *
 * @param self  Implicit object pointer.
 * @param id  Identifier.
 * @param text  Label displayed by the item.
 * @param properties  Shared font and styling properties.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
TextMenuItem *__12TextMenuItemiPCcR18TextMenuProperties(
    TextMenuItem *self, int id, const char *text, TextMenuProperties *properties) {
    VMenuItem_ctor(self, id);
    self->vtable = (int *)__vt_12TextMenuItem;
    strcpy(self->Text, text);
    self->ID = id;
    self->TheProperties = properties;
    self->Selectable = 1;
    self->Choosable = 1;
    properties->TheFont.Scale = properties->BigScale;
    int w = FontFromStorage(&self->TheProperties->TheFont)->width(self->Text);
    self->W = w;
    int h = FontFromStorage(&self->TheProperties->TheFont)->height(self->Text);
    self->H = h;
    return self;
}


/**
 *
 * Address:   0x80087D10
 * Mangled:   _._12TextMenuItem
 * Demangled: TextMenuItem::~TextMenuItem()
 *            Note: kept as a named ABI owner because natural C++ destruction changes the in-charge flag
 *
 * @param self  Implicit object pointer.
 * @param flag  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
void Menu_DestroyTextMenuItemAbi(
    TextMenuItem *self, int __in_chrg) {
    self->vtable = (int *)__vt_12TextMenuItem;
    MenuItem_dtor(self, 0);
    if (__in_chrg & 1) {
        ::CClass::operator delete(self);
    }
}


/**
 *
 * Address:   0x80087D64
 * Mangled:   setXY__12TextMenuItemii
 * Demangled: TextMenuItem::setXY(int, int)
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 */
void TextMenuItem::setXY(int x, int y) {
    this->X = x;
    this->Y = y;
}


/**
 * Draws a text menu item with the current selection and disabled-state styling.
 *
 * Address:   0x80087D70
 * Mangled:   draw__12TextMenuItem
 * Demangled: TextMenuItem::draw(void)
 *
 */
void TextMenuItem::draw() {
    TextMenuProperties *props = TheProperties;
    props->TheFont.JustX = JustX;
    TextMenuProperties *props2 = TheProperties;
    props2->TheFont.JustY = 1;
    TextMenuProperties *font = TheProperties;
    font->TheFont.Scale = font->NormalScale;
    if (Selectable == 0) {
        TextMenuProperties *p = TheProperties;
        p->TheFont.R = p->GrayR;
        TextMenuProperties *p2 = TheProperties;
        p2->TheFont.G = p2->GrayG;
        TextMenuProperties *p3 = TheProperties;
        p3->TheFont.B = p3->GrayB;
    } else if (Highlighted == 0) {
        TextMenuProperties *p = TheProperties;
        p->TheFont.R = p->NormalR;
        TextMenuProperties *p2 = TheProperties;
        p2->TheFont.G = p2->NormalG;
        TextMenuProperties *p3 = TheProperties;
        p3->TheFont.B = p3->NormalB;
    } else {
        TextMenuProperties *p = TheProperties;
        p->TheFont.Scale = p->BigScale;
        TextMenuProperties *p2 = TheProperties;
        p2->TheFont.R = p2->SelectedR;
        TextMenuProperties *p3 = TheProperties;
        p3->TheFont.G = p3->SelectedG;
        TextMenuProperties *p4 = TheProperties;
        p4->TheFont.B = p4->SelectedB;
    }
    if (TheProperties->TheFont.JustX == 1) {
        FontFromStorage(&TheProperties->TheFont)->draw(
            X + (W >> 1), Y + (H >> 1), Text, 0);
    } else {
        FontFromStorage(&TheProperties->TheFont)->draw(
            X, Y + (H >> 1), Text, 0);
    }
}


/**
 *
 * Address:   0x80087F0C
 * Mangled:   __13TitleMenuItemiPCcR18TextMenuProperties
 * Demangled: TitleMenuItem::TitleMenuItem(int, const char *, TextMenuProperties &)
 *            Note: kept as a named ABI owner because natural C++ base construction generates cleanup paths
 *
 * @param self  Implicit object pointer.
 * @param id  Identifier.
 * @param text  Label displayed by the title.
 * @param properties  Shared font and styling properties.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
TitleMenuItem *__13TitleMenuItemiPCcR18TextMenuProperties(
    TitleMenuItem *self, int id, const char *text, TextMenuProperties *properties) {
    __12TextMenuItemiPCcR18TextMenuProperties(self, id, text, properties);
    self->vtable = (int *)&__vt_13TitleMenuItem[0];
    self->Selectable = 0;
    int h = self->H;
    h = h + (h >> 1);
    self->H = h;
    return self;
}


/**
 * Draws a title menu item with the selected title color and scale.
 *
 * Address:   0x80087F54
 * Mangled:   draw__13TitleMenuItem
 * Demangled: TitleMenuItem::draw(void)
 *
 */
void TitleMenuItem::draw() {
    TextMenuProperties *props = TheProperties;
    props->TheFont.JustX = JustX;
    TextMenuProperties *props2 = TheProperties;
    props2->TheFont.JustY = 1;
    TextMenuProperties *fnt = TheProperties;
    fnt->TheFont.Scale = fnt->NormalScale;
    TextMenuProperties *p2 = TheProperties;
    p2->TheFont.R = p2->SelectedR;
    TextMenuProperties *p3 = TheProperties;
    p3->TheFont.G = p3->SelectedG;
    TextMenuProperties *p4 = TheProperties;
    p4->TheFont.B = p4->SelectedB;
    if (TheProperties->TheFont.JustX == 1) {
        FontFromStorage(&TheProperties->TheFont)->draw(
            X + (W >> 1), Y + (H >> 1), Text, 0);
    } else {
        FontFromStorage(&TheProperties->TheFont)->draw(
            X, Y + (H >> 1), Text, 0);
    }
}


/**
 *
 * Address:   0x80088044
 * Mangled:   _._14SelectMenuItem
 * Demangled: SelectMenuItem::~SelectMenuItem()
 *
 * @param self  Implicit object pointer.
 * @param flag  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
void Menu_DestroySelectMenuItemAbi(
    SelectMenuItem *self, int __in_chrg) {
    self->vtable = (int *)__vt_14SelectMenuItem;
    MenuItem_dtor(self, 0);
    if (__in_chrg & 1) {
        ::CClass::operator delete(self);
    }
}


/**
 *
 * Address:   0x80088098
 * Mangled:   setXY__14SelectMenuItemii
 * Demangled: SelectMenuItem::setXY(int, int)
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 */
void SelectMenuItem::setXY(int x, int y) {
    X = x;
    Y = y;
}


/**
 * Match the original PSX draw-path shape so register allocation stays honest.
 * draw__14SelectMenuItem
 *
 * Address:   0x800880A4
 * Mangled:   draw__14SelectMenuItem
 * Demangled: SelectMenuItem::draw(void)
 *            Reuses the scale local as the denominator to mirror the original draw register lifetime.
 *
 */
void SelectMenuItem::draw(void) {
    TextMenuProperties *props = TheProperties;
    props->TheFont.JustX = 0;
    TextMenuProperties *props2 = TheProperties;
    props2->TheFont.JustY = 1;
    if (Highlighted != 0) {
        TextMenuProperties *p = TheProperties;
        p->TheFont.Scale = p->BigScale;
    } else {
        TextMenuProperties *p = TheProperties;
        p->TheFont.Scale = p->NormalScale;
    }
    int x;
    if (JustX == 1 && AmHighlighted == 0) {
        int scale = (int)TheProperties;
        int w = W;
        int scaled = w * ((TextMenuProperties *)scale)->NormalScale;
        scale = ((TextMenuProperties *)scale)->BigScale;
        scaled = scaled / scale;
        x = X + (w >> 1) - (scaled >> 1);
    } else {
        x = X;
    }
    int count = NumOptions;
    if (count > 0) {
        int i = 0;
        char *str = Option[0];
        do {
            TextMenuProperties *p = TheProperties;
            if (i == 0 || i == InternalState) {
                if (Highlighted != 0) {
                    p->TheFont.R = p->SelectedR;
                    TextMenuProperties *p2 = TheProperties;
                    p2->TheFont.G = p2->SelectedG;
                    TextMenuProperties *p3 = TheProperties;
                    p3->TheFont.B = p3->SelectedB;
                } else {
                    p->TheFont.R = p->NormalR;
                    TextMenuProperties *p2 = TheProperties;
                    p2->TheFont.G = p2->NormalG;
                    TextMenuProperties *p3 = TheProperties;
                    p3->TheFont.B = p3->NormalB;
                }
            } else {
                p->TheFont.R = p->GrayR;
                TextMenuProperties *p2 = TheProperties;
                p2->TheFont.G = p2->GrayG;
                TextMenuProperties *p3 = TheProperties;
                p3->TheFont.B = p3->GrayB;
            }
            TextMenuProperties *fnt = TheProperties;
            int h = H;
            int y = Y;
            FontFromStorage(&fnt->TheFont)->draw(x, y + (h >> 1), str, 0);
            i++;
            TextMenuProperties *fnt2 = TheProperties;
            x += FontFromStorage(&fnt2->TheFont)->width(str);
            str += 0x20;
        } while (i < NumOptions);
    }
}

/**
 * Process a directional press while this selectable item is highlighted.
 *
 * Address:   0x800882C0
 * Mangled:   processPress__14SelectMenuItemi
 * Demangled: SelectMenuItem::processPress(int)
 *
 * @param press  Button press code.
 */
void SelectMenuItem::processPress(int press) {
    if (Highlighted == 0) return;
    if (press != 0) goto up;
    {
        int state = InternalState - 1;
        InternalState = state;
        if (state > 0) return;
        int wrap = NumOptions - 1;
        InternalState = wrap;
        return;
    }
up:
    if (press != 1) return;
    {
        int state = InternalState;
        int max = NumOptions;
        state++;
        InternalState = state;
        if (state < max) return;
        InternalState = press;
    }
}


/**
 * setArrows__9BasicMenuP12SlicedImage2T1i
 *
 * Address:   0x80088330
 * Mangled:   setArrows__9BasicMenuP12SlicedImage2T1i
 * Demangled: BasicMenu::setArrows(SlicedImage2 *, SlicedImage2 *, int)
 *            Sets up/down arrow references and configures their fields
 *
 * @param left  Up/left navigation arrow sprite.
 * @param right  Down/right navigation arrow sprite.
 * @param spacing  Horizontal offset applied to the arrows.
 */
void BasicMenu::setArrows(
    SlicedImage2 *left, SlicedImage2 *right, int spacing) {
    LeftArrow = left;
    RightArrow = right;
    ArrowSpace = spacing;
    if (left) {
        left->JustX = 2;
        SlicedImage2 *a = LeftArrow;
        a->JustY = 1;
    }
    {
        SlicedImage2 *d = RightArrow;
        if (d) {
            d->JustX = 0;
            SlicedImage2 *d2 = RightArrow;
            d2->JustY = 1;
        }
    }
}


/**
 * Return the identifier of the currently highlighted menu item.
 *
 * Address:   0x80088380
 * Mangled:   getHighlighted__9BasicMenu
 * Demangled: BasicMenu::getHighlighted(void)
 *            Searches items array for highlighted item, returns its id or -1
 *
 * @return Identifier of the highlighted item, or -1 when none is highlighted.
 */
int BasicMenu::getHighlighted(void) {
    int i = 0;
    VMenuItem **ptr = Item;

    do {
        VMenuItem *item = *ptr;
        if (item != 0) {
            if (item->Highlighted != 0) {
                return item->ID;
            }
        }
        i++;
        ptr++;
    } while (i < 16);
    return -1;
}


/**
 * Highlights the menu item with the requested identifier.
 *
 * Address:   0x800883CC
 * Mangled:   setHighlighted__9BasicMenui
 *
 * @param id Item identifier to select.
 */
void BasicMenu::setHighlighted(int id) {
    VMenuItem *item = Item[Selected];
    ((void (*)(VMenuItem *, int))item->vtable[5])(item, 0);
    Selected = 0;
    while (1) {
        int selected = Selected;
        VMenuItem *current = Item[selected];
        if (current != 0 && current->ID == id) break;
        int next = selected + 1;
        Selected = next;
        if (next >= 0x10) break;
    }
    item = Item[Selected];
    ((void (*)(VMenuItem *, int))item->vtable[5])(item, 1);
}


/**
 * __13VerticalMenu2iiiiiii (typed ABI owner)
 *
 * Address:   0x800884A8
 * Mangled:   __13VerticalMenu2iiiiiii
 * Demangled: VerticalMenu2::(int, int, int, int, int, int, int)
 *
 * @param self  Implicit object pointer.
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param w  Width.
 * @param h  Height.
 * @param just_x  Horizontal item alignment.
 * @param just_y  Vertical menu alignment.
 * @param spacing  Vertical gap between items.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
VerticalMenu2 *VerticalMenu2_ctor(
    VerticalMenu2 *self, int x, int y, int w, int h, int just_x, int just_y,
    int spacing) {
    int i = 0;
    VMenuItem **ptr = self->Item;
    self->vtable = (int *)&__vt_13VerticalMenu2[0];
    do {
        *ptr = 0;
        i++;
        ptr++;
    } while (i < 16);
    self->H = h;
    self->JustX = just_x;
    self->HighestSlot = 0;
    self->X = x;
    self->Y = y;
    self->W = w;
    self->RightArrow = 0;
    self->LeftArrow = 0;
    self->ArrowSpace = 0;
    self->Selected = 0;
    self->JustY = just_y;
    self->Spacing = spacing;
    return self;
}


/**
 * _._13VerticalMenu2 (typed ABI owner)
 *
 * Address:   0x80088518
 * Mangled:   _._13VerticalMenu2
 * Demangled: VerticalMenu2::~VerticalMenu2()
 *
 * @param self  Implicit object pointer.
 * @param flag  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
void Menu_VerticalMenu2_dtor(VerticalMenu2 *self, int flag) {
    self->vtable = &Menu_BaseVTable;
    if (flag & 1) {
        ::CClass::operator delete(self);
    }
}


/**
 * Lay out the vertical menu entries and highlight the selected entry.
 *
 * Address:   0x80088548
 * Mangled:   activate__13VerticalMenu2
 * Demangled: VerticalMenu2::activate(void)
 */
void VerticalMenu2::activate() {
    int total_h;
    int initialCount;
    VMenuItem **items;
    {
        int i = 0;
        total_h = 0;
        initialCount = HighestSlot;
        items = Item;
        if (initialCount > 0) {
            int spacing = Spacing;
            VMenuItem **ptr = items;
            do {
                VMenuItem *item = *ptr;
                ptr++;
                i++;
                int h = item->H;
                h += spacing;
                total_h += h;
            } while (i < initialCount);
        }
    }
    int y;
    if (JustY == 1) {
        int space = H - total_h;
        y = Y + (space >> 1);
    } else {
        y = Y;
    }
    int arrow_x = ArrowSpace;
    SlicedImage2 *upArrow = LeftArrow;
    if (upArrow != 0) {
        int h = upArrow->W;
        h = h << 1;
        arrow_x += h;
    }
    {
        int i = 0;
        if (initialCount > 0) {
            VMenuItem **ptr = items;
            do {
                (*ptr)->JustX = JustX;
                int alignment = JustX;
                if (alignment == 1) {
                    int halfW = W >> 1;
                    int centerX = X + halfW;
                    VMenuItem *item = *ptr;
                    int itemHalfW = item->W >> 1;
                    ((void (*)(VMenuItem *, int, int))item->vtable[3])(
                        item, centerX - itemHalfW, y);
                } else {
                    VMenuItem *item = *ptr;
                    ((void (*)(VMenuItem *, int, int))item->vtable[3])(
                        item, X + arrow_x, y);
                }
                int h = (*ptr)->H;
                ptr++;
                i++;
                int spacing = Spacing;
                h += spacing;
                y += h;
            } while (i < HighestSlot);
        }
    }
    {
        int i = 0;
        int count = HighestSlot;
        for (; i < count; i++) {
            if (Item[Selected]->Selectable == 0) {
                Selected++;
                if (Selected == count) {
                    Selected = 0;
                }
                break;
            }
            count = HighestSlot;
        }
    }
    SetMenuItemHighlight(Item[Selected], 1);
}


/**
 * Draw every visible entry and position the optional selection arrows.
 *
 * Address:   0x80088758
 * Mangled:   draw__13VerticalMenu2
 * Demangled: VerticalMenu2::draw(void)
 *
 */
void VerticalMenu2::draw() {
    SlicedImage2 *upArrow = LeftArrow;
    if (upArrow != 0) {
        VMenuItem *item = Item[Selected];
        int x = item->X - ArrowSpace;
        int y = item->Y + (item->H >> 1);
        int *vt = upArrow->CClass.vtable;
        ((void (*)(SlicedImage2 *, int, int, int))vt[5])(
            upArrow, x, y + (Spacing >> 1), 0);
    }
    VMenuItem **items = Item;
    SlicedImage2 *downArrow = RightArrow;
    if (downArrow != 0) {
        int itemOffset = Selected * sizeof(*items);
        char *itemAddress = (char *)items;
        itemAddress += itemOffset;
        VMenuItem **itemSlot = (VMenuItem **)itemAddress;
        VMenuItem *item = *itemSlot;
        int x = item->X + item->W + ArrowSpace;
        int y = item->Y + (item->H >> 1);
        int *vt = downArrow->CClass.vtable;
        ((void (*)(SlicedImage2 *, int, int, int))vt[5])(
            downArrow, x, y + (Spacing >> 1), 0);
    }
    int i = 15;
    VMenuItem **ptr = &Item[15];
    do {
        VMenuItem *item = *ptr;
        if (item != 0) {
            ((void (*)(VMenuItem *))item->vtable[4])(item);
        }
        i--;
        ptr--;
    } while (i >= 0);
}


/**
 * Update each populated item in this vertical menu.
 *
 * Address:   0x80088878
 * Mangled:   update__13VerticalMenu2
 * Demangled: VerticalMenu2::update(void)
 *            Iterates items, calling each item's virtual update method at vtable[8]
 *
 */
void VerticalMenu2::update(void) {
    int count = HighestSlot;
    if (count > 0) {
        int i = 0;
        VMenuItem **ptr = Item;
        do {
            VMenuItem *item = *ptr;
            int *vtable = item->vtable;
            ((void (*)(VMenuItem *))vtable[8])(item);
            ptr++;
            count = HighestSlot;
            i++;
        } while (i < count);
    }
}


/**
 * Pass a press to the selected item and move the highlight through selectable
 * entries, wrapping at either end of the menu.
 *
 * Address:   0x800888F0
 * Mangled:   processPress__13VerticalMenu2i
 * Demangled: VerticalMenu2::processPress(int)
 *
 * @param press  Button press code.
 */
void VerticalMenu2::processPress(int press) {
    SetMenuItemHighlight(Item[Selected], 0);

    if ((unsigned int)press < 2) {
        PassMenuItemPress(Item[Selected], press);
    }

    if (press == 2) {
        do {
            Selected--;
            if (Selected < 0) {
                Selected = HighestSlot - 1;
            }
        } while (Item[Selected]->Selectable == 0);
    }

    if (press == 3) {
        do {
            Selected++;
            if (Selected >= HighestSlot) {
                Selected = 0;
            }
        } while (Item[Selected]->Selectable == 0);
    }

    SetMenuItemHighlight(Item[Selected], 1);
}


/**
 *
 * Address:   0x80088A60
 * Mangled:   addItem__13VerticalMenu2P9VMenuItem
 * Demangled: VerticalMenu2::addItem(VMenuItem *)
 *
 * @param item  Menu item to append.
 */
void VerticalMenu2::addItem(VMenuItem *item) {
    int count = this->HighestSlot;
    this->Item[count] = item;
    this->HighestSlot = this->HighestSlot + 1;
}


/**
 * __14ScrollingMenu2iiiiii (typed ABI owner)
 *
 * Address:   0x80088A88
 * Mangled:   __14ScrollingMenu2iiiiii
 * Demangled: ScrollingMenu2::(int, int, int, int, int, int)
 *
 * @param self  Implicit object pointer.
 * @param x  X coordinate.
 * @param y  Y coordinate.
 * @param w  Width.
 * @param h  Height.
 * @param just_x  Horizontal item alignment.
 * @param slide_amount  Distance moved during each scrolling step.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
ScrollingMenu2 *__14ScrollingMenu2iiiiii(
    ScrollingMenu2 *self, int x, int y, int w, int h, int just_x,
    int slide_amount) {
    int i = 0;
    VMenuItem **ptr = self->Item;
    self->vtable = (int *)&__vt_14ScrollingMenu2[0];
    do {
        *ptr = 0;
        i++;
        ptr++;
    } while (i < 16);
    self->HighestSlot = 0;
    self->X = x;
    self->Y = y;
    self->W = w;
    self->H = h;
    self->RightArrow = 0;
    self->LeftArrow = 0;
    self->ArrowSpace = 0;
    self->Top = 0;
    self->Bottom = h;
    self->Selected = 0;
    self->JustX = just_x;
    self->ArrowSlide = slide_amount;
    return self;
}


/**
 * _._14ScrollingMenu2 (typed ABI owner)
 *
 * Address:   0x80088AF8
 * Mangled:   _._14ScrollingMenu2
 * Demangled: ScrollingMenu2::~ScrollingMenu2()
 *
 * @param self  Implicit object pointer.
 * @param flag  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
void Menu_ScrollingMenu2_dtor(ScrollingMenu2 *self, int flag) {
    self->vtable = &Menu_BaseVTable;
    if (flag & 1) {
        ::CClass::operator delete(self);
    }
}


/**
 * activate__14ScrollingMenu2 (typed ABI owner)
 *
 * Address:   0x80088B28
 * Mangled:   activate__14ScrollingMenu2
 * Demangled: ScrollingMenu2::activate(void)
 *
 * @param self  Implicit object pointer.
 */
void MenuScrollingMenu2ABI::activate__14ScrollingMenu2(ScrollingMenu2 *self) {
    int arrow_x = self->ArrowSpace;
    SlicedImage2 *upArrow = self->LeftArrow;
    if (upArrow != 0) {
        int h = upArrow->W;
        h = h << 1;
        arrow_x += h;
    }
    int j = 0;
    VMenuItem **items = self->Item;
    int y = self->Y - self->Top;
    if (self->HighestSlot > 0) {
        VMenuItem **ptr = items;
        do {
            (*ptr)->JustX = self->JustX;
            int alignment = self->JustX;
            if (alignment == 1) {
                int halfW = self->W >> 1;
                int centerX = self->X + halfW;
                VMenuItem *item = *ptr;
                int itemHalfW = item->W >> 1;
                int *vt = item->vtable;
                ((void (*)(VMenuItem *, int, int))vt[3])(
                    item, centerX - itemHalfW, y);
            } else {
                VMenuItem *item = *ptr;
                int *vt = item->vtable;
                ((void (*)(VMenuItem *, int, int))vt[3])(
                    item, self->X + arrow_x, y);
            }
            int h = (*ptr)->H;
            ptr++;
            j++;
            y += h;
        } while (j < self->HighestSlot);
    }
    int count = self->HighestSlot;
    ScrollingMenu2 *menu = self;
    for (int i = 0; i < count; i++) {
        if (menu->Item[menu->Selected]->Selectable == 0) {
            menu->Selected++;
            if (menu->Selected == count) {
                menu->Selected = 0;
            }
            break;
        }
        count = self->HighestSlot;
    }

    SetMenuItemHighlight(menu->Item[menu->Selected], 1);
}


/**
 * Draw this scrolling menu and its navigation arrows.
 *
 * Address:   0x80088CD8
 * Mangled:   draw__14ScrollingMenu2
 * Demangled: ScrollingMenu2::draw(void)
 *
 */
void ScrollingMenu2::draw(void) {
    int idx = Selected;
    VMenuItem **items_base = Item;
    char *itemAddress = (char *)items_base;
    itemAddress += idx * sizeof(*items_base);
    VMenuItem **itemSlot = (VMenuItem **)itemAddress;
    VMenuItem *item = *itemSlot;
    int halfH = item->H >> 1;
    int y = item->Y + halfH;
    int startY = Y;
    if (y < startY) {
        y = startY;
    } else {
        int endY = startY + H;
        if (y >= endY) {
            y = endY - 1;
        }
    }
    SlicedImage2 *upArrow = LeftArrow;
    if (upArrow != 0) {
        int curIdx = Selected;
        char *itemAddress = (char *)items_base;
        itemAddress += curIdx * sizeof(*items_base);
        VMenuItem **itemSlot = (VMenuItem **)itemAddress;
        VMenuItem *curItem = *itemSlot;
        int *vt = upArrow->CClass.vtable;
        int x = curItem->X - ArrowSpace;
        ((void (*)(SlicedImage2 *, int, int, int))vt[5])(
            upArrow, x, y, 0);
    }
    SlicedImage2 *downArrow = RightArrow;
    if (downArrow != 0) {
        int curIdx = Selected;
        char *itemAddress = (char *)items_base;
        itemAddress += curIdx * sizeof(*items_base);
        VMenuItem **itemSlot = (VMenuItem **)itemAddress;
        VMenuItem *curItem = *itemSlot;
        int x = curItem->X + curItem->W + ArrowSpace;
        int *vt = downArrow->CClass.vtable;
        ((void (*)(SlicedImage2 *, int, int, int))vt[5])(
            downArrow, x, y, 0);
    }
    SetClippingBox2(0, 0, 0x200, 0x100);
    int i = 15;
    VMenuItem **ptr = &items_base[15];
    do {
        VMenuItem *it = *ptr;
        if (it != 0) {
            int *vt = it->vtable;
            ((void (*)(VMenuItem *))vt[4])(it);
        }
        i--;
        ptr--;
    } while (i >= 0);
    SetClippingBox2(X, Y, W, H);
}


/**
 * update__14ScrollingMenu2 (typed ABI owner)
 *
 * Address:   0x80088E60
 * Mangled:   update__14ScrollingMenu2
 * Demangled: ScrollingMenu2::update(void)
 *
 * @param self  Implicit object pointer.
 */
void MenuScrollingMenu2ABI::update__14ScrollingMenu2(ScrollingMenu2 *self) {
    int *offsets = self->RY;
    int idx = self->Selected;
    int idxOff = idx << 2;
    int *offsetPtr = (int *)((char *)offsets + idxOff);
    int curOffset = *offsetPtr;
    int scrollPos = self->Top;
    if (curOffset < scrollPos) {
        int speed = self->ArrowSlide;
        int newPos = scrollPos - speed;
        int newBottom = self->Bottom - speed;
        self->Top = newPos;
        self->Bottom = newBottom;
        int ofs = *offsetPtr;
        if (newPos < ofs) {
            self->Top = ofs;
            self->Bottom = ofs + self->H;
        }
    } else {
        VMenuItem **itemsBase = self->Item;
        VMenuItem **itemSlot = (VMenuItem **)((char *)itemsBase + idxOff);
        VMenuItem *item = *itemSlot;
        int itemH = item->H;
        int bottom = curOffset + itemH;
        int viewBottom = self->Bottom;
        if (viewBottom < bottom) {
            int speed = self->ArrowSlide;
            int newPos = scrollPos + speed;
            int newBottom = viewBottom + speed;
            self->Top = newPos;
            self->Bottom = newBottom;
            VMenuItem *item2 = *itemSlot;
            int ofs2 = *offsetPtr;
            int itemH2 = item2->H;
            int top = ofs2 + itemH2;
            if (top < newBottom) {
                self->Bottom = top;
                self->Top = top - self->H;
            }
        }
    }
    {
        int *vt = self->vtable;
        ((void (*)(ScrollingMenu2 *))vt[4])(self);
    }
    int count = self->HighestSlot;
    if (count > 0) {
        int i = 0;
        VMenuItem **ptr = self->Item;
        do {
            VMenuItem *item = *ptr;
            int *vt = item->vtable;
            ((void (*)(VMenuItem *))vt[8])(item);
            ptr++;
            count = self->HighestSlot;
            i++;
        } while (i < count);
    }
}


/**
 * Pass a press to the selected item and move to the next selectable entry
 * without wrapping beyond the ends of a scrolling menu.
 *
 * Address:   0x80088FC4
 * Mangled:   processPress__14ScrollingMenu2i
 * Demangled: ScrollingMenu2::processPress(int)
 *
 * @param press  Button press code.
 */
void ScrollingMenu2::processPress(int press) {
    /*
     * The integer cursor form is retained after three typed Item/Selected
     * rewrites each reversed a target commutative addu operand. All other
     * ScrollingMenu2 field access is typed; this is the measured exception.
     */
    int self = (int)this;
    int *items = (int *)(self + 0x10);
    SetMenuItemHighlight(Item[Selected], 0);
    if ((unsigned int)press < 2) {
        PassMenuItemPress(Item[Selected], press);
    }
    if (press == 2) {
        int target = *(int *)(self + 0x54) - 1;
        if (target >= 0) {
            int *ptr = (int *)(target * 4 + (int)items);
            do {
                int *cur = (int *)*ptr;
                if (*(int *)((int)cur + 8) != 0) goto found_up;
                target--;
                ptr--;
            } while (target >= 0);
        }
    found_up:
        if (target >= 0) {
            *(int *)(self + 0x54) = target;
        }
    }
    if (press == 3) {
        int target = Selected + 1;
        int highestSlot = HighestSlot;
        if (target < highestSlot) {
            int *ptr = (int *)(target * 4 + (int)items);
            do {
                int *cur = (int *)*ptr;
                if (*(int *)((int)cur + 8) != 0) goto found_down;
                target++;
                ptr++;
            } while (target < highestSlot);
        found_down:
            if (target < HighestSlot) {
                Selected = target;
            }
        }
    }
    SetMenuItemHighlight(Item[Selected], 1);
}


/**
 * Append one item and record its cumulative scroll offset.
 *
 * Address:   0x80089140
 * Mangled:   addItem__14ScrollingMenu2P9VMenuItem
 * Demangled: ScrollingMenu2::addItem(VMenuItem *)
 *            Appends an item and records its cumulative scroll offset.
 *
 * @param item  Menu item to append.
 */
void ScrollingMenu2::addItem(VMenuItem *item) {
    int count = HighestSlot;
    VMenuItem **items = Item;
    VMenuItem **itemSlot =
        (VMenuItem **)((char *)items + (count << 2));
    *itemSlot = item;
    count = HighestSlot;
    register int idx asm("$8") = count << 2;
    if (count != 0) {
        goto nonzero;
    }
    {
        RY[0] = 0;
        goto done_offsets;
    }
nonzero:
    {
        int *offsets = RY;
        int prevIdx = (count - 1) << 2;
        register int *prevOffPtr =
            (int *)((char *)offsets + prevIdx);
        VMenuItem **prevItemPtr =
            (VMenuItem **)((char *)items + prevIdx);
        VMenuItem *prevItem = *prevItemPtr;
        int prevOff = *prevOffPtr;
        int height = prevItem->H;
        int *offsetSlot = (int *)((char *)offsets + idx);
        *offsetSlot = prevOff + height;
    }
done_offsets:
    count = HighestSlot;
    HighestSlot = count + 1;
}


/**
 * Builds a text-menu wrapper, its menu container, and all supplied entries.
 *
 * Address:   0x800891B0
 * Mangled:   __15TextMenuWrapperPCPcPiiPcb
 * Demangled: TextMenuWrapper::(const char **, int *, int, char *, bool)
 *
 * @param self  Wrapper being constructed.
 * @param string_list  Text for each menu entry.
 * @param id_list  Identifier for each entry; -1 denotes a title.
 * @param num_items  Number of entries to create.
 * @param font_name  Font selected for the menu.
 * @param use_scrolling_menu  Selects the scrolling rather than vertical layout.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
TextMenuWrapper *__15TextMenuWrapperPCPcPiiPcb(
    TextMenuWrapper *self, const char **string_list, int *id_list,
    int num_items, const char *font_name, bool use_scrolling_menu)
{
    TextMenuProperties *properties = &self->Props;
    self->vtable = (int *)__vt_15TextMenuWrapper;
    new (properties) TextMenuProperties();

    if (use_scrolling_menu)
    {
        ScrollingMenu2 *menu = (ScrollingMenu2 *)::CClass::operator new(sizeof(ScrollingMenu2));
        if (menu != 0)
            menu = __14ScrollingMenu2iiiiii(
                menu, 0, 0x50, 0x200, 0x50, 1, 4);
        self->Menu = menu;
    }
    else
    {
        VerticalMenu2 *menu = (VerticalMenu2 *)::CClass::operator new(sizeof(VerticalMenu2));
        if (menu != 0)
            menu = VerticalMenu2_ctor(
                menu, 0, 0, 0x200, 0x100, 1, 1, 4);
        self->Menu = menu;
    }

    MenuAssignableFont *font = reinterpret_cast<MenuAssignableFont *>(
        FontManager::GetFont(const_cast<char *>(font_name)));
    *(MenuAssignableFont *)&self->Props.TheFont = *font;

    if (strcmp(font_name, s_ingame1_fnt_800dc1b0) == 0)
    {
        self->Props.BigScale = 0x1324;
        self->Props.NormalScale = 0x1000;
    }
    else
    {
        self->Props.BigScale = 0xe5b;
        self->Props.NormalScale = 0xc00;
    }

    {
        int i = 0;

        self->Props.TheFont.ShadowDx = 2;
        self->Props.TheFont.ShadowDy = 2;
        self->NumItems = num_items;
        if (num_items > 0)
        {
            int titleId = -1;
            VMenuItem **itemsPtr = self->ItemList;
            const char **strPtr = string_list;
            int *idsPtr = id_list;

            do
            {
                if (*idsPtr == titleId)
                {
                    TitleMenuItem *menuItem =
                        (TitleMenuItem *)::CClass::operator new(sizeof(TitleMenuItem));
                    if (menuItem != 0)
                        menuItem = __13TitleMenuItemiPCcR18TextMenuProperties(
                            menuItem, *idsPtr, *strPtr, properties);
                    *itemsPtr = menuItem;
                }
                else
                {
                    TextMenuItem *menuItem =
                        (TextMenuItem *)::CClass::operator new(sizeof(TextMenuItem));
                    if (menuItem != 0)
                        menuItem = __12TextMenuItemiPCcR18TextMenuProperties(
                            menuItem, *idsPtr, *strPtr, properties);
                    *itemsPtr = menuItem;
                }

                ((void (*)(BasicMenu *, VMenuItem *))self->Menu->vtable[8])(
                    self->Menu, *itemsPtr);
                itemsPtr++;
                strPtr++;
                idsPtr++;
                i++;
            } while (i < self->NumItems);
        }
    }

    {
        BasicMenu *menu = self->Menu;
        int *vtable = menu->vtable;
        ((void (*)(BasicMenu *))vtable[4])(menu);
    }
    return self;
}


/**
 * _._15TextMenuWrapper (typed ABI owner)
 *
 * Address:   0x80089420
 * Mangled:   _._15TextMenuWrapper
 * Demangled: TextMenuWrapper::~TextMenuWrapper()
 *
 * @param self  Implicit object pointer.
 * @param flag  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
void Menu_TextMenuWrapper_dtor(TextMenuWrapper *self, int flag) {
    int i = 0;
    self->vtable = (int *)__vt_15TextMenuWrapper;
    int count = self->NumItems;
    if (count > 0) {
        VMenuItem **ptr = self->ItemList;
        do {
            VMenuItem *item = *ptr;
            if (item != 0) {
                int *vt = item->vtable;
                ((void (*)(VMenuItem *, int))vt[2])(item, 3);
                count = self->NumItems;
            }
            i++;
            ptr++;
        } while (i < count);
    }
    BasicMenu *menu = self->Menu;
    if (menu != 0) {
        int *vt = menu->vtable;
        ((void (*)(BasicMenu *, int))vt[2])(menu, 3);
    }
    FontFromStorage(&self->Props.TheFont)->~Font();
    self->Props.vtable = &Menu_BaseVTable;
    self->vtable = &Menu_BaseVTable;
    if (flag & 1) {
        ::CClass::operator delete(self);
    }
}


/**
 * Forward the latest menu press through this wrapper's active menu.
 *
 * Address:   0x80089504
 * Mangled:   HandleInput__15TextMenuWrapper
 * Demangled: TextMenuWrapper::HandleInput(void)
 *
 */
void TextMenuWrapper::HandleInput(void) {
    int press = Menu_GetPress();
    if ((unsigned int)(press - 2) < 2) {
        SFX_PlayX(0x90, 0x2000, 0);
    }
    {
        BasicMenu *menu = Menu;
        int *vt = menu->vtable;
        int fn = vt[7];
        ((void (*)(BasicMenu *, int))fn)(menu, press);
    }
    {
        BasicMenu *menu = Menu;
        int *vt = menu->vtable;
        int fn = vt[6];
        ((void (*)(BasicMenu *))fn)(menu);
    }
}


/**
 * Test whether this wrapped menu accepted or cancelled input.
 *
 * Address:   0x80089590
 * Mangled:   TestComplete__15TextMenuWrapper
 * Demangled: TextMenuWrapper::TestComplete(void)
 *
 * @return 1 when the menu accepts or cancels input, otherwise 0.
 */
int TextMenuWrapper::TestComplete(void) {
    SControl *pad = &Pad[0];
    VMenuItem **ptr;
    VMenuItem *item;
    int i;
    register int count asm("$6");
    if (pad->Start.Triggered != 0) goto do_work;
    if (pad->X.Triggered == 0) goto check_pad1;
do_work:
    i = 0;
    pad->Start.Triggered = 0;
    pad->X.Triggered = 0;
    {
        register int tmp_count asm("$3") = NumItems;
        HighlightedId = -1;
        if (tmp_count <= 0) goto sfx_91;
        count = tmp_count;
    }
    ptr = ItemList;
    do {
        item = *ptr;
        if (item->Highlighted != 0) goto loop_break;
        i++;
        ptr++;
    } while (i < count);
sfx_91:
    SFX_PlayX(0x91, 0x2000, 0);
    return 1;
loop_break:
    HighlightedId = item->ID;
    goto sfx_91;
check_pad1:
    {
        int p1 = pad->Triangle.Triggered;
        if (p1 != 0) {
            pad->Triangle.Triggered = 0;
            HighlightedId = -1;
            SFX_PlayX(0x93, 0x2000, 0);
            return 1;
        }
    }
ret_zero:
    return 0;
}


// === MENU.H-emitted function implementations (address order) ===

/** Default completion hook for a menu wrapper. */
int MenuWrapper::TestComplete() {
}


/** Default input hook for a menu wrapper. */
void MenuWrapper::HandleInput() {
}


/** Restore MenuWrapper's base dispatch table and optionally free it. */
MenuWrapper::~MenuWrapper() {
    this->vtable = &Menu_BaseVTable;
}


/** Destroy a title's TextMenuItem base and optionally free the title. */
void Menu_DestroyTitleMenuItemAbi(
    TitleMenuItem *_this, int __in_chrg) {
    Menu_DestroyTextMenuItemAbi(_this, 0);
    if (__in_chrg & 1) {
        ::CClass::operator delete(_this);
    }
}


/** Destroy the embedded font, restore base dispatch, and optionally free. */
static void Menu_DestroyTextMenuPropertiesAbi(
    TextMenuProperties *_this, int __in_chrg) {
    FontFromStorage(&_this->TheFont)->~Font();
    _this->vtable = &Menu_BaseVTable;
    if (__in_chrg & 1) {
        ::CClass::operator delete(_this);
    }
}


/** Default no-op item insertion hook for BasicMenu. */
void BasicMenu::addItem(VMenuItem *item) {
}


/** Default no-op press hook for BasicMenu. */
void BasicMenu::processPress(int press) {
}


/** Default no-op update hook for BasicMenu. */
void BasicMenu::update() {
}


/** Default no-op draw hook for BasicMenu. */
void BasicMenu::draw() {
}


/** Default no-op activation hook for BasicMenu. */
void BasicMenu::activate() {
}


/** Restore BasicMenu's base dispatch table and optionally free it. */
BasicMenu::~BasicMenu() {
    this->vtable = &Menu_BaseVTable;
}


/** Destroy the MenuItem base and optionally free the vertical item. */
static void Menu_DestroyVMenuItemAbi(
    VMenuItem *_this, int __in_chrg) {
    MenuItem_dtor(_this, 0);
    if (__in_chrg & 1) {
        ::CClass::operator delete(_this);
    }
}


/** Default no-op draw hook for MenuItem. */
void MenuItem::draw() {
}


/** Default no-op positioning hook for MenuItem. */
void MenuItem::setXY(int x, int y) {
}


// GCC emitted this private `CClass::~CClass` copy last in the owner's text,
// so it stays at the end of the translation unit.
static void __DOT__6CClass_800897F4(CClass *_this, int __in_chrg) {
    _this->vtable = &Menu_BaseVTable;
    if (__in_chrg & 1) {
        ::CClass::operator delete(_this);
    }
}
