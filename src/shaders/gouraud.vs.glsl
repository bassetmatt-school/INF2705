#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;

out ATTRIB_VS_OUT {
	vec2 texCoords;
	vec3 emission;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} attribOut;

uniform mat4 mvp;
uniform mat4 view;
uniform mat4 modelView;
uniform mat3 normalMatrix;

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

float computeSpot(in vec3 spotDir, in vec3 lightDir, in vec3 normal) {
	float spotFactor = 0.0;

	if (dot(spotDir, normal) >= 0.0)
	{
		float spotDot = dot(lightDir, spotDir);
		float opening = cos(radians(spotOpeningAngle));

		if (useDirect3D)
			spotFactor = smoothstep(pow(opening, 1.01 + spotExponent/2.0), opening, spotDot);
		else
			spotFactor = spotDot > opening ? pow(spotDot, spotExponent) : 0.0;
	}
	return spotFactor;
}

vec3 computeLight(in int lightIndex, in vec3 normal, in vec3 lightDir, in vec3 obsPos) {
	// Will only be normalized in computeSpot if needed
	vec3 spotDir = mat3(view) * -lights[lightIndex].spotDirection;

	// Ambiant component
	attribOut.ambient += mat.ambient * lights[lightIndex].ambient;

	// Diffuse component (spotlight or not)
	float LdotN = dot(lightDir, normal);
	if (LdotN > 0.0) {
		float	spot = useSpotlight ?
			computeSpot(spotDir, lightDir, normal) :
			1.0f;
		attribOut.diffuse += spot * mat.diffuse * lights[lightIndex].diffuse * LdotN;

		// Specular component
		float spec = 0.0;
		if (useBlinn) { // Blinn
			vec3 halfVec = normalize(lightDir + obsPos);
			spec = dot(halfVec, normal);
		} else { // Phong
			vec3 reflectDir = reflect(-lightDir, normal);
			spec = dot(reflectDir, obsPos);
		}

		// No need to take the max between spec and 0.0 since we ignore the negative case
		if (spec > 0) {
			// Apply shininess to lighthen formula below
			spec = pow(spec, mat.shininess);
			// Also multiply by spotlight factor to avoid reflexion outside of spotlight cone
			attribOut.specular += spot * mat.specular * lights[lightIndex].specular * spec;
		}
	}
	return vec3(0.0);
}

void main() {
	gl_Position = mvp * vec4(position, 1.0);

	attribOut.texCoords = texCoords;

	// Initializes colors components
	attribOut.emission = mat.emission;
	attribOut.ambient = mat.ambient * lightModelAmbient;
	attribOut.diffuse = vec3(0.0);
	attribOut.specular = vec3(0.0);

	vec3 pos = (modelView * vec4(position, 1.0)).xyz;
	vec3 lightDir[3];
	for (int i = 0; i < 3; ++i) {
		lightDir[i] = (view * vec4(lights[i].position, 1.0)).xyz - pos;
		computeLight(i,
			normalize(normalMatrix * normal),
			normalize(lightDir[i]),
			normalize(-pos)
		);
	}
}
