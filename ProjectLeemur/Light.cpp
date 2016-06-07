#include "Light.h"
#include "Resources.h"

Light Light::Directional = {
	std::string("licht"),
	{ 0.0f, 2.0f, 0.0f, 0.0f },		// position-directional
	{ 1.0f, 0.5f, 0.5f, 1.0f },		// ambient
	{ 1.0f, 1.0f, 1.0f, 1.0f },		// diffuse
	{ 0.0f, 5.0f, 5.0f, 1.0f },		// specular
	{ 1.0f, 1.0f, 1.0f, 1.0f },		// intensity

	{},
	45.0f,
	1.0f,
	1.0f,
	0
};

void Light::loadToShaderi() {
	Shader::loadVector("licht.position", position)
		.loadVector("licht.ambient", ambient)
		.loadVector("licht.diffuse", diffuse)
		.loadVector("licht.specular", specular)
		.loadVector("licht.intensity", color)
		.loadVector("licht.spotConeDirection", coneDirection)
		.loadFloat("licht.spotCutoff", spotCutoff)
		.loadFloat("licht.spotExponent", spotExponent)
		.loadFloat("licht.attenuationConstant", attenuation)
		.loadInt("licht.type", type);
}

void Light::loadToShader() {

	shader->use();
	Shader::loadVector("licht.position", position)
		.loadVector("licht.ambient", ambient)
		.loadVector("licht.diffuse", diffuse)
		.loadVector("licht.specular", specular)
		.loadVector("licht.intensity", color)
		.loadVector("licht.spotConeDirection", coneDirection)
		.loadFloat("licht.spotCutoff", spotCutoff)
		.loadFloat("licht.spotExponent", spotExponent)
		.loadFloat("licht.attenuationConstant", attenuation)
		.loadInt("licht.type", type);
}


void Light::init() {
#ifdef _WIN32
	Resources::addShader(SHADER_LIGHT, "light.vert.shader", "light.frag.shader");
    Resources::addShader(TOON_LIGHT, "light.vert.shader", "toon.frag.shader");
	Resources::addShader(TERRAIN_LIGHT, "light.vert.shader", "terrain.frag.shader");
	Resources::addShader(TOON_NON_EDGE_LIGHT, "light.vert.shader", "toon-non-edge.frag.shader");

#else
    Resources::addShader(SHADER_LIGHT, "/Users/sebastian/Google Drive/College/Year 3/Spring 16/ProjectLeemur/ProjectLeemur/light.vert.shader", "/Users/sebastian/Google Drive/College/Year 3/Spring 16/ProjectLeemur/ProjectLeemur/light.frag.shader");
    Resources::addShader(TOON_LIGHT, "/Users/sebastian/Google Drive/College/Year 3/Spring 16/ProjectLeemur/ProjectLeemur/light.vert.shader", "/Users/sebastian/Google Drive/College/Year 3/Spring 16/ProjectLeemur/ProjectLeemur/toon.frag.shader");
    Resources::addShader(TERRAIN_LIGHT, "/Users/sebastian/Google Drive/College/Year 3/Spring 16/ProjectLeemur/ProjectLeemur/light.vert.shader", "/Users/sebastian/Google Drive/College/Year 3/Spring 16/ProjectLeemur/ProjectLeemur/terrain.frag.shader");

#endif
//	Light::Directional.shader = &Resources::getShader(SHADER_LIGHT);
    Light::Directional.shader = &Resources::getShader(TOON_LIGHT);

}