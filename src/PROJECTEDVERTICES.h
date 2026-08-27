#ifndef PROJECTEDVERTICES_INC_H
#define PROJECTEDVERTICES_INC_H

struct SProjectedVertex;

/**
 * Shared transformed-vertex workspace pointer, physically owned and
 * initialized by M3D.cpp. Consumers form its target-recorded absolute address.
 */
extern "C" SProjectedVertex *ProjectedVertices
    __attribute__((section(".data")));

#endif // PROJECTEDVERTICES_INC_H
