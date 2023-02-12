#version 330 core

in vec3 fragColor;
in vec2 tc;

uniform sampler2D textureSamp;

out vec4 color;

void main() {

	color = texture(textureSamp,tc);
} 
