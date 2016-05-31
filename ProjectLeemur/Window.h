#pragma once

#include "Commons.h"
#include "Camera.h"


class Window {
private:
	Matrix4f P;
	Matrix4f V;

	Camera * activeCamera;
	Camera * secondActiveCamera; // split screen

public:
	struct Settings {
	public:
		std::string title;
		int width = 0, height = 0;
		float fov;
		float near;
		float far;
	} settings;

	struct AdvancedSettings {
	public:
		bool useSplitScreen = false;

	} advancedSettings;

public:
	GLFWwindow* glfwWindow;

	void onStart();
	void onRender(GLFWwindow*);
	void onUpdate();
	void onDestroy();
	static void onResize(GLFWwindow*, int width, int height);
	static void onError(int error, const char* desc);


	Window & setPerspective(Matrix4f const & val);
	Matrix4f getPerspective() const;
	Window & setView(Matrix4f const & val);
	Matrix4f getView() const;

	Window & setActiveCamera(Camera * cam);
	Window & setSecondActiveCamera(Camera * cam);

	GLFWwindow * getGlfwWindow() const;

	

private:
    GLFWwindow * newGlfwWindow();

public:

	static Window & of(std::string const & title);
	Window & setSize(int width, int height);
	Window & setFov(float fov);
	Window & setNear(float near);
	Window & setFar(float far);
	Window & start();


	static void PrintVersion();
	static Window & getFocusedWindow();
private:
	static Window * focusedWindow;
};