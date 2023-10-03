#version 450 core

uniform mat4 pvmMatrix;
uniform vec3 color;

layout (location = 0) in vec3 scenePosition;

out vec3 vColor;

void main(void)
{
    gl_Position = pvmMatrix * vec4(scenePosition, 1.0);
    vColor = color;

}
