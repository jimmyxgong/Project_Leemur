#include "Centrifuge.h"
#include "Environment.h"
#include "GameObject.h"
#include "Transform.h"
#include "Quaternion.h"
#include "Time.h"
#include "Light.h"

#include "Resources.h"
#include "ObjObject.h"

#include <random>

typedef SharedPointer<Transform>		transform_group;
typedef SharedPointer<GameObject>		game_object;

// Random number generator for 
std::random_device rng;
std::uniform_int_distribution<> distribution(1, 5);
std::uniform_real_distribution<float> float_distribution(0.0f, 0.05f);

void Centrifuge::onCreate() {
	world = unique<Transform>();

	// Origin of the the centrifuge
	transform.scaleLocal(0.2f, 0.2f, 0.2f);
}

transform_group Centrifuge::CreatePod() {
	static Component& pod = (ObjObject&) Resources::getEntity(POD_OBJ);
	static Component& cyl = (ObjObject&) Resources::getEntity(CYL_OBJ);


	transform_group parent = share<Transform>();
	parent->translateLocal(0.0f, 0.18f, 0.1f);

	transform_group pod_center = share<Transform>();
	pod_center->rotateLocal(1.0f, 0.0f, 0.0f, 270.0f)
		.rotateLocal(0.0f, 1.0f, 0.0f, 90.0f);
		
	game_object podObject = share<GameObject>(&pod);
	podObject->transform->scaleLocal(0.4f, 0.4f, 0.4f);
	pod_center->addChild(podObject);
	parent->addChild(pod_center);
	return parent;
}

transform_group Centrifuge::CreateMiniBar(float rotation) {
	static Component& cyl = (ObjObject&)Resources::getEntity(CYL_OBJ);

	transform_group center = share<Transform>();
	center->translateLocal(0.0f, 0.18f, 0.0f)
		.rotateLocal(0.0f, 0.0f, 1.0f, rotation);

		// Add in the bar and pod
		game_object bar = share<GameObject>(&cyl);
		bar->transform->translateLocal(0.0f, -0.1f, 0.0f)
			.scaleLocal(0.125f, 0.5f, 0.125f);
		center->addChild(bar);

		transform_group sub = share<Transform>();

			game_object bar2 = share<GameObject>(&cyl);
			bar2->transform->scaleLocal(0.125f, 0.5f, 0.125f)
				.translateLocal(0.0f, 0.1f, 0.0f);
			sub->addChild(bar2);
			sub->addChild(CreatePod());
		center->addChild(sub);

	return center;
}


transform_group Centrifuge::TripleMiniBars() {
	transform_group parent = share<Transform>();
	parent->translateLocal(0.0f, 0.55f, 0.06f);

	// Controls the rotation of the mini bars
	transform_group center = share<Transform>();
	center->rotateLocal(0.0f, 0.0f, 1.0f, 120.0f);

	parent->addChild(center);

	center->addChild(CreateMiniBar(0.0f));
	center->addChild(CreateMiniBar(120.0f));
	center->addChild(CreateMiniBar(240.0f));

	return parent;
}


transform_group Centrifuge::CreateBar(float rotationDeg) {
	static Component& cyl = (ObjObject&)Resources::getEntity(CYL_OBJ);


	transform_group center = share<Transform>();
	center->rotateLocal(0.0f, 0.0f, 1.0f, rotationDeg)
		.translateLocal(0.0f, 0.6f, 0.0f);

		game_object bar = share<GameObject>(&cyl);
		bar->transform
			->translateLocal(0.0f, -0.3f, 0.0f)
			.scaleLocal(0.25f, 1.5f, 0.25f);	// 3.0f
		center->addChild(bar);

		transform_group sub = share<Transform>();
			game_object bar2 = share<GameObject>(&cyl);
			bar2->transform
				->translateLocal(0.0f, 0.3f, 0.0f)
				.scaleLocal(0.25f, 1.5f, 0.25f);
			sub->addChild(bar2);
			sub->addChild(TripleMiniBars());
		center->addChild(sub);

	return center;
}

transform_group Centrifuge::TripleBars() {
	transform_group center = share<Transform>();
	center->addChild(CreateBar(0.0f));
	center->addChild(CreateBar(120.0f));
	center->addChild(CreateBar(240.0f));

	return center;
}

void Centrifuge::onStart() {
	static Component& cyl = (ObjObject&)Resources::getEntity(CYL_OBJ);

	// The center of the centrifuge:
	transform_group centrifuge_center = share<Transform>();
	centrifuge_center->rotateLocal(0.0f, 1.0f, 0.0f, 90.0f);
	world->addChild(centrifuge_center);



	// The bottom pillar controlling the transitioning of the middle cross bars
	transform_group center_pillar_bottom = share<Transform>();
	center_pillar_bottom->translateLocal(0.0f, 1.3f, 0.0f);
		// add in the pillar
		game_object pillar_bottom = share<GameObject>(&cyl);
		pillar_bottom->transform
			->translateLocal(0.0f, -0.7f, 0.0f)
			.scaleLocal(0.25f, 3.0f, 0.25f);
		center_pillar_bottom->addChild(pillar_bottom);
	centrifuge_center->addChild(center_pillar_bottom);
	


	// The top pillar controlling the transitioning of the top cross bars
	transform_group center_pillar_top = share<Transform>();		
	center_pillar_top->translateLocal(0.0f, 2.0f, 0.0f);
	centrifuge_center->addChild(center_pillar_top);
		// Add in the pillar
		game_object pillar_top = share<GameObject>(&cyl);
		pillar_top->transform
			->translateLocal(0.0f, -0.5f, 0.0f)
			.scaleLocal(0.25f, 3.0f, 0.25f);
	center_pillar_top->addChild(pillar_top);


	// Bottom cross pillars. This at center
	transform_group bottom = TripleBars();
	centrifuge_center->addChild(bottom);
	bottom->rotateLocal(1.0f, 0.0f, 0.0f, 90.0f);

	// Mid cross pillars. This at center
	transform_group mid = TripleBars();
	mid->rotateLocal(1.0f, 0.0f, 0.0f, 90.0f)
		.rotateLocal(0.0f, 0.0f, 1.0f, 90.0f)
		.translateLocal(0.0f, 0.0f, -0.2f);
	center_pillar_bottom->addChild(mid);

	// Top cross pillars. This at center
	transform_group top = TripleBars();
	top->rotateLocal(1.0f, 0.0f, 0.0f, 90.0f)
		.rotateLocal(0.0f, 0.0f, 1.0f, 195.0f);
	center_pillar_top->addChild(top);
	world->locallyUpdate(transform.asMatrix());
}

void Centrifuge::onRender() {
	Light::Directional.loadToShader();
	world->renderAll();
}

void Centrifuge::onUpdate() {
	world->updateAll();
}


void Centrifuge::onDestroy() {
	
}

Centrifuge::Centrifuge() {
	onCreate();
}