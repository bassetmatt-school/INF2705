#version 450 core

uniform vec3 color;
uniform mat4 pvmMatrix;

uniform sampler2D tex;

layout (location = 0) in vec3 scenePosition;

out vec3 vColor;

void main(void)
{
    gl_Position = pvmMatrix * vec4(scenePosition, 1.0);
    vColor = color;
	//  vColor = texture(tex, vec2(0.5, 0.5)).rgb;

}
