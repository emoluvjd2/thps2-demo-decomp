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
// Natural placement construction advances GCC 2.x's TU-wide local-label allocator.
// Keep this target-named owner-local entry until that compiler behavior is reproduced.
namespace CreateTexSpecConstructorAbi
{
extern "C" CCreateTexSpec *__14CCreateTexSpecPcT1iP15_SCreateTexListN34(
    CCreateTexSpec *, char *, char *, int, _SCreateTexList *,
    _SCreateTexList *, _SCreateTexList *, _SCreateTexList *);
}
#define CCreateTexSpec_ctor \
    CreateTexSpecConstructorAbi::__14CCreateTexSpecPcT1iP15_SCreateTexListN34

/** Process-lifetime create-a-skater specification tables owned by this TU. */
#define CREATETEXSPECS_OWNER_DECLARATION
#include "CREATETEXSPECS.h"

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
namespace CreateTexAddressViewOwners
{
extern "C"
{
static char DAT_800dab74[]
    __attribute__((section(".sdata"), aligned(4))) = "Asian Guy";
}
}
static char CreateTex_faceShadow[]
    __attribute__((section(".sdata"), aligned(4))) = "fceshdw1";
static char CreateTex_capOn[]
    __attribute__((section(".sdata"), aligned(4))) = "Cap on";
_SCreateTexList ShadowedFace
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
_SCreateTexList HelmetTop
    __attribute__((section(".sdata"))) = {
        CreateTex_helmetTopTexture, CreateTex_helmetLabel,
    };
static _SCreateTexList CreateTex_HelmetTopEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_helmetFaceTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "fcehlmt1";
static char CreateTex_chinStrap[]
    __attribute__((section(".sdata"), aligned(4))) = "Chin Strap";
_SCreateTexList HelmetFace
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
_SCreateTexList BeanieTop
    __attribute__((section(".sdata"))) = {
        CreateTex_beanieTexture, CreateTex_beanieLabel,
    };
static _SCreateTexList CreateTex_BeanieTopEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_afroALabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Afro A";
_SCreateTexList AfroFace
    __attribute__((section(".sdata"))) = {
        CreateTex_faceAfro1, CreateTex_afroALabel,
    };
static _SCreateTexList CreateTex_AfroFaceEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_rastaTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "fcersta1";
static char CreateTex_rastaLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Rasta";
_SCreateTexList RastaFace
    __attribute__((section(".sdata"))) = {
        CreateTex_rastaTexture, CreateTex_rastaLabel,
    };
static _SCreateTexList CreateTex_RastaFaceEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_skinTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "fcebald2";
static char CreateTex_baldieLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Baldie";
_SCreateTexList SkinFace
    __attribute__((section(".sdata"))) = {
        CreateTex_skinTexture, CreateTex_baldieLabel,
    };
static _SCreateTexList CreateTex_SkinFaceEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_whiteFroLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "White Fro";
_SCreateTexList WhiteAfroFace
    __attribute__((section(".sdata"))) = {
        CreateTex_faceAfroWig2, CreateTex_whiteFroLabel,
    };
static _SCreateTexList CreateTex_WhiteAfroFaceEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_gilliganTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "GlgnHat0";
static char CreateTex_gilliganLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Gilligan";
_SCreateTexList GilliganTop
    __attribute__((section(".sdata"))) = {
        CreateTex_gilliganTexture, CreateTex_gilliganLabel,
    };
static _SCreateTexList CreateTex_GilliganTopEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_asianFaceTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "fceasn_3";
_SCreateTexList AsianFace
    __attribute__((section(".sdata"))) = {
        CreateTex_asianFaceTexture, CreateTexAddressViewOwners::DAT_800dab74,
    };
static _SCreateTexList CreateTex_AsianFaceEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_slickFaceTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "fcestrt2";
namespace CreateTexAddressViewOwners
{
extern "C"
{
static char DAT_800dad70[]
    __attribute__((section(".sdata"), aligned(4))) = "Slick";
}
}
_SCreateTexList SlickFace
    __attribute__((section(".sdata"))) = {
        CreateTex_slickFaceTexture, CreateTexAddressViewOwners::DAT_800dad70,
    };
