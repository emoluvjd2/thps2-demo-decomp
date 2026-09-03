// Translation-unit declarations for CAR.cpp.
#ifndef CAR_INC_H
#define CAR_INC_H

#include "globals.h"
#include <STDDEF.H>
#include <LIBGTE.H>

struct CBody;
struct CBruce;
struct CCar;
struct SVideoInfo;

#define BITSTATE_DECLARE_BIT_COUNT
#include "BITSTATE.h"
#define VIDEOINFO_DECLARE_GVINFO1
#define VIDEOINFO_DECLARE_GVINFO2
#include "VIDEOINFO.h"
#define SOUNDLEVELS_DECLARE_SFXLEVEL
#include "SOUNDLEVELS.h"
#define BITSTATE_DECLARE_USE_CACHE_FOR_BITS
#include "BITSTATE.h"

#endif // CAR_INC_H
