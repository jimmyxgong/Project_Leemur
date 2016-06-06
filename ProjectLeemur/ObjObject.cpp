#include "ObjObject.h"
#include <fstream>
#include <sstream>


ObjObject::ObjObject(GLint drawType) : Mesh(drawType) {
}

ObjObject& ObjObject::addIndex(unsigned int i) {
	indices.push_back(i - 1);
	return *this;
}

//ObjObject& ObjObject::addFace(unsigned int i, unsigned int j, unsigned int k) {
//	return addIndex(i).addIndex(j).addIndex(k);
//}


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

ObjObject* ObjObject::load(const std::string & filepath) {
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
#ifdef _WIN32
				sscanf_s(line.substr(2, line.size() - 2).c_str(),
					"%f %f %f %f %f %f",
					&vx, &vy, &vz, &r, &g, &b
				);
#else
				sscanf(line.substr(2, line.size() - 2).c_str(),
					"%f %f %f %f %f %f",
					&vx, &vy, &vz, &r, &g, &b
				);
#endif
					

				object->addVertex(vx, vy, vz);
				continue;
			}

			// Else: start parsing vector normals:
#ifdef _WIN32
			sscanf_s(
				line.substr(3, line.size() - 3).c_str(),
				"%f %f %f",
				&vx, &vy, &vz
			);
#else
			sscanf(
				line.substr(3, line.size() - 3).c_str(),
				"%f %f %f",
				&vx, &vy, &vz
			);
#endif

			object->addNormal(vx, vy, vz);
		}
		else if (line[0] == 'f') {
			unsigned int vx, vy, vz, vnx, vny, vnz;
#ifdef _WIN32
			sscanf_s(
				line.substr(2, line.size() - 2).c_str(),
				"%d//%d %d//%d %d//%d",
				&vx, &vnx, &vy, &vny, &vz, &vnz
			);
#else
			sscanf(
				line.substr(2, line.size() - 2).c_str(),
				"%d//%d %d//%d %d//%d",
				&vx, &vnx, &vy, &vny, &vz, &vnz
			);
#endif

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
		const float toMax = 1;
		const float toMin = -1;
		vec3 spanFrom(max - min);
		vec3 spanTo(toMax - toMin);

		for (Vector3f & v : object->getVertices()) {
			v = toMin + ((v - min) / spanFrom) * spanTo;
		}
	}
	object->init();

	return object;
}

ObjObject * ObjObject::create(const std::string & val) {
	return load(val);
}
