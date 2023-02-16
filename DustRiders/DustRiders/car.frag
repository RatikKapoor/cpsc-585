#version 330 core

in vec3 fragPos;
in vec3 fragColor;
in vec3 n;

uniform vec3 lightPos;
uniform bool shouldUseLighting;
uniform vec3 cameraPos;
in vec2 tc;

in vec3 lPos;
in vec3 camPos;

uniform sampler2D baseSampler;
uniform sampler2D specularSampler;

out vec4 color;

void main() {
	vec3 lightCol = vec3(1.0, 1.0, 1.0);

	vec3 norm = normalize(n);
	vec4 d = texture(baseSampler, tc);
	vec4 spect = texture(specularSampler, tc);
	if(d.a < 0.01)
        discard; // If the texture is transparent, don't draw the fragment

	// Ambient
	vec3 ambientLighting = 0.1*lightCol;

	// Diffusion
	vec3 lightDir = normalize(camPos - fragPos);
	vec3 diff = max(dot(norm, lightDir), 0.0)*lightCol;

	// Specular
	float specularStrength = 0.9;
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specular = spec * specularStrength * spect.x*lightCol;

	// Normal mapping
	vec3 rgb_normal = n * 0.5 + 0.5;

	vec4 res = vec4((ambientLighting + diff + specular),1.0) * d; 
	color = res;
	
}