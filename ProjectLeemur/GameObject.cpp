#include "GameObject.h"
#include "Window.h"
#include "Camera.h"

void GameObject::onCreate() {
	transform = share<Transform>();
}

void GameObject::onStart() {

}

void GameObject::onRender() {
	if (component->attachedShader) 
		loadToShader();
	component->onRender();
}

void GameObject::onUpdate() {

}

void GameObject::loadToShader() {
	component->getShader().use();
	component->getMaterial().loadToShader();

	Matrix4f model = transform->getLocalToWorldMatrix();
	Matrix4f MVP = Window::getFocusedWindow().getPerspective()
		* Window::getFocusedWindow().getView()
		* model;

	Shader::loadMatrix("MVP", MVP);
	Shader::loadMatrix("model", model);
	Shader::loadMatrix("NormalMatrix", Matrix3f(transpose(inverse(model))));

	// TODO: there might be some side-effects of using CAMERA_POSITION
	// instead of using the actual Window active camera. Too lazy to
	// write another method to get the active camera.
	Shader::loadVector("CameraPosition", CAMERA_POSITION);
}

GameObject::GameObject(Component * component) :
	component(component)
{
	onCreate();
}