#version 410 core

#include "vinbase.glsl"

out vec4 fragcolor;

in struct FsInput {
    vec4 positionWS; //World space position
    vec4 positionLS; //Light space position
    mat3 TBN;
    vec2 uv0;
    vec3 cameraPosition;
} fsinput;

uniform sampler2D _MainTex;
uniform sampler2D _NormalTex;
uniform sampler2D _MetallicRoughnessTex;

uniform float _AlphaCutoff;

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
} 

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = 3.14159265359 * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 GetNormal() {
    vec3 normal = texture(_NormalTex, fsinput.uv0).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(fsinput.TBN * normal);
    return normal;
}

void main()
{
    vec4 tex = texture(_MainTex, fsinput.uv0);

    if (tex.a < _AlphaCutoff)
        discard;

    vec3 albedo = pow(vec3(tex.rgb), vec3(2.2));
    vec3 N = GetNormal();
    vec3 V = normalize(fsinput.cameraPosition - fsinput.positionWS.xyz);

    vec3 mr = texture(_MetallicRoughnessTex, fsinput.uv0).xyz;

    float metallic = mr.r;
    float roughness = mr.g;

    float shadow = SampleShadowMap(fsinput.positionLS, fsinput.positionWS.xyz);

    vec3 H = normalize(V + _MainLight.direction);
    
    vec3 radiance = _MainLight.color.rgb * _MainLight.intensity * max(shadow, 0.01);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    float NDF = DistributionGGX(N, H, roughness);       
    float G = GeometrySmith(N, V, _MainLight.direction, roughness);  

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, _MainLight.direction), 0.0)  + 0.0001;
    vec3 specular = numerator / denominator;  
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    
    kD *= 1.0 - metallic;	

    float NdotL = max(dot(N, _MainLight.direction), 0.0);        
    vec3 Lo = (kD * albedo / 3.14159265359 + specular) * radiance * NdotL;

    vec3 ambient = vec3(0.03) * albedo;
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 
    
    fragcolor = vec4(color.xyz, tex.w);
}