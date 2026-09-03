// MAINMENU.cpp -- Reconstructed translation unit
// Original: MAINMENU.CPP
// Functions: 12
// Address range: 0x80081FF0 - 0x800829E8
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

typedef unsigned char u8;
struct MainMenuScreen;

#include "FRONTSCREENS.h"

#define SCREENTOOLS_AGGREGATE_LAYOUT
#define SCREENTOOLS_AGGREGATE_LAYOUT_ONLY
#include "SCREENTOOLS.h"
#undef SCREENTOOLS_AGGREGATE_LAYOUT_ONLY
#undef SCREENTOOLS_AGGREGATE_LAYOUT

struct RotateSprite;
struct TextMenuWrapper;

#include "IMAGEBASE.h"

/** Packed/sliced image storage shared by front-end sprites. */
struct SlicedImage2 : Image {
    void *SliceTab;
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
};

/** Complete Sprite2 storage allocated by the menu owner. */
struct Sprite2 : SlicedImage2 {
    static void *operator new(unsigned int size) throw()
    {
        return ::CClass::operator new(size);
    }
    /** Build a sprite from a bitmap file. */
    Sprite2(char *name, int pack, int sliceWidth, int sliceHeight);
};

/** Rotatable sprite whose final byte selects its aspect behavior. */
struct RotateSprite : Sprite2 {
    unsigned char AspectType;

    /** Build a rotatable sprite from a bitmap file. */
    RotateSprite(char *filename, int pack, int sliceWidth, int sliceHeight);
};

#include "PROMPTSCREENELEMENT.h"

/** Original seven-slot dispatch prefix shared by front-end elements. */
struct MainMenuScreenElementDispatchTable {
    void *mReserved0;
    void *mReserved1;
    void (*Destroy)(void *, int);
    void (*Update)(void *);
    void (*Draw)(void *, int, int);
    int (*Width)(void *);
    void (*Refresh)(void *);
};

/** Sprite dispatch prefix whose draw slot also accepts a rotation/flag. */
struct MainMenuSpriteDispatchTable {
    void *mReserved0;
    void *mReserved1;
    void (*Destroy)(void *, int);
    void *mSetData;
    void *mReserved4;
    void (*Draw)(void *, int, int, int);
};

/** Read the shared sprite dispatch prefix from a Sprite2 object. */
static inline MainMenuSpriteDispatchTable *GetSpriteDispatch(Sprite2 *sprite)
{
    return (MainMenuSpriteDispatchTable *)
        sprite->CClass.vtable;
}

/** Read the shared sprite dispatch prefix from a RotateSprite object. */
static inline MainMenuSpriteDispatchTable *GetSpriteDispatch(
    RotateSprite *sprite)
{
    return (MainMenuSpriteDispatchTable *)
        sprite->CClass.vtable;
}

/** Rotating menu-wheel element and selection state. */
struct WheelElementList {
    struct ScreenElementList ScreenElementList;
    struct RotateSprite *Wheel;
    int CurrentRot;
    int TargetRot;
    int NumSlots;
    /** Full runtime slot plus the persisted low-byte view used by the dtor. */
    union {
        int Slot;
        unsigned char SlotByte;
    };

    /** Advance wheel input and child screen elements. */
    void update();
    /** Draw the wheel and its child screen elements. */
    void draw(int x, int y);
    ~WheelElementList();
    /** Releases wheel-list storage through the original CClass allocator. */
    static void operator delete(void *p) { ::CClass::operator delete(p); }
};

/** Main-menu screen and its owned UI resources. */
struct MainMenuScreen {
    struct ScreenElementList ScreenElementList;
    struct FrontPreLoaderStorage mFrontPreLoader;
    void *FinishUp;
    FontStorage TheFont;
    struct TextMenuWrapper *Menu;
    struct Sprite2 *mBackground;
    struct WheelElementList *mWheel;
    FontStorage mFont1;

    /** Begin the preload phase for a selected menu command. */
    void InitEndProcess(int returnValue);
    /** Draw and advance the active preload phase. */
    int EndProcess();
    /** Accept the current wheel selection when input is triggered. */
    int isDone(int *returnValue);
    /** Draw the menu elements and their full-screen background. */
    void draw();
    /** Update every child element in the main-menu screen. */
    void update();
    ~MainMenuScreen();
    /** Releases main-menu storage through the original CClass allocator. */
    static void operator delete(void *p) { ::CClass::operator delete(p); }
};

