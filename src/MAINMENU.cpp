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
struct MainMenuSlicedImage {
    struct Image Image;
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
struct Sprite2 {
    struct MainMenuSlicedImage SlicedImage2;

    static void *operator new(unsigned int size) throw()
    {
        return ::CClass::operator new(size);
    }
    /** Build a sprite from a bitmap file. */
    Sprite2(char *name, int pack, int sliceWidth, int sliceHeight);
};

/** Rotatable sprite whose final byte selects its aspect behavior. */
struct RotateSprite {
    struct Sprite2 Sprite2;
    unsigned char AspectType;
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
        sprite->SlicedImage2.Image.CClass.vtable;
}

/** Read the shared sprite dispatch prefix from a RotateSprite object. */
static inline MainMenuSpriteDispatchTable *GetSpriteDispatch(
    RotateSprite *sprite)
{
    return (MainMenuSpriteDispatchTable *)
        sprite->Sprite2.SlicedImage2.Image.CClass.vtable;
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
    (sizeof(MainMenuSlicedImage) == 60) ? 1 : -1];
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

extern "C" ScreenElementList *ScreenElementList_ctor(
    ScreenElementList *, int, int, int, int)
    asm("__17ScreenElementListiiii");
extern "C" void ScreenElementList_Update(ScreenElementList *)
    asm("update__17ScreenElementList");
extern "C" void ScreenElementList_Draw(ScreenElementList *, int, int)
    asm("draw__17ScreenElementListii");
extern "C" void ScreenElementList_Destroy(ScreenElementList *, int)
    asm("_._17ScreenElementList");
extern "C" int ScreenElement_Width(ScreenElement *)
    asm("width__13ScreenElement");

extern "C" SpriteScreenElement *SpriteScreenElement_ctor(
    SpriteScreenElement *, char *, int, int, int, int, int, int)
    asm("__19SpriteScreenElementPciiiiii");
extern "C" RotateSprite *RotateSprite_ctor(
    RotateSprite *, char *, int, int, int)
    asm("__12RotateSpritePciii");

#include "DB.h"
#include "SPOOL.h"
#include "PAL.h"
#include "FRONTEND2.h"

extern "C" MainMenuScreen *MainMenuScreen_ctor(MainMenuScreen *)
    asm("__14MainMenuScreen");
extern "C" void MainMenuScreen_Draw(MainMenuScreen *)
    asm("draw__14MainMenuScreen");
extern "C" void MainMenuScreen_Update(MainMenuScreen *)
    asm("update__14MainMenuScreen");
extern "C" WheelElementList *WheelElementList_ctor(
    WheelElementList *, int, int, int, int)
    asm("__16WheelElementListiiii");
extern "C" void WheelElementList_Update(WheelElementList *)
    asm("update__16WheelElementList");
extern "C" void WheelElementList_Draw(WheelElementList *, int, int)
    asm("draw__16WheelElementListii");

extern "C" void MainMenuScreen_destructor_slot(MainMenuScreen *, int)
    asm("_._14MainMenuScreen");
extern "C" void WheelElementList_destructor_slot(WheelElementList *, int)
    asm("_._16WheelElementList");

/**
 * MAINMENU's screen-element dispatch tables in original read-only order.
 *
 * Both classes override destruction and update. WheelElementList also
 * overrides draw, while MainMenuScreen retains ScreenElementList::draw.
 */
extern "C" {
WheelElementListVtable __vt_16WheelElementList
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        &WheelElementList_destructor_slot,
        &WheelElementList_Update,
        &WheelElementList_Draw,
        &ScreenElement_Width,
        &ScreenElement_Refresh,
};

MainMenuScreenVtable __vt_14MainMenuScreen
    __attribute__((section(".rdata"), aligned(8))) = {
        0,
        0,
        &MainMenuScreen_destructor_slot,
        &MainMenuScreen_Update,
        &ScreenElementList_Draw,
        &ScreenElement_Width,
        &ScreenElement_Refresh,
};
}

/** MAINMENU's initialized small-data contribution in original order. */
extern "C" {
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
}

