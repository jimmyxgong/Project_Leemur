#include "Mob.h"


void Mob::move(float x, float y, float z) {
	transform.translateLocal(x, y, z);
}

void Mob::move(Movement m) {
	if (m == Movement::RIGHT) {
		move(1.0f, 0.0f, 0.0f);
	}
	else if (m == Movement::LEFT) {
		move(-1.0f, 0.0f, 0.0f);
	}
	else if (m == Movement::FORWARD) {
		move(0.0f, 0.0f, -1.0f);
	}
	else if (m == Movement::BACKWARD) {
		move(0.0f, 0.0f, 1.0f);
	}
	else if (m == Movement::JUMP) {

	}

}

