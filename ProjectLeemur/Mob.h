#pragma once

#include "TransformEntity.h"

class Mob : public TransformEntity {

	//void onRender() override;
	//void onUpdate() override;
protected:
	enum Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		JUMP
	};

	virtual void move(float x, float y, float z);
	virtual void move(Movement move);

};