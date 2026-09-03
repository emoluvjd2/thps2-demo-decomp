#ifndef VIDEOINFO_TYPES_INC_H
#define VIDEOINFO_TYPES_INC_H

struct SVideoInfo;

#endif // VIDEOINFO_TYPES_INC_H

/*
 * VIDEO owns the two replay descriptors. Each translation unit opts into only
 * the descriptors it uses so its owner-local SVideoInfo layout continues to
 * determine the target's import or small-BSS record. This declaration header
 * is intentionally re-entrant because existing owner headers introduce the
 * two descriptors at different points.
 */
#if defined(VIDEOINFO_DECLARE_GVINFO1) && \
    !defined(VIDEOINFO_GVINFO1_DECLARED)
#define VIDEOINFO_GVINFO1_DECLARED
/** Primary replay-stream descriptor physically owned by VIDEO.cpp. */
extern SVideoInfo GVInfo1;
#endif
#undef VIDEOINFO_DECLARE_GVINFO1

#if defined(VIDEOINFO_DECLARE_GVINFO2) && \
    !defined(VIDEOINFO_GVINFO2_DECLARED)
#define VIDEOINFO_GVINFO2_DECLARED
/** Secondary replay-stream descriptor physically owned by VIDEO.cpp. */
extern SVideoInfo GVInfo2;
#endif
#undef VIDEOINFO_DECLARE_GVINFO2
