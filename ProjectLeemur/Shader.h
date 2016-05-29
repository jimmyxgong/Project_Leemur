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

	// Must use the use() function otherwise shaders will throw errors
	// when using the static function calls below or unexpected shader
	// usage such as using a terrain shader instead of a light shader.
	void use();

	// Return the shader id.
	GLint getId();

	// Destroy the shader program with glDeleteProgram(id)
	void destroy();
	
	static GLuint getLocation(const GLchar* where);
	static Shader & loadVector(const GLchar*, const Vector4f &);
	static Shader & loadVector(const GLchar*, const Vector3f &);
	static Shader & loadMatrix(const GLchar*, const Matrix4f &);
	static Shader & loadMatrix(const GLchar*, const Matrix3f &);
	static Shader & loadFloat(const GLchar*, GLfloat);
	static Shader & loadInt(const GLchar*, GLint);
	
	~Shader();
};

#endif
