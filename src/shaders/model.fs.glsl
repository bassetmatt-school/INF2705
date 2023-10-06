#version 450 core

uniform sampler2D tex;

in vec2 texCoord;

out vec4 FragColor;

void main() {
	FragColor = texture(tex, texCoord);
	if (FragColor.a < 0.3) discard;
}
