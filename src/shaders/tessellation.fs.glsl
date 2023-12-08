#version 450 core

in ATTRIB_GS_OUT {
	float height;
	vec3 barycentricCoords;
	vec2 texCoords;
	vec4 patchDistance;
} attribIn;

uniform sampler2D groundSampler;
uniform sampler2D sandSampler;
uniform sampler2D snowSampler;
uniform bool viewWireframe;

out vec4 FragColor;

float edgeFactor(vec3 barycentricCoords, float width) {
	vec3 d = fwidth(barycentricCoords);
	vec3 f = step(d * width, barycentricCoords);
	return min(min(f.x, f.y), f.z);
}

float edgeFactor(vec4 barycentricCoords, float width) {
	vec4 d = fwidth(barycentricCoords);
	vec4 f = step(d * width, barycentricCoords);
	return min(min(min(f.x, f.y), f.z), f.w);
}

const vec3 WIREFRAME_COLOR = vec3(0.5f);
const vec3 PATCH_EDGE_COLOR = vec3(1.0f, 0.0f, 0.0f);

const float WIREFRAME_WIDTH = 0.5f;
const float PATCH_EDGE_WIDTH = 0.5f;

void main() {
	vec4 sandTex = texture(sandSampler, attribIn.texCoords);
	vec4 groundTex = texture(groundSampler, attribIn.texCoords);
	vec4 snowTex = texture(snowSampler, attribIn.texCoords);

	if (attribIn.height < 0.6f) { // Sand under 0.3, ground above 0.35
		float f = smoothstep(0.3f, 0.35f, attribIn.height);
		FragColor = mix(sandTex, groundTex, f);
	} else { // Snow above 0.65
		float f = smoothstep(0.6f, 0.65f, attribIn.height);
		FragColor = mix(groundTex, snowTex, f);
	}

	if (viewWireframe) {
		float edge = edgeFactor(attribIn.barycentricCoords, WIREFRAME_WIDTH);
		float patchEdge = edgeFactor(attribIn.patchDistance, PATCH_EDGE_WIDTH);
		FragColor.rgb = mix(FragColor.rgb, WIREFRAME_COLOR, 1 - edge);
		FragColor.rgb = mix(FragColor.rgb, PATCH_EDGE_COLOR, 1 - patchEdge);
	}
}
