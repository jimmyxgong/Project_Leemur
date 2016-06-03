#pragma once

#include "Commons.h"
#include "Component.h"

class Mesh : public Component {

protected:
	bool changed = false;
	GLint glDrawType;

	GLuint VBO, NBO, VAO, EBO;

	std::vector<Vector3f> vertices;
	std::vector<Vector3f> normals;
	std::vector<Vector3f> colors;
	std::vector<unsigned int> indices;



public:

	void init();
	void render();
	void destroy();
	
	void updateMeshData();
	void clear();
	void recalculateNormals();
	void optimize();

	Mesh& setVertices(std::vector<Vector3f> const & vertices);
	Mesh& setNormals(std::vector<Vector3f> const & normals);
	Mesh& setTriangles(std::vector<unsigned int> const & indices);

	virtual Mesh& addIndex(unsigned int);
	/* 
	* addFace is different from addTriangle(s) since it 
	* does not take into account the vertex size when adding
	* to indices.
	*/
	virtual Mesh& addFace(unsigned int x, unsigned int y, unsigned z);
	Mesh& addTriangle(unsigned int i, unsigned int j, unsigned int k);
	Mesh& addTriangles(std::vector<unsigned int> const & triangles);

	Mesh& addVertex(float x, float y, float z);
	Mesh& addVertex(Vector3f const &);
	Mesh& addNormal(float x, float y, float z);
	Mesh& addNormal(Vector3f const &);
	Mesh& addColor(float, float, float);
	Mesh& addColor(Vector3f const &);


	std::vector<Vector3f>& getVertices();
	std::vector<unsigned int>& getIndices();
	std::vector<Vector3f>& getNormals();

	bool hasChanged() const;

	Mesh(GLint drawType = GL_STATIC_DRAW);

	//void onDestroy() override;
private:
	//void onCreate() override;
};