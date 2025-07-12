#version 330 core
out vec4 FragColor;


uniform vec3 defaultColor;

in vec2 TexCoords;

uniform sampler2D texture_base;

void main()
{
    vec3 baseTexColor=texture(texture_base, TexCoords).rgb;
    FragColor = vec4(mix(defaultColor,baseTexColor,0.5), 1.0);
}
