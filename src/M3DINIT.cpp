// Renderer initialization and loaded-model runtime fixups.

#define M3DINIT_OWNER_DECLARATIONS
#include "M3DINIT.h"
#undef M3DINIT_OWNER_DECLARATIONS
// Owner-local layouts used by M3DINIT.cpp.
//
// These records are kept local to the PSX model initializer until their
// definitions are shared cleanly across every owning translation unit.  The
// field names and layouts come from original/MAIN_STRUCTS.h.

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed short s16;

#include "PSXREGION.h"
#include <STDDEF.H>
#include <LIBGTE.H>
#include "SMODEL.h"

// These macros preserve the target compiler's literal integer types while
// giving the serialized model flags readable source names.
#define M3DINIT_VERTEX_STITCH_SOURCE 0x01
#define M3DINIT_VERTEX_STITCHED 0x02
#define M3DINIT_VERTEX_SPRITE 0x10
#define M3DINIT_FACE_TEXTURED 0x03
#define M3DINIT_FACE_HAS_UVS 0x04
#define M3DINIT_FACE_TRIANGLE 0x10
#define M3DINIT_FACE_ANIMATED_TEXTURE 0x20
#define M3DINIT_FACE_SEMITRANSPARENT 0x40
#define M3DINIT_FACE_STP 0x80
#define M3DINIT_FACE_TRANSPARENCY_MASK 0xc0
#define M3DINIT_FACE_GOURAUD 0x800
#define M3DINIT_FACE_ENVIRONMENT_MAPPED 0x1000
#define M3DINIT_RGB_MASK 0xffffff
#define M3DINIT_MODEL_HAS_UVS 0x04
#define M3DINIT_MODEL_ALL_TEXTURED 0x10
#define M3DINIT_MODEL_OPAQUE 0x20
#define M3DINIT_MODEL_ENVIRONMENT_MAPPED 0x40
#define M3DINIT_MODEL_HAS_SPRITES 0x100
#define M3DINIT_ITEM_ALL_TEXTURED 0x20
#define M3DINIT_ITEM_OPAQUE 0x1000

/** Absolute display globals emitted without cross-object relocations. */
enum M3dInitDisplayAddress {
    M3dInit_XresAddress = 0x800DC5FCu,
    M3dInit_YresAddress = 0x800DC600u,
    M3dInit_PixelAspectXAddress = 0x800DC604u,
    M3dInit_PixelAspectYAddress = 0x800DC608u
};

static inline u32 *M3dInit_GetXresStorage()
{
    return reinterpret_cast<u32 *>(M3dInit_XresAddress);
}

static inline u32 *M3dInit_GetYresStorage()
{
    return reinterpret_cast<u32 *>(M3dInit_YresAddress);
}

static inline int *M3dInit_GetPixelAspectXStorage()
{
    return reinterpret_cast<int *>(M3dInit_PixelAspectXAddress);
}

static inline int *M3dInit_GetPixelAspectYStorage()
{
    return reinterpret_cast<int *>(M3dInit_PixelAspectYAddress);
}

#define M3D_VERTEX_WIBBLE_LAYOUTS
#include "M3D.h"
#undef M3D_VERTEX_WIBBLE_LAYOUTS

/** The shared vertex scratch buffer is interpreted in either 8-byte form. */
union M3dInitVertexBufferEntry {
    SProjectedVertex Projected;
    SRotatedVertex Rotated;
};

// The target advances a cursor based at vAmpPhase, so the U phase is the
// preceding byte.  Direct STexWibVertInfo fields regress this function by 76
// real instructions and one relocation; keep the proven interior-base shape
// behind named accessors.
#define M3DINIT_U_AMPLITUDE_PHASE(cursor) ((cursor)[-1])
#define M3DINIT_V_AMPLITUDE_PHASE(cursor) ((cursor)[0])

static inline STexWibVertInfo *M3dInit_GetFirstWibVertex(
    STexWibItemInfo *pTexWibItemInfo)
{
    return reinterpret_cast<STexWibVertInfo *>(pTexWibItemInfo + 1);
}