static _SCreateTexList CreateTex_SlickFaceEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_openHandTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "handopn1";
static char CreateTex_openHandLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Open hand";
_SCreateTexList GenericHand
    __attribute__((section(".sdata"))) = {
        CreateTex_openHandTexture, CreateTex_openHandLabel,
    };
static _SCreateTexList CreateTex_GenericHandEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_genericShoeTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "handopn2";
static char CreateTex_shoeALabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Shoe A";
_SCreateTexList GenericShoe
    __attribute__((section(".sdata"))) = {
        CreateTex_genericShoeTexture, CreateTex_shoeALabel,
    };
static _SCreateTexList CreateTex_GenericShoeEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_tshirtTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "tshirt_0";
static char CreateTex_tshirtLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "T Shirt";
_SCreateTexList DefaultChest
    __attribute__((section(".sdata"))) = {
        CreateTex_tshirtTexture, CreateTex_tshirtLabel,
    };
static _SCreateTexList CreateTex_DefaultChestEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_shortSleeveTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "shtslve0";
_SCreateTexList DefaultBicep
    __attribute__((section(".sdata"))) = {
        CreateTex_shortSleeveTexture, CreateTex_tshirtLabel,
    };
static _SCreateTexList CreateTex_DefaultBicepEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_forearmTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "forearm1";
static char CreateTex_forearmLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Forearm";
_SCreateTexList DefaultForearm
    __attribute__((section(".sdata"))) = {
        CreateTex_forearmTexture, CreateTex_forearmLabel,
    };
static _SCreateTexList CreateTex_DefaultForearmEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_buttonShirtTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "btnshrt0";
static char CreateTex_buttonShirtLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Button Shirt";
_SCreateTexList LongSleeveChest
    __attribute__((section(".sdata"))) = {
        CreateTex_buttonShirtTexture, CreateTex_buttonShirtLabel,
    };
static _SCreateTexList CreateTex_LongSleeveChestEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_longSleeveTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "lngslve0";
static char CreateTex_longSleeve2Label[]
    __attribute__((section(".sdata"), aligned(4))) = "Long Sleeve 2";
_SCreateTexList LongSleeveFullArm
    __attribute__((section(".sdata"))) = {
        CreateTex_longSleeveTexture, CreateTex_longSleeve2Label,
    };
static _SCreateTexList CreateTex_LongSleeveFullArmEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_noShirtTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "noshirt1";
static char CreateTex_shirtlessLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Shirtless";
_SCreateTexList NoShirtChest
    __attribute__((section(".sdata"))) = {
        CreateTex_noShirtTexture, CreateTex_shirtlessLabel,
    };
static _SCreateTexList CreateTex_NoShirtChestEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_bareArmTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "barearm1";
static char CreateTex_sleevelessLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Sleeveless";
_SCreateTexList NoShirtFullArm
    __attribute__((section(".sdata"))) = {
        CreateTex_bareArmTexture, CreateTex_sleevelessLabel,
    };
static _SCreateTexList CreateTex_NoShirtFullArmEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_hoodyTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "hoody__0";
static char CreateTex_hoodyLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Hoody";
_SCreateTexList HoodyChest
    __attribute__((section(".sdata"))) = {
        CreateTex_hoodyTexture, CreateTex_hoodyLabel,
    };
static _SCreateTexList CreateTex_HoodyChestEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_longSleeveLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Long Sleeve";
_SCreateTexList HoodyFullArm
    __attribute__((section(".sdata"))) = {
        CreateTex_longSleeveTexture, CreateTex_longSleeveLabel,
    };
static _SCreateTexList CreateTex_HoodyFullArmEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_tankTopTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "tanktop1";
static char CreateTex_johnMcLaneLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "John McLane";
_SCreateTexList TanktopChest
    __attribute__((section(".sdata"))) = {
        CreateTex_tankTopTexture, CreateTex_johnMcLaneLabel,
    };
static _SCreateTexList CreateTex_TanktopChestEnd
    __attribute__((section(".sdata"))) = {0, 0};
_SCreateTexList TanktopFullArm
    __attribute__((section(".sdata"))) = {
        CreateTex_bareArmTexture, CreateTex_sleevelessLabel,
    };
