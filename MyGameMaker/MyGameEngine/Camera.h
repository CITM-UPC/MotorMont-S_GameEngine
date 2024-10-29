// Camera.h
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 upDir = glm::vec3(0.0f, 1.0f, 0.0f);
    float aspect = 16.0f / 9.0f;
    glm::mat4 view = glm::mat4(1.0f);        // Ensure initialization
    glm::mat4 projection = glm::mat4(1.0f);  // Ensure initialization
    bool isOrthographic = true;

    Camera();
    void updateViewMatrix(const glm::vec3& target);
    void setProjection(float size, float near, float far);
    void toggleProjection();
};
