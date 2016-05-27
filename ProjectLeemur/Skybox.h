#pragma once

#include "Commons.h"
#include "Shader.h"
#include "BaseEntity.h"
#include "Window.h"

struct Skybox : public BaseEntity {
private:
	Window * window;

public:
	GLuint VAO, VBO;
	Shader * shader;

	void onStart() override;
	void onRender() override;
	void onDestroy() override;

	GLuint getTexture();

	Skybox(Window * ref);
};