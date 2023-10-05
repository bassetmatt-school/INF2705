#version 450 core

uniform mat4 MVP;

layout (location = 0) in vec3 scenePosition;

out Attribs {
    vec3 texCoord;
} AttribsOut;

void main(void) {
    gl_Position = MVP * vec4(scenePosition, 1.0);
	 AttribsOut.texCoord = scenePosition;
}
