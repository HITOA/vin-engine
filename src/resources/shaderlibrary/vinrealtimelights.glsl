/**
* Light shading data & function
*/

#ifndef VINREALTIMELIGHT_SHADER_UTILS
#define VINREALTIMELIGHT_SHADER_UTILS

#include "vininput.glsl"

#define MAX_ADDITIONAL_LIGHTS 8

struct Light {
    vec3 position;
    vec3 direction;
    vec3 color;
    float attenuation;
};

// Main light IS directional light
uniform vec3 _MainLightDirection;
uniform vec3 _MainLightColor;

// Additional light is either Directional, Point, Spot, or Area
uniform int _AdditionalLightsCount;

uniform vec3 _AdditionalLightsPosition[MAX_ADDITIONAL_LIGHTS];
uniform vec3 _AdditionalLightsDirection[MAX_ADDITIONAL_LIGHTS];
uniform vec3 _AdditionalLightsColor[MAX_ADDITIONAL_LIGHTS];
uniform vec3 _AdditionalLightsAttribute[MAX_ADDITIONAL_LIGHTS]; // x = spot cos outer cone, y = spot cos inner cone, z = range

Light GetMainLight() {
    Light light;

    light.position = vec3(0, 0, 0);
    light.direction = normalize(_MainLightDirection);
    light.color = _MainLightColor;
    light.attenuation = 1;

    return light;
}

Light GetAdditionalLight(int index, InputData inputData) {
    Light light;

    light.position = _AdditionalLightsPosition[index];
    light.direction = max(normalize(_AdditionalLightsDirection[index]), normalize(light.position - inputData.positionWS));
    light.color = _AdditionalLightsColor[index];

    float dst = length(inputData.positionWS - _AdditionalLightsPosition[index]);
    float theta = dot(normalize(_AdditionalLightsPosition[index] - inputData.positionWS), normalize(_AdditionalLightsDirection[index]));
    float epsilon = _AdditionalLightsAttribute[index].y - _AdditionalLightsAttribute[index].x;

    light.attenuation = smoothstep(0.0, 1.0, (theta - _AdditionalLightsAttribute[index].x) / epsilon) * 
    _AdditionalLightsAttribute[index].z / (dst * dst);

    return light;
}

int GetAdditionalLightCount() {
    return _AdditionalLightsCount;
}

#endif
