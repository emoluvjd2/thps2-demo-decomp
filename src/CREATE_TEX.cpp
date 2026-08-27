// CREATE_TEX.cpp -- Reconstructed translation unit
// Original: CREATE_TEX.CPP
// Functions: 9 reconstructed entries
// Address range: 0x8006D0C0 - 0x8006D86C
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#include "globals.h"

#define CREATE_TEX_LIST_LAYOUT
#define CREATE_TEX_LIST_LAYOUT_ONLY
#include "CREATE.h"
#undef CREATE_TEX_LIST_LAYOUT_ONLY
#undef CREATE_TEX_LIST_LAYOUT

#define CREATE_TEX_SPEC_LAYOUT
#define CREATE_TEX_SPEC_LAYOUT_ONLY
#include "CREATE.h"
#undef CREATE_TEX_SPEC_LAYOUT_ONLY
#undef CREATE_TEX_SPEC_LAYOUT

typedef char CreateTex_Spec_size_must_be_72[
    (sizeof(CCreateTexSpec) == 72) ? 1 : -1];

// === Typed aliases for the original C++ symbols ===
extern "C" CCreateTexSpec *CCreateTexSpec_ctor(
    CCreateTexSpec *, char *, char *, int, _SCreateTexList *,
    _SCreateTexList *, _SCreateTexList *, _SCreateTexList *)
    asm("__14CCreateTexSpecPcT1iP15_SCreateTexListN34");

/** Process-lifetime create-a-skater specification tables owned by this TU. */
extern "C" {
CCreateTexSpec HeadSpec[15];
CCreateTexSpec UpperBodySpec[7];
CCreateTexSpec LowerBodySpec[5];
}

/*
 * CREATE_TEX's complete initialized small-data contribution.  The texture
 * names, menu labels, and two-entry texture lists occupy
 * 0x800DAB50-0x800DB260 in the target.  Keeping the semantic objects in target
 * order preserves every pointer relocation without raw-address initializers.
 */
static char CreateTex_faceSkin2[]
    __attribute__((section(".sdata"), aligned(4))) = "fceskin2";
static char CreateTex_buzzCut[]
    __attribute__((section(".sdata"), aligned(4))) = "Buzz cut";
static char CreateTex_faceAsian3[]
    __attribute__((section(".sdata"), aligned(4))) = "fceAsn_3";
static char CreateTex_AsianGuyStorage[] asm("DAT_800dab74")
    __attribute__((section(".sdata"), aligned(4))) = "Asian Guy";
static char CreateTex_faceShadow[]
    __attribute__((section(".sdata"), aligned(4))) = "fceshdw1";
static char CreateTex_capOn[]
    __attribute__((section(".sdata"), aligned(4))) = "Cap on";
extern "C" _SCreateTexList ShadowedFace
    __attribute__((section(".sdata"))) = {
        CreateTex_faceShadow, CreateTex_capOn,
    };
static _SCreateTexList CreateTex_ShadowedFaceEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_faceBlackBald[]
    __attribute__((section(".sdata"), aligned(4))) = "fcebkbd1";
static char CreateTex_bald[]
    __attribute__((section(".sdata"), aligned(4))) = "Bald";
static char CreateTex_faceAfro1[]
    __attribute__((section(".sdata"), aligned(4))) = "fceafro1";
static char CreateTex_shortHair[]
    __attribute__((section(".sdata"), aligned(4))) = "Short hair";
static char CreateTex_faceCurly2[]
    __attribute__((section(".sdata"), aligned(4))) = "fcecrly2";
static char CreateTex_curlyHairLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Curly Hair";
static char CreateTex_faceAfroWig2[]
    __attribute__((section(".sdata"), aligned(4))) = "fceawig2";
static char CreateTex_bleachedAfro[]
    __attribute__((section(".sdata"), aligned(4))) = "Bleached Afro";
static char CreateTex_helmetTopTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "helmet_0";
static char CreateTex_helmetLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Helmet";
extern "C" _SCreateTexList HelmetTop
    __attribute__((section(".sdata"))) = {
        CreateTex_helmetTopTexture, CreateTex_helmetLabel,
    };
static _SCreateTexList CreateTex_HelmetTopEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_helmetFaceTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "fcehlmt1";
static char CreateTex_chinStrap[]
    __attribute__((section(".sdata"), aligned(4))) = "Chin Strap";
extern "C" _SCreateTexList HelmetFace
    __attribute__((section(".sdata"))) = {
        CreateTex_helmetFaceTexture, CreateTex_chinStrap,
    };
static _SCreateTexList CreateTex_HelmetFaceEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_capBirdTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "capbird0";
static char CreateTex_birdLogo[]
    __attribute__((section(".sdata"), aligned(4))) = "Bird Logo";
static char CreateTex_capSkullTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "capskul0";
static char CreateTex_skullLogo[]
    __attribute__((section(".sdata"), aligned(4))) = "Skull Logo";
static char CreateTex_beanieTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "beanie_0";
static char CreateTex_beanieLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Beanie";
extern "C" _SCreateTexList BeanieTop
    __attribute__((section(".sdata"))) = {
        CreateTex_beanieTexture, CreateTex_beanieLabel,
    };
