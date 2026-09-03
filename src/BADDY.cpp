// BADDY.cpp -- Reconstructed translation unit
// Original: BADDY.CPP
// Functions: 15 reconstructed entries
// Address range: 0x8004080C - 0x80042364
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#define __SKIP_CBADDY_EXTERNS__

#define BADDY_OWNER_DECLARATIONS
#define BADDY_LAYOUT_DECLARATIONS
#include "BADDY.h"
#undef BADDY_LAYOUT_DECLARATIONS
#undef BADDY_OWNER_DECLARATIONS
#include "MAIN.h"
#include "MECH.h"
#include "BIT.h"
#include "OB.h"
#include "CITEM.h"
#include "CBODY.h"
#include "CSUPER.h"
#include "CPOWERUP.h"
#include "SPOOL.h"
#include "TRIG.h"
#include "PANEL.h"

/** Return caller-owned storage for an ordinary placement construction. */
inline void *operator new(unsigned int, void *storage)
{
    return storage;
}

struct CGPolyLine;

/** Camera-shake magnitudes recovered from MAIN_STRUCTS.h. */
enum EShakeType
{
    CAMERASHAKE_BIG = 0,
    CAMERASHAKE_MEDIUM = 1,
    CAMERASHAKE_SMALL = 2,
};

/** Opaque, exact-size camera view used by scripted shake commands. */
struct CCamera
{
    unsigned char opaque[1668];

    /** Apply a camera-shake envelope from a world-space source. */
    void Shake(const CVector &position, EShakeType magnitude);
};

typedef char Baddy_CCamera_size_check[
    (sizeof(CCamera) == 1668) ? 1 : -1];

// Typed aliases for methods whose owner-local layouts remain composition-only.
#define CSUPER_DECLARE_DESTRUCTOR
#define CSUPER_DESTRUCTOR_NAME Baddy_CSuperDestructorAbi
#include "CSUPER.h"
#include "CVECTOROPS.h"

#include "BIT.h"

#include "SFX.h"
#include "MEM.h"
#include "CAREER.h"
#include "UTILS.h"
#include "POWERUP.h"
#include "SMOKE.h"
#include "FLASH.h"
#include <STDIO.H>

/** Replay modes used by the goal-counter video guard. */
#include "VIDEO.h"

/** Owner-local prefix needed for the goal-counter video-mode guard. */
struct SVideoInfo {
    enum EVideoMode VideoMode;
};

/** Clear the three components of the original inline CVector value. */
static inline void Baddy_ClearVector(CVector &vector)
{
    vector.vz = 0;
    vector.vy = 0;
    vector.vx = 0;
}

/** Method-bearing owner-local view of the original inline CSVector type. */
struct BaddyCSVector : CSVectorOwner {
    /** Assign all three fixed-point vector components. */
    void Set(int x, int y, int z)
    {
        vx = x;
        vy = y;
        vz = z;
    }
};

/** Variable-reading slot used in both its normal and register-shaped forms. */
union BaddyVariableSlot
{
    short (*GetVariable)(CBaddy *, unsigned short);
    int (*GetVariableInt)(CBaddy *, unsigned short);
};

/** Typed view of the twelve-slot CBaddy dispatch table. */
struct BaddyVTable
{
    void *reserved[2];
    void (*Destroy)(CBaddy *, int);
    void (*Die)(CBaddy *);
    void (*AI)(CBaddy *);
    void (*EveryFrame)(CBodyOwner *);
    void (*DoLightingSetup)(CBodyOwner *);
    int (*Hit)(CBodyOwner *, int, const CVector &, int);
    void (*DeleteStuff)(CBodyOwner *);
    int (*ExecuteCommand)(CBaddy *, unsigned short);
    void (*SetVariable)(CBaddy *, unsigned short);
    BaddyVariableSlot Variable;
};

/** Exact function types stored in the base CBaddy table definition. */
struct BaddyBaseVTableDefinition
{
    void *reserved[2];
    void (*Destroy)(CBaddy *, int);
    void (*Die)(CBaddy *);
    void (*AI)(CBodyOwner *);
    void (*EveryFrame)(CBodyOwner *);
    void (*DoLightingSetup)(CBodyOwner *);
    int (*Hit)(CBodyOwner *, int, const CVector &, int);
    void (*DeleteStuff)(CBodyOwner *);
    int (*ExecuteCommand)(CBaddy *, unsigned short);
    void (*SetVariable)(CBaddy *, unsigned short);
    short (*GetVariable)(CBaddy *, unsigned short);
};

/** Exact function types stored in CScriptOnlyBaddy's table definition. */
struct BaddyScriptOnlyVTableDefinition
{
    void *reserved[2];
    void (*Destroy)(CScriptOnlyBaddy *, int);
    void (*Die)(CBaddy *);
    void (*AI)(CScriptOnlyBaddy *);
    void (*EveryFrame)(CBodyOwner *);
    void (*DoLightingSetup)(CBodyOwner *);
    int (*Hit)(CBodyOwner *, int, const CVector &, int);
    void (*DeleteStuff)(CBodyOwner *);
    int (*ExecuteCommand)(CScriptOnlyBaddy *, unsigned short);
    void (*SetVariable)(CScriptOnlyBaddy *, unsigned short);
    short (*GetVariable)(CBaddy *, unsigned short);
};

/** Typed prefix shared by every CBit-derived effect table. */
struct BaddyEffectVTable
{
    void *reserved[2];
    void (*Destroy)(void *, int);
};

/** Recover a baddy's target dispatch table from the composed CItem base. */
static inline BaddyVTable *Baddy_Dispatch(CBaddy *baddy)
{
    return (BaddyVTable *)baddy->CSuper.CBody.CItem.vtable;
}

typedef char BaddyVTable_size_check[
    (sizeof(BaddyVTable) == 48) ? 1 : -1];
typedef char BaddyEffectVTable_size_check[
    (sizeof(BaddyEffectVTable) == 12) ? 1 : -1];
typedef char BaddyBaseVTableDefinition_size_check[
    (sizeof(BaddyBaseVTableDefinition) == 48) ? 1 : -1];
typedef char BaddyScriptOnlyVTableDefinition_size_check[
    (sizeof(BaddyScriptOnlyVTableDefinition) == 48) ? 1 : -1];

// === External globals (absolute via incomplete array) ===
// DAT_800EA278 has no target identity or BADDY references.

// === External globals ===
extern BaddyScriptOnlyVTableDefinition const __vt_16CScriptOnlyBaddy;
extern BaddyBaseVTableDefinition const __vt_6CBaddy;
int NumBaddies = 0;
int NumGenericBaddies = 0;
int NumHoverSWATS = 0;
int NumZombies = 0;
// Scalar storage view of the first word; the helper below restores the full
// CVector type carried by the linker-visible ZeroVector object.
#define ZEROVECTOR_SCALAR_DECLARATION
#include "ZEROVECTOR.h"
#undef ZEROVECTOR_SCALAR_DECLARATION

/** Return the full vector stored at the linker-visible `ZeroVector` base. */
static inline CVector &Baddy_ZeroVector()
{
    return *(CVector *)&ZeroVector;
}
#include "MECHLIST.h"
#define BODYLISTS_DECLARE_ENVIRONMENTAL_OBJECT_LIST
#include "BODYLISTS.h"
#define SFXV_ABSOLUTE_DECLARATION
#include "SFXV.h"

