#version 330 core

in vec3 fragPos;
in vec3 fragColor;
in vec3 n;

in vec2 TexCoords;

uniform vec3 cameraPos;
in vec2 tc;

in vec3 lPos;
in vec3 camPos;

uniform sampler2D baseTex;
uniform sampler2D depthMap;

out vec4 FragColor;

void main()
{
  float depthValue = texture(depthMap, TexCoords).r;
  FragColor = vec4(vec3(depthValue), 1.0);
}