static _SCreateTexList CreateTex_TanktopFullArmEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_elbowPadsLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Elbow Pads";
_SCreateTexList ElbowPadsChest
    __attribute__((section(".sdata"))) = {
        CreateTex_tshirtTexture, CreateTex_elbowPadsLabel,
    };
static _SCreateTexList CreateTex_ElbowPadsChestEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_shortSleeveLabel[]
    __attribute__((section(".sdata"), aligned(4))) = "Short Sleeve";
_SCreateTexList ElbowPadsBicep
    __attribute__((section(".sdata"))) = {
        CreateTex_shortSleeveTexture, CreateTex_shortSleeveLabel,
    };
static _SCreateTexList CreateTex_ElbowPadsBicepEnd
    __attribute__((section(".sdata"))) = {0, 0};

static char CreateTex_elbowPadTexture[]
    __attribute__((section(".sdata"), aligned(4))) = "elbwpad1";
_SCreateTexList ElbowPadsForearm
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
_SCreateTexList KneepadThigh
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
 * Geometry and front-end labels consumed by the global constructor. Their
 * target identifiers are retained under scoped C linkage so the definitions
 * remain lexically distinct from the address-only views below.
 */
namespace CreateTexAddressViewOwners
{
extern "C"
{
static char DAT_800db0b0[]
    __attribute__((section(".sdata"), aligned(4))) = "default";
static char DAT_800db0b8[]
    __attribute__((section(".sdata"), aligned(4))) = "Default";
static char DAT_800db0c0[]
    __attribute__((section(".sdata"), aligned(4))) = "caphelmet";
static char DAT_800db0cc[]
    __attribute__((section(".sdata"), aligned(4))) = "helmet";
static char DAT_800db0d4[]
    __attribute__((section(".sdata"), aligned(4))) = "capbackward";
static char DAT_800db0e0[]
    __attribute__((section(".sdata"), aligned(4))) = "cap back";
static char DAT_800db0ec[]
    __attribute__((section(".sdata"), aligned(4))) = "capforward";
static char DAT_800db0f8[]
    __attribute__((section(".sdata"), aligned(4))) = "cap forward";
static char DAT_800db104[]
    __attribute__((section(".sdata"), aligned(4))) = "capbeanie";
static char DAT_800db110[]
    __attribute__((section(".sdata"), aligned(4))) = "beanie";
static char DAT_800db118[]
    __attribute__((section(".sdata"), aligned(4))) = "curlyhair";
static char DAT_800db124[]
    __attribute__((section(".sdata"), aligned(4))) = "curly";
static char DAT_800db12c[]
    __attribute__((section(".sdata"), aligned(4))) = "afro";
static char DAT_800db134[]
    __attribute__((section(".sdata"), aligned(4))) = "afro guy";
static char DAT_800db140[]
    __attribute__((section(".sdata"), aligned(4))) = "dreads";
static char DAT_800db148[]
    __attribute__((section(".sdata"), aligned(4))) = "rasta guy";
static char DAT_800db154[]
    __attribute__((section(".sdata"), aligned(4))) = "skin";
static char DAT_800db15c[]
    __attribute__((section(".sdata"), aligned(4))) = "skinhead";
static char DAT_800db168[]
    __attribute__((section(".sdata"), aligned(4))) = "afrowhite";
static char DAT_800db174[]
    __attribute__((section(".sdata"), aligned(4))) = "whitefro";
static char DAT_800db180[]
    __attribute__((section(".sdata"), aligned(4))) = "gilligan";
static char DAT_800db18c[]
    __attribute__((section(".sdata"), aligned(4))) = "hey skipper";
static char DAT_800db198[]
    __attribute__((section(".sdata"), aligned(4))) = "baldblack";
static char DAT_800db1a4[]
    __attribute__((section(".sdata"), aligned(4))) = "black bald";
static char DAT_800db1b0[]
    __attribute__((section(".sdata"), aligned(4))) = "slick";
static char DAT_800db1b8[]
    __attribute__((section(".sdata"), aligned(4))) = "asian";
static char DAT_800db1c0[]
    __attribute__((section(".sdata"), aligned(4))) = "";
static char DAT_800db1c4[]
    __attribute__((section(".sdata"), aligned(4))) = "normal";
static char DAT_800db1cc[]
    __attribute__((section(".sdata"), aligned(4))) = "longsleve";
static char DAT_800db1d8[]
    __attribute__((section(".sdata"), aligned(4))) = "jacket";
static char DAT_800db1e0[]
    __attribute__((section(".sdata"), aligned(4))) = "noshirt";
static char DAT_800db1e8[]
    __attribute__((section(".sdata"), aligned(4))) = "nude";
static char DAT_800db1f0[]
    __attribute__((section(".sdata"), aligned(4))) = "hoody";
static char DAT_800db1f8[]
    __attribute__((section(".sdata"), aligned(4))) = "hood";
static char DAT_800db200[]
    __attribute__((section(".sdata"), aligned(4))) = "tanktop";
static char DAT_800db208[]
    __attribute__((section(".sdata"), aligned(4))) = "tank";
static char DAT_800db210[]
    __attribute__((section(".sdata"), aligned(4))) = "elbowpads";
static char DAT_800db21c[]
    __attribute__((section(".sdata"), aligned(4))) = "pads";
static char DAT_800db224[]
    __attribute__((section(".sdata"), aligned(4))) = "standard";
static char DAT_800db230[]
    __attribute__((section(".sdata"), aligned(4))) = "longpants";
static char DAT_800db23c[]
    __attribute__((section(".sdata"), aligned(4))) = "slacks";
static char DAT_800db244[]
    __attribute__((section(".sdata"), aligned(4))) = "kneepad";
static char DAT_800db24c[]
    __attribute__((section(".sdata"), aligned(4))) = "baggypants";
static char DAT_800db258[8]
    __attribute__((section(".sdata"), aligned(4))) = "baggy";
}
}

