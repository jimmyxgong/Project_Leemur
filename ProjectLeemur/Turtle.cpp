#include "Turtle.h"
#include "Resources.h"
#include "Material.h"


Turtle::Turtle(){
	//VAO,VBO,EBO
	mesh = share<Mesh>();
	mesh->attachShader(&Resources::getShader(SHADER_LIGHT))
		.setMaterial(&Material::Gold);
	
	obj = share<GameObject>((Component *)mesh.get());
	//default up direction
	this->direction = glm::vec3(0, 1, 0);
	//default right direction
	this->right = glm::vec3(1, 0, 0);
}

void Turtle::initialize_mesh() {
	this->mesh->init();
}

void Turtle::setIndices() {
	for (int i = 0; i < mesh->getVertices().size(); i++) {
		mesh->addIndex(i);
	}
	//mesh->recalculateNormals();
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
	glm::vec3 cross = glm::cross(ax, this->right);
	direction = glm::rotate(direction, a, cross);
	right = glm::rotate(right, a, cross);
	glm::normalize(direction);
	glm::normalize(right);
	//std::cout << "direction: " << direction.x << " " << direction.y << " " << direction.z << std::endl;
}

void Turtle::rotateLeft(float angle) {
	rotateRight(-angle);
}

void Turtle::saveState() {
	Turtle t = (Turtle)*this;
	std::cout << "saved position: " << t.position.x << " " << t.position.y << " " << t.position.z << std::endl;
	std::cout << "saved direction: " << t.direction.x << " " << t.direction.y << " " << t.direction.z << std::endl;
	stack.push_back(t);
}

void Turtle::restoreState() {
	Turtle oldTurtle = stack.back();
	std::cout << "restored position: " << oldTurtle.position.x << " " << oldTurtle.position.y << " " << oldTurtle.position.z << std::endl;
	std::cout << "restored direction: " << oldTurtle.direction.x << " " << oldTurtle.direction.y << " " << oldTurtle.direction.z << std::endl;
	stack.pop_back();
	this->position = oldTurtle.position;
	this->direction = oldTurtle.direction;
	this->right = oldTurtle.right;
}

void Turtle::moveForward(float amt) {
	glm::vec3 toAdd = this->direction * amt;
	//old_position = position;
	this->position += toAdd;
}

//for now just add vertice to VBO
void Turtle::drawForward(float amt) {
	//std::cout << "direction: " << direction.x << " " << direction.y << " " << direction.z << std::endl;
	glm::vec3 toAdd = this->direction * amt;
	//old_position = position;
	this->position += toAdd;
	mesh->addVertex(this->position);
}

