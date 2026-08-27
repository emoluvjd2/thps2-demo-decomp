// GENOBJS.cpp -- Reconstructed translation unit
// Original: GENOBJS.CPP
// Functions: 37 reconstructed bodies
// Address range: 0x80052D70 - 0x80056020
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#define __SKIP_RAILSTRING_EXTERNS__

#define __SKIP_EDITOROUTMODEL_EXTERNS__

#define __SKIP_RAILPOINT_EXTERNS__

#include "GENOBJS.h"
#include "RAILINFO.h"
#include "EDITORPIECES.h"
#include "FACEREMOVAL.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include "SMODEL.h"

/** Return caller-provided storage for an owner constructor. */
inline void *operator new(unsigned int, void *address)
{
    return address;
}

/** Packed point used by generated model vertices and normals. */
struct ModelPoint
{
    s16 vx;
    s16 vy;
    s16 vz;
    u16 pad;
};

/** Signed axis extrema overlaid on an SModel bounding box. */
struct ModelBounds
{
    s16 xmax;
    s16 xmin;
    s16 ymax;
    s16 ymin;
    s16 zmax;
    s16 zmin;
};

/** Source face layout recorded by GENOBJS debug metadata. */
struct ModelFace
{
    char vertexNumber[4];
    SVector verts[5];
    signed char orientation;
    char numVerts;
    u8 outputSize;
    ModelFace *next;
};

/** Visibility record paired with a source model face. */
struct ModelFace2
{
    int area;
    void *alive;
    signed char orientation;
    ModelFace *sourceFace;
    ModelFace2 *next;
};

/** Header shared by every variable-length face in generated model data. */
struct GenObjsOutputFaceHeader
{
    u16 flags;
    u16 size;
};

typedef char GenObjs_ModelPoint_size_must_be_8[(sizeof(ModelPoint) == 8) ? 1 : -1];
typedef char GenObjs_ModelBounds_size_must_be_12[(sizeof(ModelBounds) == 12) ? 1 : -1];
typedef char GenObjs_ModelFace_size_must_be_44[(sizeof(ModelFace) == 44) ? 1 : -1];
typedef char GenObjs_ModelFace2_size_must_be_20[(sizeof(ModelFace2) == 20) ? 1 : -1];
typedef char GenObjs_OutputFaceHeader_size_must_be_4[
    (sizeof(GenObjsOutputFaceHeader) == 4) ? 1 : -1];

/** Owner-local layout of a point in a generated rail string. */
struct GenObjsRailPoint
{
    CClass CClass;
    SVector Pos;
    GenObjsRailPoint *Next;
    int ModelIndex;
    int Material;
    void *IsDoubleLink;
};

/** Owner-local layout of a generated rail string. */
struct GenObjsRailString
{
    CClass CClass;
    SVector BoundsMin;
    SVector BoundsMax;
    GenObjsRailPoint *PointList;
    GenObjsRailString *Next;
    void *IsLoop;
    int NumPoints;
};

/** Owner-local layout of the generated rail list. */
struct GenObjsRailInfo
{
    CClass CClass;
    GenObjsRailString *StringList;
    GenObjsRailString *CurrentString;
};

/** Owner-local slice of EditorInModel used while restoring shared faces. */
struct GenObjsEditorInModel
{
    CClass CClass;
    ModelFace *Faces;
    SModel *mModel;
    u8 mIndex;
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
    void *railInfo;
    u8 flags;
};

/** Owner-local slice of EditorOutModel used while restoring shared faces. */
struct GenObjsEditorOutModel
{
    CClass CClass;
    void *DeleteModelRequest;
    u8 FaceRemovalRequest;
    void *RestoreFacesRequest;
    u8 RestoreFacesSide;
    u16 Size;
    u16 NumVertices;
    u16 NumNormals;
    u16 NumFaces;
    u8 NumNeighbors;
    u8 mFirstEasternNeighbor;
    GenObjsEditorOutModel *Neighbors[16];
    ModelFace2 *Faces;
    u8 XCell;
    u8 YCell;
    u8 ZCell;
    u8 WCell;
    u8 HCell;
    u8 LCell;
    s16 X;
    s16 Y;
    s16 Z;
    u8 Rot;
    GenObjsEditorInModel *SourceModel;
    SModel *TargetModel;
    Item *TargetItem;
    GenObjsEditorOutModel *Next;
};

typedef char GenObjs_RailPoint_size_must_be_28[
    (sizeof(GenObjsRailPoint) == 28) ? 1 : -1];
typedef char GenObjs_RailString_layout_size_must_be_32[
    (sizeof(GenObjsRailString) == 32) ? 1 : -1];
typedef char GenObjs_RailInfo_size_must_be_12[
    (sizeof(GenObjsRailInfo) == 12) ? 1 : -1];
typedef char GenObjs_EditorInModel_size_must_be_68[
    (sizeof(GenObjsEditorInModel) == 68) ? 1 : -1];
typedef char GenObjs_EditorOutModel_size_must_be_128[
    (sizeof(GenObjsEditorOutModel) == 128) ? 1 : -1];
typedef char GenObjs_SDK_SVECTOR_size_must_be_8[
    (sizeof(SVECTOR) == 8) ? 1 : -1];

/** Read MAIN.SYM's scalar POLYBUFFERSIZE without adding a synthetic import. */
static inline int GenObjsPolyBufferSize(void)
{
    return *(int *)0x800DA328;
}

/** Typed imports used by the generated-model owner. */
extern "C"
{
GenObjsEditorInModel *ConstructEditorInModel(GenObjsEditorInModel *model)
    asm("__13EditorInModel");
GenObjsRailInfo *ConstructRailInfo(GenObjsRailInfo *rails)
    asm("__8RailInfo");
}

#include "MEM.h"
#include <STDIO.H>
#include <INLINE_C.H>

/**
 * Invoke a CClass-derived object's virtual destructor: vtable slot 2 with
 * the GCC 2.x in-charge/deleting flag 3 (destroy and operator delete).
 */
static inline void DestroyCClass(void *obj)
{
    int *vtable = ((CClass *)obj)->vtable;
    typedef void (*dtor_fn)(void *, int);
    ((dtor_fn)vtable[2])(obj, 3);
}

// === Forward type declarations ===

struct GenObjsAssignableRailString;

/** One point in a generated rail string with its original CClass base. */
class RailPoint : public CClass
{
public:
    SVector Pos;
    RailPoint *Next;
    int ModelIndex;
    int Material;
    void *IsDoubleLink;

    RailPoint();
    ~RailPoint();
    bool Compare(RailPoint *point);
};

typedef char GenObjs_RailPoint_class_size_must_be_28[
    (sizeof(RailPoint) == 28) ? 1 : -1];

/** Generated rail string with its original CClass base and linked-list fields. */
class RailString : public CClass
{
public:
    SVector BoundsMin;
    SVector BoundsMax;
    GenObjsRailPoint *PointList;
    RailString *Next;
    void *IsLoop;
    int NumPoints;

    void AddPoint(RailPoint *point, bool is_loop);
    void MakeCCW(void);
    void CopyString(RailString *from, int xoff, int yoff, int zoff, int rot);
    bool MergeString(RailString *string_from, bool debugify);
    RailString *SplitString(void);
    int CountPoints(void);
    RailString();
    ~RailString();
};

typedef char GenObjs_RailString_size_must_be_32[
    (sizeof(RailString) == 32) ? 1 : -1];

/**
 * Display-buffer slice needed by the editor's polygon-pool initializer.
 *
 * `MAIN_STRUCTS.h` proves `Polys` at offset 0xA8; the rest of the display
 * buffer remains deliberately opaque to this owner.
 */
struct GenObjsDoubleBufferView
{
    unsigned char _padToPolys[0xA8];
    unsigned char *Polys;
};

// === Class declarations ===
class EditorInModel
{
public:
    static int GameBuild;
    static ModelFace *pMaxBufEntry;
    static ModelFace *pPolyBufEntry;
    static void InitPolyBuf(bool game_build);
    static ModelFace *NewModelFace(void);
    ~EditorInModel();
    static void operator delete(void *p) {
        ::CClass::operator delete(p);
    }
    void KillModelInfo(void);
    void SetData(SModel *pModel, int model_index);
    void SetTransparency(bool on);
};

class EditorOutModel
{
public:
    static unsigned char *MixLookup;
    static unsigned char *IntensityLookup;
    static int FallOff;
    static int LightIntensity;
    static int DaMix;
    static int CurrentFace;
    static ModelFace2 *FaceBuf;
    static Vector LightVect[4];
    static Vector LightLoc[4];
    static void Init(void);
    void Cleanup(void);
    static ModelFace2 *NewFace(void);
    EditorOutModel(EditorInModel *source, int id);
    ~EditorOutModel();
    void RestoreFaces(void);
    void RemoveSomeHiddenFaces(EditorOutModel *neighbor, int orientation);
    void RemoveAllHiddenFaces(void);
    int SetModelDataSize(bool game_build);
    int CreateModelData(char *pData, bool game_build);
    void UnhideSharedFaces(bool unhide, EditorOutModel *wire_model, unsigned char against);
};

/** World-space directions for the editor's four static model lights. */
Vector EditorOutModel::LightVect[4]
    __attribute__((section(".data"))) = {};

/** World-space positions for the editor's four static model lights. */
Vector EditorOutModel::LightLoc[4]
    __attribute__((section(".data"))) = {};

extern "C" {

/** Rail-merge diagnostics recovered from the original small-data block. */
static char s_From_empty_800da584[12]
    __attribute__((section(".sdata"))) = "From empty\n";
static char s_To_empty_800da590[12]
    __attribute__((section(".sdata"))) = "To empty\n";
static char s_from_not_loop_800da59c[16]
    __attribute__((section(".sdata"))) = "from not loop\n";
static char s_To_not_loop_800da5ac[16]
    __attribute__((section(".sdata"))) = "To not loop\n";

/** Editor merge diagnostics in their original small-data order. */
static char s_Added_string_800da5bc[16]
    __attribute__((section(".sdata"))) = " Added string\n";
static char s_Merged_string_800da5cc[16]
    __attribute__((section(".sdata"))) = " Merged string\n";

/** EditorOutModel's target-owned small-data state, in address order. */
unsigned char *EditorOutModel::MixLookup
    __attribute__((section(".sdata"))) = 0;
unsigned char *EditorOutModel::IntensityLookup
    __attribute__((section(".sdata"))) = 0;
int EditorOutModel::FallOff __attribute__((section(".sdata"))) = 0;
int EditorOutModel::LightIntensity __attribute__((section(".sdata"))) = 0;
int EditorOutModel::DaMix __attribute__((section(".sdata"))) = 0;
int EditorOutModel::CurrentFace __attribute__((section(".sdata"))) = 0;
ModelFace2 *EditorOutModel::FaceBuf
    __attribute__((section(".sdata"))) = 0;

/** EditorInModel's target-owned polygon-buffer state, in address order. */
int EditorInModel::GameBuild __attribute__((section(".sdata"))) = 0;
ModelFace *EditorInModel::pMaxBufEntry
    __attribute__((section(".sdata"))) = 0;
ModelFace *EditorInModel::pPolyBufEntry
    __attribute__((section(".sdata"))) = 0;

/** Successful-merge prefix emitted before the merged rail contents. */
static char s_Heigh_ho_Did_Merge_800b28f0[28]
    __attribute__((section(".rdata"))) = "\nHeigh ho -- Did Merge: ";

}

// === Owner vtables and their narrow destructor-address views ===
extern "C" void RailInfoDestructor(GenObjsRailInfo *, int)
    asm("_._8RailInfo");
extern "C" void GenObjs_DestroyRailString(RailString *, int)
    asm("_._10RailString");
static void GenObjs_DestroyRailPoint(RailPoint *, int)
    asm("_._9RailPoint");
extern "C" void GenObjs_DestroyEditorOutModel(EditorOutModel *, int)
    asm("_._14EditorOutModel");
