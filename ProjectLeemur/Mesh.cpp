#include "Mesh.h"

Mesh& Mesh::updateVertices() {
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
		GL_DYNAMIC_DRAW
	);

	// Bind indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(unsigned int),
		&indices[0],
		GL_DYNAMIC_DRAW
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
		GL_DYNAMIC_DRAW
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return *this;
}

void Mesh::onStart() {
	updateVertices();
	changed = false;
}

void Mesh::onRender() {
	Component::onRender();
}

void Mesh::onUpdate() {
	if (hasChanged()) {
		updateVertices();
		changed = false;
	}
}


Mesh& Mesh::recalculateNormals() {
	for (int i = 0; i < indices.size(); i += 3) {
		Vector3f A = vertices.at(indices.at(i+0));
		Vector3f B = vertices.at(indices.at(i+1));
		Vector3f C = vertices.at(indices.at(i+2));

		Vector3f BA = B - A;
		Vector3f CA = C - A;

		Vector3f N = normalize(cross(BA, CA));
		addNormal(N.x, N.y, N.z);
		addNormal(N.x, N.y, N.z);
		addNormal(N.x, N.y, N.z);
	}

	return *this;
}

Mesh& Mesh::setVertices(std::vector<Vector3f> const & vertices) {
	this->vertices = vertices;
	changed = true;
	return *this;
}

Mesh& Mesh::setNormals(std::vector<Vector3f> const & normals) {
	this->normals = normals;
	changed = true;
	return *this;
}

Mesh& Mesh::setTriangles(std::vector<unsigned int> const & indices) {
	this->indices = indices;
	changed = true;
	return *this;
}

Mesh& Mesh::addTriangle(unsigned int i, unsigned int j, unsigned int k) {
	int count = vertices.size();
	addFace(i + count, j + count, k + count);
	return *this;
}

Mesh& Mesh::addTriangles(std::vector<unsigned int> const & triangles) {
	int count = vertices.size();
	for (unsigned int const & index : triangles) {
		//addIndex(index + count);
		indices.push_back(index + count);
	}
	return *this;
}

bool Mesh::hasChanged() const {
	return changed;
}


void Mesh::onCreate() {}
void Mesh::onDestroy() {
	Component::onDestroy();
}