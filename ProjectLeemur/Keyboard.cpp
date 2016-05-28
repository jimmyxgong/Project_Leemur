#include "Keyboard.h"
#include "Environment.h"
#include "Resources.h"

Keyboard::Layout Keyboard::defaultLayout;
Keyboard::Layout * Keyboard::layout;
std::stack<Keyboard::Layout*> Keyboard::layoutBackStack;

void Keyboard::init() {
	//layoutBackStack.push(&defaultLayout);
	layout = &defaultLayout;
	map();
}

void Keyboard::map() {
	defaultLayout.onKeyPressed(GLFW_KEY_E, [](bool isShifted) -> void {

	});

	defaultLayout.onKeyPressed(GLFW_KEY_I, [](bool) {
	
	});
}

void Keyboard::Layout::onKeyPressed(int key, std::function<void(bool)> l) {
	listeners.emplace(key, l);
}


void Keyboard::onKeyAction(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE) {
		// Close the window. This causes the program to also terminate.
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_ESCAPE) {
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		bool isShifted = mods == GLFW_MOD_SHIFT;
		auto listener = layout->listeners.find(key);
		if (listener != layout->listeners.end()) {
			listener->second(isShifted);
		}
	}
}

void Keyboard::addLayout(Keyboard::Layout * layout) {
	layoutBackStack.push(layout);
}

Keyboard::Layout * Keyboard::popLayout() {
	// do not pop the default keyboard layout
	if (layoutBackStack.size() < 2) return nullptr;

	Layout * popped = layoutBackStack.top();
	layoutBackStack.pop();
	return popped;
}


/*Deprecated*/
void Keyboard::setFocusedLayout(Layout * layout) {
	Keyboard::layout = layout;
}