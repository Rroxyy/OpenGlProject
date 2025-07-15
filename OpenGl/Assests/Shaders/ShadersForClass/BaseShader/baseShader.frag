#version 330 core
out vec4 FragColor;

uniform vec3 defaultColor;

in vec2 TexCoords;
in mat3 TBN;
in vec3 positionWS;
in vec3 normal;


void main()
{
    FragColor = vec4(defaultColor, 1.0);
}
