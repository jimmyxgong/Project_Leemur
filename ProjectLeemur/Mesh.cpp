#include "Mesh.h"

Mesh& Mesh::updateVertices() {
	Component::onStart(GL_DYNAMIC_DRAW);
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
		Vector3f A = vertices.at(indices.at(i + 0));
		Vector3f B = vertices.at(indices.at(i + 1));
		Vector3f C = vertices.at(indices.at(i + 2));

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