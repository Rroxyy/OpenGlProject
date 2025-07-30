#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec3 defaultColor;
uniform sampler2D finalTex;

void main()
{
    vec4 finalColor=texture(finalTex,TexCoords);
    FragColor = finalColor ;// 只有边缘区域画颜色
}
