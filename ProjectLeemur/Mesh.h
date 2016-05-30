#pragma once

#include "Commons.h"
#include "Component.h"

class Mesh : public Component {

private:
	bool changed = false;
	Mesh& updateVertices();

public:

	void onStart() override;
	void onRender() override;
	void onUpdate() override;

	Mesh& recalculateNormals();
	Mesh& setVertices(std::vector<Vector3f> const & vertices);
	Mesh& setNormals(std::vector<Vector3f> const & normals);
	Mesh& setTriangles(std::vector<unsigned int> const & indices);
	
	Mesh& addTriangle(unsigned int i, unsigned int j, unsigned int k);
	Mesh& addTriangles(std::vector<unsigned int> const & triangles);

	bool hasChanged() const;

	void onDestroy() override;
private:
	void onCreate() override;
};