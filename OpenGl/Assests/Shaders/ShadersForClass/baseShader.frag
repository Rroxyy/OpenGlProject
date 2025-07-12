#version 330 core
out vec4 FragColor;


uniform vec3 defaultColor;


void main()
{
    FragColor = vec4(defaultColor, 1.0);
}
