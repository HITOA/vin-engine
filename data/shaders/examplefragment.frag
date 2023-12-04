#version 450

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 normal;

void main() {
    vec3 light = vec3(dot(normal, normalize(vec3(0.3, 0.6, 0.3))));
    outColor = vec4(light, 1.0);
}