#ifndef COMPETITION_H
#define COMPETITION_H

struct CCompetition;

#endif // COMPETITION_H

#ifdef COMPETITION_DECLARE_ADDRESS_VIEWS
/** Opaque address views of COMPETITION's target-ordered text owners. */
extern char s_HEAT__d_OVER__800db9a0[];
extern char s_SCOREBOARD_800db9b0[];
extern char s_JUDGE__d__d__d_800b393c[];
extern char s_JUDGE__d_800db9c0[];
extern char s_HEAT__d_SCORE___800b3950[];
extern char s__d__d_800db994[];
extern char s_Congratulations__800b3734[];
extern char s_You_scored_all_golds__800b37e8[];
extern char s_Your_are_the_top_pro_skater_800b3800[];
extern char s_Watch_this_then_get_off_your_800b3820[];
extern char s_couch_and_go_skate__800b3840[];
extern char s_Your_third_medal_Good_going__800b3854[];
extern char s_You_just_might_become_800b3874[];
extern char s_an_average_skater_800b388c[];
extern char s_pro_skaters_get_all_golds_800b38a0[];
extern char s_COMPETITION_STANDINGS_800b38fc[];
extern char s_1_2_3_Total_800b3914[];
extern char s_Heat_800db980[];
extern char s__s_800db990[];
extern char s___800db99c[];
extern char s_You_win_the_Gold__800b38c0[];
extern char s_You_win_the_Silver__800b38d4[];
extern char s_You_win_the_Bronze__800b38e8[];
extern char s_You_lose__800db93c[];
extern char s__s__1st_place_800db948[];
extern char s__s__2nd_place_800db958[];
extern char s__s__3rd_place_800db968[];
extern char s__dth_800db978[];
extern char s__s__s___HEAT__d_800db92c[];

/** Three competition medal-message records constructed at startup. */
extern SMedalMessage MedalMessage[3];

#undef COMPETITION_DECLARE_ADDRESS_VIEWS
#endif

#ifdef COMPETITION_DECLARE_OWNER_STATE_VIEW
/** Process-lifetime competition state owned by COMPETITION. */
extern CCompetition theCompetition;

#undef COMPETITION_DECLARE_OWNER_STATE_VIEW
#endif
