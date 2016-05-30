#pragma once

#include "Commons.h"
#include "Component.h"

class Mesh : public Component {

private:



public:

	void onStart() override;
	void onRender() override;
	void onUpdate() override;

	Mesh& setVertices(std::vector<Vector3f> const & vertices);
	Mesh& setTriangles(std::vector<int> const & indices);
	

private:
	void onCreate() override;
	void onDestroy() override;
};