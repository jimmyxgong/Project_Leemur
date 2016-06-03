#include "Material.h"
#include "Shader.h"

Material Material::RedPlastic = {
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 0.5f, 0.0f, 0.0f, 1.0f },
	{ 1.7f, 1.6f, 1.6f, 1.0f },
	{ 0.f, 0.f, 0.f, 1.f },
	3.0f
};

Material Material::Parismarine{
	{ 0.1f, 0.1f, 0.31f, 1.0f }, 
	{ 1.0f, 0.5f, 0.31f, 1.0f },
	{ 0.1f, 0.1f, 0.1f, 1.0f },
	{ 0.f, 0.f, 0.f, 1.f },
	3.0f
};

Material Material::Gold = {
	{ 0.24725f, 0.1995f, 0.0745f, 1.0f },
	{ 0.75164f, 0.60648f, 0.22648f, 1.0f },
	{ 0.628281f, 0.555802f, 0.366065f, 1.0f },
	{ 0.f, 0.f, 0.f, 1.f },
	3.0f
};

Material Material::Grass = {
	{ 0.f, 0.f, 0.f, 1.f },
	{ 0.0f, 0.408f, 0.0392f, 1.f },
	{ 0.3f, 1.f, 0.5f, 1.f },
	{ 0.f, 0.f, 0.f, 1.f },
	1.0f,
	"grass"
};

Material Material::Sand = {
	{ 0.24725f, 0.1995f, 0.0745f, 1.0f },
	{ 0.75164f, 0.60648f, 0.22648f, 1.0f },
	{ 0.628281f, 0.555802f, 0.366065f, 1.0f },
	{ 0.f, 0.f, 0.f, 1.f },
	0.5f,
	"sand"
};

Material Material::Snow = {
	{ 0.f, 0.f, 0.f, 1.f },
	{ 0.5f, 0.5f, 0.5f, 0.f },
	{ 0.5f, 0.5f, 0.5f, 0.f },
	{ 0.f, 0.f, 0.f, 1.f},
	0.5f,
	"snow"
};

void Material::loadToShader() {
	using namespace std;
	if (name.empty()) {
		Shader::loadVector("material.ambient", ambient)
			.loadVector("material.diffuse", diffuse)
			.loadVector("material.specular", specular)
			.loadVector("material.emission", emission)
			.loadFloat("material.shiny", shiny);
		return;
	}
	Shader::loadVector((name + ".ambient").c_str(), ambient)
		.loadVector((name + ".diffuse").c_str(), diffuse)
		.loadVector((name + ".specular").c_str(), specular)
		.loadVector((name + ".emission").c_str(), emission)
		.loadFloat((name + ".shiny").c_str(), shiny);

}