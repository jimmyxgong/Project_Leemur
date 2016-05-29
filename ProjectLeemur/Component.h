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
	std::vector<unsigned int> indices;

	Material * material;
protected:
	virtual void renderVaoWithIndices();

public:
	Shader * attachedShader;

	virtual void onDestroy() override;

	Component& setMaterial(Material * mat);
	Component& attachShader(Shader * shader);

	Shader& getShader();
	Material& getMaterial();

	std::vector<Vector3f>& getVertices();
	std::vector<Vector3f>& getNormals();

	~Component();
};