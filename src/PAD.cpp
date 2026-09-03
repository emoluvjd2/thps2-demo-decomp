// PAD.cpp -- Reconstructed translation unit
// Original: PAD.CPP
// Functions: 15
// Address range: 0x800698E8 - 0x8006A7D0
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.

#include "PAD.h"
#include "MAIN.h"
#include <LIBPAD.H>

// === Owner-local function declarations ===
static void ReadController(ControllerData *p_controller_data, unsigned char *p_buf);
void Pad_Clear(SControl *p_pad);

// MAIN-owned game-state selector used to suppress vibration in demo play.
/** Persistent gameplay input state for the two supported controllers. */
SControl Pad[2] __attribute__((section(".data"))) = {};

/** Raw library receive buffers for the two physical controller ports. */
ControllerBuffer controllers[2] __attribute__((section(".data"))) = {};

/** Decoded state for the four possible multitap controllers on each port. */
ControllerData theControllers[8] __attribute__((section(".data"))) = {};

/** Calibrated analogue-stick bounds and centers. */
int XMinL = 0;
int XMaxL = 255;
int YMinL = 0;
int YMaxL = 255;
int XMinR = 0;
int XMaxR = 255;
int YMinR = 0;
int YMaxR = 255;
int XCenterL = 128;
int YCenterL = 128;
int XCenterR = 128;
int YCenterR = 128;

/** Timer used by the unplugged-controller warning. */
int WarningTimer = 0;

/** Frames elapsed since the last controller activity. */
int Pad_IdleTime = 0;

/** DualShock actuator alignment packet retained within the PAD owner. */
static unsigned char align[6]
    __attribute__((section(".sdata"))) = {0, 1, 255, 255, 255, 255};

/** Whether the controller subsystem has completed initialization. */
static int ControlSystemInitialised = 0;

// === Function implementations (address order) ===

/* Advance a field cursor to the corresponding field in the next pad. */
#define NEXT_PAD_BUTTON(button) \
    ((SButton *)((unsigned char *)(button) + sizeof(SControl)))
#define NEXT_PAD_BUTTON_LINK(button_link) \
    ((SButton **)((unsigned char *)(button_link) + sizeof(SControl)))

/**
 * Initialize pad button pointer links and controller state.
 *
 * Address: 0x800698E8
 */
void Pad_InitAtStart(void)
{
    int i = 0;
    SControl *pad_base = Pad;
    SControl *p_pad;
    SButton **p_x_mapping;
    SButton *x_button;
    SButton **p_square_mapping;
    SButton *square_button;
    SButton **p_circle_mapping;
    SButton *circle_button;
    SButton **p_triangle_mapping;

    p_x_mapping = &pad_base[0].pX;
    x_button = &pad_base[0].X;
    p_square_mapping = &pad_base[0].pSquare;
    square_button = &pad_base[0].Square;
    p_circle_mapping = &pad_base[0].pCircle;
    circle_button = &pad_base[0].Circle;
    p_triangle_mapping = &pad_base[0].pTriangle;
    p_pad = pad_base;
    do
    {
        *p_triangle_mapping = &p_pad->Triangle;
        *p_circle_mapping = circle_button;
        *p_square_mapping = square_button;
        *p_x_mapping = x_button;
        p_x_mapping = NEXT_PAD_BUTTON_LINK(p_x_mapping);
        x_button = NEXT_PAD_BUTTON(x_button);
        p_square_mapping = NEXT_PAD_BUTTON_LINK(p_square_mapping);
        square_button = NEXT_PAD_BUTTON(square_button);
        p_circle_mapping = NEXT_PAD_BUTTON_LINK(p_circle_mapping);
        circle_button = NEXT_PAD_BUTTON(circle_button);
        p_triangle_mapping = NEXT_PAD_BUTTON_LINK(p_triangle_mapping);
        i++;
        p_pad++;
    } while (i < 2);
    Pad_ClearAll();
    PadInitMtap(controllers[0].pad, controllers[1].pad);
    PadStartCom();
    Pad_IdleTime = 0;
    ResetMainDemoTimer();
    ControlSystemInitialised = 1;
}

#undef NEXT_PAD_BUTTON
#undef NEXT_PAD_BUTTON_LINK