/** Menu-wheel angle and result mapping at its target `.data` address. */
extern "C" int RotMap[7][2]
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

        MainMenuScreen_Draw(pScreen);
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
    MainMenuScreen *self = this;
    if ((unsigned int)(return_value - 0x20) < 2 || return_value == 0x28)
    {
        FrontPreLoaderFromStorage(&self->mFrontPreLoader)->BeginLoad(1);
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
    MainMenuScreen *self = this;
    int result;
    if (self->FinishUp == 0)
    {
        result = 0;
    }
    else
    {
        FontFromStorage(&self->TheFont)->draw(
            256, 129, EndProcess_str, 0);
        result = FrontPreLoaderFromStorage(
            &self->mFrontPreLoader)->DoneLoading();
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
    ScreenElementList_ctor(&self->ScreenElementList, 0, 0, -1, -1);
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
        wheel = WheelElementList_ctor(wheel, 255, 129, -110, 128);
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
    ScreenElementList_Destroy(&ScreenElementList, 0);
}

/**
 * Draw the menu elements and their full-screen background.
 *
 * Original symbol: `draw__14MainMenuScreen` at 0x800825A0.
 *
 * @param self Main-menu screen to draw.
 */
void MainMenuScreen_Draw(MainMenuScreen *self)
{
    ScreenElementList_Draw(&self->ScreenElementList, 0, 0);
    Sprite2 *background = self->mBackground;
    GetSpriteDispatch(background)->Draw(background, 0, 0, 1);
}

/**
 * Update every child element in the main-menu screen.
 *
 * Original symbol: `update__14MainMenuScreen` at 0x800825EC.
 *
 * @param self Main-menu screen to update.
 */
void MainMenuScreen_Update(MainMenuScreen *self)
{
    ScreenElementList_Update(&self->ScreenElementList);
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
    MainMenuScreen *self = this;
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

    WheelElementList *wheel = self->mWheel;
    int sel = wheel->Slot;
    int key = RotMap[sel][1];
    *return_value = key;

    if (key == -1)
    {
        SFX_PlayX(0x92, 0x2000, 0);
        return 0;
    }

    self->FinishUp = (void *)1;
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
extern "C" WheelElementList *WheelElementList_ctor(
    WheelElementList *self, int homeX, int homeY, int screenX, int screenY)
{
    ScreenElementList_ctor(
        &self->ScreenElementList, homeX, homeY, screenX, screenY);
    self->ScreenElementList.ScreenElement.CClass.vtable =
        (int *)&__vt_16WheelElementList;

    // Create the rotating wheel sprite.
    RotateSprite *wheelElem = AllocateCClass<RotateSprite>();
    if (wheelElem != 0)
    {
        wheelElem = RotateSprite_ctor(wheelElem, WheelBmpName, 1, 0, 0);
    }
    self->Wheel = wheelElem;
    wheelElem->Sprite2.SlicedImage2.Image.JustX = 1;
    self->Wheel->Sprite2.SlicedImage2.Image.JustY = 1;

    // Create the fixed arrow overlay.
    SpriteScreenElement *item = AllocateCClass<SpriteScreenElement>();
    if (item != 0)
    {
        item = SpriteScreenElement_ctor(
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
    ScreenElementList_Destroy(&ScreenElementList, 0);
}

/**
 * Process wheel input and ease the displayed rotation toward its target slot.
 *
 * Original symbol: `update__16WheelElementList` at 0x8008281C.
 *
 * @param self Wheel element list to update.
 */
extern "C" void WheelElementList_Update(WheelElementList *self)
{
    SControl *pad = Pad;

    // Advance the wheel when Left is triggered.
    if (pad->Left.Triggered != 0)
    {
        SFX_PlayX(0xE9, 0x2000, 0);
        pad->Left.Triggered = 0;

        int idx = self->Slot;
        int maxItems = self->NumSlots;
        idx = idx + 1;
        self->Slot = idx;
        if (idx >= maxItems)
        {
            self->Slot = 0;
        }

        int newIdx = self->Slot;
        int targetRot = RotMap[newIdx][0];
        int curRot = self->CurrentRot;
        int diff = targetRot - curRot;
        self->TargetRot = targetRot;
        if (diff > 0)
        {
            self->TargetRot = targetRot - 0x1000;
        }
    }

    // Reload the controller pointer after the sound call path.
    pad = Pad;

    // Move back around the wheel when Right is triggered.
    if (pad->Right.Triggered != 0)
    {
        SFX_PlayX(0xE9, 0x2000, 0);
        pad->Right.Triggered = 0;

        int idx = self->Slot;
        idx = idx - 1;
        self->Slot = idx;
        if (idx < 0)
        {
            int maxItems = self->NumSlots;
            self->Slot = maxItems - 1;
        }

        int newIdx = self->Slot;
        int targetRot = RotMap[newIdx][0];
        int curRot = self->CurrentRot;
        int diff = targetRot - curRot;
        self->TargetRot = targetRot;
        if (diff < 0)
        {
            self->TargetRot = targetRot + 0x1000;
        }
    }

    // Smooth scroll towards target
    int target = self->TargetRot;
    int current = self->CurrentRot;
    int diff = target - current;

    if (diff < -0x258)
    {
        self->CurrentRot = current - 0x80;
    }
    else if (diff >= 0x259)
    {
        self->CurrentRot = current + 0x80;
    }
    else if (diff < -0x40)
    {
        self->CurrentRot = current - 0x40;
    }
    else if (diff >= 0x41)
    {
        self->CurrentRot = current + 0x40;
    }
    else
    {
        self->CurrentRot = target;
    }

    // Wrap around
    current = self->CurrentRot;
    if (current >= 0x1000)
    {
        int wrapped = current - 0x1000;
        self->CurrentRot = wrapped;
        int t = self->TargetRot;
        self->TargetRot = t - 0x1000;
    }

    current = self->CurrentRot;
    if (current < 0)
    {
        int wrapped = current + 0x1000;
        self->CurrentRot = wrapped;
        int t = self->TargetRot;
        self->TargetRot = t + 0x1000;
    }

    ScreenElementList_Update(&self->ScreenElementList);
}

/**
 * Draw the arrow elements and the wheel at its current rotation.
 *
 * Original symbol: `draw__16WheelElementListii` at 0x800829E8.
 *
 * @param self Wheel element list to draw.
 * @param x  X coordinate.
 * @param y  Y coordinate.
 */
extern "C" void WheelElementList_Draw(
    WheelElementList *self, int x, int y)
{
    ScreenElementList_Draw(&self->ScreenElementList, x, y);
    RotateSprite *wheel = self->Wheel;
    int screenX = self->ScreenElementList.ScreenElement.mScreenX;
    int screenY = self->ScreenElementList.ScreenElement.mScreenY;
    int rotation = self->CurrentRot;
    GetSpriteDispatch(wheel)->Draw(
        wheel, screenX + x, screenY + y, rotation);
}
