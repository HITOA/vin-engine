#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 vin_matrix_lightspace;

void main()
{
    gl_Position = vin_matrix_lightspace * vec4(position, 1.0);
} 