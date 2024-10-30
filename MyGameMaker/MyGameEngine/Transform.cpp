#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

void Transform::translate(const vec3& v) {
	_mat = glm::translate(_mat, v);
}

void Transform::rotate(double rads, const vec3& v) {
	_mat = glm::rotate(_mat, rads, v);
}

void Transform::alignToGlobalUp(const vec3& worldUp) {
	// Keep the forward vector as it is, but adjust the up vector to align with the world up
	vec3 fwd = glm::normalize(_fwd);
	vec3 right = glm::normalize(glm::cross(worldUp, fwd));
	vec3 up = glm::cross(fwd, right);

	// Set the matrix with the new aligned vectors
	_left = right;
	_up = up;
	_fwd = fwd;
	_pos = _pos;  // Position remains unchanged
	_mat = mat4(vec4(_left, 0.0f), vec4(_up, 0.0f), vec4(_fwd, 0.0f), vec4(_pos, 1.0f));
}
