/**
* Light shading data & function
*/

#ifndef VINBRDF_SHADER_UTILS
#define VINBRDF_SHADER_UTILS

struct BRDFData {
    float roughness4;
    float roughnessPlusOne2By8;
};

void InitializeBRDFData(InputData inputData, SurfaceData surfaceData, out BRDFData brdfData) {
    brdfData.roughness4 = pow(max(surfaceData.roughness, 0.1), 4.0);
    brdfData.roughnessPlusOne2By8 = pow(max(surfaceData.roughness, 0.1) + 1.0, 2.0) / 8.0;
}

#endif