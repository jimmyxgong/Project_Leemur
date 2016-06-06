#include "Window.h"
#include "Time.h"
//#include "vld.h"
#ifdef _WIN32
#endif

#define WIDTH 2160
#define HEIGHT (WIDTH / 16 * 9)
#define TITLE "Leemur"
#define FOV 45.0f
#define NEAR 0.3f
#define FAR 1000.0f
#define FPS 60.0f

int main(void) {

	Window & window =
		Window::of(TITLE)
			.setSize(WIDTH, HEIGHT)
			.setFov(FOV)
			.setNear(NEAR)
			.setFar(FAR)
			.start();
	Window::PrintVersion();
	GLFWwindow * glWindow = window.getGlfwWindow();
	Time timer(FPS);


	while (!glfwWindowShouldClose(glWindow)) {
		if (timer.ready()) {
			window.onUpdate();
			window.onRender(glWindow);
		}
	}

	window.onDestroy();
	glfwDestroyWindow(glWindow);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

