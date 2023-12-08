#version 450 core

layout(vertices = 4) out;

uniform mat4 modelView;

#define U_BUFFER
#ifdef U_BUFFER
	layout (std140, binding = 1) uniform TessellationData {
		float MIN_TESS;
		float MAX_TESS;

		float MIN_DIST;
		float MAX_DIST;
	};
#else
	const float MIN_TESS = 1;
	const float MAX_TESS = 256;

	const float MIN_DIST = 10.0f;
	const float MAX_DIST = 100.0f;
#endif

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
			float f = (dist - MIN_DIST) / (MAX_DIST - MIN_DIST);
			f = clamp(f, 0.0f, 1.0f);
			// Using 1-f so that: MIN_DIST => MAX_TESS
			gl_TessLevelOuter[i] = mix(MIN_TESS, MAX_TESS, 1-f);
		}

		gl_TessLevelInner[0] = max(gl_TessLevelOuter[1], gl_TessLevelOuter[3]);
		gl_TessLevelInner[1] = max(gl_TessLevelOuter[0], gl_TessLevelOuter[2]);
	}
}
