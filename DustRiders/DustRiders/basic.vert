#version 330 core
layout (location = 0) in vec3 pos;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main() {
	gl_Position = P * V * M * vec4(pos, 1.0);
}