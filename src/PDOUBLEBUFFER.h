#ifndef PDOUBLEBUFFER_INC_H
#define PDOUBLEBUFFER_INC_H

struct SDoubleBuffer;

/**
 * Active display-buffer pointer physically owned by DB.cpp.
 *
 * Consumers retain the target's absolute-address view while DB emits the
 * four-byte small-BSS common used by its GP-relative accesses.
 */
#ifdef PDOUBLEBUFFER_OWNER_DECLARATIONS
#define PDOUBLEBUFFER_DECL_ATTR
#else
#define PDOUBLEBUFFER_DECL_ATTR __attribute__((section(".data")))
#endif

extern SDoubleBuffer *pDoubleBuffer PDOUBLEBUFFER_DECL_ATTR;

#undef PDOUBLEBUFFER_DECL_ATTR

#endif // PDOUBLEBUFFER_INC_H

#ifdef PDOUBLEBUFFER_DECLARE_OWNER_ADDRESS_VIEWS
/**
 * Display-buffer cursors imported through DB.cpp's GP-relative owner view.
 *
 * The selector keeps these declarations at their target-proven encounter and
 * prevents unrelated consumers from acquiring unused external records.
 */
extern SDoubleBuffer *pOtherBuffer;
extern SDoubleBuffer *pVB;

#undef PDOUBLEBUFFER_DECLARE_OWNER_ADDRESS_VIEWS
#endif
