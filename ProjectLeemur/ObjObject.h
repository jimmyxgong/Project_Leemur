#pragma once

#include "Component.h"
#include "Mesh.h"

class ObjObject : public Mesh {
private:
	ObjObject(GLint drawType = GL_STATIC_DRAW);
	static ObjObject * load(const std::string & filepath);
public:

	ObjObject& addIndex(unsigned int) override;
	//ObjObject& addFace(unsigned int, unsigned int, unsigned int) override;
	static ObjObject * create(std::string const & val);
};