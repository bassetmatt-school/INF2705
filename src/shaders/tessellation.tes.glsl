#version 450 core

layout(quads) in;

/*
in Attribs {
	vec4 couleur;
} AttribsIn[];*/


out ATTRIB_TES_OUT {
	float height;
	vec2 texCoords;
	vec4 patchDistance;
} attribOut;

uniform mat4 mvp;

uniform sampler2D heighmapSampler;

vec4 interpole(vec4 v0, vec4 v1, vec4 v2, vec4 v3) {
	// mix( x, y, f ) = x * (1-f) + y * f.
	vec4 v01 = mix(v0, v1, gl_TessCoord.x);
	vec4 v32 = mix(v3, v2, gl_TessCoord.x);
	return mix(v01, v32, gl_TessCoord.y);
}


const float PLANE_SIZE = 256.0f;

void main() {
	// Position of the vertex
	vec4 position = interpole(
		gl_in[0].gl_Position,
		gl_in[1].gl_Position,
		gl_in[2].gl_Position,
		gl_in[3].gl_Position
	);
	// Height map coords
	vec2 heightTexCoords = position.xz / PLANE_SIZE + 0.5;
	float height = texture(heighmapSampler, heightTexCoords / 4.0).r;
	attribOut.height = height;
	gl_Position = mvp * (position + vec4(0, height * 64 - 32, 0, 0));
	// Texture coordinates, multiplied by 2 to have a better texture resolution
	attribOut.texCoords = gl_TessCoord.xy * 2.0;
	attribOut.patchDistance = vec4(
		gl_TessCoord.x,
		gl_TessCoord.y,
		1 - gl_TessCoord.x,
		1 - gl_TessCoord.y
	);
}
