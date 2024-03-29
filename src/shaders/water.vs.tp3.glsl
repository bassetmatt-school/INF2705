#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 inTexCoords;

out vec2 texCoords;

uniform mat4 mvp;
uniform float time;

void main() {
	gl_Position = mvp * vec4(position.x, position.y, position.z, 1.0);

	vec2 tiledCoords = inTexCoords * vec2(2, 5);
	vec2 flow = vec2(0, time/10.0);

	texCoords = tiledCoords + flow;
}
