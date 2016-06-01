#pragma once

#ifdef _WIN32
#include <glm\vec4.hpp>
#include <glm\vec3.hpp>
#else
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#endif

#define AIR 0
#define GRASS 1
#define SNOW 2
#define SAND 3

using namespace glm;
struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emission;

	void loadToShader();

	static Material RedPlastic;
	static Material Parismarine;
	static Material Gold;
};
