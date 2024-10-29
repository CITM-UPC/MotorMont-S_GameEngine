#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

struct ModelData {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;
};

bool LoadModel(const std::string& path, ModelData& modelData);
