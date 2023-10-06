#version 450 core

uniform samplerCube tex;

in vec3 texCoord;

out vec4 FragColor;

void main() {
	FragColor = texture(tex, texCoord);
}
