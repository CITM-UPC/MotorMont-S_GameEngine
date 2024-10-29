// Camera.h
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 upDir = glm::vec3(0.0f, 1.0f, 0.0f);
    float aspect = 16.0f / 9.0f;
    float zNear = 0.1f;
    float zFar = 100.0f;
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    bool isOrthographic = true;

    Camera();
    void updateViewMatrix(const glm::vec3& target);
    void setProjection(float near, float far); // Cambiado a near y far
    void applyProjection() const; // M�todo ajustado para aplicar en OpenGL directamente
    void toggleProjection();
};
