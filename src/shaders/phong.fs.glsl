#version 450 core

in ATTRIB_VS_OUT {
	// TODO: Ask how to use textures ???????
	vec2 texCoords;
	vec3 normal;
	vec3 lightDir[3];
	vec3 spotDir[3];
	vec3 obsPos;
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
	float spotExponent;
	float spotOpeningAngle;
};

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;

out vec4 FragColor;

float computeSpot(in vec3 spotDir, in vec3 lightDir, in vec3 normal) {
	float cos_delta = cos(radians(spotOpeningAngle));
	// light dir is already normalized
	float cos_gamma = dot(normalize(spotDir), lightDir);

	if (!useDirect3D) { // OpenGL
		if (cos_gamma > cos_delta) // Inside of spotlight cone
			return pow(cos_gamma, spotExponent);
		else // Outside of spotlight cone
			return 0.0;
	} else { // Direct3D
		float cos_inner = cos_delta;
		float cos_outer = pow(cos_delta, 1.01 + spotExponent / 2.0);

		if (cos_gamma > cos_inner) // Inside of spotlight cone
			return 1.0;
		else if (cos_gamma < cos_outer) // Outside of spotlight cone
			return 0.0;
		else // Inside fading zone
			return (cos_gamma - cos_outer) / (cos_inner - cos_outer);
	}
	return 0.0;
}

vec3 computeLight(in int lightIndex, in vec3 normal, in vec3 lightDir, in vec3 obsPos) {
	vec3 color = vec3(0.0);
	// Ambiant component
	color += mat.ambient * lights[lightIndex].ambient;

	// Diffuse component (spotlight or not)
	float LdotN = max(dot(lightDir, normal), 0.0);
	if (LdotN > 0.0) {
		float	spot = useSpotlight ?
			computeSpot(attribIn.spotDir[lightIndex], lightDir, normal) :
			1.0f;
		color += spot * mat.diffuse * lights[lightIndex].diffuse * LdotN;

		// Specular component
		float spec = 0.0;
		if (useBlinn) { // Blinn
			vec3 halfVec = normalize(lightDir + obsPos);
			spec = max(dot(halfVec, normal), 0.0);
		} else { // Phong
			vec3 reflectDir = reflect(-lightDir, normal);
			spec = max(dot(reflectDir, obsPos), 0.0);
		}

		// No need to take the max between spec and 0.0 since we ignore the negative case
		if (spec > 0) {
			spec = pow(spec, mat.shininess); // Apply shininess to lighthen formula below
			color += mat.specular * lights[lightIndex].specular * spec;
		}
	}

	return color;
}

void main() {
	vec3 color = mat.emission + mat.ambient * lightModelAmbient;
	vec3 normal = normalize(attribIn.normal);
	vec3 obsPos = normalize(attribIn.obsPos);
	for (int i = 0; i < 3; ++i) {
		vec3 lightDir = normalize(attribIn.lightDir[i]);
		color += computeLight(i, normal, lightDir, obsPos);
	}
	FragColor = clamp( vec4(color, 1.0), 0.0, 1.0 );

}
