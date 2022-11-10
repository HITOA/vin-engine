/**
* Surface data
*/

#ifndef VINSURFACE_SHADER_UTILS
#define VINSURFACE_SHADER_UTILS

struct SurfaceData {
    vec3 albedo;
    float metallic;
    float roughness;

    float alpha;
};

#endif