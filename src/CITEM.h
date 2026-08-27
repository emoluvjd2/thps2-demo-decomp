#ifndef CITEM_INC_H
#define CITEM_INC_H

struct SLight;

/**
 * Common renderable gameplay-item base.
 *
 * The complete 76-byte layout and natural method surface are owned by OB.cpp.
 * Include this header after the shared CVector and CSVector definitions.
 */
class CItem
{
public:
    unsigned short mFlags;
    unsigned short mInquiry;
    CVector mPos;
    CSVector mAngles;
    unsigned short mModel;
    char mDummyFrame;
    unsigned char mTintIndex;
    unsigned char mDummyAnim;
    unsigned char mRegion;
    CItem *mNextItem;
    unsigned int mRGB;
    CSVector mScale;
    unsigned short _padScale;
    unsigned int mTRN;
    CItem *mPreviousItem;
    unsigned int mStoredRGB;
    unsigned char mPredatorEffectType;
    unsigned char mPredatorEffectFade;
    unsigned short mType;
    SLight *mpLight;
    unsigned short mFlashTimer;
    short mFlash_dR;
    short mFlash_dG;
    short mFlash_dB;
    void **_vf;

    /** Initialize an empty gameplay item. */
    CItem();
    /** Restore the base item dispatch table before destruction. */
    ~CItem();
    /** Start an RGB flash for the requested duration. */
    void Flash(int frames, unsigned char r, unsigned char g,
               unsigned char b);
    /** Advance or finish the active RGB flash. */
    void FlashUpdate();
    /** Resolve and store the model region for an item asset. */
    void InitItem(char *name);
    /** Allocate and zero-fill gameplay-item storage. */
    static void *operator new(unsigned int size);
    /** Release gameplay-item storage. */
    static void operator delete(void *storage);
};

typedef char CItem_size_must_be_76[(sizeof(CItem) == 76) ? 1 : -1];
typedef char CItem_vtable_offset_must_be_72[
    ((unsigned long)&(((CItem *)0)->_vf) == 72) ? 1 : -1];

#endif // CITEM_INC_H
