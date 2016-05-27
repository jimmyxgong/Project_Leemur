#pragma once

#include "Commons.h"
#include "TransformEntity.h"
#include "Camera.h"

class Player : public TransformEntity {
private:
	UniquePointer<Camera> camera;

public:
	void onStart() override;
	void onRender() override;
	void onUpdate() override;

	Camera & getCamera() const;
};