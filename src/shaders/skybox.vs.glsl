#version 450 core

uniform mat4 MVP;

layout (location = 0) in vec3 pos;

out vec3 texCoord;

void main(void) {
	 vec4 out_pos = MVP * vec4(pos, 1.0);
    gl_Position = out_pos.xyww;
	 texCoord = pos;
}
