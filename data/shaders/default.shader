NAME = "Default lit"
PROPERTY = {
    Color = ("color", "color")
}

GLSLVERTEXBEGIN

    #include <vin.glsl>

    VIN_POSITION vec3 _position;
    VIN_NORMAL vec3 _normal;
    VIN_TEXCOORD0 vec2 _texcoord0;

    layout(location = 0) out struct FsInput {
        vec3 normalOS;
        vec2 texcoord0;
    } fsInput;

    void main() {
        fsInput.normalOS = _normal;
        fsInput.texcoord0 = _texcoord0;

        gl_Position = VIN_MATRIX_MVP * vec4(_position, 1.0);
    }

GLSLVERTEXEND

GLSLFRAGMENTBEGIN

    layout(location = 0) in struct FsInput {
        vec3 normalOS;
        vec2 texcoord0;
    } fsInput;

    layout(location = 0) out vec4 outColor;

    uniform MaterialProperty {
        vec4 colorA;
        vec4 colorB;
    };

    uniform sampler albedoSampler;
    uniform texture2D albedo;

    void main() {
        vec3 diff = vec3(dot(fsInput.normalOS, normalize(vec3(0.3, 0.6, 0.3))));
        outColor = vec4(diff * colorA.xyz * colorB.xyz * texture(sampler2D(albedo, albedoSampler), fsInput.texcoord0).xyz, 1.0);
    }

GLSLFRAGMENTEND