static _SCreateTexList CreateTex_BeanieTopEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_afroALabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Afro A";
extern "C" _SCreateTexList AfroFace
    __attribute__((section(".sdata"))) = {
        CreateTex_faceAfro1, CreateTex_afroALabel,
    };
static _SCreateTexList CreateTex_AfroFaceEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_rastaTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "fcersta1";
static char CreateTex_rastaLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Rasta";
extern "C" _SCreateTexList RastaFace
    __attribute__((section(".sdata"))) = {
        CreateTex_rastaTexture, CreateTex_rastaLabel,
    };
static _SCreateTexList CreateTex_RastaFaceEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_skinTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "fcebald2";
static char CreateTex_baldieLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Baldie";
extern "C" _SCreateTexList SkinFace
    __attribute__((section(".sdata"))) = {
        CreateTex_skinTexture, CreateTex_baldieLabel,
    };
static _SCreateTexList CreateTex_SkinFaceEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_whiteFroLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "White Fro";
extern "C" _SCreateTexList WhiteAfroFace
    __attribute__((section(".sdata"))) = {
        CreateTex_faceAfroWig2, CreateTex_whiteFroLabel,
    };
static _SCreateTexList CreateTex_WhiteAfroFaceEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_gilliganTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "GlgnHat0";
static char CreateTex_gilliganLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Gilligan";
extern "C" _SCreateTexList GilliganTop
    __attribute__((section(".sdata"))) = {
        CreateTex_gilliganTexture, CreateTex_gilliganLabel,
    };
static _SCreateTexList CreateTex_GilliganTopEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_asianFaceTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "fceasn_3";
extern "C" _SCreateTexList AsianFace
    __attribute__((section(".sdata"))) = {
        CreateTex_asianFaceTexture, CreateTex_AsianGuyStorage,
    };
static _SCreateTexList CreateTex_AsianFaceEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_slickFaceTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "fcestrt2";
static char CreateTex_SlickStorage[] asm("DAT_800dad70")
    __attribute__((section(".sdata"), aligned(4))) = "Slick";
extern "C" _SCreateTexList SlickFace
    __attribute__((section(".sdata"))) = {
        CreateTex_slickFaceTexture, CreateTex_SlickStorage,
    };
static _SCreateTexList CreateTex_SlickFaceEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_openHandTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "handopn1";
static char CreateTex_openHandLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Open hand";
extern "C" _SCreateTexList GenericHand
    __attribute__((section(".sdata"))) = {
        CreateTex_openHandTexture, CreateTex_openHandLabel,
    };
static _SCreateTexList CreateTex_GenericHandEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_genericShoeTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "handopn2";
static char CreateTex_shoeALabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Shoe A";
extern "C" _SCreateTexList GenericShoe
    __attribute__((section(".sdata"))) = {
        CreateTex_genericShoeTexture, CreateTex_shoeALabel,
    };
static _SCreateTexList CreateTex_GenericShoeEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_tshirtTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "tshirt_0";
static char CreateTex_tshirtLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "T Shirt";
extern "C" _SCreateTexList DefaultChest
    __attribute__((section(".sdata"))) = {
        CreateTex_tshirtTexture, CreateTex_tshirtLabel,
    };
static _SCreateTexList CreateTex_DefaultChestEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_shortSleeveTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "shtslve0";
extern "C" _SCreateTexList DefaultBicep
    __attribute__((section(".sdata"))) = {
        CreateTex_shortSleeveTexture, CreateTex_tshirtLabel,
    };
static _SCreateTexList CreateTex_DefaultBicepEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_forearmTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "forearm1";
static char CreateTex_forearmLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Forearm";
extern "C" _SCreateTexList DefaultForearm
    __attribute__((section(".sdata"))) = {
        CreateTex_forearmTexture, CreateTex_forearmLabel,
    };
static _SCreateTexList CreateTex_DefaultForearmEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_buttonShirtTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "btnshrt0";
static char CreateTex_buttonShirtLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Button Shirt";
extern "C" _SCreateTexList LongSleeveChest
    __attribute__((section(".sdata"))) = {
        CreateTex_buttonShirtTexture, CreateTex_buttonShirtLabel,
    };
static _SCreateTexList CreateTex_LongSleeveChestEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_longSleeveTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "lngslve0";
static char CreateTex_longSleeve2Label[]
    __attribute__((section(".sdata"), aligned(4))) = "Long Sleeve 2";
extern "C" _SCreateTexList LongSleeveFullArm
    __attribute__((section(".sdata"))) = {
        CreateTex_longSleeveTexture, CreateTex_longSleeve2Label,
    };
static _SCreateTexList CreateTex_LongSleeveFullArmEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_noShirtTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "noshirt1";
static char CreateTex_shirtlessLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Shirtless";
extern "C" _SCreateTexList NoShirtChest
    __attribute__((section(".sdata"))) = {
        CreateTex_noShirtTexture, CreateTex_shirtlessLabel,
    };
static _SCreateTexList CreateTex_NoShirtChestEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_bareArmTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "barearm1";
static char CreateTex_sleevelessLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Sleeveless";
extern "C" _SCreateTexList NoShirtFullArm
    __attribute__((section(".sdata"))) = {
        CreateTex_bareArmTexture, CreateTex_sleevelessLabel,
    };
