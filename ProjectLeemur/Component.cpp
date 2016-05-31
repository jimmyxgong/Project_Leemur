#include "Component.h"

SharedPointer<Component> Component::EMPTY = share<Component>();

void Component::onCreate() {}
void Component::onStart() {}
void Component::onRender() {}
void Component::onUpdate() {}
void Component::onDestroy() {}




Component::~Component() {
}




Component& Component::setMaterial(Material * mat) {
	material = mat;
	return *this;
}

Component& Component::attachShader(Shader * shader) {
	attachedShader = shader;
	return *this;
}

Shader& Component::getShader() {
	return *attachedShader;
}

Material& Component::getMaterial() {
	return *material;
}
