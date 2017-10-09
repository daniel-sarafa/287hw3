#include "Plane.h"

/**
* Constructor for the Plane.
*/
Plane::Plane(const glm::vec3 & point, const glm::vec3 & normal, const color & material)
	: Surface(material), a(point), n(normalize(normal)) {
}

Plane::Plane(std::vector<glm::vec3> vertices, const color & material)
	: Surface(material) {
	a = vertices[0];

	n = glm::normalize(glm::cross(vertices[2] - vertices[1], vertices[0] - vertices[1]));
}

Plane::~Plane(void) {
}

/*
* Checks a ray for intersection with the surface. Finds the closest point of intersection
* if one exits. Returns a HitRecord with the t parmeter set to FLT_MAX if there is no
* intersection.
*/
HitRecord Plane::findClosestIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection) {
	HitRecord hitRecord;

	// (a-e)*n/d*n
	float num = glm::dot(a - rayOrigin, n);
	float denom = glm::dot(rayDirection, n);
	if (denom != 0) {
		hitRecord.t = num / denom;
		hitRecord.material = material;
		hitRecord.interceptPoint = rayOrigin + hitRecord.t * rayDirection;
		hitRecord.surfaceNormal = n;
		if (hitRecord.t <= 0)
			hitRecord.t = FLT_MAX;
	}

	return hitRecord;

} // end checkIntercept