static _SCreateTexList CreateTex_NoShirtFullArmEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_hoodyTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "hoody__0";
static char CreateTex_hoodyLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Hoody";
extern "C" _SCreateTexList HoodyChest
    __attribute__((section(".sdata"))) = {
        CreateTex_hoodyTexture, CreateTex_hoodyLabel,
    };
static _SCreateTexList CreateTex_HoodyChestEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_longSleeveLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Long Sleeve";
extern "C" _SCreateTexList HoodyFullArm
    __attribute__((section(".sdata"))) = {
        CreateTex_longSleeveTexture, CreateTex_longSleeveLabel,
    };
static _SCreateTexList CreateTex_HoodyFullArmEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_tankTopTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "tanktop1";
static char CreateTex_johnMcLaneLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "John McLane";
extern "C" _SCreateTexList TanktopChest
    __attribute__((section(".sdata"))) = {
        CreateTex_tankTopTexture, CreateTex_johnMcLaneLabel,
    };
static _SCreateTexList CreateTex_TanktopChestEnd
    __attribute__((section(".sdata"))) = {0, 0};
extern "C" _SCreateTexList TanktopFullArm
    __attribute__((section(".sdata"))) = {
        CreateTex_bareArmTexture, CreateTex_sleevelessLabel,
    };
static _SCreateTexList CreateTex_TanktopFullArmEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_elbowPadsLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Elbow Pads";
extern "C" _SCreateTexList ElbowPadsChest
    __attribute__((section(".sdata"))) = {
        CreateTex_tshirtTexture, CreateTex_elbowPadsLabel,
    };
static _SCreateTexList CreateTex_ElbowPadsChestEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_shortSleeveLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Short Sleeve";
extern "C" _SCreateTexList ElbowPadsBicep
    __attribute__((section(".sdata"))) = {
        CreateTex_shortSleeveTexture, CreateTex_shortSleeveLabel,
    };
static _SCreateTexList CreateTex_ElbowPadsBicepEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_elbowPadTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "elbwpad1";
extern "C" _SCreateTexList ElbowPadsForearm
    __attribute__((section(".sdata"))) = {
        CreateTex_elbowPadTexture, CreateTex_elbowPadsLabel,
    };
static _SCreateTexList CreateTex_ElbowPadsForearmEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_shortsTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "shorts_0";
static char CreateTex_shortsLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Shorts";
static char CreateTex_cargoShortsTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "cgshort0";
static char CreateTex_cargoShortsLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Cargo Shorts";
static char CreateTex_bareShinTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "shinnkd1";
static char CreateTex_noSocksLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "No Socks";
static char CreateTex_ankleSocksTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "shinsok1";
static char CreateTex_ankleSocksLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Ankle Socks";
static char CreateTex_highSocksWhiteTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "shinhskw";
static char CreateTex_highSocksLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "High Socks";
static char CreateTex_highSocksTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "shinhsok";
static char CreateTex_mervynsLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Mervyns";
static char CreateTex_longPantsTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "lngpant0";
static char CreateTex_regularPantsLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Regular Pants";
static char CreateTex_cargoPantsTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "cgpants0";
static char CreateTex_cargoPantsLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Cargo Pants";
static char CreateTex_kneepadTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "kneepad0";
static char CreateTex_kneepadsLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Kneepads";
extern "C" _SCreateTexList KneepadThigh
    __attribute__((section(".sdata"))) = {
        CreateTex_kneepadTexture, CreateTex_kneepadsLabel,
    };
static _SCreateTexList CreateTex_KneepadThighEnd
    __attribute__((section(".sdata"))) = {0, 0};
static char CreateTex_noSocksLowerLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "No socks";
static char CreateTex_ankleSocksLowerLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Ankle socks";

/*
 * Geometry and front-end labels consumed by the global constructor.  Their
 * original anonymous DAT names are retained as assembly aliases so native
 * references from the constructor resolve to this owner.
 */
static char CreateTex_defaultStorage[] asm("DAT_800db0b0")
    __attribute__((section(".sdata"), aligned(4))) = "default";
static char CreateTex_DefaultStorage[] asm("DAT_800db0b8")
    __attribute__((section(".sdata"), aligned(4))) = "Default";
static char CreateTex_caphelmetStorage[] asm("DAT_800db0c0")
    __attribute__((section(".sdata"), aligned(4))) = "caphelmet";
static char CreateTex_helmetStorage[] asm("DAT_800db0cc")
    __attribute__((section(".sdata"), aligned(4))) = "helmet";
static char CreateTex_capbackwardStorage[] asm("DAT_800db0d4")
    __attribute__((section(".sdata"), aligned(4))) = "capbackward";
static char CreateTex_cap_backwardStorage[] asm("DAT_800db0e0")
    __attribute__((section(".sdata"), aligned(4))) = "cap back";
static char CreateTex_capforwardStorage[] asm("DAT_800db0ec")
    __attribute__((section(".sdata"), aligned(4))) = "capforward";
static char CreateTex_cap_forwardStorage[] asm("DAT_800db0f8")
    __attribute__((section(".sdata"), aligned(4))) = "cap forward";
