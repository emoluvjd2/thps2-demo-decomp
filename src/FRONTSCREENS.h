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