extern "C" void GenObjs_DestroyEditorInModel(EditorInModel *, int)
    asm("_._13EditorInModel");
static void GenObjs_DestroyClass(CClass *, int)
    asm("__DOT__6CClass_80055FF0");

/** RailInfo's public three-word table and its target alignment word. */
int __vt_8RailInfo __attribute__((section(".sdata"), aligned(4))) = 0;
static int GenObjs_RailInfoVTableIndex
    __attribute__((section(".sdata"))) = 0;
static void (*GenObjs_RailInfoVTableDtor)(GenObjsRailInfo *, int)
    __attribute__((section(".sdata"))) = &RailInfoDestructor;
static int GenObjs_RailInfoVTablePadding
    __attribute__((section(".sdata"))) = 0;

/** RailString's public three-word table and its target alignment word. */
int __vt_10RailString __attribute__((section(".sdata"), aligned(4))) = 0;
static int GenObjs_RailStringVTableIndex
    __attribute__((section(".sdata"))) = 0;
static void (*GenObjs_RailStringVTableDtor)(RailString *, int)
    __attribute__((section(".sdata"))) = &GenObjs_DestroyRailString;
static int GenObjs_RailStringVTablePadding
    __attribute__((section(".sdata"))) = 0;

/** RailPoint's private three-word table and its target alignment word. */
static int GenObjs_RailPointVTable
    __attribute__((section(".sdata"), aligned(4))) = 0;
static int GenObjs_RailPointVTableIndex
    __attribute__((section(".sdata"))) = 0;
static void (*GenObjs_RailPointVTableDtor)(RailPoint *, int)
    __attribute__((section(".sdata"))) = &GenObjs_DestroyRailPoint;
static int GenObjs_RailPointVTablePadding
    __attribute__((section(".sdata"))) = 0;

/** EditorOutModel's public three-word table and its target alignment word. */
int __vt_14EditorOutModel
    __attribute__((section(".sdata"), aligned(4))) = 0;
static int GenObjs_EditorOutVTableIndex
    __attribute__((section(".sdata"))) = 0;
static void (*GenObjs_EditorOutVTableDtor)(EditorOutModel *, int)
    __attribute__((section(".sdata"))) = &GenObjs_DestroyEditorOutModel;
static int GenObjs_EditorOutVTablePadding
    __attribute__((section(".sdata"))) = 0;

/** EditorInModel's public three-word table and its target alignment word. */
int __vt_13EditorInModel
    __attribute__((section(".sdata"), aligned(4))) = 0;
static int GenObjs_EditorInVTableIndex
    __attribute__((section(".sdata"))) = 0;
static void (*GenObjs_EditorInVTableDtor)(EditorInModel *, int)
    __attribute__((section(".sdata"))) = &GenObjs_DestroyEditorInModel;
static int GenObjs_EditorInVTablePadding
    __attribute__((section(".sdata"))) = 0;

/** Private CClass table emitted at the end of GenObjs.obj's small data. */
static int GenObjs_BaseVTable
    __attribute__((section(".sdata"), aligned(4))) = 0;
static int GenObjs_BaseVTableIndex
    __attribute__((section(".sdata"))) = 0;
static void (*GenObjs_BaseVTableDtor)(CClass *, int)
    __attribute__((section(".sdata"))) = &GenObjs_DestroyClass;

// === Function implementations (address order) ===

/**
 * InitPolyBuf__13EditorInModelb — Initialize polygon buffer for model editing
 * Sets up the pre-allocated polygon buffer used by NewModelFace:
 *   - GameBuild flag controls whether buffer allocation or Mem_New is used
 *   - Buffer base comes from the active display buffer's polygon arena
 *   - Max entries = (POLYBUFFERSIZE * 2) / sizeof(ModelFace)
 * Each polygon-buffer entry is one ModelFace.
 *
 * Address:   0x80052D70, Size: 84 bytes (21 instructions)
 * Mangled:   InitPolyBuf__13EditorInModelb
 * Demangled: void InitPolyBuf(bool)
 *
 * @param game_build  Whether faces may be drawn from the fixed build buffer.
 */
void EditorInModel::InitPolyBuf(bool game_build)
{
    EditorInModel::GameBuild = game_build;
    int buf_size = GenObjsPolyBufferSize();
    int num_entries = ((u32)buf_size << 1) / sizeof(ModelFace);
    EditorInModel::pPolyBufEntry =
        (ModelFace *)reinterpret_cast<GenObjsDoubleBufferView *>(
            DoubleBuffer)[0].Polys;
    EditorInModel::pMaxBufEntry =
        EditorInModel::pPolyBufEntry + num_entries;
}

/**
 * NewModelFace__13EditorInModel — Allocate a new face entry from poly buffer
 * Part of the level build tool's model construction pipeline.
 * If GameBuild mode is active and the pre-allocated poly buffer hasn't
 * overflowed, advances the buffer pointer by one face entry.
 * Otherwise falls back to dynamic allocation via Mem_New.
 * Buffer is initialized by InitPolyBuf with a fixed pool from POLYBUFFERSIZE.
 *
 * Address:   0x80052DC4, Size: 84 bytes (21 instructions)
 * Mangled:   NewModelFace__13EditorInModel
 * Demangled: ModelFace *EditorInModel::NewModelFace(void)
 *
 */

ModelFace *EditorInModel::NewModelFace(void)
{
    if (EditorInModel::GameBuild != 0 &&
        EditorInModel::pPolyBufEntry < EditorInModel::pMaxBufEntry)
    {
        EditorInModel::pPolyBufEntry++;
        return EditorInModel::pPolyBufEntry;
    }
    return (ModelFace *)Mem_New(sizeof(ModelFace), 0, 1);
}

/**
 *
 * Address:   0x80052E18
 * Mangled:   __13EditorInModel
 * Demangled: EditorInModel::(void)
 *
 * @param self  Constructor receiver, represented by the proven owner-local layout.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
GenObjsEditorInModel *ConstructEditorInModel(
    GenObjsEditorInModel *self)
{
    self->CClass.vtable = (int *)&__vt_13EditorInModel;
    return self;
}

/**
 *
 * Address:   0x80052E28
 * Mangled:   _._13EditorInModel
 * Demangled: EditorInModel::~EditorInModel()
 */
EditorInModel::~EditorInModel()
{
    GenObjsEditorInModel *model = (GenObjsEditorInModel *)this;
    model->CClass.vtable = (int *)&__vt_13EditorInModel;
    KillModelInfo();
    model->CClass.vtable = &__vt_13EditorInModel + 4;
}

/**
 * KillModelInfo__13EditorInModel — Free face list and destroy associated object
 * Two phases:
 *   1. Walk the Faces linked list, freeing each entry
 *      with Mem_Delete unless within pre-allocated buffer (< pMaxBufEntry)
 *   2. Destroy railInfo through its virtual destructor and clear the field
 *
 * Address:   0x80052E78, Size: 144 bytes (36 instructions)
 * Mangled:   KillModelInfo__13EditorInModel
 * Demangled: EditorInModel::KillModelInfo(void)
 *
 */
void EditorInModel::KillModelInfo()
{
    GenObjsEditorInModel *self = (GenObjsEditorInModel *)this;
    ModelFace *face;
    ModelFace *next_face;

    if (self->Faces != 0)
    {
        do
        {
            face = self->Faces;
            next_face = face->next;

            if ((u32)face < (u32)EditorInModel::pMaxBufEntry)
            {
                // Within pre-allocated buffer, don't free
            }
            else
            {
                Mem_Delete(face);
            }
            self->Faces = next_face;
        } while (next_face != 0);
    }

    /* Destroy the rail info through its vtable (slot 2, __in_chrg = 3). */
    if (self->railInfo != 0)
    {
        DestroyCClass(self->railInfo);
    }
    self->railInfo = 0;
}

/**
 * Recover source-model dimensions, face metadata, and generated rail data.
 *
 * Address:   0x80052F08
 * Mangled:   SetData__13EditorInModelP6SModeli
 * Demangled: EditorInModel::SetData(SModel *, int)
 *
 * @param pModel  Model data being processed.
 * @param model_index  Editor piece index used to recover the piece flags.
 */
void EditorInModel::SetData(SModel *pModel, int model_index)
{
    GenObjsEditorInModel *modelInfo = (GenObjsEditorInModel *)this;

    modelInfo->mModel = pModel;
    modelInfo->mIndex = (u8)model_index;

    s16 xmax = ((ModelBounds *)&pModel->Box)->xmax;
    s16 xmin = ((ModelBounds *)&pModel->Box)->xmin;
    {
        int cellsize = _14LevelGenerator_GridCellWidth;
        int cells = (xmax - xmin) / cellsize;
        int diff = (xmax - xmin) % cellsize;
        modelInfo->w = cells;
        if (diff > cellsize - 4)
        {
            modelInfo->w++;
        }
    }

    s16 ymax = ((ModelBounds *)&modelInfo->mModel->Box)->ymax;
    s16 ymin = ((ModelBounds *)&modelInfo->mModel->Box)->ymin;
    {
        int cellsize = _14LevelGenerator_GridCellHeight;
        int cells = (ymax - ymin) / cellsize;
        int diff = (ymax - ymin) % cellsize;
        modelInfo->h = cells;
        if (diff > cellsize - 4)
        {
            modelInfo->h++;
        }
    }

    s16 zmax = ((ModelBounds *)&modelInfo->mModel->Box)->zmax;
    s16 zmin = ((ModelBounds *)&modelInfo->mModel->Box)->zmin;
    {
        int cellsize = _14LevelGenerator_GridCellWidth;
        int cells = (zmax - zmin) / cellsize;
        int diff = (zmax - zmin) % cellsize;
        modelInfo->l = cells;
        if (diff > cellsize - 4)
        {
            modelInfo->l++;
        }
    }

    modelInfo->xleft = -xmin;
    modelInfo->xright = xmax;
    modelInfo->ztop = zmax;
    modelInfo->zbottom = -zmin;
    modelInfo->yoff = ((ModelBounds *)&modelInfo->mModel->Box)->ymax;

    modelInfo->flags = EditorPieces_GetPieceFlags(modelInfo->mIndex);
    if ((modelInfo->flags & 0x10) == 0)
    {
        modelInfo->h = 1;
    }

    modelInfo->Faces = 0;
    ModelFace *last_face = 0;
    SModel *sourceModel = modelInfo->mModel;
    unsigned char *pFace =
        (unsigned char *)&sourceModel->Vertices[
            sourceModel->NumVertices + sourceModel->NumNormals];
    unsigned int NumFaces = sourceModel->NumFaces;

    for (int j = 0; j < (int)NumFaces; j++)
    {
        ModelFace *face = NewModelFace();
        face->next = 0;
        if (last_face == 0)
        {
            modelInfo->Faces = face;
        }
        else
        {
            last_face->next = face;
        }
        last_face = face;

        u16 flags = *(u16 *)pFace;
        face->outputSize = 0x10;
        if ((flags & 1) != 0)
        {
            face->outputSize = 0x1c;
        }
        if ((flags & 0x0c) == 0x0c)
        {
            face->outputSize += 8;
        }
        if ((flags & 0x20) != 0)
        {
            face->outputSize += 4;
        }

        *(u32 *)face->vertexNumber = *(u32 *)(pFace + 4);
        pFace += face->outputSize;

        SVector *point = face->verts;
        ModelPoint *verts =
            (ModelPoint *)modelInfo->mModel->Vertices;
        for (int i = 0; i < 4; i++)
        {
            int index = (u8)face->vertexNumber[i];
            point[i].vx = verts[index].vx;
            point[i].vz = verts[index].vz;
            point[i].vy = verts[index].vy;
        }

        int x = point[0].vx;
        int z = point[0].vz;
        char plane = 3;
        int d;
        for (int i = 1; i < 4; i++)
        {
            d = point[i].vx - x;
            if ((unsigned int)(d + 1) > 2)
            {
                plane &= 1;
            }
            d = point[i].vz - z;
            if ((unsigned int)(d + 1) > 2)
            {
                plane &= 2;
            }
        }

        if (plane == 0)
        {
            face->orientation = -1;
        }
        else
        {
            if ((plane & 2) != 0)
            {
                plane = 2;
            }
            else
            {
                plane = 1;
            }

            face->numVerts = 4;
            if (point[3].vx == point[2].vx &&
                point[3].vy == point[2].vy &&
                point[3].vz == point[2].vz)
            {
                face->numVerts = 3;
            }

            for (int i = 0; i < face->numVerts; i++)
            {
                SVector temp = point[i];
                if (plane != 1)
                {
                    if (plane == 2)
                    {
                        face->orientation = 0;
                        point[i].vx = temp.vz;
                        point[i].vz = temp.vx;
                    }
                }
                else
                {
                    face->orientation = plane;
                    point[i].vz = -temp.vz;
                }
            }

            if (face->numVerts == 4)
            {
                SVector temp = point[3];
                point[3] = point[2];
                point[2] = temp;
            }

            point[(u8)face->numVerts] = point[0];
            int dir =
                (point[1].vx - point[0].vx) * (point[2].vy - point[1].vy) -
                (point[1].vy - point[0].vy) * (point[2].vx - point[1].vx);
            if (dir < 0) { dir = 0; } else { dir = 2; }
            face->orientation |= dir;

            for (int i = 0; i < face->numVerts; i++)
            {
                if (dir == 0)
                {
                    point[i].vx = -point[i].vx;
                    point[i].vz = -point[i].vz;
                }
            }
        }
    }

    GenObjsRailInfo *railInfo =
        (GenObjsRailInfo *)CClass::operator new(sizeof(GenObjsRailInfo));
    if (railInfo != 0)
    {
        railInfo = ConstructRailInfo(railInfo);
    }
    modelInfo->railInfo = railInfo;
}

