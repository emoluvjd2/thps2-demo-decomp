#ifndef STATSTABLE_INC_H
#define STATSTABLE_INC_H

/*
 * STATS physically owns the fourteen-record skater table.  Most translation
 * units consume its complete SStats[14] type, while CREATE retains the
 * target-proven unsized word-base view and PLAYERSELECT declares the table
 * before its owner-local SStats layout is complete.
 */
#if defined(STATSTABLE_WORD_BASE_DECLARATION) && \
    defined(STATSTABLE_UNSIZED_STATS_DECLARATION)
#error "Select only one Stats declaration view"
#elif defined(STATSTABLE_WORD_BASE_DECLARATION)
#define STATSTABLE_DECL_TYPE int
#define STATSTABLE_DECL_BOUND
#elif defined(STATSTABLE_UNSIZED_STATS_DECLARATION)
#define STATSTABLE_DECL_TYPE SStats
#define STATSTABLE_DECL_BOUND
#else
#define STATSTABLE_DECL_TYPE SStats
#define STATSTABLE_DECL_BOUND SkaterStatsCount
#endif

#ifdef STATSTABLE_WITHIN_C_LINKAGE
#define STATSTABLE_DECL_LINKAGE extern
#else
#define STATSTABLE_DECL_LINKAGE extern "C"
#endif

/** Static identity, appearance, and gameplay settings for every skater. */
STATSTABLE_DECL_LINKAGE STATSTABLE_DECL_TYPE Stats[STATSTABLE_DECL_BOUND];

#undef STATSTABLE_DECL_LINKAGE
#undef STATSTABLE_DECL_BOUND
#undef STATSTABLE_DECL_TYPE
#undef STATSTABLE_UNSIZED_STATS_DECLARATION
#undef STATSTABLE_WORD_BASE_DECLARATION
#undef STATSTABLE_WITHIN_C_LINKAGE

#endif // STATSTABLE_INC_H
