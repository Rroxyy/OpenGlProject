#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // 包含所有OpenGL相关的类型声明

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

#include <string>
#include <vector>
using namespace std;

#define MAX_BONE_INFLUENCE 4 // 每个顶点最多受到4个骨骼的影响

// 顶点结构体定义
struct Vertex {
    glm::vec3 Position;             // 顶点位置坐标
    glm::vec3 Normal;               // 顶点法线方向，用于光照计算
    glm::vec2 TexCoords;            // 顶点纹理坐标，用于贴图采样
    glm::vec3 Tangent;              // 顶点切线，用于法线贴图（normal mapping）
    glm::vec3 Bitangent;            // 顶点副切线，与法线、切线一起构成TBN矩阵
    int m_BoneIDs[MAX_BONE_INFLUENCE];    // 影响该顶点的骨骼索引数组
    float m_Weights[MAX_BONE_INFLUENCE];  // 对应骨骼索引的权重值
};

// 网格类
class Mesh {
public:
    // 网格数据
    vector<Vertex>       vertices;   // 顶点数据列表
    vector<unsigned int> indices;    // 索引数组，用于按顺序绘制三角形
    unsigned int VAO;                // 顶点数组对象（VAO）

    // 构造函数：接收顶点、索引和纹理数据，并初始化网格
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices)
    {
        this->vertices = vertices;
        this->indices = indices;

        // 初始化所有缓冲对象和设置顶点属性
        setupMesh();
    }

    // 渲染网格
    void Draw(Shader& shader)
    {
        // 绑定相应的纹理到对应的纹理单元
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;

        // 绑定VAO并绘制网格（使用索引）
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // 重置为默认纹理单元
        glActiveTexture(GL_TEXTURE0);
    }

private:
    // 渲染所需的缓冲区对象
    unsigned int VBO, EBO;

    // 初始化所有缓冲区和设置顶点属性指针
    void setupMesh()
    {
        // 创建VAO、VBO、EBO对象
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // 绑定并传输顶点数据到VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // 使用结构体直接传递数据，因为结构体内存是顺序排列的
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        // 绑定并传输索引数据到EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // 设置各个顶点属性指针

        // 顶点位置属性（位置）
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // 顶点法线属性
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        // 顶点纹理坐标属性
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        // 顶点切线属性
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

        // 顶点副切线属性
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

        // 顶点骨骼ID（整数属性）
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

        // 顶点骨骼权重（浮点数属性）
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));

        // 解绑VAO，防止误操作
        glBindVertexArray(0);
    }
};

#endif
