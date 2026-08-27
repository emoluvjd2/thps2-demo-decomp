// MULTISELECT.cpp -- Reconstructed translation unit
// Original: MULTISELECT.CPP
// Functions: 9 reconstructed entries
// Address range: 0x8008984C - 0x8008A2B8
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.



#define SCREENTOOLS_AGGREGATE_LAYOUT
#define SCREENTOOLS_AGGREGATE_LAYOUT_ONLY
#include "SCREENTOOLS.h"
#undef SCREENTOOLS_AGGREGATE_LAYOUT_ONLY
#undef SCREENTOOLS_AGGREGATE_LAYOUT
#include "PROMPTSCREENELEMENT.h"
#include "FRONTSCREENS.h"

struct VerticalMenu2;
struct VMenuItem;
struct MultiSelectScreen;

/** Virtual interface used by the multiplayer selector's outer screen. */
struct MultiSelectScreenVtable {
    void (*Reserved0)(MultiSelectScreen *screen);
    void (*Reserved1)(MultiSelectScreen *screen);
    void (*destroy)(MultiSelectScreen *screen, int flags);
    void (*update)(MultiSelectScreen *screen);
    void (*draw)(MultiSelectScreen *screen, int x, int y);
};

/** Common prefix of deleting-destructor-capable front-end vtables. */
struct MultiSelectObjectVtable {
    void (*Reserved0)(void *object);
    void (*Reserved1)(void *object);
    void (*destroy)(void *object, int flags);
    void (*update)(void *object);
    void (*draw)(void *object, int x, int y);
    void (*drawSprite)(void *object, int x, int y, int drawFirst);
};

/** Front-end sprite allocation and dispatch prefix used by this owner. */
struct Sprite2 {
    struct MultiSelectObjectVtable *vtable;
    unsigned char mStorage[56];

    static void *operator new(unsigned int size) throw()
    {
        return ::CClass::operator new(size);
    }
    /** Build a sprite from a bitmap file. */
    Sprite2(char *name, int pack, int sliceWidth, int sliceHeight);
};

/** Flat runtime dispatch table used by multiplayer menu items. */
struct MultiSelectVMenuItemVtable {
    void (*Reserved0)(VMenuItem *item);
    void (*Reserved1)(VMenuItem *item);
    void (*destroy)(VMenuItem *item, int flags);
    void (*setXY)(VMenuItem *item, int x, int y);
};

/** Proven prefix of a vertical menu item, through its X/Y coordinates. */
struct VMenuItem {
    struct MultiSelectVMenuItemVtable *vtable;
    int ID;
    int Selectable;
    int Choosable;
    int Highlighted;
    int InternalState;
    int W;
    int H;
    int X;
    int Y;
    int JustX;
};

/** Flat runtime dispatch table used by the vertical menu. */
struct MultiSelectVerticalMenuVtable {
    void (*Reserved0)(VerticalMenu2 *menu);
    void (*Reserved1)(VerticalMenu2 *menu);
    void (*destroy)(VerticalMenu2 *menu, int flags);
    void (*setArrows)(VerticalMenu2 *menu, Sprite2 *left,
                      Sprite2 *right, int spacing);
    void (*activate)(VerticalMenu2 *menu);
    void (*draw)(VerticalMenu2 *menu);
    void (*update)(VerticalMenu2 *menu);
    void (*processPress)(VerticalMenu2 *menu, int press);
    void (*addItem)(VerticalMenu2 *menu, VMenuItem *item);
};

/** Proven runtime prefix of the vertical menu object. */
struct VerticalMenu2 {
    struct MultiSelectVerticalMenuVtable *vtable;
};

/** Original polymorphic class prefix used by embedded Font assignments. */
struct MultiSelectClassAssignmentBase {
    virtual ~MultiSelectClassAssignmentBase();
};

/** Owner-local polymorphic Font model used by the multiplayer selector. */
struct MultiSelectFontAssignmentView : MultiSelectClassAssignmentBase {
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
    struct FontCharacter *CharTab;
    int NumChars;
    int Clut;
    struct TextEffect *CurrentTextEffect;
    unsigned char OldR;
    unsigned char OldG;
    unsigned char OldB;
};

