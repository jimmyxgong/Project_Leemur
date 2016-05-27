#include "Skybox.h"
#include "Window.h"
#include "Resources.h"
#include "Transform.h"
#include "Window.h"

#include <SOIL.h>

GLuint skyboxTexture;

const static GLfloat skyboxVertices[] = {
	// Positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};

void Skybox::onStart() {
	shader = new Shader(LoadShaders("skybox.vert.shader", "skybox.frag.shader"));
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	skyboxTexture =
		SOIL_load_OGL_cubemap(
			"down-under_rt.tga",
			"down-under_lf.tga",
			"down-under_up.tga",
			"down-under_dn.tga",
			"down-under_bk.tga",
			"down-under_ft.tga",
			SOIL_LOAD_RGB,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS
		);
}

void Skybox::onRender() {
	glDepthMask(GL_FALSE);

	shader->use();
	glm::mat4 VP = window->getPerspective() * (Transform::StripTranslation((window->getView()), 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader->getId(), "VP"), 1, GL_FALSE, &VP[0][0]);
	// skybox cube
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader->getId(), "skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

	// Make sure no bytes are padded:
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Select GL_MODULATE to mix texture with polygon color for shading:
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Use bilinear interpolation:
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Use clamp to edge to hide skybox edges:
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}

void Skybox::onDestroy() {
	glDeleteProgram(shader->getId());
	delete shader;
}

GLuint Skybox::getTexture() {
	return skyboxTexture;
}

Skybox::Skybox(Window * ref) {
	this->window = ref;
}