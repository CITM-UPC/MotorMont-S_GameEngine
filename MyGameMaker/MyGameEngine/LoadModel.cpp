#include "LoadModel.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdexcept>
#include <iostream>

bool LoadModel(const std::string& path, ModelData& modelData) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        return false;
    }

    aiMesh* mesh = scene->mMeshes[0];
    modelData.vertices.reserve(mesh->mNumVertices);
    modelData.normals.reserve(mesh->mNumVertices);
    modelData.texCoords.reserve(mesh->mNumVertices);
    modelData.colors.reserve(mesh->mNumVertices); // Reserve for colors
    modelData.indices.reserve(mesh->mNumFaces * 3);

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        modelData.vertices.emplace_back(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

        if (mesh->HasNormals()) {
            modelData.normals.emplace_back(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        }

        if (mesh->HasVertexColors(0)) {
            modelData.colors.emplace_back(mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b);
        }

        if (mesh->HasTextureCoords(0)) {
            modelData.texCoords.emplace_back(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else {
            modelData.texCoords.emplace_back(0.0f, 0.0f);
        }
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace& face = mesh->mFaces[i];
        if (face.mNumIndices != 3) {
            std::cerr << "WARNING: Mesh face does not have 3 indices." << std::endl;
            continue;
        }
        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
            modelData.indices.push_back(face.mIndices[j]);
        }
    }

    return true;
}
