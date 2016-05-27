#version 330 core

layout(location = 0) in vec3 position;

out vec3 TextureCoordinates;

uniform mat4 VP;

void main() {
	TextureCoordinates = position;
	gl_Position = VP * vec4(position, 1.0);
}