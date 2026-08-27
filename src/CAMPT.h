#ifndef CAMPT_INC_H
#define CAMPT_INC_H

struct CBruce;

/** Initialize the camera-point table for one level. */
void CamPt_Init(void);

/** Restore camera-point defaults and clear the active table. */
void CamPt_Cleanup(void);

/** Append one trigger node to the active camera-point table. */
void CamPt_New(int node);

/** Select the closest usable camera point for one skater. */
void CamPt_SetClosestCamera(CBruce *skater);

#endif // CAMPT_INC_H
