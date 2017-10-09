#pragma once
#include "Surface.h"

struct QuadricParameters {
	float A, B, C, D, E, F, G, H, I, J;
};

class QuadricSurface : public Surface {
public:
	QuadricSurface(const QuadricParameters &params,
					const glm::vec3 &position,
					const color &material);
	~QuadricSurface() {}
	glm::vec3 normal(const glm::vec3 &pt);
	virtual HitRecord findClosestIntersection(const glm::vec3 &origin, const glm::vec3 &dir);
	int findIntersections(const glm::vec3 &origin, const glm::vec3 &dir,
							HitRecord hits[]);
protected:
	glm::vec3 center;
	float A, B, C, D, E, F, G, H, I, J;
};