/** Target-complete transient-bit base used by the weather-line consumer. */
struct CBit {
    CBit *mPrevious;
    CBit *mNext;
    short mAge;
    unsigned short mLifetime;
    CVector mTweenPos;
    CVector mPos;
    CVector mVel;
    CVector mAcc;
    CFriction mFric;
    unsigned char mDead;
    unsigned short mFrigDeltaZ;
    unsigned char mProtected;
    unsigned char mType;
    BaddyEffectVTable *_vf;

    /** Allocate zero-initialized storage from the transient-bit heap. */
    static void *operator new(unsigned int size);
    /** Release transient-bit storage through its owning allocator. */
    static void operator delete(void *p);
};

/** Exact-size spark storage used only for construction in this owner. */
struct CSpark {
    unsigned char opaque[BIT_SPARK_STORAGE_SIZE];

    /** Construct one transient spark. */
    CSpark(CVector &position, int velocity, int gravity, int life);
};

#include "CSPECIALDISPLAY.h"
#include "CTEXTUREDRIBBON.h"
#include "CSMOKEJET.h"

#include "SLINESEG.h"
#include "CGPOLYLINE.h"

typedef char Baddy_SLineSeg_size_must_be_16[
    (sizeof(struct SLineSeg) == 16) ? 1 : -1];
typedef char Baddy_CBit_size_must_be_72[
    (sizeof(CBit) == 72) ? 1 : -1];
typedef char Baddy_CSpecialDisplay_size_must_be_72[
    (sizeof(CSpecialDisplay) == 72) ? 1 : -1];
typedef char Baddy_CTexturedRibbon_size_must_be_108[
    (sizeof(CTexturedRibbon) == 108) ? 1 : -1];
typedef char Baddy_SWidthWobble_size_must_be_8[
    (sizeof(SWidthWobble) == 8) ? 1 : -1];
typedef char Baddy_CSmokeJet_size_must_be_364[
    (sizeof(CSmokeJet) == 364) ? 1 : -1];
typedef char Baddy_CGPolyLine_size_must_be_100[
    (sizeof(CGPolyLine) == 100) ? 1 : -1];
typedef char Baddy_SRainEntry_size_must_be_16[
    (sizeof(SRainEntry) == 16) ? 1 : -1];

// === Extern "C" forward declarations ===
#define CBADDY_DECLARE_CONSTRUCTOR
#define CBADDY_CONSTRUCTOR_NAMESPACE BaddyConstructorABI
#define CBADDY_CONSTRUCTOR_RECEIVER CBaddy
#define CBADDY_DECLARE_DESTRUCTOR
#define CBADDY_DESTRUCTOR_NAME Baddy_CBaddyDestructorAbi
#define CBADDY_DESTRUCTOR_RECEIVER CBaddy
#define CBADDY_DECLARE_SCRIPT_ONLY_DESTRUCTOR
#define CBADDY_SCRIPT_ONLY_DESTRUCTOR_NAME Baddy_ScriptOnlyDestructorAbi
#define CBADDY_SCRIPT_ONLY_DESTRUCTOR_RECEIVER CScriptOnlyBaddy
#include "CBADDYABI.h"
#define CBaddy_construct BaddyConstructorABI::__6CBaddy

// === Function implementations (address order) ===

/**
 * __6CBaddy (extern "C")
 *
 * Address:   0x8004080C
 * Mangled:   __6CBaddy
 * Demangled: CBaddy::CBaddy(void)
 *            Initialize CBaddy defaults and bump the active baddy count.
 *
 * @param baddy  Baddy instance being initialized.
 * @return `this` pointer (GCC 2.x ctor convention).
 */
CBaddy *CBaddy_construct(CBaddy *baddy)
{
    ::new (reinterpret_cast<CSuper *>(&baddy->CSuper)) CSuper();
    CBaddy *retVal = baddy;
    asm volatile("" : "+r"(retVal));
    int tmpV1 = (int)&__vt_6CBaddy;
    baddy->CSuper.CBody.CItem.vtable = (void *)tmpV1;
    tmpV1 = 0x80;
    baddy->CSuper.CBody.mRMinor = (u16)tmpV1;
    tmpV1 = 0x40;
    baddy->CSuper.CBody.mPushVal = tmpV1;
    tmpV1 = NumBaddies;
    int tmpA0 = 0xFFFF;
    baddy->CSuper.CBody.mNode = (u16)tmpA0;
    tmpA0 = 0x20;
    baddy->mVerticalCompliance = (u16)tmpA0;
    tmpA0 = -1;
    baddy->mWaitTimer = 0;
    baddy->mpWeaponAttributes = 0;
    baddy->mLoopingSound = 0;
    baddy->mLoopingSoundUpdate = tmpA0;
    NumBaddies = tmpV1 + 1;
    return retVal;
}

/**
 * Destroy CBaddy-owned sound state.
 *
 * Address:   0x80040880
 * Mangled:   _._6CBaddy
 * Demangled: CBaddy::~CBaddy()
 *
 * The compiler supplies the original hidden in-charge flag and emits the
 * target `_._6CBaddy` ABI entry.
 */
CBaddy::~CBaddy()
{
    int cnt = NumBaddies;
    CSuper.CBody.CItem.vtable = (void *)&__vt_6CBaddy;
    int scriptData = mLoopingSound;
    NumBaddies = cnt - 1;
    if (scriptData != 0)
    {
        SFX_Stop(scriptData);
    }
    Baddy_CSuperDestructorAbi(&CSuper, 0);
}

/**
 * Die__6CBaddy
 *
 * Address:   0x800408F4
 * Mangled:   Die__6CBaddy
 * Demangled: CBaddy::Die(void)
 *
 */
void CBaddy::Die()
{
    if (reinterpret_cast<const CBody *>(&CSuper.CBody)->IsDead() == 0)
    {
        SendDeathPulse();
        u16 flags52 = CSuper.CBody.mCBodyFlags;
        u16 flags00 = CSuper.CBody.CItem.mFlags;
        flags52 |= 0x40;
        flags00 |= 0x41;
        CSuper.CBody.mCBodyFlags = flags52;
        CSuper.CBody.CItem.mFlags = flags00;
    }
}

/**
 * Send the death pulse once for a valid linked trigger node.
 *
 * Address:   0x80040940
 * Mangled:   SendDeathPulse__6CBaddy
 * Demangled: CBaddy::SendDeathPulse(void)
 *
 */
void CBaddy::SendDeathPulse()
{
    ushort node;

    if ((mDeathPulseSent == '\0') &&
        (CSuper.CBody.mNode != (u16)0xffff)) {
        node = CSuper.CBody.mNode;
        mDeathPulseSent = '\x01';
        Trig_SendPulse((ushort *)(OffsetList[node] + 3));
    }
}

/**
 * SkipToEndif__6CBaddy
 *
 * Address:   0x8004099C
 * Mangled:   SkipToEndif__6CBaddy
 * Demangled: CBaddy::SkipToEndif(void)
 *
 */
