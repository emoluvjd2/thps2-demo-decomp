// RAIL.cpp -- Reconstructed translation unit
// Original: RAIL.CPP
// Functions: 16
// Address range: 0x8005086C - 0x800515EC
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#define RAIL_OWNER_DECLARATIONS
#include "RAIL.h"
#undef RAIL_OWNER_DECLARATIONS
#include "MAIN.h"
#include "MEM.h"
#include "M3DZONE.h"
#include "M3DUTILS.h"
#define M3DCOLIJ_LAYOUT_PREREQUISITES
#define M3DCOLIJ_OWNER_LAYOUT
#include "M3DCOLIJ.h"
#undef M3DCOLIJ_OWNER_LAYOUT
#undef M3DCOLIJ_LAYOUT_PREREQUISITES

// Owner-local layouts used while recovering this translation unit.
// Keep these slices local until the same fields are proven by other owners.
#include "RAIL.h"
#define OB_LAYOUT_ONLY
#define OB_OWNER_LAYOUT
#include "OB.h"
#undef OB_OWNER_LAYOUT
#undef OB_LAYOUT_ONLY

enum { RAIL_NODE_CAPACITY = 2000 };
enum {
    RAIL_PLAYER_FLAG_SHIFT = 4,
    RAIL_PLAYER_FLAG_MASK = 3 << RAIL_PLAYER_FLAG_SHIFT,
    RAIL_TYPE_MASK = 15,
    RAIL_DISABLED_FLAG = 1 << 15,
    RAIL_COORD_MAX = 2147483647,
    RAIL_COORD_MIN = -2147483647 - 1,
    RAIL_FIXED_ONE = 1 << 12,
    RAIL_COLLISION_INSET = 10 << 12,
    RAIL_COLLISION_SHORT_HEIGHT = 15 << 12,
    RAIL_COLLISION_RAISE = 20 << 12,
    RAIL_COLLISION_HEIGHT = 30 << 12,
    RAIL_SNAP_DISTANCE = 100 << 12
};

/* Target-order address formation used while filling the lookup table. */
#define RAIL_LOOKUP_ADDRESS(nodes) ((int)(nodes))
#define RAIL_LOOKUP_AT_ADDRESS(address) ((NodeSort *)(address))

/*
 * Preserve the target's store-before-copy schedule when publishing the final
 * count. Straight assignments commute these two instructions under GCC 2.91.
 */
#define RAIL_PUBLISH_NODE_COUNT(count, savedCount) \
    ((TotalRailNodes = (count)) ? ((savedCount) = (count)) \
                                : ((savedCount) = (count)))

/** Virtual prefix used to retire an allocated rail segment. */
struct RailVTable {
    void (*reserved0)(CRail *rail);
    void (*reserved1)(CRail *rail);
    void (*destroy)(CRail *rail, int flags);
};

/** Alias-compatible view of the two list links used during insertion. */
struct RailLinkAlias {
    unsigned char prefix[32];
    CRail *previous;
    CRail *next;
};

#define RAIL_LINK_ALIAS_OFFSET(member) \
    ((unsigned int)&((RailLinkAlias *)0)->member)
#define RAIL_PREVIOUS_LINK_ALIAS(rail) \
    (*(CRail **)((unsigned char *)(rail) + \
                 RAIL_LINK_ALIAS_OFFSET(previous)))
#define RAIL_NEXT_LINK_ALIAS(rail) \
    (*(CRail **)((unsigned char *)(rail) + RAIL_LINK_ALIAS_OFFSET(next)))

typedef char rail_previous_alias_offset_check[
    ((unsigned int)&((RailLinkAlias *)0)->previous ==
     (unsigned int)&((CRail *)0)->mpPrevious) ? 1 : -1];
typedef char rail_next_alias_offset_check[
    ((unsigned int)&((RailLinkAlias *)0)->next ==
     (unsigned int)&((CRail *)0)->mpNext) ? 1 : -1];
typedef char rail_node_sort_size_check[
    (sizeof(NodeSort) == 4) ? 1 : -1];
typedef char rail_object_size_check[
    (sizeof(CRail) == 40) ? 1 : -1];

/** Dispatch destruction through CRail's named virtual slot. */
static inline void Rail_Destroy(CRail *rail) {
    RailVTable *vtable = (RailVTable *)rail->vtable;
    vtable->destroy(rail, 3);
}

/*
 * The destructor's first vtable write must retain the original all-types alias
 * edge against RailList; otherwise GCC hoists it ahead of the target prologue.
 */
#define RAIL_VTABLE_ALIAS(object) (*(int **)(object))

/** The triangle-button prefix read by the rail acquisition pass. */
struct SControlRailOwner {
    u8 TrianglePressed;
};

/** Physics states referenced by the rail-acquisition height filter. */
enum EPhysicsStateRailOwner {
    PHYSICS_ON_GROUND_RAIL_OWNER = 0,
    PHYSICS_ON_INVISIBLE_RAIL_OWNER = 2
};

/**
 * CBruce slice covering the movement and rail state consumed by RAIL.cpp.
 *
 * Field offsets come from original/MAIN_STRUCTS.h and are independently
 * visible in Rail_Update's target loads and stores.
 */
