/**
* Light shading data & function
*/

#ifndef VINLIGHTING_SHADER_UTILS
#define VINLIGHTING_SHADER_UTILS

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062

#include "vinrealtimelights.glsl"
#include "vinsurface.glsl"
#include "vinbrdf.glsl"

vec3 CalculateBlinnPhong(Light light, InputData inputData, SurfaceData surfaceData) {
    float NdotL = max(dot(inputData.normalWS, light.direction), 0.0);

    vec3 diffuse = light.color * NdotL * light.attenuation;

    vec3 H = normalize(light.direction + inputData.viewDirectionWS);
    float NdotH = dot(inputData.normalWS, H);
    
    vec3 specular = pow(max(NdotH, 0.0), exp2(10 * (1 - surfaceData.roughness) + 1)) * light.color * light.attenuation * NdotL;

    return (diffuse + specular) * surfaceData.albedo * surfaceData.alpha; //Alpha Premultiplied
}

vec3 CalculatePBR(Light light, InputData inputData, SurfaceData surfaceData, BRDFData brdfData) {
    float NdotL = max(dot(inputData.normalWS, light.direction), 0.0);
    float NdotV = max(dot(inputData.normalWS, inputData.viewDirectionWS), 0.0);

    vec3 H = normalize(light.direction + inputData.viewDirectionWS);
    float NdotH = max(dot(inputData.normalWS, H), 0.0);
    float HdotV = max(dot(H, inputData.viewDirectionWS), 0.0);

    float NDF = brdfData.roughness4 / (PI * pow(pow(NdotH, 2) * (brdfData.roughness4 - 1.0) + 1.0, 2.0));
    float G = (NdotV / (NdotV * (1.0 - brdfData.roughnessPlusOne2By8) + brdfData.roughnessPlusOne2By8)) * 
                (NdotL / (NdotL * (1.0 - brdfData.roughnessPlusOne2By8) + brdfData.roughnessPlusOne2By8));

    vec3 F0 = mix(vec3(0.04), surfaceData.albedo, surfaceData.metallic);
    vec3 F = F0 + (1.0 - F0) * pow(clamp(1.0 - HdotV, 0.0, 1.0), 5.0);

    vec3 kD = (vec3(1.0) - F) * (1.0 - surfaceData.metallic);

    vec3 num = NDF * G * F;
    float denom = 4.0 * NdotV * NdotL + 0.0001;
    vec3 spec = num / denom;
    
    return (kD * surfaceData.albedo / PI + spec) * light.color * light.attenuation * NdotL;
}

#endif