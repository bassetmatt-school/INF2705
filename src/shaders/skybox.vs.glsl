#version 450 core

uniform mat4 MVP;

layout (location = 0) in vec3 inPos;

out vec3 vTexCoord;

void main(void) {
	 vec4 out_pos = MVP * vec4(inPos, 1.0);
    gl_Position = out_pos.xyww;
	 vTexCoord = inPos;
}
