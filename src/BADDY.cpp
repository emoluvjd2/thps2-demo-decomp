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
#include "CBODY.h"
#include "CSUPER.h"
#include "CPOWERUP.h"
#include "SPOOL.h"
#include "TRIG.h"
#include "PANEL.h"

struct CGPolyLine;
struct CSpark;

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
extern "C" CSuperOwner *Baddy_CSuperDefaultCtor(CSuperOwner *super)
    asm("__6CSuper");
extern "C" void Baddy_CSuperDestructorAbi(CSuperOwner *super, int inCharge)
    asm("_._6CSuper");
extern "C" void Baddy_InitItem(CItemOwner *item, char *name)
    asm("InitItem__5CItemPc");
extern "C" void Baddy_CBodyAI(CBodyOwner *body) asm("AI__5CBody");
extern "C" void Baddy_CBodyDoLightingSetup(CBodyOwner *body)
    asm("DoLightingSetup__5CBody");
extern "C" int Baddy_CBodyHit(
    CBodyOwner *body, int damage, const CVector &knock, int whatHit)
    asm("Hit__5CBodyiRC7CVectori");
extern "C" void Baddy_CBodyDeleteStuff(CBodyOwner *body)
    asm("DeleteStuff__5CBody");

/** Lowered aggregate-return ABI for CVector addition. */
extern "C" void Baddy_VectorAddAbi(
    CVectorOwner *result, const CVectorOwner *left,
    const CVectorOwner *right) asm("__pl__FRC7CVectorT0");
/** Lowered aggregate-return ABI for CVector subtraction. */
extern "C" void Baddy_VectorSubtractAbi(
    CVectorOwner *result, const CVectorOwner *left,
    const CVectorOwner *right) asm("__mi__FRC7CVectorT0");
/** Lowered aggregate-return ABI for CVector's arithmetic right shift. */
extern "C" void Baddy_VectorShiftRightAbi(
    CVectorOwner *result, const CVectorOwner *vector,
    const int *shift) asm("__rs__FRC7CVectorRCi");

extern "C" CGPolyLine *Baddy_ConstructWeatherLine(
    CGPolyLine *line, int segments) asm("__10CGPolyLinei");
