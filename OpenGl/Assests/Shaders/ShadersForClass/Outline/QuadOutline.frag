#version 330 core
out vec4 FragColor;

uniform vec3 defaultColor;
in vec2 TexCoords;

uniform sampler2D sceneColor;
uniform vec2 texelSize;

void main()
{
    vec3 col = texture(sceneColor, TexCoords).rgb;
    vec3 colRight = texture(sceneColor, TexCoords + vec2(texelSize.x, 0.0)).rgb;
    vec3 colUp = texture(sceneColor, TexCoords + vec2(0.0, texelSize.y)).rgb;

    float diff = length(col - colRight) + length(col - colUp);

    float edge = step(0.1, diff);  // 超过阈值算边界
    FragColor = vec4(vec3(edge), 1.0);  // 白色边框
}