struct CBruceRailOwner {
    struct CSuperOwner CSuper;
    u8 pad_to_mRail_LedgeType[3228 - sizeof(struct CSuperOwner)];
    int mRail_LedgeType;
    int mRail_Direction;
    u8 pad_to_mPad[3508 - 3236];
    struct SControlRailOwner *mPad;
    u8 pad_to_mRailInhibit[3692 - 3512];
    int mRailInhibit;
    u8 pad_to_mRailType[3780 - 3696];
    int mRailType;
    int mOnRail;
    int mStickToRail;
    int mLastRailStart;
    int mCurrentRailStart;
    int mCurrentRailSegment;
    int mDistToCurrentRail;
    struct CVector mRailPos;
    struct CVector mRailDir;
    int mDontStickTimer;
    int mSameRailTimer;
    int mDontStickRail;
    u8 pad_to_mInBail[3964 - 3844];
    int mInBail;
    u8 pad_to_mPhysicsState[4204 - 3968];
    EPhysicsStateRailOwner mPhysicsState;
    u8 pad_to_mFront[4256 - 4208];
    struct CVector mFront;
};

typedef char CBruceRailOwner_minimum_size_check[
    (sizeof(struct CBruceRailOwner) == 4268) ? 1 : -1];

#define CAREER_OWNER_LAYOUT
#include "CAREER.h"
#undef CAREER_OWNER_LAYOUT
#define OB_LAYOUT_ONLY
#define OB_OWNER_LAYOUT
#include "OB.h"
#undef OB_OWNER_LAYOUT
#undef OB_LAYOUT_ONLY
#include "TRIG.h"
#include "MIPS.h"

// === External function declarations ===
extern void CVector_AddRefs_rail(CVector *, const CVector &, const CVector &)
    asm("__pl__FRC7CVectorT0");
extern void CVector_SubRefs_rail(CVector *, const CVector &, const CVector &)
    asm("__mi__FRC7CVectorT0");
extern CVector CVector_NegateValue_rail(const CVector &)
    asm("__mi__C7CVector");
CVector operator+(const CVector &, const CVector &);
CVector operator-(const CVector &, const CVector &);

// === Extern "C" gp-relative globals ===
// GCC 2.x emits a private copy of `CClass::~CClass` beside a private base
// vtable in every owner that derives from CClass.  `MAIN.SYM` records 25 such
// `_._6CClass` copies and names no base vtable, so both are file-local here.
// The filesystem-safe spelling keeps the per-address target assembly reachable.
static void Rail_ClassDestructor(CClass *self, int flags)
    asm("__DOT__6CClass_80051EDC");

// C++ forbids taking a destructor's address, so the public vtable slot uses an
// addressable declaration of the symbol this owner already defines.
extern "C" void Rail_DestructorAbi(CRail *self, int flags)
    asm("_._5CRail");

// Three consecutive eight-byte-aligned small-data words per vtable rather than
// arrays, so -G8 still yields the target's `addiu $vX, $gp, off` form.
extern "C" int __vt_5CRail
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int Rail_VTableIndex = 0;
static void (*Rail_VTableDtor)(CRail *, int) = Rail_DestructorAbi;

static int Rail_BaseVTableWord
    __attribute__((section(".sdata"), aligned(8))) = 0;
static int Rail_BaseVTableIndex = 0;
static void (*Rail_BaseVTableDtor)(CClass *, int) = Rail_ClassDestructor;

// === External function declarations ===
// Const-qualified view of the same object.  The lookup-table base read in
// CRail::CRail must float above the bounding-box stores; through the plain
// declaration cc1plus keeps a may-alias edge from `this->mBBox*` to the global
// load and pins it just before its use.  A const object is RTX_UNCHANGING, so
// the edge disappears and the load lands where the original has it.
extern "C" short **const OffsetList_c asm("OffsetList")
    __attribute__((section(".data")));
#include "MECHLIST.h"

// === Owner-local free function declarations ===
int GetIndexOfNode(unsigned short node);
void SetupLookup(void);

// === Function implementations (address order) ===

/**
 * Rail_Flags__FUs
 *
 * Address:   0x8005086C
 * Mangled:   Rail_Flags__FUs
 * Demangled: Rail_Flags(unsigned short)
 *
 * @param Node  Rail / graph node identifier.
 * @return The stored rail flags, or zero when the node has no rail definition.
 */
unsigned short Rail_Flags(unsigned short Node)
{
    CVector Dummy;
    // MAIN.SYM records pRailDef as signed because -1 is the absent-rail
    // sentinel in the payload returned through the canonical unsigned API.
    short *pRailDef = reinterpret_cast<short *>(
        Trig_GetPosition(Dummy, Node));
    pRailDef += 3;
    if (*pRailDef == -1)
        return 0;
    return (unsigned short)*pRailDef;
}

/**
 * Rail_FlagsPointer__FUs
 *
 * Address:   0x800508A8
 * Mangled:   Rail_FlagsPointer__FUs
 * Demangled: Rail_FlagsPointer(unsigned short)
 *
 * @param Node  Rail / graph node identifier.
 * @return The mutable rail-flag word, or null when no definition is present.
 */
unsigned short *Rail_FlagsPointer(unsigned short Node)
{
    CVector Dummy;
    // Keep the signed MAIN.SYM view for the -1 sentinel; callers receive the
    // same storage through this function's unsigned-word interface.
    short *pRailDef = reinterpret_cast<short *>(
        Trig_GetPosition(Dummy, Node));
    pRailDef += 3;
    if (*pRailDef == -1)
        return 0;
    return (unsigned short *)pRailDef;
}

