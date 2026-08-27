// LOCALTXT.h -- Localized frontend text owned by localtxt.obj.

#if !defined(LOCALTXT_LAYOUT_ONLY) && !defined(LOCALTXT_LAYOUT)
#define LOCALTXT_LAYOUT
#define LOCALTXT_LAYOUT_WAS_IMPLIED
#endif

#if defined(LOCALTXT_LAYOUT_ONLY) && !defined(LOCALTXT_LAYOUT)
#error "LOCALTXT_LAYOUT_ONLY requires LOCALTXT_LAYOUT"
#endif

#if defined(LOCALTXT_LAYOUT) && !defined(LOCALTXT_LAYOUT_EMITTED)
#define LOCALTXT_LAYOUT_EMITTED

/**
 * Target-proven field layout of the localized frontend text table.
 *
 * This narrow public definition mirrors MAIN_STRUCTS.h exactly. Each member
 * is one 32-bit pointer, giving the original 652-byte object layout.
 */
struct LocalText {
    char *pLevelHints;
    char *pEndGame;
    char *pEndRun;
    char *pNoVideoFiles;
    char *pNoCareerFiles;
    char *pSaveCareerQM;
    char *pAFileByThat;
    char *pNameAlreadyExists;
    char *pSKATETape;
    char *p20000Tape;
    char *p40000Tape;
    char *p5GoalsTape;
    char *pHiddenTape;
    char *pEnterName;
    char *pPressStart;
    char *pCareerMode;
    char *pSingleSession;
    char *pTwoPlayers;
    char *pPractice;
    char *pAutoKick;
    char *pOn;
    char *pOff;
    char *pOllie;
    char *pFullReplay;
    char *pHighlights;
    char *pSaveVideo;
    char *pLoadVideo;
    char *pExit;
    char *pSaveCareer;
    char *pRetry;
    char *pRestart;
    char *pSelectLevel;
    char *pEquipmentSelect;
    char *pWatchReplay;
    char *pMainMenu;
    char *pYouGotATape;
    char *pYouGotNTapes;
    char *pStatistics;
    char *pTotalScore;
    char *pHighScore;
    char *pTapesCollected;
    char *pTricksLanded;
    char *pBails;
    char *pBestCombo;
    char *pNone;
    char *pBestComboNTricksNPoints;
    char *pBestComboNTrickNPoints;
    char *pAreYouSure;
    char *pCarPlant;
    char *pNextHeat;
    char *pEndComp;
    char *pLoadComplete;
    char *pSaveComplete;
    char *pLoadFailed;
    char *pSaveFailed;
    char *pGrindSlide;
    char *pGrabTrick;
    char *pFlipTrick;
    char *pFlipTrickKick;
    char *pJumpOllie;
    char *pBooleanOn;
    char *pBooleanOff;
    char *pTrickTips;
    char *pScoreDisplay;
    char *pVibration;
    char *pHiResUnformatted1;
    char *pHiResUnformatted2;
    char *pHiResUnformatted3;
    char *pHiResFormatting;
    char *pHiResFormatFailed1;
    char *pHiResFormatFailed2;
    char *pHiResFormatFailed3;
    char *pHiResFormatComplete;
    char *pReturnToCompetition;
    char *pHiResScroll;
    char *pHiResUp;
    char *pHiResDown;
    char *pHiResUpOrDown;
    char *pBlueSteals;
    char *pRedSteals;
    char *pHighScores;
    char *pHorsePlayTo;
    char *pHorseChangeWord;
    char *pChangeSoundLevels;
    char *pSFXLevel;
    char *pMusicLevel;
    char *pSkipTrack;
    char *pBackToMainMenu;
    char *pYouGetTheLetter;
    char *pUpdateSaveFile;
    char *pOutOfBounds1;
    char *pOutOfBounds2;
    char *pOutOfBounds3;
    char *pOutOfBounds4;
    char *pReplayInstructions;
    char *pPressXToContinue;
    char *pNextPlayer;
    char *pPaused;
    char *pContinue;
    char *pSave;
    char *pQuit;
    char *pPleaseReinsertController;
    char *pEnterCheat;
    char *pToGoBack;
    char *pToSelect;
    char *pToAccept;
    char *pLevelSelect;
    char *pSkipToRestart;
    char *pAccept;
    char *pCancel;
    char *pPressAnyButton;
    char *pYes;
    char *pNo;
    char *pReallyQuit;
    char *pOptions;
    char *pEmpty;
    char *pPleaseInsert1;
    char *pPleaseInsert2;
    char *pPleaseInsert3;
    char *pPleaseInsert4;
    char *pUnformatted1;
    char *pUnformatted2;
    char *pUnformatted3;
    char *pUnformatted4;
    char *pSureWishToFormat1;
    char *pSureWishToFormat2;
    char *pSureWishToFormat3;
    char *pFormatting;
    char *pDoNotRemove1;
    char *pDoNotRemove2;
    char *pCheckingCard;
    char *pReadingCard;
    char *pCardFull1;
    char *pCardFull2;
    char *pCardFull3;
    char *pCardFull4;
    char *pCardFull5;
    char *pCardFull6;
    char *pCardFullWarning1;
    char *pCardFullWarning2;
    char *pCardFullWarning3;
    char *pCardFullWarning4;
    char *pCardFullWarning5;
    char *pSelectSaveSlot1;
    char *pSelectSaveSlot2;
    char *pOKToOverwrite1;
    char *pOKToOverwrite2;
    char *pOKToOverwrite3;
    char *pSaving;
    char *pWriteFailed;
    char *pFormatFailed;
    char *pFormatComplete;
    char *pSelectGameToLoad;
    char *pInsertCardWithSavedGame1;
    char *pInsertCardWithSavedGame2;
    char *pInsertCardWithSavedGame3;
    char *pInsertCardWithSavedGame4;
    char *pTPickup;
    char *pTapePickup;
    char *pEPickup;
    char *pSPickup;
    char *pKPickup;
    char *pAPickup;
};

#endif // LOCALTXT_LAYOUT && !LOCALTXT_LAYOUT_EMITTED

#if !defined(LOCALTXT_LAYOUT_ONLY)

#ifndef LOCALTXT_INC_H
#define LOCALTXT_INC_H

#ifndef LOCALTXT_LAYOUT_EMITTED
#error "LOCALTXT.h ordinary API requires the LocalText layout"
#endif

typedef char LocalText_size_must_be_652[(sizeof(LocalText) == 652) ? 1 : -1];

/**
 * Ordered frontend text pointers used by menus, prompts, and memory-card UI.
 *
 * The field layout is the original 652-byte `LocalText` layout recovered from
 * MAIN.SYM.  The strings themselves are defined by LOCALTXT.cpp.
 */
extern "C" LocalText GameText;

#endif // LOCALTXT_INC_H

#endif // !LOCALTXT_LAYOUT_ONLY

#ifdef LOCALTXT_LAYOUT_WAS_IMPLIED
#undef LOCALTXT_LAYOUT
#undef LOCALTXT_LAYOUT_WAS_IMPLIED
#endif
