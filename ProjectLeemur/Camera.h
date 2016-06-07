#pragma once

#include "Commons.h"
#include "Transform.h"

/* Default values for the camera */
#define CAMERA_POSITION   Vector3f(0.0f, 0.0f, 2.0f)
#define CAMERA_LOOK_AT    Vector3f(0.0f, 6.f, 0.0f)
#define CAMERA_UP_VECTOR  Vector3f(0.0f, 1.0f, 0.0f)

class Camera {
private:
	Matrix4f OriginViewMatrix;
	Matrix4f relativeWorldMatrix;

	Vector3f upVector;
	Matrix4f view;

public:
	Transform transform;
	Vector3f lookAt;


	Matrix4f viewMatrix();
	Camera& setLookAt(const Vector3f & val);
	Camera& setUpVector(const Vector3f & val);

	void update();

	//void use();

	explicit Camera(Vector3f position = CAMERA_POSITION, 
					Vector3f lookat = CAMERA_LOOK_AT, 
					Vector3f upVector = CAMERA_UP_VECTOR);
	
	Camera(const Camera &);
	~Camera();
};