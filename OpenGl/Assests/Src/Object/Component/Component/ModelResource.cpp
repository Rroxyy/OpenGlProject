#include "ModelResource.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


void ModelResource::copyMesh(Mesh* mesh)
{
    meshes.emplace_back(*mesh);
    updateAABB(mesh->aabb);
}

void ModelResource::moveMesh(Mesh* mesh)
{
    updateAABB(mesh->aabb);
    meshes.emplace_back(std::move(*mesh));
}





void ModelResource::updateAABB(const AABB& meshAABB)
{
    localAABB.merge(meshAABB);
}



void ModelResource::loadModel(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	processNode(scene->mRootNode, scene);
}



void ModelResource::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
       moveMesh(processMesh(mesh));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}
// 处理网格
Mesh* ModelResource::processMesh(aiMesh* mesh)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

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

        vertices.emplace_back(vertex);
    }

    // 处理编组数据
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.emplace_back(face.mIndices[j]);
    }

    //std::cout << ab.max_AABB.x << std::endl;
    return new Mesh(vertices, indices);
}


void ModelResource::showUI()
{
	if (ImGui::TreeNode("Model"))
	{
		localAABB.showUI();
		int index = 0;
		for (auto& it : meshes)
		{
			std::string str = "Mesh: ";
			str += std::to_string(index);
			if (ImGui::TreeNode(str.c_str()))
			{
				it.showUI();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}