/*
 * The initialized texture-list catalogs precede the three mutable
 * CCreateTexSpec tables at 0x800C44A0-0x800C4588.
 */
_SCreateTexList DefaultFace[3]
    __attribute__((section(".data"))) = {
        {CreateTex_faceSkin2, CreateTex_buzzCut},
        {CreateTex_faceAsian3, CreateTexAddressViewOwners::DAT_800dab74},
        {0, 0},
    };
_SCreateTexList BlackBald[3]
    __attribute__((section(".data"))) = {
        {CreateTex_faceBlackBald, CreateTex_bald},
        {CreateTex_faceAfro1, CreateTex_shortHair},
        {0, 0},
    };
_SCreateTexList CurlyHair[3]
    __attribute__((section(".data"))) = {
        {CreateTex_faceCurly2, CreateTex_curlyHairLabel},
        {CreateTex_faceAfroWig2, CreateTex_bleachedAfro},
        {0, 0},
    };
_SCreateTexList CapTop[3]
    __attribute__((section(".data"))) = {
        {CreateTex_capBirdTexture, CreateTex_birdLogo},
        {CreateTex_capSkullTexture, CreateTex_skullLogo},
        {0, 0},
    };
_SCreateTexList DefaultThigh[3]
    __attribute__((section(".data"))) = {
        {CreateTex_shortsTexture, CreateTex_shortsLabel},
        {CreateTex_cargoShortsTexture, CreateTex_cargoShortsLabel},
        {0, 0},
    };
_SCreateTexList DefaultShin[5]
    __attribute__((section(".data"))) = {
        {CreateTex_bareShinTexture, CreateTex_noSocksLabel},
        {CreateTex_ankleSocksTexture, CreateTex_ankleSocksLabel},
        {CreateTex_highSocksWhiteTexture, CreateTex_highSocksLabel},
        {CreateTex_highSocksTexture, CreateTex_mervynsLabel},
        {0, 0},
    };
_SCreateTexList LongPantsFullLeg[3]
    __attribute__((section(".data"))) = {
        {CreateTex_longPantsTexture, CreateTex_regularPantsLabel},
        {CreateTex_cargoPantsTexture, CreateTex_cargoPantsLabel},
        {0, 0},
    };
_SCreateTexList KneepadShin[3]
    __attribute__((section(".data"))) = {
        {CreateTex_bareShinTexture, CreateTex_noSocksLowerLabel},
        {CreateTex_ankleSocksTexture, CreateTex_ankleSocksLowerLabel},
        {0, 0},
    };