/**
 * SetTransparency__13EditorInModelb — Toggle transparency bits on all faces
 * Walks face linked list, for each face that has transparency bits (0xC0):
 *   on=true:  set bit 6, clear bit 7 (semi-transparent mode)
 *   on=false: clear bit 6, set bit 7 (opaque with STP bit)
 *
 * Address:   0x80053428, Size: 116 bytes (29 instructions)
 * Mangled:   SetTransparency__13EditorInModelb
 * Demangled: EditorInModel::SetTransparency(bool)
 *
 * @param on  Select the semi-transparent state when true.
 */
void EditorInModel::SetTransparency(bool on)
{
    GenObjsEditorInModel *model = (GenObjsEditorInModel *)this;
    SModel *meshData = model->mModel;
    u16 NumVertices = meshData->NumVertices;
    u16 NumNormals = meshData->NumNormals;
    ModelFace *face = model->Faces;
    int offset = ((u32)(NumVertices + NumNormals) << 3) + 0x1c;
    char *pFace = (char *)meshData + offset;

    while (face != 0)
    {
        u16 flags = *(u16 *)pFace;
        u16 newFlags;
        if ((flags & 0xc0) != 0)
        {
            if (on)
            {
                newFlags = (flags | 0x40) & 0xff7f;
            }
            else
            {
                newFlags = (flags & 0xffbf) | 0x80;
            }
            *(u16 *)pFace = newFlags;
        }
        pFace += face->outputSize;
        face = face->next;
    }
}

/**
 * Initialize EditorOutModel's lighting tables and shared face pool.
 *
 * Address:   0x8005349C
 * Mangled:   Init__14EditorOutModel
 * Demangled: EditorOutModel::Init(void)
 *
 * Initializes the shared editor lighting tables and reusable face pool.
 */
void EditorOutModel::Init(void)
{
    {
        int i = 0;
        int lightLocationY = -0x3e8;
        int lightVectorY = 0x1000;
        int *lightBase = (int *)LightVect;
        int *lightVz = lightBase + 2;
        int *lightVy = lightBase + 1;
        Vector *lightVect = LightVect;
        int *lightLoc = (int *)LightLoc;
        int *lightLocY = lightLoc + 1;

        lightLoc[0] = -0x7f8;
        lightLoc[2] = 0x7f8;
        lightLoc[3] = 0x7f8;
        lightLoc[5] = 0x7f8;
        lightLoc[6] = -0x7f8;
        lightLoc[8] = -0x7f8;
        lightLoc[9] = 0x7f8;
        lightLoc[11] = -0x7f8;

        do
        {
            lightVect->vx = 0;
            *lightVy = lightVectorY;
            *lightVz = 0;
            lightVz += 3;
            lightVy += 3;
            lightVect++;
            *lightLocY = lightLocationY;
            lightLocY += 3;
        } while (++i < 4);
    }

    EditorOutModel::DaMix = 0x28;
    EditorOutModel::LightIntensity = 0x4b000;
    EditorOutModel::FallOff = 0xa;

    {
        int i;

        EditorOutModel::IntensityLookup =
            (unsigned char *)Mem_New(0x400, 1, 1);
        for (i = 0; i < 0x400; i++)
        {
            int color =
                (0x3ff - ((0x3ff - i) * EditorOutModel::FallOff) / 10) >> 2;
            if (color < 0)
                color = 0;
            ((u8 *)EditorOutModel::IntensityLookup)[i] = (u8)color;
        }
    }

    {
        int i;

        EditorOutModel::MixLookup =
            (unsigned char *)Mem_New(0x100, 1, 1);
        for (i = 0; i < 0x100; i++)
        {
            ((u8 *)EditorOutModel::MixLookup)[i] =
                (u8)(((i - 0x80) * EditorOutModel::DaMix + 0x3200) / 100);
        }
    }

    {
        int i;
        int freeArea;
        ModelFace2 *faceBuf;

        EditorOutModel::FaceBuf =
            (ModelFace2 *)Mem_New(0x9c40, 0, 1);
        i = 0;
        freeArea = -9999;
        faceBuf = EditorOutModel::FaceBuf;
        do
        {
            faceBuf->area = freeArea;
            i++;
            faceBuf++;
        } while (i < 2000);
    }

    EditorOutModel::CurrentFace = 0;
}

/**
 * Release EditorOutModel's shared lookup tables and face pool.
 *
 * Address:   0x8005366C
 * Mangled:   Cleanup__14EditorOutModel
 * Demangled: EditorOutModel::Cleanup(void)
 *
 */
void EditorOutModel::Cleanup(void)
{
    Mem_Delete(EditorOutModel::IntensityLookup);
    Mem_Delete(EditorOutModel::MixLookup);
    Mem_Delete(EditorOutModel::FaceBuf);
    EditorOutModel::FaceBuf = 0;
}

/**
 * NewFace__14EditorOutModel — Find and initialize a free face slot
 * Circular buffer search for the next free face slot (marked with -9999
 * sentinel value). Buffer holds 2000 ModelFace2 entries.
 * Once found, initializes the slot:
 *   - area = -8888 (allocated)
 *   - alive = true
 *   - orientation = -1
 *   - sourceFace and next = null
 * Sentinel values: -9999 = free, -8888 = allocated/in-use.
 *
 * Address:   0x800536A8, Size: 152 bytes (38 instructions)
 * Mangled:   NewFace__14EditorOutModel
 * Demangled: ModelFace2 *EditorOutModel::NewFace(void)
 *
 */
ModelFace2 *EditorOutModel::NewFace(void)
{
    int count = 2000;
    ModelFace2 *faceBuf = EditorOutModel::FaceBuf;
    int sentinel = -9999;
    int nextIdx;
    ModelFace2 *face;

    do
    {
        register int cf asm("$4") = EditorOutModel::CurrentFace;
        nextIdx = cf + 1;
        if (faceBuf[cf].area == sentinel)
        {
            goto found;
        }
        EditorOutModel::CurrentFace = nextIdx;
        if (nextIdx > 1999)
        {
            EditorOutModel::CurrentFace = 0;
        }
        count--;
    } while (count != 0);

found:
{
    int cf = EditorOutModel::CurrentFace;
    face = &EditorOutModel::FaceBuf[cf];
    cf = cf + 1;
    EditorOutModel::CurrentFace = cf;
}
    face->alive = (void *)1;
    face->orientation = -1;
    face->area = -8888;
    face->next = 0;
    face->sourceFace = 0;
    return face;
}

/**
 * Construct an output-model face list from an editor input model.
 *
 * Address:   0x80053740
 * Mangled:   __14EditorOutModelP13EditorInModeli
 * Demangled: EditorOutModel::(EditorInModel *, int)
 *
 * @param source  Source editor model whose live faces are copied.
 * @param id  Recovered model identifier; this constructor does not store it.
 */
EditorOutModel::EditorOutModel(EditorInModel *source, int id)
{
    GenObjsEditorOutModel *model = (GenObjsEditorOutModel *)this;
    GenObjsEditorInModel *inModel = (GenObjsEditorInModel *)source;
    ModelFace *source_face;
    ModelFace2 *last_face;
    int i;

    model->CClass.vtable = (int *)&__vt_14EditorOutModel;
    model->SourceModel = inModel;

    model->NumVertices = inModel->mModel->NumVertices;
    model->NumNormals = inModel->mModel->NumNormals;
    model->NumFaces = inModel->mModel->NumFaces;

    // Zero 16 neighbor slots
    {
        i = 0;
        GenObjsEditorOutModel **neighbor_slot = model->Neighbors;
        do
        {
            *neighbor_slot = 0;
            i++;
            neighbor_slot++;
        } while (i < 16);
    }

    model->Next = 0;
    model->Faces = 0;

    source_face = inModel->Faces;
    if (source_face != 0)
    {
        last_face = 0;
        do
        {
            if (source_face->orientation != -1)
            {
                ModelFace2 *face = NewFace();
                face->sourceFace = source_face;
                if (last_face != 0)
                {
                    last_face->next = face;
                }
                else
                {
                    model->Faces = face;
                }
                last_face = face;
            }
            source_face = source_face->next;
        } while (source_face != 0);
    }

    model->FaceRemovalRequest = 1;
    model->RestoreFacesRequest = 0;
    model->DeleteModelRequest = 0;
}

/**
 * Release this output model's face records and restore its base vtable.
 *
 * Address:   0x80053844
 * Mangled:   _._14EditorOutModel
 * Demangled: EditorOutModel::~EditorOutModel()
 *
 */
EditorOutModel::~EditorOutModel()
{
    GenObjsEditorOutModel *model = (GenObjsEditorOutModel *)this;
    ModelFace2 *faceInit = model->Faces;
    model->CClass.vtable = (int *)&__vt_14EditorOutModel;
    if (faceInit != 0)
    {
        int sentinel = -9999;
        ModelFace2 *face;
        do
        {
            model->Faces->area = sentinel;
            face = model->Faces->next;
            model->Faces = face;
        } while (face != 0);
    }
    model->CClass.vtable = (int *)&GenObjs_BaseVTable;
}

/**
 * RestoreFaces__14EditorOutModel — Restore every output face to its live state
 * Walks the Faces linked list and restores the active marker and allocated
 * area sentinel used by the face-removal pass.
 *
 * Address:   0x800538B0, Size: 52 bytes (13 instructions)
 * Mangled:   RestoreFaces__14EditorOutModel
 * Demangled: void EditorOutModel::RestoreFaces(void)
 */
void EditorOutModel::RestoreFaces(void)
{
    ModelFace2 *face;

    for (face = ((GenObjsEditorOutModel *)this)->Faces;
         face != 0;
         face = face->next)
    {
        face->alive = (void *)1;
        face->area = -8888;
    }
}