/** Owner-local layout of the menu style copied by MultiSelectScreen. */
struct TextMenuProperties : MultiSelectClassAssignmentBase {
    TextMenuProperties();
    struct MultiSelectFontAssignmentView TheFont;
    int BigScale;
    int NormalScale;
    int SelectedR;
    int SelectedG;
    int SelectedB;
    int NormalR;
    int NormalG;
    int NormalB;
    int GrayR;
    int GrayG;
    int GrayB;
};

/** Multiplayer-mode selection screen and its owned menu resources. */
struct MultiSelectScreen {
    struct ScreenElementList ScreenElementList;
    struct Sprite2 *mBackground;
    struct MultiSelectFontAssignmentView mFont1;
    struct MultiSelectFontAssignmentView mFont2;
    struct VerticalMenu2 *mMenu;
    struct TextMenuProperties *mProps;
    struct VMenuItem *mItem[5];
    struct Sprite2 *mpLeftArrow;
    struct Sprite2 *mpRightArrow;
    struct FrontPreLoader mFrontPreLoader;
    int mFinishUp;

    /** Handle multiplayer-selection confirmation or cancellation input. */
    int handlePress();
    /** Return the currently highlighted multiplayer game mode. */
    int GetGameMode();
    /** Begin the preload phase for a completed selection. */
    void InitEndProcess(int press);
    /** Return whether the active preload phase has completed. */
    int EndProcess();
};

typedef char multiselect_screen_size_must_be_360[
    (sizeof(MultiSelectScreen) == 360) ? 1 : -1];
typedef char multiselect_font_size_must_be_80[
    (sizeof(MultiSelectFontAssignmentView) == 80) ? 1 : -1];
typedef char multiselect_text_menu_properties_size_must_be_128[
    (sizeof(TextMenuProperties) == 128) ? 1 : -1];
typedef char multiselect_vmenu_item_size_must_be_44[
    (sizeof(VMenuItem) == 44) ? 1 : -1];
typedef char multiselect_sprite2_size_must_be_60[
    (sizeof(Sprite2) == 60) ? 1 : -1];

#include "SFX.h"
#include "MAIN.h"
#include "PAD.h"
#include "SCREENTOOLS.h"
#include "UTILS.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBGPU.H>

/** Placement construction for explicitly managed Font storage. */
inline void *operator new(unsigned int, void *address)
{
    return address;
}

// === External function declarations ===
#include "MENUINPUT.h"
extern "C" ScreenElementList *ScreenElementList_ctor(
    ScreenElementList *, int, int, int, int) asm("__17ScreenElementListiiii");
extern "C" void ScreenElementList_update(ScreenElementList *)
    asm("update__17ScreenElementList");
extern "C" void ScreenElementList_draw(ScreenElementList *, int, int)
    asm("draw__17ScreenElementListii");
extern "C" void ScreenElementList_dtor(ScreenElementList *, int)
    asm("_._17ScreenElementList");
extern "C" ScreenElement *TextScreenElement_ctor(
    ScreenElement *, char *, Font *,
    int, int, int, int) asm("__17TextScreenElementPcP4Fontiiii");
extern "C" TextMenuProperties *TextMenuProperties_ctor(
    TextMenuProperties *) asm("__18TextMenuProperties");
extern "C" VMenuItem *TextMenuItem_ctor(
    VMenuItem *, int, char *, TextMenuProperties *)
    asm("__12TextMenuItemiPCcR18TextMenuProperties");
extern "C" VerticalMenu2 *VerticalMenu2_ctor(
    VerticalMenu2 *, int, int, int, int, int, int, int)
    asm("__13VerticalMenu2iiiiiii");
extern "C" int BasicMenu_GetHighlighted(VerticalMenu2 *)
    asm("getHighlighted__9BasicMenu");
#include "DB.h"
#include "FRONTEND2.h"

/**
 * Read-only multiplayer heading strings in their original object order.
 */
static char MultiSelectSplitScreenHeading[20]
    asm("s_splitscreen_modes_800b3fa8")
    __attribute__((section(".rdata"), aligned(4))) = "splitscreen modes";
static char MultiSelectMultiplayerHeading[20]
    asm("s_multiplayer_modes_800b3fbc")
    __attribute__((section(".rdata"), aligned(4))) = "multiplayer modes";

/**
 * MULTISELECT's target-owned menu resource names and labels.
 */
static char MultiSelectBackgroundName[12]
    asm("s_s2bgd4_bmp_800dc214")
    __attribute__((section(".sdata"), aligned(4))) = "s2bgd4.bmp";
