#version 450 core

in ATTRIB_VS_OUT {
	vec2 texCoords;
	vec3 emission;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} attribIn;

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;

struct Material {
	vec3 emission;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct UniversalLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
	vec3 spotDirection;
};

layout (std140) uniform LightingBlock {
	Material mat;
	UniversalLight lights[3];
	vec3 lightModelAmbient;
	bool useBlinn;
	bool useSpotlight;
	bool useDirect3D;
	bool useTexture;
	float spotExponent;
	float spotOpeningAngle;
};

out vec4 FragColor;

void main() {
	vec4 diffuseTexel = vec4(1.0f);
	vec4 specularTexel = vec4(1.0f);
	if (useTexture) {
		diffuseTexel = texture(diffuseSampler, attribIn.texCoords);
		if (diffuseTexel.a < 0.3) discard;
		specularTexel = texture(specularSampler, attribIn.texCoords);
	}

	vec3 color = attribIn.emission + min(attribIn.ambient + attribIn.diffuse, vec3(1.0f)) * vec3(diffuseTexel) + attribIn.specular * vec3(specularTexel.x);
	color = clamp(color, vec3(0.0f), vec3(1.0f));
	FragColor = vec4(color, 1.0f);
}
