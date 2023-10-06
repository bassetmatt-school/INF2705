#version 450 core

uniform sampler2D tex;
uniform float time;

in vec2 texCoord;

out vec4 FragColor;

void main() {
	// Linear speed + wave function
	float dx = 0.1 * time + 0.025 * cos(1 * 0.5 * time + 2 * (texCoord.s + texCoord.t));
	float dz =              0.01  * sin(2 * 0.5 * time + 2 * (texCoord.s + texCoord.t));
	FragColor = texture(tex, texCoord + vec2(dx, dz));
}
