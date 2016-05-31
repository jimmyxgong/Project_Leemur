#pragma once

#include "Commons.h"

class MeshData {
public:
	std::vector<Vector3f> vertices;
	std::vector<unsigned int> indices;

	MeshData& addVertex(float x, float y, float z);
	MeshData& addVertex(Vector3f const &);

	MeshData& addTriangle(unsigned int i, unsigned int j, unsigned int k);
	MeshData& addTriangles(std::vector<unsigned int> const & triangles);
	MeshData& addIndex(unsigned int i);

	std::vector<Vector3f>& getVertices();
	std::vector<unsigned int>& getIndices();
};