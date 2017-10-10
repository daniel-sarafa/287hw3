#include "RayTracer.h"
#define EPSILON 1.0E-4f

RayTracer::RayTracer(FrameBuffer & cBuffer, int alias, color defaultColor)
	:colorBuffer(cBuffer), defaultColor(defaultColor), recursionDepth(2), alias(alias) {

}

RayTracer::~RayTracer(void) {
}

void RayTracer::setCameraFrame(const glm::vec3 & viewPosition, const glm::vec3 & viewingDirection, glm::vec3 up) {
	// Calculate and set the the w, u, and vdata members and 
	// set the eye data member to the viewing position.
	// TODO

	eye = viewPosition;
	// Squaring up a basis
	w = glm::normalize(-viewingDirection);
	u = glm::normalize(glm::cross(up, w));
	v = glm::normalize(glm::cross(w, u));
} // end setCameraFrame

void RayTracer::calculatePerspectiveViewingParameters(const float verticalFieldOfViewDegrees) {
	// Set the set the topLimit, bottomLimit, rightLimit, 
	// leftLimit, distToPlane, nx, and ny data members
	// TODO
	nx = (float)colorBuffer.getWindowWidth();
	ny = (float)colorBuffer.getWindowHeight();
	topLimit = 1;
	bottomLimit = -topLimit;
	rightLimit = topLimit * nx / ny;
	leftLimit = -rightLimit;
	float rads = glm::radians(verticalFieldOfViewDegrees / 2);
	distToPlane = 1.0f / std::tan(rads);

} // end calculatePerspectiveViewingParameters

void RayTracer::calculateOrthographicViewingParameters(const float viewPlaneHeight) {
	topLimit = fabs(viewPlaneHeight) / 2.0f;

	rightLimit = topLimit * ((float)colorBuffer.getWindowWidth() / colorBuffer.getWindowHeight()); // Set r based on aspect ratio and height of plane

	// Make view plane symetrical about the viewing direction
	leftLimit = -rightLimit;
	bottomLimit = -topLimit;

	// Calculate the distance between pixels in the horizontal and vertical directions
	nx = (float)colorBuffer.getWindowWidth();
	ny = (float)colorBuffer.getWindowHeight();

	distToPlane = 0.0; // Rays start on the view plane

} // end calculateOrthographicViewingParameters

glm::vec2 RayTracer::getProjectionPlaneCoordinates(const int x, const int y) {
	// Page 75
	// Calculate and return the u and v data members as the x an y coordinates
	// of the 
	glm::vec2 uvScalarValues;
	uvScalarValues.x = leftLimit + (rightLimit - leftLimit) * (x + 0.5f) / nx;
	uvScalarValues.y = bottomLimit + (topLimit - bottomLimit) * (y + 0.5f) / ny;
	// TODO
	return uvScalarValues;
}

void RayTracer::setOrthoRayOriginAndDirection(const int x, const int y) {
	glm::vec2 uv = getProjectionPlaneCoordinates(x, y);

	// Page 75
	rayDirection = glm::normalize(-w);
	rayOrigin = eye + uv.x * u + uv.y * v;

} // end setOrthoOriginAndDirection

void RayTracer::setPerspectiveRayOriginAndDirection(const int x, const int y) {
	glm::vec2 uv = getProjectionPlaneCoordinates(x, y);
	rayDirection = glm::normalize(-distToPlane*w + uv.x*u + uv.y*v);
	rayOrigin = eye;
} // end setPerspectiveOriginAndDirection

void RayTracer::raytraceScene(const std::vector<std::shared_ptr<Surface>> & surfaces, const std::vector<std::shared_ptr<LightSource>> & lights, int alias) {
	this->surfacesInScene = surfaces;
	this->lightsInScene = lights;
	this->alias = alias;
	for (int x = 0; x < colorBuffer.getWindowWidth(); x++) {
		for (int y = 0; y < colorBuffer.getWindowHeight(); y++) {
			color total(0, 0, 0, 0);
			for (float i = 0; i < alias; i++) {
				for (float j = 0; j < alias; j++) {
					setPerspectiveRayOriginAndDirection(x+(i/2), y+(j/2));
					color C = traceIndividualRay(rayOrigin, rayDirection, 1);
					total += C;
				}
			}
			color newTotal(total.r / pow(alias, 2), total.g / pow(alias, 2), total.b / pow(alias, 2), total.a / pow(alias, 2));
			colorBuffer.setPixel(x, y, newTotal);
		}
	}
	// Iterate through each and every pixel in the rendering window
	// TODO


} // end raytraceScene

color RayTracer::traceIndividualRay(const glm::vec3 &e, const glm::vec3 &d, int recursionLevel) {
	// Find surface intersection that is closest to 'e' in the direction 'd.'
	// TODO
	float distance = FLT_MAX;
	color result = defaultColor;
	color shadow(0, 0, 0, 1);
	PositionalLight light(glm::vec3(-10, 4, 0), color(1, 1, 1, 1));
	for (unsigned int i = 0; i < surfacesInScene.size(); i++) {
		HitRecord hit = surfacesInScene[i]->findClosestIntersection(e, d);
		glm::vec3 lightDir(light.lightPosition - hit.interceptPoint);
		glm::vec3 acne(hit.interceptPoint.x, hit.interceptPoint.y, hit.interceptPoint.z);
		HitRecord shadow = findIntersection(acne, lightDir, surfacesInScene);
		if (shadow.t != FLT_MAX && shadow.t < distance && shadow.t > EPSILON) {
			hit.material = color(0, 0, 0, 1);
		}
		if (hit.t != FLT_MAX && hit.t < distance) {
			distance = hit.t;
			result = hit.material;
		}
		
	}

	return result;
} // end traceRay