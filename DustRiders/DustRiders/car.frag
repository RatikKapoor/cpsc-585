#version 330 core

in vec3 fragPos;
in vec3 fragColor;
in vec3 n;

uniform vec3 cameraPos;
in vec2 tc;

in vec3 lPos;
in vec3 camPos;

uniform sampler2D baseTex;
uniform sampler2D specularTex;
uniform sampler2D emissionTex;

out vec4 color;

void main() {
	vec3 lightCol = vec3(1.0, 1.0, 1.0);

	vec3 norm = normalize(n);
	vec4 d = texture(baseTex, tc);
	vec4 spect = texture(specularTex, tc);
	vec4 emis = texture(emissionTex, tc);
	if(d.a < 0.01)
        discard; // If the texture is transparent, don't draw the fragment

	// Ambient
	vec3 ambientLighting = 0.05*lightCol;
	
	// Diffusion
	vec3 lightDir = normalize(lPos - fragPos);
	vec3 diff = max(dot(norm, lightDir), 0.0)*lightCol;

	// Specular
	float specularStrength = 0.9;
	vec3 viewDir = normalize(camPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = spec * specularStrength * spect.x*lightCol;

	// Emission

	// Normal mapping
	vec3 rgb_normal = n * 0.5 + 0.5;

	vec4 res = vec4((ambientLighting + specular + diff + vec3(emis)),d.a) * d; 
	color = res;
	
}