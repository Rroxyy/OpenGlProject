#pragma once
#include "mesh.h"


class QuadMesh:public  Mesh
{
public:
    QuadMesh()
    {
        std::vector<Vertex> _vertices;
        std::vector<unsigned int> _indices;

        // 六个顶点（两个三角形），带贴图坐标
        glm::vec2 positions[6] = {
            {-1.0f,  1.0f},   // 左上
            {-1.0f, -1.0f},   // 左下
            { 1.0f, -1.0f},   // 右下

            {-1.0f,  1.0f},   // 左上
            { 1.0f, -1.0f},   // 右下
            { 1.0f,  1.0f}    // 右上
        };

        glm::vec2 texCoords[6] = {
            {0.0f, 1.0f},
            {0.0f, 0.0f},
            {1.0f, 0.0f},

            {0.0f, 1.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f}
        };

        for (int i = 0; i < 6; ++i)
        {
            glm::vec3 pos(positions[i], 0.0f); // Z = 0
            Vertex v = createQuadVertex(pos, texCoords[i]);
            _vertices.push_back(v);
            _indices.push_back(i); // 每个顶点唯一，无复用
        }

        this->vertices = std::move(_vertices);
        this->indices = std::move(_indices);
        this->PrimitiveType = GL_TRIANGLES;

        setupMesh(); // 通用 VAO/VBO/EBO 设置函数
    }

    Vertex createQuadVertex(const glm::vec3& pos, const glm::vec2& uv)
    {
        Vertex v;
        v.Position = pos;
        v.Normal = glm::vec3(0.0f, 0.0f, 1.0f); // 面朝屏幕
        v.TexCoords = uv;
        v.Tangent = glm::vec3(1.0f, 0.0f, 0.0f);
        v.Bitangent = glm::vec3(0.0f, 1.0f, 0.0f);

        for (int i = 0; i < MAX_BONE_INFLUENCE; ++i) {
            v.m_BoneIDs[i] = 0;
            v.m_Weights[i] = 0.0f;
        }

        return v;
    }


private:
};
