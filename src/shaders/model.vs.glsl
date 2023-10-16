#version 450 core

uniform mat4 MVP;

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoord;

out vec2 vTexCoord;

void main(void) {
    gl_Position = MVP * vec4(inPos, 1.0);
	 vTexCoord = inTexCoord.st;
}
