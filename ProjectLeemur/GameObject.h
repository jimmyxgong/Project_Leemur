#pragma once

#include "Component.h"
#include "TransformEntity.h"
#include "Mesh.h"

class GameObject : public BaseEntity {
private:
	Component * component;

public:
	// TODO: singular component API
	UniquePointer<Mesh> mesh;
	SharedPointer<Transform> transform;

	void onCreate() override;
	void onStart() override;
	void onRender() override;
	void onUpdate() override;

	virtual void loadToShader();

	GameObject(Component * component);
};