#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <GL/glew.h>
#include "BufferObject.h"
#include "BoundingBox.h"
#include "LoadModel.h"

class Mesh {
    GLuint _vao;
    BufferObject _vertexBuffer;
    BufferObject _normalBuffer;
    BufferObject _texCoordBuffer;
    BufferObject _colorBuffer; // Added for colors
    BufferObject _indexBuffer;

    std::vector<glm::vec3> _vertices;
    std::vector<unsigned int> _indices;
    BoundingBox _boundingBox;

public:
    Mesh();
    ~Mesh();

    const auto& vertices() const { return _vertices; }
    const auto& indices() const { return _indices; }
    const auto& boundingBox() const { return _boundingBox; }

    bool loadModel(const std::string& path);
    void loadFromModelData(const ModelData& modelData);
    void draw() const;
};
