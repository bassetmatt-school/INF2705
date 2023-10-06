#version 450 core

uniform mat4 MVP;

layout (location = 0) in vec3 scenePosition;

out vec3 texCoord;

void main(void) {
	 vec4 pos = MVP * vec4(scenePosition, 1.0);
    gl_Position = pos.xyww;
	 texCoord = scenePosition;
}