/**
 * Rail_IsRailNode__FUs
 *
 * Address:   0x800508E4
 * Mangled:   Rail_IsRailNode__FUs
 * Demangled: Rail_IsRailNode(unsigned short)
 *            Check whether a node is rail terrain and is enabled for the active players.
 *
 * @param Node  Rail / graph node identifier.
 * @return Nonzero when the node is rail terrain enabled for this player count.
 */
int Rail_IsRailNode(unsigned short Node)
{
    int result;
    short *entry = OffsetList[Node];
    int type = (unsigned short)*entry;
    if ((unsigned)(type - 10) < 2)
    {
        unsigned int flags = Rail_Flags(Node);
        unsigned int players =
            (flags & RAIL_PLAYER_FLAG_MASK) >> RAIL_PLAYER_FLAG_SHIFT;
        if (players == 0)
            goto active_rail;
        if (players & GNumberOfPlayers)
            goto active_rail;
        return 0;
    }
    else
    {
        result = 0;
    }
    return result;

active_rail:
    return 1;
}

/**
 * Rail_GetLink__FUss
 *
 * Address:   0x80050964
 * Mangled:   Rail_GetLink__FUss
 * Demangled: Rail_GetLink(unsigned short, short)
 *
 * @param Node  Rail / graph node identifier.
 * @param Link  Zero-based link index.
 * @return The linked trigger-node identifier.
 */
unsigned short Rail_GetLink(unsigned short Node, short Link)
{
    unsigned short *links = Trig_GetLinksPointer(Node);
    return links[Link + 1];
}

/**
 * Rail_NumLinks__FUs
 *
 * Address:   0x8005099C
 * Mangled:   Rail_NumLinks__FUs
 * Demangled: Rail_NumLinks(unsigned short)
 *
 * @param Node  Rail / graph node identifier.
 * @return Number of links stored on the trigger node.
 */
unsigned short Rail_NumLinks(unsigned short Node)
{
    unsigned short *links = Trig_GetLinksPointer(Node);
    return *links;
}

/**
 * Rail_NextNode__FUs
 *
 * Address:   0x800509C0
 * Mangled:   Rail_NextNode__FUs
 * Demangled: Rail_NextNode(unsigned short)
 *
 * @param Node  Rail / graph node identifier.
 * @return The last enabled rail link, or zero when none exists.
 */
unsigned short Rail_NextNode(unsigned short Node)
{
    int Links = Rail_NumLinks(Node);
    int Link = 0;
    int NextRailNode;

    if (Links == 0)
    {
        return 0;
    }

    NextRailNode = 0;
    for (Link = 0; Link < Links; Link++)
    {
        unsigned short linkedNode = Rail_GetLink(Node, (short)Link);

        if (Rail_IsRailNode(linkedNode))
        {
            if (!(Rail_Flags(linkedNode) & RAIL_DISABLED_FLAG))
            {
                NextRailNode = linkedNode;
            }
        }
    }

    return (unsigned short)NextRailNode;
}

/**
 * Rail_PrevNode__FUs
 *
 * Address:   0x80050A74
 * Mangled:   Rail_PrevNode__FUs
 * Demangled: Rail_PrevNode(unsigned short)
 *
 * @param Node  Rail / graph node identifier.
 * @return The rail node whose next link reaches `Node`, or zero if absent.
 */
unsigned short Rail_PrevNode(unsigned short Node)
{
    int Prev;
    for (Prev = 1; Prev < NumNodes; Prev++)
    {
        if (Rail_IsRailNode((unsigned short)Prev))
        {
            if (Rail_NextNode((unsigned short)Prev) == Node)
            {
                return (unsigned short)Prev;
            }
        }
    }
    return 0;
}

/**
 * Rail_KillRail__Fi
 *
 * Address:   0x80050AF8
 * Mangled:   Rail_KillRail__Fi
 * Demangled: Rail_KillRail(int)
 *
 * @param node  Rail / graph node identifier.
 */
void Rail_KillRail(int node)
{
    CRail *pRail = RailList;
    while (pRail != 0)
    {
        int CurrentNode = pRail->mStartNode;
        for (;;)
        {
            if (CurrentNode == node)
            {
                // Retain the source-level null guard visible in the target
                // control flow around the virtual destructor call.
                if (pRail != 0)
                {
                    Rail_Destroy(pRail);
                }
                return;
            }
            int NextNode = Rail_NextNode((unsigned short)CurrentNode);
            if (NextNode == 0)
                break;
            if (NextNode == pRail->mStartNode)
                break;
            CurrentNode = NextNode;
        }
        pRail = pRail->mpNext;
    }
}

/**
 * Finds a rail node's 1-based index in RailNodeLookup.
 *
 * Address:   0x80050B98
 * Mangled:   GetIndexOfNode__FUs
 * Demangled: GetIndexOfNode(unsigned short)
 *
 * @param node  Rail / graph node identifier.
 * @return The 1-based lookup index, or 0 when the node is absent.
 */
int GetIndexOfNode(unsigned short node)
{
    if (node == 0)
        return 0;

    int r = 1;
    if (r < TotalRailNodes)
    {
        NodeSort *pNode = RailNodeLookup + 1;
        do
        {
            if (pNode->nodeNum == node)
                return r;
            r++;
            pNode++;
        } while (r < TotalRailNodes);
    }
    return 0;
}

