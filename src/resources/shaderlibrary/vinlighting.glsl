/**
* Light shading data & function
*/

#ifndef VINLIGHTING_SHADER_UTILS
#define VINLIGHTING_SHADER_UTILS

#include "vinrealtimelights.glsl"
#include "vinsurface.glsl"

vec3 CalculateBlinnPhong(Light light, InputData inputData, SurfaceData surfaceData) {
    float NdotL = max(dot(inputData.normalWS, light.direction), 0.0);

    vec3 diffuse = light.color * NdotL * light.attenuation;

    vec3 H = normalize(light.direction + inputData.viewDirectionWS);
    float NdotH = dot(inputData.normalWS, H);
    
    vec3 specular = pow(max(NdotH, 0.0), exp2(10 * (1 - surfaceData.roughness) + 1)) * light.color * light.attenuation * NdotL;

    return (diffuse + specular) * surfaceData.albedo * surfaceData.alpha; //Alpha Premultiplied
}

#endif