static char MultiSelectMainFontName[12]
    asm("s_mainf_fnt_800dc220")
    __attribute__((section(".sdata"), aligned(4))) = "mainf.fnt";
static char MultiSelectSecondFontName[12]
    asm("s_s2font2_fnt_800dc22c")
    __attribute__((section(".sdata"), aligned(4))) = "s2font2.fnt";
static char MultiSelectTrickAttackLabel[16]
    asm("s_trick_attack_800dc238")
    __attribute__((section(".sdata"), aligned(4))) = "trick attack";
static char MultiSelectGraffitiLabel[12]
    asm("s_graffiti_800dc248")
    __attribute__((section(".sdata"), aligned(4))) = "graffiti";
static char MultiSelectTagLabel[4] asm("DAT_800dc254")
    __attribute__((section(".sdata"), aligned(4))) = "tag";
static char MultiSelectContestLabel[8] asm("s_contest_800dc258")
    __attribute__((section(".sdata"), aligned(4))) = "contest";
static char MultiSelectHorseLabel[8] asm("s_horse_800dc260")
    __attribute__((section(".sdata"), aligned(4))) = "horse";
static char MultiSelectLeftArrowName[16]
    asm("s_s2arrow2_bmp_800dc268")
    __attribute__((section(".sdata"), aligned(4))) = "s2arrow2.bmp";
static char MultiSelectRightArrowName[16]
    asm("s_s2arrow1_bmp_800dc278")
    __attribute__((section(".sdata"), aligned(4))) = "s2arrow1.bmp";
static char MultiSelectSelectPromptImageName[16]
    asm("s_s2prmpt4_bmp_800dc288")
    __attribute__((section(".sdata"), aligned(4))) = "s2prmpt4.bmp";
static char MultiSelectSelectPromptText[8] asm("s_select_800dc298")
    __attribute__((section(".sdata"), aligned(4))) = "select";
static char MultiSelectAcceptPromptImageName[16]
    asm("s_s2prmpt1_bmp_800dc2a0")
    __attribute__((section(".sdata"), aligned(4))) = "s2prmpt1.bmp";
static char MultiSelectAcceptPromptText[8] asm("s_accept_800dc2b0")
    __attribute__((section(".sdata"), aligned(4))) = "accept";
static char MultiSelectBackPromptImageName[16]
    asm("s_s2prmpt2_bmp_800dc2b8")
    __attribute__((section(".sdata"), aligned(4))) = "s2prmpt2.bmp";
static char MultiSelectBackPromptText[8] asm("DAT_800dc2c8")
    __attribute__((section(".sdata"), aligned(4))) = "back";

/*
 * Keep the source-facing declarations incomplete. GCC 2 then retains the
 * target's absolute lui/addiu address formation while the semantic owner
 * objects above provide the actual storage and linker exports.
 */
extern "C" char SplitScreenHeadingText[]
    asm("s_splitscreen_modes_800b3fa8");
extern "C" char MultiplayerHeadingText[]
    asm("s_multiplayer_modes_800b3fbc");
extern "C" char MultiSelectBackgroundResource[]
    asm("s_s2bgd4_bmp_800dc214");
extern "C" char MultiSelectMainFontResource[] asm("s_mainf_fnt_800dc220");
extern "C" char MultiSelectSecondFontResource[]
    asm("s_s2font2_fnt_800dc22c");
extern "C" char TrickAttackText[] asm("s_trick_attack_800dc238");
extern "C" char GraffitiText[] asm("s_graffiti_800dc248");
extern "C" char TagText[] asm("DAT_800dc254");
extern "C" char ContestText[] asm("s_contest_800dc258");
extern "C" char HorseText[] asm("s_horse_800dc260");
extern "C" char LeftArrowResource[] asm("s_s2arrow2_bmp_800dc268");
extern "C" char RightArrowResource[] asm("s_s2arrow1_bmp_800dc278");
extern "C" char SelectPromptImage[] asm("s_s2prmpt4_bmp_800dc288");
extern "C" char SelectPromptText[] asm("s_select_800dc298");
extern "C" char AcceptPromptImage[] asm("s_s2prmpt1_bmp_800dc2a0");
extern "C" char AcceptPromptText[] asm("s_accept_800dc2b0");
extern "C" char BackPromptImage[] asm("s_s2prmpt2_bmp_800dc2b8");
extern "C" char BackPromptText[] asm("DAT_800dc2c8");