/** Exact callable types stored in WheelElementList's target vtable. */
struct WheelElementListVtable {
    void *mReserved0;
    void *mReserved1;
    void (*Destroy)(WheelElementList *, int);
    void (*Update)(WheelElementList *);
    void (*Draw)(WheelElementList *, int, int);
    int (*Width)(ScreenElement *);
    void (*Refresh)(ScreenElement *);
};

/** Exact callable types stored in MainMenuScreen's target vtable. */
struct MainMenuScreenVtable {
    void *mReserved0;
    void *mReserved1;
    void (*Destroy)(MainMenuScreen *, int);
    void (*Update)(MainMenuScreen *);
    void (*Draw)(ScreenElementList *, int, int);
    int (*Width)(ScreenElement *);
    void (*Refresh)(ScreenElement *);
};

/** Read the manually reconstructed dispatch table for a main-menu screen. */
static inline MainMenuScreenElementDispatchTable *GetScreenDispatch(
    MainMenuScreen *pScreen)
{
    return (MainMenuScreenElementDispatchTable *)
        pScreen->ScreenElementList.ScreenElement.CClass.vtable;
}

typedef char mainmenu_wheel_size_must_be_124[
    (sizeof(WheelElementList) == 124) ? 1 : -1];
typedef char mainmenu_screen_size_must_be_332[
    (sizeof(MainMenuScreen) == 332) ? 1 : -1];
typedef char mainmenu_image_size_must_be_28[
    (sizeof(Image) == 28) ? 1 : -1];
typedef char mainmenu_sliced_image_size_must_be_60[
    (sizeof(SlicedImage2) == 60) ? 1 : -1];
typedef char mainmenu_sprite_size_must_be_60[
    (sizeof(Sprite2) == 60) ? 1 : -1];
typedef char mainmenu_rotate_sprite_size_must_be_64[
    (sizeof(RotateSprite) == 64) ? 1 : -1];
typedef char mainmenu_prompt_size_must_be_104[
    (sizeof(PromptScreenElement) == 104) ? 1 : -1];
typedef char mainmenu_wheel_vtable_size_must_be_28[
    (sizeof(WheelElementListVtable) == 28) ? 1 : -1];
typedef char mainmenu_screen_vtable_size_must_be_28[
    (sizeof(MainMenuScreenVtable) == 28) ? 1 : -1];

#include "SFX.h"
#include "MAIN.h"
#include "PACK.h"
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

/** Polymorphic base used to preserve a destination Font's dispatch pointer. */
struct MainMenuFontAssignmentBase {
    virtual ~MainMenuFontAssignmentBase();
};

/** Owner-local view of Font, whose inherited vptr is preserved on assignment. */
struct MainMenuFontAssignmentView : MainMenuFontAssignmentBase {
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

typedef char mainmenu_font_view_size_must_be_80[
    (sizeof(MainMenuFontAssignmentView) == 80) ? 1 : -1];

/** Allocate one typed object through the original CClass arena. */
template <class T>
static inline T *AllocateCClass()
{
    return (T *)::CClass::operator new(sizeof(T));
}

#define SCREENTOOLS_DECLARE_SCREEN_ELEMENT_LIST_CONSTRUCTOR
#include "SCREENTOOLS.h"
#define SCREENTOOLS_DECLARE_SCREEN_ELEMENT_LIST_DESTRUCTOR
#include "SCREENTOOLS.h"
#define SCREENTOOLS_DECLARE_SPRITE_SCREEN_ELEMENT_CONSTRUCTOR
#include "SCREENTOOLS.h"
#include "IMAGE.h"

#include "DB.h"
#include "SPOOL.h"
#include "PAL.h"
#include "FRONTEND2.h"

#define FRONTSCREENS_DECLARE_MAIN_MENU_CONSTRUCTOR_ABI
#include "FRONTSCREENS.h"
#define MainMenuScreen_ctor MainMenuConstructorABI::__14MainMenuScreen
extern "C" WheelElementList *__16WheelElementListiiii(
    WheelElementList *, int, int, int, int);

#define FRONTSCREENS_DECLARE_MAIN_MENU_DESTRUCTOR_ABI
#include "FRONTSCREENS.h"

/**
 * MAINMENU's screen-element dispatch tables in original read-only order.
 *
 * Both classes override destruction and update. WheelElementList also
 * overrides draw, while MainMenuScreen retains ScreenElementList::draw.
 */
WheelElementListVtable __vt_16WheelElementList
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        &WheelElementList_destructor_slot,
        (void (*)(WheelElementList *))&WheelElementList::update,
        (void (*)(WheelElementList *, int, int))&WheelElementList::draw,
        (int (*)(ScreenElement *))&ScreenElement::width,
        (void (*)(ScreenElement *))&ScreenElement::refresh,
};

