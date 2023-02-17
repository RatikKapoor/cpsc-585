#version 330 core

in vec3 fragPos;
in vec3 fragColor;
in vec3 n;

uniform vec3 lightPos;
uniform bool shouldUseLighting;
uniform vec3 cameraPos;
in vec2 tc;


uniform sampler2D sampler;

out vec4 color;

void main() {
	vec4 d = texture(sampler, tc);
	if(d.a < 0.01)
        discard; // If the texture is transparent, don't draw the fragment

	if (shouldUseLighting) {
		// Ambient
		float ambientLighting = 0.01;

		// Diffusion
		vec3 lightDir = normalize(lightPos - fragPos);
		float diff = max(dot(n, lightDir), 0.0);

		// Specular
		float specularStrength = 0.9;
		vec3 viewDir = normalize(cameraPos - fragPos);
		vec3 reflectDir = reflect(-lightDir, n);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
		float specular = spec * specularStrength;

		// Normal mapping
		vec3 rgb_normal = n * 0.5 + 0.5;

		color = (ambientLighting + diff + specular) * d;
	} else {
		color = d;
	}
}