/**
 * Merge the shadowed area of two neighbouring output faces and retire the
 * losers.
 *
 * Runs over every pair of output faces in `this` and `neighbor`. Each face's
 * source orientation is rotated into world space by its model's `Rot`, and the
 * pair is only considered when the two rotated orientations describe opposed
 * co-planar quads (same bit 0 as `orientation`, opposite bit 1). For such a
 * pair both faces are rebuilt as world-space FP rings from the shared
 * FACEREMOVAL point pool, wound CCW when bit 1 of the orientation is clear,
 * and the embedded (overlapping) area is subtracted from both. A face whose
 * remaining area drops below 301 units, or below 1/32 of what it started with,
 * is marked dead.
 *
 * When either model still carries FaceRemovalRequest 2 every face is
 * reprocessed; otherwise only faces that are still alive take part.
 *
 * Address:   0x800538E4, Size: 2672 bytes (668 instructions)
 * Mangled:   RemoveSomeHiddenFaces__14EditorOutModelP14EditorOutModeli
 * Demangled: EditorOutModel::RemoveSomeHiddenFaces(EditorOutModel *, int)
 * Match:     PERFECT (668/668 instructions, 0 real and 0 relocation mismatches)
 *
 * @param neighbor  Adjacent model whose faces are tested against this one's.
 * @param orientation  Side-orientation mask selected by RemoveAllHiddenFaces.
 */
void EditorOutModel::RemoveSomeHiddenFaces(EditorOutModel *neighbor, int orientation)
{
    GenObjsEditorOutModel *model1 = (GenObjsEditorOutModel *)this;
    GenObjsEditorOutModel *model2 = (GenObjsEditorOutModel *)neighbor;

    if (model1->FaceRemovalRequest == 2 || model2->FaceRemovalRequest == 2)
    {
        ModelFace2 *face1;
        ModelFace2 *face2;

        for (face1 = model1->Faces; face1 != 0; face1 = face1->next)
        {
            for (face2 = model2->Faces; face2 != 0; face2 = face2->next)
            {
                unsigned char facing = (unsigned char)orientation;
                ModelFace2 *face[2];
                GenObjsEditorOutModel *model[2];
                FP *fp_list[2];
                int orientation[2];
                int i;
                int index;
                int orig_area;
                int sub_area;

                ResetFPList();

                face[0] = face1;
                face[1] = face2;
                model[0] = model1;
                model[1] = model2;

                for (i = 0; i < 2; i++)
                {
                    orientation[i] = (s8)face[i]->sourceFace->orientation;
                    if (!(orientation[i] & 1))
                        orientation[i] = 2 - orientation[i];
                    orientation[i] = (orientation[i] + model[i]->Rot) & 3;
                    if (!(orientation[i] & 1))
                        orientation[i] = 2 - orientation[i];
                    face[i]->orientation = (unsigned char)orientation[i];
                }

                if (orientation[1] == facing &&
                    (orientation[0] & 2) != (facing & 2) &&
                    (orientation[0] & 1) == (facing & 1))
                {
                    for (index = 0; index < 2; index++)
                    {
                        FP *fp;
                        FP *last_fp;
                        FP *next;
                        int i;

                        fp_list[index] = 0;
                        last_fp = 0;

                        for (i = 0; i < (u8)face[index]->sourceFace->numVerts; i++)
                        {
                            fp = NewFP();
                            if (last_fp != 0)
                                last_fp->next = fp;
                            else
                                fp_list[index] = fp;
                            last_fp = fp;

                            switch (orientation[index])
                            {
                            case 0:
                                fp->pt.vz = model[index]->X -
                                            face[index]->sourceFace->verts[i].vz;
                                fp->pt.vx = model[index]->Z -
                                            face[index]->sourceFace->verts[i].vx;
                                break;
                            case 1:
                                fp->pt.vx = model[index]->X -
                                            face[index]->sourceFace->verts[i].vx;
                                fp->pt.vz = model[index]->Z +
                                            face[index]->sourceFace->verts[i].vz;
                                break;
                            case 2:
                                fp->pt.vz = model[index]->X +
                                            face[index]->sourceFace->verts[i].vz;
                                fp->pt.vx = model[index]->Z +
                                            face[index]->sourceFace->verts[i].vx;
                                break;
                            case 3:
                                fp->pt.vx = model[index]->X +
                                            face[index]->sourceFace->verts[i].vx;
                                fp->pt.vz = model[index]->Z -
                                            face[index]->sourceFace->verts[i].vz;
                                break;
                            }

                            fp->pt.vy = model[index]->Y +
                                        face[index]->sourceFace->verts[i].vy;
                        }

                        fp->next = fp_list[index];

                        if (!(orientation[index] & 2))
                        {
                            last_fp = 0;
                            fp = fp_list[index];
                            do
                            {
                                next = fp->next;
                                if (last_fp != 0)
                                    fp->next = last_fp;
                                last_fp = fp;
                                fp = next;
                            } while (next != fp_list[index]);
                            next->next = last_fp;
                        }

                        if (face[index]->area == -8888)
                            face[index]->area = GetArea(fp_list[index]);
                    }

                    sub_area = GetEmbeddedArea(fp_list[0], fp_list[1]);

                    orig_area = face1->area;
                    face1->area = orig_area - sub_area;
                    if (face1->area < 301 || face1->area <= (orig_area >> 5))
                        face1->alive = 0;

                    orig_area = face2->area;
                    face2->area = orig_area - sub_area;
                    if (face2->area < 301 || face2->area <= (orig_area >> 5))
                        face2->alive = 0;
                }
            }
        }
    }
    else
    {
        ModelFace2 *face1;
        ModelFace2 *face2;

        for (face1 = model1->Faces; face1 != 0; face1 = face1->next)
        {
            if (face1->alive == 0)
                continue;

            for (face2 = model2->Faces; face2 != 0; face2 = face2->next)
            {
                if (face2->alive == 0)
                    continue;

                unsigned char facing = (unsigned char)orientation;
                ModelFace2 *face[2];
                GenObjsEditorOutModel *model[2];
                FP *fp_list[2];
                int orientation[2];
                int i;
                int index;
                int orig_area;
                int sub_area;

                ResetFPList();

                face[0] = face1;
                face[1] = face2;
                model[0] = model1;
                model[1] = model2;

                for (i = 0; i < 2; i++)
                {
                    orientation[i] = (s8)face[i]->sourceFace->orientation;
                    if (!(orientation[i] & 1))
                        orientation[i] = 2 - orientation[i];
                    orientation[i] = (orientation[i] + model[i]->Rot) & 3;
                    if (!(orientation[i] & 1))
                        orientation[i] = 2 - orientation[i];
                    face[i]->orientation = (unsigned char)orientation[i];
                }

                if (orientation[1] == facing &&
                    (orientation[0] & 2) != (facing & 2) &&
                    (orientation[0] & 1) == (facing & 1))
                {
                    for (index = 0; index < 2; index++)
                    {
                        FP *fp;
                        FP *last_fp;
                        FP *next;
                        int i;

                        fp_list[index] = 0;
                        last_fp = 0;

                        for (i = 0; i < (u8)face[index]->sourceFace->numVerts; i++)
                        {
                            fp = NewFP();
                            if (last_fp != 0)
                                last_fp->next = fp;
                            else
                                fp_list[index] = fp;
                            last_fp = fp;

                            switch (orientation[index])
                            {
                            case 0:
                                fp->pt.vz = model[index]->X -
                                            face[index]->sourceFace->verts[i].vz;
                                fp->pt.vx = model[index]->Z -
                                            face[index]->sourceFace->verts[i].vx;
                                break;
                            case 1:
                                fp->pt.vx = model[index]->X -
                                            face[index]->sourceFace->verts[i].vx;
                                fp->pt.vz = model[index]->Z +
                                            face[index]->sourceFace->verts[i].vz;
                                break;
                            case 2:
                                fp->pt.vz = model[index]->X +
                                            face[index]->sourceFace->verts[i].vz;
                                fp->pt.vx = model[index]->Z +
                                            face[index]->sourceFace->verts[i].vx;
                                break;
                            case 3:
                                fp->pt.vx = model[index]->X +
                                            face[index]->sourceFace->verts[i].vx;
                                fp->pt.vz = model[index]->Z -
                                            face[index]->sourceFace->verts[i].vz;
                                break;
                            }

                            fp->pt.vy = model[index]->Y +
                                        face[index]->sourceFace->verts[i].vy;
                        }

                        fp->next = fp_list[index];

                        if (!(orientation[index] & 2))
                        {
                            last_fp = 0;
                            fp = fp_list[index];
                            do
                            {
                                next = fp->next;
                                if (last_fp != 0)
                                    fp->next = last_fp;
                                last_fp = fp;
                                fp = next;
                            } while (next != fp_list[index]);
                            next->next = last_fp;
                        }

                        if (face[index]->area == -8888)
                            face[index]->area = GetArea(fp_list[index]);
                    }

                    sub_area = GetEmbeddedArea(fp_list[0], fp_list[1]);

                    orig_area = face1->area;
                    face1->area = orig_area - sub_area;
                    if (face1->area < 301 || face1->area <= (orig_area >> 5))
                        face1->alive = 0;

                    orig_area = face2->area;
                    face2->area = orig_area - sub_area;
                    if (face2->area < 301 || face2->area <= (orig_area >> 5))
                        face2->alive = 0;
                }
            }
        }
    }
}

/**
 * RemoveAllHiddenFaces__14EditorOutModel — Remove hidden faces between neighboring models
 * Level build tool optimization: removes faces shared between adjacent model
 * pieces that would never be visible (internal faces between connected geometry).
 * For each neighbor in the model's neighbor list:
 *   - Skip if either source model has its transparent/special flag set
 *   - Skip if the neighbor is a floor piece (determined by IsAFloorPiece)
 *   - Skip if neither model requests face removal
 *   - Call RemoveSomeHiddenFaces with a mode (0-3) based on:
 *     - SourceModel::flags bit 3 selects mode pair (0/1 vs 2/3)
 *     - the neighbor index and mFirstEasternNeighbor select within the pair
 *
 * Address:   0x80054354, Size: 360 bytes (90 instructions)
 * Mangled:   RemoveAllHiddenFaces__14EditorOutModel
 * Demangled: void EditorOutModel::RemoveAllHiddenFaces(void)
 * Match:     PERFECT (90/90 instructions, 0 real and 0 relocation mismatches)
 */

void EditorOutModel::RemoveAllHiddenFaces(void)
{
    auto model_ = this;
    GenObjsEditorOutModel *model = (GenObjsEditorOutModel *)model_;
    int i;
    GenObjsEditorOutModel **neighborSlot;

    // Skip transparent/special models (info flags bit 0)
    if ((model->SourceModel->flags & 1) != 0)
        return;

    // Skip floor pieces (no hidden face removal needed)
    if (IsAFloorPiece(model->SourceModel->mIndex) != 0)
        return;

    i = 0;
    if (model->NumNeighbors != 0)
    {
        do
        {
            register int neighbor_offset asm("$4") = (u32)i << 2;
            register int neighbor_base asm("$2") = (int)model->Neighbors;
            neighborSlot =
                (GenObjsEditorOutModel **)(neighbor_base + neighbor_offset);

            // Check neighbor eligibility: not flagged, not a floor piece,
            // and at least one of the pair has hidden faces
            if ((((*neighborSlot)->SourceModel->flags & 1) == 0) &&
                (IsAFloorPiece((*neighborSlot)->SourceModel->mIndex) == 0) &&
                ((model->FaceRemovalRequest != 0 ||
                  (*neighborSlot)->FaceRemovalRequest != 0)))
            {

                // Select removal mode based on flags bit 3 and neighbor index
                if ((model->SourceModel->flags & 8) != 0)
                {
                    // Mode pair 2/3 (bit 3 set)
                    if (i < (int)(u32)model->mFirstEasternNeighbor)
                    {
                        RemoveSomeHiddenFaces((EditorOutModel *)*neighborSlot, 3);
                    }
                    else
                    {
                        RemoveSomeHiddenFaces((EditorOutModel *)*neighborSlot, 2);
                    }
                }
                else
                {
                    // Mode pair 0/1 (bit 3 clear)
                    if (i < (int)(u32)model->mFirstEasternNeighbor)
                    {
                        RemoveSomeHiddenFaces((EditorOutModel *)*neighborSlot, 1);
                    }
                    else
                    {
                        RemoveSomeHiddenFaces((EditorOutModel *)*neighborSlot, 0);
                    }
                }
            }

            i = i + 1;
        } while (i < (int)(u32)model->NumNeighbors);
    }
}

