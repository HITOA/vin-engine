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
    float diff = max(dot(norm, lightDir), 0.0);
    
    fragcolor = vec4(diff * texture(_MainTex, texcoord0 * _MainTexTiling).xyz, 1.0f);
}