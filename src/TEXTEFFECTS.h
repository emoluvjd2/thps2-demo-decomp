// Shared text-effect layout and callable C++ interface.
#ifndef TEXTEFFECTS_INC_H
#define TEXTEFFECTS_INC_H

#include "globals.h"

struct Font;
struct Sprite2;

#include "TEXTEFFECT.h"

/** Per-character timing and blood-sprite motion recovered from MAIN.SYM. */
struct myTextEffectInfo {
    short waitTime;
    short bloodOffsetX;
    short bloodOffsetY;
    short bloodRotation;
};

/** Falling-character text animation and its shared blood sprite. */
struct FallingTextEffect {
    enum {
        CharacterCount = 32,
        CharacterMask = CharacterCount - 1,
        FixedPointShift = 12
    };

    TextEffect TextEffect;
    myTextEffectInfo mTextEffectInfo[CharacterCount];
    int mOffscreenY;
    int mCharCount;
    int mScale;
    int mFallingCount;
    int mBloody;
    int mFrameSize;

    static Sprite2 *mpBloodSprite;

    FallingTextEffect();
    ~FallingTextEffect();

    static void FreeSprite();
    static void InitSprite();

    void PostEffect(int oldX, int oldY, int screenX, int screenY);
    void SetBloodiness(int bloody);
    void bind(Font *pFont);
    void drawBloodyFrame(int x, int y, int width);
    void getColor(unsigned char *pR, unsigned char *pG,
                  unsigned char *pB);
    void getScreenPos(int *pX, int *pY);
    int handleComplete();
    void init();
    void setScale(int scale);
    void update();
};

/** Scale and color animation used for trick and bail text. */
struct TrickTextEffect {
    enum State {
        Idle = 0,
        Growing,
        Settling,
        Holding,
        Shrinking,
        Bailing,
        BailComplete,
        Complete
    };

    struct TextEffect TextEffect;
    int mTrickState;
    int mTrickTime;
    int mScale;
    int mBailStartSize;

    TrickTextEffect();
    ~TrickTextEffect();
    void Bail();
    void bind(Font *);
    void getColor(unsigned char * pR, unsigned char * pG, unsigned char * pB);
    int handleComplete();
    void init();
    void update();
};

/**
 * Typed entry points for owners that must spell GCC 2.x lifecycle flags.
 * Ordinary objects should use normal construction and destructor syntax.
 */
extern "C" FallingTextEffect *FallingTextEffect_ConstructAbi(
    FallingTextEffect *effect) asm("__17FallingTextEffect");
extern "C" TrickTextEffect *TrickTextEffect_ConstructAbi(
    TrickTextEffect *effect) asm("__15TrickTextEffect");

typedef char TextEffect_size_must_be_8[
    (sizeof(TextEffect) == 8) ? 1 : -1];
typedef char myTextEffectInfo_size_must_be_8[
    (sizeof(myTextEffectInfo) == 8) ? 1 : -1];
typedef char FallingTextEffect_size_must_be_288[
    (sizeof(FallingTextEffect) == 288) ? 1 : -1];

#endif // TEXTEFFECTS_INC_H

#ifdef TEXTEFFECTS_DECLARE_BASE_DESTRUCTOR_ABI
#if !defined(TEXTEFFECTS_BASE_LIFECYCLE_TYPE) || \
    !defined(TEXTEFFECTS_BASE_DESTRUCTOR_NAME)
#error "Select the TextEffect base destructor receiver and source name"
#endif

/** Destroy the embedded TextEffect base with an explicit in-charge flag. */
void TEXTEFFECTS_BASE_DESTRUCTOR_NAME(
    TEXTEFFECTS_BASE_LIFECYCLE_TYPE *effect, int inCharge)
    asm("_._10TextEffect");

#undef TEXTEFFECTS_BASE_DESTRUCTOR_NAME
#undef TEXTEFFECTS_BASE_LIFECYCLE_TYPE
#undef TEXTEFFECTS_DECLARE_BASE_DESTRUCTOR_ABI
#endif