/**
 * SetModelDataSize__14EditorOutModelb — Calculate model data size (vertices + included faces)
 * Leaf function. Iterates face linked list, optionally checking a secondary list
 * to determine inclusion. Sums face byte sizes for included faces.
 *
 * Address:   0x800544BC, Size: 148 bytes (37 instructions)
 * Mangled:   SetModelDataSize__14EditorOutModelb
 * Demangled: EditorOutModel::SetModelDataSize(bool)
 *
 * @param game_build  Whether dead faces are omitted from the output.
 * @return Total serialized model size in bytes.
 */
int EditorOutModel::SetModelDataSize(bool game_build)
{
    GenObjsEditorOutModel *model = (GenObjsEditorOutModel *)this;
    ModelFace2 *out_face = model->Faces;
    u16 normalCount = model->NumNormals;
    ModelFace *source_face = model->SourceModel->Faces;
    u16 vertexCount = model->NumVertices;
    int size_of_faces = 0;
    model->NumFaces = 0;
    int size_to_faces = ((int)(vertexCount + normalCount) << 3) + 0x1c;
    if (source_face == 0)
        goto done;

loop:
{
    int copy_it = 1;
    if (game_build == 0)
        goto skipCheck;
    if (out_face == 0)
        goto skipCheck;
    if (out_face->sourceFace != source_face)
        goto skipCheck;
    {
        copy_it = out_face->alive != 0;
        out_face = out_face->next;
    }
skipCheck:
    if (copy_it == 0)
        goto noInclude;
    size_of_faces = size_of_faces + source_face->outputSize;
    model->NumFaces = model->NumFaces + 1;
noInclude:
    source_face = source_face->next;
}
    if (source_face != 0)
        goto loop;

done:
{
    model->Size = size_to_faces + size_of_faces;
    return model->Size;
}
}

/**
 * CreateModelData__14EditorOutModelPcb — Write per-model PSX binary data with lighting
 * Writes the binary mesh data for a single model into the output PSX file buffer.
 * Called by CreatePSX for each placed model (main and floor lists).
 * Steps:
 *   1. Copy mesh header + vertex/normal data from source model via Mem_Copy
 *   2. Rotate bounding box in the copied header based on model rotation (0-3)
 *   3. Rotate all vertex X/Z coordinates based on model rotation
 *   4. Rotate all normal X/Z directions based on model rotation
 *   5. Walk the face linked list, copying face data with per-vertex lighting:
 *      - Check hidden face list: mark invisible (clear flags) or skip entirely
 *      - Set gouraud shading flag (0x0800) on each face
 *      - For each of 4 vertex indices per face, accumulate lighting from 4 lights:
 *        distance = ((worldPos + vtxPos) - lightPos) >> 1, squared via GTE SQR
 *        intensity = (LightIntensity << 8) / distanceSq, clamped to 1023
 *        contribution = IntensityLookup[intensity]
 *        total = sum of 4 lights, clamped to 255
 *        color = MixLookup[total]
 *   6. Write face count into output mesh header, return model identifier
 * EditorOutModel fields used:
 *   +0x12: u16 modelIdentifier (returned)
 *   +0x14: u16 vertexCount
 *   +0x16: u16 normalCount
 *   +0x18: u16 faceCount (written to output header at +0x06)
 *   +0x5C: int* hiddenFaceList (linked: +0x04=visible, +0x0C=facePtr, +0x10=next)
 *   +0x66: s16 worldX
 *   +0x6A: s16 worldZ
 *   +0x6C: u8  rotation (0-3, 90° increments)
 *   +0x70: int* sourceModel (+0x04=faceListHead, +0x08=meshDataPtr)
 *   +0x74: int* outputBufPtr (set to outputBuf on entry)
 * Face entry (linked list from sourceModel+0x04):
 *   +0x24: u8  faceDataSize (byte count to copy)
 *   +0x28: int* next face
 * Face output layout (first 12 bytes):
 *   +0x00: u16 faceFlags (bit 0x0800 = gouraud shading set by this function)
 *   +0x04: u8[4] vertex indices (byte per vertex, index into vertex array)
 *   +0x08: u8[4] vertex colors (computed from lighting, via MixLookup)
 * GTE instruction used:
 *   SQR (0x0A00428): Square of IR1/IR2/IR3 → MAC1/MAC2/MAC3 (sf=1, lm=0)
 *   Used to compute squared distance from vertex to light source.
 * Parameters:
 *   self      — EditorOutModel*
 *   outputBuf — write cursor in the output PSX file buffer
 *   game_build — if true, hidden faces are skipped entirely;
 *               if false, hidden faces are copied with visibility bits cleared
 * Returns: Size — the output model's 16-bit identifier / flags
 * External globals — lighting system
 * DAT_800c044c
 *
 * Address:   0x80054550, Size: 1224 bytes (306 instructions)
 * Mangled:   CreateModelData__14EditorOutModelPcb
 * Demangled: u16 EditorOutModel::CreateModelData(char *, bool)
 *
 * @param pData  Destination cursor for the generated model and face data.
 * @param game_build  Whether dead faces are omitted instead of marked invisible.
 * @return The output model's recovered 16-bit result field.
 */

int EditorOutModel::CreateModelData(char *pData, bool game_build)
{
    GenObjsEditorOutModel *self = (GenObjsEditorOutModel *)this;
    SModel *pModel;
    int size_to_faces;
    SModel *outbuf;
    ModelBounds *bounds;
    int xmax, xmin, zmax, zmin;
    ModelPoint *vertex;
    ModelPoint *normal;
    char *pFace;
    ModelFace *face;
    ModelFace2 *test_face;
    void *copy_it;
    void *make_invisible;
    u8 *pColor;
    int vert;
    int color;
    int lt;
    Vector light;
    CVector VSqd;
    int r_squared;
    int add_color;

    // ================================================================
    // 1. Copy mesh header + vertex/normal data
    // ================================================================
    // Store output buffer pointer in model
    self->TargetModel = (SModel *)pData;

    // Total copy size: header (0x1C) + (vertexCount + normalCount) × 8 bytes per SVECTOR
    size_to_faces =
        ((u32)self->NumVertices + (u32)self->NumNormals) * 8 + 0x1c;
    pModel = self->SourceModel->mModel;
    Mem_Copy(pData, pModel, size_to_faces);

    // ================================================================
    // 2. Rotate bounding box based on model rotation
    // ================================================================
    outbuf = self->TargetModel;
    bounds = (ModelBounds *)&outbuf->Box;

    // Read original bbox values from the copied header
    xmax = bounds->xmax;
    xmin = bounds->xmin;
    zmax = bounds->zmax;
    zmin = bounds->zmin;

    // Face data starts after header + vertices + normals
    pData += size_to_faces;

    switch (self->Rot)
    {
    case 0:
        // 0°: no rotation, copy as-is
        bounds->xmin = xmin;
        bounds->xmax = xmax;
        bounds->zmin = zmin;
        bounds->zmax = zmax;
        outbuf = self->TargetModel;
        break;
    case 1:
        // 90° CW: X' = -Z, Z' = X
        bounds->xmin = -zmax;
        bounds->xmax = -zmin;
        bounds->zmin = xmin;
        bounds->zmax = xmax;
        outbuf = self->TargetModel;
        break;
    case 2:
        // 180°: negate both axes
        bounds->xmin = -xmax;
        bounds->xmax = -xmin;
        bounds->zmin = -zmax;
        bounds->zmax = -zmin;
        outbuf = self->TargetModel;
        break;
    case 3:
        // 270° CW: X' = Z, Z' = -X
        bounds->xmin = zmin;
        bounds->xmax = zmax;
        bounds->zmin = -xmax;
        bounds->zmax = -xmin;
        outbuf = self->TargetModel;
        break;
    }
    // ================================================================
    // 3. Rotate vertex positions (X and Z) based on rotation
    // ================================================================
    {
        int i;
        short x;
        short z;

        vertex = (ModelPoint *)outbuf->Vertices;
        for (i = 0; i < (int)self->NumVertices; i++)
        {
            x = vertex[i].vx;
            z = vertex[i].vz;
            switch (self->Rot)
            {
            case 0:
                vertex[i].vx = x;
                vertex[i].vz = z;
                break;
            case 1:
                vertex[i].vx = -z;
                vertex[i].vz = x;
                break;
            case 2:
                vertex[i].vx = -x;
                vertex[i].vz = -z;
                break;
            case 3:
                vertex[i].vx = z;
                vertex[i].vz = -x;
                break;
            }
        }
    }

    // ================================================================
    // 4. Rotate normal directions (same logic, starting after vertices)
    // ================================================================
    {
        int i;
        short x;
        short z;

        normal = vertex + self->NumVertices;
        for (i = 0; i < (int)self->NumNormals; i++)
        {
            x = normal[i].vx;
            z = normal[i].vz;
            switch (self->Rot)
            {
            case 0:
                normal[i].vx = x;
                normal[i].vz = z;
                break;
            case 1:
                normal[i].vx = -z;
                normal[i].vz = x;
                break;
            case 2:
                normal[i].vx = -x;
                normal[i].vz = -z;
                break;
            case 3:
                normal[i].vx = z;
                normal[i].vz = -x;
                break;
            }
        }
    }

    // ================================================================
    // 5. Walk face list, copy face data with per-vertex lighting
    // ================================================================
    pFace = (char *)pModel + size_to_faces;
    test_face = (ModelFace2 *)self->Faces;

    for (face = (ModelFace *)self->SourceModel->Faces; face != 0;
         face = face->next)
    {
        copy_it = (void *)1;
        make_invisible = 0;

        // Check if this face is in the hidden face list
        if ((test_face != 0) && (test_face->sourceFace == face))
        {
            if (test_face->alive == 0)
            {
                // Face is hidden
                if (game_build)
                {
                    copy_it = 0;
                }
                else
                {
                    make_invisible = (void *)1;
                }
            }
            test_face = test_face->next;
        }

        if (copy_it)
        {
            // Copy face data from source
            Mem_Copy(pData, pFace, face->outputSize);
            pColor = (u8 *)pData + 8;

            // Clear visibility bits if face is marked invisible
            if (make_invisible)
            {
                *(u16 *)pData = *(u16 *)pData & 0xff3f;
            }

            // Set gouraud shading flag
            *(u16 *)pData = *(u16 *)pData | 0x800;

            // --------------------------------------------------------
            // Per-vertex lighting: 4 vertices × 4 lights
            // --------------------------------------------------------
            for (vert = 0; vert < 4; vert++)
            {
                color = 0;
                for (lt = 0; lt < 4; lt++)
                {
                    // Compute delta vector from vertex world-position to light
                    // dX = ((worldX + vertexX) - lightX) >> 1
                    // dZ = ((worldZ + vertexZ) - lightZ) >> 1
                    // dY = 0 (2D distance only)
                    light.vx =
                        ((int)self->X +
                         (int)vertex[(u32)(u8)pData[vert + 4]].vx -
                         LightLoc[lt].vx) >>
                        1;
                    light.vz =
                        ((int)self->Z +
                         (int)vertex[(u32)(u8)pData[vert + 4]].vz -
                         LightLoc[lt].vz) >>
                        1;
                    light.vy = 0;

                    // GTE SQR: square the delta vector components
                    // MAC1 = dX², MAC2 = 0, MAC3 = dZ²
                    gte_ldlvl(&light);
                    gte_sqr0();
                    gte_stlvnl(&VSqd);
                    r_squared = VSqd.vx + VSqd.vy + VSqd.vz;

                    // Compute intensity from distance falloff
                    if (r_squared == 0)
                    {
                        add_color = 0x3ff;
                    }
                    else
                    {
                        add_color =
                            (EditorOutModel::LightIntensity << 8) / r_squared;
                    }
                    /* Preserve the target branch label at ASPSX's mandatory
                     * divide-result delay instruction. */
                    __asm__ __volatile__("nop");
                    if (0x3ff < add_color)
                    {
                        add_color = 0x3ff;
                    }

                    // Accumulate contribution from this light
                    add_color = (u32)*(u8 *)(EditorOutModel::IntensityLookup +
                                            add_color);
                    color += add_color;
                }

                // Clamp total intensity to 255
                if (0xff < color)
                {
                    color = 0xff;
                }

                pColor[vert] =
                    *(u8 *)(EditorOutModel::MixLookup + color);
            }

            // Advance face write cursor by face data size
            pData += face->outputSize;
        }

        // Advance source read cursor regardless of whether face was copied
        pFace += face->outputSize;
    }

    // ================================================================
    // 6. Write face count into output mesh header and return
    // ================================================================
    self->TargetModel->NumFaces = self->NumFaces;
    return self->Size;
}

