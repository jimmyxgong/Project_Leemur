#include "Mesh.h"

using VecRef = std::reference_wrapper<const Vector3f>;

static int hashCode(const char * bytes, int numBytes)
{
	unsigned long h = 0, g;
	for (int i = 0; i<numBytes; i++)
	{
		h = (h << 4) + bytes[i];
		if (g = h & 0xF0000000L) { h ^= g >> 24; }
		h &= ~g;
	}
	return h;
}

struct Hasher {
	std::size_t operator()(VecRef code) const {
		return hashCode((const char *) &code, sizeof(code));
	}
};

struct Comparator {
	bool operator()(VecRef lhs, VecRef rhs) const {
		bool x = lhs.get().x == rhs.get().x;
		bool y = lhs.get().y == rhs.get().y;
		bool z = lhs.get().z == rhs.get().z;

		return x && y && z;
	}
};


void Mesh::init() {
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

	changed = false;
}

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

void Mesh::render() {
	glBindVertexArray(VAO);
	glDrawElements(
		GL_TRIANGLES,
		indices.size(),
		GL_UNSIGNED_INT,
		0
	);
	glBindVertexArray(0);
}

void Mesh::destroy() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &NBO);
}

void Mesh::clear() {
	vertices.clear();
	indices.clear();
	normals.clear();
}

void Mesh::recalculateNormals() {
	// needs to optimize to be able to calculate the normals correctlys
	//optimize();
	normals.resize(vertices.size());
	for (int i = 0; i < indices.size(); i += 3) {
		Vector3f A = vertices.at(indices.at(i));
		Vector3f B = vertices.at(indices.at(i + 1));
		Vector3f C = vertices.at(indices.at(i + 2));

		Vector3f BA = B - A;
		Vector3f CA = C - A;

		Vector3f N = (cross(BA, CA));	// normalize(cross...)

		//if (normals.size() > i + 2) {
		Vector3f NA = normals.at(indices.at(i));
		Vector3f NB = normals.at(indices.at(i + 1));
		Vector3f NC = normals.at(indices.at(i + 2));

		NA = normalize(N + NA);
		NB = normalize(N + NB);
		NC = normalize(N + NC);

		normals[indices.at(i)] = NA;
		normals[indices.at(i + 1)] = NB;
		normals[indices.at(i + 2)] = NC;

			//addNormal(NA);
			//addNormal(NB);
			//addNormal(NC);
			
			//normals[indices.at(i)] = NA;
			//normals[indices.at(i + 1)] = NB;
			//normals[indices.at(i + 2)] = NC;
		//}
		//addNormal(N);
		//addNormal(N);
		//addNormal(N);
	}
}

void Mesh::optimize() {
	int i = 0;
	std::unordered_map<VecRef, unsigned int, Hasher, Comparator> mapped;
	for (Vector3f & vertex : getVertices()) {

		// map does not contain vertices, add them.
		auto & pair = mapped.find(vertex);
		if (pair == mapped.end()) {
			mapped.emplace(vertex, indices[i++]);
			continue;
		}

		// map contains the vertex:
		indices[i++] = pair->second;
	}
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




Mesh& Mesh::addFace(
	unsigned int x,
	unsigned int y,
	unsigned int z)
{
	return addIndex(x)
		.addIndex(y)
		.addIndex(z);
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





Mesh& Mesh::addVertex(float x, float y, float z) {
	return addVertex(Vector3f(x, y, z));
}

Mesh& Mesh::addVertex(Vector3f const & vec) {
	vertices.push_back(vec);
	return *this;
}


Mesh& Mesh::addNormal(float x, float y, float z) {
	return addNormal(Vector3f(x, y, z));
}

Mesh& Mesh::addNormal(Vector3f const & vec) {
	normals.push_back(vec);
	return *this;
}

Mesh& Mesh::addIndex(unsigned int i) {
	indices.push_back(i);
	return *this;
}

Mesh& Mesh::addColor(float x, float y, float z) {
	return addColor(Vector3f(x, y, z));
}

Mesh& Mesh::addColor(Vector3f const & val) {
	colors.push_back(val);
	return *this;
}


std::vector<Vector3f>& Mesh::getVertices() {
	return vertices;
}

std::vector<unsigned int>& Mesh::getIndices() {
	return indices;
}

std::vector<Vector3f>& Mesh::getNormals() {
	return normals;
}



bool Mesh::hasChanged() const {
	return changed;
}

Mesh::Mesh(GLint drawType) : glDrawType(drawType) {}

//void Mesh::onCreate() {}
//void Mesh::onDestroy() {
//	Component::onDestroy();
//}