MainMenuScreenVtable __vt_14MainMenuScreen
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        &MainMenuScreen_destructor_slot,
        (void (*)(MainMenuScreen *))&MainMenuScreen::update,
        (void (*)(ScreenElementList *, int, int))&ScreenElementList::draw,
        (int (*)(ScreenElement *))&ScreenElement::width,
        (void (*)(ScreenElement *))&ScreenElement::refresh,
};

/** MAINMENU's initialized small-data contribution in original order. */
u8 MainMenu_LastSelection
    __attribute__((section(".sdata"), aligned(4))) = 2;
static char EndProcess_str[]
    __attribute__((section(".sdata"), aligned(4))) = "loading";
static char MainMenuBmpName[]
    __attribute__((section(".sdata"), aligned(4))) = "s2bgd2.bmp";
static char MainMenuFont1[]
    __attribute__((section(".sdata"), aligned(4))) = "mainf.fnt";
static char MainMenuFont2[]
    __attribute__((section(".sdata"), aligned(4))) = "s2font2.fnt";
static char MainMenuItemNames[]
    __attribute__((section(".sdata"), aligned(4))) = "s2prmpt4.bmp";
static char MainMenuItemNames2[]
    __attribute__((section(".sdata"), aligned(4))) = "select";
static char MainMenuBmpName2[]
    __attribute__((section(".sdata"), aligned(4))) = "s2prmpt1.bmp";
static char MainMenuBmpName3[]
    __attribute__((section(".sdata"), aligned(4))) = "accept";
static char WheelBmpName[]
    __attribute__((section(".sdata"), aligned(4))) = "s2wheel.bmp";
static char WheelItemNames[]
    __attribute__((section(".sdata"), aligned(4))) = "s2arrow3.bmp";

/** Menu-wheel angle and result mapping at its target `.data` address. */
int RotMap[7][2]
    __attribute__((section(".data"), aligned(4))) = {
        {0x0000, 0x27},
        {0x0D6D, -1},
        {0x0B56, 0x28},
        {0x0934, -1},
        {0x06EF, -1},
        {0x04E4, 0x22},
        {0x02AB, -1},
};

// === Function implementations (address order) ===

/**
 * Run the main-menu frame loop and return the selected front-end command.
 *
 * Original symbol: `MainMenu_Main__Fv` at 0x80081FF0.
 *
 * @return Selected command from RotMap, or -1 until a selection is made.
 */
int MainMenu_Main(void)
{
    unsigned int oldSkyColor = Db_SkyColor;
    Db_SkyColor = 0;
    Db_UpdateSky();
    Spool_RemoveUnusedTextures();
    Pal_RemoveUnusedPalettes();
    Pack_DefragmentFully();

    MainMenuScreen *pScreen = AllocateCClass<MainMenuScreen>();
    if (pScreen != 0)
    {
        pScreen = MainMenuScreen_ctor(pScreen);
    }

    Pad_ClearTriggers(&Pad[0]);
    Pad_ClearTriggers(&Pad[1]);

    int return_value = -1;
    int Finishing = 0;

    while (1)
    {
        FrontEnd2_UpdateMusicLoop();
        GStartTime = Vblanks;
        Db_FlipClear();
        CalcPolyBufferEnd();
        Pad_Update();

        if (Finishing != 0)
        {
            Pad_ClearTriggers(&Pad[0]);
            Pad_ClearTriggers(&Pad[1]);
        }

        if (pScreen->isDone(&return_value))
        {
            Finishing = 1;
            pScreen->InitEndProcess(return_value);
        }

        GetScreenDispatch(pScreen)->Update(pScreen);

        if (pScreen->EndProcess() != 0)
        {
            break;
        }

        pScreen->draw();
        DrawSync(0);
        Pause(1);

        // Wait for two vertical blanks before presenting the next frame.
        while ((unsigned int)Vblanks <
               (unsigned int)(GStartTime + 2))
        {
        }

        Db_DisplayDraw();
        PollHost();
    }

    Db_SkyColor = oldSkyColor;

    if (pScreen != 0)
    {
        GetScreenDispatch(pScreen)->Destroy(pScreen, 3);
    }

    return return_value;
}