/**
 * Restore or hide shared output faces according to their source-face records.
 *
 * Address:   0x80054A18
 * Mangled:   UnhideSharedFaces__14EditorOutModelbP14EditorOutModelUc
 * Demangled: EditorOutModel::UnhideSharedFaces(bool, EditorOutModel *, unsigned char)
 *
 * Match: PERFECT (87/87 instructions, 0 real and 0 relocation mismatches).
 *
 * @param unhide  False clears hidden flags; true reapplies them selectively.
 * @param wire_model  Original API argument; unused by this implementation.
 * @param against  Side-orientation mask used when reapplying hidden flags.
 */
void EditorOutModel::UnhideSharedFaces(
    bool unhide,
    EditorOutModel *wire_model,
    unsigned char against)
{
    GenObjsEditorOutModel *self = (GenObjsEditorOutModel *)this;
    char *pFaceData;
    ModelFace *face1;
    ModelFace2 *face1_info;

    if ((self->SourceModel->flags & 8) == 0)
    {
        pFaceData = (char *)&self->TargetModel->Vertices[
            self->TargetModel->NumVertices + self->TargetModel->NumNormals];
        face1 = self->SourceModel->Faces;
        face1_info = self->Faces;

        if (!unhide)
        {
            for (int j = 0; j < self->NumFaces; j++)
            {
                GenObjsOutputFaceHeader *outputFace =
                    reinterpret_cast<GenObjsOutputFaceHeader *>(pFaceData);
                if (face1_info != 0 && face1_info->sourceFace == face1)
                {
                    if (face1_info->alive == 0)
                        outputFace->flags &= ~0x80;
                    face1_info = face1_info->next;
                }

                pFaceData += outputFace->size;
                face1 = face1->next;
            }
        }
        else
        {
            for (int j = 0; j < self->NumFaces; j++)
            {
                GenObjsOutputFaceHeader *outputFace =
                    reinterpret_cast<GenObjsOutputFaceHeader *>(pFaceData);
                bool is_hidden_face = false;
                bool is_outer_face = false;

                if (face1_info != 0 && face1_info->sourceFace == face1)
                {
                    is_hidden_face = face1_info->alive == 0;
                    is_outer_face = true;
                }

                if (is_hidden_face &&
                    (face1_info->orientation & 1) == (against & 1) &&
                    (face1_info->orientation & 2) != (against & 2))
                {
                    outputFace->flags |= 0x80;
                }

                if (is_outer_face)
                    face1_info = face1_info->next;

                pFaceData += outputFace->size;
                face1 = face1->next;
            }
        }
    }
}

/**
 * __9RailPoint — Constructor for RailPoint
 *
 * Address:   0x80054B74, Size: 36 bytes (9 instructions)
 * Mangled:   __9RailPoint
 * Demangled: RailPoint::RailPoint(void)
 *
 */
RailPoint::RailPoint()
{
    vtable = (int *)&GenObjs_RailPointVTable;
    Next = 0;
    ModelIndex = -1;
    Material = 0;
    IsDoubleLink = 0;
}

/**
 * Compare__9RailPointP9RailPoint — Compare if two RailPoints are within 6 units
 *
 * Address:   0x80054B98, Size: 104 bytes (26 instructions)
 * Mangled:   Compare__9RailPointP9RailPoint
 * Demangled: RailPoint::Compare(RailPoint *)
 *
 * @param point  Candidate point to compare.
 * @return True when all three coordinate differences are within six units.
 */
bool RailPoint::Compare(RailPoint *point)
{
    int dx = Pos.vx - point->Pos.vx;
    int dy = Pos.vy - point->Pos.vy;
    int dz = Pos.vz - point->Pos.vz;

    if ((unsigned int)(dx + 6) >= 13)
        goto fail;
    if (dy < -6)
        goto done;
    if (dy >= 7)
        goto fail;
    if (dz < -6)
        goto done;
    if (dz >= 7)
        goto fail;
    return 1;
fail:;
done:
    return 0;
}

/**
 * Construct an empty generated rail string.
 *
 * Address:   0x80054C00
 * Mangled:   __10RailString
 * Demangled: RailString::RailString(void)
 *
 */
RailString::RailString()
{
    vtable = (int *)&__vt_10RailString;
    PointList = 0;
    Next = 0;
    IsLoop = 0;
    NumPoints = 0;
}

/**
 * Destroy each linked RailPoint owned by this generated rail string.
 *
 * Address:   0x80054C20
 * Mangled:   _._10RailString
 * Demangled: RailString::~RailString()
 */
RailString::~RailString()
{
    int i = 0;
    vtable = (int *)&__vt_10RailString;
    GenObjsRailPoint *point = PointList;
    int count = NumPoints;
    if (count > 0)
    {
        do
        {
            GenObjsRailPoint *next_point = point->Next;
            if (point != 0)
            {
                DestroyCClass(point);
                count = NumPoints;
            }
            i = i + 1;
            point = next_point;
        } while (i < count);
    }
    vtable = (int *)&GenObjs_BaseVTable;
}

/**
 * AddPoint__10RailStringP9RailPointb — Append a RailPoint to the linked list
 * If is_loop is true, stores the loop flag and links the point to the current head.
 * Otherwise sets point->next to null.
 * Then: if list is empty, sets head and count=1.
 * Otherwise walks to tail and appends, incrementing count.
 * Purpose: append a rail point while preserving loop linkage and point count.
 *
 * Address:   0x80054CC4, Size: 116 bytes (29 instructions)
 * Mangled:   AddPoint__10RailStringP9RailPointb
 * Demangled: RailString::AddPoint(RailPoint *, bool)
 *
 * @param new_point  Point appended to the linked list.
 * @param is_loop  Whether the new tail links back to the current head.
 */
void RailString::AddPoint(RailPoint *new_point_, bool is_loop)
{
    GenObjsRailString *self = (GenObjsRailString *)this;
    GenObjsRailPoint *new_point = (GenObjsRailPoint *)new_point_;
    self->IsLoop = (void *)is_loop;
    if (is_loop != 0)
    {
        new_point->Next = self->PointList;
    }
    else
    {
        new_point->Next = 0;
    }

    register GenObjsRailPoint *head asm("$2") = self->PointList;
    GenObjsRailPoint *point = head;
    if (head == 0)
    {
        self->PointList = new_point;
        self->NumPoints = 1;
        return;
    }

    GenObjsRailPoint *next = point->Next;
    if (next != 0)
    {
        GenObjsRailPoint *next2;
        do
        {
            point = next;
            next2 = point->Next;
            next = next2;
        } while (next2 != 0);
    }
    point->Next = new_point;
    self->NumPoints = self->NumPoints + 1;
    return;
}

/**
 * Ensure a closed rail string uses counter-clockwise winding.
 *
 * Address:   0x80054D38
 * Mangled:   MakeCCW__10RailString
 * Demangled: RailString::MakeCCW(void)
 *
 * Match: PERFECT (70/70 instructions, 0 real and 0 relocation mismatches).
 *
 * Operates on this rail string's circular point list.
 */
void RailString::MakeCCW(void)
{
    SVector center;
    GenObjsRailPoint *a;
    int cross;
    GenObjsRailPoint *last;

    if (IsLoop != 0)
    {
        center.vx = 0;
        center.vz = 0;
        a = PointList;

        do
        {
            center.vx += a->Pos.vx;
            center.vz += a->Pos.vz;
            a = a->Next;
        } while (a != PointList);

        center.vx /= NumPoints;
        center.vz /= NumPoints;

        a = PointList;
        do
        {
            GenObjsRailPoint *b = a->Next;
            cross = (a->Pos.vx - center.vx) * (b->Pos.vz - a->Pos.vz) -
                    (a->Pos.vz - center.vz) * (b->Pos.vx - a->Pos.vx);
            if (cross > 0)
                return;
            a = b;
        } while (cross == 0);

        {
            GenObjsRailPoint *next;

            a = PointList;
            last = 0;
            do
            {
                next = a->Next;
                a->Next = last;
                last = a;
                a = next;
            } while (next != PointList);
            next->Next = last;
        }
    }
}

/**
 * CopyString__10RailStringP10RailStringiiii - Clone another RailString's points
 * into this string, rotating X/Z, applying XYZ offsets, preserving flags, and
 * restoring circular linkage before recounting the copied point bounds/count.
 *
 * Address:   0x80054E50
 * Mangled:   CopyString__10RailStringP10RailStringiiii
 * Demangled: RailString::CopyString(RailString *, int, int, int, int)
 *
 * @param from  Source rail string to clone.
 * @param xoff  World-space X offset.
 * @param yoff  World-space Y offset.
 * @param zoff  World-space Z offset.
 * @param rot  Quarter-turn rotation applied in the X/Z plane.
 */
void RailString::CopyString(
    RailString *from_,
    int xoff, int yoff, int zoff, int rot)
{
    GenObjsRailString *self = (GenObjsRailString *)this;
    GenObjsRailString *from = (GenObjsRailString *)from_;
    GenObjsRailPoint *last_copy = 0;
    int i = 0;
    GenObjsRailPoint *source;
    GenObjsRailPoint *copy;

    self->IsLoop = from->IsLoop;
    source = from->PointList;

    if (from->NumPoints <= 0)
        goto done;

    do
    {
        // Allocate and construct a new RailPoint
        copy = (GenObjsRailPoint *)CClass::operator new(sizeof(RailPoint));
        if (copy != 0)
        {
            copy = (GenObjsRailPoint *)::new (copy) RailPoint();
        }

        // Link into list
        copy->Next = 0;
        if (i != 0)
            goto appendPrev;
        self->PointList = copy;
        goto afterLink;
    appendPrev:
        last_copy->Next = copy;
    afterLink:;

        // Rotate coordinates based on rotation parameter
        switch (rot)
        {
        case 0:
        {
            u16 srcX = *(u16 *)&source->Pos.vx;
            copy->Pos.vx = (s16)srcX;
            u16 srcZ = *(u16 *)&source->Pos.vz;
            copy->Pos.vz = (s16)srcZ;
            break;
        }
        case 1:
        {
            u16 srcZ = *(u16 *)&source->Pos.vz;
            copy->Pos.vx = -(s16)srcZ;
            u16 srcX = *(u16 *)&source->Pos.vx;
            copy->Pos.vz = (s16)srcX;
            break;
        }
        case 2:
        {
            u16 srcX = *(u16 *)&source->Pos.vx;
            copy->Pos.vx = -(s16)srcX;
            u16 srcZ = *(u16 *)&source->Pos.vz;
            copy->Pos.vz = -(s16)srcZ;
            break;
        }
        case 3:
        {
            u16 srcZ = *(u16 *)&source->Pos.vz;
            copy->Pos.vx = (s16)srcZ;
            u16 srcX = *(u16 *)&source->Pos.vx;
            copy->Pos.vz = -(s16)srcX;
            break;
        }
        }
        last_copy = copy;
        // Copy Y and add offsets
        {
            u16 srcY = *(u16 *)&source->Pos.vy;
            u16 newX = *(u16 *)&copy->Pos.vx;
            u16 newZ = *(u16 *)&copy->Pos.vz;
            newX = newX + xoff;
            copy->Pos.vy = (s16)srcY;
            srcY = srcY + yoff;
            newZ = newZ + zoff;
            copy->Pos.vx = (s16)newX;
            copy->Pos.vy = (s16)srcY;
            copy->Pos.vz = (s16)newZ;
        }

        // Copy flags
        {
            int flags = source->Material;
            i = i + 1;
            copy->Material = flags;
        }
        source = source->Next;
        if (i >= from->NumPoints)
            break;
    } while (1);

done:
    // If circular, link tail to head
    if (self->IsLoop != 0)
    {
        GenObjsRailPoint *head = self->PointList;
        copy->Next = head;
    }

    CountPoints();
}

