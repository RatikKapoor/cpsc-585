#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 nM;
uniform vec3 cameraPos;

out vec3 fragPos;
out vec3 fragColor;
out vec3 n;
out mat4 nMatrix;
out vec3 lPos;
out vec3 camPos;

out vec2 tc;

void main() {
	fragPos = mat3(M)*pos;
	fragColor = color;
//	 n = normalize(normal);
	n = mat3(transpose(inverse(M))) * normal;  
	tc = texCoord;
	gl_Position = P * V * M * vec4(pos, 1.0);
	lPos = vec3(M * vec4(4.0, 4.0, 4.0, 1.0));
	camPos = vec3(M* vec4((cameraPos),1.0 ));

}