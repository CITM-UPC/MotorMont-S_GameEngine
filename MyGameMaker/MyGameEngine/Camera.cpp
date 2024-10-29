#include "Camera.h"
#include <GL/glew.h>  
#include <GL/gl.h>    

Camera::Camera() : position(0.0f, 0.0f, 5.0f) {
    updateViewMatrix(glm::vec3(0.0f, 0.0f, 0.0f));
    setProjection(fov, zNear, zFar);
}

void Camera::updateViewMatrix(const glm::vec3& target) {
    view = glm::lookAt(position, target, upDir);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(&view[0][0]);
}

void Camera::setProjection(float fov, float near, float far) {
    zNear = near;
    zFar = far;
    this->fov = fov;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (isOrthographic) {
        float size = 10.0f;
        glOrtho(-size * aspect, size * aspect, -size, size, near, far);
    }
    else {
        float top = zNear * tan(glm::radians(fov) / 2.0f);
        float right = top * aspect;
        glFrustum(-right, right, -top, top, zNear, zFar);
    }

    glMatrixMode(GL_MODELVIEW);
}

void Camera::applyProjection() const {
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(&projection[0][0]);
    glMatrixMode(GL_MODELVIEW);
}

void Camera::toggleProjection() {
    isOrthographic = !isOrthographic;
    setProjection(fov, zNear, zFar);
}
