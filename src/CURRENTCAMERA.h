#ifndef CURRENTCAMERA_INC_H
#define CURRENTCAMERA_INC_H

struct SCamera;

/**
 * Render camera currently selected by M3D. The owner stores this pointer in
 * small data; consumers retain their target-recorded absolute address view.
 */
extern SCamera *pCurrentCamera
    __attribute__((section(".data")));

#endif // CURRENTCAMERA_INC_H
