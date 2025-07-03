#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 positionWS;
in vec3 normal;
in mat3 TBN;

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 lightColor;

uniform int useNormalTex;

uniform float ambientStrength;

uniform float specularStrength = 0.5;
uniform float shininess = 32.0;

uniform sampler2D texture_base;
uniform sampler2D texture_normal;

void main()
{
    vec3 lightDir = normalize(lightPos - positionWS);
    vec3 viewDir = normalize(cameraPos - positionWS);

    
    // 采样 normal map，映射到 [-1, 1]
    vec3 tangentNormal = texture(texture_normal, TexCoords).rgb;
    tangentNormal =  normalize(tangentNormal * 2.0 - 1.0);

    vec3 normalWS =mix(normal,normalize(TBN * tangentNormal),useNormalTex);

    //环境光
    vec3 ambient = ambientStrength * lightColor;

    // 漫反射
    float diff = max(dot(normalWS, lightDir), 0.0);
    vec3 diffuse=diff*lightColor;

    //镜面反射
    vec3 halfViewLight = normalize(viewDir + lightDir);
    float hdn = max(dot(halfViewLight, normalWS), 0.0);
    float spec = pow(hdn, shininess);
    vec3 specular = specularStrength * spec * lightColor;


    vec3 baseColor = texture(texture_base, TexCoords).rgb;
    vec3 finalColor = (ambient + diffuse + specular) * baseColor;

    FragColor = vec4(finalColor, 1.0);
}
