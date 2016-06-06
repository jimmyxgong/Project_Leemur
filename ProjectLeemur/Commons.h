#pragma once
#ifndef COMMONS_H
#define COMMONS_H

#ifdef _WIN32
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\matrix.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\string_cast.hpp>
#else
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#endif

#include <algorithm>
#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <list>
#include <memory>
#include <unordered_map>
#include <typeinfo>

#define M_PI	3.14159365

/**   NOTES
* 1. All naming conventions are done here.
* 2. Constants that are not appropriate for user-defined classes should be alotted here.
* 3. Most used dependencies are store here as well.
*/

typedef glm::vec3	Vector3f;
typedef glm::vec4	Vector4f;
typedef glm::mat3	Matrix3f;
typedef glm::mat4	Matrix4f;

template <typename T>
using SharedPointer = std::shared_ptr<T>;

template <typename T>
auto share() {
	return std::make_shared<T>();
}

template <typename T, typename ...Args>
auto share(Args&&... args) {
	// Perfect forwarding for template function aliasing
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T>
using UniquePointer = std::unique_ptr<T>;

template <typename T>
auto unique() {
	return std::make_unique<T>();
}

template <typename T, typename ...Args>
auto unique(Args&&... args) {
	// Perfect forwarding for template function aliasing
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using WeakPointer = std::weak_ptr<T>;

#endif