/**
 * Select and lock analogue mode on both physical controller ports.
 *
 * Address: 0x80069998
 */
void Pad_LockAnalog(void)
{
    PadSetMainMode(0, 1, 3);
    PadSetMainMode(16, 1, 3);
}

/**
 * Clear button, analogue, and reset state for one gameplay pad.
 *
 * Address: 0x800699D0
 *
 * @param pad Index of the gameplay pad to clear.
 */
void Pad_ClearAllOne(int pad)
{
    int offset = pad * sizeof(SControl);
    unsigned char *base = (unsigned char *)Pad;
    SControl *p_pad = (SControl *)(base + offset);

    Pad_Clear(p_pad);
    Pad[pad].ResetCounter = 0;
    p_pad->AnalogueAimLeftRight = 0;
    p_pad->AnalogueAimForwardsBackwards = 0;
    p_pad->AnalogueMoveLeftRight = 0;
    p_pad->AnalogueMoveForwardsBackwards = 0;
}

/**
 * Clear both gameplay pads.
 *
 * Address: 0x80069A44
 */
void Pad_ClearAll(void)
{
    int i;
    for (i = 0; i < 2; i++)
    {
        Pad_ClearAllOne(i);
    }
}

/**
 * Clear every button-history record and reset the first pad's analogue values.
 *
 * Address: 0x80069A7C
 *
 * @param p_pad Controller state whose twenty button records are cleared.
 */
void Pad_Clear(SControl *p_pad)
{
    SButton *pBut = &p_pad->Triangle;
    int i = 0;
    do
    {
        pBut->Pressed = 0;
        pBut->Triggered = 0;
        pBut->PressedTime = 0;
        pBut->ReleasedTime = 0;
        pBut->TriggeredTime = 0;
        pBut++;
        i++;
    } while (i < 20);

    Pad[0].AnalogueMoveForwardsBackwards = 0;
    Pad[0].AnalogueMoveLeftRight = 0;
    Pad[0].AnalogueAimForwardsBackwards = 0;
    Pad[0].AnalogueAimLeftRight = 0;
}

/**
 * Clear the one-frame trigger flag from every button on a pad.
 *
 * Address: 0x80069AD0
 *
 * @param p_pad Controller state whose twenty button triggers are cleared.
 */
void Pad_ClearTriggers(SControl *p_pad)
{
    int i;
    SButton *pBut = &p_pad->Triangle;

    for (i = 0; i < 20; i++)
    {
        pBut->Triggered = 0;
        pBut++;
    }
}

/**
 * Start or update one DualShock actuator and its shutoff timer.
 *
 * Address: 0x80069AF0
 *
 * @param index Gameplay-pad index.
 * @param time Number of updates before the actuator is stopped.
 * @param motor Actuator index, zero or one.
 * @param motor_value Value sent to the selected actuator.
 */
void Pad_ActuatorOn(unsigned char index, unsigned short time, unsigned char motor, unsigned char motor_value)
{
    int checkIndex = 0;
    int port;
    int state;
    register int aligned asm("$20");

    if (GGame != 7)
    {
        checkIndex = index;
    }

    port = (index > 0) ? 16 : 0;
    state = PadGetState(port);

    if (state != 0)
    {
        aligned = 1;

        if (state == aligned)
        {
            unsigned char *base = (unsigned char *)Pad;
            ((SControl *)(base + checkIndex * sizeof(SControl)))->AlignCalled = 0;
        }

        {
            unsigned char *base = (unsigned char *)Pad;
            SControl *p_pad = (SControl *)(base + checkIndex * sizeof(SControl));

            if (p_pad->AlignCalled == 0)
            {
                PadSetAct(port, &Pad[index].Motor0, 2);

                if (state == 2 ||
                    (state == 6 && PadSetActAlign(port, align) != 0))
                {
                    p_pad->AlignCalled = aligned;
                }
            }

            unsigned char *p_motor_value = &Pad[index].Motor0 + motor;
            unsigned short *p_motor_timer = &Pad[index].Motor0Timer;

            *p_motor_value = motor_value;
            p_motor_timer[motor] = time;
        }
    }
}

