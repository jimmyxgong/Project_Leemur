#pragma once
#include "Commons.h"
#include "Transform.h"

#define CAMERA_POSITION   Vector3f(0.0f, 0.0f, 2.0f)
#define CAMERA_LOOK_AT    Vector3f(0.0f, 0.0f, 0.0f)
#define CAMERA_UP_VECTOR  Vector3f(0.0f, 1.0f, 0.0f)

class Camera {
private:
	Matrix4f OriginViewMatrix;
	Matrix4f relativeWorldMatrix;

public:
	Transform transform;

	// TODO: Create * operator's
	Matrix4f operator*(Matrix4f&);
	Matrix4f viewMatrix();

	void apply(Matrix4f const &);
	void use();

	explicit Camera(Vector3f position);
	Camera();
	~Camera();
};