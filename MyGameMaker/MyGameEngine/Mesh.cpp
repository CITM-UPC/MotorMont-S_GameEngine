#include "Mesh.h"
#include <stdexcept>
#include <iostream>

Mesh::Mesh() : _vao(0) {}

Mesh::~Mesh() {
    if (_vao != 0) {
        glDeleteVertexArrays(1, &_vao);
    }
}

bool Mesh::loadModel(const std::string& path) {
    if (_vao == 0) {
        glGenVertexArrays(1, &_vao);
    }
    glBindVertexArray(_vao);

    ModelData modelData;
    if (!LoadModel(path, modelData)) {
        std::cerr << "Failed to load model: " << path << std::endl;
        return false;
    }

    loadFromModelData(modelData);
    glBindVertexArray(0);

    return true;
}

void Mesh::loadFromModelData(const ModelData& modelData) {
    glBindVertexArray(_vao);

    // Load vertices
    _vertices = modelData.vertices;
    _vertexBuffer.loadElements(_vertices.size(), _vertices.data());
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    // Load normals
    if (!modelData.normals.empty()) {
        _normalBuffer.loadElements(modelData.normals.size(), modelData.normals.data());
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(1);
    }

    // Load texture coordinates
    if (!modelData.texCoords.empty()) {
        _texCoordBuffer.loadElements(modelData.texCoords.size(), modelData.texCoords.data());
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(2);
    }

    // Load indices
    _indices = modelData.indices;
    _indexBuffer.loadIndices(_indices.size(), _indices.data());

    glBindVertexArray(0);
}

void Mesh::draw() const {
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(_indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
