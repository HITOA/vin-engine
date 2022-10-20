#version 330 core

#include "vin.glsl"

VIN_POSITION vec3 _position;
VIN_NORMAL vec3 _normal;
VIN_TEXCOORD0 vec2 _texcoord0;

out vec3 normal;
out vec2 texcoord0;
out vec3 position;
out vec4 positionLightspace;

void main()
{
    gl_Position = ((vin_matrix_mvp) * vec4(_position, 1.0));
    normal = (vin_matrix_model * vec4(_normal, 0.0)).xyz;
    texcoord0 = _texcoord0;
    position = _position;
    positionLightspace = vin_matrix_lightspace * vec4(_position, 1.0);
}
