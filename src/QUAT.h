// Hand-maintained per-TU interface for QUAT.cpp.
// tools/gen_tu_headers.py is retained for bootstrapping and does not reproduce it.
#ifndef QUAT_INC_H
#define QUAT_INC_H

// Primitive types
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned char byte;
typedef int undefined;
typedef char undefined1;
typedef short undefined2;
typedef int undefined4;
typedef unsigned long ulong;
typedef unsigned char undefined3[3];

#include <STDDEF.H>
#include <LIBGTE.H>
#include <LIBGPU.H>

#include "globals.h"

// CClass — Base class with vtable pointer

/** Fixed-point quaternion with the original CClass vtable prefix. */
struct CQuat {
    struct CClass CClass;
    int x;
    int y;
    int z;
    int w;
    CQuat();
    CQuat(const CQuat &_ctor_arg);
    ~CQuat();
    void Unit();
};

#include "QUAT_API.h"

#endif // QUAT_INC_H

#ifdef QUAT_DECLARE_VTABLE_ABI
/** One-word GP-relative address view of CQuat's virtual table. */
extern int __vt_5CQuat;
#undef QUAT_DECLARE_VTABLE_ABI
#endif
