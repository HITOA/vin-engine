#version 330 core

#define POSITION_LOCATION 0

layout (location = POSITION_LOCATION) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0f);
}
