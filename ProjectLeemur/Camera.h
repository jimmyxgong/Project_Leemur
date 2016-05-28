#pragma once
#include "Commons.h"
#include "Transform.h"

/* Default values for the camera */
#define CAMERA_POSITION   Vector3f(0.0f, 0.0f, 2.0f)
#define CAMERA_LOOK_AT    Vector3f(0.0f, 0.0f, 0.0f)
#define CAMERA_UP_VECTOR  Vector3f(0.0f, 1.0f, 0.0f)

class Camera {
private:
	Matrix4f OriginViewMatrix;
	Matrix4f relativeWorldMatrix;

	Vector3f lookAt;
	Vector3f upVector;
	Matrix4f view;

public:
	Transform transform;


	Matrix4f viewMatrix();
	Camera& setLookAt(const Vector3f & val);
	Camera& setUpVector(const Vector3f & val);


	//void use();

	explicit Camera();
	explicit Camera(Vector3f position);
	explicit Camera(Vector3f position, Vector3f lookat);
	explicit Camera(Vector3f position, Vector3f lookat, Vector3f upVector);
	
	Camera(const Camera &);
	~Camera();
};