/**
 * Begin the common preload phase for menu commands that require it.
 *
 * Original symbol: `InitEndProcess__14MainMenuScreeni` at 0x800821C0.
 *
 * @param return_value Selected front-end command.
 */
void MainMenuScreen::InitEndProcess(int return_value)
{
    if ((unsigned int)(return_value - 0x20) < 2 || return_value == 0x28)
    {
        FrontPreLoaderFromStorage(&mFrontPreLoader)->BeginLoad(1);
    }
}

/**
 * Draw the loading message and advance the active front-end preload.
 *
 * Original symbol: `EndProcess__14MainMenuScreen` at 0x800821FC.
 *
 * @return Zero while idle or the current loader-completion state.
 */
int MainMenuScreen::EndProcess()
{
    int result;
    if (FinishUp == 0)
    {
        result = 0;
    }
    else
    {
        FontFromStorage(&TheFont)->draw(
            256, 129, EndProcess_str, 0);
        result = FrontPreLoaderFromStorage(
            &mFrontPreLoader)->DoneLoading();
    }
    return result;
}

/**
 * Construct the main-menu screen, fonts, background, wheel, and prompts.
 *
 * Original symbol: `__14MainMenuScreen` at 0x80082254.
 *
 * @param self Storage allocated for the screen.
 * @return The constructed screen.
 */
MainMenuScreen *MainMenuScreen_ctor(MainMenuScreen *self)
{
    __17ScreenElementListiiii(&self->ScreenElementList, 0, 0, -1, -1);
    self->ScreenElementList.ScreenElement.CClass.vtable =
        (int *)&__vt_14MainMenuScreen;

    ::new (FrontPreLoaderFromStorage(&self->mFrontPreLoader))
        FrontPreLoader;

    ::new (FontFromStorage(&self->TheFont)) Font;
    ::new (FontFromStorage(&self->mFont1)) Font;

    // Create the full-screen background sprite.
    Sprite2 *flatBit = new Sprite2(MainMenuBmpName, 1, 0, 0);
    self->mBackground = flatBit;

    // Create and attach the rotating menu wheel.
    WheelElementList *wheel = AllocateCClass<WheelElementList>();
    if (wheel != 0)
    {
        wheel = __16WheelElementListiiii(wheel, 255, 129, -110, 128);
    }
    self->mWheel = wheel;
    self->ScreenElementList.addItem(
        &wheel->ScreenElementList.ScreenElement);

    // Load and configure the loading-message font.
    Font *fontData1 = FontManager::LoadFont(MainMenuFont1);
    *(MainMenuFontAssignmentView *)&self->TheFont =
        *(MainMenuFontAssignmentView *)fontData1;
    self->TheFont.JustX = 1;
    self->TheFont.JustY = 1;
    self->TheFont.Scale = 0x800;
    self->TheFont.R = 0x40;
    self->TheFont.G = 0x40;
    self->TheFont.B = 0xFF;
    self->FinishUp = 0;

    // Load and configure the prompt font.
    Font *fontData2 = FontManager::LoadFont(MainMenuFont2);
    *(MainMenuFontAssignmentView *)&self->mFont1 =
        *(MainMenuFontAssignmentView *)fontData2;
    self->mFont1.JustX = 0;
    self->mFont1.JustY = 3;
    FontFromStorage(&self->mFont1)->setRGB(128, 128, 128);

    // Create the select prompt.
    self->mFont1.Shadow = 0;
    self->mFont1.SpaceX = 0;
    self->mFont1.SpaceY = 0;
    self->mFont1.Scale = 0x1000;

    PromptScreenElement *pPromptElement = new PromptScreenElement(
        MainMenuItemNames, MainMenuItemNames2,
        FontFromStorage(&self->mFont1), 159, 220, -1, 260);
    self->ScreenElementList.addItem(
        &pPromptElement->ScreenElementList.ScreenElement);

    // Create the accept prompt.
    pPromptElement = new PromptScreenElement(
        MainMenuBmpName2, MainMenuBmpName3,
        FontFromStorage(&self->mFont1), 272, 220, -1, 260);
    self->ScreenElementList.addItem(
        &pPromptElement->ScreenElementList.ScreenElement);

    return self;
}

