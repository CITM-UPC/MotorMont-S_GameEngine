#include "LoadModel.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdexcept>
#include <iostream>

void processNode(aiNode* node, const aiScene* scene, ModelData& modelData) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        // Procesar cada malla en el nodo
        for (unsigned int v = 0; v < mesh->mNumVertices; ++v) {
            modelData.vertices.emplace_back(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);

            if (mesh->HasNormals()) {
                modelData.normals.emplace_back(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
            }

            if (mesh->HasTextureCoords(0)) {
                modelData.texCoords.emplace_back(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
            }
            else {
                modelData.texCoords.emplace_back(0.0f, 0.0f);
            }
        }

        for (unsigned int f = 0; f < mesh->mNumFaces; ++f) {
            const aiFace& face = mesh->mFaces[f];
            for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                modelData.indices.push_back(face.mIndices[j]);
            }
        }
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, modelData);
    }
}

bool LoadModel(const std::string& path, ModelData& modelData) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
        aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_OptimizeMeshes);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error::Assimp:: " << importer.GetErrorString() << std::endl;
        return false;
    }

    processNode(scene->mRootNode, scene, modelData);  // Procesa todos los nodos de la escena

    return true;
}