static char CreateTex_capbeanieStorage[] asm("DAT_800db104")
    __attribute__((section(".sdata"), aligned(4))) = "capbeanie";
static char CreateTex_beanieStorage[] asm("DAT_800db110")
    __attribute__((section(".sdata"), aligned(4))) = "beanie";
static char CreateTex_curlyhairStorage[] asm("DAT_800db118")
    __attribute__((section(".sdata"), aligned(4))) = "curlyhair";
static char CreateTex_curlyStorage[] asm("DAT_800db124")
    __attribute__((section(".sdata"), aligned(4))) = "curly";
static char CreateTex_afroStorage[] asm("DAT_800db12c")
    __attribute__((section(".sdata"), aligned(4))) = "afro";
static char CreateTex_afro_guyStorage[] asm("DAT_800db134")
    __attribute__((section(".sdata"), aligned(4))) = "afro guy";
static char CreateTex_dreadsStorage[] asm("DAT_800db140")
    __attribute__((section(".sdata"), aligned(4))) = "dreads";
static char CreateTex_rasta_guyStorage[] asm("DAT_800db148")
    __attribute__((section(".sdata"), aligned(4))) = "rasta guy";
static char CreateTex_skinStorage[] asm("DAT_800db154")
    __attribute__((section(".sdata"), aligned(4))) = "skin";
static char CreateTex_skinheadStorage[] asm("DAT_800db15c")
    __attribute__((section(".sdata"), aligned(4))) = "skinhead";
static char CreateTex_afrowhiteStorage[] asm("DAT_800db168")
    __attribute__((section(".sdata"), aligned(4))) = "afrowhite";
static char CreateTex_whiteafroStorage[] asm("DAT_800db174")
    __attribute__((section(".sdata"), aligned(4))) = "whitefro";
static char CreateTex_gilliganStorage[] asm("DAT_800db180")
    __attribute__((section(".sdata"), aligned(4))) = "gilligan";
static char CreateTex_hey_skipperStorage[] asm("DAT_800db18c")
    __attribute__((section(".sdata"), aligned(4))) = "hey skipper";
static char CreateTex_baldblackStorage[] asm("DAT_800db198")
    __attribute__((section(".sdata"), aligned(4))) = "baldblack";
static char CreateTex_black_baldStorage[] asm("DAT_800db1a4")
    __attribute__((section(".sdata"), aligned(4))) = "black bald";
static char CreateTex_slickStorage[] asm("DAT_800db1b0")
    __attribute__((section(".sdata"), aligned(4))) = "slick";
static char CreateTex_asianStorage[] asm("DAT_800db1b8")
    __attribute__((section(".sdata"), aligned(4))) = "asian";
static char CreateTex_emptyStorage[] asm("DAT_800db1c0")
    __attribute__((section(".sdata"), aligned(4))) = "";
static char CreateTex_normalStorage[] asm("DAT_800db1c4")
    __attribute__((section(".sdata"), aligned(4))) = "normal";
static char CreateTex_longsleeveStorage[] asm("DAT_800db1cc")
    __attribute__((section(".sdata"), aligned(4))) = "longsleve";
static char CreateTex_jacketStorage[] asm("DAT_800db1d8")
    __attribute__((section(".sdata"), aligned(4))) = "jacket";
static char CreateTex_noshirtStorage[] asm("DAT_800db1e0")
    __attribute__((section(".sdata"), aligned(4))) = "noshirt";
static char CreateTex_nudeStorage[] asm("DAT_800db1e8")
    __attribute__((section(".sdata"), aligned(4))) = "nude";
static char CreateTex_hoodyStorage[] asm("DAT_800db1f0")
    __attribute__((section(".sdata"), aligned(4))) = "hoody";
static char CreateTex_hoodStorage[] asm("DAT_800db1f8")
    __attribute__((section(".sdata"), aligned(4))) = "hood";
static char CreateTex_tanktopStorage[] asm("DAT_800db200")
    __attribute__((section(".sdata"), aligned(4))) = "tanktop";
static char CreateTex_tankStorage[] asm("DAT_800db208")
    __attribute__((section(".sdata"), aligned(4))) = "tank";
static char CreateTex_elbowpadsStorage[] asm("DAT_800db210")
    __attribute__((section(".sdata"), aligned(4))) = "elbowpads";
static char CreateTex_padsStorage[] asm("DAT_800db21c")
    __attribute__((section(".sdata"), aligned(4))) = "pads";
static char CreateTex_standardStorage[] asm("DAT_800db224")
    __attribute__((section(".sdata"), aligned(4))) = "standard";
static char CreateTex_longpantsStorage[] asm("DAT_800db230")
    __attribute__((section(".sdata"), aligned(4))) = "longpants";
static char CreateTex_slacksStorage[] asm("DAT_800db23c")
    __attribute__((section(".sdata"), aligned(4))) = "slacks";
static char CreateTex_kneepadStorage[] asm("DAT_800db244")
    __attribute__((section(".sdata"), aligned(4))) = "kneepad";
static char CreateTex_baggypantsStorage[] asm("DAT_800db24c")
    __attribute__((section(".sdata"), aligned(4))) = "baggypants";
static char CreateTex_baggyStorage[8] asm("DAT_800db258")
    __attribute__((section(".sdata"), aligned(4))) = "baggy";

