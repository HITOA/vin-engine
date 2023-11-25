#version 450
#extension GL_GOOGLE_include_directive : require

#include <vin.glsl>

VIN_POSITION vec3 position;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = VIN_MATRIX_MVP * vec4(position, 1.0);
    fragColor = position;
}