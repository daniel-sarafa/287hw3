#include "CylinderY.h"
#include "Quadric.h"

CylinderY::CylinderY(const glm::vec3 &position, float H, float rad,
	const color &material)
	: Surface(material), center(position), radius(rad), height(H) {
}

HitRecord CylinderY::findClosestIntersection(const glm::vec3 &rayOrigin,
	const glm::vec3 &rayDirection) {
	float R2 = radius*radius;
	QuadricParameters cylParams = { 1.0f / R2, 0, 1.0f / R2, 0, 0, 0, 0, 0, 0, -1 };
	QuadricSurface s(cylParams, center, material);
	HitRecord hits[2];
	int numHits = s.findIntersections(rayOrigin, rayDirection, hits);
	// findIntersections puts the hits in ascending order.
	for (int i = 0; i < numHits; i++) {
		if (hits[i].interceptPoint.y < center.y + height / 2 &&
			hits[i].interceptPoint.y > center.y - height / 2) {
			return hits[i];
		}
	}
	HitRecord theHit;
	theHit.t = FLT_MAX;
	return theHit;
}
