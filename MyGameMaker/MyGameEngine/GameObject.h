#pragma once
#include <string>
#include <glm/glm.hpp> // For vector and transformation handling

class GameObject
{
public:
    GameObject(const std::string& name = "Unnamed GameObject"); // Constructor with default name

    // GameObject name
    std::string name;

    // Transform properties
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    // Mesh properties
    bool hasMesh;
    bool displayTriangleNormals;
    bool displayFaceNormals;

    // Texture properties
    bool hasTexture;
    std::string texturePath;
    int textureWidth;
    int textureHeight;
    bool useCheckerTexture;

    // Methods
    void paint();

    // Additional helper methods (optional)
    void setPosition(float x, float y, float z);
    void setRotation(float x, float y, float z);
    void setScale(float x, float y, float z);
    void setTexture(const std::string& path, int width, int height);
    void toggleMeshNormals(bool triangleNormals, bool faceNormals);
};
