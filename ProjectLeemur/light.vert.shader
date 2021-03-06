#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 MVP;

out vec3 FragmentPosition;
out vec3 FragmentNormal;

void main() {
	FragmentPosition = position;
	FragmentNormal = normal;

	gl_Position = MVP * vec4(position.x, position.y, position.z, 1.0);
}
