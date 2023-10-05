#version 450 core

uniform samplerCube tex;

in Attribs {
    vec3 texCoord;
} AttribsIn;

out vec4 FragColor;

void main() {
	FragColor = texture(tex, AttribsIn.texCoord);
}