void CBaddy::SkipToEndif()
{
    int conditionals = 0;
    int endif_cmd = 0x4120;
    int if_cmd = 0x4100;
    u16 token;
    do
    {
        u16 *scriptPtr = mpScript;
        token = *scriptPtr;
        mpScript = scriptPtr + 1;
        if (token == endif_cmd)
        {
            if (conditionals == 0)
                return;
            conditionals--;
        }
        else
        {
            if ((u32)(token - 0x4112) < 5)
            {
                conditionals++;
            }
        }
    } while (token != if_cmd);
}

/**
 * ParseScript__6CBaddyPUs
 *
 * Address:   0x800409F4
 * Mangled:   ParseScript__6CBaddyPUs
 * Demangled: CBaddy::ParseScript(unsigned short *)
 *
 * @param p_script  Script command stream to execute.
 */
void CBaddy::ParseScript(u16 *p_script)
{
    mpScript = p_script;
    u16 cmd = *p_script;
    if (cmd == 0x4100)
        goto end_mark;
    do
    {
        u16 *scriptPtr = mpScript;
        u16 command = *scriptPtr;
        scriptPtr++;
        mpScript = scriptPtr;
        if (command & 0x4000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(this);
            int (*execute)(CBaddy *, unsigned short) =
                vtable->ExecuteCommand;
            int result = execute(this, command);
            if (result == 0)
                goto ret0;
            goto loop_cont;
        }
        {
            int masked = command & 0x2000;
            if (masked)
            {
                BaddyVTable *vtable = Baddy_Dispatch(this);
                void (*setVariable)(CBaddy *, unsigned short) =
                    vtable->SetVariable;
                setVariable(this, command);
            }
        }
    loop_cont:
        ;
        u16 *scriptPtr2 = mpScript;
        cmd = *scriptPtr2;
    } while (cmd != 0x4100);
end_mark:
    mDoScript = 0;
ret0:
    ;
}

/**
 * ExecuteCommand__6CBaddyUs
 *
 * Address:   0x80040AB4
 * Mangled:   ExecuteCommand__6CBaddyUs
 * Demangled: CBaddy::ExecuteCommand(unsigned short)
 *
 * @param command  Script opcode to execute.
 * @return 0 when script execution must pause; otherwise 1.
 */
