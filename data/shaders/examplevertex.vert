#version 450
#extension GL_GOOGLE_include_directive : require

#include <vin.glsl>

VIN_POSITION vec3 position;
VIN_NORMAL vec3 normal;

layout(location = 0) out vec3 out_normal;

void main() {
    gl_Position = VIN_MATRIX_MVP * vec4(position, 1.0);
    out_normal = normal;
}