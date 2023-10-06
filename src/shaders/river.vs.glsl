#version 450 core

uniform mat4 MVP;


layout (location = 0) in vec3 scenePosition;
layout (location = 1) in vec2 inTexCoord;

out vec2 texCoord;

void main(void) {
	 texCoord = inTexCoord.st ;
    gl_Position = MVP * vec4(scenePosition, 1.0);
}
