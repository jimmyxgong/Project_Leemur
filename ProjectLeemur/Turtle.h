#pragma once

#include "Mesh.h"
#include "Commons.h"
#include "GameObject.h"
#include "Quaternion.h"
#include "glm/gtx/rotate_vector.hpp"

class Turtle : public BaseEntity{
public:
	Turtle();

	SharedPointer<Transform> world;
	SharedPointer<Transform> branches;
	SharedPointer<Transform> leaves;
	SharedPointer<Mesh> mesh;
	SharedPointer<GameObject> obj;
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 right;
	std::vector<Turtle> stack;
	float distance;

	void onCreate();
	void onStart();
	void onRender();
	void onUpdate();
	void onDestroy();

	void initialize_mesh();
	void setIndices();
	void drawForward(float amt);
	void moveForward(float amt);
	void rotateRight(float angle);
	void rotateLeft(float angle);
	void rollLeft(float angle);
	void rollRight(float angle);
	void pitchUp(float angle);
	void pitchDown(float angle);
	void turnAround();
	void setPosition(glm::vec3 pos);
	void setDirection(glm::vec3 dir);
	void saveState();
	void restoreState();
};