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
	void destroy();

	static Shader & init(GLint&);
	static Shader & loadVector(const GLchar*, const Vector4f &);
	static Shader & loadVector(const GLchar*, const Vector3f &);
	static Shader & loadMatrix(const GLchar*, const Matrix4f &);
	static Shader & loadFloat(const GLchar*, GLfloat);
	static Shader & loadInt(const GLchar*, GLint);
	
	~Shader();
};

#endif
