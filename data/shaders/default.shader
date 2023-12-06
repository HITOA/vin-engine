SOMESTRINGFIELD = "Some String"
SOMEBLOCKFIELD = {
    SomeFieldInABlock = ("An", "array", "of", 5.0, "value")
}

NAME = "Default lit"
PROPERTY = {}

GLSLVERTEXBEGIN

    #include <vin.glsl>

    VIN_POSITION vec3 position;
    VIN_NORMAL vec3 normal;

    layout(location = 0) out vec3 out_normal;

    void main() {
        gl_Position = VIN_MATRIX_MVP * vec4(position, 1.0);
        out_normal = normal;
    }

GLSLVERTEXEND

GLSLFRAGMENTBEGIN

    layout(location = 0) out vec4 outColor;

    layout(location = 0) in vec3 normal;

    void main() {
        vec3 light = vec3(dot(normal, normalize(vec3(0.3, 0.6, 0.3))));
        outColor = vec4(light, 1.0);
    }

GLSLFRAGMENTEND