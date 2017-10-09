#include "Disk.h"
#include "Plane.h"
#include <glm/glm.hpp>

Disk::Disk(const glm::vec3 &pos, const glm::vec3 &n, float rad,
			const color &material)
	: Surface(material), center(pos), normal(n), radius(rad) {
}

HitRecord Disk::findClosestIntersection(const glm::vec3 &rayOrigin,
										const glm::vec3 &rayDirection) {
	Plane plane(center, normal, material);
	HitRecord hitRecord = plane.findClosestIntersection(rayOrigin, rayDirection);
	if (glm::distance(hitRecord.interceptPoint, center) > radius) {
		hitRecord.t = FLT_MAX;
	}
	return hitRecord;
}
