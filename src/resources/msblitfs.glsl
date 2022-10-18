#version 330 core

out vec4 fragcolor;

uniform sampler2DMS _MainTex;
uniform int _MainTexSample;

void main()
{
    vec4 color = vec4(0.0);

    for (int i = 0; i < _MainTexSample; i++)
        color += texelFetch(_MainTex, ivec2(gl_FragCoord.xy), i);

    fragcolor = (color / float(_MainTexSample));
}