/**
 * SetupLookup__Fv
 *
 * Address:   0x80050BFC
 * Mangled:   SetupLookup__Fv
 * Demangled: SetupLookup(void)
 *            Build the rail-node lookup table and record each rail node's previous index.
 */
void SetupLookup(void)
{
    unsigned short r;
    unsigned short n;

    RailNodeLookup = static_cast<NodeSort *>(
        Mem_New(RAIL_NODE_CAPACITY * sizeof(NodeSort), 1, 1));
    r = 1;

    for (n = 0; n < NumNodes; n++)
    {
        if (Rail_IsRailNode(n))
        {
            int offset = r * sizeof(NodeSort);
            r++;
            int base = RAIL_LOOKUP_ADDRESS(RailNodeLookup);
            NodeSort *p = RAIL_LOOKUP_AT_ADDRESS(offset + base);
            p->nodeNum = n;
            p->prevNodeIndex = 0;
        }
    }

    int total;
    RAIL_PUBLISH_NODE_COUNT(r, total);
    RailNodeLookup[0].nodeNum = 0;

    r = 1;
    if (r < total)
    {
        do
        {
            unsigned short node = RailNodeLookup[r].nodeNum;
            int next_r = GetIndexOfNode(Rail_NextNode(node));
            if (next_r)
            {
                RailNodeLookup[next_r].prevNodeIndex = r;
            }
            r++;
        } while (r < TotalRailNodes);
    }
}

/**
 * Rail_CreateRails__Fv
 *
 * Address:   0x80050CF8
 * Mangled:   Rail_CreateRails__Fv
 * Demangled: Rail_CreateRails(void)
 *            Normalize rail trigger nodes, build lookup links, and instantiate rail objects.
 */
void Rail_CreateRails(void)
{
    int total;
    int rail_num;

    M3dColij_FlushCache();

    total = NumNodes;
    if (NumNodes > 0)
    {
        unsigned short Node = 0;
        do
        {
            short *entry = OffsetList[Node];
            short type = *entry;
            if (type < 12)
            {
                if (type > 9)
                {
                    *entry = 10;
                    total = NumNodes;
                }
            }
            Node++;
        } while ((int)Node < total);
    }

    rail_num = 1;
    SetupLookup();

    unsigned short Node = 0;
    if (NumNodes > 0)
    {
        do
        {
            int is_rail_node = Rail_IsRailNode(Node);
            short *entry = OffsetList[Node];
            if (*entry == 10)
            {
                if (is_rail_node)
                {
                    unsigned int prevIndex =
                        RailNodeLookup[rail_num].prevNodeIndex;
                    unsigned short Prev = RailNodeLookup[prevIndex].nodeNum;
                    unsigned short First = Node;

                    for (;;)
                    {
                        if (Rail_Flags(First) & RAIL_DISABLED_FLAG)
                            break;
                        if (Prev == 0)
                            break;

                        prevIndex = RailNodeLookup[prevIndex].prevNodeIndex;
                        First = Prev;
                        Prev = RailNodeLookup[prevIndex].nodeNum;
                        if (Prev == Node)
                        {
                            First = Node;
                            break;
                        }
                    }

                    new CRail(First);
                    goto next_node;
                }
            }
            else
            {
next_node:
                if (is_rail_node)
                {
                    rail_num++;
                }
            }

            Node++;
        } while ((int)Node < NumNodes);
    }

    Mem_Delete(RailNodeLookup);
}

/**
 * Rail_Init__Fv
 *
 * Address:   0x80050EC4
 * Mangled:   Rail_Init__Fv
 * Demangled: Rail_Init(void)
 *            Destroy each CRail instance and clear the global list head.
 */
void Rail_Init(void)
{
    while (RailList != 0)
    {
        Rail_Destroy(RailList);
    }
    RailList = 0;
}

/**
 * Rail_DeleteAll__Fv
 *
 * Address:   0x80050F0C
 * Mangled:   Rail_DeleteAll__Fv
 * Demangled: Rail_DeleteAll(void)
 *
 */
void Rail_DeleteAll(void)
{
    CRail *pRail = RailList;
    if (pRail != 0)
    {
        do
        {
            CRail *pNext = pRail->mpNext;
            // The original loop guards each virtual call independently even
            // though the enclosing condition already proved this iteration.
            if (pRail != 0)
            {
                Rail_Destroy(pRail);
            }
            pRail = pNext;
        } while (pRail != 0);
    }
}

/**
 * Reconstruct a rail from its linked trigger nodes.
 *
 * Address:   0x80050F60
 * Mangled:   __5CRaili
 * Demangled: CRail::CRail(int)
 *
 */
