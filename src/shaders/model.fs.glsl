#version 450 core

uniform sampler2D tex;

in vec2 vTexCoord;

out vec4 FragColor;

void main() {
	FragColor = texture(tex, vTexCoord);
	if (FragColor.a < 0.3) discard;
}
