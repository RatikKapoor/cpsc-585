#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 nM;

out vec3 fragPos;
out vec3 fragColor;
out vec3 n;
out mat4 nMatrix;

out vec2 tc;

void main() {
	fragPos = vec3(M * vec4(pos, 1.0));
	fragColor = color;
	// n = normalize(normal);
	n = normalize(mat3(transpose(inverse(M))) * normal);  
	tc = texCoord;
	gl_Position = P * V * M * vec4(pos, 1.0);
}