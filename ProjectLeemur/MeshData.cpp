#include "MeshData.h"


MeshData& MeshData::addVertex(float x, float y, float z) {
	return addVertex(Vector3f(x, y, z));
}

MeshData& MeshData::addVertex(Vector3f const & vec) {
	vertices.push_back(vec);
	return *this;
}

MeshData& MeshData::addTriangles(std::vector<unsigned int> const & triangles) {
	int count = vertices.size();
	for (unsigned int const & index : triangles) {
		addIndex(index + count);
	}
	return *this;
}

MeshData& MeshData::addTriangle(unsigned int i, unsigned int j, unsigned int k) {
	int count = vertices.size();
	addIndex(i + count);
	addIndex(j + count);
	addIndex(k + count);
	return *this;
}

MeshData& MeshData::addIndex(unsigned int i) {
	indices.push_back(i);
	return *this;
}



std::vector<Vector3f>& MeshData::getVertices() {
	return vertices;
}

std::vector<unsigned int>& MeshData::getIndices() {
	return indices;
}