/**
 * Stop one DualShock actuator and clear its timer.
 *
 * Address: 0x80069C94
 *
 * @param index Gameplay-pad index.
 * @param motor Actuator index, zero or one.
 */
void Pad_ActuatorOff(unsigned char index, unsigned char motor)
{
    unsigned char *p_motor_value = &Pad[index].Motor0 + motor;
    unsigned short *p_motor_timer = &Pad[index].Motor0Timer + motor;

    *p_motor_value = 0;
    *p_motor_timer = 0;
}

/**
 * Update one button's pressed, released, and triggered history.
 *
 * Address: 0x80069CE4
 *
 * @param p_but Button history to update.
 * @param state Nonzero when the button is currently pressed.
 */
void Pad_Button(SButton *p_but, int state)
{
    p_but->TriggeredTime++;
    if (p_but->Pressed == 0)
    {
        if (state != 0)
        {
            p_but->Triggered = 1;
            p_but->TriggeredTime = 0;
        }
    }

    if (state != 0)
    {
        Pad_IdleTime = 0;
        ResetMainDemoTimer();
        p_but->Pressed = 1;
    }
    else
    {
        p_but->Pressed = 0;
    }

    if (p_but->Pressed != 0)
    {
        p_but->ReleasedTime = 0;
        p_but->PressedTime++;
    }
    else
    {
        p_but->PressedTime = 0;
        p_but->ReleasedTime++;
    }
}

/**
 * Poll controller buffers and refresh pad state.
 *
 * Address: 0x80069D80
 */
static void ReadControllers(void)
{
    for (int controller_port = 0; controller_port < 2; controller_port++)
    {
        if (controllers[controller_port].pad[1] == 0x80)
        {
            ReadController(&theControllers[controller_port * 4], &controllers[controller_port].pad[2]);
            ReadController(&theControllers[controller_port * 4 + 1], &controllers[controller_port].pad[10]);
            ReadController(&theControllers[controller_port * 4 + 2], &controllers[controller_port].pad[18]);
            ReadController(&theControllers[controller_port * 4 + 3], &controllers[controller_port].pad[26]);
        }
        else
        {
            ReadController(&theControllers[controller_port * 4], controllers[controller_port].pad);
            theControllers[controller_port * 4 + 1].type = 0;
            theControllers[controller_port * 4 + 2].type = 0;
            theControllers[controller_port * 4 + 3].type = 0;
        }
    }
}

/**
 * Decode one eight-byte controller packet and restart communication on a type change.
 *
 * Address: 0x80069ECC
 *
 * @param p_controller_data Decoded controller state to update.
 * @param p_buf Raw controller packet returned by the pad library.
 */
static void ReadController(ControllerData *p_controller_data, unsigned char *p_buf)
{
    u8 status;
    status = p_buf[0];
    u8 old_type = p_controller_data->type;
    p_controller_data->status = status;
    p_controller_data->type = p_buf[1];
    p_controller_data->button1 = p_buf[2];
    p_controller_data->button2 = p_buf[3];
    p_controller_data->analog0 = p_buf[4];
    p_controller_data->analog1 = p_buf[5];
    p_controller_data->analog2 = p_buf[6];
    p_controller_data->analog3 = p_buf[7];
    if (p_buf[0] == 0xFF)
    {
        p_controller_data->type = 0;
    }
    else if (old_type != p_controller_data->type)
    {
        PadStopCom();
        PadStartCom();
        Pad[0].AlignCalled = 0;
        Pad[1].AlignCalled = 0;
    }
}

/**
 * Normalize one calibrated analogue axis to the signed gameplay range.
 *
 * Address: 0x80069F88
 *
 * @param x Raw axis value.
 * @param min Calibrated minimum.
 * @param max Calibrated maximum.
 * @param c Calibrated center.
 * @return A value from -127 to 127, with a 32-unit center dead zone.
 */
int Pad_Analog(int x, int min, int max, int c)
{
    if (x < min)
        return -127;
    if (max < x)
        return 127;
    if (x < c - 32)
    {
        Pad_IdleTime = 0;
        int diff = x + 32;
        diff = c - diff;
        int scaled = diff << 7;
        diff = diff - scaled;
        int range = min + 32;
        range = c - range;
        return diff / range;
    }
    if (c + 32 < x)
    {
        int diff = x - 32;
        Pad_IdleTime = 0;
        diff = diff - c;
        int scaled = diff << 7;
        scaled = scaled - diff;
        int range = max - 32;
        range = range - c;
        return scaled / range;
    }
    return 0;
}