_SCreateTexList BaggyPantsFullLeg[3]
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

static void CreateTexSpecDestructorBody(CCreateTexSpec *, int)
    asm("_._14CCreateTexSpec");
extern "C" {
static void __DOT__6CClass_8006D83C(CClass *, int);
}

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
        __DOT__6CClass_8006D83C,
    };

/*
 * The backing arrays above are known `.sdata` objects.  The target global
 * initializer nevertheless addresses these strings as ordinary externals,
 * using `lui`/`addiu` pairs instead of GP-relative loads.  These second
 * declarations deliberately hide the storage class from cc1plus while their
 * identifiers resolve to the same owner-local labels. Referencing the
 * definitions directly changes 338 instructions and 13 relocations in
 * CreateTex_GlobalInit.
 */
/** Absolute address views of CREATE_TEX's GP-sized specification strings. */
extern char DAT_800db0b0[];
extern char DAT_800db0b8[];
extern char DAT_800db0c0[];
extern char DAT_800db0cc[];
extern char DAT_800db0d4[];
extern char DAT_800db0e0[];
extern char DAT_800db0ec[];
extern char DAT_800db0f8[];
extern char DAT_800db104[];
extern char DAT_800db110[];
extern char DAT_800db118[];
extern char DAT_800db124[];
extern char DAT_800db12c[];
extern char DAT_800db134[];
extern char DAT_800db140[];
extern char DAT_800db148[];
extern char DAT_800db154[];
extern char DAT_800db15c[];
extern char DAT_800db168[];
extern char DAT_800db174[];
extern char DAT_800db180[];
extern char DAT_800db18c[];
extern char DAT_800db198[];
extern char DAT_800db1a4[];
extern char DAT_800db1b0[];
extern char DAT_800dad70[];
extern char DAT_800db1b8[];
extern char DAT_800dab74[];
extern char DAT_800db1c0[];
extern char DAT_800db1c4[];
extern char DAT_800db1cc[];
extern char DAT_800db1d8[];
extern char DAT_800db1e0[];
extern char DAT_800db1e8[];
extern char DAT_800db1f0[];
extern char DAT_800db1f8[];
extern char DAT_800db200[];
extern char DAT_800db208[];
extern char DAT_800db210[];
extern char DAT_800db21c[];
extern char DAT_800db224[];
extern char DAT_800db230[];
extern char DAT_800db23c[];
extern char DAT_800db244[];
extern char DAT_800db24c[];
extern char DAT_800db258[];

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
void CreateTex_GlobalDtor(void) asm("_GLOBAL_.D.DefaultFace");

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
void CreateTex_GlobalInit(void) asm("_GLOBAL_.I.DefaultFace");

