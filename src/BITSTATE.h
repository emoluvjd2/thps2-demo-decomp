#ifndef BITSTATE_TYPES_INC_H
#define BITSTATE_TYPES_INC_H

struct CGLine;
struct CGPolyLine;
struct CNonRenderedBit;
struct CPolyLine;
struct Texture;

#endif // BITSTATE_TYPES_INC_H

/*
 * BIT owns GP-addressed small-data storage for these globals.  Each consumer
 * opts into only the declarations it uses and retains the target's absolute
 * address formation.  This header is intentionally re-entrant because BIT's
 * owner declarations are interleaved with other common symbols.
 */
#ifdef BITSTATE_OWNER_DECLARATIONS
#define BITSTATE_DECL_ATTR
#else
#define BITSTATE_DECL_ATTR __attribute__((section(".data")))
#endif

#if defined(BITSTATE_DECLARE_USE_CACHE_FOR_BITS) && \
    !defined(BITSTATE_USE_CACHE_FOR_BITS_DECLARED)
#define BITSTATE_USE_CACHE_FOR_BITS_DECLARED
/** Select whether new transient bits may use the shared effect cache. */
extern "C" int UseCacheForBits BITSTATE_DECL_ATTR;
#endif
#undef BITSTATE_DECLARE_USE_CACHE_FOR_BITS

#if defined(BITSTATE_DECLARE_BIT_COUNT) && \
    !defined(BITSTATE_BIT_COUNT_DECLARED)
#define BITSTATE_BIT_COUNT_DECLARED
/** Number of live transient bit effects. */
extern "C" int BitCount BITSTATE_DECL_ATTR;
#endif
#undef BITSTATE_DECLARE_BIT_COUNT

#if defined(BITSTATE_DECLARE_TOTAL_BIT_USAGE) && \
    !defined(BITSTATE_TOTAL_BIT_USAGE_DECLARED)
#define BITSTATE_TOTAL_BIT_USAGE_DECLARED
/** Bytes currently owned by transient bit effects. */
extern "C" int TotalBitUsage BITSTATE_DECL_ATTR;
#endif
#undef BITSTATE_DECLARE_TOTAL_BIT_USAGE

#if defined(BITSTATE_DECLARE_NON_RENDERED_BIT_LIST) && \
    !defined(BITSTATE_NON_RENDERED_BIT_LIST_DECLARED)
#define BITSTATE_NON_RENDERED_BIT_LIST_DECLARED
/** Head of the non-rendered transient-bit list. */
extern "C" CNonRenderedBit *NonRenderedBitList BITSTATE_DECL_ATTR;
#endif
#undef BITSTATE_DECLARE_NON_RENDERED_BIT_LIST

#if defined(BITSTATE_DECLARE_G_LINE_LIST) && \
    !defined(BITSTATE_G_LINE_LIST_DECLARED)
#define BITSTATE_G_LINE_LIST_DECLARED
/** Head of the Gouraud line-effect list. */
extern "C" CGLine *GLineList BITSTATE_DECL_ATTR;
#endif
#undef BITSTATE_DECLARE_G_LINE_LIST

#if defined(BITSTATE_DECLARE_G_POLY_LINE_LIST) && \
    !defined(BITSTATE_G_POLY_LINE_LIST_DECLARED)
#define BITSTATE_G_POLY_LINE_LIST_DECLARED
/** Head of the Gouraud polyline-effect list. */
extern "C" CGPolyLine *GPolyLineList BITSTATE_DECL_ATTR;
#endif
#undef BITSTATE_DECLARE_G_POLY_LINE_LIST

#if defined(BITSTATE_DECLARE_POLY_LINE_LIST) && \
    !defined(BITSTATE_POLY_LINE_LIST_DECLARED)
#define BITSTATE_POLY_LINE_LIST_DECLARED
/** Head of the flat polyline-effect list. */
extern "C" CPolyLine *PolyLineList BITSTATE_DECL_ATTR;
#endif
#undef BITSTATE_DECLARE_POLY_LINE_LIST

#if defined(BITSTATE_DECLARE_SHADOW_TEXTURE) && \
    !defined(BITSTATE_SHADOW_TEXTURE_DECLARED)
#define BITSTATE_SHADOW_TEXTURE_DECLARED
/** Texture used by projected blob shadows. */
extern "C" Texture *pShadowTexture BITSTATE_DECL_ATTR;
#endif
#undef BITSTATE_DECLARE_SHADOW_TEXTURE

#if defined(BITSTATE_DECLARE_SPARK_SEMI_TRANS) && \
    !defined(BITSTATE_SPARK_SEMI_TRANS_DECLARED)
#define BITSTATE_SPARK_SEMI_TRANS_DECLARED
/** Nonzero when newly constructed sparks use semi-transparency. */
extern "C" void *SparkSemiTrans BITSTATE_DECL_ATTR;
#endif
#undef BITSTATE_DECLARE_SPARK_SEMI_TRANS

#undef BITSTATE_DECL_ATTR