CRail::CRail(int node_index)
{
    CVector NodePos;

    vtable = &__vt_5CRail;

    // Insert this rail at the head of the global doubly linked list.  Holding
    // the old head in a local is what lets `mStartNode` be stored before the
    // link fields without forcing a reload of the global: with a direct
    // `mpNext = RailList` the store to `this->mStartNode` may alias the global
    // pointer and GCC re-reads it.
    CRail *head = RailList;
    if (head != 0)
        head->mpPrevious = this;
    mStartNode = node_index;
    RAIL_NEXT_LINK_ALIAS(this) = head;
    RAIL_PREVIOUS_LINK_ALIAS(this) = 0;
    RailList = this;

    // Reading the member back (rather than reusing `node_index`) makes the
    // zero-extended call argument depend on the `sh` above, so cc1plus emits
    // the `andi` at the call instead of hoisting it over the list insertion.
    mType = Rail_Flags((unsigned short)mStartNode) & RAIL_TYPE_MASK;

    // Build a bounding box while marking every member node as processed.
    mBBoxMin.vx = mBBoxMin.vy = mBBoxMin.vz = RAIL_COORD_MAX;
    mBBoxMax.vx = mBBoxMax.vy = mBBoxMax.vz = RAIL_COORD_MIN;

    // MAIN.SYM places NodePos outside the traversal and the collision-query
    // values in the successful-link block below. Those lifetimes also recover
    // the target's stack slots for every named local and aggregate temporary.
    int CurrentNode = mStartNode;
    *OffsetList_c[CurrentNode] = 11;

    do
    {
        unsigned short NextNode;

        Trig_GetPosition(NodePos, CurrentNode);
        if (NodePos.vx < mBBoxMin.vx)
            mBBoxMin.vx = NodePos.vx;
        if (NodePos.vy < mBBoxMin.vy)
            mBBoxMin.vy = NodePos.vy;
        if (NodePos.vz < mBBoxMin.vz)
            mBBoxMin.vz = NodePos.vz;
        if (NodePos.vx > mBBoxMax.vx)
            mBBoxMax.vx = NodePos.vx;
        if (NodePos.vy > mBBoxMax.vy)
            mBBoxMax.vy = NodePos.vy;
        if (NodePos.vz > mBBoxMax.vz)
            mBBoxMax.vz = NodePos.vz;

        NextNode = Rail_NextNode((unsigned short)CurrentNode);
        if (NextNode == 0)
            return;

        {
            /*
             * The prototype validates each rail span against level collision.
             * Bad spans are disabled through the high rail-flag bit instead of
             * being admitted to the traversal loop.
             */
            CVector StartPos;
            CVector EndPos;
            CVector MidPos;
            CVector Stick;
            int temp;

            Trig_GetPosition(StartPos, CurrentNode);
            Trig_GetPosition(EndPos, NextNode);

            MidPos = StartPos + EndPos;
            MidPos /= 2;
            MidPos -= StartPos;
            MidPos /= 2;
            MidPos += StartPos;
            MidPos.vy -= RAIL_COLLISION_INSET;

            Stick = EndPos - StartPos;
            Stick.vy = 0;
            M3dUtils_NormaliseVector(&Stick);
            temp = Stick.vx;
            Stick.vx = Stick.vz;
            Stick.vz = -temp;
            Stick *= 10;

            SLineInfo line_info;
            line_info.pItem = 0;
            BulletCollisionCheck = 1;
            BaddyCollisionCheck = 1;

            if (GenerateLevel)
            {
                // The first collision probe addresses line_info through its own
                // pointer, which is invalidated before the retry probes below
                // so each group re-forms the address the way the original does.
                //
                // The pointer belongs to the guarded block, not to the enclosing
                // one.  Declared outside, its `addiu $s0, $sp, 0x70` joins the
                // pre-branch block and the scheduler then ranks the `li $v0, 1`
                // feeding the two collision-check stores above the
                // `GenerateLevel` load, emitting the constant before the test.
                // Declared here the address computation belongs to the guarded
                // block, delayed-branch scheduling pulls it into the `beqz`
                // delay slot, and the load moves ahead of the constant exactly
                // as the original has it.
                SLineInfo *pLine = &line_info;
                line_info.StartCoords.vx = MidPos.vx + Stick.vx;
                line_info.StartCoords.vy = MidPos.vy;
                line_info.StartCoords.vz = MidPos.vz + Stick.vz;
                line_info.EndCoords.vx = MidPos.vx - Stick.vx;
                line_info.EndCoords.vy = MidPos.vy;
                line_info.EndCoords.vz = MidPos.vz - Stick.vz;
                M3dColij_InitLineInfo(pLine);
                M3dZone_LineToItem(pLine, 1);

                if (line_info.pItem == 0)
                {
                    line_info.StartCoords.vx = MidPos.vx - Stick.vx;
                    line_info.StartCoords.vy = MidPos.vy;
                    line_info.StartCoords.vz = MidPos.vz - Stick.vz;
                    line_info.EndCoords.vx = MidPos.vx + Stick.vx;
                    line_info.EndCoords.vy = MidPos.vy;
                    line_info.EndCoords.vz = MidPos.vz + Stick.vz;
                    M3dColij_InitLineInfo(pLine);
                    M3dZone_LineToItem(pLine, 1);
                }
                // Killing the pointer inside the guarded block (rather than
                // after it) leaves nothing on the `GenerateLevel == 0` path, so
                // jump threading forwards that edge past both retry blocks the
                // way the original does.
                pLine = 0;
            }

            if (GenerateLevel && line_info.pItem == 0)
            {
                Stick.vx *= 2;
                Stick.vz *= 2;

                line_info.StartCoords.vx = MidPos.vx - Stick.vx;
                line_info.StartCoords.vy = MidPos.vy;
                line_info.StartCoords.vz = MidPos.vz - Stick.vz;
                line_info.EndCoords.vx = line_info.StartCoords.vx;
                line_info.EndCoords.vy = MidPos.vy + RAIL_COLLISION_HEIGHT;
                line_info.EndCoords.vz = line_info.StartCoords.vz;
                M3dColij_InitLineInfo(&line_info);
                M3dZone_LineToItem(&line_info, 1);
                int leftside = line_info.pItem != 0;

                line_info.StartCoords.vx = MidPos.vx + Stick.vx;
                line_info.StartCoords.vy = MidPos.vy;
                line_info.StartCoords.vz = MidPos.vz + Stick.vz;
                line_info.EndCoords.vx = line_info.StartCoords.vx;
                line_info.EndCoords.vy = MidPos.vy + RAIL_COLLISION_HEIGHT;
                line_info.EndCoords.vz = line_info.StartCoords.vz;
                M3dColij_InitLineInfo(&line_info);
                M3dZone_LineToItem(&line_info, 1);

                if (line_info.pItem != 0 && !leftside)
                    line_info.pItem = 0;
            }

            if (GenerateLevel && line_info.pItem == 0)
            {
                Stick.vx *= 2;
                Stick.vz *= 2;
                MidPos.vy += RAIL_COLLISION_RAISE;
                line_info.StartCoords.vx = MidPos.vx + Stick.vx;
                line_info.StartCoords.vy = MidPos.vy;
                line_info.StartCoords.vz = MidPos.vz + Stick.vz;
                line_info.EndCoords.vx = MidPos.vx - Stick.vx;
                line_info.EndCoords.vy = MidPos.vy;
                line_info.EndCoords.vz = MidPos.vz - Stick.vz;
                M3dColij_InitLineInfo(&line_info);
                M3dZone_LineToItem(&line_info, 1);

                if (line_info.pItem == 0)
                {
                    line_info.StartCoords.vx = MidPos.vx - Stick.vx;
                    line_info.StartCoords.vy = MidPos.vy;
                    line_info.StartCoords.vz = MidPos.vz - Stick.vz;
                    line_info.EndCoords.vx = MidPos.vx + Stick.vx;
                    line_info.EndCoords.vy = MidPos.vy;
                    line_info.EndCoords.vz = MidPos.vz + Stick.vz;
                    M3dColij_InitLineInfo(&line_info);
                    M3dZone_LineToItem(&line_info, 1);
                }

                if (line_info.pItem != 0)
                    line_info.pItem = 0;
                else
                    line_info.pItem = (void *)-1;
            }

            BulletCollisionCheck = 0;
            if (GenerateLevel && line_info.pItem != 0)
            {
                unsigned short *pFlags = Rail_FlagsPointer(NextNode);
                *pFlags |= RAIL_DISABLED_FLAG;
                return;
            }

            CurrentNode = NextNode;
        }
        *OffsetList[CurrentNode] = 11;
        NextNode = Rail_NextNode((unsigned short)CurrentNode);
    } while (CurrentNode != mStartNode);

}

