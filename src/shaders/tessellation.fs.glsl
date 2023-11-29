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
	vec4 sandTex, groundTex, snowTex;
	if (attribIn.height < 0.3f) {
		FragColor = texture(sandSampler, attribIn.texCoords);
	} else if (attribIn.height < 0.35f) {
		sandTex = texture(sandSampler, attribIn.texCoords);
		groundTex = texture(groundSampler, attribIn.texCoords);
		FragColor = mix(sandTex, groundTex, (attribIn.height - 0.3) / 0.05f);
	} else if (attribIn.height < 0.6f) {
		FragColor = texture(groundSampler, attribIn.texCoords);
	} else if (attribIn.height < 0.65f) {
		groundTex = texture(groundSampler, attribIn.texCoords);
		snowTex = texture(snowSampler, attribIn.texCoords);
		FragColor = mix(groundTex, snowTex, (attribIn.height - 0.6) / 0.05f);
	} else {
		FragColor = texture(snowSampler, attribIn.texCoords);
	}

	if (viewWireframe) {
		float edge = edgeFactor(attribIn.barycentricCoords, WIREFRAME_WIDTH);
		float patchEdge = edgeFactor(attribIn.patchDistance, PATCH_EDGE_WIDTH);
		FragColor.rgb = mix(FragColor.rgb, WIREFRAME_COLOR, 1 - edge);
		FragColor.rgb = mix(FragColor.rgb, PATCH_EDGE_COLOR, 1 - patchEdge);
	}
}