/** Allocation extent of a text-menu item beyond its proven public prefix. */
struct MultiSelectTextMenuItemStorage {
    unsigned char bytes[0x50];
};

/** Allocation extent of one prompt image/text element. */
struct MultiSelectPromptStorage {
    unsigned char bytes[0x68];
};

/** Allocate and construct the original menu-style object, preserving null. */
static inline TextMenuProperties *NewTextMenuProperties(void) {
    TextMenuProperties *properties =
        (TextMenuProperties *)::CClass::operator new(sizeof(TextMenuProperties));
    if (properties != 0) {
        properties = TextMenuProperties_ctor(properties);
    }
    return properties;
}

/** Destroy one owned front-end object through its deleting-destructor slot. */
static inline void DeleteMultiSelectObject(void *object) {
    if (object != 0) {
        MultiSelectObjectVtable *vtable =
            *(MultiSelectObjectVtable **)object;
        vtable->destroy(object, 3);
    }
}

/** Dispatch the menu item's original virtual setXY slot. */
static inline void SetMultiSelectItemXY(VMenuItem *item, int y) {
    item->vtable->setXY(item, item->X, y);
}

/** Attach the selector arrows through the vertical menu's runtime table. */
static inline void SetMultiSelectMenuArrows(VerticalMenu2 *menu,
                                             Sprite2 *left,
                                             Sprite2 *right) {
    menu->vtable->setArrows(menu, left, right, 5);
}

/** Activate the selector menu through its runtime dispatch table. */
static inline void ActivateMultiSelectMenu(VerticalMenu2 *menu) {
    menu->vtable->activate(menu);
}

// === Extern "C" forward declarations ===
extern "C" MultiSelectScreen *MultiSelectScreen_ctor(MultiSelectScreen *)
    asm("__17MultiSelectScreen");
extern "C" void MultiSelectScreen_dtor(MultiSelectScreen *, int)
    asm("_._17MultiSelectScreen");
extern "C" void MultiSelectScreen_draw(MultiSelectScreen *, int, int)
    asm("draw__17MultiSelectScreenii");
extern "C" void MultiSelectScreen_update(MultiSelectScreen *)
    asm("update__17MultiSelectScreen");

typedef void (*MultiSelectVirtualFunction)(void);

extern "C" int MultiSelectV_Width(void)
    asm("width__13ScreenElement");

/**
 * MultiSelectScreen's original virtual dispatch table.
 */
extern "C" MultiSelectVirtualFunction const __vt_17MultiSelectScreen[7]
    __attribute__((section(".rdata"), aligned(4))) = {
        0,
        0,
        (MultiSelectVirtualFunction)&MultiSelectScreen_dtor,
        (MultiSelectVirtualFunction)&MultiSelectScreen_update,
        (MultiSelectVirtualFunction)&MultiSelectScreen_draw,
        (MultiSelectVirtualFunction)&MultiSelectV_Width,
        (MultiSelectVirtualFunction)&ScreenElement_Refresh,
    };
// === Function implementations (address order) ===

