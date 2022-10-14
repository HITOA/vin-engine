#version 330 core

out vec4 FragColor;

uniform int texSamples;
uniform sampler2DMS srcTexture;

void main()
{
    vec4 color = vec4(0.0);

    for (int i = 0; i < texSamples; i++)
        color += texelFetch(srcTexture, ivec2(gl_FragCoord.xy), i);

    FragColor = (color / float(texSamples));
}