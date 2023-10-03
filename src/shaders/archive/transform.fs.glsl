#version 450 core

in vec3 vColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(vColor.rgb, 1.0);
}
