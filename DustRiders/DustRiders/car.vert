#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 color;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 nM;
uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform float hitVisible;


out vec3 fragPos;
out vec3 fragColor;
out vec3 n;
out mat4 nMatrix;
out vec3 lPos;
out vec3 camPos;
out float hitVis;

out vec2 tc;

void main() {
	fragPos =  mat3(M)*pos;
	fragColor = color;
	n = mat3(transpose(inverse(M))) * normal;
	tc = texCoord;
	gl_Position = P * V * M * vec4(pos, 1.0);
	lPos =  vec3( vec4((lightPos),1.0 ));
	camPos = vec3(M*vec4((cameraPos),1.0));
	hitVis = hitVisible;
}