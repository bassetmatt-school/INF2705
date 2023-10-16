#version 450 core

uniform mat4 MVP;

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoord;

out vec2 vTexCoord;
out vec3 vPos;

void main(void) {
    gl_Position = MVP * vec4(inPos, 1.0);
	 vPos = inPos;
	 vTexCoord = inTexCoord.st;
}
