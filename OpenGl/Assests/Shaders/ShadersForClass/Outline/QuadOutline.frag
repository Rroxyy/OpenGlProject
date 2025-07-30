#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec3 defaultColor;
uniform sampler2D sceneTex;
uniform vec2 texelSize;

void main()
{
    vec4 rawColor=texture(sceneTex,TexCoords);
    float total=0;
    for (int dx = -1; dx <= 1; ++dx)
        for (int dy = -1; dy <= 1; ++dy) {
            vec2 offset = vec2(dx, dy) * texelSize;
            total += texture(sceneTex, TexCoords + offset).r;
    }


    total=step(0.9,total);

    //FragColor=rawColor;
    FragColor = vec4( defaultColor * total, 1.0); // 只有边缘区域画颜色
}
