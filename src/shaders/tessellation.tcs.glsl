#version 450 core

layout(vertices = 4) out;

uniform mat4 modelView;


const float MIN_TESS = 4;
const float MAX_TESS = 64;

const float MIN_DIST = 30.0f;
const float MAX_DIST = 100.0f;

float getTessLevel(float dist) {
	float f = (dist - MIN_DIST) / (MAX_DIST - MIN_DIST);
	// The closer, the higher the tesselation
	return mix(MAX_DIST, MIN_TESS, f);
}

void main() {
	// Setting output position
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	// Only calls the main function once
	if (gl_InvocationID == 0) {
		vec4 centers[4];
		centers[0] = gl_in[0].gl_Position; // (0,0)
		centers[1] = gl_in[1].gl_Position; // (1,0)
		centers[2] = gl_in[2].gl_Position; // (1,1)
		centers[3] = gl_in[3].gl_Position; // (0,1)

		for (int i = 0; i < 4; ++i) {
			centers[i] = (centers[i] + gl_in[(i+3) % 4].gl_Position) / 2.0f;
			float dist = length((modelView * centers[i]).xyz);
			gl_TessLevelOuter[i] = getTessLevel(dist);
		}

		gl_TessLevelInner[0] = max(gl_TessLevelOuter[1], gl_TessLevelOuter[3]);
		gl_TessLevelInner[1] = max(gl_TessLevelOuter[0], gl_TessLevelOuter[2]);
	}
}
