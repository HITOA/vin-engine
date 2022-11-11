#version 410 core

#include "vinlighting.glsl"
#include "vinshadows.glsl"

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
    vec4 mr = texture(_MetallicRoughnessTex, fsinput.uv0);

    if (tex.a < _AlphaCutoff)
        discard;

    vec3 normal = texture(_NormalTex, fsinput.uv0).xyz;
    normal = normal * 2.0 - 1.0;
    normal = normalize(fsinput.TBN * normal);

    InputData inputData;
    inputData.positionWS = fsinput.positionWS.xyz;
    inputData.normalWS = fsinput.normalOS;
    inputData.viewDirectionWS = normalize(fsinput.cameraPosition.xyz - fsinput.positionWS.xyz);

    SurfaceData surfaceData;
    surfaceData.albedo = tex.rgb;
    surfaceData.roughness = mr.g;
    surfaceData.alpha = tex.a;

    vec3 color = CalculateBlinnPhong(GetMainLight(), inputData, surfaceData) * SampleShadowMapPCF(fsinput.positionLS);

    for (int i = 0; i < GetAdditionalLightCount(); ++i) {
        color += CalculateBlinnPhong(GetAdditionalLight(i, inputData), inputData, surfaceData);
    }
    
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 

    fragcolor = vec4(color, tex.a);
}