/*
 * The initialized texture-list catalogs precede the three mutable
 * CCreateTexSpec tables at 0x800C44A0-0x800C4588.
 */
extern "C" _SCreateTexList DefaultFace[3]
    __attribute__((section(".data"))) = {
        {CreateTex_faceSkin2, CreateTex_buzzCut},
        {CreateTex_faceAsian3, CreateTex_AsianGuyStorage},
        {0, 0},
    };
extern "C" _SCreateTexList BlackBald[3]
    __attribute__((section(".data"))) = {
        {CreateTex_faceBlackBald, CreateTex_bald},
        {CreateTex_faceAfro1, CreateTex_shortHair},
        {0, 0},
    };
extern "C" _SCreateTexList CurlyHair[3]
    __attribute__((section(".data"))) = {
        {CreateTex_faceCurly2, CreateTex_curlyHairLabel},
        {CreateTex_faceAfroWig2, CreateTex_bleachedAfro},
        {0, 0},
    };
extern "C" _SCreateTexList CapTop[3]
    __attribute__((section(".data"))) = {
        {CreateTex_capBirdTexture, CreateTex_birdLogo},
        {CreateTex_capSkullTexture, CreateTex_skullLogo},
        {0, 0},
    };
extern "C" _SCreateTexList DefaultThigh[3]
    __attribute__((section(".data"))) = {
        {CreateTex_shortsTexture, CreateTex_shortsLabel},
        {CreateTex_cargoShortsTexture, CreateTex_cargoShortsLabel},
        {0, 0},
    };
extern "C" _SCreateTexList DefaultShin[5]
    __attribute__((section(".data"))) = {
        {CreateTex_bareShinTexture, CreateTex_noSocksLabel},
        {CreateTex_ankleSocksTexture, CreateTex_ankleSocksLabel},
        {CreateTex_highSocksWhiteTexture, CreateTex_highSocksLabel},
        {CreateTex_highSocksTexture, CreateTex_mervynsLabel},
        {0, 0},
    };
extern "C" _SCreateTexList LongPantsFullLeg[3]
    __attribute__((section(".data"))) = {
        {CreateTex_longPantsTexture, CreateTex_regularPantsLabel},
        {CreateTex_cargoPantsTexture, CreateTex_cargoPantsLabel},
        {0, 0},
    };
extern "C" _SCreateTexList KneepadShin[3]
    __attribute__((section(".data"))) = {
        {CreateTex_bareShinTexture, CreateTex_noSocksLowerLabel},
        {CreateTex_ankleSocksTexture, CreateTex_ankleSocksLowerLabel},
        {0, 0},
    };
extern "C" _SCreateTexList BaggyPantsFullLeg[3]
    __attribute__((section(".data"))) = {
        {CreateTex_longPantsTexture, CreateTex_regularPantsLabel},
        {CreateTex_cargoPantsTexture, CreateTex_cargoPantsLabel},
        {0, 0},
    };

typedef void (*CreateTexSpecDestructor)(CCreateTexSpec *, int);
typedef void (*CreateTexSpecCClassDestructor)(CClass *, int);

struct CCreateTexSpecVtable {
    int reserved[2];
    CreateTexSpecDestructor destroy;
};

/** Dispatch the compiler-emitted complete-object destructor slot. */
static inline void DestroyCreateTexSpec(CCreateTexSpec *spec)
{
    CCreateTexSpecVtable *vtable =
        (CCreateTexSpecVtable *)spec->CClass.vtable;
    vtable->destroy(spec, 2);
}

extern "C" {
static void CreateTexSpecDestructorBody(CCreateTexSpec *, int)
    asm("_._14CCreateTexSpec");
static void CreateTexSpecCClassDestructorBody(CClass *, int)
    asm("__DOT__6CClass_8006D83C");

/**
 * Contiguous complete-object and base-object dispatch tables.
 *
 * The first table retains the established constructor anchor. The second is
 * addressed as its owner-local tail, so the split reconstruction's invented
 * `__vt_14CCreateTexSpec` export is no longer needed.
 */
static int CCreateTexSpec_vtbl
    __attribute__((section(".sdata"))) = 0;
static CreateTexSpecDestructor CreateTexSpecCompleteVtableTail[3]
    __attribute__((section(".sdata"))) = {
        0,
        CreateTexSpecDestructorBody,
        0,
    };
static int CreateTexSpecBaseVtableAnchor
    __attribute__((section(".sdata"))) = 0;
static CreateTexSpecCClassDestructor CreateTexSpecBaseVtableTail[2]
    __attribute__((section(".sdata"))) = {
        0,
        CreateTexSpecCClassDestructorBody,
    };
}

/*
 * The backing arrays above are known `.sdata` objects.  The target global
 * initializer nevertheless addresses these strings as ordinary externals,
 * using `lui`/`addiu` pairs instead of GP-relative loads.  These second C
 * names deliberately hide the storage class from cc1plus while resolving to
 * the same owner-local assembly labels.  Folding both views into one symbol
 * changes 338 instructions and 13 relocations in CreateTex_GlobalInit.
 */
#define CREATE_TEX_STRING_ALIAS(name, address) \
    extern "C" char name[] asm(address)