int CBaddy::ExecuteCommand(unsigned short command)
{
    int dispatchCommand = command;

    // The original sparse command dispatch is a balanced comparison tree.
    // Keeping the ranges explicit avoids a large jump table for mostly unused
    // opcodes and leaves the individual command implementations below in
    // their natural source order.
    if (command == 0x4281)
    {
        goto command_4281;
    }
    if (dispatchCommand < 0x4282)
    {
        if (command == 0x4115)
        {
            goto command_4115;
        }
        if (dispatchCommand < 0x4116)
        {
            if (command == 0x4107)
            {
                goto command_4107;
            }
            if (dispatchCommand < 0x4108)
            {
                if (command == 0x4104)
                {
                    goto command_4104;
                }
                if (dispatchCommand < 0x4105)
                {
                    if (command == 0x4101)
                    {
                        goto command_4101;
                    }
                    if (command == 0x4102)
                    {
                        goto command_4102;
                    }
                    goto command_default;
                }
                if (command == 0x4105)
                {
                    goto command_4105;
                }
                if (command == 0x4106)
                {
                    goto command_4106;
                }
                goto command_default;
            }
            if (command == 0x4112)
            {
                goto command_4112;
            }
            if (dispatchCommand < 0x4113)
            {
                if (dispatchCommand < 0x4110)
                {
                    goto command_default;
                }
                goto command_4110_4111;
            }
            if (command == 0x4113)
            {
                goto command_4113;
            }
            if (command == 0x4114)
            {
                goto command_4114;
            }
            goto command_default;
        }

        if (command == 0x4203)
        {
            goto command_4203;
        }
        if (dispatchCommand < 0x4204)
        {
            if (command == 0x4200)
            {
                goto command_4200;
            }
            if (dispatchCommand < 0x4201)
            {
                if (command == 0x4116)
                {
                    goto command_4116;
                }
                goto command_default;
            }
            if (command == 0x4201)
            {
                goto command_4201;
            }
            if (command == 0x4202)
            {
                goto command_4202;
            }
            goto command_default;
        }
        if (command == 0x4226)
        {
            goto command_4226;
        }
        if (dispatchCommand < 0x4227)
        {
            if (command == 0x4204)
            {
                goto command_4204;
            }
            if (command == 0x4205)
            {
                goto command_4205;
            }
            goto command_default;
        }
        if (command == 0x4240)
        {
            goto command_4240;
        }
        if (dispatchCommand < 0x4241)
        {
            if (command == 0x4227)
            {
                goto command_4227;
            }
            goto command_default;
        }
        if (command == 0x4280)
        {
            goto command_4280;
        }
        goto command_default;
    }

    if (command == 0x429D)
    {
        goto command_429D;
    }
    if (dispatchCommand < 0x429E)
    {
        if (command == 0x4295)
        {
            goto command_default;
        }
        if (dispatchCommand < 0x4296)
        {
            if (command == 0x4292)
            {
                goto command_4292;
            }
            if (dispatchCommand < 0x4293)
            {
                if (command == 0x4290)
                {
                    goto command_4290;
                }
                if (command == 0x4291)
                {
                    goto command_4291;
                }
                goto command_default;
            }
            if (command == 0x4293)
            {
                goto command_4293;
            }
            if (command == 0x4294)
            {
                goto command_4294;
            }
            goto command_default;
        }
        if (command == 0x4298)
        {
            goto command_4298;
        }
        if (dispatchCommand < 0x4299)
        {
            if (command == 0x4296)
            {
                goto command_4296;
            }
            if (command == 0x4297)
            {
                goto command_4297;
            }
            goto command_default;
        }
        if (command == 0x429A)
        {
            goto command_429A;
        }
        if (dispatchCommand < 0x429A)
        {
            goto command_4299;
        }
        if (command == 0x429C)
        {
            goto command_429C;
        }
        goto command_default;
    }

    if (command == 0x42B0)
    {
        goto command_42B0;
    }
    if (dispatchCommand < 0x42B1)
    {
        if (command == 0x42A2)
        {
            goto command_42A2;
        }
        if (dispatchCommand < 0x42A3)
        {
            if (command == 0x429E)
            {
                goto command_429E;
            }
            if (command == 0x42A0)
            {
                goto command_42A0;
            }
            goto command_default;
        }
        if (command == 0x42A3)
        {
            goto command_default;
        }
        if (command == 0x42A6)
        {
            goto command_42A6;
        }
        goto command_default;
    }
    if (dispatchCommand < 0x42B5)
    {
        if (dispatchCommand < 0x42B3)
        {
            goto command_42B1_42B2;
        }
        goto command_42B3_42B4;
    }
    if (command == 0x42C0)
    {
        goto command_42C0;
    }
    goto command_default;

    {
    command_42A0:
    {
        unsigned short type = *mpScript++;
        if (type & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(this);
            type = vtable->Variable.GetVariable(this, type);
        }

        unsigned int flags = *mpScript++;
        int lifetime = *mpScript++;
        int gravity = *mpScript++;
        CPowerUp *p_powerup = PowerUp_Create(
            type, *(const CVector *)&CSuper.CBody.CItem.mPos,
            flags, lifetime, Baddy_ZeroVector());
        if (p_powerup)
        {
            p_powerup->SetGravity(gravity << 12, 5);
        }
        return 1;
    }

    command_429E:
        if (mpCreatedByPlayer != 0)
        {
            if ((CBodyOwner *)mpCreatedByPlayer ==
                reinterpret_cast<CBodyOwner *>(MechList))
            {
                CurrentWaterLevel = CSuper.CBody.CItem.mPos.vy;
                goto command_default;
            }
            CurrentWaterLevel2 = CSuper.CBody.CItem.mPos.vy;
            goto command_default;
        }
        CurrentWaterLevel = CSuper.CBody.CItem.mPos.vy;
        CurrentWaterLevel2 = CurrentWaterLevel;
        goto command_default;

    command_42A2:
        CurrentWaterDamage = *mpScript++;
        goto command_default;

    {
        CVector pos;

    command_429A:
    {
        unsigned short node = *mpScript++;
        if (node & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(this);
            node = vtable->Variable.GetVariable(this, node);
        }
        Trig_GetPosition(pos, node);
        goto process_position;
    }

    command_42A6:
        pos = *(const CVector *)&CSuper.CBody.CItem.mPos;

    process_position:
        switch (*mpScript++)
        {
        case 0:
        case 1:
            break;
        case 2:
            GetScriptValue();
            mpScript += 2;
            goto command_default;
        }
        return 1;
    }

    command_4227:
    {
        unsigned short interval = *mpScript++;
        if (interval)
        {
            mComplexSurfaceInterval = (unsigned char)interval;
            mComplexSurfaceTimer = 0;
            mSpecificFlags |= 2;
            goto command_default;
        }
        mSpecificFlags &= ~2U;
        goto command_default;
    }

    command_4298:
    {
        unsigned short shake = *mpScript++;
        if (shake & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(this);
            shake = vtable->Variable.GetVariable(this, shake);
        }

        if (CameraList == 0)
        {
            return 1;
        }
        if (shake == 0)
        {
            ((CCamera *)CameraList)->Shake(
                *(const CVector *)&CSuper.CBody.CItem.mPos,
                CAMERASHAKE_SMALL);
            return 1;
        }
        if (shake == 1)
        {
            ((CCamera *)CameraList)->Shake(
                *(const CVector *)&CSuper.CBody.CItem.mPos,
                CAMERASHAKE_MEDIUM);
            return 1;
        }
        ((CCamera *)CameraList)->Shake(
            *(const CVector *)&CSuper.CBody.CItem.mPos,
            CAMERASHAKE_BIG);
        return 1;
    }

    command_4296:
        SFX_MIDIFadeIn(*mpScript++);
        return 1;

    command_4297:
        SFX_MIDIFadeOut(*mpScript++);
        return 1;

    command_4299:
        printf("ignoring script cmd: setting fmv track %d\n", *mpScript++);
        return 1;

    command_4293:
    {
        unsigned int *p_checksum = (unsigned int *)
            (((unsigned int)mpScript + 3) & ~3U);
        printf("ignoring script cmd: setting fmv checksum %x\n", p_checksum);
        mpScript = (unsigned short *)(p_checksum + 1);
        goto command_default;
    }

    command_4294:
        printf("ignoring script cmd: starting fmv\n");
        return 1;

    command_429D:
        printf("ignoring script cmd: starting framebuffer fmv\n");
        return 1;

    command_42B1_42B2:
    {
        unsigned short node = *mpScript++;
        if (node & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(this);
            node = vtable->Variable.GetVariable(this, node);
        }
        unsigned short *p_links = Trig_GetLinksPointer(node);
        if (command == 0x42B1)
        {
            SendSignalToLinks(p_links);
        }
        else
        {
            Trig_SendPulse(p_links);
        }
        return 1;
    }

    command_42B3_42B4:
    {
        unsigned short node = *mpScript++;
        if (node & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(this);
            node = vtable->Variable.GetVariable(this, node);
        }
        if (command == 0x42B3)
        {
            SendSignalToNode(BaddyList, node);
            SendSignalToNode(EnvironmentalObjectList, node);
        }
        else
        {
            Trig_SendPulseToNode(node);
        }
        return 1;
    }

    command_42C0:
    {
        unsigned short goal = *mpScript++;
        if ((GGame == 1) && !Career_Got(1) &&
            ((GVInfo1.VideoMode ^ VIDEO_PLAYBACK) != 0))
        {
            Panel_GoalCounter(goal);
        }
        return 1;
    }

    command_42B0:
    {
        char *p_string = (char *)mpScript;
        while (*p_string)
        {
            ++p_string;
        }
        if ((unsigned int)p_string & 1)
        {
            mpScript = (unsigned short *)(p_string + 1);
        }
        else
        {
            mpScript = (unsigned short *)(p_string + 2);
        }
        return 1;
    }

    command_4226:
        Baddy_ClearVector(
            reinterpret_cast<CVector &>(CSuper.CBody.mVel));
        goto command_default;

    command_4200:
    {
        reinterpret_cast<CItem *>(&CSuper.CBody.CItem)->InitItem(
            (char *)mpScript);
        char *p_string = (char *)mpScript;
        while (*p_string)
        {
            ++p_string;
        }
        if ((unsigned int)p_string & 1)
        {
            mpScript = (unsigned short *)(p_string + 1);
        }
        else
        {
            mpScript = (unsigned short *)(p_string + 2);
        }
        return 1;
    }

    command_4106:
    {
        CVector dummy;
        unsigned short node = *mpScript++;
        if (node & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(this);
            node = vtable->Variable.GetVariable(this, node);
        }
        unsigned short *pNode = Trig_GetPosition(dummy, node);
        mpScript = pNode + 3;
        return 1;
    }

    command_4104:
    {
        unsigned short label = *mpScript++;
        mpScriptLabel[label] = mpScript;
        goto command_default;
    }

    command_4105:
    {
        unsigned short *p_return = mpScript;
        while (*mpScript != 0x4100)
        {
            unsigned short script_command = *mpScript++;
            if (script_command == 0x4104)
            {
                unsigned short label = *mpScript++;
                mpScriptLabel[label] = mpScript;
            }
        }
        mpScript = p_return;
        goto command_default;
    }

    command_4101:
    {
        unsigned short label = *mpScript++;
        mpScript = mpScriptLabel[label];
        goto command_default;
    }

    command_4102:
    {
        unsigned short label = *mpScript++;
        mpScript = mpScriptLabel[label];
        return 0;
    }

    command_4116:
    {
        unsigned int flags = *mpScript++;
        if ((mSpecificFlags & flags) == 0)
        {
            goto command_default;
        }
        goto skip_to_endif;
    }

    command_4115:
    {
        unsigned int flags = *mpScript++;
        if ((mSpecificFlags & flags) == flags)
        {
            goto command_default;
        }
        goto skip_to_endif;
    }

    command_4110_4111:
    {
        unsigned short dummy_script[8];
        int dummy_script_index = 0;
        int var_id = *mpScript++;
        unsigned short *p_start = mpScript;
        int v0 = Baddy_Dispatch(this)->Variable.GetVariable(this, var_id);
        if (p_start != mpScript)
        {
            do
            {
                dummy_script[dummy_script_index] = *p_start++;
                ++dummy_script_index;
            }
            while (p_start != mpScript);
        }

        int v1 = *mpScript++;
        if (v1 & 0x2000)
        {
            v1 = Baddy_Dispatch(this)->Variable.GetVariable(this, v1);
        }
        dummy_script[dummy_script_index] =
            (command == 0x4110) ? v0 + v1 : v0 - v1;

        p_start = mpScript;
        mpScript = dummy_script;
        BaddyVTable *vtable = Baddy_Dispatch(this);
        vtable->SetVariable(this, var_id);
        mpScript = p_start;
        goto command_default;
    }

    command_4112:
    {
        int v0 = *mpScript++;
        if (v0 & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(this);
            v0 = vtable->Variable.GetVariable(this, v0);
        }

        int v1 = *mpScript++;
        if (v1 & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(this);
            v1 = vtable->Variable.GetVariable(this, v1);
        }

        if (v1 < v0)
        {
            goto command_default;
        }
        goto skip_to_endif;
    }

    command_4113:
    {
        int v0 = *mpScript++;
        if (v0 & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(this);
            v0 = vtable->Variable.GetVariable(this, v0);
        }

        int v1 = *mpScript++;
        if (v1 & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(this);
            v1 = vtable->Variable.GetVariable(this, v1);
        }

        if (v0 < v1)
        {
            goto command_default;
        }
        goto skip_to_endif;
    }

    command_4114:
    {
        int v0 = *mpScript++;
        if (v0 & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(this);
            v0 = vtable->Variable.GetVariable(this, v0);
        }

        int v1 = *mpScript++;
        if (v1 & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(this);
            v1 = vtable->Variable.GetVariable(this, v1);
        }

        if (v0 == v1)
        {
            goto command_default;
        }
    }

    skip_to_endif:
        SkipToEndif();
        return 1;

    command_4280:
    {
        mWaitTimer = *mpScript++;
        if (mWaitTimer & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(this);
            mWaitTimer =
                vtable->Variable.GetVariable(this, mWaitTimer);
        }
        return 0;
    }

    command_4281:
        if (mInputFlagsCopy & 1)
        {
            return 1;
        }
        --mpScript;
        return 0;

    command_4240:
        mDoScript = 0;
        return 0;

    command_4107:
        return 0;

    command_4201:
    {
        int anim = *mpScript++;
        int direction = *mpScript++;
        reinterpret_cast< ::CSuper *>(&CSuper)->CycleAnim(
            anim, direction);
        return 1;
    }

    command_4202:
        reinterpret_cast< ::CSuper *>(&CSuper)->RunAnim(
            *mpScript++, 0, -1, -1);
        return 1;

    command_4203:
        CSuper.CBody.CItem.mFlags &= ~1;
        goto command_default;

    command_4204:
        CSuper.CBody.CItem.mFlags |= 0x41;
        goto command_default;

    command_4205:
    {
        BaddyVTable *vtable = Baddy_Dispatch(this);
        vtable->Die(this);
        return 1;
    }

    command_4290:
        SFX_PlayX((short)*mpScript++, 0x2000, 0);
        return 1;

    command_4291:
        SFX_PlayPos(
            (short)*mpScript++,
            *(const CVector *)&CSuper.CBody.CItem.mPos, 0);
        return 1;

    command_4292:
    {
        unsigned short sparks = *mpScript++;
        if (BitCount >= 200)
        {
            return 1;
        }

        Bit_SetSparkTrajectory(
            *(const CSVector *)&CSuper.CBody.CItem.mAngles);
        BaddyCSVector cone;
        cone.Set(0x200, 0x1000, 0);
        Bit_SetSparkTrajectoryCone(*(const CSVector *)&cone);
        Bit_SetSparkSize(1);
        Bit_SetSparkRGB(0x80, 0x80, 0x80);
        Bit_SetSparkFadeRGB(4, 4, 4);

        UseCacheForBits = 0;
        for (int i = 0; i < sparks; ++i)
        {
            if (Mem_GetFree(1) > 10000)
            {
                CSpark *spark = static_cast<CSpark *>(
                    CBit::operator new(BIT_SPARK_STORAGE_SIZE));
                if (spark)
                {
                    ::new (spark) CSpark(
                        *(CVector *)&CSuper.CBody.CItem.mPos,
                        0x20, 0x2000, 0x20);
                }
            }
        }
        UseCacheForBits = 1;
        goto command_default;
    }

    command_429C:
        Flash_Screen(
            (unsigned char)mpScript[0],
            (unsigned char)mpScript[1],
            (unsigned char)mpScript[2],
            mpScript[3], 0, mpScript[4]);
        mpScript += 5;
        goto command_default;

    command_default:
        return 1;
    }
}

