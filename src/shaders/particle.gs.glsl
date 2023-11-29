#version 450 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;


in ATTRIB_VS_OUT {
	vec4 color;
	vec2 size;
} attribIn[];

out ATTRIB_GS_OUT {
	vec4 color;
	vec2 texCoords;
} attribOut;

uniform mat4 projection;

void main() {
	// TODO: Check if loop useful ?
	for (int i = 0 ; i < gl_in.length(); ++i) {
		gl_Position = projection * (gl_in[i].gl_Position + vec4(-attribIn[i].size.x / 2, -attribIn[i].size.y / 2, 0.0, 0.0));
		attribOut.color = attribIn[i].color;
		attribOut.texCoords = vec2(0.0, 0.0);
		EmitVertex();

		gl_Position = projection * (gl_in[i].gl_Position + vec4(attribIn[i].size.x / 2, -attribIn[i].size.y / 2, 0.0, 0.0));
		attribOut.color = attribIn[i].color;
		attribOut.texCoords = vec2(1.0, 0.0);
		EmitVertex();

		gl_Position = projection * (gl_in[i].gl_Position + vec4(-attribIn[i].size.x / 2, attribIn[i].size.y / 2, 0.0, 0.0));
		attribOut.color = attribIn[i].color;
		attribOut.texCoords = vec2(0.0, 1.0);
		EmitVertex();

		gl_Position = projection * (gl_in[i].gl_Position + vec4(attribIn[i].size.x / 2, attribIn[i].size.y / 2, 0.0, 0.0));
		attribOut.color = attribIn[i].color;
		attribOut.texCoords = vec2(1.0, 1.0);
		EmitVertex();

		EndPrimitive();
	}
}
