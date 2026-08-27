/*
 * OB physically owns the active and suspended environmental-body heads, while
 * BACKGRND owns the background-body head.  This header is intentionally
 * re-entrant and opt-in per symbol so consumers retain their original
 * declaration order and absolute-address import views.
 */
struct CBody;

#ifdef BODYLISTS_WITHIN_C_LINKAGE
#define BODYLISTS_DECL_LINKAGE extern
#else
#define BODYLISTS_DECL_LINKAGE extern "C"
#endif

#ifdef BODYLISTS_GP_RELATIVE_DECLARATIONS
#define BODYLISTS_DECL_ATTR
#else
#define BODYLISTS_DECL_ATTR __attribute__((section(".data")))
#endif

#ifdef BODYLISTS_DECLARE_BACKGROUND_LIST
/** Head of the active background-object list, physically owned by BACKGRND.cpp. */
BODYLISTS_DECL_LINKAGE CBody *BackgroundList BODYLISTS_DECL_ATTR;
#endif

#ifdef BODYLISTS_DECLARE_ENVIRONMENTAL_OBJECT_LIST
/** Head of the active environmental-object list, physically owned by OB.cpp. */
BODYLISTS_DECL_LINKAGE CBody *EnvironmentalObjectList BODYLISTS_DECL_ATTR;
#endif

#ifdef BODYLISTS_DECLARE_SUSPENDED_LIST
/** Head of the suspended environmental-object list, physically owned by OB.cpp. */
BODYLISTS_DECL_LINKAGE CBody *SuspendedList BODYLISTS_DECL_ATTR;
#endif

#undef BODYLISTS_DECL_ATTR
#undef BODYLISTS_DECL_LINKAGE
#undef BODYLISTS_DECLARE_BACKGROUND_LIST
#undef BODYLISTS_DECLARE_ENVIRONMENTAL_OBJECT_LIST
#undef BODYLISTS_DECLARE_SUSPENDED_LIST
#undef BODYLISTS_GP_RELATIVE_DECLARATIONS
#undef BODYLISTS_WITHIN_C_LINKAGE
