#version 410 core

#include "vinrealtimelights.glsl"

out vec4 fragcolor;

in struct FsInput {
    vec4 positionWS; //World space position
    vec4 positionLS; //Light space position
    vec3 normalOS;
    mat3 TBN;
    vec2 uv0;
    vec3 cameraPosition;
} fsinput;

uniform sampler2D _MainTex;
uniform sampler2D _NormalTex;
uniform sampler2D _MetallicRoughnessTex;

uniform float _AlphaCutoff;

void main()
{
    vec4 tex = texture(_MainTex, fsinput.uv0);

    if (tex.a < _AlphaCutoff)
        discard;

    Light mainLight = GetMainLight();

    float diff = max(dot(fsinput.normalOS, mainLight.direction), 0.0);
    vec3 diffuse = diff * mainLight.color;

    vec3 color = tex.rgb * diffuse;
    
    InputData inputData;
    inputData.positionWS = fsinput.positionWS.xyz;

    for (int i = 0; i < GetAdditionalLightCount(); ++i) {
        Light additionalLight = GetAdditionalLight(i, inputData);

        float addLightDiff = max(dot(fsinput.normalOS, additionalLight.direction), 0.0);
        vec3 addLightDiffuse = vec3(addLightDiff * additionalLight.attenuation) * additionalLight.color;

        color += tex.rgb * addLightDiffuse;
    }
    
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 

    fragcolor = vec4(color, tex.a);
}