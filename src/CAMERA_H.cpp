// CAMERA_H.cpp -- Reconstructed translation unit
// Original: CAMERA.H
// Functions: 3 reconstructed entries
// Address range: 0x8007E7E0 - 0x8007E87C
//
// Compiled with cc1plus (GCC 2.91.66-psx). Compiler auto-mangles symbol names.


#define CAMERA_HEADER_EMISSION_LAYOUT
#include "CAMERA.h"
#undef CAMERA_HEADER_EMISSION_LAYOUT

extern "C" int __vt_t9AnimParam1Z5CQuat;
extern "C" int CQuatBaseVtable asm("DAT_800dbbb0");

extern "C" void CameraH_DestroyAnimParam(t9AnimParam1Z5CQuat *, int)
    asm("__DOT__t9AnimParam1Z5CQuat");
extern "C" void CameraH_DestroyQuat(CQuat *, int)
    asm("__DOT__5CQuat_8007E81C");
static void CameraClassDestructor(CClass *, int)
    asm("__DOT__6CClass_8007E84C");

/**
 * Destroy CAMERA.H's instantiated quaternion parameter.
 *
 * Address: 0x8007E7E0
 * Mangled: _._t9AnimParam1Z5CQuat
 */
void CameraH_DestroyAnimParam(t9AnimParam1Z5CQuat *_this,
                                          int __in_chrg)
{
    _this->CClass.vtable = &__vt_t9AnimParam1Z5CQuat;
    _this->mParam.CClass.vtable = &CQuatBaseVtable;
    _this->CClass.vtable = &CQuatBaseVtable;
    if (__in_chrg & 1)
        CClass::operator delete(_this);
}

/**
 * Destroy QUAT.H's camera-local quaternion instance.
 *
 * Address: 0x8007E81C
 * Mangled: _._5CQuat
 */
void CameraH_DestroyQuat(CQuat *_this, int __in_chrg)
{
    _this->CClass.vtable = &CQuatBaseVtable;
    if (__in_chrg & 1)
        CClass::operator delete(_this);
}

/**
 * Destroy the private CClass base instantiated by `camera.obj`.
 *
 * Address: 0x8007E84C
 * Mangled: _._6CClass
 */
static void CameraClassDestructor(CClass *_this, int __in_chrg)
{
    _this->vtable = &CQuatBaseVtable;
    if (__in_chrg & 1)
        CClass::operator delete(_this);
}

/** Private three-word CClass vtable image emitted by CAMERA.H. */
struct CameraClassVtable {
    unsigned int reserved[2];
    void (*destroy)(CClass *, int);
};

extern "C" {
/** Keep the destructor and its private base vtable in one emission shard. */
static CameraClassVtable CameraClassVtableImage
    __attribute__((section(".sdata"), aligned(4))) = {
        {0, 0}, &CameraClassDestructor,
    };
}
