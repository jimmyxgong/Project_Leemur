#include "Camera.h"
#include "Window.h"


// TODO: FIXME Needs to use better Camera API
Matrix4f Camera::viewMatrix() {
	return glm::lookAt(transform.getLocalPosition(), lookAt, upVector) * transform.getLocalRotation().toMatrix();
}

//void Camera::use() {
//	Window::getFocusedWindow()
//		.setView(OriginViewMatrix * relativeWorldMatrix);
//}




Camera& Camera::setLookAt(const Vector3f & val) {
	lookAt = val;
	return *this;
}

Camera& Camera::setUpVector(const Vector3f & val) {
	upVector = val;
	return *this;
}






Camera::Camera(Vector3f position, Vector3f lookat, Vector3f upvector) :
	lookAt(lookat), upVector(upvector)
{
	transform.setLocalPosition(position);
	view = glm::lookAt(position, lookat, upvector);
}

Camera::Camera(const Camera & cam) {
	lookAt = cam.lookAt;
	upVector = cam.upVector;
	transform = cam.transform;
	view = cam.view;
}

Camera::~Camera() {

}