#version 410 core

#include "vin.glsl"

VIN_POSITION vec3 _position;
VIN_NORMAL vec3 _normal;
VIN_TANGENT vec4 _tangent;
VIN_TEXCOORD0 vec2 _texcoord0;

out struct FsInput {
    vec4 positionWS; //World space position
    vec4 positionLS; //Light space position
    vec3 normalOS;
    mat3 TBN;
    vec2 uv0;
    vec3 cameraPosition;
} fsinput;

void main()
{
    fsinput.positionWS = vin_matrix_model * vec4(_position, 1.0);
    fsinput.positionLS = vin_matrix_lightspace * vec4(_position, 1.0);
    fsinput.normalOS = _normal;
    fsinput.uv0 = _texcoord0;

    fsinput.cameraPosition = _WorldSpaceCameraPosition;

    vec3 T = normalize((vin_matrix_model * vec4(_tangent.xyz, 0.0)).xyz);
    vec3 N = normalize((vin_matrix_model * vec4(_normal, 0.0)).xyz);
    vec3 B = normalize(cross(N, T)) * _tangent.w;

    fsinput.TBN = mat3(T, B, N);

    gl_Position = ((vin_matrix_mvp) * vec4(_position, 1.0));
}