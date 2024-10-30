
#include "BoundingBox.h"
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/component_wise.hpp>  // Added for compMax and compMin

BoundingBox::BoundingBox(const vec3* vertices, size_t num_verts) {
	min = max = vertices[0];
	for (size_t i = 1; i < num_verts; ++i) {
		min = glm::min(min, vertices[i]);
		max = glm::max(max, vertices[i]);
	}
}

BoundingBox BoundingBox::operator+(const BoundingBox& other) const {
	BoundingBox result;
	result.min = glm::min(min, other.min);
	result.max = glm::max(max, other.max);
	return result;
}

BoundingBox operator*(const mat4& mat, const BoundingBox& bbox) {
	auto vertices = bbox.vertices();
	for (auto& v : vertices) v = mat * vec4(v, 1);
	return BoundingBox(vertices.data(), vertices.size());
}

// Intersects method implementation
bool BoundingBox::intersects(const glm::vec3& rayDirection, const glm::vec3& rayOrigin) const {
	glm::vec3 invDir = 1.0f / rayDirection;
	glm::vec3 t0 = glm::vec3(min) - rayOrigin; // asegúrate de que rayOrigin es vec3
	glm::vec3 t1 = glm::vec3(max) - rayOrigin;

	glm::vec3 tmin = glm::min(t0, t1);
	glm::vec3 tmax = glm::max(t0, t1);

	float tNear = glm::compMax(tmin);
	float tFar = glm::compMin(tmax);

	return tNear <= tFar && tFar >= 0;
}
