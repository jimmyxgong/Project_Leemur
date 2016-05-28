#include "ObjObject.h"




void ObjObject::onStart() {
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
		GL_STATIC_DRAW
	);

	// Bind indices to the vertices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(unsigned int),
		&indices[0],
		GL_STATIC_DRAW
	);

	// This first parameter x should be the same as the number passed into the line "layout 
	// (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to 
	// GL_MAX_UNIFORM_LOCATIONS.
	//
	// This second param tells us how any components there are per vertex. In this case, it's
	// 3 (we have an x, y, and z component)
	//
	// Param 3: What type these components are
	// Param 4: GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
	// Param 5: Offset between consecutive vertex attributes. Since each of our vertices have 
	// 3 floats, they should have the size of 3 floats in between
	//
	// Param 6: // Offset of the first vertex's component. In our case it's 0 since we don't 
	// pad the vertices array with anything.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (GLvoid*)0);

	// Bind normals
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		normals.size() * sizeof(Vector3f),
		&normals[0],
		GL_STATIC_DRAW
	);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (GLvoid*)0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ObjObject::onRender() {
	// TODO add shader
	renderVaoWithIndices();
}

void ObjObject::onUpdate() {

}

ObjObject& ObjObject::addVertex(float x, float y, float z) {
	vertices.push_back(Vector3f(x, y, z));
	return *this;
}

ObjObject& ObjObject::addNormal(float x, float y, float z) {
	normals.push_back(Vector3f(x, y, z));
	return *this;
}

ObjObject& ObjObject::addFace(unsigned int x, unsigned int y, unsigned int z) {
	return addIndex(x)
		.addIndex(y)
		.addIndex(z);
}

ObjObject& ObjObject::addIndex(unsigned int i) {
	indices.push_back(i - 1);
	return *this;
}