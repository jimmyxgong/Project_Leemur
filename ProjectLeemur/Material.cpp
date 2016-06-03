#include "Material.h"
#include "Shader.h"

Material Material::RedPlastic = {
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 0.5f, 0.0f, 0.0f, 1.0f },
	{ 1.7f, 1.6f, 1.6f, 1.0f }
};

Material Material::Parismarine{
	{ 0.1f, 0.1f, 0.31f, 1.0f }, 
	{ 1.0f, 0.5f, 0.31f, 1.0f },
	{ 0.1f, 0.1f, 0.1f, 1.0f }
};

Material Material::Gold = {
	{ 0.24725f, 0.1995f, 0.0745f, 1.0f },
	{ 0.75164f, 0.60648f, 0.22648f, 1.0f },
	{ 0.628281f, 0.555802f, 0.366065f, 1.0f }
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
	Shader::loadVector("material.ambient", ambient)
			.loadVector("material.diffuse", diffuse)
			.loadVector("material.specular", specular)
			.loadVector("material.emission", emission);
}