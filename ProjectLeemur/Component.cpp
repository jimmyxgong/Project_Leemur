#include "Component.h"


Component::~Component() {
	BaseEntity::~BaseEntity();
}

void Component::onRender() {
	glBindVertexArray(VAO);
	glDrawElements(
		GL_TRIANGLES,
		indices.size(),
		GL_UNSIGNED_INT,
		0
	);
	glBindVertexArray(0);
}

void Component::onDestroy() {
	BaseEntity::onDestroy();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &NBO);
}


Component& Component::setMaterial(Material * mat) {
	material = mat;
	return *this;
}

Component& Component::attachShader(Shader * shader) {
	this->attachedShader = shader;
	return *this;
}

Shader& Component::getShader() {
	return *attachedShader;
}

Material& Component::getMaterial() {
	return *material;
}

std::vector<Vector3f>& Component::getVertices() {
	return vertices;
}

std::vector<unsigned int>& Component::getIndices() {
	return indices;
}

std::vector<Vector3f>& Component::getNormals() {
	return normals;
}

Component& Component::addVertex(float x, float y, float z) {
	vertices.push_back(Vector3f(x, y, z));
	return *this;
}

Component& Component::addNormal(float x, float y, float z) {
	normals.push_back(Vector3f(x, y, z));
	return *this;
}

Component& Component::addFace(unsigned int x, unsigned int y, unsigned int z) {
	return addIndex(x)
		.addIndex(y)
		.addIndex(z);
}

Component& Component::addIndex(unsigned int i) {
	indices.push_back(i - 1);
	return *this;
}