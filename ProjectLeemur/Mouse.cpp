#include "Mouse.h"
#include "Camera.h"
#include "Window.h"

void Mouse::Layout::setOnDrag(std::function<void(const Point&, bool)> listener) {
	onDrag = listener;
}

void Mouse::Layout::setOnScroll(std::function<void(bool)> listener) {
	onScroll = listener;
}

void Mouse::Layout::setOnClick(std::function<void()> listener) {
	onClick = listener;
}

Mouse::Layout::Layout() : 
	onDrag(defaultLayout.onDrag),
	onScroll(defaultLayout.onScroll),
	onClick(defaultLayout.onClick)
{}



Mouse::Layout Mouse::defaultLayout;
std::stack<Mouse::Layout*> Mouse::layoutBackstack;
bool Mouse::dragging = false;
bool Mouse::clickedLeft = true;

Point Mouse::old;
Point Mouse::now;

void Mouse::init() {
	std::cout << "Mouse initializng... "<< std::endl;

	// If the a key isn't set for a defined layout, 
	// it will automatically be filled with the default layout's bindings.
	pushLayout(&defaultLayout);
	defaultLayout.setOnDrag([](const Point &, bool isLeft) {

	});

	defaultLayout.setOnScroll([](bool isUp) {
		Camera & cam = Window::getFocusedWindow().getActiveCamera();

		// TODO Transform Position isn't working
		Vector3f position = cam.transform.getLocalPosition();
		//std::cout << to_string(position) << std::endl;
		if (position.x == 0 && position.y == 0 && position.z == 0) {
			position = CAMERA_POSITION;
		}

		position = abs(normalize(position)) / 5.0f;
		if (isUp) position = -position;
		cam.transform.translateLocal(position);
		cam.update();

	});

	defaultLayout.setOnClick([](){

	});
}

void Mouse::pushLayout(Layout * layout) {
	layoutBackstack.push(layout);
}

Mouse::Layout * Mouse::popLayout() {
	if (layoutBackstack.size() == 1) return &defaultLayout;

	Mouse::Layout * layout = layoutBackstack.top();
	layoutBackstack.pop();
	return layout;
}

Mouse::Layout & Mouse::topLayout() {
	return *layoutBackstack.top();
}

void Mouse::onMouseUpdate(GLFWwindow* window, double x, double y) {
	Mouse::now.x = (float) x;
	Mouse::now.y = (float) y;

	if (dragging) {
		topLayout().onDrag(now, clickedLeft);
	}
}

void Mouse::onMouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
	topLayout().onScroll(yoffset > 0);
}

void Mouse::onMousePress(GLFWwindow* window, int button, int action, int mods) {
	if ((button == GLFW_MOUSE_BUTTON_LEFT
		|| button == GLFW_MOUSE_BUTTON_RIGHT)
		&& action == GLFW_PRESS) 
	{
		old = now;

		dragging = true;
		clickedLeft = button == GLFW_MOUSE_BUTTON_LEFT;
		topLayout().onClick();
	}
	else if (action == GLFW_RELEASE) {
		dragging = false;
	}
}