/**
 * Consume the next script word, resolving virtual-variable references.
 *
 * @return Script literal or virtual variable value.
 */
unsigned short CBaddy::GetScriptValue()
{
    u16 *scriptPtr = mpScript;
    u16 value = *scriptPtr;
    mpScript = scriptPtr + 1;
    if ((value & 0x8000) == 0)
    {
        if (value & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(this);
            value = vtable->Variable.GetVariable(this, value);
        }
    }
    return value;
}

/**
 * Virtual dispatch table emitted by the script-only baddy owner.
 *
 * The original GCC 2.x translation unit places both baddy vtables between
 * ExecuteCommand's diagnostic strings and the SetVariable switch table.
 */
BaddyScriptOnlyVTableDefinition const __vt_16CScriptOnlyBaddy = {
    0,
    0,
    Baddy_ScriptOnlyDestructorAbi,
    (void (*)(CBaddy *))&CBaddy::Die,
    (void (*)(CScriptOnlyBaddy *))&CScriptOnlyBaddy::AI,
    (void (*)(CBodyOwner *))&CBody::EveryFrame,
    (void (*)(CBodyOwner *))&CBody::DoLightingSetup,
    (int (*)(CBodyOwner *, int, const CVector &, int))&CBody::Hit,
    (void (*)(CBodyOwner *))&CBody::DeleteStuff,
    (int (*)(CScriptOnlyBaddy *, unsigned short))
        &CScriptOnlyBaddy::ExecuteCommand,
    (void (*)(CScriptOnlyBaddy *, unsigned short))
        &CScriptOnlyBaddy::SetVariable,
    (short (*)(CBaddy *, unsigned short))&CBaddy::GetVariable,
};

/**
 * Virtual dispatch table emitted by the base baddy owner.
 */
BaddyBaseVTableDefinition const __vt_6CBaddy = {
    0,
    0,
    Baddy_CBaddyDestructorAbi,
    (void (*)(CBaddy *))&CBaddy::Die,
    (void (*)(CBodyOwner *))&CBody::AI,
    (void (*)(CBodyOwner *))&CBody::EveryFrame,
    (void (*)(CBodyOwner *))&CBody::DoLightingSetup,
    (int (*)(CBodyOwner *, int, const CVector &, int))&CBody::Hit,
    (void (*)(CBodyOwner *))&CBody::DeleteStuff,
    (int (*)(CBaddy *, unsigned short))&CBaddy::ExecuteCommand,
    (void (*)(CBaddy *, unsigned short))&CBaddy::SetVariable,
    (short (*)(CBaddy *, unsigned short))&CBaddy::GetVariable,
};

