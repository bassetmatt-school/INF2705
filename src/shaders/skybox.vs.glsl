#version 450 core

uniform mat4 MVP;

layout (location = 0) in vec3 scenePosition;
layout (location = 1) in vec2 texCoord;

out Attribs {
    vec2 texCoord;
} AttribsOut;

void main(void) {
    gl_Position = MVP * vec4(scenePosition, 1.0);
	 AttribsOut.texCoord = texCoord.st;
}
