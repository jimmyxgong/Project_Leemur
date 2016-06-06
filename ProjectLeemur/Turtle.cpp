#include "Turtle.h"
#include "Resources.h"
#include "Material.h"
#include "Light.h"

typedef SharedPointer<Transform>		transform_group;
typedef SharedPointer<GameObject>		game_object;

Turtle::Turtle(){
	world = share<Transform>();
	branches = share<Transform>();
	leaves = share<Transform>();
	//default up direction
	this->direction = glm::vec3(0, 1, 0);
	//default right direction
	this->right = glm::vec3(1, 0, 0);
}

void Turtle::initialize_mesh() {
	this->mesh->init();
}

void Turtle::onCreate() {
	world->addChild(branches)
		.addChild(leaves);
}

void Turtle::onStart() {}

void Turtle::onRender() {
	Resources::getShader(SHADER_LIGHT);
	Light::Directional.loadToShader();
	world->renderAll();
}

void Turtle::onUpdate() {
	world->updateAll();
}

void Turtle::onDestroy() {

}

void Turtle::setIndices() {
	int pos = 0;
	int count = 0;
	for (int i = 0; i < mesh->getVertices().size(); i++) {
		if (count == 3) {
			mesh->addIndex(pos);
			mesh->addIndex(pos+1);
			mesh->addIndex(pos+2);
			pos += 3;
			count = 0;
		}
		else count++;
		//mesh->addIndex(i);
	}
	//obj->transform->scaleLocal(0.5f);
	//mesh->recalculateNormals();
}

void Turtle::setPosition(glm::vec3 pos) {
	this->position = pos;
	//mesh->addVertex(this->position);
}

// set direction of UP
void Turtle::setDirection(glm::vec3 dir) {
	this->direction = dir;
}

void Turtle::rotateRight(float angle) {
	float a = angle * glm::pi<float>() / 180.0f;
	glm::vec3 ax = this->direction;
	glm::vec3 cross = glm::cross(ax, this->right);
	this->direction = glm::rotate(direction, a, cross);
	this->right = glm::rotate(right, a, cross);
	glm::normalize(this->direction);
	glm::normalize(this->right);
}

void Turtle::rotateLeft(float angle) {
	rotateRight(-angle);
}

void Turtle::saveState() {
	Turtle t = (Turtle)*this;
	this->stack.push_back(t);
}

void Turtle::restoreState() {
	Turtle oldTurtle = stack.back();
	stack.pop_back();
	this->position = oldTurtle.position;
	this->direction = oldTurtle.direction;
	this->right = oldTurtle.right;
}

void Turtle::moveForward(float amt) {
	glm::vec3 toAdd = this->direction * amt;
	this->position += toAdd;
}

//for now just add vertice to VBO
void Turtle::drawForward(float amt) {
	Component& cylinder = (ObjObject&)Resources::getEntity(CYL_OBJ);
	cylinder.setMaterial(&Material::RedPlastic);
	transform_group recenter = share<Transform>();
	transform_group cyl_t = share<Transform>();
	transform_group rotated_recenter = share<Transform>();

	game_object c = share<GameObject>(&cylinder);
	branches->addChild(rotated_recenter);
	rotated_recenter->addChild(recenter);
	recenter->addChild(cyl_t);
	cyl_t->addChild(c);

	//recenter->translateLocal(0, 2.0f, 0.0f);
	rotated_recenter->rotateLocal(Quaternion(direction.x, direction.y, direction.z, 1));
	recenter->translateLocal(position.x, position.y, position.z);
	cyl_t->scaleLocal(0.1f, amt * 2, 0.1f)
		.translateLocal(0, 0.5f, 0);
	

	//c->transform->rotateLocal(direction.x, direction.y, direction.z, 23 );
	//c->transform->translateLocal(this->position);

	glm::vec3 toAdd = this->direction * amt;
	this->position += toAdd;


	//mesh->addVertex(this->position);
	//std::cout << "curr direction: " << this->direction.x << " " << this->direction.y << " " << this->direction.z << std::endl;
	//std::cout << "curr position: " << this->position.x << " " << this->position.y << " " << this->position.z << std::endl;
	//std::cout << "position: " << c->transform->getPosition().x << " " << c->transform->getPosition().y << c->transform->getPosition().z << std::endl;
	std::cout << "branches size: " << branches->children.size() << std::endl;
}

void Turtle::pitchUp(float amt) {
	float ang = amt * glm::pi<float>() / 180.0f;
	this->direction = glm::rotate(this->direction, ang, right);
	glm::normalize(this->direction);
}

void Turtle::pitchDown(float amt) {
	pitchUp(-amt);
}

void Turtle::rollRight(float amt) {
	float ang = amt * glm::pi<float>() / 180.0f;
	this->right = glm::rotate(this->right, ang, direction);
	glm::normalize(this->right);
}

void Turtle::rollLeft(float amt) {
	rollRight(-amt);
}

void Turtle::turnAround() {
	rollRight(glm::pi<float>());
}
