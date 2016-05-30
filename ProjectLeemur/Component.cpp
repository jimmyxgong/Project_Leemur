#include "Component.h"


Component::~Component() {
	BaseEntity::~BaseEntity();
}


void Component::onStart(GLint glDrawType) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &NBO);

	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	// Bind vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices.size() * sizeof(Vector3f),
		&vertices[0],
		glDrawType
	);

	// Bind indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(unsigned int),
		&indices[0],
		glDrawType
	);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (GLvoid*)0);


	// Bind normals
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		normals.size() * sizeof(Vector3f),
		&normals[0],
		glDrawType
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
	attachedShader = shader;
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
	indices.push_back(i);
	return *this;
}