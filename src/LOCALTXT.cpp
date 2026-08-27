// LOCALTXT.cpp -- Localized frontend text data owned by localtxt.obj.
// Original: localtxt.obj

#include "LOCALTXT.h"

// Each private name below gives one target literal stable storage. The table
// deliberately reuses three of those names, preserving the target's shared
// relocations. GCC 2.91.66 selects .sdata for strings of at most 16 bytes with
// this TU's recovered -G16 setting.
#define DEFINE_LOCAL_TEXT(name, value) \
    static const char name##Storage[] = value; \
    static char *const name = const_cast<char *>(name##Storage)

DEFINE_LOCAL_TEXT(kLevelHintsText, "Tape Challenges");
DEFINE_LOCAL_TEXT(kEndGameText, "End Game");
DEFINE_LOCAL_TEXT(kEndRunText, "End Run");
DEFINE_LOCAL_TEXT(kNoVideoFilesText, "No replay files");
DEFINE_LOCAL_TEXT(kNoCareerFilesText, "No career files");
DEFINE_LOCAL_TEXT(kSaveCareerQMText, "Save Career?");
DEFINE_LOCAL_TEXT(kAFileByThatText, "A file by that");
DEFINE_LOCAL_TEXT(kNameAlreadyExistsText, "name already exists");
DEFINE_LOCAL_TEXT(kSKATETapeText, "S-K-A-T-E TAPE!");
DEFINE_LOCAL_TEXT(k20000TapeText, "HI SCORE TAPE!");
DEFINE_LOCAL_TEXT(k40000TapeText, "PRO SCORE TAPE!");
DEFINE_LOCAL_TEXT(k5GoalsTapeText, "5 %s TAPE!");
DEFINE_LOCAL_TEXT(kHiddenTapeText, "HIDDEN TAPE!");
DEFINE_LOCAL_TEXT(kEnterNameText, "Enter name");
DEFINE_LOCAL_TEXT(kPressStartText, "Press start");
DEFINE_LOCAL_TEXT(kCareerModeText, "Career mode");
DEFINE_LOCAL_TEXT(kSingleSessionText, "Single session");
DEFINE_LOCAL_TEXT(kTwoPlayersText, "Two players");
DEFINE_LOCAL_TEXT(kPracticeText, "Practice");
DEFINE_LOCAL_TEXT(kAutoKickText, "Autokick");
DEFINE_LOCAL_TEXT(kOnText, "On");
DEFINE_LOCAL_TEXT(kOffText, "Off");
DEFINE_LOCAL_TEXT(kOllieText, "Ollie");
DEFINE_LOCAL_TEXT(kFullReplayText, "Full Replay");
DEFINE_LOCAL_TEXT(kHighlightsText, "Highlights");
DEFINE_LOCAL_TEXT(kSaveVideoText, "Save Replay");
DEFINE_LOCAL_TEXT(kLoadVideoText, "Load Replay");
DEFINE_LOCAL_TEXT(kExitText, "Exit");
DEFINE_LOCAL_TEXT(kSaveCareerText, "Save Career");
DEFINE_LOCAL_TEXT(kRetryText, "Retry");
DEFINE_LOCAL_TEXT(kRestartText, "Restart Competition");
DEFINE_LOCAL_TEXT(kSelectLevelText, "Level Select");
DEFINE_LOCAL_TEXT(kEquipmentSelectText, "Equipment Select");
DEFINE_LOCAL_TEXT(kWatchReplayText, "Watch replay");
DEFINE_LOCAL_TEXT(kMainMenuText, "Main Menu");
DEFINE_LOCAL_TEXT(kYouGotATapeText, "You got a tape!");
DEFINE_LOCAL_TEXT(kYouGotNTapesText, "You got %d tapes!");
DEFINE_LOCAL_TEXT(kStatisticsText, "Statistics");
DEFINE_LOCAL_TEXT(kTotalScoreText, "Total Score");
DEFINE_LOCAL_TEXT(kHighScoreText, "High Score");
DEFINE_LOCAL_TEXT(kTapesCollectedText, "Tapes Collected");
DEFINE_LOCAL_TEXT(kTricksLandedText, "Tricks Landed");
DEFINE_LOCAL_TEXT(kBailsText, "Bails");
DEFINE_LOCAL_TEXT(kBestComboText, "Best Combo");
DEFINE_LOCAL_TEXT(kNoneText, "None");
DEFINE_LOCAL_TEXT(kBestComboNTricksNPointsText, "Best Combo - %d Tricks - %d Points");
DEFINE_LOCAL_TEXT(kBestComboNTrickNPointsText, "Best Combo - %d Trick - %d Points");
DEFINE_LOCAL_TEXT(kAreYouSureText, "Are you sure?");
DEFINE_LOCAL_TEXT(kCarPlantText, "Car plant");
DEFINE_LOCAL_TEXT(kNextHeatText, "Next Heat");
DEFINE_LOCAL_TEXT(kEndCompText, "End Competition");
DEFINE_LOCAL_TEXT(kLoadCompleteText, "load completed");
DEFINE_LOCAL_TEXT(kSaveCompleteText, "save completed");
DEFINE_LOCAL_TEXT(kLoadFailedText, "load failed");
DEFINE_LOCAL_TEXT(kSaveFailedText, "save failed");
DEFINE_LOCAL_TEXT(kGrindSlideText, "grind/slide");
DEFINE_LOCAL_TEXT(kGrabTrickText, "grab trick");
DEFINE_LOCAL_TEXT(kFlipTrickText, "flip trick");
DEFINE_LOCAL_TEXT(kFlipTrickKickText, "flip trick/kick");
DEFINE_LOCAL_TEXT(kJumpOllieText, "jump/ollie");
DEFINE_LOCAL_TEXT(kBooleanOnText, "on <off>");
DEFINE_LOCAL_TEXT(kBooleanOffText, "<on> off");
DEFINE_LOCAL_TEXT(kTrickTipsText, "trick tips");
DEFINE_LOCAL_TEXT(kScoreDisplayText, "score display");
DEFINE_LOCAL_TEXT(kVibrationText, "vibration");
DEFINE_LOCAL_TEXT(kHiResUnformatted1Text, "the memory card in");
DEFINE_LOCAL_TEXT(kHiResUnformatted2Text, "memory card slot 1");
DEFINE_LOCAL_TEXT(kHiResUnformatted3Text, "is unformatted");
DEFINE_LOCAL_TEXT(kHiResFormattingText, "Formatting memory card");
DEFINE_LOCAL_TEXT(kHiResFormatFailed1Text, "Format failed");
DEFINE_LOCAL_TEXT(kHiResFormatFailed2Text, "Try again or insert");
DEFINE_LOCAL_TEXT(kHiResFormatFailed3Text, "A new memory card");
DEFINE_LOCAL_TEXT(kHiResFormatCompleteText, "Format complete");
DEFINE_LOCAL_TEXT(kReturnToCompetitionText, "Return to competition");
DEFINE_LOCAL_TEXT(kHiResScrollText, "Scroll %s for more choices");
DEFINE_LOCAL_TEXT(kHiResUpText, "up");
DEFINE_LOCAL_TEXT(kHiResDownText, "down");
DEFINE_LOCAL_TEXT(kHiResUpOrDownText, "up or down");
DEFINE_LOCAL_TEXT(kBlueStealsText, "blue steals!");
DEFINE_LOCAL_TEXT(kRedStealsText, "red steals!");
DEFINE_LOCAL_TEXT(kHighScoresText, "%s high scores");
DEFINE_LOCAL_TEXT(kHorsePlayToText, "Play to:");
DEFINE_LOCAL_TEXT(kHorseChangeWordText, "Change word");
DEFINE_LOCAL_TEXT(kChangeSoundLevelsText, "Sound Levels");
DEFINE_LOCAL_TEXT(kSFXLevelText, "SFX level: %d");
DEFINE_LOCAL_TEXT(kMusicLevelText, "Music level: %d");
DEFINE_LOCAL_TEXT(kSkipTrackText, "Skip track");
DEFINE_LOCAL_TEXT(kBackToMainMenuText, "Back");
DEFINE_LOCAL_TEXT(kYouGetTheLetterText, "You get the letter");
DEFINE_LOCAL_TEXT(kUpdateSaveFileText, "Update save file");
DEFINE_LOCAL_TEXT(kOutOfBounds1Text, "Out of bounds!");
DEFINE_LOCAL_TEXT(kOutOfBounds2Text, "Nice one!");
DEFINE_LOCAL_TEXT(kOutOfBounds3Text, "You suck!");
DEFINE_LOCAL_TEXT(kOutOfBounds4Text, "Don't do drugs!");
DEFINE_LOCAL_TEXT(kReplayInstructionsText, "X to exit   Square to pause   Circle to frame adv");
DEFINE_LOCAL_TEXT(kPressXToContinueText, "Press X to continue");
DEFINE_LOCAL_TEXT(kNextPlayerText, "Next Player");
DEFINE_LOCAL_TEXT(kPausedText, "Paused");
DEFINE_LOCAL_TEXT(kContinueText, "Continue");
DEFINE_LOCAL_TEXT(kSaveText, "Save game ?");
DEFINE_LOCAL_TEXT(kQuitText, "Quit");
DEFINE_LOCAL_TEXT(kPleaseReinsertControllerText, "Please reinsert controller");
DEFINE_LOCAL_TEXT(kEnterCheatText, "enter cheat");
DEFINE_LOCAL_TEXT(kToGoBackText, "go back");
DEFINE_LOCAL_TEXT(kToSelectText, "select");
DEFINE_LOCAL_TEXT(kToAcceptText, "accept");
DEFINE_LOCAL_TEXT(kLevelSelectText, "select level");
DEFINE_LOCAL_TEXT(kSkipToRestartText, "skip to restart");
DEFINE_LOCAL_TEXT(kCancelText, "cancel");
DEFINE_LOCAL_TEXT(kPressAnyButtonText, "press any button");
DEFINE_LOCAL_TEXT(kYesText, "yes");
DEFINE_LOCAL_TEXT(kNoText, "no");
DEFINE_LOCAL_TEXT(kReallyQuitText, "really quit ?");
DEFINE_LOCAL_TEXT(kOptionsText, "options");
DEFINE_LOCAL_TEXT(kEmptyText, "Empty");
DEFINE_LOCAL_TEXT(kPleaseInsert1Text, "Please insert a memory");
DEFINE_LOCAL_TEXT(kPleaseInsert2Text, "card in memory card");
DEFINE_LOCAL_TEXT(kPleaseInsert3Text, "slot 1 or press any");
DEFINE_LOCAL_TEXT(kPleaseInsert4Text, "button to cancel");
DEFINE_LOCAL_TEXT(kUnformatted1Text, "The memory card in");
DEFINE_LOCAL_TEXT(kUnformatted2Text, "memory card slot 1 is");
DEFINE_LOCAL_TEXT(kUnformatted3Text, "unformatted. Do you");
DEFINE_LOCAL_TEXT(kUnformatted4Text, "want to format it ?");
DEFINE_LOCAL_TEXT(kSureWishToFormat1Text, "Are you sure you wish to");
DEFINE_LOCAL_TEXT(kSureWishToFormat2Text, "format the memory card");
DEFINE_LOCAL_TEXT(kSureWishToFormat3Text, "in memory card slot 1 ?");
DEFINE_LOCAL_TEXT(kFormattingText, "formatting memory card");
DEFINE_LOCAL_TEXT(kDoNotRemove1Text, "do not remove");
DEFINE_LOCAL_TEXT(kDoNotRemove2Text, "memory card");
DEFINE_LOCAL_TEXT(kCheckingCardText, "checking memory card");
DEFINE_LOCAL_TEXT(kReadingCardText, "reading memory card");
DEFINE_LOCAL_TEXT(kCardFull1Text, "your memory card in");
DEFINE_LOCAL_TEXT(kCardFull2Text, "memory card slot 1 is full");
DEFINE_LOCAL_TEXT(kCardFull3Text, "insert a memory card with");
DEFINE_LOCAL_TEXT(kCardFull4Text, "at least %d free block%s or delete");
DEFINE_LOCAL_TEXT(kCardFull5Text, "%d block%s using the console's");
DEFINE_LOCAL_TEXT(kCardFull6Text, "internal memory card manager");
DEFINE_LOCAL_TEXT(kCardFullWarning1Text, "please insert a memory card");
DEFINE_LOCAL_TEXT(kCardFullWarning2Text, "with at least 1 free block if");
DEFINE_LOCAL_TEXT(kCardFullWarning3Text, "you wish to save your progress");
DEFINE_LOCAL_TEXT(kCardFullWarning4Text, "saving a replay will require an");
DEFINE_LOCAL_TEXT(kCardFullWarning5Text, "additional 3 blocks");
DEFINE_LOCAL_TEXT(kSelectSaveSlot1Text, "Select save slot to save");
DEFINE_LOCAL_TEXT(kSelectSaveSlot2Text, "to and press any button");
DEFINE_LOCAL_TEXT(kOKToOverwrite1Text, "Okay to overwrite");
DEFINE_LOCAL_TEXT(kOKToOverwrite2Text, "previously saved game");
DEFINE_LOCAL_TEXT(kOKToOverwrite3Text, "in this save slot ?");
DEFINE_LOCAL_TEXT(kSavingText, "Now Saving data");
DEFINE_LOCAL_TEXT(kFormatFailedText, "format failed");
DEFINE_LOCAL_TEXT(kFormatCompleteText, "format completed");
DEFINE_LOCAL_TEXT(kSelectGameToLoadText, "select game to load");
DEFINE_LOCAL_TEXT(kInsertCardWithSavedGame1Text, "insert memory card with");
DEFINE_LOCAL_TEXT(kInsertCardWithSavedGame2Text, "saved game in memory");
DEFINE_LOCAL_TEXT(kInsertCardWithSavedGame3Text, "card slot 1 or press any");
DEFINE_LOCAL_TEXT(kTPickupText, "T");
DEFINE_LOCAL_TEXT(kTapePickupText, "tape");
DEFINE_LOCAL_TEXT(kEPickupText, "E");
DEFINE_LOCAL_TEXT(kSPickupText, "S");
DEFINE_LOCAL_TEXT(kKPickupText, "K");
DEFINE_LOCAL_TEXT(kAPickupText, "A");

#undef DEFINE_LOCAL_TEXT

/**
 * Target-proven localized text table exported by localtxt.obj.
 *
 * Repeated entries deliberately reuse the same named string so their pointer
 * relocations resolve to the same address, as they do in the original binary.
 */
LocalText GameText = {
    kLevelHintsText,
    kEndGameText,
    kEndRunText,
    kNoVideoFilesText,
    kNoCareerFilesText,
    kSaveCareerQMText,
    kAFileByThatText,
    kNameAlreadyExistsText,
    kSKATETapeText,
    k20000TapeText,
    k40000TapeText,
    k5GoalsTapeText,
    kHiddenTapeText,
    kEnterNameText,
    kPressStartText,
    kCareerModeText,
    kSingleSessionText,
    kTwoPlayersText,
    kPracticeText,
    kAutoKickText,
    kOnText,
    kOffText,
    kOllieText,
    kFullReplayText,
    kHighlightsText,
    kSaveVideoText,
    kLoadVideoText,
    kExitText,
    kSaveCareerText,
    kRetryText,
    kRestartText,
    kSelectLevelText,
    kEquipmentSelectText,
    kWatchReplayText,
    kMainMenuText,
    kYouGotATapeText,
    kYouGotNTapesText,
    kStatisticsText,
    kTotalScoreText,
    kHighScoreText,
    kTapesCollectedText,
    kTricksLandedText,
    kBailsText,
    kBestComboText,
    kNoneText,
    kBestComboNTricksNPointsText,
    kBestComboNTrickNPointsText,
    kAreYouSureText,
    kCarPlantText,
    kNextHeatText,
    kEndCompText,
    kLoadCompleteText,
    kSaveCompleteText,
    kLoadFailedText,
    kSaveFailedText,
    kGrindSlideText,
    kGrabTrickText,
    kFlipTrickText,
    kFlipTrickKickText,
    kJumpOllieText,
    kBooleanOnText,
    kBooleanOffText,
    kTrickTipsText,
    kScoreDisplayText,
    kVibrationText,
    kHiResUnformatted1Text,
    kHiResUnformatted2Text,
    kHiResUnformatted3Text,
    kHiResFormattingText,
    kHiResFormatFailed1Text,
    kHiResFormatFailed2Text,
    kHiResFormatFailed3Text,
    kHiResFormatCompleteText,
    kReturnToCompetitionText,
    kHiResScrollText,
    kHiResUpText,
    kHiResDownText,
    kHiResUpOrDownText,
    kBlueStealsText,
    kRedStealsText,
    kHighScoresText,
    kHorsePlayToText,
    kHorseChangeWordText,
    kChangeSoundLevelsText,
    kSFXLevelText,
    kMusicLevelText,
    kSkipTrackText,
    kBackToMainMenuText,
    kYouGetTheLetterText,
    kUpdateSaveFileText,
    kOutOfBounds1Text,
    kOutOfBounds2Text,
    kOutOfBounds3Text,
    kOutOfBounds4Text,
    kReplayInstructionsText,
    kPressXToContinueText,
    kNextPlayerText,
    kPausedText,
    kContinueText,
    kSaveText,
    kQuitText,
    kPleaseReinsertControllerText,
    kEnterCheatText,
    kToGoBackText,
    kToSelectText,
    kToAcceptText,
    kLevelSelectText,
    kSkipToRestartText,
    kToAcceptText,
    kCancelText,
    kPressAnyButtonText,
    kYesText,
    kNoText,
    kReallyQuitText,
    kOptionsText,
    kEmptyText,
    kPleaseInsert1Text,
    kPleaseInsert2Text,
    kPleaseInsert3Text,
    kPleaseInsert4Text,
    kUnformatted1Text,
    kUnformatted2Text,
    kUnformatted3Text,
    kUnformatted4Text,
    kSureWishToFormat1Text,
    kSureWishToFormat2Text,
    kSureWishToFormat3Text,
    kFormattingText,
    kDoNotRemove1Text,
    kDoNotRemove2Text,
    kCheckingCardText,
    kReadingCardText,
    kCardFull1Text,
    kCardFull2Text,
    kCardFull3Text,
    kCardFull4Text,
    kCardFull5Text,
    kCardFull6Text,
    kCardFullWarning1Text,
    kCardFullWarning2Text,
    kCardFullWarning3Text,
    kCardFullWarning4Text,
    kCardFullWarning5Text,
    kSelectSaveSlot1Text,
    kSelectSaveSlot2Text,
    kOKToOverwrite1Text,
    kOKToOverwrite2Text,
    kOKToOverwrite3Text,
    kSavingText,
    kSaveFailedText,
    kFormatFailedText,
    kFormatCompleteText,
    kSelectGameToLoadText,
    kInsertCardWithSavedGame1Text,
    kInsertCardWithSavedGame2Text,
    kInsertCardWithSavedGame3Text,
    kPleaseInsert4Text,
    kTPickupText,
    kTapePickupText,
    kEPickupText,
    kSPickupText,
    kKPickupText,
    kAPickupText,
};