/**
 * __17MultiSelectScreen
 *
 * Address:   0x8008984C
 * Mangled:   __17MultiSelectScreen
 * Demangled: MultiSelectScreen::(void)
 *
 * Constructs the multiplayer mode selector, its fonts, menu items, arrows,
 * prompts, and preload state.
 *
 * @param self  Object being operated on.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
MultiSelectScreen *MultiSelectScreen_ctor(
    MultiSelectScreen *self) {
    MultiSelectScreen *screen = self;
    MultiSelectFontAssignmentView *font1;
    MultiSelectFontAssignmentView *font2;

    ScreenElementList_ctor(&screen->ScreenElementList, 0, 0, -1, -1);
    screen->ScreenElementList.ScreenElement.CClass.vtable =
        reinterpret_cast<int *>(__vt_17MultiSelectScreen);
    font1 = &screen->mFont1;
    ::new (reinterpret_cast<Font *>(font1)) Font;
    font2 = &screen->mFont2;
    ::new (reinterpret_cast<Font *>(font2)) Font;
    ::new (&screen->mFrontPreLoader) FrontPreLoader;

    {
        Sprite2 *sprite = new Sprite2(
            MultiSelectBackgroundResource, 1, 0x40, 0x40);
        screen->mBackground = sprite;
    }

    screen->mFinishUp = 0;

    screen->mFont1 = *reinterpret_cast<MultiSelectFontAssignmentView *>(
        FontManager::GetFont(MultiSelectMainFontResource));

    screen->mFont1.JustX = 1;
    screen->mFont1.JustY = 3;
    screen->mFont1.R = 0x40;
    screen->mFont1.G = 0x80;
    screen->mFont1.B = 0x80;
    screen->mFont1.Shadow = 0;
    screen->mFont1.SpaceX = 0;
    screen->mFont1.SpaceY = 0;
    screen->mFont1.Scale = 0xC00;

    screen->mFont2 = *reinterpret_cast<MultiSelectFontAssignmentView *>(
        FontManager::GetFont(MultiSelectSecondFontResource));

    screen->mFont2.JustX = 0;
    screen->mFont2.JustY = 3;
    screen->mFont2.R = 0x80;
    screen->mFont2.G = 0x80;
    screen->mFont2.B = 0x80;
    screen->mFont2.Shadow = 0;
    screen->mFont2.SpaceX = 0;

    ScreenElement *item[16];

    {
        int i = 0;
        ScreenElement **itemSlot = item;
        do {
            *itemSlot = 0;
            i++;
            itemSlot++;
        } while (i < 16);
    }

    {
        ScreenElement *pElement =
            (ScreenElement *)::CClass::operator new(sizeof(TextScreenElement));
        if (pElement != 0) {
            pElement = TextScreenElement_ctor(
                pElement, SplitScreenHeadingText,
                reinterpret_cast<Font *>(font1),
                0x100, 0x3A, -0x40, 0x3A);
        }
        item[0] = pElement;
        screen->ScreenElementList.addItem(item[0]);
    }

    {
        ScreenElement *pElement =
            (ScreenElement *)::CClass::operator new(sizeof(TextScreenElement));
        if (pElement != 0) {
            pElement = TextScreenElement_ctor(
                pElement, MultiplayerHeadingText,
                reinterpret_cast<Font *>(font1),
                0x100, 0x97, 0x240, 0x97);
        }
        item[1] = pElement;
        screen->ScreenElementList.addItem(item[1]);
    }

    {
        VerticalMenu2 *menu =
            (VerticalMenu2 *)::CClass::operator new(0x74);
        if (menu != 0) {
            menu = VerticalMenu2_ctor(
                menu, 0, 0x14, 0x200, 0xDC, 1, 0, 3);
        }
        screen->mMenu = menu;
    }

    {
        screen->mProps = NewTextMenuProperties();
        screen->mProps->TheFont = screen->mFont1;
        screen->mProps->TheFont.R = 0x80;
        screen->mProps->TheFont.G = 0x80;
        screen->mProps->TheFont.B = 0x80;
        screen->mProps->TheFont.Scale = 0xC00;
        screen->mProps->BigScale = 0xC00;
        screen->mProps->NormalScale = 0xC00;
    }

    {
        VMenuItem *menuItem = (VMenuItem *)::CClass::operator new(
            sizeof(MultiSelectTextMenuItemStorage));
        if (menuItem != 0) {
            menuItem = TextMenuItem_ctor(
                menuItem, 5, TrickAttackText, screen->mProps);
        }
        screen->mItem[0] = menuItem;
    }

    {
        VMenuItem *menuItem = (VMenuItem *)::CClass::operator new(
            sizeof(MultiSelectTextMenuItemStorage));
        if (menuItem != 0) {
            menuItem = TextMenuItem_ctor(
                menuItem, 8, GraffitiText, screen->mProps);
        }
        screen->mItem[1] = menuItem;
    }

    {
        VMenuItem *menuItem = (VMenuItem *)::CClass::operator new(
            sizeof(MultiSelectTextMenuItemStorage));
        if (menuItem != 0) {
            menuItem = TextMenuItem_ctor(
                menuItem, 9, TagText, screen->mProps);
        }
        screen->mItem[2] = menuItem;
        menuItem->Selectable = 0;
    }

    {
        VMenuItem *menuItem = (VMenuItem *)::CClass::operator new(
            sizeof(MultiSelectTextMenuItemStorage));
        if (menuItem != 0) {
            menuItem = TextMenuItem_ctor(
                menuItem, 10, ContestText, screen->mProps);
        }
        screen->mItem[3] = menuItem;
    }

    {
        VMenuItem *menuItem = (VMenuItem *)::CClass::operator new(
            sizeof(MultiSelectTextMenuItemStorage));
        if (menuItem != 0) {
            menuItem = TextMenuItem_ctor(
                menuItem, 7, HorseText, screen->mProps);
        }
        screen->mItem[4] = menuItem;
    }

    screen->mItem[0]->Selectable = 1;
    screen->mItem[1]->Selectable = 0;
    screen->mItem[2]->Selectable = 0;
    screen->mItem[3]->Selectable = 0;
    screen->mItem[4]->Selectable = 0;

    {
        int idx = 0;
        VMenuItem **items = screen->mItem;
        do {
            screen->mMenu->vtable->addItem(screen->mMenu, *items);
            items++;
            idx++;
        } while (idx < 5);
    }

    {
        Sprite2 *sprite = new Sprite2(LeftArrowResource, 1, 0, 0);
        screen->mpLeftArrow = sprite;
    }

    {
        Sprite2 *sprite = new Sprite2(RightArrowResource, 1, 0, 0);
        screen->mpRightArrow = sprite;
    }

    {
        SetMultiSelectMenuArrows(
            screen->mMenu, screen->mpLeftArrow, screen->mpRightArrow);
        ActivateMultiSelectMenu(screen->mMenu);
    }

    {
        SetMultiSelectItemXY(screen->mItem[0], 0x44);
        SetMultiSelectItemXY(screen->mItem[1], 0x5B);
        SetMultiSelectItemXY(screen->mItem[2], 0x72);
        SetMultiSelectItemXY(screen->mItem[3], 0xA1);
        SetMultiSelectItemXY(screen->mItem[4], 0xB8);
    }

    {
        ScreenElement *prompt = reinterpret_cast<ScreenElement *>(
            new PromptScreenElement(
                SelectPromptImage, SelectPromptText,
                reinterpret_cast<Font *>(font2), 0x73, 0xDC, -1, 0x104));
        screen->ScreenElementList.addItem(prompt);
    }

    {
        ScreenElement *prompt = reinterpret_cast<ScreenElement *>(
            new PromptScreenElement(
                AcceptPromptImage, AcceptPromptText,
                reinterpret_cast<Font *>(font2), 0xE4, 0xDC, -1, 0x104));
        screen->ScreenElementList.addItem(prompt);
    }

    {
        ScreenElement *prompt = reinterpret_cast<ScreenElement *>(
            new PromptScreenElement(
                BackPromptImage, BackPromptText,
                reinterpret_cast<Font *>(font2), 0x14D, 0xDC, -1, 0x104));
        screen->ScreenElementList.addItem(prompt);
    }

    return self;
}


/**
 * _._17MultiSelectScreen - Destroys owned submenu children, release preload/font resources, clear ScreenElementList, and optionally free self.
 *
 * Address:   0x80089F1C
 * Mangled:   _._17MultiSelectScreen
 * Demangled: MultiSelectScreen::~MultiSelectScreen()
 *
 * @param _this  Object being operated on.
 * @param flag  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
void MultiSelectScreen_dtor(
    MultiSelectScreen *self, int __in_chrg) {
    self->ScreenElementList.ScreenElement.CClass.vtable =
        reinterpret_cast<int *>(__vt_17MultiSelectScreen);

    {
        void *obj = self->mBackground;
        if (obj != 0) {
            MultiSelectObjectVtable *vtable =
                *(MultiSelectObjectVtable **)obj;
            vtable->destroy(obj, 3);
        }
    }
    {
        void *obj = self->mMenu;
        if (obj != 0) {
            MultiSelectObjectVtable *vtable =
                *(MultiSelectObjectVtable **)obj;
            vtable->destroy(obj, 3);
        }
    }
    {
        void *obj = self->mProps;
        if (obj != 0) {
            MultiSelectObjectVtable *vtable =
                *(MultiSelectObjectVtable **)obj;
            vtable->destroy(obj, 3);
        }
    }
    {
        void *obj = self->mpLeftArrow;
        if (obj != 0) {
            MultiSelectObjectVtable *vtable =
                *(MultiSelectObjectVtable **)obj;
            vtable->destroy(obj, 3);
        }
    }
    {
        void *obj = self->mpRightArrow;
        if (obj != 0) {
            MultiSelectObjectVtable *vtable =
                *(MultiSelectObjectVtable **)obj;
            vtable->destroy(obj, 3);
        }
    }
    {
        int i = 0;
        VMenuItem **ptr = self->mItem;
        do {
            VMenuItem *obj = *ptr;
            if (obj != 0) {
                MultiSelectVMenuItemVtable *vtable = obj->vtable;
                vtable->destroy(obj, 3);
            }
            i++;
            ptr++;
        } while (i < 5);
    }
    self->mFrontPreLoader.~FrontPreLoader();
    reinterpret_cast<Font *>(&self->mFont2)->~Font();
    reinterpret_cast<Font *>(&self->mFont1)->~Font();
    ScreenElementList_dtor(&self->ScreenElementList, 0);
    if (__in_chrg & 1) {
        ::CClass::operator delete(self);
    }
}


/**
 * draw__17MultiSelectScreenii
 *
 * Address:   0x8008A0A8
 * Mangled:   draw__17MultiSelectScreenii
 * Demangled: MultiSelectScreen::draw(int, int)
 *
 * @param self  Object being operated on.
 * @param x  X coordinate.
 * @param y  Y coordinate.
 */
