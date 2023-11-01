#version 450 core

in ATTRIB_VS_OUT {
	vec2 texCoords;
	vec3 emission;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} attribIn;

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

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;

out vec4 FragColor;

void main() {
	vec3 texDiffuse = vec3(1.0f);
	vec3 texSpecular = vec3(1.0f);
	if (useTexture)
		texDiffuse = texture(diffuseSampler, attribIn.texCoords).rgb;
		// TODO: Same as phong, which component?
		texSpecular = texture(specularSampler, attribIn.texCoords).rgb;
	vec3 color = attribIn.emission;
	color += (attribIn.ambient + attribIn.diffuse) * texDiffuse;
	color += attribIn.specular * texSpecular;
	FragColor = vec4(clamp(color, 0.0f, 1.0f), 1.0f);
}
