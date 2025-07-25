#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec3 defaultColor;
uniform sampler2D sceneTex;
uniform vec2 texelSize;

void main()
{
    float center = texture(sceneTex, TexCoords).r; // 白 or 黑
    float mask3 = textureLod(sceneTex, TexCoords, 3.0).r;
    float edge = 0.0;
    for (int dx = -2; dx <= 2; ++dx)
        for (int dy = -2; dy <= 2; ++dy) {
            vec2 offset = vec2(dx, dy) * texelSize;
            float sample = texture(sceneTex, TexCoords + offset).r;
            edge += abs(center - sample);
    }


    // 若周围存在不同像素，则为边缘
    float isEdge = step(0.001, edge); // 只要有一个不同就会变为1.0

    FragColor = vec4( defaultColor * isEdge, 1.0); // 只有边缘区域画颜色
}
