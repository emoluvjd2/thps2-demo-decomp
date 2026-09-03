// Per-TU header for MENU.cpp
// Class definitions reconstructed from This Dust Remembers MAIN.H
#ifndef MENU_ABI_DECLARATIONS_ONLY
#ifndef MENU_INC_H
#define MENU_INC_H

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

#ifdef __cplusplus
}
#endif

// Forward declarations
class SlicedImage2;

// ============================================================
// CClass — root base class (vtable pointer only)
// Size: 0x4
// ============================================================
#include "globals.h"
#include "FONTTOOLS.h"

// ============================================================
// TextMenuProperties — font + color configuration for menus
// Size: 0x80 (128 bytes)
// Note: kept as struct to avoid ICE with extern "C" constructor
// ============================================================
struct TextMenuProperties {
    TextMenuProperties();
    int *vtable;            // 0x00 — CClass base
    FontStorage TheFont;    // 0x04 — explicitly managed Font storage
    int BigScale;           // 0x54
    int NormalScale;        // 0x58
    int SelectedR;          // 0x5C
    int SelectedG;          // 0x60
    int SelectedB;          // 0x64
    int NormalR;             // 0x68
    int NormalG;             // 0x6C
    int NormalB;             // 0x70
    int GrayR;              // 0x74
    int GrayG;              // 0x78
    int GrayB;              // 0x7C
};

// ============================================================
// MenuItem — base menu item
// Size: 0x28 (40 bytes)
// ============================================================
class MenuItem {
public:
    MenuItem(int id);
    ~MenuItem();
    void draw();
    void setXY(int x, int y);
    void setHighlightState(bool highlight_state);
    void setItemState(int state);
    void processPress(int press);
    void update();

    int *vtable;            // 0x00 — CClass base
    int ID;                 // 0x04
    int Selectable;         // 0x08
    int Choosable;          // 0x0C
    int Highlighted;        // 0x10
    int InternalState;      // 0x14
    int W;                  // 0x18
    int H;                  // 0x1C
    int X;                  // 0x20
    int Y;                  // 0x24
};

// ============================================================
// VMenuItem — menu item with justification
// Size: 0x2C (44 bytes)
// ============================================================
class VMenuItem : public MenuItem {
public:
    VMenuItem(int id);
    ~VMenuItem();

    int JustX;              // 0x28
};

// ============================================================
// TextMenuItem — text-based menu item
// Size: 0x50 (80 bytes)
// ============================================================
class TextMenuItem : public VMenuItem {
public:
    void setXY(int x, int y);
    void draw();

    char Text[32];          // 0x2C
    TextMenuProperties *TheProperties; // 0x4C
};

// ============================================================
// TitleMenuItem — title text menu item (no new fields)
// Size: 0x50 (80 bytes)
// ============================================================
class TitleMenuItem : public TextMenuItem {
public:
    void draw();
};

// ============================================================
// SelectMenuItem — selection menu item with multiple options
// Size: 0x100 (256 bytes)
// ============================================================
class SelectMenuItem : public VMenuItem {
public:
    void setXY(int x, int y);
    void draw();
    void processPress(int press);

    int AmHighlighted;      // 0x2C
    char Option[6][32];     // 0x30 — 6 options, 32 chars each (0xC0 bytes)
    int NumOptions;         // 0xF0
    int Spacing;            // 0xF4
    TextMenuProperties *TheProperties; // 0xF8
    int SmallW;             // 0xFC
};

// ============================================================
// BasicMenu — base menu container with item array
// Size: 0x68 (104 bytes)
// ============================================================
class BasicMenu {
public:
    ~BasicMenu();
    void activate();
    void draw();
    void update();
    void processPress(int press);
    void addItem(VMenuItem *item);
    void setArrows(SlicedImage2 *left, SlicedImage2 *right, int spacing);
    int getHighlighted();
    void setHighlighted(int id);

    int *vtable;            // 0x00 — CClass base
    SlicedImage2 *LeftArrow;  // 0x04
    SlicedImage2 *RightArrow; // 0x08
    int ArrowSpace;         // 0x0C
    VMenuItem *Item[16];    // 0x10 — array of 16 pointers (0x40 bytes)
    int HighestSlot;        // 0x50
    int Selected;           // 0x54
    int X;                  // 0x58
    int Y;                  // 0x5C
    int W;                  // 0x60
    int H;                  // 0x64
};

// ============================================================
// VerticalMenu2 — vertical menu layout
// Size: 0x74 (116 bytes)
// ============================================================
class VerticalMenu2 : public BasicMenu {
public:
    void activate();
    void draw();
    void update();
    void processPress(int press);
    void addItem(VMenuItem *item);

