#ifndef SHADER_HPP
#define SHADER_HPP

#include "Commons.h"

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

class Shader {

private:
	GLint id;
public:
	static Shader* instance;
	
	Shader(GLint id) : id(id) {}
	void use();
	GLint getId();

	static Shader & init(GLint&);
	static Shader & loadVector(const GLchar*, glm::vec4);
	static Shader & loadVector(const GLchar*, glm::vec3);
	static Shader & loadFloat(const GLchar*, GLfloat);
	static Shader & loadInt(const GLchar*, GLint);
	static void cleanup();
	
	~Shader();
};

#endif
