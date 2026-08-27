// Trick-object triggers, completion state, and environment-item tint updates.
// Target text range: 0x80050414 - 0x8005086C.

#define TRICKOB_OWNER_DECLARATIONS
#include "TRICKOB.h"
#undef TRICKOB_OWNER_DECLARATIONS
#include "MAIN.h"
#include "UTILS.h"

/** Virtual prefix shared by active trick-object instances. */
struct TrickObVtable
{
    void (*reserved0)(void *object);
    void (*reserved1)(void *object);
    void (*destroy)(CTrickOb *object, int flags);
};

/** Dispatch destruction through the named CTrickOb vtable slot. */
static inline void TrickOb_Destroy(CTrickOb *object)
{
    TrickObVtable *vtable =
        reinterpret_cast<TrickObVtable *>(object->CClass.vtable);
    vtable->destroy(object, 3);
}

typedef char trickob_owner_size_must_be_24[
    (sizeof(CTrickOb) == 24) ? 1 : -1];
/** Load-bearing vtable alias used by the constructor's original store order. */
static inline int *&TrickOb_VtableAlias(CTrickOb *object)
{
    return *reinterpret_cast<int **>(object);
}

#include "SPOOL.h"

// === Forward type declarations ===

// === Owner-local $gp-relative globals ===
extern "C" CTrickOb *TrickObList
    __attribute__((section(".sdata"))) = 0;
extern "C" unsigned short DarkenTimer[3]
    __attribute__((section(".sdata"))) = {1800, 600, 900};
static int MapX __attribute__((section(".sdata"))) = 256;
static int MapY __attribute__((section(".sdata"))) = 220;
static int MapScale __attribute__((section(".sdata"))) = 128;
extern "C" int Missed
    __attribute__((section(".sdata"))) = 0;

// GCC 2.x emits a private copy of `CClass::~CClass` beside a private base
// vtable in every owner that derives from CClass.  `MAIN.SYM` records 25 such
// `_._6CClass` copies and no vtable name at all, so both belong to this owner
// and neither is an XDEF.  The filesystem-safe spelling keeps the per-address
// target assembly reachable.
static void TrickOb_ClassDestructor(CClass *self, int flags)
    asm("__DOT__6CClass_8005083C");

// C++ forbids taking a destructor's address, so the vtable slot goes through an
// addressable declaration of the same symbol this owner already defines.
extern "C" void TrickOb_DestructorAbi(CTrickOb *, int)
    asm("_._8CTrickOb");

// Both vtables are three consecutive eight-byte-aligned small-data words rather
// than arrays, so -G8 still yields the target's `addiu $vX, $gp, off` form.
extern "C" int __vt_8CTrickOb
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int TrickOb_VTableIndex __attribute__((section(".sdata"))) = 0;
static void (*TrickOb_VTableDtor)(CTrickOb *, int)
    __attribute__((section(".sdata"))) = &TrickOb_DestructorAbi;

static int TrickOb_BaseVTable asm("__vt_8CTrickOb_base")
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int TrickOb_BaseVTableIndex __attribute__((section(".sdata"))) = 0;
static void (*TrickOb_BaseVTableDtor)(CClass *, int)
    __attribute__((section(".sdata"))) = &TrickOb_ClassDestructor;


// === Function implementations (address order) ===

/**
 * Destroy every currently linked trick object.
 *
 * Address:   0x80050414
 * Mangled:   TrickOb_Init__Fv
 * Demangled: TrickOb_Init(void)
 */
void TrickOb_Init(void)
{
    while (TrickObList != 0)
    {
        TrickOb_Destroy(TrickObList);
    }
}

/**
 * Delete the complete active trick-object list without relying on head reuse.
 *
 * Address:   0x8005045C
 * Mangled:   TrickOb_DeleteAll__Fv
 * Demangled: TrickOb_DeleteAll(void)
 *
 */
void TrickOb_DeleteAll(void)
{
    CTrickOb *pTrickOb = TrickObList;
    if (pTrickOb != 0)
    {
        do
        {
            CTrickOb *pNext = pTrickOb->mpNext;
            if (pTrickOb != 0)
            {
                TrickOb_Destroy(pTrickOb);
            }
            pTrickOb = pNext;
        } while (pTrickOb != 0);
    }
}

/**
 * Construct and link one trick trigger.
 *
 * Address:   0x800504B0
 * Mangled:   __8CTrickObUsUs
 * Demangled: CTrickOb::(unsigned short, unsigned short)
 *            Link a trick object into the active list and cache its trigger command pointer.
 *
 * @param object  Constructor receiver.
 * @param Node  Rail / graph node identifier.
 * @param DelayLevel  Initial delay tier (unused in this demo build).
 * @return `this` pointer (GCC 2.x ctor convention).
 */
CTrickOb *CTrickOb_Construct(
    CTrickOb *object, unsigned short Node, unsigned short DelayLevel)
{
    CTrickOb *oldHead = TrickObList;
    // GCC 2.91 hoists the Node mask in each unpinned form, reordering eight
    // target instructions, so this inherited register variable remains.
    register int *vtable asm("$2") = &__vt_8CTrickOb;
    TrickObList = object;
    object->mState = 0;
    short **offsets = OffsetList;
    object->mNode = Node;
    TrickOb_VtableAlias(object) = vtable;
    object->mpItem = 0;
    object->mpNext = oldHead;
    short *pNode = offsets[Node];
    unsigned short count = pNode[1];
    // The command pointer is aligned after skipping the variable short payload.
    pNode += count + 2;
    if (reinterpret_cast<unsigned int>(pNode) & 3)
        pNode++;
    object->mChecksum = *reinterpret_cast<unsigned int *>(pNode);
    return object;
}

