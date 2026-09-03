// LEVELGEN.cpp
// Original: LEVELGEN.CPP
// Address range: 0x80056020 - 0x80059FF4
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#include "FACEREMOVAL.h"
#include "M3DZONE.h"
#include "M3DINIT.h"
#include "PACK.h"
#include "RAIL.h"
#define LEVELGEN_OWNER_DECLARATIONS
#include "LEVELGEN.h"
#undef LEVELGEN_OWNER_DECLARATIONS
#include "RAILINFO.h"
// Owner-local level-editor model and generator layouts used by LEVELGEN.cpp.
//
// Field order and sizes are recovered from original/MAIN_STRUCTS.h. Keeping
// these records local avoids importing the monolithic aggregate header while
// preserving the original PSX ABI exactly.
#include "TRIG.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include "SMODEL.h"

struct ModelFace;
struct ModelFace2;

/** Return caller-provided storage for an owner constructor. */
inline void *operator new(unsigned int, void *address)
{
    return address;
}

/** Word/halfword view used by the serialized shell-model bounds. */
struct LevelGenModelBounds
{
    short half[6];
};

/** Header node emitted before the generated rail trigger nodes. */
struct LevelGen_FakeTriggerNode
{
    int x;
    int y;
    int z;
    unsigned short angleX;
    unsigned short angleY;
    unsigned short angleZ;
    char name[4];
    short nextNode;
};

/** Texture header slice through the usage counter serialized by CreatePSX. */
struct LevelGenTextureUsageView
{
    unsigned char u0;
    unsigned char v0;
    unsigned short clut;
    unsigned char u1;
    unsigned char v1;
    unsigned short tpage;
    unsigned char u2;
    unsigned char v2;
    unsigned char u3;
    unsigned char v3;
    unsigned int TexWin;
    unsigned short Usage;
};

/** Fixed 28-byte header emitted before the 20 by 20 zone-cell payload. */
struct LevelGenZonePacketHeader
{
    int Type;
    int PayloadBytes;
    int MinX;
    int MinZ;
    int MaxX;
    int MaxZ;
    short Width;
    short Height;
};

/** Fixed trigger-file prefix followed by its node-offset pointer table. */
struct LevelGenTriggerFileHeader
{
    unsigned int Magic;
    unsigned int Version;
    unsigned short NumNodes;
    unsigned short Reserved;
};

/** One source model available to the level editor (68 bytes). */
struct EditorInModel
{
    /** Recovers source-model dimensions, faces, and rail metadata. */
    void SetData(SModel *pModel, int model_index);
    /** Releases this source model's faces and generated rail metadata. */
    void KillModelInfo();

    struct CClass CClass;
    struct ModelFace *Faces;
    struct SModel *mModel;
    unsigned char mIndex;
    unsigned char mIndexPadding[3];
    int w;
    int h;
    int l;
    int xleft;
    int xright;
    int ztop;
    int zbottom;
    int yoff;
    int railXOff;
    int railYOff;
    int railZOff;
    struct RailInfo *railInfo;
    unsigned char flags;
    unsigned char flagsPadding[3];
};

/** One point in a generated rail string (28 bytes). */
struct RailPoint : public CClass
{
    struct SVector Pos;
    struct RailPoint *Next;
    int ModelIndex;
    int Material;
    void *IsDoubleLink;

    /** Constructs an unlinked rail point with default flags. */
    RailPoint();
};

/** Linked string of rail points (32 bytes). */
struct RailString
{
    struct CClass CClass;
    struct SVector BoundsMin;
    struct SVector BoundsMax;
    struct RailPoint *PointList;
    struct RailString *Next;
    void *IsLoop;
    int NumPoints;

    /** Appends one point and records whether it closes the string. */
    void AddPoint(RailPoint *point, bool is_loop);
    /** Ensures a closed rail string has counter-clockwise winding. */
    void MakeCCW(void);
    /** Copies and transforms every point from another rail string. */
    void CopyString(RailString *from, int xoff, int yoff, int zoff, int rot);
    /** Constructs an empty rail string. */
    RailString();
};

/** Generated model placed in the level-editor occupancy maps (128 bytes). */
struct EditorOutModel
{
    /** Constructs one generated model from its source model and editor id. */
    EditorOutModel(EditorInModel *source, int id);
    /** Initializes shared lighting tables and the output-face pool. */
    static void Init();
    /** Releases the shared output-model lookup tables and face pool. */
    static void Cleanup();
    /** Restores every generated face to its live state. */
    void RestoreFaces();
    /** Removes hidden faces shared with neighboring generated models. */
    void RemoveAllHiddenFaces();
    /** Restores or hides shared faces against one neighboring side. */
    void UnhideSharedFaces(
        bool unhide, EditorOutModel *wire_model, unsigned char against);

    struct CClass CClass;
    void *DeleteModelRequest;
    unsigned char FaceRemovalRequest;
    unsigned char FaceRemovalPadding[3];
    void *RestoreFacesRequest;
    unsigned char RestoreFacesSide;
    unsigned char RestoreFacesPadding;
    unsigned short Size;
    unsigned short NumVertices;
    unsigned short NumNormals;
    unsigned short NumFaces;
    unsigned char NumNeighbors;
    unsigned char mFirstEasternNeighbor;
    struct EditorOutModel *Neighbors[16];
    struct ModelFace2 *Faces;
    unsigned char XCell;
    unsigned char YCell;
    unsigned char ZCell;
    unsigned char WCell;
    unsigned char HCell;
    unsigned char LCell;
    short X;
    short Y;
    short Z;
    unsigned char Rot;
    unsigned char RotPadding[3];
    struct EditorInModel *SourceModel;
    struct SModel *TargetModel;
    struct Item *TargetItem;
    struct EditorOutModel *Next;
};

/** Complete level generator object (42548 bytes / 0xA634). */
class LevelGenerator
{
public:
    /** Region slot holding the generated editor-level output. */
    static int OutRegion;

    struct CClass CClass;
    unsigned int NumSourceModels;
    char SourceFileName[16];
    int SourceRegion;
    struct EditorInModel SourceModelTab[150];
    char ShellFileName[16];
    int ShellRegion;
    struct EditorOutModel *OutModelList;
    struct MapCell OutModelMap[24][24];
    int GameBuild;
    int ZoneCellWidth;
    int ZoneBytes;
    struct ZoneCell **ZoneTab;
    struct RailInfo OutRails;
    struct GridCell FloorTab[24][24];
    struct EditorOutModel *FloorModelList;
    struct MapCell FloorModelMap[24][24];
    int FirstShellModel;
    int NumShellModels;

    LevelGenerator(bool game_build);
    static void Init(void);
    void LoadModels(char *file);
    void UnloadModels(void);
    void ProcessGrid(void);
    void ClearOutModels(void);
    void GenerateOutModels(void);
    void RemoveDeletedModels(void);
    void FigureOutNeighbors(void);
    void ClearFloorModels(void);
    void GenerateFloorModels(void);
    void HideFloorPieces(void);
    void LoadRails(char *file);
    static void UnloadOutPSX(bool killBuffers);
    static void LoadMap(char *load_name);
    static void SetMapTheme(int theme);
    int RemoveModelFromMap(int xmap, int zmap, int *no_lower_than,
                           EditorInModel **ret_model);
    u32 *CreatePSX(int *ret_region, bool refresh_models);
    int CreateZonePacket(int *at);
    void CreateRails(void);
    void FakeTrig(void);
    void UnhideFaces(EditorOutModel *das_model, bool on);
    void UnhideFloor(EditorOutModel *das_model, bool unhide);
    void HideWalls(bool hide);
    void AddModel(int x, int z, int slot, int rot, int index);
    void KillModel(int x, int z, int slot);
};

/** Typed owner-local lifecycle and method ABI entry points. */
#define LEVELGEN_DECLARE_DESTRUCTOR_ABI
#define LEVELGEN_DESTRUCTOR_NAME LevelGenerator_destruct
#define LEVELGEN_DESTRUCTOR_RECEIVER LevelGenerator
#include "LEVELGEN.h"
namespace LevelGenConstructorABI
{
#define GENOBJS_ABI_SELECTORS_ONLY
#define GENOBJS_DECLARE_EDITOR_IN_MODEL_CONSTRUCTOR_ABI
#define GENOBJS_EDITOR_IN_MODEL_CONSTRUCTOR_TYPE EditorInModel
#define GENOBJS_DECLARE_LEVELGEN_EDITOR_METHOD_ABI
#define GENOBJS_LEVELGEN_EDITOR_OUT_MODEL_TYPE EditorOutModel
#include "GENOBJS.h"
}
#define RAILINFO_DECLARE_DESTRUCTOR
#define RAILINFO_DESTRUCTOR_NAME LevelGen_RailInfoDestructor
#define RAILINFO_DESTRUCTOR_RECEIVER RailInfo
#include "RAILINFO.h"
#define LevelGen_InitEditorPolyBuffer \
    LevelGenConstructorABI::InitPolyBuf__13EditorInModelb
#define LevelGen_SetEditorModelDataSize \
    LevelGenConstructorABI::SetModelDataSize__14EditorOutModelb
#define LevelGen_CreateEditorModelData \
    LevelGenConstructorABI::CreateModelData__14EditorOutModelPcb

// Natural C++ spellings for free-function imports used only by this owner.
#include "SPOOL.h"
#include "MEM.h"
#include "PAL.h"
#include "FILEIO.h"

typedef char LevelGen_FakeTriggerNode_size_must_be_24[
    (sizeof(LevelGen_FakeTriggerNode) == 24) ? 1 : -1];
typedef char LevelGen_ModelBounds_size_must_be_12[
    (sizeof(LevelGenModelBounds) == 12) ? 1 : -1];
typedef char LevelGen_TextureUsage_view_size_must_be_20[
    (sizeof(LevelGenTextureUsageView) == 20) ? 1 : -1];
typedef char LevelGen_ZonePacketHeader_size_must_be_28[
    (sizeof(LevelGenZonePacketHeader) == 28) ? 1 : -1];
typedef char LevelGen_TriggerFileHeader_size_must_be_12[
    (sizeof(LevelGenTriggerFileHeader) == 12) ? 1 : -1];
