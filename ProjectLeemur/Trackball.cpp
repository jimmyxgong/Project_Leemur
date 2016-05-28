#include "Trackball.h"

Vector3f Trackball::toTrackBallPoint(const Point & point) {
	return toTrackBallPoint(point.x, point.y);
}

Vector3f Trackball::toTrackBallPoint(float x, float y) {
	Vector3f mappedPoint;
	mappedPoint.x = (2.0f*x - window->width()) / window->width();
	mappedPoint.y = (window->height() - 2.0f*y) / window->height();
	mappedPoint.z = 0.0f;

	double depth = length(mappedPoint);
	if (depth > 1.0f) depth = 1.0f;

	mappedPoint.z = (float) sqrt(1.001 - (depth*depth));
	normalize(mappedPoint);
	return mappedPoint;
}

Quaternion Trackball::fromPoints(const Point & from, const Point & to) {
	Vector3f start = toTrackBallPoint(from);
	Vector3f end = toTrackBallPoint(to);

	Vector3f direction = end - start;
	Vector3f axis = cross(start, end);
	axis = normalize(axis);

	float angle = ROT_SCALE * length(direction);

	if (angle > 1.0f) angle = 1.0f;
	if (angle < -1.0f) angle = -1.0f;

	Quaternion quat;
	if (axis.length() > epsilon) {
		float sine = sinf(angle / 2);
		quat.x = axis.x * sine;
		quat.y = axis.y * sine;
		quat.z = axis.z * sine;
		quat.w = cosf(angle / 2);
		quat.normalize();
	}

	return quat;
}

Trackball::Trackball(Window * win, float rotationScale, float epsilon) : 
	window(win), rotationScale(rotationScale), epsilon(epsilon) {}

Trackball::Trackball() {}