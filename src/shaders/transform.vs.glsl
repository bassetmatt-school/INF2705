#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;

out vec3 vColor;

void main(void)
{
    vColor = color;
    gl_Position = vec4(aPos, 1.0);
}