typedef char LevelGen_RailInfo_size_must_be_12[(sizeof(RailInfo) == 12) ? 1 : -1];
typedef char LevelGen_EditorInModel_size_must_be_68[(sizeof(EditorInModel) == 68) ? 1 : -1];
typedef char LevelGen_RailPoint_size_must_be_28[(sizeof(RailPoint) == 28) ? 1 : -1];
typedef char LevelGen_RailString_size_must_be_32[(sizeof(RailString) == 32) ? 1 : -1];
typedef char LevelGen_GridCell_size_must_be_16[(sizeof(GridCell) == 16) ? 1 : -1];
typedef char LevelGen_ZoneCell_size_must_be_98[(sizeof(ZoneCell) == 98) ? 1 : -1];
typedef char LevelGen_MapCell_size_must_be_20[(sizeof(MapCell) == 20) ? 1 : -1];
typedef char LevelGen_EditorOutModel_size_must_be_128[(sizeof(EditorOutModel) == 128) ? 1 : -1];
typedef char LevelGen_LevelGenerator_size_must_be_42548[(sizeof(LevelGenerator) == 42548) ? 1 : -1];
#define EDITORPIECES_SIZED_FLOOR_DECLARATIONS
#include "EDITORPIECES.h"
#undef EDITORPIECES_SIZED_FLOOR_DECLARATIONS

// === External declarations and owner-local definitions ===
#define SPOOLENV_ABSOLUTE_DECLARATIONS
#define SPOOLENV_DECLARE_ENV_MODEL_CHECKSUMS
#include "SPOOLENV.h"
#undef SPOOLENV_ABSOLUTE_DECLARATIONS

/** Editor grid dimensions and generated-region defaults. */
int GridSize __attribute__((section(".sdata"))) = 24;
namespace LevelGenAddressViewOwners
{
extern "C"
{
static char DAT_800da664[8]
    __attribute__((section(".sdata"))) = "edit";
}
}
int LevelGenerator::OutRegion
    __attribute__((section(".sdata"))) = 0;
int _14LevelGenerator_GridCellHeight
    __attribute__((section(".sdata"))) = 0;
int _14LevelGenerator_GridCellWidth
    __attribute__((section(".sdata"))) = 0;

// The original compilation addresses this fixed name as an ordinary object,
// rather than through the small-data base used for the surrounding globals.
#define LEVELGEN_DECLARE_OUTPUT_NAME_ADDRESS_VIEW
#include "LEVELGEN.h"

// GCC 2.x emits a private copy of `CClass::~CClass` beside a private base
// vtable in every owner that derives from CClass.  `MAIN.SYM` records 25 such
// `_._6CClass` copies and names no base vtable, so both are file-local here.
// The legal per-address identifier keeps the private target copy distinct.
extern "C"
{
static void __DOT__6CClass_8005A060(CClass *self, int __in_chrg);
}

// C++ forbids taking a destructor's address, so the owner source provides an
// addressable ABI alias for the symbol this TU defines below.

// Consecutive eight-byte-aligned small-data words per vtable rather than
// arrays, so -G8 still yields the target's `addiu $vX, $gp, off` form.
int __vt_14LevelGenerator
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int LevelGen_VTableIndex __attribute__((section(".sdata"))) = 0;
static int LevelGen_VTableDtor __attribute__((section(".sdata"))) =
    (int)&LevelGenerator_destruct;

static int LevelGen_BaseVTable
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int LevelGen_BaseVTableIndex __attribute__((section(".sdata"))) = 0;
static int LevelGen_BaseVTableDtor __attribute__((section(".sdata"))) =
    (int)&__DOT__6CClass_8005A060;

/** Model-placement grid cleared by LevelGenerator::Init. */
GridCell _14LevelGenerator_GridTab[24][24]
    __attribute__((section(".data"))) = {{{0}}};

// === Function implementations (address order) ===

// ====== 0x80056020 __14LevelGeneratorb =========================================
// __14LevelGeneratorb
// Address: 0x80056020
// Demangled: LevelGenerator::(bool)
LevelGenerator::LevelGenerator(bool game_build)
{
    CClass.vtable = &__vt_14LevelGenerator;
    EditorInModel *ptr = SourceModelTab;
    int i = 0x95;
    do
    {
        LevelGenConstructorABI::__13EditorInModel(ptr);
        i--;
        ptr++;
    } while (i != -1);
    RailInfo_ctor(&OutRails);
    GameBuild = game_build;
    SourceRegion = -1;
    ShellRegion = -1;
    ZoneCellWidth = 0x320;
    OutModelList = 0;
    ClearOutModels();
    FloorModelList = 0;
    ClearFloorModels();
    EditorOutModel::Init();
    M3d_FadeColour = 0x80000000;
    M3dInit_SetFoggingParams(10, 0x1770, 0x800);
}

// ====== 0x800560EC _._14LevelGenerator =========================================
// _._14LevelGenerator
// Address: 0x800560EC
// Demangled: LevelGenerator::~LevelGenerator()
void LevelGenerator_destruct(LevelGenerator *self, int __in_chrg)
{
    self->CClass.vtable = &__vt_14LevelGenerator;
    self->UnloadModels();
    self->ClearOutModels();
    self->ClearFloorModels();

    EditorInModel *start = self->SourceModelTab;
    EditorOutModel::Cleanup();
    LevelGen_RailInfoDestructor(&self->OutRails, 2);

    EditorInModel *ptr = &self->SourceModelTab[150];
    if (start != 0 && start != ptr)
    {
        do
        {
            ptr--;
            typedef void (*EditorInModelDestructor)(EditorInModel *, int);
            EditorInModelDestructor dtor =
                (EditorInModelDestructor)ptr->CClass.vtable[2];
            dtor(ptr, 2);
        } while (start != ptr);
    }

    self->CClass.vtable = &__vt_14LevelGenerator + 4;
    if (__in_chrg & 1)
    {
        ::CClass::operator delete(self);
    }
}

/**
 *
 * Address:   0x800561A0
 * Mangled:   Init__14LevelGenerator
 * Demangled: LevelGenerator::Init(void)
 */
void LevelGenerator::Init(void)
{
    int x, y;
    int z = 0;
    int layerOff = 0;
    short cellInit = -1;
    u8 flagInit = 1;

    int base = (int)_14LevelGenerator_GridTab;
    int rotBase = base + sizeof(_14LevelGenerator_GridTab[0][0].modelIndex);

    _14LevelGenerator_GridCellWidth = 0x154;
    _14LevelGenerator_GridCellHeight = 0xAA;

    do
    {
        x = 0;
        u8 *flagPtr = (u8 *)(layerOff + base + sizeof(GridCell) - 1);
        int cellOff = layerOff;
        do
        {
            y = 0;
            u8 *rotPtr = (u8 *)(cellOff + rotBase);
            short *gridPtr = (short *)(cellOff + base);
            do
            {
                *gridPtr = cellInit;
                *flagPtr = flagInit;
                *rotPtr = 0;
                rotPtr++;
                y++;
                gridPtr++;
            } while (y < 5);
            flagPtr += sizeof(GridCell);
            x++;
            cellOff += sizeof(GridCell);
        } while (x < 24);
        layerOff += sizeof(_14LevelGenerator_GridTab[0]);
        z++;
    } while (z < 24);

    LevelGenerator::OutRegion = -1;
}

/**
 * Loads a level's geometry, source models, and rail trigger data.
 *
 * The wall geometry comes from `<name>_W`, while the main level data comes
 * from `<name>`. Item positions in the main file are converted from 20.12
 * fixed point and retained as rail offsets for the corresponding source
 * models. Rail trigger data is then loaded from `<name>_T`.
 *
 * Address:   0x8005623C
 * Mangled:   LoadModels__14LevelGeneratorPc
 * Demangled: LevelGenerator::LoadModels(char *)
 *
 * @param file Base filename for the level data files.
 */

void LevelGenerator::LoadModels(char *file)
{
    unsigned int *pPSX;
    int num_items;
    SModel **ppModels;
    char trig_file[32];
    int game_build = GameBuild;

    if (game_build != 0)
    {
        TransparentPalForEditor = 1;
    }
    LevelGen_InitEditorPolyBuffer(game_build);
    UnloadModels();

    int piece = 0;
    while (PieceMap[piece].map[0] != 0xff)
    {
        piece++;
    }

    {
        int x;
        for (x = 0; file[x] != '\0'; x++)
        {
            ShellFileName[x] = file[x];
        }
        ShellFileName[x++] = '_';
        ShellFileName[x++] = 'W';
        ShellFileName[x] = '\0';
    }

    ShellRegion = Spool_PSX(ShellFileName, 0, 0, 1);
    Spool_Sync();

    SourceRegion = Spool_PSX(file, 0, 0, 1);
    strcpy(SourceFileName, file);
    Spool_Sync();

    pPSX = PSXRegion[SourceRegion].pPSX;
    num_items = pPSX[2];
    ppModels = (SModel **)(num_items * sizeof(Item) +
                           (unsigned int)pPSX + 16);
    NumSourceModels = *((unsigned int *)ppModels - 1);

    {
        int i;
        int *item_data = (int *)pPSX;
        for (i = 0; i < num_items; i++)
        {
            SourceModelTab[i].railXOff = item_data[i * 9 + 4] >> 12;
            SourceModelTab[i].railYOff = item_data[i * 9 + 5] >> 12;
            SourceModelTab[i].railZOff = item_data[i * 9 + 6] >> 12;
        }
    }

    Spool_SkipPackets(pPSX);

    {
        int i = 0;
        if (NumSourceModels != 0)
        {
            SModel **models = ppModels;
            EditorInModel *source_model = SourceModelTab;
            do
            {
                source_model->SetData(*models, i);
                models++;
                source_model++;
                i++;
            } while (i < NumSourceModels);
        }
    }

    {
        int x;
        for (x = 0; file[x] != '\0'; x++)
        {
            trig_file[x] = file[x];
        }
        trig_file[x++] = '_';
        trig_file[x++] = 'T';
        trig_file[x] = '\0';
    }

    LoadRails(trig_file);
    TransparentPalForEditor = 0;
}

/**
 *
 * Address:   0x800564E0
 * Mangled:   UnloadModels__14LevelGenerator
 * Demangled: LevelGenerator::UnloadModels(void)
 */
void LevelGenerator::UnloadModels(void)
{
    unsigned int i = 0;

    ClearOutModels();
    ClearFloorModels();

    if (NumSourceModels != 0)
    {
        EditorInModel *ptr = SourceModelTab;
        do
        {
            ptr->KillModelInfo();
            ptr++;
        } while (++i < NumSourceModels);
    }

    if (SourceRegion != -1)
    {
        Spool_ClearPSX(SourceFileName);
    }
    SourceRegion = -1;

    if (ShellRegion != -1)
    {
        Spool_ClearPSX(ShellFileName);
    }
    ShellRegion = -1;

    Spool_RemoveUnusedTextures();
    Pal_RemoveUnusedPalettes();
    Pack_DefragmentFully();
}

