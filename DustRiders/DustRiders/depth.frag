#version 330 core

in vec3 fragPos;
in vec3 fragColor;
in vec3 n;

uniform vec3 cameraPos;
in vec2 tc;

in vec3 lPos;
in vec3 camPos;

uniform sampler2D baseTex;

void main()
{
  gl_FragDepth = gl_FragCoord.z;
}