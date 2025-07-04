#pragma once
#include <vector>
#include <glad/glad.h>



class gridMesh
{
public:
    int indicesCnt;
    GLuint VAO, VBO, EBO;
	gridMesh()
	{
        std::vector<glm::vec3> gridVertices;
        std::vector<unsigned int> indices;
        int gridSize = 10;     // 网格扩展范围：-10 到 +10
        float step = 1.0f;     // 每格间隔

        for (int i = -gridSize; i <= gridSize; ++i) {
            // 水平方向点（Z 方向）
            gridVertices.push_back(glm::vec3(i * step, 0.0f, -gridSize * step));  // A
            gridVertices.push_back(glm::vec3(i * step, 0.0f, gridSize * step));   // B

            // 垂直方向点（X 方向）
            gridVertices.push_back(glm::vec3(-gridSize * step, 0.0f, i * step));  // C
            gridVertices.push_back(glm::vec3(gridSize * step, 0.0f, i * step));   // D
        }

        for (int i = 0; i < gridVertices.size(); i += 2) {
            indices.push_back(i);
            indices.push_back(i + 1);
        }
        indicesCnt = indices.size();
        setupMesh(gridVertices,indices);
	}

    void Draw()
	{
        glBindVertexArray(VAO);
        glDrawElements(GL_LINES, indicesCnt, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
	}
private:
    void setupMesh(std::vector<glm::vec3>& gridVertices, std::vector<unsigned int>& indices)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(glm::vec3), gridVertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

        glBindVertexArray(0);

    }
};
