#pragma once

#include "Commons.h"
#include "TransformEntity.h"
#include "Shader.h"

/* Components are like Graphical Resources that one loads from files */
class Component : public BaseEntity {
protected:
	GLuint VBO, NBO, VAO, EBO;

	std::vector<Vector3f> vertices;
	std::vector<Vector3f> normals;
	std::vector<unsigned int> indices;

	WeakPointer<Shader> attachedShader;

	~Component();

public:
	void onDestroy() override;
	void renderVaoWithIndices();

	/* Readonly values */
	const std::vector<Vector3f>& getVertices() const;
	const std::vector<Vector3f>& getNormals() const;
};