/**
 * CTrickOb::~CTrickOb()
 *
 * Address:   0x80050520
 * Mangled:   _._8CTrickOb
 * Demangled: CTrickOb::~CTrickOb()
 *
 */
CTrickOb::~CTrickOb()
{
    CTrickOb *pLast = 0;
    CTrickOb *pTrickOb = TrickObList;
    CClass.vtable = &__vt_8CTrickOb;
    if (pTrickOb == this)
        goto found;
    do
    {
        pLast = pTrickOb;
        pTrickOb = pLast->mpNext;
    } while (pTrickOb != this);
found:
    if (pLast != 0)
    {
        pLast->mpNext = mpNext;
    }
    else
    {
        TrickObList = mpNext;
    }
    CClass.vtable = &TrickOb_BaseVTable;
}

/**
 * Handle a trick performed on an environment item.
 *
 * Address:   0x80050598
 * Mangled:   TrickOb_TrickedOnItem__FP4ItemPvi
 * Demangled: TrickOb_TrickedOnItem(Item *, void *, int)
 *
 * @param pItem  Item to operate on.
 * @param pB  Skater/context pointer supplied by the caller.
 * @param Score  Score value.
 * @return Unspecified legacy result; the target body leaves `$v0` untouched.
 */
int TrickOb_TrickedOnItem(Item *pItem, void *pB, int Score)
{
}

/**
 * Mark a matching trick node complete and darken its environment item.
 *
 * Address:   0x800505A0
 * Mangled:   TrickOb_DarkenNode__Fi
 * Demangled: TrickOb_DarkenNode(int)
 *
 * @param NodeIndex  Rail node index.
 */
void TrickOb_DarkenNode(int NodeIndex)
{
    if (GGame == 8)
    {
        CTrickOb *pTrickOb = TrickObList;
        if (pTrickOb != 0)
        {
            int darkColor = 0x00200000;
            int scoreMode = 8;
            darkColor |= 0x2020;

            do
            {
                if (pTrickOb->mNode != NodeIndex)
                    goto next;

                if (pTrickOb->mState != 0)
                {
                    if (GGame != scoreMode)
                        goto done;
                }

                {
                    Item *pItem = Spool_FindEnviroItem(pTrickOb->mChecksum);
                    unsigned short score =
                        static_cast<unsigned short>(TrickScore);
                    pTrickOb->mpItem = pItem;
                    pTrickOb->mScore = score;
                    pTrickOb->mPlayer =
                        static_cast<unsigned char>(TrickPlayer);
                    unsigned short flags = pItem->Flags;
                    pItem->RGB = darkColor;
                    pItem->Flags = flags | 0x400;
                    pTrickOb->mState = 1;
                }
                goto done;

            next:
                pTrickOb = pTrickOb->mpNext;
            } while (pTrickOb != 0);
        }
    }
done:;
}

/**
 * Updates the red color channel of every active trick object.
 *
 * In score mode the channel pulses according to the object's score threshold;
 * otherwise it follows the global frame counter. Player two uses the green
 * channel for the score-mode pulse.
 *
 * Address:   0x8005066C
 * Mangled:   TrickOb_Update__Fv
 * Demangled: TrickOb_Update(void)
 *
 */
void TrickOb_Update(void)
{
    CTrickOb *pTrickOb = TrickObList;
    if (pTrickOb == 0)
        return;
    int game = GGame;

    do
    {
        Item *pItem;
        if (pTrickOb->mState != 0 &&
            (pItem = pTrickOb->mpItem) != 0)
        {
            if (game == 8)
            {
                int Red;
                unsigned int pulsePhase =
                    static_cast<unsigned int>(Xblanks * 4);
                if (pTrickOb->mScore < 2001)
                {
                    Red = pulsePhase %
                          static_cast<unsigned int>(
                              ((pTrickOb->mScore << 6) / 2000 + 1) * 2);
                }
                else
                {
                    Red = pulsePhase % 130;
                }
                if (pTrickOb->mScore < 2001)
                {
                    if ((pTrickOb->mScore << 6) / 2000 + 1 < Red)
                    {
                        goto adjust;
                    }
                }
                else if (65 < Red)
                {
                adjust:
                    if (pTrickOb->mScore < 2001)
                    {
                        Red = ((pTrickOb->mScore << 6) / 2000 + 1) * 2 - Red;
                    }
                    else
                    {
                        Red = 130 - Red;
                    }
                }
                if (pTrickOb->mPlayer == 1)
                {
                    pItem->RGB =
                        (pItem->RGB & 0xff00ffff) | ((Red + 64) * 0x10000);
                    goto next;
                }
                pItem->RGB =
                    (pItem->RGB & 0xffffff00) | (Red + 64);
            }
            else
            {
                int Red = static_cast<unsigned int>(Xblanks) >> 1 & 0x3f;
                if (32 < Red)
                {
                    Red = 64 - Red;
                }
                pItem->RGB =
                    (pItem->RGB & 0xffffff00) | (Red + 32);
            }
        }
    next:
        pTrickOb = pTrickOb->mpNext;
    } while (pTrickOb != 0);

}

/**
 * Destroy the owner-local CClass base and optionally release its storage.
 *
 * GCC emitted this private copy last in the owner's text, so it remains at
 * the end of the translation unit.
 */
static void TrickOb_ClassDestructor(CClass *self, int flags)
{
    self->vtable = &TrickOb_BaseVTable;
    if (flags & 1)
    {
        CClass::operator delete(self);
    }
}
