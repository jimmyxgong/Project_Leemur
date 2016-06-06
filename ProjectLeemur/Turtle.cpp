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
    original_position = pos;
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
//    SharedPointer<GameObject> temp =
//    share<GameObject>((Component*)&Resources::getEntity(CYL_OBJ));
//    SharedPointer<Transform> parent = share<Transform>();
//    parent->addChild(temp);
//    parent->scaleLocal(10, .0001, 10);
//    branches->addChild(parent);
    
    // draw the points
    Component& pod = (ObjObject&)Resources::getEntity(SPH_OBJ);
    pod.setMaterial(&Material::Parismarine);
    game_object p = share<GameObject>(&pod);
    transform_group pod_t = share<Transform>();
    pod_t->addChild(p);
    pod_t->scaleLocal(amt/2);
    pod_t->translateLocal(position.x, position.y, position.z);
    pod_t->translateLocal(amt*normalize(Vector3f(direction.x, direction.y*amt, direction.z)));
    pod_t->translateLocal(0, amt/4, 0);

    transform_group rotated_pod = share<Transform>();
    rotated_pod->addChild(pod_t);
    if (position.y > amt*2+original_position.y)
        leaves->addChild(pod_t);
    
    
    
	Component& cylinder = (ObjObject&)Resources::getEntity(CYL_OBJ);
	cylinder.setMaterial(&Material::RedPlastic);
	
    game_object c = share<GameObject>(&cylinder);

    transform_group cyl_t = share<Transform>();
    cyl_t->addChild(c);
    cyl_t->scaleLocal(0.05f, amt/2, 0.05f);
//    cyl_t->scaleLocal(.05);
    cyl_t->translateLocal(0, amt/4, 0);
//    cyl_t->translateLocal(position.x, position.y, position.z);
    
	transform_group rotated = share<Transform>();
    rotated->addChild(cyl_t);
    
    Vector3f up = Vector3f(0,1,0);
    Vector3f new_up = normalize(Vector3f(direction.x, direction.y*amt, direction.z));
    Vector3f rotation_axis = cross(new_up, up);
    float rotation_degree = acos(dot(up, new_up)/(length(up)*length(new_up))) * 360 / (2*M_PI);
    
    rotated->rotateLocal(rotation_axis.x, rotation_axis.y, rotation_axis.z, rotation_degree);
    
    transform_group translated_rotated = share<Transform>();
    translated_rotated->addChild(rotated);
    translated_rotated->translateLocal(position);

    
	branches->addChild(translated_rotated);
//	rotated_recenter->addChild(recenter);
//	recenter->addChild(cyl_t);

	//recenter->translateLocal(0, 2.0f, 0.0f);
//    Quaternion rot = Quaternion(direction.x, direction.y, direction.z, 1);
//	rotated_recenter->rotateLocal(rot);
//    recenter->translateLocal(0, amt * 2,0);
//	recenter->translateLocal(position.x, position.y, position.z);
//		.translateLocal(0, 0.5f, 0);
	

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
