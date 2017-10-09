#include "CylinderY.h"
#include "Quadric.h"

CylinderY::CylinderY(const glm::vec3 &position, float H, float rad,
							const color &material)
	: Surface(material), center(position), radius(rad), height(H) {
}

HitRecord CylinderY::findClosestIntersection(const glm::vec3 &rayOrigin,
										const glm::vec3 &rayDirection) {
	HitRecord theResult;
	QuadricParameters params = { 1.0 / CylinderY::radius, 0, 1.0 / CylinderY::radius, 0, 0, 0, 0, 0, 0, -1 };
	QuadricSurface cylQuad(params, center, material);
	HitRecord hits[2];
	int numIntercepts = cylQuad.findIntersections(rayOrigin, rayDirection, hits);
	int i = 0;
	while(i < numIntercepts){
		if (hits[i].interceptPoint.y > (center.y + height / 2) || 
			hits[i].interceptPoint.y < (center.y - height / 2)) {
			hits[i].t = FLT_MAX;
		}
		i++;
	}
	if (numIntercepts == 1 && hits[0].t > 0) {
		theResult = hits[0];
	}
	else if (numIntercepts == 2) {
		if (hits[0].t > 0 && hits[0].t < hits[1].t) {
			theResult = hits[0];
		}
		else if (hits[1].t > 0 && hits[1].t < hits[0].t) {
			theResult = hits[1];
		}
	}
	return theResult;
}
