#pragma once
#include "BasicIncludesAndDefines.h"
#include "Surface.h"

class Disk : public Surface {
public:
	Disk(const glm::vec3 &position, const glm::vec3 &n, float rad, const color &mat);
	~Disk() {}
	virtual HitRecord findClosestIntersection(const glm::vec3 &origin, const glm::vec3 &dir);
protected:
	glm::vec3 center, normal;
	float radius;
};