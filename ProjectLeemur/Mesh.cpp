#include "Mesh.h"

void Mesh::updateMeshData() {
	const static GLint offset = 0;

	// update vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(
		GL_ARRAY_BUFFER,
		offset, 
		vertices.size() * sizeof(Vector3f), 
		&vertices[0]
	);

	// update indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferSubData(
		GL_ELEMENT_ARRAY_BUFFER,
		offset,
		indices.size() * sizeof(unsigned int),
		&indices[0]
	);

	// update normals
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferSubData(
		GL_ARRAY_BUFFER,
		offset,
		normals.size() * sizeof(Vector3f),
		&normals[0]
	);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::onStart() {
	Component::onStart(GL_DYNAMIC_DRAW);
	changed = false;
}

void Mesh::onUpdate() {

}

Mesh& Mesh::clear() {
	vertices.clear();
	indices.clear();
	normals.clear();
	return *this;
}

Mesh& Mesh::recalculateNormals() {
	for (int i = 0; i < indices.size(); i += 3) {
		Vector3f A = vertices.at(indices.at(i + 0));
		Vector3f B = vertices.at(indices.at(i + 1));
		Vector3f C = vertices.at(indices.at(i + 2));

		Vector3f BA = B - A;
		Vector3f CA = C - A;

		Vector3f N = normalize(cross(BA, CA));

		if (normals.size() > i + 2) {
			Vector3f NA = normals.at(indices.at(i + 0));
			Vector3f NB = normals.at(indices.at(i + 1));
			Vector3f NC = normals.at(indices.at(i + 2));

			addNormal(N + NA);
			addNormal(N + NB);
			addNormal(N + NC);
			continue;
		}
		addNormal(N);
		addNormal(N);
		addNormal(N);
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
		addIndex(index + count);
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