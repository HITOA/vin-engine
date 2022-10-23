#version 330 core

#include "vinpbr.glsl"

out vec4 fragcolor;

in vec3 normal;
in vec2 texcoord0;
in vec3 position;
in vec4 positionLightspace;

uniform sampler2D _MainTex;
uniform vec2 _MainTexTiling;

void main()
{
    vec3 norm = normalize(normal);
    vec3 ambient = 0.2 * _MainLight.color.xyz;
    float diff = dot(norm, _MainLight.direction);
    float shadow = SampleShadowMap(positionLightspace, position);
    vec4 color = texture(_MainTex, texcoord0);

    fragcolor = vec4((ambient + max(shadow * diff, 0.0)) * color.xyz, 1.0f);
}