#include "Mesh.h"


void Mesh::onStart() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &NBO);

	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices.size() * sizeof(Vector3f),
		&vertices[0],
		GL_DYNAMIC_DRAW
	);

	// Bind indices to the vertices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(unsigned int),
		&indices[0],
		GL_STATIC_DRAW
	);

}

void Mesh::onRender() {
}

void Mesh::onUpdate() {

}



void Mesh::onCreate() {}
void Mesh::onDestroy() {
	Component::onDestroy();
}