static inline u8 *M3dInit_GetAmplitudeCursor(
    STexWibItemInfo *pTexWibItemInfo)
{
    return &M3dInit_GetFirstWibVertex(pTexWibItemInfo)->vAmpPhase;
}

static inline STexWibItemInfo *M3dInit_GetNextWibItem(
    STexWibVertInfo *pTexWibVertInfo)
{
    return reinterpret_cast<STexWibItemInfo *>(pTexWibVertInfo);
}

extern M3dInitVertexBufferEntry Vertices[];
/** Shared 1024-entry depth-scaled sine table. */
#define WIBBLETABLES_CONSUMER_DECLARATION
#include "WIBBLETABLES.h"
#include "RCOSSIN.h"
extern int GBoardPart, GWheel1Part, GWheel2Part;
/**
 * Absolute-address views of globals physically owned by M3D's small-data
 * contribution.  Without these per-TU addressing attributes, InitAtStart,
 * SetFoggingParams, and ParsePSX regress by 38+1, 40+0, and 284+2
 * real/relocation mismatches respectively.
 */
#define DPQGLOBALS_DECLARE_SHIFT
#include "DPQGLOBALS.h"
#define M3DINITSTATE_ABSOLUTE_DECLARATION
#define M3DINITSTATE_DECLARE_DEFAULT_LOD_DISTANCE
#include "M3DINITSTATE.h"
#define M3DCOLOURSTATE_PLAIN_LINKAGE
#define M3DCOLOURSTATE_DECLARE_FADE_COLOUR
#include "M3DCOLOURSTATE.h"
#include "PROJECTEDVERTICES.h"
#define M3DINITSTATE_ABSOLUTE_DECLARATION
#define M3DINITSTATE_DECLARE_ROTATED_VERTICES
#include "M3DINITSTATE.h"

/** Return the item count stored in a loaded PSX packet header. */
static inline int M3dInit_GetItemCount(u32 *pPSX)
{
    return pPSX[2];
}

/** Return the first item following a loaded PSX packet header. */
static inline Item *M3dInit_GetFirstItem(u32 *pPSX)
{
    return reinterpret_cast<Item *>(pPSX + 3);
}

struct tag_S_Pal;

#include "TEXTURE.h"

/** Minimum common layout of a variable-length model face (32 bytes). */
struct M3dInitFace {
    u32 Flags;
    u8 VertexIndex[4];
    u32 Colour;
    u16 PackedUV;
    u16 FunctionFlags;
    union {
        Texture *pTexture;
        u32 ClutAndUV0;
    };
    union {
        struct {
            u16 UV0;
            u16 UV1;
        } UV01;
        u32 TPageAndUV1;
    };
    union {
        struct {
            u16 UV2;
            u16 UV3;
        } UV23;
        u32 UV2AndUV3;
    };
    u32 TextureWindow;
};

/** View the current word cursor as the common prefix of a model face. */
static inline M3dInitFace *M3dInit_GetFace(u32 *pFace)
{
    return reinterpret_cast<M3dInitFace *>(pFace);
}

/** Return the unsigned type halfword stored in an SDK vertex's pad field. */
static inline u16 *M3dInit_GetVertexType(SVECTOR *pVertex)
{
    return reinterpret_cast<u16 *>(&pVertex->pad);
}

/** Return the first face word after a model's normal array. */
static inline u32 *M3dInit_GetFirstFace(SVECTOR *pVertex, int NumNormals)
{
    return reinterpret_cast<u32 *>(pVertex + NumNormals);
}

/** View the end of one variable-length face list as the next model header. */
static inline SModel *M3dInit_GetNextModel(u32 *pFace)
{
    return reinterpret_cast<SModel *>(pFace);
}

/** Return the model count stored immediately before a region's model table. */
static inline int M3dInit_GetModelCount(SModel **ppModels)
{
    return reinterpret_cast<int *>(ppModels)[-1];
}

typedef char M3dInit_Texture_size_must_be_40[(sizeof(Texture) == 40) ? 1 : -1];
typedef char M3dInit_Face_size_must_be_32[
    (sizeof(M3dInitFace) == 32) ? 1 : -1];
