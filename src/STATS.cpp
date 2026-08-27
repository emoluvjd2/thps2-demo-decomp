#include "STATS.h"

/** Tony Hawk biography lines displayed by the skater-selection front end. */
char *HawkBio[HawkBioLineCount] = {
    "Possibly the most",
    "influential skate-",
    "boarder of all time,",
    "Tony has invented",
    "hundreds of tricks",
    "including the stale-",
    "fish, madonna, and",
    "720, and is the only",
    "person to land the",
    "vaunted 900.",
    0,
};

/** Chad Muska biography lines displayed by the skater-selection front end. */
char *MuskaBio[MuskaBioLineCount] = {
    "The Muska's popularity",
    "grows each and every",
    "year, a fact most",
    "recently evidenced",
    "by his choice as",
    "Transworld's Street",
    "Skater of 1999.",
    "Perhaps inspired",
    "by a youth spent",
    "growing up in",
    "Las Vegas, Chad is",
    "constantly pushing",
    "both street skating",
    "and equipment design",
    "in ever more",
    "outrageous directions.",
    0,
};

#define UNAVAILABLE_BIOGRAPHY \
    {" ", " ", "    Not Available", "      In Demo", 0}

/** Demo placeholder biography for Rodney Mullen. */
char *MullenBio[UnavailableBioLineCount] = UNAVAILABLE_BIOGRAPHY;

/** Demo placeholder biography for Bob Burnquist. */
char *BurnquistBio[UnavailableBioLineCount] = UNAVAILABLE_BIOGRAPHY;

/** Demo placeholder biography for Geoff Rowley. */
char *RowleyBio[UnavailableBioLineCount] = UNAVAILABLE_BIOGRAPHY;

/** Demo placeholder biography for Bucky Lasek. */
char *LasekBio[UnavailableBioLineCount] = UNAVAILABLE_BIOGRAPHY;

/** Demo placeholder biography for Kareem Campbell. */
char *CampbellBio[UnavailableBioLineCount] = UNAVAILABLE_BIOGRAPHY;

/** Demo placeholder biography for Andrew Reynolds. */
char *ReynoldsBio[UnavailableBioLineCount] = UNAVAILABLE_BIOGRAPHY;

/** Demo placeholder biography for Rune Glifberg. */
char *GlifbergBio[UnavailableBioLineCount] = UNAVAILABLE_BIOGRAPHY;

/** Demo placeholder biography for Jamie Thomas. */
char *ThomasBio[UnavailableBioLineCount] = UNAVAILABLE_BIOGRAPHY;

/** Demo placeholder biography for Elissa Steamer. */
char *SteamerBio[UnavailableBioLineCount] = UNAVAILABLE_BIOGRAPHY;

/** Demo placeholder biography for Steve Caballero. */
char *CaballeroBio[UnavailableBioLineCount] = UNAVAILABLE_BIOGRAPHY;

/** Demo placeholder biography for Eric Koston. */
char *KostonBio[UnavailableBioLineCount] = UNAVAILABLE_BIOGRAPHY;

/** Demo placeholder biography for the created-skater slot. */
char *CustomBio[UnavailableBioLineCount] = UNAVAILABLE_BIOGRAPHY;

#undef UNAVAILABLE_BIOGRAPHY

/**
 * The fourteen skater records used by gameplay and the front end.
 *
 * Array order and every initializer are recovered from the original
 * `stats.obj` contribution identified by `original/MAIN.SYM`.
 */
