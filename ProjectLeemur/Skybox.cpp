#include "Skybox.h"
#include "Window.h"
#include "Resources.h"
#include "Transform.h"
#include "Window.h"

#ifdef _WIN32
#include <SOIL.h>
#else
GLuint loadCubemap(std::vector<const GLchar*> faces);
unsigned char* loadPPM(const char* filename, int& width, int& height);
#endif

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
#ifdef _WIN32
    shader = new Shader(LoadShaders("skybox.vert.shader", "skybox.frag.shader"));
#else
	shader = new Shader(LoadShaders("/Users/sebastian/Google\ Drive/College/Year\ 3/Spring\ 16/ProjectLeemur/ProjectLeemur/skybox.vert.shader", "/Users/sebastian/Google\ Drive/College/Year\ 3/Spring\ 16/ProjectLeemur/ProjectLeemur/skybox.frag.shader"));
#endif
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

#ifdef _WIN32
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
#else
    // Cubemap (Skybox)
    std::vector<const GLchar*> faces;
    faces.push_back("/Users/sebastian/Google Drive/College/Year 3/Spring 16/ProjectLeemur/ProjectLeemur/down-under_rt.ppm");
    faces.push_back("/Users/sebastian/Google Drive/College/Year 3/Spring 16/ProjectLeemur/ProjectLeemur/down-under_lf.ppm");
    faces.push_back("/Users/sebastian/Google Drive/College/Year 3/Spring 16/ProjectLeemur/ProjectLeemur/down-under_up.ppm");
    faces.push_back("/Users/sebastian/Google Drive/College/Year 3/Spring 16/ProjectLeemur/ProjectLeemur/down-under_dn.ppm");
    faces.push_back("/Users/sebastian/Google Drive/College/Year 3/Spring 16/ProjectLeemur/ProjectLeemur/down-under_bk.ppm");
    faces.push_back("/Users/sebastian/Google Drive/College/Year 3/Spring 16/ProjectLeemur/ProjectLeemur/down-under_ft.ppm");
    skyboxTexture = loadCubemap(faces);
#endif
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

#ifndef _WIN32
GLuint loadCubemap(std::vector<const GLchar*> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    int width,height;
    unsigned char* image;
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    
    for(GLuint i = 0; i < faces.size(); i++)
    {
        image = loadPPM(faces[i], width, height);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        free(image);
    }
    
    // Make sure no bytes are padded:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // Select GL_MODULATE to mix texture with polygon color for shading:
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    
    return textureID;
}

unsigned char* loadPPM(const char* filename, int& width, int& height)
{
    //    std::cout << "Loading image : " << filename << std::endl;
    
    const int BUFSIZE = 128;
    FILE* fp;
    unsigned int read;
    unsigned char* rawData;
    char buf[3][BUFSIZE];
    char* retval_fgets;
    size_t retval_sscanf;
    
    if ( (fp=fopen(filename, "rb")) == NULL)
    {
        std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
        width = 0;
        height = 0;
        return NULL;
    }
    
    // Read magic number:
    retval_fgets = fgets(buf[0], BUFSIZE, fp);
    
    // Read width and height:
    do
    {
        retval_fgets=fgets(buf[0], BUFSIZE, fp);
    } while (buf[0][0] == '#');
    retval_sscanf=sscanf(buf[0], "%s %s", buf[1], buf[2]);
    width  = atoi(buf[1]);
    height = atoi(buf[2]);
    
    // Read maxval:
    do
    {
        retval_fgets=fgets(buf[0], BUFSIZE, fp);
    } while (buf[0][0] == '#');
    
    // Read image data:
    rawData = new unsigned char[width * height * 3];
    read = (int) fread(rawData, size_t(width * height * 3), 1, fp);
    fclose(fp);
    if (read != 1)
    {
        std::cerr << "error parsing ppm file, incomplete data" << std::endl;
        delete[] rawData;
        width = 0;
        height = 0;
        return NULL;
    }
    //    std::cout << "w: " << width << " h: " << height << std::endl;
    return rawData;
}
#endif