typedef char M3dInit_VertexBufferEntry_size_must_be_8[
    (sizeof(M3dInitVertexBufferEntry) == 8) ? 1 : -1];

/** Previous fog maximum retained while camera interpolation is active. */
int LastDpqMax __attribute__((section(".sdata"))) = 0;

/** Previous fog minimum retained while camera interpolation is active. */
int LastDpqMin __attribute__((section(".sdata"))) = 0;

/** Previous fog shift retained alongside the interpolation endpoints. */
int LastDpqShift __attribute__((section(".sdata"))) = 0;

// === Function implementations (address order) ===

/**
 * Initializes the renderer's scratch buffers, display geometry, fog defaults,
 * and depth-scaled sine table.
 *
 * The shared vertex buffer is split into 400 projected and 400 rotated entries.
 * WibbleTables receives 16 amplitude rows of 64 sine samples each.
 */
void M3dInit_InitAtStart(void) {
    ProjectedVertices = &Vertices[0].Projected;
    RotatedVertices = &Vertices[400].Rotated;
    M3dInit_SetResolution(512, 240);
    M3dInit_SetFoggingParams(0, 6000, 2048);

    {
        int Amp = 0;
        s16 *pHalfword = WibbleTables;
        // This target loop keeps the table base in $t0.  An ordinary base,
        // a direct rcossin_tbl cursor, and a resettable outer cursor regress
        // by 2, 23, and 13 real instructions respectively (the last also
        // changes one relocation), so the inherited fixed binding remains.
        register s16 *sinBase asm("$8") = rcossin_tbl;
        do {
            int Angle = 0;
            s16 *sinPtr = sinBase;
            do {
                *pHalfword = static_cast<s16>((*sinPtr * Amp) >> 4);
                sinPtr += 128;
                Angle += 64;
                pHalfword++;
            } while (Angle < 4096);
            Amp++;
        } while (Amp < 16);
    }

    M3d_FadeColour = 0x80000000;
}

/**
 * Stores the display dimensions and derives the 4.12 fixed-point pixel aspect.
 *
 * @param X Display width in pixels.
 * @param Y Display height in pixels.
 */
void M3dInit_SetResolution(u32 X, u32 Y) {
    u32 widthTimes3 = (X << 1) + X;
    u32 heightTimes4 = Y << 2;
    u32 *xres = M3dInit_GetXresStorage();
    u32 *yres = M3dInit_GetYresStorage();
    int *px = M3dInit_GetPixelAspectXStorage();
    int *py = M3dInit_GetPixelAspectYStorage();

    *xres = X;
    *yres = Y;
    if (heightTimes4 < widthTimes3) {
        *px = 4096;
        *py = (widthTimes3 << 12) / heightTimes4;
    } else {
        *py = 4096;
        *px = (Y << 14) / widthTimes3;
    }
}

/**
 * Stores the fog endpoints and normalizes the range around 4096 while
 * recording the corresponding binary shift.
 *
 * @param Dummy Unused legacy near-plane argument.
 * @param Min Fog minimum distance.
 * @param Range Fog distance range.
 */
void M3dInit_SetFoggingParams(int Dummy, int Min, unsigned int Range) {
    DpqMin = Min;
    Min += Range;
    DpqMax = Min;
    DpqShift = 0;

    if (Range < 4096) {
        do {
            Range <<= 1;
            DpqShift = DpqShift + 1;
        } while (Range < 4096);
    }

    while (Range > 4096) {
        Range >>= 1;
        DpqShift = DpqShift - 1;
    }

    LastDpqMax = DpqMax;
    LastDpqMin = DpqMin;
    LastDpqShift = DpqShift;
}

