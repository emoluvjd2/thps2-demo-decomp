/*
 * RFRONT physically owns the career-load flags in initialized small data.
 * Cross-owner consumers retain their target-proven absolute-address views.
 * This header is intentionally re-entrant so each flag stays at its original
 * declaration point.
 */
#if defined(CAREERLOADSTATE_OWNER_DECLARATION) && \
    defined(CAREERLOADSTATE_ABSOLUTE_DECLARATION)
#error "Select only one career-load state declaration view"
#elif defined(CAREERLOADSTATE_OWNER_DECLARATION)
#define CAREERLOADSTATE_DECL_ATTR
#elif defined(CAREERLOADSTATE_ABSOLUTE_DECLARATION)
#define CAREERLOADSTATE_DECL_ATTR __attribute__((section(".data")))
#else
#error "Select the career-load state owner or absolute declaration view"
#endif

#if !defined(CAREERLOADSTATE_DECLARE_HAS_CAREER_LOADED) && \
    !defined(CAREERLOADSTATE_DECLARE_LOADED_CAREER_IS_SAVED)
#error "Select at least one career-load state symbol"
#endif

#ifdef CAREERLOADSTATE_DECLARE_HAS_CAREER_LOADED
/** Whether a career has been loaded into the front end. */
extern int GHasCareerLoaded
    CAREERLOADSTATE_DECL_ATTR;
#endif

#ifdef CAREERLOADSTATE_DECLARE_LOADED_CAREER_IS_SAVED
/** Whether the loaded career has already been saved. */
extern int GLoadedCareerIsSaved
    CAREERLOADSTATE_DECL_ATTR;
#endif

#undef CAREERLOADSTATE_DECL_ATTR
#undef CAREERLOADSTATE_DECLARE_HAS_CAREER_LOADED
#undef CAREERLOADSTATE_DECLARE_LOADED_CAREER_IS_SAVED
#undef CAREERLOADSTATE_ABSOLUTE_DECLARATION
#undef CAREERLOADSTATE_OWNER_DECLARATION
