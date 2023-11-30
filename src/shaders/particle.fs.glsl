#version 450 core

out vec4 FragColor;


uniform sampler2D textureSampler;

in ATTRIB_GS_OUT {
	vec4 color;
	vec2 texCoords;
} attribIn;

void main() {
	// Discards fragments with alpha < 0.05
	if (attribIn.color.a < 0.05) discard;

	// vec4 texel = texture(textureSampler, attribIn.texCoords);
	// TODO Change
	// FragColor = mix(texel, attribIn.color, 0.5);
	FragColor = attribIn.color;
}
