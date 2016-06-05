#include "Resources.h"
#include "ObjObject.h"
#include "Light.h"

Resources & Resources::getInstance() {
	static Resources res;
	return res;
}

void Resources::init() {
	Light::init();
#ifdef _WIN32
	// Create Pod object and add it to resources.
	Resources::newObjEntity(POD_OBJ, "pod.obj")
		.attachShader(&Resources::getShader(TOON_LIGHT))
		.setMaterial(&Material::RedPlastic);

	// Create Cylinder object and add it to resources.
	Resources::newObjEntity(CYL_OBJ, "cylinder.obj")
		.attachShader(&Resources::getShader(TOON_LIGHT))
		.setMaterial(&Material::Gold);

	// Create Icosahedron object and add it to resources.
	/*Resources::newObjEntity(ICOSAHEDRON_OBJ, "icosahedron.obj")
		.attachShader(&Resources::getShader(TOON_LIGHT))
		.setMaterial(&Material::Gold);*/

#else
    // Create Pod object and add it to resources.
    Resources::newObjEntity(POD_OBJ, "/Users/sebastian/Google Drive/College/Year 3/Spring 16/ProjectLeemur/ProjectLeemur/pod.obj")
    .attachShader(&Resources::getShader(TOON_LIGHT))
    .setMaterial(&Material::RedPlastic);
    
    // Create Cylinder object and add it to resources.
    Resources::newObjEntity(CYL_OBJ, "/Users/sebastian/Google Drive/College/Year 3/Spring 16/ProjectLeemur/ProjectLeemur/cylinder.obj")
    .attachShader(&Resources::getShader(TOON_LIGHT))
    .setMaterial(&Material::Gold);
#endif



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
