#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec2 TexCoord;
in vec3 pos;

uniform sampler2D ourTexture;
uniform vec3 color;

void main()
{
    vec3 lightPos = vec3(100, 100, 100);
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - pos);
    float diff = max(dot(norm, lightDir), 0.0);
    //FragColor = vec4(ourColor * color, 1.0f);
    FragColor = vec4(diff * texture(ourTexture, TexCoord * 100).xyz, 1.0);
}