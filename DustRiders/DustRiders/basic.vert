#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 fragColor;

void main() {
	fragColor = normal;
	gl_Position = P * V * M * vec4(pos, 1.0);
}