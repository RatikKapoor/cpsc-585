#version 330 core
layout (location = 0) in vec3 pos;



uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 nM;
uniform vec3 cameraPos;
uniform vec3 lightPos;

uniform mat4 lightSpaceMatrix;


out vec3 fragPos;
out vec3 fragColor;
out vec3 n;
out mat4 nMatrix;
out vec3 lPos;
out vec3 camPos;

void main()
{
  gl_Position = lightSpaceMatrix * M * vec4(pos, 1.0);
}