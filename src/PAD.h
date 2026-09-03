#if defined(PAD_DECLARE_BUFFER_DESTRUCTOR_ABI)
struct PadBuffer;
/** Addressable deleting-destructor entry used by the shell pad-buffer table. */
static void Shell_PadBufferDestructor(PadBuffer *buffer, int inCharge)
    asm("_._9PadBuffer");
#endif

#if defined(PAD_BUTTON_LAYOUT_ONLY) && !defined(PAD_BUTTON_LAYOUT)
#error PAD_BUTTON_LAYOUT_ONLY requires PAD_BUTTON_LAYOUT
#endif

#if defined(PAD_API_ONLY) && !defined(PAD_BUTTON_LAYOUT_ONLY) && \
    !defined(PAD_API_ONLY_EMITTED)
#define PAD_API_ONLY_EMITTED

struct SControl;

/** Initialize gameplay-pad links and begin controller communication. */
void Pad_InitAtStart(void);

/** Select and lock analogue mode on both physical controller ports. */
void Pad_LockAnalog(void);

/** Clear button, analogue, and reset state for one gameplay pad. */
void Pad_ClearAllOne(int pad);

/** Clear both gameplay pads. */
void Pad_ClearAll(void);

/** Clear every button's one-frame trigger flag on one gameplay pad. */
void Pad_ClearTriggers(SControl *p_pad);

/** Start or update one DualShock actuator and its shutoff timer. */
void Pad_ActuatorOn(unsigned char index, unsigned short time,
                    unsigned char motor, unsigned char motor_value);

/** Stop one DualShock actuator and clear its timer. */
void Pad_ActuatorOff(unsigned char index, unsigned char motor);

/** Poll both ports and update their digital, analogue, and motor state. */
int Pad_Update(void);

/** Copy canonical face-button state into each configured remap target. */
void Pad_Restore(void);

/** Copy configured face-button state back into each canonical slot. */
void Pad_Remap(void);

#endif // PAD_API_ONLY && !PAD_BUTTON_LAYOUT_ONLY && !PAD_API_ONLY_EMITTED

#if !defined(PAD_API_ONLY) && !defined(PAD_BUTTON_LAYOUT_ONLY) && \
    !defined(PAD_INC_H)
#define PAD_INC_H
#define PAD_ORDINARY_LAYOUT_ACTIVE


/*
 * PAD-owned input layouts recovered from MAIN_STRUCTS.h.  Keeping this small
 * slice local avoids pulling the unrelated, incomplete gameplay layouts from
 * a monolithic compatibility header into the controller translation unit.
 */
typedef unsigned char u8;
typedef unsigned short u16;

#ifndef PAD_BUTTON_LAYOUT
#define PAD_BUTTON_LAYOUT
#define PAD_BUTTON_LAYOUT_IMPLIED_BY_ORDINARY
#endif
#endif // !PAD_API_ONLY && !PAD_BUTTON_LAYOUT_ONLY && !PAD_INC_H

#if defined(PAD_BUTTON_LAYOUT) && !defined(PAD_BUTTON_LAYOUT_EMITTED)
#define PAD_BUTTON_LAYOUT_EMITTED

/** One digital button's current state and frame-history counters. */
struct SButton {
    unsigned char Pressed;
    unsigned char Triggered;
    int PressedTime;
    int ReleasedTime;
    int TriggeredTime;
};

#endif // PAD_BUTTON_LAYOUT && !PAD_BUTTON_LAYOUT_EMITTED

#if defined(PAD_BUTTON_LAYOUT_IMPLIED_BY_ORDINARY)
#undef PAD_BUTTON_LAYOUT_IMPLIED_BY_ORDINARY
#undef PAD_BUTTON_LAYOUT
#endif

#if defined(PAD_ORDINARY_LAYOUT_ACTIVE)

/** Decoded gameplay state, motor state, and remap targets for one pad. */
struct SControl {
    struct SButton Triangle;
    struct SButton Square;
    struct SButton Circle;
    struct SButton X;
    struct SButton LeftOne;
    struct SButton LeftTwo;
    struct SButton RightOne;
    struct SButton RightTwo;
    struct SButton Left;
    struct SButton Right;
    struct SButton Up;
    struct SButton Down;
    struct SButton AnalogueLeft;
    struct SButton AnalogueRight;
    struct SButton Start;
    struct SButton Select;
    struct SButton Crouch;
    struct SButton Jump;
    struct SButton SmartBomb;
    struct SButton SelectWeapon;
    unsigned char AnaloguePotMapping[4];
    unsigned char RawAnalogueMoveForwardsBackwards;
    unsigned char RawAnalogueMoveLeftRight;
    unsigned char RawAnalogueAimForwardsBackwards;
    unsigned char RawAnalogueAimLeftRight;
    char AnalogueMoveForwardsBackwards;
    char AnalogueMoveLeftRight;
    char AnalogueAimForwardsBackwards;
    char AnalogueAimLeftRight;
    int Type;
    unsigned int ResetCounter;
    unsigned char Motor0;
    unsigned char Motor1;
    unsigned char Lock;
    unsigned char AlignCalled;
    unsigned short Motor0Timer;
    unsigned short Motor1Timer;
    struct SButton *pTriangle;
    struct SButton *pSquare;
    struct SButton *pCircle;
    struct SButton *pX;
};

/** Eight-byte controller packet decoded from a physical port or multitap. */
struct ControllerData {
    unsigned char status;
    unsigned char type;
    unsigned char button1;
    unsigned char button2;
    unsigned char analog0;
    unsigned char analog1;
    unsigned char analog2;
    unsigned char analog3;
};

/** Raw thirty-four-byte receive buffer filled by the PSY-Q pad library. */
struct ControllerBuffer {
    unsigned char pad[34];
};

typedef char SButton_size_must_be_16[(sizeof(SButton) == 16) ? 1 : -1];
typedef char SControl_size_must_be_364[(sizeof(SControl) == 364) ? 1 : -1];
typedef char ControllerData_size_must_be_8[(sizeof(ControllerData) == 8) ? 1 : -1];
typedef char ControllerBuffer_size_must_be_34[(sizeof(ControllerBuffer) == 34) ? 1 : -1];

/** Shared gameplay input state for the two supported controllers. */
extern struct SControl Pad[2];


void Pad_InitAtStart(void);
void Pad_LockAnalog(void);
void Pad_ClearAllOne(int pad);
void Pad_ClearAll(void);
void Pad_ClearTriggers(SControl *p_pad);
void Pad_ActuatorOn(unsigned char index, unsigned short time,
                    unsigned char motor, unsigned char motor_value);
void Pad_ActuatorOff(unsigned char index, unsigned char motor);
int Pad_Update(void);
void Pad_Restore(void);
void Pad_Remap(void);

#undef PAD_ORDINARY_LAYOUT_ACTIVE
#endif // PAD_ORDINARY_LAYOUT_ACTIVE