/** Minimal polymorphic base used to model CClass assignment behavior. */
class GenObjsAssignableCClass
{
public:
    virtual void AssignmentAnchor();
};

/** RailPoint view whose implicit assignment preserves the destination vtable. */
class GenObjsAssignableRailPoint : public GenObjsAssignableCClass
{
public:
    SVector Pos;
    GenObjsAssignableRailPoint *Next;
    int ModelIndex;
    int Material;
    void *IsDoubleLink;
};

/** RailString view using the assignment-capable RailPoint type. */
class GenObjsAssignableRailString : public GenObjsAssignableCClass
{
public:
    SVector BoundsMin;
    SVector BoundsMax;
    GenObjsAssignableRailPoint *PointList;
    GenObjsAssignableRailString *Next;
    void *IsLoop;
    int NumPoints;
};

typedef char genobjs_assignable_rail_point_size_must_be_28[
    (sizeof(GenObjsAssignableRailPoint) == 28) ? 1 : -1];
typedef char genobjs_assignable_rail_string_size_must_be_32[
    (sizeof(GenObjsAssignableRailString) == 32) ? 1 : -1];

/**
 * Merge a compatible rail string into this string.
 *
 * Open strings are oriented so matching endpoints can be spliced together.
 * Closed strings are merged by locating the corresponding shared spans,
 * deleting their interiors, and cross-linking the remaining endpoints.
 *
 * Address:   0x80055038
 * Mangled:   MergeString__10RailStringP10RailStringb
 * Demangled: RailString::MergeString(RailString *, bool)
 *
 * The destination is the current rail string.
 *
 * @param string_from  String whose points are merged into the destination.
 * @param debugify     Emit diagnostic messages for invalid or successful merges.
 * @return One when a merge was performed; otherwise zero.
 */
bool RailString::MergeString(
    RailString *string_from,
    bool debugify)
{
    GenObjsAssignableRailPoint *from =
        (GenObjsAssignableRailPoint *)string_from->PointList;
    GenObjsAssignableRailPoint *to =
        (GenObjsAssignableRailPoint *)PointList;
    GenObjsAssignableRailPoint *a;
    GenObjsAssignableRailPoint *b;
    GenObjsAssignableRailPoint *to_first;
    GenObjsAssignableRailPoint *to_last;
    GenObjsAssignableRailPoint *from_first;
    GenObjsAssignableRailPoint *from_last;
    bool scanning;
    GenObjsAssignableRailPoint *kill;

    if (debugify)
    {
        if (from == 0)
            printf(s_From_empty_800da584);
        if (to == 0)
            printf(s_To_empty_800da590);
        if (string_from->IsLoop == 0)
            printf(s_from_not_loop_800da59c);
        if (IsLoop == 0)
            printf(s_To_not_loop_800da5ac);
    }

    if (from == 0 || to == 0)
        return 0;

    if (BoundsMin.vx - 6 > string_from->BoundsMax.vx)
        return 0;
    if (string_from->BoundsMin.vx > BoundsMax.vx + 6)
        return 0;
    if (BoundsMin.vy - 6 > string_from->BoundsMax.vy)
        return 0;
    if (string_from->BoundsMin.vy > BoundsMax.vy + 6)
        return 0;
    if (BoundsMin.vz - 6 > string_from->BoundsMax.vz)
        return 0;
    if (string_from->BoundsMin.vz > BoundsMax.vz + 6)
        return 0;

    if (IsLoop == 0)
    {
        if (string_from->IsLoop != 0)
            return 0;

        GenObjsAssignableRailPoint *from_last;
        GenObjsAssignableRailPoint *to_last;

        from_last = from;
        while (from_last->Next != 0)
            from_last = from_last->Next;

        to_last = to;
        while (to_last->Next != 0)
            to_last = to_last->Next;

        if (((RailPoint *)from)->Compare((RailPoint *)to) != 0 ||
            ((RailPoint *)from_last)->Compare((RailPoint *)to_last) != 0)
        {
            GenObjsAssignableRailPoint *prev = 0;

            from_last = from;
            while (from != 0)
            {
                GenObjsAssignableRailPoint *next = from->Next;
                from->Next = prev;
                prev = from;
                from = next;
            }
            from = prev;
        }

        if (((RailPoint *)to)->Compare((RailPoint *)from_last) != 0)
        {
            from_last->Next = to->Next;
            if (to != 0)
            {
                DestroyCClass(to);
            }

            PointList = (GenObjsRailPoint *)from;
            if (((RailPoint *)from)->Compare((RailPoint *)to_last) != 0)
            {
                to_last->Next = from->Next;
                PointList = (GenObjsRailPoint *)from->Next;
                if (from != 0)
                {
                    DestroyCClass(from);
                }
                IsLoop = (void *)1;
            }

            CountPoints();
            return 1;
        }
        else
        {
            if (((RailPoint *)from)->Compare((RailPoint *)to_last) != 0)
            {
                to_last->Next = from->Next;
                if (from != 0)
                {
                    DestroyCClass(from);
                }

                if (((RailPoint *)to)->Compare((RailPoint *)from_last) != 0)
                {
                    from_last->Next = to->Next;
                    PointList = (GenObjsRailPoint *)to->Next;
                    if (to != 0)
                    {
                        DestroyCClass(to);
                    }
                    IsLoop = (void *)1;
                }

                CountPoints();
                return 1;
            }

            return 0;
        }
    }

    if (string_from->IsLoop == 0)
        return 0;

    {
        int j;

        to_first = to_last = 0;
        scanning = false;
        a = to;
        j = 0;
        for (; j < (NumPoints * 2); j++)
        {
            bool is_shared = false;
            {
                int i;

                b = from;
                for (i = 0; i < string_from->NumPoints; i++)
                {
                    if (((RailPoint *)a)->Compare((RailPoint *)b) != 0)
                    {
                        is_shared = true;
                        break;
                    }
                    b = b->Next;
                }
            }

            if (is_shared)
            {
                if (scanning)
                {
                    if (to_first == 0)
                        to_first = a;
                    to_last = a;
                }
            }
            else
            {
                scanning = (to_last == 0);
            }

            a = a->Next;
        }
    }

    if (to_first == to_last)
        return 0;

    {
        int j;

        scanning = false;
        b = from;
        from_first = from_last = 0;
        j = 0;

        for (; j < (string_from->NumPoints * 2); j++)
        {
            a = to;
            bool is_shared = false;
            {
                int i;

                for (i = 0; i < NumPoints; i++)
                {
                    if (((RailPoint *)b)->Compare((RailPoint *)a) != 0)
                    {
                        is_shared = true;
                        break;
                    }
                    a = a->Next;
                }
            }

            if (is_shared)
            {
                if (scanning)
                {
                    if (from_first == 0)
                    {
                        from_first = b;
                        to_last = a;
                    }
                    from_last = b;
                    to_first = a;
                }
            }
            else
            {
                scanning = (from_last == 0);
            }

            b = b->Next;
        }
    }

    if (from_first == from_last)
        return 0;

    if (to_first == to_last)
    {
        to_last = (GenObjsAssignableRailPoint *)CClass::operator new(
            sizeof(GenObjsAssignableRailPoint));
        if (to_last != 0)
            to_last = (GenObjsAssignableRailPoint *)::new (to_last) RailPoint();
        *to_last = *to_first;
        to_first->Next = to_last;
        to_first->IsDoubleLink = (void *)1;
        to_last->IsDoubleLink = (void *)1;
    }

    if (from_first == from_last)
    {
        from_last = (GenObjsAssignableRailPoint *)CClass::operator new(
            sizeof(GenObjsAssignableRailPoint));
        if (from_last != 0)
            from_last =
                (GenObjsAssignableRailPoint *)::new (from_last) RailPoint();
        *from_last = *from_first;
        from_first->Next = from_last;
        from_first->IsDoubleLink = (void *)1;
        from_last->IsDoubleLink = (void *)1;
    }

    kill = to_first->Next;
    while (kill != to_last)
    {
        GenObjsAssignableRailPoint *next = kill->Next;
        if (kill != 0)
        {
            DestroyCClass(kill);
        }
        kill = next;
    }

    kill = from_first->Next;
    while (kill != from_last)
    {
        GenObjsAssignableRailPoint *next = kill->Next;
        if (kill != 0)
        {
            DestroyCClass(kill);
        }
        kill = next;
    }

    to_first->Next = from_last->Next;
    from_first->Next = to_last->Next;

    if (from_last != 0)
    {
        DestroyCClass(from_last);
    }
    if (to_last != 0)
    {
        DestroyCClass(to_last);
    }

    PointList = (GenObjsRailPoint *)to_first;
    CountPoints();

    if (debugify)
        printf(s_Heigh_ho_Did_Merge_800b28f0);
    return 1;
}

/**
 * Split a closed rail loop at the first span bounded by duplicate points.
 * Singleton endpoints are cloned, interior points are destroyed, and the two
 * resulting loops are relinked and recounted independently.
 *
 * Address:   0x800557D0
 * Mangled:   SplitString__10RailString
 * Demangled: RailString::SplitString(void)
 *
 * Match: PERFECT (224/224 instructions, 0 real and 0 relocation mismatches).
 *
 * @return Newly allocated second loop, or null when no valid split is found.
 */