CREATE_TEX_STRING_ALIAS(CreateTex_default, "DAT_800db0b0");
CREATE_TEX_STRING_ALIAS(CreateTex_Default, "DAT_800db0b8");
CREATE_TEX_STRING_ALIAS(CreateTex_caphelmet, "DAT_800db0c0");
CREATE_TEX_STRING_ALIAS(CreateTex_helmet, "DAT_800db0cc");
CREATE_TEX_STRING_ALIAS(CreateTex_capbackward, "DAT_800db0d4");
CREATE_TEX_STRING_ALIAS(CreateTex_cap_backward, "DAT_800db0e0");
CREATE_TEX_STRING_ALIAS(CreateTex_capforward, "DAT_800db0ec");
CREATE_TEX_STRING_ALIAS(CreateTex_cap_forward, "DAT_800db0f8");
CREATE_TEX_STRING_ALIAS(CreateTex_capbeanie, "DAT_800db104");
CREATE_TEX_STRING_ALIAS(CreateTex_beanie, "DAT_800db110");
CREATE_TEX_STRING_ALIAS(CreateTex_curlyhair, "DAT_800db118");
CREATE_TEX_STRING_ALIAS(CreateTex_curly, "DAT_800db124");
CREATE_TEX_STRING_ALIAS(CreateTex_afro, "DAT_800db12c");
CREATE_TEX_STRING_ALIAS(CreateTex_afro_guy, "DAT_800db134");
CREATE_TEX_STRING_ALIAS(CreateTex_dreads, "DAT_800db140");
CREATE_TEX_STRING_ALIAS(CreateTex_rasta_guy, "DAT_800db148");
CREATE_TEX_STRING_ALIAS(CreateTex_skin, "DAT_800db154");
CREATE_TEX_STRING_ALIAS(CreateTex_skinhead, "DAT_800db15c");
CREATE_TEX_STRING_ALIAS(CreateTex_afrowhite, "DAT_800db168");
CREATE_TEX_STRING_ALIAS(CreateTex_whiteafro, "DAT_800db174");
CREATE_TEX_STRING_ALIAS(CreateTex_gilligan, "DAT_800db180");
CREATE_TEX_STRING_ALIAS(CreateTex_hey_skipper, "DAT_800db18c");
CREATE_TEX_STRING_ALIAS(CreateTex_baldblack, "DAT_800db198");
CREATE_TEX_STRING_ALIAS(CreateTex_black_bald, "DAT_800db1a4");
CREATE_TEX_STRING_ALIAS(CreateTex_slick, "DAT_800db1b0");
CREATE_TEX_STRING_ALIAS(CreateTex_Slick, "DAT_800dad70");
CREATE_TEX_STRING_ALIAS(CreateTex_asian, "DAT_800db1b8");
CREATE_TEX_STRING_ALIAS(CreateTex_AsianGuy, "DAT_800dab74");
CREATE_TEX_STRING_ALIAS(CreateTex_empty, "DAT_800db1c0");
CREATE_TEX_STRING_ALIAS(CreateTex_normal, "DAT_800db1c4");
CREATE_TEX_STRING_ALIAS(CreateTex_longsleeve, "DAT_800db1cc");
CREATE_TEX_STRING_ALIAS(CreateTex_jacket, "DAT_800db1d8");
CREATE_TEX_STRING_ALIAS(CreateTex_noshirt, "DAT_800db1e0");
CREATE_TEX_STRING_ALIAS(CreateTex_nude, "DAT_800db1e8");
CREATE_TEX_STRING_ALIAS(CreateTex_hoody, "DAT_800db1f0");
CREATE_TEX_STRING_ALIAS(CreateTex_hood, "DAT_800db1f8");
CREATE_TEX_STRING_ALIAS(CreateTex_tanktop, "DAT_800db200");
CREATE_TEX_STRING_ALIAS(CreateTex_tank, "DAT_800db208");
CREATE_TEX_STRING_ALIAS(CreateTex_elbowpads, "DAT_800db210");
CREATE_TEX_STRING_ALIAS(CreateTex_pads, "DAT_800db21c");
CREATE_TEX_STRING_ALIAS(CreateTex_standard, "DAT_800db224");
CREATE_TEX_STRING_ALIAS(CreateTex_longpants, "DAT_800db230");
CREATE_TEX_STRING_ALIAS(CreateTex_slacks, "DAT_800db23c");
CREATE_TEX_STRING_ALIAS(CreateTex_kneepad, "DAT_800db244");
CREATE_TEX_STRING_ALIAS(CreateTex_baggypants, "DAT_800db24c");
CREATE_TEX_STRING_ALIAS(CreateTex_baggy, "DAT_800db258");

#undef CREATE_TEX_STRING_ALIAS

// === Function implementations (address order) ===

#include <STRINGS.H>

/**
 * Initialize a create-a-skater texture specification.
 *
 * @param self Object being initialized.
 * @param name Geometry name shown to the model builder.
 * @param frontEndName Label shown by the front end.
 * @param hue Default palette hue.
 * @param list0 First texture-list choice.
 * @param list1 Second texture-list choice.
 * @param list2 Third texture-list choice.
 * @param list3 Fourth texture-list choice.
 * @return The initialized object.
 */
