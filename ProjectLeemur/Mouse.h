#pragma once

#include "Commons.h"
#include "Point.h"
#include <stack>

class Mouse {
public:

	static class Layout {
	public:
		std::function<void(const Point &, bool isLeft)> onDrag;
		std::function<void(bool isUp)> onScroll;
		std::function<void()> onClick;

		void setOnDrag(std::function<void(const Point &, bool isLeft)>);
		void setOnScroll(std::function<void(bool isUp)>);
		void setOnClick(std::function<void()>);

		Layout();
	} defaultLayout;


	static Point old;
	static Point now;

private:

	static std::stack<Layout*> layoutBackstack;
	static bool dragging;
	static bool clickedLeft;

public:
	static void init();
	static void pushLayout(Layout *);
	static Layout * popLayout();
	static Layout & topLayout();

	static void onMouseUpdate(GLFWwindow*, double, double);
	static void onMouseScroll(GLFWwindow* window, double xoffset, double yoffset);
	static void onMousePress(GLFWwindow* window, int button, int action, int mods);
};