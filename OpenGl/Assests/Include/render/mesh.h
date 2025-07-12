#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // 包含所有OpenGL相关的类型声明

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

#include <string>
#include <vector>

#include "Component.h"
#include "plugins.h"

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
class Mesh:public Component
{
public:
    // 网格数据
    std::vector<Vertex>       vertices;   // 顶点数据列表
    std::vector<unsigned int> indices;    // 索引数组，用于按顺序绘制三角形
    unsigned int VAO;                // 顶点数组对象（VAO）
    GLenum PrimitiveType;

    // 构造函数：接收顶点、索引和纹理数据，并初始化网格
    // 注意：移动赋值，原来的数据会消失
    Mesh()
    {
        componentName = "Mesh";
        VBO = EBO = VAO = 0;
    }

    Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices
        , GLenum _primitiveType = GL_TRIANGLES):Mesh()
    {
        this->vertices = std::move(vertices);
        this->indices = std::move(indices);
        this->PrimitiveType = _primitiveType;
        componentName = "Mesh";

        setupMesh();
    }

    ~Mesh()
    {

    }

    Mesh(Mesh&& other) noexcept
        : vertices(std::move(other.vertices)),
        indices(std::move(other.indices)),
        VAO(other.VAO),
        VBO(other.VBO),
		EBO(other.EBO),
        PrimitiveType(other.PrimitiveType)
    {
        other.VAO = 0; // 避免析构时重复释放VAO
        componentName = "Mesh";
    }

    Mesh& operator=(Mesh&& other) noexcept
    {
        if (this != &other) {
            // 移动数据
            vertices = std::move(other.vertices);
            indices = std::move(other.indices);
            VAO = other.VAO;
            VBO = other.VBO;
            EBO = other.EBO;
            PrimitiveType = other.PrimitiveType;
            componentName = "Mesh";
            // 置空other的资源
            other.VAO = 0;
        }
        return *this;
    }
    Mesh(const Mesh& other)
        : vertices(other.vertices),
        indices(other.indices),
        PrimitiveType(other.PrimitiveType)

    {
        componentName = "Mesh";
        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;
    }

    Mesh& operator=(const Mesh& other)
    {
        if (this != &other) {
            componentName = "Mesh";
            vertices = other.vertices;
            indices = other.indices;
            PrimitiveType = other.PrimitiveType;
            VAO = other.VAO;
            VBO = other.VBO;
            EBO = other.EBO;

        }
        return *this;
    }

    void update() override
    {
        Draw();
    }
    const std::string getComponentName() const override
    {
        return componentName;
    }

    nlohmann::json toJson() override
    {
        nlohmann::json ret;
        nlohmann::json data;

        nlohmann::json verticesJson = nlohmann::json::array();

        for (auto& it : vertices)
        {
            nlohmann::json vertexJson;

            vertexJson["Position"] =     vec3ToJson(it.Position);
            vertexJson["Normal"] =       vec3ToJson(it.Normal);
            vertexJson["TexCoords"] =    vec2ToJson(it.TexCoords);
            vertexJson["Tangent"] =      vec3ToJson(it.Tangent);
            vertexJson["Bitangent"] =    vec3ToJson(it.Bitangent);

            // 骨骼信息
            nlohmann::json boneIDsJson = nlohmann::json::array();
            nlohmann::json weightsJson = nlohmann::json::array();

            for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
            {
                boneIDsJson.push_back(it.m_BoneIDs[i]);
                weightsJson.push_back(it.m_Weights[i]);
            }

            vertexJson["BoneIDs"] = boneIDsJson;
            vertexJson["Weights"] = weightsJson;

            verticesJson.push_back(vertexJson);
        }

        nlohmann::json indicesJson = nlohmann::json::array();

        for (auto&it:indices)
        {
            indicesJson.push_back(it);
        }

        // 可以将 verticesJson 存储进 data 或 ret 中
        data["vertices"] = verticesJson;
        data["indices"] = indicesJson;
        ret["componentData"] = data;
        ret["componentName"] = getComponentName();
        return ret;
    }

    void loadJson(const nlohmann::json& js) override
    {
        vertices.clear();
        indices.clear();

        const auto& verticesJson = js.at("vertices");
        for (const auto& vertexJson : verticesJson)
        {
            Vertex v;
            v.Position = jsonToVec3(vertexJson.at("Position"));
            v.Normal = jsonToVec3(vertexJson.at("Normal"));
            v.TexCoords = jsonToVec2(vertexJson.at("TexCoords"));
            v.Tangent = jsonToVec3(vertexJson.at("Tangent"));
            v.Bitangent = jsonToVec3(vertexJson.at("Bitangent"));

            const auto& boneIDs = vertexJson.at("BoneIDs");
            const auto& weights = vertexJson.at("Weights");

            for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
            {
                v.m_BoneIDs[i] = boneIDs[i].get<int>();
                v.m_Weights[i] = weights[i].get<float>();
            }

            vertices.push_back(v);
        }

        const auto& indicesJson = js.at("indices");
        for (const auto& idx : indicesJson)
        {
            indices.push_back(idx.get<unsigned int>());
        }
    }

    std::unique_ptr<Component> clone() const override
    {
        return std::make_unique<Mesh>(*this);
    }

    void showUI() override
    {
        if (ImGui::TreeNode(getComponentName().c_str()))
        {
            // 展示顶点数量、索引数量等基本信息
            ImGui::Text("Vertices: %zu", vertices.size());
            ImGui::Text("Indices: %zu", indices.size());

            static int vertexStart = 0;
            const int vertexShowCount = 10;  // 一次展示 10 个


            if (ImGui::TreeNode("Vertices"))
            {
                ImGui::SliderInt("Start Vertex", &vertexStart, 0, std::max(0, (int)vertices.size() - vertexShowCount));

                for (int i = vertexStart; i < vertexStart + vertexShowCount && i < vertices.size(); ++i)
                {
                    const auto& v = vertices[i];
                    std::string label = "Vertex #" + std::to_string(i);
                    if (ImGui::TreeNode(label.c_str()))
                    {
                        ImGui::Text("Position: (%.2f, %.2f, %.2f)", v.Position.x, v.Position.y, v.Position.z);
                        ImGui::Text("Normal:   (%.2f, %.2f, %.2f)", v.Normal.x, v.Normal.y, v.Normal.z);
                        ImGui::Text("TexCoord: (%.2f, %.2f)", v.TexCoords.x, v.TexCoords.y);
                        ImGui::Text("Tangent:  (%.2f, %.2f, %.2f)", v.Tangent.x, v.Tangent.y, v.Tangent.z);
                        ImGui::Text("Bitangent:(%.2f, %.2f, %.2f)", v.Bitangent.x, v.Bitangent.y, v.Bitangent.z);

                        ImGui::Text("Bones: ");
                        ImGui::Indent();
                        for (int j = 0; j < MAX_BONE_INFLUENCE; ++j)
                        {
                            ImGui::Text("#%d: ID = %d, Weight = %.2f", j, v.m_BoneIDs[j], v.m_Weights[j]);
                        }
                        ImGui::Unindent();

                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Indices"))
            {
                static int indexStart = 0;
                const int indexShowCount = 30;
                ImGui::SliderInt("Start Index", &indexStart, 0, std::max(0, (int)indices.size() - indexShowCount));

                for (int i = indexStart; i < indexStart + indexShowCount && i < indices.size(); ++i)
                {
                    ImGui::Text("#%d: %u", i, indices[i]);
                }

                ImGui::TreePop();
            }

            ImGui::TreePop(); // Component根结点
        }
    }

  

    // 渲染网格
    void Draw()
    {
        glBindVertexArray(VAO);
        glDrawElements(PrimitiveType, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

protected:
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
