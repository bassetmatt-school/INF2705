#version 450 core

uniform mat4 MVP;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 inTexCoord;

out vec2 texCoord;

void main(void) {
    gl_Position = MVP * vec4(pos, 1.0);
	 texCoord = inTexCoord.st;
}
