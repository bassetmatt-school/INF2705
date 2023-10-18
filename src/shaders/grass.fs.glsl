#version 450 core

uniform sampler2D tex;
uniform float time;

in vec2 texCoords;
in vec3 vPos;

out vec4 FragColor;

void main() {
	FragColor = texture(tex, texCoords);
	if (FragColor.a < 0.7) discard;
}
