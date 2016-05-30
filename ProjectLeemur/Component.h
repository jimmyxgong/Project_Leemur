#pragma once

#include "Commons.h"
#include "TransformEntity.h"
#include "Shader.h"
#include "Material.h"

/* Components are like Graphical Resources that one loads from files */
class Component : public BaseEntity {
protected:
	GLuint VBO, NBO, VAO, EBO;

	std::vector<Vector3f> vertices;
	std::vector<Vector3f> normals;
	std::vector<uint> indices;

	Material * material;

public:
	Shader * attachedShader;

	virtual void onStart(GLint);
	virtual void onRender() override;
	virtual void onDestroy() override;

	Component& setMaterial(Material * mat);
	Component& attachShader(Shader * shader);

	Shader& getShader();
	Material& getMaterial();

	virtual Component& addVertex(float x, float y, float z);
	virtual Component& addNormal(float x, float y, float z);
	virtual Component& addFace(unsigned int x, unsigned int y, unsigned z);
	virtual Component& addIndex(unsigned int i);

	std::vector<Vector3f>& getVertices();
	std::vector<unsigned int>& getIndices();
	std::vector<Vector3f>& getNormals();

	~Component();
};