/**
* Shadow
*/

#ifndef VINSHADOWS_SHADER_UTILS
#define VINSHADOWS_SHADER_UTILS

uniform sampler2DShadow _ShadowMap;

float SampleShadowMap(vec4 shadowCoord) {
    vec3 projCoords = shadowCoord.xyz / shadowCoord.w;
    projCoords = projCoords * 0.5 + 0.5;
    return texture(_ShadowMap, projCoords);
}

#endif