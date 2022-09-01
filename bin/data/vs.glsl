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
layout (location = COLOR_LOCATION) in vec3 aColor;
layout (location = TEXCOORD0_LOCATION) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 randommat;
void main()
{
    gl_Position = (randommat * vec4(aPos.x, aPos.y, aPos.z, 1.0));
    ourColor = aColor;
    TexCoord = aTexCoord;
}
