#version 330 core

out vec4 fragcolor;

in vec3 normal;
in vec2 texcoord0;
in vec3 position;

uniform sampler2D _MainTex;
uniform vec2 _MainTexTiling;

void main()
{
    vec3 lightPos = vec3(100, 100, 100);
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - position);
    float diff = max(dot(norm, lightDir), 0.2);
    vec3 diffcolor = diff * vec3(0.85, 0.85, 1.0f);
    
    fragcolor = vec4(diffcolor * texture(_MainTex, texcoord0).xyz, 1.0f);
}