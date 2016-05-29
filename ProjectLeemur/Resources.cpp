#include "Resources.h"
#include "ObjObject.h"

Resources & Resources::getInstance() {
	static Resources res;
	return res;
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

Resources & Resources::addShader
	(int key, 
	 Shader * value) 
{
	getInstance()
		.shaders
		.emplace(key, value);

	return getInstance();
}

Entity & Resources::getEntity(int key) {
	return *getInstance()
			.entities
			.at(key);
}

Resources & Resources::addEntity(int key, Entity * value) {
	getInstance()
		.entities
		.emplace(key, value);

	return getInstance();
}

Resources & Resources::newObjEntity(int key, std::string const & obj) {
	return addEntity(key, ObjObject::create(obj));
}



void Resources::destroy() {
	for (auto shader : getInstance().shaders) {
		Shader* value = shader.second;
		glDeleteProgram(value->getId());
	}
}
