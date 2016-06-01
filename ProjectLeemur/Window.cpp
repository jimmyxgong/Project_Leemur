#include "Window.h"
#include "Camera.h"
#include "Environment.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Resources.h"

UniquePointer<Environment> environment;
UniquePointer<Camera> defaultCamera;

void Window::onStart() {
	Keyboard::init();
	Mouse::init();
	Resources::init();

	activeCamera = defaultCamera.get();
	secondActiveCamera = defaultCamera.get();

	environment = Environment::create(this);
	environment->onCreate();
	environment->onStart();
}

void Window::onRender(GLFWwindow* window) {
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	V = activeCamera->viewMatrix();
	

	glViewport(0, 0, settings.width / (advancedSettings.useSplitScreen ? 2 : 1), settings.height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Render all objects in the environment
	environment->onRender();


	// TODO the second screen does not work yet. Second Active Camera's logic still needs to
	// be implemented in the code.
	if (advancedSettings.useSplitScreen) {
		glViewport(settings.width / 2 + 1, 0, settings.width / 2, settings.height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// TODO use second camera here
		environment->onRender();
	}

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Window::onUpdate() {
	environment->onUpdate();
}

void Window::onDestroy() {
	Resources::destroy();
	environment->onDestroy();
	delete this;
}

void Window::onResize(GLFWwindow* window, int width, int height) {
	Window & w = getFocusedWindow();
	w.settings.width = width;
	w.settings.height = height;

	if (height > 0) {
		
		// Set the perspective of the projection viewing frustum
		w.P = glm::perspective(w.settings.fov, 
							   (float)width / (float)height, 
							   w.settings.near, 
							   w.settings.far);

		w.V = w.activeCamera == nullptr
			? glm::lookAt(CAMERA_POSITION, CAMERA_LOOK_AT, CAMERA_UP_VECTOR)
			: w.activeCamera->viewMatrix();
	}
}

void Window::onError(int error, const char* description) {
	fputs(description, stderr);
}





/** GETTERS and SETTERS**/

Window & Window::setPerspective(Matrix4f const & val) {
	P = val;
	return (*this);
}

Matrix4f Window::getPerspective() const {
	return P;
}

Window & Window::setView(Matrix4f const & val) {
	V = val;
	return (*this);
}

Matrix4f Window::getView() const {
	return V;
}

Matrix4f Window::VP() const {
	return P * V;
}


Window & Window::setActiveCamera(Camera * cam) {
	activeCamera = cam;
	return (*this);
}

Window & Window::setSecondActiveCamera(Camera * cam) {
	secondActiveCamera = cam;
	return (*this);
}

Camera & Window::getActiveCamera() {
	return *activeCamera;
}



GLFWwindow * Window::getGlfwWindow() const {
	return glfwWindow;
}


int Window::width() const {
	return settings.width;
}

int Window::height() const {
	return settings.height;
}






GLFWwindow * Window::newGlfwWindow() {
#ifndef _WIN32
    glewExperimental = GL_TRUE;
#endif
    if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

#ifndef _WIN32 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#endif

	glfwWindowHint(GLFW_SAMPLES, 4);				// 4x antialiasing
	GLFWwindow* window = glfwCreateWindow(settings.width,
		settings.height,
		settings.title.c_str(),
		NULL, NULL);

	if (window == NULL) {
		// Window failed to open
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);				// Make the context of the window
	glfwSwapInterval(1);						// Set swap interval to 1

	int x, y;
	glfwGetFramebufferSize(window, &x, &y);
	onResize(window, x, y);

	return window;
}

void setupGlew() {
	// Initialize GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		glfwTerminate();
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}

void setupOpenGLsettings() {
	// Enable depth buffering
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Set polygon drawing mode to fill front and back of each polygon
	// You can also use the paramter of GL_LINE instead of GL_FILL to see wireframes
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Disable backface culling to render both sides of polygons
	//glDisable(GL_CULL_FACE);
	glClearColor(0.2f, 0.2f, 0.5f, 1.0f);

	// Enable backface culling for skybox
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

}


Window & Window::of(std::string const & title) {
	Window * window = new Window();
	window->settings.title = title;
	return *window;
}

Window & Window::setSize(int width, int height) {
	settings.width = width;
	settings.height = height;
	return *this;
}

Window & Window::setFov(float fov) {
	settings.fov = fov;
	return *this;
}

Window & Window::setNear(float near) {
	settings.near = near;
	return *this;
}

Window & Window::setFar(float far) {
	settings.far = far;
	return *this;
}

Window & Window::start() {
	focusedWindow = this;

	glfwWindow = newGlfwWindow();
	glfwSetErrorCallback(onError);
	glfwSetWindowSizeCallback(glfwWindow, onResize);

	glfwSetKeyCallback(glfwWindow, Keyboard::onKeyAction);
	glfwSetCursorPosCallback(glfwWindow, Mouse::onMouseUpdate);
	glfwSetMouseButtonCallback(glfwWindow, Mouse::onMousePress);
	glfwSetScrollCallback(glfwWindow, Mouse::onMouseScroll);

	setupGlew();
	setupOpenGLsettings();
	
	onStart();
	return *this;
}


void Window::PrintVersion() {
	// Get info of GPU and supported OpenGL version
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

	//If the shading language symbol is defined
#ifdef GL_SHADING_LANGUAGE_VERSION
	std::printf("Supported GLSL version is %s.\n",
		(char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif
}

Window & Window::getFocusedWindow() {
	return *focusedWindow;
}

Window * Window::focusedWindow;