/**
 * SetVariable__6CBaddyUs (extern "C")
 *
 * Address:   0x800419A4
 * Mangled:   SetVariable__6CBaddyUs
 * Demangled: CBaddy::SetVariable(unsigned short)
 *
 * @param variable  Script variable selector.
 */
void CBaddy::SetVariable(unsigned short variable)
{
    switch (variable)
    {
    case 0x2135:
    {
        unsigned int *p_checksum =
            (unsigned int *)(((int)mpScript + 3) & ~3);
        unsigned int val = *p_checksum;
        mpScript = (u16 *)(p_checksum + 1);
        mDeadChecksum = val;
        break;
    }
    case 0x212f:
    {
        unsigned int *p_checksum =
            (unsigned int *)(((int)mpScript + 3) & ~3);
        u16 model = Spool_GetModel(
            *p_checksum, CSuper.CBody.CItem.mRegion);
        CSuper.CBody.CItem.mModel = model;
        mpScript = (u16 *)(p_checksum + 1);
        break;
    }
    case 0x2140:
    {
        int val = GetScriptValue();
        CSuper.CBody.CItem.mPos.vx = (val << 16) >> 4;
        break;
    }
    case 0x2141:
    {
        int val = GetScriptValue();
        CSuper.CBody.CItem.mPos.vy = (val << 16) >> 4;
        break;
    }
    case 0x2142:
    {
        int val = GetScriptValue();
        CSuper.CBody.CItem.mPos.vz = (val << 16) >> 4;
        break;
    }
    case 0x2131:
    {
        u16 *scriptPtr = mpScript;
        u16 val = *scriptPtr;
        mpScript = scriptPtr + 1;
        if (val != 0)
        {
            CSuper.CBody.CItem.mFlags =
                CSuper.CBody.CItem.mFlags | 8;
        }
        else
        {
            CSuper.CBody.CItem.mFlags =
                CSuper.CBody.CItem.mFlags & 0xFFF7;
        }
        break;
    }
    case 0x212e:
    {
        u16 *scriptPtr = mpScript;
        u16 val = *scriptPtr;
        mpScript = scriptPtr + 1;
        mVerticalCompliance = val;
        break;
    }
    case 0x212c:
    {
        u16 *scriptPtr = mpScript;
        u8 val = *(u8 *)scriptPtr;
        mpScript = scriptPtr + 1;
        mInputSignal = val;
        break;
    }
    case 0x212d:
    {
        u16 *scriptPtr = mpScript;
        u8 val = *(u8 *)scriptPtr;
        mpScript = scriptPtr + 1;
        mHit = val;
        break;
    }
    case 0x2100:
    {
        u16 *scriptPtr = mpScript;
        u16 val = *scriptPtr;
        mpScript = scriptPtr + 1;
        CSuper.CBody.mHealth = val;
        break;
    }
    case 0x2101:
    {
        u16 *scriptPtr = mpScript;
        u16 val = *scriptPtr;
        mpScript = scriptPtr + 1;
        CSuper.CBody.mRMinor = val;
        break;
    }
    case 0x2125:
    {
        u16 *p = mpScript;
        u16 idx = *p;
        p++;
        mpScript = p;
        u16 val2 = *p;
        p++;
        mBaddyAttributes[idx] = val2;
        mpScript = p;
        break;
    }
    case 0x2114:
    {
        u16 *scriptPtr = mpScript;
        u16 val = *scriptPtr;
        mpScript = scriptPtr + 1;
        mPattern = val;
        break;
    }
    case 0x2115:
    {
        u16 *scriptPtr = mpScript;
        u16 val = *scriptPtr;
        mpScript = scriptPtr + 1;
        mSubPattern = val;
        break;
    }
    case 0x2120:
    {
        u16 *scriptPtr = mpScript;
        u16 idx = *scriptPtr;
        mpScript = scriptPtr + 1;
        u16 val = GetScriptValue();
        mRegister[idx & 0xFF] = val;
        break;
    }
    case 0x2121:
    {
        u16 *scriptPtr = mpScript;
        short val = *(short *)scriptPtr;
        mpScript = scriptPtr + 1;
        CSuper.mAnimSpeed = (int)val;
        break;
    }
    case 0x2122:
    {
        u16 *scriptPtr = mpScript;
        u16 val = *scriptPtr;
        mpScript = scriptPtr + 1;
        mGroundToOrigin = (s16)val;
        break;
    }
    }
}

/**
 * Read a script-visible baddy variable.
 *
 * Address:   0x80041C28
 * Mangled:   GetVariable__6CBaddyUs
 * Demangled: CBaddy::GetVariable(unsigned short)
 *
 * @param variable Script variable selector.
 * @return The selected signed 16-bit value, or zero when unavailable.
 */
short CBaddy::GetVariable(unsigned short variable)
{
    short value;

    switch (variable)
    {
    case 0x2136:
        value = *(short *)GameFMV_CurrentFrame;
        break;

    case 0x2132:
        if (reinterpret_cast<CBodyOwner *>(MechList) != 0)
        {
            unsigned int dist = Utils_XZDist(
                *reinterpret_cast<const CVector *>(
                    &this->CSuper.CBody.CItem.mPos),
                *reinterpret_cast<const CVector *>(
                    &reinterpret_cast<CBodyOwner *>(MechList)->CItem.mPos));
            if (dist >= 0x2000)
            {
                dist = 0x1FFF;
            }
            value = (unsigned short)dist;
            break;
        }
        return 0x1FFF;

    case 0x2133:
        if (reinterpret_cast<CBodyOwner *>(MechList) != 0)
        {
            CVector eye_pos = reinterpret_cast<const CVector &>(
                this->CSuper.CBody.CItem.mPos);
            eye_pos.vy -= this->mOriginToEye << 12;
            return (short)Utils_LineOfSight(
                eye_pos,
                *reinterpret_cast<const CVector *>(
                    &reinterpret_cast<CBodyOwner *>(MechList)->CItem.mPos));
        }
        return 0;

    case 0x212E:
        value = this->mVerticalCompliance;
        break;

    case 0x212C:
        value = this->mInputSignal;
        break;

    case 0x212D:
        value = this->mHit;
        break;

    case 0x2140:
        value = (short)(this->CSuper.CBody.CItem.mPos.vx >> 12);
        break;

    case 0x2141:
        value = (short)(this->CSuper.CBody.CItem.mPos.vy >> 12);
        break;

    case 0x2142:
        value = (short)(this->CSuper.CBody.CItem.mPos.vz >> 12);
        break;

    case 0x2150:
        if (reinterpret_cast<CBodyOwner *>(MechList) != 0)
        {
            return (short)(reinterpret_cast<CBodyOwner *>(MechList)->CItem.mPos.vx >> 12);
        }
        return 0;

    case 0x2151:
        if (reinterpret_cast<CBodyOwner *>(MechList) != 0)
        {
            return (short)(reinterpret_cast<CBodyOwner *>(MechList)->CItem.mPos.vy >> 12);
        }
        return 0;

    case 0x2152:
        if (reinterpret_cast<CBodyOwner *>(MechList) != 0)
        {
            return (short)(reinterpret_cast<CBodyOwner *>(MechList)->CItem.mPos.vz >> 12);
        }
        return 0;

    case 0x2100:
        value = this->CSuper.CBody.mHealth;
        break;

    case 0x2129:
    {
        unsigned short *script = this->mpScript;
        unsigned short limit = *script;
        this->mpScript = script + 1;
        value = (short)Rnd(limit);
        break;
    }

    case 0x2120:
    {
        unsigned short *script = this->mpScript;
        unsigned char index = *(unsigned char *)script;
        this->mpScript = script + 1;
        value = this->mRegister[index];
        break;
    }

    case 0x212A:
        value = this->CSuper.CBody.mNode;
        break;

    case 0x212B:
    {
        unsigned short *script = this->mpScript;
        unsigned short node = *script;
        this->mpScript = script + 1;
        if (node & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(this);
            int (*method)(CBaddy *, unsigned short) =
                vtable->Variable.GetVariableInt;
            node = method(this, node) & 0xFFFF;
        }

        unsigned short *p_links = Trig_GetLinksPointer(node);
        if (p_links[0] != 0)
        {
            return (short)p_links[1];
        }
        return 0;
    }

    default:
        return 0;
    }

    return value;
}

