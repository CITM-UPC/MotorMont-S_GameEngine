// Camera.cpp
#include "Camera.h"

Camera::Camera() : position(0.0f, 0.0f, 5.0f) {
    updateViewMatrix(glm::vec3(0.0f, 0.0f, 0.0f));
    setProjection(10.0f, 0.1f, 100.0f);
}

void Camera::updateViewMatrix(const glm::vec3& target) {
    view = glm::lookAt(position, target, upDir);
}

void Camera::setProjection(float size, float near, float far) {
    if (isOrthographic) {
        float orthoSize = size;
        projection = glm::ortho(-orthoSize * aspect, orthoSize * aspect, -orthoSize, orthoSize, near, far);
    }
    else {
        projection = glm::perspective(glm::radians(size), aspect, near, far);
    }
}

void Camera::toggleProjection() {
    isOrthographic = !isOrthographic;
}
