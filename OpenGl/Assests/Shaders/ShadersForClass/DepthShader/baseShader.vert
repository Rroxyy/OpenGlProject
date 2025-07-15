#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;     
layout (location = 4) in vec3 aBitangent;   


out vec2 TexCoords;
out mat3 TBN; 
out vec3 positionWS;
out vec3 normal;


void main()
{
   TexCoords = aTexCoords;

    vec3 T = normalize(mat3(model) * aTangent);
    vec3 B = normalize(mat3(model) * aBitangent);
    vec3 N = normalize(mat3(model) * aNormal);
    TBN = mat3(T, B, N);  // Tangent, Bitangent, Normal 

    positionWS = vec3(model * vec4(aPos, 1.0));

    normal=mat3(transpose(inverse(model))) * aNormal;  


    gl_Position = projection * view * vec4(positionWS, 1.0);

 }