/**
 * __16CScriptOnlyBaddyPsi
 *
 * Address:   0x80041E80
 * Mangled:   __16CScriptOnlyBaddyPsi
 * Demangled: CScriptOnlyBaddy::(short *, int)
 *            Initialize a script-only baddy from trigger data and attach it to the baddy list.
 *
 * @param p_info  Packed position, angle, and script data.
 * @param node_index  Trigger node associated with the baddy.
 */
CScriptOnlyBaddy::CScriptOnlyBaddy(short *p_info, int node_index)
{
    void *vtable = (void *)&__vt_16CScriptOnlyBaddy;
    CBruce *createdByPlayer = pCurrentPlayer;
    CSuper.CBody.CItem.vtable = vtable;
    mpCreatedByPlayer = createdByPlayer;
    CBody *body = reinterpret_cast<CBody *>(&CSuper.CBody);
    short *pos = body->SquirtPos(p_info);
    short *angles = body->SquirtAngles(pos);
    mpScript = (u16 *)angles;
    unsigned short flags = CSuper.CBody.CItem.mFlags;
    mLoopingSoundUpdate = -1;
    unsigned short field52 = CSuper.CBody.mCBodyFlags;
    mDoScript = 1;
    CSuper.CBody.mNode = (unsigned short)node_index;
    flags |= 0x41;
    field52 &= 0xFFEF;
    CSuper.CBody.CItem.mFlags = flags;
    CSuper.CBody.mCBodyFlags = field52;
    body->AttachTo(reinterpret_cast<CBody **>(&BaddyList));
    CSuper.CBody.mRMinor = 0;
    CSuper.CBody.CItem.mType = 0xCB;
}

/**
 * Destroy script-only baddy effects and weather state.
 *
 * Address:   0x80041F38
 * Mangled:   _._16CScriptOnlyBaddy
 * Demangled: CScriptOnlyBaddy::~CScriptOnlyBaddy()
 *
 * The compiler supplies the original hidden in-charge flag and emits the
 * target `_._16CScriptOnlyBaddy` ABI entry.
 */
CScriptOnlyBaddy::~CScriptOnlyBaddy()
{
    CSuper.CBody.CItem.vtable = (void *)&__vt_16CScriptOnlyBaddy;
    reinterpret_cast<CBody *>(&CSuper.CBody)->DeleteFrom(
        reinterpret_cast<CBody **>(&BaddyList));

    if (mLoopingSound != 0)
    {
        SFX_Stop(mLoopingSound);
    }

    {
        CSmokeJet *obj = mpSmokeJet;
        if (obj != 0)
        {
            BaddyEffectVTable *vtbl = obj->_vf;
            void (*dtor)(void *, int) = vtbl->Destroy;
            dtor(obj, 3);
        }
    }

    {
        SRainEntry *arr = mpWeather;
        if (arr != 0)
        {
            for (int i = 0; i < (int)mWeatherData; i++)
            {
                int idx = i;
                SRainEntry *base = mpWeather;
                SRainEntry *p_entry = &base[idx];
                CGPolyLine *obj = p_entry->p_line;
                if (obj != 0)
                {
                    BaddyEffectVTable *vtbl = obj->_vf;
                    void (*dtor)(void *, int) = vtbl->Destroy;
                    dtor(obj, 3);
                }
            }
            Mem_Delete(mpWeather);
        }
    }

}

/**
 * Advance script state, looping audio, and weather streaks.
 *
 * Address:   0x80042054
 * Mangled:   AI__16CScriptOnlyBaddy
 * Demangled: CScriptOnlyBaddy::AI(void)
 *
 */
void CScriptOnlyBaddy::AI()
{
    unsigned short inputFlags = CSuper.CBody.mInputFlags;
    CSuper.CBody.CItem.mFlags |= 0x41;
    mInputFlagsCopy = inputFlags;

    if (inputFlags & 1)
    {
        CSuper.CBody.mInputFlags = inputFlags & 0xFFFE;
        mInputSignal++;
    }

    if (mDoScript != 0)
    {
        if (mWaitTimer != 0)
        {
            mWaitTimer--;
        }
        else
        {
            ParseScript(mpScript);
        }
    }
    else
    {
        BaddyVTable *vtable = Baddy_Dispatch(this);
        void (*quickAI)(CBaddy *) = vtable->Die;
        quickAI(this);
    }

    if (mLoopingSoundUpdate != -1 &&
        TTime % mLoopingSoundUpdate == 0)
    {
        int t = SFXLEVEL;
        unsigned int sound = mLoopingSound;
        SFXLEVEL = t * mLoopingSoundVol / 100;
        SFX_ModifyPos(
            sound,
            *(const CVector *)&CSuper.CBody.CItem.mPos,
            0);
        SFXLEVEL = t;
    }

    if (mpWeather != 0)
    {
        int ground = reinterpret_cast<CBodyOwner *>(MechList)->mShadowPos.vy;
        int i = 0;

        if (mWeatherData != 0)
        {
            CVector pos;

            do
            {
                SRainEntry *p_entry = &mpWeather[i];

                {
                    CGPolyLine *line = p_entry->p_line;
                    pos = line->mStart;
                }
                pos += p_entry->vel;

                if (ground < pos.vy)
                {
                    CVector offset;
                    int randomX = (Rnd(0x960) - 0x4B0) << 12;
                    int randomY = (Rnd(0x190) - 0x4B0) << 12;
                    int randomZ = (Rnd(0x960) - 0x4B0) << 12;
                    offset.vx = randomX;
                    offset.vy = randomY;
                    offset.vz = randomZ;
                    pos = reinterpret_cast<const CVector &>(
                              reinterpret_cast<CBodyOwner *>(MechList)
                                  ->CItem.mPos) +
                          offset;
                }

                p_entry->p_line->mStart = pos;
                SLineSeg *segment = p_entry->p_line->mSegs;
                CVector offset =
                    (pos - p_entry->vel) +
                    (reinterpret_cast<const CVector &>(
                         reinterpret_cast<CBodyOwner *>(CameraList)->mVel) >>
                     1);
                segment->End = offset;

                i++;
            } while (i < mWeatherData);
        }
    }
}

