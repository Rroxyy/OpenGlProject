#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D colorTex;
uniform sampler2D rawOutlineTex;
uniform sampler2D dilatedTex;


uniform vec3 defaultColor;

void main()
{
    vec3 rawColor=texture(colorTex,TexCoords).xyz;
    vec3 rawOutlineColor=texture(rawOutlineTex,TexCoords).xyz;
    vec3 dilatedColor=texture(dilatedTex,TexCoords).xyz;


    float isEdge=dilatedColor.x-rawOutlineColor.x;
    isEdge=step(0.1,isEdge);

    vec3 outlineColor=vec3(1)*isEdge;

    rawColor+=outlineColor;

    FragColor = vec4( rawColor , 1.0); // 只有边缘区域画颜色
}
