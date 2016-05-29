#pragma once

#include "Commons.h"
#include "Quaternion.h"
#include "Point.h"
#include "Window.h"

/* Defaults */
#define EPSILON 0.0001
#define ROT_SCALE 0.3f

class Trackball {

private:
	float rotationScale;
	float epsilon;

	Window * window;
	Vector3f toTrackBallPoint(const Point &);
	Vector3f toTrackBallPoint(float x, float y);

public:
	Quaternion fromPoints(const Point & from, const Point & to);

	explicit Trackball(Window* ref, float rotationScale = ROT_SCALE, float epsilon = EPSILON);
	Trackball();
};