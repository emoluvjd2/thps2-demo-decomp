#ifndef FRONTSCREENS_H
#define FRONTSCREENS_H

/** Run the main-menu screen and return the selected destination. */
int MainMenu_Main(void);

/** Run character selection for the requested game mode. */
int PlayerSelect_Main(int gameMode);

/** Run level selection and return the selected destination. */
int LevelSelect2_Main(void);

/** Run multiplayer selection and update the requested game mode. */
int MultiSelect_Main(int *gameMode);

#endif // FRONTSCREENS_H

#ifdef FRONTSCREENS_DECLARE_MAIN_MENU_CONSTRUCTOR_ABI
struct MainMenuScreen;

namespace MainMenuConstructorABI
{
/** Construct the owner-local main-menu screen through its target entry. */
extern "C" MainMenuScreen *__14MainMenuScreen(MainMenuScreen *screen);
}

#undef FRONTSCREENS_DECLARE_MAIN_MENU_CONSTRUCTOR_ABI
#endif

#ifdef FRONTSCREENS_DECLARE_MAIN_MENU_DESTRUCTOR_ABI
struct MainMenuScreen;
struct WheelElementList;

/** Addressable ABI names used by the main-menu dispatch tables. */
void MainMenuScreen_destructor_slot(MainMenuScreen *screen, int inCharge)
    asm("_._14MainMenuScreen");
void WheelElementList_destructor_slot(WheelElementList *list, int inCharge)
    asm("_._16WheelElementList");

#undef FRONTSCREENS_DECLARE_MAIN_MENU_DESTRUCTOR_ABI
#endif

#ifdef FRONTSCREENS_DECLARE_PLAYERSELECT_SCREEN_DESTRUCTOR_ABI
struct PlayerSelectScreen;

/** Addressable ABI name used by the player-select screen's manual vtable. */
void PlayerSelectScreen_dtor(PlayerSelectScreen *, int)
    asm("_._18PlayerSelectScreen");

#undef FRONTSCREENS_DECLARE_PLAYERSELECT_SCREEN_DESTRUCTOR_ABI
#endif

#ifdef FRONTSCREENS_DECLARE_PLAYERSELECT_ELEMENT_DESTRUCTOR_ABI
/** Addressable ABI names used by the player-select element dispatch tables. */
void PlayerSelect_MultiPlayerDestructorVtable(void)
    asm("_._24MultiPlayerScreenElement");
void PlayerSelect_StatsDestructorVtable(void)
    asm("_._18StatsScreenElement");
void PlayerSelect_SkaterDestructorVtable(void)
    asm("_._19SkaterScreenElement");
void PlayerSelect_BioDestructorVtable(void)
    asm("_._16BioScreenElement");
void PlayerSelect_CareerDestructorVtable(void)
    asm("_._19CareerScreenElement");
void PlayerSelect_PromptDestructorVtable(void)
    asm("_._19PromptScreenElement");
void PlayerSelect_ScoreDestructorVtable(void)
    asm("_._18ScoreScreenElement");

#undef FRONTSCREENS_DECLARE_PLAYERSELECT_ELEMENT_DESTRUCTOR_ABI
#endif

#ifdef FRONTSCREENS_DECLARE_LEVELSELECT_DESTRUCTOR_ABI
/** Addressable ABI names used by the level-select dispatch tables. */
void LevelSelectScreenDestructorVtable(void)
    asm("_._17LevelSelectScreen");
void SkaterNameScreenElementDestructorVtable(void)
    asm("_._23SkaterNameScreenElement");
void LevelSelectGoalDestructorVtable(void)
    asm("_._17GoalScreenElement");
void LevelSelectVideoDestructorVtable(void)
    asm("_._18VideoScreenElement");
void LevelSelectNameDestructorVtable(void)
    asm("_._17NameScreenElement");
void LevelSelectLevelNameDestructorVtable(void)
    asm("_._22LevelNameScreenElement");
void LevelSelectGlobeDestructorVtable(void)
    asm("_._18GlobeScreenElement");

#undef FRONTSCREENS_DECLARE_LEVELSELECT_DESTRUCTOR_ABI
#endif

#ifdef FRONTSCREENS_DECLARE_MULTISELECT_CONSTRUCTOR_ABI
struct MultiSelectScreen;

namespace MultiSelectScreenConstructorABI
{
/** Construct the owner-local multiplayer screen through its target entry. */
extern "C" MultiSelectScreen *__17MultiSelectScreen(
    MultiSelectScreen *screen);
}

#undef FRONTSCREENS_DECLARE_MULTISELECT_CONSTRUCTOR_ABI
#endif

#ifdef FRONTSCREENS_DECLARE_MULTISELECT_DESTRUCTOR_ABI
struct MultiSelectScreen;

/** Addressable ABI name used by the multiplayer screen's manual vtable. */
void MultiSelectScreen_dtor(MultiSelectScreen *screen, int inCharge)
    asm("_._17MultiSelectScreen");

#undef FRONTSCREENS_DECLARE_MULTISELECT_DESTRUCTOR_ABI
#endif

#ifdef FRONTSCREENS_DECLARE_LEVELSELECT_OWNER_VTABLES

/** LEVELSELECT2-owned screen-element virtual tables. */
extern LevelSelectVirtualFunction const __vt_17GoalScreenElement[];
extern LevelSelectVirtualFunction const __vt_18VideoScreenElement[];
extern LevelSelectVirtualFunction const __vt_17NameScreenElement[];
extern LevelSelectVirtualFunction const __vt_22LevelNameScreenElement[];
extern LevelSelectVirtualFunction const __vt_18GlobeScreenElement[];
extern LevelSelectVirtualFunction const __vt_23SkaterNameScreenElement[];
extern LevelSelectVirtualFunction const __vt_17LevelSelectScreen[];

#undef FRONTSCREENS_DECLARE_LEVELSELECT_OWNER_VTABLES
#endif

#ifdef FRONTSCREENS_DECLARE_MULTISELECT_OWNER_VTABLE

/** MULTISELECT-owned screen virtual table. */
extern MultiSelectVirtualFunction const __vt_17MultiSelectScreen[7];

#undef FRONTSCREENS_DECLARE_MULTISELECT_OWNER_VTABLE
#endif