    int JustX;              // 0x68
    int JustY;              // 0x6C
    int Spacing;            // 0x70
};

// ============================================================
// ScrollingMenu2 — scrolling menu layout
// Size: 0xB8 (184 bytes)
// ============================================================
class ScrollingMenu2 : public BasicMenu {
public:
    void activate();
    void draw();
    void update();
    void processPress(int press);
    void addItem(VMenuItem *item);

    int JustX;              // 0x68
    int RY[16];             // 0x6C — render Y offsets (0x40 bytes)
    int ArrowSlide;         // 0xAC
    int Top;                // 0xB0
    int Bottom;             // 0xB4
};

// ============================================================
// MenuWrapper — base wrapper for menu systems
// Size: 0x0C (12 bytes)
// ============================================================
class MenuWrapper {
public:
    ~MenuWrapper();
    void HandleInput();
    int TestComplete();
    int GetHighlightedId();

    int *vtable;            // 0x00 — CClass base
    BasicMenu *Menu;        // 0x04
    int HighlightedId;      // 0x08
};

// ============================================================
// TextMenuWrapper — complete text menu system
// Size: 0x110 (272 bytes)
// ============================================================
class TextMenuWrapper : public MenuWrapper {
public:
    void HandleInput();
    int TestComplete();

    TextMenuProperties Props; // 0x0C — embedded (0x80 bytes)
    VMenuItem *ItemList[32];  // 0x8C — array of 32 pointers (0x80 bytes)
    int NumItems;             // 0x10C
};

typedef char menu_font_size_must_be_80[(sizeof(Font) == 80) ? 1 : -1];
typedef char menu_properties_size_must_be_128[(sizeof(TextMenuProperties) == 128) ? 1 : -1];
typedef char menu_item_size_must_be_40[(sizeof(MenuItem) == 40) ? 1 : -1];
typedef char menu_vertical_item_size_must_be_44[(sizeof(VMenuItem) == 44) ? 1 : -1];
typedef char menu_text_item_size_must_be_80[(sizeof(TextMenuItem) == 80) ? 1 : -1];
typedef char menu_select_item_size_must_be_256[(sizeof(SelectMenuItem) == 256) ? 1 : -1];
typedef char menu_basic_size_must_be_104[(sizeof(BasicMenu) == 104) ? 1 : -1];
typedef char menu_vertical_size_must_be_116[(sizeof(VerticalMenu2) == 116) ? 1 : -1];
typedef char menu_scrolling_size_must_be_184[(sizeof(ScrollingMenu2) == 184) ? 1 : -1];
typedef char menu_wrapper_size_must_be_12[(sizeof(MenuWrapper) == 12) ? 1 : -1];
typedef char menu_text_wrapper_size_must_be_272[(sizeof(TextMenuWrapper) == 272) ? 1 : -1];

#endif // MENU_INC_H
#endif // MENU_ABI_DECLARATIONS_ONLY

#ifdef MENU_DECLARE_ITEM_CONSTRUCTOR_ABI

/** Address the target MenuItem destructor used by derived owner bodies. */
void MenuItem_dtor(MenuItem *self, int __in_chrg) asm("_._8MenuItem");

namespace MenuConstructorAbi
{
/** Construct the VMenuItem base through its target C-linkage entry. */
extern "C" VMenuItem *__9VMenuItemi(VMenuItem *self, int id);
}

/** Construct a text menu item through its target C-linkage entry. */
extern "C" TextMenuItem *__12TextMenuItemiPCcR18TextMenuProperties(
    TextMenuItem *self, int id, const char *text,
    TextMenuProperties *properties);

/** Construct a title menu item through its target C-linkage entry. */
extern "C" TitleMenuItem *__13TitleMenuItemiPCcR18TextMenuProperties(
    TitleMenuItem *self, int id, const char *text,
    TextMenuProperties *properties);

#undef MENU_DECLARE_ITEM_CONSTRUCTOR_ABI
#endif

#ifdef MENU_DECLARE_OWNER_SMALL_DATA_DESTRUCTOR_ABI

/** Address the TextMenuProperties destructor held by MENU's small-data table. */
static void Menu_DestroyTextMenuPropertiesAbi(
    TextMenuProperties *self, int __in_chrg)
    asm("_._18TextMenuProperties");

extern "C" {
/** Declare MENU's owner-private CClass destructor tail. */
static void __DOT__6CClass_800897F4(CClass *self, int __in_chrg);
}

#undef MENU_DECLARE_OWNER_SMALL_DATA_DESTRUCTOR_ABI
#endif

#ifdef MENU_DECLARE_CONTAINER_OWNER_ABI_HEAD

