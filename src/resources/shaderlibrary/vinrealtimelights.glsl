/**
* Light shading data & function
*/

#ifndef VINREALTIMELIGHT_SHADER_UTILS
#define VINREALTIMELIGHT_SHADER_UTILS

#define MAX_ADDITIONAL_LIGHTS 8

struct Light {
    vec3 position;
    vec3 direction;
    vec3 color;
    float range;
    vec2 spotCosOuterInnerCone; // x = outer, y = inner
    vec3 scale; // Area light
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
uniform vec3 _AdditionalLightsScale[MAX_ADDITIONAL_LIGHTS];

Light GetMainLight() {
    Light light;

    light.position = vec3(0, 0, 0);
    light.direction = normalize(_MainLightDirection);
    light.color = _MainLightColor;
    light.range = 0;
    light.spotCosOuterInnerCone = vec2(-2, 1);
    light.scale = vec3(0, 0, 0);

    return light;
}

Light GetAdditionalLight(int index) {
    Light light;

    light.position = _AdditionalLightsPosition[index];
    light.direction = normalize(_AdditionalLightsDirection[index]);
    light.color = _AdditionalLightsColor[index];
    light.range = _AdditionalLightsAttribute[index].z;
    light.spotCosOuterInnerCone = _AdditionalLightsAttribute[index].xy;
    light.scale = _AdditionalLightsScale[index];

    return light;
}

int GetAdditionalLightCount() {
    return _AdditionalLightsCount;
}

#endif
