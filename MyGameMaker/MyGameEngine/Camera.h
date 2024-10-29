#pragma once

#include "Transform.h"
#include <glm/glm.hpp>

class Camera {
public:
    // Orthographic properties
    double orthoSize = 10.0;
    double aspect = 16.0 / 9.0;
    double zNear = 0.1;
    double zFar = 128.0;

private:
    class Transform {
    public:
        glm::vec3 position; // or glm::dvec3 for double precision
        glm::vec3 forward; // or glm::dvec3 for double precision
        // ...

        glm::vec3& pos() { return position; } // Return reference
        glm::vec3& fwd() { return forward; } // Return reference
    };
};
