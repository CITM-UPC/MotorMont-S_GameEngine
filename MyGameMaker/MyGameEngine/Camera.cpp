#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

glm::dmat4 Camera::projection() const {
    return glm::ortho(-orthoSize * aspect, orthoSize * aspect, -orthoSize, orthoSize, zNear, zFar);
}

glm::dmat4 Camera::view() const {
    return glm::lookAt(_transform.pos(), _transform.pos() + _transform.fwd(), _transform.up());
}