/** Address the VerticalMenu2 destructor used by MENU's owner-local table. */
void Menu_VerticalMenu2_dtor(VerticalMenu2 *, int)
    asm("_._13VerticalMenu2");

/** Construct a scrolling menu through its target C-linkage entry. */
extern "C" ScrollingMenu2 *__14ScrollingMenu2iiiiii(
    ScrollingMenu2 *, int, int, int, int, int, int);

/** Address the ScrollingMenu2 destructor used by MENU's owner-local table. */
void Menu_ScrollingMenu2_dtor(ScrollingMenu2 *, int)
    asm("_._14ScrollingMenu2");

namespace MenuScrollingMenu2ABI
{
/** Activate a scrolling menu through its target C-linkage entry. */
extern "C" void activate__14ScrollingMenu2(ScrollingMenu2 *);
/** Update a scrolling menu through its target C-linkage entry. */
extern "C" void update__14ScrollingMenu2(ScrollingMenu2 *);
}

#undef MENU_DECLARE_CONTAINER_OWNER_ABI_HEAD
#endif

#ifdef MENU_DECLARE_TEXT_WRAPPER_CONSTRUCTOR_ABI
#ifndef MENU_TEXT_WRAPPER_CONSTRUCTOR_RECEIVER
#error "Select the TextMenuWrapper constructor ABI receiver type"
#endif

/** Construct a text-menu wrapper through its target C-linkage entry. */
extern "C" MENU_TEXT_WRAPPER_CONSTRUCTOR_RECEIVER *
__15TextMenuWrapperPCPcPiiPcb(
    MENU_TEXT_WRAPPER_CONSTRUCTOR_RECEIVER *, const char **, int *, int,
    const char *, bool);

#undef MENU_TEXT_WRAPPER_CONSTRUCTOR_RECEIVER
#undef MENU_DECLARE_TEXT_WRAPPER_CONSTRUCTOR_ABI
#endif

#ifdef MENU_DECLARE_CONTAINER_OWNER_ABI_TAIL

/** Address the TextMenuWrapper destructor used by MENU's owner-local table. */
void Menu_TextMenuWrapper_dtor(TextMenuWrapper *, int)
    asm("_._15TextMenuWrapper");
/** Address the SelectMenuItem destructor used by MENU's owner-local table. */
void Menu_DestroySelectMenuItemAbi(SelectMenuItem *, int)
    asm("_._14SelectMenuItem");
/** Address the TextMenuItem destructor used by MENU's owner-local table. */
void Menu_DestroyTextMenuItemAbi(TextMenuItem *, int)
    asm("_._12TextMenuItem");

#undef MENU_DECLARE_CONTAINER_OWNER_ABI_TAIL
#endif

#ifdef MENU_DECLARE_ITEM_OWNER_DESTRUCTOR_ABI

/** Address the TitleMenuItem destructor used by MENU's owner-local table. */
void Menu_DestroyTitleMenuItemAbi(
    TitleMenuItem *self, int __in_chrg) asm("_._13TitleMenuItem");
/** Address the owner-private VMenuItem destructor tail. */
static void Menu_DestroyVMenuItemAbi(VMenuItem *self, int __in_chrg)
    asm("_._9VMenuItem");

#undef MENU_DECLARE_ITEM_OWNER_DESTRUCTOR_ABI
#endif

#ifdef MENU_DECLARE_ERASED_BASE_DESTRUCTOR_ABI

/** Erased address view of MenuWrapper's destructor for its virtual table. */
void MenuV_DtorMenuWrapper(void)
    asm("_._11MenuWrapper");
/** Erased address view of BasicMenu's destructor for its virtual table. */
void MenuV_DtorBasicMenu(void)
    asm("_._9BasicMenu");

#undef MENU_DECLARE_ERASED_BASE_DESTRUCTOR_ABI
#endif

#ifdef MENU_DECLARE_OWNER_VTABLES

/** MENU-owned virtual tables with external linkage. */
extern MenuVirtualFunction const __vt_15TextMenuWrapper[];
extern MenuVirtualFunction const __vt_11MenuWrapper[];
extern MenuVirtualFunction const __vt_14ScrollingMenu2[];
extern MenuVirtualFunction const __vt_13VerticalMenu2[];
extern MenuVirtualFunction const __vt_14SelectMenuItem[];
extern MenuVirtualFunction const __vt_13TitleMenuItem[];
extern MenuVirtualFunction const __vt_12TextMenuItem[];
extern MenuVirtualFunction const __vt_9BasicMenu[];
extern MenuVirtualFunction const __vt_8MenuItem[];

#undef MENU_DECLARE_OWNER_VTABLES
#endif
