#include "Turtle.h"


Turtle::Turtle(){
	//VAO,VBO,EBO
	mesh = share<Mesh>();
	obj = share<GameObject>((Component *)mesh.get());
	//default up direction
	this->direction = glm::vec3(0, 1, 0);
	//default right direction
	this->right = glm::vec3(1, 0, 0);
}

void Turtle::initialize_mesh() {
	this->mesh->init();
}

void Turtle::setPosition(glm::vec3 pos) {
	this->position = pos;
	mesh->addVertex(this->position);
}

// set direction of UP
void Turtle::setDirection(glm::vec3 dir) {
	this->direction = dir;
}

void Turtle::rotateRight(float angle) {
	float a = angle * glm::pi<float>() / 180.0f;
	glm::vec3 ax = this->direction;
	glm::cross(ax, this->right);
	glm::rotate(direction, a, ax);
	glm::rotate(right, a, ax);
	glm::normalize(direction);
	glm::normalize(right);
}

void Turtle::rotateLeft(float angle) {
	rotateRight(-angle);
}

void Turtle::saveState() {
	stack.push_back(this);
}

void Turtle::restoreState() {
	Turtle * oldTurtle = stack.front();
	stack.pop_back();
	this->position = oldTurtle->position;
	this->direction = oldTurtle->direction;
	this->right = oldTurtle->right;
}

void Turtle::moveForward(float amt) {
	glm::vec3 toAdd = this->direction * amt;
	//old_position = position;
	this->position += toAdd;
}

//for now just add vertice to VBO
void Turtle::drawForward(float amt) {
	glm::vec3 toAdd = this->direction * amt;
	//old_position = position;
	this->position += toAdd;
	mesh->addVertex(this->position);
}

