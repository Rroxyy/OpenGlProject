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
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <json.hpp>
#include <map>
#include <vector>

#include "AABB.h"
#include "baseShader.h"
#include "Component.h"
#include "Object.h"
#include <glm/gtx/string_cast.hpp>

// 从文件读取纹理并生成纹理ID
unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model:public Component
{
public:
    // 模型数据
    std::vector<Mesh> meshes;                // 模型包含的网格
    std::string filePath;                   // 模型所在的文件夹
    AABB aabb;


    Model()
    {
        componentName = "Model";
    }
    
    
    Model(std::string const& _path) 
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
        meshes.emplace_back(std::move(*mesh));

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
        return std::make_unique<Model>(*this);
    }

    void showUI() override
    {
	    if (ImGui::TreeNode(getComponentName().c_str()))
	    {
		    std::cout << aabb << std::endl;
            aabb.showUI();

		    for (auto&it:meshes)
		    {
                it.showUI();
		    }
            ImGui::TreePop();
	    }
    }



    // 渲染整个模型
    void Draw()
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw();
    }


private:

    void setMeshName()
    {
	    int index = 0;
	    for (auto& it:meshes)
	    {
		    it.meshName = "mesh" + std::to_string(index);
	    }
    }

    // 使用 ASSIMP 读取模型，并处理所有网格
    void loadModel(std::string const& path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals  | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
	        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }

        processNode(scene->mRootNode, scene);

        setMeshName();

        resetAABB();
    }

    void resetAABB()
    {
        
        aabb = meshes.front().aabb;
        std::cout << aabb << std::endl;
        std::cout << meshes.front().aabb << std::endl;
        for (auto& it:meshes)
        {
            std::cout << "it:  " << it.aabb << std::endl;
            aabb.merge(it.aabb);
        }
    }

    // 递归处理模型的节点
    void processNode(aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.emplace_back(processMesh(mesh));
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    // 处理网格
    Mesh processMesh(aiMesh* mesh)
    {
	    std::vector<Vertex> vertices;
	    std::vector<unsigned int> indices;
        AABB ab;

        // 处理顶点数据
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            ab.updateAABB(vector);

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

       
        //std::cout << ab.max_AABB.x << std::endl;
        return Mesh(vertices, indices,ab);
    }
};



#endif
