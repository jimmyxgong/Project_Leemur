#include "ObjObject.h"
#include <fstream>
#include <sstream>

void ObjObject::onStart() {
	Component::onStart(GL_STATIC_DRAW);
}

void ObjObject::onRender() {
	Component::onRender();
}

void ObjObject::onUpdate() {

}

ObjObject& ObjObject::addIndex(unsigned int i) {
	return (ObjObject&) Component::addIndex(i - 1);
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