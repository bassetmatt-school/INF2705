#version 450 core

uniform sampler2D tex;

in Attribs {
    vec2 texCoord;
} AttribsIn;

out vec4 FragColor;

void main() {
	FragColor = texture(tex, AttribsIn.texCoord);
}
