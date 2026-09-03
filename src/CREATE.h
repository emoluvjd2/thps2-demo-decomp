#if defined(CREATE_TEX_LIST_LAYOUT_ONLY) && \
    !defined(CREATE_TEX_LIST_LAYOUT)
#error CREATE_TEX_LIST_LAYOUT_ONLY requires CREATE_TEX_LIST_LAYOUT
#endif

#if defined(CREATE_TEX_SPEC_LAYOUT_ONLY) && \
    !defined(CREATE_TEX_SPEC_LAYOUT)
#error CREATE_TEX_SPEC_LAYOUT_ONLY requires CREATE_TEX_SPEC_LAYOUT
#endif

#if !defined(CREATE_TEX_LIST_LAYOUT_ONLY) && \
    !defined(CREATE_TEX_SPEC_LAYOUT_ONLY)
#ifndef CREATE_INC_H
#define CREATE_INC_H

struct CSuper;

/** Initialize the active skater for gameplay or front-end use. */
void Create_InitSkater(int forGame);

/** Initialize create-a-skater state for the front-end preview. */
void Create_InitSkaterFrontEnd(void);

/** Release create-a-skater state owned by the front-end preview. */
void Create_FreeSkaterFrontEnd(void);

/** Build the create-a-skater model into an existing skater object. */
void Create_BuildSkater(CSuper *pBuildSkater);

/** Release the current create-a-skater model and its owned resources. */
void Create_FreeSkater(void);

/** Copy the active create-a-skater textures back to their source regions. */
void Create_SaveTextures(void);

/** Return whether the source create-a-skater region is being processed. */
int Create_ProcessingSourcePSX(void);

/** Return whether the build create-a-skater region is being processed. */
int Create_ProcessingBuildPSX(void);

/** Return whether a selected-skater index denotes create-a-skater. */
int Create_UseSkater(int selectedSkater);

/** Run the create-a-skater front-end screen. */
int Create_Main(void);

/** Initialize create-a-skater system resources. */
void Create_InitSystem(void);

/** Request any create-a-skater textures that are not resident. */
int Create_SpoolTextures(void);

/** Begin asynchronous create-a-skater model spooling. */
void Create_BeginSpooling(void);

#endif // CREATE_INC_H
#endif // !CREATE_TEX_LIST_LAYOUT_ONLY && !CREATE_TEX_SPEC_LAYOUT_ONLY

#if defined(CREATE_TEX_LIST_LAYOUT) && \
    !defined(CREATE_TEX_LIST_LAYOUT_EMITTED)
#define CREATE_TEX_LIST_LAYOUT_EMITTED

/** One texture choice and its front-end label. */
struct _SCreateTexList {
    char *TextureName;
    char *FrontEndName;
};

typedef struct _SCreateTexList SCreateTexList;

typedef char SCreateTexList_size_must_be_8[
    (sizeof(struct _SCreateTexList) == 8) ? 1 : -1];
typedef char SCreateTexList_TextureName_offset_must_be_0[
    ((unsigned long)&(((struct _SCreateTexList *)0)->TextureName) == 0)
        ? 1 : -1];
typedef char SCreateTexList_FrontEndName_offset_must_be_4[
    ((unsigned long)&(((struct _SCreateTexList *)0)->FrontEndName) == 4)
        ? 1 : -1];

#endif // CREATE_TEX_LIST_LAYOUT

#if defined(CREATE_TEX_SPEC_LAYOUT) && \
    !defined(CREATE_TEX_SPEC_LAYOUT_EMITTED)
#define CREATE_TEX_SPEC_LAYOUT_EMITTED

#ifndef SRC_GLOBALS_H
#error CREATE_TEX_SPEC_LAYOUT requires ordinary globals.h
#endif

#ifndef CREATE_TEX_LIST_LAYOUT_EMITTED
struct _SCreateTexList;
#endif

/** One create-a-skater geometry choice and its texture-list slots. */
struct CCreateTexSpec {
#ifdef __cplusplus
    /** Return the geometry name used by this specification. */
    char *GetGeometryName();

    /** Return the front-end label used by this specification. */
    char *GetFrontEndName();

    /** Return one of this specification's four texture lists. */
    struct _SCreateTexList *GetTextureList(int index);

    /** Return this specification's default palette hue. */
    int GetDefaultHue();
#endif
    struct CClass CClass;
    char mGeometryName[16];
    char mFrontEndName[32];
    struct _SCreateTexList *mpTextureList[4];
    int mDefaultHue;
};

#endif // CREATE_TEX_SPEC_LAYOUT

#ifdef CREATE_DECLARE_VIEWPORT_OWNER_VIEW
struct SViewport;

/** CREATE-owned projection bounds used by the editor camera. */
extern SViewport CreateViewport;

#undef CREATE_DECLARE_VIEWPORT_OWNER_VIEW
#endif

#ifdef CREATE_DECLARE_OWNER_DESTRUCTOR_ABI
struct CClass;
struct MySkaterTextureInfo;

/** Erased destructor entry used by CREATE's MySkater table image. */
void Create_MySkaterDestructorSlot(void) asm("_._8MySkater");

/** Typed destructor entry used by CREATE's texture-info table image. */
static void Create_MySkaterTextureInfoDestructor(
    MySkaterTextureInfo *info, int inCharge)
    asm("_._19MySkaterTextureInfo");

extern "C"
{
/** Owner-private CClass destructor instantiated in create.obj. */
static void __DOT__6CClass_80073244(CClass *object, int inCharge);
}

#undef CREATE_DECLARE_OWNER_DESTRUCTOR_ABI
#endif

#ifdef CREATE_DECLARE_EARLY_CONSTRUCTOR_ABI
struct CharacterEditor;
struct MySkaterTextureInfo;

namespace CreateEarlyConstructorAbi
{
/** Construct one CREATE-owned texture-info record. */
extern "C" MySkaterTextureInfo *__19MySkaterTextureInfoUi(
    MySkaterTextureInfo *info, unsigned int checksum);

/** Construct CREATE's character editor through its target entry. */
extern "C" CharacterEditor *__15CharacterEditor(CharacterEditor *editor);
}

#undef CREATE_DECLARE_EARLY_CONSTRUCTOR_ABI
#endif

#ifdef CREATE_DECLARE_CHARACTER_EDITOR_DESTRUCTOR_ABI
/** Erased destructor entry used by CREATE's CharacterEditor table image. */
void Create_CharacterEditorDestructorSlot(void) asm("_._15CharacterEditor");
#undef CREATE_DECLARE_CHARACTER_EDITOR_DESTRUCTOR_ABI
#endif

#ifdef CREATE_DECLARE_MYSKATER_CONSTRUCTOR_ABI
struct MySkater;

namespace CreateMySkaterConstructorAbi
{
/** Construct CREATE's owner-local MySkater singleton. */
extern "C" MySkater *__8MySkater(MySkater *skater);
}

#undef CREATE_DECLARE_MYSKATER_CONSTRUCTOR_ABI
#endif

#ifdef CREATE_DECLARE_GLOBAL_DTOR_ABI
/** Target-named teardown hook for CREATE's MySkater singleton. */
void Create_GlobalDtor_BodyPart(void) asm("_GLOBAL_.D.BodyPart");
#undef CREATE_DECLARE_GLOBAL_DTOR_ABI
#endif

#ifdef CREATE_DECLARE_GLOBAL_INIT_ABI
/** Target-named startup hook for CREATE's MySkater singleton. */
void Create_GlobalInit_BodyPart(void) asm("_GLOBAL_.I.BodyPart");
#undef CREATE_DECLARE_GLOBAL_INIT_ABI
#endif