RailString *RailString::SplitString(void)
{
    GenObjsAssignableRailPoint *a;
    void *scanning;
    GenObjsAssignableRailPoint *first1;
    GenObjsAssignableRailPoint *first2;
    GenObjsAssignableRailPoint *last1;
    GenObjsAssignableRailPoint *last2;
    GenObjsAssignableRailPoint *kill;
    GenObjsRailString *new_string;

    if (IsLoop == 0)
        return 0;

    {
        int j;

        scanning = 0;
        first1 = 0;
        first2 = 0;
        last1 = 0;
        j = 0;
        last2 = 0;
        a = (GenObjsAssignableRailPoint *)PointList;

        for (; j < (NumPoints * 2); j++)
        {
            void *is_double = 0;
            GenObjsAssignableRailPoint *b =
                (GenObjsAssignableRailPoint *)PointList;

            if (a->IsDoubleLink == 0)
            {
                int i;

                for (i = 0; i < NumPoints; i++)
                {
                    if (((RailPoint *)a)->Compare((RailPoint *)b) != 0)
                    {
                        if (a != b)
                        {
                            is_double = (void *)1;
                            break;
                        }
                    }
                    b = b->Next;
                }
            }

            if (is_double)
            {
                if (scanning)
                {
                    if (first1 == 0)
                    {
                        first1 = a;
                        last2 = b;
                    }
                    last1 = a;
                    first2 = b;
                }
            }
            else
            {
                scanning = (void *)(last1 == 0);
            }

            a = a->Next;
        }
    }

    if (first1 == 0)
        return 0;
    if (last1 == 0)
        return 0;

    if (first1 == last1)
    {
        last1 = (GenObjsAssignableRailPoint *)CClass::operator new(
            sizeof(GenObjsAssignableRailPoint));
        if (last1 != 0)
            last1 = (GenObjsAssignableRailPoint *)::new (last1) RailPoint();
        *last1 = *first1;
        first1->Next = last1;
    }
    if (first2 == last2)
    {
        last2 = (GenObjsAssignableRailPoint *)CClass::operator new(
            sizeof(GenObjsAssignableRailPoint));
        if (last2 != 0)
            last2 = (GenObjsAssignableRailPoint *)::new (last2) RailPoint();
        *last2 = *first2;
        first2->Next = last2;
    }

    kill = first1->Next;
    while (kill != last1)
    {
        GenObjsAssignableRailPoint *next = kill->Next;
        if (kill != 0)
        {
            DestroyCClass(kill);
        }
        kill = next;
    }

    kill = first2->Next;
    while (kill != last2)
    {
        GenObjsAssignableRailPoint *next = kill->Next;
        if (kill != 0)
        {
            DestroyCClass(kill);
        }
        kill = next;
    }

    new_string = (GenObjsRailString *)CClass::operator new(
        sizeof(GenObjsRailString));
    if (new_string != 0)
        new_string = (GenObjsRailString *)::new (new_string) RailString();

    new_string->PointList = (GenObjsRailPoint *)first2;
    first2->Next = last1->Next;
    if (last1 != 0)
    {
        DestroyCClass(last1);
    }
    new_string->IsLoop = (void *)1;
    ((RailString *)new_string)->CountPoints();

    PointList = (GenObjsRailPoint *)first1;
    first1->Next = last2->Next;
    if (last2 != 0)
    {
        DestroyCClass(last2);
    }
    CountPoints();
    return (RailString *)new_string;
}

/**
 * Recount a rail string's points and rebuild its axis-aligned bounds.
 *
 * Address:   0x80055B50
 * Mangled:   CountPoints__10RailString
 * Demangled: RailString::CountPoints(void)
 *
 * Match: PERFECT (72/72 instructions, 0 real and 0 relocation mismatches).
 *
 * @return Number of points encountered before the list ends or wraps to its head.
 */
int RailString::CountPoints(void)
{
    GenObjsRailString *self = (GenObjsRailString *)this;
    GenObjsRailPoint *point = self->PointList;

    self->NumPoints = 0;
    self->BoundsMin.vx = 10000;
    self->BoundsMin.vy = 10000;
    self->BoundsMin.vz = 10000;
    self->BoundsMax.vx = -10000;
    self->BoundsMax.vy = -10000;
    self->BoundsMax.vz = -10000;
    do
    {
        if (point == 0)
            break;

        if (point->Pos.vx < self->BoundsMin.vx)
            self->BoundsMin.vx = point->Pos.vx;
        if (point->Pos.vx > self->BoundsMax.vx)
            self->BoundsMax.vx = point->Pos.vx;

        if (point->Pos.vy < self->BoundsMin.vy)
            self->BoundsMin.vy = point->Pos.vy;
        if (point->Pos.vy > self->BoundsMax.vy)
            self->BoundsMax.vy = point->Pos.vy;

        if (point->Pos.vz < self->BoundsMin.vz)
            self->BoundsMin.vz = point->Pos.vz;
        if (point->Pos.vz > self->BoundsMax.vz)
            self->BoundsMax.vz = point->Pos.vz;

        self->NumPoints++;
        point = point->Next;
    } while (point != self->PointList);
    return self->NumPoints;
}

/**
 * Construct an empty generated rail collection.
 *
 * Address:   0x80055C70
 * Mangled:   __8RailInfo
 * Demangled: RailInfo::RailInfo(void)
 *
 * @param self  Constructor receiver represented by the owner-local layout.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
GenObjsRailInfo *ConstructRailInfo(GenObjsRailInfo *self)
{
    self->CClass.vtable = (int *)&__vt_8RailInfo;
    self->StringList = 0;
    return self;
}

/**
 * Destroy the owned rail strings and optionally release the RailInfo object.
 *
 * Address:   0x80055C84
 * Mangled:   _._8RailInfo
 * Demangled: RailInfo::~RailInfo()
 *
 * @param self  Rail collection being destroyed.
 * @param flag  GCC 2.x in-charge flag (bit 0 → also invoke `operator delete` on `this`).
 */
void RailInfoDestructor(GenObjsRailInfo *self, int flag)
{
    self->CClass.vtable = (int *)&__vt_8RailInfo;
    reinterpret_cast<RailInfo *>(self)->Clear();
    self->CClass.vtable = (int *)&GenObjs_BaseVTable;
    if (flag & 1)
    {
        CClass::operator delete(self);
    }
}

/**
 * NewString__8RailInfo — Allocate a new RailString and append to linked list
 * Allocates and constructs a RailString, records it as CurrentString, and
 * either initializes StringList or appends it to the tail.
 *
 * Address:   0x80055CD4, Size: 136 bytes (34 instructions)
 * Mangled:   NewString__8RailInfo
 * Demangled: RailInfo::NewString(void)
 *
 * @param self  Rail collection receiving the new string.
 * @return Newly constructed rail string.
 */
RailString *RailInfo::NewString()
{
    GenObjsRailInfo *self = reinterpret_cast<GenObjsRailInfo *>(this);
    GenObjsRailString *string;

    // Allocate and construct
    string = (GenObjsRailString *)CClass::operator new(
        sizeof(GenObjsRailString));
    if (string != 0)
    {
        string = (GenObjsRailString *)::new (string) RailString();
    }

    {
        register GenObjsRailString *next asm("$3") = self->StringList;
        self->CurrentString = string;

        if (next == 0)
        {
            self->StringList = string;
        }
        else
        {
            GenObjsRailString *node = next;
            next = node->Next;
            if (next != 0)
            {
                do
                {
                    node = next;
                    string = node->Next;
                    next = string;
                } while (string != 0);
            }
            next = self->CurrentString;
            node->Next = next;
            string = next;
        }
    }
    return reinterpret_cast<RailString *>(string);
}

/**
 * Clear__8RailInfo — Destroy all RailString nodes in the linked list
 * Walks StringList, calls the deleting virtual destructor on each node, then
 * clears the list head.
 *
 * Address:   0x80055D5C, Size: 108 bytes (27 instructions)
 * Mangled:   Clear__8RailInfo
 * Demangled: RailInfo::Clear(void)
 *
 * @param self  Rail collection to empty.
 */
void RailInfo::Clear()
{
    GenObjsRailInfo *self = reinterpret_cast<GenObjsRailInfo *>(this);
    GenObjsRailString *string = self->StringList;
    GenObjsRailString *next_string;

    while (string != 0)
    {
        next_string = string->Next;
        if (string != 0)
        {
            DestroyCClass(string);
        }
        string = next_string;
    }
    self->StringList = 0;
}

/**
 * CountStrings__8RailInfo — Count entries in StringList
 *
 * Address:   0x80055DC8, Size: 40 bytes (10 instructions)
 * Mangled:   CountStrings__8RailInfo
 * Demangled: RailInfo::CountStrings(void)
 *
 * @param self  Rail collection to inspect.
 * @return Number of rail strings in the collection.
 */
int RailInfo::CountStrings()
{
    GenObjsRailInfo *self = reinterpret_cast<GenObjsRailInfo *>(this);
    GenObjsRailString *string = self->StringList;
    int count = 0;
    while (string != 0)
    {
        string = string->Next;
        count++;
    }
    return count;
}

/**
 * CountPoints__8RailInfo — Sum CountPoints for all RailStrings in this RailInfo
 * Walks StringList and recounts each member, returning the combined total.
 *
 * Address:   0x80055DF0, Size: 80 bytes (20 instructions)
 * Mangled:   CountPoints__8RailInfo
 * Demangled: RailInfo::CountPoints(void)
 *
 * @param self  Rail collection to inspect.
 * @return Combined number of rail points.
 */
int RailInfo::CountPoints()
{
    GenObjsRailInfo *self = reinterpret_cast<GenObjsRailInfo *>(this);
    GenObjsRailString *string = self->StringList;
    int count = 0;
    while (string != 0)
    {
        count += ((RailString *)string)->CountPoints();
        string = string->Next;
    }
    return count;
}

/**
 * Merge a candidate rail-string list into this rail collection where possible.
 *
 * Address:   0x80055E40
 * Mangled:   MergeAdd__8RailInfoP10RailStringb
 * Demangled: RailInfo::MergeAdd(RailString *, bool)
 *
 * @param railInfo  Implicit `this` pointer represented by the owner-local layout.
 * @param new_string  First candidate string to merge or append.
 * @param debugify  Whether to print merge and append diagnostics.
 */
void RailInfo::MergeAdd(RailString *string_to_merge, bool debugify)
{
    GenObjsRailInfo *railInfo = reinterpret_cast<GenObjsRailInfo *>(this);
    GenObjsRailString *new_string =
        reinterpret_cast<GenObjsRailString *>(string_to_merge);
    GenObjsRailString *merge_list = new_string;

    merge_list->Next = 0;
    while (merge_list != 0)
    {
        GenObjsRailString *merge_list_next = merge_list->Next;
        bool repeat_merge = true;
        while (repeat_merge)
        {
            GenObjsRailString *string;
            GenObjsRailString *prev;
            GenObjsRailString *head = railInfo->StringList;
            if (head == 0)
            {
                if (debugify)
                    printf(s_Added_string_800da5bc);
                railInfo->StringList = merge_list;
                merge_list->Next = 0;
                break;
            }
            else
            {
                string = head;
                prev = 0;
                while (string != 0)
                {
                    if (((RailString *)merge_list)->MergeString(
                            (RailString *)string, debugify) != 0)
                    {
                        if (debugify)
                            printf(s_Merged_string_800da5cc);
                        if (prev != 0)
                            prev->Next = string->Next;
                        else
                            railInfo->StringList = string->Next;
                        string->PointList = 0;
                        if (string != 0)
                            DestroyCClass(string);
                        GenObjsRailString *branch = (GenObjsRailString *)
                            ((RailString *)merge_list)->SplitString();
                        if (branch != 0)
                        {
                            branch->Next = merge_list_next;
                            merge_list_next = branch;
                        }
                        break;
                    }

                    prev = string;
                    if (string->Next == 0)
                    {
                        if (debugify)
                            printf(s_Added_string_800da5bc);
                        repeat_merge = false;
                        string->Next = merge_list;
                        merge_list->Next = 0;
                        break;
                    }
                    string = string->Next;
                }
            }
        }
        merge_list = merge_list_next;
    }
}


/**
 * Destroy a generated rail point and optionally release its storage.
 *
 * Address:   0x80055FC0
 * Mangled:   _._9RailPoint
 * Demangled: RailPoint::~RailPoint()
 *
 * This target destructor is local even though the constructor and Compare
 * method are exported.  A natural out-of-line destructor makes GCC export the
 * symbol, so this addressable ABI implementation preserves the proven local
 * binding used by the private vtable.
 *
 * @param self Rail point being destroyed.
 * @param in_charge GCC 2.x in-charge flag; bit 0 releases the object.
 */
static void GenObjs_DestroyRailPoint(RailPoint *self, int in_charge)
{
    self->vtable = &GenObjs_BaseVTable;
    if (in_charge & 1)
    {
        CClass::operator delete(self);
    }
}


/**
 * Destroy GenObjs.obj's private CClass base and optionally release storage.
 *
 * Address:   0x80055FF0
 * Mangled:   _._6CClass
 *
 * @param self Base object being destroyed.
 * @param in_charge GCC 2.x in-charge flag; bit 0 releases the object.
 */
static void GenObjs_DestroyClass(CClass *self, int in_charge)
{
    self->vtable = &GenObjs_BaseVTable;
    if (in_charge & 1)
    {
        CClass::operator delete(self);
    }
}
