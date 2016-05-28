#include "Mouse.h"

void Mouse::Layout::setOnDrag(const std::function<void(const Point&, bool)> & listener) {
	onDrag = listener;
}

void Mouse::Layout::setOnScroll(const std::function<void(bool)> & listener) {
	onScroll = listener;
}

void Mouse::Layout::setOnClick(const std::function<void()> & listener) {
	onClick = listener;
}



Mouse::Layout defaulLayout;
std::stack<Mouse::Layout*> layoutBackstack;
bool dragging = false;
bool clickedLeft = true;

void Mouse::init() {
	defaulLayout.setOnDrag([](const Point &, bool isLeft) {

	});

	defaultLayout.setOnScroll([](bool isUp) {

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
	Mouse::now.x = (int) x;
	Mouse::now.y = (int) y;

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
		&& action == GLFW_PRESS) {

		dragging = true;
		clickedLeft = button == GLFW_MOUSE_BUTTON_LEFT;
		topLayout().onClick();
	}
	else if (action == GLFW_RELEASE) {
		dragging = false;
	}
}