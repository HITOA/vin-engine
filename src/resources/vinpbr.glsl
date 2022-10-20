#ifndef VINPBR_SHADER_UTILS
#define VINPBR_SHADER_UTILS

#define SOFT_SHADOW_SAMPLE_NUMBER 16

/* Lighting */

struct DirectionalLight {
    vec3 direction;
    vec4 color;
    float intensity;
};

struct AdditionalLight {
    int type;
};

uniform DirectionalLight _MainLight;

uniform AdditionalLight _AdditionalLight[16];
uniform int _AdditionalLightCount;

/* Shadow */

uniform sampler2D _ShadowMap;
uniform float _ShadowBias;
uniform int _SoftShadow;

vec3 Hash( uvec3 x )
{
    const uint k = 1103515245U;

    x = ((x>>8U)^x.yzx)*k;
    x = ((x>>8U)^x.yzx)*k;
    x = ((x>>8U)^x.yzx)*k;
    
    return vec3(x)*(1.0/float(0xffffffffU));
}

float SampleShadowMap(vec4 lightspacePosition) {
    vec3 projCoords = lightspacePosition.xyz / lightspacePosition.w;
    projCoords = projCoords * 0.5 + 0.5;

    float currentDepth = projCoords.z; 
    
    float shadow = 0.0;

    if (_SoftShadow == 1) {
        vec2 texelSize = 1.0 / textureSize(_ShadowMap, 0);

        for (int i = 0; i <= SOFT_SHADOW_SAMPLE_NUMBER; ++i) {
            shadow += currentDepth - _ShadowBias > texture(_ShadowMap, projCoords.xy + 
            Hash(uvec3(lightspacePosition.xz, i)).xy * texelSize * (i + 1)).r  ? 0.0 : 1.0;
        }

        shadow /= SOFT_SHADOW_SAMPLE_NUMBER;
    }else {
        float closestDepth = texture(_ShadowMap, projCoords.xy).r;   
        shadow = currentDepth - _ShadowBias > closestDepth  ? 0.0 : 1.0;
    }
    
    return shadow;
}

#endif //VINPBR_SHADER_UTILS