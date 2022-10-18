#version 330 core

#define POSITION_LOCATION 0

layout (location = POSITION_LOCATION) in vec3 position;

void main()
{
    gl_Position = vec4(position, 1.0f);
}