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
        projection = glm::ortho(-size * aspect, size * aspect, -size, size, near, far);
    }
    else {
        projection = glm::perspective(glm::radians(size), aspect, near, far);
    }
}

void Camera::applyProjection(GLuint shaderProgram) const {
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);
}

void Camera::toggleProjection() {
    isOrthographic = !isOrthographic;
    setProjection(10.0f, 0.1f, 100.0f);
}
