#version 330 core

in vec3 fragPos;
in vec3 fragColor;
in vec3 n;

uniform vec3 cameraPos;
uniform float hitVisible;
in vec2 tc;
in float hitVis;

in vec3 lPos;
in vec3 camPos;
in vec3 lCol;

uniform sampler2D baseTex;
uniform sampler2D specularTex;
uniform sampler2D emissionTex;

out vec4 color;

void main() {
	vec3 lCol = vec3(1.0, 1.0, 1.0);

	vec3 norm = normalize(n);
	vec4 d = texture(baseTex, tc);
	vec4 spect = texture(specularTex, tc);
	vec4 emis = texture(emissionTex, tc);
	if(d.a < 0.01)
        discard; // If the texture is transparent, don't draw the fragment

	// Ambient
	vec3 ambientLighting = 0.05*lCol;

	// Diffusion
	vec3 lightDir = normalize(lPos - fragPos);
	vec3 diff = max(dot(norm, lightDir), 0.0)*lCol;


	vec3 specular = vec3(0.f, 0.f, 0.f);

	// Emission

	// Normal mapping
	vec3 rgb_normal = n * 0.5 + 0.5;
	vec3 visibleHit = vec3(0.f, 0.f, 0.f);
	if(hitVis > 0.f){
			visibleHit = vec3(1.f, 0.f, 0.f);
	}


	vec4 res = vec4((vec3(emis)),d.a) * d;
	color = res;

}