void MultiSelectScreen_draw(
    MultiSelectScreen *self, int x, int y) {
    ScreenElementList_draw(&self->ScreenElementList, x, y);
    Sprite2 *background = self->mBackground;
    MultiSelectObjectVtable *vtable = background->vtable;
    vtable->drawSprite(background, 0, 0, 1);
}


/**
 * update__17MultiSelectScreen
 *
 * Address:   0x8008A0EC
 * Mangled:   update__17MultiSelectScreen
 * Demangled: MultiSelectScreen::update(void)
 *            Calls menu->update(), Menu_GetPress(), menu->processPress(press), menu->draw(), then ScreenElementList update
 *
 * @param self  Object being operated on.
 */
void MultiSelectScreen_update(MultiSelectScreen *self) {
    VerticalMenu2 *menu = self->mMenu;
    menu->vtable->draw(menu);
    int press = Menu_GetPress();
    menu = self->mMenu;
    menu->vtable->processPress(menu, press);
    menu = self->mMenu;
    menu->vtable->update(menu);
    ScreenElementList_update(&self->ScreenElementList);
}


/**
 * handlePress__17MultiSelectScreen
 *
 * Address:   0x8008A17C
 * Mangled:   handlePress__17MultiSelectScreen
 * Demangled: MultiSelectScreen::handlePress(void)
 *
 * @return 1 when X confirms, 2 when Triangle cancels, or 0 otherwise.
 */