CCreateTexSpec *CCreateTexSpec_ctor(
    CCreateTexSpec *self, char *name, char *frontEndName, int hue,
    _SCreateTexList *list0, _SCreateTexList *list1,
    _SCreateTexList *list2, _SCreateTexList *list3)
{
    self->CClass.vtable = &CCreateTexSpec_vtbl;
    strcpy(self->mGeometryName, name);
    strcpy(self->mFrontEndName, frontEndName);
    self->mpTextureList[0] = list0;
    self->mpTextureList[1] = list1;
    self->mpTextureList[2] = list2;
    self->mpTextureList[3] = list3;
    self->mDefaultHue = hue;
    return self;
}


/**
 * Return the geometry name used by this specification.
 *
 * @return Mutable geometry-name storage owned by the specification.
 */
char *CCreateTexSpec::GetGeometryName() {
    return mGeometryName;
}


/**
 * Return the front-end label used by this specification.
 *
 * @return Mutable front-end-name storage owned by the specification.
 */
char *CCreateTexSpec::GetFrontEndName() {
    return mFrontEndName;
}


/**
 * Return one of the specification's four texture lists.
 *
 * @param i Texture-list slot in the range 0 through 3.
 * @return The selected texture list, or null when that slot is unused.
 */
_SCreateTexList *CCreateTexSpec::GetTextureList(int i)
{
    return mpTextureList[i];
}


/**
 * Return the default palette hue for this specification.
 *
 * @return Default hue value.
 */
int CCreateTexSpec::GetDefaultHue() {
    return mDefaultHue;
}


/**
 * Destroy the three specification tables in reverse definition order.
 *
 * Address: 0x8006D18C
 */
extern "C" void CreateTex_GlobalDtor(void) asm("_GLOBAL_.D.DefaultFace");

void CreateTex_GlobalDtor(void)
{
    {
        CCreateTexSpec *base = &LowerBodySpec[0];
        if (base != 0)
        {
            CCreateTexSpec *current = base + 5;
            if (current != base)
            {
                CCreateTexSpec *first = base;
                do
                {
                    --current;
                    DestroyCreateTexSpec(current);
                } while (current != first);
            }
        }
    }

    {
        CCreateTexSpec *base = &UpperBodySpec[0];
        if (base != 0)
        {
            CCreateTexSpec *current = base + 7;
            if (current != base)
            {
                CCreateTexSpec *first = base;
                do
                {
                    --current;
                    DestroyCreateTexSpec(current);
                } while (current != first);
            }
        }
    }

    {
        CCreateTexSpec *base = &HeadSpec[0];
        if (base != 0)
        {
            CCreateTexSpec *current = base + 15;
            if (current != base)
            {
                CCreateTexSpec *first = base;
                do
                {
                    --current;
                    DestroyCreateTexSpec(current);
                } while (current != first);
            }
        }
    }
}


/**
 * Construct the head, upper-body, and lower-body specification catalogs used
 * by the create-a-skater menu.
 *
 * Address: 0x8006D270
 */
extern "C" void CreateTex_GlobalInit(void) asm("_GLOBAL_.I.DefaultFace");

