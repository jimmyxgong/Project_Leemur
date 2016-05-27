#include "Camera.h"
#include "Window.h"



Matrix4f Camera::viewMatrix() {
	Matrix4f rotation;
	Quaternion::toMatrix(transform.localRotation, rotation);
	return glm::lookAt(transform.localPosition, CAMERA_LOOK_AT, CAMERA_UP_VECTOR) * rotation;
}

void Camera::apply(Matrix4f const & value) {
	relativeWorldMatrix = glm::inverse(value);
}

void Camera::use() {
	Window::getFocusedWindow()
		.setView(OriginViewMatrix * relativeWorldMatrix);
}




Camera::Camera(Vector3f position) {
	transform.localPosition = position;
	OriginViewMatrix = glm::lookAt(position, CAMERA_LOOK_AT, CAMERA_UP_VECTOR);
}

Camera::Camera() {
	transform.localPosition = CAMERA_POSITION;
	OriginViewMatrix = glm::lookAt(CAMERA_POSITION, CAMERA_LOOK_AT, CAMERA_UP_VECTOR);
}

Camera::~Camera() {

}