SStats Stats[SkaterStatsCount] = {
    // Tony Hawk
    {
        "hawk2", "hawk2_L", "hawk_fe", 0x0,
        16, 17, 18, 12, 8,
        3, 7, 7, 4, 0, "Tony Hawk",
        0x19B1A71FU, 0x59BD3DD8U, 0x52158AD3U,
        {
            {"b1hawk02.bmp", "Falcon 2", 0, 0, 0, 0, 0, 0},
            {"b1bird05.bmp", "B Logo", 0, 0, 0, 0, 0, 0},
            {"b1hawk03.bmp", "Evil Cat 2", 3, 1, 1, 1, 1, 0},
            {"b1hawk07.bmp", "Full Skull 99", 7, 2, 1, 1, 2, 0},
            {"b1bird09.bmp", "The End", 10, 3, 2, 2, 2, 0},
            {"b1hawk04.bmp", "Falcon", 15, 5, 2, 2, 4, 0},
            {"b1hawk01.bmp", "Evil Cat", 21, 6, 3, 3, 5, 0},
            {"b1hawk08.bmp", "Falcon 3", 24, 7, 3, 3, 6, 0},
        },
        {7, 5, 2, 6, 8, 5, 2, 2, 6, 7, 0, 1},
        "s2fhawk.bmp", HawkBio, 1, 0,
    },
    // Chad Muska
    {
        "muska2", "muska2_L", "muska_fe", 0x1000,
        16, 17, 18, 15, -5,
        6, 4, 4, 7, 1, "Chad Muska",
        0x3C18FEE7U, 0xEB2666C5U, 0xF29F6D93U,
        {
            {"b1musk05.bmp", "Large Red", 0, 0, 0, 0, 0, 0},
            {"b1shor02.bmp", "Medium Shortys", 0, 0, 0, 0, 0, 0},
            {"b1shor05.bmp", "Big Poppa", 3, 1, 1, 1, 1, 0},
            {"b1musk02.bmp", "XX Large Green", 7, 2, 2, 2, 1, 0},
            {"b1shor04.bmp", "Large Shortys", 10, 2, 3, 3, 2, 0},
            {"b1shor06.bmp", "Westsider", 15, 3, 4, 4, 2, 0},
            {"b1musk03.bmp", "X Large Blue", 21, 3, 5, 5, 3, 0},
            {"b1musk04.bmp", "Muska Thugsta", 24, 4, 6, 6, 3, 0},
        },
        {4, 3, 8, 4, 5, 6, 6, 8, 4, 2, 1, 0},
        "s2fmuska.bmp", MuskaBio, 1, -600,
    },
    // Bob Burnquist
    {
        "mullen", "mullen", "burnq_fe", 0x3000,
        16, 17, 18, 12, -5,
        5, 6, 6, 4, 4, "Bob Burnquist",
        0x19CBB6A2U, 0x5EB19FCCU, 0x9989BF40U,
        {
            {"b1burn10.bmp", "Brazil", 0, 0, 0, 0, 0, 0},
            {"b1burn02.bmp", "Stencil Logo", 0, 0, 0, 0, 0, 0},
            {"b1burn08.bmp", "Bob La Familia", 3, 1, 1, 1, 1, 0},
            {"b1burn05.bmp", "Ribbon", 7, 2, 2, 2, 2, 0},
            {"b1burn13.bmp", "Team United", 10, 3, 2, 2, 3, 0},
            {"b1burn03.bmp", "Bob Stencil Logo", 15, 4, 3, 3, 4, 0},
            {"b1burn12.bmp", "The Firm Team", 21, 5, 3, 3, 5, 0},
            {"b1burn07.bmp", "Team La Familia", 24, 6, 4, 4, 6, 0},
        },
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        "s2win2.bmp", BurnquistBio, 0, 0,
    },
    // Steve Caballero
    {
        "mullen", "mullen", "steamr_f", 0xD000,
        16, 17, 18, 15, 0,
        6, 4, 5, 6, 1, "Steve Caballero",
        0x87CAF65CU, 0xDA8162BAU, 0x4D46A154U,
        {
            {"b1stea02.bmp", "Pushin Up", 0, 0, 0, 0, 0, 0},
            {"b1stea07.bmp", "Monster", 0, 0, 0, 0, 0, 0},
            {"b1stea01.bmp", "Griffin", 3, 1, 1, 1, 1, 0},
            {"b1stea04.bmp", "Diamond Wizard", 7, 2, 2, 2, 2, 0},
            {"b1stea08.bmp", "Small Fists", 10, 3, 3, 2, 2, 0},
            {"b1stea03.bmp", "Steamer Cat", 15, 3, 4, 3, 3, 0},
            {"b1stea09.bmp", "Throwin Goats", 21, 3, 5, 4, 3, 0},
            {"b1stea05.bmp", "Stubborn Wood", 24, 4, 6, 5, 4, 0},
        },
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        "s2win2.bmp", CaballeroBio, 0, 0,
    },
    // Kareem Campbell
    {
        "sk2mod", "sk2mod", "campb_fe", 0x6000,
        16, 17, 18, 12, -7,
        7, 4, 4, 6, 1, "Kareem Campbell",
        0x4CB23BA0U, 0x2EB016A9U, 0x48BBE151U,
        {
            {"b1camp07.bmp", "Rock", 0, 0, 0, 0, 0, 0},
            {"b1camp02.bmp", "Assassins", 0, 0, 0, 0, 0, 0},
            {"b1camp05.bmp", "CS Logo", 3, 1, 1, 1, 1, 0},
            {"b1camp03.bmp", "City Stars", 7, 1, 2, 2, 2, 0},
            {"b1camp06.bmp", "Stealth", 10, 2, 3, 3, 2, 0},
            {"b1camp01.bmp", "Abalists 2000", 15, 2, 4, 4, 3, 0},
            {"b1camp04.bmp", "Kareem Campbell", 21, 3, 5, 5, 3, 0},
            {"b1camp08.bmp", "Racer", 24, 3, 6, 6, 4, 0},
        },
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        "s2win2.bmp", CampbellBio, 0, 0,
    },
    // Rune Glifberg
    {
        "mullen", "mullen", "glif_fe", 0x8000,
        16, 17, 18, 12, -8,
        4, 7, 7, 3, 3, "Rune Glifberg",
        0x74785085U, 0xBAF82C20U, 0xFBFDD452U,
        {
            {"b1glif01.bmp", "Psycho", 0, 0, 0, 0, 0, 0},
            {"b1glif02.bmp", "Flip HKD", 0, 0, 0, 0, 0, 0},
            {"b1glif03.bmp", "Lil Vikings", 3, 1, 1, 1, 1, 0},
            {"b1glif04.bmp", "Atomic 2", 7, 2, 1, 1, 2, 0},
            {"b1glif05.bmp", "Snake Eyes", 10, 3, 2, 2, 3, 0},
            {"b1glif06.bmp", "Purple Haze", 15, 4, 2, 2, 5, 0},
            {"b1glif07.bmp", "Easyrider", 21, 5, 3, 3, 6, 0},
            {"b1glif08.bmp", "Skunk", 24, 6, 3, 3, 7, 0},
        },
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        "s2win2.bmp", GlifbergBio, 0, 0,
    },
    // Eric Koston
    {
        "mullen", "mullen", "steamr_f", 0xE000,
        16, 17, 18, 15, 0,
        6, 4, 5, 6, 1, "Eric Koston",
        0x87CAF65CU, 0xDA8162BAU, 0x4D46A154U,
        {
            {"b1stea02.bmp", "Pushin Up", 0, 0, 0, 0, 0, 0},
            {"b1stea07.bmp", "Monster", 0, 0, 0, 0, 0, 0},
            {"b1stea01.bmp", "Griffin", 3, 1, 1, 1, 1, 0},
            {"b1stea04.bmp", "Diamond Wizard", 7, 2, 2, 2, 2, 0},
            {"b1stea08.bmp", "Small Fists", 10, 3, 3, 2, 2, 0},
            {"b1stea03.bmp", "Steamer Cat", 15, 3, 4, 3, 3, 0},
            {"b1stea09.bmp", "Throwin Goats", 21, 3, 5, 4, 3, 0},
            {"b1stea05.bmp", "Stubborn Wood", 24, 4, 6, 5, 4, 0},
        },
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        "s2win2.bmp", KostonBio, 0, 0,
    },
    // Bucky Lasek
    {
        "mullen", "mullen", "lasek_fe", 0x5000,
        16, 17, 18, 12, -6,
        5, 7, 6, 3, 0, "Bucky Lasek",
        0x27AEA97DU, 0xA85230E0U, 0x08C72375U,
        {
            {"b1lase01.bmp", "Bird", 0, 0, 0, 0, 0, 0},
            {"b1bird04.bmp", "B Logo", 0, 0, 0, 0, 0, 0},
            {"b1lase03.bmp", "Flame Bird", 3, 1, 1, 1, 1, 0},
            {"b1bird07.bmp", "The End", 7, 2, 1, 2, 2, 0},
            {"b1bird06.bmp", "Team Seal", 10, 2, 2, 2, 3, 0},
            {"b1lase04.bmp", "Bird Head", 15, 3, 2, 3, 5, 0},
            {"b1bird10.bmp", "Birdhouse", 21, 4, 3, 3, 6, 0},
            {"b1lase05.bmp", "Cat Killer", 24, 5, 3, 4, 7, 0},
        },
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        "s2win2.bmp", LasekBio, 0, 0,
    },
    // Rodney Mullen
    {
        "mullen", "mullen", "burnq_fe", 0x2000,
        16, 17, 18, 12, -5,
        5, 6, 6, 4, 4, "Rodney Mullen",
        0x19CBB6A2U, 0x5EB19FCCU, 0x9989BF40U,
        {
            {"b1burn10.bmp", "Brazil", 0, 0, 0, 0, 0, 0},
            {"b1burn02.bmp", "Stencil Logo", 0, 0, 0, 0, 0, 0},
            {"b1burn08.bmp", "Bob La Familia", 3, 1, 1, 1, 1, 0},
            {"b1burn05.bmp", "Ribbon", 7, 2, 2, 2, 2, 0},
            {"b1burn13.bmp", "Team United", 10, 3, 2, 2, 3, 0},
            {"b1burn03.bmp", "Bob Stencil Logo", 15, 4, 3, 3, 4, 0},
            {"b1burn12.bmp", "The Firm Team", 21, 5, 3, 3, 5, 0},
            {"b1burn07.bmp", "Team La Familia", 24, 6, 4, 4, 6, 0},
        },
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        "s2win2.bmp", MullenBio, 0, 0,
    },
    // Andrew Reynolds
    {
        "sk2mod", "sk2mod", "rynld_fe", 0x7000,
        0, 1, 2, 18, 7,
        6, 5, 3, 7, 1, "Andrew Reynolds",
        0xDB6C5180U, 0x0F557CCAU, 0x46074169U,
        {
            {"b1reyn08.bmp", "Monkey", 0, 0, 0, 0, 0, 0},
            {"b1bird11.bmp", "Birdhouse", 0, 0, 0, 0, 0, 0},
            {"b1reyn09.bmp", "Axe Man", 3, 1, 1, 1, 1, 0},
            {"b1reyn01.bmp", "Reaper 2", 7, 2, 2, 2, 1, 0},
            {"b1bird13.bmp", "B Logo", 10, 2, 2, 3, 2, 0},
            {"b1reyn03.bmp", "Executioner", 15, 3, 3, 5, 2, 0},
            {"b1bird08.bmp", "The End", 21, 3, 4, 6, 3, 0},
            {"b1reyn04.bmp", "Reaper", 24, 4, 5, 7, 3, 0},
        },
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        "s2win2.bmp", ReynoldsBio, 0, 0,
    },
    // Geoff Rowley
    {
        "sk2mod", "sk2mod", "rowley_f", 0x4000,
        16, 17, 18, 12, -13,
        7, 5, 3, 6, 1, "Geoff Rowley",
        0xE48875C7U, 0xFE601E03U, 0x3FE2DE21U,
        {
            {"b1rowl01.bmp", "Industrial", 0, 0, 0, 0, 0, 0},
            {"b1rowl02.bmp", "Magic Lamp", 0, 0, 0, 0, 0, 0},
            {"b1rowl03.bmp", "Punk Skull", 3, 1, 1, 1, 1, 0},
            {"b1rowl04.bmp", "Panic Attack", 7, 1, 2, 2, 2, 0},
            {"b1rowl05.bmp", "Magic Cat", 10, 2, 2, 3, 2, 0},
            {"b1rowl06.bmp", "White Trash", 15, 2, 3, 5, 3, 0},
            {"b1rowl08.bmp", "Bull", 21, 3, 4, 6, 3, 0},
            {"b1rowl07.bmp", "Rocky", 24, 3, 5, 7, 4, 0},
        },
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        "s2win2.bmp", RowleyBio, 0, 0,
    },
    // Elissa Steamer
    {
        "sk2mod", "sk2mod", "steamr_f", 0xA000,
        16, 17, 18, 15, 0,
        6, 4, 5, 6, 1, "Elissa Steamer",
        0x87CAF65CU, 0xDA8162BAU, 0x4D46A154U,
        {
            {"b1stea02.bmp", "Pushin Up", 0, 0, 0, 0, 0, 0},
            {"b1stea07.bmp", "Monster", 0, 0, 0, 0, 0, 0},
            {"b1stea01.bmp", "Griffin", 3, 1, 1, 1, 1, 0},
            {"b1stea04.bmp", "Diamond Wizard", 7, 2, 2, 2, 2, 0},
            {"b1stea08.bmp", "Small Fists", 10, 3, 3, 2, 2, 0},
            {"b1stea03.bmp", "Steamer Cat", 15, 3, 4, 3, 3, 0},
            {"b1stea09.bmp", "Throwin Goats", 21, 3, 5, 4, 3, 0},
            {"b1stea05.bmp", "Stubborn Wood", 24, 4, 6, 5, 4, 0},
        },
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        "s2win2.bmp", SteamerBio, 0, 0,
    },
    // Jamie Thomas
    {
        "sk2mod", "sk2mod", "thomas_f", 0x9000,
        16, 17, 18, 12, -5,
        5, 5, 4, 7, 1, "Jamie Thomas",
        0x19B1A71FU, 0xA197689BU, 0x90B3E1B3U,
        {
            {"b1thom04.bmp", "Mug Shot", 0, 0, 0, 0, 0, 0},
            {"b1zero02.bmp", "Medium Bold", 0, 0, 0, 0, 0, 0},
            {"b1zero06.bmp", "Bloody Nose", 3, 1, 1, 1, 1, 0},
            {"b1zero05.bmp", "American Zero", 7, 2, 2, 2, 1, 0},
            {"b1zero08.bmp", "Bird", 10, 2, 2, 3, 2, 0},
            {"b1zero07.bmp", "Small Skull", 15, 3, 3, 4, 2, 0},
            {"b1thom03.bmp", "Skeleton", 21, 4, 4, 5, 3, 0},
            {"b1thom01.bmp", "Smith Grind", 24, 5, 5, 6, 3, 0},
        },
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        "s2win2.bmp", ThomasBio, 0, 0,
    },
    // Created Skater
    {
        "sk2mod", "sk2mod", "fatcop_f", 0xB000,
        16, 17, 18, 12, -5,
        5, 5, 5, 5, 2, "Created Skater",
        0xFB7CFA8BU, 0xA1CFA04AU, 0x956362BCU,
        {
            {"b1offi04.bmp", "Got Pork", 0, 0, 0, 0, 0, 0},
            {"b1offi06.bmp", "American Way", 0, 0, 0, 0, 0, 0},
            {"b1offi02.bmp", "Double Dunkin", 3, 1, 1, 1, 1, 0},
            {"b1offi07.bmp", "Nightstick", 7, 2, 2, 2, 2, 0},
            {"b1offi03.bmp", "Disaster", 10, 3, 2, 3, 3, 0},
            {"b1offi08.bmp", "On the Beat", 15, 4, 3, 3, 4, 0},
            {"b1offi05.bmp", "3 Course Meal", 21, 4, 4, 4, 5, 0},
            {"b1offi01.bmp", "Game Over", 24, 5, 5, 5, 5, 0},
        },
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        "s2win2.bmp", CustomBio, 0, 0,
    },
};

