#include "Quadric.h"

QuadricSurface::QuadricSurface(const QuadricParameters &params,
								const glm::vec3 &position,
								const color &mat)
	: Surface(mat), center(position),
		A(params.A), B(params.B), C(params.C), D(params.D), E(params.E),
		F(params.F), G(params.G), H(params.H), I(params.I), J(params.J) {
}

glm::vec3 QuadricSurface::normal(const glm::vec3 &pt) {
	glm::vec3 normal(2 * A * pt.x + D * pt.y + E * pt.z + G,
					2 * B * pt.y + D * pt.x + F * pt.z + H,
					2 * C * pt.z + E * pt.x + F * pt.y + I);
	return glm::normalize(normal);
}

int QuadricSurface::findIntersections(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection,
										HitRecord hits []) {
	glm::vec3 Ro = rayOrigin - center;
	const glm::vec3 &Rd = rayDirection;

	// After substituting the parametric form of the ray, Ro + t* Rd, into the 
	// generalized form of the quadratic equation for a quadric surface the equation
	// reduces to Aq(tt) + Bq(t) + Cq where

	float Aq = A * (Rd.x*Rd.x) + B * (Rd.y*Rd.y) + C * (Rd.z*Rd.z) +
		D * (Rd.x * Rd.y) + E * (Rd.x * Rd.z) + F * (Rd.y * Rd.z);

	float Bq = (2 * A * Ro.x*Rd.x) + (2 * B * Ro.y*Rd.y) + (2 * C * Ro.z*Rd.z) +
		D * (Ro.x * Rd.y + Ro.y * Rd.x) +
		E * (Ro.x * Rd.z + Ro.z * Rd.x) +
		F * (Ro.y * Rd.z + Ro.z * Rd.y) +
		G * Rd.x + H * Rd.y + I * Rd.z;

	float Cq = A * (Ro.x * Ro.x) + B * (Ro.y * Ro.y) + C * (Ro.z * Ro.z) +
		D * (Ro.x * Ro.y) + E * (Ro.x * Ro.z) + F * (Ro.y * Ro.z) +
		G * Ro.x + H * Ro.y + I * Ro.z + J;

	float roots[2];
	int numRoots = solveQuadratic(Aq, Bq, Cq, roots);
	int numIntersections = 0;
	for (int i = 0; i < numRoots; i++) {
		if (roots[i] > 0) {
			const float &t = roots[i];
			hits[numIntersections].t = t;
			hits[numIntersections].material = material;
			glm::vec3 interceptPt = Ro + t * Rd + center;
			hits[numIntersections].interceptPoint = interceptPt;
			hits[numIntersections].surfaceNormal = normal(interceptPt);
			// Check if the intersection with the inside or back of the surface
			if (glm::dot(hits[numIntersections].surfaceNormal, Rd) > 0) {
				hits[numIntersections].surfaceNormal = -hits[numIntersections].surfaceNormal;
			}
			numIntersections++;
		}
	}

	return numIntersections;
}

HitRecord QuadricSurface::findClosestIntersection(const glm::vec3 &rayOrigin,
													const glm::vec3 &rayDirection) {
	HitRecord hits[2];
	HitRecord theResult;
	theResult.t = FLT_MAX;

	int numIntercepts = findIntersections(rayOrigin, rayDirection, hits);
	if (numIntercepts == 1 && hits[0].t > 0) {
		theResult = hits[0];
	} else if (numIntercepts == 2) {
		if (hits[0].t > 0) {
			theResult = hits[0];
		} else if (hits[1].t > 0) {
			theResult = hits[1];
		}
	}

	return theResult;
}