/**
 * ProcessGrid__14LevelGenerator - Build and compact the generated floor grid.
 *
 * Address:   0x80056594
 * Mangled:   ProcessGrid__14LevelGenerator
 * Demangled: LevelGenerator::ProcessGrid(void)
 */
void LevelGenerator::ProcessGrid(void)
{
    int replacement_top;

    /*
     * Pass 1: seed every floor-grid slot.  A slot below the cell's used count
     * takes the flat 1x1 floor piece; everything else is emptied.  Cells are
     * sixteen bytes (five short model indices, five rotation bytes, a count).
     */
    for (int slot = 0; slot < 5; slot++)
    {
        for (int j = 0; j < 24; j++)
        {
            for (int i = 0; i < 24; i++)
            {
                FloorTab[j][i].modelIndex[slot] = -1;
                FloorTab[j][i].rot[slot] = 0;
                if (slot < _14LevelGenerator_GridTab[j][i].numUsedSlots)
                {
                    FloorTab[j][i].modelIndex[slot] =
                        (short)Floor1X1Piece[slot];
                }
            }
        }
    }

    /*
     * Pass 2: pieces placed in the editor grid knock out the floor slots they
     * cover.  A piece's footprint swaps width and length on odd rotations, and
     * three flag bits choose which slots go: 0x10 clears the column above the
     * piece up to its height, 0x02 clears the piece's own layer, and 0x04
     * clears it only for game builds above the ground layer.
     */
    for (int slot = 0; slot < 5; slot++)
    {
        for (int j = 0; j < 24; j++)
        {
            for (int i = 0; i < 24; i++)
            {
                if (slot <
                    _14LevelGenerator_GridTab[j][i].numUsedSlots)
                {
                    int piece_num =
                        _14LevelGenerator_GridTab[j][i].modelIndex[slot];
                    if (piece_num != -1)
                    {
                        int w;
                        int l;
                        int h;
                        int start_level;

                        switch (_14LevelGenerator_GridTab[j][i].rot[slot])
                        {
                        case 0:
                        case 2:
                            w = SourceModelTab[piece_num].w;
                            l = SourceModelTab[piece_num].l;
                            break;
                        case 1:
                        case 3:
                            w = SourceModelTab[piece_num].l;
                            l = SourceModelTab[piece_num].w;
                            break;
                        }

                        h = SourceModelTab[piece_num].h;
                        if ((SourceModelTab[piece_num].flags & 0x10) != 0)
                        {
                            start_level = slot < 1;
                            for (int z = j; z < j + l; z++)
                            {
                                for (int x = i; x < i + w; x++)
                                {
                                    for (int s = start_level; s < h; s++)
                                    {
                                        FloorTab[z][x]
                                            .modelIndex[slot + s] = -1;
                                    }
                                }
                            }
                        }
                        if ((SourceModelTab[piece_num].flags & 0x02) != 0)
                        {
                            for (int z = j; z < j + l; z++)
                            {
                                for (int x = i; x < i + w; x++)
                                {
                                    FloorTab[z][x].modelIndex[slot] = -1;
                                }
                            }
                        }
                        if (GameBuild != 0 &&
                            (SourceModelTab[piece_num].flags & 0x04) != 0 &&
                            slot > 0)
                        {
                            for (int z = j; z < j + l; z++)
                            {
                                for (int x = i; x < i + w; x++)
                                {
                                    FloorTab[z][x].modelIndex[slot] = -1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    /*
     * Collapse groups of four identical 1x1 floor tiles into one 2x2 tile.
     * Only the ground layer is generated for an editor build; game builds
     * generate all five vertical layers.
     */
    replacement_top = 5;
    if (!GameBuild)
    {
        replacement_top = 1;
    }
    for (int slot = 0; slot < replacement_top; slot++)
    {
        for (int j = 0; j < 24; j += 2)
        {
            for (int i = 0; i < 24; i += 2)
            {
                short piece = FloorTab[j][i].modelIndex[slot];
                if (piece == Floor1X1Piece[slot] &&
                    FloorTab[j][i + 1].modelIndex[slot] == piece &&
                    FloorTab[j + 1][i].modelIndex[slot] == piece &&
                    FloorTab[j + 1][i + 1].modelIndex[slot] == piece)
                {
                    FloorTab[j][i].modelIndex[slot] =
                        (short)Floor2X2Piece[slot];
                    FloorTab[j][i + 1].modelIndex[slot] = -1;
                    FloorTab[j + 1][i].modelIndex[slot] = -1;
                    FloorTab[j + 1][i + 1].modelIndex[slot] = -1;
                }
            }
        }
    }

    /*
     * Pair the remaining 1x1 tiles.  Prefer an east-west pair, then a
     * north-south pair; rotation three turns the 1x2 model onto the latter.
     */
    for (int slot = 0; slot < replacement_top; slot++)
    {
        for (int j = 0; j < 24; j++)
        {
            for (int i = 0; i < 24; i++)
            {
                if (i < 23 &&
                    FloorTab[j][i].modelIndex[slot] ==
                        Floor1X1Piece[slot] &&
                    FloorTab[j][i + 1].modelIndex[slot] ==
                        FloorTab[j][i].modelIndex[slot])
                {
                    FloorTab[j][i].modelIndex[slot] =
                        (short)Floor1X2Piece[slot];
                    FloorTab[j][i + 1].modelIndex[slot] = -1;
                    FloorTab[j][i].rot[slot] = 0;
                }
                else if (j < 23 &&
                         FloorTab[j][i].modelIndex[slot] ==
                             Floor1X1Piece[slot] &&
                         FloorTab[j + 1][i].modelIndex[slot] ==
                             FloorTab[j][i].modelIndex[slot])
                {
                    FloorTab[j][i].modelIndex[slot] =
                        (short)Floor1X2Piece[slot];
                    FloorTab[j + 1][i].modelIndex[slot] = -1;
                    FloorTab[j][i].rot[slot] = 3;
                }
            }
        }
    }
}

/**
 * ClearOutModels__14LevelGenerator - Delete generated output models and clear grid slots.
 *
 * Address:   0x80056CD4
 * Mangled:   ClearOutModels__14LevelGenerator
 * Demangled: LevelGenerator::ClearOutModels(void)
 */
void LevelGenerator::ClearOutModels(void)
{
    EditorOutModel *model = OutModelList;

    while (model != 0)
    {
        EditorOutModel *next_model = model->Next;
        if (model != 0)
        {
            typedef void (*EditorOutModelDestructor)(EditorOutModel *, int);
            EditorOutModelDestructor destroy =
                (EditorOutModelDestructor)model->CClass.vtable[2];
            destroy(model, 3);
        }
        model = next_model;
    }

    OutModelList = 0;

    for (int i = 0; i < 24; i++)
    {
        for (int j = 0; j < 24; j++)
        {
            for (int slot = 0; slot < 5; slot++)
            {
                OutModelMap[i][j].modelInSlot[slot] = 0;
            }
        }
    }
}

/**
 * GenerateOutModels__14LevelGenerator
 *
 * Address:   0x80056D94
 * Mangled:   GenerateOutModels__14LevelGenerator
 * Demangled: LevelGenerator::GenerateOutModels(void)
 *            Rebuilds the output-model list and three-dimensional occupancy map.
 */
void LevelGenerator::GenerateOutModels(void)
{
    EditorOutModel *last_model;
    int z;

    ClearOutModels();
    last_model = 0;

    z = (_14LevelGenerator_GridCellWidth * 24) >> 1;
    {
        int j;
        for (j = 0; j < 24; j++)
        {
            int x = (_14LevelGenerator_GridCellWidth * -24) >> 1;
            {
                int i;
                for (i = 0; i < 24; i++)
                {
                    int y = 0;
                    {
                        int slot;
                        for (slot = 0; slot < 5; slot++)
                        {
                            EditorInModel *in_model;
                            EditorOutModel *out_model;
                            int xoff;
                            int zoff;

                            if (_14LevelGenerator_GridTab[j][i]
                                        .modelIndex[slot] == -1 ||
                                slot >= _14LevelGenerator_GridTab[j][i]
                                            .numUsedSlots)
                            {
                                y -= _14LevelGenerator_GridCellHeight;
                            }
                            else
                            {
                                in_model =
                                    &SourceModelTab[
                                        _14LevelGenerator_GridTab[j][i]
                                            .modelIndex[slot]];
                                out_model = static_cast<EditorOutModel *>(
                                    ::CClass::operator new(
                                        sizeof(EditorOutModel)));
                                if (out_model != 0)
                                {
                                    out_model = ::new (out_model)
                                        EditorOutModel(
                                            in_model, j * 100 + i);
                                }

                                switch (_14LevelGenerator_GridTab[j][i]
                                            .rot[slot])
                                {
                                case 0:
                                    xoff = in_model->xleft;
                                    zoff = in_model->ztop;
                                    out_model->WCell =
                                        (unsigned char)in_model->w;
                                    out_model->LCell =
                                        (unsigned char)in_model->l;
                                    break;
                                case 1:
                                    xoff = in_model->ztop;
                                    zoff = in_model->xright;
                                    out_model->WCell =
                                        (unsigned char)in_model->l;
                                    out_model->LCell =
                                        (unsigned char)in_model->w;
                                    break;
                                case 2:
                                    xoff = in_model->xright;
                                    zoff = in_model->zbottom;
                                    out_model->WCell =
                                        (unsigned char)in_model->w;
                                    out_model->LCell =
                                        (unsigned char)in_model->l;
                                    break;
                                case 3:
                                    xoff = in_model->zbottom;
                                    zoff = in_model->xleft;
                                    out_model->WCell =
                                        (unsigned char)in_model->l;
                                    out_model->LCell =
                                        (unsigned char)in_model->w;
                                    break;
                                }

                                int positioned_x = x;
                                positioned_x += xoff;
                                unsigned char height =
                                    (unsigned char)in_model->h;
                                out_model->Z = (short)(z - zoff);
                                out_model->X = (short)positioned_x;
                                out_model->Y = (short)y;
                                out_model->XCell = (unsigned char)i;
                                out_model->YCell = (unsigned char)slot;
                                out_model->ZCell = (unsigned char)j;
                                out_model->HCell = height;
                                out_model->Rot =
                                    _14LevelGenerator_GridTab[j][i]
                                        .rot[slot];

                                {
                                    int n;
                                    for (n = j;
                                         n < j + out_model->LCell;
                                         n++)
                                    {
                                        int m;
                                        for (m = i;
                                             m < i + out_model->WCell;
                                             m++)
                                        {
                                            int s;
                                            for (s = 0;
                                                 s < out_model->HCell;
                                                 s++)
                                            {
                                                OutModelMap[n][m]
                                                    .modelInSlot[slot + s] =
                                                    out_model;
                                            }
                                        }
                                    }
                                }

                                if (last_model == 0)
                                {
                                    OutModelList = out_model;
                                }
                                else
                                {
                                    last_model->Next = out_model;
                                }
                                last_model = out_model;
                                y -= _14LevelGenerator_GridCellHeight;
                            }
                        }
                    }
                    x += _14LevelGenerator_GridCellWidth;
                }
            }
            z -= _14LevelGenerator_GridCellWidth;
        }
    }
}

/**
 * RemoveDeletedModels__14LevelGenerator
 *
 * Address:   0x8005714C
 * Mangled:   RemoveDeletedModels__14LevelGenerator
 * Demangled: LevelGenerator::RemoveDeletedModels(void)
 *
 */

void LevelGenerator::RemoveDeletedModels(void)
{
    EditorOutModel *model = OutModelList;
    EditorOutModel *prev = 0;

    while (model)
    {
        EditorOutModel *next = model->Next;

        if (model->DeleteModelRequest)
        {
            if (model)
            {
                typedef void (*EditorOutModelDestructor)(
                    EditorOutModel *, int);
                EditorOutModelDestructor dtor =
                    (EditorOutModelDestructor)model->CClass.vtable[2];
                dtor(model, 3);
            }
            if (prev)
            {
                prev->Next = next;
            }
            else
            {
                OutModelList = next;
            }
        }
        else
        {
            if (model->RestoreFacesRequest)
            {
                model->RestoreFaces();
            }
            model->RestoreFacesRequest = 0;
            prev = model;
        }
        model = next;
    }
}

/**
 * Collects the models touching the southern and eastern edges of each model.
 *
 * Neighbors are de-duplicated as they are read from the occupancy map.  The
 * first pass also records where the eastern-edge neighbors begin in the
 * neighbor array.
 *
 * Address:   0x80057208
 * Mangled:   FigureOutNeighbors__14LevelGenerator
 * Demangled: LevelGenerator::FigureOutNeighbors(void)
 */
void LevelGenerator::FigureOutNeighbors(void)
{
    EditorOutModel *model = OutModelList;
    while (model != 0)
    {
        int z;
        int x;

        model->NumNeighbors = 0;
        model->mFirstEasternNeighbor = 0;
        x = model->XCell;
        z = model->ZCell + model->LCell;

        if (z < 24)
        {
            for (; x < model->XCell + model->WCell; x++)
            {
                if (x >= 24)
                {
                    break;
                }

                int neighbor_slot;
                for (neighbor_slot = 0; neighbor_slot < 5; neighbor_slot++)
                {
                    EditorOutModel *possible_add;
                    int i;

                    possible_add = OutModelMap[z][x].modelInSlot[neighbor_slot];
                    if (possible_add != 0)
                    {
                        i = 0;
                        while (i < model->NumNeighbors)
                        {
                            if (model->Neighbors[i] == possible_add)
                            {
                                break;
                            }
                            i++;
                        }

                        if (i == model->NumNeighbors)
                        {
                            model->NumNeighbors = i + 1;
                            model->Neighbors[(unsigned char)i] = possible_add;
                            model->mFirstEasternNeighbor = model->NumNeighbors;
                        }
                    }
                }
            }
        }

        z = model->ZCell;
        x = model->XCell + model->WCell;
        if (x < 24)
        {
            for (; z < model->ZCell + model->LCell; z++)
            {
                if (z >= 24)
                {
                    break;
                }

                int neighbor_slot;
                for (neighbor_slot = 0; neighbor_slot < 5; neighbor_slot++)
                {
                    EditorOutModel *possible_add;
                    int i;

                    possible_add = OutModelMap[z][x].modelInSlot[neighbor_slot];
                    if (possible_add != 0)
                    {
                        i = 0;
                        while (i < model->NumNeighbors)
                        {
                            if (model->Neighbors[i] == possible_add)
                            {
                                break;
                            }
                            i++;
                        }

                        if (i == model->NumNeighbors)
                        {
                            model->NumNeighbors = i + 1;
                            model->Neighbors[(unsigned char)i] = possible_add;
                        }
                    }
                }
            }
        }

        model = model->Next;
    }
}

/**
 * ClearFloorModels__14LevelGenerator
 *
 * Address:   0x80057428
 * Mangled:   ClearFloorModels__14LevelGenerator
 * Demangled: LevelGenerator::ClearFloorModels(void)
 *            Iterates the floor-model list and calls each virtual destructor.
 */
void LevelGenerator::ClearFloorModels(void)
{
    EditorOutModel *model = FloorModelList;
    if (model != 0)
    {
        do
        {
            EditorOutModel *next_model = model->Next;
            if (model != 0)
            {
                typedef void (*DestroyFunc)(EditorOutModel *, int);
                DestroyFunc func =
                    (DestroyFunc)model->CClass.vtable[2];
                func(model, 3);
            }
            model = next_model;
        } while (model != 0);
    }
    FloorModelList = 0;
}

/**
 * GenerateFloorModels__14LevelGenerator
 *
 * Address:   0x80057494
 * Mangled:   GenerateFloorModels__14LevelGenerator
 * Demangled: LevelGenerator::GenerateFloorModels(void)
 *            Rebuilds the floor-model list and grid occupancy map.
 */
void LevelGenerator::GenerateFloorModels(void)
{
    EditorOutModel *last_model;
    int z;

    ClearFloorModels();
    last_model = 0;

    {
        int j;
        for (j = 0; j < 24; j++)
        {
            int i;
            for (i = 0; i < 24; i++)
            {
                int slot;
                for (slot = 0; slot < 5; slot++)
                {
                    FloorModelMap[j][i].modelInSlot[slot] = 0;
                }
            }
        }
    }

    z = (_14LevelGenerator_GridCellWidth * 24) >> 1;
    {
        int j;
        for (j = 0; j < 24; j++)
        {
            int x = (_14LevelGenerator_GridCellWidth * -24) >> 1;
            {
                int i;
                for (i = 0; i < 24; i++)
                {
                    int y = 0;
                    {
                        int slot;
                        for (slot = 0; slot < 5; slot++)
                        {
                            EditorInModel *in_model;
                            EditorOutModel *out_model;
                            int xoff;
                            int zoff;

                            if (FloorTab[j][i].modelIndex[slot] == -1)
                            {
                                y -= _14LevelGenerator_GridCellHeight;
                            }
                            else
                            {
                                in_model = &SourceModelTab[
                                    FloorTab[j][i].modelIndex[slot]];
                                out_model = static_cast<EditorOutModel *>(
                                    ::CClass::operator new(
                                        sizeof(EditorOutModel)));
                                if (out_model != 0)
                                {
                                    out_model = ::new (out_model)
                                        EditorOutModel(
                                            in_model, j * 100 + i);
                                }

                                switch (FloorTab[j][i].rot[slot])
                                {
                                case 0:
                                    xoff = in_model->xleft;
                                    zoff = in_model->ztop;
                                    out_model->WCell =
                                        (unsigned char)in_model->w;
                                    out_model->LCell =
                                        (unsigned char)in_model->l;
                                    break;
                                case 1:
                                    xoff = in_model->ztop;
                                    zoff = in_model->xright;
                                    out_model->WCell =
                                        (unsigned char)in_model->l;
                                    out_model->LCell =
                                        (unsigned char)in_model->w;
                                    break;
                                case 2:
                                    xoff = in_model->xright;
                                    zoff = in_model->zbottom;
                                    out_model->WCell =
                                        (unsigned char)in_model->w;
                                    out_model->LCell =
                                        (unsigned char)in_model->l;
                                    break;
                                case 3:
                                    xoff = in_model->zbottom;
                                    zoff = in_model->xleft;
                                    out_model->WCell =
                                        (unsigned char)in_model->l;
                                    out_model->LCell =
                                        (unsigned char)in_model->w;
                                    break;
                                }

                                int positioned_x = x;
                                positioned_x += xoff;
                                unsigned char height =
                                    (unsigned char)in_model->h;
                                out_model->Z = (short)(z - zoff);
                                out_model->X = (short)positioned_x;
                                out_model->Y = (short)y;
                                out_model->XCell = (unsigned char)i;
                                out_model->YCell = (unsigned char)slot;
                                out_model->ZCell = (unsigned char)j;
                                out_model->HCell = height;
                                out_model->Rot =
                                    FloorTab[j][i].rot[slot];

                                {
                                    int n;
                                    for (n = j;
                                         n < j + out_model->LCell;
                                         n++)
                                    {
                                        int m;
                                        for (m = i;
                                             m < i + out_model->WCell;
                                             m++)
                                        {
                                            FloorModelMap[n][m]
                                                .modelInSlot[slot] =
                                                out_model;
                                        }
                                    }
                                }

                                if (last_model == 0)
                                {
                                    FloorModelList = out_model;
                                }
                                else
                                {
                                    last_model->Next = out_model;
                                }
                                last_model = out_model;
                                y -= _14LevelGenerator_GridCellHeight;
                            }
                        }
                    }
                    x += _14LevelGenerator_GridCellWidth;
                }
            }
            z -= _14LevelGenerator_GridCellWidth;
        }
    }
}

/**
 * HideFloorPieces__14LevelGenerator
 *
 * Address:   0x8005784C
 * Mangled:   HideFloorPieces__14LevelGenerator
 * Demangled: LevelGenerator::HideFloorPieces(void)
 *
 */
void LevelGenerator::HideFloorPieces(void)
{
    if (GameBuild == 0)
    {
        EditorOutModel *model = OutModelList;
        while (model != 0)
        {
            UnhideFloor(model, 0);
            model = model->Next;
        }
    }
}

/**
 * Imports the rail strings associated with the source models used by a level.
 *
 * Address:   0x800578B0
 * Mangled:   LoadRails__14LevelGeneratorPc
 * Demangled: LevelGenerator::LoadRails(char *)
 *
 * @param file  Rail data filename.
 */
void LevelGenerator::LoadRails(char *file)
{
    unsigned int *pModelChecksums;
    unsigned short node;

    pModelChecksums = Spool_SkipPackets(PSXRegion[SourceRegion].pPSX);
    Trig_LoadTRG(file);

    for (node = 0; node < NumNodes;)
    {
        short num_links;
        int trickob;
        unsigned short *pTrick;
        unsigned int checksum;
        int model;
        RailString *string;
        unsigned short linked_node;

        if (Rail_IsRailNode(node))
        {
            short i;

            num_links = (short)Rail_NumLinks(node);
            trickob = -1;
            for (i = 0; i < num_links; i++)
            {
                unsigned short link = Rail_GetLink(node, i);
                if (*OffsetList[link] == 12)
                {
                    trickob = link;
                }
            }

            if (trickob != -1)
            {
                pTrick = (unsigned short *)OffsetList[trickob];
                pTrick += pTrick[1] + 2;
                if (((unsigned int)pTrick & 3) != 0)
                {
                    pTrick++;
                }
                checksum = *(unsigned int *)pTrick;

                for (model = 0; model < NumSourceModels; model++)
                {
                    if (pModelChecksums[model] == checksum)
                    {
                        break;
                    }
                }

                string = SourceModelTab[model].railInfo->NewString();
                linked_node = node;
                {
                    CVector loc;
                    unsigned short *pMaterial;
                    RailPoint *point;
                    bool is_loop;

                    while (true)
                    {
                        pMaterial = Trig_GetPosition(loc, linked_node);
                        asm volatile("" : "+r"(pMaterial));
                        point = static_cast<RailPoint *>(
                            ::CClass::operator new(sizeof(RailPoint)));
                        if (point != 0)
                        {
                            point = ::new (point) RailPoint();
                        }

                        point->Pos.vx = (loc.vx >> 12) -
                                        (short)SourceModelTab[model].railXOff;
                        point->Pos.vy = (loc.vy >> 12) -
                                        (short)SourceModelTab[model].railYOff;
                        point->Pos.vz = (loc.vz >> 12) -
                                        (short)SourceModelTab[model].railZOff;
                        point->Material = pMaterial[3];

                        linked_node = Rail_NextNode(linked_node);
                        is_loop = linked_node == node;
                        string->AddPoint(point, is_loop);
                        if (linked_node == 0 || is_loop)
                        {
                            break;
                        }
                    }
                }

                string->MakeCCW();
            }
        }
        {
            register int next asm("$2") = node + 1;
            node = (unsigned short)next;
        }
        asm volatile("" : "+r"(node));
    }

    Trig_DeleteTrigFile();
}

/**
 * UnloadOutPSX__14LevelGeneratorb
 *
 * Address:   0x80057B68
 * Mangled:   UnloadOutPSX__14LevelGeneratorb
 * Demangled: LevelGenerator::UnloadOutPSX(bool)
 *            Release the generated PSX output region and reset the active region index.
 *
 * @param killBuffers Release the generated region's colour table first.
 */
void LevelGenerator::UnloadOutPSX(bool killBuffers)
{
    if (LevelGenerator::OutRegion != -1)
    {
        if (killBuffers)
        {
            Mem_Delete(PSXRegion[LevelGenerator::OutRegion].pColourTable);
            pColourTable = 0;
            PSXRegion[LevelGenerator::OutRegion].pColourTable = 0;
        }
        ClearRegion(LevelGenerator::OutRegion, 0, 0);
    }
    LevelGenerator::OutRegion = -1;
}

/**
 * LoadMap__14LevelGeneratorPc
 *
 * Address:   0x80057BE8
 * Mangled:   LoadMap__14LevelGeneratorPc
 * Demangled: LevelGenerator::LoadMap(char *)
 *
 * @param load_name  Saved level-map filename.
 */
void LevelGenerator::LoadMap(char *load_name)
{
    unsigned int size = FileIO_Open(load_name);
    char *buf = (char *)Mem_New(size, 1, 1);
    FileIO_Load(buf);
    FileIO_Sync();

    GridCell *loaded_grid = (GridCell *)buf;
    GridCell *out_grid = &_14LevelGenerator_GridTab[0][0];
    for (int i = 0; i < 24 * 24; i++)
    {
        int slot = 0;
        if (loaded_grid[i].numUsedSlots != 0)
        {
            short empty = -1;
            do
            {
                if (loaded_grid[i].modelIndex[slot] != empty)
                {
                    loaded_grid[i].modelIndex[slot] =
                        (short)EditorPieces_TranslatePieceIn(
                            loaded_grid[i].modelIndex[slot]);
                }
                slot++;
            } while (slot < loaded_grid[i].numUsedSlots);
        }

        out_grid[i].modelIndex[0] = loaded_grid[i].modelIndex[0];
        out_grid[i].modelIndex[1] = loaded_grid[i].modelIndex[1];
        out_grid[i].modelIndex[2] = loaded_grid[i].modelIndex[2];
        out_grid[i].modelIndex[3] = loaded_grid[i].modelIndex[3];
        out_grid[i].modelIndex[4] = -1;
        out_grid[i].rot[0] = loaded_grid[i].rot[0];
        out_grid[i].rot[1] = loaded_grid[i].rot[1];
        out_grid[i].rot[2] = loaded_grid[i].rot[2];
        out_grid[i].rot[3] = loaded_grid[i].rot[3];
        out_grid[i].rot[4] = loaded_grid[i].rot[4];
        out_grid[i].numUsedSlots = loaded_grid[i].numUsedSlots;
    }

    if (buf != 0)
        Mem_Delete(buf);
}

/**
 * Translates placed-piece identifiers around a level-theme change.
 *
 * Address:   0x80057E88
 * Mangled:   SetMapTheme__14LevelGeneratori
 * Demangled: LevelGenerator::SetMapTheme(int)
 *
 * @param theme  Theme identifier.
 */
void LevelGenerator::SetMapTheme(int theme)
{
    GridCell *grid = &_14LevelGenerator_GridTab[0][0];
    for (int i = 0; i < 24 * 24; i++)
    {
        int slot = 0;
        if (grid[i].numUsedSlots != 0)
        {
            short empty = -1;
            do
            {
                if (grid[i].modelIndex[slot] != empty)
                {
                    grid[i].modelIndex[slot] =
                        (short)EditorPieces_TranslatePieceOut(grid[i].modelIndex[slot]);
                }
                slot++;
            } while (slot < grid[i].numUsedSlots);
        }
    }

    EditorPieces_SetTheme(theme);

    grid = &_14LevelGenerator_GridTab[0][0];
    for (int i = 0; i < 24 * 24; i++)
    {
        int slot = 0;
        if (grid[i].numUsedSlots != 0)
        {
            short empty = -1;
            do
            {
                if (grid[i].modelIndex[slot] != empty)
                {
                    grid[i].modelIndex[slot] =
                        (short)EditorPieces_TranslatePieceIn(grid[i].modelIndex[slot]);
                }
                slot++;
            } while (slot < grid[i].numUsedSlots);
        }
    }
}

/**
 * Removes the uppermost model occupying a map cell.
 *
 * Models that sit above the selected model are removed across its entire
 * footprint.  Neighbors around that footprint are then marked so hidden-face
 * data will be rebuilt.
 *
 * Address:   0x80057FD0
 * Mangled:   RemoveModelFromMap__14LevelGeneratoriiPiPP13EditorInModel
 * Demangled: LevelGenerator::RemoveModelFromMap(int, int, int *, EditorInModel **)
 *
 * @param xmap Grid X coordinate to inspect.
 * @param zmap Grid Z coordinate to inspect.
 * @param no_lower_than Lowest removable vertical slot, updated on success.
 * @param ret_model Receives the source model for the removed placement.
 * @return One when a model was removed; otherwise zero.
 */
int LevelGenerator::RemoveModelFromMap(int xmap, int zmap, int *no_lower_than, EditorInModel **ret_model)
{
    int slot;
    EditorOutModel *model;
    int x0;
    int z0;
    int x1;
    int z1;

    slot = 4;
    while (true)
    {
        model = OutModelMap[zmap][xmap].modelInSlot[slot];
        if (model != 0)
        {
            slot = model->YCell;
            break;
        }

        if (slot == 0)
        {
            break;
        }
        slot--;
    }

    if (model == 0)
    {
        return 0;
    }
    if (slot < *no_lower_than)
    {
        return 0;
    }

    *no_lower_than = slot;
    x0 = model->XCell;
    z0 = model->ZCell;
    x1 = x0 + model->WCell;
    z1 = z0 + model->LCell;

    if ((model->SourceModel->flags & 0x10) != 0)
    {
        int upper_slot = slot + model->HCell;
        for (int z = z0; z < z1; z++)
        {
            for (int x = x0; x < x1; x++)
            {
                if (OutModelMap[z][x].modelInSlot[upper_slot] != 0)
                {
                    return 0;
                }
            }
        }
    }

    *ret_model = OutModelMap[z0][x0].modelInSlot[slot]->SourceModel;

    for (int z = z0; z < z1; z++)
    {
        for (int x = x0; x < x1; x++)
        {
            for (int s = 0; s < model->HCell; s++)
            {
                EditorOutModel *mod =
                    OutModelMap[z][x].modelInSlot[slot + s];
                if (mod != 0)
                {
                    KillModel(x, z, slot + s);
                    _14LevelGenerator_GridTab[z][x].numUsedSlots =
                        (unsigned char)(slot + 1);
                }
            }
        }
    }

    for (int z = z0 - 1; z < z1 + 1; z++)
    {
        for (int x = x0 - 1; x < x1 + 1; x++)
        {
            for (int s = 0; s < 5; s++)
            {
                EditorOutModel *mod = OutModelMap[z][x].modelInSlot[s];
                if (mod != 0 && x >= 0 && x < 24 &&
                    z >= 0 && z < 24)
                {
                    mod->FaceRemovalRequest = 2;
                    mod->RestoreFacesRequest = (void *)1;
                }
            }
        }
    }

    return 1;
}

/**
 * Construct a PSX binary file from level-editor data.
 * This is the "save level" function for the PS1 level editor. It constructs
 * a complete PSX binary file from the LevelGenerator's in-memory representation:
 *   models, items, textures, face data, zone data, and lighting.
 * The output PSX file contains:
 *   +0x00: Header (version 0x20004, total size, item count)
 *   +0x0C: Item entry array (0x24 bytes per item) — positions as 20.12 fixed-point
 *   After items: Mesh name hash array, model data pointers
 *   Model data: Per-model face/vertex binary (written by CreateModelData)
 *   Wall face data: Copied from the loaded wall region
 *   Zone packet: Spatial subdivision data
 *   Texture refs: Texture entry pointer lists from both regions
 * Two model lists are processed: OutModelList for editor-placed models and
 * FloorModelList for generated floor pieces. Each is traversed through Next.
 * Wall geometry is copied from a previously-loaded "_W.PSX" file, including
 * its item entries and face data.
 * Parameters:
 *   self           — LevelGenerator *
 *   ret_region     — output: receives the allocated PSXRegion slot index
 *   refresh_models — if nonzero, call GenerateOutModels before processing
 * Returns: pointer to the allocated PSX binary buffer
 * @param ret_region Receives the allocated PSXRegion slot index.
 * @param refresh_models Regenerate editor output models before serialization.
 * @return The allocated PSX binary buffer.
 */

u32 *LevelGenerator::CreatePSX(int *ret_region, bool refresh_models)
{
    u32 *pSourcePSX;
    EditorOutModel *out_model;
    int num_ed_models;
    int model_data_size;
    u32 *pShellPSX;
    SModel **ppShellModels;
    u32 *pShellPacket;
    int shell_model_data_size;
    int num_models;
    int model_table_size;
    int item_table_size;

    ProcessGrid();

    if (LevelGenerator::OutRegion == -1)
    {
        int i;
        for (i = 0; i < 40; i++)
        {
            if (PSXRegion[i].Filename[0] == '\0')
            {
                LevelGenerator::OutRegion = i;
                PSXRegion[i].pColourTable = 0;
                break;
            }
        }
    }

    ClearRegion(LevelGenerator::OutRegion, 0, 0);
    __builtin_memcpy(PSXRegion[LevelGenerator::OutRegion].Filename,
                     DAT_800da664, 5);
    *ret_region = LevelGenerator::OutRegion;

    pSourcePSX = PSXRegion[SourceRegion].pPSX;

    if (refresh_models != 0)
    {
        GenerateOutModels();
    }

    RemoveDeletedModels();
    FigureOutNeighbors();
    GenerateFloorModels();

    FaceRemoval_Setup();
    for (out_model = OutModelList; out_model != 0;
         out_model = out_model->Next)
    {
        out_model->RemoveAllHiddenFaces();
    }
    FaceRemoval_Cleanup();

    for (out_model = OutModelList; out_model != 0;
         out_model = out_model->Next)
    {
        out_model->FaceRemovalRequest = 0;
    }

    num_ed_models = 0;
    model_data_size = 0;
    for (int which_list = 0; which_list < 2; which_list++)
    {
        if (which_list == 0)
        {
            out_model = OutModelList;
        }
        else
        {
            out_model = FloorModelList;
        }

        while (out_model != 0)
        {
            num_ed_models++;
            model_data_size += LevelGen_SetEditorModelDataSize(
                out_model, (void *)GameBuild);
            out_model = out_model->Next;
        }
    }

    FirstShellModel = num_ed_models;
    pShellPSX = PSXRegion[ShellRegion].pPSX;
    NumShellModels = pShellPSX[2];
    ppShellModels =
        (SModel **)((Item *)(pShellPSX + 3) + NumShellModels) + 1;
    pShellPacket = (u32 *)((char *)pShellPSX + pShellPSX[1]);
    shell_model_data_size = (char *)pShellPacket - (char *)ppShellModels[0];
    num_models = num_ed_models + NumShellModels;
    model_data_size += shell_model_data_size;
    item_table_size = num_models * sizeof(Item);
    model_table_size = num_models * 4 + 4;

    int packet_data_size = CreateZonePacket(0) + 4;

    u32 *pSourceTextures =
        Spool_SkipPackets(pSourcePSX) + NumSourceModels + 1;
    u32 num_ed_textures = pSourceTextures[-1];
    u32 *pShellTextures =
        Spool_SkipPackets(pShellPSX) + NumShellModels + 1;
    u32 num_shell_textures = pShellTextures[-1];
    u32 num_textures = num_ed_textures + num_shell_textures;

    int texture_checksum_size = num_textures * 4 + 4;
    item_table_size += 12;

    u32 *pHeader = (u32 *)Mem_New(
        item_table_size + model_table_size + model_data_size +
            packet_data_size + num_models * 4 +
            texture_checksum_size,
        1, 1);

    pHeader[0] = 0x20004;
    pHeader[1] = item_table_size + model_table_size + model_data_size;
    pHeader[2] = num_models;

    Item *pSourceItem = (Item *)(pSourcePSX + 3);
    Item *pItem = (Item *)(pHeader + 3);
    int i;

    i = 0;
    for (int which_list = 0; which_list < 2; which_list++)
    {
        if (which_list == 0)
        {
            out_model = OutModelList;
        }
        else
        {
            out_model = FloorModelList;
        }

        while (out_model != 0)
        {
            int source_index;

            pItem[i].Flags = 0;
            pItem[i].Inquiry = 0;
            pItem[i].TintIndex = 0;
            pItem[i].RGB = 0;
            pItem[i].Angles.vx = 0;
            pItem[i].Angles.vy = 0;
            pItem[i].Angles.vz = 0;
            pItem[i].Model = i;
            pItem[i].Region = (u8)LevelGenerator::OutRegion;
            pItem[i].NextItem = &pItem[i + 1];
            pItem[i].Position.vx = out_model->X << 12;
            pItem[i].Position.vz = out_model->Z << 12;
            pItem[i].Position.vy = out_model->Y << 12;

            source_index = out_model->SourceModel->mIndex;
            pItem[i].Frame = pSourceItem[source_index].Frame;
            pItem[i].Anim = pSourceItem[source_index].Anim;
            out_model->TargetItem = &pItem[i];

            out_model = out_model->Next;
            i++;
        }
    }

    Item *pShellItems = (Item *)(pShellPSX + 3);
    {
        int i;
        for (i = 0; i < NumShellModels; i++)
        {
            int item_num = num_ed_models + i;
            pItem[item_num] = pShellItems[i];
            pItem[item_num].Model = item_num;
            pItem[item_num].Region = (u8)LevelGenerator::OutRegion;
            pItem[item_num].NextItem = &pItem[item_num + 1];
        }
    }

    pItem[num_ed_models + NumShellModels - 1].NextItem = 0;
    u32 *pModelTable = (u32 *)(pItem + num_models) + 1;
    pModelTable[-1] = num_models;
    char *pModelData = (char *)(pModelTable + num_models);

    i = 0;
    for (int which_list = 0; which_list < 2; which_list++)
    {
        if (which_list == 0)
        {
            out_model = OutModelList;
        }
        else
        {
            out_model = FloorModelList;
        }

        while (out_model != 0)
        {
            pModelTable[i] = (u32)pModelData;
            LevelGen_CreateEditorModelData(
                out_model, pModelData, (void *)GameBuild);
            pModelData += out_model->Size;
            out_model = out_model->Next;
            i++;
        }
    }

    {
        int i;
        for (i = 0; i < NumShellModels; i++)
        {
            int model_num = num_ed_models + i;
            int model_size;

            pModelTable[model_num] = (u32)pModelData;
            if (i == NumShellModels - 1)
            {
                model_size = (char *)pShellPacket - (char *)ppShellModels[i];
            }
            else
            {
                model_size = (char *)ppShellModels[i + 1] -
                             (char *)ppShellModels[i];
            }
            Mem_Copy(pModelData, ppShellModels[i], model_size);
            pModelData += model_size;
        }
    }

    CreateZonePacket((int *)pModelData);
    u32 *pModelChecksums =
        (u32 *)(((packet_data_size >> 2) << 2) + (u32)pModelData);
    pModelChecksums[-1] = 0xffffffff;

    u32 *pTexture = pModelChecksums + num_models + 1;
    pTexture[-1] = num_textures;
    {
        int i;
        for (i = 0; (u32)i < num_ed_textures; i++)
        {
            LevelGenTextureUsageView *texture =
                (LevelGenTextureUsageView *)pSourceTextures[i];
            pTexture[i] = (u32)texture;
            texture->Usage++;
        }
    }
    {
        int i;
        for (i = 0; (u32)i < num_shell_textures; i++)
        {
            int num = num_ed_textures + i;
            LevelGenTextureUsageView *texture =
                (LevelGenTextureUsageView *)pShellTextures[i];
            pTexture[num] = (u32)texture;
            texture->Usage++;
        }
    }

    PSXRegion[LevelGenerator::OutRegion].pPSX = pHeader;
    PSXRegion[LevelGenerator::OutRegion].ppModels =
        (SModel **)pModelTable;
    PSXRegion[LevelGenerator::OutRegion].pModelChecksums =
        pModelChecksums;
    PSXRegion[LevelGenerator::OutRegion].TexturesInRAM = 1;
    PSXRegion[LevelGenerator::OutRegion].IsSuper = 0;
    PSXRegion[LevelGenerator::OutRegion].pTexWibData = 0;
    PSXRegion[LevelGenerator::OutRegion].pColourPulseData = 0;
    PSXRegion[LevelGenerator::OutRegion].Usable = 1;
    PSXRegion[LevelGenerator::OutRegion].WhichHeap = 1;

    if (PSXRegion[LevelGenerator::OutRegion].pColourTable != 0)
    {
        Mem_Delete(PSXRegion[LevelGenerator::OutRegion].pColourTable);
    }
    {
        int i;

        u32 *const new_colour_table =
            (u32 *)Mem_New(0x400, 1, 1);
        const int out_region_index = LevelGenerator::OutRegion;
        PSXRegion[out_region_index].pColourTable = new_colour_table;
        for (i = 0; i < 0x100; i++)
        {
            PSXRegion[out_region_index].pColourTable[i] =
                i * 0x10101;
        }
    }

    pColourTable = PSXRegion[LevelGenerator::OutRegion].pColourTable;
    EnvRegions[0] = LevelGenerator::OutRegion;
    EnvRegions[1] = -1;
    EnviroList = (Item *)(PSXRegion[LevelGenerator::OutRegion].pPSX + 3);
    EnvModelChecksums[0] = pModelChecksums;
    M3dZone_SetZone(0, (u32 *)(pModelData + 8));

    if (GameBuild != 0)
    {
        CreateRails();
    }
    HideFloorPieces();

    return pHeader;
}

/**
 * CreateZonePacket__14LevelGeneratorPi
 *
 * Address:   0x80058E3C
 * Mangled:   CreateZonePacket__14LevelGeneratorPi
 * Demangled: LevelGenerator::CreateZonePacket(int *)
 *
 * @param at Optional output buffer; null performs the sizing pass only.
 * @return Number of bytes required by the zone packet.
 */
int LevelGenerator::CreateZonePacket(int *at)
{
    /*
     * Two passes in one body: called with `at == 0` it only counts the entries
     * so the caller can size the buffer, and called with a real pointer it
     * allocates the 20x20 bucket table, fills it, and emits the packet.
    */
    bool actual_create = at != 0;
    int i;

    if (actual_create)
    {
        ZoneTab = (ZoneCell **)Mem_New(
            20 * sizeof(ZoneCell *), 1, 1);
        for (i = 0; i < 20; i++)
        {
            ZoneTab[i] = (ZoneCell *)Mem_New(
                20 * sizeof(ZoneCell), 1, 1);
        }
    }

    int zone_length = (_14LevelGenerator_GridCellWidth * 24) / 20;

    if (actual_create)
    {
        for (int j = 0; j < 20; j++)
        {
            for (int i = 0; i < 20; i++)
            {
                ZoneTab[j][i].num = 0;
            }
        }
    }

    int zone_item_count = 0;
    int ed_item_index = 0;

    /* Pass one over the placed models, then over the floor pieces. */
    for (i = 0; i < 2; i++)
    {
        EditorOutModel *model =
            (i == 0) ? OutModelList : FloorModelList;
        for (; model != 0; model = model->Next)
        {
            int zz1 = 20 - (model->ZCell * _14LevelGenerator_GridCellWidth) / zone_length;
            int ix0 = (model->XCell * _14LevelGenerator_GridCellWidth) / zone_length;
            int ix1 = ((model->XCell + model->WCell) * _14LevelGenerator_GridCellWidth +
                       zone_length - 1) / zone_length;
            int zz0 = 18 - ((model->ZCell + model->LCell) * _14LevelGenerator_GridCellWidth +
                            zone_length - 1) / zone_length;

            for (int zx = ix0; zx < ix1; zx++)
            {
                for (int zz = zz0; zz < zz1; zz++)
                {
                    if (zx >= 0 && zz >= 0 && zx < 20 && zz < 20)
                    {
                        if (actual_create)
                        {
                            ZoneTab[zz][zx].item[ZoneTab[zz][zx].num++] =
                                (unsigned short)ed_item_index;
                        }
                        zone_item_count++;
                    }
                }
            }
            ed_item_index++;
        }
    }

    /* Pass two over the shell models, which carry their own bounding boxes. */
    u32 *pShellPSX = PSXRegion[ShellRegion].pPSX;
    int start_x = -(_14LevelGenerator_GridCellWidth * 12);
    SModel **ppShellModels =
        (SModel **)((Item *)(pShellPSX + 3) + NumShellModels) + 1;
    for (i = 0; i < NumShellModels; i++)
    {
        int *entry = (int *)(pShellPSX + i * 9 + 4);

        int x0 = (entry[0] >> 12) +
                 ((LevelGenModelBounds *)&ppShellModels[i]->Box)->half[1] - start_x;
        int x1 = (entry[0] >> 12) +
                 ((LevelGenModelBounds *)&ppShellModels[i]->Box)->half[0] - start_x;
        int z0 = (entry[2] >> 12) +
                 ((LevelGenModelBounds *)&ppShellModels[i]->Box)->half[5] - start_x;
        int z1 = (entry[2] >> 12) +
                 ((LevelGenModelBounds *)&ppShellModels[i]->Box)->half[4] - start_x;

        int zx0 = x0 / zone_length;
        if (zx0 < 0)
            zx0 = 0;
        if (zx0 > 19)
            zx0 = 19;
        int zx1 = x1 / zone_length;
        if (zx1 < 0)
            zx1 = 0;
        if (zx1 > 19)
            zx1 = 19;
        int zz0 = z0 / zone_length;
        if (zz0 < 0)
            zz0 = 0;
        if (zz0 > 19)
            zz0 = 19;
        int zz1 = z1 / zone_length;
        if (zz1 < 0)
            zz1 = 0;
        if (zz1 > 19)
            zz1 = 19;

        for (int zx = zx0; zx <= zx1; zx++)
        {
            for (int zz = zz0; zz <= zz1; zz++)
            {
                if (actual_create)
                {
                    ZoneTab[zz][zx].item[ZoneTab[zz][zx].num++] =
                        (unsigned short)(i + ed_item_index);
                }
                zone_item_count++;
            }
        }
    }

    ZoneBytes = zone_item_count * sizeof(int) +
                20 * 20 * 4 * sizeof(int) +
                sizeof(LevelGenZonePacketHeader);

    if (!actual_create)
    {
        return ZoneBytes;
    }

    at[0] = 10;
    at[1] = ZoneBytes - 8;
    at[2] = -(_14LevelGenerator_GridCellWidth * 0xC000);
    at[3] = at[2];
    at[4] = _14LevelGenerator_GridCellWidth * 0xC000;
    at[5] = at[4];
    ((short *)at)[12] = 20;
    ((short *)at)[13] = 20;

    /* Each cell reserves two words before its count; the count and trailing
     * zero make the four fixed words included in ZoneBytes. */
    int index = sizeof(LevelGenZonePacketHeader) / sizeof(int);
    for (i = 0; i < 20; i++)
    {
        for (int zx = 0; zx < 20; zx++)
        {
            index += 2;
            at[index++] = ZoneTab[i][zx].num;
            for (int n = 0; n < ZoneTab[i][zx].num; n++)
            {
                at[index++] = ZoneTab[i][zx].item[n];
            }
            at[index++] = 0;
        }
    }

    for (i = 0; i < 20; i++)
    {
        Mem_Delete(ZoneTab[i]);
    }
    Mem_Delete(ZoneTab);

    return ZoneBytes;
}

// ====== 0x8005946C CreateRails__14LevelGenerator ===============================
// CreateRails__14LevelGenerator
// Address: 0x8005946C
// Demangled: LevelGenerator::CreateRails(void)
void LevelGenerator::CreateRails(void)
{
    OutRails.Clear();
    register EditorOutModel *model asm("$17") = OutModelList;
    while (model != 0)
    {
        EditorInModel *in_model = model->SourceModel;
        RailString *in_string = in_model->railInfo->StringList;
        goto inner_test;
    inner_loop:
    {
        RailString *out_string =
            static_cast<RailString *>(
                ::CClass::operator new(sizeof(RailString)));
        if (out_string != 0)
        {
            out_string = ::new (out_string) RailString();
        }
        out_string->CopyString(
            in_string, model->X, model->Y, model->Z, model->Rot);
        OutRails.MergeAdd(out_string, false);
        in_string = in_string->Next;
    }
    inner_test:
        if (in_string != 0)
            goto inner_loop;
        model = model->Next;
    }
}

/**
 * GenerateRailNode__FPsP9RailPointT1i
 *
 * Address:   0x80059550
 * Mangled:   GenerateRailNode__FPsP9RailPointT1i
 * Demangled: GenerateRailNode(short *, RailPoint *, RailPoint *, int)
 *
 * @param pData  Data to operate on.
 * @param point Rail point whose coordinates and material are serialized.
 * @param next_point Optional linked point; controls whether a link is emitted.
 * @param next_node Trigger-node index written for that link.
 * @return Serialized record length in 16-bit words.
 */
int GenerateRailNode(short *pData, RailPoint *point, RailPoint *next_point, int next_node)
{
    register int pad asm("$3") = ((int)pData & 3) != 0;
    int num_links = next_point != 0;
    pad = (pad + num_links) & 1;
    asm volatile("" : "+r"(pad));
    register int v0 asm("$2") = pad;
    v0 += 2;
    v0 = num_links + v0;
    int size = v0 + 11;

    if (!pData)
        return size;

    int n = 2;
    pData[0] = 10;
    pData[1] = num_links;

    if (num_links)
    {
        pData[2] = next_node;
        n = 3;
    }

    register int posOff asm("$5") = n << 1;
    if (pad)
    {
        n++;
        posOff = n << 1;
    }
    n += 6;
    int *pos = (int *)(posOff + (int)pData);

    pos[0] = point->Pos.vx;
    pos[1] = point->Pos.vy;
    pos[2] = point->Pos.vz;

    pData[n] = 0;
    n++;
    pData[n] = 0;
    n++;
    pData[n] = 0;
    n++;
    pData[n] = (unsigned short)point->Material;
    n++;
    pData[n] = -1;

    return size;
}

/**
 * Builds the trigger-file image used to expose generated rails to the trigger
 * system.
 *
 * Address:   0x8005962C
 * Mangled:   FakeTrig__14LevelGenerator
 * Demangled: LevelGenerator::FakeTrig(void)
 */
void LevelGenerator::FakeTrig(void)
{
    OutRails.CountStrings();
    int num_rail_points = OutRails.CountPoints();
    int rail_nodes_size = 0;

    RailString *string = OutRails.StringList;
    for (; string != 0; string = string->Next)
    {
        RailPoint *point = string->PointList;
        int i = 0;
        if (string->NumPoints > 0)
        {
            do
            {
                rail_nodes_size += GenerateRailNode(
                    0, point, point->Next, 0);
                point = point->Next;
                i++;
            } while (i < string->NumPoints);
        }
    }

    rail_nodes_size *= sizeof(short);
    NumNodes = num_rail_points + 2;
    TrigFile = (unsigned short *)Mem_New(
        rail_nodes_size + NumNodes * sizeof(short *) +
            sizeof(LevelGenTriggerFileHeader) +
            sizeof(LevelGen_FakeTriggerNode) + 4 * sizeof(short),
        1, 1);
    unsigned short *trigFile = TrigFile;
    LevelGenTriggerFileHeader *trig_header =
        (LevelGenTriggerFileHeader *)trigFile;

    *(unsigned int *)trigFile = 0x4752545f;
    unsigned short nodeCount = (unsigned short)NumNodes;
    trig_header->Version = 2;
    trig_header->NumNodes = nodeCount;

    OffsetList = (short **)(trigFile + 6);
    short *pNode = (short *)(OffsetList + NumNodes);
    OffsetList[0] = pNode;
    pNode[0] = 8;
    pNode[1] = 0;
    pNode += 2;
    int coordinate = 0xaa;
    if (((unsigned int)pNode & 3) != 0)
    {
        pNode++;
    }

    LevelGen_FakeTriggerNode *header = (LevelGen_FakeTriggerNode *)pNode;
    header->y = -2000;
    header->x = coordinate;
    header->z = coordinate;
    header->angleY = 0x800;
    char *pNodeName = header->name;
    pNodeName[0] = 'a';
    header->angleX = 0;
    header->angleZ = 0;
    pNodeName[1] = 'c';
    pNodeName[2] = 'k';
    pNodeName[3] = 0;
    header->nextNode = -1;
    pNode += sizeof(LevelGen_FakeTriggerNode) / sizeof(short);

    string = OutRails.StringList;
    int node_count = 1;
    for (; string != 0; string = string->Next)
    {
        RailPoint *point = string->PointList;
        int i = 0;
        int first_node = node_count;
        if (string->NumPoints > 0)
        {
            do
            {
                OffsetList[node_count] = pNode;

                int size;
                if ((i < string->NumPoints - 1) || !string->IsLoop)
                {
                    node_count++;
                    size = GenerateRailNode(
                        pNode, point, point->Next, node_count);
                }
                else
                {
                    size = GenerateRailNode(
                        pNode, point, point->Next, first_node);
                    node_count++;
                }

                i++;
                point = point->Next;
                pNode += size;
            } while (i < string->NumPoints);
        }
    }

    OffsetList[NumNodes - 1] = pNode;
    *pNode = 0xff;
    NumCheatRestarts = 0;
    NumHorseRestarts = 0;
    RestartNode = 0;
}

/**
 * Updates shared faces on every occupied cell around a model's perimeter.
 *
 * Address:   0x800598B8
 * Mangled:   UnhideFaces__14LevelGeneratorP14EditorOutModelb
 * Demangled: LevelGenerator::UnhideFaces(EditorOutModel *, bool)
 *
 * @param das_model  Placed model whose perimeter neighbors are updated.
 * @param on  Whether the shared faces should be exposed.
 */
void LevelGenerator::UnhideFaces(EditorOutModel *das_model, bool on)
{
    if ((das_model->SourceModel->flags & 1) != 0)
    {
        return;
    }

    int m0 = das_model->XCell;
    int m1 = m0 + das_model->WCell;
    int n0 = das_model->ZCell;
    int n1 = n0 + das_model->LCell;

    for (int s = das_model->YCell; s < 5; s++)
    {
        int m;
        int n;

        m = m0 - 1;
        if (m >= 0)
        {
            for (n = n0; n < n1; n++)
            {
                if (OutModelMap[n][m].modelInSlot[s] != 0)
                {
                    OutModelMap[n][m].modelInSlot[s]->UnhideSharedFaces(
                        on, das_model, 0);
                }
            }
        }

        m = m1;
        if (m < 24)
        {
            for (n = n0; n < n1; n++)
            {
                if (OutModelMap[n][m].modelInSlot[s] != 0)
                {
                    OutModelMap[n][m].modelInSlot[s]->UnhideSharedFaces(
                        on, das_model, 2);
                }
            }
        }

        n = n0 - 1;
        if (n >= 0)
        {
            for (m = m0; m < m1; m++)
            {
                if (OutModelMap[n][m].modelInSlot[s] != 0)
                {
                    OutModelMap[n][m].modelInSlot[s]->UnhideSharedFaces(
                        on, das_model, 1);
                }
            }
        }

        n = n1;
        if (n < 24)
        {
            for (m = m0; m < m1; m++)
            {
                if (OutModelMap[n][m].modelInSlot[s] != 0)
                {
                    OutModelMap[n][m].modelInSlot[s]->UnhideSharedFaces(
                        on, das_model, 3);
                }
            }
        }
    }
}

/**
 * Toggles visibility flags on the floor pieces beneath a placed model.
 *
 * Address:   0x80059B2C
 * Mangled:   UnhideFloor__14LevelGeneratorP14EditorOutModelb
 * Demangled: LevelGenerator::UnhideFloor(EditorOutModel *, bool)
 *
 * @param das_model  Placed model defining the covered floor cells.
 * @param unhide  Whether to expose instead of hide the floor pieces.
 */
void LevelGenerator::UnhideFloor(EditorOutModel *das_model, bool unhide)
{
    int slot = das_model->YCell;
    int z0 = das_model->ZCell;
    int z1 = z0 + das_model->LCell;
    int x0 = das_model->XCell;
    int x1 = x0 + das_model->WCell;

    if (((das_model->SourceModel->flags & 4) != 0) && (slot != 0))
    {
        for (int z = z0; z < z1; z++)
        {
            for (int x = x0; x < x1; x++)
            {
                EditorOutModel *floor = FloorModelMap[z][x].modelInSlot[slot];
                if (floor != 0)
                {
                    if (!unhide)
                    {
                        floor->TargetModel->Flags |= 0x20;
                        floor->TargetModel->Flags |= 2;
                    }
                    else
                    {
                        floor->TargetModel->Flags &= 0xffdf;
                        floor->TargetModel->Flags &= 0xfffd;
                    }
                }
            }
        }
    }
}

/**
 *
 * Address:   0x80059C40
 * Mangled:   HideWalls__14LevelGeneratorb
 * Demangled: LevelGenerator::HideWalls(bool)
 *            Toggle wall visibility bits in the loaded PSX wall list.
 *
 * @param hide Set wall-model hidden flags when true; clear them otherwise.
 */
void LevelGenerator::HideWalls(bool hide)
{
    int num_models = FirstShellModel + NumShellModels;
    u32 *pHeader = PSXRegion[LevelGenerator::OutRegion].pPSX;
    u32 *model_table_base = pHeader + num_models * 9 + 4;
    int i = FirstShellModel;

    if (i < num_models)
    {
		register int offset asm("$2") = i * 4;
        register u32 *pModelTable asm("$4") =
            (u32 *)(offset + (int)model_table_base);
        SModel *wall;
        do
        {
            if (hide)
            {
                wall = (SModel *)*pModelTable;
                wall->Flags |= 0x20;
            }
            else
            {
                wall = (SModel *)*pModelTable;
                wall->Flags &= 0xffdf;
            }
            i++;
            pModelTable++;
        } while (i < num_models);
    }
}

/**
 * AddModel__14LevelGeneratoriiiii — Place a model into the level grid
 * Places a mesh piece into the spatial grid at the given cell coordinates,
 * with one of 4 rotation variants (90° increments). Creates an EditorOutModel
 * from the corresponding EditorInModel and links it to the model list.
 * GridCell::modelIndex stores the selected piece and GridCell::rot stores its
 * quarter-turn rotation for each occupied vertical slot.
 * OutModelMap records the resulting EditorOutModel across every cell and
 * vertical slot covered by its WCell, LCell, and HCell footprint.
 * Grid is centered at (12, 12): world X = (gridX - 12) * cellWidth + offsetX
 *                                world Z = (12 - gridZ) * cellWidth - offsetZ
 *                                world Y = -gridY * cellHeight
 * Rotation selects the corresponding EditorInModel width/length and bound
 * offsets before the output model is linked through Next.
 *
 * Address:   0x80059CE0, Size: 788 bytes (197 instructions)
 * Mangled:   AddModel__14LevelGeneratoriiiii
 * Demangled: void LevelGenerator::AddModel(int, int, int, int, int)
 *
 * @param x  Grid X coordinate.
 * @param z  Grid Z coordinate.
 * @param slot  Vertical grid slot.
 * @param rot  Quarter-turn rotation index.
 * @param index  Source-model index to place.
 */

void LevelGenerator::AddModel(int x, int z, int slot, int rot, int index)
{
    EditorInModel *in_model;
    EditorOutModel *out_model;
    int xoff;
    int zoff;
    int xworld;
    int zworld;
    int yworld;
    _14LevelGenerator_GridTab[z][x].modelIndex[slot] = (short)index;
    in_model = &SourceModelTab[index];
    _14LevelGenerator_GridTab[z][x].rot[slot] = (unsigned char)rot;

    out_model = static_cast<EditorOutModel *>(
        ::CClass::operator new(sizeof(EditorOutModel)));
    if (out_model != 0)
    {
        out_model = ::new (out_model) EditorOutModel(in_model, 0);
    }

    if (OutModelList == 0)
    {
        OutModelList = out_model;
    }
    else
    {
        EditorOutModel *last_model = OutModelList;
        while (last_model->Next != 0)
        {
            last_model = last_model->Next;
        }
        last_model->Next = out_model;
    }

    switch (rot)
    {
    case 0:
        xoff = in_model->xleft;
        zoff = in_model->ztop;
        out_model->WCell = in_model->w;
        out_model->LCell = in_model->l;
        break;
    case 1:
        xoff = in_model->ztop;
        zoff = in_model->xright;
        out_model->WCell = in_model->l;
        out_model->LCell = in_model->w;
        break;
    case 2:
        xoff = in_model->xright;
        zoff = in_model->zbottom;
        out_model->WCell = in_model->w;
        out_model->LCell = in_model->l;
        break;
    case 3:
        xoff = in_model->zbottom;
        zoff = in_model->xleft;
        out_model->WCell = in_model->l;
        out_model->LCell = in_model->w;
        break;
    }

    out_model->HCell = in_model->h;
    xworld = (x - 12) * _14LevelGenerator_GridCellWidth + xoff;
    yworld = -slot * _14LevelGenerator_GridCellHeight;
    zworld = (12 - z) * _14LevelGenerator_GridCellWidth - zoff;
    out_model->XCell = x;
    out_model->YCell = slot;
    out_model->ZCell = z;
    out_model->X = (short)xworld;
    out_model->Y = (short)yworld;
    out_model->Z = (short)zworld;
    out_model->Rot = _14LevelGenerator_GridTab[z][x].rot[slot];

    for (int n = z; n < z + out_model->LCell; n++)
    {
        for (int m = x; m < x + out_model->WCell; m++)
        {
            for (int s = 0; s < out_model->HCell; s++)
            {
                OutModelMap[n][m].modelInSlot[slot + s] = out_model;
            }
        }
    }
}

/**
 * KillModel__14LevelGeneratoriii
 *
 * Address:   0x80059FF4
 * Mangled:   KillModel__14LevelGeneratoriii
 * Demangled: LevelGenerator::KillModel(int, int, int)
 *            Clear a model slot and invalidate the matching grid entry.
 *
 * @param x  X coordinate.
 * @param z  Z coordinate.
 * @param slot Vertical occupancy slot to clear.
 */
void LevelGenerator::KillModel(int x, int z, int slot)
{
    int xSlotOffset = x * 5 + slot;
    xSlotOffset <<= 2;
    int zOffset = z * 15;
    zOffset <<= 5;
    xSlotOffset += zOffset;
    EditorOutModel **slotPtr = (EditorOutModel **)&OutModelMap[0][0];
    slotPtr = (EditorOutModel **)((char *)slotPtr + xSlotOffset);
    asm volatile("" : "+r"(slotPtr));
    register int one asm("$2") = 1;
    register char *grid asm("$9") =
        (char *)_14LevelGenerator_GridTab;
    asm volatile("" : "+r"(grid));
    int gridOffset = slot << 1;
    int xGridOffset = x << 4;
    gridOffset += xGridOffset;
    register EditorOutModel *model asm("$8") = *slotPtr;
    register int negOne asm("$3") = -1;
    model->DeleteModelRequest = (void *)one;
    register int zGridOffset asm("$2") = z * 3;
    zGridOffset <<= 7;
    gridOffset += zGridOffset;
    gridOffset += (int)grid;
    *slotPtr = 0;
    *(short *)gridOffset = negOne;
}

// GCC emitted this private `CClass::~CClass` copy last in the owner's text, so
// it stays at the end of the translation unit.
static void __DOT__6CClass_8005A060(CClass *self, int __in_chrg)
{
    self->vtable = &LevelGen_BaseVTable;
    if (__in_chrg & 1)
    {
        ::CClass::operator delete(self);
    }
}