void CreateTex_GlobalInit(void)
{
    CCreateTexSpec *headSpecs = HeadSpec;

#define CREATE_TEX_SPEC(table, index, geometry, front_end, hue, list0, list1, list2, list3) \
    CCreateTexSpec_ctor(                                                       \
        &table[index], geometry, front_end, hue,                                \
        list0, list1, list2, list3)

    CREATE_TEX_SPEC(headSpecs, 0, CreateTex_default, CreateTex_Default, 0,
                    DefaultFace, 0, 0, 0);
    CREATE_TEX_SPEC(headSpecs, 1, CreateTex_caphelmet, CreateTex_helmet, 0,
                    &HelmetFace, &HelmetTop, 0, 0);
    CREATE_TEX_SPEC(headSpecs, 2, CreateTex_capbackward, CreateTex_cap_backward, 0,
                    &ShadowedFace, CapTop, 0, 0);
    CREATE_TEX_SPEC(headSpecs, 3, CreateTex_capforward, CreateTex_cap_forward, 0,
                    &ShadowedFace, CapTop, 0, 0);
    CREATE_TEX_SPEC(headSpecs, 4, CreateTex_capbeanie, CreateTex_beanie, 0,
                    &ShadowedFace, &BeanieTop, 0, 0);
    CREATE_TEX_SPEC(headSpecs, 5, CreateTex_curlyhair, CreateTex_curly, 0,
                    CurlyHair, 0, 0, 0);
    CREATE_TEX_SPEC(headSpecs, 6, CreateTex_afro, CreateTex_afro_guy, 0,
                    &AfroFace, 0, 0, 0);
    CREATE_TEX_SPEC(headSpecs, 7, CreateTex_dreads, CreateTex_rasta_guy, 0,
                    &RastaFace, 0, 0, 0);
    CREATE_TEX_SPEC(headSpecs, 8, CreateTex_skin, CreateTex_skinhead, 0,
                    &SkinFace, 0, 0, 0);
    CREATE_TEX_SPEC(headSpecs, 9, CreateTex_afrowhite, CreateTex_whiteafro, 0,
                    &WhiteAfroFace, 0, 0, 0);
    CREATE_TEX_SPEC(headSpecs, 10, CreateTex_gilligan, CreateTex_hey_skipper, 0,
                    &ShadowedFace, &GilliganTop, 0, 0);
    CREATE_TEX_SPEC(headSpecs, 11, CreateTex_baldblack, CreateTex_black_bald, 0,
                    BlackBald, 0, 0, 0);
    CREATE_TEX_SPEC(headSpecs, 12, CreateTex_slick, CreateTex_Slick, 0,
                    &SlickFace, 0, 0, 0);
    CREATE_TEX_SPEC(headSpecs, 13, CreateTex_asian, CreateTex_AsianGuy, 0,
                    &AsianFace, 0, 0, 0);
    CREATE_TEX_SPEC(headSpecs, 14, CreateTex_empty, CreateTex_empty, 0,
                    0, 0, 0, 0);

    CREATE_TEX_SPEC(UpperBodySpec, 0, CreateTex_default, CreateTex_normal, 0,
                    &DefaultChest, &DefaultBicep, &DefaultForearm, &GenericHand);
    CREATE_TEX_SPEC(UpperBodySpec, 1, CreateTex_longsleeve, CreateTex_jacket, 0,
                    &LongSleeveChest, &LongSleeveFullArm, 0, &GenericHand);
    CREATE_TEX_SPEC(UpperBodySpec, 2, CreateTex_noshirt, CreateTex_nude, 0,
                    &NoShirtChest, &NoShirtFullArm, 0, &GenericHand);
    CREATE_TEX_SPEC(UpperBodySpec, 3, CreateTex_hoody, CreateTex_hood, 0,
                    &HoodyChest, &HoodyFullArm, 0, &GenericHand);
    CREATE_TEX_SPEC(UpperBodySpec, 4, CreateTex_tanktop, CreateTex_tank, 0,
                    &TanktopChest, &TanktopFullArm, 0, &GenericHand);
    CREATE_TEX_SPEC(UpperBodySpec, 5, CreateTex_elbowpads, CreateTex_pads, 120,
                    &ElbowPadsChest, &ElbowPadsBicep, &ElbowPadsForearm,
                    &GenericHand);
    CREATE_TEX_SPEC(UpperBodySpec, 6, CreateTex_empty, CreateTex_empty, 0,
                    0, 0, 0, 0);

    CREATE_TEX_SPEC(LowerBodySpec, 0, CreateTex_default, CreateTex_standard, 248,
                    DefaultThigh, DefaultShin, &GenericShoe, 0);
    CREATE_TEX_SPEC(LowerBodySpec, 1, CreateTex_longpants, CreateTex_slacks, 210,
                    LongPantsFullLeg, 0, &GenericShoe, 0);
    CREATE_TEX_SPEC(LowerBodySpec, 2, CreateTex_kneepad, CreateTex_pads, 140,
                    &KneepadThigh, KneepadShin, &GenericShoe, 0);
    CREATE_TEX_SPEC(LowerBodySpec, 3, CreateTex_baggypants, CreateTex_baggy, 160,
                    BaggyPantsFullLeg, 0, &GenericShoe, 0);
    CREATE_TEX_SPEC(LowerBodySpec, 4, CreateTex_empty, CreateTex_empty, 0,
                    0, 0, 0, 0);

#undef CREATE_TEX_SPEC
}

/**
 * `.ctors` table slot for this owner's hand-written global constructor.
 *
 * GCC emits this entry itself for a compiler-generated constructor, but this owner's
 * hook is written by hand, so the entry has to be declared explicitly. The
 * recovered application object order reproduces the target's table sequence.
 */
static void (*CreateTex_GlobalCtorSlot)(void)
    __attribute__((section(".ctors"))) = CreateTex_GlobalInit;

/**
 * `.dtors` table slot for this owner's hand-written global destructor.
 *
 * GCC emits this entry itself for a compiler-generated destructor, but this owner's
 * hook is written by hand, so the entry has to be declared explicitly. The
 * recovered application object order reproduces the target's table sequence.
 */
static void (*CreateTex_GlobalDtorSlot)(void)
    __attribute__((section(".dtors"))) = CreateTex_GlobalDtor;

/**
 * Destroy a create-a-skater texture specification and optionally free it.
 *
 * `MAIN.SYM` attributes this inline `CREATE_TEX.H` body to the enclosing
 * `create_tex.obj` FILE block, where it remains owner-local.
 *
 * Address: 0x8006D80C
 * Mangled: _._14CCreateTexSpec
 */
extern "C" {
static void CreateTexSpecDestructorBody(CCreateTexSpec *self, int flags)
{
    self->CClass.vtable = &CreateTexSpecBaseVtableAnchor;
    if (flags & 1)
        CClass::operator delete(self);
}

/**
 * Destroy the private `CClass` base instantiated by `create_tex.obj`.
 *
 * Address: 0x8006D83C
 * Mangled: _._6CClass
 */
static void CreateTexSpecCClassDestructorBody(CClass *self, int flags)
{
    self->vtable = &CreateTexSpecBaseVtableAnchor;
    if (flags & 1)
        CClass::operator delete(self);
}
}