/**
 * Poll both controller ports and update their digital, analogue, and motor state.
 *
 * Address: 0x8006A020
 *
 * @return 1 when either controller has held the reset chord long enough; otherwise 0.
 */
int Pad_Update(void)
{
    unsigned int pad_data;
    SButton *pBut;
    unsigned short pad0;
    unsigned short pad1;
    static int olda0;
    static int oldb0;
    static int oldc0;
    static int oldd0;
    static int olda1;
    static int oldb1;
    static int oldc1;
    static int oldd1;
    int a0;
    int b0;
    int c0;
    int d0;
    int a1;
    int b1;
    int c1;
    int d1;

    {
        int i = 0;

        do
        {
            if (Pad[i].Motor0Timer != 0)
            {
                Pad[i].Motor0Timer--;
                if (Pad[i].Motor0Timer == 0)
                {
                    Pad_ActuatorOff((unsigned char)i, 0);
                }
            }

            if (Pad[i].Motor1Timer != 0)
            {
                Pad[i].Motor1Timer--;
                if (Pad[i].Motor1Timer == 0)
                {
                    Pad_ActuatorOff((unsigned char)i, 1);
                }
            }

            i++;
        } while (i < 2);
    }

    ReadControllers();

    if (theControllers[0].type == 'A' || theControllers[0].type == 's')
    {
        pad0 = (unsigned short)~((theControllers[0].button1 << 8) | theControllers[0].button2);
    }
    else
    {
        Pad_ActuatorOff(0, 0);
        Pad_ActuatorOff(0, 1);
        pad0 = 0;
    }

    if (theControllers[4].type == 'A' || theControllers[4].type == 's')
    {
        pad1 = (unsigned short)~((theControllers[4].button1 << 8) |
                                 theControllers[4].button2);
    }
    else
    {
        Pad_ActuatorOff(1, 0);
        Pad_ActuatorOff(1, 1);
        pad1 = 0;
    }

    pad_data = ((unsigned int)pad1 << 16) + pad0;
    Pad[0].Type = theControllers[0].type;
    Pad[1].Type = theControllers[4].type;

    a0 = theControllers[0].analog0 & ~3;
    b0 = theControllers[0].analog1 & ~3;
    c0 = theControllers[0].analog2 & ~3;
    d0 = theControllers[0].analog3 & ~3;
    a1 = theControllers[4].analog0 & ~3;
    b1 = theControllers[4].analog1 & ~3;
    c1 = theControllers[4].analog2 & ~3;
    d1 = theControllers[4].analog3 & ~3;

    if (pad_data != 0 ||
        a0 != olda0 || b0 != oldb0 || c0 != oldc0 || d0 != oldd0 ||
        a1 != olda1 || b1 != oldb1 || c1 != oldc1 || d1 != oldd1)
    {
        olda0 = a0;
        oldb0 = b0;
        oldc0 = c0;
        oldd0 = d0;
        olda1 = a1;
        oldb1 = b1;
        oldc1 = c1;
        oldd1 = d1;
    }
    else
    {
        Pad_IdleTime++;
    }

    if (((pad_data >> 16) & 0x900) != 0x900)
    {
        Pad[1].ResetCounter = 0;
    }
    else
    {
        Pad[1].ResetCounter++;
    }

    if (Pad[0].ResetCounter == 120 || Pad[1].ResetCounter == 120)
    {
        return 1;
    }

    {
        int p = 0;
        do
        {
            if (Pad[p].Type == 's' || Pad[p].Type == 'S')
            {
                Pad[p].AnalogueAimLeftRight = (char)Pad_Analog(theControllers[p * 4].analog0, XMinR, XMaxR, XCenterR);
                Pad[p].AnalogueAimForwardsBackwards = (char)Pad_Analog(theControllers[p * 4].analog1, YMinR, YMaxR, YCenterR);
                Pad[p].AnalogueMoveLeftRight = (char)Pad_Analog(theControllers[p * 4].analog2, XMinL, XMaxL, XCenterL);
                Pad[p].AnalogueMoveForwardsBackwards = (char)Pad_Analog(theControllers[p * 4].analog3, YMinL, YMaxL, YCenterL);
            }
            else
            {
                Pad[p].AnalogueAimLeftRight = 0;
                Pad[p].AnalogueAimForwardsBackwards = 0;
                Pad[p].AnalogueMoveLeftRight = 0;
                Pad[p].AnalogueMoveForwardsBackwards = 0;
            }

            p++;
        } while (p < 2);
    }

    {
        int p = 0;
        do
        {
            pBut = &Pad[p].Triangle;

            Pad_Button(pBut++, pad_data & 0x10);
            Pad_Button(pBut++, pad_data & 0x80);
            Pad_Button(pBut++, pad_data & 0x20);
            Pad_Button(pBut++, pad_data & 0x40);
            Pad_Button(pBut++, pad_data & 0x04);
            Pad_Button(pBut++, pad_data & 0x01);
            Pad_Button(pBut++, pad_data & 0x08);
            Pad_Button(pBut++, pad_data & 0x02);

            Pad_Button(pBut++, (pad_data & 0x8000) | ((signed char)Pad[p].AnalogueMoveLeftRight < -40));
            Pad_Button(pBut++, (pad_data & 0x2000) | ((signed char)Pad[p].AnalogueMoveLeftRight >= 41));
            Pad_Button(pBut++, (pad_data & 0x1000) | ((signed char)Pad[p].AnalogueMoveForwardsBackwards < -40));
            Pad_Button(pBut++, (pad_data & 0x4000) | ((signed char)Pad[p].AnalogueMoveForwardsBackwards >= 41));
            Pad_Button(pBut++, pad_data & 0x0200);
            Pad_Button(pBut++, pad_data & 0x0400);
            Pad_Button(pBut++, pad_data & 0x0800);
            Pad_Button(pBut, pad_data & 0x0100);

            p++;
            pad_data >>= 16;
        } while (p < 2);
    }

    return 0;
}