/**
 * Applies the runtime representation fixups to a newly loaded PSX region.
 * Called after ProcessNewPSX loads a PSX file into RAM.
 * Iterates over all meshes in the file, modifying vertices and faces IN PLACE
 * for runtime rendering. The on-disk format is the pre-modification layout.
 * VERTEX PROCESSING (per mesh):
 *   - Type bit 0 (0x01): Stitch source vertex — counted for fast-path threshold
 *   - Type bit 1 (0x02): Stitched vertex — X replaced with Y<<3 (byte offset
 *     into stitch buffer), Y zeroed. After this function, the vertex X field
 *     holds a byte offset to the source SVECTOR it will copy from at runtime.
 *   - Type bit 4 (0x10): Sprite vertex — sets mesh flag 0x100 (has sprites)
 *   - GHeadPart special case: enlarges head mesh vertices for character select
 *     screen (x*3, y*2 + GHeadUp*16, z*3; if GHeadUp<0 also z -= 240)
 * NORMAL PROPAGATION (when NumNormals == NumVertices + NumFaces):
 *   Per-vertex normals present: propagate stitch type flags from vertices to
 *   their corresponding normals. Type-2 normals get vertex X (byte offset) and
 *   Y zeroed, matching the vertex transformation.
 * FACE PROCESSING (per mesh):
 *   - UV precision: shift PackedUV for VRAM addressing
 *   - STP bit fixup: if face not semi-transparent (bit 6 clear), toggle bit 7
 *   - Geometry cleanup: if not gouraud (bit 11 clear), mask Colour to 24-bit
 *   - Textured faces (bits 0+1 set):
 *     - Animated (bit 5 set): copy the texture-window word
 *     - Static (bit 5 clear): add base U offset from texture entry to all 4 UVs
 *   - Triangle (bit 4 set): copy vertex index 2 to position 3 (degenerate quad)
 *   - Face stride: upper 14 bits of dword 0, >>18 then <<2 = byte stride
 * MESH FLAGS ACCUMULATED:
 *   0x10: all faces have bit 0 set (all textured — can skip untextured path)
 *   0x20: no faces have bits 6|7 set (fully opaque — skip transparency sort)
 *   0x40: at least one face has bit 12 (environment/reflection mapping present)
 *   0x04: at least one face has bit 2 set (UV coordinates present)
 * ITEM SETUP (after all meshes):
 *   Links item entries into a forward-linked list, stores the region slot index
 *   in each item, and propagates mesh flags → item flags:
 *     mesh 0x10 (all-textured fast path) → item 0x20
 *     mesh 0x20 (fully opaque) → item 0x1000
 * @param Region  PSX region index.
 */
