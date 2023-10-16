#version 450 core

uniform sampler2D tex;
uniform float time;

in vec2 vTexCoord;
in vec3 vPos;

out vec4 FragColor;

void main() {
	// Linear speed + wave function
	float dx = 0.05 * vTexCoord.y * sin(time/ 2.0 + vPos.x + vPos.z);
	float dz = 0.05 * vTexCoord.y * sin(time/ 2.0 + vPos.x + vPos.z);
	FragColor = texture(tex, vTexCoord + vec2(dx, dz));
	if (FragColor.a < 0.7) discard;
}
