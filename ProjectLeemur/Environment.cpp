#include "Environment.h"
#include "Resources.h"
#include "ObjObject.h"
#include "Centrifuge.h"
#include "Light.h"
#include "Chunk.h"

/* 
	See the centrifuge for an example as to how to use
	the scene graph. NOTE: Animations are not implemented in
	the scene graph yet.
*/
SharedPointer<Centrifuge> centrifuge;
Chunk chunk;

// TODO LOGGER
void print(const char * const & val) {
	std::cout << val << std::endl;
}

void Environment::onCreate() {
	print("Creating Environment...");

	Light::init();

	skybox = unique<Skybox>(window);
	player = unique<Player>();
	Resources::addEntity(SKYBOX, skybox.get());
	Resources::addEntity(PLAYER, player.get());

	// Create Pod object and add it to resources.
	Resources::newObjEntity(POD_OBJ, "pod.obj")
		.attachShader(&Resources::getShader(SHADER_LIGHT))
		.setMaterial(&Material::RedPlastic);

	// Create Cylinder object and add it to resources.
	Resources::newObjEntity(CYL_OBJ, "cylinder.obj")
		.attachShader(&Resources::getShader(SHADER_LIGHT))
		.setMaterial(&Material::Gold);

	centrifuge = unique<Centrifuge>();

	Window::getFocusedWindow()
		.setActiveCamera(&player->getCamera());

	chunk.onCreate();
}

void Environment::onStart() {
	print("Environment starting...");
	skybox->onStart();
	player->onStart();



	chunk.onStart();
	//centrifuge->onStart();
	//world.addChild(player->transform);
}

void loadToShader() {
	//component->getShader().use();
	//component->getMaterial().loadToShader();

	Matrix4f model = Matrix4f(1.0f);// transform->getLocalToWorldMatrix();
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


void Environment::onRender() {
	skybox->onRender();
	player->onRender();
	chunk.onRender();

	//Resources::getShader(SHADER_LIGHT).use();
	//Light::Directional.loadToShader();
	//Material::RedPlastic.loadToShader();
	//loadToShader();
	//Resources::getEntity(POD_OBJ).onRender();

	//centrifuge->onRender();
}

void Environment::onUpdate() {
	player->onUpdate();
	//centrifuge->onUpdate();
}

void Environment::onDestroy() {
	delete (&Resources::getShader(SHADER_LIGHT));

	skybox->onDestroy();

	Component& pod = (Component&) Resources::getEntity(POD_OBJ);
	Component& cyl = (Component&) Resources::getEntity(CYL_OBJ);
	pod.onDestroy();
	cyl.onDestroy();

	delete &pod;
	delete &cyl;


}

UniquePointer<Environment> Environment::create(Window * ref) {
	UniquePointer<Environment> instance = unique<Environment>();
	instance->window = ref;
	return instance;
}