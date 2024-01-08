#version 450 core

out vec4 FragColor;


uniform sampler2D textureSampler;

in ATTRIB_GS_OUT {
	vec4 color;
	vec2 texCoords;
} attribIn;

void main() {
	FragColor = attribIn.color;
	vec4 texel = texture(textureSampler, attribIn.texCoords);
	// Only mixing alpha looks better I think
	FragColor.a = mix(texel.a, attribIn.color.a, 0.5);
	// Discards fragments with alpha < 0.05
	if (FragColor.a <= 0.05) discard;
}
