#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 colour;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
out vec3 lCol;
uniform vec3 lightCol;

out vec3 fragColour;
out vec2 tc;

void main() {
	tc = texCoord;
	fragColour = colour;

	gl_Position = P * V * M * vec4(pos, 1.0);
	lCol = lightCol;

}