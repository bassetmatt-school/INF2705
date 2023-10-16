#version 450 core

uniform samplerCube tex;

in vec3 vTexCoord;

out vec4 FragColor;

void main() {
	FragColor = texture(tex, vTexCoord);
}
