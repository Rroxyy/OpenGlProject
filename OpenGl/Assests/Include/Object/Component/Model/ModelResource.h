#pragma once
#include <assimp/mesh.h>
#include <assimp/scene.h>

#include "mesh.h"

class ModelResource {
public:
    std::vector<Mesh> meshes;
    std::string filePath="";
    AABB localAABB;

    ModelResource() = default;
    ~ModelResource() = default;

    ModelResource(const std::string& path) {
        loadModel(path);
        filePath = path;
    }

    void copyMesh(Mesh* mesh);
    void moveMesh(Mesh* mesh);


    void showUI();
    void Draw() {
        for (auto& mesh : meshes)
            mesh.Draw();
    }


private:
   
    void updateAABB(const AABB& meshAABB);
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh* processMesh(aiMesh* mesh);

};
