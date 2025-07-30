#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec3 defaultColor;

void main()
{
    FragColor = vec4( defaultColor , 1.0); // 只有边缘区域画颜色
}
