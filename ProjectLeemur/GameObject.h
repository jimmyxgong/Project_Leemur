#pragma once

#include "Component.h"
#include "TransformEntity.h"

class GameObject : public BaseEntity {
private:
	Component * component;

public:
	SharedPointer<Transform> transform;

	void onCreate() override;
	void onStart() override;
	void onRender() override;
	void onUpdate() override;

	virtual void loadToShader();

	GameObject(Component * component);
};