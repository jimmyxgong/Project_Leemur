#pragma once

#include <glm\vec4.hpp>
#include <glm\vec3.hpp>

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
