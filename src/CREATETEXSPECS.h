#ifndef CREATETEXSPECS_INC_H
#define CREATETEXSPECS_INC_H

struct CCreateTexSpec;

#ifdef CREATETEXSPECS_OWNER_DECLARATION
#define CREATETEXSPECS_DECL_LINKAGE
#elif defined(CREATETEXSPECS_WITHIN_C_LINKAGE)
#define CREATETEXSPECS_DECL_LINKAGE extern
#else
#define CREATETEXSPECS_DECL_LINKAGE extern "C"
#endif

/** Head-model and texture specifications owned by CREATE_TEX. */
#ifndef CREATETEXSPECS_OWNER_DECLARATION
CREATETEXSPECS_DECL_LINKAGE CCreateTexSpec HeadSpec[];
#else
CREATETEXSPECS_DECL_LINKAGE CCreateTexSpec HeadSpec[15];
#endif

/** Upper-body model and texture specifications owned by CREATE_TEX. */
#ifndef CREATETEXSPECS_OWNER_DECLARATION
CREATETEXSPECS_DECL_LINKAGE CCreateTexSpec UpperBodySpec[];
#else
CREATETEXSPECS_DECL_LINKAGE CCreateTexSpec UpperBodySpec[7];
#endif

/** Lower-body model and texture specifications owned by CREATE_TEX. */
#ifndef CREATETEXSPECS_OWNER_DECLARATION
CREATETEXSPECS_DECL_LINKAGE CCreateTexSpec LowerBodySpec[];
#else
CREATETEXSPECS_DECL_LINKAGE CCreateTexSpec LowerBodySpec[5];
#endif

#undef CREATETEXSPECS_DECL_LINKAGE
#undef CREATETEXSPECS_OWNER_DECLARATION
#undef CREATETEXSPECS_WITHIN_C_LINKAGE

#endif // CREATETEXSPECS_INC_H
