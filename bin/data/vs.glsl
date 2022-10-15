#version 330 core

#include "data/vin.glsl"

VIN_POSITION vec3 aPos;
VIN_NORMAL vec3 aNormal;
VIN_TEXCOORD0 vec2 aTexCoord;

out vec3 normal;
out vec2 TexCoord;
out vec3 pos;

void main()
{
    gl_Position = (vin_matrix_mvp * vec4(aPos.x, aPos.y, aPos.z, 1.0));
    normal = aNormal;
    TexCoord = aTexCoord;
    pos = aPos;
}
