#pragma once
#include <vector>
#include <glad/glad.h>



class gridMesh:public Mesh
{
public:
   
	gridMesh()
	{
        std::vector<Vertex> _vertices;
        std::vector<unsigned int> _indices;

        int gridSize = 10;     // 网格扩展范围：-10 到 +10
        float step = 1.0f;     // 每格间隔

        for (int i = -gridSize; i <= gridSize; ++i) {
            glm::vec3 a(i * step, 0.0f, -gridSize * step); // A
            glm::vec3 b(i * step, 0.0f, gridSize * step); // B
            glm::vec3 c(-gridSize * step, 0.0f, i * step); // C
            glm::vec3 d(gridSize * step, 0.0f, i * step); // D

            // 将每个点转成 Vertex 格式（可复用下面的创建函数）
            _vertices.push_back(createGridVertex(a));
            _vertices.push_back(createGridVertex(b));
            _vertices.push_back(createGridVertex(c));
            _vertices.push_back(createGridVertex(d));
        }

        // 每两个点组成一条线段
        for (unsigned int i = 0; i < _vertices.size(); i += 2) {
            _indices.push_back(i);
            _indices.push_back(i + 1);
        }
        this->vertices = std::move(_vertices);
        this->indices =std::move(_indices);
        this->PrimitiveType = GL_LINES;

        setupMesh();
	}

    Vertex createGridVertex(const glm::vec3& pos)
    {
        Vertex v;
        v.Position = pos;
        v.Normal = glm::vec3(0.0f, 1.0f, 0.0f); // 向上（Y+），用于调试也行
        v.TexCoords = glm::vec2(0.0f);          // 网格线不贴图，可为0
        v.Tangent = glm::vec3(1.0f, 0.0f, 0.0f);   // 默认右方向
        v.Bitangent = glm::vec3(0.0f, 0.0f, 1.0f); // 默认前方向

        // 骨骼初始化为无绑定
        for (int i = 0; i < MAX_BONE_INFLUENCE; ++i) {
            v.m_BoneIDs[i] = 0;
            v.m_Weights[i] = 0.0f;
        }

        return v;
    }

    
};
