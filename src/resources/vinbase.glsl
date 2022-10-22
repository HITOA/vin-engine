#ifndef VINPBR_SHADER_UTILS
#define VINPBR_SHADER_UTILS

#define SHADOWMAP_SAMPLE_COUNT 16

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

float Random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

float SampleShadowMap(vec4 lightspacePosition, vec3 worldspacePosition) {
    vec3 projCoords = lightspacePosition.xyz / lightspacePosition.w;
    projCoords = projCoords * 0.5 + 0.5;

    float currentDepth = projCoords.z; 
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(_ShadowMap, 0);
    const float radius = 4;

    for (int i = 0; i < SHADOWMAP_SAMPLE_COUNT; ++i) {
        vec2 offset = vec2(Random(floor(worldspacePosition.xyz * 1000), i), Random(floor(worldspacePosition.xyz * 1000), i + SHADOWMAP_SAMPLE_COUNT));

        float pcfDepth = texture(_ShadowMap, projCoords.xy + offset * radius * texelSize).r;
        shadow += currentDepth - _ShadowBias > pcfDepth ? 0.0 : 1.0;
    }

    return shadow / SHADOWMAP_SAMPLE_COUNT;
}

#endif //VINPBR_SHADER_UTILS