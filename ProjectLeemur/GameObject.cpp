#include "GameObject.h"
#include "Window.h"
#include "Camera.h"
#include "ObjObject.h"

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
	//Mesh * mesh = new Mesh();
	//addComponent<Mesh>(*mesh);
}

void GameObject::onRender() {
	//getComponent<Mesh>().render();
	//getComponent<ObjObject>();
	//if (m != *Component::EMPTY)
	if (component->attachedShader) 
		loadToShader();
	((Mesh*)component)->render();
}

void GameObject::onUpdate() {

}

template <class T>
T & GameObject::getComponent() {
	const std::type_info & id = typeid(T);
	auto & val = components.find(id);
	if (val != components.end()) {
		return (T &) *val->second;
	}

	std::cout << "Empty component returned" << std::endl;
	return (T&) (T());
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
	: component(component)
{
	onCreate();
}