/**
 * Release the screen-owned sprite, fonts, preloader, and element list.
 *
 * Original symbol: `_._14MainMenuScreen` at 0x800824EC.
 */
MainMenuScreen::~MainMenuScreen()
{
    ScreenElementList.ScreenElement.CClass.vtable =
        (int *)&__vt_14MainMenuScreen;
    Sprite2 *background = mBackground;
    if (background != 0)
    {
        GetSpriteDispatch(background)->Destroy(background, 3);
    }
    FontManager::UnloadFont(MainMenuFont1);
    FontManager::UnloadFont(MainMenuFont2);
    FontFromStorage(&mFont1)->~Font();
    FontFromStorage(&TheFont)->~Font();
    FrontPreLoaderFromStorage(&mFrontPreLoader)->~FrontPreLoader();
    ScreenElementList_DestroyAbi(&ScreenElementList, 0);
}

/**
 * Draw the menu elements and their full-screen background.
 *
 * Original symbol: `draw__14MainMenuScreen` at 0x800825A0.
 *
 */
void MainMenuScreen::draw()
{
    ScreenElementList.draw(0, 0);
    Sprite2 *background = mBackground;
    GetSpriteDispatch(background)->Draw(background, 0, 0, 1);
}

/**
 * Update every child element in the main-menu screen.
 *
 * Original symbol: `update__14MainMenuScreen` at 0x800825EC.
 *
 */
void MainMenuScreen::update()
{
    ScreenElementList.update();
}

/**
 * Accept the current wheel selection when X or Start is triggered.
 *
 * Original symbol: `isDone__14MainMenuScreenPi` at 0x8008260C.
 *
 * @param return_value Receives the selected front-end command.
 * @return One when a valid command is accepted, otherwise zero.
 */
int MainMenuScreen::isDone(int *return_value)
{
    SControl *pad = Pad;
    if (pad->X.Triggered == 0)
    {
        if (pad->Start.Triggered == 0)
        {
            return 0;
        }
    }
    pad->X.Triggered = 0;
    pad->Start.Triggered = 0;

    WheelElementList *wheel = mWheel;
    int sel = wheel->Slot;
    int key = RotMap[sel][1];
    *return_value = key;

    if (key == -1)
    {
        SFX_PlayX(0x92, 0x2000, 0);
        return 0;
    }

    FinishUp = (void *)1;
    SFX_PlayX(0x91, 0x2000, 0);
    return 1;
}

/**
 * Construct the rotating wheel and its overlaid arrow sprite.
 *
 * Original symbol: `__16WheelElementListiiii` at 0x800826B4.
 *
 * @param self Storage allocated for the wheel element list.
 * @param homeX  Home X coordinate (resting position).
 * @param homeY  Home Y coordinate (resting position).
 * @param screenX  Screen-space X coordinate.
 * @param screenY  Screen-space Y coordinate.
 * @return The constructed wheel element list.
 */
WheelElementList *__16WheelElementListiiii(
    WheelElementList *self, int homeX, int homeY, int screenX, int screenY)
{
    __17ScreenElementListiiii(
        &self->ScreenElementList, homeX, homeY, screenX, screenY);
    self->ScreenElementList.ScreenElement.CClass.vtable =
        (int *)&__vt_16WheelElementList;

    // Create the rotating wheel sprite.
    RotateSprite *wheelElem = AllocateCClass<RotateSprite>();
    if (wheelElem != 0)
    {
        wheelElem = ::new (wheelElem) RotateSprite(
            WheelBmpName, 1, 0, 0);
    }
    self->Wheel = wheelElem;
    wheelElem->JustX = 1;
    self->Wheel->JustY = 1;

    // Create the fixed arrow overlay.
    SpriteScreenElement *item = AllocateCClass<SpriteScreenElement>();
    if (item != 0)
    {
        item = __19SpriteScreenElementPciiiiii(
            item, WheelItemNames, 1, -81, 0, 0, 1, 1);
    }
    self->ScreenElementList.addItem(&item->ScreenElement);

    // Restore the previous selection and its target angle.
    int lastSel = MainMenu_LastSelection;
    self->NumSlots = 7;
    self->Slot = lastSel;
    int entry = RotMap[lastSel][0];
    self->TargetRot = entry;
    self->CurrentRot = entry;

    return self;
}

