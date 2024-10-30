
#pragma once

#include "Transform.h"
#include <glm/glm.hpp>

class Camera {

public:
	double fov = glm::radians(60.0);
	double aspect = 16.0 / 9.0;
	double zNear = 0.1;
	double zFar = 128.0;

private:
	Transform _transform;

public:
	const auto& transform() const { return _transform; }
	auto& transform() { return _transform; }

	glm::mat4 projection() const;
	glm::mat4 view() const;

	glm::vec3 calculateRayFromMouse(float mouseX, float mouseY, float screenWidth, float screenHeight) const;  // Declared as member function
	glm::vec3 position() const;  // Declares position getter
};
