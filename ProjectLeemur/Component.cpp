#include "Component.h"


Component::~Component() {
	BaseEntity::~BaseEntity();
}

void Component::onDestroy() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &NBO);
}

void Component::renderVaoWithIndices() {
	glBindVertexArray(VAO);
	glDrawElements(
		GL_TRIANGLES,
		indices.size(),
		GL_UNSIGNED_INT,
		0
	);
	glBindVertexArray(0);
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

std::vector<Vector3f>& Component::getNormals() {
	return normals;
}