/**
 * Remember the selected slot and release the wheel sprite and element list.
 *
 * Original symbol: `_._16WheelElementList` at 0x8008279C.
 */
WheelElementList::~WheelElementList()
{
    MainMenu_LastSelection = SlotByte;
    ScreenElementList.ScreenElement.CClass.vtable =
        (int *)&__vt_16WheelElementList;
    RotateSprite *wheel = Wheel;
    if (wheel != 0)
    {
        GetSpriteDispatch(wheel)->Destroy(wheel, 3);
    }
    ScreenElementList_DestroyAbi(&ScreenElementList, 0);
}

/**
 * Process wheel input and ease the displayed rotation toward its target slot.
 *
 * Original symbol: `update__16WheelElementList` at 0x8008281C.
 *
 */
void WheelElementList::update()
{
    SControl *pad = Pad;

    // Advance the wheel when Left is triggered.
    if (pad->Left.Triggered != 0)
    {
        SFX_PlayX(0xE9, 0x2000, 0);
        pad->Left.Triggered = 0;

        int idx = Slot;
        int maxItems = NumSlots;
        idx = idx + 1;
        Slot = idx;
        if (idx >= maxItems)
        {
            Slot = 0;
        }

        int newIdx = Slot;
        int targetRot = RotMap[newIdx][0];
        int curRot = CurrentRot;
        int diff = targetRot - curRot;
        TargetRot = targetRot;
        if (diff > 0)
        {
            TargetRot = targetRot - 0x1000;
        }
    }

    // Reload the controller pointer after the sound call path.
    pad = Pad;

    // Move back around the wheel when Right is triggered.
    if (pad->Right.Triggered != 0)
    {
        SFX_PlayX(0xE9, 0x2000, 0);
        pad->Right.Triggered = 0;

        int idx = Slot;
        idx = idx - 1;
        Slot = idx;
        if (idx < 0)
        {
            int maxItems = NumSlots;
            Slot = maxItems - 1;
        }

        int newIdx = Slot;
        int targetRot = RotMap[newIdx][0];
        int curRot = CurrentRot;
        int diff = targetRot - curRot;
        TargetRot = targetRot;
        if (diff < 0)
        {
            TargetRot = targetRot + 0x1000;
        }
    }

    // Smooth scroll towards target
    int target = TargetRot;
    int current = CurrentRot;
    int diff = target - current;

    if (diff < -0x258)
    {
        CurrentRot = current - 0x80;
    }
    else if (diff >= 0x259)
    {
        CurrentRot = current + 0x80;
    }
    else if (diff < -0x40)
    {
        CurrentRot = current - 0x40;
    }
    else if (diff >= 0x41)
    {
        CurrentRot = current + 0x40;
    }
    else
    {
        CurrentRot = target;
    }

    // Wrap around
    current = CurrentRot;
    if (current >= 0x1000)
    {
        int wrapped = current - 0x1000;
        CurrentRot = wrapped;
        int t = TargetRot;
        TargetRot = t - 0x1000;
    }

    current = CurrentRot;
    if (current < 0)
    {
        int wrapped = current + 0x1000;
        CurrentRot = wrapped;
        int t = TargetRot;
        TargetRot = t + 0x1000;
    }

    ScreenElementList.update();
}

/**
 * Draw the arrow elements and the wheel at its current rotation.
 *
 * Original symbol: `draw__16WheelElementListii` at 0x800829E8.
 *
 * @param x  X coordinate.
 * @param y  Y coordinate.
 */
void WheelElementList::draw(int x, int y)
{
    ScreenElementList.draw(x, y);
    RotateSprite *wheel = Wheel;
    int screenX = ScreenElementList.ScreenElement.mScreenX;
    int screenY = ScreenElementList.ScreenElement.mScreenY;
    int rotation = CurrentRot;
    GetSpriteDispatch(wheel)->Draw(
        wheel, screenX + x, screenY + y, rotation);
}
