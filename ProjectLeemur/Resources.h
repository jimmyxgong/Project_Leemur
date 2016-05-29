#pragma once


#include "Commons.h"
#include "Shader.h"
#include "Entity.h"

// Put all keys here:
#define SKYBOX 0
#define PLAYER 1
#define POD_OBJ 2
#define CYL_OBJ 3

#define SHADER_LIGHT 0

class Resources {
private:
	std::unordered_map<int, Shader*> shaders;
	std::unordered_map<int, Entity*> entities;
	Resources() {}

public:
	Resources(Resources const &) = delete;
	void operator=(Resources const &) = delete;


	static Resources & getInstance();
	
	static Shader & getShader(int key);
	static Resources & addShader(
		int key,		
		std::string const &, 
		std::string const &);

	static Resources & addShader(
		int key, Shader*);


	static Entity & getEntity(int key);
	static Resources & addEntity(int key, Entity*);
	static Resources & newObjEntity(int key, std::string const &);


	static void destroy();
};