/**
 * CRail::~CRail()
 *
 * Address:   0x80051568
 * Mangled:   _._5CRail
 * Demangled: CRail::~CRail()
 *
 */
CRail::~CRail()
{
    CRail *self = this;
    CClass *base = self;
    CRail *head = RailList;
    RAIL_VTABLE_ALIAS(self) = &__vt_5CRail;
    if (head == self)
    {
        RailList = self->mpNext;
    }
    {
        CRail *previous = self->mpPrevious;
        if (previous != 0)
        {
            previous->mpNext = self->mpNext;
        }
    }
    {
        CRail *next = self->mpNext;
        if (next != 0)
        {
            next->mpPrevious = self->mpPrevious;
        }
    }
    base->vtable = &Rail_BaseVTableWord;
}

/**
 * Update the closest eligible rail for each active skater.
 *
 * Address:   0x800515EC
 * Mangled:   Rail_Update__Fv
 * Demangled: Rail_Update(void)
 *
 * Finds the closest eligible rail segment for every active skater and records
 * the rail direction, contact point, and ledge side needed by physics.
 */
void Rail_Update(void)
{
    int s;
    int t;
    int SkaterMin_vx;
    int SkaterMin_vy;
    int SkaterMin_vz;
    int SkaterMax_vx;
    int SkaterMax_vy;
    int SkaterMax_vz;

    CVector SegMin;
    CVector SegMax;
    CVector SegFrom;
    CVector SegTo;
    CVector SkaterPos;
    CVector RailPos;

    // Multiple players share MechList, so inspect every skater body.
    for (CBodyOwner *MLCopy = (CBodyOwner *)MechList; MLCopy != 0;
         MLCopy = (CBodyOwner *)MLCopy->CItem.mNextItem)
    {
        CBruceRailOwner *pSkater = (CBruceRailOwner *)MLCopy;

        if (!pSkater->mPad->TrianglePressed)
        {
            pSkater->mRailInhibit = 0;
            continue;
        }
        if (pSkater->mOnRail)
            continue;
        if (pSkater->mInBail)
            continue;
        if (pSkater->mRailInhibit)
            continue;

        SkaterMin_vx = pSkater->CSuper.CBody.mOldPos.vx;
        SkaterMin_vy = pSkater->CSuper.CBody.mOldPos.vy;
        SkaterMin_vz = pSkater->CSuper.CBody.mOldPos.vz;
        SkaterMax_vx = pSkater->CSuper.CBody.CItem.mPos.vx;
        SkaterMax_vy = pSkater->CSuper.CBody.CItem.mPos.vy;
        SkaterMax_vz = pSkater->CSuper.CBody.CItem.mPos.vz;

        if (SkaterMin_vx > SkaterMax_vx)
        {
            t = SkaterMin_vx;
            SkaterMin_vx = SkaterMax_vx;
            SkaterMax_vx = t;
        }
        if (SkaterMin_vy > SkaterMax_vy)
        {
            t = SkaterMin_vy;
            SkaterMin_vy = SkaterMax_vy;
            SkaterMax_vy = t;
        }
        if (SkaterMin_vz > SkaterMax_vz)
        {
            t = SkaterMin_vz;
            SkaterMin_vz = SkaterMax_vz;
            SkaterMax_vz = t;
        }

        // Expand the swept skater box by the 100-unit rail snap distance.
        SkaterMin_vx -= RAIL_SNAP_DISTANCE;
        SkaterMax_vx += RAIL_SNAP_DISTANCE;
        SkaterMin_vy -= RAIL_SNAP_DISTANCE;
        SkaterMax_vy += RAIL_SNAP_DISTANCE;
        SkaterMin_vz -= RAIL_SNAP_DISTANCE;
        SkaterMax_vz += RAIL_SNAP_DISTANCE;

        for (CRail *pRail = RailList; pRail != 0;
             pRail = pRail->mpNext)
        {
            int CurrentNode;
            int NextNode;
            int Dist;

            if (SkaterMax_vx < pRail->mBBoxMin.vx ||
                SkaterMin_vx > pRail->mBBoxMax.vx)
                continue;
            if (SkaterMax_vy < pRail->mBBoxMin.vy ||
                SkaterMin_vy > pRail->mBBoxMax.vy)
                continue;
            if (SkaterMax_vz < pRail->mBBoxMin.vz ||
                SkaterMin_vz > pRail->mBBoxMax.vz)
                continue;

            if (pSkater->mSameRailTimer > 0 &&
                pSkater->mDontStickRail == pRail->mStartNode)
                continue;

            for (CurrentNode = pRail->mStartNode;;
                 CurrentNode = NextNode)
            {
                NextNode = Rail_NextNode((unsigned short)CurrentNode);
                if (NextNode == 0)
                    break;

                Trig_GetPosition(SegFrom, CurrentNode);
                Trig_GetPosition(SegTo, NextNode);
                SegMin = SegFrom;
                SegMax = SegTo;

                if (SegMin.vx > SegMax.vx)
                {
                    t = SegMin.vx;
                    SegMin.vx = SegMax.vx;
                    SegMax.vx = t;
                }
                if (SegMin.vy > SegMax.vy)
                {
                    t = SegMin.vy;
                    SegMin.vy = SegMax.vy;
                    SegMax.vy = t;
                }
                if (SegMin.vz > SegMax.vz)
                {
                    t = SegMin.vz;
                    SegMin.vz = SegMax.vz;
                    SegMax.vz = t;
                }

                if (SkaterMax_vx >= SegMin.vx &&
                    SkaterMin_vx <= SegMax.vx &&
                    SkaterMax_vy >= SegMin.vy &&
                    SkaterMin_vy <= SegMax.vy &&
                    SkaterMax_vz >= SegMin.vz &&
                    SkaterMin_vz <= SegMax.vz)
                {
                    Dist = M3dUtils_LineLineClosestPoints(
                        (CVector *)&pSkater->CSuper.CBody.mOldPos,
                        (CVector *)&pSkater->CSuper.CBody.CItem.mPos,
                        &SegFrom, &SegTo, &SkaterPos, &s, &RailPos, &t);

                    if (Dist < pSkater->mDistToCurrentRail)
                    {
                        if ((pSkater->mPhysicsState !=
                                 PHYSICS_ON_GROUND_RAIL_OWNER &&
                             pSkater->mPhysicsState !=
                                 PHYSICS_ON_INVISIBLE_RAIL_OWNER) ||
                            RailPos.vy >= pSkater->CSuper.CBody.CItem.mPos.vy)
                        {
                            CVector SegDir;
                            CVector SkaterVel;
                            int VelocityDotSegment;

                            CVector_SubRefs_rail(&SegDir, SegTo, SegFrom);
                            SkaterVel = *(CVector *)&pSkater->CSuper.CBody.mVel;
                            M3dUtils_NormaliseVector(&SegDir);
                            VelocityDotSegment =
                                Mips_CVecDotProd(&SegDir, &SkaterVel);

                            // Branch-free absolute value of the fixed-point dot product.
                            if (((VelocityDotSegment ^
                                  (VelocityDotSegment >> 31)) -
                                 (VelocityDotSegment >> 31)) < RAIL_FIXED_ONE)
                            {
                                CVector SkaterVel = pSkater->mFront;
                                VelocityDotSegment =
                                    -Mips_CVecDotProd(&SegDir, &SkaterVel);
                            }

                            int End = 0;
                            if (VelocityDotSegment >= 0)
                            {
                                pSkater->mRailDir = SegDir;
                                pSkater->mRail_Direction = 1;
                                if (RailPos == SegTo)
                                {
                                    End = 1;
                                }
                            }
                            else
                            {
                                pSkater->mRailDir =
                                    CVector_NegateValue_rail(SegDir);
                                pSkater->mRail_Direction = -1;
                                if (RailPos == SegFrom)
                                {
                                    End = 1;
                                }
                            }

                            if (!End)
                            {
                                int ledge_left = 1;
                                pSkater->mDistToCurrentRail = Dist;
                                pSkater->mStickToRail = ledge_left;
                                pSkater->mCurrentRailStart = pRail->mStartNode;
                                pSkater->mCurrentRailSegment = CurrentNode;
                                pSkater->mRailPos = RailPos;
                                pSkater->mRailType = pRail->mType;

                                /*
                                 * Classify which side of the rail has supporting
                                 * collision so the grind animation can face the ledge.
                                 */
                                CVector StartPos;
                                CVector EndPos;
                                CVector MidPos;
                                CVector Stick;
                                CVector result;
                                int temp;
                                SLineInfo line_info;
                                int leftside;
                                int rightside;

                                Trig_GetPosition(StartPos, CurrentNode);
                                Trig_GetPosition(EndPos, NextNode);
                                int ledge_right = 2;
                                CVector_AddRefs_rail(&result, StartPos, EndPos);
                                MidPos = result;
                                MidPos /= 2;
                                MidPos -= StartPos;
                                MidPos /= 2;
                                MidPos += StartPos;
                                MidPos.vy -= RAIL_COLLISION_INSET;

                                CVector_SubRefs_rail(&result, EndPos, StartPos);
                                Stick = result;
                                Stick.vy = 0;
                                M3dUtils_NormaliseVector(&Stick);
                                temp = Stick.vx;
                                Stick.vx = Stick.vz;
                                Stick.vz = -temp;
                                Stick *= 10;

                                line_info.pItem = 0;
                                BulletCollisionCheck = ledge_left;
                                Stick.vx *= 2;
                                Stick.vz *= 2;

                                line_info.StartCoords.vx = MidPos.vx - Stick.vx;
                                line_info.StartCoords.vy = MidPos.vy;
                                line_info.StartCoords.vz = MidPos.vz - Stick.vz;
                                line_info.EndCoords.vx = line_info.StartCoords.vx;
                                line_info.EndCoords.vy =
                                    MidPos.vy + RAIL_COLLISION_HEIGHT;
                                line_info.EndCoords.vz = line_info.StartCoords.vz;
                                M3dColij_InitLineInfo(&line_info);
                                M3dZone_LineToItem(&line_info, ledge_left);
                                leftside = line_info.pItem != 0;

                                line_info.StartCoords.vx = MidPos.vx + Stick.vx;
                                line_info.StartCoords.vy = MidPos.vy;
                                line_info.StartCoords.vz = MidPos.vz + Stick.vz;
                                line_info.EndCoords.vx = line_info.StartCoords.vx;
                                line_info.EndCoords.vy =
                                    MidPos.vy + RAIL_COLLISION_HEIGHT;
                                line_info.EndCoords.vz = line_info.StartCoords.vz;
                                M3dColij_InitLineInfo(&line_info);
                                M3dZone_LineToItem(&line_info, ledge_left);
                                rightside = line_info.pItem != 0;

                                if (leftside | rightside)
                                {
                                    if (rightside)
                                        pSkater->mRail_LedgeType = ledge_right;
                                    if (leftside)
                                        pSkater->mRail_LedgeType = ledge_left;
                                }
                                else
                                {
                                    Stick.vx /= 4;
                                    Stick.vz /= 4;

                                    line_info.StartCoords.vx = MidPos.vx - Stick.vx;
                                    line_info.StartCoords.vy = MidPos.vy;
                                    line_info.StartCoords.vz = MidPos.vz - Stick.vz;
                                    line_info.EndCoords.vx = line_info.StartCoords.vx;
                                    line_info.EndCoords.vy =
                                        MidPos.vy + RAIL_COLLISION_SHORT_HEIGHT;
                                    line_info.EndCoords.vz = line_info.StartCoords.vz;
                                    M3dColij_InitLineInfo(&line_info);
                                    M3dZone_LineToItem(&line_info, ledge_left);
                                    leftside = line_info.pItem != 0;

                                    line_info.StartCoords.vx = MidPos.vx + Stick.vx;
                                    line_info.StartCoords.vy = MidPos.vy;
                                    line_info.StartCoords.vz = MidPos.vz + Stick.vz;
                                    line_info.EndCoords.vx = line_info.StartCoords.vx;
                                    line_info.EndCoords.vy =
                                        MidPos.vy + RAIL_COLLISION_SHORT_HEIGHT;
                                    line_info.EndCoords.vz = line_info.StartCoords.vz;
                                    M3dColij_InitLineInfo(&line_info);
                                    M3dZone_LineToItem(&line_info, ledge_left);
                                    rightside = line_info.pItem != 0;

                                    if (leftside != rightside)
                                    {
                                        if (rightside)
                                            pSkater->mRail_LedgeType =
                                                ledge_right;
                                        if (leftside)
                                            pSkater->mRail_LedgeType =
                                                ledge_left;
                                    }
                                    else
                                    {
                                        pSkater->mRail_LedgeType = 0;
                                    }
                                }
                                BulletCollisionCheck = 0;
                            }
                        }
                    }
                }
                if (NextNode == pRail->mStartNode)
                    break;
            }
        }
    }
}

// GCC emitted this private `CClass::~CClass` copy last in the owner's
// text, so it stays at the end of the translation unit.
static void Rail_ClassDestructor(CClass *self, int flags)
{
    self->vtable = &Rail_BaseVTableWord;
    if (flags & 1)
    {
        CClass::operator delete(self);
    }
}
