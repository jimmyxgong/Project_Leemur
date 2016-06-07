#pragma once

#include "Commons.h"
#include <stack>

class Keyboard {
public:
	static class Layout {
	public:
		std::unordered_map<int, std::function<void(bool)>> listeners;
		void onKeyPressed(int key, std::function<void(bool isShifted)>);
	} defaultLayout;
	

private:
	/*Deprecated*/
	static std::stack<Layout*> layoutBackstack;
	
	
	static std::vector<Layout*> layoutStack;

public:
	static void init();
	static void onKeyAction(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void addLayout(Layout * layout);
	static Layout * popLayout();
	static Layout & topLayout();

	/*Deprecated */
	static Layout * layout;
	static void setFocusedLayout(Layout * layout);
	/*			 */
};