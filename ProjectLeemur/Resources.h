#pragma once

#include "Commons.h"
#include "Shader.h"
#include "Entity.h"
#include "ObjObject.h"

// Put all keys here:

/* Entity keys */
#define SKYBOX 0
#define PLAYER 1
#define POD_OBJ 2
#define CYL_OBJ 3

/* Shader keys */
#define SHADER_LIGHT 0


/* 
  Non owned resources: i.e. if you add resources to the Resources pool,
  you'll have to delete them later where you added them, or when you no
  longer need them.
*/
class Resources {

private: /* Pool of resources */
	std::unordered_map<int, Shader*> shaders;
	std::unordered_map<int, Entity*> entities;
	Resources() {}


public: /* Singleton */
	Resources(Resources const &) = delete;
	void operator=(Resources const &) = delete;


public: /* Singleton methods */

	static Resources & getInstance();
	static void init();
	
	static Shader & getShader(int key);
	static Resources & addShader(
		int key,		
		std::string const &, 
		std::string const &);

	static Resources & addShader(int key, Shader *);


	static Entity & getEntity(int key);
	static Resources & addEntity(int key, Entity *);
	static ObjObject & newObjEntity(int key, const std::string &);


	static void destroy();
};