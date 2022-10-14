#version 330 core

#define POSITION_LOCATION 0
#define NORMAL_LOCATION 1
#define TANGENT_LOCATION 2
#define COLOR_LOCATION 3

#define TEXCOORD0_LOCATION 4
#define TEXCOORD1_LOCATION 5
#define TEXCOORD2_LOCATION 6
#define TEXCOORD3_LOCATION 7
#define TEXCOORD4_LOCATION 8
#define TEXCOORD5_LOCATION 9
#define TEXCOORD6_LOCATION 10
#define TEXCOORD7_LOCATION 11

#define BLENDWEIGHT_LOCATION 12
#define BLENDINDICES_LOCATION 13

layout (location = POSITION_LOCATION) in vec3 aPos;
layout (location = NORMAL_LOCATION) in vec3 aNormal;
layout (location = TEXCOORD0_LOCATION) in vec2 aTexCoord;

out vec3 normal;
out vec2 TexCoord;
out vec3 pos;

uniform mat4 vin_matrix_mvp;
void main()
{
    gl_Position = (vin_matrix_mvp * vec4(aPos.x, aPos.y, aPos.z, 1.0));
    normal = aNormal;
    TexCoord = aTexCoord;
    pos = aPos;
}