void M3dInit_ParsePSX(int Region) {
    Item *pItem;
    int NumItems;
    u16 vtxType;
    int NumNormals; // Mesh normal count
    u32 *pFace;
    s16 *vtxYPtr;  // Pointer to vertex Y field
    SVECTOR *pVertex;
    int NumVertices; // Mesh vertex count
    // Individually unbinding vtxTypePtr, j, NumStitchVertices, and i changes
    // ParsePSX by 48+1, 44+1, 44+1, and 16+0 real/relocation mismatches.
    // Their ordinary typed forms were restored after those measured attempts.
    register int j asm("$10");
    int NumFaces;  // Mesh face count
    Texture *pTex;
    u16 clut;
    u16 tpage;
    u16 u0v0;
    u16 u1v1;
    u16 u2v2;
    u16 u3v3;
    u32 OrOfFlags;
    unsigned int AndOfFUFlags;

    // Select the part indices used by the loaded model version.
    if (GHeadPart != -1) {
        if (GHeadPart == 18) {
            GBoardPart = 0;
            GWheel1Part = 1;
            GWheel2Part = 2;
        } else {
            GBoardPart = 16;
            GWheel1Part = 17;
            GWheel2Part = 18;
        }
    }

    // The model count precedes ppModels; ppModels[0] starts the contiguous mesh data.
    register int NumStitchVertices asm("$16") = 0;
    unsigned int Fastable = 1;
    int NumParts = 0;
    int NumModels = M3dInit_GetModelCount(PSXRegion[Region].ppModels);
    register int i asm("$14") = 0;
    SModel *pModel = PSXRegion[Region].ppModels[0];

    if (0 < NumModels) {
        do {
            OrOfFlags = 0;
            AndOfFUFlags = -1;
            NumVertices = pModel->NumVertices;
            NumNormals = pModel->NumNormals;
            NumFaces = pModel->NumFaces;

            if (pModel->NextLOD == 0xffff) {
                NumParts = NumParts + 1;
            }
            j = 0;
            // A zMax of -1 requests the default LOD distance.
            if (pModel->zMax == AndOfFUFlags) {
                pModel->zMax = M3d_DefaultLODDistance;
            }
            pVertex = pModel->Vertices;

            // VERTEX LOOP
            if (NumVertices != 0) {
                int headUp = GHeadUp;
                register u16 *vtxTypePtr asm("$8") =
                    M3dInit_GetVertexType(pVertex);
                vtxYPtr = &pVertex->vy;
                do {
                    // Head-part enlargement for character-select screen.
                    if ((i == GHeadPart) &&
                        ((*vtxTypePtr & (M3DINIT_VERTEX_STITCHED |
                            M3DINIT_VERTEX_SPRITE)) == 0)) {
                        pVertex->vx = pVertex->vx * 3;
                        {
                            int y2 = *vtxYPtr * 2;
                            int z3 = vtxYPtr[1] * 3;
                            *vtxYPtr = y2;                               // y = y*2
                            y2 = y2 + headUp * 16;
                            vtxYPtr[1] = z3;                             // z = z*3
                            *vtxYPtr = y2;                               // y = y*2 + headUp*16
                            if (headUp < 0) {
                                vtxYPtr[1] = z3 - 240;
                            }
                        }
                    }

                    vtxType = *vtxTypePtr;

                    // Type bit1 (0x02): stitched — X = Y<<3 (byte offset), Y = 0.
                    if ((vtxType & M3DINIT_VERTEX_STITCHED) != 0) {
                        pVertex->vx = *vtxYPtr << 3;
                        *vtxYPtr = 0;
                        vtxType = *vtxTypePtr;
                    }

                    // Type bit4 (0x10): sprite — flag the mesh.
                    if ((vtxType & M3DINIT_VERTEX_SPRITE) != 0) {
                        pModel->Flags =
                            pModel->Flags | M3DINIT_MODEL_HAS_SPRITES;
                        vtxType = *vtxTypePtr;
                    }

                    // Type bit0 (0x01): stitch source — count for fast-path threshold.
                    if ((vtxType & M3DINIT_VERTEX_STITCH_SOURCE) != 0) {
                        NumStitchVertices = NumStitchVertices + 1;
                    }

                    j = j + 1;
                    vtxYPtr = vtxYPtr + 4;
                    vtxTypePtr = vtxTypePtr + 4;
                    pVertex++;
                } while (j < NumVertices);
            }

            if (100 < NumVertices + NumStitchVertices) {
                Fastable = 0;
            }

            // NORMAL STITCH PROPAGATION when normals cover vertices and faces.
            if (NumNormals == NumFaces + NumVertices) {
                // Rewind to the vertex start, then finish at the face-data boundary.
                SVECTOR *pNormal = pVertex;
                pVertex = pVertex - NumVertices;
                j = 0;
                if (NumVertices != 0) {
                    u16 *vT = M3dInit_GetVertexType(pVertex);
                    s16 *nY = &pNormal->vy;
                    u16 *nT = M3dInit_GetVertexType(pNormal);
                    do {
                        if ((*vT & M3DINIT_VERTEX_STITCH_SOURCE) != 0) {
                            *nT = *nT | M3DINIT_VERTEX_STITCH_SOURCE;
                        }
                        if ((*vT & M3DINIT_VERTEX_STITCHED) != 0) {
                            *nT = *nT | M3DINIT_VERTEX_STITCHED;
                            pNormal->vx = pVertex->vx;
                            *nY = 0;               // zero normal Y
                        }
                        j = j + 1;
                        nT = nT + 4;
                        nY = nY + 4;
                        pNormal++;
                        vT = vT + 4;
                        pVertex++;
                    } while (j < NumVertices);
                }
            }

            // FACE LOOP
            pFace = M3dInit_GetFirstFace(pVertex, NumNormals);
            j = 0;
            if (NumFaces != 0) {
                do {
                    M3dInitFace *pCurrentFace = M3dInit_GetFace(pFace);
                    u32 faceWord = pCurrentFace->Flags;

                    pCurrentFace->PackedUV =
                        static_cast<u16>(pCurrentFace->PackedUV << 3);

                    // Toggle the STP bit on opaque faces.
                    if ((faceWord & M3DINIT_FACE_SEMITRANSPARENT) == 0) {
                        pCurrentFace->Flags = faceWord ^ M3DINIT_FACE_STP;
                        faceWord = pCurrentFace->Flags;
                    }

                    if ((faceWord & M3DINIT_FACE_GOURAUD) == 0) {
                        pCurrentFace->Colour =
                            pCurrentFace->Colour & M3DINIT_RGB_MASK;
                        faceWord = pCurrentFace->Flags;
                    }

                    // Textured face (bits 0 and 1).
                    if ((faceWord & M3DINIT_FACE_TEXTURED) ==
                        M3DINIT_FACE_TEXTURED) {
                        u0v0 = pCurrentFace->UV01.UV0;
                        u1v1 = pCurrentFace->UV01.UV1;
                        u2v2 = pCurrentFace->UV23.UV2;
                        pTex = pCurrentFace->pTexture;
                        u3v3 = pCurrentFace->UV23.UV3;
                        clut = pTex->clut;
                        tpage = pTex->tpage;

                        if ((faceWord & M3DINIT_FACE_ANIMATED_TEXTURE) != 0) {
                            pCurrentFace->TextureWindow = pTex->TexWin;
                        } else {
                            u16 baseUV =
                                *reinterpret_cast<u16 *>(&pTex->u0);
                            u0v0 = u0v0 + baseUV;
                            u1v1 = u1v1 + baseUV;
                            u2v2 = u2v2 + baseUV;
                            u3v3 = u3v3 + baseUV;
                        }
                        pCurrentFace->ClutAndUV0 =
                            u0v0 | static_cast<u32>(clut) << 16;
                        faceWord = pCurrentFace->Flags;
                        pCurrentFace->TPageAndUV1 =
                            u1v1 | static_cast<u32>(tpage) << 16;
                        pCurrentFace->UV2AndUV3 =
                            u2v2 | static_cast<u32>(u3v3) << 16;
                    }

                    OrOfFlags = OrOfFlags | faceWord;
                    AndOfFUFlags =
                        AndOfFUFlags & pCurrentFace->FunctionFlags;

                    if ((faceWord & M3DINIT_FACE_TRIANGLE) != 0) {
                        pCurrentFace->VertexIndex[3] =
                            pCurrentFace->VertexIndex[2];
                        faceWord = pCurrentFace->Flags;
                    }

                    pFace = pFace + (faceWord >> 18);
                    j = j + 1;
                } while (j < NumFaces);
            }

            // SET MESH FLAGS from accumulated face info.
            if ((AndOfFUFlags & 1) != 0) {
                pModel->Flags = pModel->Flags | M3DINIT_MODEL_ALL_TEXTURED;
            }
            if ((OrOfFlags & M3DINIT_FACE_TRANSPARENCY_MASK) == 0) {
                pModel->Flags = pModel->Flags | M3DINIT_MODEL_OPAQUE;
            }
            if ((OrOfFlags & M3DINIT_FACE_ENVIRONMENT_MAPPED) != 0) {
                pModel->Flags =
                    pModel->Flags | M3DINIT_MODEL_ENVIRONMENT_MAPPED;
            }
            if ((OrOfFlags & M3DINIT_FACE_HAS_UVS) != 0) {
                pModel->Flags = pModel->Flags | M3DINIT_MODEL_HAS_UVS;
            }
            i = i + 1;
            pModel = M3dInit_GetNextModel(pFace);
        } while (i < NumModels);
    }

    // STORE RESULTS and ITEM SETUP.
    PSXRegion[Region].Fastable = Fastable;
    PSXRegion[Region].NumParts = NumParts;

    i = 0;
    NumItems = M3dInit_GetItemCount(PSXRegion[Region].pPSX);
    pItem = M3dInit_GetFirstItem(PSXRegion[Region].pPSX);
    if (0 < NumItems) {
        do {
            if (i < NumItems - 1) {
                pItem->NextItem = pItem + 1;
            } else {
                pItem->NextItem = 0;
            }
            pItem->Region = static_cast<u8>(Region);
            if ((PSXRegion[pItem->Region].ppModels[pItem->Model]->Flags &
                    M3DINIT_MODEL_ALL_TEXTURED) != 0) {
                pItem->Flags |= M3DINIT_ITEM_ALL_TEXTURED;
            }
            if ((PSXRegion[pItem->Region].ppModels[pItem->Model]->Flags &
                    M3DINIT_MODEL_OPAQUE) != 0) {
                pItem->Flags |= M3DINIT_ITEM_OPAQUE;
            }
            i++;
            pItem++;
        } while (i < NumItems);
    }
}