void CreateTex_GlobalInit(void)
{
    CCreateTexSpec *headSpecs = HeadSpec;

#define CREATE_TEX_SPEC(table, index, geometry, front_end, hue, list0, list1, list2, list3) \
    CCreateTexSpec_ctor(                                                       \
        &table[index], geometry, front_end, hue,                               \
        list0, list1, list2, list3)

    CREATE_TEX_SPEC(
        headSpecs, 0, DAT_800db0b0,
        DAT_800db0b8, 0, DefaultFace, 0, 0, 0);
    CREATE_TEX_SPEC(
        headSpecs, 1, DAT_800db0c0,
        DAT_800db0cc, 0,
        &HelmetFace, &HelmetTop, 0, 0);
    CREATE_TEX_SPEC(
        headSpecs, 2, DAT_800db0d4,
        DAT_800db0e0, 0,
        &ShadowedFace, CapTop, 0, 0);
    CREATE_TEX_SPEC(
        headSpecs, 3, DAT_800db0ec,
        DAT_800db0f8, 0,
        &ShadowedFace, CapTop, 0, 0);
    CREATE_TEX_SPEC(
        headSpecs, 4, DAT_800db104,
        DAT_800db110, 0,
        &ShadowedFace, &BeanieTop, 0, 0);
    CREATE_TEX_SPEC(
        headSpecs, 5, DAT_800db118,
        DAT_800db124, 0, CurlyHair, 0, 0, 0);
    CREATE_TEX_SPEC(
        headSpecs, 6, DAT_800db12c,
        DAT_800db134, 0, &AfroFace, 0, 0, 0);
    CREATE_TEX_SPEC(
        headSpecs, 7, DAT_800db140,
        DAT_800db148, 0, &RastaFace, 0, 0, 0);
    CREATE_TEX_SPEC(
        headSpecs, 8, DAT_800db154,
        DAT_800db15c, 0, &SkinFace, 0, 0, 0);
    CREATE_TEX_SPEC(
        headSpecs, 9, DAT_800db168,
        DAT_800db174, 0,
        &WhiteAfroFace, 0, 0, 0);
    CREATE_TEX_SPEC(
        headSpecs, 10, DAT_800db180,
        DAT_800db18c, 0,
        &ShadowedFace, &GilliganTop, 0, 0);
    CREATE_TEX_SPEC(
        headSpecs, 11, DAT_800db198,
        DAT_800db1a4, 0, BlackBald, 0, 0, 0);
    CREATE_TEX_SPEC(
        headSpecs, 12, DAT_800db1b0,
        DAT_800dad70, 0, &SlickFace, 0, 0, 0);
    CREATE_TEX_SPEC(
        headSpecs, 13, DAT_800db1b8,
        DAT_800dab74, 0, &AsianFace, 0, 0, 0);
    CREATE_TEX_SPEC(
        headSpecs, 14, DAT_800db1c0,
        DAT_800db1c0, 0, 0, 0, 0, 0);

    CREATE_TEX_SPEC(
        UpperBodySpec, 0, DAT_800db0b0,
        DAT_800db1c4, 0,
        &DefaultChest, &DefaultBicep, &DefaultForearm, &GenericHand);
    CREATE_TEX_SPEC(
        UpperBodySpec, 1, DAT_800db1cc,
        DAT_800db1d8, 0,
        &LongSleeveChest, &LongSleeveFullArm, 0, &GenericHand);
    CREATE_TEX_SPEC(
        UpperBodySpec, 2, DAT_800db1e0,
        DAT_800db1e8, 0,
        &NoShirtChest, &NoShirtFullArm, 0, &GenericHand);
    CREATE_TEX_SPEC(
        UpperBodySpec, 3, DAT_800db1f0,
        DAT_800db1f8, 0,
        &HoodyChest, &HoodyFullArm, 0, &GenericHand);
    CREATE_TEX_SPEC(
        UpperBodySpec, 4, DAT_800db200,
        DAT_800db208, 0,
        &TanktopChest, &TanktopFullArm, 0, &GenericHand);
    CREATE_TEX_SPEC(
        UpperBodySpec, 5, DAT_800db210,
        DAT_800db21c, 120,
        &ElbowPadsChest, &ElbowPadsBicep, &ElbowPadsForearm, &GenericHand);
    CREATE_TEX_SPEC(
        UpperBodySpec, 6, DAT_800db1c0,
        DAT_800db1c0, 0, 0, 0, 0, 0);

    CREATE_TEX_SPEC(
        LowerBodySpec, 0, DAT_800db0b0,
        DAT_800db224, 248,
        DefaultThigh, DefaultShin, &GenericShoe, 0);
    CREATE_TEX_SPEC(
        LowerBodySpec, 1, DAT_800db230,
        DAT_800db23c, 210,
        LongPantsFullLeg, 0, &GenericShoe, 0);
    CREATE_TEX_SPEC(
        LowerBodySpec, 2, DAT_800db244,
        DAT_800db21c, 140,
        &KneepadThigh, KneepadShin, &GenericShoe, 0);
    CREATE_TEX_SPEC(
        LowerBodySpec, 3, DAT_800db24c,
        DAT_800db258, 160,
        BaggyPantsFullLeg, 0, &GenericShoe, 0);
    CREATE_TEX_SPEC(
        LowerBodySpec, 4, DAT_800db1c0,
        DAT_800db1c0, 0, 0, 0, 0, 0);

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
static void __DOT__6CClass_8006D83C(CClass *self, int flags)
{
    self->vtable = &CreateTexSpecBaseVtableAnchor;
    if (flags & 1)
        CClass::operator delete(self);
}
