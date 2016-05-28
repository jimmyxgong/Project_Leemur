#include "Component.h"


Component::~Component() {
	onDestroy();
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

const std::vector<Vector3f>& Component::getVertices() const {
	return vertices;
}

const std::vector<Vector3f>& Component::getNormals() const {
	return normals;
}