int MultiSelectScreen::handlePress() {
    if (Pad[0].Up.Triggered != 0 || Pad[0].Down.Triggered != 0) {
        SFX_PlayX(0x92, 0x2000, 0);
    }
    if (Pad[0].X.Triggered != 0) {
        SFX_PlayX(0x91, 0x2000, 0);
        Pad[0].X.Triggered = 0;
        return 1;
    }
    if (Pad[0].Triangle.Triggered != 0) {
        SFX_PlayX(0x93, 0x2000, 0);
        Pad[0].Triangle.Triggered = 0;
        return 2;
    }
    return 0;
}


/**
 * GetGameMode__17MultiSelectScreen
 *
 * Address:   0x8008A224
 * Mangled:   GetGameMode__17MultiSelectScreen
 * Demangled: MultiSelectScreen::GetGameMode(void)
 *
 * @return Identifier of the currently highlighted game mode.
 */
int MultiSelectScreen::GetGameMode() {
    MultiSelectScreen *self = this;
    return BasicMenu_GetHighlighted(self->mMenu);
}


/**
 * InitEndProcess__17MultiSelectScreeni
 *
 * Address:   0x8008A248
 * Mangled:   InitEndProcess__17MultiSelectScreeni
 * Demangled: MultiSelectScreen::InitEndProcess(int)
 *            If key == 1, calls BeginLoad. Always sets unk164 = 1
 *
 * @param press  Button press code.
 */
void MultiSelectScreen::InitEndProcess(int press) {
    MultiSelectScreen *self = this;
    int one = 1;
    if (press == one) {
        self->mFrontPreLoader.BeginLoad(1);
    }
    self->mFinishUp = one;
}