#define LEVEL_LOCKED_FLAG ((SLevelFlag)1)

/**
 * The ten level records present in the prototype executable.
 *
 * Explicit char casts retain the raw high-bit STR identifiers in the signed
 * byte field recorded by MAIN_STRUCTS.
 */
SLevel Levels[LevelStatsCount] = {
    // Marseille
    {
        "Marseille", "France", "mar",
        0, 0, "SkMar_T",
        0U, 0U, 0U, "mar.bmp", (char)0xE5,
        {0, -21}, "s2levl03.bmp",
        0, 0, 5000, 15000, 1,
        {0, 0}, {0, 0}, 0,
        0, 0, 0, 0, 0, 0,
    },
    // SkateStreet
    {
        "SkateStreet", "Ventura", "ware",
        0, 0, "SkSS_T",
        0U, 0U, 0U, "levna.bmp", (char)0xE4,
        {2638, -8}, "s2levl04.bmp",
        0, 0, 5000, 15000, 0,
        {50000, 50000}, {0, 0}, 0,
        0, 0, 0, 0, 0, 0,
    },
    // New York City
    {
        "NY City", "New York", "ware",
        0, 0, "SkNY_T",
        0U, 0U, 0U, "levna.bmp", (char)0xE4,
        {3238, -24}, "s2levl05.bmp",
        0, 0, 5000, 15000, 0,
        {0, 0}, {0, 0}, 0,
        0, 0, 0, 0, 0, 0,
    },
    // School 2
    {
        "School 2", "Southern Cal", "SCHOOL",
        0, 0, "SkSl2_T",
        0U, 0U, 0U, "levna.bmp", (char)0xE4,
        {2668, -6}, "s2levl06.bmp",
        0, 0, 5000, 15000, 0,
        {0, 0}, {0, 0}, 0,
        0, 0, 0, 0, 0, 0,
    },
    // Philadelphia
    {
        "Philadelphia", "Pennsylvania", "ware",
        0, 0, "SkPh_T",
        0U, 0U, 0U, "levna.bmp", (char)0xE4,
        {3148, -23}, "s2levl08.bmp",
        0, 0, 69, 666, 0,
        {0, 0}, {0, 0}, 0,
        0, 0, 0, 0, 0, 0,
    },
    // Venice Beach
    {
        "Venice Beach", "California", "SF",
        0, 0, "SkVen_T",
        0U, 0U, 0U, "levna.bmp", (char)0xE4,
        {2668, -6}, "s2levl09.bmp",
        0, 0, 69, 666, 0,
        {0, 0}, {0, 0}, 0,
        0, 0, 0, 0, 0, 0,
    },
    // The Hangar
    {
        "The Hangar", "Tadzhikistan", "SF",
        0, 0, "SkVen_T",
        0U, 0U, 0U, "levna.bmp", (char)0xE4,
        {760, -19}, "s2levl09.bmp",
        0, 0, 69, 666, 0,
        {0, 0}, {0, 0}, 0,
        0, 0, 0, 0, 0, 0,
    },
    // The Bullring
    {
        "The Bullring", "Mexico", "SF",
        0, 0, "SkVen_T",
        0U, 0U, 0U, "levna.bmp", (char)0xE4,
        {2732, -5}, "s2levl09.bmp",
        0, 0, 69, 666, 0,
        {0, 0}, {0, 0}, 0,
        0, 0, 0, 0, 0, 0,
    },
    // Demo park
    {
        "demo park", "", "",
        0, LEVEL_LOCKED_FLAG, "",
        0U, 0U, 0U, "", 0,
        {0, 0}, "",
        0, 0, 5000, 0, 0,
        {0, 0}, {0, 0}, 1,
        0, 0, 0, 0, 0, 0,
    },
    // End-of-table placeholder
    {
        "", "", "",
        0, LEVEL_LOCKED_FLAG, "",
        0U, 0U, 0U, "", 0,
        {0, 0}, "",
        0, 0, 0, 0, 0,
        {0, 0}, {0, 0}, 1,
        0, 0, 0, 0, 0, 0,
    },
};

#undef LEVEL_LOCKED_FLAG
