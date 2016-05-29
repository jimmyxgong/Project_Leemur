#pragma once

#include "Commons.h"
#include "BaseEntity.h"
#include "Camera.h"
#include "Transform.h"

class Centrifuge : public BaseEntity {
private:
	Transform transform;

	UniquePointer<Transform> world;

	SharedPointer<Transform> CreatePod();
	SharedPointer<Transform> CreateMiniBar(float rot_deg);
	SharedPointer<Transform> TripleMiniBars();

	SharedPointer<Transform> CreateBar(float rot_deg);
	SharedPointer<Transform> TripleBars();

public:

	void onCreate() override;
	void onStart() override;
	void onRender() override;
	void onUpdate() override;

private:
	void onDestroy() override;

public:
	Centrifuge();
};