#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 positionWS;
in vec3 normal;
in mat3 TBN;

uniform vec3 lightDir;
uniform vec3 lightColor;

uniform vec3 cameraPos;

uniform int useBaseTex;
uniform int useNormalTex;
uniform int useSpecGlossTex;
uniform int useDispTex;

uniform vec3 defaultColor;

uniform float dispTexScale;

uniform float ambientStrength;

uniform float specularStrength = 0.5;
uniform float shininess = 32.0;

uniform sampler2D texture_base;
uniform sampler2D texture_normal;
uniform sampler2D texture_specGloss;
uniform sampler2D texture_disp;

void main()
{
    vec2 uv = TexCoords;

    // --- Parallax Mapping 简易版 ---
    if (useDispTex != 0)
    {
        float height = texture(texture_disp, uv).r; // 置换贴图一般是灰度图
        vec3 viewDirTS = normalize(TBN * (cameraPos - positionWS)); // 视线方向到切线空间
        uv += viewDirTS.xy * (height * dispTexScale);
    }

    // --- Base Color ---
    vec3 baseColor = mix(defaultColor, texture(texture_base, uv).rgb, useBaseTex);

    // --- Normal Mapping ---
    vec3 normalWS = normal;
    if (useNormalTex != 0)
    {
        vec3 tangentNormal = texture(texture_normal, uv).rgb;
        tangentNormal = normalize(tangentNormal * 2.0 - 1.0);
        normalWS = normalize(TBN * tangentNormal);
    }

    // --- Ambient ---
    vec3 ambient = ambientStrength * lightColor;

    // --- Diffuse ---
    float diff = max(dot(normalWS, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // --- Specular + Glossiness ---
    float specularFactor = specularStrength;
    float glossiness = shininess;
    if (useSpecGlossTex != 0)
    {
        vec4 specGloss = texture(texture_specGloss, uv);
        specularFactor = specGloss.r; // 高光强度（可以用 R 或者 RGB）
        glossiness = mix(1.0, 256.0, specGloss.a); // Glossiness (Alpha 通道控制光滑度)
    }

    vec3 viewDir = normalize(cameraPos - positionWS);
    vec3 halfViewLight = normalize(viewDir + lightDir);
    float hdn = max(dot(halfViewLight, normalWS), 0.0);
    float spec = pow(hdn, glossiness);
    vec3 specular = specularFactor * spec * lightColor;

    // --- Final Color ---
    vec3 finalColor = (ambient + diffuse + specular) * baseColor;
    FragColor = vec4(finalColor, 1.0);
}
