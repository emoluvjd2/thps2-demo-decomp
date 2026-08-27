/*
 * FRONT physically owns both audio-level words in small data. This header is
 * intentionally re-entrant so each translation unit can request either word
 * at its original declaration point and retain its target addressing view.
 */
#ifdef SOUNDLEVELS_OWNER_DECLARATIONS
#define SOUNDLEVELS_DECL_ATTR
#else
#define SOUNDLEVELS_DECL_ATTR __attribute__((section(".data")))
#endif

#ifdef SOUNDLEVELS_DECLARE_SFXLEVEL
/** Global sound-effect volume, physically owned by FRONT.cpp. */
extern "C" int SFXLEVEL SOUNDLEVELS_DECL_ATTR; // 0x800D8C00
#endif
#undef SOUNDLEVELS_DECLARE_SFXLEVEL

#ifdef SOUNDLEVELS_DECLARE_XALEVEL
/** Global streamed-music volume, physically owned by FRONT.cpp. */
extern "C" int XALEVEL SOUNDLEVELS_DECL_ATTR; // 0x800D8C04
#endif
#undef SOUNDLEVELS_DECLARE_XALEVEL

#ifdef SOUNDLEVELS_DECLARE_MIDILEVEL
/** Global MIDI volume, physically owned by FRONT.cpp. */
extern "C" int MIDILEVEL SOUNDLEVELS_DECL_ATTR; // 0x800D8BFC
#endif
#undef SOUNDLEVELS_DECLARE_MIDILEVEL

#undef SOUNDLEVELS_DECL_ATTR
