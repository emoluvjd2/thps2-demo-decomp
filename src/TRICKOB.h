#if defined(TRICKOB_LAYOUT_ONLY) && !defined(TRICKOB_LAYOUT)
#error "TRICKOB layout-only mode requires TRICKOB_LAYOUT"
#endif

#if defined(TRICKOB_OWNER_DECLARATIONS) && defined(TRICKOB_LAYOUT_ONLY)
#error "TRICKOB owner declarations cannot be requested in layout-only mode"
#endif

#ifndef TRICKOB_LAYOUT_ONLY
#ifndef TRICKOB_INC_H
#define TRICKOB_INC_H

struct Item;
struct CTrickOb;

void TrickOb_Init(void);
void TrickOb_DeleteAll(void);
/**
 * Notify the trick-object system about a scored environment item.
 *
 * The legacy target ABI declares an `int` result, but the empty target body
 * never assigns `$v0`; callers must ignore the incidental register value.
 */
int TrickOb_TrickedOnItem(Item *item, void *skater, int score);
void TrickOb_DarkenNode(int nodeIndex);
void TrickOb_Update(void);

#endif // TRICKOB_INC_H
#endif // TRICKOB_LAYOUT_ONLY

#if defined(TRICKOB_OWNER_DECLARATIONS) && \
    !defined(TRICKOB_OWNER_DECLARATIONS_HEAD_EMITTED) && \
    defined(TRICKOB_LAYOUT_EMITTED)
#error "TRICKOB owner declarations must precede a focused layout encounter"
#endif

#if defined(TRICKOB_OWNER_DECLARATIONS) && \
    !defined(TRICKOB_OWNER_DECLARATIONS_HEAD_EMITTED)
#define TRICKOB_OWNER_DECLARATIONS_HEAD_EMITTED
#define TRICKOB_OWNER_DECLARATIONS_ACTIVE

#include "globals.h"

#endif // TRICKOB_OWNER_DECLARATIONS_HEAD_EMITTED

#if defined(TRICKOB_OWNER_DECLARATIONS_ACTIVE) && \
    !defined(TRICKOB_LAYOUT)
#define TRICKOB_LAYOUT
#define TRICKOB_LAYOUT_WAS_IMPLIED
#endif

#if defined(TRICKOB_LAYOUT) && !defined(TRICKOB_LAYOUT_EMITTED)

#ifndef SRC_GLOBALS_H
#error "TRICKOB layout requires the ordinary globals declarations first"
#endif

#define TRICKOB_LAYOUT_EMITTED

/** One active trick trigger linked into the owner-local trick list. */
struct CTrickOb
{
    CClass CClass;
    unsigned int mChecksum;
    unsigned short mNode;
    unsigned char mState;
    unsigned char mPlayer;
    unsigned short mScore;
    CTrickOb *mpNext;
    Item *mpItem;

    /** Constructs and links one trigger object. */
    CTrickOb(unsigned short node, unsigned short delayLevel);
    /** Unlinks this trigger from the active trick-object list. */
    ~CTrickOb();
    /** Releases trigger storage through the owning CClass allocator. */
    static void operator delete(void *p) {
        ::CClass::operator delete(p);
    }
};

#endif // TRICKOB_LAYOUT && !TRICKOB_LAYOUT_EMITTED

#ifdef TRICKOB_LAYOUT_WAS_IMPLIED
#undef TRICKOB_LAYOUT
#undef TRICKOB_LAYOUT_WAS_IMPLIED
#endif

#if defined(TRICKOB_OWNER_DECLARATIONS_ACTIVE) && \
    !defined(TRICKOB_OWNER_DECLARATIONS_TAIL_EMITTED)
#define TRICKOB_OWNER_DECLARATIONS_TAIL_EMITTED

// === Variables (gp-relative in this TU) ===
#define TRICKOBLIST_OWNER_DECLARATION
#include "TRICKOBLIST.h"
#define TRICKPLAYER_OWNER_DECLARATION
#include "TRICKPLAYER.h"
extern "C" int TrickScore;       // 0x800DA520

// === Variables (absolute in this TU) ===
#define TRIGSTATE_ABSOLUTE_DECLARATION
#define TRIGSTATE_DECLARE_OFFSET_LIST
#include "TRIGSTATE.h"

#endif // TRICKOB_OWNER_DECLARATIONS_TAIL_EMITTED

#ifdef TRICKOB_OWNER_DECLARATIONS_ACTIVE
#undef TRICKOB_OWNER_DECLARATIONS_ACTIVE
#endif
