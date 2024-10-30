
#define GLM_ENABLE_EXPERIMENTAL  // Enable experimental features for GLM

#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

glm::mat4 Camera::projection() const {
    return glm::perspective(fov, aspect, zNear, zFar);
}

glm::mat4 Camera::view() const {
    return glm::lookAt(_transform.pos(), _transform.pos() + _transform.fwd(), _transform.up());
}

glm::vec3 Camera::calculateRayFromMouse(float mouseX, float mouseY, float screenWidth, float screenHeight) const {
    float x = (2.0f * mouseX) / screenWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / screenHeight;
    glm::vec4 rayClip(x, y, -1.0f, 1.0f);

    glm::vec4 rayEye = glm::inverse(projection()) * rayClip;
    rayEye.z = -1.0f; rayEye.w = 0.0f;

    glm::vec3 rayWorld = glm::normalize(glm::vec3(glm::inverse(view()) * rayEye));
    return rayWorld;
}

glm::vec3 Camera::position() const {
    return _transform.pos();  // Returns camera position based on _transform
}
