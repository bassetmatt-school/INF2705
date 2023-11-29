#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;


in ATTRIB_TES_OUT {
	float height;
	vec2 texCoords;
	vec4 patchDistance;
} attribIn[];

out ATTRIB_GS_OUT {
	float height;
	vec3 barycentricCoords;
	vec2 texCoords;
	vec4 patchDistance;
} attribOut;

void main() {

	const vec3 bary[3] = {
		vec3(1.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 0.0f, 1.0f)
	};

	for (int i = 0; i < gl_in.length(); ++i) {
		gl_Position = gl_in[i].gl_Position;
		attribOut.height = attribIn[i].height;
		attribOut.texCoords = attribIn[i].texCoords;
		attribOut.patchDistance = attribIn[i].patchDistance;
		attribOut.barycentricCoords = bary[i];
		EmitVertex();
	}
	EndPrimitive();
}
