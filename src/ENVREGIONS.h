#ifndef ENVREGIONS_INC_H
#define ENVREGIONS_INC_H

/*
 * SPOOL physically owns the two environment-region indices in small data.
 * Most consumers import the complete array at its absolute address; M3D keeps
 * the target-proven scalar first-word view used by its pointer arithmetic.
 */
#if (defined(ENVREGIONS_OWNER_DECLARATION) + \
     defined(ENVREGIONS_ABSOLUTE_DECLARATION) + \
     defined(ENVREGIONS_SCALAR_ABSOLUTE_DECLARATION)) != 1
#error "Select exactly one EnvRegions declaration view"
#elif defined(ENVREGIONS_OWNER_DECLARATION)
#define ENVREGIONS_DECL_ATTR
#else
#define ENVREGIONS_DECL_ATTR __attribute__((section(".data")))
#endif

#ifdef ENVREGIONS_WITHIN_C_LINKAGE
#define ENVREGIONS_DECL_LINKAGE extern
#else
#define ENVREGIONS_DECL_LINKAGE extern "C"
#endif

#ifdef ENVREGIONS_SCALAR_ABSOLUTE_DECLARATION
/** First environment-region word, used as the base of the two-word array. */
ENVREGIONS_DECL_LINKAGE int EnvRegions ENVREGIONS_DECL_ATTR;
#else
/** Region numbers occupying the two streamed environment slots. */
ENVREGIONS_DECL_LINKAGE int EnvRegions[2] ENVREGIONS_DECL_ATTR;
#endif

#undef ENVREGIONS_DECL_LINKAGE
#undef ENVREGIONS_DECL_ATTR
#undef ENVREGIONS_SCALAR_ABSOLUTE_DECLARATION
#undef ENVREGIONS_ABSOLUTE_DECLARATION
#undef ENVREGIONS_OWNER_DECLARATION
#undef ENVREGIONS_WITHIN_C_LINKAGE

#endif // ENVREGIONS_INC_H
