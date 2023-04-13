#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 2) in vec2 texCoords;

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

out vec2 TexCoords;

void main()
{
  TexCoords = texCoords;
  gl_Position = vec4(pos, 1.0);
}