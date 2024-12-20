#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "BufferObject.h"
#include "BoundingBox.h"
#include <assimp/scene.h>
#include <GL/glew.h>

class Mesh {

    BufferObject _texCoordBuffer;
    BufferObject _normalBuffer;
    BufferObject _colorBuffer;
    BufferObject _vertexBuffer;
    BufferObject _indexBuffer;

    std::vector<glm::vec3> _vertices;
    std::vector<unsigned int> _indices;

    BoundingBox _boundingBox;

public:
    std::vector<Mesh> subMeshes;

    const auto& vertices() const { return _vertices; }
    const auto& indices() const { return _indices; }
    const auto& boundingBox() const { return _boundingBox; }

    void load(const glm::vec3* verts, size_t num_verts, const unsigned int* indexs, size_t num_indexs);
    void loadTexCoords(const glm::vec2* texCoords);
    void loadNormals(const glm::vec3* normals);
    void loadColors(const glm::u8vec3* colors);
    void draw() const;
    bool drawboundingBox = true;
	static void drawBoundingBox(const BoundingBox& bbox);
    static void drawWiredQuad(const glm::dvec3& v0, const glm::dvec3& v1, const glm::dvec3& v2, const glm::dvec3& v3);
	inline static void glVertex3(const glm::dvec3& v) { glVertex3dv(&v.x); }

    bool loadFromFile(const char* file_path);  // Ahora es la �nica funci�n de carga en Mesh
};
