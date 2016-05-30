#include "ObjObject.h"
#include <fstream>
#include <sstream>

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
	Component::onRender();
}

void ObjObject::onUpdate() {

}








std::vector<std::string> stringSplit(const std::string& input, const char delim = ' ') {
	using namespace std;
	stringstream args(input);
	string token;
	vector<string> tokens;

	// Start splliting the string into tokens delimitted by the delimitter specified.
	while (getline(args, token, delim)) {

		if (token.length() > 0) {
			// token is not an empty space.
			tokens.push_back(token);
		}
	}
	return tokens;
}

ObjObject* load(const std::string & filepath) {
	// Print name of the object being loaded:
	using namespace glm;
	using namespace std;

	ifstream inputStream(filepath);
	if (!inputStream.is_open()) {
		// Check if file is valid
		cout << "File could not be opened or is not valid" << endl;
		return NULL;
	}

	vector<string> tokens = stringSplit(filepath, '/');
	cout << "Loading " << tokens[tokens.size() - 1];

	ObjObject* object = new ObjObject();

	int count = 1;
	for (string line; getline(inputStream, line); count++) {
		if (count % 10000 == 0) {
			count = 1;
			cout << ".";
		}
		if (line.empty() || line[0] == '#')   continue;

		if (line[0] == 'v') {
			float vx, vy, vz;

			// A non-normal vertex.
			if (line[1] == ' ') {
				float r = -1, g, b;
				// read in the line wtih a standard format:
				sscanf_s(
					line.substr(2, line.size() - 2).c_str(),
					"%f %f %f %f %f %f",
					&vx, &vy, &vz, &r, &g, &b
				);

				object->addVertex(vx, vy, vz);
				continue;
			}

			// Else: start parsing vector normals:
			sscanf_s(
				line.substr(3, line.size() - 3).c_str(),
				"%f %f %f",
				&vx, &vy, &vz
			);
			object->addNormal(vx, vy, vz);
		}
		else if (line[0] == 'f') {
			unsigned int vx, vy, vz, vnx, vny, vnz;
			sscanf_s(
				line.substr(2, line.size() - 2).c_str(),
				"%d//%d %d//%d %d//%d",
				&vx, &vnx, &vy, &vny, &vz, &vnz
			);

			object->addFace(vx, vy, vz);
		}
	}

	cout << "\nParsing completed for " << tokens[tokens.size() - 1] << "\n" << endl;
	if (inputStream.is_open()) inputStream.close();

	{ /* Linear time: 2*|V| for mapping each vertex to the range -1 : 1 */
		Vector3f max(0.0f);
		Vector3f min(0.0f);
		for (Vector3f const & v : object->getVertices()) {
			if (max.x < v.x) max.x = v.x;
			if (max.y < v.y) max.y = v.y;
			if (max.z < v.z) max.z = v.z;

			if (min.x > v.x) min.x = v.x;
			if (min.y > v.y) min.y = v.y;
			if (min.z > v.z) min.z = v.z;
		}

		// Start mapping to the follow toMax/toMin range
		const float toMax = 0.2f;
		const float toMin = -0.2f;
		vec3 spanFrom(max - min);
		vec3 spanTo(toMax - toMin);

		for (Vector3f & v : object->getVertices()) {
			v = toMin + ((v - min) / spanFrom) * spanTo;
		}
	}
	// ObjObject successfully loaded. 
	// Time to start its internals:
	object->onStart();

	return object;
}

ObjObject * ObjObject::create(const std::string & val) {
	return load(val);
}