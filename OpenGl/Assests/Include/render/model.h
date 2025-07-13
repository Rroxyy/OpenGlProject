#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> // OpenGL类型声明

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh.h>
#include <shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <json.hpp>
#include <map>
#include <vector>

#include "baseShader.h"
#include "Component.h"
#include "Object.h"
using namespace std;

// 从文件读取纹理并生成纹理ID
unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model:public Component
{
public:
    // 模型数据
    vector<Mesh> meshes;                // 模型包含的网格
    string filePath;                   // 模型所在的文件夹

    Model()
    {
        componentName = "Model";
    }
    
    
    Model(string const& _path) 
    {
        componentName = "Model";
        filePath = _path;
        loadModel(_path);
    }

    ~Model()
    {

    }

    void addMesh(Mesh* mesh)
    {
        meshes.emplace_back(move(*mesh));

        setMeshName();
    }


    const std::string getComponentName() const override
    {
        return componentName;
    }
    nlohmann::json toJson() override
    {
        nlohmann::json ret;
        nlohmann::json data;
        data["filePath"] = filePath;
        ret["componentData"] = data;
        ret["componentName"] = getComponentName();
        return ret;
    }

    void loadJson(const nlohmann::json& js) override
    {
        filePath = js["filePath"].get<std::string>();
        loadModel(filePath);
    }
    std::unique_ptr<Component> clone() const override
    {
        return make_unique<Model>(*this);
    }

    void showUI() override
    {
	    if (ImGui::TreeNode(getComponentName().c_str()))
	    {
		    for (auto&it:meshes)
		    {
                it.showUI();
		    }
            ImGui::TreePop();
	    }
    }

    void update() override
    {
        Draw();
    }

    // 渲染整个模型
    void Draw()
    {
        object->GetComponent<baseShader>()->blind_shader_value();
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw();
        object->GetComponent<baseShader>()->unblindTexturesChannel();
    }


   
    Model(Model&&other) noexcept:meshes(std::move(other.meshes)),filePath(std::move(other.filePath))
    {
        componentName = "Model";
    }
    Model& operator=(Model&& other)noexcept
	{
        if (this == &other)return *this;
        componentName = "Model";
        meshes = std::move(other.meshes);
        filePath = std::move(other.filePath);
        return *this;
	}
    // 拷贝构造函数
    Model(const Model& other)
        : meshes(other.meshes),    // 调用 Mesh 的拷贝构造（确保 Mesh 支持拷贝）
        filePath(other.filePath)
    {
        componentName = "Model";
    }
    // 拷贝赋值运算符
    Model& operator=(const Model& other)
    {
        if (this != &other)
        {
            componentName = "Model";
            meshes = other.meshes;      // 调用 Mesh 的拷贝赋值
            filePath = other.filePath;
        }
        return *this;
    }


private:
    void setMeshName()
    {
	    int index = 0;
	    for (auto& it:meshes)
	    {
		    it.meshName = "mesh" + to_string(index);
	    }
    }

    // 使用 ASSIMP 读取模型，并处理所有网格
    void loadModel(string const& path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals  | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }

        processNode(scene->mRootNode, scene);

        setMeshName();
    }

    // 递归处理模型的节点
    void processNode(aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh));
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    // 处理网格
    Mesh processMesh(aiMesh* mesh)
    {
        vector<Vertex> vertices;
        vector<unsigned int> indices;

        // 处理顶点数据
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }

            if (mesh->mTextureCoords[0])
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;

                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;

                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }

        // 处理编组数据
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

       

        return Mesh(vertices, indices);
    }
};



#endif