extern "C" CSpark *Baddy_ConstructSpark(
    CSpark *spark, CVector &position, int velocity, int gravity, int life)
    asm("__6CSparkR7CVectoriii");

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
static inline void Baddy_ClearVector(CVectorOwner *vector)
{
    vector->vz = 0;
    vector->vy = 0;
    vector->vx = 0;
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
extern "C" BaddyScriptOnlyVTableDefinition const __vt_16CScriptOnlyBaddy;
extern "C" BaddyBaseVTableDefinition const __vt_6CBaddy;
extern "C" {
int NumBaddies = 0;
int NumGenericBaddies = 0;
int NumHoverSWATS = 0;
int NumZombies = 0;
}
// Scalar storage view of the first word; the helper below restores the full
// CVector type carried by the linker-visible ZeroVector object.
extern "C" int BaddyZeroVectorFirstComponent asm("ZeroVector")
    __attribute__((section(".data")));

/** Return the full vector stored at the linker-visible `ZeroVector` base. */
static inline CVector &Baddy_ZeroVector()
{
    return *(CVector *)&BaddyZeroVectorFirstComponent;
}
#include "MECHLIST.h"
#define BODYLISTS_DECLARE_ENVIRONMENTAL_OBJECT_LIST
#include "BODYLISTS.h"
extern "C" int SFX_v __attribute__((section(".data")));

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

    /** Release transient-bit storage through its owning allocator. */
    static void operator delete(void *p);
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
extern "C" CBaddy *CBaddy_construct(CBaddy *) asm("__6CBaddy");
extern "C" void Baddy_CBaddyDestructorAbi(CBaddy *, int)
    asm("_._6CBaddy");
extern "C" void CBaddy_Die(CBaddy *) asm("Die__6CBaddy");
extern "C" int CBaddy_ExecuteCommand(CBaddy *, unsigned short)
    asm("ExecuteCommand__6CBaddyUs");
extern "C" void CBaddy_SetVariable(CBaddy *, unsigned short)
    asm("SetVariable__6CBaddyUs");
extern "C" short CBaddy_GetVariable(CBaddy *, unsigned short)
    asm("GetVariable__6CBaddyUs");
extern "C" void Baddy_ScriptOnlyDestructorAbi(CScriptOnlyBaddy *, int)
    asm("_._16CScriptOnlyBaddy");
extern "C" void CScriptOnlyBaddy_AI(CScriptOnlyBaddy *)
    asm("AI__16CScriptOnlyBaddy");
extern "C" void CScriptOnlyBaddy_SetVariable(
    CScriptOnlyBaddy *, unsigned short)
    asm("SetVariable__16CScriptOnlyBaddyUs");
extern "C" int CScriptOnlyBaddy_ExecuteCommand(
    CScriptOnlyBaddy *, unsigned short)
    asm("ExecuteCommand__16CScriptOnlyBaddyUs");

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
    Baddy_CSuperDefaultCtor(&baddy->CSuper);
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
 * @param baddy  Baddy instance entering its dead state.
 */
void CBaddy_Die(CBaddy *baddy)
{
    if (reinterpret_cast<const CBody *>(&baddy->CSuper.CBody)->IsDead() == 0)
    {
        baddy->SendDeathPulse();
        u16 flags52 = baddy->CSuper.CBody.mCBodyFlags;
        u16 flags00 = baddy->CSuper.CBody.CItem.mFlags;
        flags52 |= 0x40;
        flags00 |= 0x41;
        baddy->CSuper.CBody.mCBodyFlags = flags52;
        baddy->CSuper.CBody.CItem.mFlags = flags00;
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
    CBaddy *baddy = this;
    ushort node;

    if ((baddy->mDeathPulseSent == '\0') &&
        (baddy->CSuper.CBody.mNode != (u16)0xffff)) {
        node = baddy->CSuper.CBody.mNode;
        baddy->mDeathPulseSent = '\x01';
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
    CBaddy *baddy = this;
    int conditionals = 0;
    int endif_cmd = 0x4120;
    int if_cmd = 0x4100;
    u16 token;
    do
    {
        u16 *scriptPtr = baddy->mpScript;
        token = *scriptPtr;
        baddy->mpScript = scriptPtr + 1;
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
    CBaddy *baddy = this;
    baddy->mpScript = p_script;
    u16 cmd = *p_script;
    if (cmd == 0x4100)
        goto end_mark;
    do
    {
        u16 *scriptPtr = baddy->mpScript;
        u16 command = *scriptPtr;
        scriptPtr++;
        baddy->mpScript = scriptPtr;
        if (command & 0x4000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(baddy);
            int (*execute)(CBaddy *, unsigned short) =
                vtable->ExecuteCommand;
            int result = execute(baddy, command);
            if (result == 0)
                goto ret0;
            goto loop_cont;
        }
        {
            int masked = command & 0x2000;
            if (masked)
            {
                BaddyVTable *vtable = Baddy_Dispatch(baddy);
                void (*setVariable)(CBaddy *, unsigned short) =
                    vtable->SetVariable;
                setVariable(baddy, command);
            }
        }
    loop_cont:
        ;
        u16 *scriptPtr2 = baddy->mpScript;
        cmd = *scriptPtr2;
    } while (cmd != 0x4100);
end_mark:
    baddy->mDoScript = 0;
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
 * @param self     Baddy receiving the command.
 * @param command  Script opcode to execute.
 * @return 0 when script execution must pause; otherwise 1.
 */
int CBaddy_ExecuteCommand(
    CBaddy *self, unsigned short command)
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
        unsigned short type = *self->mpScript++;
        if (type & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(self);
            type = vtable->Variable.GetVariable(self, type);
        }

        unsigned int flags = *self->mpScript++;
        int lifetime = *self->mpScript++;
        int gravity = *self->mpScript++;
        CPowerUp *p_powerup = PowerUp_Create(
            type, *(const CVector *)&self->CSuper.CBody.CItem.mPos,
            flags, lifetime, Baddy_ZeroVector());
        if (p_powerup)
        {
            p_powerup->SetGravity(gravity << 12, 5);
        }
        return 1;
    }

    command_429E:
        if (self->mpCreatedByPlayer != 0)
        {
            if ((CBodyOwner *)self->mpCreatedByPlayer ==
                reinterpret_cast<CBodyOwner *>(MechList))
            {
                CurrentWaterLevel = self->CSuper.CBody.CItem.mPos.vy;
                goto command_default;
            }
            CurrentWaterLevel2 = self->CSuper.CBody.CItem.mPos.vy;
            goto command_default;
        }
        CurrentWaterLevel = self->CSuper.CBody.CItem.mPos.vy;
        CurrentWaterLevel2 = CurrentWaterLevel;
        goto command_default;

    command_42A2:
        CurrentWaterDamage = *self->mpScript++;
        goto command_default;

    {
        CVector pos;

    command_429A:
    {
        unsigned short node = *self->mpScript++;
        if (node & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(self);
            node = vtable->Variable.GetVariable(self, node);
        }
        Trig_GetPosition(pos, node);
        goto process_position;
    }

    command_42A6:
        pos = *(const CVector *)&self->CSuper.CBody.CItem.mPos;

    process_position:
        switch (*self->mpScript++)
        {
        case 0:
        case 1:
            break;
        case 2:
            self->GetScriptValue();
            self->mpScript += 2;
            goto command_default;
        }
        return 1;
    }

    command_4227:
    {
        unsigned short interval = *self->mpScript++;
        if (interval)
        {
            self->mComplexSurfaceInterval = (unsigned char)interval;
            self->mComplexSurfaceTimer = 0;
            self->mSpecificFlags |= 2;
            goto command_default;
        }
        self->mSpecificFlags &= ~2U;
        goto command_default;
    }

    command_4298:
    {
        unsigned short shake = *self->mpScript++;
        if (shake & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(self);
            shake = vtable->Variable.GetVariable(self, shake);
        }

        if (CameraList == 0)
        {
            return 1;
        }
        if (shake == 0)
        {
            ((CCamera *)CameraList)->Shake(
                *(const CVector *)&self->CSuper.CBody.CItem.mPos,
                CAMERASHAKE_SMALL);
            return 1;
        }
        if (shake == 1)
        {
            ((CCamera *)CameraList)->Shake(
                *(const CVector *)&self->CSuper.CBody.CItem.mPos,
                CAMERASHAKE_MEDIUM);
            return 1;
        }
        ((CCamera *)CameraList)->Shake(
            *(const CVector *)&self->CSuper.CBody.CItem.mPos,
            CAMERASHAKE_BIG);
        return 1;
    }

    command_4296:
        SFX_MIDIFadeIn(*self->mpScript++);
        return 1;

    command_4297:
        SFX_MIDIFadeOut(*self->mpScript++);
        return 1;

    command_4299:
        printf("ignoring script cmd: setting fmv track %d\n", *self->mpScript++);
        return 1;

    command_4293:
    {
        unsigned int *p_checksum = (unsigned int *)
            (((unsigned int)self->mpScript + 3) & ~3U);
        printf("ignoring script cmd: setting fmv checksum %x\n", p_checksum);
        self->mpScript = (unsigned short *)(p_checksum + 1);
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
        unsigned short node = *self->mpScript++;
        if (node & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(self);
            node = vtable->Variable.GetVariable(self, node);
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
        unsigned short node = *self->mpScript++;
        if (node & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(self);
            node = vtable->Variable.GetVariable(self, node);
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
        unsigned short goal = *self->mpScript++;
        if ((GGame == 1) && !Career_Got(1) &&
            ((GVInfo1.VideoMode ^ VIDEO_PLAYBACK) != 0))
        {
            Panel_GoalCounter(goal);
        }
        return 1;
    }

    command_42B0:
    {
        char *p_string = (char *)self->mpScript;
        while (*p_string)
        {
            ++p_string;
        }
        if ((unsigned int)p_string & 1)
        {
            self->mpScript = (unsigned short *)(p_string + 1);
        }
        else
        {
            self->mpScript = (unsigned short *)(p_string + 2);
        }
        return 1;
    }

    command_4226:
        Baddy_ClearVector(&self->CSuper.CBody.mVel);
        goto command_default;

    command_4200:
    {
        Baddy_InitItem(&self->CSuper.CBody.CItem,
                       (char *)self->mpScript);
        char *p_string = (char *)self->mpScript;
        while (*p_string)
        {
            ++p_string;
        }
        if ((unsigned int)p_string & 1)
        {
            self->mpScript = (unsigned short *)(p_string + 1);
        }
        else
        {
            self->mpScript = (unsigned short *)(p_string + 2);
        }
        return 1;
    }

    command_4106:
    {
        CVector dummy;
        unsigned short node = *self->mpScript++;
        if (node & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(self);
            node = vtable->Variable.GetVariable(self, node);
        }
        unsigned short *pNode = Trig_GetPosition(dummy, node);
        self->mpScript = pNode + 3;
        return 1;
    }

    command_4104:
    {
        unsigned short label = *self->mpScript++;
        self->mpScriptLabel[label] = self->mpScript;
        goto command_default;
    }

    command_4105:
    {
        unsigned short *p_return = self->mpScript;
        while (*self->mpScript != 0x4100)
        {
            unsigned short script_command = *self->mpScript++;
            if (script_command == 0x4104)
            {
                unsigned short label = *self->mpScript++;
                self->mpScriptLabel[label] = self->mpScript;
            }
        }
        self->mpScript = p_return;
        goto command_default;
    }

    command_4101:
    {
        unsigned short label = *self->mpScript++;
        self->mpScript = self->mpScriptLabel[label];
        goto command_default;
    }

    command_4102:
    {
        unsigned short label = *self->mpScript++;
        self->mpScript = self->mpScriptLabel[label];
        return 0;
    }

    command_4116:
    {
        unsigned int flags = *self->mpScript++;
        if ((self->mSpecificFlags & flags) == 0)
        {
            goto command_default;
        }
        goto skip_to_endif;
    }

    command_4115:
    {
        unsigned int flags = *self->mpScript++;
        if ((self->mSpecificFlags & flags) == flags)
        {
            goto command_default;
        }
        goto skip_to_endif;
    }

    command_4110_4111:
    {
        unsigned short dummy_script[8];
        int dummy_script_index = 0;
        int var_id = *self->mpScript++;
        unsigned short *p_start = self->mpScript;
        int v0 = Baddy_Dispatch(self)->Variable.GetVariable(self, var_id);
        if (p_start != self->mpScript)
        {
            do
            {
                dummy_script[dummy_script_index] = *p_start++;
                ++dummy_script_index;
            }
            while (p_start != self->mpScript);
        }

        int v1 = *self->mpScript++;
        if (v1 & 0x2000)
        {
            v1 = Baddy_Dispatch(self)->Variable.GetVariable(self, v1);
        }
        dummy_script[dummy_script_index] =
            (command == 0x4110) ? v0 + v1 : v0 - v1;

        p_start = self->mpScript;
        self->mpScript = dummy_script;
        BaddyVTable *vtable = Baddy_Dispatch(self);
        vtable->SetVariable(self, var_id);
        self->mpScript = p_start;
        goto command_default;
    }

    command_4112:
    {
        int v0 = *self->mpScript++;
        if (v0 & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(self);
            v0 = vtable->Variable.GetVariable(self, v0);
        }

        int v1 = *self->mpScript++;
        if (v1 & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(self);
            v1 = vtable->Variable.GetVariable(self, v1);
        }

        if (v1 < v0)
        {
            goto command_default;
        }
        goto skip_to_endif;
    }

    command_4113:
    {
        int v0 = *self->mpScript++;
        if (v0 & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(self);
            v0 = vtable->Variable.GetVariable(self, v0);
        }

        int v1 = *self->mpScript++;
        if (v1 & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(self);
            v1 = vtable->Variable.GetVariable(self, v1);
        }

        if (v0 < v1)
        {
            goto command_default;
        }
        goto skip_to_endif;
    }

    command_4114:
    {
        int v0 = *self->mpScript++;
        if (v0 & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(self);
            v0 = vtable->Variable.GetVariable(self, v0);
        }

        int v1 = *self->mpScript++;
        if (v1 & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(self);
            v1 = vtable->Variable.GetVariable(self, v1);
        }

        if (v0 == v1)
        {
            goto command_default;
        }
    }

    skip_to_endif:
        self->SkipToEndif();
        return 1;

    command_4280:
    {
        self->mWaitTimer = *self->mpScript++;
        if (self->mWaitTimer & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(self);
            self->mWaitTimer =
                vtable->Variable.GetVariable(self, self->mWaitTimer);
        }
        return 0;
    }

    command_4281:
        if (self->mInputFlagsCopy & 1)
        {
            return 1;
        }
        --self->mpScript;
        return 0;

    command_4240:
        self->mDoScript = 0;
        return 0;

    command_4107:
        return 0;

    command_4201:
    {
        int anim = *self->mpScript++;
        int direction = *self->mpScript++;
        reinterpret_cast<CSuper *>(&self->CSuper)->CycleAnim(
            anim, direction);
        return 1;
    }

    command_4202:
        reinterpret_cast<CSuper *>(&self->CSuper)->RunAnim(
            *self->mpScript++, 0, -1, -1);
        return 1;

    command_4203:
        self->CSuper.CBody.CItem.mFlags &= ~1;
        goto command_default;

    command_4204:
        self->CSuper.CBody.CItem.mFlags |= 0x41;
        goto command_default;

    command_4205:
    {
        BaddyVTable *vtable = Baddy_Dispatch(self);
        vtable->Die(self);
        return 1;
    }

    command_4290:
        SFX_PlayX((short)*self->mpScript++, 0x2000, 0);
        return 1;

    command_4291:
        SFX_PlayPos(
            (short)*self->mpScript++,
            *(const CVector *)&self->CSuper.CBody.CItem.mPos, 0);
        return 1;

    command_4292:
    {
        unsigned short sparks = *self->mpScript++;
        if (BitCount >= 200)
        {
            return 1;
        }

        Bit_SetSparkTrajectory(
            *(const CSVector *)&self->CSuper.CBody.CItem.mAngles);
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
                    Bit_AllocateStorage(BIT_SPARK_STORAGE_SIZE));
                if (spark)
                {
                    Baddy_ConstructSpark(
                        spark,
                        *(CVector *)&self->CSuper.CBody.CItem.mPos,
                        0x20, 0x2000, 0x20);
                }
            }
        }
        UseCacheForBits = 1;
        goto command_default;
    }

    command_429C:
        Flash_Screen(
            (unsigned char)self->mpScript[0],
            (unsigned char)self->mpScript[1],
            (unsigned char)self->mpScript[2],
            self->mpScript[3], 0, self->mpScript[4]);
        self->mpScript += 5;
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
    CBaddy *self = this;
    u16 *scriptPtr = self->mpScript;
    u16 value = *scriptPtr;
    self->mpScript = scriptPtr + 1;
    if ((value & 0x8000) == 0)
    {
        if (value & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(self);
            value = vtable->Variable.GetVariable(self, value);
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
extern "C" BaddyScriptOnlyVTableDefinition const __vt_16CScriptOnlyBaddy = {
    0,
    0,
    Baddy_ScriptOnlyDestructorAbi,
    CBaddy_Die,
    CScriptOnlyBaddy_AI,
    (void (*)(CBodyOwner *))CBody_EveryFrame,
    Baddy_CBodyDoLightingSetup,
    Baddy_CBodyHit,
    Baddy_CBodyDeleteStuff,
    CScriptOnlyBaddy_ExecuteCommand,
    CScriptOnlyBaddy_SetVariable,
    CBaddy_GetVariable,
};

/**
 * Virtual dispatch table emitted by the base baddy owner.
 */
extern "C" BaddyBaseVTableDefinition const __vt_6CBaddy = {
    0,
    0,
    Baddy_CBaddyDestructorAbi,
    CBaddy_Die,
    Baddy_CBodyAI,
    (void (*)(CBodyOwner *))CBody_EveryFrame,
    Baddy_CBodyDoLightingSetup,
    Baddy_CBodyHit,
    Baddy_CBodyDeleteStuff,
    CBaddy_ExecuteCommand,
    CBaddy_SetVariable,
    CBaddy_GetVariable,
};

/**
 * SetVariable__6CBaddyUs (extern "C")
 *
 * Address:   0x800419A4
 * Mangled:   SetVariable__6CBaddyUs
 * Demangled: CBaddy::SetVariable(unsigned short)
 *
 * @param self  Baddy receiving the script-visible value.
 * @param variable  Script variable selector.
 */
void CBaddy_SetVariable(CBaddy *self, unsigned short variable)
{
    switch (variable)
    {
    case 0x2135:
    {
        unsigned int *p_checksum =
            (unsigned int *)(((int)self->mpScript + 3) & ~3);
        unsigned int val = *p_checksum;
        self->mpScript = (u16 *)(p_checksum + 1);
        self->mDeadChecksum = val;
        break;
    }
    case 0x212f:
    {
        unsigned int *p_checksum =
            (unsigned int *)(((int)self->mpScript + 3) & ~3);
        u16 model = Spool_GetModel(
            *p_checksum, self->CSuper.CBody.CItem.mRegion);
        self->CSuper.CBody.CItem.mModel = model;
        self->mpScript = (u16 *)(p_checksum + 1);
        break;
    }
    case 0x2140:
    {
        int val = self->GetScriptValue();
        self->CSuper.CBody.CItem.mPos.vx = (val << 16) >> 4;
        break;
    }
    case 0x2141:
    {
        int val = self->GetScriptValue();
        self->CSuper.CBody.CItem.mPos.vy = (val << 16) >> 4;
        break;
    }
    case 0x2142:
    {
        int val = self->GetScriptValue();
        self->CSuper.CBody.CItem.mPos.vz = (val << 16) >> 4;
        break;
    }
    case 0x2131:
    {
        u16 *scriptPtr = self->mpScript;
        u16 val = *scriptPtr;
        self->mpScript = scriptPtr + 1;
        if (val != 0)
        {
            self->CSuper.CBody.CItem.mFlags =
                self->CSuper.CBody.CItem.mFlags | 8;
        }
        else
        {
            self->CSuper.CBody.CItem.mFlags =
                self->CSuper.CBody.CItem.mFlags & 0xFFF7;
        }
        break;
    }
    case 0x212e:
    {
        u16 *scriptPtr = self->mpScript;
        u16 val = *scriptPtr;
        self->mpScript = scriptPtr + 1;
        self->mVerticalCompliance = val;
        break;
    }
    case 0x212c:
    {
        u16 *scriptPtr = self->mpScript;
        u8 val = *(u8 *)scriptPtr;
        self->mpScript = scriptPtr + 1;
        self->mInputSignal = val;
        break;
    }
    case 0x212d:
    {
        u16 *scriptPtr = self->mpScript;
        u8 val = *(u8 *)scriptPtr;
        self->mpScript = scriptPtr + 1;
        self->mHit = val;
        break;
    }
    case 0x2100:
    {
        u16 *scriptPtr = self->mpScript;
        u16 val = *scriptPtr;
        self->mpScript = scriptPtr + 1;
        self->CSuper.CBody.mHealth = val;
        break;
    }
    case 0x2101:
    {
        u16 *scriptPtr = self->mpScript;
        u16 val = *scriptPtr;
        self->mpScript = scriptPtr + 1;
        self->CSuper.CBody.mRMinor = val;
        break;
    }
    case 0x2125:
    {
        u16 *p = self->mpScript;
        u16 idx = *p;
        p++;
        self->mpScript = p;
        u16 val2 = *p;
        p++;
        self->mBaddyAttributes[idx] = val2;
        self->mpScript = p;
        break;
    }
    case 0x2114:
    {
        u16 *scriptPtr = self->mpScript;
        u16 val = *scriptPtr;
        self->mpScript = scriptPtr + 1;
        self->mPattern = val;
        break;
    }
    case 0x2115:
    {
        u16 *scriptPtr = self->mpScript;
        u16 val = *scriptPtr;
        self->mpScript = scriptPtr + 1;
        self->mSubPattern = val;
        break;
    }
    case 0x2120:
    {
        u16 *scriptPtr = self->mpScript;
        u16 idx = *scriptPtr;
        self->mpScript = scriptPtr + 1;
        u16 val = self->GetScriptValue();
        self->mRegister[idx & 0xFF] = val;
        break;
    }
    case 0x2121:
    {
        u16 *scriptPtr = self->mpScript;
        short val = *(short *)scriptPtr;
        self->mpScript = scriptPtr + 1;
        self->CSuper.mAnimSpeed = (int)val;
        break;
    }
    case 0x2122:
    {
        u16 *scriptPtr = self->mpScript;
        u16 val = *scriptPtr;
        self->mpScript = scriptPtr + 1;
        self->mGroundToOrigin = (s16)val;
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
 * @param self Implicit `this` pointer.
 * @param variable Script variable selector.
 * @return The selected signed 16-bit value, or zero when unavailable.
 */
short CBaddy_GetVariable(CBaddy *self, unsigned short variable)
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
                    &self->CSuper.CBody.CItem.mPos),
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
            CVectorOwner eye_pos = self->CSuper.CBody.CItem.mPos;
            eye_pos.vy -= self->mOriginToEye << 12;
            return (short)Utils_LineOfSight(
                *reinterpret_cast<const CVector *>(&eye_pos),
                *reinterpret_cast<const CVector *>(
                    &reinterpret_cast<CBodyOwner *>(MechList)->CItem.mPos));
        }
        return 0;

    case 0x212E:
        value = self->mVerticalCompliance;
        break;

    case 0x212C:
        value = self->mInputSignal;
        break;

    case 0x212D:
        value = self->mHit;
        break;

    case 0x2140:
        value = (short)(self->CSuper.CBody.CItem.mPos.vx >> 12);
        break;

    case 0x2141:
        value = (short)(self->CSuper.CBody.CItem.mPos.vy >> 12);
        break;

    case 0x2142:
        value = (short)(self->CSuper.CBody.CItem.mPos.vz >> 12);
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
        value = self->CSuper.CBody.mHealth;
        break;

    case 0x2129:
    {
        unsigned short *script = self->mpScript;
        unsigned short limit = *script;
        self->mpScript = script + 1;
        value = (short)Rnd(limit);
        break;
    }

    case 0x2120:
    {
        unsigned short *script = self->mpScript;
        unsigned char index = *(unsigned char *)script;
        self->mpScript = script + 1;
        value = self->mRegister[index];
        break;
    }

    case 0x212A:
        value = self->CSuper.CBody.mNode;
        break;

    case 0x212B:
    {
        unsigned short *script = self->mpScript;
        unsigned short node = *script;
        self->mpScript = script + 1;
        if (node & 0x2000)
        {
            BaddyVTable *vtable = Baddy_Dispatch(self);
            int (*method)(CBaddy *, unsigned short) =
                vtable->Variable.GetVariableInt;
            node = method(self, node) & 0xFFFF;
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
    CScriptOnlyBaddy *self = this;
    void *vtable = (void *)&__vt_16CScriptOnlyBaddy;
    CBruce *createdByPlayer = pCurrentPlayer;
    self->CSuper.CBody.CItem.vtable = vtable;
    self->mpCreatedByPlayer = createdByPlayer;
    CBody *body = reinterpret_cast<CBody *>(&self->CSuper.CBody);
    short *pos = body->SquirtPos(p_info);
    short *angles = body->SquirtAngles(pos);
    self->mpScript = (u16 *)angles;
    unsigned short flags = self->CSuper.CBody.CItem.mFlags;
    self->mLoopingSoundUpdate = -1;
    unsigned short field52 = self->CSuper.CBody.mCBodyFlags;
    self->mDoScript = 1;
    self->CSuper.CBody.mNode = (unsigned short)node_index;
    flags |= 0x41;
    field52 &= 0xFFEF;
    self->CSuper.CBody.CItem.mFlags = flags;
    self->CSuper.CBody.mCBodyFlags = field52;
    body->AttachTo(reinterpret_cast<CBody **>(&BaddyList));
    self->CSuper.CBody.mRMinor = 0;
    self->CSuper.CBody.CItem.mType = 0xCB;
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
            register int count asm("$5") = (int)mWeatherData;
            if (count != 0)
            {
                int i = 0;
                do
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
                    count = (int)mWeatherData;
                    i++;
                } while (i < count);
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
 * @param self Implicit `this` pointer.
 */
void CScriptOnlyBaddy_AI(CScriptOnlyBaddy *self)
{
    unsigned short inputFlags = self->CSuper.CBody.mInputFlags;
    self->CSuper.CBody.CItem.mFlags |= 0x41;
    self->mInputFlagsCopy = inputFlags;

    if (inputFlags & 1)
    {
        self->CSuper.CBody.mInputFlags = inputFlags & 0xFFFE;
        self->mInputSignal++;
    }

    if (self->mDoScript != 0)
    {
        if (self->mWaitTimer != 0)
        {
            self->mWaitTimer--;
        }
        else
        {
            self->ParseScript(self->mpScript);
        }
    }
    else
    {
        BaddyVTable *vtable = Baddy_Dispatch(self);
        void (*quickAI)(CBaddy *) = vtable->Die;
        quickAI(self);
    }

    if (self->mLoopingSoundUpdate != -1 &&
        TTime % self->mLoopingSoundUpdate == 0)
    {
        int t = SFXLEVEL;
        unsigned int sound = self->mLoopingSound;
        SFXLEVEL = t * self->mLoopingSoundVol / 100;
        SFX_ModifyPos(
            sound,
            *(const CVector *)&self->CSuper.CBody.CItem.mPos,
            0);
        SFXLEVEL = t;
    }

    if (self->mpWeather != 0)
    {
        int ground = reinterpret_cast<CBodyOwner *>(MechList)->mShadowPos.vy;
        int i = 0;

        if (self->mWeatherData != 0)
        {
            CVectorOwner pos;
            CVectorOwner offset;
            CVectorOwner sum;
            CVectorOwner tail;
            CVectorOwner cameraVelocity;
            CVectorOwner *posVector = &pos;
            CVectorOwner *offsetVector = &offset;
            int shift;

            do
            {
                SRainEntry *p_entry = &self->mpWeather[i];

                {
                    CGPolyLine *line = p_entry->p_line;
                    pos = *(CVectorOwner *)&line->mStart;
                }
                reinterpret_cast<CVector &>(*posVector) += p_entry->vel;

                if (ground < pos.vy)
                {
                    int randomX = (Rnd(0x960) - 0x4B0) << 12;
                    int randomY = (Rnd(0x190) - 0x4B0) << 12;
                    int randomZ = (Rnd(0x960) - 0x4B0) << 12;
                    offset.vx = randomX;
                    offset.vy = randomY;
                    offset.vz = randomZ;
                    Baddy_VectorAddAbi(
                        &sum,
                        &reinterpret_cast<CBodyOwner *>(MechList)->CItem.mPos,
                        &offset);
                    pos = sum;
                }

                *(CVectorOwner *)&p_entry->p_line->mStart = pos;
                SLineSeg *segment = p_entry->p_line->mSegs;
                Baddy_VectorSubtractAbi(
                    &tail, posVector,
                    (const CVectorOwner *)&p_entry->vel);
                shift = 1;
                Baddy_VectorShiftRightAbi(
                    &cameraVelocity,
                    &reinterpret_cast<CBodyOwner *>(CameraList)->mVel,
                    &shift);
                Baddy_VectorAddAbi(offsetVector, &tail, &cameraVelocity);
                *(CVectorOwner *)&segment->End = offset;

                i++;
            } while (i < self->mWeatherData);
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
 * @param self  Script-only baddy receiving the variable.
 * @param variable  Script variable selector.
 */
void CScriptOnlyBaddy_SetVariable(
    CScriptOnlyBaddy *self, unsigned short variable)
{
    CBaddy_SetVariable(self, variable);
}

/**
 * Execute one of the commands owned by a script-only environmental actor.
 *
 * Address:   0x80042364
 * Mangled:   ExecuteCommand__16CScriptOnlyBaddyUs
 * Demangled: CScriptOnlyBaddy::ExecuteCommand(unsigned short)
 *
 * @param self     Script-only actor receiving the command.
 * @param command  Script opcode to execute.
 * @return One when the command was handled, otherwise the base handler's result.
 */
int CScriptOnlyBaddy_ExecuteCommand(
    CScriptOnlyBaddy *self, unsigned short command)
{
    CVectorOwner origin;
    int i;
    SRainEntry *p_entry;
    CVectorOwner offset;
    CGPolyLine *p_droplet;

    switch (command)
    {
    case 0x4507:
    {
        if (self->mLoopingSound != 0)
        {
            SFX_Stop(self->mLoopingSound);
        }

        unsigned short *script = self->mpScript;
        unsigned short sound = *script;
        self->mpScript = script + 1;
        unsigned int handle = SFX_PlayX(sound, 0x2000, 0);
        int volume = SFX_v;
        self->mLoopingSound = handle;
        self->mLoopingSoundUpdate = -1;
        self->mLoopingSoundVol = volume;
        break;
    }

    case 0x4508:
    {
        if (self->mLoopingSound != 0)
        {
            SFX_Stop(self->mLoopingSound);
        }

        unsigned short sound = *self->mpScript++;
        self->mLoopingSoundUpdate = *self->mpScript++;
        unsigned int handle = SFX_PlayPos(
            sound,
            *(const CVector *)&self->CSuper.CBody.CItem.mPos,
            0);
        int volume = SFX_v;
        self->mLoopingSound = handle;
        self->mLoopingSoundVol = volume;
        break;
    }

    case 0x4509:
        if (self->mLoopingSound != 0)
        {
            SFX_Stop(self->mLoopingSound);
        }
        self->mLoopingSound = 0;
        break;

    case 0x4506:
        if (reinterpret_cast<CBodyOwner *>(MechList) != 0)
        {
            self->CSuper.CBody.CItem.mPos =
                reinterpret_cast<CBodyOwner *>(MechList)->CItem.mPos;
        }
        break;

    case 0x4700:
    {
        unsigned short *script = self->mpScript;
        unsigned short enabled = *script;
        self->mpScript = script + 1;
        if (enabled != 0)
        {
            self->mSpecificFlags |= 0x100;
        }
        else
        {
            self->mSpecificFlags &= ~0x100;
        }
        break;
    }

    case 0x4504:
        self->mpScript += 4;
        // Fall through: this variant skips four leading script parameters.
    case 0x429B:
    {
        CSmokeJet *smoke = static_cast<CSmokeJet *>(
            Bit_AllocateStorage(sizeof(CSmokeJet)));
        if (smoke != 0)
        {
            unsigned short *script = self->mpScript;
            smoke = Smoke_Construct(
                smoke,
                reinterpret_cast<const CVector &>(
                    self->CSuper.CBody.CItem.mPos),
                reinterpret_cast<const CSVector &>(
                    self->CSuper.CBody.CItem.mAngles),
                script[0],
                0xFFFF,
                script[1],
                script[2],
                script[3],
                script[4],
                *((unsigned char *)script + 10),
                0xCCE0187C);
        }

        self->mpSmokeJet = smoke;
        smoke->mProtected = 1;
        self->mpScript += 6;
        break;
    }

    case 0x42A1:
    {
        CSmokeJet *smoke = static_cast<CSmokeJet *>(
            Bit_AllocateStorage(sizeof(CSmokeJet)));
        if (smoke != 0)
        {
            unsigned short *script = self->mpScript;
            smoke = Smoke_Construct(
                smoke,
                reinterpret_cast<const CVector &>(
                    self->CSuper.CBody.CItem.mPos),
                reinterpret_cast<const CSVector &>(
                    self->CSuper.CBody.CItem.mAngles),
                script[0],
                0xFFFF,
                script[1],
                script[2],
                script[3],
                script[4],
                *((unsigned char *)script + 10),
                0x68B7B136);
        }

        self->mpSmokeJet = smoke;
        self->mpScript += 6;
        break;
    }

    case 0x429F:
    {
        CSmokeJet *smoke = self->mpSmokeJet;
        if (smoke != 0)
        {
            void (*destroy)(void *, int) = smoke->_vf->Destroy;
            destroy(smoke, 3);
        }
        self->mpSmokeJet = 0;
        break;
    }

    case 0x4503:
    {
        self->mWeatherData = self->GetScriptValue();
        SRainEntry *weather = (SRainEntry *)Mem_New(
            (unsigned int)self->mWeatherData << 4, 0, 1);
        CBodyOwner *mech = reinterpret_cast<CBodyOwner *>(MechList);
        unsigned int weatherCount = self->mWeatherData;
        self->mpWeather = weather;
        UseCacheForBits = 0;

        origin = mech->CItem.mPos;
        i = 0;
        if (weatherCount != 0)
        {
            CVectorOwner end;
            CVectorOwner tail;
            CVectorOwner cameraVelocity;
            int shift;
            CVectorOwner *offsetVector = &offset;
            CVectorOwner *endVector = &end;

            do
            {
                p_entry = &self->mpWeather[i];

                int offsetX = (Rnd(0x960) - 0x4B0) << 12;
                int offsetY = (Rnd(0x190) - 0x4B0) << 12;
                int offsetZ = (Rnd(0x960) - 0x4B0) << 12;
                offset.vx = offsetX;
                offset.vy = offsetY;
                offset.vz = offsetZ;
                reinterpret_cast<CVector &>(*offsetVector) +=
                    reinterpret_cast<const CVector &>(origin);

                int velocityX = (Rnd(0x10) - 8) << 12;
                int velocityY = (Rnd(0x30) + 0x30) << 12;
                int velocityZ = (Rnd(0x10) - 8) << 12;
                p_entry->vel.vx = velocityX;
                p_entry->vel.vy = velocityY;
                p_entry->vel.vz = velocityZ;

                p_droplet = static_cast<CGPolyLine *>(
                    Bit_AllocateStorage(sizeof(CGPolyLine)));
                if (p_droplet != 0)
                {
                    p_droplet = Baddy_ConstructWeatherLine(p_droplet, 1);
                }

                p_entry->p_line = p_droplet;
                p_droplet->mProtected = 1;
                *(CVectorOwner *)&p_droplet->mStart = offset;

                SLineSeg *segment = p_droplet->mSegs;
                Baddy_VectorSubtractAbi(
                    &tail, offsetVector,
                    (const CVectorOwner *)&p_entry->vel);
                shift = 1;
                Baddy_VectorShiftRightAbi(
                    &cameraVelocity,
                    &reinterpret_cast<CBodyOwner *>(CameraList)->mVel,
                    &shift);
                Baddy_VectorAddAbi(endVector, &tail, &cameraVelocity);
                *(CVectorOwner *)&segment->End = end;

                p_droplet->mStartR = 0x40;
                p_droplet->mStartG = 0x40;
                p_droplet->mStartB = 0x50;
                p_droplet->mSegs->r = 0x10;
                p_droplet->mSegs->g = 0x10;
                p_droplet->mSegs->b = 0x20;
                p_droplet->SetSemiTransparent();

                i++;
            } while (i < self->mWeatherData);
        }

        UseCacheForBits = 1;
        break;
    }

    default:
        return CBaddy_ExecuteCommand(self, command);
    }

    return 1;
}
