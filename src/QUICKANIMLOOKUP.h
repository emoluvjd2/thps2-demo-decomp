/*
 * BIT physically owns this six-pointer animation lookup in initialized data.
 * M3D's same-sized declaration naturally retains the target's absolute
 * address formation.
 */
#if defined(QUICKANIMLOOKUP_OWNER_DECLARATION) && \
    defined(QUICKANIMLOOKUP_CONSUMER_DECLARATION)
#error "Select only one QuickAnimLookup declaration view"
#elif !defined(QUICKANIMLOOKUP_OWNER_DECLARATION) && \
      !defined(QUICKANIMLOOKUP_CONSUMER_DECLARATION)
#error "Select the QuickAnimLookup owner or consumer declaration view"
#endif

#if defined(QUICKANIMLOOKUP_OWNER_DECLARATION) || \
    defined(QUICKANIMLOOKUP_WITHIN_C_LINKAGE)
#define QUICKANIMLOOKUP_DECL_LINKAGE extern
#else
#define QUICKANIMLOOKUP_DECL_LINKAGE extern "C"
#endif

struct SAnimFrame;

/** Fast animation-frame lookup populated by BIT initialization. */
QUICKANIMLOOKUP_DECL_LINKAGE SAnimFrame *QuickAnimLookup[6];

#undef QUICKANIMLOOKUP_DECL_LINKAGE
#undef QUICKANIMLOOKUP_CONSUMER_DECLARATION
#undef QUICKANIMLOOKUP_OWNER_DECLARATION
#undef QUICKANIMLOOKUP_WITHIN_C_LINKAGE
