// Camera.cpp
#include "Camera.h"
#include <GL/glew.h>  // Para inicializar y manejar extensiones modernas de OpenGL
#include <GL/gl.h>


Camera::Camera() : position(0.0f, 0.0f, 5.0f) {
    updateViewMatrix(glm::vec3(0.0f, 0.0f, 0.0f));
    setProjection(zNear, zFar);
}

void Camera::updateViewMatrix(const glm::vec3& target) {
    view = glm::lookAt(position, target, upDir);
    glMatrixMode(GL_MODELVIEW);  // Configuración de vista
    glLoadMatrixf(&view[0][0]);
}

void Camera::setProjection(float near, float far) {
    zNear = near;
    zFar = far;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (isOrthographic) {
        float size = 10.0f; // Ajuste de tamaño de ortografía según aspecto
        glOrtho(-size * aspect, size * aspect, -size, size, near, far);
    }
    else {
        float fov = 45.0f; // Campo de visión para perspectiva
        float top = near * tan(glm::radians(fov) / 2);
        float right = top * aspect;
        glFrustum(-right, right, -top, top, near, far);
    }

    glMatrixMode(GL_MODELVIEW); // Volver a modo de modelo/vista
}

void Camera::applyProjection() const {
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(&projection[0][0]);
    glMatrixMode(GL_MODELVIEW);
}

void Camera::toggleProjection() {
    isOrthographic = !isOrthographic;
    setProjection(zNear, zFar);
}
