#include "Resources.h"
#include "ObjObject.h"
#include "Light.h"

Resources & Resources::getInstance() {
	static Resources res;
	return res;
}

void Resources::init() {
	Light::init();

	// Create Pod object and add it to resources.
	Resources::newObjEntity(POD_OBJ, "pod.obj")
		.attachShader(&Resources::getShader(SHADER_LIGHT))
		.setMaterial(&Material::RedPlastic);

	// Create Cylinder object and add it to resources.
	Resources::newObjEntity(CYL_OBJ, "cylinder.obj")
		.attachShader(&Resources::getShader(SHADER_LIGHT))
		.setMaterial(&Material::Gold);





}


Shader & Resources::getShader(const int key) {
	return *(getInstance().shaders.at(key));
}

Resources & Resources::addShader
	(int key,
	 std::string const & vert, 
	 std::string const & frag) 
{
	GLuint shaderId = LoadShaders(vert.c_str(), frag.c_str());

	getInstance()
		.shaders
		.emplace(key, new Shader(shaderId));
	return getInstance();
}

Resources & Resources::addShader(int key, Shader * value) {
	getInstance()
		.shaders
		.emplace(key, value);

	return getInstance();
}

Entity & Resources::getEntity(int key) {
	return *(getInstance().entities.at(key));
}

Resources & Resources::addEntity(int key, Entity * value) {
	getInstance()
		.entities
		.emplace(key, value);

	return getInstance();
}

ObjObject & Resources::newObjEntity(int key, const std::string & obj) {
	ObjObject * ob = ObjObject::create(obj);
	addEntity(key, (Entity*) ob);
	return *ob;
}

void Resources::destroy() {
	for (auto shader : getInstance().shaders) {
		Shader* value = shader.second;
		value->destroy();
		//glDeleteProgram(value->getId());
	}
}
