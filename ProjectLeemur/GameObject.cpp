#include "GameObject.h"
#include "Window.h"
#include "Camera.h"

/* Used for type_id mapping */
std::size_t Hasher::operator()(TypeRef code) const {
	return code.get().hash_code();
}

bool Comparator::operator()(TypeRef lhs, TypeRef rhs) const {
	return lhs.get() == rhs.get();
}








void GameObject::onCreate() {
	transform = share<Transform>();
}

void GameObject::onStart() {
	Mesh * mesh = new Mesh();
	addComponent<Mesh>(*mesh);
}

void GameObject::onRender() {
	if (component->attachedShader) 
		loadToShader();
	component->onRender();
}

void GameObject::onUpdate() {

}

template <class T>
auto & GameObject::getComponent() {
	const std::type_info & id = typeid(T);
	auto & val = components.find(id);
	if (val != components.end()) {
		return *val->second;
	}

	std::cout << "Empty component returned" << std::endl;
	return Component::EMPTY;
}

template <class T>
void GameObject::addComponent(T const & val) {
	components.emplace(typeid(T), (Component*) &val);
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

	Camera & cam = Window::getFocusedWindow().getActiveCamera();
	Shader::loadVector("CameraPosition", cam.transform.getLocalPosition());
}

GameObject::GameObject(Component * component)
{

	onCreate();
}