/**
 * EndProcess__17MultiSelectScreen
 *
 * Address:   0x8008A288
 * Mangled:   EndProcess__17MultiSelectScreen
 * Demangled: MultiSelectScreen::EndProcess(void)
 *            If unk164 != 0, calls DoneLoading on preloader at +0x130
 *
 * @return 0
 */
int MultiSelectScreen::EndProcess() {
    MultiSelectScreen *self = this;
    if (self->mFinishUp == 0) return 0;
    return self->mFrontPreLoader.DoneLoading();
}


/**
 * MultiSelect_Main__FPi
 *
 * Address:   0x8008A2B8
 * Mangled:   MultiSelect_Main__FPi
 * Demangled: MultiSelect_Main(int *)
 *
 * @param pGameMode  Receives the selected game mode.
 * @return `-1` when the player backs out; otherwise `0`.
 */
int MultiSelect_Main(int *pGameMode) {
    unsigned int oldSkyColor;
    MultiSelectScreen *pMultiSelectScreen;
    int press;
    int exitPress;

    oldSkyColor = Db_SkyColor;
    Db_SkyColor = 0;

    Db_UpdateSky();

    // Load the two selector fonts (%hi addresses are CSE'd into saved regs).
    FontManager::LoadFont(MultiSelectMainFontResource);
    FontManager::LoadFont(MultiSelectSecondFontResource);

    // Allocate and construct MultiSelectScreen
    pMultiSelectScreen =
        (MultiSelectScreen *)::CClass::operator new(sizeof(MultiSelectScreen));
    if (pMultiSelectScreen != 0) {
        pMultiSelectScreen = MultiSelectScreen_ctor(pMultiSelectScreen);
    }

    Pad_ClearTriggers(Pad);
    Pad_ClearTriggers(Pad + 1);

    // Main loop
    while (1) {
        FrontEnd2_UpdateMusicLoop();

        // Save current Vblanks to GStartTime
        GStartTime = Vblanks;

        Db_FlipClear();
        Pad_Update();

        // Call handlePress(screen)
        {
            press = pMultiSelectScreen->handlePress();
            if (press == 1) {
                // Start pressed - get game mode and begin end process
                *pGameMode = pMultiSelectScreen->GetGameMode();
                pMultiSelectScreen->InitEndProcess(1);
                exitPress = 1;
            } else if (press == 2) {
                // Back pressed
                *pGameMode = press;
                pMultiSelectScreen->InitEndProcess(2);
                exitPress = 2;
            }
        }

        // Call EndProcess, if done then break
        if (pMultiSelectScreen->EndProcess() != 0) {
            break;
        }

        // Call screen->update() via vtable offset 0x0C
        {
            MultiSelectScreenVtable *vtable =
                (MultiSelectScreenVtable *)pMultiSelectScreen->
                    ScreenElementList.ScreenElement.CClass.vtable;
            vtable->update(pMultiSelectScreen);
        }
        // Call screen->draw(0, 0) via vtable offset 0x10
        {
            MultiSelectScreenVtable *vtable =
                (MultiSelectScreenVtable *)pMultiSelectScreen->
                    ScreenElementList.ScreenElement.CClass.vtable;
            vtable->draw(pMultiSelectScreen, 0, 0);
        }

        DrawSync(0);
        Pause(1);

        // Wait for vblanks: while (Vblanks < GStartTime + 2)
        {
            int target = GStartTime + 2;
            while ((unsigned)Vblanks < (unsigned)target) {
                // spin
            }
        }

        Db_DisplayDraw();
        PollHost();
    }

    // Restore sky color
    Db_SkyColor = oldSkyColor;

    // Destroy screen
    if (pMultiSelectScreen != 0) {
        MultiSelectScreenVtable *vtable =
            (MultiSelectScreenVtable *)pMultiSelectScreen->
                ScreenElementList.ScreenElement.CClass.vtable;
        vtable->destroy(pMultiSelectScreen, 3);
    }

    // Unload fonts
    FontManager::UnloadFont(MultiSelectMainFontResource);
    FontManager::UnloadFont(MultiSelectSecondFontResource);

    // Return -1 if the player backed out, else 0.
    if (exitPress == 2) return -1;
    return 0;
}