/**
 * Flags texture-wibble items whose U or V amplitude data is entirely usable.
 *
 * Each item is followed by four vertex entries per face. An amplitude phase
 * with an upper nibble of six or greater clears the matching item flag. A zero
 * ItemOffset terminates the inline table.
 *
 * @param pTexWibItemInfo  TexWibItemInfo to operate on.
 */
void M3dInit_FlagZeroWibbles(STexWibItemInfo *pTexWibItemInfo) {
    for (;;) {
        if (pTexWibItemInfo->ItemOffset == 0) {
            return;
        }

        {
            STexWibVertInfo *pTexWibVertInfo =
                M3dInit_GetFirstWibVertex(pTexWibItemInfo);
            int NumFaces = pTexWibItemInfo->NumFaces;
            int i = 0;
            pTexWibItemInfo->ZeroUAmplitudes = 1;

            pTexWibItemInfo->ZeroVAmplitudes = 1;
            if (NumFaces != 0) {
                u8 *pAmplitudePhase =
                    M3dInit_GetAmplitudeCursor(pTexWibItemInfo);
                do {
                    if ((M3DINIT_U_AMPLITUDE_PHASE(pAmplitudePhase) >> 4) >= 6)
                        pTexWibItemInfo->ZeroUAmplitudes = 0;
                    if ((M3DINIT_V_AMPLITUDE_PHASE(pAmplitudePhase) >> 4) >= 6)
                        pTexWibItemInfo->ZeroVAmplitudes = 0;
                    pAmplitudePhase += sizeof(STexWibVertInfo);
                    pTexWibVertInfo++;
                    if ((M3DINIT_U_AMPLITUDE_PHASE(pAmplitudePhase) >> 4) >= 6)
                        pTexWibItemInfo->ZeroUAmplitudes = 0;
                    if ((M3DINIT_V_AMPLITUDE_PHASE(pAmplitudePhase) >> 4) >= 6)
                        pTexWibItemInfo->ZeroVAmplitudes = 0;
                    pAmplitudePhase += sizeof(STexWibVertInfo);
                    pTexWibVertInfo++;
                    if ((M3DINIT_U_AMPLITUDE_PHASE(pAmplitudePhase) >> 4) >= 6)
                        pTexWibItemInfo->ZeroUAmplitudes = 0;
                    if ((M3DINIT_V_AMPLITUDE_PHASE(pAmplitudePhase) >> 4) >= 6)
                        pTexWibItemInfo->ZeroVAmplitudes = 0;
                    pAmplitudePhase += sizeof(STexWibVertInfo);
                    pTexWibVertInfo++;
                    if ((M3DINIT_U_AMPLITUDE_PHASE(pAmplitudePhase) >> 4) >= 6)
                        pTexWibItemInfo->ZeroUAmplitudes = 0;
                    if ((M3DINIT_V_AMPLITUDE_PHASE(pAmplitudePhase) >> 4) >= 6)
                        pTexWibItemInfo->ZeroVAmplitudes = 0;
                    pAmplitudePhase += sizeof(STexWibVertInfo);
                    pTexWibVertInfo++;
                    i++;
                } while (i < NumFaces);
            }
            pTexWibItemInfo = M3dInit_GetNextWibItem(pTexWibVertInfo);
        }
    }
}
