#version 450
#extension GL_GOOGLE_include_directive : require

#include <vin.glsl>

VIN_POSITION vec3 a_position;

void main() {
    gl_Position = vec4(a_position, 1.0);
}