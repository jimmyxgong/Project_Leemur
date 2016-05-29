#pragma once

#include "Commons.h"
#include "Shader.h"

struct Light {
public:
	std::string name;

	Vector4f position;
	Vector4f ambient;
	Vector4f diffuse;
	Vector4f specular;
	Vector4f color;

	Vector3f coneDirection;
	float spotCutoff;
	float spotExponent;

	float attenuation;

	Shader * shader;

	void loadToShader();

	static Light Directional;
	static void init();
};