/**
 * Copy canonical face-button state into each pad's configured remap targets.
 *
 * Address: 0x8006A5B4
 */
void Pad_Restore(void)
{
    SButton TempButtonTriangle;
    SButton TempButtonX;
    SButton TempButtonCircle;
    SButton TempButtonSquare;
    int i;
    SButton Temp;

    i = 0;
    do
    {
        TempButtonTriangle = Pad[i].Triangle;
        TempButtonCircle = Pad[i].Circle;
        TempButtonSquare = Pad[i].Square;
        TempButtonX = Pad[i].X;

        *Pad[i].pTriangle = TempButtonTriangle;
        *Pad[i].pCircle = TempButtonCircle;
        *Pad[i].pSquare = TempButtonSquare;
        *Pad[i].pX = TempButtonX;

        if (Cheat_FlipScreen)
        {
            Temp = Pad[i].Right;
            Pad[i].Right = Pad[i].Left;
            Pad[i].Left = Temp;
        }

        i++;
    } while (i < 2);
}

/**
 * Copy each pad's configured face-button state back into its canonical slots.
 *
 * Address: 0x8006A7D0
 */
void Pad_Remap(void)
{
    SButton TempButtonTriangle;
    SButton TempButtonX;
    SButton TempButtonCircle;
    SButton TempButtonSquare;
    int i;
    SButton Temp;

    i = 0;
    do
    {
        TempButtonTriangle = *Pad[i].pTriangle;
        TempButtonCircle = *Pad[i].pCircle;
        TempButtonSquare = *Pad[i].pSquare;
        TempButtonX = *Pad[i].pX;

        Pad[i].Triangle = TempButtonTriangle;
        Pad[i].Circle = TempButtonCircle;
        Pad[i].Square = TempButtonSquare;
        Pad[i].X = TempButtonX;

        if (Cheat_FlipScreen)
        {
            Temp = Pad[i].Right;
            Pad[i].Right = Pad[i].Left;
            Pad[i].Left = Temp;
        }

        i++;
    } while (i < 2);
}