/**
 * SetVariable__16CScriptOnlyBaddyUs
 *
 * Address:   0x80042344
 * Mangled:   SetVariable__16CScriptOnlyBaddyUs
 * Demangled: CScriptOnlyBaddy::SetVariable(unsigned short)
 *
 * @param variable  Script variable selector.
 */
void CScriptOnlyBaddy::SetVariable(unsigned short variable)
{
    CBaddy::SetVariable(variable);
}

/**
 * Execute one of the commands owned by a script-only environmental actor.
 *
 * Address:   0x80042364
 * Mangled:   ExecuteCommand__16CScriptOnlyBaddyUs
 * Demangled: CScriptOnlyBaddy::ExecuteCommand(unsigned short)
 *
 * @param command  Script opcode to execute.
 * @return One when the command was handled, otherwise the base handler's result.
 */
int CScriptOnlyBaddy::ExecuteCommand(unsigned short command)
{
    CVector origin;
    int i;
    SRainEntry *p_entry;
    CVector offset;
    CGPolyLine *p_droplet;

    switch (command)
    {
    case 0x4507:
    {
        if (mLoopingSound != 0)
        {
            SFX_Stop(mLoopingSound);
        }

        unsigned short *script = mpScript;
        unsigned short sound = *script;
        mpScript = script + 1;
        unsigned int handle = SFX_PlayX(sound, 0x2000, 0);
        int volume = SFX_v;
        mLoopingSound = handle;
        mLoopingSoundUpdate = -1;
        mLoopingSoundVol = volume;
        break;
    }

    case 0x4508:
    {
        if (mLoopingSound != 0)
        {
            SFX_Stop(mLoopingSound);
        }

        unsigned short sound = *mpScript++;
        mLoopingSoundUpdate = *mpScript++;
        unsigned int handle = SFX_PlayPos(
            sound,
            *(const CVector *)&CSuper.CBody.CItem.mPos,
            0);
        int volume = SFX_v;
        mLoopingSound = handle;
        mLoopingSoundVol = volume;
        break;
    }

    case 0x4509:
        if (mLoopingSound != 0)
        {
            SFX_Stop(mLoopingSound);
        }
        mLoopingSound = 0;
        break;

    case 0x4506:
        if (reinterpret_cast<CBodyOwner *>(MechList) != 0)
        {
            CSuper.CBody.CItem.mPos =
                reinterpret_cast<CBodyOwner *>(MechList)->CItem.mPos;
        }
        break;

    case 0x4700:
    {
        unsigned short *script = mpScript;
        unsigned short enabled = *script;
        mpScript = script + 1;
        if (enabled != 0)
        {
            mSpecificFlags |= 0x100;
        }
        else
        {
            mSpecificFlags &= ~0x100;
        }
        break;
    }

    case 0x4504:
        mpScript += 4;
        // Fall through: this variant skips four leading script parameters.
    case 0x429B:
    {
        CSmokeJet *smoke = static_cast<CSmokeJet *>(
            CBit::operator new(sizeof(CSmokeJet)));
        if (smoke != 0)
        {
            unsigned short *script = mpScript;
            smoke = ::new (smoke) CSmokeJet(
                reinterpret_cast<const CVector &>(
                    CSuper.CBody.CItem.mPos),
                reinterpret_cast<const CSVector &>(
                    CSuper.CBody.CItem.mAngles),
                script[0],
                0xFFFF,
                script[1],
                script[2],
                script[3],
                script[4],
                *((unsigned char *)script + 10),
                0xCCE0187C);
        }

        mpSmokeJet = smoke;
        smoke->mProtected = 1;
        mpScript += 6;
        break;
    }

    case 0x42A1:
    {
        CSmokeJet *smoke = static_cast<CSmokeJet *>(
            CBit::operator new(sizeof(CSmokeJet)));
        if (smoke != 0)
        {
            unsigned short *script = mpScript;
            smoke = ::new (smoke) CSmokeJet(
                reinterpret_cast<const CVector &>(
                    CSuper.CBody.CItem.mPos),
                reinterpret_cast<const CSVector &>(
                    CSuper.CBody.CItem.mAngles),
                script[0],
                0xFFFF,
                script[1],
                script[2],
                script[3],
                script[4],
                *((unsigned char *)script + 10),
                0x68B7B136);
        }

        mpSmokeJet = smoke;
        mpScript += 6;
        break;
    }

    case 0x429F:
    {
        CSmokeJet *smoke = mpSmokeJet;
        if (smoke != 0)
        {
            void (*destroy)(void *, int) = smoke->_vf->Destroy;
            destroy(smoke, 3);
        }
        mpSmokeJet = 0;
        break;
    }

    case 0x4503:
    {
        mWeatherData = GetScriptValue();
        SRainEntry *weather = (SRainEntry *)Mem_New(
            (unsigned int)mWeatherData << 4, 0, 1);
        CBodyOwner *mech = reinterpret_cast<CBodyOwner *>(MechList);
        unsigned int weatherCount = mWeatherData;
        mpWeather = weather;
        UseCacheForBits = 0;

        origin = reinterpret_cast<const CVector &>(mech->CItem.mPos);
        i = 0;
        if (weatherCount != 0)
        {
            do
            {
                p_entry = &mpWeather[i];

                int offsetX = (Rnd(0x960) - 0x4B0) << 12;
                int offsetY = (Rnd(0x190) - 0x4B0) << 12;
                int offsetZ = (Rnd(0x960) - 0x4B0) << 12;
                offset.vx = offsetX;
                offset.vy = offsetY;
                offset.vz = offsetZ;
                offset += origin;

                int velocityX = (Rnd(0x10) - 8) << 12;
                int velocityY = (Rnd(0x30) + 0x30) << 12;
                int velocityZ = (Rnd(0x10) - 8) << 12;
                p_entry->vel.vx = velocityX;
                p_entry->vel.vy = velocityY;
                p_entry->vel.vz = velocityZ;

                p_droplet = static_cast<CGPolyLine *>(
                    CBit::operator new(sizeof(CGPolyLine)));
                if (p_droplet != 0)
                {
                    p_droplet = ::new (p_droplet) CGPolyLine(1);
                }

                p_entry->p_line = p_droplet;
                p_droplet->mProtected = 1;
                p_droplet->mStart = offset;

                SLineSeg *segment = p_droplet->mSegs;
                segment->End =
                    (offset - p_entry->vel) +
                    (reinterpret_cast<const CVector &>(
                         reinterpret_cast<CBodyOwner *>(CameraList)->mVel) >>
                     1);

                p_droplet->mStartR = 0x40;
                p_droplet->mStartG = 0x40;
                p_droplet->mStartB = 0x50;
                p_droplet->mSegs->r = 0x10;
                p_droplet->mSegs->g = 0x10;
                p_droplet->mSegs->b = 0x20;
                p_droplet->SetSemiTransparent();

                i++;
            } while (i < mWeatherData);
        }

        UseCacheForBits = 1;
        break;
    }

    default:
        return CBaddy::ExecuteCommand(command);
    }

    return 1;
}
