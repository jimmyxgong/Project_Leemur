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
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 0.f, 0.f, 0.f, 1.f },
	3.0f
};

Material Material::Emerald{
	{ 0.0215 ,0.1745, 0.0215, 0.55 },
	{ 0.07568, 0.61424,	0.07568, 0.55 },
	{ 0.633, 0.727811, 0.633, 0.55 },
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

Material Material::Water = {
	{ 0.f, 0.f, 0.f, 1.f },
	{ 0.251f, 0.6431f, 0.87451f, 1.f },
	{ 0.2f, 0.6f, 0.8f, 1.f },
	{ 0.f, 0.f, 0.f, 1.f },
	1.0f,
	"water"
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

Material Material::Silver = {
	{0.19225, 0.19225 ,0.19225, 0.4},
	{0.50754, 0.50754 ,0.50754, 0.4},
	{0.508273, 0.508273, 0.508273, 0.4}
};

Material Material::Bronze = {
	{0.2125, 0.1275, 0.054, 0.2},
	{0.714, 0.4284, 0.18144, 0.2},
	{0.393548, 0.271906, 0.166721, 0.2}
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