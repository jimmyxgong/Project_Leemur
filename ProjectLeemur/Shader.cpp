#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <string.h>

#include "Shader.h"


Shader* Shader::instance = nullptr;


void Shader::use() {
	Shader::instance = this;

	glUseProgram(id);
}

GLint Shader::getId() {
	return id;
}

GLuint Shader::getLocation(const GLchar* where) {
	return glGetUniformLocation(instance->getId(), where);
}

Shader& Shader::loadVector(const GLchar* where, const Vector4f & vector) {
	GLuint id = glGetUniformLocation(instance->getId(), where);
	glUniform4f(id, vector.x, vector.y, vector.z, vector.w);
	return *instance;
}

Shader& Shader::loadVector(const GLchar* where, const Vector3f & vector) {
	GLuint id = glGetUniformLocation(instance->getId(), where);
	glUniform3f(id, vector.x, vector.y, vector.z);
	return *instance;
}

Shader& Shader::loadMatrix(const GLchar* where, const Matrix4f & matrix) {
	GLuint id = glGetUniformLocation(instance->getId(), where);
	glUniformMatrix4fv(id, 1, GL_FALSE, &matrix[0][0]);
	return *instance;
}

Shader& Shader::loadMatrix(const GLchar* where, const Matrix3f & matrix) {
	GLuint id = getLocation(where);
	glUniformMatrix3fv(id, 1, GL_FALSE, &matrix[0][0]);
	return *instance;
}

Shader& Shader::loadFloat(const GLchar* where, GLfloat value) {
	GLuint id = glGetUniformLocation(instance->getId(), where);
	glUniform1f(id, value);
	return *instance;
}

Shader& Shader::loadInt(const GLchar* where, GLint value) {
	GLuint id = glGetUniformLocation(instance->getId(), where);
	glUniform1ui(id, value);
	
	return *instance;
}

void Shader::destroy() {
	glDeleteProgram(getId());
}

Shader::~Shader() {
	destroy();
}




/* The following functions are for loading the shaders from text files */

bool readFile(std::string& filepath, std::string& out) {
	using namespace std;
	ifstream fileread(filepath, ios::in);
	if (fileread.is_open()) {
		string Line = "";
		while (getline(fileread, Line))
			out += "\n" + Line;
		fileread.close();
		return true;
	}
	return false;
}

void compileShader(GLuint& id, std::string& code) {
	char const * VertexSourcePointer = code.c_str();
	glShaderSource(id, 1, &VertexSourcePointer, NULL);
	glCompileShader(id);
}


GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {
	using namespace std;

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	string VertexShaderCode = "";
    string path = string(vertex_file_path);
	if (!readFile(path, VertexShaderCode)) {
		printf(
			"Impossible to open %s. Check to make sure the file exists and is in the right directory !\n", 
			vertex_file_path
		);
		printf("The current working directory is:");

	  #ifdef _WIN32
		system("CD");
	  #else
		system("pwd");
	  #endif
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	string FragmentShaderCode = "";
    path = string(fragment_file_path);
	readFile(path, FragmentShaderCode);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	compileShader(VertexShaderID, VertexShaderCode);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ) {
		vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog (
			VertexShaderID, 
			InfoLogLength, 
			NULL, 
			&VertexShaderErrorMessage[0]
		);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ) {
		vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog (
			FragmentShaderID, 
			InfoLogLength, 
			NULL, 
			&FragmentShaderErrorMessage[0]
		);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ) {
		vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(
			ProgramID, 
			InfoLogLength, 
			NULL, 
			&ProgramErrorMessage[0]
		);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}