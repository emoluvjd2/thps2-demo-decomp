/*
 * TRIG physically owns the current object-file cursor in small data. This
 * header is intentionally re-entrant because TRIG first needs its owner view
 * and later encounters BACKGRND's target-recorded absolute-address view.
 */
#ifdef CURRENTOBJFILE_OWNER_DECLARATION
#define CURRENTOBJFILE_DECL_ATTR
#else
#define CURRENTOBJFILE_DECL_ATTR __attribute__((section(".data")))
#endif

/** Start of the object file currently being interpreted. */
extern "C" char *pCurrentObjFile CURRENTOBJFILE_DECL_ATTR;

#undef CURRENTOBJFILE_DECL_ATTR
#undef CURRENTOBJFILE_OWNER_DECLARATION
