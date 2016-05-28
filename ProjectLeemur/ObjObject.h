#pragma once

#include "Component.h"

class ObjObject : public Component {
public:
	void onStart() override;
	void onRender() override;
	void onUpdate() override;

	ObjObject& addVertex(float x, float y, float z);
	ObjObject& addNormal(float x, float y, float z);
	ObjObject& addFace(unsigned int x, unsigned int y, unsigned int z);
	ObjObject& addIndex(unsigned int i);


};